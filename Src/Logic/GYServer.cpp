/////////////////////////////////////////////
// create time: 2011/9/6 23:24
// author:	colin chen
// file name:	GYServer
// file type:	cpp
////////////////////////////////////////////
#include "GYServer.h"

static GYServer* ServerSingleton = GYNULL;

GYServer::GYServer()
{
	GYAssert(GYNULL == ServerSingleton);
	ServerSingleton = this;
}

GYServer::~GYServer()
{

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
		result = 0;
	} while (GYFALSE);
	return result;
}

GYVOID GYServer::Release()
{
	m_packetFactoryManager.Release();
}

GYServer& GYServer::GetSingleton()
{
	return *ServerSingleton;
}

GYVOID GYServer::RunOnce()
{
	
}