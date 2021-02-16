

#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__

#include <string>
#include <functional>
#include <memory>

#include "Socket.h"
#include "EventContainer.h"

namespace CWSLib
{
	class EventDispatcher
	{
	public:
		EventDispatcher();
		~EventDispatcher();

		int32_t init();
		int32_t wait();

	private:

	private:
		EventContainer container;
		Socket acceptor;
	};
}

#endif // !__EVENT_DISPATCHER_H__
