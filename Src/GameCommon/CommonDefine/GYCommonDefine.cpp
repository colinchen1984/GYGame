/////////////////////////////////////////////
// create time:2011/6/10 10:57
// author:	colin chen
// file name:	GYCommonDefine
// file type:	h
////////////////////////////////////////////
#include "GYCommonDefine.h"
#ifdef WIN32
#include <Windows.h>
#endif
#ifdef LINUX64
#include <sys/select.h>
#include <sys/time.h>
#endif


const GYCHAR MAX_CHAR = 127;
const GYCHAR MIN_CHAR = -128;
const GYINT8 MAX_INT8 = 127;
const GYINT8 MIN_INT8 = -128;
const GYUINT8 MAX_UINT8 = 0xff;
const GYINT16 MAX_INT16 = 32767;
const GYINT16 MIN_INT16 = -32768;
const GYUINT16 MAX_UINT16 = 0xffff;
const GYINT32 MAX_INT32 = 2147483647;
const GYINT32 MIN_INT32 = -2147483647 - 1;
const GYUINT32 MAX_UINT32 = 0xffffffff;
const GYINT64 MAX_INT64 = 9223372036854775807i64;
const GYINT64 MIN_INT64 = (-9223372036854775807i64 - 1);
const GYUINT64 MAX_UINT64 = 0xffffffffffffffffui64;
const GYFLOAT GYFLT_EPSILON = 1.192092896e-07F;


GYVOID GYSleep(GYINT32 ms)
{
#ifdef WIN32
	Sleep(ms);
#endif

#ifdef LINUX64
	static timeval waitTime;
	waitTime.tv_sec = (ms / 1000);
	waitTime.tv_usec =((ms % 1000) * 1000);
	::select(0,GYNULL, GYNULL, GYNULL, &waitTime);
#endif
}
