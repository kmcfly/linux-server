#include "RTSP_Common.h"


_rtsp_date_time_::_rtsp_date_time_()
{
	seconds			= 0;
	microsecond		= 0;
}

_rtsp_date_time_ & _rtsp_date_time_::operator=(tm & time)
{
	{
		assert((100 < time.tm_year) && (time.tm_year < 137));
		assert((0 <= time.tm_mon) && (time.tm_mon < 12));
		assert((0 < time.tm_mday) && (time.tm_mday < 32));

		assert((0 <= time.tm_hour) && (time.tm_hour < 24));
		assert((0 <= time.tm_min) && (time.tm_min < 60));
		assert((0 <= time.tm_sec) && (time.tm_sec < 60));
	}

	seconds			= (int)mktime(&time);
	microsecond		= 0;

	return *this;
}

_rtsp_date_time_ & _rtsp_date_time_::operator=(const timeval & time)
{
	seconds			= time.tv_sec;
	microsecond		= time.tv_usec;

	return *this;
}

_rtsp_date_time_ & _rtsp_date_time_::operator=(const _rtsp_local_time_ & time)
{
	{
		tm tm_time;
		memset(&tm_time, 0, sizeof(tm));

		tm_time.tm_year		= time.year - 1900;
		tm_time.tm_mon		= time.month - 1;
		tm_time.tm_mday		= time.mday;

		tm_time.tm_hour		= time.hour;
		tm_time.tm_min		= time.minute;
		tm_time.tm_sec		= time.second;

		{
			assert((100 < tm_time.tm_year) && (tm_time.tm_year < 137));
			assert((0 <= tm_time.tm_mon) && (tm_time.tm_mon < 12));
			assert((0 < tm_time.tm_mday) && (tm_time.tm_mday < 32));

			assert((0 <= tm_time.tm_hour) && (tm_time.tm_hour < 24));
			assert((0 <= tm_time.tm_min) && (tm_time.tm_min < 60));
			assert((0 <= tm_time.tm_sec) && (tm_time.tm_sec < 60));
		}

		seconds			= (int)mktime(&tm_time);
	}

	microsecond		= time.microsecond;

	return *this;
}

tm _rtsp_date_time_::TM() const
{
	tm time;
	memset(&time, 0, sizeof(tm));

	time_t SecNum = seconds;

#ifdef WIN32
	if (0 != localtime_s(&time, &SecNum))
#else
	if (NULL == localtime_r(&SecNum, &time))
#endif
	{
		assert(false);
	}

	return time;
}

timeval _rtsp_date_time_::TimeVal() const
{
	timeval time;

	time.tv_sec		= seconds;
	time.tv_usec	= microsecond;

	return time;
}

_rtsp_local_time_ _rtsp_date_time_::LocalTime() const
{
	_rtsp_local_time_ time;

	tm tm_time;
	memset(&tm_time, 0, sizeof(tm));

	time_t SecNum = seconds;

#ifdef WIN32
	if (0 != localtime_s(&tm_time, &SecNum))
#else
	if (NULL == localtime_r(&SecNum, &tm_time))
#endif
	{
		assert(false);
		memset(&time, 0, sizeof(_rtsp_local_time_));
	}
	else
	{
		time.year	= tm_time.tm_year + 1900;
		time.month	= tm_time.tm_mon + 1;
		time.mday	= tm_time.tm_mday;

		time.hour	= tm_time.tm_hour;
		time.minute	= tm_time.tm_min;
		time.second	= tm_time.tm_sec;

		time.microsecond	= microsecond;
	}

	return time;
}

_rtsp_date_time_ & _rtsp_date_time_::operator+=(int usec)
{
	if (usec < 0)
	{
		usec = labs(usec);
		(*this) -= usec;
	}
	else//0 <= usec
	{
		int sum = microsecond + usec;

		//需要进位
		if (1000000 <= sum)
		{
			seconds += (sum / 1000000);
		}

		microsecond = (sum % 1000000);
	}

	return *this;
}

_rtsp_date_time_ & _rtsp_date_time_::operator-=(int usec)
{
	if (usec < 0)
	{
		usec = labs(usec);
		(*this) += usec;
	}
	else//0 <= usec
	{
		seconds -= (usec / 1000000);

		int u = (usec % 1000000);
		//需要借位
		if (microsecond < u)
		{
			-- seconds;
			microsecond = (1000000 + microsecond - u);
		}
		else
		{
			microsecond -= u;
		}
	}

	return *this;
}

//返回微秒
long long _rtsp_date_time_::Dec(const _rtsp_date_time_ & time)
{
	long long	usec = seconds;
	usec = (1000000*usec) + microsecond;

	long long	usec1 = time.seconds;
	usec1 = (1000000*usec1) + time.microsecond;

	return (usec - usec1);
}

//返回秒
int _rtsp_date_time_::operator-(const _rtsp_date_time_ & time)
{
	return this->seconds - time.seconds;
}

bool _rtsp_date_time_::operator!=(const _rtsp_date_time_ & time) const
{
	return (0 == memcmp(this, &time, sizeof(RTSP_DATE_TIME))) ? false : true;
}

bool _rtsp_date_time_::operator==(const _rtsp_date_time_ & time) const
{
	return (0 == memcmp(this, &time, sizeof(RTSP_DATE_TIME))) ? true : false;
}

bool _rtsp_date_time_::operator<(const _rtsp_date_time_ & time) const
{
	if (this->seconds < time.seconds)
	{
		return true;
	}
	else
	{
		if (this->seconds == time.seconds)
		{
			return (this->microsecond < time.microsecond);
		}
		else
		{
			return false;
		}
	}
}

bool _rtsp_date_time_::operator<=(const _rtsp_date_time_ & time) const
{
	if (this->seconds < time.seconds)
	{
		return true;
	}
	else
	{
		if (this->seconds == time.seconds)
		{
			return (this->microsecond <= time.microsecond);
		}
		else
		{
			return false;
		}
	}
}

bool _rtsp_date_time_::operator>(const _rtsp_date_time_ & time) const
{
	if (this->seconds > time.seconds)
	{
		return true;
	}
	else
	{
		if (this->seconds == time.seconds)
		{
			return (this->microsecond > time.microsecond);
		}
		else
		{
			return false;
		}
	}
}

bool _rtsp_date_time_::operator>=(const _rtsp_date_time_ & time) const
{
	if (this->seconds > time.seconds)
	{
		return true;
	}
	else
	{
		if (this->seconds == time.seconds)
		{
			return (this->microsecond >= time.microsecond);
		}
		else
		{
			return false;
		}
	}
}


int  RTSPGetTime()
{
	int time = 0;

#ifdef WIN32
	SYSTEMTIME systemtime;
	GetLocalTime(&systemtime);

	tm tm_time;

	tm_time.tm_year		= systemtime.wYear - 1900;
	tm_time.tm_mon		= systemtime.wMonth - 1;
	tm_time.tm_mday		= systemtime.wDay;

	tm_time.tm_wday		= systemtime.wDayOfWeek;
	tm_time.tm_isdst	= 0;

	tm_time.tm_hour		= systemtime.wHour;
	tm_time.tm_min		= systemtime.wMinute;
	tm_time.tm_sec		= systemtime.wSecond;

	time = (int)mktime(&tm_time);
#else
	timeval tv;
	//timezone *tz;
	if (0 == gettimeofday(&tv, NULL))
	{
		time = tv.tv_sec;
	}
#endif

	return time;
}

RTSP_DATE_TIME RTSPGetCurrTime()
{
	RTSP_DATE_TIME time;

#ifdef WIN32
	SYSTEMTIME systemtime;
	GetLocalTime(&systemtime);

	tm tm_time;
	memset(&tm_time, 0, sizeof(tm));

	tm_time.tm_year		= systemtime.wYear - 1900;
	tm_time.tm_mon		= systemtime.wMonth - 1;
	tm_time.tm_mday		= systemtime.wDay;

	tm_time.tm_hour		= systemtime.wHour;
	tm_time.tm_min		= systemtime.wMinute;
	tm_time.tm_sec		= systemtime.wSecond;

	time				= tm_time;
	time.microsecond	= systemtime.wMilliseconds*1000;
#else
	timeval tv;
	//timezone *tz;
	if (0 == gettimeofday(&tv, NULL))
	{
		time = tv;
	}
#endif

	return time;
}

RTSP_LOCAL_TIME RTSPGetLocalTime()
{
	RTSP_LOCAL_TIME time;

#ifdef WIN32
	SYSTEMTIME systemtime;
	GetLocalTime(&systemtime);

	time.year		= systemtime.wYear;
	time.month		= systemtime.wMonth;
	time.mday		= systemtime.wDay;

	time.hour		= systemtime.wHour;
	time.minute		= systemtime.wMinute;
	time.second		= systemtime.wSecond;

	time.microsecond= systemtime.wMilliseconds*1000;
#else
	timeval tv;
	//timezone *tz;
	if (0 == gettimeofday(&tv, NULL))
	{
		tm tm_time;
		memset(&tm_time, 0, sizeof(tm));

		time_t SecNum = tv.tv_sec;

		if (NULL == localtime_r(&SecNum, &tm_time))
		{
			time.year	= 2012;
			time.month	= 1;
			time.mday	= 1;
			time.hour	= 0;
			time.minute	= 0;
			time.second	= 0;
			time.microsecond	= 0;

			assert(false);
		}
		else
		{
			time.year	= tm_time.tm_year + 1900;
			time.month	= tm_time.tm_mon + 1;
			time.mday	= tm_time.tm_mday;
			time.hour	= tm_time.tm_hour;
			time.minute	= tm_time.tm_min;
			time.second	= tm_time.tm_sec;
			time.microsecond	= tv.tv_usec;
		}

		return time;
	}
#endif

	return time;
}

bool _rtsp_data_frame_::operator!=(const _rtsp_data_frame_ & frame) const
{
	return (0 == memcmp(this, &frame, sizeof(RTSP_DATA_FRAME))) ? false : true;
}

bool _rtsp_data_frame_::operator==(const _rtsp_data_frame_ & frame) const
{
	return (0 == memcmp(this, &frame, sizeof(RTSP_DATA_FRAME))) ? true : false;
}
