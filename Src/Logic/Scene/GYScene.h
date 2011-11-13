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
#include "GYObjectCommenDefine.h"

class GYZone;
class GYArea;
class GYString;
class GYCreature;
struct GYSceneConfig;
struct GYPosition;
struct GYAreaConfig;

class GYScene
{
private:
	GYZone*							m_zone;										//场景内区域对象
	GYArea*							m_Area;										//场景内事件区域对象
	const GYSceneConfig*			m_sceneConfig;								//区域配置
	GYINT32							m_maxXZoneCount;							//x轴方向区域的数量
	GYINT32							m_maxZZoneCount;							//Z轴方向区域的数量
	GYINT32							m_zoneCount;								//场景内区域的数量
	GYTable<GYAreaConfig>			m_sceneAreaDefine;							//场景对应的事件区域配置
	GYPointArray<GYCreature>		m_creatureSet[EM_GAME_OBJECT_TYPE_COUNT];	//场景内的creature
	GYArray<GYINT32>				m_oldSyncZoneID;
	GYArray<GYINT32>				m_newSyncZoneID;
public:
	GYScene();
	~GYScene();

	GYINT32 Init(const GYSceneConfig& sceneDefine);

	GYINT32 GetSceneID() const ;

	GYINT32 Release();

	GYINT32 GetZoneID(const GYPosition& position) const;

	GYZone* GetZone(const GYPosition& position);

	GYZone* GetZone(GYINT32 zoneID);

	GYINT32 GetMaxXZoneCount() const;

	GYINT32 GetMaxZZoneCount() const;

	GYINT32 AddCreature(GYCreature& creature, const GYPosition& position);

	GYINT32 RemoveCreature(GYCreature& creature);

	GYVOID OnCreatureChangePosition(GYCreature& creature, const GYPosition& beforChange, const GYPosition& afterChange);

	GYVOID Tick(GYUINT32 frameTime);
private:
	GYINT32 _LoadAreaData(const GYString& areaConfigTableName);

	GYVOID _GetSyncRange(const GYZone* beforZone, const GYZone* afertZone, GYINT32 syncRange, GYArray<GYINT32>& inBNotA, GYArray<GYINT32>& inANotB);
};
#endif

