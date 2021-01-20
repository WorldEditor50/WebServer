

#ifndef __CWS_SERVER_H__
#define __CWS_SERVER_H__

#include <string>
#include <map>
#include <memory>
#include <google/protobuf/service.h>

#include "commlib/net/EventDispatcher.h"

namespace CwsFrame
{
	class Server
	{
	public:
		int32_t AddService(const std::string& serviceName, google::protobuf::Service* servicePtr);
		void Run();

	private:
		std::map<std::string, std::shared_ptr<google::protobuf::Service>> m_serviceMap;
		CWSLib::EventDispatcher dispatcher;
	};
}

#endif // !__CWS_SERVER_H__

