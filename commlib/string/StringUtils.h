

#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <string>
#include <stdarg.h>

namespace CWSLib
{
namespace String
{
    // no more than 4096 bytes
    std::string format(const char* fmt, ...);
}
}

#endif // !__STRING_UTILS_H__

