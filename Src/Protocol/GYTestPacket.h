/////////////////////////////////////////////
// create time: 2011/7/31 20:11
// author:	colin chen
// file name:	GYTestPacket
// file type:	h
////////////////////////////////////////////

#ifndef __GYTESTPACKET_H__
#define __GYTESTPACKET_H__
#include "GYCommonDefine.h"
struct GYTestPacket;
class GYTestPacketWrap
{
	GYTestPacket* m_data;
public:
	GYTestPacketWrap();
	~GYTestPacketWrap();

	GYBOOL Init(GYCHAR* allocatedMemory, GYINT32 userNameLen);
	GYINT32 GetPacketLen(GYINT32 userNameLen);

	GYVOID SetUseName(const GYCHAR* userName, GYINT32 userNameLen);

};
#endif