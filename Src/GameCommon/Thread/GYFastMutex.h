/////////////////////////////////////////////
// create time: 2011/6/24 0:15
// author:	colin chen
// file name:	GYFastMutex
// file type:	h
////////////////////////////////////////////

#ifndef __GYFASTMUTEX_H__
#define __GYFASTMUTEX_H__

#include "GYThreadCommon.h"
#ifdef WIN32
class GYFastMutex 
{
	GYFastMutexHandle m_handler;
public:
	GYFastMutex();

	~GYFastMutex();

	 GYVOID Acquire();

	 GYVOID Release();

	 GYBOOL TryAcquire();
};
#endif // WIN32

#ifdef LINUX64
#include "GYMutex.h"
#define GYFastMutex GYMutex  
#endif // LINUX64

#endif
