

#include "Server.h"
#include "EpollSolution.h"
#include "MyProtocol.h"

namespace CWSLib
{
	void Server::init()
	{
		
	}

	void Server::run()
	{
		init();

		EventDispatcher dispatcher;
		dispatcher.init();
		dispatcher.wait(SolutionFunc<MyProtocol, CbContext>);
	}
}
