

#include "EventContainer.h"
#include "commlib/app/MacroAssemble.h"

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>

namespace CWSLib {
	EventContainer::EventContainer()
	{
		maxEvent = 100;
		timeout = 200;
	}

	int32_t EventContainer::Init(int32_t listenFd)
	{
		m_listenFd = listenFd;
		m_epfd = epoll_create(256);
		epoll_event initEvent;
		initEvent.data.fd = listenFd;
		initEvent.events = EPOLLIN | EPOLLET; // 设置要处理的事件类型
		epoll_ctl(m_epfd, EPOLL_CTL_ADD, listenFd, &initEvent); // 注册epoll事件
		for (int i = 0; i < sizeof(events) / sizeof(events[0]); ++i)
		{
			bzero(&events[i], sizeof(events[0]));
		}
		return m_epfd;
	}

	int32_t EventContainer::Wait()
	{
		// 等待epoll事件的发生
		int fdNum = epoll_wait(m_epfd, events, maxEvent, timeout);
		// 处理所发生的所有事件
		for (int i = 0; i < fdNum; ++i)
		{
			NORMAL_LOG("==X==");
			if (events[i].data.fd == m_listenFd)
			{
				// 由于采用了边缘触发模式，这里需要使用循环，保证所有新的连接都被注册
				for (; ; )
				{
					std::shared_ptr<Socket> sock = m_listenFunc();
					int connFd = sock->GetFd();
					m_sockMap.insert(std::make_pair(connFd, sock));
					if (connFd < 0)
					{
						ERROR_LOG("Accept new socket failed");
						return -1;
					}
					epoll_event registEvent;
					registEvent.data.fd = connFd; // 设置用于读操作的文件描述符
					registEvent.events = EPOLLIN | EPOLLET; // 设置用于注册的读操作事件
					// 为新accept的 file describe 设置epoll事件
					if (-1 == epoll_ctl(m_epfd, EPOLL_CTL_ADD, connFd, &registEvent))
					{
						ERROR_LOG("epoll_ctl");
						return -1;
					}
				}
			}
			else if (events[i].events & EPOLLIN)
			{
				NORMAL_LOG("==X==");
				// 如果是已经连接的用户，并且收到数据，那么进行读入。
				auto itor = m_sockMap.find(events[i].data.fd);
				if (itor == m_sockMap.end())
				{
					ERROR_LOG("fd [%d] not cached.", events[i].data.fd);
					return -1;
				}
				int ret = m_readFunc(itor->second);
				NORMAL_LOG("==X==");
				if (ret < 0)
				{
					DEBUG_LOG("Read from [%d] failed.", events[i].data.fd);
					events[i].data.fd = -1;
				}
				epoll_event registEvent;
				registEvent.data.fd = events[i].data.fd; // 设置用于写操作的文件描述符
				registEvent.events = EPOLLOUT | EPOLLET; // 设置用于注册的写操作事件
				epoll_ctl(m_epfd, EPOLL_CTL_MOD, events[i].data.fd, &registEvent); // 修改sockFd上要处理的事件为EPOLLOUT
				NORMAL_LOG("==X==");
			}
			else if (events[i].events & EPOLLOUT) // 如果有数据发送
			{
				int sockFd = events[i].data.fd;
				epoll_event registEvent;
				registEvent.data.fd = sockFd; // 设置用于读操作的文件描述符
				registEvent.events = EPOLLIN | EPOLLET; // 设置用于注册的读操作事件
				epoll_ctl(m_epfd, EPOLL_CTL_MOD, sockFd, &registEvent); // 修改sockFd上要处理的事件为EPOLIN
			}
		}

		return 0;
	}

	void EventContainer::OnListen(std::function<std::shared_ptr<Socket>()> func)
	{
		m_listenFunc = func;
	}

	void EventContainer::OnRead(std::function<int32_t(std::shared_ptr<Socket>)> func)
	{
		m_readFunc = func;
	}

}
