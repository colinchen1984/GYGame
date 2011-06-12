/////////////////////////////////////////////
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
	GYVOID*						m_data;			//ָ������ݿ�
	GYBOOL						m_accept;		//�Ƿ���˼����˿�
	GYBOOL						m_busy;			//�Ƿ����ڹ���
	GYNetEventType				m_eventType;	//�¼�����
	GYStreamSocket*				m_fd;			//fd
	GYEventHandler				m_eventHandler;	//�¼�handler
	
	GYNetEvent*					m_prevEvent;	//�����е���һ��
	GYNetEvent*					m_nexEvent;		//�����е���һ��

	GYINT32						m_reactorIndex;	//reactor event*�����ڵ�index

};

#endif