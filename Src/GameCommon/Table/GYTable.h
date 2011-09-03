/////////////////////////////////////////////
// create time: 2011/9/3 17:34
// author:	colin chen
// file name:	GYTable
// file type:	h
////////////////////////////////////////////

#ifndef __GYTABLE_H__
#define __GYTABLE_H__
#include "GYCommonDefine.h"
#include "GYTableSerialization.h"
#include "GYString.h"
template<typename T>
class GYTable
{
	T*		m_tableRow;
	GYINT32	m_tableRowCount;
	GYINT32	m_tableColumCount;
	GYTableSerialization m_fileLoader;
public:
	GYTable()
	{
		m_tableRow = GYNULL;
		m_tableRowCount = 0;
		m_tableColumCount = 0;
	}
	~GYTable()
	{
		GYDelete[] m_tableRow;
	}

	GYINT32 Load(const GYString& fileName)
	{
		GYINT32 result = INVALID_VALUE;
		do 
		{
			if(0 != m_fileLoader.Init(fileName))
			{
				break;
			}
			m_tableRowCount = m_fileLoader.GetTableDataRowCount();
			m_tableColumCount = m_fileLoader.GetTableDataColumCount();
			if (m_tableRowCount <= 0)
			{
				break;
			}
			m_tableRow = GYNew T[m_tableRowCount];
			for (GYINT32 i = 0; i < m_tableRowCount; ++i)
			{
				m_tableRow[i].Serializ(m_fileLoader);
			}
			result = 0;
		} while (GYFALSE);
		return result;
	}

	const T* operator[](GYINT32 tableID)
	{
			binary_search()
	}
};

#endif

