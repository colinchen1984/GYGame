/////////////////////////////////////////////
// create time: 2011/10/27 22:41
// author:	colin chen
// file name:	GYArea
// file type:	cpp
////////////////////////////////////////////
#include "GYArea.h"
#include "GYObjectCommenDefine.h"
#include "GYGameTableDefine.h"
#include "GYScene.h"

GYArea::GYArea()
{
	m_scene = GYNULL;	
	m_areaConfig = GYNULL;	
	m_areaGUID = INVALID_VALUE;
}

GYArea::~GYArea()
{

}

GYINT32 GYArea::Init( GYScene& secne, const GYAreaConfig& areaConfig )
{
	m_scene = &secne;
	m_areaConfig = &areaConfig;
	GYINT32* pGUID = reinterpret_cast<GYINT32*>(&m_areaGUID);
	pGUID[0] = 	areaConfig.AreaID;
	pGUID[1] = 	secne.GetSceneID();
	return 0;
}

const GYGUID& GYArea::GetGUID()	const 
{
	return m_areaGUID;
}
