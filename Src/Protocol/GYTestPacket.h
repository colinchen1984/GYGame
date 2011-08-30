/////////////////////////////////////////////
// create time: 2011/7/31 20:11
// author:	colin chen
// file name:	GYTestPacket
// file type:	h
////////////////////////////////////////////

#ifndef __GYTESTPACKET_H__
#define __GYTESTPACKET_H__
#include "GYCommonDefine.h"
#include "GYString.h"
#include "GYProtocolDefine.h"

class GYStringManager;
class GYSerializationInteface;

class GYTestPacket : public GYPacketInteface
{
	GYINT32		m_userID;
	GYString	m_name;
public:
	GYTestPacket(GYStringManager& stringManager):m_name(stringManager)
	{
	};
	~GYTestPacket(){};

	virtual GYPACKETID GetPacketID(){return EM_PACKET_ID_TEST_ID;}
	virtual GYCHAR GetPacketFlags(){return 0;}

	GYVOID SetUserName(const GYString& name);
	const GYString& GetUserName();

	GYVOID SetUserID(GYINT32 userID);
	GYINT32 GetUserID();

	virtual GYVOID Serializ(GYSerializationInteface& serializer);

};
#endif
