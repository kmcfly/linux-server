/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2007-10-12
** Name         : Schedule.h
** Version      : 1.0
** Description  :
** Modify Record:
1:原来使用了老的配置结构体，先改为用新的配置结构体。2009032516
***********************************************************************/
#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#include "dvrdvsdef.h"
#include "Calendar.h"
#include "mylist.h"
#include <list>

typedef struct _time_sec
{
	unsigned char	startTimeHour;		//开始时间——小时——[0,23]
	unsigned char	startTimeMinute;	//开始时间——分钟——[0,59]
	unsigned char	endTimeHour;		//结束时间——小时——[0,23]
	unsigned char	endTimeMinute;		//结束时间——分钟——[0,59]
}TIME_SEC;

class CSchedule
{
public:
	//interface
	CSchedule();
	~CSchedule();

	void SetWeekSchedule(const WEEK_SCHEDULE & schedule);
	void SetHolidaySchedule(const CMyList <HOLIDAY_SCHEDULE> &holidayList);
	bool CheckTime(unsigned long time);

	static void ScheduleToTimeSec(std::list<TIME_SEC> & timeSecList, const DATE_SCHEDULE &date);
	static void TimeSecToSchedule(const std::list<TIME_SEC> & timeSecList, DATE_SCHEDULE &date, const std::list<TIME_SEC>* pTmSecListDec = NULL);
protected:
	//methods

	//variable

private:
	//methods
	bool GetWeekdaySchedule(BYTE wDay, BYTE hour, BYTE min);

	bool GetHolidaySchedule(unsigned short year, BYTE month, BYTE day, BYTE hour, BYTE min);

	//variable
	WEEK_SCHEDULE	m_weekSchedule;
	CMyList <HOLIDAY_SCHEDULE> m_holidayList;
	CCalendar	m_calendar;
};
#endif //_SCHEDULE_H_
