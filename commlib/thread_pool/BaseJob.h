

#ifndef __BASE_JOB_H__
#define __BASE_JOB_H__

namespace CWSLib
{
	class BaseJob
	{
	public:
		virtual ~BaseJob() {};
		virtual void execute() = 0;
	};
}

#endif // !__BASE_JOB_H__

