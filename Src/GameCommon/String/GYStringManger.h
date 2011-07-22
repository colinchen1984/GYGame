/////////////////////////////////////////////
// create time: 2011/7/22 13:35
// author:	colin chen
// file name:	GYStringManger
// file type:	h
////////////////////////////////////////////

#ifndef __GYSTRINGMANGER_H__
#define __GYSTRINGMANGER_H__

#include "GYCommonDefine.h"
#include "GYList.h"

const GYINT32 BufferCount16 = 1024 * 1024;
const GYINT32 BufferCount32 = 1024 * 1024;
const GYINT32 BufferCount64 = 512 * 1024;
const GYINT32 BufferCount128 = 256*1024;
const GYINT32 BufferCount512 = 128*1024;
const GYINT32 BufferCount1024 = 128*1024;

class GYStringManager
{
	template<int bufferSize>
	struct GYStringBuffer
	{
		GYWCHAR m_buffer[bufferSize];
		GYINT32	m_referenceCount;
		GYStringBuffer<bufferSize>*		m_prev;
		GYStringBuffer<bufferSize>*		m_next;
		GYStringBuffer()
		{
			memset(m_buffer, 0, sizeof(m_buffer));
			m_referenceCount = 0;
			m_prev = GYNULL;
			m_next = GYNULL;
		}
	};

	GYList<GYStringBuffer<16>>		m_16poolFree;
	GYList<GYStringBuffer<16>>		m_16poolUsed;
	GYStringBuffer<16>				m_16pooldata[BufferCount16];
	GYList<GYStringBuffer<32>>		m_32poolFree;
	GYList<GYStringBuffer<32>>		m_32poolUsed;
	GYStringBuffer<32>				m_32pooldata[BufferCount32];
	GYList<GYStringBuffer<64>>		m_64poolFree;
	GYList<GYStringBuffer<64>>		m_64poolUsed;
	GYStringBuffer<64>				m_64pooldata[BufferCount64];
	GYList<GYStringBuffer<128>>		m_128poolFree;
	GYList<GYStringBuffer<128>>		m_128poolUsed;
	GYStringBuffer<128>				m_128pooldata[BufferCount128];
	GYList<GYStringBuffer<512>>		m_512poolFree;
	GYList<GYStringBuffer<512>>		m_512poolUsed;
	GYStringBuffer<512>				m_512pooldata[BufferCount512];
	GYList<GYStringBuffer<1024>>	m_1024poolFree;
	GYList<GYStringBuffer<1024>>	m_1024poolUsed;
	GYStringBuffer<1024>			m_1024pooldata[BufferCount1024];
	
public:
	GYStringManager();
	~GYStringManager(){};

	GYINT32		Init();

	GYWCHAR*	AllocateString(const GYWCHAR* str, GYINT32 strLength);

	GYVOID		AddStringReference(GYWCHAR* str, GYINT32 strLength);
	GYVOID		DeleteStringReference(GYWCHAR* str, GYINT32 strLength);

private:
	GYStringBuffer<16>*		_Allocate16();
	GYStringBuffer<32>*		_Allocate32();
	GYStringBuffer<64>*		_Allocate64();
	GYStringBuffer<128>*	_Allocate128();
	GYStringBuffer<512>*	_Allocate512();
	GYStringBuffer<1024>*	_Allocate1024();
};

#endif

