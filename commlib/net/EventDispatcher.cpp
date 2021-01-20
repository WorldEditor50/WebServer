

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
	const static int LISTENQ = 20;

	EventDispatcher::EventDispatcher() :
		m_epSock(epoll_create(256))
	{
		timeout = 200;
		maxEvent = 100;
	}

	EventDispatcher::~EventDispatcher()
	{
	}

	int32_t EventDispatcher::init(int port, std::function<void(CWSLib::Socket&, const std::string&)> cbFunc)
	{
		m_listenSock.SetNonblocking();

		epoll_event initEvent;
		initEvent.data.fd = m_listenSock.GetFd();
		initEvent.events = EPOLLIN | EPOLLET; // 设置要处理的事件类型
		epoll_ctl(m_epSock.GetFd(), EPOLL_CTL_ADD, m_listenSock.GetFd(), &initEvent); // 注册epoll事件

		sockaddr_in serverAddr;
		bzero(&serverAddr, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		std::string localAddr = "127.0.0.1";
		inet_aton(localAddr.c_str(), &(serverAddr.sin_addr));

		//int portNumber = SERV_PORT;
		serverAddr.sin_port = htons(port);
		bind(m_listenSock.GetFd(), (sockaddr*)&serverAddr, sizeof(serverAddr));
		listen(m_listenSock.GetFd(), LISTENQ);

		mCb = cbFunc;
		return 0;
	}

	int32_t EventDispatcher::tick()
	{
		char hostBuf[NI_MAXHOST]; // IP地址缓存
		char portBuf[NI_MAXSERV]; // PORT缓存
		epoll_event events[20];

		// 等待epoll事件的发生
		int fdNum = epoll_wait(m_epSock.GetFd(), events, maxEvent, timeout);
		// 处理所发生的所有事件
		for (int i = 0; i < fdNum; ++i)
		{
			if (events[i].data.fd == m_listenSock.GetFd())
			{
				// 由于采用了边缘触发模式，这里需要使用循环，保证所有新的连接都被注册
				for (; ; )
				{
					sockaddr_in clientAddr = { 0 };
					socklen_t addrSize = sizeof(clientAddr);
					int connFd = accept(m_listenSock.GetFd(), (sockaddr*)&clientAddr, &addrSize);
					if (-1 == connFd)
					{
						break;
					}
					Socket newConn(connFd);
					int ret = getnameinfo((sockaddr*)&clientAddr, sizeof(clientAddr),
						hostBuf, sizeof(hostBuf) / sizeof(hostBuf[0]),
						portBuf, sizeof(portBuf) / sizeof(portBuf[0]),
						NI_NUMERICHOST | NI_NUMERICSERV);
					if (!ret)
					{
						DEBUG_LOG("New connection: host = %s, port = %s\n", hostBuf, portBuf);
					}
					DEBUG_LOG("Receive new connection[%d]\n", connFd);

					newConn.SetNonblocking();
					epoll_event registEvent;
					registEvent.data.fd = connFd; // 设置用于读操作的文件描述符
					registEvent.events = EPOLLIN | EPOLLET; // 设置用于注册的读操作事件
					// 为新accept的 file describe 设置epoll事件
					if (-1 == epoll_ctl(m_epSock.GetFd(), EPOLL_CTL_ADD, connFd, &registEvent))
					{
						ERROR_LOG("epoll_ctl");
						return -1;
					}
				}
			}
			else if (events[i].events & EPOLLIN)
			{
				std::string out;
				//int sockFd;
				//如果是已经连接的用户，并且收到数据，那么进行读入。
				DEBUG_LOG("EPOLLIN");
				Socket sock(events[i].data.fd);
				if (sock.GetFd() < 0)
					continue;
				ssize_t socketLen;
				if ((socketLen = sock.ReadAll(out)) < 0)
				{
					if (errno == ECONNRESET)
					{
						sock.Close();
						events[i].data.fd = -1;
						//epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
						DEBUG_LOG("Close socket[%d]\n", sock.GetFd());
					}
					else
					{
						DEBUG_LOG("read out");
					}
				}
				else if (socketLen == 0)
				{
					sock.Close();
					events[i].data.fd = -1;
					//epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
					DEBUG_LOG("Close socket[%d]\n", sock.GetFd());
				}
				DEBUG_LOG("Read line[%s]\n", out.c_str());
				epoll_event registEvent;
				registEvent.data.fd = sock.GetFd(); // 设置用于写操作的文件描述符
				registEvent.events = EPOLLOUT | EPOLLET; // 设置用于注册的写操作事件
				epoll_ctl(m_epSock.GetFd(), EPOLL_CTL_MOD, sock.GetFd(), &registEvent); // 修改sockFd上要处理的事件为EPOLLOUT
				mCb(sock, out);
			}
			else if (events[i].events & EPOLLOUT) // 如果有数据发送
			{
				//char line[MAXLINE];
				int sockFd = events[i].data.fd;
				//writeAll(sockFd, line, strnlen(line, sizeof(line)));
				epoll_event registEvent;
				registEvent.data.fd = sockFd; // 设置用于读操作的文件描述符
				registEvent.events = EPOLLIN | EPOLLET; // 设置用于注册的读操作事件
				epoll_ctl(m_epSock.GetFd(), EPOLL_CTL_MOD, sockFd, &registEvent); // 修改sockFd上要处理的事件为EPOLIN
			}
		}

		return 0;
	}
}

