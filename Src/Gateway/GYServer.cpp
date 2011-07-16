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

const GYINT32 GatewayClientSessionCount = 10000;
const GYINT32 GatewayThreadCount = GatewayClientSessionCount / 60 + 1;
const GYINT32 GateListenReactorMaxCount = 32;
GYServer::GYServer()
{
	m_gateThread = GYNULL;
}

GYServer::~GYServer()
{
}

static GYVOID AcceptEventHandler(GYNetEvent& event)
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
		listenAddress.SetAddr("127.0.0.1");
		listenAddress.SetPort(5555);
		m_usingClientSession.CleanUp();
		m_freeClientSession.CleanUp();
		if (0 != m_acceptorSocket.Open(listenAddress))
		{
			break;
		}
		if (m_acceptorSocket.SetBlock(GYFALSE))
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

		//设置gatewany工作线程
		m_gateThread = GYNew GYGatewayThread[GatewayThreadCount];
		
		if(0 != ThreadPool.Init(GatewayThreadCount))
		{
			break;
		}
		GYNetAddress logicServerAddress;
		logicServerAddress.SetAddr("127.0.0.1");
		logicServerAddress.SetPort(5556);
		GYBOOL threadInit = GYTRUE;
		GYThreadTask task;
		for (GYINT32 i = 0; i < GatewayThreadCount && GYTRUE == threadInit; ++i)
		{
			if (0 != m_gateThread[i].Init(logicServerAddress))
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
		m_wholeClientSession = new GYClientSession[GatewayClientSessionCount];
		for (GYINT32 i = 0; i < GatewayClientSessionCount; ++i)
		{
			m_freeClientSession.Add(m_wholeClientSession[i]);
		}

		result = 0;
	} while (GYFALSE);
	return result;	
}

GYINT32 GYServer::Run()
{
	while(GYTRUE)
	{
		GYTimeStamp termTime;
		termTime.m_termTime = 30;
		m_reactor.RunOnce(termTime);
	}
}

GYVOID GYServer::_OnAcceptClient()
{
	GYStreamSocket sock;
	GYNetAddress address;
	while(0 == m_acceptorSocket.Accept(sock, address))
	{
		if (m_freeClientSession.GetItemCount() <= 0)
		{
			//TODO: 通知客户端不能稍后再试
			sock.Close();
			continue;
		}
		static GYINT32 count  = 0;
		GYClientSession& session = *m_freeClientSession.PickUpFirstItem();
		if(0 != session.Init(sock, address, *this))
		{
			m_freeClientSession.Add(session);
			continue;
		}
// 		if (0 != session.Regeist2Reactor(m_reactor, GYNULL))
// 		{
// 			m_freeClientSession.Add(session);
// 			continue;
// 		}
// 		
// 		m_usingClientSession.Add(session);

		m_gateThread[count++].AddSession(session);
		if (count >= GatewayThreadCount)
		{
			count = 0;
		}
	}
}

