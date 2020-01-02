

#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <thread>
#include <list>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "BaseTask.h"

namespace CWSLib
{
    class TaskList
    {
    public:
        ~TaskList();

        void addTask(BaseTask* task);
        BaseTask* getTask();
        void popTask();
        bool empty();
        int size();

    private:
        //std::list<BaseTask*> mTaskList;
        std::list<std::unique_ptr<BaseTask>> mTaskList;
    };

    class ThreadPool
    {
    public:
        ThreadPool() : mTaskLimit(0), mThreadLimit(0), mInitThdnum(0), mContinue(true),
            mCurrentThdSize(0) {}

        // initThreadNum -- number of working threads
        // maxTaskNum -- max task number in task list
        // maxThreadNum -- max working thread number
        void init(int initThreadNum, int maxTaskNum, int maxThreadNum);
        void addTask(BaseTask* task);
        void shutdown();

    private:
        void work();

    private:
        int mTaskLimit;
        int mThreadLimit;
        int mInitThdnum;
        int mCurrentThdSize;
        bool mContinue;

        TaskList mTasks;
        std::mutex mMutex;
        std::condition_variable mWakeCond;
        std::condition_variable mPushCond;

    };
}

#endif // !__THREAD_POOL_H__

