/////////////////////////////////////////////
// create time: 2011/6/26 10:41
// author:	colin chen
// file name:	GYThreadPool
// file type:	cpp
////////////////////////////////////////////
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
		ZeroMemory(m_threads, sizeof(GYThread*) * maxThreadCount);
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
		if (m_threads[i]->GetStatus()==GY_THREAD_STATUS_SUSPEND)
		{
			m_threads[i]->Resume();
		}
		m_threads[i]->Terminate();
		m_threads[i]->Join();
		delete m_threads[i];
		m_threads[i] = GYNULL;
	}
	return 0;
}

GYINT32 GYThreadPool::SuspendThread( GYINT32 threadIndex )
{
	if (threadIndex >= m_currentThreadCount)
	{
		return INVALID_VALUE;
	}
	if (GYNULL == m_threads[threadIndex])
	{
		return INVALID_VALUE;
	}

	return m_threads[threadIndex]->Suspend();
}

GYINT32 GYThreadPool::ResumeThread( GYINT32 threadIndex )
{
	if (threadIndex >= m_currentThreadCount)
	{
		return INVALID_VALUE;
	}
	if (GYNULL == m_threads[threadIndex])
	{
		return INVALID_VALUE;
	}

	return m_threads[threadIndex]->Resume();
}

GYINT32 GYThreadPool::TerminateThread( GYINT32 threadIndex )
{
	if (threadIndex >= m_currentThreadCount)
	{
		return INVALID_VALUE;
	}
	if (GYNULL == m_threads[threadIndex])
	{
		return INVALID_VALUE;
	}

	if (m_threads[threadIndex]->GetStatus()==GY_THREAD_STATUS_SUSPEND)
	{
		m_threads[threadIndex]->Resume();
	}
	m_threads[threadIndex]->Terminate();
	m_threads[threadIndex]->Join();
	delete m_threads[threadIndex];
	if (threadIndex != --m_currentThreadCount)
	{
		m_threads[threadIndex] = m_threads[m_currentThreadCount];
		m_threads[m_currentThreadCount] = GYNULL;
	}
	else
	{
		m_threads[threadIndex] = GYNULL;
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
	th->InitThread();
	th->SetTask(task);
	th->Resume();
	return 0;
}