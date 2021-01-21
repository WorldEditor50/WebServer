

#include "EventDispatcher.h"
#include "commlib/app/MacroAssemble.h"

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

namespace CWSLib
{
	EventDispatcher::EventDispatcher()
	{
	}

	EventDispatcher::~EventDispatcher()
	{
	}

	int32_t EventDispatcher::init(std::function<void(Socket&, const std::string&)> func)
	{
		int32_t lsFd = acceptor.Init("127.0.0.1", 9091);
		container.Init(lsFd);
		container.OnListen([&]() {
			std::shared_ptr<Socket> sock = acceptor.Accept();
			if (!sock)
			{
				return -1;
			}
			return sock->GetFd();
		});
		container.OnRead([func](int32_t fd) {
			Socket sock(fd);
			std::string out;
			int32_t readSize = sock.ReadAll(out);
			if (readSize < 0 && errno == ECONNRESET)
			{
				sock.Close();
				return -1;
			}
			else if (readSize == 0)
			{
				sock.Close();
				return -1;
			}
			func(sock, out);
			return 0;
		});
		return 0;
	}

	int32_t EventDispatcher::tick()
	{
		return container.Wait();
	}
}

