/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-11-06
** Name         : 
** Version      : 1.0
** Description  : 报警输入基类，从该类继承出CMotionMan,CSensorMan, CVideoLossMan
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
	unsigned short	bDetection;						//是否需要触发报警
	unsigned short	received;						//计时器序号

	unsigned long	holdTime;						//延迟时间

	unsigned char	toPTZType[MAX_CHNN_NUM];		//触发PTZ的类型
	unsigned char	toPTZPreset[MAX_CHNN_NUM];		//触发云台预置点
	unsigned char	toPTZCruise[MAX_CHNN_NUM];		//触发云台群组
	unsigned char	toPTZTrack[MAX_CHNN_NUM];		//触发轨迹

	DVR_TIMER_TYPE	timerType;

	ULONGLONG		toRecCH;						//触发录像通道
	ULONGLONG		toAlarmOut;						//触发报警输出	
	ULONGLONG		hasRecCH;						//已经触发录像

	ULONGLONG		toBigViewCH;					//触发大画面通道
	unsigned long   toEmail;                        //触发email报警

	ULONGLONG       toSnapCH;                       //触发抓图通道
	ULONGLONG       toFtpCH;                        //触发ftp通道
	ULONGLONG		hasUpdateCH;					//已经上传的录像通道
	ULONGLONG       toEmailCH;                      //触发Email抓图通道
}ALARM_ATTRIB;

class CAlarmIn  
{
public:

	//接口部分
	//构造函数
	CAlarmIn();

	//析构函数
	virtual ~CAlarmIn();
	//////////////////////////////////////////////////////////////////////////
	//初始化
	bool Initial(unsigned char chnnNum, DVR_TIMER_TYPE timerType, CAlarmOutMan *pAlarmOutMan, CLogMan *pLogMan, CStreamRecordManEx *pStreamRecManEx, CDisplayMan *pDisplayMan, unsigned long localVideoInputNum, unsigned long netVideoInputNum);
	//清理函数
	void Quit();
	//////////////////////////////////////////////////////////////////////////
	void SetEnable(bool bEnable, unsigned char chnn);
	void Action(bool bEnable);
	void SetHoldTime(unsigned short holdTime, unsigned char chnn);
	void SetToRecCH(ULONGLONG toRecCH, unsigned char chnn);
	void SetToAlarmOutCH(ULONGLONG toAlarmOutCH, unsigned char chnn, unsigned char relayOutNum);
	void SetToBigViewCH(ULONGLONG toBigViewCH, unsigned char chnn);
	void SetToBuzzer(bool bToBuzzer, unsigned char chnn, unsigned char relayOutNum);
	//设置传感器触发的云台信息
	void SetToPTZ(const TO_PTZ *pToPTZ, int alarmInputDeviceNum, int videoInputNum);
	void SetToEmail(unsigned long toEmail, unsigned char chnn);
	void SetToSnapCH(ULONGLONG toSnapCH, unsigned char chnn);
	//////////////////////////////////////////////////////////////////////////
	//处理报警输入函数，
	ULONGLONG AlarmIn(ULONGLONG chnnBits);
	void Timer();
	void SetWeekSchedule( int chnn, WEEK_SCHEDULE weekSchedule ){ m_pSchedule[chnn].SetWeekSchedule( weekSchedule);}

	void SetToFTPVideo(ULONGLONG toFtpCH, unsigned char chnn);
	void SetToFTPPic(ULONGLONG toFtpCH, unsigned char chnn);
protected:

	//子类重写部分
	virtual	inline void AdjustAlarmIn(ULONGLONG & chnnBits) {};
	//在motion中云台不能触发自己产生动作
	virtual void DisableTrigerSelf( int alarmOutDeviceNum ){;}

	virtual void AdjustSnap(ULONGLONG & llSnapChnn, unsigned long alarmChnn){;}

	virtual void InitialStatus() = 0;

	//获取录像类型
	virtual RECORD_TYPE GetRecType() = 0;

	//获取报警类型
	virtual inline ALARM_TYPE GetAlarmType(unsigned long chnn) {return ALARM_TYPE_NULL;};

	//开启报警
	virtual void OpenAlarm(unsigned long chnn) = 0;

	//关闭报警
	virtual void CloseAlarm(unsigned long chnn) = 0;

	//报警的状态
	enum ALARMIN_STATUS
	{
		ALARMIN_STATUS_NO,     //非报警
		ALARMIN_STATUS_YES,    //报警中
		ALARMIN_STATUS_DELAY,  //延迟中
	};


	//以下部分是：子类可以共用的部分
	//得到通道为号chnn的报警配置信息，开启相应的通道进行录像
	void Record(unsigned long chnn);

	//得到通道为号chnn的报警配置信息，关闭相应的通道录像
	void StopRecord(unsigned long chnn);

	//开启云台
	void toPTZ(unsigned long chnn);

	//关闭云台
	void StopPTZ(unsigned long chnn);

	//开启报警继电器
	void AlarmOut(unsigned long chnn);
	void CloseAlarmOut(unsigned long chnn);

	//触发大画面
	void BigView(unsigned long chnn);

	//发送邮件
	void SendEmail(unsigned long chnn);


	//抓取图片
	void SnapPicture(unsigned long chnn);


	void StartFtpUpdate(unsigned long chnn);

	void StopFtpUpdate(unsigned long chnn);
protected:
	unsigned char	m_chnnNum;
	//////////////////////////////////////////////////////////////////////////
	CDVRTimer		*m_pTimer;	//管理计数器类
	CAlarmOutMan	*m_pAlarmOutMan;
	CLogMan			*m_pLogMan;
	CStreamRecordManEx		*m_pStreamRecManEx;
	CDisplayMan		*m_pDisplayMan;
	//////////////////////////////////////////////////////////////////////////
	ULONGLONG		m_status;
	//////////////////////////////////////////////////////////////////////////
	//有三种状态：非报警状态，报警状态，延迟计时状态
	bool			*m_pAlarmOut;	//记录需要触发报警
	int				*m_pAlarmInStatus; //记录报警输入的当前状态， （多通道） 
	ALARM_ATTRIB	*m_pAlarmInAttrib; //报警输入的属性，（多通道）
	//bool            *m_pOldDection;    //用于报警休眠和激活
	EVENT_LOG		*m_pEventInfo;
	//////////////////////////////////////////////////////////////////////////
	CPUB_Lock		m_lock;

	bool            m_bAction;

	CSchedule		*m_pSchedule;		//记录所有通道的排程输入信息

	LONGLONG        m_SendEmailIntval;
	LONGLONG        *m_pLastSendEmailTime;

	PUSH_ARARM_TYPE	m_alarmType;
	unsigned long	m_localVideoInputNum;
	unsigned long	m_netVideoInputNum;
};
#endif 

