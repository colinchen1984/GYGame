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
	Sleep(INVALID_VALUE);
	return 0;
}

