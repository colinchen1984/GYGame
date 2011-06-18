/////////////////////////////////////////////
// create time: 2011/6/12 16:58
// author:	colin chen
// file name:	GYBufferStreamSock
// file type:	h
////////////////////////////////////////////

#ifndef __GYBUFFERSTREAMSOCK_H__
#define __GYBUFFERSTREAMSOCK_H__

//include file
#include "GYSocket.h"

//using declare

//foward declare

template< GYINT32 input_buffer_len, GYINT32 output_buffer_len>
class GYBufferStreamSocket : public GYStreamSocket
{
public:
    //GYCycleBuffer<input_buffer_len> m_inputBuffer;
    //GYCycleBuffer<output_buffer_len> m_outputBuffer;

    GYBufferStreamSocket() {};

    ~GYBufferStreamSocket() {}

    GYINT32
    Close()
    {
        GYStreamSocket::Close();
        //m_inputBuffer.Clear();
        //m_outputBuffer.Clear();
        return 0;
    }

    GYINT32
    Send()
    {
// 		GYINT32 buf_len = m_outputBuffer.GetReadSize();
// 		if(0 == buf_len)
// 			return (GYINT32)SOCKNOERROR;
// 		SocketError err = SOCKNOERROR;
// 		const char* reader = m_outputBuffer.ReadPtr();
// 		const char* const tail = m_outputBuffer.GetTail();
// 		if(reader + buf_len <= tail)
// 		{
// 			if(SOCKNOERROR == (err = Send(reader, buf_len)))
// 				m_outputBuffer.ReadPtr(buf_len);
// 		}
// 		else
// 		{
// 			err = Send(reader, tail - reader);
// 			if(SOCKNOERROR == err)
// 			{
// 				m_outputBuffer.ReadPtr(tail - reader);
// 				if(SOCKNOERROR == (err = Send(m_outputBuffer.GetHead(), buf_len - (tail - reader))))
// 					m_outputBuffer.ReadPtr(buf_len - (tail - reader));
// 			}
// 		}
// 		return err;
        return 0;
    }

    GYINT32
    Recv()
    {
// 		//get data from socket buff to cycle buff
// 		GYINT32 buf_len = m_inputBuffer.GetWriteSize();
// 		GYINT32 toread = 0;
// 		if(0 == buf_len)
// 		{
// 			//cycle buff is empty
// 			return (GYINT32)NOINPUTBUFFER;
// 		}
// 		if(GYIOCtl(socket_fd, FIONREAD, &toread))
// 		{
// 			//can not get number of readable bytes from socket buff
// 			return (GYINT32)FAILTOCHECKSOCKBUFFER;
// 		}
//
// 		if(0 == toread)
// 		{
// 			//no readable byte
// 			//try to figure out whether the connection is closed
// 			char t;
// 			GYINT32 ret = GYStreamSocket::Recv((BYTE*)&t, 1);
// 			if(0 == ret)
// 				return (GYINT32)CONNETCTIONCLOSED;
// 			else
// 			{
// 				GYINT32 err = GYGetLastError();
// 				if(SOCKEWOULDBLOCK == err || EINTR == err
// #ifndef WIN32
// 					|| EAGAIN == err
// #endif
// 					)
// 				{
// 					//no more data to read
// 					return (GYINT32)SOCKBUFFEREMPTY;
// 				}
// 				else
// 				{
// 					//the connection is closed by client
// 					return (GYINT32)CONNETCTIONCLOSED;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			if (toread > buf_len)
// 				toread = buf_len - 1;
//
// 			const char* const tail = m_inputBuffer.GetTail();
// 			char* writer = m_inputBuffer.WritePtr();
// 			GYINT32 ret = -1;
// 			GYINT32 ret_ = 0;
// DORECV:
// 			if(tail - writer >= toread)
// 				ret = GYStreamSocket::Recv((BYTE*)writer, toread);
// 			else
// 			{
// 				if(0 < (ret_ = GYStreamSocket::Recv((BYTE*)writer, tail - writer)))
// 					ret = GYStreamSocket::Recv((BYTE*)m_inputBuffer.GetHead(), toread- (tail - writer));
// 			}
//
// 			if(0 == ret)
// 			{
// 				//the connection is closed by client
// 				return (GYINT32)CONNETCTIONCLOSED;
// 			}
// 			else if(0 > ret)
// 			{
// 				GYINT32 err = GYGetLastError();
// 				if(SOCKEWOULDBLOCK == err
// #ifndef WIN32
// 					|| EAGAIN == err
// #endif
// 					)
// 				{
// 					//no more data to read
// 					return (GYINT32)SOCKWOULDBLOCK;
// 				}
// 				else if (EINTR == err)
// 				{
// 					goto DORECV;
// 				}
//
// 				else
// 				{
// 					//the connection is closed by client
// 					return (GYINT32)CONNETCTIONCLOSED;
// 				}
//
// 			}
// 			m_inputBuffer.WritePtr(ret + ret_);
// 		}
// 		return (GYINT32)SOCKNOERROR;
        return 0;
    }

private:
    GYINT32
    Send(const char* p, GYINT32 len)
    {
// 	    SocketError err = SOCKNOERROR;
// 		if(NULL == p)
// 			return  NOSENDCONTENT;
//
// 		GYINT32 n = -1;
// 		for(; len > 0;)
// 		{
// 			n = GYStreamSocket::Send((BYTE*)p, len);
// 			if(n > 0)
// 				len -= n;
// 			else if(-1 == n)
// 			{
// 				GYINT32 err = GYGetLastError();
// 				if(SOCKEWOULDBLOCK == err
// #ifndef WIN32
// 					|| EAGAIN == err
// #endif
// 					)
// 				{
// 					err = SOCKWOULDBLOCK;
// 					break;
// 				}
// 				else if(EINTR == err)
// 				{
// 					continue;
// 				}
// 				else
// 				{
// 					//the connection is closed by client
//                     err = CONNETCTIONCLOSED;
// 					break;
// 				}
// 			}
// 			else if(0 == n)
// 			{
// 			    	err = SENDZEROBYTE;
// 			    	break;
// 			}
//
// 		}
// 		return err;
        return 0;
    }
};
#endif

