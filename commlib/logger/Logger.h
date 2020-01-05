

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "commlib/basic/CommonSingleton.h"
#include "commlib/string/StringUtils.h"

#include <string>

namespace CWSLib
{
    enum class LogLevel
    {
        DEBUG = 0,
        NORMAL = 1,
        WARNING = 2,
        ERROR = 3,
    };

    enum class LogDirection
    {
        TO_FILE = 0,    // 日志写入文件
        TO_TERM = 1,    // 日志打印到终端
    };

    class Logger
    {
    public:
        Logger() :mLevel(LogLevel::DEBUG), mDir(LogDirection::TO_TERM) {}

        void setLevel(LogLevel level);
        void setFile(const std::string& fileName);
        void log(LogLevel logLevel, const std::string& logInfo);

    private:

    private:
        LogLevel mLevel;
        LogDirection mDir;
        std::string mLogFile;
    };
}

typedef CWSLib::CommSingleton<CWSLib::Logger> CWSLogger;

#define BASIC_LOG(level, fmt, ...) do{ CWSLogger::instance().log(\
    level, std::move(CWSLib::String::format(fmt, ##__VA_ARGS__))); }while(0)
#define SET_LOG_FILE(fileName) do { CWSLogger::instance().setFile(fileName); } while(0)


#define DEBUG_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::DEBUG, "[DEBUG] " fmt, ##__VA_ARGS__)
#define NORMAL_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::NORMAL, "[NORMAL] " fmt, ##__VA_ARGS__)
#define WARN_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::WARNING, "[WARNING] " fmt, ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::ERROR, "[ERROR] " fmt, ##__VA_ARGS__)

#endif // !__LOGGER_H__

