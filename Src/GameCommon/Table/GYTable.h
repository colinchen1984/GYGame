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
#ifdef CHECK_DATA_TYPE
				m_fileLoader.BeginLoadRowData();
#endif
				m_tableRow[i].Serializ(m_fileLoader);
			}
			result = 0;
		} while (GYFALSE);
		return result;
	}

	const T* GetRowByIndex(GYINT32 index)
	{
		const T* pResult = GYNULL;
		if (index >= 0 && index < m_tableRowCount)
		{
			pResult = &m_tableRow[index];
		}
		return pResult;
	}

	const T* operator[](GYINT32 tableID)
	{
		const T* pResult = GYNULL;
		if (GYNULL == m_tableRow)
		{
			return pResult;
		}
		GYINT32 beginIndex = 0;
		GYINT32 endIndex = m_tableRowCount;
		GYINT32 index = m_tableRowCount / 2;
		while(endIndex - beginIndex >= 8)
		{
			if ((*(GYINT32*)(&m_tableRow[index])) == tableID)
			{
				pResult = &m_tableRow[index];
				break;
			}
			else if((*(GYINT32*)(&m_tableRow[index])) > tableID)
			{
				endIndex = index - 1;
			}
			else
			{
				beginIndex = index + 1;
			}
			index = (beginIndex + endIndex) >> 1;
		}
		if (GYNULL == pResult)
		{
			for (GYINT32 i = beginIndex; i < endIndex; ++i)
			{
				if ((*(GYINT32*)(&m_tableRow[i])) == tableID)
				{
					pResult = &m_tableRow[i];
					break;
				}
			}
		}
		return pResult;
	}
};

#endif

