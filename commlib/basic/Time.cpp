

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "Time.h"

using namespace std;

string CWSLib::TimeSpan::tostr(const char* pFormat /*= "%D/%H/%M/%S"*/) const
{
	if (!pFormat)
		return "";

	string str;
	char ch;
	while ((ch = *pFormat++) != '\0')
	{
		if (ch == '%')
		{
			char tempBuff[8] = { 0 };
			switch (ch = *pFormat++)
			{
			case '%':
				str += ch;
				break;
			case 'D':
				snprintf(tempBuff, sizeof(tempBuff), "%d", getDays());
				str += tempBuff;
				break;
			case 'H':
				snprintf(tempBuff, sizeof(tempBuff), "%02d", getHours());
				str += tempBuff;
				break;
			case 'M':
				snprintf(tempBuff, sizeof(tempBuff), "%02d", getMinutes());
				str += tempBuff;
				break;
			case 'S':
				snprintf(tempBuff, sizeof(tempBuff), "%02d", getSeconds());
				str += tempBuff;
				break;
			}
		}
		else
		{
			str += ch;
			if ((unsigned char)ch > 0x80)
			{
				// 多字节字符串的简单处理
				str += *pFormat++;
			}
		}
	}

	return str;
}

string CWSLib::Time::tostr(const char* pszFormat)const
{
	if (mTime == -1)
		return "invalid time";

	char szBuffer[128] = { 0 };
	if (pszFormat == NULL)
		return szBuffer;

	struct tm* ptm = getLocalTm();
	if (ptm != NULL)
	{
		if (!strftime(szBuffer, sizeof(szBuffer), pszFormat, ptm))
			szBuffer[0] = '\0';
	}

	return szBuffer;
}

string CWSLib::Time::togmtstr(const char* pszFormat) const
{
	if (mTime == -1)
		return "invalid time";

	char szBuffer[128] = { 0 };
	if (pszFormat == NULL)
		return szBuffer;

	struct tm* ptm = getGmtTm();
	if (ptm != NULL)
	{
		if (!strftime(szBuffer, sizeof(szBuffer), pszFormat, ptm))
			szBuffer[0] = '\0';
	}

	return szBuffer;
}

CWSLib::Time CWSLib::Time::parse(const char* szTimeStr, const char* szFormat)
{
	struct tm atm;
	memset(&atm, 0, sizeof(atm));

	const char* pFormatTag = szFormat;
	const char* pDataPart = szTimeStr;
	bool bValidData = true;

	if (!szTimeStr || !szFormat)
		return Time(-1);

	for (; *pFormatTag != '\0' && bValidData;)
	{
		for (; *pFormatTag != '%' && *pFormatTag != '\0';)
		{
			char ch = *pFormatTag;
			char ch2 = *(pFormatTag + 1);

			++pFormatTag;
			++pDataPart;
			if ((ch == '\t' || ch == '\x20') && (ch2 != '\t' && ch2 != '\x20'))
			{
				// 跳过多余的空格
				for (; (*pDataPart == '\x20' || *pDataPart == '\t') && (*pDataPart != '\0'); ++pDataPart);
			}
			else if ((unsigned char)ch > 0x80)
			{
				// 跳过多字节字符
				++pFormatTag;
				++pDataPart;
			}
		}

		if (*pFormatTag == '\0')
			break;

		// get a tag, format: %[width](y|m|d|H|M|S|%)
		++pFormatTag;
		int width = -1;
		string strTemp;
		for (; *pFormatTag >= '0' && *pFormatTag <= '9';
			strTemp += *pFormatTag, ++pFormatTag);

		if (!strTemp.empty())
			width = atoi(strTemp.c_str());

		char flag = *pFormatTag++;
		if (flag == '%')
		{
			bValidData = (*pDataPart++ == '%');
			continue;
		}

		strTemp = "";
		for (; *pDataPart >= '0' && *pDataPart <= '9' && (width == -1 || (int)strTemp.length() < width) && *pDataPart != '\0';
			strTemp += *pDataPart, ++pDataPart);

		if (strTemp.empty())
		{
			bValidData = false;
			continue;
		}

		int val = atoi(strTemp.c_str());
		switch (flag)
		{
		case 'y':
			atm.tm_year = val + 100; // 2000 + val - 1900
			break;

		case 'Y':
			atm.tm_year = val - 1900;
			break;

		case 'm':
			atm.tm_mon = val - 1;
			break;

		case 'd':
			atm.tm_mday = val;
			break;

		case 'H':
			atm.tm_hour = val;
			break;

		case 'M':
			atm.tm_min = val;
			break;

		case 'S':
			atm.tm_sec = val;
			break;

		default:
			bValidData = false;
			break;
		}
	}

	time_t t = -1;
	if (bValidData)
	{
		t = mktime(&atm);
	}

	return Time(t);
}

int64_t CWSLib::PreciseTime::getNowMiniSec()
{
	MicroClock timePoint = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	return timePoint.time_since_epoch().count();
}
