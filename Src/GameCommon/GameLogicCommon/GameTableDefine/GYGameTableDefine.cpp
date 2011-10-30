/////////////////////////////////////////////
// create time: 2011/10/28 17:10
// author:	colin chen
// file name:	GYGameTableDefine
// file type:	cpp
////////////////////////////////////////////

#include "GYGameTableDefine.h"
#include "GYTableSerialization.h"
#include "GYStringManger.h"
	  
static GYTableInitSerialization cs;

GYSceneConfig::GYSceneConfig():SceneName(GETSTRINGMANAGERSINGLETON), AreaConfigFileName(GETSTRINGMANAGERSINGLETON)
{
	Serializ(cs);
}

GYSceneConfig::~GYSceneConfig()
{

}

GYVOID GYSceneConfig::Serializ( GYSerializationInteface& seralizer )
{
	seralizer << SceneID;
	seralizer << SceneName;
	seralizer << SceneLength;
	seralizer << SceneWight;
	seralizer << ZoneSize;
	seralizer << MaxHumanInScene;
	seralizer << AreaConfigFileName;
}

const GYCHAR* GYSceneConfig::GetTableFileName()
{
	static const GYCHAR* pTableName = "../Game/Table/GYSceneDefin.tab";
	return pTableName;
}

GYAreaConfig::GYAreaConfig()
{
	Serializ(cs);
}

GYAreaConfig::~GYAreaConfig()
{

}

GYVOID GYAreaConfig::Serializ( GYSerializationInteface& seralizer )
{
	seralizer << AreaID;
	seralizer << StartZoneID;
	seralizer << AreaLength;
	seralizer << AreaWeight;
	seralizer << AreaType;
	seralizer << ScriptID;
}

