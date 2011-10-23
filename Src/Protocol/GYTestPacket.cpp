/////////////////////////////////////////////
// create time: 2011/10/23 19:28
// author:	colin chen
// file name:	GYTestPacket
// file type:	cpp
////////////////////////////////////////////
#include "GYTestPacket.h"
#include "GYProtocolDefine.h"
#include "GYSerialization.h"

GYVOID GYTestPacket::Serializ( GYSerializationInteface& serializer )
{
		serializer << UserID;
	serializer << Name;
	serializer << GatewayReceiveTime;

}

GYVOID GYTestPacket::CleanUp()
{
		UserID = INVALID_VALUE;
	Name.CleanUp();
	GatewayReceiveTime = INVALID_VALUE;
	
}




