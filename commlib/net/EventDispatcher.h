

#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__

#include <string>
#include <functional>

#include "Socket.h"

namespace CWSLib
{
	class EventDispatcher
	{
	public:
		EventDispatcher();

		~EventDispatcher();

		int32_t init(int port, std::function<void(CWSLib::Socket&, const std::string&)> cbFunc);

		int32_t tick();

	private:

	private:
		//int epfd;
		//int listenFd;
		Socket m_epSock;
		Socket m_listenSock;
		int timeout;
		int maxEvent;
		std::function<void(CWSLib::Socket&, const std::string&)> mCb;
	};
}

#endif // !__EVENT_DISPATCHER_H__
