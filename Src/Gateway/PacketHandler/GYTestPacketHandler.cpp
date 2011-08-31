/////////////////////////////////////////////
// create time: 2011/8/31 21:58
// author:	colin chen
// file name:	GYTestPacketHandler
// file type:	cpp
////////////////////////////////////////////
#include "GYPacketHandler.h"
#include "GYClientSession.h"
#include "GYTestPacket.h"
GYBOOL GYTestPacketHandler(GYClientSession& session, GYPacketInteface& packet)
{
	GYTestPacket& testPacket = static_cast<GYTestPacket&>(packet);
	session.SendPacket(packet);
	return GYTRUE;
}