/////////////////////////////////////////////
// create time: 2011/9/6 23:24
// author:	colin chen
// file name:	GYServer
// file type:	cpp
////////////////////////////////////////////
#include "GYServer.h"
#include "GYNetAddress.h"
#include "GYTimeStamp.h"
#include "GYPosition.h"

static GYServer* ServerSingleton = GYNULL;
const GYCHAR* AddrStr = "127.0.0.1";

GYServer::GYServer()
{
	GYAssert(GYNULL == ServerSingleton);
	ServerSingleton = this;
}

GYServer::~GYServer()
{

}

GYServer& GYServer::GetSingleton()
{
	return *ServerSingleton;
}

GYINT32 GYServer::Init()
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if (0 != m_stringManager.Init())
		{
			break;
		}
		
		if (0 != m_packetFactoryManager.Init(m_stringManager))
		{
			break;
		}

		if (0 != m_tableManger.Init())
		{
			m_packetFactoryManager.Release();
			break;
		}
		if (0 != m_scene.Init(*m_tableManger.m_sceneDefine.GetRowByIndex(0)))
		{
			m_tableManger.Release();
			m_packetFactoryManager.Release();
			break;
		}
		
		GYINT32 maxHumanCount = 0;
		for (GYINT32 i = 0; i < m_tableManger.m_sceneDefine.GetTableRowCount(); ++i)
		{
			maxHumanCount += m_tableManger.m_sceneDefine.GetRowByIndex(i)->MaxHumanInScene;
		}
		if(0 != m_humanPool.Init(maxHumanCount))
		{
			m_scene.Release();
			m_tableManger.Release();
			m_packetFactoryManager.Release();
		}

		GYNetAddress listenAddress;
		listenAddress.SetAddr(AddrStr);
		listenAddress.SetPort(5556);
		if (0 != m_networkManager.Init(listenAddress))
		{
			m_humanPool.Release();
			m_scene.Release();
			m_tableManger.Release();
			m_packetFactoryManager.Release();
			break;
		}
		
		result = 0;
	} while (GYFALSE);
	return result;
}

GYVOID GYServer::Release()
{
	m_packetFactoryManager.Release();
}

GYVOID GYServer::RunOnce()
{
	GYTimeStamp time;
	time.m_termTime = 30;
	m_networkManager.RunOnce(time);
}

GYINT32 GYServer::AddHumanToScene( const GYGUID& guid, GYINT32 secneID )
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		GYHuman* pHuman = m_humanPool.FindObject(guid);
		if (GYNULL != pHuman)
		{
			break;
		}

		pHuman = m_humanPool.AllocObject(guid);
		if (GYNULL == pHuman)
		{
			break;
		}

		GYPosition position(100.0f, 100.0f);
		if(0 != m_scene.AddHuman(*pHuman, position))
		{
			m_humanPool.ReleaseObject(guid, *pHuman);
			break;
		}
		result = 0;
	} while (GYFALSE);
	return result;
}

GYINT32 GYServer::RemoveHumanFromScene( const GYGUID& guid )
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		GYHuman* pHuman = m_humanPool.FindObject(guid);
		if (GYNULL == pHuman)
		{
			break;
		}

		if(0 != m_scene.RemoveHuman(*pHuman))
		{
			m_humanPool.ReleaseObject(guid, *pHuman);
			break;
		}



		result = 0;
	} while (GYFALSE);
	return result;
}
