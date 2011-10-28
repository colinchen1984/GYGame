/////////////////////////////////////////////
// create time: 2011/10/27 22:41
// author:	colin chen
// file name:	GYArea
// file type:	h
////////////////////////////////////////////

#ifndef __GYAREA_H__
#define __GYAREA_H__
#include "GYCommonDefine.h"
class GYObjectHuman;
class GYCreature;
class GYScene;
struct GYAreaConfig;

class GYArea 
{
	GYScene*					m_scene;						//所在场景
	const GYAreaConfig*			m_areaConfig;					//事件区域配置
	GYGUID						m_areaGUID;						//事件区域GUID
public:
	GYArea();
	~GYArea();

	GYINT32 Init(GYScene& secne, const GYAreaConfig& areaConfig);

	const GYGUID&	GetGUID() const;
};

#endif
