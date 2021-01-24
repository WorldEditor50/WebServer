

#ifndef __MACRO_ASSEMBLE_H__
#define __MACRO_ASSEMBLE_H__

#include "commlib/logger/Logger.h"
#include "commlib/basic/Time.h"
#include "commlib/basic/StringUtils.h"

#define DEBUG_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::DEBUG, "[DEBUG] [%s] [%s : %d] " fmt,\
	CWSLib::Time::now().tostr().c_str(), CWSLib::String::baseName(__FILE__).c_str(), __LINE__, ##__VA_ARGS__)
#define NORMAL_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::NORMAL, "[NORMAL] [%s] [%s : %d] " fmt,\
	CWSLib::Time::now().tostr().c_str(), CWSLib::String::baseName(__FILE__).c_str(), __LINE__, ##__VA_ARGS__)
#define WARN_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::WARNING, "[WARNING] [%s] [%s : %d] " fmt,\
	CWSLib::Time::now().tostr().c_str(), CWSLib::String::baseName(__FILE__).c_str(), __LINE__, ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::ERROR, "[ERROR] [%s] [%s : %d] " fmt,\
	CWSLib::Time::now().tostr().c_str(), CWSLib::String::baseName(__FILE__).c_str(), __LINE__, ##__VA_ARGS__)

#define PURE_LOG(fmt, ...) BASIC_LOG(CWSLib::LogLevel::ERROR, fmt, ##__VA_ARGS__)


#endif // !__MACRO_ASSEMBLE_H__

