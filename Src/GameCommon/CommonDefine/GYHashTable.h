/////////////////////////////////////////////
// create time: 2011/7/23 11:37
// author:	colin chen
// file name:	GYHashTable
// file type:	h
////////////////////////////////////////////

#ifndef __GYHASHTABLE_H__
#define __GYHASHTABLE_H__
#include "GYCommonDefine.h"
#include <malloc.h>


template<typename T>
class GYHashTable
{
	struct  __Hash
	{
		T value;
		GYUINT32 HashValue3;
		GYBOOL bUsed;
		__Hash()
		{
			CleanUp();
		}
		GYVOID CleanUp()
		{
			bUsed			= GYFALSE;
			HashValue3		= INVALID_VALUE;
		}
	};
	
	__Hash** m_pHashTable;
	GYINT32	m_nTableSize;
	GYINT32 m_nBucketSize;

	GYUINT32 _HashFunction1(const char* key)
	{
		GYUINT32 hash = 0 ;

		while(*key)
		{
			hash = (*key++) + (hash << 6) + (hash << 16 ) - hash;
		} 

		return (hash & 0x7FFFFFFF);
	}

	GYUINT32 _HashFunction2(const char* key)
	{
		GYUINT32 hash = 5381;

		while(*key)
		{
			hash += (hash << 5) + (*key++);
		} 

		return  (hash & 0x7FFFFFFF);
	}

	GYUINT32 _HashFunction3(const char* key)
	{
		GYUINT32  hash  =   0 ;
		GYUINT32  x     =   0 ;

		while  (*key)
		{
			hash = (hash << 4) + (*key++);
			if((x = hash & 0xF0000000L) != 0)
			{
				hash ^= (x >> 24);
				hash &= ~ x;
			} 
		} 

		return (hash & 0x7FFFFFFF);
	}

public:
	GYHashTable()
	{
		m_pHashTable = GYNULL;
		m_nTableSize = 0;
	}

	~GYHashTable()
	{
		m_pHashTable = GYNULL;
		m_nTableSize = 0;
	}
	
	GYINT32	Init(GYINT32 nTableSize, GYINT32 nBucketSize)
	{
		GYINT32 err = 0;
		m_nTableSize = nTableSize;
		m_nBucketSize = nBucketSize;
		m_pHashTable = GYNew __Hash*[nTableSize];
		for(GYINT32 i = 0; i < nTableSize; ++i)
		{
			m_pHashTable[i] = GYNew __Hash[nBucketSize];
		}
		return err;
	}

	GYINT32 Release()
	{
		for(GYINT32 i = 0; i < m_nTableSize; ++i)
		{
			GYDelete[] m_pHashTable[i];
		}
		GYDelete[] m_pHashTable;
		m_pHashTable = GYNULL;
		m_nTableSize = 0;
		m_nBucketSize = 0;
		return 0;
	}

	GYVOID CleanUp()
	{
		if (GYNULL != m_pHashTable)
		{
			for(GYINT32 i = 0; i < m_nTableSize; ++i)
			{
				if(GYNULL != m_pHashTable[i])
				{
					for (GYINT32 t = 0; t < m_nBucketSize; ++t)
					{
						m_pHashTable[i][t].CleanUp();
					}
				}
			}
		}
	}

	GYINT32 Insert(GYINT32 key, const T& value)
	{
		char key__[5] = {0};
		*((GYINT32*)key__) = key;
		GYINT32 err = Insert(key__, value);
		return err;
	}

	GYINT32 Insert(const GYGUID& guid, const T& value)
	{
		char key__[GYGUIDLEN+1] = {0};
		*(reinterpret_cast<GYGUID*>(key__)) = guid;
		GYINT32 err = Insert(key__, value);
		return err;
	}

	GYINT32 Insert(const char* key, const T& value)
	{
		GYINT32 err = INVALID_VALUE;
		GYUINT32 hash_value1 = _HashFunction1(key);
		GYUINT32 pos1 = hash_value1 % m_nTableSize;
		if(m_pHashTable[pos1])
		{
			GYUINT32 hash_value2 = _HashFunction2(key);
			GYUINT32 pos2 = hash_value2 % m_nBucketSize;
			if (GYFALSE == m_pHashTable[pos1][pos2].bUsed)
			{
				m_pHashTable[pos1][pos2].bUsed = GYTRUE;
				m_pHashTable[pos1][pos2].HashValue3 = _HashFunction3(key);
				m_pHashTable[pos1][pos2].value = value;
				err = 0;

			}
		}
		return err;
	}

	GYINT32 Remove(GYINT32 key)
	{
		char key__[5] = {0};
		*((GYINT32*)key__) = key;
		GYINT32 err = Remove(key__);
		return err;
	}

	GYINT32 Remove(const GYGUID& guid)
	{
		char key__[GYGUIDLEN+1] = {0};
		*(reinterpret_cast<GYGUID*>(key__)) = guid;
		GYINT32 err = Remove(key__);
		return err;
	}
	
	GYINT32 Remove(const char* key)
	{
		GYINT32 err = INVALID_VALUE;
		GYUINT32 hash_value1 = _HashFunction1(key);
		GYUINT32 pos1 = hash_value1 % m_nTableSize;
		if(m_pHashTable[pos1])
		{
			GYUINT32 hash_value2 = _HashFunction2(key);
			GYUINT32 pos2 = hash_value2 % m_nBucketSize;
			if(m_pHashTable[pos1][pos2].HashValue3 == _HashFunction3(key)&&
				GYTRUE == m_pHashTable[pos1][pos2].bUsed)
			{
				m_pHashTable[pos1][pos2].bUsed = GYFALSE;
				err = 0;
			}
		}
		return err;
	}

	T* Find(GYINT32 key)
	{
		char key__[5] = {0};
		*((GYINT32*)key__) = key;
		T* ret = Find(key__);
		return ret;
	}

	T* Find(const GYGUID& guid)
	{
		char key__[GYGUIDLEN+1] = {0};
		*(reinterpret_cast<GYGUID*>(key__)) = guid;
		T* ret = Find(key__);
		return ret;
	}

	T* Find(const GYCHAR* key)
	{
		T* ret = GYNULL;
		GYUINT32 hash_value1 = _HashFunction1(key);
		GYUINT32 pos1 = hash_value1 % m_nTableSize;
		if(m_pHashTable[pos1])
		{
			GYUINT32 hash_value2 = _HashFunction2(key);
			GYUINT32 pos2 = hash_value2 % m_nBucketSize;
			if(m_pHashTable[pos1][pos2].HashValue3 == _HashFunction3(key)
				&& m_pHashTable[pos1][pos2].bUsed)
			{
				ret = &m_pHashTable[pos1][pos2].value;
			}
		}
		return ret;
	}


};
#endif