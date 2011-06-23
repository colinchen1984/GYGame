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


//using declare

//foward declare
/*enum ThraedStatus
{
	NOTHREAD = -1,
	SUSPEND,
	RUNNING,
	TERMINATE,
	TERMINATED,
};

class GYThread : public GYManagerHelper, private GYNoncopyable
{
#ifdef WIN32
	friend static GYUINT __stdcall thread_proc(GYVOID* param);
#else
	friend GYVOID* thread_proc(GYVOID* param);
#endif
	friend GYVOID GYSuspendThread(GYThread& th);
	friend GYVOID GYResumeThread(GYThread& th);
	friend GYINT  GYJoinThread(GYThread& th);

	ThraedStatus		m_threadStatus;
	GYFastMutex			m_threadMutex;
	GYThreadHandle		m_threadHandle;
	ThreadTaskInfo		m_taskInfo;

#ifndef WIN32
	GYThreadCondition cond;
#endif

public:
	GYThread()
	{
		m_threadStatus = NOTHREAD;
		m_taskInfo.p = NULL;
		m_taskInfo.mission_index = -1;
	};

	~GYThread()
	{
	};

	GYINT
	InitThread();

	GYINT
	SetTask(const ThreadTaskInfo& task_info);

	ThraedStatus
	GetStatus();

	GYVOID 
	SetStatus(ThraedStatus state);

	GYINT
	Suspend();

	GYINT
	Resume();

	GYINT
	Terminate();

	GYINT
	Join();
};
*/

#endif
