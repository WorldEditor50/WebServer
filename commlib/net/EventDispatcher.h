

#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__

#include <string>

namespace CWSLib
{
	int32_t readAll(int32_t sock, char* buf, size_t size);

	int32_t writeAll(int32_t sock, char* buf, size_t size);

	struct CbContext
	{
		std::string data;
		int socketfd;

		static void close(int socketfd);
	};

	typedef void (*__svcCallback)(CWSLib::CbContext&);

	class EventDispatcher
	{
	public:
		EventDispatcher();

		~EventDispatcher();

		int32_t init(__svcCallback cbFunc);

		int32_t wait();

	private:
		void setNonblocking(int32_t sock);

	private:
		int epfd;
		int listenFd;
		int timeout;
		int maxEvent;
		__svcCallback mCb;
	};
}

#endif // !__EVENT_DISPATCHER_H__
