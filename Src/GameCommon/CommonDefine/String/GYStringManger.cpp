/////////////////////////////////////////////
// create time: 2011/7/22 13:35
// author:	colin chen
// file name:	GYStringManger
// file type:	h
////////////////////////////////////////////

#include "GYStringManger.h"


GYStringManager::GYStringManager()
{

}

GYINT32 GYStringManager::Init()
{
	if (0 != m_strHashTable.Init(StringHashTableSize, StringHashTableBucketSize))
	{
		return INVALID_VALUE;
	}
	
	for (GYINT32 i = 0; i < BufferCount16; ++i)
	{
		m_16poolFree.Add(m_16pooldata[i]);
	}
	
	for (GYINT32 i = 0; i < BufferCount32; ++i)
	{
		m_32poolFree.Add(m_32pooldata[i]);
	}

	for (GYINT32 i = 0; i < BufferCount64; ++i)
	{
		m_64poolFree.Add(m_64pooldata[i]);
	}

	for (GYINT32 i = 0; i < BufferCount128; ++i)
	{
		m_128poolFree.Add(m_128pooldata[i]);
	}

	for (GYINT32 i = 0; i < BufferCount512; ++i)
	{
		m_512poolFree.Add(m_512pooldata[i]);
	}

	for (GYINT32 i = 0; i < BufferCount1024; ++i)
	{
		m_1024poolFree.Add(m_1024pooldata[i]);
	}
	
	return 0;
}

GYCHAR* GYStringManager::AllocateString( const GYCHAR* str, GYINT32 strLength )
{
	GYCHAR** findResult = m_strHashTable.Find(str);
	GYCHAR* pString = GYNULL;
	if (GYNULL == findResult)
	{
		GYCHAR* inSertData = GYNULL;
		if (strLength < 16)
		{
			GYStringBuffer<16>* pStringBuffer = _Allocate16();
			if (GYNULL != pStringBuffer)
			{
				inSertData = reinterpret_cast<GYCHAR*>(pStringBuffer);
				m_16poolUsed.Add(*pStringBuffer);
				pStringBuffer->m_referenceCount = 1;
				pString = pStringBuffer->m_buffer;
			}
		}
		else if (strLength >= 16 && strLength <= 32)
		{
			GYStringBuffer<32>* pStringBuffer = _Allocate32();
			if (GYNULL != pStringBuffer)
			{
				inSertData = reinterpret_cast<GYCHAR*>(pStringBuffer);
				m_32poolUsed.Add(*pStringBuffer);
				pStringBuffer->m_referenceCount = 1;
				pString = pStringBuffer->m_buffer;
			}
		}
		else if (strLength >= 32 && strLength <= 64)
		{
			GYStringBuffer<64>* pStringBuffer = _Allocate64();
			if (GYNULL != pStringBuffer)
			{
				inSertData = reinterpret_cast<GYCHAR*>(pStringBuffer);
				m_64poolUsed.Add(*pStringBuffer);
				pStringBuffer->m_referenceCount = 1;
				pString = pStringBuffer->m_buffer;
			}
		}
		else if (strLength >= 64 && strLength <= 128)
		{
			GYStringBuffer<128>* pStringBuffer = _Allocate128();
			if (GYNULL != pStringBuffer)
			{
				inSertData = reinterpret_cast<GYCHAR*>(pStringBuffer);
				m_128poolUsed.Add(*pStringBuffer);
				pStringBuffer->m_referenceCount = 1;
				pString = pStringBuffer->m_buffer;
			}
		}
		else if (strLength >= 128 && strLength <= 512)
		{
			GYStringBuffer<512>* pStringBuffer = _Allocate512();
			if (GYNULL != pStringBuffer)
			{
				inSertData = reinterpret_cast<GYCHAR*>(pStringBuffer);
				m_512poolUsed.Add(*pStringBuffer);
				pStringBuffer->m_referenceCount = 1;
				pString = pStringBuffer->m_buffer;
			}
		}
		else if (strLength >= 512 && strLength <= 1024)
		{
			GYStringBuffer<1024>* pStringBuffer = _Allocate1024();
			if (GYNULL != pStringBuffer)
			{
				inSertData = reinterpret_cast<GYCHAR*>(pStringBuffer);
				m_1024poolUsed.Add(*pStringBuffer);
				pStringBuffer->m_referenceCount = 1;
				pString = pStringBuffer->m_buffer;
			}
		}
		if (GYNULL != pString && GYNULL != inSertData)
		{
			m_strHashTable.Insert(str, inSertData);
			GYMemcpy(pString, str, sizeof(str[0]) * strLength);
			pString[strLength] = 0;
		}
	}
	else
	{
		pString = *findResult;
		++(*reinterpret_cast<GYINT32*>(pString));
		pString += sizeof(GYINT32*) + 2 * sizeof(GYVOID*);
	}
	return pString;
}


GYStringManager::GYStringBuffer<1024>* GYStringManager::_Allocate1024()
{
	return m_1024poolFree.PickUpFirstItem();
}

GYStringManager::GYStringBuffer<512>* GYStringManager::_Allocate512()
{
	if(m_512poolFree.GetItemCount() > 0)
	{
		return m_512poolFree.PickUpFirstItem();
	}
	return GYNULL;
}

GYStringManager::GYStringBuffer<128>* GYStringManager::_Allocate128()
{
	if(m_128poolFree.GetItemCount() > 0)
	{
		return m_128poolFree.PickUpFirstItem();
	}
	
	//尝试从1024 pool内获得内存，然后转成128 pool
	GYStringBuffer<1024>* p1024Pool = _Allocate1024();
	if (GYNULL != p1024Pool)
	{
		GYINT32 dataCount = sizeof(GYStringBuffer<1024>) / sizeof(GYStringBuffer<128>);
		GYStringBuffer<128>* p128Pool = reinterpret_cast<GYStringBuffer<128>*>(p1024Pool);
		for (GYINT32 i = 1; i < dataCount; ++i)
		{
			m_128poolFree.Add(p128Pool[i]);
		}
		return p128Pool;	
	}
	return GYNULL;
}

GYStringManager::GYStringBuffer<64>* GYStringManager::_Allocate64()
{
	if(m_64poolFree.GetItemCount() > 0)
	{
		return m_64poolFree.PickUpFirstItem();
	}

	//尝试从1024 pool内获得内存，然后转成16 pool
	GYStringBuffer<1024>* p1024Pool = _Allocate1024();
	if (GYNULL != p1024Pool)
	{
		GYINT32 dataCount = sizeof(GYStringBuffer<1024>) / sizeof(GYStringBuffer<64>);
		GYStringBuffer<64>* p64Pool = reinterpret_cast<GYStringBuffer<64>*>(p1024Pool);
		for (GYINT32 i = 1; i < dataCount; ++i)
		{
			m_64poolFree.Add(p64Pool[i]);
		}
		return p64Pool;	
	}

	//尝试从1024 pool内获得内存，然后转成16 pool
	GYStringBuffer<512>* p512Pool = _Allocate512();
	if(GYNULL != p512Pool)
	{
		GYINT32 dataCount = sizeof(GYStringBuffer<512>) / sizeof(GYStringBuffer<64>);
		GYStringBuffer<64>* p64Pool = reinterpret_cast<GYStringBuffer<64>*>(p512Pool);
		for (GYINT32 i = 1; i < dataCount; ++i)
		{
			m_64poolFree.Add(p64Pool[i]);
		}
		return p64Pool;		
	}

	return GYNULL;
}

GYStringManager::GYStringBuffer<32>* GYStringManager::_Allocate32()
{
	if(m_32poolFree.GetItemCount() > 0)
	{
		return m_32poolFree.PickUpFirstItem();
	}

	//尝试从1024 pool内获得内存，然后转成16 pool
	GYStringBuffer<1024>* p1024Pool = _Allocate1024();
	if (GYNULL != p1024Pool)
	{
		GYINT32 dataCount = sizeof(GYStringBuffer<1024>) / sizeof(GYStringBuffer<32>);
		GYStringBuffer<32>* p32Pool = reinterpret_cast<GYStringBuffer<32>*>(p1024Pool);
		for (GYINT32 i = 1; i < dataCount; ++i)
		{
			m_32poolFree.Add(p32Pool[i]);
		}
		return p32Pool;	
	}

	//尝试从1024 pool内获得内存，然后转成16 pool
	GYStringBuffer<512>* p512Pool = _Allocate512();
	if(GYNULL != p512Pool)
	{
		GYINT32 dataCount = sizeof(GYStringBuffer<512>) / sizeof(GYStringBuffer<32>);
		GYStringBuffer<32>* p32Pool = reinterpret_cast<GYStringBuffer<32>*>(p512Pool);
		for (GYINT32 i = 1; i < dataCount; ++i)
		{
			m_32poolFree.Add(p32Pool[i]);
		}
		return p32Pool;		
	}

	return GYNULL;
}

GYStringManager::GYStringBuffer<16>* GYStringManager::_Allocate16()
{
	if(m_16poolFree.GetItemCount() > 0)
	{
		return m_16poolFree.PickUpFirstItem();
	}

	//尝试从1024 pool内获得内存，然后转成16 pool
	GYStringBuffer<1024>* p1024Pool = _Allocate1024();
	if (GYNULL != p1024Pool)
	{
		GYINT32 dataCount = sizeof(GYStringBuffer<1024>) / sizeof(GYStringBuffer<16>);
		GYStringBuffer<16>* p16Pool = reinterpret_cast<GYStringBuffer<16>*>(p1024Pool);
		for (GYINT32 i = 1; i < dataCount; ++i)
		{
			m_16poolFree.Add(p16Pool[i]);
		}
		return p16Pool;	
	}

	//尝试从1024 pool内获得内存，然后转成16 pool
	GYStringBuffer<512>* p512Pool = _Allocate512();
	if(GYNULL != p512Pool)
	{
		GYINT32 dataCount = sizeof(GYStringBuffer<512>) / sizeof(GYStringBuffer<16>);
		GYStringBuffer<16>* p16Pool = reinterpret_cast<GYStringBuffer<16>*>(p512Pool);
		for (GYINT32 i = 1; i < dataCount; ++i)
		{
			m_16poolFree.Add(p16Pool[i]);
		}
		return p16Pool;		
	}

	return GYNULL;
}

GYVOID GYStringManager::DeleteStringReference( GYCHAR* str, GYINT32 strLength )
{
	GYCHAR** targetStr = m_strHashTable.Find(str);
	if (GYNULL == targetStr)
	{
		//该字符串没有注册过
		//或者hash表有bug？
		GYAssert(GYFALSE);
		return;
	}
	if (strLength < 16)
	{
		GYStringBuffer<16>* pStringBuffer = reinterpret_cast<GYStringBuffer<16>*>(*targetStr);
		--pStringBuffer->m_referenceCount;
		if (pStringBuffer->m_referenceCount <= 0)
		{
			m_strHashTable.Remove(str);
			m_16poolUsed.Delete(*pStringBuffer);
			m_16poolFree.Add(*pStringBuffer);
		}
		
	}
	else if (strLength >= 16 && strLength < 32)
	{
		GYStringBuffer<32>* pStringBuffer = reinterpret_cast<GYStringBuffer<32>*>(*targetStr);
		--pStringBuffer->m_referenceCount;
		if (pStringBuffer->m_referenceCount <= 0)
		{
			m_strHashTable.Remove(str);
			m_32poolUsed.Delete(*pStringBuffer);
			m_32poolFree.Add(*pStringBuffer);
		}
	}
	else if (strLength >= 32 && strLength < 64)
	{
		GYStringBuffer<64>* pStringBuffer = reinterpret_cast<GYStringBuffer<64>*>(*targetStr);
		--pStringBuffer->m_referenceCount;
		if (pStringBuffer->m_referenceCount <= 0)
		{
			m_strHashTable.Remove(str);
			m_64poolUsed.Delete(*pStringBuffer);
			m_64poolFree.Add(*pStringBuffer);
		}
	}
	else if (strLength >= 64 && strLength < 128)
	{
		GYStringBuffer<128>* pStringBuffer = reinterpret_cast<GYStringBuffer<128>*>(*targetStr);
		--pStringBuffer->m_referenceCount;
		if (pStringBuffer->m_referenceCount <= 0)
		{
			m_strHashTable.Remove(str);
			m_128poolUsed.Delete(*pStringBuffer);
			m_128poolFree.Add(*pStringBuffer);
		}
	}
	else if (strLength >= 128 && strLength < 512)
	{
		GYStringBuffer<512>* pStringBuffer = reinterpret_cast<GYStringBuffer<512>*>(*targetStr);
		--pStringBuffer->m_referenceCount;
		if (pStringBuffer->m_referenceCount <= 0)
		{
			m_strHashTable.Remove(str);
			m_512poolUsed.Delete(*pStringBuffer);
			m_512poolFree.Add(*pStringBuffer);
		}
	}
	else if (strLength >= 512 && strLength < 1024)
	{
		GYStringBuffer<1024>* pStringBuffer = reinterpret_cast<GYStringBuffer<1024>*>(*targetStr);
		--pStringBuffer->m_referenceCount;
		if (pStringBuffer->m_referenceCount <= 0)
		{
			m_strHashTable.Remove(str);
			m_1024poolUsed.Delete(*pStringBuffer);
			m_1024poolFree.Add(*pStringBuffer);
		}
	}
}

StringMangerSingleton& StringMangerSingleton::GetSingleton()
{
	static StringMangerSingleton singleon;
	return singleon;
}
