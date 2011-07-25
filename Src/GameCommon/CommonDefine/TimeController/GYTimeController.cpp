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

#ifdef LINUX64
#include <sys/time.h>
#endif // LINUX64

GYUINT32 GYTimeController::GetCupTime()
{
	GYUINT32 t= clock();
	return t;
}

GYUINT32	GYTimeController::GetNowTime()
{
	m_currentTime = time(GYNULL);	
	m_currentTime += m_timeZone;
	return m_currentTime;
}

GYINT32 GYTimeController::Init( GYINT32 timeZone )
{
	m_timeZone = SECOND_PER_HOUR * timeZone;
#ifdef WIN32
	m_startTime = GetTickCount();
#endif

#ifdef LINUX64
	gettimeofday(&m_startTime);
#endif // LINUX64
	m_currentTime = m_startTime;
	return 0;
}
