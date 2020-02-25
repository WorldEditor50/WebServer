

#ifndef __BASE_TASK_H__
#define __BASE_TASK_H__

#include "commlib/app/TypeRegSingleton.h"

class BaseTask
{
public:
	virtual ~BaseTask() {}
	virtual void parseRequest() = 0;
	virtual void process() = 0;
	virtual void packResponse() = 0;
};

typedef CWSLib::CommSingleton<TypeFactory<BaseTask>> CTaskFactory;

#endif // !__BASE_TASK_H__

