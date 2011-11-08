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
#include "GYZone.h"
#include "GYGameCreature.h"

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
	
	GYINT32 xZoneMaxCount = secne.GetMaxXZoneCount();
	//向zone注册area
	for (GYINT32 z = 0; z < areaConfig.AreaWeight; ++z)
	{
		for (GYINT32 x = 0; x < areaConfig.AreaLength; ++x)
		{
			GYZone* pZone = secne.GetZone(z * xZoneMaxCount + areaConfig.StartZoneID + x);
			if(GYNULL != pZone)
			{
				pZone->AddArea(*this);
			}
			else
			{
				GYAssert(GYFALSE);
			}
		}
	}
	return 0;
}

const GYGUID& GYArea::GetGUID()	const 
{
	return m_areaGUID;
}

GYVOID GYArea::OnTickCreature( GYCreature& creature, GYUINT32 frameTime )
{

}

GYINT32 GYArea::OnAddCreature( GYCreature& creature )
{
	return 0;
}

GYINT32 GYArea::OnRemoveCreature( GYCreature& creature )
{
	return 0;
}

GYINT32 GYArea::Release()
{
	return 0;
}