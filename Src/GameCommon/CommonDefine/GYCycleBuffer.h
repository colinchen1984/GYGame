/////////////////////////////////////////////
// create time:2011/6/13 12:58
// author:	colin chen
// file name:	GYCycleBuffer
// file type:	h
////////////////////////////////////////////
#ifndef __GYCYCLEBUFFER_H__
#define __GYCYCLEBUFFER_H__

//include file
#include <memory.h>
#include "GYCommonDefine.h"

//using declare

//foward declare

class GYCycleBufferBase
{
public:
	GYCycleBufferBase(){};
	virtual ~GYCycleBufferBase(){};
	
	virtual const GYCHAR* ReadPtr() = 0;
	
    virtual GYINT32 GetWriteSize() = 0;

    virtual GYINT32 GetReadSize() = 0;

    virtual GYINT32 CleanUp() = 0;

    virtual const GYCHAR* const GetHead() = 0;

    virtual const GYCHAR* const GetTail() = 0;

    virtual GYINT32 WritePtr(GYINT32 n) = 0;

    virtual GYINT32 Write(const GYCHAR* p, GYINT32 nLen) = 0;
	
    virtual GYINT32 ReadPtr(GYINT32 n) = 0;

    virtual GYINT32 Read(GYCHAR* p, GYINT32 len) = 0;
};

template<GYINT32 MAX_BUFFER_LEN = 1024 * 16>
class GYCycleBuffer
{
    GYCHAR		m_pBuffer[MAX_BUFFER_LEN];
    GYCHAR*		m_pReader;
    GYCHAR*		m_pWriter;
    GYCHAR*		m_pHeader;
    GYCHAR*		m_pTail;
    GYINT32		m_nBufferLen;
public:
	GYCycleBuffer()
    {
        m_pReader = m_pBuffer;
        m_pWriter = m_pBuffer;
        m_pHeader = m_pBuffer;
        m_pTail = m_pBuffer + sizeof(m_pBuffer);
        m_nBufferLen = sizeof(m_pBuffer);
    };

    ~GYCycleBuffer()
    {};

    GYCHAR* WritePtr()
    {
        return m_pWriter;
    }

    const GYCHAR* ReadPtr()
    {
        return m_pReader;
    }

    GYINT32 GetWriteSize()
    {
        const GYCHAR* const pWriter = m_pWriter;
        GYINT32 nWriteBufferSize = pWriter >= m_pReader ? m_nBufferLen - (pWriter - m_pReader): m_pReader - pWriter;
        return nWriteBufferSize;
    }

    GYINT32 GetReadSize()
    {
        const GYCHAR* const pWriter = m_pWriter;
        GYINT32 nReadBufferSize = 0;
        if(m_pReader >= pWriter)
		{
			nReadBufferSize = m_nBufferLen - (m_pReader - pWriter);
		}
        else if(m_pReader < pWriter)
		{
			nReadBufferSize = (pWriter - m_pReader);
		}
        return nReadBufferSize;
    }

    GYINT32 CleanUp()
    {
        m_pWriter = m_pHeader;
        m_pReader = m_pHeader;
        return 0;
    }

    const GYCHAR* const GetHead()
    {
        return m_pHeader;
    }

    const GYCHAR* const GetTail()
    {
        return m_pTail;
    }

    GYINT32 WritePtr(GYINT32 n)
    {
        GYINT32 err = INVALID_VALUE;
        if(GetWriteSize() >= n)
        {
            if(m_pWriter + n >= m_pTail)
			{
				m_pWriter = m_pHeader + (n - (m_pTail - m_pWriter));
			}
            else
			{
				m_pWriter += n;
			}
			if(m_pWriter == m_pReader)
			{
				m_pWriter = m_pReader = m_pHeader;
			}
            err = 0;
        }
        return err;
    }

    GYINT32 Write(const GYCHAR* p, GYINT32 nLen)
    {
        GYINT32 err = INVALID_VALUE;
        if(p && GetWriteSize() >= nLen)
        {
            if(m_pWriter + nLen <= m_pTail)
			{
				GYMemcpy(m_pWriter, p, nLen);
			}
            else
            {
                GYMemcpy(m_pWriter, p, m_pTail - m_pWriter);
                GYMemcpy(m_pHeader, p + (m_pTail - m_pWriter), nLen - (m_pTail - m_pWriter));
            }
            WritePtr(nLen);
            err = 0;
        }
        return err;
    }

    GYINT32 ReadPtr(GYINT32 n)
    {
        GYINT32 err = INVALID_VALUE;
        if(GetReadSize() >= n)
        {
            if(m_pReader + n >= m_pTail)
			{
				m_pReader = m_pHeader + (n - (m_pTail - m_pReader));
			}
            else
			{
				m_pReader += n;
			}
			if(m_pWriter == m_pReader)
			{
				m_pWriter = m_pReader = m_pHeader;
			}
            err = 0;
        }
        return err;
    }

    GYINT32 Read(GYCHAR* p, GYINT32 len)
    {
        GYINT32 err = INVALID_VALUE;
        if(p && GetReadSize() >= len)
        {
            if(m_pReader + len <= m_pTail)
			{
				GYMemcpy(p, m_pReader, len);
			}
            else
            {
                GYMemcpy(p, m_pReader, m_pTail - m_pReader);
                GYMemcpy(p + (m_pTail - m_pReader), m_pHeader, len - (m_pTail - m_pReader));
            }
            ReadPtr(len);
            err = 0;
        }
        return err;
    }
};

#endif

