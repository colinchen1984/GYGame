/////////////////////////////////////////////
// create time:2011/6/10 10:55
// author:	colin chen
// file name:	GYNetWorkCommonDefine
// file type:	h
////////////////////////////////////////////
#ifndef __GYNETWORKCOMMONDEFINE_H__
#define __GYNETWORKCOMMONDEFINE_H__

#include "GYCommonDefine.h"

#ifdef WIN32
	
#endif
#ifdef LINUX64
	typedef GYINT32 SOCKET;
#endif

extern GYINT32 InitNetWork();
extern GYINT32 ReleaseNetWork();
#endif