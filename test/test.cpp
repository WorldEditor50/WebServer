

#include "test.h"
#include "TestTask.h"
#include "commlib/thread_pool/ThreadPool.h"
#include "commlib/logger/Logger.h"

#include <iostream>

void CWSTest::helloWorld()
{
    std::cout << "Hello world!\n";
}

void CWSTest::threadPool()
{
    srand((unsigned int)time(NULL));

    CWSLib::ThreadPool pool;
    pool.init(5, 20, 20);

    for (int i = 0; i < 100000; ++i)
    {
        std::cout << i << "\n";
        pool.addTask(new TestTask);
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10));
    }

    std::this_thread::sleep_for(std::chrono::seconds(200000));
}

void CWSTest::log()
{
    int num = 10086;
    const char* lala = "aa10086";
    char c = 'c';
    BASIC_LOG(CWSLib::LogLevel::DEBUG, "hahaha[][][]");
    BASIC_LOG(CWSLib::LogLevel::DEBUG, "hahaha[%d][%s][%c]", num, lala, c);
    DEBUG_LOG("hahaha[%d][%s][%c]", num, lala, c);
    NORMAL_LOG("hahaha[%d][%s][%c]", num, lala, c);
    WARN_LOG("hahaha[%d][%s][%c]", num, lala, c);
    ERROR_LOG("hahaha[%d][%s][%c]", num, lala, c);
}


