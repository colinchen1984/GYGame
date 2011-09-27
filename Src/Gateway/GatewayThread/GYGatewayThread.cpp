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
#include "GYProtocolDefine.h"
#include "GYStreamSerialization.h"
#include "GYTimeController.h"

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

		if(0 != m_stringManager.Init())
		{
			m_connection2Logic.Close();
			m_reactor.Release();
			break;
		}
		
		if (0 != m_packetFactory.Init(m_stringManager))
		{
			m_connection2Logic.Close();
			m_reactor.Release();
			break;
		}
		if (0 != m_workSessionHash.Init(CLIENT_FOR_PER_THREAD, CLIENT_FOR_PER_THREAD))
		{
			m_connection2Logic.Close();
			m_reactor.Release();
			m_packetFactory.Release();
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

GYVOID GYGatewayThread::Release()
{
	_StopCurrentService();
	m_reactor.Release();
	m_packetFactory.Release();
}

static GYVOID LogicServerConnectionEventHandler(GYNetEvent& event)
{
	GYGatewayThread* pThrad = static_cast<GYGatewayThread*>(event.m_data);
	pThrad->ProcessLogicServerData();
}


GYVOID GYGatewayThread::Run()
{
	while(GYTRUE)
	{
		//printf("Begin GYGatewayThread::Run in %u\n", this);
		if (EM_GATE_WAY_THREAD_STATUS_EXIT != m_status)
		{
			(this->*handler[m_status])();
			;
			GYClientSession* pSession;
			GYList<GYClientSession> tempSessionList;
			while(GYNULL != (pSession = m_workSession.PickUpFirstItem()))
			{
				if (GYTRUE == pSession->Tick())
				{
					tempSessionList.Add(*pSession);
				}
			}
			m_workSession = tempSessionList;
		} 
		else
		{
			_StopCurrentService();
			m_reactor.Release();
			break;
		}
		
	};
}

GYVOID GYGatewayThread::AddWorkingSession( GYClientSession& session )
{
	GYGuard<GYFastMutex> g(m_addSessionMutex);
	m_addSession.Add(session);
}

GYVOID GYGatewayThread::OnClientSessionClose( GYClientSession& session )
{
	m_workSession.Delete(session);
	m_ClosedSession.Add(session);
	m_workSessionHash.Remove(session.GetGUID());
}

GYVOID GYGatewayThread::_ConnectLogicServer()
{
	if(0 != m_connection2Logic.Connect(m_targetServerAddress))
	{
		printf("Connect logic server fail\n");
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
	m_event4Logic.m_busy = GYFALSE;
	m_event4Logic.m_data = this;
	m_event4Logic.SetEventHandler(GY_NET_EVENT_TYPE_READ, LogicServerConnectionEventHandler);
	if (0 != m_reactor.AddEvent(m_event4Logic))
	{
		m_connection2Logic.Close();
		return;
	}
	_SetThreadStatus(EM_GATE_WAY_THREAD_STATUS_SERVERING_CLIENT_SESSION);

}

GYVOID GYGatewayThread::_ServeringClientSession()
{
	//printf("Enter %s\n", "_ServeringClientSession");
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
	m_connection2Logic.Send();
	static GYTimeStamp termTime;
	termTime.m_termTime = 1;
	m_reactor.RunOnce(termTime);
	return;
}

GYVOID GYGatewayThread::ProcessLogicServerData()
{
	//printf("Enter %s\n", "ProcessLogicServerData");
	GY_SOCKET_OPERATION_ERROR_CODE result = m_connection2Logic.Recv();
	if (GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED == result 
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
	while(GYTRUE)
	{
		GYINT32 length = m_connection2Logic.m_inputBuffer.GetReadSize();
		const static GYINT32 headLen = GYGUIDLEN + PacektHeadLen;
		if (length > headLen)
		{
			const GYCHAR* pData = m_connection2Logic.m_inputBuffer.ReadPtr();
			const GYGUID& guid = *reinterpret_cast<const GYGUID*>(pData);
			const GYPacketHead& packetHead = *reinterpret_cast<const GYPacketHead*>(pData + GYGUIDLEN);
			if (length >= headLen + packetHead.m_packetLen)
			{
				_ProcessInputData(guid, packetHead);			
			}

		}
		else
		{
			break;
		}
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
		pSession->Release();
	}
	//将关闭了的Client Session还给GYServer
	while (GYNULL != (pSession = m_ClosedSession.PickUpFirstItem()))
	{
		m_server->OnClientSessionClose(*pSession);
	}
	//清楚Hash表
	m_workSessionHash.CleanUp();
}

GYINT32 GYGatewayThread::SendDataToServer( GYClientSession& session, const GYPacketHead& packetHead, const GYCHAR* pData)
{
	//printf("Enter %s\n", "SendDataToServer");
	//这里插入玩家的GUID在数据包前面
	m_connection2Logic.Send();
	const GYGUID& clientGUID = session.GetGUID();
	GYINT32 writeSpaceSize = m_connection2Logic.m_outputBuffer.GetWriteSize();
	const GYINT32 needSpaceSize = GYGUIDLEN + PacektHeadLen + packetHead.m_packetLen;
	if (writeSpaceSize < needSpaceSize)
	{
		m_connection2Logic.Send();
		writeSpaceSize = m_connection2Logic.m_outputBuffer.GetWriteSize();
		if (writeSpaceSize < needSpaceSize)
		{
			//实在是没有地方了，等等吧
			return INVALID_VALUE;
		}
	}
	GYAssert(0 == m_connection2Logic.m_outputBuffer.Write(reinterpret_cast<const GYCHAR*>(&clientGUID), GYGUIDLEN));
	GYAssert(0 == m_connection2Logic.m_outputBuffer.Write(pData, PacektHeadLen + packetHead.m_packetLen));
	return 0;
}

GYINT32 GYGatewayThread::SendPacketToServer(GYClientSession& session, const GYPacketInteface& packet)
{
	//printf("Enter %s\n", "SendPacketToServer");
	//这里插入玩家的GUID在数据包前面
	m_connection2Logic.Send();
	GYStreamSerialization<LOGIC_SESSION_SEND_BUFFER_LEN> packetSender(m_connection2Logic.m_outputBuffer, EM_SERIALIZAION_MODE_WRITE);
	packetSender << const_cast<GYGUID&>(session.GetGUID());
	packetSender << const_cast<GYPacketInteface&>(packet);
	return 0;
}


GYVOID GYGatewayThread::_ProcessInputData(const GYGUID& clientGUID, const GYPacketHead& packetHead)
{
	//printf("Enter %s\n", "GYGatewayThread::_ProcessInputData");
	const GYINT32 dataLen = GYGUIDLEN + PacektHeadLen + packetHead.m_packetLen;

	GYClientSession** pClentSession = m_workSessionHash.Find(clientGUID);
	if (GYNULL == pClentSession || GYNULL == *pClentSession)
	{
		//服务器发过来的数据出错
		//可能是Client断开后logic发来数据
		m_connection2Logic.m_inputBuffer.ReadPtr(dataLen);
		return;
	}

	GYPacketInteface* packet = m_packetFactory.GetPacketByID(GYGetPacketID(packetHead.m_id));
	GYClientSession& client = **pClentSession;
	GYAssert(client.GetGUID() == clientGUID);
	if (GYNULL != packet)
	{
		GYStreamSerialization<LOGIC_SESSION_RECV_BUFFER_LEN> streamSerializer(m_connection2Logic.m_inputBuffer, EM_SERIALIZAION_MODE_READ);
		GYGUID guid;
		streamSerializer << guid;

		streamSerializer << *packet;
		//保证数据读取正确
		GYAssert(dataLen == streamSerializer.GetSerializDataSize());
		//调用相关函数处理数据包
		PacketHandler pHandler = m_packetFactory.GetPacketHandlerByID(GYGetPacketID(packetHead.m_id));
		GYAssert(GYNULL != pHandler);
		if (GYTRUE == pHandler(client, *packet))
		{
		}
		else
		{
			client.Release();
		}
		m_packetFactory.ReleasePacket(*packet);
		packet = GYNULL;
	}
	else
	{
		//目前认为如果没有注册该包，就认为Gateway不需要处理该包
		//直接发往Client
		const GYCHAR* const pData = m_connection2Logic.m_inputBuffer.ReadPtr();
		if(0 == client.SendDataToClient(packetHead, (pData + GYGUIDLEN)))
		{
			m_connection2Logic.m_inputBuffer.ReadPtr(dataLen);
		}
		else
		{
			//等会再发吧
		}
	}
	GYAssert(GYNULL == packet);
}

GYINT32 GYGatewayThread::RegisteSession( GYClientSession& session )
{
	GYINT32 result = m_workSessionHash.Insert(session.GetGUID(), &session);
	return result;
}


