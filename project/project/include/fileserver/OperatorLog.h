/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :Ԭʯά
** Date         : 2008-12-08
** Name         : OperatorLog.h
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#ifndef _OPERATOR_LOG_H_
#define _OPERATOR_LOG_H_

#include "Log.h"

class COperatorLog : public CLog
{
public:
	COperatorLog();
	~COperatorLog();

#if 0
	unsigned long GetOperatorList (OPERATOR_LOG_LIST & list);
#endif

	long WriteOperatorLog(OPERATOR_LOG & operatorLog);
	unsigned long GetOperatorLog(unsigned long startTime , unsigned long endTime, unsigned short contentID, OPERATOR_LOG_LIST & list);
protected:
private:
	bool GetLogTime(unsigned long index, unsigned long & startTime, unsigned long & endTime);
	bool GetLogTime(unsigned char *pData, unsigned long num , unsigned long index, unsigned long & startTime, unsigned long & endTime);
	void AddToList(const OPERATOR_LOG & log, OPERATOR_LOG_LIST & list);
};
#endif //_OPERATOR_LOG_H_
