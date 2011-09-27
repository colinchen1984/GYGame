/////////////////////////////////////////////
// create time: 2011/6/12 22:31
// author:	colin chen
// file name:	GYEpoll
// file type:	h
////////////////////////////////////////////

#ifndef __GYEPOLL_H__
#define __GYEPOLL_H__
#ifdef LINUX64
#include "GYCommonDefine.h"
#include "GYNetWorkCommonDefine.h"
class GYNetEvent;
class GYReactor;
struct epoll_event;
class GYTimeStamp;

class GYEpollReactor
{
    friend class GYReactor;
    GYReactor*					m_reactor;
    GYINT32						m_nFdForEpoll;
    epoll_event*				m_pEvForWait;
private:
    GYEpollReactor();
    ~GYEpollReactor();

    GYINT32 _Init(GYINT32 maxEventCount, GYReactor* reactor);

    GYINT32 _Release();

    GYINT32 _AddEvent(GYNetEvent& event);

    GYINT32 _DeleteEvent(GYNetEvent& event);

    GYINT32	_RunOnce(const GYTimeStamp& timeStamp);

private:
    GYVOID _CleanUp();

	static GYINT32 EventMask2EpollEvent(const GYNetEvent& event);
};

#endif // LINUX64
#endif

