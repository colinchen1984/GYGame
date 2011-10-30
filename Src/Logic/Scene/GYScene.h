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
#include "GYArray.h"

struct GYSceneConfig;
class GYZone;
class GYArea;
class GYString;
class GYHuman;
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
	GYINT32							m_zoneCount;					//场景内区域的数量
	GYTable<GYAreaConfig>			m_sceneAreaDefine;				//场景对应的事件区域配置
	GYPointArray<GYHuman>			m_humanSet;						//场景内的玩家 
public:
	GYScene();
	~GYScene();

	GYINT32 Init(const GYSceneConfig& sceneDefine);

	GYINT32 Release();

	GYINT32 GetZoneID(const GYPosition& position) const;


	GYZone* GetZone(const GYPosition& position);

	GYZone* GetZone(GYINT32 zoneID);

	GYINT32 GetSceneID() const;
	GYINT32 GetMaxXZoneCount() const;

	GYINT32 GetMaxZZoneCount() const;

	GYINT32 AddHuman(GYHuman& human, const GYPosition& position);

	GYINT32 RemoveHuman(GYHuman& human);

private:
	GYINT32 _LoadAreaData(const GYString& areaConfigTableName);
};
#endif

