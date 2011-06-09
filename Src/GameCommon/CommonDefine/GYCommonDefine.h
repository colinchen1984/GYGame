#ifndef __GY_COMMON_DEFINE_H__
#define __GY_COMMON_DEFINE_H__
#ifdef __WIN32__
	typedef char 					GYCHAR;
	typedef wchar_t 				GYWCHAR;
	typedef char					GYINT8;
	typedef short					GYINT16;
	typedef int						GYINT32;
	typedef unsigned int			GYUINT32;
	typedef __int64					GYINT64;
	typedef void					GYVOID;
#endif
#ifdef __LINUX64
	typedef char 					GYCHAR;
	typedef wchar_t 				GYWCHAR;
	typedef char					GYINT8;
	typedef short					GYINT16;
	typedef int						GYINT32;
	typedef unsigned int			GYUINT32;
	typedef long long				GYINT64;
	typedef void					GYVOID;
#endif
	#define GUID GYINT64
#endif
