﻿/////////////////////////////////////////////
// create time: 2011/9/6 22:46
// author:	colin chen
// file name:	GYGatewaySession
// file type:	h
////////////////////////////////////////////

#ifndef __GYGATEWAYSESSION_H__
#define __GYGATEWAYSESSION_H__
#include "GYCommonDefine.h"
#include "GYNetAddress.h"
#include "GYBufferStreamSocket.h"
#include "GYEvent.h"
#include "GYObject.h"

const GYINT32 GATEWAY_SESSION_RECV_BUFFER_LEN = 16 * 1024 * 1024;
const GYINT32 GATEWAY_SESSION_SEND_BUFFER_LEN = 16 * 1024 * 1024;

class GYReactor;
class GYPacketInteface;
class GYPacketFactoryManager;
class GYNetWorkManager;
struct GYPacketHead;

class GYGatewaySession : public GYObject
{
	friend GYVOID HandleGatewaySessionReavData(GYNetEvent& event);
	GYBufferStreamSocket<GATEWAY_SESSION_RECV_BUFFER_LEN, GATEWAY_SESSION_SEND_BUFFER_LEN>	m_connection;
	GYNetAddress	m_gatewayAddress;
	GYNetEvent		m_gatewaySessionEvnet;
	GYNetWorkManager* m_networkManger;
	GYReactor*		m_reactor;
	GYINT64			m_allSendDataSize;
	GYINT64			m_allRecvDataSize;
public:
	GYGatewaySession();
	~GYGatewaySession();
	GYVOID	CleanUp();

	GYINT32	Init(GYNetWorkManager& networkManager, GYReactor& reactor, const GYSocket& sock, const GYNetAddress& gatewayAddress);

	GYVOID	Release();

	GYVOID	SendPacket(const GYGUID& guid, const GYPacketInteface& packet);

	GYBOOL	Tick();

private:
	GYVOID	_OnReceive();

	GYVOID	_ProcessInputData(GYPacketFactoryManager& packetFactory, const GYGUID& guid, const GYPacketHead& packetHead);
};

#endif
