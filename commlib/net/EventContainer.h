

#ifndef __EVENT_CONTAINER_H__
#define __EVENT_CONTAINER_H__

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <functional>
#include <map>
#include <memory>

#include "Socket.h"

namespace CWSLib
{
	struct SockCtx
	{
		bool available;
		std::shared_ptr<Socket> sock;
	};

	class EventContainer
	{
	public:
		EventContainer();

		int32_t Init(int32_t listenFd);
		int32_t Wait();
		void OnListen(std::function<std::shared_ptr<Socket>()> func);

	private:
		int32_t m_epfd;
		int32_t m_listenFd;
		epoll_event events[20];
		std::map<int32_t, std::shared_ptr<SockCtx>> m_sockMap;
		int timeout;
		int maxEvent;
		std::function<std::shared_ptr<Socket>()> m_listenFunc;
	};
}

#endif // !__EVENT_CONTAINER_H__

