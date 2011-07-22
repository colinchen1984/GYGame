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
	pServer->Run();
	return 0;
}

