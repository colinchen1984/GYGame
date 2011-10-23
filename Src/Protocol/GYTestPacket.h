/////////////////////////////////////////////
// create time:2011/10/23 19:28
// author:	colin chen
// file name:	GYTestPacket
// file type:	h
////////////////////////////////////////////
#ifndef __GYTESTPACKET_H__
#define __GYTESTPACKET_H__
#include "GYCommonDefine.h"
#include "GYProtocolDefine.h"
#include "GYString.h"



class GYSerializationInteface;
//测试协议
class GYTestPacket : public GYPacketInteface
{
		GYGUID			UserID;			//用户GUID
	GYString			Name;			//用户姓名
	GYUINT64			GatewayReceiveTime;			//Gateway收到数据包时间
	
public:
	GYTestPacket(GYStringManager& stringManager):Name(stringManager)
	{
		
	}
	~GYTestPacket(){};

	virtual GYPACKETID GetPacketID(){return EM_PACKET_ID_TEST_ID;}
	virtual GYCHAR GetPacketFlags(){return 0;}

	GYINLINE	GYVOID			SetUserID(GYGUID value){ UserID = value; };
	GYINLINE	GYGUID			GetUserID() const { return UserID; };
	GYINLINE	GYVOID			SetName(const GYString& value){ Name = value; };
	GYINLINE	const GYString&			GetName() const { return Name; };
	GYINLINE	GYVOID			SetGatewayReceiveTime(GYUINT64 value){ GatewayReceiveTime = value; };
	GYINLINE	GYUINT64			GetGatewayReceiveTime() const { return GatewayReceiveTime; };


	virtual GYVOID Serializ(GYSerializationInteface& serializer);

	virtual GYVOID CleanUp();

};
#endif
