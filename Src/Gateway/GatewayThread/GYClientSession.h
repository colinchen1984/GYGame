﻿/////////////////////////////////////////////
// create time: 2011/7/4 23:51
// author:	colin chen
// file name:	GYClientSession
// file type:	h
////////////////////////////////////////////

#ifndef __GYCLIENTSESSION_H__
#define __GYCLIENTSESSION_H__
#include "GYCommonDefine.h"
#include "GYNetAddress.h"
#include "GYBufferStreamSocket.h"
#include "GYEvent.h"
#include "GYObject.h"
const GYINT32 CLIENT_SESSION_RECV_BUFFER_LEN = 16 * 1024;
const GYINT32 CLIENT_SESSION_SEND_BUFFER_LEN = 16 * 1024;
class GYReactor;
enum EM_CLIENT_SESSION_STATUS
{
	EM_CLIENT_SESSION_STATUS_INVALID = -1,
	EM_CLIENT_SESSION_STATUS_IN_SERVER_WITH_NO_SERVER,			//尚未为客户的连接到指定的服务器
	EM_CLIENT_SESSION_STATUS_WITH_SERVER,						//已经为客户的连接到指定的服务器
	EM_CLIENT_SESSION_STATUS_COUNT,
};

static GYVOID HandleClientData(GYNetEvent& event);
class GYClientSession : public GYObject
{
	friend GYVOID HandleClientData(GYNetEvent& event);
	GYBufferStreamSocket<CLIENT_SESSION_RECV_BUFFER_LEN, CLIENT_SESSION_SEND_BUFFER_LEN>	m_connection;
	GYNetAddress	m_clientAddress;
	GYNetAddress	m_targetServerAddress;
	GYNetEvent		m_clientNetEvnet;
	GYVOID*			m_server;
	GYReactor*		m_reactor;
	EM_CLIENT_SESSION_STATUS m_status;
public:
	GYClientSession();
	~GYClientSession();
	GYVOID CleanUp();
	GYINT32 Init(const GYSocket& sock, const GYNetAddress& clientAddress);
	GYINT32 Regeist2Reactor(GYReactor& reactor, GYVOID* Onwer, EM_CLIENT_SESSION_STATUS status);

public:
	GYVOID OnReceiveWithServer();
	GYVOID OnReceiveWithNoServer();
	GYVOID OnClientCloseWithServer();
	GYVOID OnClientCloseWithNoServer();
};

#endif
