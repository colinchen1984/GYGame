/////////////////////////////////////////////
// create time: 2011/7/1 17:27
// author:	colin chen
// file name:	GYServer
// file type:	cpp
////////////////////////////////////////////
#include "GYServer.h"
#include "GYThreadTask.h"
#include "GYNetAddress.h"
#include "GYGatewayThread.h"
#include "GYTimeStamp.h"
#include "GYGuard.h"
#include "GYTimeController.h"
#include <wchar.h>
const GYINT32 GatewayThreadCount = 5;
const GYINT32 GatewayClientSessionCount = GatewayThreadCount * (CLIENT_FOR_PER_THREAD - 1) ;
const GYINT32 GateListenReactorMaxCount = CLIENT_FOR_PER_THREAD;
GYServer::GYServer()
{
	m_gateThread = GYNULL;
	m_isServering = GYFALSE;
}

GYServer::~GYServer()
{
}

GYVOID AcceptEventHandler(GYNetEvent& event)
{
	GYServer* pServer = static_cast<GYServer*>(event.m_data);
	pServer->_OnAcceptClient();
}

GYVOID GatewayThreadFunction(GYVOID* param)
{
	GYGatewayThread* p = static_cast<GYGatewayThread*>(param);
	p->Run();
}

GYINT32 GYServer::Init()
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		GYNetAddress listenAddress;
		listenAddress.SetAddr("192.168.1.100");
		listenAddress.SetPort(5555);
		m_usingClientSession.CleanUp();
		m_freeClientSession.CleanUp();
		if (0 != m_acceptorSocket.Open(listenAddress))
		{
			break;
		}
		if (0 != m_acceptorSocket.SetBlock(GYFALSE))
		{
			break;
		}
		
		if (0 != m_reactor.Init(GateListenReactorMaxCount))
		{
			break;
		}

		m_listenEvent.CleanUp();
		m_listenEvent.m_data = static_cast<GYVOID*>(this);
		m_listenEvent.m_accept = GYTRUE;
		m_listenEvent.m_busy = GYFALSE;
		m_listenEvent.m_eventType = GY_NET_EVENT_TYPE_READ;
		m_listenEvent.m_fd = &m_acceptorSocket;
		m_listenEvent.m_eventHandler = AcceptEventHandler;
		if (0 != m_reactor.AddEvent(m_listenEvent))
		{
			break;
		}
		m_stringManager.Init();
		m_packetFactory.Init(m_stringManager);
		//设置gatewany工作线程
		m_gateThread = GYNew GYGatewayThread[GatewayThreadCount];
		m_gateThreadCount = GatewayThreadCount;
		if(0 != ThreadPool.Init(GatewayThreadCount))
		{
			break;
		}
		GYNetAddress logicServerAddress;
		logicServerAddress.SetAddr("192.168.1.100");
		logicServerAddress.SetPort(5556);
		GYBOOL threadInit = GYTRUE;
		GYThreadTask task;
		for (GYINT32 i = 0; i < GatewayThreadCount && GYTRUE == threadInit; ++i)
		{
			if (0 != m_gateThread[i].Init(logicServerAddress, this))
			{
				threadInit = GYFALSE;
				break;
			}
			task.m_threadFunction = GatewayThreadFunction;
			task.param = &m_gateThread[i];
			ThreadPool.AddTask(task);

		}
		if (GYTRUE != threadInit)
		{
			break;
		}
		m_wholeClientSession = GYNew GYClientSession[GatewayClientSessionCount];
		for (GYINT32 i = 0; i < GatewayClientSessionCount; ++i)
		{
			m_freeClientSession.Add(m_wholeClientSession[i]);
		}
		m_isServering = GYTRUE;
		result = 0;
	} while (GYFALSE);
	return result;	
}

GYINT32 GYServer::RunOnce()
{
	{
		static GYINT32 count = 0;
		GYGuard<GYFastMutex> g(m_sessionCloseMutex);
		GYINT32 freeSessionCount = m_freeClientSession.GetItemCount();
		if (count != freeSessionCount)
		{
			count = freeSessionCount;
			wprintf(L"Current free session count is %d \n", freeSessionCount);
		}
		
	}
	GYTimeStamp termTime;
	termTime.m_termTime = 30;
	GYINT32 result = m_reactor.RunOnce(termTime);
	GYAssert(0 == result);
	return result;
		
}

GYVOID GYServer::_OnAcceptClient()
{
	GYStreamSocket sock;
	GYNetAddress address;
	GYINT32 err = INVALID_VALUE;
	while(GYTRUE)
	{
		static GYINT32 debugCount = 0;
		if(0 != m_acceptorSocket.Accept(sock, address))
		{
			err = GetLastNetWorkError();
			if(GYSOCKEWOULDBLOCK == err || GYEINTR == err
#ifdef LINUX64
					|| GYEAGAIN == err
#endif
					)
			{
				break;
			}
			else if(GYEMFILE == err)
			{
				m_reactor.DeleteEvent(m_listenEvent);
				m_reactor.AddEvent(m_listenEvent);
				break;
			}
			else
			{
				//不可修复错误，关闭服务器
				wprintf(L"Can't handle this error %d, current client session count is %d\n", err, debugCount);
			}
		}
		
		static GYINT32 count = 0;
		GYClientSession* session = GYNULL;
		{	
			GYGuard<GYFastMutex> g(m_sessionCloseMutex);
			if (GYNULL == (session = m_freeClientSession.PickUpFirstItem()))
			{
				//TODO: 通知客户端不能稍后再试
				sock.Close();
				continue;
			}
			;
		}


		if(0 != session->Init(sock, address))
		{
			sock.Close();
			m_freeClientSession.Add(*session);
			continue;
		}
// 		if (0 != session.Regeist2Reactor(m_reactor, GYNULL))
// 		{
// 			m_freeClientSession.Add(session);
// 			continue;
// 		}
// 		
// 		m_usingClientSession.Add(session);
		m_gateThread[count++].AddWorkingSession(*session);
		if (count >= GatewayThreadCount)
		{
			count = 0;
		}
		debugCount++;

	}
}

GYVOID GYServer::OnClientSessionClose( GYClientSession& session )
{
	GYGuard<GYFastMutex> g(m_sessionCloseMutex);
	m_freeClientSession.Add(session);
}

GYINT32 GYServer::Release()
{
	m_acceptorSocket.Close();
	for (GYINT32 i = 0; i < m_gateThreadCount; ++i)
	{
		m_gateThread[i].StopGateThread();
	}
	ThreadPool.Release();
	GYDelete[] m_gateThread;
	GYDelete[] m_wholeClientSession;
	m_usingClientSession.CleanUp();
	m_freeClientSession.CleanUp();
	m_reactor.Release();
	m_packetFactory.Release();
	return 0;
}

