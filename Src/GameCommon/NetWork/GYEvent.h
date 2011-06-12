�?////////////////////////////////////////////
// create time: 2011/6/12 14:33
// author:	colin chen
// file name:	GYEvent
// file type:	h
////////////////////////////////////////////

#ifndef __GYEVENT_H__
#define __GYEVENT_H__
#include "GYCommonDefine.h"

enum GYNetEventType
{
	GYNetEventTypeInvalid = -1,
	GYNetEventTypeRead,				
	GYNetEventTypeWrite, 
	GYNetEventTypeException,
	GYNetEventTypeCount,
};
struct GYNetEvent;
typedef GYVOID (*GYEventHandler)(GYNetEvent& event);

class GYStreamSocket;

struct GYNetEvent
{
	GYVOID*						m_data;			//指向的数据块
	GYBOOL						m_accept;		//是否绑定了监听端�?
	GYBOOL						m_busy;			//是否正在工作
	GYNetEventType				m_eventType;	//事件类型
	GYStreamSocket*				m_fd;			//fd
	GYEventHandler				m_eventHandler;	//事件handler
	
	GYNetEvent*					m_prevEvent;	//链表中的上一�?
	GYNetEvent*					m_nexEvent;		//链表中的下一�?

	GYINT32						m_reactorIndex;	//reactor event*数组内的index

};

#endif