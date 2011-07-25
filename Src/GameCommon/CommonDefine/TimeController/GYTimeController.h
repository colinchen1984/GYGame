/////////////////////////////////////////////
// create time: 2011/7/25 12:51
// author:	colin chen
// file name:	GYTimeController 
// file type:	h
////////////////////////////////////////////

#ifndef __GYTIMECONTROLLER_H__
#define __GYTIMECONTROLLER_H__
#include "GYCommonDefine.h"

#ifdef LINUX64
#include <sys/time.h>
#endif // LINUX64

const GYINT32 SECOND_PER_HOUR = 3600;

class GYTimeController
{
#ifdef WIN32
	GYUINT32	m_startTime;
	GYUINT32	m_currentTime;
#endif 

#ifdef LINUX64
	timeval		m_startTime;
	timeval		m_currentTime;
#endif

	GYUINT32	m_currentTimeMS;
	GYUINT32	m_startTimeMS;
	GYINT32		m_timeZone;
public:
	GYUINT32	GetProcessRunTime();		//取得进程启动后运行了多长时间 单位毫秒
	
	GYUINT32	GetNowTime();				//取得当前 GMT时间， 如果设置了时区，则会加上时区 单位秒

	static GYUINT32	GetCupTime();				//取得当前进程获得了多少CUP时间单位毫秒

	GYINT32		Init(GYINT32 timeZone);		
};

#endif

