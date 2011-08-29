/////////////////////////////////////////////
// create time: 2011/8/28 15:07
// author:	colin chen
// file name:	GYPacketFactoryManager
// file type:	h
////////////////////////////////////////////

#ifndef __GYPACKETFACTORYMANAGER_H__
#define __GYPACKETFACTORYMANAGER_H__
#include "GYCommonDefine.h"
#include "GYProtocolID.h"

class GYPacketInteface;

class GYPacketFactoryManager
{
	GYPacketInteface* m_packetFactory[EM_PACKET_ID_COUNT];

public:
	GYPacketFactoryManager(){CleanUp();};
	~GYPacketFactoryManager(){};

	GYBOOL Init();
	GYVOID Release();
	GYVOID CleanUp();
	GYPacketInteface* GetPacketbyID(EM_PACKET_ID packetID);
private:
	GYVOID _RegisterPacket();
};

#endif