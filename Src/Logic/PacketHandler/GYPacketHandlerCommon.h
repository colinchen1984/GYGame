/////////////////////////////////////////////
// create time: 2011/8/31 22:05
// author:	colin chen
// file name:	GYPacketHandlerCommon
// file type:	h
////////////////////////////////////////////

#ifndef __GYPACKETHANDLERCOMMON_H__
#define __GYPACKETHANDLERCOMMON_H__
#include "GYCommonDefine.h"
class GYPacketInteface;
class GYGatewaySession;
typedef GYBOOL (*PacketHandler)(GYGatewaySession& gatewaySession, const GYGUID& guid, GYPacketInteface& packet);

#endif