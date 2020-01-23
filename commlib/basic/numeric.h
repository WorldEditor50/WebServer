

#ifndef __NUMERIC_H__
#define __NUMERIC_H__

#include <string>

namespace CWSLib
{
	bool isDouble(const std::string& input);
	bool isLonglong(const std::string& input);
	bool isInt(const std::string& input);
	bool isBool(const std::string& input);
}

#endif // !__NUMERIC_H__


