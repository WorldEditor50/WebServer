

#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include <sys/types.h>
#include <memory>

#include "Socket.h"

namespace CWSLib
{
	class Acceptor
	{
	public:
		int32_t Init(const std::string& ip, int32_t port);
		std::shared_ptr<Socket> Accept();

	private:
		int32_t m_listenFd;
	};
}

#endif // !__ACCEPTOR_H__

