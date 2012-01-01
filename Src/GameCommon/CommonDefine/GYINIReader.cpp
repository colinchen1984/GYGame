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

	// 需要添加日志函数***************************

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
		// 查找当前行的结尾
		lineEnd = GetLineEnd(lineStart);

		// 没有找到节点
		if (!isNodeFound)
		{
			// 查找左括号，如果是空格，制表符，换行，调过这个字符，如果是其他非[的字符，认为这行不是节点行
			while (lineStart != lineEnd && *lineStart != LbracketChar &&
				(*lineStart == SpaceChar || *lineStart == DataSplitChar || *lineStart == WindowsNewLineStart))
			{
				++lineStart;
			}

			// 找到的不是左括号，而是其他字符，跳过这行
			if (lineStart != lineEnd && *lineStart != LbracketChar)
			{
				lineStart = lineEnd + 1;
				continue;
			}
			// 找到左括号
			else if (lineStart != lineEnd)
			{
				const GYCHAR* lbracket = lineStart + 1;

				// 查找右括号
				while (lineStart != lineEnd && *lineStart != RbracketChar)
				{
					++lineStart;
				}

				// 找到右括号
				if (lineStart != lineEnd)
				{
					const GYCHAR* rbracket = lineStart;

					const GYCHAR* node = nodeName;
					while (lbracket < rbracket && *node != StringEndChar && *lbracket == *node)
					{
						++lbracket;
						++node;
					}

					// 找到了节点
					if (lbracket == rbracket && *node == StringEndChar)
					{
						isNodeFound = GYTRUE;
					}
				}
			}
			lineStart = lineEnd + 1;
		}
		// 找到了节点，查找键
		else
		{
			const GYCHAR* lineIndex = lineStart;

			// 先判断是否是另一个节点
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
				// 找到了左括号，现在查找右括号
				if (*lineIndex == LbracketChar)
				{
					while (lineIndex != lineEnd)
					{
						// 找到了右括号，该行是一个节点，所以需要退出，重新判断该节点与要查找的节点是否一致
						if (*lineIndex == RbracketChar)
						{
							isNodeFound = GYFALSE;
							continue;
						}
					}
				}
			}

			// 没有找到右括号，
			lineIndex = lineStart;

			// 查找等号，并去除INI文件中键前面可能存在的空格、制表符、换行
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
				// 找到等号，除去键后面可能存在的空格、制表符、换行
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

				// 找到了一个键，将其与指定的键进行比较
				if (lineKeyEnd != lineStart)
				{
					const GYCHAR* keyStart = keyName;
					const GYCHAR* keyEnd = keyName;
					// 去除指定键中的空格
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

					// 将找到的键与指定的键进行比较
					while (lineStart != lineKeyEnd)
					{
						if (*keyStart == StringEndChar)
							break;
						if (*keyStart != *lineStart)
							break;

						++keyStart;
						++lineStart;
					}

					// 找到了指定的键
					if (lineStart == lineKeyEnd && keyStart == keyEnd)
					{
						// 去除键值中可能存在的空格和制表符
						do 
						{
							++lineIndex;
							if (*lineIndex != SpaceChar && 
								*lineIndex != DataSplitChar)
								break;
						} while(lineIndex != lineEnd);

						// 去除键值末尾的空格和制表符
						while (lineEnd != lineIndex)
						{
							if (*(lineEnd - 1) == SpaceChar)
								--lineEnd;
							else if (*(lineEnd - 1) == DataSplitChar)
								--lineEnd;
							else
								break;
						}

						// 键值为空，返回错误
						if (lineEnd == lineIndex)
						{
							result = ERROR_NO_VALUE;
							return GYNULL;
						}
						
						// 将键值赋予缓存，供下一步处理
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

	// 没有找到节点，返回错误
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

// 获得一行的结尾
const GYCHAR* GetLineEnd(const GYCHAR* lineStart)
{
	while (*lineStart != UnixNewLine && *lineStart != StringEndChar)
		++lineStart;

	return lineStart;
}

