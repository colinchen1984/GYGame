/////////////////////////////////////////////
// create time: 2011/7/23 11:37
// author:	colin chen
// file name:	GYHashTable
// file type:	h
////////////////////////////////////////////

#ifndef __GYHASHTABLE_H__
#define __GYHASHTABLE_H__
#include "GYCommonDefine.h"
#include "GYString.h"
#include <malloc.h>

//下面提供插入操作测试数据
//所有数据均成功插入
//key类型为GYGUID
//value类型为GYINT32
//数据来源为n个不重复的key
//n = 1024 × 32
//m_nTableSize = 32 * 2
//m_nBucketSize = 1024
//冲突次数为8952
//探测步数最多为28，发生1次
//耗时16ms

//n = 1024 × 32
//m_nTableSize = 32 * 4
//m_nBucketSize = 1024
//冲突次数为4526
//探测步数最多为9，发生1次
//耗时< 1ms
//
//建议配置为
//m_nBucketSize = 1024
//m_nTableSize = 2 × n / 1024
///////////////////////////
template<typename T>
class GYHashTable
{
	struct  __Hash
	{
		GYUINT64 HashValue3;
		T value;
		__Hash()
		{
			CleanUp();
		}
		GYVOID CleanUp()
		{
			HashValue3		= INVALID_VALUE;
		}
	};
	
	__Hash** m_pHashTable;
	GYINT32	m_nTableSize;
	GYINT32 m_nBucketSize;

	GYUINT64 _HashFunction1(const GYCHAR* key, GYINT32 len)
	{
		GYUINT64 hash = 0;
		for (GYINT32 i = 0; i <= len; ++i) 
		{
			hash = ( hash * 31 + key[i]);
		}
		return hash;
	}

	GYUINT64 _HashFunction2(const char* key)
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

	GYUINT64 _HashFunction3(const char* key)
	{
		GYUINT32 hash = 5381;
		while(*key)
		{
			hash += (hash << 5) + (*key++);
		} 
		return  (hash & 0x7FFFFFFF);
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
		printf("%d\n", sizeof(__Hash));
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
		GYCHAR key__[5] = {0};
		*((GYINT32*)key__) = key;
		GYINT32 err = Insert(key__, sizeof(key), value);
		return err;
	}

	GYINT32 Insert(const GYGUID& guid, const T& value)
	{
		GYCHAR key__[GYGUIDLEN+1] = {0};
		*(reinterpret_cast<GYGUID*>(key__)) = guid;
		GYINT32 err = Insert(key__, sizeof(guid), value);
		return err;
	}

	GYINT32 Insert(const GYString& key, const T& value)
	{
		GYINT32 err = Insert(key.c_str(), key.length(), value);
		return err;
	}

	GYINT32 Insert(const char* key, GYINT32 keyLen, const T& value)
	{
		GYINT32 err = INVALID_VALUE;
		GYUINT64 hash_value1 = _HashFunction1(key, keyLen);
		GYINT32 pos1 = hash_value1 % m_nTableSize;
		if(m_pHashTable[pos1])
		{
			GYUINT64 hash_value2 = _HashFunction2(key);
			GYINT32 pos2 = hash_value2 % m_nBucketSize;
			const GYUINT64 hash3Value = _HashFunction3(key);
			if (GYFALSE == m_pHashTable[pos1][pos2].bUsed)
			{
				m_pHashTable[pos1][pos2].bUsed = GYTRUE;
				m_pHashTable[pos1][pos2].HashValue3 = hash3Value;
				m_pHashTable[pos1][pos2].value = value;
				err = 0;
			}
			else
			{
				//static GYINT32 count = 0;
				for (GYINT32 i = 0; i < m_nBucketSize; ++i)
				{
					GYINT32 position = INVALID_VALUE;
					if (pos2 - i >= 0 && INVALID_VALUE == m_pHashTable[pos1][pos2 - i].HashValue3)
					{
						position = pos2 - i;
					}
					else if (pos2 + i < m_nBucketSize && INVALID_VALUE == m_pHashTable[pos1][pos2 + i].HashValue3)
					{
						position = pos2 + i;
					}
					if (INVALID_VALUE != position)
					{
						m_pHashTable[pos1][position].HashValue3 = hash3Value;
						m_pHashTable[pos1][position].value = value;
						//printf("%d\t%d\t%d\t%d\t%d\n", ++count, i, pos1, pos2, position);
						err = 0;
						break;
					}
					if (pos2 - i < 0 && pos2 + i >= m_nBucketSize)
					{
						break;
					}
				}
			}
		}
		return err;
	}

	GYINT32 Remove(GYINT32 key)
	{
		char key__[5] = {0};
		*((GYINT32*)key__) = key;
		GYINT32 err = Remove(key__, sizeof(key));
		return err;
	}

	GYINT32 Remove(const GYGUID& guid)
	{
		char key__[GYGUIDLEN+1] = {0};
		*(reinterpret_cast<GYGUID*>(key__)) = guid;
		GYINT32 err = Remove(key__, sizeof(guid));
		return err;
	}
	
	GYINT32 Remove(const GYString& key)
	{
		GYINT32 err = Remove(key.c_str(), key.length());
		return err;
	}

	GYINT32 Remove(const char* key, GYINT32 keyLen)
	{
		GYINT32 err = INVALID_VALUE;
		GYUINT64 hash_value1 = _HashFunction1(key, keyLen);
		GYINT32 pos1 = hash_value1 % m_nTableSize;
		if(m_pHashTable[pos1])
		{
			GYUINT64 hash_value2 = _HashFunction2(key);
			GYINT32 pos2 = hash_value2 % m_nBucketSize;
			const GYUINT64 hash3Value = _HashFunction3(key);
			if(m_pHashTable[pos1][pos2].HashValue3 == hash3Value&&
				GYTRUE == m_pHashTable[pos1][pos2].bUsed)
			{
				m_pHashTable[pos1][pos2].bUsed = GYFALSE;
				err = 0;
			}
			else
			{
				for (GYINT32 i = 0; i < m_nBucketSize; ++i)
				{
					GYINT32 position = INVALID_VALUE;
					if (pos2 - i >= 0 && hash3Value == m_pHashTable[pos1][pos2 - i].HashValue3)
					{
						position = pos2 - i;
					}
					else if (pos2 + i < m_nBucketSize && hash3Value == m_pHashTable[pos1][pos2 + i].HashValue3)
					{
						position = pos2 + i;
					}
					if (INVALID_VALUE != position)
					{
						m_pHashTable[pos1][position].HashValue3 = INVALID_VALUE;
						err = 0;
						break;
					}
					if (pos2 - i < 0 && pos2 + i >= m_nBucketSize)
					{
						break;
					}
				}
			}
		}
		return err;
	}

	T* Find(GYINT32 key)
	{
		char key__[5] = {0};
		*((GYINT32*)key__) = key;
		T* ret = Find(key__, sizeof(key));
		return ret;
	}

	T* Find(const GYGUID& guid)
	{
		char key__[GYGUIDLEN+1] = {0};
		*(reinterpret_cast<GYGUID*>(key__)) = guid;
		T* ret = Find(key__, sizeof(guid));
		return ret;
	}

	T* Find(const GYString& key)
	{
		T* result = Find(key.c_str(), key.length());
		return result;
	}

	T* Find(const GYCHAR* key, GYINT32 dataLen)
	{
		T* ret = GYNULL;
		GYUINT64 hash_value1 = _HashFunction1(key, dataLen);
		GYINT32 pos1 = hash_value1 % m_nTableSize;
		if(m_pHashTable[pos1])
		{
			GYUINT64 hash_value2 = _HashFunction2(key);
			GYINT32 pos2 = hash_value2 % m_nBucketSize;
			const GYUINT64 hash3Value = _HashFunction3(key);
			if(hash3Value == m_pHashTable[pos1][pos2].HashValue3
				&& m_pHashTable[pos1][pos2].bUsed)
			{
				ret = &m_pHashTable[pos1][pos2].value;
			}
			else
			{
				for (GYINT32 i = 0; i < m_nBucketSize; ++i)
				{
					GYINT32 position = INVALID_VALUE;
					if (pos2 - i >= 0 && hash3Value == m_pHashTable[pos1][pos2 - i].HashValue3)
					{
						position = pos2 - i;
					}
					else if (pos2 + i < m_nBucketSize && hash3Value == m_pHashTable[pos1][pos2 + i].HashValue3)
					{
						position = pos2 + i;
					}
					if (INVALID_VALUE != position)
					{
						ret = &m_pHashTable[pos1][position].value;
						break;
					}
					if (pos2 - i < 0 && pos2 + i >= m_nBucketSize)
					{
						break;
					}
				}
			}
		}
		return ret;
	}
};
#endif
