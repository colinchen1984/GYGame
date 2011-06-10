/////////////////////////////////////////////
// create time:2011/6/10 11:25
// author:	colin chen
// file name:	GYSocket
// file type:	cpp
////////////////////////////////////////////
#include "GYSocket.h"

//include file
#include "GYSocket.h"
#include <fcntl.h>

//using declare

//foward declare

GYINT32
GYSock::SetBlock(bool block)
{
	GYINT32 ret = -1;
	u_long opt = 0xffffffff;
#ifndef WIN32
	ret = fcntl(socket_fd, F_GETFL);
	opt = block ? ~O_NONBLOCK : O_NONBLOCK;
	if(-1 != ret)
		ret = fcntl(socket_fd, F_SETFL, ret | opt);
#else
	opt = block ? 0 : 1;
	ret = 0 == ioctlsocket(socket_fd, FIONBIO, &opt) ? 0 : -1;
#endif
	return -1 != ret ? 0 : -1;
}

SOCKET
GYSock::Open(GYINT32 family, GYINT32 type, GYINT32 protocol)
{
	socket_fd = ::socket(family, type, protocol);
	return socket_fd;
}

GYINT32
GYSock::Close()
{
#ifndef WIN32
	::close(socket_fd);
#else
	::closesocket(socket_fd);
#endif
	SetFd(INVALID_SOCKET);
	return 0;
}

GYINT32
GYSock::Bind(const GYNetAddress& addr)
{
	GYINT32 ret = -1;
	if(GYIsValidSocket(socket_fd) )
		ret = 0 == ::bind(socket_fd, addr.GetConstAddr(), addr.GetAddrLen()) ? 0 : -1;
	return ret;
}

const GYINT32 listen_num = 500;

GYINT32
GYListenSock::Open(GYNetAddress& addr)
{
	GYINT32 ret = -1;
	GYSock::Open(AF_INET, SOCK_STREAM, 0);
	if(0 == Bind(addr))
		ret = ::listen(socket_fd, listen_num);
	return ret;
}

SOCKET
GYListenSock::Accept(GYSock& s, GYNetAddress* client_addr)
{
	s.SetFd(Accept(client_addr));
	return s.GetFd();
}

SOCKET
GYListenSock::Accept(GYNetAddress* client_addr /* = NULL */)
{
	const static socklen_t len = client_addr->GetAddrLen();
	sockaddr_in addr;
	GYINT32 sock = ::accept(socket_fd, (sockaddr*)&addr, (GYINT32*)&len);
	if(INVALID_SOCKET  != sock && client_addr)
	{
		client_addr->SetAddr(addr.sin_addr.s_addr);
		client_addr->SetPort(addr.sin_port);
	}
	return sock;
}

GYINT32
GYDgramSock::Open()
{ return GYSock::Open(AF_INET, SOCK_DGRAM, 0); }

GYINT32
GYDgramSock::Connect(GYNetAddress* addr)
{ GYINT32 ret = ::connect(socket_fd, addr->GetConstAddr(), addr->GetAddrLen()); connected = 0 == ret; return ret;}

GYINT32
GYDgramSock::Send(const BYTE* buff, GYINT32 len)
{ return connected ? ::send(socket_fd, (char*)buff, len, 0) : -1; }

GYINT32
GYDgramSock::Recv(BYTE* buff, GYINT32 len)
{ return connected ? ::recv(socket_fd, (char*)buff, len, 0) : -1; }

GYINT32
GYDgramSock::Sendto(const BYTE* buff, GYINT32 len, GYNetAddress& addr)
{ return ::sendto(socket_fd, (const char*)buff, len, 0, addr.GetConstAddr(), addr.GetAddrLen()); }

GYINT32
GYDgramSock::Recvfrom(BYTE* buff, GYINT32 len, GYNetAddress& addr)
{ GYINT32 addr_len = addr.GetAddrLen(); return ::recvfrom(socket_fd, (char*)buff, len, 0, addr.GetSocketAddr(), (socklen_t*)&addr_len); }

GYINT32
GYStreamSock::Open()
{ return GYSock::Open(AF_INET, SOCK_STREAM, 0); }

GYINT32
GYStreamSock::Connect(const GYNetAddress* addr)
{ return ::connect(socket_fd, addr->GetConstAddr(), addr->GetAddrLen()); }

GYINT32
GYStreamSock::Send(const BYTE* buff, GYINT32 len)
{ return ::send(socket_fd, (char*)buff, len, 0); }

GYINT32
GYStreamSock::Recv(BYTE* buff, GYINT32 len)
{ return ::recv(socket_fd, (char*)buff, len, 0); }

GYINT32
GYStreamSock::GetPeerName(GYNetAddress& addr)
{
	GYINT32 len = addr.GetAddrLen();
	return ::getpeername(socket_fd, addr.GetSocketAddr(), (socklen_t*)&len);
}

GYINT32
GYStreamSock::GetSockName(GYNetAddress& addr)
{
	GYINT32 len = addr.GetAddrLen();
	return ::getsockname(socket_fd, addr.GetSocketAddr(), (socklen_t*)&len);
}
