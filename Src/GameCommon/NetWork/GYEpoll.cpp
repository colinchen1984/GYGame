/////////////////////////////////////////////
// create time: 2011/6/12 22:31
// author:	colin chen
// file name:	GYEpoll
// file type:	cpp
////////////////////////////////////////////
#ifdef LINUX64
#include "GYEpoll.h"
#include "GYEvent.h"
#include <sys/epoll.h>
#include <memory.h>

GYEpollReactor::GYEpollReactor()
{
	_CleanUp();
}

GYEpollReactor::~GYEpollReactor()
{

}

GYVOID GYEpollReactor::_CleanUp()
{
	m_reactor = GYNULL;
	m_nFdForEpoll = INVALID_SOCKET;
	m_pEvForWait = GYNULL;
}

GYINT32 GYEpollReactor::_Init(GYINT32 maxEventCount, GYReactor* reactor)
{
	GYINT32 result = INVALID_VALUE;
	do
	{
		if (GYNULL == reactor)
		{
			break;
		}

		m_nFdForEpoll = epoll_create(maxEventCount);
		if(INVALID_SOCKET == m_nFdForEpoll)
		{
			break;
		}

		m_pEvForWait = GYNew epoll_event[maxEventCount];
		if (GYNULL == m_pEvForWait)
		{
			::close(m_nFdForEpoll);
			break;
		}
		
		GYZeroMem(m_pEvForWait, sizeof(epoll_event) * maxEventCount);
		result = 0;
	}while(GYFALSE);
	return result;
}

GYINT32 GYEpollReactor::_Release()
{
	GYDelete[] m_pEvForWait;
	::close(m_nFdForEpoll);
	_CleanUp();
	return 0;
}

static GYINT32 EventMask2EpollEvent(GYNetEventType eventType)
{
	GYINT32 result = 0;
	switch(eventType)
	{
	case GYNetEventTypeRead:
		{
			result |= EPOLLIN;
		}
		break;
	case GYNetEventTypeWrite:
		{
			result |= EPOLLOUT;
		}
		break;
	case GYNetEventTypeException:
		{
			result |= EPOLLHUP | EPOLLERR | EPOLLPRI;
		}
		break;
	}
	return result;
}

GYINT32 GYEpollReactor::_AddEvent(GYNetEvent& event)
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if(GYTRUE == event.m_fd->IsRegisted())
		{
			break;
		}
		GYINT32 currentEventCount = m_reactor->GetCurrentEventCount();
		if (currentEventCount >= m_reactor->GetMaxEventCount())
		{
			break;
		}
		epoll_event operation;
		GYZeroMem(&operation, sizeof(operation));
		operation.events |= EPOLLET；
		operation.events |= EventMask2EpollEvent(event.m_eventType);
		operation.data.ptr = static<GYVOID*>(&event);

		if(INVALID_VALUE == epoll_ctl(m_nFdForEpoll, EPOLL_CTL_ADD, event.m_fd->GetFD(), &operation))
		{
			break;
		}
		result = 0;
	} while (GYFALSE);
	return result;	
}

GYINT32 GYEpollReactor::_DeleteEvent( GYNetEvent& event )
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if(GYTRUE != event.m_fd->IsRegisted())
		{
			break;
		}

		epoll_event operation;
		GYZeroMem(&operation, sizeof(operation));
		operation.events |= EPOLLET；
		operation.events |= EventMask2EpollEvent(event.m_eventType);
		operation.data.ptr = static<GYVOID*>(&event);

		if(INVALID_VALUE == epoll_ctl(m_nFdForEpoll, EPOLL_CTL_DEL, event.m_fd->GetFD(), &operation))
		{
			break;
		}
		result = 0;
	} while (GYFALSE);
	return result;	
}

GYINT32 GYEpollReactor::_RunOnce()
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		GYINT32 currentEventCount = m_reactor->GetCurrentEventCount();
		if (currentEventCount <= 0)
		{
			//TODO: 函数参数中添加时间，然后调用select来sleep
			result = 0;
			break;
		}
		
		GYINT32 eventCount = epoll_wait(m_nFdForEpoll, m_pEvForWait, currentEventCount, NULL);
		if (INVALID_VALUE == eventCount)
		{
			if (EINTR == GetLastNetWorkError())
			{
				result = 0;
			}
			break;
		}

		if (0 == eventCount)
		{
			result = 0;
			break;
		}
		
		for (GYINT32 i = 0; i < eventCount; ++i)
		{
			epoll_event& eventData = m_pEvForWait[i];
			const GYNetEvent& event = *static_cast<GYNetEvent*>(eventData.data.ptr);.
			if(eventData.events & EPOLLIN && GYNetEventTypeRead == event.m_eventType)
			{
				m_reactor->PostEvent(event);
			}
			if (eventData.events & EPOLLOUT && GYNetEventTypeWrite == event.m_eventType)
			{
				m_reactor->PostEvent(event);
			}
			if ((eventData.events & EPOLLHUP || eventData.events & EPOLLERR || eventData.events & EPOLLPRI) && GYNetEventTypeException == event.m_eventType)
			{
				m_reactor->PostEvent(event);
			}		
		}
				
		result = 0;
	} while (GYFALSE);
	return result;
}

#endif
