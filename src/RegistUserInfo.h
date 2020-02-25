

#ifndef __REGIST_USER_INFO_H__
#define __REGIST_USER_INFO_H__

#include "frame/BaseTask.h"

class RegistUserInfo : public BaseTask
{
public:

	virtual void parseRequest();
	virtual void process();
	virtual void packResponse();

private:

private:

};

#endif // !__REGIST_USER_INFO_H__

