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
	GY_NET_EVENT_TYPE_READ,
	GY_NET_EVENT_TYPE_WRITE,
	GY_NET_EVENT_TYPE_EXCEPTION,
	GY_NET_EVENT_TYPE_COUNT,
};
struct GYNetEvent;
typedef GYVOID (*GYEventHandler)(GYNetEvent& event);

class GYSocket;

struct GYNetEvent : public GYObject
{
	GYVOID*						m_data;			//指向的数据块
	GYBOOL						m_accept;		//是否绑定了监听端口
	GYBOOL						m_busy;			//是否正在工作
	GY_NET_EVNET_TYPE			m_eventType;	//事件类型
	GYSocket*					m_fd;			//fd
	GYEventHandler				m_eventHandler;	//事件handler
	GYVOID CleanUp()
	{
		m_data = GYNULL;
		m_accept = GYFALSE;
		m_busy = GYFALSE;
		m_eventType = GY_NET_EVNET_TYPE_INVALID;
		m_fd = GYNULL;
		m_eventHandler = GYNULL;
	}
};

#endif

