

#ifndef __CWS_TASK_IMPL_H__
#define __CWS_TASK_IMPL_H__

#include <map>
#include <functional>
#include <memory>

#include "commlib/thread_pool/BaseJob.h"
#include "commlib/net/Socket.h"
#include "commlib/app/TypeRegSingleton.h"

namespace CwsFrame
{
	class JobImpl : public CWSLib::BaseJob
	{
	public:
		JobImpl() {}
		virtual ~JobImpl() {}
		//void excute() override;
		void Init(std::shared_ptr<CWSLib::Socket> sock)
		{
			m_sock = sock;
		}

	private:

	protected:
		std::shared_ptr<CWSLib::Socket> m_sock;
	};

	class JobFactory
	{
	public:
		void AddType(const std::string& name, std::function<JobImpl*()> func, bool isDefault)
		{
			if (m_funcMap.find(name) == m_funcMap.end())
			{
				m_funcMap.insert(std::make_pair(name, func));
			}
			if (isDefault)
			{
				m_defaultName = isDefault;
			}
		}

		JobImpl* Create(const std::string& name)
		{
			auto itor = m_funcMap.find(name);
			if (itor == m_funcMap.end())
			{
				return nullptr;
			}
			else
			{
				return itor->second();
			}
		}

		JobImpl* Create()
		{
			return Create(m_defaultName);
		}

	private:
		std::map<std::string, std::function<JobImpl*()>> m_funcMap;
		std::string m_defaultName;
	};

	//using CJobFactory = CWSLib::CommSingleton<JobFactory>;
	using CJobFactory = CWSLib::CommSingleton<TypeFactory<JobImpl>>;

	/*class JobMeta
	{
	public:
		JobMeta(const std::string& name, std::function<JobImpl*()> func, bool isDefault)
		{
			CJobFactory::instance().AddType(name, func, isDefault);
		}
	};*/
}

#endif // !__CWS_TASK_IMPL_H__

