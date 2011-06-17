/////////////////////////////////////////////
// create time:2011/6/10 11:07
// author:	colin chen
// file name:	GYNetWorkCommonDefine
// file type:	cpp
////////////////////////////////////////////
#include "GYNetWorkCommonDefine.h"

extern GYINT32 InitNetWork()
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
#ifdef WIN32
		WSADATA wsaData;
		if (NO_ERROR != WSAStartup(MAKEWORD(2,2), &wsaData))
		{
			break;
		}
#endif
		result = 0;
	} while (GYFALSE);

	return result;
}

GYINT32 ReleaseNetWork()
{
	return 0;
}

GYINT32 GYIsValidSocket( GYSOCKET fd )
{
	return INVALID_SOCKET == fd ? GYFALSE : GYTRUE;
}

GYINT32 GetLastNetWorkError()
{
#ifdef WIN32
	return WSAGetLastError();
#endif

#ifdef LINUX64
	return errno;
#endif
}

