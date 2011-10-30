/////////////////////////////////////////////
// create time: 2011/10/30 20:18
// author:	colin chen
// file name:	GYLoginPacketHandler
// file type:	cpp
////////////////////////////////////////////
#include "GYLogout.h"
#include "GYPacketHandler.h"
#include "GYServer.h"

GYBOOL GYLogoutPacketHandler(GYGatewaySession& gatewaySession, const GYGUID& guid, GYPacketInteface& packet)
{
	const GYLogout& logoutPacket = static_cast<GYLogout&>(packet);
	GETLOGICSERVER.RemoveHumanFromScene(logoutPacket.GetUserID());
	
	return GYTRUE;
}

