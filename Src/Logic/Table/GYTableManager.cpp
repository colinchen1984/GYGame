/////////////////////////////////////////////
// create time: 2011/9/4 16:04
// author:	colin chen
// file name:	GYTableManager
// file type:	cpp
////////////////////////////////////////////
#include "GYTableManager.h"

static GYTableManager* pTableSingleton = GYNULL;
GYTableManager::GYTableManager()
{
	GYAssert(GYNULL == pTableSingleton);
	pTableSingleton = this;
}

GYTableManager::~GYTableManager()
{

}

GYTableManager& GYTableManager::GetSingleton()
{
	return *pTableSingleton;
}

GYINT32 GYTableManager::Init()
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if (0 != m_sceneDefine.Load(m_sceneDefine.GetTableFileName()))
		{
			break;
		}
		result = 0;
	} while (GYFALSE);
	return result;
}
