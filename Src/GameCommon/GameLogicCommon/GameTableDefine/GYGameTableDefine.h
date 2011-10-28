/////////////////////////////////////////////
// create time: 2011/9/6 23:40
// author:	colin chen
// file name:	GYGameTableDefine
// file type:	h
////////////////////////////////////////////

#ifndef __GYTABLEDEFINE_H__
#define __GYTABLEDEFINE_H__
#include "GYCommonDefine.h"
#include "GYString.h"
class GYSerializationInteface;

struct GYSceneConfig
{
	GYINT32		SceneID;			// 场景ID
	GYString	SceneName;			// 场景名
	GYINT32		SceneLength;		// 场景长度
	GYINT32		SceneWight;			// 场景宽度
	GYINT32		ZoneSize;			// 场景内区域的宽度
	GYString	AreaConfigFileName;	// 场景内事件区域配置文件名

	GYSceneConfig();

	~GYSceneConfig();

	GYVOID Serializ(GYSerializationInteface& seralizer);

	static const GYCHAR* GetTableFileName();
};

struct GYAreaConfig
{
	GYINT32		AreaID;				// 事件区域ID
	GYINT32		StartZoneID;		// 事件区域起始ZoneID
	GYINT32		AreaLength;			// 事件区域的长度
	GYINT32		AreaWeight;			// 事件区域的宽度
	GYINT32		AreaType;			// 事件区域类型
	GYINT32		ScriptID;			// 事件区域脚本

	GYAreaConfig();

	~GYAreaConfig();

	GYVOID Serializ(GYSerializationInteface& seralizer);
};
#endif
