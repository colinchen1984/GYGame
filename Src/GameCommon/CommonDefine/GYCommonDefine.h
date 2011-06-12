/////////////////////////////////////////////
// create time:2011/6/10 10:57
// author:	colin chen
// file name:	GYCommonDefine
// file type:	h
////////////////////////////////////////////
#ifndef __GYCOMMONDEFINE_H__
#define __GYCOMMONDEFINE_H__
#ifdef WIN32
	typedef char 					GYCHAR;
	typedef wchar_t 				GYWCHAR;
	typedef char					GYINT8;
	typedef short					GYINT16;
	typedef unsigned short			GYUINT16;
	typedef int						GYINT32;
	typedef unsigned long			GYUINT32;
	typedef __int64					GYINT64;
	typedef void					GYVOID;
	typedef char					GYBOOL;
#endif
#ifdef LINUX64
	typedef char 					GYCHAR;
	typedef wchar_t 				GYWCHAR;
	typedef char					GYINT8;
	typedef short					GYINT16;
	typedef unsigned short			GYUINT16;
	typedef int						GYINT32;
	typedef unsigned int			GYUINT32;
	typedef long long				GYINT64;
	typedef void					GYVOID;
	typedef char					GYBOOL;
#endif
	#define GYGUID					GYINT64
	#define INVALID_VALUE			-1
	#define GYMemcpy				memcpy
	#define GYMemset				memset
	#define GYZeroMem(x, y)			GYMemset(x, 0, y)
	#define GYStrncpy				strncpy
	#define GYNew					new
	#define GYDelete				delete

	const GYBOOL GYTRUE = 1;
	const GYBOOL GYFALSE = 0;
	const GYINT32 GYNULL = 0;
#endif
