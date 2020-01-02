

#include "test.h"
#include "TestTask.h"
#include "commlib/thread_pool/ThreadPool.h"

#include <iostream>

void CWSTest::helloWorld()
{
    std::cout << "Hello world!\n";
}

void CWSTest::threadPool()
{
    CWSLib::ThreadPool pool;
    pool.init(5, 20, 10);

    for (int i = 0; i < 100; ++i)
    {
        std::cout << i << "\n";
        pool.addTask(new TestTask);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::this_thread::sleep_for(std::chrono::seconds(200));
}
