

#ifndef __CWS_TASK_IMPL_H__
#define __CWS_TASK_IMPL_H__

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

	using CJobFactory = CWSLib::CommSingleton<TypeFactory<JobImpl>>;
}

#endif // !__CWS_TASK_IMPL_H__

