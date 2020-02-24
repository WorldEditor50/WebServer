

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

const static int MAXLINE = 30000;
const static int OPEN_MAX = 100;
const static int LISTENQ = 20;
const static int SERV_PORT = 12000;
const static int INFTIM = 1000;

namespace CWSLib
{
	EventDispatcher::EventDispatcher()
	{
	}

	EventDispatcher::~EventDispatcher()
	{
	}

	int32_t EventDispatcher::init()
	{
		std::cout << "Initialize service...\n";
		epfd = epoll_create(256); // �������ڴ���accept��epollר�õ��ļ�������
		listenFd = socket(AF_INET, SOCK_STREAM, 0);
		setNonblocking(listenFd); // ��socket����Ϊ��������ʽ

		epoll_event initEvent;
		initEvent.data.fd = listenFd; // ������Ҫ������¼���ص��ļ�������
		initEvent.events = EPOLLIN | EPOLLET; // ����Ҫ������¼�����
		epoll_ctl(epfd, EPOLL_CTL_ADD, listenFd, &initEvent); // ע��epoll�¼�

		sockaddr_in serverAddr;
		bzero(&serverAddr, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		std::string localAddr = "127.0.0.1";
		inet_aton(localAddr.c_str(), &(serverAddr.sin_addr));

		int portNumber = SERV_PORT;
		serverAddr.sin_port = htons(portNumber);
		bind(listenFd, (sockaddr*)&serverAddr, sizeof(serverAddr));
		listen(listenFd, LISTENQ);

		return 0;
	}

	int32_t EventDispatcher::wait(__svcCallback cbFunc)
	{
		DEBUG_LOG("Start waiting...");
		epoll_event events[20];
		for (; ; )
		{
			char hostBuf[NI_MAXHOST]; // IP��ַ����
			char portBuf[NI_MAXSERV]; // PORT����

			// �ȴ�epoll�¼��ķ���
			int fdNum = epoll_wait(epfd, events, 20, 500);
			// �����������������¼�
			for (int i = 0; i < fdNum; ++i)
			{
				if (events[i].data.fd == listenFd)
				{
					// ���ڲ����˱�Ե����ģʽ��������Ҫʹ��ѭ������֤�����µ����Ӷ���ע��
					for (; ; )
					{
						sockaddr_in clientAddr = { 0 };
						socklen_t addrSize = sizeof(clientAddr);
						int connFd = accept(listenFd, (sockaddr*)&clientAddr, &addrSize);
						if (-1 == connFd)
						{
							//perror("Accept");
							ERROR_LOG("Accept error");
							break;
						}
						int ret = getnameinfo((sockaddr*)&clientAddr, sizeof(clientAddr),
							hostBuf, sizeof(hostBuf) / sizeof(hostBuf[0]),
							portBuf, sizeof(portBuf) / sizeof(portBuf[0]),
							NI_NUMERICHOST | NI_NUMERICSERV);
						if (!ret)
						{
							DEBUG_LOG("New connection: host = %s, port = %s\n", hostBuf, portBuf);
						}
						DEBUG_LOG("Receive new connection[%d]\n", connFd);

						setNonblocking(connFd);
						epoll_event registEvent;
						registEvent.data.fd = connFd; // �������ڶ��������ļ�������
						registEvent.events = EPOLLIN | EPOLLET; // ��������ע��Ķ������¼�
						// Ϊ��accept�� file describe ����epoll�¼�
						if (-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, connFd, &registEvent))
						{
							ERROR_LOG("epoll_ctl");
							return 0;
						}
					}
				}
				else if (events[i].events & EPOLLIN)
				{
					char line[MAXLINE] = { 0 };
					int sockFd;
					//������Ѿ����ӵ��û��������յ����ݣ���ô���ж��롣
					DEBUG_LOG("EPOLLIN");
					if ((sockFd = events[i].data.fd) < 0)
						continue;
					ssize_t socketLen;
					if ((socketLen = readAll(sockFd, line, MAXLINE)) < 0)
					{
						if (errno == ECONNRESET)
						{
							close(sockFd);
							events[i].data.fd = -1;
							//epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
							DEBUG_LOG("Close socket[%d]\n", sockFd);
						}
						else
						{
							DEBUG_LOG("read out");
						}
					}
					else if (socketLen == 0)
					{
						close(sockFd);
						events[i].data.fd = -1;
						//epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
						DEBUG_LOG("Close socket[%d]\n", sockFd);
					}
					DEBUG_LOG("Read line[%d]\n", line);
					epoll_event registEvent;
					registEvent.data.fd = sockFd; // ��������д�������ļ�������
					registEvent.events = EPOLLOUT | EPOLLET; // ��������ע���д�����¼�
					epoll_ctl(epfd, EPOLL_CTL_MOD, sockFd, &registEvent); // �޸�sockFd��Ҫ������¼�ΪEPOLLOUT
					cbFunc(line);
				}
				else if (events[i].events & EPOLLOUT) // ��������ݷ���
				{
					char line[MAXLINE];
					int sockFd = events[i].data.fd;
					writeAll(sockFd, line, strnlen(line, sizeof(line)));
					epoll_event registEvent;
					registEvent.data.fd = sockFd; // �������ڶ��������ļ�������
					registEvent.events = EPOLLIN | EPOLLET; // ��������ע��Ķ������¼�
					epoll_ctl(epfd, EPOLL_CTL_MOD, sockFd, &registEvent); // �޸�sockFd��Ҫ������¼�ΪEPOLIN
				}
			}
		}
		return 0;
	}

	void EventDispatcher::setNonblocking(int32_t sock)
	{
		int opts;
		opts = fcntl(sock, F_GETFL);
		if (opts < 0)
		{
			ERROR_LOG("fcntl(sock,GETFL)");
			exit(1);
		}
		opts = opts | O_NONBLOCK;
		if (fcntl(sock, F_SETFL, opts) < 0)
		{
			ERROR_LOG("fcntl(sock,SETFL,opts)");
			exit(1);
		}
	}

	int32_t EventDispatcher::readAll(int32_t sock, char* buf, size_t size)
	{
		int32_t recvSum = 0;
		int32_t recvCnt = recv(sock, buf, size, MSG_DONTWAIT | MSG_PEEK);
		while (true)
		{
			int32_t readLen = read(sock, buf + recvSum, size - recvSum);
			if (readLen < 0)
			{
				return -1;
			}
			else if (readLen == 0)
			{
				break;
			}
			DEBUG_LOG("recv:[%s]\n", buf + recvSum);
			recvSum += readLen;
			//write(sock, buf, str_len);
			if (recvSum >= recvCnt)
				break;
		}
		return recvSum;
	}

	int32_t EventDispatcher::writeAll(int32_t sock, char* buf, size_t size)
	{
		int32_t sendCnt = 0;
		while (true)
		{
			int32_t senSize = write(sock, buf + sendCnt, size - sendCnt);
			if (senSize < 0)
			{
				return -1;
			}
			else if (senSize == 0)
			{
				break;
			}
			sendCnt += senSize;
		}
		//write(sock, buf, size);
		return sendCnt;
	}
}

