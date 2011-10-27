/////////////////////////////////////////////
// create time: 2011/10/27 23:01
// author:	colin chen
// file name:	GYPosition
// file type:	h
////////////////////////////////////////////

#ifndef __GYPOSITION_H__
#define __GYPOSITION_H__
#include "GYCommonDefine.h"
#include "GYMath.h"

struct GYPosition
{
	GYFLOAT	m_x;
	GYFLOAT	m_z;

	GYPosition(GYFLOAT x = 0.0f, GYFLOAT z = 0.0f):m_x(x), m_z(z)
	{

	}

	~GYPosition()
	{

	}

	GYPosition(const GYPosition& pos)
	{
		*this = pos;
	}

	GYPosition& operator=(const GYPosition& pos)
	{
		this->m_x = pos.m_x;
		this->m_z = pos.m_z;
		return *this;
	}

	GYBOOL	operator==(const GYPosition& pos)const
	{
		GYBOOL result = GYFALSE;
		if (GYTRUE == GYMath::IsEqualFloat(m_x, pos.m_x) && GYTRUE == GYMath::IsEqualFloat(m_z, pos.m_z))
		{
			result = GYTRUE;
		}
		return result;
	}
};
#endif
