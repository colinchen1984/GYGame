/////////////////////////////////////////////
// create time: 2011/6/26 10:40
// author:	colin chen
// file name:	GYThreadPool
// file type:	h
////////////////////////////////////////////

#ifndef __GYTHREADPOOL_H__
#define __GYTHREADPOOL_H__
#include "GYThreadTask.h"
class GYThread;
struct GYThreadTask;
class GYThreadPool
{
	GYThread**	m_threads;
	GYINT32		m_maxThreadCount;
	GYINT32		m_currentThreadCount;
	//fast_mutex mutex;

public:
	GYINT32 Init(GYINT32 maxThreadCount);
	
	GYINT32 Release();

	GYINT32 AddTask(const GYThreadTask& task);

	static GYThreadPool& Singleton()
	{
		static GYThreadPool p;
		return p;
	}

private:
	GYThreadPool();

	~GYThreadPool();

	GYBOOL RightThreadIndex(GYINT32 threadIndex);
};
#define ThreadPool GYThreadPool::Singleton()

#endif
