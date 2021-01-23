

#ifndef __USER_INFO_REGIST_CLIENT_H__
#define __USER_INFO_REGIST_CLIENT_H__

#include "ClientsJobQueue.h"

namespace CWSTest
{
	class UserInfoRegistClient : public ClientJobBase
	{
	public:
		int32_t Execute() override;
	};
}

#endif // !__USER_INFO_REGIST_CLIENT_H__

