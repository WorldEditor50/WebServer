

#include "StringUtils.h"

std::string CWSLib::String::format(const char* fmt, ...)
{
	char buf[4096] = { 0 };
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	return buf;
}
