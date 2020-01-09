

#ifndef __MACRO_ASSEMBLE_H__
#define __MACRO_ASSEMBLE_H__

#include "commlib/logger/Logger.h"
#include "commlib/basic/Time.h"

#define DEBUG_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::DEBUG, "[DEBUG] " fmt " [%s]", ##__VA_ARGS__, CWSLib::Time::now().tostr().c_str())
#define NORMAL_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::NORMAL, "[NORMAL] " fmt " [%s]", ##__VA_ARGS__, CWSLib::Time::now().tostr().c_str())
#define WARN_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::WARNING, "[WARNING] " fmt " [%s]", ##__VA_ARGS__, CWSLib::Time::now().tostr().c_str())
#define ERROR_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::ERROR, "[ERROR] " fmt " [%s]", ##__VA_ARGS__, CWSLib::Time::now().tostr().c_str())

#define PURE_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::ERROR, fmt, ##__VA_ARGS__)


#endif // !__MACRO_ASSEMBLE_H__

