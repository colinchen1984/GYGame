/////////////////////////////////////////////
// create time: 2011/8/31 21:55
// author:	colin chen
// file name:	GYPacketHandler
// file type:	h
////////////////////////////////////////////

#ifndef __GYPACKETHANDLER_H__
#define __GYPACKETHANDLER_H__
#include "GYCommonDefine.h"
class GYPacketInteface;
class GYGatewaySession;

extern GYBOOL GYTestPacketHandler(GYGatewaySession& gatewaySession, const GYGUID& guid, GYPacketInteface& packet);
#endif