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
	return 0;
}
