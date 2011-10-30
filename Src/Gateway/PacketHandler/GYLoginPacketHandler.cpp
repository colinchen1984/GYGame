/////////////////////////////////////////////
// create time: 2011/10/30 20:18
// author:	colin chen
// file name:	GYLoginPacketHandler
// file type:	cpp
////////////////////////////////////////////
#include "GYLogin.h"
#include "GYPacketHandler.h"
#include "GYClientSession.h"

GYBOOL GYLoginPacketHandler(GYClientSession& session, GYPacketInteface& packet)
{
	const GYLogin& logInPacket = static_cast<GYLogin&>(packet);
	session.SetGUID(logInPacket.GetUserID());
	session.SendPacketToLogic(packet);
	return GYTRUE;
}
