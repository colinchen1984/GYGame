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

class GYServer
{
	StringMangerSingleton		m_stringManager;
	GYPacketFactoryManager		m_packetFactoryManager;
	GYTableManager				m_tableManger;
public:
	GYServer();

	~GYServer();

	GYINT32 Init();

	GYVOID	Release();

	GYVOID	RunOnce();

	static GYServer& GetSingleton();
};

#define GETLOGICSERVER GYServer::GetSingleton()
#endif
