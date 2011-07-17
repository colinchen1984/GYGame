/////////////////////////////////////////////
// create time: 2011/7/4 23:51
// author:	colin chen
// file name:	GYClientSession
// file type:	cpp
////////////////////////////////////////////
#include "GYClientSession.h" 
#include "GYReactor.h"

GYClientSession::GYClientSession()
{
	CleanUp();
}

GYClientSession::~GYClientSession()
{

}

static GYVOID HandleClientData(GYNetEvent& event)
{
	GYClientSession* pSession = static_cast<GYClientSession*>(event.m_data);
	pSession->_OnReceive();
}

GYINT32 GYClientSession::Init(const GYSocket& sock, const GYNetAddress& clientAddress, GYServer& server)
{
	GYINT32 result= INVALID_VALUE;
	do 
	{
		m_connection.SetFd(sock.GetFD());
		if (m_connection.SetBlock(GYFALSE))
		{
			m_connection.Close();
			break;
		}
		m_server = &server;
		m_clientAddress = clientAddress;
		result = 0;
	} while (GYFALSE);
	return result;
}

GYVOID GYClientSession::CleanUp()
{
	m_server = GYNULL;
	m_reactor = GYNULL;
	m_connection.CleanUp();
	m_clientAddress.CleanUp();
	m_targetServerAddress.CleanUp();
	m_clientNetEvnet.CleanUp();
}

#define  INPUTBUFFER m_connection.m_inputBuffer
#define  OUTPUTBUFFER m_connection.m_outputBuffer
GYVOID GYClientSession::_OnReceive()
{
	GY_SOCKET_OPERATION_ERROR_CODE result = m_connection.Recv();
	if (GY_SOCKET_OPERATION_ERROR_CODE_SUCESS != result)
	{
		if(GYNULL != m_reactor)
		{
			m_reactor->DeleteEvent(m_clientNetEvnet);
			m_reactor = GYNULL;
		}
		return;
	}
	GYINT32 length = INPUTBUFFER.GetReadSize();
	if (length > 0)
	{
		const GYCHAR* p = INPUTBUFFER.ReadPtr();
		m_connection.Send(p, length);
		INPUTBUFFER.ReadPtr(length);
	}
	
}

GYINT32 GYClientSession::Regeist2Reactor( GYReactor& reactor)
{
	if (GYNULL != m_reactor && 0 != m_reactor->DeleteEvent(m_clientNetEvnet))
	{
		return INVALID_VALUE;
	}
	m_clientNetEvnet.CleanUp();
	m_clientNetEvnet.m_accept = GYFALSE;
	m_clientNetEvnet.m_data = this;
	m_clientNetEvnet.m_eventHandler = HandleClientData;
	m_clientNetEvnet.m_fd = &m_connection;
	m_clientNetEvnet.m_eventType = GY_NET_EVENT_TYPE_READ;
	if (0 != reactor.AddEvent(m_clientNetEvnet))
	{
		m_connection.Close();
		m_reactor = GYNULL;
		return INVALID_VALUE;
	}
	m_reactor = &reactor;
	return 0;
}

