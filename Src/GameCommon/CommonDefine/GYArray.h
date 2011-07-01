/////////////////////////////////////////////
// create time: 2011/7/1 14:07
// author:	colin chen
// file name:	GYArray
// file type:	h
////////////////////////////////////////////

#ifndef __GYARRAY_H__
#define __GYARRAY_H__
template<typename T, GYINT32 length = 32>
class GYArray
{
	T			m_dataArray[length];
	GYINT32		m_currentCount;
	GYINT32		m_maxCount;
public:
	GYArray()
	{
		m_currentCount = 0;
		m_maxCount = sizeof(m_dataArray) / sizeof(T);
	}

	GYBOOL	Add(const T& data)
	{
		if (m_currentCount >= m_maxCount)
		{
			return GYFALSE;
		}
		m_dataArray[m_currentCount++] = data;
		return GYTRUE;
	}

	T&	operator[](GYINT32 index)
	{
		GYAssert(index >=0 && index < m_currentCount)
		return m_dataArray[index];
	}
};

#endif