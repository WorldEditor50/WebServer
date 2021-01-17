

#ifndef __REGIST_USER_INFO_H__
#define __REGIST_USER_INFO_H__

#include "frame/BaseTask.h"

class RegistUserInfo : public BaseTask
{
public:

	virtual void parseRequest(CWSLib::Param& input) override;
	virtual void process() override;
	virtual void packResponse(CWSLib::Param& output) override;

private:

private:

};

#endif // !__REGIST_USER_INFO_H__

