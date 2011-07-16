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
	char t[1] = {0};
	char tt[4] = {0};
	return 0;
}

