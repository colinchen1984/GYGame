/////////////////////////////////////////////
// create time: 2011/10/27 22:40
// author:	colin chen
// file name:	GYZone
// file type:	h
////////////////////////////////////////////

#ifndef __GYZONE_H__
#define __GYZONE_H__
#include "GYRect.h"
#include "GYArray.h"
class GYScene;
class GYArea;
class GYHuman;
class GYCreature;

class GYZone
{
	GYPointArray<GYArea>			m_area;
 	GYPointArray<GYHuman>			m_objectHumanSet;
// 	GYPointArray<GYCreature>		m_objectGYCreatureSet;
	GYRect							m_rect;
	GYScene*						m_scene;
	GYINT32							m_id;
public:
	GYZone();
	~GYZone();

	GYINT32 Init(GYScene& scene, GYINT32 id, const GYRect& rect);

	GYINT32 AddArea(GYArea& area);

	GYINT32 AddHuman(GYHuman& human);

	GYINT32 RemoveHuman(GYHuman& human);
};

#endif

