/////////////////////////////////////////////
// create time: 2011/7/22 15:55
// author:	colin chen
// file name:	GYString
// file type:	cpp
////////////////////////////////////////////
#include "GYString.h"
#include "GYStringManger.h"
#include <string.h>

GYString::GYString(GYStringManager& stringManager):m_strManager(stringManager)
{
	m_stringBuffer = GYNULL;
	m_stringLength = 0;
}

GYString::GYString(const GYString& str):m_strManager(str.m_strManager)
{
	m_stringBuffer = GYNULL;
	m_stringLength = 0;
	*this = str;
}

GYString::GYString(const GYCHAR* pString, GYINT32 stringLength, GYStringManager& stringManger):m_strManager(stringManger)
{
	m_stringLength = stringLength;
	m_stringBuffer = stringManger.AllocateString(pString, m_stringLength);
	if (GYNULL == m_stringBuffer)
	{
		m_stringLength = 0;
	}
	
}

GYString::~GYString()
{
	if (GYNULL != m_stringBuffer)
	{
		m_strManager.DeleteStringReference(m_stringBuffer, m_stringLength);
	}
}

GYBOOL GYString::operator==( const GYString& str ) const
{
	GYBOOL result = GYFALSE;
	if (str.m_stringBuffer == m_stringBuffer)
	{
		result = GYTRUE;
	}
	
	return result;
}

GYBOOL GYString::operator==( const GYCHAR* str ) const
{
	return 0 == strcmp(str, m_stringBuffer) ? GYTRUE : GYFALSE;
}


GYString& GYString::operator=( const GYString& str )
{	
	if (m_stringBuffer != str.m_stringBuffer)
	{
		CleanUp();

		m_stringLength = str.m_stringLength;
		m_stringBuffer = m_strManager.AllocateString(str.m_stringBuffer, str.m_stringLength);
		if (GYNULL == m_stringBuffer)
		{
			m_stringLength = 0;
		}
	}
	return *this;
}

GYString& GYString::operator=( const GYCHAR* str )
{
	CleanUp();

	m_stringLength = strlen(str);
	m_stringBuffer = m_strManager.AllocateString(str, m_stringLength);
	if (GYNULL == m_stringBuffer)
	{
		m_stringLength = 0;
	}
	return *this;
}

GYVOID GYString::CleanUp()
{
	if (GYNULL != m_stringBuffer)
	{
		m_strManager.DeleteStringReference(m_stringBuffer, m_stringLength);
		m_stringLength = 0;
		m_stringBuffer = GYNULL;
	}
}




