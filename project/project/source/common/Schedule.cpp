/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2007-10-12
** Name         : Schedule.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "Schedule.h"
#include "PUB_common.h"
#include "mylist.cpp"

//public
void DecMinute(unsigned char & h, unsigned char & m)
{
	if (0 == m)
	{
		assert (h > 0);
		-- h;
		m = 59;
	}
	else
	{
		-- m;
	}
}

void CSchedule::ScheduleToTimeSec(std::list<TIME_SEC> & timeSecList, const DATE_SCHEDULE &date)
{
	//������б�
	timeSecList.clear();

	TIME_SEC sec;
	memset(&sec, 0, sizeof(TIME_SEC));
	
	bool bStart = false;
	for (unsigned char h=0; h<24; ++h)
	{
		for (unsigned char m=0; m<60; ++m)
		{
			//����m��Ӧ�ķ���Ϊ��Ч���Ҵ�ʱ��ҪѰ��һ�εĿ�ʼ
			if ((date.hour[h] & ((ULONGLONG)(0x01) << m)) && !bStart)
			{
				sec.startTimeHour	= h;
				sec.startTimeMinute	= m;
				bStart = !bStart;
			}

			//����m��Ӧ�ķ���Ϊ��Ч���Ҵ�ʱ��ҪѰ��һ�εĽ���
			if ((0 == (date.hour[h] & ((ULONGLONG)(0x01) << m))) && bStart)
			{
				sec.endTimeHour		= h;
				sec.endTimeMinute	= m;

				DecMinute(sec.endTimeHour, sec.endTimeMinute);

				bStart = !bStart;

				timeSecList.push_back(sec);
			}
		}
	}

	//���һ����Ҳ����Ч�ģ�����ֻ�п�ʼ��ǣ���û���ҵ��������
	if (bStart)
	{
		sec.endTimeHour		= 23;
		sec.endTimeMinute	= 59;

		timeSecList.push_back(sec);
	}
}

void CSchedule::TimeSecToSchedule(const std::list<TIME_SEC> & timeSecList, DATE_SCHEDULE &date, const std::list<TIME_SEC>* pTmSecListDec)
{
	memset(&date, 0, sizeof(DATE_SCHEDULE));

	if (0 != timeSecList.size())
	{
		std::list<TIME_SEC>::const_iterator it;
		for (it = timeSecList.begin(); it != timeSecList.end(); ++it) 
		{
			const TIME_SEC & sec = *(it);
			
			{
				//һ��Сʱ֮��
				if (sec.startTimeHour == sec.endTimeHour)
				{
					for (unsigned char m=sec.startTimeMinute; m<=sec.endTimeMinute; ++m)
					{
						date.hour[sec.startTimeHour] |= ((ULONGLONG)(0x01) << m);
					}
				}
				else if (sec.startTimeHour < sec.endTimeHour)
				{
					//��ʼһ��Сʱ�ڲ�
					ULONGLONG info = 0;
					for (unsigned char m=sec.startTimeMinute; m<60; ++m)
					{
						info |= ((ULONGLONG)(0x01) << m);
					}
					date.hour[sec.startTimeHour] |= info;

					for (unsigned char h=sec.startTimeHour+1; h<sec.endTimeHour; ++h)
					{
						date.hour[h] = ((ULONGLONG)0x0fffffff<<32) + (ULONGLONG)(0xffffffff);
					}

					info = 0;
					for (unsigned char m=0; m<=sec.endTimeMinute; ++m)
					{
						info |= ((ULONGLONG)(0x01) << m);
					}
					date.hour[sec.endTimeHour] |= info;
				}
			}

		//	it = timeSecList.erase(it);
		}
	}

	if(pTmSecListDec && 0 != pTmSecListDec->size())
	{
		std::list<TIME_SEC>::const_iterator it;
		for (it = pTmSecListDec->begin(); it != pTmSecListDec->end(); ++it) 
		{
			const TIME_SEC & sec = *(it);

			{
				//һ��Сʱ֮��
				if (sec.startTimeHour == sec.endTimeHour)
				{
					for (unsigned char m=sec.startTimeMinute; m<=sec.endTimeMinute; ++m)
					{
						date.hour[sec.startTimeHour] &= ~((ULONGLONG)(0x01) << m);
					}
				}
				else if (sec.startTimeHour < sec.endTimeHour)
				{
					//��ʼһ��Сʱ�ڲ�
					ULONGLONG info = 0;
					for (unsigned char m=sec.startTimeMinute; m<60; ++m)
					{
						info |= ((ULONGLONG)(0x01) << m);
					}
					date.hour[sec.startTimeHour] &= ~info;

					for (unsigned char h=sec.startTimeHour+1; h<sec.endTimeHour; ++h)
					{
						date.hour[h] = 0/*((ULONGLONG)0x0fffffff<<32) + (ULONGLONG)(0xffffffff)*/;
					}

					info = 0;
					for (unsigned char m=0; m<=sec.endTimeMinute; ++m)
					{
						info |= ((ULONGLONG)(0x01) << m);
					}
					date.hour[sec.endTimeHour] &= ~info;
				}
			}
		}
	}
}

CSchedule::CSchedule()
{
	memset(&m_weekSchedule, 0, sizeof (WEEK_SCHEDULE));
}

CSchedule::~CSchedule()
{
}

void CSchedule::SetWeekSchedule(const WEEK_SCHEDULE & schedule)
{
	m_weekSchedule = schedule;
}

void CSchedule::SetHolidaySchedule(const CMyList <HOLIDAY_SCHEDULE> &holidayList)
{
	//�Է�ֹ�ظ����룬�����
	m_holidayList.RemoveAll();

	POS pos = holidayList.GetHeadPosition();
	while (NULL != pos)
	{
		m_holidayList.AddTail(holidayList.GetNext(pos));
	}
}

bool CSchedule::CheckTime(unsigned long time)
{
	tm t = DVRTime32ToTm(time);
	
	BYTE wDay = m_calendar.GetWeekday((t.tm_year + 1900), (t.tm_mon + 1), t.tm_mday);
	
	return GetWeekdaySchedule(wDay, t.tm_hour, t.tm_min) || GetHolidaySchedule(t.tm_year + 1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min);
}

//protected

//private
bool CSchedule::GetWeekdaySchedule(BYTE wDay, BYTE hour, BYTE min)
{
	return (0 != (m_weekSchedule.week[wDay].hour[hour] & ((ULONGLONG)(0x01) << min))) ? true : false;
}

bool CSchedule::GetHolidaySchedule(unsigned short year, BYTE month, BYTE day, BYTE hour, BYTE min)
{
	POS pos = m_holidayList.GetHeadPosition();
	while (NULL != pos)
	{
		HOLIDAY_SCHEDULE & node = m_holidayList.GetNext(pos);

#if 0
		if ((year == node.year) && (month == node.month) && (day == node.day))
#else
		if ((month == node.month) && (day == node.day))
#endif
		{
			return (0 != (node.date.hour[hour] & ((ULONGLONG)(0x01) << min))) ? true : false;
		}
	}

	return false;
}
//end
