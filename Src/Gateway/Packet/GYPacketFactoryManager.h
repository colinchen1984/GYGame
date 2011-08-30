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
class GYStringManager;

class GYPacketFactoryManager
{
	GYPacketInteface*	m_packetFactory[EM_PACKET_ID_COUNT];
	GYBOOL				m_packetFactoryBit[EM_PACKET_ID_COUNT];
	GYStringManager*	m_stringManager;
public:
	GYPacketFactoryManager(){CleanUp();};
	~GYPacketFactoryManager(){};

	GYINT32 Init(GYStringManager& stringManager);
	GYVOID Release();
	GYVOID CleanUp();
	GYPacketInteface* GetPacketByID(EM_PACKET_ID packetID);
	GYVOID ReleasePacketByID(GYPacketInteface& packet);
private:
	GYVOID _RegisterPacket();
};

#endif