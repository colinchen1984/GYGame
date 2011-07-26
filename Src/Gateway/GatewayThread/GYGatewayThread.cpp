/////////////////////////////////////////////
// create time: 2011/7/1 21:57
// author:	colin chen
// file name:	GYGatewayThread
// file type:	cpp
////////////////////////////////////////////
#include "GYGatewayThread.h"
#include "GYGuard.h"
#include <stdio.h>
#include <wchar.h>
#include "GYTimeStamp.h"
#include "GYServer.h"

GYGatewayThread::GYGatewayThread()
{
	_SetThreadStatus(EM_GATE_WAY_THREAD_STATUS_INVALID);
}

GYGatewayThread::~GYGatewayThread()
{
}

typedef GYVOID (GYGatewayThread::*ThreadRunFunction)();
static ThreadRunFunction handler[EM_GATE_WAY_THREAD_STATUS_COUNT] = {GYNULL};

GYINT32 GYGatewayThread::Init(const GYNetAddress& targetServerAddress, GYServer* server)
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if (GYNULL == server)
		{
			break;
		}
		
		if (0 != m_connection2Logic.Open())
		{
			break;
		}
		
		if (0 != m_reactor.Init(CLIENT_FOR_PER_THREAD))
		{
			m_connection2Logic.Close();
			break;
		}
		m_targetServerAddress = targetServerAddress;
		m_server = server;
		handler[EM_GATE_WAY_THREAD_STATUS_CONNECTING_LOGIC_SERVER] = &GYGatewayThread::_ConnectLogicServer;
		handler[EM_GATE_WAY_THREAD_STATUS_SERVERING_CLIENT_SESSION] = &GYGatewayThread::_ServeringClientSession;
		_SetThreadStatus(EM_GATE_WAY_THREAD_STATUS_CONNECTING_LOGIC_SERVER);
		result = 0;
	} while (GYFALSE);
	return result;
}

static GYVOID LogicServerConnectionEventHandler(GYNetEvent& event)
{
	GYGatewayThread* pThrad = static_cast<GYGatewayThread*>(event.m_data);
	pThrad->ProcessLogicServerData();
}

#define _GetThreadStatus() m_status
GYVOID GYGatewayThread::Run()
{
	while(GYTRUE)
	{
		if (EM_GATE_WAY_THREAD_STATUS_EXIT != _GetThreadStatus())
		{
			(this->*handler[_GetThreadStatus()])();
		} 
		else
		{
			_StopCurrentService();
			m_reactor.Release();
			break;
		}
		
	};
}

GYVOID GYGatewayThread::AddSession( GYClientSession& session )
{
	GYGuard<GYFastMutex> g(m_addSessionMutex);
	m_addSession.Add(session);
}

GYVOID GYGatewayThread::OnClientSessionClose( GYClientSession& session )
{
	m_ClosedSession.Add(session);
}

GYVOID GYGatewayThread::_ConnectLogicServer()
{
	if(0 != m_connection2Logic.Connect(m_targetServerAddress))
	{
		wprintf(L"Connect logic server fail\n");
		GYSleep(1 * 1000);
		return;
	}

	if (0 != m_connection2Logic.SetBlock(GYFALSE))
	{
		m_connection2Logic.Close();
		return;
	}
	m_event4Logic.m_accept = GYFALSE;
	m_event4Logic.m_fd = &m_connection2Logic;
	m_event4Logic.m_eventType = GY_NET_EVENT_TYPE_READ;
	m_event4Logic.m_busy = GYFALSE;
	m_event4Logic.m_data = this;
	m_event4Logic.m_eventHandler = LogicServerConnectionEventHandler;
	if (0 != m_reactor.AddEvent(m_event4Logic))
	{
		m_connection2Logic.Close();
		return;
	}
	_SetThreadStatus(EM_GATE_WAY_THREAD_STATUS_SERVERING_CLIENT_SESSION);

}

GYVOID GYGatewayThread::_ServeringClientSession()
{
	GYClientSession* pSession = GYNULL;
	pSession = GYNULL;
	while(GYNULL != (pSession = m_ClosedSession.PickUpFirstItem()))
	{
		m_server->OnClientSessionClose(*pSession);
	}
	pSession = GYNULL;
	{
		GYGuard<GYFastMutex> g(m_addSessionMutex);
		while(GYNULL != (pSession = m_addSession.PickUpFirstItem()))
		{
			if(0 != pSession->Regeist2Reactor(m_reactor, this, EM_CLIENT_SESSION_STATUS_WITH_SERVER))
			{
				m_server->OnClientSessionClose(*pSession);
				continue;
			}
			m_workSession.Add(*pSession);
		}
	}
	static GYTimeStamp termTime;
	termTime.m_termTime = 1;
	m_reactor.RunOnce(termTime);
}

GYVOID GYGatewayThread::ProcessLogicServerData()
{
	GY_SOCKET_OPERATION_ERROR_CODE result = m_connection2Logic.Recv();
	if (GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED == result 
		|| GY_SOCKET_OPERATION_ERROR_CODE_FAIL_TO_CHECK_SOCKET_CORE_BUFFER == result
		|| GY_SOCKET_OPERATION_ERROR_CODE_FAIL_TO_CHECK_SOCKET_CORE_BUFFER == result)
	{
		//当与logic server断开连接后，重新打开套接字，并尝试连接logic server
		//同时释放所有正在服务的client sesson， 与其断开连接，并将所有的client session还给GYServer
		_StopCurrentService();
		_SetThreadStatus(EM_GATE_WAY_THREAD_STATUS_CONNECTING_LOGIC_SERVER);
		return;
	}
	if (GY_SOCKET_OPERATION_ERROR_CODE_SOCKET_CORE_BUFFER_EMPTY == result)
	{
		return;
	}

	GYINT32 length = m_connection2Logic.m_inputBuffer.GetReadSize();
	if (length > 0)
	{
		const GYCHAR* p = m_connection2Logic.m_inputBuffer.ReadPtr();
		//TODO: 分析数据包，找到对应的Client Session，并将数据发送个该Client 
	}
}

GYVOID GYGatewayThread::_StopCurrentService()
{
	m_connection2Logic.Close();
	m_reactor.DeleteEvent(m_event4Logic);
	m_connection2Logic.Open();
	GYClientSession* pSession = GYNULL;
	//通知Client Session关闭
	while (GYNULL != (pSession = m_workSession.PickUpFirstItem()))
	{
		pSession->_OnClientCloseWithServer();
	}
	//将关闭了的Client Session还给GYServer
	while (GYNULL != (pSession = m_ClosedSession.PickUpFirstItem()))
	{
		m_server->OnClientSessionClose(*pSession);
	}
	//清楚Hash表
	m_workSessionHash.CleanUp();
}