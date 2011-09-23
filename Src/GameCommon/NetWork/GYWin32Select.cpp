/////////////////////////////////////////////
// create time: 2011/6/12 16:19
// author:	colin chen
// file name:	GYWin32Select
// file type:	cpp
////////////////////////////////////////////
#ifdef WIN32

#include "GYWin32Select.h"
#include "GYBufferStreamSocket.h"
#include "GYReactor.h"
#include "GYTimeStamp.h"

GYWin32SelectReactor::GYWin32SelectReactor()
{
    _CleanUp();
}

GYWin32SelectReactor::~GYWin32SelectReactor()
{

}

GYVOID GYWin32SelectReactor::_CleanUp()
{
    for (GYINT32 i = 0; i < GY_NET_EVENT_TYPE_COUNT; ++i)
    {
        FD_ZERO(&m_workingfdSet[i]);
        FD_ZERO(&m_masterfdSet[i]);
    }
    m_reactor = GYNULL;
	m_eventArray.CleanUp();
}

GYINT32 GYWin32SelectReactor::_Init(GYINT32 maxEventCount, GYReactor* reactor)
{
    GYINT32 result = INVALID_VALUE;
    do
    {
        if (GYNULL == reactor)
        {
            break;
        }

        if (FD_SETSIZE < maxEventCount)
        {
            break;
        }

		_CleanUp();

		if (0 != m_eventArray.Init(maxEventCount))
		{
			break;
		}
		
        m_reactor = reactor;
        result = 0;
    }
    while(GYFALSE);
    return result;
}

GYINT32 GYWin32SelectReactor::_Release()
{
	m_eventArray.Release();
    return 0;
}

GYINT32 GYWin32SelectReactor::_AddEvent(GYNetEvent& event)
{
	if(GYTRUE != m_eventArray.Add(event))
	{
		return INVALID_VALUE;
	}

    FD_SET(event.m_fd->GetFD(), &m_masterfdSet[event.m_eventType]);
    m_isUpdated = GYTRUE;
    return 0;
}

GYINT32 GYWin32SelectReactor::_DeleteEvent( GYNetEvent& event )
{
	if(GYTRUE != m_eventArray.Delete(event))
	{
		return INVALID_VALUE;
	}
    FD_CLR(event.m_fd->GetFD(), &m_masterfdSet[event.m_eventType]);
    m_isUpdated = GYTRUE;
    return 0;
}

GYINT32 GYWin32SelectReactor::_RunOnce(const GYTimeStamp& timeStamp)
{
    GYINT32 result = INVALID_VALUE;
    do
    {
        for (GYINT32 i = 0; i < GY_NET_EVENT_TYPE_COUNT; ++i)
        {
            m_workingfdSet[i] = m_masterfdSet[i];
        }

		timeval waitTime;
		waitTime.tv_sec = (timeStamp.m_termTime / 1000);
		waitTime.tv_usec =((timeStamp.m_termTime % 1000) * 1000);
		GYINT32 eventCount = m_reactor->GetCurrentEventCount();
		if (eventCount <= 0)
        {
			GYSleep(timeStamp.m_termTime);
			break;
        }

        GYINT32 err = ::select(0, &m_workingfdSet[GY_NET_EVENT_TYPE_READ], &m_workingfdSet[GY_NET_EVENT_TYPE_WRITE], &m_workingfdSet[GY_NET_EVENT_TYPE_EXCEPTION], &waitTime);
		if (SOCKET_ERROR == err)
        {
			GYINT32 t = GetLastNetWorkError();
            break;
        }
        if (err > 0)
        {
            for (GYINT32 i = 0; i < eventCount && err > 0; ++i)
            {
                const GYNetEvent& event = *m_eventArray[i];
                if(FD_ISSET(event.m_fd->GetFD(), &m_workingfdSet[event.m_eventType]))
                {
                    m_reactor->PostEvent(*const_cast<GYNetEvent*>(&event));
					--err;
                }
            }
        }
        result = 0;
    }
    while (GYFALSE);
    return result;
}
#endif // WIN32

