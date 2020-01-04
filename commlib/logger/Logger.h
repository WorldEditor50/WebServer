

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

    class Logger
    {
    public:
        Logger() :mLevel(LogLevel::DEBUG) {}

        void setLevel(LogLevel level);
        void log(LogLevel logLevel, const std::string& logInfo);

    private:

    private:
        LogLevel mLevel;
    };
}

typedef CWSLib::CommSingleton<CWSLib::Logger> CWSLogger;

#define BASIC_LOG(level, fmt, ...) do{ CWSLogger::instance().log(\
    level, CWSLib::String::format(fmt, ##__VA_ARGS__)); }while(0)

#define DEBUG_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::DEBUG, "[DEBUG] " fmt, ##__VA_ARGS__)
#define NORMAL_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::NORMAL, "[NORMAL] " fmt, ##__VA_ARGS__)
#define WARN_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::WARNING, "[WARNING] " fmt, ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::ERROR, "[ERROR] " fmt, ##__VA_ARGS__)

#endif // !__LOGGER_H__

