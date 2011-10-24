/////////////////////////////////////////////
// create time:2011/6/10 10:57
// author:	colin chen
// file name:	GYCommonDefine
// file type:	h
////////////////////////////////////////////
#ifndef __GYCOMMONDEFINE_H__
#define __GYCOMMONDEFINE_H__
#include <memory.h>
#include <errno.h>

#include <assert.h>
#define GYAssert(x)				assert(x)
#define GYINLINE				inline
#define GYStaticAssert			static_assert

#ifdef WIN32
typedef char 					GYCHAR;
typedef char					GYINT8;
typedef unsigned char			GYUINT8;
typedef short					GYINT16;
typedef unsigned short			GYUINT16;
typedef int						GYINT32;
typedef unsigned int			GYUINT32;
typedef __int64					GYINT64;
typedef unsigned __int64		GYUINT64;
typedef float					GYFLOAT;
typedef void					GYVOID;
typedef char					GYBOOL;
GYStaticAssert(4 == sizeof(GYVOID*), "WIN32 micro is for 32-bit platform only");
#endif

#ifdef LINUX64
typedef char 					GYCHAR;
typedef char					GYINT8;
typedef unsigned char			GYUINT8;
typedef short					GYINT16;
typedef unsigned short			GYUINT16;
typedef int						GYINT32;
typedef unsigned int			GYUINT32;
typedef long long				GYINT64;
typedef unsigned long long		GYUINT64;
typedef float					GYFLOAT;
typedef void					GYVOID;
typedef char					GYBOOL;
GYStaticAssert(8 == sizeof(GYVOID*), "LINUX64 micro is for 64-bit platform only");
#endif
GYStaticAssert(1 == sizeof(GYCHAR), "sizeof GYCHAR isn't 1");
GYStaticAssert(1 == sizeof(GYBOOL), "sizeof GYBOOL isn't 1");
GYStaticAssert(1 == sizeof(GYINT8), "sizeof GYINT8 isn't 1");
GYStaticAssert(1 == sizeof(GYUINT8), "sizeof GYUINT8 isn't 1");
GYStaticAssert(2 == sizeof(GYINT16), "sizeof GYUINT8 isn't 2");
GYStaticAssert(2 == sizeof(GYUINT16), "sizeof GYUINT16 isn't 2");
GYStaticAssert(4 == sizeof(GYINT32), "sizeof GYINT32 isn't 4");
GYStaticAssert(4 == sizeof(GYUINT32), "sizeof GYUINT32 isn't 4");
GYStaticAssert(8 == sizeof(GYINT64), "sizeof GYINT64 isn't 8");
GYStaticAssert(8 == sizeof(GYUINT64), "sizeof GYUINT64 isn't 8");
GYStaticAssert(8 == sizeof(GYUINT64), "sizeof GYUINT64 isn't 8");

#define GYGUID					GYUINT64
#define INVALID_VALUE			-1
#define GYMemcpy				memcpy
#define GYMemcmp				memcmp
#define GYMemset				memset
#define GYZeroMem(x, y)			GYMemset(x, 0, y)
#define GYStrncpy				strncpy
#define GYNew					new
#define GYDelete				delete

const GYINT32 GYGUIDLEN = sizeof(GYGUID);
const GYBOOL GYTRUE = 1;
const GYBOOL GYFALSE = 0;
const GYINT32 GYNULL = 0;

extern GYVOID	GYSleep(GYINT32 ms);					
#endif



