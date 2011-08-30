/////////////////////////////////////////////
// create time: 2011/7/31 19:58
// author:	colin chen
// file name:	GYProtocolDefine
// file type:	h
////////////////////////////////////////////

#ifndef __GYPROTOCOLDEFINE_H__
#define __GYPROTOCOLDEFINE_H__
#include "GYCommonDefine.h"
#include "GYProtocolID.h"

class GYSerializationInteface;

typedef GYUINT16 GYPACKETID;
typedef GYUINT16 GYPACKETLEN;
#define GYMakePacketID(packetID) static_cast<GYPACKETID>(packetID)
#define GYGetPacketID(packetID) static_cast<EM_PACKET_ID>(packetID)


#pragma pack (1)
struct GYPacketHead 
{
	GYPACKETID m_id;
	GYPACKETLEN m_packetLen;
	GYCHAR		m_flags;
	GYPacketHead()
	{
		m_id = GYMakePacketID(EM_PACKET_ID_INVALID);
		m_packetLen = 0;
		m_flags = 0;
	}
	GYVOID Serializ(GYSerializationInteface& serializer);
};
#pragma pack ()

const GYINT32 PacektHeadLen = sizeof(GYPacketHead);
const GYINT32 PacketMaxLen = (static_cast<GYPACKETLEN>(INVALID_VALUE) / 2);

class GYPacketInteface
{
protected:
	GYPacketInteface(){};
public:
	virtual ~GYPacketInteface(){};
	virtual GYPACKETID GetPacketID(){return EM_PACKET_ID_INVALID;}
	virtual GYCHAR GetPacketFlags(){return 0;}
	virtual GYVOID Serializ(GYSerializationInteface& serializer) = 0;
};

#endif
