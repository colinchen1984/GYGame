/////////////////////////////////////////////
// create time: 2011/6/23 22:17
// author:	colin chen
// file name:	GYThreadCommon
// file type:	cpp
////////////////////////////////////////////
#include "GYThreadCommon.h"
#include "GYThread.h"
#include "GYThreadTask.h"
GYBOOL (*ThreadSubProcess[GY_THREAD_STATUS_COUNT])(GYThread& thread);
GYBOOL WhenSuspend(GYThread& thread)
{
	thread._Suspend();
	return GYTRUE;
}

GYBOOL WhenRunning(GYThread& thread)
{
	//guard<fast_mutex> g(t->mutex);
	const GYThreadTask& task = thread.GetTask();
	task.m_threadFunction(task.param);
	return GYTRUE;
}

GYBOOL WhenTeminating(GYThread& thread)
{
	const GYThreadTask& task = thread.GetTask();
	if (task.m_threadFinishCallBack)
	{
		task.m_threadFinishCallBack(task.param);
	}
	thread._SetStatus(GY_THREAD_STATUS_TERMINATED);
	return GYFALSE;
}

#ifdef WIN32
GYUINT32 __stdcall thread_proc(GYVOID* param)
#endif
#ifdef LINUX64
GYVOID* thread_proc(GYVOID * param)
#endif
{
	GY_THREAD_STATUS state = GY_THREAD_STATUS_INVALID;
	GYThread& t = *static_cast<GYThread*>(param);
	while (GYTRUE)
	{
		state = t.GetStatus();
		GYBOOL result = ((*ThreadSubProcess[state])(t));
		if (!result)
		{
			break;
		}

	}
#ifdef WIN32
	return 0;
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
	memset(ThreadSubProcess, 0, sizeof(ThreadSubProcess));
	ThreadSubProcess[GY_THREAD_STATUS_SUSPEND] = WhenSuspend;
	ThreadSubProcess[GY_THREAD_STATUS_RUNNING] = WhenRunning;
	ThreadSubProcess[GY_THREAD_STATUS_TERMINATE] = WhenTeminating;
	
	return 0;
}
