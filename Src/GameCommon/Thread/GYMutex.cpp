/////////////////////////////////////////////
// create time: 2011/6/23 23:59
// author:	colin chen
// file name:	GYMutex
// file type:	cpp
////////////////////////////////////////////
#include "GYMutex.h"

GYMutex::GYMutex()
{
#ifdef WIN32
	m_handle = CreateMutex(GYNULL, GYFALSE, GYNULL);
#endif

#if LINUX64
	pthread_mutex_init(&m_handle, GYNULL);
#endif
}

GYMutex::~GYMutex()
{
#ifdef WIN32
	CloseHandle(m_handle);
#endif

#if LINUX64
	pthread_mutex_destroy(&m_handle);
#endif
}

GYVOID GYMutex::Acquire()
{
#ifdef WIN32
	WaitForSingleObject(m_handle, INFINITE);
#endif

#if LINUX64
	pthread_mutex_lock(&m_handle);
#endif
}

GYVOID GYMutex::Release()
{
#ifdef WIN32
	ReleaseMutex(m_handle);
#endif

#if LINUX64
	pthread_mutex_unlock(&m_handle);
#endif
}

GYBOOL GYMutex::TryAcquire()
{
#ifdef WIN32
	return WAIT_TIMEOUT == WaitForSingleObject(m_handle, 0) ? GYTRUE : GYFALSE;
#endif

#if LINUX64
	return 0 == pthread_mutex_trylock(&m_handle) ? GYTRUE : GYFALSE;
#endif
}

