/////////////////////////////////////////////
// create time: 2011/8/31 21:58
// author:	colin chen
// file name:	GYTestPacketHandler
// file type:	cpp
////////////////////////////////////////////
#include "GYPacketHandler.h"
#include "GYClientSession.h"
#include "GYTestPacket.h"
#include "GYTimeController.h"
GYBOOL GYTestPacketHandler(GYClientSession& session, GYPacketInteface& packet)
{
	GYTestPacket& testPacket = static_cast<GYTestPacket&>(packet);
	if (INVALID_VALUE == session.GetGUID())
	{
		session.SetGUID(testPacket.GetUserID());
	}
	testPacket.SetGatewayReceiveTime(GYTimeController::GetCpuTime());
	session.SendPacketToLogic(testPacket);
	return GYTRUE;
}
