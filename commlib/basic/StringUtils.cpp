

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

void CWSLib::String::split(std::vector<std::string>& strList, const std::string& src, char spliter)
{
	size_t pos = 0, endpos = 0;
	size_t len = src.length();

	while (pos < len && (endpos = src.find(spliter, endpos + 1)) != std::string::npos)
	{
		strList.push_back(src.substr(pos, endpos - pos));
		pos = endpos + 1;
	}
	strList.push_back(src.substr(pos));
}
