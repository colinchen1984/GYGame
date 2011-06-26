/////////////////////////////////////////////
// create time:2011/6/21 10:37
// author:	colin chen
// file name:	GYThreadCommon
// file type:	h
////////////////////////////////////////////
#ifndef __GYTHREADCOMMON_H__
#define __GYTHREADCOMMON_H__
#include "GYCommonDefine.h"
#ifdef WIN32
#include <process.h>
#include <Windows.h>
typedef HANDLE GYThreadHandle;
typedef CRITICAL_SECTION GYFastMutexHandle;
typedef HANDLE  GYMutexHandle;
typedef GYUINT32 __stdcall GYTHREADPROCESS(GYVOID* param);
#endif // WIN32

#ifdef LINUX64
#include <pthread.h>
typedef	pthread_t GYThreadHandle;
typedef pthread_mutex_t GYMutexHandle;
typedef GYMutexHandle GYFastMutexHandle;
typedef pthread_cond_t GYContition;
typedef GYVOID* GYTHREADPROCESS(GYVOID* param);
#endif // LINUX64

enum GY_THREAD_STATUS
{
	GY_THREAD_STATUS_INVALID = -1,
	GY_THREAD_STATUS_NO_THREAD,
	GY_THREAD_STATUS_SUSPEND,
	GY_THREAD_STATUS_RUNNING,
	GY_THREAD_STATUS_TERMINATE,
	GY_THREAD_STATUS_TERMINATED,
	GY_THREAD_STATUS_COUNT,
};

extern GYINT32 InitThread();
#ifdef WIN32
extern GYUINT32 __stdcall thread_proc(GYVOID* param);
#endif
#ifdef LINUX64
extern GYVOID* thread_proc(GYVOID * param);
#endif

#endif
