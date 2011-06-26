/////////////////////////////////////////////
// create time: 2011/6/24 0:13
// author:	colin chen
// file name:	GYThread
// file type:	h
////////////////////////////////////////////

#ifndef __GYTHREAD_H__
#define __GYTHREAD_H__

#include "GYThreadCommon.h"
#include "GYFastMutex.h"
#include "GYGuard.h"
#include "GYThreadTask.h"
#include "GYCondition.h"

class GYThread
{
#ifdef WIN32
	friend  GYUINT32 __stdcall thread_proc(GYVOID* param);
#endif 
#ifdef LINUX64
	friend GYVOID* thread_proc(GYVOID* param);
#endif
	friend class GYThreadPool;
	friend GYINT32 InitThread();
	friend GYBOOL WhenSuspend(GYThread& thread);
	friend GYBOOL WhenRunning(GYThread& thread);
	friend GYBOOL WhenTeminating(GYThread& thread);
	GY_THREAD_STATUS	m_threadStatus;
	GYFastMutex			m_threadMutex;
	GYThreadHandle		m_threadHandle;
	GYThreadTask		m_taskInfo;

#ifdef LINUX64
	GYThreadCondition m_condition;
	static pthread_attr_t thread_attr;
#endif

public:
	GYINT32 InitThread();

	GYINT32 SetTask(const GYThreadTask& task);
	
	const GYThreadTask& GetTask();

	GY_THREAD_STATUS GetStatus();

	GYINT32 Terminate();

	GYINT32 Join();

	GYINT32 Resume();
	
	GYINT32 Suspend();
private:
	GYVOID _SetStatus(const GY_THREAD_STATUS& state);

	GYINT32 _Suspend();

	GYINT32 _Resume();

	GYThread();

	~GYThread();
};
#endif
