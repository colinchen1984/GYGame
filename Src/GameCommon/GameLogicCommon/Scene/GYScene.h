/////////////////////////////////////////////
// create time: 2011/10/27 22:41
// author:	colin chen
// file name:	GYScene
// file type:	h
////////////////////////////////////////////

#ifndef __GYSCENE_H__
#define __GYSCENE_H__
#include "GYCommonDefine.h"
#include "GYTable.h"

struct GYSceneConfig;
class GYZone;
class GYArea;
class GYString;
struct GYPosition;
struct GYAreaConfig;
class GYScene
{
private:
	GYZone*							m_zone;							//场景内区域对象
	GYArea*							m_Area;							//场景内事件区域对象
	const GYSceneConfig*			m_sceneConfig;					//区域配置
	GYINT32							m_maxXZoneCount;				//x轴方向区域的数量
	GYINT32							m_maxZZoneCount;				//Z轴方向区域的数量
	GYTable<GYAreaConfig>			m_sceneAreaDefine;				//场景对应的事件区域配置
public:
	GYScene();
	~GYScene();

	GYINT32 Init(const GYSceneConfig& sceneDefine);

	GYINT32 Release();

	GYINT32 GetZoneID(const GYPosition& position);

	GYINT32 GetSceneID();
private:
	GYINT32 _LoadAreaData(const GYString& areaConfigTableName);
};
#endif
