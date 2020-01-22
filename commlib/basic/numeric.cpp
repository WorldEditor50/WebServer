

#include "numeric.h"

bool CWSLib::isDouble(const std::string& input)
{
	bool hasPoint = false;
	for (auto item : input)
	{
		if (item == '.')
		{
			hasPoint = true;
		}
		else if (item > '9' || item < '0')
		{
			return false;
		}
	}
	return hasPoint;
}

bool CWSLib::isLonglong(const std::string& input)
{
	if (input.size() < 10)
	{
		return false;
	}
	for (auto item : input)
	{
		if (item > '9' || item < '0')
		{
			return false;
		}
	}
	return true;
}

bool CWSLib::isInt(const std::string& input)
{
	if (input.size() >= 10)
	{
		return false;
	}
	for (auto item : input)
	{
		if (item > '9' || item < '0')
		{
			return false;
		}
	}
	return true;
}
