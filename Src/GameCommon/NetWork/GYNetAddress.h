/////////////////////////////////////////////
// create time:2011/6/10 11:27
// author:	colin chen
// file name:	GYNetAddress
// file type:	h
////////////////////////////////////////////
#ifndef __GYNETADDRESS_H__
#define __GYNETADDRESS_H__

#include "GYCommonDefine.h"
#include "GYNetWorkCommonDefine.h"

class GYNetAddress
{
private:
    sockaddr_in		m_sockAddr;
    static const GYINT32 MaxAddrStringLengh = 32;
#ifdef _DEBUG
    GYCHAR			m_addressString[MaxAddrStringLengh];
    GYUINT16		m_port;

#endif
public:
    GYNetAddress();

    ~GYNetAddress();

    GYVOID operator=(const GYNetAddress& addr);

    GYBOOL operator==(const GYNetAddress& addr)const ;

    GYINT32 SetAddr(const char* addr);

    GYINT32 SetPort(GYUINT16 port, GYBOOL bIsNetOrder = GYFALSE);

    GYVOID CleanUp();

    GYINT32 SetAddr(GYINT32 addr, GYBOOL is_net_order = GYFALSE);

    GYINT32 GetAddressString(GYCHAR* addr, GYINT32 len);

    GYINT32 GetAddressLength()const
    {
        return sizeof(sockaddr_in);
    };

    const sockaddr* GetAddress() const;
private:
    GYUINT32 _inet_ntoa_r(in_addr in, GYCHAR* buf, GYINT32 buflen);
//
// 	GYINT32
// 	SetAddrByName(const char* name)
// 	{
//         GYINT32 ret = -1;
//         hostent* remoteHost = gethostbyname(name);//, 4, AF_INET);
//         if(remoteHost && remoteHost->h_addrtype == AF_INET && remoteHost->h_addr_list[0] != 0)
//         {
//             ret = SetAddr(*(u_long *) remoteHost->h_addr_list[0]);
//         }
//         return ret;
//     }
//
// 	char*
// 	GetHostName(char* name, GYINT32 name_len)
// 	{
//         if(0 == gethostname(name, name_len))
//             return name;
//         else
//             return NULL;
//     }
//
//     const sockaddr*
// 	GetConstAddr() const
// 	{ return (const sockaddr*)(&m_sockAddr); }
//
//     sockaddr*
// 	GetSocketAddr()
// 	{ return reinterpret_cast< sockaddr* >(&m_sockAddr); };
//

//
// 	const char*
// 	GetAddr() const
// 	{
// 		return m_addressString;
// 	}
//
// 	GYUSHORT
// 	GetPort() const
// 	{
// 		return  m_usPort;
// 	}
//

};
#endif

