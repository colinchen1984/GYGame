#ifndef __GYINIREADER_H__
#define __GYINIREADER_H__

#include "GYCommonDefine.h"

class GYString;

const GYINT32 OK						= 0;
const GYINT32 ERROR_LOAD_FILE_ERROR		= -1;
const GYINT32 ERROR_NO_SPECIFIED_FILE	= -2;
const GYINT32 ERROR_NO_SPECIFIED_NODE	= -3;
const GYINT32 ERROR_NO_SPECIFIED_KEY	= -4;
const GYINT32 ERROR_FIND_NODE_ERROR		= -5;
const GYINT32 ERROR_FIND_KEY_ERROR		= -6;
const GYINT32 ERROR_NO_VALUE			= -7;

extern GYINT32 ReadINIStringValue(const GYCHAR* fileName, const GYCHAR* nodeName, const GYCHAR* keyName, GYString& value);
extern GYINT32 ReadINIIntValue(const GYCHAR* fileName, const GYCHAR* nodeName, const GYCHAR* keyName, int& value);
extern GYINT32 ReadINIFloatValue(const GYCHAR* fileName, const GYCHAR* nodeName, const GYCHAR* keyName, float& value);

#endif

