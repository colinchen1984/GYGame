/////////////////////////////////////////////
// create time: 2011/10/27 22:41
// author:	colin chen
// file name:	GYZone
// file type:	cpp
////////////////////////////////////////////
#include "GYZone.h"
#include "GYArea.h"
#include "GYGameHuman.h"
#include "GYPacketHandler.h"
GYZone::GYZone()
{

}

GYZone::~GYZone()
{

}
static const GYINT32 MAX_AREA_COUNT_IN_ZONE = 64;
static const GYINT32 MAX_HUMAN_COUNT_IN_ZONE = 64;
GYINT32 GYZone::Init(GYScene& scene, GYINT32 id, const GYRect& rect, GYINT32 xID, GYINT32 zID)
{
	m_scene = &scene;
	m_id = id;
	m_rect =  rect;
	m_area.Init(MAX_AREA_COUNT_IN_ZONE);
	m_xID = xID;
	m_zID = zID;
	return 0;
}


GYINT32 GYZone::Release()
{
	m_area.Release();
	return 0;
}

GYINT32 GYZone::AddArea( GYArea& area )
{
	return m_area.Add(area);
}

GYINT32 GYZone::AddCreature(GYCreature& creature)
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		m_creatureSet[creature.GetObjectType()].Add(creature);
		GYINT32 areaCount = m_area.GetCurrentItemCount();
		for (GYINT32 i = 0; i < areaCount; ++i)
		{
			m_area[i]->OnAddCreature(creature);
		}
		result = 0;
	} while (GYFALSE);
	return result;
}

GYINT32 GYZone::RemoveCreature(GYCreature& creature)
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		m_creatureSet[creature.GetObjectType()].Delete(creature);
		GYINT32 areaCount = m_area.GetCurrentItemCount();
		for (GYINT32 i = 0; i < areaCount; ++i)
		{
			m_area[i]->OnRemoveCreature(creature);
		}
		result = 0;
	} while (GYFALSE);
	return result;
}

GYVOID GYZone::_AreaTickForEach(GYCreature& creature, GYVOID* param)
{
	GYZone* pZone = reinterpret_cast<GYZone*>(param);
	if (GYNULL != pZone)
	{
		GYINT32 areaCount = pZone->m_area.GetCurrentItemCount();
		for (GYINT32 i = 0; i < areaCount; ++i)
		{
			pZone->m_area[i]->OnTickCreature(creature, pZone->m_frameTime);
		}
	}
}

GYVOID GYZone::Tick( GYUINT32 frameTime )
{
	m_frameTime = frameTime;
	for (GYINT32 i = 0; i < EM_GAME_OBJECT_TYPE_COUNT; ++i)
	{
		m_creatureSet[i].ForEachElement(_AreaTickForEach, this);
	}
	
}

GYVOID GYZone::_SendPacketForEach( GYCreature& creature, GYVOID* param )
{
	GYPacketInteface* packet = reinterpret_cast<GYPacketInteface*>(param);
	if (GYNULL != packet)
	{
		creature.SendPacket(*packet);
	}
}
