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
	GYFastMutex			m_threadMutex;
	GYThreadHandle		m_threadHandle;
	GYThreadTask		m_taskInfo;

public:
	GYINT32 InitThread(const GYThreadTask& task);
	
	GYINT32 Join();
	
	const GYThreadTask& GetTask();
private:
	GYThread();

	~GYThread();
};
#endif
