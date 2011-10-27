/////////////////////////////////////////////
// create time: 2011/10/27 23:01
// author:	colin chen
// file name:	GYRect
// file type:	h
////////////////////////////////////////////

#ifndef __GYRECT_H__
#define __GYRECT_H__
#include "GYPosition.h"
#include "GYMisc.h"

struct GYRect
{
	GYPosition m_pointA;
	GYPosition m_pointB;

	GYRect()
	{
	}

	~GYRect()
	{
	}

	GYRect(const GYPosition& pointA, const GYPosition& potinB)
	{
		m_pointA = pointA;
		m_pointB = potinB;
	}

	GYRect(const GYRect& rect)
	{
		*this = rect;
	}

	GYRect& operator=(const GYRect& rect)
	{
		m_pointA = rect.m_pointA;
		m_pointB = rect.m_pointB;
		return *this;
	}

	GYBOOL operator==(const GYRect& rect)const
	{
		GYBOOL result = GYFALSE;
		if (m_pointA == rect.m_pointA && m_pointB == rect.m_pointB)
		{
			result = GYTRUE;
		}
		return result;
	}

	GYBOOL InRect(const GYPosition& pos)const
	{
		GYBOOL result = GYFALSE;
		if (pos.m_x >= Min(m_pointA.m_x, m_pointB.m_x) && pos.m_x <= Max(m_pointA.m_x, m_pointB.m_x))
		{
			if (pos.m_z >= Min(m_pointA.m_z, m_pointB.m_z) && pos.m_x <= Max(m_pointA.m_z, m_pointB.m_z))
			{
				result = GYTRUE;
			}
		}
		return result;
	}
};
#endif
