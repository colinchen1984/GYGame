/////////////////////////////////////////////
// create time: 2011/6/12 16:19
// author:	colin chen
// file name:	GYWin32Select
// file type:	cpp
////////////////////////////////////////////
#ifdef WIN32

#include "GYWin32Select.h"
#include "GYBufferStreamSock.h"
#include "GYReactor.h"
GYWin32SelectReactor::GYWin32SelectReactor()
{
	_CleanUp();
}

GYWin32SelectReactor::~GYWin32SelectReactor()
{

}

GYVOID GYWin32SelectReactor::_CleanUp()
{
	for (GYINT32 i = 0; i < GYNetEventTypeCount; ++i)
	{
		FD_ZERO(&m_workingfdSet[i]);
		FD_ZERO(&m_masterfdSet[i]);
	}
	m_reactor = GYNULL;
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
		m_reactor = reactor;
		result = 0;
	}while(GYFALSE);
	return result;	
}

GYINT32 GYWin32SelectReactor::_Release()
{
	return 0;
}

GYINT32 GYWin32SelectReactor::_AddEvent(GYNetEvent& event)
{
	FD_SET(event.m_fd->GetFD(), &m_masterfdSet[event.m_eventType]);
	m_isUpdated = GYTRUE;
	return 0;
}

GYINT32 GYWin32SelectReactor::_DeleteEvent( GYNetEvent& event )
{
	FD_CLR(event.m_fd->GetFD(), &m_masterfdSet[event.m_eventType]);
	m_isUpdated = GYTRUE;
	return 0;
}

GYINT32 GYWin32SelectReactor::_RunOnce()
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if (GYTRUE == m_isUpdated)
		{
			for (GYINT32 i = 0; i < GYNetEventTypeCount; ++i)
			{
				m_workingfdSet[i] = m_masterfdSet[i];
			}
			m_isUpdated= GYFALSE;
		}
		
		GYINT32 eventCount = m_reactor->GetCurrentEventCount();
		const GYNetEvent** const eventSet = m_reactor->GetEventSet();
		if (eventCount <= 0)
		{
			//TODO: 函数参数中添加时间，然后调用select来sleep
			result = 0;
			break;
		}

		GYINT32 err = ::select(0, &m_workingfdSet[GYNetEventTypeRead], &m_workingfdSet[GYNetEventTypeWrite], &m_workingfdSet[GYNetEventTypeException], GYNULL);
		if (SOCKET_ERROR == err)
		{
			break;
		}
		if (err > 0)
		{
			for (GYINT32 i = 0; i < eventCount; ++i)
			{
				const GYNetEvent& event = *eventSet[i];
				if(FD_ISSET(event.m_fd->GetFD(), &m_workingfdSet[event.m_eventType]))
				{
					m_reactor->PostEvent(*const_cast<GYNetEvent*>(&event));
				}
			}
		}
		result = 0;
	} while (GYFALSE);
	return result;
}
#endif // WIN32
