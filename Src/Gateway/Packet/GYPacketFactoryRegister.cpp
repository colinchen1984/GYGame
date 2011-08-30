/////////////////////////////////////////////
// create time: 2011/8/29 23:47
// author:	colin chen
// file name:	GYPacketFactoryRegister
// file type:	cpp
////////////////////////////////////////////
#include "GYPacketFactoryManager.h"
#include "GYTestPacket.h"
GYVOID GYPacketFactoryManager::_RegisterPacket()
{
	GYPacketInteface* pPacket =GYNULL;

	pPacket = GYNew GYTestPacket(*m_stringManager);
	m_packetFactory[pPacket->GetPacketID()] = pPacket;
	m_packetFactoryBit[pPacket->GetPacketID()] = GYTRUE;
}