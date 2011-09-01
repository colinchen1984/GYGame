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
	GYZeroMem(m_packetFactory, sizeof(m_packetFactory));
	GYZeroMem(m_packetFactoryBit,sizeof(m_packetFactoryBit));
	GYZeroMem(m_packetHandler, sizeof(m_packetHandler));
	
}

GYINT32 GYPacketFactoryManager::Init(GYStringManager& stringManager)
{
	CleanUp();
	m_stringManager = &stringManager; 
	_RegisterPacket();
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
	GYTRUE == m_packetFactoryBit[packetID] ? GYAssert(GYNULL != packet) : GYAssert(GYNULL != packet); //如果出现断言，则认为多线程同时访问了同一个Factory
	if (GYNULL != packet)
	{
		packet->CleanUp();
	}
	return packet;
}

GYVOID GYPacketFactoryManager::ReleasePacket( GYPacketInteface& packet )
{
	EM_PACKET_ID packetID = GYGetPacketID(packet.GetPacketID());
	GYAssert(packetID >=0 && packetID < EM_PACKET_ID_COUNT);
	GYAssert(GYNULL == m_packetFactory[packetID] && GYTRUE == m_packetFactoryBit[packetID]);
	m_packetFactory[packetID] = &packet;
}

PacketHandler GYPacketFactoryManager::GetPacketHandlerByID( EM_PACKET_ID packetID )
{
	GYAssert(packetID >=0 && packetID < EM_PACKET_ID_COUNT);
	PacketHandler packetHandler = m_packetHandler[packetID];
	GYTRUE == m_packetFactoryBit[packetID] ? GYAssert(GYNULL != packetHandler) : GYAssert(GYNULL != packetHandler); //如果出现断言，则认为多线程同时访问了同一个Factory

	return packetHandler;
}