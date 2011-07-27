/////////////////////////////////////////////
// create time:2011/6/10 11:25
// author:	colin chen
// file name:	GYSocket
// file type:	cpp
////////////////////////////////////////////

//include file
#include "GYSocket.h"
#include <fcntl.h>
#include "GYNetAddress.h"
#ifdef LINUX64
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
//using declare

//foward declare

GYINT32 GYSocket::SetBlock(GYBOOL block)
{
    GYINT32 ret = INVALID_VALUE;
    u_long opt = 0xffffffff;
#ifdef LINUX64
    ret = fcntl(m_fd, F_GETFL);
    opt = GYTRUE == block ? ~O_NONBLOCK : O_NONBLOCK;
    if(-1 != ret)
        ret = fcntl(m_fd, F_SETFL, ret | opt);
#endif
#ifdef WIN32
    opt = GYTRUE == block ? 0 : 1;
    ret = 0 == ioctlsocket(m_fd, FIONBIO, &opt) ? 0 : -1;
#endif
    return INVALID_VALUE != ret ? 0 : INVALID_VALUE;
}

GYSOCKET GYSocket::Open(GYINT32 family, GYINT32 type, GYINT32 protocol)
{
    m_fd = ::socket(family, type, protocol);
    return m_fd;
}

GYINT32 GYSocket::Close()
{
#ifndef WIN32
    ::close(m_fd);
#else
    ::closesocket(m_fd);
#endif
    SetFd(INVALID_SOCKET);
    return 0;
}

GYINT32 GYSocket::Bind(const GYNetAddress& addr)
{
    GYINT32 ret = -1;
    if(GYIsValidSocket(m_fd) )
    {
        ret = 0 == ::bind(m_fd, addr.GetAddress(), addr.GetAddressLength()) ? 0 : -1;
    }
    return ret;
}

GYListenSocket::GYListenSocket()
{
#ifdef LINUX64
	m_dummyFile = ::open("/dev/null", O_RDONLY);
#endif // LINUX64
};

GYListenSocket::~GYListenSocket()
{
#ifdef LINUX64
	::close(m_dummyFile);
#endif // LINUX64
};

const GYINT32 listen_num = 500;

GYINT32 GYListenSocket::Open(const GYNetAddress& addr)
{
    GYINT32 ret = INVALID_VALUE;
    GYSocket::Open(AF_INET, SOCK_STREAM, 0);
    if(0 == Bind(addr))
    {
        ret = ::listen(m_fd, listen_num);
    }
    return ret;
}

GYINT32 GYListenSocket::Accept(GYSocket& s, GYNetAddress& clientAddress)
{
    GYINT32 result = INVALID_VALUE;
    const static GYINT32 len = clientAddress.GetAddressLength();
    sockaddr_in addr;
    clientAddress.CleanUp();
#ifdef WIN32
    GYSOCKET sock = ::accept(m_fd, (sockaddr*)&addr, (GYINT32*)&len);
#endif // WIN32

#ifdef LINUX64
    GYSOCKET sock = ::accept(m_fd, (sockaddr*)&addr, (socklen_t*)&len);
	if (INVALID_SOCKET == sock)
	{
		GYINT32 err = GetLastNetWorkError();
		if (GYEMFILE == err)
		{
			::close(m_dummyFile);
			m_dummyFile = ::accept(m_fd, NULL, NULL);
			::close(m_dummyFile);
			m_dummyFile = ::open("/dev/null", O_RDONLY);
		}
		
	}
	
#endif // LINUX64

    if(GYTRUE == GYIsValidSocket(sock))
    {
        clientAddress.SetAddr(addr.sin_addr.s_addr, GYTRUE);
        clientAddress.SetPort(addr.sin_port, GYTRUE);
        s.SetFd(sock);
        result = 0;
    }
    return result;
}



GYINT32 GYStreamSocket::Open()
{
    GYINT32 result = GYSocket::Open(AF_INET, SOCK_STREAM, 0);
	return result > 0 ? 0 : INVALID_VALUE;
}

GYINT32 GYStreamSocket::Connect(const GYNetAddress& addr)
{
    GYINT32 result = ::connect(m_fd, addr.GetAddress(), addr.GetAddressLength());
    return result;
}

GYINT32 GYStreamSocket::Send(const GYCHAR* buff, GYINT32 len)
{
    GYINT32 result = ::send(m_fd, (char*)buff, len, 0);
	if (result > 0)
	{
		m_totalSend +=result;
	}
	
    return result;
}

GYINT32 GYStreamSocket::Recv(GYCHAR* buff, GYINT32 len)
{
    GYINT32 result = ::recv(m_fd, buff, len, 0);
	if (result > 0)
	{
		m_totalRecv +=result;
	}
    return result;
}

GYINT32
GYStreamSocket::GetPeerName(GYNetAddress& addr)
{
    GYINT32 len = addr.GetAddressLength();
#ifdef WIN32
    GYINT32 result = ::getpeername(m_fd, const_cast<sockaddr *>(addr.GetAddress()), &len);
#endif // WIN32
#ifdef LINUX64
    GYINT32 result = ::getpeername(m_fd, const_cast<sockaddr *>(addr.GetAddress()), (socklen_t*)&len);
#endif // LINUX64
    return result;
}

GYINT32
GYStreamSocket::GetSockName(GYNetAddress& addr)
{
    GYINT32 len = addr.GetAddressLength();
#ifdef WIN32
    GYINT32 result = ::getsockname(m_fd, const_cast<sockaddr *>(addr.GetAddress()), &len);
#endif // WIN32
#ifdef LINUX64
    GYINT32 result = ::getsockname(m_fd, const_cast<sockaddr *>(addr.GetAddress()), (socklen_t*)&len);
#endif // LINUX64
    return result;
}

