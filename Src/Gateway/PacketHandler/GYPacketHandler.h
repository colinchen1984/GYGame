/////////////////////////////////////////////
// create time: 2011/8/31 21:55
// author:	colin chen
// file name:	GYPacketHandler
// file type:	h
////////////////////////////////////////////

#ifndef __GYPACKETHANDLER_H__
#define __GYPACKETHANDLER_H__
#include "GYCommonDefine.h"
class GYClientSession;
class GYPacketInteface;

extern GYBOOL GYTestPacketHandler(GYClientSession& session, GYPacketInteface& packet);
#endif