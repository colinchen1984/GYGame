/////////////////////////////////////////////
// create time: 2011/6/12 14:33
// author:	colin chen
// file name:	GYEvent
// file type:	h
////////////////////////////////////////////

#ifndef __GYEVENT_H__
#define __GYEVENT_H__
#include "GYCommonDefine.h"

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

struct GYNetEvent
{
	GYVOID*						m_data;			//指向的数据块
	GYBOOL						m_accept;		//是否绑定了监听端口
	GYBOOL						m_busy;			//是否正在工作
	GY_NET_EVNET_TYPE			m_eventType;	//事件类型
	GYSocket*					m_fd;			//fd
	GYEventHandler				m_eventHandler;	//事件handler
	GYNetEvent*					m_prevEvent;	//链表中的上一个
	GYNetEvent*					m_nexEvent;		//链表中的下一个
	GYINT32						m_reactorIndex;	//reactor event*数组内的index
	GYINT64						m_recvDataCount;//接收的数据的总数
	GYINT64						m_sendDataCount;//发送的数据的总数

};

#endif

