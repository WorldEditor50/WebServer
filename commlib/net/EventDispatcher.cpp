

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

	int32_t EventDispatcher::init(std::function<void(std::shared_ptr<Socket>)> func)
	{
		acceptor.Init();
		acceptor.SetNonblocking();
		acceptor.Bind("127.0.0.1", 9091);
		container.Init(acceptor.GetFd());
		container.OnListen([&]() {
			std::shared_ptr<Socket> sock = acceptor.Accept();
			if(sock.get())
				sock->SetNonblocking();
			return sock;
		});
		container.OnRead([func](std::shared_ptr<Socket> sock) {
			func(sock);
			return 0;
		});
		return 0;
	}

	int32_t EventDispatcher::wait()
	{
		while (true)
		{
			if (container.Wait())
			{
				ERROR_LOG("Container wait error.");
				break;
			}
		}
		return 0;
	}
}

