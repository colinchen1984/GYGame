/////////////////////////////////////////////
// create time: 2011/6/26 8:17
// author:	colin chen
// file name:	GYCondition
// file type:	h
////////////////////////////////////////////

#ifndef __GYCONDITION_H__
#define __GYCONDITION_H__
#ifdef LINUX64

//include file
#include <pthread.h>
#include "GYThreadCommon.h"

class GYThreadCondition
{
	GYContition m_condition;
	GYMutexHandle m_conditionMutex;

public:
	GYThreadCondition();

	~GYThreadCondition();

    GYVOID Acquire();

	GYVOID Signal();
};

#endif

#endif
