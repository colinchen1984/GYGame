#include "GYINIReader.h"
#include "GYString.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static const GYCHAR UnixNewLine = '\n';
static const GYCHAR WindowsNewLineStart = '\r';
static const GYCHAR DataSplitChar = '\t';
static const GYCHAR SpaceChar = ' ';
static const GYCHAR StringEndChar = '\0';
static const GYCHAR LbracketChar = '[';
static const GYCHAR RbracketChar =']';
static const GYCHAR EqualChar = '=';

static const GYINT32 bufferForINIFileLoadLen = 1024 * 1024 * 2;
static GYCHAR BufferForINIFileLoad[bufferForINIFileLoadLen] = {0};

const GYCHAR* GetLineEnd(const GYCHAR* lineStart);

static const GYINT32 StringToDataBufferLen = 1024;
static GYCHAR StringToDataBuffer[StringToDataBufferLen] = {0};

static const GYCHAR* ReadINIPointCharValue(const GYCHAR* fileName, const GYCHAR* nodeName, const GYCHAR* keyName, GYINT32& result)
{
	if (GYNULL == fileName)
	{
		result = ERROR_NO_SPECIFIED_FILE;
		return GYNULL;
	}

	if (GYNULL == nodeName)
	{
		result = ERROR_NO_SPECIFIED_NODE;
		return GYNULL;
	}

	if (GYNULL == keyName)
	{
		result = ERROR_NO_SPECIFIED_KEY;
		return GYNULL;
	}

	struct stat fileInfo;
	if (stat(fileName, &fileInfo))
	{
		result = ERROR_LOAD_FILE_ERROR;
		return GYNULL;
	}

	// ��Ҫ�����־����***************************

	GYINT32 fileSize = fileInfo.st_size;
	FILE* pFile = fopen(fileName, "rb");

	if (GYNULL == pFile)
	{
		result = ERROR_LOAD_FILE_ERROR;
		return GYNULL;
	}

	if (1 != fread(BufferForINIFileLoad, fileSize, 1, pFile))
	{
		result = ERROR_LOAD_FILE_ERROR;
		fclose(pFile);
		return GYNULL;
	}
	fclose(pFile);
	BufferForINIFileLoad[fileSize] = StringEndChar;

	//bool isNodeFound = false;
	//const char* lineStart = BufferForINIFileLoad;
	//const char* lineEnd = FindNode(nodeName, lineStart, isNodeFound);

	const GYCHAR* lineStart = BufferForINIFileLoad;
	const GYCHAR* lineEnd = BufferForINIFileLoad;
	bool isNodeFound = GYFALSE;

	while(*lineEnd != StringEndChar)
	{
		// ���ҵ�ǰ�еĽ�β
		lineEnd = GetLineEnd(lineStart);

		// û���ҵ��ڵ�
		if (!isNodeFound)
		{
			// ���������ţ�����ǿո��Ʊ�������У���������ַ��������������[���ַ�����Ϊ���в��ǽڵ���
			while (lineStart != lineEnd && *lineStart != LbracketChar &&
				(*lineStart == SpaceChar || *lineStart == DataSplitChar || *lineStart == WindowsNewLineStart))
			{
				++lineStart;
			}

			// �ҵ��Ĳ��������ţ����������ַ�����������
			if (lineStart != lineEnd && *lineStart != LbracketChar)
			{
				lineStart = lineEnd + 1;
				continue;
			}
			// �ҵ�������
			else if (lineStart != lineEnd)
			{
				const GYCHAR* lbracket = lineStart + 1;

				// ����������
				while (lineStart != lineEnd && *lineStart != RbracketChar)
				{
					++lineStart;
				}

				// �ҵ�������
				if (lineStart != lineEnd)
				{
					const GYCHAR* rbracket = lineStart;

					const GYCHAR* node = nodeName;
					while (lbracket < rbracket && *node != StringEndChar && *lbracket == *node)
					{
						++lbracket;
						++node;
					}

					// �ҵ��˽ڵ�
					if (lbracket == rbracket && *node == StringEndChar)
					{
						isNodeFound = GYTRUE;
					}
				}
			}
			lineStart = lineEnd + 1;
		}
		// �ҵ��˽ڵ㣬���Ҽ�
		else
		{
			const GYCHAR* lineIndex = lineStart;

			// ���ж��Ƿ�����һ���ڵ�
			while (lineIndex != lineEnd)
			{
				if (*lineIndex == LbracketChar)
				{
					break;
				}
				if (*lineIndex != SpaceChar &&
					*lineIndex != DataSplitChar &&
					*lineIndex != WindowsNewLineStart)
				{
					break;
				}

				++lineIndex;
			}

			if (lineIndex != lineEnd)
			{
				// �ҵ��������ţ����ڲ���������
				if (*lineIndex == LbracketChar)
				{
					while (lineIndex != lineEnd)
					{
						// �ҵ��������ţ�������һ���ڵ㣬������Ҫ�˳��������жϸýڵ���Ҫ���ҵĽڵ��Ƿ�һ��
						if (*lineIndex == RbracketChar)
						{
							isNodeFound = GYFALSE;
							continue;
						}
					}
				}
			}

			// û���ҵ������ţ�
			lineIndex = lineStart;

			// ���ҵȺţ���ȥ��INI�ļ��м�ǰ����ܴ��ڵĿո��Ʊ��������
			bool isIgnoring = GYTRUE;
			while (lineIndex != lineEnd)
			{
				if (*lineIndex == EqualChar)
				{
					break;
				}

				if (isIgnoring)
				{
					if (*lineIndex == SpaceChar)
					{
						++lineStart;
					}
					else if (*lineIndex == DataSplitChar)
					{
							++lineStart;
					}
					else if (*lineIndex == WindowsNewLineStart)
					{
						++lineStart;
					}
					else
					{
						isIgnoring = GYFALSE;
					}
				}

				++lineIndex;
			}

			if (lineIndex != lineEnd)
			{
				// �ҵ��Ⱥţ���ȥ��������ܴ��ڵĿո��Ʊ��������
				const GYCHAR* lineKeyEnd = lineIndex;
				while (lineKeyEnd != lineStart)
				{
					if (*(lineKeyEnd - 1) == SpaceChar)
						--lineKeyEnd;
					else if (*(lineKeyEnd - 1) == DataSplitChar)
						--lineKeyEnd;
					else if (*(lineKeyEnd - 1) == WindowsNewLineStart)
						--lineKeyEnd;
					else
						break;
				}

				// �ҵ���һ������������ָ���ļ����бȽ�
				if (lineKeyEnd != lineStart)
				{
					const GYCHAR* keyStart = keyName;
					const GYCHAR* keyEnd = keyName;
					// ȥ��ָ�����еĿո�
					while (*keyStart != StringEndChar)
					{
						if (*keyStart == SpaceChar)
							++keyStart;
						else
							break;
					}

					keyEnd = GetLineEnd(keyStart);
					while (keyEnd != keyStart)
					{
						if (*(keyEnd - 1) == SpaceChar)
							--keyEnd;
						else
							break;
					}

					// ���ҵ��ļ���ָ���ļ����бȽ�
					while (lineStart != lineKeyEnd)
					{
						if (*keyStart == StringEndChar)
							break;
						if (*keyStart != *lineStart)
							break;

						++keyStart;
						++lineStart;
					}

					// �ҵ���ָ���ļ�
					if (lineStart == lineKeyEnd && keyStart == keyEnd)
					{
						// ȥ����ֵ�п��ܴ��ڵĿո���Ʊ��
						do 
						{
							++lineIndex;
							if (*lineIndex != SpaceChar && 
								*lineIndex != DataSplitChar)
								break;
						} while(lineIndex != lineEnd);

						// ȥ����ֵĩβ�Ŀո���Ʊ��
						while (lineEnd != lineIndex)
						{
							if (*(lineEnd - 1) == SpaceChar)
								--lineEnd;
							else if (*(lineEnd - 1) == DataSplitChar)
								--lineEnd;
							else
								break;
						}

						// ��ֵΪ�գ����ش���
						if (lineEnd == lineIndex)
						{
							result = ERROR_NO_VALUE;
							return GYNULL;
						}
						
						// ����ֵ���軺�棬����һ������
						GYCHAR* bufferIndex = StringToDataBuffer;
						while (lineIndex != lineEnd)
							*bufferIndex++ = *lineIndex++;
						*bufferIndex = StringEndChar;

						result = OK;
						return StringToDataBuffer;
					}
				}
			}
			lineStart = lineEnd + 1;
		}
	}

	// û���ҵ��ڵ㣬���ش���
	if (*lineEnd == StringEndChar)
	{
		if (!isNodeFound)
		{
			result = ERROR_FIND_NODE_ERROR;
			return GYNULL;
		}
		else
		{
			result = ERROR_FIND_KEY_ERROR;
			return GYNULL;
		}
	}

	return OK;
}

GYINT32 ReadINIStringValue(const GYCHAR* fileName, const GYCHAR* nodeName, const GYCHAR* keyName, GYString& value)
{
	GYINT32 result = 0;
	const GYCHAR* buffer = ReadINIPointCharValue(fileName, nodeName, keyName, result);

	if (result != OK)
		return result;

	if (buffer != GYNULL)
		value = buffer;
	return OK;
}

GYINT32 ReadINIIntValue(const GYCHAR* fileName, const GYCHAR* nodeName, const GYCHAR* keyName, GYINT32& value)
{
	GYINT32 result = 0;
	const GYCHAR* buffer = ReadINIPointCharValue(fileName, nodeName, keyName, result);

	if (result != OK)
		return result;

	if (buffer != GYNULL)
		value = atoi(buffer);
	return OK;
}

GYINT32 ReadINIFloatValue(const GYCHAR* fileName, const GYCHAR* nodeName, const GYCHAR* keyName, GYFLOAT& value)
{
	GYINT32 result = 0;
	const GYCHAR* buffer = ReadINIPointCharValue(fileName, nodeName, keyName, result);

	if (result != OK)
		return result;

	if (buffer != GYNULL)
		value = static_cast<GYFLOAT>(atof(buffer));
	return OK;
}

// ���һ�еĽ�β
const GYCHAR* GetLineEnd(const GYCHAR* lineStart)
{
	while (*lineStart != UnixNewLine && *lineStart != StringEndChar)
		++lineStart;

	return lineStart;
}

