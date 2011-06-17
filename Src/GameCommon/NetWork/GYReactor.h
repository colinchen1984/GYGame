/////////////////////////////////////////////
// create time: 2011/6/12 14:29
// author:	colin chen
// file name:	GYReactor
// file type:	h
////////////////////////////////////////////

#ifndef __GYREACTOR_H__
#define __GYREACTOR_H__
#ifdef WIN32
#include "GYWin32Select.h"
#endif // WIN32
#ifdef LINUX64
#include "GYEpoll.h"
#endif // LINUX64

#include "GYCommonDefine.h"
struct GYNetEvent;

class GYReactor
{
	GYNetEvent**	m_event;
	GYINT32			m_maxEventCount;
	GYINT32			m_currentEventCount;
	GYNetEvent*		m_eventList;
#ifdef WIN32
	GYWin32SelectReactor m_reactor;
#endif // WIN32
#ifdef LINUX64
	GYEpollReactor m_reactor;
#endif // LINUX64

public:
	GYReactor();
	~GYReactor();

	GYINT32 Init(GYINT32 maxEventCount);
	
	GYINT32 Release();

	GYINT32 AddEvent(GYNetEvent& event);

	GYINT32 DeleteEvent(GYNetEvent& event);
	
	GYINT32	RunOnce();

	GYINT32 GetCurrentEventCount();

	GYINT32 GetMaxEventCount();

	const GYNetEvent** GetEventSet();

	GYVOID	PostEvent(GYNetEvent& event);
private:
	GYVOID	_CleanUp();
};

#endif

