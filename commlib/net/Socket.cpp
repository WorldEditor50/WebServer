

#include "Socket.h"

#include <unistd.h>
#include <fcntl.h>

namespace CWSLib
{
	Socket::Socket()
	{
		
	}

	Socket::Socket(int32_t fd)
	{
		Init(fd);
	}

	Socket::~Socket()
	{
		Close();
	}

	void Socket::Init(int32_t fd)
	{
		m_fd = fd;
	}

	void Socket::Init()
	{
		m_fd = socket(AF_INET, SOCK_STREAM, 0);
	}

	int32_t Socket::GetFd()
	{
		return m_fd;
	}

	void Socket::Close()
	{
		if (m_fd != 0)
			close(m_fd);
	}

	int32_t Socket::SetNonblocking()
	{
		int opts;
		opts = fcntl(m_fd, F_GETFL);
		if (opts < 0)
		{
			return -1;
		}
		opts = opts | O_NONBLOCK;
		if (fcntl(m_fd, F_SETFL, opts) < 0)
		{
			return -2;
		}
		return 0;
	}

	int32_t Socket::ReadAll(std::string& out)
	{
		char buf[MAX_BUF_SIZE] = { 0 };
		int32_t recvSum = 0;
		int32_t recvCnt = recv(m_fd, buf, MAX_BUF_SIZE, MSG_DONTWAIT | MSG_PEEK);
		while (true)
		{
			int32_t readLen = read(m_fd, buf + recvSum, MAX_BUF_SIZE - recvSum);
			if (readLen < 0)
			{
				return -1;
			}
			else if (readLen == 0)
			{
				break;
			}
			
			recvSum += readLen;
			//write(sock, buf, str_len);
			if (recvSum >= recvCnt)
				break;
		}
		out.append(buf);
		return recvSum;
	}

	int32_t Socket::WriteAll(const std::string& content)
	{
		int32_t sendCnt = 0;
		size_t size = content.length();
		while (true)
		{
			int32_t senSize = write(m_fd, content.c_str() + sendCnt, size - sendCnt);
			if (senSize < 0)
			{
				return -1;
			}
			else if (senSize == 0)
			{
				break;
			}
			sendCnt += senSize;
		}
		//write(sock, buf, size);
		return sendCnt;
	}

}
