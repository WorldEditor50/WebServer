

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <string>
#include <memory>

namespace CWSLib
{
	class Socket
	{
	public:
		Socket();
		Socket(int32_t fd);
		~Socket();

		void Init(int32_t fd);
		void Init();
		int32_t GetFd();
		int32_t GetPort();
		std::string GetHost();
		void Close();
		int32_t SetNonblocking();
		int32_t Connect(const std::string& host, int32_t port);
		int32_t ReadAll(std::string& out);
		int32_t Read(std::string& out, size_t length);
		int32_t Read(char* buf, size_t length);
		int32_t WriteAll(const std::string& content);
		void Bind(const std::string host, int32_t port);
		std::shared_ptr<Socket> Accept();

	private:
		int32_t m_fd;
		std::string m_host;
		int32_t m_port;
		const static int32_t MAX_BUF_SIZE = 30000;
	};
}

#endif // !__SOCKET_H__

