/////////////////////////////////////////////
// create time:2011/6/10 11:25
// author:	colin chen
// file name:	GYSocket
// file type:	h
////////////////////////////////////////////
#ifndef __GYSOCKET_H__
#define __GYSOCKET_H__

//include file
#include <stdio.h>

//using declare

//foward declare

class GYSock
{
protected:
	SOCKET socket_fd;
protected:
	GYSock()
	{ CleanUp();}

	GYSock(SOCKET fd)
	{ socket_fd = fd; };

	~GYSock()
	{};

	GYSock(const GYSock& socket)
	{ socket_fd = socket.socket_fd; };

	 GYVOID
	operator=(const GYSock& socket)
	{ socket_fd = socket.socket_fd; };

public:
	SOCKET
	Open(GYINT32 family, GYINT32 type, GYINT32 protocol);

	GYINT32
	Close();

	GYINT32
	Bind(const GYNetAddress& addr);

    SOCKET
	GetFd()
	{ return socket_fd; };

    GYVOID
	SetFd(SOCKET fd)
	{ socket_fd = fd; };

	GYINT32
	SetBlock(bool block);

	GYVOID
	CleanUp()
	{socket_fd = INVALID_SOCKET;}
};

class GYListenSock : public GYSock
{
public:
	GYListenSock()
	{};

	~GYListenSock()
	{};

	GYListenSock(const GYListenSock& socket)
		: GYSock(socket)
	{};

	 GYVOID
	operator=(const GYListenSock& socket)
	{ GYSock::operator=(socket); };

	GYINT32
	Open(GYNetAddress& addr);

	SOCKET
	Accept(GYNetAddress*  client_addr = NULL);

	SOCKET
	Accept(GYSock& s, GYNetAddress* client_addr = NULL);

    SOCKET
	Accept(GYNetAddress& client_addr)
	{ return Accept(&client_addr); }

};

class GYDgramSock : public GYSock
{
	bool connected;
public:
	GYDgramSock() :connected(false)
	{};

	~GYDgramSock()
	{};

	GYDgramSock(const GYDgramSock& socket)
		: GYSock(socket), connected(socket.connected)
	{};

	 GYVOID
	operator=(const GYDgramSock& socket)
	{ GYSock::operator=(socket);};

	GYINT32
	Open();

	GYINT32
	Connect(GYNetAddress* addr);

	GYINT32
	Connect(GYNetAddress& addr)
	{ return Connect(&addr); };

	GYINT32
	Send(const BYTE* buff, GYINT32 len);

	GYINT32
	Recv(BYTE* buff, GYINT32 len);

	GYINT32
	Sendto(const BYTE* buff, GYINT32 len, GYNetAddress& addr);

	GYINT32
	Recvfrom(BYTE* buff, GYINT32 len, GYNetAddress& addr);

};

class GYStreamSock : public GYSock
{
public:
	GYStreamSock()
	{};

	~GYStreamSock()
	{};

	GYStreamSock(const GYStreamSock& socket)
		: GYSock(socket)
	{};

	GYVOID
	operator=(const GYStreamSock& socket)
	{ GYSock::operator=(socket); };

	GYINT32
	Open();

	GYINT32
	Connect(const GYNetAddress* addr);

	GYINT32
	Connect(const GYNetAddress& addr)
	{ return Connect(&addr); };

	GYINT32
	Send(const BYTE* buff, GYINT32 len);

	GYINT32
	Recv(BYTE* buff, GYINT32 len);

	GYINT32
	GetPeerName(GYNetAddress& addr);

	GYINT32
	GetSockName(GYNetAddress& addr);

};


#endif
