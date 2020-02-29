

#ifndef __SERVER_H__
#define __SERVER_H__

#include "EpollSolution.h"
#include "MyProtocol.h"

namespace CWSLib
{
	class Server
	{
	public:
		void run();

	private:
		void init();
		void loop();

	private:
		EventDispatcher dispatcher;

	};
}

#endif // !__SERVER_H__


