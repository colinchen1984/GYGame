/////////////////////////////////////////////
// create time: 2011/9/3 18:45
// author:	colin chen
// file name:	GYTableSerialization
// file type:	cpp
////////////////////////////////////////////
#include "GYTableSerialization.h"
#include "GYString.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

static const GYCHAR UnixNewLine = '\n';
static const GYCHAR WindowsNewLineStart = '\r';
static const GYCHAR* const TableFileLoadMode = "rb";
static const GYCHAR DataSplitChar = '\t';
static const GYCHAR LineCommentChar = '#';

GYTableSerialization::GYTableSerialization()
{
	_CleanUp();
}

GYTableSerialization::~GYTableSerialization()
{
}

static const GYINT32 bufferForTableFileLoadLen = 1024 * 1024 * 2;
static GYCHAR BufferForTableFileLoad[bufferForTableFileLoadLen] = {0};
GYINT32 GYTableSerialization::Init( const GYString& fileName )
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		_CleanUp();

		struct stat fileInfo;
		if(stat(fileName.c_str(), &fileInfo) < 0)
		{
			break;
		}
		GYAssert(bufferForTableFileLoadLen > fileInfo.st_size);
		m_fileSize = fileInfo.st_size;
		FILE* pFile = fopen(fileName.c_str(), TableFileLoadMode);
		if (GYNULL == pFile)
		{
			break;
		}
		
		if(1 != fread(BufferForTableFileLoad, m_fileSize, 1, pFile))
		{
			break;
		}
		BufferForTableFileLoad[m_fileSize] = 0;
		const GYCHAR* pDataStart = BufferForTableFileLoad;
		static const GYCHAR BOM[3] ={(GYCHAR)0xef, (GYCHAR)0xbb, (GYCHAR)0xbf} ;
		if (0 == GYMemcmp(BufferForTableFileLoad, BOM, 3))
		{
			pDataStart += 3;
		}
		
		const GYCHAR* nestLine = pDataStart;
		while(GYNULL != (nestLine = _GetLine(nestLine)))
		{
			++m_tableRowCount;
		}

		//至少要有三行数据
		//第一行注释
		//第二行格式
		//第三行数据
		if (3 > m_tableRowCount)
		{
			break;
		}
		//跳过第一行数据

		//处理第二行数据
		nestLine =  _GetLine(pDataStart);
		const GYCHAR* pLineStart = nestLine;
		while(nestLine >= (pLineStart = _GetDataString(pLineStart)))
		{
			++m_tableColumCount;
		}
		
		//得到第三行数据，也就是表内有效数据的开始
		nestLine =  _GetLine(nestLine);
		
		m_dataStart = nestLine;
		result = 0;
	} while (GYFALSE);
	return result;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYCHAR& value)
{
	GYAssert(GYFALSE);
	return *this;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYUINT8& value)
{
	GYAssert(GYFALSE);
	return *this;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYINT16& value)
{
	GYAssert(GYFALSE);
	return *this;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYUINT16& value)
{
	GYAssert(GYFALSE);
	return *this;
}

static const GYINT32 StringToDataBufferLen = 1024;
static GYCHAR StringToDataBuffer[StringToDataBufferLen] = {0};
GYSerializationInteface& GYTableSerialization::operator<<(GYINT32& value)
{
	GYAssert(&m_dataStart[m_serializationDataCount] < &BufferForTableFileLoad[m_fileSize]);
	const GYCHAR* const pDataStart = &m_dataStart[m_serializationDataCount];
	const GYCHAR* pDataEnd = _GetDataString(pDataStart);
	if (GYNULL != pDataEnd)
	{
		GYINT32 readDataCount = pDataEnd - pDataStart;
		if(DataSplitChar == *(pDataEnd - 1) || UnixNewLine == *(pDataEnd -1))
		{
			--pDataEnd;
			if (WindowsNewLineStart == *(pDataEnd -1))
			{
				--pDataEnd;
			}
			GYINT32 datalen = pDataEnd - pDataStart;
			GYAssert(datalen < StringToDataBufferLen);
			GYMemcpy(StringToDataBuffer, pDataStart, datalen);
			StringToDataBuffer[datalen] = 0;
			value = atoi(StringToDataBuffer);
		}		
		else if (0 == *pDataEnd)
		{
			value = atoi(pDataStart);
		}
		else 
		{
			GYAssert(GYFALSE);
		}
		m_serializationDataCount += readDataCount;
	}
	else 
	{
		GYAssert(GYFALSE);
	}
	return *this;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYUINT32& value)
{
	GYINT64 tempValue = 0;
	*this << tempValue;
	value = static_cast<GYUINT32>(tempValue);
	return *this;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYINT64& value)
{
	GYAssert(&m_dataStart[m_serializationDataCount] < &BufferForTableFileLoad[m_fileSize]);
	const GYCHAR* const pDataStart = &m_dataStart[m_serializationDataCount];
	const GYCHAR* pDataEnd = _GetDataString(pDataStart);
	if (GYNULL != pDataEnd)
	{
		GYINT32 readDataCount = pDataEnd - pDataStart;
		if(DataSplitChar == *(pDataEnd - 1) || UnixNewLine == *(pDataEnd -1))
		{
			--pDataEnd;
			if (WindowsNewLineStart == *(pDataEnd -1))
			{
				--pDataEnd;
			}
			GYINT32 datalen = pDataEnd - pDataStart;
			GYAssert(datalen < StringToDataBufferLen);
			GYMemcpy(StringToDataBuffer, pDataStart, datalen);
			StringToDataBuffer[datalen] = 0;
#ifdef WIN32
			value = _atoi64(StringToDataBuffer);
#else 
			value = strtol(StringToDataBuffer, GYNULL, 10);
#endif
		}		
		else if (0 == *pDataEnd)
		{
#ifdef WIN32
			value = _atoi64(pDataStart);
#else 
			value = strtol(pDataStart, GYNULL, 10);
#endif
		}
		else 
		{
			GYAssert(GYFALSE);
		}
		m_serializationDataCount += readDataCount;
	}
	else 
	{
		GYAssert(GYFALSE);
	}
	return *this;
}


GYSerializationInteface& GYTableSerialization::operator<<( GYUINT64& value )
{
	GYAssert(&m_dataStart[m_serializationDataCount] < &BufferForTableFileLoad[m_fileSize]);
	const GYCHAR* const pDataStart = &m_dataStart[m_serializationDataCount];
	const GYCHAR* pDataEnd = _GetDataString(pDataStart);
	if (GYNULL != pDataEnd)
	{
		GYINT32 readDataCount = pDataEnd - pDataStart;
		if(DataSplitChar == *(pDataEnd - 1) || UnixNewLine == *(pDataEnd -1))
		{
			--pDataEnd;
			if (WindowsNewLineStart == *(pDataEnd -1))
			{
				--pDataEnd;
			}
			GYINT32 datalen = pDataEnd - pDataStart;
			GYAssert(datalen < StringToDataBufferLen);
			GYMemcpy(StringToDataBuffer, pDataStart, datalen);
			StringToDataBuffer[datalen] = 0;
#ifdef WIN32
			value = _atoi64(StringToDataBuffer);
#else 
			value = strtoull(StringToDataBuffer, GYNULL, 10);
#endif
		}		
		else if (0 == *pDataEnd)
		{
#ifdef WIN32
			value = _atoi64(pDataStart);
#else 
			value = strtoull(pDataStart, GYNULL, 10);
#endif
		}
		else 
		{
			GYAssert(GYFALSE);
		}
		m_serializationDataCount += readDataCount;
	}
	else 
	{
		GYAssert(GYFALSE);
	}
	return *this;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYFLOAT& value)
{
	GYAssert(&m_dataStart[m_serializationDataCount] < &BufferForTableFileLoad[m_fileSize]);
	const GYCHAR* const pDataStart = &m_dataStart[m_serializationDataCount];
	const GYCHAR* pDataEnd = _GetDataString(pDataStart);
	if (GYNULL != pDataEnd)
	{
		GYINT32 readDataCount = pDataEnd - pDataStart;
		if(DataSplitChar == *(pDataEnd - 1) || UnixNewLine == *(pDataEnd -1))
		{
			--pDataEnd;
			if (WindowsNewLineStart == *(pDataEnd -1))
			{
				--pDataEnd;
			}
			GYINT32 datalen = pDataEnd - pDataStart;
			GYAssert(datalen < StringToDataBufferLen);
			GYMemcpy(StringToDataBuffer, pDataStart, datalen);
			StringToDataBuffer[datalen] = 0;
			value = static_cast<GYFLOAT>(atof(StringToDataBuffer));
		}		
		else if (0 == *pDataEnd)
		{
			value = static_cast<GYFLOAT>(atof(pDataStart));
		}
		else 
		{
			GYAssert(GYFALSE);
		}
		m_serializationDataCount += readDataCount;
	}
	else 
	{
		GYAssert(GYFALSE);
	}
	return *this;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYString& value)
{
	GYAssert(&m_dataStart[m_serializationDataCount] < &BufferForTableFileLoad[m_fileSize]);
	const GYCHAR* const pDataStart = &m_dataStart[m_serializationDataCount];
	const GYCHAR* pDataEnd = _GetDataString(pDataStart);
	if (GYNULL != pDataEnd)
	{
		GYINT32 readDataCount = pDataEnd - pDataStart;
		if(DataSplitChar == *(pDataEnd - 1) || UnixNewLine == *(pDataEnd -1))
		{
			--pDataEnd;
			if (WindowsNewLineStart == *(pDataEnd -1))
			{
				--pDataEnd;
			}
			GYINT32 datalen = pDataEnd - pDataStart;
			GYAssert(datalen < StringToDataBufferLen);
			GYMemcpy(StringToDataBuffer, pDataStart, datalen);
			StringToDataBuffer[datalen] = 0;
			value = StringToDataBuffer;
		}		
		else if (0 == *pDataEnd)
		{
			value = pDataStart;
		}
		else 
		{
			GYAssert(GYFALSE);
		}
		m_serializationDataCount += readDataCount;
	}
	else 
	{
		GYAssert(GYFALSE);
	}
	return *this;
}

const GYCHAR* GYTableSerialization::_GetLine( const GYCHAR* pData )
{
	const GYCHAR* pResult = GYNULL;
	const GYCHAR* const pLineHead = pData;
	do
	{
		while(GYNULL != pData && 0 != *pData && UnixNewLine != *pData)
		{
			++pData;
		}
		if (GYNULL != pData)
		{
			if(UnixNewLine == *pData)
			{
				pResult = ++pData;
			}
			else if(0 == *pData && pLineHead != pData)
			{
				pResult = pData;
			}
		}
		if (GYNULL != pResult && LineCommentChar == *pResult)
		{
			pResult = _GetLine(pResult);
		}
		
	}while(GYFALSE);
	return pResult;
}

const GYCHAR* GYTableSerialization::_GetDataString( const GYCHAR* pData )
{
	const GYCHAR* pResult = GYNULL;
	const GYCHAR* const pLineHead = pData;
	do
	{
		while(GYNULL != pData && 0 != *pData && UnixNewLine != *pData && DataSplitChar != *pData)
		{
			++pData;
		}
		if (GYNULL != pData)
		{
			if(DataSplitChar == *pData || UnixNewLine == *pData)
			{
				pResult = ++pData;
			}
			else if(0 == *pData && pLineHead != pData)
			{
				pResult = pData;
			}
		}
	}while(GYFALSE);
	return pResult;
}

GYVOID GYTableSerialization::_CleanUp()
{
	m_tableRowCount = 0;
	m_tableColumCount = 0;
	m_fileSize = 0;
	m_serializationDataCount = 0;
	m_dataStart = GYNULL;
}
