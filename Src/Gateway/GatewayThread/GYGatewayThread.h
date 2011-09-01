/////////////////////////////////////////////
// create time: 2011/7/1 21:57
// author:	colin chen
// file name:	GYGatewayThread
// file type:	h
////////////////////////////////////////////

#ifndef __GYGATEWAYTHREAD_H__
#define __GYGATEWAYTHREAD_H__
#include "GYNetAddress.h"
#include "GYBufferStreamSocket.h"
#include "GYReactor.h"
#include "GYClientSession.h"
#include "GYFastMutex.h"
#include "GYList.h"
#include "GYHashTable.h"
#include "GYPacketFactoryManager.h"
#include "GYStringManger.h"

#ifdef WIN32
const GYINT32 CLIENT_FOR_PER_THREAD = 60;
#endif
#ifdef LINUX64
const GYINT32 CLIENT_FOR_PER_THREAD = 1024;
#endif // LINUX64

const GYINT32 LOGIC_SESSION_RECV_BUFFER_LEN = 1024 * 1024;
const GYINT32 LOGIC_SESSION_SEND_BUFFER_LEN = 1024 * 1024;

class GYServer;
struct GYPacketHead;

enum EM_GATE_WAY_THREAD_STATUS
{
	EM_GATE_WAY_THREAD_STATUS_INVALID = -1,
	EM_GATE_WAY_THREAD_STATUS_CONNECTING_LOGIC_SERVER,
	EM_GATE_WAY_THREAD_STATUS_SERVERING_CLIENT_SESSION,
	EM_GATE_WAY_THREAD_STATUS_EXIT,
	EM_GATE_WAY_THREAD_STATUS_COUNT,
};

class GYGatewayThread
{
	//连接logic server
	GYBufferStreamSocket<LOGIC_SESSION_RECV_BUFFER_LEN, LOGIC_SESSION_SEND_BUFFER_LEN>	m_connection2Logic;
	//logic server的Address
	GYNetAddress					m_targetServerAddress;
	//logic server相关的NetEvent
	GYNetEvent						m_event4Logic;

	//本线程的Reactor
	GYReactor						m_reactor;

	//接受监听线程传来的session，并且在下一帧的时候将所有的session转到自己的reactor内
	GYList<GYClientSession> 		m_addSession;
	GYFastMutex						m_addSessionMutex;

	//当前正在服务的session
	GYList<GYClientSession>			m_workSession;
	//当前正在服务的session的hash表，用于快速查找logic server传来的数据包对应的Client session
	GYHashTable<GYClientSession*>	m_workSessionHash;

	//当前帧内关闭的session，将在下一帧中还给监听线程
	GYList<GYClientSession>			m_ClosedSession;
	
	//监听线程中的GYServer
	GYServer*						m_server;
	
	//当前线程的状态
	EM_GATE_WAY_THREAD_STATUS		m_status;

	//下面的是每个线程需要的一些基础组件
	GYPacketFactoryManager			m_packetFactory;	//包工厂
	GYStringManager					m_stringManager;	//字符串管理器

public:
	GYGatewayThread();

	~GYGatewayThread();

	GYINT32 Init(const GYNetAddress& targetServerAddress, GYServer* server);
	
	GYVOID	Release();

	GYVOID	Run();

	GYVOID	AddSession(GYClientSession& session);

	GYVOID	OnClientSessionClose(GYClientSession& session);
	
	GYVOID	ProcessLogicServerData();

	GYINLINE const GYNetAddress& GetLogicServerAddress(){return m_targetServerAddress;}

	GYINLINE const GYINT32 GetCurrentSessionCount(){return m_workSession.GetItemCount();}

	GYINLINE GYVOID	StopGateThread(){_SetThreadStatus(EM_GATE_WAY_THREAD_STATUS_EXIT);};

	GYINLINE GYPacketFactoryManager& GetPacketFactoryManager(){return m_packetFactory;}

	GYINT32	SendDataToServer(GYClientSession& session, const GYPacketHead& packetHead, const GYCHAR* pData);

private:
	GYVOID	_ConnectLogicServer();

	GYVOID	_ServeringClientSession();

	GYINLINE GYVOID	_SetThreadStatus(EM_GATE_WAY_THREAD_STATUS status){m_status = status;}

	GYVOID _StopCurrentService();

	GYVOID _ProcessInputData(const GYGUID& clientGUID, const GYPacketHead& packetHead);

};

#endif
