/////////////////////////////////////////////
// create time: 2011/7/1 21:57
// author:	colin chen
// file name:	GYGatewayThread
// file type:	h
////////////////////////////////////////////

#ifndef __GYGATEWAYTHREAD_H__
#define __GYGATEWAYTHREAD_H__
#include "GYNetAddress.h"


class GYGatewayThread
{
	GYNetAddress m_targetServerAddress;
	
public:
	GYGatewayThread();

	~GYGatewayThread();

	GYINT32 Init(const GYNetAddress& targetServerAddress);

};

#endif
