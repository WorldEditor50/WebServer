

#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <string>
#include <stdarg.h>
#include <vector>

namespace CWSLib
{
namespace String
{
	// no more than 4096 bytes
	std::string format(const char* fmt, ...);

	void split(std::vector<std::string>& strList, const std::string& src, char spliter);
}
}

#endif // !__STRING_UTILS_H__

