

#include "commlib/app/TypeRegSingleton.h"
#include "commlib/app/MacroAssemble.h"
#include "commlib/basic/Exception.h"

#include "frame/CwsServer.h"

#include "ProJobImpl.h"
#include "ProError.h"

int32_t ProJobImpl::execute()
{
	try
	{
		if (ParseInputContent() < 0)
		{
			return -1;
		}
		CallMethod();
		Response();
	}
	catch (CWSLib::Exception& exp)
	{
		ERROR_LOG("Caught exception[%d][%s].", exp.error(), exp.what());
		return -1;
	}
	catch (...)
	{
		ERROR_LOG("Caught unknown error.");
		return -1;
	}
	return 0;
}

int32_t ProJobImpl::ParseInputContent()
{
	char buf[sizeof(size_t)];
	m_sock->Read(buf, sizeof(size_t));
	size_t contextLen = *(size_t*)buf;
	std::string contextBuf;
	int readLen = m_sock->Read(contextBuf, contextLen);
	if (readLen < 0)
	{
		return -1;
	}
	if (!m_context.ParseFromString(contextBuf))
	{
		throw CWSLib::Exception(Error::PROTO_PARSE, "Context parse from string failed.");
	}
	NORMAL_LOG("Service[%s], Method[%s].", m_context.service_name().c_str(), m_context.method_name().c_str());
	return 0;
}

void ProJobImpl::CallMethod()
{
	CwsFrame::Server& server = *CServerSingleton::instance();
	std::shared_ptr<CwsFrame::Service> service = server.GetService(m_context.service_name());
	if (!service.get())
	{
		throw CWSLib::Exception(Error::UNKNOWN_SERVICE, "Unknown service.");
	}
	m_method = service->GetMethod(m_context.method_name());
	if (!m_method.get())
	{
		throw CWSLib::Exception(Error::UNKNOWN_METHOD, "Unknown method.");
	}
	std::string messageBuf;
	m_sock->Read(messageBuf, m_context.content_length());
	auto request = m_method->GetRequest();
	request->ParseFromString(messageBuf);
	m_method->Execute();
}

void ProJobImpl::Response()
{
	auto response = m_method->GetResponse();
	std::string output;
	std::string respStr = response->SerializeAsString();
	size_t length = respStr.length();
	output += (char*)&length;
	output += respStr;
	m_sock->WriteAll(output);
}

REG_TYPE(CwsFrame::JobImpl, ProJobImpl, pro_job_impl, true);
