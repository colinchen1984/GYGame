/////////////////////////////////////////////
// create time: 2011/7/1 13:47
// author:	colin chen
// file name:	GYListenThread
// file type:	cpp
////////////////////////////////////////////
#include "GYListenThread.h"
#include "GYNetAddress.h"

GYListenThread::GYListenThread()
{
}

GYListenThread::~GYListenThread()
{
}

GYVOID GYListenThread::EpllEventHandler(GYNetEvent& event)
{
	GYListenThread* pThread = static_cast<GYListenThread*>(event.m_data);
	pThread->_HandlerAcceptSocket();
}

GYVOID GYListenThread::_HandlerAcceptSocket()
{
	GYStreamSocket sock;
	GYNetAddress address;
	while (GYTRUE)
	{
		if(0 != m_acceptorSocket.Accept(sock, address))
		{
			break;
		}

		m_eventHandler.m_handler(sock, address, m_eventHandler.m_data);
	}
}

GYINT32 GYListenThread::Init( const GYNetAddress& listenAddress, const AcceptEventHandler& handler )
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if(GYTRUE == m_acceptorSocket.IsRegisted())
		{
			break;
		}

		if (0 != m_acceptorSocket.Open(listenAddress))
		{
			break;
		}

		if (0 != m_reactor.Init(32))
		{
			break;
		}
		
		m_event.CleanUp();
		m_event.m_data = static_cast<GYVOID*>(this);
		m_event.m_accept = GYTRUE;
		m_event.m_busy = GYFALSE;
		m_event.m_eventType = GY_NET_EVENT_TYPE_READ;
		m_event.m_fd = &m_acceptorSocket;
		m_event.m_eventHandler = EpllEventHandler;
		if (0 != m_reactor.AddEvent(m_event))
		{
			break;
		}

		m_eventHandler = handler;
		result = 0;
	} while (GYFALSE);
	return result;
}

GYINT32 GYListenThread::Run()
{
	while(GYTRUE)
	{
		m_reactor.RunOnce();
	}
}

