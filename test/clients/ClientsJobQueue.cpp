

#include "ClientsJobQueue.h"

void CWSTest::ClientQueue::Add(std::shared_ptr<ClientJobBase> job)
{
	m_queue.push_back(job);
}

void CWSTest::ClientQueue::Exec()
{
	for (auto job : m_queue)
	{
		int ret = job->Execute();
		if (ret)
		{
			m_result.fail_num++;
		}
		else
		{
			m_result.succ_num++;
		}
	}
}

CWSTest::ExecResult& CWSTest::ClientQueue::Result()
{
	return m_result;
}
