/////////////////////////////////////////////
// create time:2011/9/7 14:54
// author:	colin chen
// file name:	GYSCTest
// file type:	cpp
////////////////////////////////////////////
#include "GYSCTest.h"
#include "GYProtocolDefine.h"
#include "GYSerialization.h"

GYVOID GYSCTest::SetUserName( const GYString& name )
{
	m_name = name;
}

const GYString& GYSCTest::GetUserName()
{
	return m_name;
}

GYVOID GYSCTest::Serializ( GYSerializationInteface& serializer )
{
	serializer << m_userID << m_name;
}

GYVOID GYSCTest::SetUserID(const GYGUID& userID)
{
	m_userID = userID;
}

const GYGUID& GYSCTest::GetUserID()
{
	return m_userID;
}

GYVOID GYSCTest::CleanUp()
{
	m_userID = INVALID_VALUE;
	m_name.CleanUp();
}