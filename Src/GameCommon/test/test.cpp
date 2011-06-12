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

#include <stdio.h>

GYVOID testHandler(GYNetEvent& event)
{
	static GYINT32 i = 0;
	GYCHAR testBUffer[1024] = {0};
	GYINT32 ret = event.m_fd->Recv(testBUffer, 1024);
	event.m_fd->Send(testBUffer, ret);
	printf("%d\n", ++i);
}

GYINT32 main()
{
	InitNetWork();
	GYNetAddress test;
	test.SetAddr("127.0.0.1");
	test.SetPort(5555);
	GYCHAR testBUffer[1024] = {0};
	GYListenSocket listensocket;
	GYStreamSocket stream;
	listensocket.Open(test);
	if (INVALID_VALUE == listensocket.Accept(stream, test))
	{
		return -1;
	}
	GYINT32 i = 0;
	GYNetEvent event;
	event.m_accept = GYFALSE;
	event.m_eventHandler = testHandler;
	event.m_fd = &stream;
	event.m_eventType = GYNetEventTypeRead;
	GYReactor reactor;
	reactor.Init(32);
	reactor.AddEvent(event);
	while (GYTRUE)
	{
		reactor.RunOnce();
	}
	return 0;
}
