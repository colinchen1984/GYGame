/////////////////////////////////////////////
// create time: 2011/7/31 20:14
// author:	colin chen
// file name:	GYTestPacket
// file type:	cpp
////////////////////////////////////////////
#include "GYTestPacket.h"
#include "GYProtocolDefine.h"

#pragma pack (1)
struct GYTestPacket
{
	GYUINT8	m_userNameLen;
	GYCHAR	m_userName[1];
	
	GYBOOL Init(GYINT32 userNameLen)
	{
		m_userNameLen = static_cast<GYUINT8>(userNameLen);
		return GYTRUE;
	}

	EM_PACKET_ID GetPacketID(){return EM_PACKET_ID_TEST_ID;}

	static GYPACKETLEN GetPacketLen(GYINT32 userNameLen)
	{	
		GYPACKETLEN len = sizeof(GYTestPacket);
		len += sizeof(GYCHAR) * (userNameLen);
		return len;
	}
};
#pragma pack ()

GYVOID GYTestPacketWrap::SetUseName( const GYCHAR* userName, GYINT32 userNameLen )
{
	memcpy(m_data->m_userName, userName, sizeof(m_data->m_userName[0]) * userNameLen);
	m_data->m_userName[userNameLen] = 0;
}

GYBOOL GYTestPacketWrap::Init( GYCHAR* allocatedMemory, GYINT32 userNameLen)
{
	GYPacketHead* pHead = reinterpret_cast<GYPacketHead*>(allocatedMemory);
	m_data =reinterpret_cast<GYTestPacket*>(&pHead[1]);
	m_data->Init(userNameLen);
	pHead->m_id = m_data->GetPacketID();
	pHead->m_packetLen = m_data->GetPacketLen(userNameLen);
	return GYTRUE;
}

GYINT32 GYTestPacketWrap::GetPacketLen( GYINT32 userNameLen )
{
	GYINT32 result = GYTestPacket::GetPacketLen(userNameLen) + PacektHeadLen;
	return result;
}

GYTestPacketWrap::GYTestPacketWrap()
{

}

GYTestPacketWrap::~GYTestPacketWrap()
{

}