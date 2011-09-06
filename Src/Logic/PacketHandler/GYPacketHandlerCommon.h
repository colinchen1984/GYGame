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
typedef GYBOOL (*PacketHandler)(const GYGUID& guid, GYPacketInteface& packet);

#endif