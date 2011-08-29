/////////////////////////////////////////////
// create time: 2011/8/28 15:07
// author:	colin chen
// file name:	GYPacketFactoryManager
// file type:	cpp
////////////////////////////////////////////
#include "GYPacketFactoryManager.h"
#include "GYProtocolDefine.h"

GYVOID GYPacketFactoryManager::CleanUp()
{
	GYMemset(m_packetFactory, 0, sizeof(m_packetFactory));
}

GYBOOL GYPacketFactoryManager::Init()
{
	CleanUp();
	_RegisterPacket();
	return GYTRUE;
}

GYVOID GYPacketFactoryManager::Release()
{
	for (GYINT32 i = 0; i < EM_PACKET_ID_COUNT; ++i)
	{
		if (GYNULL != m_packetFactory[i])
		{
			GYDelete m_packetFactory[i];
		}
	}
	CleanUp();
}

GYPacketInteface* GYPacketFactoryManager::GetPacketbyID(EM_PACKET_ID packetID)
{
	GYAssert(packetID >=0 && packetID < EM_PACKET_ID_COUNT);
	return m_packetFactory[packetID];
}