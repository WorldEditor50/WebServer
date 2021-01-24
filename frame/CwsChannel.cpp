

#include <google/protobuf/message.h>

#include "commlib/app/MacroAssemble.h"

#include "build/stubs/user_service.pb.h"
#include "CwsChannel.h"

namespace CwsFrame
{
	void Channel::CallMethod(const google::protobuf::MethodDescriptor* method,
		google::protobuf::RpcController* controller,
		const google::protobuf::Message* request,
		google::protobuf::Message* response,
		google::protobuf::Closure* done)
	{
		std::string requestData = request->SerializeAsString();
		cws::common::RpcContext context;
		context.set_service_name(method->service()->name());
		context.set_method_name(method->name());
		context.set_content_length(requestData.length());
		std::string ctxString = context.SerializeAsString();
		std::string content;
		size_t length = ctxString.length();
		content += (char*)&length;
		content += ctxString;
		content += requestData;
		int32_t sendCnt = m_sock.WriteAll(content);
		NORMAL_LOG("Sent: %d", sendCnt);

		char buf[sizeof(size_t)] = {0};
		m_sock.Read(buf, sizeof(size_t));
		size_t rLen = *(int*)buf;
		NORMAL_LOG("Size[%u]", rLen);
		std::string respStr;
		m_sock.Read(respStr, rLen);
		response->ParseFromString(respStr);
		NORMAL_LOG("==X==");
	}

	void Channel::Init(const std::string& host, int32_t port)
	{
		m_sock.Init();
		int32_t ret = m_sock.Connect(host, port);
		NORMAL_LOG("connect res[%d]", ret);
	}

}


