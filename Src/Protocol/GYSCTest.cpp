/////////////////////////////////////////////
// create time: 2011/10/30 21:44
// author:	colin chen
// file name:	GYSCTest
// file type:	cpp
////////////////////////////////////////////
#include "GYSCTest.h"
#include "GYProtocolDefine.h"
#include "GYSerialization.h"

GYVOID GYSCTest::Serializ( GYSerializationInteface& serializer )
{
		serializer << UserID;
	serializer << Name;
	serializer << GatewayReceiveTime;
	serializer << LogicReceiveGatewayPacketTime;
	serializer << GateReceiveLogicPacketTime;

}

GYVOID GYSCTest::CleanUp()
{
		UserID = INVALID_VALUE;
	Name.CleanUp();
	GatewayReceiveTime = INVALID_VALUE;
	LogicReceiveGatewayPacketTime = INVALID_VALUE;
	GateReceiveLogicPacketTime = INVALID_VALUE;
	
}




