/////////////////////////////////////////////
// create time: 2011/6/23 22:17
// author:	colin chen
// file name:	GYThreadCommon
// file type:	cpp
////////////////////////////////////////////
#include "GYThreadCommon.h"
#ifdef LINUX64
static pthread_attr_t thread_attr;
#endif
GYINT32 InitThread()
{
#ifdef LINUX64
	if(0 != pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE))
	{
		return INVALID_VALUE;
	}
#endif // LINUX64
	return 0;
}

