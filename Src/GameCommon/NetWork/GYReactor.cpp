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
    m_event					= GYNULL;
    m_eventList				= GYNULL;
    m_maxEventCount			= 0;
    m_currentEventCount		= 0;
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
        m_event = GYNew GYNetEvent* [maxEventCount];
        if (GYNULL == m_event)
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
        if (GYNULL != m_event)
        {
            GYDelete[] m_event;
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
        event.m_reactorIndex = m_currentEventCount++;
        m_event[event.m_reactorIndex] = &event;
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
        if (event.m_reactorIndex >= m_currentEventCount)
        {
            break;
        }

        if (event.m_reactorIndex < 0)
        {
            break;
        }
        if (0 != m_reactor._DeleteEvent(event))
        {
            break;
        }
        event.m_busy = GYFALSE;
        --m_currentEventCount;
        if (event.m_reactorIndex != m_currentEventCount)
        {
            m_event[event.m_reactorIndex] = m_event[m_currentEventCount];
            m_event[event.m_reactorIndex]->m_reactorIndex = event.m_reactorIndex;
        }
        m_event[m_currentEventCount] = GYNULL;
        event.m_fd->SetRegisted(GYFALSE);
        result = 0;
    }
    while(GYFALSE);
    return result;
}

GYINT32 GYReactor::RunOnce()
{
    m_eventList = GYNULL;
    GYINT32 result = m_reactor._RunOnce();
    while (GYNULL != m_eventList)
    {
        m_eventList->m_eventHandler(*m_eventList);
        m_eventList = m_eventList->m_nexEvent;
    }
    return result;
}

GYINT32 GYReactor::GetCurrentEventCount()
{
    return m_currentEventCount;
}

const GYNetEvent** GYReactor::GetEventSet()
{
    return (const_cast<const GYNetEvent**>(m_event));
}

GYINT32 GYReactor::GetMaxEventCount()
{
    return m_maxEventCount;
}

GYVOID GYReactor::PostEvent( GYNetEvent& event )
{
    event.m_nexEvent = m_eventList;
    m_eventList = &event;
}

