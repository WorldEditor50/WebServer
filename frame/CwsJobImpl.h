

#ifndef __CWS_TASK_IMPL_H__
#define __CWS_TASK_IMPL_H__

#include "commlib/thread_pool/BaseJob.h"
#include "commlib/net/Socket.h"

namespace CwsFrame
{
	class JobImpl : public CWSLib::BaseJob
	{
	public:
		JobImpl(CWSLib::Socket& sock, const std::string& input);
		void excute() override;

	private:

	};
}

#endif // !__CWS_TASK_IMPL_H__

