/////////////////////////////////////////////
// create time:2011/10/30 21:44
// author:	colin chen
// file name:	GYSCTest
// file type:	h
////////////////////////////////////////////
#ifndef __GYSCTEST_H__
#define __GYSCTEST_H__
#include "GYCommonDefine.h"
#include "GYProtocolDefine.h"
#include "GYString.h"



class GYSerializationInteface;
//测试协议
class GYSCTest : public GYPacketInteface
{
		GYGUID			UserID;			//用户GUID
	GYString			Name;			//用户姓名
	GYUINT64			GatewayReceiveTime;			//Gateway收到数据包时间
	GYUINT64			LogicReceiveGatewayPacketTime;			//逻辑服务器收到数据包时间
	GYUINT64			GateReceiveLogicPacketTime;			//Gateway收到逻辑服务器数据包时间
	
public:
	GYSCTest(GYStringManager& stringManager):Name(stringManager)
	{
		
	}
	~GYSCTest(){};

	virtual GYPACKETID GetPacketID(){return EM_PACKET_ID_SC_TEST_ID;}
	virtual GYCHAR GetPacketFlags(){return 0;}

	GYINLINE	GYVOID			SetUserID(GYGUID value){ UserID = value; };
	GYINLINE	GYGUID			GetUserID() const { return UserID; };
	GYINLINE	GYVOID			SetName(const GYString& value){ Name = value; };
	GYINLINE	const GYString&			GetName() const { return Name; };
	GYINLINE	GYVOID			SetGatewayReceiveTime(GYUINT64 value){ GatewayReceiveTime = value; };
	GYINLINE	GYUINT64			GetGatewayReceiveTime() const { return GatewayReceiveTime; };
	GYINLINE	GYVOID			SetLogicReceiveGatewayPacketTime(GYUINT64 value){ LogicReceiveGatewayPacketTime = value; };
	GYINLINE	GYUINT64			GetLogicReceiveGatewayPacketTime() const { return LogicReceiveGatewayPacketTime; };
	GYINLINE	GYVOID			SetGateReceiveLogicPacketTime(GYUINT64 value){ GateReceiveLogicPacketTime = value; };
	GYINLINE	GYUINT64			GetGateReceiveLogicPacketTime() const { return GateReceiveLogicPacketTime; };


	virtual GYVOID Serializ(GYSerializationInteface& serializer);

	virtual GYVOID CleanUp();

};
#endif
