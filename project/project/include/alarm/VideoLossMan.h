/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2008-12-31
** Name         : 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _VIDEO_LOSS_MAN_H_
#define _VIDEO_LOSS_MAN_H_
#include "AlarmIn.h"

class CVideoLossMan : public CAlarmIn
{
public:

	//���캯��
	CVideoLossMan();

	//��������
	virtual ~CVideoLossMan();
protected:
	void InitialStatus();
	//�ú��������ƶ�¼������
	RECORD_TYPE GetRecType();

	inline ALARM_TYPE GetAlarmType(unsigned long chnn) {return ALARM_TYPE_VIDEOLOSS;};

	//�ƶ���ⴥ���ı����У�¼�񣬼̵���
	void OpenAlarm(unsigned long chnn);

	//�ر��Ѿ������ı���
	void CloseAlarm(unsigned long chnn);

	void AdjustSnap(ULONGLONG & llSnapChnn, unsigned long alarmChnn);
private:
#if defined(__SUPPORT_SNAPD1__)
	bool *m_pAlarmUpload;
#endif
};
#endif //_VIDEO_LOSS_MAN_H_
