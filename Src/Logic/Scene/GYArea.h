/////////////////////////////////////////////
// create time: 2011/10/27 22:41
// author:	colin chen
// file name:	GYArea
// file type:	h
////////////////////////////////////////////

#ifndef __GYAREA_H__
#define __GYAREA_H__
#include "GYCommonDefine.h"
#include "GYObject.h"
#include "GYArray.h"
#include "GYGameObject.h"

class GYObjectHuman;
class GYCreature;
class GYScene;
struct GYAreaConfig;

class GYArea : public GYObject
{
	GYScene*						m_scene;						//所在场景
	const GYAreaConfig*				m_areaConfig;					//事件区域配置
	GYGUID							m_areaGUID;						//事件区域GUID
public:
	GYArea();

	~GYArea();

	GYINT32 Init(GYScene& secne, const GYAreaConfig& areaConfig);

	GYINT32 Release();

	const GYGUID&	GetGUID() const;

	GYINT32 OnAddCreature(GYCreature& creature);

	GYINT32 OnRemoveCreature(GYCreature& creature);

	GYVOID OnTickCreature(GYCreature& creature, GYUINT32 frameTime);

};

#endif

