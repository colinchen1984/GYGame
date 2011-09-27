/////////////////////////////////////////////
// create time: 2011/6/12 14:33
// author:	colin chen
// file name:	GYEvent
// file type:	h
////////////////////////////////////////////

#ifndef __GYEVENT_H__
#define __GYEVENT_H__
#include "GYCommonDefine.h"
#include "GYObject.h"
enum GY_NET_EVNET_TYPE
{
	GY_NET_EVNET_TYPE_INVALID = -1,
	GY_NET_EVENT_TYPE_READ = 1,
	GY_NET_EVENT_TYPE_WRITE = 2,
	GY_NET_EVENT_TYPE_EXCEPTION = 4,
	GY_NET_EVENT_TYPE_COUNT,
};
struct GYNetEvent;
typedef GYVOID (*GYEventHandler)(GYNetEvent& event);

class GYSocket;
class GYReactor;
class GYEpollReactor;
class GYWin32SelectReactor;

struct GYNetEvent : public GYObject
{
	friend  GYEpollReactor;
	friend	GYWin32SelectReactor;
	friend  GYReactor;

	GYVOID*						m_data;			//指向的数据块
	GYBOOL						m_accept;		//是否绑定了监听端口
	GYBOOL						m_busy;			//是否正在工作
	GYINT16						m_eventType;	//事件类型
	GYSocket*					m_fd;			//fd
	GYNetEvent()
	{
		CleanUp();
	}
	GYVOID CleanUp()
	{
		m_data = GYNULL;
		m_accept = GYFALSE;
		m_busy = GYFALSE;
		m_eventType = 0;
		m_fd = GYNULL;
		GYZeroMem(m_eventHandler, sizeof(m_eventHandler));
	}
	GYVOID	SetEventHandler(GY_NET_EVNET_TYPE eventType, GYEventHandler handler)
	{
		m_eventHandler[eventType] = handler;
	}

private:
	GYEventHandler				m_eventHandler[GY_NET_EVENT_TYPE_COUNT];	//事件handler

};

#endif

