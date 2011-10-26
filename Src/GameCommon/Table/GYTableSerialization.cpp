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
#include <string.h>
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
#ifdef CHECK_DATA_TYPE
	GYDelete[] m_dataType;
	m_dataType = GYNULL;
	m_loadColumCount = 0;
#endif
}

#ifdef CHECK_DATA_TYPE
enum EM_TABLE_DATA_TYPE
{
	EM_TABLE_DATA_TYPE_INVALID = -1,
	EM_TABLE_DATA_TYPE_INT,
	EM_TABLE_DATA_TYPE_UINT,
	EM_TABLE_DATA_TYPE_INT64,
	EM_TABLE_DATA_TYPE_UINT64,
	EM_TABLE_DATA_TYPE_FLOAT,
	EM_TABLE_DATA_TYPE_STRING,
	EM_TABLE_DATA_TYPE_COUNT,
};

static const GYCHAR* DataTypeString[EM_TABLE_DATA_TYPE_COUNT] = 
{
	"INT",
	"UINT",
	"INT64",
	"UINT64",
	"FLOAT",
	"STR",
};
#endif

static const GYINT32 bufferForTableFileLoadLen = 1024 * 1024 * 2;
static GYCHAR BufferForTableFileLoad[bufferForTableFileLoadLen] = {0};
GYINT32 GYTableSerialization::Init( const GYCHAR* fileName )
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if (GYNULL == fileName)
		{
			break;
		}
		_CleanUp();

		struct stat fileInfo;
		if(stat(fileName, &fileInfo) < 0)
		{
			break;
		}
		GYAssert(bufferForTableFileLoadLen > fileInfo.st_size);
		m_fileSize = fileInfo.st_size;
		FILE* pFile = fopen(fileName, TableFileLoadMode);
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
		if (0 == GYMemcmp(BufferForTableFileLoad, BOM, sizeof(BOM)))
		{
			pDataStart += 3;
		}
		
		const GYCHAR* nestLine = pDataStart;
		while(GYNULL != (nestLine = _GetLine(nestLine, GYTRUE)))
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

		nestLine =  _GetLine(pDataStart, GYTRUE);
		const GYCHAR* pDataStringStart = nestLine;
		const GYCHAR* pSecondRowEnd = _GetLine(pDataStringStart, GYFALSE);


		//处理第二行数据
		while(pSecondRowEnd >= (pDataStringStart = _GetDataString(pDataStringStart)))
		{
			++m_tableColumCount;
		}
#ifdef CHECK_DATA_TYPE
		pDataStringStart = nestLine;
		m_dataType = GYNew EM_TABLE_DATA_TYPE[m_tableColumCount];
		const GYCHAR* pDataStringEnd = GYNULL;
		for (GYINT32 i = 0; i < m_tableColumCount; ++i)
		{
			pDataStringEnd = _GetDataString(pDataStringStart);
			pDataStringStart = _GetDataString(pDataStringStart, pDataStringEnd);
			for (GYINT32 t = 0; t < EM_TABLE_DATA_TYPE_COUNT; ++t)
			{
				if (0 == strcmp(pDataStringStart, DataTypeString[t]))
				{
					m_dataType[i] = static_cast<EM_TABLE_DATA_TYPE>(t);
					break;
				}
			}
			pDataStringStart = pDataStringEnd;
		}
#endif
		//得到第三行数据，也就是表内有效数据的开始
		nestLine =  _GetLine(nestLine, GYTRUE);
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

GYSerializationInteface& GYTableSerialization::operator<<(GYINT32& value)
{
	GYAssert(&m_dataStart[m_serializationDataCount] < &BufferForTableFileLoad[m_fileSize]);
#ifdef CHECK_DATA_TYPE
	GYAssert(EM_TABLE_DATA_TYPE_INT == m_dataType[m_loadColumCount++]);
#endif
	const GYCHAR* pDataStart = &m_dataStart[m_serializationDataCount];
	const GYCHAR* pDataEnd = _GetDataString(pDataStart);
	GYINT32 readDataCount = pDataEnd - pDataStart;
	GYAssert(readDataCount > 1);
	const GYCHAR* pData = _GetDataString(pDataStart, pDataEnd);
	GYAssert(GYNULL != pData);
	value = atoi(pData);
	m_serializationDataCount += readDataCount;
	return *this;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYUINT32& value)
{
	GYAssert(&m_dataStart[m_serializationDataCount] < &BufferForTableFileLoad[m_fileSize]);
#ifdef CHECK_DATA_TYPE
	GYAssert(EM_TABLE_DATA_TYPE_UINT == m_dataType[m_loadColumCount++]);
#endif
	const GYCHAR* pDataStart = &m_dataStart[m_serializationDataCount];
	const GYCHAR* pDataEnd = _GetDataString(pDataStart);
	GYINT32 readDataCount = pDataEnd - pDataStart;
	GYAssert(readDataCount > 1);
	const GYCHAR* pData = _GetDataString(pDataStart, pDataEnd);
	GYAssert(GYNULL != pData);
	sscanf(pDataStart, "%llu", &value);
	m_serializationDataCount += readDataCount;
	return *this;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYINT64& value)
{
	GYAssert(&m_dataStart[m_serializationDataCount] < &BufferForTableFileLoad[m_fileSize]);
#ifdef CHECK_DATA_TYPE
	GYAssert(EM_TABLE_DATA_TYPE_INT64 == m_dataType[m_loadColumCount++]);
#endif
	const GYCHAR* pDataStart = &m_dataStart[m_serializationDataCount];
	const GYCHAR* pDataEnd = _GetDataString(pDataStart);
	GYINT32 readDataCount = pDataEnd - pDataStart;
	GYAssert(readDataCount > 1);
	const GYCHAR* pData = _GetDataString(pDataStart, pDataEnd);
	GYAssert(GYNULL != pData);
	sscanf(pDataStart, "%ll", &value);
	m_serializationDataCount += readDataCount;
	return *this;
}


GYSerializationInteface& GYTableSerialization::operator<<( GYUINT64& value )
{
	GYAssert(&m_dataStart[m_serializationDataCount] < &BufferForTableFileLoad[m_fileSize]);
#ifdef CHECK_DATA_TYPE
	GYAssert(EM_TABLE_DATA_TYPE_UINT64 == m_dataType[m_loadColumCount++]);
#endif
	const GYCHAR* pDataStart = &m_dataStart[m_serializationDataCount];
	const GYCHAR* pDataEnd = _GetDataString(pDataStart);
	GYINT32 readDataCount = pDataEnd - pDataStart;
	GYAssert(readDataCount > 1);
	const GYCHAR* pData = _GetDataString(pDataStart, pDataEnd);
	GYAssert(GYNULL != pData);
#ifdef WIN32
	value = _atoi64(pData);
#else 
	value = strtoull(pData, GYNULL, 10);
#endif
	m_serializationDataCount += readDataCount;
	return *this;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYFLOAT& value)
{
	GYAssert(&m_dataStart[m_serializationDataCount] < &BufferForTableFileLoad[m_fileSize]);
#ifdef CHECK_DATA_TYPE
	GYAssert(EM_TABLE_DATA_TYPE_FLOAT == m_dataType[m_loadColumCount++]);
#endif
	const GYCHAR* pDataStart = &m_dataStart[m_serializationDataCount];
	const GYCHAR* pDataEnd = _GetDataString(pDataStart);
	GYINT32 readDataCount = pDataEnd - pDataStart;
	GYAssert(readDataCount > 1);
	const GYCHAR* pData = _GetDataString(pDataStart, pDataEnd);
	GYAssert(GYNULL != pData);
	value = static_cast<GYFLOAT>(atof(pData));
	m_serializationDataCount += readDataCount;
	return *this;
}

GYSerializationInteface& GYTableSerialization::operator<<(GYString& value)
{
	GYAssert(&m_dataStart[m_serializationDataCount] < &BufferForTableFileLoad[m_fileSize]);
#ifdef CHECK_DATA_TYPE
	GYAssert(EM_TABLE_DATA_TYPE_STRING == m_dataType[m_loadColumCount++]);
#endif
	const GYCHAR* pDataStart = &m_dataStart[m_serializationDataCount];
	const GYCHAR* pDataEnd = _GetDataString(pDataStart);
	GYINT32 readDataCount = pDataEnd - pDataStart;
	GYAssert(readDataCount > 1);
	const GYCHAR* pData = _GetDataString(pDataStart, pDataEnd);
	GYAssert(GYNULL != pData);
	value = pData;
	m_serializationDataCount += readDataCount;
	return *this;
}

const GYCHAR* GYTableSerialization::_GetLine(const GYCHAR* pData, GYBOOL processCommetChar)
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
		if (GYTRUE == processCommetChar && GYNULL != pResult && LineCommentChar == *pResult)
		{
			pResult = _GetLine(pResult, processCommetChar);
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

static const GYINT32 StringToDataBufferLen = 1024;
static GYCHAR StringToDataBuffer[StringToDataBufferLen] = {0};
const GYCHAR* GYTableSerialization::_GetDataString( const GYCHAR* pStart, const GYCHAR* pEnd )
{
	const GYCHAR* pResult = GYNULL;
	if (GYNULL != pStart && GYNULL != pEnd)
	{
		if(DataSplitChar == *(pEnd - 1) || UnixNewLine == *(pEnd -1))
		{
			--pEnd;
			if (WindowsNewLineStart == *(pEnd -1))
			{
				--pEnd;
			}
			GYINT32 datalen = pEnd - pStart;
			GYAssert(datalen > 0 && datalen < StringToDataBufferLen);
			GYMemcpy(StringToDataBuffer, pStart, datalen);
			StringToDataBuffer[datalen] = 0;
			pResult = StringToDataBuffer;
		}		
		else if (0 == *pEnd)
		{
			pResult = pEnd;
		}
	}
	return pResult;
}

GYVOID GYTableSerialization::_CleanUp()
{
	m_tableRowCount = 0;
	m_tableColumCount = 0;
	m_fileSize = 0;
	m_serializationDataCount = 0;
	m_dataStart = GYNULL;
#ifdef CHECK_DATA_TYPE
	m_dataType = GYNULL;
	m_loadColumCount = 0;
#endif
}
