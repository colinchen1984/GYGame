/////////////////////////////////////////////
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
class GYServer;
class GYClientSession : public GYObject
{
	friend static GYVOID HandleClientData(GYNetEvent& event);
	GYBufferStreamSocket<CLIENT_SESSION_RECV_BUFFER_LEN, CLIENT_SESSION_SEND_BUFFER_LEN>	m_connection;
	GYNetAddress	m_clientAddress;
	GYNetAddress	m_targetServerAddress;
	GYNetEvent		m_clientNetEvnet;
	GYServer*		m_server;
	GYReactor*		m_reactor;
public:
	GYClientSession();
	~GYClientSession();
	GYVOID CleanUp();
	GYINT32 Init(const GYSocket& sock, const GYNetAddress& clientAddress, GYServer& server);
	GYINT32 Regeist2Reactor(GYReactor& reactor);

private:
	GYVOID _OnReceive();
};
#endif
