#include "GYCommonDefine.h"
#include "GYServer.h"
#include "GYNetWorkCommonDefine.h"
#include "GYThreadCommon.h"
GYINT32 main()
{
	InitNetWork();
	InitThread();
	GYServer* pServer = GYNew GYServer();
	pServer->Init();
	for (GYINT32 i = 0; i < 1000; ++i)
	{
		pServer->RunOnce();
	}
	pServer->Release();
	return 0;
}

