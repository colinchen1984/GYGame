/////////////////////////////////////////////
// create time: 2011/6/23 22:17
// author:	colin chen
// file name:	GYThreadCommon
// file type:	cpp
////////////////////////////////////////////
#include "GYThreadCommon.h"
#include "GYThread.h"
#include "GYThreadTask.h"

#ifdef WIN32
GYUINT32 __stdcall thread_proc(GYVOID* param)
#endif
#ifdef LINUX64
GYVOID* thread_proc(GYVOID * param)
#endif
{
	GY_THREAD_STATUS state = GY_THREAD_STATUS_INVALID;
	GYThread& t = *static_cast<GYThread*>(param);
	const GYThreadTask& task = t.GetTask();
	task.m_threadFunction(task.param);
#ifdef WIN32
	return 0;
#endif
#ifdef LINUX64
	return GYNULL;
#endif
}

GYINT32 InitThread()
{
#ifdef LINUX64
	if(0 != pthread_attr_setdetachstate(&GYThread::thread_attr, PTHREAD_CREATE_JOINABLE))
	{
		return INVALID_VALUE;
	}
#endif // LINUX64
	return 0;
}
