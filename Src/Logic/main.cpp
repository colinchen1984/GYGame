#include "GYCommonDefine.h"
#include "GYNetWorkCommonDefine.h"
#include "GYServer.h"
GYINT32 main()
{
	InitNetWork();
	GYServer* pServer = GYNew GYServer();
	if (0 != pServer->Init())
	{
		return INVALID_VALUE;
	}
	while(GYTRUE)
	{
		pServer->RunOnce();
	}
	return 0;
}

