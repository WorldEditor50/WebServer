

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <string>

namespace CWSLib
{
	class Socket
	{
	public:
		Socket();
		Socket(int32_t fd);
		~Socket();

		int32_t GetFd();
		void Close();
		int32_t SetNonblocking();
		int32_t ReadAll(std::string& out);
		int32_t WriteAll(const std::string& content);

	private:
		int32_t m_fd;
		const static int32_t MAX_BUF_SIZE = 30000;
	};
}

#endif // !__SOCKET_H__

