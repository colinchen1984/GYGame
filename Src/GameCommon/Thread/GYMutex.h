/////////////////////////////////////////////
// create time: 2011/6/24 0:00
// author:	colin chen
// file name:	GYMutex
// file type:	h
////////////////////////////////////////////

#ifndef __GYMUTEX_H__
#define __GYMUTEX_H__

#include "GYThreadCommon.h"

class GYMutex
{
	GYMutexHandle m_handle;
public:
	GYMutex();

	~GYMutex();

	GYVOID Acquire();

	GYVOID Release();

	GYBOOL TryAcquire();
};

#endif
