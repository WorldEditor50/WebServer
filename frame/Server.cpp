

#include "commlib/basic/CommonSingleton.h"
#include "commlib/app/MacroAssemble.h"

#include "Server.h"

namespace CWSLib
{
	void Server::init()
	{
		dispatcher.init(SolutionFunc<MyProtocol, CbContext>);
		CWSLib::ThreadPool& thdPool = CWSLib::CommSingleton<CWSLib::ThreadPool>::instance();
		thdPool.init(5, 100, 20);
		NORMAL_LOG("Finish init server");
	}

	void Server::loop()
	{
		CWSLib::ThreadPool& thdPool = CWSLib::CommSingleton<CWSLib::ThreadPool>::instance();
		while (true)
		{
			if (dispatcher.wait() < 0)
			{
				break;
			}
		}
	}

	void Server::run()
	{
		init();
		loop();
	}
}
