/////////////////////////////////////////////
// create time: 2011/8/29 23:11
// author:	colin chen
// file name:	GYClientProcessData
// file type:	cpp
////////////////////////////////////////////
#include "GYClientSession.h"
#include "GYProtocolDefine.h"
#include "GYGatewayThread.h"
#include "GYPacketFactoryManager.h"
#include "GYStreamSerialization.h"

#define  INPUTBUFFER m_connection.m_inputBuffer
#define  OUTPUTBUFFER m_connection.m_outputBuffer

GYVOID GYClientSession::_ProcessInputData(GYPacketFactoryManager& packetFactory, const GYCSPacketHead& packetHead)
{
	GYPacketInteface* packet = packetFactory.GetPacketByID(GYGetPacketID(packetHead.m_id));
	const GYINT32 dataLen = CSPacektHeadLen + packetHead.m_packetLen;
	if (GYNULL != packet)
	{
		GYStreamSerialization<CLIENT_SESSION_RECV_BUFFER_LEN> streamSerializer(INPUTBUFFER, EM_SERIALIZAION_MODE_READ);
		streamSerializer << *packet;
		//保证数据读取正确
		GYAssert(dataLen == streamSerializer.GetSerializDataSize());
		//调用相关函数处理数据包
		

		m_allRecvDataSize += dataLen;
	}
	else
	{
		//目前认为如果没有注册该包，就认为Gateway不需要处理该包
		//直接发往LogicServer
		//如果当前还没有连接到指定的LogicServer
		//则认为数据错误，直接跳过相关数据
		if(EM_CLIENT_SESSION_STATUS_WITH_SERVER == m_status)
		{
			GYGatewayThread& gatewayThread = *static_cast<GYGatewayThread*>(m_server);
			const GYCHAR* pData = INPUTBUFFER.ReadPtr();
			if(0 == gatewayThread.SendDataToServer(*this, packetHead, pData))
			{
				INPUTBUFFER.ReadPtr(dataLen);
				m_allRecvDataSize += dataLen;
			}
			else
			{
				//发送数据包错误，下次继续发送
			}
		}
		else
		{
			//数据错误，如果需要则进行相关处理
			INPUTBUFFER.ReadPtr(dataLen);
			m_allRecvDataSize += dataLen;
		}
	}
}
