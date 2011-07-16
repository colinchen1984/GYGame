/////////////////////////////////////////////
// create time:2011/6/10 13:28
// author:	colin chen
// file name:	test
// file type:	cpp
////////////////////////////////////////////



/*#include "GYNetAddress.h"
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
	GYBOOL err = GYTRUE;
	if(ret > 0)
	{
		ret = stream->Send(testBUffer, ret);
		if(ret > 0)
		{
			err = GYTRUE;				
		}
		else
		{
			err = GYFALSE;
		}
	}
	else
	{
		err = GYFALSE;
	}
	if(GYTRUE != err)
	{
		printf("Delete Error Peer\n");
		reactor.DeleteEvent(event);
		delete stream;
		delete &event;
	}
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
		e->m_eventType = GY_NET_EVENT_TYPE_READ;
		reactor.AddEvent(*e);
	}
}

GYINT32 main()
{
	InitNetWork();
	GYNetAddress test;
	test.SetAddr("127.0.0.1");
	test.SetPort(9998);
	GYListenSocket listensocket;
	GYStreamSocket stream;
	listensocket.Open(test);
	listensocket.SetBlock(GYFALSE);
	GYNetEvent event;
	event.m_accept = GYTRUE;
	event.m_eventHandler = acceptHandler;
	event.m_fd = &listensocket;
	event.m_eventType = GY_NET_EVENT_TYPE_READ;
	
	reactor.Init(32);
	reactor.AddEvent(event);
	while (GYTRUE)
	{
		reactor.RunOnce();
	}
	return 0;
}
*/

/*#include <stdio.h>
#include <stdlib.h>
#include "GYThreadPool.h"
#include "GYThreadTask.h"
#include "GYThreadCommon.h"

const GYINT32 length = 1000;
GYINT32 g_array[length] = {0};
GYVOID ThreadHandler(GYVOID* param)
{
	for (GYINT32 i = 0; i < length; ++i)
	{
		g_array[i] = rand();
	}
	Sleep(3 * 1000);
	return ;
};
int main()
{
	InitThread();
	GYThreadTask t;
	t.m_threadFunction = ThreadHandler;
	ThreadPool.Init(10);
	ThreadPool.AddTask(t);
	ThreadPool.Release();
	getc(stdin);
};
*/

#include "GYList.h"
#include "GYArray.h"
#include "WinSock2.h"
#pragma comment(lib, "WSock32.lib")

struct Test
{
	Test* m_prev;
	Test* m_next;
	Test* m_this;
	GYINT32 m_index;
	Test()
	{
		memset(this, 0, sizeof(*this));
		m_this = this;
	}
};
const int len = 100;
Test g[len];
int main()
{
	WSADATA wsaData;
	if (NO_ERROR != WSAStartup(MAKEWORD(2,2), &wsaData))
	{
		return -1;
	}
	timeval waitTime;
	waitTime.tv_sec =0;
	waitTime.tv_usec =0;
	fd_set gg;
	int e = ::select(0, &gg, 0, 0, &waitTime);
	e = WSAGetLastError();
	GYList<Test> t;
	GYList<Test> t2;
	GYPointArray<Test> a;
	a.Init(len);
	for (int i =0; i < len; ++i)
	{
		a.Add(g[i]);
	}
	for (int i =49; i >= 0; --i)
	{
		t.Add(g[i]);
	}
	for (int i =len - 1; i >= 50; --i)
	{
		t2.Add(g[i]);
	}
	t.Add(t2);


	a.Delete(g[1]);
	
}

/*int main()
{
	int a = 0;
	if (a)
	{
		return 1;
	}
	if (0 != a)
	{
		return 1;
	}
	
	
}*/