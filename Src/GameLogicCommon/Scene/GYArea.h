/////////////////////////////////////////////
// create time: 2011/10/27 22:41
// author:	colin chen
// file name:	GYArea
// file type:	h
////////////////////////////////////////////

#ifndef __GYAREA_H__
#define __GYAREA_H__
#include "GYCommonDefine.h"
#include "GYArray.h"
#include "GYGameObject.h"
#include "GYRect.h"
class GYObjectHuman;
class GYCreature;
class GYScene;
struct GYAreaConfig;

class GYArea : public GYGameObject
{
	GYPointArray<GYObjectHuman> m_objectHumanSet;
	GYPointArray<GYCreature>	m_objectCreatureSet;
	GYRect						m_rect;
	GYScene*					m_scene;
	const GYAreaConfig*			m_areaConfig;
public:
	GYArea();
	~GYArea();

	GYINT32 Init(GYScene& secne, const GYAreaConfig& areaConfig);

};

#endif
