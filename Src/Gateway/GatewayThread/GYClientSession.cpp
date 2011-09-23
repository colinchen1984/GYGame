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
#include <stdio.h>
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
	CleanUp();
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


GYVOID GYClientSession::Release()
{
	if(GYNULL != m_reactor)
	{
		m_reactor->DeleteEvent(m_clientNetEvnet);
		m_reactor = GYNULL;
	}
	m_connection.Close();

	//剔除玩家
	if(EM_CLIENT_SESSION_STATUS_WITH_SERVER == m_status)
	{
		static_cast<GYGatewayThread*>(m_server)->OnClientSessionClose(*this);
	}
	else if(EM_CLIENT_SESSION_STATUS_IN_SERVER_WITH_NO_SERVER == m_status)
	{
		static_cast<GYServer*>(m_server)->OnClientSessionClose(*this);
	}
	else
	{
		GYAssert(GYFALSE);
	}
	CleanUp();
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
	m_allSendDataSize = 0;
	m_allRecvDataSize = 0;
	m_clientGUID = INVALID_VALUE;
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
	//printf("Enter %s\n", "_OnReceiveWithServer");
	GY_SOCKET_OPERATION_ERROR_CODE result = m_connection.Recv();
	if (GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED == result 
		|| GY_SOCKET_OPERATION_ERROR_CODE_FAIL_TO_CHECK_SOCKET_CORE_BUFFER == result
		|| GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED == result)
	{
		Release();
		return;
	}
	if (GY_SOCKET_OPERATION_ERROR_CODE_SUCESS != result)
	{
		return;
	}
	while (GYTRUE)
	{
		GYINT32 length = INPUTBUFFER.GetReadSize();
		if (length > PacektHeadLen)
		{
			const GYPacketHead* const pPacketHead = reinterpret_cast<const GYPacketHead* const>(INPUTBUFFER.ReadPtr());
			GYAssert(PacketMaxLen >= pPacketHead->m_packetLen);
			if (length >= pPacketHead->m_packetLen + PacektHeadLen)
			{
				_ProcessInputData(static_cast<GYGatewayThread*>(m_server)->GetPacketFactoryManager(), *pPacketHead);
			}
		}
		else
		{
			break;
		}
	}
}

GYVOID GYClientSession::_OnReceiveWithNoServer()
{
	//printf("Enter %s\n", "_OnReceiveWithNoServer");
	GY_SOCKET_OPERATION_ERROR_CODE result = m_connection.Recv();
	if (GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED == result 
		|| GY_SOCKET_OPERATION_ERROR_CODE_FAIL_TO_CHECK_SOCKET_CORE_BUFFER == result
		|| GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED == result)
	{
		Release();
		return;
	}
	if (GY_SOCKET_OPERATION_ERROR_CODE_SUCESS != result)
	{
		return;
	}
	while(GYTRUE)
	{
		GYINT32 length = INPUTBUFFER.GetReadSize();
		if (length > PacektHeadLen)
		{
			const GYPacketHead* pPacketHead = reinterpret_cast<const GYPacketHead*>(INPUTBUFFER.ReadPtr());
			GYAssert(PacketMaxLen > pPacketHead->m_packetLen);
			if (length > pPacketHead->m_packetLen + PacektHeadLen)
			{
				_ProcessInputData(static_cast<GYServer*>(m_server)->GetPacketFactoryManager(), *pPacketHead);
			}
		}
		else
		{
			break;
		}
	}

}

GYVOID GYClientSession::SendPacketToClient(GYPacketInteface& packet)
{
	//printf("Enter %s\n", "SendPacketToClient");
	m_connection.Send();
	GYStreamSerialization<CLIENT_SESSION_SEND_BUFFER_LEN> packetSender(OUTPUTBUFFER, EM_SERIALIZAION_MODE_WRITE);
	packetSender << packet;
	m_allSendDataSize += packetSender.GetSerializDataSize();
}


GYINT32 GYClientSession::SendDataToClient( const GYPacketHead& packetHead, const GYCHAR* pData )
{
	//printf("Enter %s\n", "SendDataToClient");
	//这里插入玩家的GUID在数据包前面
	m_connection.Send();
	GYINT32 writeSpaceSize = m_connection.m_outputBuffer.GetWriteSize();
	const GYINT32 needSpaceSize = PacektHeadLen + packetHead.m_packetLen;
	if (writeSpaceSize <= needSpaceSize)
	{
		if (writeSpaceSize < needSpaceSize)
		{
			//实在是没有地方了，等等吧
			return INVALID_VALUE;
		}
	}
	else
	{
		GYAssert(GYFALSE);
	}
	GYAssert(0 == m_connection.m_outputBuffer.Write(pData, PacektHeadLen + packetHead.m_packetLen));
	return 0;
}

GYBOOL GYClientSession::Tick()
{
	GYBOOL result = GYFALSE;
	if (GY_SOCKET_OPERATION_ERROR_CODE_SUCESS == m_connection.Send())
	{
		result = GYTRUE;
	}
	else
	{
		Release();
	}
	return result;
}

GYINT32 GYClientSession::SendPacketToLogic( GYPacketInteface& packet )
{
	//printf("Enter %s\n", "SendPacketToLogic");
	GYINT32 result = INVALID_VALUE;
	if(EM_CLIENT_SESSION_STATUS_WITH_SERVER == m_status)
	{
		GYGatewayThread& gatewayThread = *static_cast<GYGatewayThread*>(m_server);
		gatewayThread.SendPacketToServer(*this, packet);
		result = 0;
	}
	return result;
}

GYVOID GYClientSession::SetGUID( const GYGUID& guid )
{
	//printf("Enter %s\n", "SendPacketToLogic");
	m_clientGUID = guid;
	GYGatewayThread& gatewayThread = *static_cast<GYGatewayThread*>(m_server);
	gatewayThread.RegisteSession(*this);
}





