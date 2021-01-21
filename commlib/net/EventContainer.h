

#ifndef __EVENT_CONTAINER_H__
#define __EVENT_CONTAINER_H__

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <functional>

namespace CWSLib
{
	class EventContainer
	{
	public:
		EventContainer();

		int32_t Init(int32_t listenFd);
		int32_t Wait();
		void OnListen(std::function<int32_t()> func);
		void OnRead(std::function<int32_t(int32_t)> func);

	private:
		int32_t m_epfd;
		int32_t m_listenFd;
		epoll_event events[20];
		int timeout;
		int maxEvent;
		std::function<int32_t()> m_listenFunc;
		std::function<int32_t(int32_t)> m_readFunc;
	};
}

#endif // !__EVENT_CONTAINER_H__

