/////////////////////////////////////////////
// create time: 2011/6/26 10:41
// author:	colin chen
// file name:	GYThreadPool
// file type:	cpp
////////////////////////////////////////////
#include "GYThreadTask.h"
#include "GYThreadPool.h"
#include "GYThread.h"
GYThreadPool::GYThreadPool()
{
	m_threads = GYNULL;
	m_maxThreadCount = 0;
	m_currentThreadCount= 0;
}

GYThreadPool::~GYThreadPool()
{

}

GYINT32 GYThreadPool::Init(GYINT32 maxThreadCount)
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if(GYNULL != m_threads)
		{
			break;
		}
		m_threads = new GYThread*[maxThreadCount];
		GYZeroMem(m_threads, sizeof(GYThread*) * maxThreadCount);
		m_maxThreadCount = maxThreadCount;
		m_currentThreadCount = 0;
		result = 0;
	} while (GYFALSE);
	return result;
}

GYINT32 GYThreadPool::Release()
{
	for (GYINT32 i = 0; i < m_currentThreadCount; ++i)
	{
		m_threads[i]->Join();
		delete m_threads[i];
		m_threads[i] = GYNULL;
	}
	return 0;
}

GYINT32 GYThreadPool::AddTask( const GYThreadTask& task )
{
	if (m_currentThreadCount >= m_maxThreadCount)
	{
		return INVALID_VALUE;
	}
	
	GYThread*& th = m_threads[m_currentThreadCount++];
	th = new GYThread;
	th->InitThread(task);
	return 0;
}