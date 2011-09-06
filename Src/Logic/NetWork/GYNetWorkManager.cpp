/////////////////////////////////////////////
// create time: 2011/9/4 16:31
// author:	colin chen
// file name:	GYNetWorkManager
// file type:	cpp
////////////////////////////////////////////
#include "GYNetWorkManager.h"

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
	m_usingGatewaySessionList.CleanUp();
	m_listenAddress.CleanUp();
	m_listenSocket.CleanUp();
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
		if(0 != m_listenSocket.Open(listenAddress))
		{
			break;
		}

		if (0 != m_reactor.Init(MAX_REACTOR_SIZE))
		{
			m_listenSocket.Close();
			break;
		}

		m_listenEvent.m_accept = GYTRUE;
		m_listenEvent.m_data = this;
		m_listenEvent.m_eventHandler = AcceptEventHandler;
		m_listenEvent.m_fd = &m_listenSocket;
		m_listenEvent.m_eventType = GY_NET_EVENT_TYPE_READ;
		if (0 != m_reactor.AddEvent(m_listenEvent))
		{
			m_listenSocket.Close();
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
	while(GYNULL != (pUsingGatewaySession = m_usingGatewaySessionList.PickUpFirstItem()))
	{
		pUsingGatewaySession->Release();
	}
	GYDelete[] m_gatewaySession;
	m_reactor.DeleteEvent(m_listenEvent);
	m_reactor.Release();
	m_listenSocket.Close();
	CleanUp();
}

GYVOID GYNetWorkManager::_OnAcceptGateway()
{

}


