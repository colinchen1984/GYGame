/////////////////////////////////////////////
// create time:2011/6/10 11:25
// author:	colin chen
// file name:	GYSocket
// file type:	h
////////////////////////////////////////////
#ifndef __GYSOCKET_H__
#define __GYSOCKET_H__

//include file
#include "GYNetWorkCommonDefine.h"

//using declare

//foward declare
class GYNetAddress;

class GYSocket
{
private:
	GYBOOL m_isRegisterToReactor;
protected:
	GYSOCKET m_fd;
protected:
	GYSocket()
	{ CleanUp();}

	GYSocket(GYSOCKET fd)
	{ m_fd = fd; };

	~GYSocket()
	{};

	GYSocket(const GYSocket& socket)
	{ m_fd = socket.m_fd; };

	 GYVOID
	operator=(const GYSocket& socket)
	{ m_fd = socket.m_fd; };

public:
	GYSOCKET Open(GYINT32 family, GYINT32 type, GYINT32 protocol);

	GYINT32 Close();

	GYINT32 Bind(const GYNetAddress& addr);

    GYSOCKET GetFD()
	{ 
		return m_fd; 
	};

    GYVOID SetFd(GYSOCKET fd)
	{ 
		m_fd = fd; 
	};

	GYINT32 SetBlock(GYBOOL block);

	GYBOOL IsRegisted()
	{
		return m_isRegisterToReactor;
	}

	GYVOID SetRegisted(GYBOOL r)
	{
		m_isRegisterToReactor = r;
	}

	GYVOID CleanUp()
	{
		m_fd = INVALID_SOCKET;
		m_isRegisterToReactor = GYFALSE;
	}
};

class GYListenSocket : public GYSocket
{
public:
	GYListenSocket()
	{};

	~GYListenSocket()
	{};

	GYListenSocket(const GYListenSocket& socket)
		: GYSocket(socket)
	{};

	GYVOID operator=(const GYListenSocket& socket)
	{ 
		GYSocket::operator=(socket); 
	};

	GYINT32 Open(GYNetAddress& addr);

	GYINT32 Accept(GYSocket& s, GYNetAddress& clientAddress);

};

class GYStreamSocket : public GYSocket
{
public:
	GYStreamSocket()
	{};

	~GYStreamSocket()
	{};

	GYStreamSocket(const GYStreamSocket& socket)
		: GYSocket(socket)
	{};

	GYVOID operator=(const GYStreamSocket& socket)
	{ 
		GYSocket::operator=(socket); 
	};

	GYINT32 Open();

	GYINT32 Connect(const  GYNetAddress& addr);

	GYINT32 Send(const GYCHAR* buff, GYINT32 len);

	GYINT32 Recv(GYCHAR* buff, GYINT32 len);

	GYINT32 GetPeerName(GYNetAddress& addr);

	GYINT32 GetSockName(GYNetAddress& addr);

};
#endif
