

#ifndef __CWS_SERVICE_H__
#define __CWS_SERVICE_H__

#include <map>
#include <string>
#include <memory>
#include <functional>

#include <google/protobuf/service.h>

namespace CwsFrame
{
	class Method
	{
	public:
		virtual ~Method() {}
		virtual int32_t Execute() = 0;
		virtual std::shared_ptr<google::protobuf::Message> GetRequest() = 0;
		virtual std::shared_ptr<google::protobuf::Message> GetResponse() = 0;
	};

	class Service
	{
	public:
		Service(const std::string& name) : m_name(name) {}
		void AddMethod(const std::string& methodName, std::function<std::shared_ptr<Method>()> func);
		std::shared_ptr<Method> GetMethod(const std::string& name);
		std::string Name();
		
	private:
		std::map<std::string, std::function<std::shared_ptr<Method>()>> m_methods;
		std::string m_name;
	};
}

#endif // !__CWS_SERVICE_H__

