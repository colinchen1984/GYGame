/////////////////////////////////////////////
// create time: 2011/10/30 20:18
// author:	colin chen
// file name:	GYLoginPacketHandler
// file type:	cpp
////////////////////////////////////////////
#include "GYLogin.h"
#include "GYPacketHandler.h"
#include "GYServer.h"

GYBOOL GYLoginPacketHandler(GYGatewaySession& gatewaySession, const GYGUID& guid, GYPacketInteface& packet)
{
	const GYLogin& loginPacket = static_cast<GYLogin&>(packet);
	if (0 != GETLOGICSERVER.AddHumanToScene(gatewaySession, loginPacket.GetUserID(), loginPacket.GetSceneID()))
	{
		//通知gateway断开与客户端的连接
	}
	return GYTRUE;
}

