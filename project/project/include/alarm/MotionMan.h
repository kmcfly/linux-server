
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-11-06
** Name         : 
** Version      : 1.0
** Description  : �ƶ��������࣬
** Modify Record:
1:
***********************************************************************/

#ifndef _MOTIONMAN_H_
#define _MOTIONMAN_H_

#include "AlarmIn.h"

//�ƶ���������
class CMotionMan  : public  CAlarmIn
{
public:
	
	//���캯��
	CMotionMan();

	//��������
	virtual ~CMotionMan();
protected:
	void InitialStatus();
	void DisableTrigerSelf( int alarmOutDeviceNum );

	//�ú��������ƶ�¼������
	RECORD_TYPE GetRecType();

	inline ALARM_TYPE GetAlarmType(unsigned long chnn) {return ALARM_TYPE_MONTION;};

	//�ƶ���ⴥ���ı����У�¼�񣬼̵���
	void OpenAlarm(unsigned long chnn);

	//�ر��Ѿ������ı���
	void CloseAlarm(unsigned long chnn);

private:
#if defined(__SUPPORT_SNAPD1__)
	bool *m_pAlarmUpload;
#endif
};

#endif


