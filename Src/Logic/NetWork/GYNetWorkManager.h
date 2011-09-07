/////////////////////////////////////////////
// create time: 2011/9/4 16:31
// author:	colin chen
// file name:	GYNetWorkManager
// file type:	h
////////////////////////////////////////////

#ifndef __GYNETWORKMANAGER_H__
#define __GYNETWORKMANAGER_H__
#include "GYReactor.h"
#include "GYList.h"
#include "GYGatewaySession.h"
#include "GYSocket.h"
#include "GYEvent.h"

class GYTimeStamp;

class GYNetWorkManager
{
	friend GYVOID AcceptEventHandler(GYNetEvent& event);
	GYReactor						m_reactor;
	GYGatewaySession*				m_gatewaySession;
	GYList<GYGatewaySession>		m_freeGatewaySessionList;
	GYList<GYGatewaySession>		m_workSession;
	GYNetAddress					m_listenAddress;
	GYListenSocket					m_acceptorSocket;
	GYNetEvent						m_listenEvent;
public:
	GYNetWorkManager();

	~GYNetWorkManager();

	GYVOID	CleanUp();

	GYINT32	Init(const GYNetAddress& listenAddress);

	GYVOID Release();

	GYVOID RunOnce(const GYTimeStamp& timeStamp);

	GYVOID OnGatewaySessionClose(GYGatewaySession& session);

private:
	GYVOID	_OnAcceptGateway();
};

#endif
