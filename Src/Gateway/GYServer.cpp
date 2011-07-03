/////////////////////////////////////////////
// create time: 2011/7/1 17:27
// author:	colin chen
// file name:	GYServer
// file type:	cpp
////////////////////////////////////////////
#include "GYServer.h"
#include "GYThreadTask.h"
#include "GYNetAddress.h"
#include "GYGatewayThread.h"

const GYINT32 listendThreadCount = 1;
const GYINT32 gatewayThreadCount = 1;
GYServer::GYServer()
{
	m_gateThread = GYNULL;
}

GYServer::~GYServer()
{
}

static GYVOID AcceptThread(GYVOID* param)
{
	GYListenThread* pThread = static_cast<GYListenThread*>(param);
	pThread->Run();
}

static GYVOID AcceptConnectionHandler(const GYSocket& sock, const GYNetAddress& listenAddress, GYVOID* RegistedData)
{
	GYServer* pServer = static_cast<GYServer*>(RegistedData);
	//pServer->
}

GYINT32 GYServer::Init()
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		//设置监听线程
		GYNetAddress address;
		address.SetAddr("127.0.0.1");
		address.SetPort(5555);
		AcceptEventHandler acceptHandler;
		acceptHandler.m_handler = AcceptConnectionHandler;
		acceptHandler.m_data = static_cast<GYVOID*>(this);
		if (0 != m_listenThread.Init(address, acceptHandler))
		{
			break;
		}
		
		//设置gatewany工作线程
		m_gateThread = GYNew GYGatewayThread[gatewayThreadCount];
		
		if(0 != ThreadPool.Init(gatewayThreadCount + listendThreadCount))
		{
			break;
		}
		GYThreadTask acceptTask;
		acceptTask.m_threadFunction = AcceptThread;
		acceptTask.param = static_cast<GYVOID*>(&m_listenThread);
		ThreadPool.AddTask(acceptTask);
	} while (GYFALSE);
	return result;	
}

