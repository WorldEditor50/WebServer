

#include "commlib/app/TypeRegSingleton.h"
#include "commlib/app/MacroAssemble.h"
#include "commlib/basic/Exception.h"

#include "frame/CwsServer.h"

#include "ProJobImpl.h"
#include "ProError.h"

void ProJobImpl::execute()
{
	try
	{
		ParseInputContent();
		CallMethod();
		Response();
	}
	catch (CWSLib::Exception& exp)
	{
		ERROR_LOG("Caught exception[%d][%s].", exp.error(), exp.what());
		return;
	}
	catch (...)
	{
		ERROR_LOG("Caught unknown error.");
		return;
	}
}

void ProJobImpl::ParseInputContent()
{
	char buf[sizeof(size_t)];
	m_sock->Read(buf, sizeof(size_t));
	size_t contextLen = *(size_t*)buf;
	std::string contextBuf;
	m_sock->Read(contextBuf, contextLen);
	if (!m_context.ParseFromString(contextBuf))
	{
		throw CWSLib::Exception(Error::PROTO_PARSE, "Context parse from string failed.");
	}
	NORMAL_LOG("Service[%s], Method[%s].", m_context.service_name().c_str(), m_context.method_name().c_str());
}

void ProJobImpl::CallMethod()
{
	CwsFrame::Server& server = CServerSingleton::instance();
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
	response->SerializePartialToString(&output);
	m_sock->WriteAll(output);
}

REG_TYPE_DEFAULT(CwsFrame::JobImpl, ProJobImpl, pro_job_impl);
