/////////////////////////////////////////////
// create time:2011/6/10 13:28
// author:	colin chen
// file name:	test
// file type:	cpp
////////////////////////////////////////////



#include "GYNetAddress.h"
#include "GYCommonDefine.h"
#include "GYNetWorkCommonDefine.h"

GYINT32 main()
{
	InitNetWork();
	GYNetAddress test;
	test.SetAddr("10.1.1.0");
//	test.SetPort(80);
	GYCHAR testBUffer[1024] = {0};
	test.GetAddr(testBUffer, 1024);
	return 0;
}
