/////////////////////////////////////////////
// create time:2011/6/10 10:57
// author:	colin chen
// file name:	GYCommonDefine
// file type:	h
////////////////////////////////////////////
#include "GYCommonDefine.h"

#ifdef WIN32
#include <Windows.h>
#endif
#ifdef LINUX64
#include <sys/select.h>
#include <sys/time.h>
#endif

GYVOID GYSleep(GYINT32 ms)
{
#ifdef WIN32
	Sleep(ms);
#endif

#ifdef LINUX64
	static timeval waitTime;
	waitTime.tv_sec = (ms / 1000);
	waitTime.tv_usec =((ms % 1000) * 1000);
	::select(0,GYNULL, GYNULL, GYNULL, &waitTime);
#endif
}
