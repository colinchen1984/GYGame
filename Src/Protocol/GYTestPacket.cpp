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

const GYString& GYTestPacket::GetName() const
{
	return m_name;
}

GYVOID GYTestPacket::Serializ( GYSerializationInteface& serializer )
{
	serializer << m_userID << m_name << m_gatewayReceiveTime;
}

GYVOID GYTestPacket::SetUserID(const GYGUID& userID)
{
	m_userID = userID;
}

const GYGUID& GYTestPacket::GetUserID()
{
	return m_userID;
}

GYVOID GYTestPacket::CleanUp()
{
	m_userID = INVALID_VALUE;
	m_name.CleanUp();
	m_gatewayReceiveTime = INVALID_VALUE;
}

GYVOID GYTestPacket::SetGatewayReceiveTime( const GYUINT64& time )
{
	m_gatewayReceiveTime = time;
}

const GYUINT64& GYTestPacket::GetGatewayReceiveTime() const
{
	return m_gatewayReceiveTime;
}