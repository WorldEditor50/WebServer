

#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__

#include <string>
#include <functional>

#include "Socket.h"
#include "EventContainer.h"
#include "Acceptor.h"

namespace CWSLib
{
	class EventDispatcher
	{
	public:
		EventDispatcher();

		~EventDispatcher();

		int32_t init(std::function<void(Socket&, const std::string&)> func);

		int32_t tick();

	private:

	private:
		EventContainer container;
		Acceptor acceptor;
	};
}

#endif // !__EVENT_DISPATCHER_H__
