/////////////////////////////////////////////
// create time: 2011/7/25 12:51
// author:	colin chen
// file name:	GYTimeController 
// file type:	cpp
////////////////////////////////////////////

#include "GYTimeController.h"
#include <time.h>
#ifdef WIN32
#include <Windows.h>
#endif // WIN32

GYUINT32 GYTimeController::GetProcessRunTime()
{
	GYUINT32 result = INVALID_VALUE;
#ifdef WIN32
	result = m_currentTime - m_startTime;
#endif

#ifdef LINUX64
	gettimeofday(&m_currentTime, GYNULL);
	result = 1000 * (m_currentTime.tv_sec - m_startTime.tv_sec) + (m_currentTime.tv_usec - m_startTime.tv_usec) / 1000;
#endif
	return result;
}

GYUINT64 GYTimeController::GetCpuTime()
{
	GYUINT64 result = INVALID_VALUE;
#ifdef WIN32
	result = GetTickCount();
#endif

#ifdef LINUX64
	timeval current;
	gettimeofday(&current, GYNULL);
	result = 1000 * (current.tv_sec) + (current.tv_usec) / 1000;
#endif
	return result;
}

GYUINT32	GYTimeController::GetNowTime()
{
	m_currentTimeMS = time(GYNULL);	
	m_currentTimeMS += m_timeZone;
	return m_currentTimeMS;
}

GYINT32 GYTimeController::Init( GYINT32 timeZone )
{
	m_timeZone = SECOND_PER_HOUR * timeZone;
#ifdef WIN32
	m_startTime = GetTickCount();
#endif

#ifdef LINUX64
	gettimeofday(&m_startTime, GYNULL);
#endif // LINUX64
	m_currentTime = m_startTime;
	return 0;
}

