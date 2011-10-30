/////////////////////////////////////////////
// create time: 2011/10/27 22:41
// author:	colin chen
// file name:	GYZone
// file type:	cpp
////////////////////////////////////////////
#include "GYZone.h"
#include "GYArea.h"
#include "GYGameHuman.h"

GYZone::GYZone()
{

}

GYZone::~GYZone()
{

}
static const GYINT32 MAX_AREA_COUNT_IN_ZONE = 64;
static const GYINT32 MAX_HUMAN_COUNT_IN_ZONE = 64;
GYINT32 GYZone::Init( GYScene& scene, GYINT32 id, const GYRect& rect )
{
	m_scene = &scene;
	m_id = id;
	m_rect =  rect;
	m_area.Init(MAX_AREA_COUNT_IN_ZONE);
	m_objectHumanSet.Init(MAX_HUMAN_COUNT_IN_ZONE);
	return 0;
}

GYINT32 GYZone::AddArea( GYArea& area )
{
	return m_area.Add(area);
}

GYINT32 GYZone::AddHuman( GYHuman& human )
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if (0 != m_objectHumanSet.Add(human))
		{
			break;
		}

		result = 0;
	} while (GYFALSE);
	return result;
}

GYINT32 GYZone::RemoveHuman( GYHuman& human )
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if (0 != m_objectHumanSet.Delete(human))
		{
			break;
		}

		result = 0;
	} while (GYFALSE);
	return result;
}
