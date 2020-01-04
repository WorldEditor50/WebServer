

#include <iostream>

#include "Logger.h"

void CWSLib::Logger::setLevel(LogLevel level)
{
    mLevel = level;
}

void CWSLib::Logger::log(LogLevel logLevel, const std::string& logInfo)
{
    if (logLevel >= mLevel)
    {
        std::cout << logInfo << "\n";
    }
}
