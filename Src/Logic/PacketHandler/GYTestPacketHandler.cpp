/////////////////////////////////////////////
// create time: 2011/8/31 21:58
// author:	colin chen
// file name:	GYTestPacketHandler
// file type:	cpp
////////////////////////////////////////////
#include "GYPacketHandler.h"
#include "GYTestPacket.h"
#include "GYGatewaySession.h"
#include "GYSCTest.h"
#include "GYStringManger.h"
#include "GYTimeController.h"
GYBOOL GYTestPacketHandler(GYGatewaySession& gatewaySession, const GYGUID& guid, GYPacketInteface& packet)
{
	const GYTestPacket& tPacket = static_cast<GYTestPacket&>(packet);
	GYSCTest replyPacket(GETSTRINGMANAGERSINGLETON);
	replyPacket.SetName(tPacket.GetName());
	replyPacket.SetUserID(guid);
	replyPacket.SetGatewayReceiveTime(tPacket.GetGatewayReceiveTime());
	replyPacket.SetLogicReceiveGatewayPacketTime(GYTimeController::GetCpuTime());
	gatewaySession.SendPacket(guid, replyPacket);
	return GYTRUE;
}
