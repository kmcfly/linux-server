/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Zds
** Date         : 2007-09-10
** Name         : Calendar.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/

#ifndef _CALENDAR_H_
#define _CALENDAR_H_


const unsigned long HUNDRED_YEAR_DAYS = 36524;
const unsigned long FOUR_YEAR_DAYS   = 1461;
const unsigned long ONE_YEAR_DAYS   = 365;

enum WEEK_DAY
{
SUNDAY      = 0,
MONDAY      = 1,
TUESDAY     = 2,
WEDNESDAY   = 3,
THURSDAY    = 4,
FRIDAY      = 5,
SATURDAY    = 6,
SUM_WK_DAYS = 7
};

class CCalendar
{
public:
	CCalendar();
	virtual ~CCalendar();

	//返回某年某月有多少天，返回零表示参数错误
	unsigned char GetMonthDays(unsigned short year, unsigned char month);

	//返回某年某月某日为星期几
	unsigned char GetWeekday(unsigned short year, unsigned char month, unsigned char mday);

private:
    //判断是否为闰年
	bool IsLeapYear(unsigned short year);

	//返回某年某月某日是一年里的第几天
	unsigned short GetYearDays(unsigned short year, unsigned char month, unsigned char mday);

};

#endif //_CALENDAR_H_

