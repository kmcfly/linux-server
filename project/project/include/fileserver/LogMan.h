/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :袁石维
** Date         : 2008-12-10
** Name         : LogMan.h
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#ifndef _LOG_MAN_H_
#define _LOG_MAN_H_

#include "EventLog.h"
#include "OperatorLog.h"

class CLogMan
{
public:
	CLogMan ();
	~CLogMan ();

	static CLogMan* Instance();

	bool Initial (DISK_LOG_INFO *pDiskStatus, unsigned char diskNum, const char *pFilePath = NULL);
	void Quit();

//	bool Action(bool bAction, unsigned char newDiskNum);
	bool ActionDisk(DISK_LOG_INFO *pDiskNewStatus, unsigned char newNum, bool bAction);
    bool AddDisk(DISK_LOG_INFO *pDiskNewStatus, unsigned char newNum);
	bool WriteEventLog(EVENT_LOG & event);
	bool WriteOperatorLog(OPERATOR_LOG & log);

	unsigned long GetOperatorLog(unsigned long startTime , unsigned long endTime, unsigned short contentID, OPERATOR_LOG_LIST & list);
	unsigned long GetEventLog(unsigned long startTime , unsigned long endTime, unsigned char type, ULONGLONG chnnBits, EVENT_LOT_LIST & list);
    
	//获取正在使用的分区掩码。lowMask用于表示第1个到第64个分区的情况。heightMask用于表示第65个到第128个分区的情况
	void GetUsingPartitionMask(ULONGLONG &heightMask, ULONGLONG &lowMask);

protected:
private:
	
	bool InitialImp(DISK_LOG_INFO *pDiskStatus, unsigned char diskNum, const char *pFilePath = NULL);

	//////////////////////////////////////////////////////////////////
	bool ActionDiskIndexE(POS &actionPos);	//return disk index for event
	bool ActionDiskIndexO(POS &actionPos);	//return disk index for operator
	//////////////////////////////////////////////////////////////////
	typedef struct _log_section
	{
		unsigned long startTime;
		unsigned long endTime;
	}LOG_SECTION;

	typedef struct _log_status
	{
		bool           bAction;
		unsigned char  diskOwnedBy;
		unsigned char  diskIndex;
		unsigned char  diskProperty;
		LOG_SECTION    firstLog;
		LOG_SECTION    endLog;		
	}LOG_STATUS;
	typedef CMyList<LOG_STATUS> LOG_STATUS_LIST;


	char			*m_pFilePath;

	LOG_STATUS_LIST m_logDiskListE;
	POS             m_actionDPE;	
	CEventLog		m_actionEventLog;
	ULONGLONG       m_usingDiskMaskEL;
	ULONGLONG       m_usingDiskMaskEH;

	LOG_STATUS_LIST m_logDiskListO;
    POS             m_actionDPO;	
	COperatorLog	m_actionOperatorLog;
	ULONGLONG       m_usingDiskMaskOL;
	ULONGLONG       m_usingDiskMaskOH;
	

	//bool            m_bAction;
	//unsigned char   m_diskNum;
	CPUB_Lock		m_lock;
};

#endif //_LOG_MAN_H_
