/////////////////////////////////////////////
// create time:2011/6/21 10:37
// author:	colin chen
// file name:	GYThreadCommon
// file type:	h
////////////////////////////////////////////
#ifndef __GYTHREADCOMMON_H__
#define __GYTHREADCOMMON_H__

#ifdef WIN32
#include <windows.h>
typedef HANDLE GYThreadHandle;
typedef CRITICAL_SECTION GYFastMutexHandle;
typedef HANDLE  GYMutexHandle;
#endif // WIN32

#ifdef LINUX64
#include <pthread.h>
typedef	pthread_t GYThreadHandle;
typedef pthread_mutex_t GYMutexHandle;
typedef GYMutexHandle GYFastMutexHandle;
typedef pthread_cond_t GYContition;
#endif // LINUX64

#endif