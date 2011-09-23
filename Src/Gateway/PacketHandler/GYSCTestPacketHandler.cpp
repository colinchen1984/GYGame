/////////////////////////////////////////////
// create time: 2011/9/19 23:32
// author:	colin chen
// file name:	GYSCTestPacketHandler
// file type:	cpp
////////////////////////////////////////////
#include "GYPacketHandler.h"
#include "GYClientSession.h"
#include "GYSCTest.h"
#include "GYTimeController.h"
GYBOOL GYSCTestPacketHandler(GYClientSession& session, GYPacketInteface& packet)
{
	GYSCTest& testPacket = static_cast<GYSCTest&>(packet);
	testPacket.m_gateReceiveLogicPacketTime = GYTimeController::GetCpuTime();
	session.SendPacketToClient(testPacket);
	return GYTRUE;
}