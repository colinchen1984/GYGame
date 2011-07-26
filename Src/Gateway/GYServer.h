/////////////////////////////////////////////
// create time: 2011/7/1 17:27
// author:	colin chen
// file name:	GYServer
// file type:	h
////////////////////////////////////////////

#ifndef __GYSERVER_H__
#define __GYSERVER_H__
#include "GYThreadPool.h"
#include "GYReactor.h"
#include "GYSocket.h"
#include "GYList.h"
#include "GYClientSession.h"
#include "GYFastMutex.h"
class GYGatewayThread;
class GYServer
{
	friend GYVOID AcceptEventHandler(GYNetEvent& event);

	//GYServer需要负责监听
	GYReactor							m_reactor;
	GYListenSocket						m_acceptorSocket;
	GYNetEvent							m_listenEvent;
	GYGatewayThread*					m_gateThread;
	GYINT32								m_gateThreadCount;
	GYList<GYClientSession>				m_usingClientSession;
	GYList<GYClientSession>				m_freeClientSession;
	GYClientSession*					m_wholeClientSession;
	GYFastMutex							m_sessionCloseMutex;
	GYBOOL								m_isServering;
public:
	GYServer();
	~GYServer();

	GYINT32 Init();
	GYINT32 RunOnce();
	GYINT32 Release();

	GYVOID OnClientSessionClose(GYClientSession& session);
private:
	GYVOID _OnAcceptClient();
};

#endif