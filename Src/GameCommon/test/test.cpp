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
#include "GYTimeStamp.h"
#include "GYTestPacket.h"
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
char testbuffer[1024] = {0};
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
		int err =setsockopt(p->GetFD(), SOL_SOCKET,SO_RCVBUF,testbuffer,1024 );
		int error = GetLastNetWorkError();
		reactor.AddEvent(*e);
	}
}

GYINT32 main()
{
	char b[1024];
	GYTestPacketWrap w;
	const char* name = "陈琳";
	w.Init(b, strlen(name));
	w.SetUseName(name, strlen(name));
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
		GYTimeStamp t;
		t.m_termTime = 30;
		reactor.RunOnce(t);
	}
	return 0;
}


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
};
*/

/*#include "GYList.h"
#include "GYArray.h"
#include "GYStringManger.h"
#include "GYHashTable.h"
struct Test
{
	Test* m_prev;
	Test* m_next;
	Test* m_this;
	GYINT32 m_index;
	GYINT32 testData[1];
	Test()
	{
		memset(this, 0, sizeof(*this));
		m_this = this;
	}
}__attribute__( ( packed, aligned( 1 ) ) );
const int len = 100;
Test g[len];
#include <bitset>
#include "GYString.h"
using std::bitset;
GYStringManager gM;
int main()
{
	Test t1[1];
	int lene = sizeof(*t1);
	t1->m_prev = (Test*)0x0fffffff;
	t1->m_next = (Test*)0x0fffffff;
	t1->m_index = 0x0fffffff;
	t1->testData = (int*)0x0fffffff;
	gM.Init();
	GYHashTable<GYINT32> hashTest;
	hashTest.Init(100, 100);
	hashTest.Insert(12, 22);
	hashTest.Remove(12);
	hashTest.Remove(12);
	const GYWCHAR* p = L"test";
	GYINT32 len = wcslen(p);
	GYString teststing(p, len, gM);
	GYString teststing2 = teststing;
	teststing == teststing2;
// 	timeval waitTime;
// 	waitTime.tv_sec = 1;
// 	waitTime.tv_usec = 0;
// 	int e = ::select(0, 0, 0, 0, &waitTime);
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

int main()
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
