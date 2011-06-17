/////////////////////////////////////////////
// create time:2011/6/10 11:43
// author:	colin chen
// file name:	GYNetAddress
// file type:	cpp
////////////////////////////////////////////
#include "GYNetAddress.h"

GYNetAddress::GYNetAddress()
{
	CleanUp();
}

GYNetAddress::~GYNetAddress()
{

}

GYVOID GYNetAddress::CleanUp()
{
	GYZeroMem(&m_sockAddr, sizeof(m_sockAddr));
	m_sockAddr.sin_family = AF_INET;
#ifdef _DEBUG
	GYZeroMem(&m_addressString, sizeof(m_addressString));
	m_port = INVALID_VALUE;
#endif // _DEBUG

}

GYVOID GYNetAddress::operator=(const GYNetAddress& addr) 
{
	GYMemcpy(&m_sockAddr, &addr.m_sockAddr, sizeof(m_sockAddr));
#ifdef _DEBUG
	GYMemcpy(m_addressString, addr.m_addressString, sizeof(m_addressString));
	m_port = addr.m_port;
#endif
}

GYBOOL GYNetAddress::operator==(const GYNetAddress& addr)const 
{
	GYBOOL result = GYFALSE;
	result = m_sockAddr.sin_addr.s_addr == addr.m_sockAddr.sin_addr.s_addr ? GYTRUE : GYFALSE;
	return result;
}


GYINT32 GYNetAddress::SetAddr(const char* addr)
{	
	GYINT32 result = INVALID_VALUE;
	if(INADDR_NONE != (m_sockAddr.sin_addr.s_addr = inet_addr(addr)))
	{
		result = 0;
#ifdef _DEBUG
		GYStrncpy(m_addressString, addr, sizeof(m_addressString));
#endif
	}
	return result;
}

GYINT32 GYNetAddress::SetPort(GYUINT16 port, GYBOOL bIsNetOrder)
{
	m_sockAddr.sin_port = GYTRUE == bIsNetOrder ? port : htons(port);
#ifdef _DEBUG
	m_port = GYTRUE == bIsNetOrder ? ntohs(port) : port;
#endif
	return 0;
}

GYINT32 GYNetAddress::SetAddr(GYINT32 addr, GYBOOL is_net_order)
{
	GYINT32 err = 0;
	m_sockAddr.sin_addr.s_addr = GYTRUE == is_net_order ? addr :htonl(addr);
	GYCHAR address[MaxAddrStringLengh] = {0};
	if(0 != GetAddressString(address, sizeof(address)))
	{
		err = -1;
		GYMemset(&m_sockAddr, 0, sizeof(m_sockAddr));
	}
	else
	{
#ifdef _DEBUG
		GYStrncpy(m_addressString, address, sizeof(m_addressString));
#endif // _DEBUG

	}
	return err;
}

GYINT32 GYNetAddress::GetAddressString(char* address, GYINT32 len)
{
	GYINT32 result = INVALID_VALUE;
	do 
	{
		if (0 != _inet_ntoa_r(m_sockAddr.sin_addr, address, len))
		{
			break;
		}
		result = 0;
	} while (GYFALSE);
	return result;
}

const sockaddr* GYNetAddress::GetAddress() const
{
	return reinterpret_cast<const sockaddr*>(&m_sockAddr);
}

GYUINT32 GYNetAddress::_inet_ntoa_r(in_addr in, GYCHAR* buf, GYINT32 buflen) 
{
	GYUINT32 addr = in.s_addr;
	register GYUINT8* addr_p = (GYUINT8*)&addr;
	register GYUINT8* t = GYNULL;
	register GYINT32 i = 0;
	register GYINT32 pos  = 0;
	GYUINT8 tmp[4] = {0};

	for (i = 4, pos = 0; ; addr_p++) 
	{
		i--;
		t = tmp;
		do 
		{
			*t++ = "0123456789"[*addr_p % 10];
		} while (*addr_p /= 10);
		for (; t > tmp; pos++)
		{
			if (pos >= buflen)
			{
				return INVALID_VALUE;
			}
			buf[pos] = *--t;
		}
		if (!i)
		{
			break;
		}
		if (pos >= buflen)
		{
			return INVALID_VALUE;
		}
		buf[pos++] = '.';
	}

	if (pos >= buflen)
	{
		return INVALID_VALUE;
	}
	buf[pos] = 0;

	return 0;
}

