/////////////////////////////////////////////
// create time: 2011/6/26 9:33
// author:	colin chen
// file name:	GYThreadTask
// file type:	h
////////////////////////////////////////////

#ifndef __GYTHREADTASK_H__
#define __GYTHREADTASK_H__
#include "GYCommonDefine.h"

typedef GYVOID (*task)(GYVOID* param);

struct GYThreadTask
{
	GYVOID* param;
	task m_threadFunction;
	task m_threadFinishCallBack;
	GYThreadTask()
	{
		GYZeroMem(this, sizeof(GYThreadTask));
	}
};

#endif
