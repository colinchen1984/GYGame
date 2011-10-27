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
	GYINT32		SceneID;			// 场景ID
	GYINT32		SceneLength;		// 场景长度
	GYINT32		SceneWight;			// 场景宽度
	GYINT32		ZoneSize;			// 场景内区域的宽度
	GYString	AreaConfigFileName;	// 场景内事件区域配置文件名

	GYSceneDefine():AreaConfigFileName(GETSTRINGMANAGERSINGLETON)
	{

	};

	~GYSceneDefine(){};

	GYVOID Serializ(GYSerializationInteface& seralizer)
	{
		seralizer << SceneID;
		seralizer << SceneLength;
		seralizer << SceneWight;
		seralizer << ZoneSize;
		seralizer << AreaConfigFileName;
	}

	static const GYCHAR* GetTableFileName()
	{
		static const GYCHAR* pTableName = "../Game/Table/GYSceneDefin.tab";
		return pTableName;
	}
};
#endif
