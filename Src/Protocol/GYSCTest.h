/////////////////////////////////////////////
// create time:2011/9/7 14:54
// author:	colin chen
// file name:	GYSCTest
// file type:	h
////////////////////////////////////////////
#ifndef __GYSCTEST_H__
#define __GYSCTEST_H__
#include "GYCommonDefine.h"
#include "GYString.h"
#include "GYProtocolDefine.h"

class GYSerializationInteface;

class GYSCTest : public GYPacketInteface
{
public:
	GYGUID		m_userID;
	GYString	m_name;
	GYUINT64	m_gatewayReceiveClientPacketTime;
	GYUINT64	m_logicReceiveGatewayPacketTime;
	GYUINT64	m_gateReceiveLogicPacketTime;

public:
	GYSCTest(GYStringManager& stringManager):m_name(stringManager)
	{
	};
	~GYSCTest(){};

	virtual GYPACKETID GetPacketID(){return EM_PACKET_ID_SC_TEST_ID;}
	virtual GYCHAR GetPacketFlags(){return 0;}

	GYVOID SetUserName(const GYString& name);
	const GYString& GetUserName();

	GYVOID SetUserID(const GYGUID& userID);
	const GYGUID& GetUserID();

	virtual GYVOID Serializ(GYSerializationInteface& serializer);

	virtual GYVOID CleanUp();

};
#endif
