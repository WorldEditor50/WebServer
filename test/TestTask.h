

#ifndef __TEST_TASK_H__
#define __TEST_TASK_H__

#include <chrono>
#include <iostream>
#include <thread>
#include <ctime>
#include <cstdlib>

#include "commlib/thread_pool/BaseJob.h"

class TestTask : public CWSLib::BaseJob
{
public:
    virtual void execute() override
    {
        srand((unsigned int)time(NULL));
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 300));
        std::cout << std::this_thread::get_id() << ": Haha\n";
    }

private:

};

#endif // !__TEST_TASK_H__

