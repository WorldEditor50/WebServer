

#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__

#include <string>

typedef void __svcCallback(const char*);

namespace CWSLib
{
	class EventDispatcher
	{
	public:
		EventDispatcher();

		~EventDispatcher();

		int32_t init();

		int32_t wait(__svcCallback cbFunc);

	private:

		void setNonblocking(int32_t sock);

		int32_t readAll(int32_t sock, char* buf, size_t size);

		int32_t writeAll(int32_t sock, char* buf, size_t size);

	private:
		int epfd;
		int listenFd;
	};
}

#endif // !__EVENT_DISPATCHER_H__
