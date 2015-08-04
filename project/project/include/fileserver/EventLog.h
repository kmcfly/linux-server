/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :Ԭʯά
** Date         : 2008-12-08
** Name         : EventLog.h
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#ifndef _EVENT_LOG_H_
#define _EVENT_LOG_H_

#include "Log.h"

class CEventLog : public CLog
{
public:
	CEventLog();
	~CEventLog();

#if 0
	unsigned long GetEventLog (EVENT_LOT_LIST & list);
#endif

	long WriteEventLog(EVENT_LOG & event);
	unsigned long GetEventLog(unsigned long startTime , unsigned long endTime, unsigned char type, ULONGLONG chnnBits, EVENT_LOT_LIST & list);
protected:
private:
	bool GetLogTime(unsigned long index, unsigned long & startTime, unsigned long & endTime);
	bool GetLogTime(unsigned char *pData, unsigned long num , unsigned long index, unsigned long & startTime, unsigned long & endTime);
	void AddToList(const EVENT_LOG & log, EVENT_LOT_LIST & list);
};
#endif //_EVENT_LOG_H_
