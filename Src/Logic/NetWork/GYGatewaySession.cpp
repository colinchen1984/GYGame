/////////////////////////////////////////////
// create time: 2011/9/6 22:47
// author:	colin chen
// file name:	GYGatewaySession
// file type:	cpp
////////////////////////////////////////////
#include "GYGatewaySession.h"
#include "GYReactor.h"
#include "GYProtocolDefine.h"
#include "GYStreamSerialization.h"
#include "GYServer.h"

GYGatewaySession::GYGatewaySession()
{
 	CleanUp();
}

GYGatewaySession::~GYGatewaySession()
{

}

GYVOID HandleGatewayData(GYNetEvent& event)
{
	GYGatewaySession* pSession = static_cast<GYGatewaySession*>(event.m_data);
	pSession->_OnReceive();
}

GYINT32 GYGatewaySession::Init(GYNetWorkManager& networkManager, GYReactor& reactor, const GYSocket& sock, const GYNetAddress& gatewayAddress)
{
	GYINT32 result= INVALID_VALUE;
	do 
	{
		if (GYNULL != m_reactor)
		{
			break;
		}
		
		if (GYNULL != m_networkManger)
		{
			break;
		}
		CleanUp();
		m_networkManger = &networkManager;
		m_connection.SetFd(sock.GetFD());
		if (m_connection.SetBlock(GYFALSE))
		{
			m_connection.Close();
			CleanUp();
			break;
		}

		m_gatewaySessionEvnet.CleanUp();
		m_gatewaySessionEvnet.m_accept = GYFALSE;
		m_gatewaySessionEvnet.m_data = this;
		m_gatewaySessionEvnet.m_eventHandler = HandleGatewayData;
		m_gatewaySessionEvnet.m_fd = &m_connection;
		m_gatewaySessionEvnet.m_eventType = GY_NET_EVENT_TYPE_READ;
		if (0 != reactor.AddEvent(m_gatewaySessionEvnet))
		{
			m_connection.Close();
			reactor.Release();
			CleanUp();
			break;
		}
		m_reactor = &reactor;

		m_gatewayAddress = gatewayAddress;

		result = 0;
	} while (GYFALSE);
	return result;
}


GYVOID GYGatewaySession::Release()
{
	if(GYNULL != m_reactor)
	{
		m_reactor->DeleteEvent(m_gatewaySessionEvnet);
		m_reactor = GYNULL;
	}
	m_connection.Close();
	m_networkManger->OnGatewaySessionClose(*this);
	CleanUp();
}


GYVOID GYGatewaySession::CleanUp()
{
	m_reactor = GYNULL;
	m_networkManger = GYNULL;
	m_connection.CleanUp();
	m_gatewayAddress.CleanUp();
	m_gatewaySessionEvnet.CleanUp();
	m_allSendDataSize = 0;
	m_allRecvDataSize = 0;
}

#define  INPUTBUFFER m_connection.m_inputBuffer
#define  OUTPUTBUFFER m_connection.m_outputBuffer
GYVOID GYGatewaySession::_OnReceive()
{
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
		if (length > PacektHeadLen + GYGUIDLEN)
		{
			const GYGUID* const pHumanGUID = reinterpret_cast<const GYGUID* const>(INPUTBUFFER.ReadPtr());
			const GYPacketHead* const pPacketHead = reinterpret_cast<const GYPacketHead* const>(pHumanGUID + 1);
			GYAssert(PacketMaxLen >= pPacketHead->m_packetLen);
			if (length >= pPacketHead->m_packetLen + PacektHeadLen + GYGUIDLEN)
			{
				_ProcessInputData(GETPACKETFACTORY, *pHumanGUID, *pPacketHead);
			}
		}
		else
		{
			break;
		}
	}

}
	

GYVOID GYGatewaySession::SendPacket(const GYGUID& guid, GYPacketInteface& packet)
{
	m_connection.Send();
	GYStreamSerialization<GATEWAY_SESSION_SEND_BUFFER_LEN> packetSender(m_connection.m_outputBuffer, EM_SERIALIZAION_MODE_WRITE);
	packetSender << const_cast<GYGUID&>(guid);
	packetSender << packet;
	m_allSendDataSize += packetSender.GetSerializDataSize();
	GYAssert(m_connection.m_outputBuffer.GetWriteSize() > 0);
}

GYBOOL GYGatewaySession::Tick()
{
	GYBOOL result = GYFALSE;
	if (GY_SOCKET_OPERATION_ERROR_CODE_SUCESS == m_connection.Send())
	{
		result = GYTRUE;
	}
	return result;
}


GYVOID GYGatewaySession::_ProcessInputData( GYPacketFactoryManager& packetFactory, const GYGUID& guid, const GYPacketHead& packetHead )
{
	GYPacketInteface* packet = packetFactory.GetPacketByID(GYGetPacketID(packetHead.m_id));
	const GYINT32 dataLen = GYGUIDLEN + PacektHeadLen + packetHead.m_packetLen;
	if (GYNULL != packet)
	{
		GYStreamSerialization<GATEWAY_SESSION_RECV_BUFFER_LEN> streamSerializer(m_connection.m_inputBuffer, EM_SERIALIZAION_MODE_READ);
		GYGUID tempGuid;
		streamSerializer << tempGuid;
		streamSerializer << *packet;
		//保证数据读取正确
		GYAssert(dataLen == streamSerializer.GetSerializDataSize());
		m_allRecvDataSize += dataLen;
		//调用相关函数处理数据包
		PacketHandler pHandler = packetFactory.GetPacketHandlerByID(GYGetPacketID(packetHead.m_id));
		GYAssert(GYNULL != pHandler);
		if (GYTRUE == pHandler(*this, tempGuid, *packet))
		{
		}
		else
		{
			Release();
		}
		packetFactory.ReleasePacket(*packet);

	}
	else
	{
		GYAssert(GYFALSE);
	}
}



