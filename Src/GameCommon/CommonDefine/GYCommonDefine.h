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

#ifdef WIN32
typedef char 					GYCHAR;
typedef wchar_t 				GYWCHAR;
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
#endif

#ifdef LINUX64
typedef char 					GYCHAR;
typedef wchar_t 				GYWCHAR;
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
#endif
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

#include <assert.h>
#define GYAssert(x)				assert(x)
#define GYINLINE				inline
extern GYVOID	GYSleep(GYINT32 ms);					
#endif



