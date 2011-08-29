/////////////////////////////////////////////
// create time: 2011/7/4 23:51
// author:	colin chen
// file name:	GYClientSession
// file type:	cpp
////////////////////////////////////////////
#include "GYClientSession.h" 
#include "GYReactor.h"
#include "GYGatewayThread.h"
#include "GYProtocolDefine.h"
#include "GYStreamSerialization.h"
#include "GYServer.h"

GYClientSession::GYClientSession()
{
	CleanUp();
}

GYClientSession::~GYClientSession()
{

}

typedef GYVOID (GYClientSession::*ClientHandler)();
static ClientHandler handler[EM_CLIENT_SESSION_STATUS_COUNT] = {GYNULL};

static GYVOID HandleClientData(GYNetEvent& event)
{
	GYClientSession* pSession = static_cast<GYClientSession*>(event.m_data);
	(pSession->*handler[pSession->GetStatus()])();
}

GYINT32 GYClientSession::Init(const GYSocket& sock, const GYNetAddress& clientAddress)
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
		m_clientAddress = clientAddress;
		result = 0;

		handler[EM_CLIENT_SESSION_STATUS_IN_SERVER_WITH_NO_SERVER] = &GYClientSession::_OnReceiveWithNoServer;
		handler[EM_CLIENT_SESSION_STATUS_WITH_SERVER] = &GYClientSession::_OnReceiveWithServer;
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
	m_status = EM_CLIENT_SESSION_STATUS_INVALID;
}

GYINT32 GYClientSession::Regeist2Reactor(GYReactor& reactor,  GYVOID* Onwer, EM_CLIENT_SESSION_STATUS status)
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
	m_status = status;
	m_reactor = &reactor;
	m_server = Onwer;
	return 0;
}

#define  INPUTBUFFER m_connection.m_inputBuffer
#define  OUTPUTBUFFER m_connection.m_outputBuffer
GYVOID GYClientSession::_OnReceiveWithServer()
{
	GY_SOCKET_OPERATION_ERROR_CODE result = m_connection.Recv();
	if (GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED == result 
		|| GY_SOCKET_OPERATION_ERROR_CODE_FAIL_TO_CHECK_SOCKET_CORE_BUFFER == result
		|| GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED == result)
	{
		_OnClientCloseWithServer();
		return;
	}
	if (GY_SOCKET_OPERATION_ERROR_CODE_SUCESS == result)
	{
		return;
	}

	GYINT32 length = INPUTBUFFER.GetReadSize();
	if (length > PacektHeadLen)
	{
		const GYPacketHead* const pPacketHead = reinterpret_cast<const GYPacketHead* const>(INPUTBUFFER.ReadPtr());
		GYAssert(PacketMaxLen > pPacketHead->m_packetLen);
		if (length > pPacketHead->m_packetLen + PacektHeadLen)
		{
			_ProcessInputData(*pPacketHead);
		}
	}
}

GYVOID GYClientSession::_OnReceiveWithNoServer()
{
	GY_SOCKET_OPERATION_ERROR_CODE result = m_connection.Recv();
	if (GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED == result 
		|| GY_SOCKET_OPERATION_ERROR_CODE_FAIL_TO_CHECK_SOCKET_CORE_BUFFER == result
		|| GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED == result)
	{
		_OnClientCloseWithNoServer();
		return;
	}
	if (GY_SOCKET_OPERATION_ERROR_CODE_SUCESS != result)
	{
		return;
	}

	GYINT32 length = INPUTBUFFER.GetReadSize();
	if (length > PacektHeadLen)
	{
		const GYPacketHead* pPacketHead = reinterpret_cast<const GYPacketHead*>(INPUTBUFFER.ReadPtr());
		GYAssert(PacketMaxLen > pPacketHead->m_packetLen);
		if (length > pPacketHead->m_packetLen + PacektHeadLen)
		{
			_ProcessInputData(*pPacketHead);
		}
	}
}

GYVOID GYClientSession::_OnClientCloseWithServer()
{
	if(GYNULL != m_reactor)
	{
		m_reactor->DeleteEvent(m_clientNetEvnet);
		m_reactor = GYNULL;
	}
	m_connection.Close();
	static_cast<GYGatewayThread*>(m_server)->OnClientSessionClose(*this);
	return;
}

GYVOID GYClientSession::_OnClientCloseWithNoServer()
{
	if(GYNULL != m_reactor)
	{
		m_reactor->DeleteEvent(m_clientNetEvnet);
		m_reactor = GYNULL;
	}
	m_connection.Close();
	static_cast<GYServer*>(m_server)->OnClientSessionClose(*this);
	return;
}

GYVOID GYClientSession::SendPacket(GYPacketInteface& packet)
{
	GYStreamSerialization<CLIENT_SESSION_SEND_BUFFER_LEN> packetSender(OUTPUTBUFFER, EM_SERIALIZAION_MODE_WRITE);
	packetSender << packet;
}

GYVOID GYClientSession::Tick()
{
	if (GY_SOCKET_OPERATION_ERROR_CODE_SUCESS != m_connection.Send())
	{
		switch(m_status)
		{
		case EM_CLIENT_SESSION_STATUS_WITH_SERVER:
			{
				_OnClientCloseWithServer();
			}
			break;
		case EM_CLIENT_SESSION_STATUS_IN_SERVER_WITH_NO_SERVER:
			{
				_OnClientCloseWithNoServer();
			}
			break;
		default:
			{
				GYAssert(GYFALSE);
			}
			break;
		}
	}
}



