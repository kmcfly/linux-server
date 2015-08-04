/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2007-10-12
** Name         : Schedule.h
** Version      : 1.0
** Description  :
** Modify Record:
1:ԭ��ʹ�����ϵ����ýṹ�壬�ȸ�Ϊ���µ����ýṹ�塣2009032516
***********************************************************************/
#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#include "dvrdvsdef.h"
#include "Calendar.h"
#include "mylist.h"
#include <list>

typedef struct _time_sec
{
	unsigned char	startTimeHour;		//��ʼʱ�䡪��Сʱ����[0,23]
	unsigned char	startTimeMinute;	//��ʼʱ�䡪�����ӡ���[0,59]
	unsigned char	endTimeHour;		//����ʱ�䡪��Сʱ����[0,23]
	unsigned char	endTimeMinute;		//����ʱ�䡪�����ӡ���[0,59]
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
