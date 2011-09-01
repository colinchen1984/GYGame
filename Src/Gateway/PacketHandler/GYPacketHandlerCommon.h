/////////////////////////////////////////////
// create time: 2011/8/31 22:05
// author:	colin chen
// file name:	GYPacketHandlerCommon
// file type:	h
////////////////////////////////////////////

#ifndef __GYPACKETHANDLERCOMMON_H__
#define __GYPACKETHANDLERCOMMON_H__
class GYClientSession;
class GYPacketInteface;
typedef GYBOOL (*PacketHandler)(GYClientSession& session, GYPacketInteface& packet);

#endif