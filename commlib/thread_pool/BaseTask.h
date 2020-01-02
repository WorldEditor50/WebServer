

#ifndef __BASE_TASK_H__
#define __BASE_TASK_H__

namespace CWSLib
{
    class BaseTask
    {
    public:
        virtual ~BaseTask() {};
        virtual void excute() = 0;
    };
}

#endif // !__BASE_TASK_H__

