/////////////////////////////////////////////
// create time: 2011/7/31 19:58
// author:	colin chen
// file name:	GYProtocolDefine
// file type:	h
////////////////////////////////////////////

#ifndef __GYPROTOCOLDEFINE_H__
#define __GYPROTOCOLDEFINE_H__
#include "GYCommonDefine.h"

typedef GYUINT16 GYPACKETID;
typedef GYUINT16 GYPACKETLEN;
#define GYMakePacketID(packetID) static_cast<GYPACKETID>(packetID)
#define GYGetPacketID(packetID) static_cast<EM_PACKET_ID>(packetID)

enum EM_PACKET_ID
{
	EM_PACKET_ID_INVALID = -1,
	EM_PACKET_ID_TEST_ID,
	EM_PACKET_ID_COUNT,
};

#pragma pack (1)
struct GYPacketHead 
{
	GYPACKETID m_id;
	GYPACKETLEN m_packetLen;
	GYPacketHead()
	{
		m_id = GYMakePacketID(EM_PACKET_ID_INVALID);
		m_packetLen = 0;
	}
};

 #pragma pack ()
const GYINT32 PacektHeadLen = sizeof(GYPacketHead);
#endif