/////////////////////////////////////////////
// create time: 2011/10/27 22:40
// author:	colin chen
// file name:	GYZone
// file type:	h
////////////////////////////////////////////

#ifndef __GYZONE_H__
#define __GYZONE_H__
#include "GYRect.h"
#include "GYList.h"
#include "GYArray.h"
#include "GYObjectCommenDefine.h"
class GYScene;
class GYArea;
class GYCreature;

class GYZone
{
	GYPointArray<GYArea>			m_area;
	GYRect							m_rect;
	GYScene*						m_scene;
	GYINT32							m_xID;
	GYINT32							m_zID;
	GYINT32							m_id;
	GYList<GYCreature>				m_creatureSet[EM_GAME_OBJECT_TYPE_COUNT];

public:
	GYZone();
	~GYZone();

	GYINT32 Init(GYScene& scene, GYINT32 id, const GYRect& rect, GYINT32 xID, GYINT32 zID);

	GYINT32 Release();

	GYINT32 AddArea(GYArea& area);

	GYINT32 AddCreature(GYCreature& creature);

	GYINT32 RemoveCreature(GYCreature& creature);

	GYVOID Tick(GYUINT32 frameTime);

	GYINLINE GYINT32 GetXID()const {return m_xID;};

	GYINLINE GYINT32 GetZID()const {return m_zID;};

private:

	static GYVOID _AreaTickForEach(GYCreature& creature, GYVOID* param);

	static GYVOID _SendPacketForEach(GYCreature& creature, GYVOID* param);

private:
	GYUINT32 m_frameTime;
};

#endif

