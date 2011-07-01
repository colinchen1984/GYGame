/////////////////////////////////////////////
// create time: 2011/7/1 13:47
// author:	colin chen
// file name:	GYListenThread
// file type:	h
////////////////////////////////////////////

#ifndef __GYLISTENTHREAD_H__
#define __GYLISTENTHREAD_H__
#include "GYReactor.h"
#include "GYSocket.h"
#include "GYArray.h"

class GYSocket;
typedef GYVOID (*GYAcceptConnectionEventHandler)(GYSocket& sock, GYVOID* RegistedData);

struct RegisteAcceptConnection 
{
	GYAcceptConnectionEventHandler	m_handler;
	GYVOID*							m_data;
	RegisteAcceptConnection()
	{
		m_data = GYNULL;
		m_handler = GYNULL;
	}
};

class GYListenThread
{
	GYReactor							m_reactor;
	GYListenSocket						m_acceptorActor;
	GYArray<RegisteAcceptConnection>	m_eventHandler;
public:
	GYListenThread();
	~GYListenThread();

	GYINT32 Init();

};
#endif
