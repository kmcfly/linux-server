/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Zds
** Date         : 2007-09-10
** Name         : Calendar.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/

#include "Calendar.h"

CCalendar::CCalendar( )
{

}

CCalendar::~CCalendar( )
{

}

bool CCalendar::IsLeapYear(unsigned short year)
{
	if (0 != (year % 4))
	{
		return false;
	}
	else if ((0 == (year % 100)) && (0 != (year % 400)))
	{
		return false;
	}
	else
	{
		return true;
	}
}

unsigned short CCalendar::GetYearDays(unsigned short year, unsigned char month, unsigned char mday)
{
	unsigned short days = 0;

	for(unsigned char m=1; m<month; ++m)
	{
		days += GetMonthDays(year, m);
	}

	days += mday;
	return days;
}

unsigned char CCalendar::GetMonthDays(unsigned short year, unsigned char month)
{
	unsigned char leapyear = IsLeapYear(year) ? 1 : 0;
	switch(month)
	{
	case 1:
		return 31;
	case 2:
		return 28 + leapyear;
	case 3:
		return 31;
	case 4:
		return 30;
	case 5:
		return 31;
	case 6:
		return 30;
	case 7:
		return 31;
	case 8:
		return 31;
	case 9:
		return 30;
	case 10:
		return 31;
	case 11:
		return 30;
	case 12:
		return 31;
	default:
		return 0;
	}
}

unsigned char CCalendar::GetWeekday(unsigned short year , unsigned char month, unsigned char mday)
{
	//应该把某一年做成0-399轮回中的一个，所以应该先减一
	unsigned short short_year = (year - 1) % 400;

	unsigned long day_sum     = (short_year / 100) * HUNDRED_YEAR_DAYS;

	short_year = short_year % 100;

	day_sum += (short_year / 4) * FOUR_YEAR_DAYS;

	day_sum += (short_year % 4) * ONE_YEAR_DAYS;

	day_sum += GetYearDays(year, month, mday);

	return static_cast<unsigned char>(day_sum % SUM_WK_DAYS);
}

