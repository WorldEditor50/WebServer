

#include <iostream>
#include <fstream>
#include <mutex>

#include "Logger.h"

static std::mutex gLogMutex;

void CWSLib::Logger::setLevel(LogLevel level)
{
    mLevel = level;
}

void CWSLib::Logger::setFile(const std::string& fileName)
{
    mLogFile = fileName;
    mDir = (mLogFile == "") ? LogDirection::TO_TERM : LogDirection::TO_FILE;
}

void CWSLib::Logger::log(LogLevel logLevel, const std::string& logInfo)
{
    if (logLevel < mLevel)
    {
        return;
    }

    std::unique_lock<std::mutex> lock(gLogMutex);
    if (mDir == LogDirection::TO_TERM)
    {
        std::cout << logInfo << "\n";
    }
    else
    {
        std::ofstream ofs;
        ofs.open(mLogFile, std::ios::app);
        if (!ofs)
        {
            std::cerr << "Open file failed [" << mLogFile << "]\n";
            return;
        }
        ofs << logInfo << "\n";
        ofs.close();
    }
}
