

/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-11-06
** Name         : 
** Version      : 1.0
** Description  : �������������࣬
** Modify Record:
1:
***********************************************************************/

#ifndef _SENSOR_IN_MAN_H_
#define _SENSOR_IN_MAN_H_

#include "AlarmIn.h"

#if defined(__SUPPORT_SNAPD1__)
#include "ShanghaiCmd.h"
#include "ShanghaiConnectMan.h"
#endif
//��������������
class CSensorInMan  : public  CAlarmIn
{
public:
	
	//���캯��
	CSensorInMan();

	//��������
	virtual ~CSensorInMan();

	//�����豸���ͣ�NO��NC
	void SetDeviceType(bool bNO, unsigned char chnn);
protected:
	void InitialStatus();
	void AdjustAlarmIn(ULONGLONG & chnnBits);

	//�ú������ص�¼������Ϊ��������¼��
	RECORD_TYPE GetRecType();

	inline ALARM_TYPE GetAlarmType(unsigned long chnn) {return ALARM_TYPE_SENSOR;};

	//�������������ഥ���ı����У�¼����̨���̵���
	void OpenAlarm(unsigned long chnn);

	//�ر��Ѿ������ı���
	void CloseAlarm(unsigned long chnn);

private:

	//�ñ������ڵ�������ı�����Ϣ��
	//�ñ����е�ĳλ��0����ʾ�ڸ�λ�ϵ������ź���1��Ч��0��Ч
	//              ��1����ʾ�ڸ�λ�ϵ������ź���1��Ч��0��Ч
	//����������Ǻ��ⲿ�ź��������
	ULONGLONG m_chnnBits;
#if defined(__SUPPORT_SNAPD1__)
	bool *m_pAlarmUpload;
#endif
};

#endif //_SENSOR_IN_MAN_H_
