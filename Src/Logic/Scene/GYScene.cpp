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
#include "GYGameCreature.h"

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
			m_zone[sceneID].Init(*this, sceneID, zoneRect, x, z);
		}
	}
	m_oldSyncZoneID.Init((2 * sceneDefine.SysnRange -1) * (2 * sceneDefine.SysnRange -1));
	m_newSyncZoneID.Init((2 * sceneDefine.SysnRange -1) * (2 * sceneDefine.SysnRange -1));
	if (0 != _LoadAreaData(sceneDefine.AreaConfigFileName))
	{
		Release();
		return INVALID_VALUE;
	}

	for (GYINT32 i = 0; i < EM_GAME_OBJECT_TYPE_COUNT; ++i)
	{
		m_creatureSet[i].Init(sceneDefine.MaxHumanInScene);
	}

	return 0;
}										 

//对于位置处于zone与zone的边界的问题，采用左闭右开的原则
GYINT32 GYScene::GetZoneID( const GYPosition& position ) const
{
	GYINT32 sceneID = INVALID_VALUE;
	if(position.m_x >= 0.0f && position.m_x <= m_sceneConfig->SceneLength)
	{
		if (position.m_z >= 0.0f && position.m_z <= m_sceneConfig->SceneWight)
		{
			GYINT32 xIndex = static_cast<GYINT32>(position.m_x / m_sceneConfig->ZoneSize);
			GYINT32 zIndex = static_cast<GYINT32>(position.m_z / m_sceneConfig->ZoneSize);
			sceneID = zIndex * m_maxXZoneCount + xIndex;
		}
	}
	return sceneID; 
}

GYINT32 GYScene::Release()
{
	for (GYINT32 i = 0; i < EM_GAME_OBJECT_TYPE_COUNT; ++i)
	{
		GYINT32 creatureCount = m_creatureSet[i].GetCurrentItemCount();
		//TODO 通知场景内所有玩家离开场景,保持相关数据
		m_creatureSet[i].Release();
	}
	for (GYINT32 i = 0; i < m_zoneCount; ++i)
	{
		m_zone[i].Release();
	}
	GYDelete[] m_zone;
	m_zone = GYNULL;
	for (GYINT32 i = 0; i < m_sceneAreaDefine.GetTableRowCount(); ++i)
	{
		m_Area[i].Release();
	}
	GYDelete[] m_Area;
	m_Area = GYNULL;

	m_sceneConfig = GYNULL;
	m_maxXZoneCount = 0;
	m_maxZZoneCount = 0;
	m_sceneAreaDefine.Release();
	m_oldSyncZoneID.Release();
	m_newSyncZoneID.Release();
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

GYINT32 GYScene::AddCreature(GYCreature& creature, const GYPosition& position)
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		GYZone* pZone = GetZone(position);
		if (GYNULL == pZone)
		{
			break;
		}

		if (GYTRUE != m_creatureSet[creature.GetObjectType()].Add(creature))
		{
			break;
		}

		if (0 != pZone->AddCreature(creature))
		{
			m_creatureSet[creature.GetObjectType()].Delete(creature);
			break;
		}
		creature.SetPosition(position);
		creature.OnEnterScene(*this);
		result = 0;

	} while (GYFALSE);
	return result;
}

GYINT32 GYScene::RemoveCreature(GYCreature& creature)
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		GYZone* pZone = GetZone(creature.GetPosition());
		if (GYNULL == pZone)
		{
			break;
		}

		creature.OnLeaveScene(*this);

		if (0 != pZone->RemoveCreature(creature))
		{
			break;
		}

		if (0 !=  m_creatureSet[creature.GetObjectType()].Delete(creature))
		{
			break;
		}

		result = 0;

	} while (GYFALSE);
	return result;
}

GYVOID GYScene::Tick( GYUINT32 frameTime )
{
	for (GYINT32 i = 0; i < EM_GAME_OBJECT_TYPE_COUNT; ++i)
	{
		GYINT32 creatureCount = m_creatureSet[i].GetCurrentItemCount();
		for (GYINT32 t = 0; t < creatureCount; ++i)
		{
			m_creatureSet[i][t]->Tick(frameTime);
		}
	}

	for (GYINT32 i = 0; i < m_zoneCount; ++i)
	{
		m_zone[i].Tick(frameTime);
	}
}

GYVOID GYScene::OnCreatureChangePosition(GYCreature& creature, const GYPosition& beforChange, const GYPosition& afterChange)
{
	GYZone* pBeforZone = GetZone(beforChange);
	GYZone* pAfterZone = GetZone(afterChange);
	if (pBeforZone == pAfterZone)
	{
		return;
	}

	//设同步范围为n
	//则以当前Zone为中心,取正方形
	//变长为2n-1
	//整个同步面积为(2n-1)(2n-1)

	//1 通知在pBeforZone同步范围内,不在pAfterZone同步范围内的玩家
	//删除该creature

	//2 通知不在pBeforZone同步范围内,在pAfterZone同步范围内的玩家
	//添加该creature

	//3 如果该creature是玩家,通知该玩家删除在pBeforZone同步范围内,不在pAfterZone同步范围内的creature

	//4 如果该creature是玩家,通知该玩家添加不在pBeforZone同步范围内,在pAfterZone同步范围内的creature

}

GYVOID GYScene::_GetSyncRange(GYZone& centerZone, GYINT32 syncRange, GYArray<GYINT32>& array )
{
	array.CleanUp();
	GYINT32 leftXID = centerZone.GetXID() - syncRange;
	leftXID = leftXID >= 0 ? leftXID : 0;
	GYINT32 leftZID = centerZone.GetZID() - syncRange;
	leftXID = leftZID >= 0 ? leftZID : 0;
	GYINT32 rightXID = centerZone.GetXID + syncRange;
	rightXID = rightXID >= m_maxXZoneCount ? m_maxXZoneCount - 1 : rightXID;
	GYINT32 rightZID = centerZone.GetZID() + syncRange;
	rightZID = rightZID >= m_maxZZoneCount ? m_maxZZoneCount - 1 : rightZID;


}

