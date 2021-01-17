

#ifndef __TIME_H__
#define __TIME_H__

#include <time.h>
#include <string>
#include <chrono>

namespace CWSLib
{
	class TimeSpan
	{
	public:
		TimeSpan() : mTimeSpan(0)
		{}

		TimeSpan(time_t time) : mTimeSpan(time)
		{}

		TimeSpan(int lDays, int nHours, int nMins, int nSecs)
		{
			mTimeSpan = (int64_t)nSecs + 60 * ((int64_t)nMins + 60 * ((int64_t)nHours + 24 * (int64_t)lDays));
		}

		int getDays() const {
			return((int)mTimeSpan / (24 * 3600));
		}

		int getTotalHours() const {
			return ((int)mTimeSpan / 3600);
		}

		int getHours() const {
			return (getTotalHours() % 24);
		}

		int getTotalMinutes() const {
			return ((int)mTimeSpan / 60);
		}

		int getMinutes() const {
			return (getTotalMinutes() % 60);
		}

		int getTotalSeconds() const {
			return (int)mTimeSpan;
		}

		int getSeconds() const {
			return ((int)mTimeSpan % 60);
		}

		time_t getTimeSpan() const {
			return mTimeSpan;
		}

		TimeSpan operator+(TimeSpan span) const {
			return(TimeSpan(mTimeSpan + span.mTimeSpan));
		}

		TimeSpan operator-(TimeSpan span) const {
			return(TimeSpan(mTimeSpan - span.mTimeSpan));
		}

		TimeSpan& operator+=(TimeSpan span) {
			mTimeSpan += span.mTimeSpan;
			return(*this);
		}

		TimeSpan& operator-=(TimeSpan span) {
			mTimeSpan -= span.mTimeSpan;
			return(*this);
		}

		bool operator==(TimeSpan span) const {
			return(mTimeSpan == span.mTimeSpan);
		}

		bool operator!=(TimeSpan span) const {
			return(mTimeSpan != span.mTimeSpan);
		}

		bool operator<(TimeSpan span) const {
			return(mTimeSpan < span.mTimeSpan);
		}

		bool operator>(TimeSpan span) const {
			return(mTimeSpan > span.mTimeSpan);
		}

		bool operator<=(TimeSpan span) const {
			return(mTimeSpan <= span.mTimeSpan);
		}

		bool operator>=(TimeSpan span) const {
			return(mTimeSpan >= span.mTimeSpan);
		}

	public:
		//  * the only valid formats:
		//  %D - # of days
		//  %H - hour in 24 hour format
		//  %M - minute (0-59)
		//  %S - seconds (0-59)
		//  %% - percent sign
		std::string tostr(const char* pszFormat = "%D/%H/%M/%S") const;

	private:
		time_t mTimeSpan;
	};


	class Time
	{
	public:
		static Time now() {
			return Time(time(NULL));
		}

		// parse time from string, avaliable format: %[w](y|Y|m|d|H|M|S)
		// %Y: long year field (1970-2026), %y: short year field(00-26)
		// %m: month field (1-12), %d: day field (1-31), 
		// %H: hour filed (0-23),  %M: minute filed (0-59), 
		// %S: second filed (0-59), %%: percent sign
		// w: field width
		// perfect programming art (:
		static Time parse(const char* szTimeStr, const char* szFormat = "%Y-%m-%d %H:%M:%S");

		Time() : mTime(0) {}
		Time(time_t time) : mTime(time) {}

		Time(struct tm time) {
			mTime = mktime(&time);
		}

		Time(int nYear, int nMonth, int nDay, int nHour = 0, int nMin = 0, int nSec = 0,
			int nDST = -1)
		{
			struct tm atm;
			atm.tm_sec = nSec;
			atm.tm_min = nMin;
			atm.tm_hour = nHour;
			atm.tm_mday = nDay;
			atm.tm_mon = nMonth - 1;        // tm_mon is 0 based        
			atm.tm_year = nYear - 1900;     // tm_year is 1900 based
			atm.tm_isdst = nDST;
			mTime = mktime(&atm);
		}

		Time(const Time& time) {
			mTime = time.mTime;
		}

		Time& operator=(time_t time) {
			mTime = time;
			return(*this);
		}

		Time& operator+=(TimeSpan span) {
			mTime += span.getTimeSpan();
			return(*this);
		}

		Time& operator-=(TimeSpan span) {
			mTime -= span.getTimeSpan();
			return (*this);
		}

		TimeSpan operator-(Time time) const {
			return(TimeSpan(mTime - time.mTime));
		}

		Time operator-(TimeSpan span) const {
			return(Time(mTime - span.getTimeSpan()));
		}

		Time operator+(TimeSpan span) const {
			return(Time(mTime + span.getTimeSpan()));
		}

		bool operator==(Time time) const {
			return(mTime == time.mTime);
		}

		bool operator!=(Time time) const {
			return(mTime != time.mTime);
		}

		bool operator<(Time time) const {
			return(mTime < time.mTime);
		}

		bool operator>(Time time) const {
			return(mTime > time.mTime);
		}

		bool operator<=(Time time) const {
			return(mTime <= time.mTime);
		}

		bool operator>=(Time time) const {
			return(mTime >= time.mTime);
		}

		struct tm* getGmtTm(struct tm* ptm = NULL) const {
			struct tm* ptmTemp;
			ptmTemp = gmtime(&mTime);

			if (ptmTemp == NULL)
				return NULL;

			if (ptm != NULL)
			{
				*ptm = *ptmTemp;
				return ptm;
			}
			else
			{
				return ptmTemp;
			}
		}

		struct tm* getLocalTm(struct tm* ptm = NULL) const {
			struct tm* ptmTemp;
			ptmTemp = localtime(&mTime);

			if (ptmTemp == NULL)
				return NULL;

			if (ptm != NULL) {
				*ptm = *ptmTemp;
				return ptm;
			}
			else
			{
				return ptmTemp;
			}
		}

		time_t getTime() const {
			return mTime;
		}

		int getYear() const {
			struct tm* ptm;
			ptm = getLocalTm();
			return ptm ? (ptm->tm_year + 1900) : 0;
		}

		int getMonth() const {
			struct tm* ptm;
			ptm = getLocalTm();
			return ptm ? (ptm->tm_mon + 1) : 0;
		}

		int getDay() const {
			struct tm* ptm;
			ptm = getLocalTm();
			return ptm ? ptm->tm_mday : 0;
		}

		int getHour() const {
			struct tm* ptm;
			ptm = getLocalTm();
			return ptm ? ptm->tm_hour : -1;
		}

		int getMinute() const {
			struct tm* ptm;
			ptm = getLocalTm();
			return ptm ? ptm->tm_min : -1;
		}

		int getSecond() const {
			struct tm* ptm;
			ptm = getLocalTm();
			return ptm ? ptm->tm_sec : -1;
		}

		int getDayOfWeek() const {
			struct tm* ptm;
			ptm = getLocalTm();
			return ptm ? ptm->tm_wday + 1 : 0;
		}

		// formatting using "C" strftime
		std::string tostr(const char* pszFormat = "%Y-%m-%d %H:%M:%S") const;
		std::string togmtstr(const char* pszFormat = "%Y-%m-%d %H:%M:%S") const;

	private:
		time_t mTime;
	};

	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// PreciseTime ///////////////////////////////

	typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> MicroClock;

	class PreciseTime
	{
	public:
		static int64_t getNowMiniSec();
	};
}

#endif // !__TIME_H__

