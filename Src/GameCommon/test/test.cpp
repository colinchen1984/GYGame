/////////////////////////////////////////////
// create time:2011/6/10 13:28
// author:	colin chen
// file name:	test
// file type:	cpp
////////////////////////////////////////////



#include "GYNetAddress.h"
#include "GYCommonDefine.h"
#include "GYNetWorkCommonDefine.h"
#include "GYSocket.h"
#include "GYEvent.h"
#include "GYReactor.h"
#include <list>
#include <stdio.h>
using std::list;

GYReactor reactor;
GYVOID testHandler(GYNetEvent& event)
{
	static GYINT32 i = 0;
	GYCHAR testBUffer[1024] = {0};
	GYStreamSocket* stream = static_cast<GYStreamSocket*>(event.m_fd);
	GYINT32 ret = stream->Recv(testBUffer, 1024);
	if(ret > 0)
	{
		stream->Send(testBUffer, ret);
	}
	else
	{
		reactor.DeleteEvent(event);
		delete stream;
		delete &event;
	}
	printf("%d\n", ++i);
}

GYVOID acceptHandler(GYNetEvent& event)
{
	GYStreamSocket stream;
	GYNetAddress address;
	GYStreamSocket* p = GYNULL;
	GYNetEvent* e = GYNULL;
	GYListenSocket& listensocket = *static_cast<GYListenSocket*>(event.m_fd);
	while(0 == listensocket.Accept(stream, address))
	{
		p = new GYStreamSocket();
		e = new GYNetEvent();
		*p = stream;
		e->m_accept = GYFALSE;
		e->m_eventHandler = testHandler;
		e->m_fd = p;
		e->m_eventType = GYNetEventTypeRead;
		reactor.AddEvent(*e);
	}
}

GYINT32 main()
{
	InitNetWork();
	GYNetAddress test;
	test.SetAddr("127.0.0.1");
	test.SetPort(9999);
	GYCHAR testBUffer[1024] = {0};
	GYListenSocket listensocket;
	GYStreamSocket stream;
	listensocket.Open(test);
	listensocket.SetBlock(GYFALSE);
	GYINT32 i = 0;
	GYNetEvent event;
	event.m_accept = GYTRUE;
	event.m_eventHandler = acceptHandler;
	event.m_fd = &listensocket;
	event.m_eventType = GYNetEventTypeRead;
	
	reactor.Init(1024);
	reactor.AddEvent(event);
	while (GYTRUE)
	{
		reactor.RunOnce();
	}
	return 0;
}

