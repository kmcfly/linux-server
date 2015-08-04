/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2010-07-03
** Name         : OtherAlarmMan.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _OTHER_ALARM_MAN_H_
#define _OTHER_ALARM_MAN_H_

#include "AlarmIn.h"

//��������������
class COtherAlarmMan  : public  CAlarmIn
{
public:

	//���캯��
	COtherAlarmMan();

	//��������
	virtual ~COtherAlarmMan();
protected:
	void InitialStatus();

	//�ú��������ƶ�¼������
	RECORD_TYPE GetRecType();

	ALARM_TYPE GetAlarmType(unsigned long chnn);

	//�������������ı����У�¼�񣬼̵���
	void OpenAlarm(unsigned long chnn);

	//�ر��Ѿ������ı���
	void CloseAlarm(unsigned long chnn);

private:
};

#endif //_OTHER_ALARM_MAN_H_
