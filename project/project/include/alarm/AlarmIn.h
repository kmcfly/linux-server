/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-11-06
** Name         : 
** Version      : 1.0
** Description  : ����������࣬�Ӹ���̳г�CMotionMan,CSensorMan, CVideoLossMan
** Modify Record:
1:
***********************************************************************/
#ifndef _ALARMIN_H_
#define _ALARMIN_H_

#include "PUB_common.h"
#include "DVRTimer.h"
#include "AlarmOutMan.h"
#include "LogMan.h"
#include "StreamRecordManEx.h"
#include "RecordMan.h"
#include "DisplayMan.h"
#include "Schedule.h"

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
#include "FTPUpdateMan.h"
#endif

#ifdef __ENVIRONMENT_LINUX__
#include "PtzMan.h"
#include "NetDeviceManager.h"

#ifdef __DVR_BASIC__
#include "SmtpMan.h"
#endif
#endif

#include "PushMan.h"

typedef enum _to_ptz_type
{
	TO_PTZ_NULL,
	TO_PTZ_PRESET,
	TO_PTZ_CRUISE,
	TO_PTZ_TRACK
}TO_PTZ_TYPE;

typedef struct _alarm_attrib
{
	unsigned short	bDetection;						//�Ƿ���Ҫ��������
	unsigned short	received;						//��ʱ�����

	unsigned long	holdTime;						//�ӳ�ʱ��

	unsigned char	toPTZType[MAX_CHNN_NUM];		//����PTZ������
	unsigned char	toPTZPreset[MAX_CHNN_NUM];		//������̨Ԥ�õ�
	unsigned char	toPTZCruise[MAX_CHNN_NUM];		//������̨Ⱥ��
	unsigned char	toPTZTrack[MAX_CHNN_NUM];		//�����켣

	DVR_TIMER_TYPE	timerType;

	ULONGLONG		toRecCH;						//����¼��ͨ��
	ULONGLONG		toAlarmOut;						//�����������	
	ULONGLONG		hasRecCH;						//�Ѿ�����¼��

	ULONGLONG		toBigViewCH;					//��������ͨ��
	unsigned long   toEmail;                        //����email����

	ULONGLONG       toSnapCH;                       //����ץͼͨ��
	ULONGLONG       toFtpCH;                        //����ftpͨ��
	ULONGLONG		hasUpdateCH;					//�Ѿ��ϴ���¼��ͨ��
	ULONGLONG       toEmailCH;                      //����Emailץͼͨ��
}ALARM_ATTRIB;

class CAlarmIn  
{
public:

	//�ӿڲ���
	//���캯��
	CAlarmIn();

	//��������
	virtual ~CAlarmIn();
	//////////////////////////////////////////////////////////////////////////
	//��ʼ��
	bool Initial(unsigned char chnnNum, DVR_TIMER_TYPE timerType, CAlarmOutMan *pAlarmOutMan, CLogMan *pLogMan, CStreamRecordManEx *pStreamRecManEx, CDisplayMan *pDisplayMan, unsigned long localVideoInputNum, unsigned long netVideoInputNum);
	//������
	void Quit();
	//////////////////////////////////////////////////////////////////////////
	void SetEnable(bool bEnable, unsigned char chnn);
	void Action(bool bEnable);
	void SetHoldTime(unsigned short holdTime, unsigned char chnn);
	void SetToRecCH(ULONGLONG toRecCH, unsigned char chnn);
	void SetToAlarmOutCH(ULONGLONG toAlarmOutCH, unsigned char chnn, unsigned char relayOutNum);
	void SetToBigViewCH(ULONGLONG toBigViewCH, unsigned char chnn);
	void SetToBuzzer(bool bToBuzzer, unsigned char chnn, unsigned char relayOutNum);
	//���ô�������������̨��Ϣ
	void SetToPTZ(const TO_PTZ *pToPTZ, int alarmInputDeviceNum, int videoInputNum);
	void SetToEmail(unsigned long toEmail, unsigned char chnn);
	void SetToSnapCH(ULONGLONG toSnapCH, unsigned char chnn);
	//////////////////////////////////////////////////////////////////////////
	//���������뺯����
	ULONGLONG AlarmIn(ULONGLONG chnnBits);
	void Timer();
	void SetWeekSchedule( int chnn, WEEK_SCHEDULE weekSchedule ){ m_pSchedule[chnn].SetWeekSchedule( weekSchedule);}

	void SetToFTPVideo(ULONGLONG toFtpCH, unsigned char chnn);
	void SetToFTPPic(ULONGLONG toFtpCH, unsigned char chnn);
protected:

	//������д����
	virtual	inline void AdjustAlarmIn(ULONGLONG & chnnBits) {};
	//��motion����̨���ܴ����Լ���������
	virtual void DisableTrigerSelf( int alarmOutDeviceNum ){;}

	virtual void AdjustSnap(ULONGLONG & llSnapChnn, unsigned long alarmChnn){;}

	virtual void InitialStatus() = 0;

	//��ȡ¼������
	virtual RECORD_TYPE GetRecType() = 0;

	//��ȡ��������
	virtual inline ALARM_TYPE GetAlarmType(unsigned long chnn) {return ALARM_TYPE_NULL;};

	//��������
	virtual void OpenAlarm(unsigned long chnn) = 0;

	//�رձ���
	virtual void CloseAlarm(unsigned long chnn) = 0;

	//������״̬
	enum ALARMIN_STATUS
	{
		ALARMIN_STATUS_NO,     //�Ǳ���
		ALARMIN_STATUS_YES,    //������
		ALARMIN_STATUS_DELAY,  //�ӳ���
	};


	//���²����ǣ�������Թ��õĲ���
	//�õ�ͨ��Ϊ��chnn�ı���������Ϣ��������Ӧ��ͨ������¼��
	void Record(unsigned long chnn);

	//�õ�ͨ��Ϊ��chnn�ı���������Ϣ���ر���Ӧ��ͨ��¼��
	void StopRecord(unsigned long chnn);

	//������̨
	void toPTZ(unsigned long chnn);

	//�ر���̨
	void StopPTZ(unsigned long chnn);

	//���������̵���
	void AlarmOut(unsigned long chnn);
	void CloseAlarmOut(unsigned long chnn);

	//��������
	void BigView(unsigned long chnn);

	//�����ʼ�
	void SendEmail(unsigned long chnn);


	//ץȡͼƬ
	void SnapPicture(unsigned long chnn);


	void StartFtpUpdate(unsigned long chnn);

	void StopFtpUpdate(unsigned long chnn);
protected:
	unsigned char	m_chnnNum;
	//////////////////////////////////////////////////////////////////////////
	CDVRTimer		*m_pTimer;	//�����������
	CAlarmOutMan	*m_pAlarmOutMan;
	CLogMan			*m_pLogMan;
	CStreamRecordManEx		*m_pStreamRecManEx;
	CDisplayMan		*m_pDisplayMan;
	//////////////////////////////////////////////////////////////////////////
	ULONGLONG		m_status;
	//////////////////////////////////////////////////////////////////////////
	//������״̬���Ǳ���״̬������״̬���ӳټ�ʱ״̬
	bool			*m_pAlarmOut;	//��¼��Ҫ��������
	int				*m_pAlarmInStatus; //��¼��������ĵ�ǰ״̬�� ����ͨ���� 
	ALARM_ATTRIB	*m_pAlarmInAttrib; //������������ԣ�����ͨ����
	//bool            *m_pOldDection;    //���ڱ������ߺͼ���
	EVENT_LOG		*m_pEventInfo;
	//////////////////////////////////////////////////////////////////////////
	CPUB_Lock		m_lock;

	bool            m_bAction;

	CSchedule		*m_pSchedule;		//��¼����ͨ�����ų�������Ϣ

	LONGLONG        m_SendEmailIntval;
	LONGLONG        *m_pLastSendEmailTime;

	PUSH_ARARM_TYPE	m_alarmType;
	unsigned long	m_localVideoInputNum;
	unsigned long	m_netVideoInputNum;
};
#endif 

