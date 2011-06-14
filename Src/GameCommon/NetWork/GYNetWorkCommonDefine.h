/////////////////////////////////////////////
// create time:2011/6/10 10:55
// author:	colin chen
// file name:	GYNetWorkCommonDefine
// file type:	h
////////////////////////////////////////////
#ifndef __GYNETWORKCOMMONDEFINE_H__
#define __GYNETWORKCOMMONDEFINE_H__

#include "GYCommonDefine.h"
#include <errno.h>
#ifdef WIN32
	#include <WinSock2.h>
	typedef SOCKET GYSOCKET;
#endif
#ifdef LINUX64
	#include <netdb.h>
	#include <unistd.h>
	typedef GYINT32 GYSOCKET;
	const GYINT32 INVALID_SOCKET = INVALID_VALUE;
#endif

extern GYINT32 InitNetWork();
extern GYINT32 ReleaseNetWork();
extern GYINT32 GYIsValidSocket(GYSOCKET fd);
extern GYINT32 GetLastNetWorkError();
#endif