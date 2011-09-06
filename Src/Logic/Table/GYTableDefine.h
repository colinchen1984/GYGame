/////////////////////////////////////////////
// create time: 2011/9/6 23:40
// author:	colin chen
// file name:	GYTableDefine
// file type:	h
////////////////////////////////////////////

#ifndef __GYTABLEDEFINE_H__
#define __GYTABLEDEFINE_H__
#include "GYCommonDefine.h"
#include "GYString.h"
class GYSerializationInteface;

struct GYSceneDefine
{
	GYINT32		testArray[5];
	GYString	testArrayString;

	GYSceneDefine();

	~GYSceneDefine();

	GYVOID Serializ(GYSerializationInteface& seralizer);

	static const GYCHAR* GetTableFileName();
};
#endif
