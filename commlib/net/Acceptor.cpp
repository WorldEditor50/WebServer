

#include "Acceptor.h"

#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>

namespace CWSLib
{
	const static int LISTENQ = 20;

	int32_t Acceptor::Init(const std::string& ip, int32_t port)
	{
		Socket sock;
		sock.Init();
		sock.SetNonblocking();
		m_listenFd = sock.GetFd();

		sockaddr_in serverAddr;
		bzero(&serverAddr, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		inet_aton(ip.c_str(), &(serverAddr.sin_addr));

		serverAddr.sin_port = htons(port);
		bind(m_listenFd, (sockaddr*)&serverAddr, sizeof(serverAddr));
		listen(m_listenFd, LISTENQ);
		return m_listenFd;
	}

	std::shared_ptr<Socket> Acceptor::Accept()
	{
		sockaddr_in clientAddr = { 0 };
		socklen_t addrSize = sizeof(clientAddr);
		int connFd = accept(m_listenFd, (sockaddr*)&clientAddr, &addrSize);
		if (-1 == connFd)
		{
			return std::shared_ptr<Socket>();
		}
		std::shared_ptr<Socket> newConn = std::shared_ptr<Socket>(new Socket(connFd));
		newConn->SetNonblocking();
		return newConn;
	}

}
