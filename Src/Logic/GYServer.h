/////////////////////////////////////////////
// create time: 2011/9/6 23:23
// author:	colin chen
// file name:	GYServer
// file type:	h
////////////////////////////////////////////

#ifndef __GYSERVER_H__
#define __GYSERVER_H__
#include "GYStringManger.h"
#include "GYPacketFactoryManager.h"
#include "GYTableManager.h"
#include "GYNetWorkManager.h"
#include "GYScene.h"
#include "GYObjectPool.h"
#include "GYGameHuman.h"

class GYGatewaySession;

class GYServer
{
	StringMangerSingleton				m_stringManager;			//字符串管理器
	GYPacketFactoryManager				m_packetFactoryManager;		//网络包工厂管理器
	GYTableManager						m_tableManger;				//表格管理器
	GYNetWorkManager					m_networkManager;			//网络管理器
	GYScene								m_scene;					//场景
	GYObjectPool<GYGUID, GYHuman>		m_humanPool;				//玩家对象池
public:
	GYServer();

	~GYServer();

	GYINT32 Init();

	GYVOID	Release();

	GYVOID	RunOnce();

	GYINT32 AddHumanToScene(GYGatewaySession& gatewaySession, const GYGUID& guid, GYINT32 secneID);

	GYINT32 RemoveHumanFromScene(const GYGUID& guid);

	GYScene* GetSceneByID(GYINT32 sceneID);

	static GYServer& GetSingleton();
};

#define GETLOGICSERVER GYServer::GetSingleton()
#endif

