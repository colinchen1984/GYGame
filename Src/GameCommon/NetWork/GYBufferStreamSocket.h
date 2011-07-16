/////////////////////////////////////////////
// create time: 2011/6/12 16:58
// author:	colin chen
// file name:	GYBufferStreamSock
// file type:	h
////////////////////////////////////////////

#ifndef __GYBUFFERSTREAMSOCK_H__
#define __GYBUFFERSTREAMSOCK_H__

#include "GYSocket.h"
#include "GYCycleBuffer.h"
#include "GYNetWorkErrorCode.h"

template< GYINT32 input_buffer_len, GYINT32 output_buffer_len>
class GYBufferStreamSocket : public GYStreamSocket
{
public:
    GYCycleBuffer<input_buffer_len> m_inputBuffer;
    GYCycleBuffer<output_buffer_len> m_outputBuffer;

    GYBufferStreamSocket() {};

    ~GYBufferStreamSocket() {}

    GYINT32 Close()
    {
        GYStreamSocket::Close();
        m_inputBuffer.CleanUp();
        m_outputBuffer.CleanUp();
        return 0;
    }

    GY_SOCKET_OPERATION_ERROR_CODE _Send()
    {
			GYINT32 bufferLength = m_outputBuffer.GetReadSize();
			if(0 == bufferLength)
			{
				return GY_SOCKET_OPERATION_ERROR_CODE_SUCESS;
			}

			const char* reader = m_outputBuffer.ReadPtr();
			const char* const tail = m_outputBuffer.GetTail();
			if(reader + bufferLength <= tail)
			{
				GY_SOCKET_OPERATION_ERROR_CODE result = _Send(reader, bufferLength);
				if(GY_SOCKET_OPERATION_ERROR_CODE_SUCESS == result)
				{
					m_outputBuffer.ReadPtr(bufferLength);
				}
				else
				{
					return result;
				};
			}
			else
			{
				GY_SOCKET_OPERATION_ERROR_CODE result = _Send(reader, tail - reader);
				if(GY_SOCKET_OPERATION_ERROR_CODE_SUCESS != result)
				{
					return result;
				}
				m_outputBuffer.ReadPtr(tail - reader);
				result = _Send(m_outputBuffer.GetHead(), bufferLength - (tail - reader));
				if(GY_SOCKET_OPERATION_ERROR_CODE_SUCESS == result)
				{
					m_outputBuffer.ReadPtr(bufferLength - (tail - reader));
				}
				else
				{
					return result;
				}
			}
			return GY_SOCKET_OPERATION_ERROR_CODE_SUCESS;
    }

    GY_SOCKET_OPERATION_ERROR_CODE Recv()
    {
 		//get data from socket buff to cycle buff
		GYINT32 buf_len = m_inputBuffer.GetWriteSize();
		GYINT32 toread = 0;
		if(0 == buf_len)
		{
			//cycle buff is empty
			return GY_SOCKET_OPERATION_ERROR_CODE_NO_MORE_BUFFER_FOR_READ;
		}
		if(0 != GYIOCtl(m_fd, FIONREAD, &toread))
		{
			//can not get number of readable bytes from socket buff
			return GY_SOCKET_OPERATION_ERROR_CODE_FAIL_TO_CHECK_SOCKET_CORE_BUFFER;
		}

		if(0 == toread)
		{
			//no readable byte
			//try to figure out whether the connection is closed
			char t;
			GYINT32 ret = GYStreamSocket::Recv((GYCHAR*)&t, 1);
			if(0 == ret)
			{
				return GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED;
			}
			else
			{
				GYINT32 err = GetLastNetWorkError();
				if(GYSOCKEWOULDBLOCK == err || GYEINTR == err
#ifndef WIN32
					|| GYEAGAIN == err
#endif
					)
				{
					//no more data to read
					return GY_SOCKET_OPERATION_ERROR_CODE_SOCKET_CORE_BUFFER_EMPTY;
				}
				else
				{
					//the connection is closed by client
					return GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED;
				}
			}
		}
		else
		{
			if (toread > buf_len)
			{
				toread = buf_len - 1;
			}

			const char* const tail = m_inputBuffer.GetTail();
			char* writer = m_inputBuffer.WritePtr();
			GYINT32 ret = INVALID_VALUE;
			GYINT32 ret_ = 0;
DORECV:
			if(tail - writer >= toread)
			{
				ret = GYStreamSocket::Recv(writer, toread);
			}
			else
			{
				if(0 < (ret_ = GYStreamSocket::Recv(writer, tail - writer)))
				{
					ret = GYStreamSocket::Recv((GYCHAR*)m_inputBuffer.GetHead(), toread- (tail - writer));
				}
			}

			if(0 == ret)
			{
				//the connection is closed by client
				return GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED;
			}
			else if(0 > ret)
			{
				GYINT32 err = GetLastNetWorkError();
				if(GYSOCKEWOULDBLOCK == err
#ifndef WIN32
					|| GYEAGAIN == err
#endif
					)
				{
					//no more data to read
					return GY_SOCKET_OPERATION_ERROR_CODE_WOULD_BLOCK;
				}
				else if (GYEINTR == err)
				{
					goto DORECV;
				}

				else
				{
					//the connection is closed by client
					return GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED;
				}

			}
			m_inputBuffer.WritePtr(ret + ret_);
		}
		return GY_SOCKET_OPERATION_ERROR_CODE_SUCESS;
    }

private:
    GY_SOCKET_OPERATION_ERROR_CODE _Send(const char* p, GYINT32 len)
    {
		if (GYNULL == p)
		{
			return GY_SOCKET_OPERATION_ERROR_CODE_WRONG_PARAM;
		}
		GYINT32 sendResult = 0;
		while (len > 0)
		{
			sendResult = GYStreamSocket::Send((BYTE*)p, len);
			if (sendResult > 0)
			{
				len -= sendResult;
			}
			else if(INVALID_VALUE == sendResult)
			{
				GYINT32 err = GetLastNetWorkError();
				if(GYSOCKEWOULDBLOCK == err
#ifndef WIN32
					|| GYEAGAIN == err
#endif
					)
				{
					return GY_SOCKET_OPERATION_ERROR_CODE_WOULD_BLOCK;
				}
				else if(GYEINTR == err)
				{
					continue;
				}
				else
				{
					//the connection is closed by client
					return GY_SOCKET_OPERATION_ERROR_CODE_CONNECTION_CLOSED;
				}
			}
		}
		
		return GY_SOCKET_OPERATION_ERROR_CODE_SUCESS;
    }
};
#endif

