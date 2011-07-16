/////////////////////////////////////////////
// create time: 2011/6/12 14:30
// author:	colin chen
// file name:	GYReactor
// file type:	cpp
////////////////////////////////////////////
#include "GYReactor.h"
#include "GYEvent.h"
#include "GYSocket.h"

GYReactor::GYReactor()
{
    _CleanUp();
}

GYReactor::~GYReactor()
{

}

GYVOID GYReactor::_CleanUp()
{
    m_maxEventCount			= 0;
    m_currentEventCount		= 0;
	m_eventList.CleanUp();
}

GYINT32 GYReactor::Init(GYINT32 maxEventCount)
{
    _CleanUp();
    GYINT32 result = INVALID_VALUE;
    do
    {
        if (0 != m_reactor._Init(maxEventCount, this))
        {
            break;
        }
        m_maxEventCount = maxEventCount;
        result = 0;
    }
    while (GYFALSE);
    return result;
}

GYINT32 GYReactor::Release()
{
    GYINT32 result = INVALID_VALUE;
    do
    {
        if (0 != m_reactor._Release())
        {
            break;
        }
        _CleanUp();
        result = 0;
    }
    while (GYFALSE);
    return result;
}

GYINT32 GYReactor::AddEvent(GYNetEvent& event)
{
    GYINT32 result = INVALID_VALUE;
    do
    {
        if (m_currentEventCount >= m_maxEventCount)
        {
            break;
        }

        if (GYTRUE == event.m_busy)
        {
            break;
        }
        if (0 != m_reactor._AddEvent(event))
        {
            break;
        }
        ++m_currentEventCount;
        event.m_busy = GYTRUE;
        event.m_fd->SetRegisted(GYTRUE);
        result = 0;
    }
    while(GYFALSE);
    return result;
}

GYINT32 GYReactor::DeleteEvent(GYNetEvent& event)
{
    GYINT32 result = INVALID_VALUE;
    do
    {
        if (GYTRUE != event.m_busy)
        {
            break;
        }
        if (0 != m_reactor._DeleteEvent(event))
        {
            break;
        }
        event.m_busy = GYFALSE;
        --m_currentEventCount;
        event.m_fd->SetRegisted(GYFALSE);
        result = 0;
    }
    while(GYFALSE);
    return result;
}

GYINT32 GYReactor::RunOnce(const GYTimeStamp& timeStamp)
{
    m_eventList.CleanUp();
    GYINT32 result = m_reactor._RunOnce(timeStamp);
    while (0 != m_eventList.GetItemCount())
    {
		GYNetEvent& event = *m_eventList.PickUpFirstItem();
		event.m_eventHandler(event);
    }
    return result;
}

GYINT32 GYReactor::GetCurrentEventCount()
{
    return m_currentEventCount;
}

GYINT32 GYReactor::GetMaxEventCount()
{
    return m_maxEventCount;
}

GYVOID GYReactor::PostEvent( GYNetEvent& event )
{
	m_eventList.Add(event);
}

