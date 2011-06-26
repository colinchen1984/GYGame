/////////////////////////////////////////////
// create time: 2011/6/26 8:33
// author:	colin chen
// file name:	GYThread
// file type:	cpp
////////////////////////////////////////////
#include "GYThread.h"

GYThread::GYThread()
{
}

GYThread::~GYThread()
{
}

GYINT32 GYThread::Join()
{
	GYINT32 result = INVALID_VALUE;
#ifdef WIN32
	result = WaitForSingleObject(m_threadHandle, 0xFFFFFFFF);
#endif
#ifdef LINUX64
	result = pthread_join(m_threadHandle, NULL);
#endif
	return result;
}

GYINT32 GYThread::InitThread(const GYThreadTask& task)
{
	GYINT32 result = INVALID_VALUE;
	m_taskInfo = task;
#ifdef WIN32
	m_threadHandle = reinterpret_cast<GYThreadHandle>(_beginthreadex( NULL, 0, thread_proc, static_cast<GYVOID*>(this), 0, 0));
	result = 0 == m_threadHandle ? INVALID_VALUE : 0;
#endif
#ifdef LINUX64
	result = pthread_create(&m_threadHandle, GYNULL, thread_proc, static_cast<GYVOID*>(this));
	result = 0 == result ? 0 : INVALID_VALUE;
#endif
	if (INVALID_VALUE == result)
	{
		return result;
	}
	return result;
}

const GYThreadTask& GYThread::GetTask()
{
	return m_taskInfo;
}