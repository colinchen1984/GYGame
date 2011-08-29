﻿/////////////////////////////////////////////
// create time: 2011/8/28 20:55
// author:	colin chen
// file name:	GYStreamSerialization
// file type:	h
////////////////////////////////////////////

#ifndef __GYSTREAMSERIALIZATION_H__
#define __GYSTREAMSERIALIZATION_H__
#include "GYSerialization.h"
#include "GYString.h"
#include "GYCycleBuffer.h"
#include "GYProtocolDefine.h"
enum EM_SERIALIZAION_MODE
{
	EM_SERIALIZAION_MODE_INVALID = -1,
	EM_SERIALIZAION_MODE_READ,
	EM_SERIALIZAION_MODE_WRITE,
	EM_SERIALIZAION_MODE_COUNT,
};

template< GYINT32 CYCLE_BUFFER_LEN>
class GYStreamSerialization : public GYSerializationInteface
{
	GYCycleBuffer<CYCLE_BUFFER_LEN>&	m_buffer;
	EM_SERIALIZAION_MODE				m_mode;
	GYINT32								m_serializDataSize;
public:
	explicit GYStreamSerialization(GYCycleBuffer<CYCLE_BUFFER_LEN>& buffer, EM_SERIALIZAION_MODE mode):m_buffer(buffer)
	{
		m_serializDataSize = 0;
		m_mode = mode;
	}
	virtual ~GYStreamSerialization(){};

	GYINT32 GetSerializDataSize()
	{
		return m_serializDataSize;
	}

	virtual GYSerializationInteface& operator<<(GYCHAR& value)
	{
		switch(m_mode)
		{
		case EM_SERIALIZAION_MODE_READ:
			{
				GYAssert(0 == m_buffer.Read(&value, sizeof(value)));
			}
			break;
		case EM_SERIALIZAION_MODE_WRITE:
			{
				GYAssert(0 == m_buffer.Write(&value, sizeof(value)));
			}
			break;
		}
		m_serializDataSize += sizeof(value);
		return *this;
	}

	virtual GYSerializationInteface& operator<<(GYUINT8& value)
	{
		switch(m_mode)
		{
		case EM_SERIALIZAION_MODE_READ:
			{
				GYAssert(0 == m_buffer.Read(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		case EM_SERIALIZAION_MODE_WRITE:
			{
				GYAssert(0 == m_buffer.Write(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		}
		m_serializDataSize += sizeof(value);
		return *this;
	}

	virtual GYSerializationInteface& operator<<(GYINT16& value)
	{
		switch(m_mode)
		{
		case EM_SERIALIZAION_MODE_READ:
			{
				GYAssert(0 == m_buffer.Read(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		case EM_SERIALIZAION_MODE_WRITE:
			{
				GYAssert(0 == m_buffer.Write(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		}
		m_serializDataSize += sizeof(value);
		return *this;
	}

	virtual GYSerializationInteface& operator<<(GYUINT16& value)
	{
		switch(m_mode)
		{
		case EM_SERIALIZAION_MODE_READ:
			{
				GYAssert(0 == m_buffer.Read(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		case EM_SERIALIZAION_MODE_WRITE:
			{
				GYAssert(0 == m_buffer.Write(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		}
		m_serializDataSize += sizeof(value);
		return *this;
	}

	virtual GYSerializationInteface& operator<<(GYINT32& value)
	{
		switch(m_mode)
		{
		case EM_SERIALIZAION_MODE_READ:
			{
				GYAssert(0 == m_buffer.Read(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		case EM_SERIALIZAION_MODE_WRITE:
			{
				GYAssert(0 == m_buffer.Write(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		}
		m_serializDataSize += sizeof(value);
		return *this;
	}

	virtual GYSerializationInteface& operator<<(GYUINT32& value)
	{
		switch(m_mode)
		{
		case EM_SERIALIZAION_MODE_READ:
			{
				GYAssert(0 == m_buffer.Read(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		case EM_SERIALIZAION_MODE_WRITE:
			{
				GYAssert(0 == m_buffer.Write(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		}
		m_serializDataSize += sizeof(value);
		return *this;
	}

	virtual GYSerializationInteface& operator<<(GYINT64& value)
	{
		switch(m_mode)
		{
		case EM_SERIALIZAION_MODE_READ:
			{
				GYAssert(0 == m_buffer.Read(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		case EM_SERIALIZAION_MODE_WRITE:
			{
				GYAssert(0 == m_buffer.Write(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		}
		m_serializDataSize += sizeof(value);
		return *this;
	}

	virtual GYSerializationInteface& operator<<(GYFLOAT& value)
	{
		switch(m_mode)
		{
		case EM_SERIALIZAION_MODE_READ:
			{
				GYAssert(0 == m_buffer.Read(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		case EM_SERIALIZAION_MODE_WRITE:
			{
				GYAssert(0 == m_buffer.Write(reinterpret_cast<GYCHAR*>(&value), sizeof(value)));
			}
			break;
		}
		m_serializDataSize += sizeof(value);
		return *this;
	}

	virtual GYSerializationInteface& operator<<(GYString& value)
	{
		GYINT16 strlen = value.length();
		const GYCHAR* str = value.c_str();
		switch(m_mode)
		{
		case EM_SERIALIZAION_MODE_READ:
			{
				GYAssert(0 == m_buffer.Read(reinterpret_cast<GYCHAR*>(&strlen), sizeof(strlen)));
				GYAssert(1024 > strlen);
				GYCHAR tempBuffer[1024] = {0};
				GYAssert(0 == m_buffer.Read(tempBuffer, strlen));
				value = tempBuffer;
			}
			break;
		case EM_SERIALIZAION_MODE_WRITE:
			{
				GYAssert(0 == m_buffer.Write(reinterpret_cast<GYCHAR*>(&strlen), sizeof(strlen)));
				GYAssert(0 == m_buffer.Write(str, strlen));
			}
			break;
		}
		m_serializDataSize += sizeof(strlen) + strlen;
		return *this;
	}

	GYSerializationInteface& operator<<(GYPacketInteface& packet)
	{
		packet.Serializ(*this);
		return *this;
	}
};
#endif
