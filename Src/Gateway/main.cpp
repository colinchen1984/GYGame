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
	while(GYTRUE)
	{
		pServer->RunOnce();
	}
	pServer->Release();
	return 0;
}

