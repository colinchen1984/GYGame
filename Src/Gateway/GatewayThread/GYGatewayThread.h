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

const GYINT32 CLIENT_FOR_PER_THREAD = 32;
const GYINT32 LOGIC_SESSION_RECV_BUFFER_LEN = 1024 * 1024;
const GYINT32 LOGIC_SESSION_SEND_BUFFER_LEN = 1024 * 1024;
class GYGatewayThread
{
	GYBufferStreamSocket<LOGIC_SESSION_RECV_BUFFER_LEN, LOGIC_SESSION_SEND_BUFFER_LEN>	m_connection2Logic;
	GYNetAddress			m_targetServerAddress;
	GYNetEvent				m_event4Logic;
	GYReactor				m_reactor;
	GYList<GYClientSession> m_addSession;
	GYFastMutex				m_addSessionMutex;
	GYList<GYClientSession> m_workSession;
public:
	GYGatewayThread();

	~GYGatewayThread();

	GYINT32 Init(const GYNetAddress& targetServerAddress);
	
	GYVOID	Run();

	GYVOID	AddSession(GYClientSession& session);
};

#endif
