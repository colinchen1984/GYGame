/////////////////////////////////////////////
// create time: 2011/6/12 22:31
// author:	colin chen
// file name:	GYEpoll
// file type:	cpp
////////////////////////////////////////////
#ifdef LINUX64
#include "GYEpoll.h"
#include <sys/epoll.h>

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
	m_EvForOpt = GYNULL;
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
			close(m_nFdForEpoll);
			break
		}
		
		GYZeroMem(m_pEvForWait, sizeof(epoll_event) * maxEventCount);
		result = 0;
	}while(GYFALSE);
	return result;
}
#endif
