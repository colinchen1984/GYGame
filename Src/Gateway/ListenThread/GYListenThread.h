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
#include "GYEvent.h"
class GYNetAddress;
class GYSocket;
typedef GYVOID (*GYAcceptConnectionEventHandler)(const GYSocket& sock, const GYNetAddress& listenAddress, GYVOID* RegistedData);

struct AcceptEventHandler 
{
	GYAcceptConnectionEventHandler	m_handler;
	GYVOID*							m_data;
	AcceptEventHandler()
	{
		m_data = GYNULL;
		m_handler = GYNULL;
	}
};

class GYListenThread
{
	GYReactor							m_reactor;
	GYListenSocket						m_acceptorSocket;
	AcceptEventHandler					m_eventHandler;
	GYNetEvent							m_event;
public:
	GYListenThread();
	~GYListenThread();

	GYINT32 Init(const GYNetAddress& listenAddress, const AcceptEventHandler& handler);
	
	GYINT32 Run();

	static GYVOID EpllEventHandler(GYNetEvent& event);

private:
	GYVOID _HandlerAcceptSocket();
};
#endif
