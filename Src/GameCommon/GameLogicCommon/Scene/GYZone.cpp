/////////////////////////////////////////////
// create time: 2011/10/27 22:41
// author:	colin chen
// file name:	GYZone
// file type:	cpp
////////////////////////////////////////////
#include "GYZone.h"


GYZone::GYZone()
{

}

GYZone::~GYZone()
{

}
static const GYINT32 MAX_AREA_COUNT_IN_ZONE = 64;
GYINT32 GYZone::Init( GYScene& scene, GYINT32 id, const GYRect& rect )
{
	m_scene = &scene;
	m_id = id;
	m_rect =  rect;
	m_area.Init(MAX_AREA_COUNT_IN_ZONE);
	return 0;
}
