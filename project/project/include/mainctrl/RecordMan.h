/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2007-10-08
** Name         : RecordMan.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _RECORD_MAN_H_
#define _RECORD_MAN_H_

#include "RecordProc.h"
#include "Schedule.h"
#include "DVRTimer.h"

class CRecordMan
{
public:
	//interface
	CRecordMan();
	~CRecordMan();
	bool Initial(unsigned char videoInputNum, unsigned char audioInputNum,unsigned long preRecordType, CReclogManEX *pReclogManEx, const char *pFilePath = NULL, unsigned char preRecStartPar = 0, unsigned char parNum = 0, CMessageMan *pMsgMan = NULL);
	void GetInitialPreDiskRecordDiskMask(ULONGLONG &heightMask, ULONGLONG &lowMask);

	void Quit();
	void Action(bool bAction);
	
	RECORD_WRITE_STATUS DateProc(CFrameData & frameData, bool & bKeyFrame);
	bool Record(unsigned char chnn, RECORD_TYPE type);
	bool StopRecord(unsigned char chnn, RECORD_TYPE type);
	
	bool Start();
	void Stop();
	void RemoveAllDate();

	//�������ò���
	//ֻȡ��Ƶ����Ƶ
	void SetRecEnable(FRAME_TYPE type, bool bEnable, unsigned char chnn);
	//ֻȡ�ų̡��ƶ���⡢����
	void SetWeekSchedule(RECORD_TYPE type, const WEEK_SCHEDULE & weekSch, unsigned char chnn);
	void SetHolidaySchedule(RECORD_TYPE type, const CMyList <HOLIDAY_SCHEDULE> &holidayList, unsigned char chnn);
	void SetHoldTime(unsigned long holdTime, unsigned char chnn);
	void SetPreRecTime(unsigned long time, unsigned char chnn);
	void SetPreRecTime(unsigned long time);
	void SetPreRecDataLen(unsigned char chnn, unsigned long maxPreRecDataLen);
	void RecoverPreDiskRecTime();

	//����ѭ��¼�������¼��
	void SetRecycleRecord(bool bEnable);
	void SetRedundancyRecord(bool bEnable, unsigned char chnn);

	//
	void ResetPreDiskRecordPath(unsigned char preRecStartPar, unsigned char parNum, ULONGLONG &heightMask, ULONGLONG &lowMask);


	//�������ͨ����״̬�����Ƿ�����ֶ�¼�񼰼ƻ�¼��
	void SetChannelLive(unsigned char chnn, bool bLive);
protected:
	//methods

	//variable
private:
	//methods
	void Timer();

	static	RESULT WINAPI TimerThread(LPVOID lpParameter);

	void ResetStatus();
	//variable
	bool			m_bTimer;
	PUB_thread_t	m_timerTheadID;
	CDVRTimer		*m_pTimer;

	unsigned char		m_videoInputNum;
	unsigned char		m_audioInputNum;

	unsigned char		*m_pRecord;
	unsigned char		*m_pSchedule;
	unsigned char		*m_pMotion;
	unsigned char		*m_pSensor;
	unsigned char		*m_pManual;
	unsigned char		*m_pAudio;

	unsigned char		*m_pPreRecProc;
	unsigned char		*m_pActionRec;

	unsigned char		*m_pManualStatus;
	unsigned char		*m_pMotionStatus;
	unsigned char		*m_pSensorStatus;
	unsigned char		*m_pScheduleStatus;
	short				*m_pMotionCount;
	short				*m_pSensorCount;
	unsigned long		*m_pRecHoldTime;

	CRecordProc			*m_pRecordProc;

	CSchedule			*m_pScheduleSch;
	CSchedule			*m_pMotionSch;
	CSchedule			*m_pSensorSch;

	CPUB_Lock		m_actionLock;

	bool                 m_bAction;

	//�����ڳ�ʼ��ʱԤ¼���õ����ĸ�����
	ULONGLONG          m_initialPreRecordLMask;
	ULONGLONG          m_initialPreRecordHMask;

	unsigned long      m_preRecordType;
	unsigned char		*m_pChannelLive;

	enum
	{
		RECORD_WAITING,
		RECORD_DOING,
		RECORD_DELAY,
	};
};
#endif //_RECORD_MAN_H_
