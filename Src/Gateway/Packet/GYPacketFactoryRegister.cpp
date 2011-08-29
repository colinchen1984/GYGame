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
	GYPacketInteface* pPacket = GYNew GYTestPacket();
	m_packetFactory[pPacket->GetPacketID()] = pPacket;
}