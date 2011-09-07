/////////////////////////////////////////////
// create time: 2011/9/4 16:31
// author:	colin chen
// file name:	GYNetWorkManager
// file type:	cpp
////////////////////////////////////////////
#include "GYNetWorkManager.h"
#include <stdio.h>

GYNetWorkManager::GYNetWorkManager()
{
	CleanUp();
}

GYNetWorkManager::~GYNetWorkManager()
{

}

GYVOID GYNetWorkManager::CleanUp()
{
	m_gatewaySession = GYNULL;
	m_freeGatewaySessionList.CleanUp();
	m_workSession.CleanUp();
	m_listenAddress.CleanUp();
	m_acceptorSocket.CleanUp();
	m_listenEvent.CleanUp();
}

const GYINT32 MAX_GATEWAY_SESSION_COUNT = 8;
const GYINT32 MAX_REACTOR_SIZE = MAX_GATEWAY_SESSION_COUNT + 1;
GYVOID AcceptEventHandler(GYNetEvent& event)
{
	GYNetWorkManager* pNetManager = static_cast<GYNetWorkManager*>(event.m_data);
	pNetManager->_OnAcceptGateway();
}

GYINT32 GYNetWorkManager::Init( const GYNetAddress& listenAddress )
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if(0 != m_acceptorSocket.Open(listenAddress))
		{
			break;
		}
		if (0 != m_acceptorSocket.SetBlock(GYFALSE))
		{
		}
		
		if (0 != m_reactor.Init(MAX_REACTOR_SIZE))
		{
			m_acceptorSocket.Close();
			break;
		}

		m_listenEvent.m_accept = GYTRUE;
		m_listenEvent.m_data = this;
		m_listenEvent.m_eventHandler = AcceptEventHandler;
		m_listenEvent.m_fd = &m_acceptorSocket;
		m_listenEvent.m_eventType = GY_NET_EVENT_TYPE_READ;
		if (0 != m_reactor.AddEvent(m_listenEvent))
		{
			m_acceptorSocket.Close();
			m_reactor.Release();
			break;
		}
		m_gatewaySession = GYNew GYGatewaySession[MAX_GATEWAY_SESSION_COUNT];
		for (GYINT32 i = 0; i < MAX_GATEWAY_SESSION_COUNT; ++i)
		{
			m_freeGatewaySessionList.Add(m_gatewaySession[i]);
		}
		result = 0;
	} while (GYFALSE);
	return result;
}

GYVOID GYNetWorkManager::Release()
{
	GYGatewaySession* pUsingGatewaySession = GYNULL;
	while(GYNULL != (pUsingGatewaySession = m_workSession.PickUpFirstItem()))
	{
		pUsingGatewaySession->Release();
	}
	GYDelete[] m_gatewaySession;
	m_reactor.DeleteEvent(m_listenEvent);
	m_reactor.Release();
	m_acceptorSocket.Close();
	CleanUp();
}

GYVOID GYNetWorkManager::_OnAcceptGateway()
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
		GYGatewaySession* session = GYNULL;
		if (GYNULL == (session = m_freeGatewaySessionList.PickUpFirstItem()))
		{
			//TODO: 通知客户端不能稍后再试
			sock.Close();
			continue;
		}


		if(0 != session->Init(*this, m_reactor, sock, address))
		{
			sock.Close();
			m_freeGatewaySessionList.Add(*session);
			continue;
		}
		m_workSession.Add(*session);
	}
}

GYVOID GYNetWorkManager::RunOnce(const GYTimeStamp& timeStamp)
{
	m_reactor.RunOnce(timeStamp);

	GYGatewaySession* pSession;
	GYList<GYGatewaySession> tempSessionList;
	while(GYNULL != (pSession = m_workSession.PickUpFirstItem()))
	{
		if (GYTRUE == pSession->Tick())
		{
			tempSessionList.Add(*pSession);
		}
	}
	m_workSession = tempSessionList;
}

GYVOID GYNetWorkManager::OnGatewaySessionClose( GYGatewaySession& session )
{
	m_freeGatewaySessionList.Add(session);
}




