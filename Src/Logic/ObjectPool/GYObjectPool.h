/////////////////////////////////////////////
// create time: 2011/10/30 15:38
// author:	colin chen
// file name:	GYObjectPool
// file type:	h
////////////////////////////////////////////

#ifndef __GYOBJECTPOOL_H__
#define __GYOBJECTPOOL_H__
#include "GYList.h"
#include "GYHashTable.h"
template<typename HashKeyType, typename T>
class GYObjectPool
{
	GYHashTable<T*>		m_hashTable;
	GYList<T>			m_pFreeObjectChain;
	T*					m_pObjectChain;
	GYINT32				m_nAllocateNum;

	 
	GYINT32 _AllocateObject(GYINT32 allocatNum)
	{
		GYINT32 tableSize = allocatNum * 4 / 1024;
		tableSize = 0 == tableSize ? 1 : tableSize;
		GYINT32 buketSize = 1024;
		GYINT32 result = INVALID_VALUE;
		do 
		{
			if (0 != m_hashTable.Init(tableSize, buketSize))
			{
				break;
			}
			m_pObjectChain = GYNew T[allocatNum];
			if (GYNULL == m_pObjectChain)
			{
				m_hashTable.Release();
				break;
			}

			m_pFreeObjectChain.CleanUp();
			for(GYINT32 i = 0; i < allocatNum; ++i)
			{
				m_pFreeObjectChain.Add(m_pObjectChain[i]);
			}
			result = 0;
		} while (GYFALSE);
		return result;
	}

public:
	GYObjectPool()
	{
		m_pObjectChain = NULL;
	}

	~GYObjectPool()
	{
		m_pObjectChain = NULL;
	}

	GYINT32	Init(GYINT32 nAllocateNum)
	{
		m_pObjectChain = NULL;
		m_nAllocateNum = nAllocateNum;		
		
		return _AllocateObject(nAllocateNum);
	}

	GYINT32 Release()
	{
		GYDelete[] m_pObjectChain;
		m_pObjectChain = NULL;
		m_hashTable.Release();
		m_pFreeObjectChain.CleanUp();
		return 0;
	}

	T* 	AllocObject(const HashKeyType& key)
	{
		T* object = m_pFreeObjectChain.PickUpFirstItem();
		if (GYNULL != object)
		{
			if (0 != m_hashTable.Insert(key, object))
			{
				m_pFreeObjectChain.Add(*object);
				object = GYNULL;
			}
		}
		return object;
	}
	T* 	FindObject(const HashKeyType& key)
	{
		T** object = m_hashTable.Find(key);
		return GYNULL == object ? GYNULL : *object;
	}

	GYINT32 ReleaseObject(const HashKeyType& key, T& object)
	{
		GYINT32 result = INVALID_VALUE;
		if(0 == m_hashTable.Remove(key))
		{
			m_pFreeObjectChain.Add(object);
			result = 0;
		}
		return result;
	}

	GYINT32	GetAllocatedNum()
	{
		return m_nAllocateNum;
	}
};
#endif

