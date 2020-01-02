

#ifndef __TEST_TASK_H__
#define __TEST_TASK_H__

#include <chrono>
#include <iostream>
#include <thread>
#include <ctime>
#include <cstdlib>

#include "commlib/thread_pool/BaseTask.h"

class TestTask : public CWSLib::BaseTask
{
public:
    virtual void excute()
    {
        srand((unsigned int)time(NULL));
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
        std::cout << std::this_thread::get_id() << ": Haha\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

private:

};

#endif // !__TEST_TASK_H__

