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

//测试包
extern GYBOOL GYTestPacketHandler(GYClientSession& session, GYPacketInteface& packet);
extern GYBOOL GYSCTestPacketHandler(GYClientSession& session, GYPacketInteface& packet);

//登陆包
extern GYBOOL GYLoginPacketHandler(GYClientSession& session, GYPacketInteface& packet);

#endif
