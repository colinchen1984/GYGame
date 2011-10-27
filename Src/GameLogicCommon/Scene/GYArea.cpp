/////////////////////////////////////////////
// create time: 2011/10/27 22:41
// author:	colin chen
// file name:	GYArea
// file type:	cpp
////////////////////////////////////////////
#include "GYArea.h"


GYArea::GYArea()
{

}

GYArea::~GYArea()
{

}
const GYINT32 MAX_HUMAN_COUNT_IN_AREA = 16;
const GYINT32 MAX_CREATE_COUNT_IN_AREA = 16;
GYINT32 GYArea::Init( GYScene& secne, const GYAreaConfig& areaConfig )
{
	m_scene = &secne;
	m_areaConfig = &areaConfig;
	m_objectHumanSet.Init(MAX_HUMAN_COUNT_IN_AREA);
	m_objectCreatureSet.Init(MAX_CREATE_COUNT_IN_AREA);
	return 0;
}