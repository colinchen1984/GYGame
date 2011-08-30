/////////////////////////////////////////////
// create time: 2011/8/29 22:27
// author:	colin chen
// file name:	GYProtocolDefine
// file type:	cpp
////////////////////////////////////////////
#include "GYProtocolDefine.h"
#include "GYSerialization.h"

GYVOID GYCSPacketHead::Serializ( GYSerializationInteface& serializer )
{
	serializer << m_id;
	serializer << m_packetLen;
	serializer << m_flags;
}