

#include "CwsService.h"

void CwsFrame::Service::AddMethod(const std::string& methodName, std::function<std::shared_ptr<Method>()> func)
{
	auto itor = m_methods.find(methodName);
	if (itor == m_methods.end())
	{
		m_methods.insert(std::make_pair(methodName, func));
	}
}

std::shared_ptr<CwsFrame::Method> CwsFrame::Service::GetMethod(const std::string& name)
{
	auto itor = m_methods.find(name);
	if (itor == m_methods.end())
	{
		return nullptr;
	}
	else
	{
		return itor->second();
	}
}

std::string CwsFrame::Service::Name()
{
	return m_name;
}
