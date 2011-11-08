/////////////////////////////////////////////
// create time: 2011/7/1 14:07
// author:	colin chen
// file name:	GYArray
// file type:	h
////////////////////////////////////////////

#ifndef __GYARRAY_H__
#define __GYARRAY_H__
#include "GYCommonDefine.h"

template<typename T>
class GYArray
{
	T*			m_dataArray;
	GYINT32		m_currentCount;
	GYINT32		m_maxCount;
public:
	GYArray()
	{
		m_currentCount = 0;
		m_maxCount = 0;
		m_dataArray = GYNULL;
	}

	GYINT32 Init(GYINT32 maxCount)
	{
		GYINT32 result = INVALID_VALUE;
		m_dataArray = GYNew T[maxCount];
		if (GYNULL != m_dataArray)
		{
			m_currentCount = 0;
			m_maxCount = maxCount;
			result = 0;
		}
		return result;		
	}

	GYVOID CleanUp()
	{
		m_currentCount = 0;
	}

	GYINT32 Release()
	{
		GYDelete[] m_dataArray;
		m_dataArray = GYNULL;
		m_currentCount = 0;
		m_maxCount = 0;
		return 0;		
	}

	GYINT32	Add(const T& data)
	{
		GYINT32 result = INVALID_VALUE;
		if (m_currentCount < m_maxCount)
		{
			m_dataArray[m_currentCount++] = data;
			result = 0;
		}
		return result;
	}

	T&	operator[](GYINT32 index)
	{
		GYAssert(index >= 0 && index < m_currentCount);
		return m_dataArray[index];
	}
};

template<typename T>
class GYPointArray
{
	T**			m_dataArray;
	GYINT32		m_currentCount;
	GYINT32		m_maxCount;
public:
	GYPointArray()
	{
		CleanUp();
	}

	GYINLINE GYVOID CleanUp()
	{
		m_dataArray = GYNULL;
		m_currentCount = 0;
		m_maxCount = 0;
	}

	GYINLINE GYINT32 Init(GYINT32 maxCount)
	{
		m_dataArray = GYNew T*[maxCount];
		m_maxCount = maxCount;
		m_currentCount = 0;
		GYZeroMem(m_dataArray, sizeof(T*) * maxCount);
		return 0;
	}

	GYINLINE GYINT32 Release()
	{
		GYDelete[] m_dataArray;
		CleanUp();
		return 0;
	}

	GYBOOL	Add(T& data)
	{
		if (m_currentCount >= m_maxCount)
		{
			return GYFALSE;
		}
		data.m_index = m_currentCount;
		m_dataArray[m_currentCount++] = &data;
		return GYTRUE;
	}

	GYBOOL Delete(T& data)
	{
		if (data.m_index >= m_currentCount || data.m_index < 0 || m_currentCount <= 0)
		{
			return GYFALSE;
		}
		
		if (data.m_index != --m_currentCount)
		{
			m_dataArray[m_currentCount]->m_index = data.m_index;
			m_dataArray[data.m_index] = m_dataArray[m_currentCount];
		}
		m_dataArray[m_currentCount] = GYNULL;
		data.m_index = INVALID_VALUE;
		return GYTRUE;

	}

	GYINLINE GYINT32 GetCurrentItemCount()
	{
		return m_currentCount;
	}

	GYINLINE T*	operator[](GYINT32 index)
	{
		GYAssert(index >=0 && index < m_currentCount);
		return m_dataArray[index];
	}
};

#endif

