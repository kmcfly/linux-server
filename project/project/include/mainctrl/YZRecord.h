/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-03-21
** Name         : YZRecord.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _YZ_RECORD_H_
#define _YZ_RECORD_H_

#include "Product.h"
#include "PUB_common.h"
#include "RecordMan.h"
#include "MANUALDVRTimer.h"
#include "LogMan.h"
#include "UserMan.h"
enum YZ_REC_CMD//�ͻ��˷���������
{
	YZ_REC_STOP, //Զ��¼��ֹͣ
	YZ_REC_START,//Զ��¼����

};

class CYZRecord
{
public:
	static CYZRecord * Instance();
	~CYZRecord();
	bool Initial (unsigned char videoInputNum,CRecordMan *pRecMan);
	void Quit();
	bool start();//�����߳�
	void stop();//ֹͣ�߳�
	//������chnn-ͨ����
	//     recTime-¼���ӳ�ʱ��
	//	   recStatus-¼��״̬
	//	   clientID-��Ϣ�Ŀͻ���id��
	void StartRec(unsigned char chnn,unsigned short recTime,unsigned char recStatus,unsigned long clientID);//����¼��
	void StopRec(unsigned char chnn,unsigned char recStatus);//ֹͣ¼��
private:
	CYZRecord ();
	

	bool CreateYZCheckTimerThd();//����һ���̼߳���ͨ��¼���Ƿ���
	void CloseYZTimerThd();//�˳��߳�
	static	RESULT WINAPI YZTimerThread(LPVOID lpParameter);
	void YZTimer();
private:
	enum YZ_REC_STATUS
	{
		YZ_REC_ING,    //Զ��¼����
		YZ_REC_END,  //Զ��¼�����
	};
	
	int	*m_pYzRecStatus; //��¼Զ��¼��ĵ�ǰ״̬�� ����ͨ����
	PUB_thread_t	m_checkYzTimer;//��鶨ʱ���Ƿ�ʱ�߳�
	bool			m_bYzTimer;	
	CRecordMan		*m_pRecMan;
	CMANUALDVRTimer *m_pmaunalDVRTimer;//Զ��¼��ʱ��
	CPUB_Lock m_lock;
	unsigned char m_videoInputNum;
	unsigned long m_clientID;
};

#endif
