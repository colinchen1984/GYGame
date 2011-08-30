/////////////////////////////////////////////
// create time: 2011/7/31 20:14
// author:	colin chen
// file name:	GYTestPacket
// file type:	cpp
////////////////////////////////////////////
#include "GYTestPacket.h"
#include "GYProtocolDefine.h"
#include "GYSerialization.h"

GYVOID GYTestPacket::SetUserName( const GYString& name )
{
	m_name = name;
}

const GYString& GYTestPacket::GetUserName()
{
	return m_name;
}

GYVOID GYTestPacket::Serializ( GYSerializationInteface& serializer )
{
	serializer << m_userID << m_name;
}

GYVOID GYTestPacket::SetUserID( GYINT32 userID )
{
	m_userID = userID;
}

GYINT32 GYTestPacket::GetUserID()
{
	return m_userID;
}
