/////////////////////////////////////////////
// create time: 2011/7/31 20:14
// author:	colin chen
// file name:	GYTestPacket
// file type:	cpp
////////////////////////////////////////////
#include "GYTestPacket.h"
#include "GYProtocolDefine.h"
#include "GYSerialization.h"

GYVOID GYTestPacket::SetUseName( const GYString& name )
{
	m_name = name;
}

const GYString& GYTestPacket::GetUseName()
{
	return m_name;
}

GYVOID GYTestPacket::Serializ( GYSerializationInteface& serializer )
{
	serializer << m_name;
}
