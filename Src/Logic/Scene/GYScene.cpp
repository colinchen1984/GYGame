/////////////////////////////////////////////
// create time: 2011/10/27 22:41
// author:	colin chen
// file name:	GYScene
// file type:	cpp
////////////////////////////////////////////
#include "GYScene.h"
#include "GYGameTableDefine.h"
#include "GYArea.h"
#include "GYZone.h"
#include "GYGameHuman.h"

GYScene::GYScene()
{
	m_zone = GYNULL;
	m_Area = GYNULL;
	m_sceneConfig = GYNULL;
	m_maxXZoneCount = 0;
	m_maxZZoneCount = 0;
	m_zoneCount = 0;
}

GYScene::~GYScene()
{

}

//左上角为场景原点，往右为x轴正方形，往下为z轴正方向
//（0， 0）------------------------------------------->x(最大值为sceneDefine.SceneLength)
//		  |
//		  |
//		  |
//		  |
//		  |
//		  |
//		  |
//		  |
//		  |
//		  |
//		  |
//		  |
//		  |
//		  |z(最大值为sceneDefine.SceneWight)

GYINT32 GYScene::Init( const GYSceneConfig& sceneDefine )
{
	GYAssert(sceneDefine.SceneLength > sceneDefine.ZoneSize && sceneDefine.SceneLength % sceneDefine.ZoneSize == 0);
	GYAssert(sceneDefine.SceneWight > sceneDefine.ZoneSize && sceneDefine.SceneWight % sceneDefine.ZoneSize == 0);
	m_sceneConfig = &sceneDefine;
	m_maxXZoneCount = sceneDefine.SceneLength / sceneDefine.ZoneSize;
	m_maxZZoneCount = sceneDefine.SceneWight / sceneDefine.ZoneSize;
	m_zoneCount = m_maxXZoneCount * m_maxZZoneCount;
	m_zone = GYNew GYZone[m_zoneCount];
	GYRect zoneRect;
	GYPosition point;
	for (GYINT32 z = 0; z < m_maxZZoneCount; ++z)
	{
		for(GYINT32 x = 0; x < m_maxXZoneCount; ++x)
		{
			point.m_x = static_cast<GYFLOAT>(x * sceneDefine.ZoneSize);
			point.m_z = static_cast<GYFLOAT>(z * sceneDefine.ZoneSize);
			zoneRect.m_pointA =  point;
			point.m_x = static_cast<GYFLOAT>((x + 1) * sceneDefine.ZoneSize);
			point.m_z = static_cast<GYFLOAT>((z + 1) * sceneDefine.ZoneSize);
			zoneRect.m_pointB =  point;
			GYINT32 sceneID = z * m_maxXZoneCount + x;
			m_zone[sceneID].Init(*this, sceneID, zoneRect);
		}
	}

	if (0 != _LoadAreaData(sceneDefine.AreaConfigFileName))
	{
		Release();
		return INVALID_VALUE;
	}
	m_humanSet.Init(sceneDefine.MaxHumanInScene);
	return 0;
}										 

//对于位置处于zone与zone的边界的问题，采用左闭右开的原则
GYINT32 GYScene::GetZoneID( const GYPosition& position ) const
{
	GYAssert(position.m_x >= 0.0f && position.m_x <= m_sceneConfig->SceneLength);
	GYAssert(position.m_z >= 0.0f && position.m_z <= m_sceneConfig->SceneWight);
	GYINT32 xIndex = static_cast<GYINT32>(position.m_x / m_sceneConfig->ZoneSize);
	GYINT32 zIndex = static_cast<GYINT32>(position.m_z / m_sceneConfig->ZoneSize);
	GYINT32 sceneID = zIndex * m_maxXZoneCount + xIndex;
	return sceneID; 
}

GYINT32 GYScene::Release()
{
	GYDelete[] m_zone;
	GYDelete[] m_Area;
	m_zone = GYNULL;
	m_Area = GYNULL;
	m_sceneConfig = GYNULL;
	m_maxXZoneCount = 0;
	m_maxZZoneCount = 0;
	m_sceneAreaDefine.Release();
	return 0;
}

GYINT32 GYScene::_LoadAreaData( const GYString& areaConfigTableName )
{
	if (areaConfigTableName.empty())
	{
		return 0;
	}
	if(0 != m_sceneAreaDefine.Load(areaConfigTableName.c_str()))
	{
		return INVALID_VALUE;
	}
	GYINT32 areaCount =   m_sceneAreaDefine.GetTableRowCount();
	GYAssert(areaCount > 0);
	m_Area = new GYArea[areaCount];
	for (GYINT32 i = 0; i < areaCount; ++i)
	{
		m_Area[i].Init(*this, *m_sceneAreaDefine.GetRowByIndex(i));
	}
	return 0;
}

GYINT32 GYScene::GetSceneID() const
{
	GYINT32 result = INVALID_VALUE;
	if (GYNULL != m_sceneConfig)
	{
		result = m_sceneConfig->SceneID;
	}
	return result;
}

GYZone* GYScene::GetZone( GYINT32 zoneID )
{
	GYZone* result = GYNULL;
	if (zoneID >= 0 && zoneID < m_zoneCount)
	{
		result = &m_zone[zoneID];
	}
	return result;
}

GYZone* GYScene::GetZone( const GYPosition& position )
{
	return GetZone(GetZoneID(position));
}

GYINT32 GYScene::GetMaxXZoneCount() const
{
	return m_maxXZoneCount;
}

GYINT32 GYScene::GetMaxZZoneCount() const
{
	return m_maxZZoneCount;
}

GYINT32 GYScene::AddHuman( GYHuman& human, const GYPosition& position )
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		GYZone* pZone = GetZone(position);
		if (GYNULL == pZone)
		{
			break;
		}

		if (GYTRUE != m_humanSet.Add(human))
		{
			break;
		}

		if (0 != pZone->AddHuman(human))
		{
			m_humanSet.Delete(human);
			break;
		}
		human.SetPosition(position);
		human.OnEnterScene();
		result = 0;
		
	} while (GYFALSE);
	return result;
}

GYINT32 GYScene::RemoveHuman( GYHuman& human )
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		GYZone* pZone = GetZone(human.GetPosition());
		if (GYNULL == pZone)
		{
			break;
		}

		human.OnLeaveScene();

		if (0 != pZone->RemoveHuman(human))
		{
			break;
		}

		if (0 != m_humanSet.Delete(human))
		{
			break;
		}

		result = 0;

	} while (GYFALSE);
	return result;
}
