/////////////////////////////////////////////
// create time: 2011/9/6 23:57
// author:	colin chen
// file name:	GYTableDefine
// file type:	cpp
////////////////////////////////////////////
#include "GYTableDefine.h"
#include "GYServer.h"
#include "GYSerialization.h"

GYSceneDefine::GYSceneDefine() : testArrayString(GETSTRINGMANAGERSINGLETON)
{

}

GYSceneDefine::~GYSceneDefine()
{

}

GYVOID GYSceneDefine::Serializ( GYSerializationInteface& seralizer )
{
	for (GYINT32 i = 0; i < 5; ++i)
	{
		seralizer << testArray[i];
	}
	seralizer << testArrayString;
}


const GYCHAR* GYSceneDefine::GetTableFileName()
{
	static const GYCHAR* pTableName = "../Game/Table/GYSceneDefin.tab";
	return pTableName;
}
