/////////////////////////////////////////////
// create time: 2011/9/4 16:04
// author:	colin chen
// file name:	GYTableManager
// file type:	h
////////////////////////////////////////////

#ifndef __GYTABLEMANAGER_H__
#define __GYTABLEMANAGER_H__
#include "GYCommonDefine.h"
#include "GYStringManger.h"
#include "GYTable.h"
#include "GYGameTableDefine.h"

class GYTableManager
{
public:
	GYTable<GYSceneDefine>		m_sceneDefine;
public:
	GYTableManager();

	~GYTableManager();

	GYINT32 Init();

	static GYTableManager& GetSingleton();
};

#define GETTABLEMANAGER GYTableManager::GetSingleton()
#endif
