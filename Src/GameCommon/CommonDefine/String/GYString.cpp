﻿/////////////////////////////////////////////
// create time: 2011/7/22 15:55
// author:	colin chen
// file name:	GYString
// file type:	cpp
////////////////////////////////////////////
#include "GYString.h"
#include "GYStringManger.h"
#include <wchar.h>

GYString::GYString(GYStringManager& stringManager):m_strManager(stringManager)
{
	m_stringBuffer = GYNULL;
	m_stringLength = 0;
}

GYString::GYString(const GYString& str):m_strManager(str.m_strManager)
{
	*this = str;
}

GYString::GYString(const GYWCHAR* pString, GYINT32 stringLength, GYStringManager& stringManger):m_strManager(stringManger)
{
	m_stringBuffer = stringManger.AllocateString(pString, stringLength);
	m_stringLength = stringLength;
}

GYString::~GYString()
{
	m_strManager.DeleteStringReference(m_stringBuffer, m_stringLength);
}

GYBOOL GYString::operator==( const GYString& str ) const
{
	if(str.m_stringBuffer == m_stringBuffer)
	{
		GYAssert(str.m_stringLength == m_stringLength);
		return GYTRUE;
	}
	else if(str.m_stringLength != m_stringLength)
	{
		return GYFALSE;
	}
	else
	{
		return 0 == wcscmp(str.m_stringBuffer, m_stringBuffer) ? GYTRUE : GYFALSE;
	}
	return GYFALSE;
}

GYBOOL GYString::operator==( const GYWCHAR* str ) const
{
	return 0 == wcscmp(str, m_stringBuffer) ? GYTRUE : GYFALSE;
}


GYVOID GYString::operator=( const GYString& str )
{
	m_stringBuffer = str.m_stringBuffer;
	m_stringLength = str.m_stringLength;
	m_strManager = str.m_strManager;
	m_strManager.AddStringReference(m_stringBuffer, m_stringLength);
}

// GYVOID GYString::operator=(const GYWCHAR* str)
// {
// 	m_stringLength = wcslen(str);
// 	m_stringBuffer = m_strManager->AllocateString(str, m_stringLength);
// }


