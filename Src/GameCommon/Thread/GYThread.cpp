/////////////////////////////////////////////
// create time: 2011/6/26 8:33
// author:	colin chen
// file name:	GYThread
// file type:	cpp
////////////////////////////////////////////
#include "GYThread.h"

GYThread::GYThread()
{
	m_threadStatus = GY_THREAD_STATUS_NO_THREAD;
}

GYThread::~GYThread()
{
}

GYINT32 GYThread::_Suspend()
{
#ifdef WIN32
	::SuspendThread(m_threadHandle);
#endif
#ifdef LINUX64
	m_condition.Acquire();
#endif
	return 0;
}

GYINT32 GYThread::_Resume()
{
#ifdef WIN32
	::ResumeThread(m_threadHandle);
#endif
#ifdef LINUX64
	m_condition.Signal();
#endif
	m_threadStatus = GY_THREAD_STATUS_RUNNING;
	return 0;
}

GYINT32 GYThread::Join()
{
	GYINT32 result = INVALID_VALUE;
	{
		GY_THREAD_STATUS status = GetStatus();
		if (GY_THREAD_STATUS_TERMINATED != status && GY_THREAD_STATUS_TERMINATE != status)
		{
			return result;
		}
	}
	
#ifdef WIN32
	result = WaitForSingleObject(m_threadHandle, 0xFFFFFFFF);
#endif
#ifdef LINUX64
	result = pthread_join(m_threadHandle, NULL);
#endif
	return result;
}




GYINT32 GYThread::InitThread()
{
	if(GY_THREAD_STATUS_NO_THREAD != m_threadStatus)
	{
		return INVALID_VALUE;
	}
	m_threadStatus = GY_THREAD_STATUS_SUSPEND;
	GYINT32 result = INVALID_VALUE;
#ifdef WIN32
	m_threadHandle = reinterpret_cast<GYThreadHandle>(_beginthreadex( NULL, 0, thread_proc, static_cast<GYVOID*>(this), 0, 0));
	result = 0 == m_threadHandle ? INVALID_VALUE : 0;
#endif
#ifdef LINUX64
	result = pthread_create(m_threadHandle, &thread_attr, thread_proc, static_cast<GYVOID*>(this));
	result = 0 == result ? 0 : INVALID_VALUE;
#endif
	if (INVALID_VALUE == result)
	{
		return result;
	}

	return result;
}

GYINT32 GYThread::SetTask(const GYThreadTask& task )
{
	GYGuard<GYFastMutex> g(m_threadMutex);
	m_taskInfo = task;
	return 0;
}

GY_THREAD_STATUS GYThread::GetStatus()
{
	GYGuard<GYFastMutex> g(m_threadMutex);
	return m_threadStatus;
}

const GYThreadTask& GYThread::GetTask()
{
	return m_taskInfo;
}

GYVOID GYThread::_SetStatus( const GY_THREAD_STATUS& state )
{
	GYGuard<GYFastMutex> g(m_threadMutex);
	m_threadStatus = state;
}

GYINT32 GYThread::Resume()
{
	_SetStatus(GY_THREAD_STATUS_RUNNING);
	_Resume();
	return 0;
}

GYINT32 GYThread::Suspend()
{
	_SetStatus(GY_THREAD_STATUS_SUSPEND);
	return 0;
}

GYINT32 GYThread::Terminate()
{
	_SetStatus(GY_THREAD_STATUS_TERMINATE);
	return 0;
}