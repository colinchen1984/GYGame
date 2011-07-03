/////////////////////////////////////////////
// create time: 2011/7/1 17:27
// author:	colin chen
// file name:	GYServer
// file type:	h
////////////////////////////////////////////

#ifndef __GYSERVER_H__
#define __GYSERVER_H__
#include "GYListenThread.h"
#include "GYThreadPool.h"
class GYGatewayThread;
class GYServer
{
	GYListenThread		m_listenThread;
	GYGatewayThread*	m_gateThread;
public:
	GYServer();
	~GYServer();

	GYINT32 Init();
};

#endif