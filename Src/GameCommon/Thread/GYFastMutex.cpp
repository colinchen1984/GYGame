/////////////////////////////////////////////
// create time: 2011/6/24 0:18
// author:	colin chen
// file name:	GYFastMutex
// file type:	cpp
////////////////////////////////////////////
#include "GYFastMutex.h"

#ifdef WIN32
GYFastMutex::GYFastMutex()
{
	InitializeCriticalSection(&m_handler);
}

GYFastMutex::~GYFastMutex()
{
	DeleteCriticalSection(&m_handler);
}

GYVOID GYFastMutex::Acquire()
{
	EnterCriticalSection(&m_handler);
}

GYVOID GYFastMutex::Release()
{
	LeaveCriticalSection(&m_handler);
}

GYBOOL GYFastMutex::TryAcquire()
{
	return 1 == TryEnterCriticalSection(&m_handler) ? GYTRUE : GYFALSE;
}
#endif // WIN32
