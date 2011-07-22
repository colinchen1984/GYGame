/////////////////////////////////////////////
// create time: 2011/7/1 21:57
// author:	colin chen
// file name:	GYGatewayThread
// file type:	cpp
////////////////////////////////////////////
#include "GYGatewayThread.h"
#include "GYGuard.h"
#include <stdio.h>
#include "GYTimeStamp.h"
#include "GYServer.h"

GYGatewayThread::GYGatewayThread()
{
}

GYGatewayThread::~GYGatewayThread()
{
}

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
		result = 0;
	} while (GYFALSE);
	return result;
}

static GYVOID LogicServerConnectionEventHandler(GYNetEvent& event)
{

}

GYVOID GYGatewayThread::Run()
{
	while(0 != m_connection2Logic.Connect(m_targetServerAddress))
	{
		wprintf(L"Connect logic server fail\n");
		GYSleep(1 * 1000);
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
		m_reactor.Release();
		return;
	}

	GYClientSession* pSession = GYNULL;
	while(GYTRUE)
	{
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
					//TODO: 将该session还给server
					continue;
				}
				m_workSession.Add(*pSession);
			}
		}
		static GYTimeStamp termTime;
		termTime.m_termTime = 30;
		m_reactor.RunOnce(termTime);
	}

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