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

GYINT32 GYPacketFactoryManager::Init(GYStringManager& stringManager)
{
	CleanUp();
	_RegisterPacket();
	m_stringManager = &stringManager; 
	return 0;
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

GYPacketInteface* GYPacketFactoryManager::GetPacketByID(EM_PACKET_ID packetID)
{
	GYAssert(packetID >=0 && packetID < EM_PACKET_ID_COUNT);
	GYPacketInteface* packet = m_packetFactory[packetID];
	m_packetFactory[packetID] = GYNULL;
	return packet;
}

GYVOID GYPacketFactoryManager::ReleasePacketByID( GYPacketInteface& packet )
{
	EM_PACKET_ID packetID = GYGetPacketID(packet.GetPacketID());
	GYAssert(packetID >=0 && packetID < EM_PACKET_ID_COUNT);
	GYAssert(GYNULL == m_packetFactory[packetID]);
	m_packetFactory[packetID] = &packet;
}
