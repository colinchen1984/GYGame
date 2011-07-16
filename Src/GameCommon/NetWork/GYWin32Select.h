/////////////////////////////////////////////
// create time: 2011/6/12 16:19
// author:	colin chen
// file name:	GYWin32Select
// file type:	h
////////////////////////////////////////////

#ifndef __GYWIN32SELECT_H__
#define __GYWIN32SELECT_H__
#ifdef WIN32

#include "GYCommonDefine.h"
#include "GYNetWorkCommonDefine.h"
#include "GYEvent.h"
#include "GYArray.h"
class GYReactor;
class GYTimeStamp;
class GYWin32SelectReactor
{
    friend class GYReactor;
    fd_set						m_workingfdSet[GY_NET_EVENT_TYPE_COUNT];
    fd_set						m_masterfdSet[GY_NET_EVENT_TYPE_COUNT];
    GYReactor*					m_reactor;
    GYBOOL						m_isUpdated;
	GYPointArray<GYNetEvent>	m_eventArray;
private:
    GYWin32SelectReactor();
    ~GYWin32SelectReactor();

    GYINT32 _Init(GYINT32 maxEventCount, GYReactor* reactor);

    GYINT32 _Release();

    GYINT32 _AddEvent(GYNetEvent& event);

    GYINT32 _DeleteEvent(GYNetEvent& event);

    GYINT32	_RunOnce(const GYTimeStamp& timeStamp);

private:
    GYVOID _CleanUp();
};
#endif // WIN32

#endif

