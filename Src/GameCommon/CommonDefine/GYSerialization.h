/////////////////////////////////////////////
// create time: 2011/8/28 20:30
// author:	colin chen
// file name:	GYSerialization
// file type:	h
////////////////////////////////////////////

#ifndef __GYSERIALIZATION_H__
#define __GYSERIALIZATION_H__
#include "GYCommonDefine.h"
enum EM_SERIALIZAION_MODE
{
	EM_SERIALIZAION_MODE_INVALID = -1,
	EM_SERIALIZAION_MODE_READ,
	EM_SERIALIZAION_MODE_WRITE,
	EM_SERIALIZAION_MODE_COUNT,
};
class GYString;
class GYSerializationInteface
{
protected:
	GYSerializationInteface(){};
	virtual ~GYSerializationInteface(){};

public:
	virtual GYSerializationInteface& operator<<(GYCHAR& value)=0;
	virtual GYSerializationInteface& operator<<(GYUINT8& value)=0;
	virtual GYSerializationInteface& operator<<(GYINT16& value)=0;
	virtual GYSerializationInteface& operator<<(GYUINT16& value)=0;
	virtual GYSerializationInteface& operator<<(GYINT32& value)=0;
	virtual GYSerializationInteface& operator<<(GYUINT32& value)=0;
	virtual GYSerializationInteface& operator<<(GYINT64& value)=0;
	virtual GYSerializationInteface& operator<<(GYUINT64& value)=0;
	virtual GYSerializationInteface& operator<<(GYFLOAT& value)=0;
	virtual GYSerializationInteface& operator<<(GYString& value)=0;
};

#endif
