/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author             : �ܷ�
** Date         	      : 2007-09-3
** Name              : Keyboard.h
** Version            : 3.0
** Description       :����ʵ��ARM����ץȡ��Ƭ
�����ڵ����ݣ�ʵ�������豸�Ŀ���
** Modify Record   :
1:��������CKeyboard��ΪCMCU8952
***********************************************************************/
#ifndef  _KEYBOARD_H_
#define  _KEYBOARD_H_

#include <termios.h>   
#include <errno.h>

//#define __REMOTE_TR12__ 1

#include "Typedef.h"
#include "PTZ.h"
#include "mylist.h"
#include "PUB_common.h"

//��Ƭ��8952ռ���˴���ttys2
#if defined (__CHIP3520__) || defined (__CHIP3531__) 
const char dev1[]  = "/dev/ttyAMA1";
#elif defined(__TDFH__)
const char dev1[]  = "/dev/ttyS2";
#elif defined(__TDNXP__)
const char dev1[]  = "/dev/ttyS3";
#elif defined __CHIP3535__
const char dev1[]  = "/dev/ttyAMA2";
#else
const char dev1[]  = "/dev/ttyS2";
#endif

//���ڲ���������
typedef enum _serial_operation
{
	SERIAL_SHUTDOWN 		=	1 ,     //��ػ�
	SERIAL_LAMP     			=	2 ,     //���
	SERIAL_ALARM			=	3 ,     //��ѯ�澯
	SERIAL_RELAY    			=	4 ,     //�ϵ���
	SERIAL_BUZZER   		=	5 ,     //������
	SERIAL_SPOT     			=	6 ,     //SPOT����, ��2316��û����, �÷ָ�оƬʵ��
	SERIAL_FEEDDOG  		=	7 ,		//ι��
	SERIAL_KILLDOG  		=	8 ,		//ɱ��
	SERIAL_CTRL485  		=	9 ,		//����485����app��485����ǰbuf[2]=1��app��485���ͽ�����buf[2]=0
	SERIAL_GETVERSION  	=	10,	//��ѯ��Ƭ���汾��Ӳ���汾
	SERIAL_SETVIDEOLOSSLAMP = 0xc, //��Ƭ������vedio loss ��Ϣ����Ƭ��������ʾloss״̬����16λ�ĵ�
    SERIAL_SETVIDEOLOSSLAMP_H = 0xd//��Ƭ������vedio loss ��Ϣ����Ƭ��������ʾloss״̬����16λ�ĵ�
} SERIAL_OPERATION;

//���ڲ�����
//����ػ�������
const BYTE	SERIAL_SHUTDOWN_ON =  0x00;
	
//��ƹصƲ�����
const BYTE	SERIAL_LAMP2_ON =  0x40;
const BYTE	SERIAL_LAMP3_ON =  0x20;
const BYTE	SERIAL_LAMP4_ON =  0x10;
const BYTE	SERIAL_LAMP5_ON =  0x08;
const BYTE	SERIAL_LAMP6_ON =  0x04;

const BYTE	SERIAL_LAMP2_OFF = 0xBC;
const BYTE	SERIAL_LAMP3_OFF = 0xDC;
const BYTE	SERIAL_LAMP4_OFF = 0xEC;
const BYTE	SERIAL_LAMP5_OFF = 0xF4;
const BYTE	SERIAL_LAMP6_OFF = 0xF8;

//�̵���������
const BYTE	SERIAL_RELAY1_ON	=  0x01;	
const BYTE	SERIAL_RELAY2_ON	=  0x02;
const BYTE	SERIAL_RELAY3_ON	=  0x04;
const BYTE	SERIAL_RELAY4_ON	=  0x08;

const BYTE	SERIAL_RELAY1_OFF	=  0x8E;	
const BYTE	SERIAL_RELAY2_OFF	=  0x8d;
const BYTE	SERIAL_RELAY3_OFF	=  0x8b;
const BYTE	SERIAL_RELAY4_OFF	=  0x87;

//������������
const BYTE	SERIAL_BUZZER_OFF	= 0x00;
const BYTE	SERIAL_BUZZER_ON	= 0x01;

//spot������
const BYTE	SERIAL_SPOT_CH1		= 0x0;
const BYTE	SERIAL_SPOT_CH2		= 0x1;
const BYTE	SERIAL_SPOT_CH3		= 0x2;
const BYTE	SERIAL_SPOT_CH4		= 0x3;
const BYTE	SERIAL_SPOT_CH5		= 0x4;
const BYTE	SERIAL_SPOT_CH6		= 0x5;
const BYTE	SERIAL_SPOT_CH7		= 0x6;
const BYTE	SERIAL_SPOT_CH8		= 0x7;

//ι��������
const BYTE	SERIAL_FEEDDOG_ON	= 0x0;

const BYTE 	LIST_KEYVALUE_NUM		= 10;


class CMcu8952
{
public:
	static CMcu8952* Instance ();		//singleton mode
	bool Initial( DWORD &sensorInputType );						//init keyboard and start lisen thread
	void Quit();						//stop listen thread
	
	//DVR��ȡ���������ң����������
	bool GetMCUKeyInfo(KEY_VALUE_INFO *pKeyInfo);

	bool Start();
	void Stop();

	//��������
	bool Operation(SERIAL_OPERATION opType, unsigned long opeValue);

	//��ѯ�澯��Ϣ
	DWORD inline GetAlarm();

	//��ѯ��Ƭ���汾
	void inline GetMCUVersion(char* msg);

	//��ѯӲ���汾
	void inline GetHardwareVersion(char* msg);

	//��ȡMCU �����������
	int GetMCUSerialConfig( struct termios *pOptions);

	//��ӡMCU����������Ϣ
	void PrintMCUSerialConfig(struct termios *pOptions);

protected:
	static RESULT WINAPI ThreadKeyboardProc(LPVOID lpParame);
	
private:
	CMcu8952();
	virtual ~CMcu8952();

	//����¼�������
	void AddEventList(unsigned long);


	//���ô�������λ��ֹͣλ��Ч��λ
	bool SetSerialParity(int databits, int stopbits, int parity);

	//���ô���ͨ������
	bool SetSerialSpeed(PORT_BAUDRATE baudrate);

	//����������
	void ReadSerial();

	//д���ڲ���
	bool WriteSerial(BYTE eventType, BYTE eventValue);
	
	//д���ڲ���
	bool WriteSerialEx(BYTE eventType1, BYTE eventValue2, BYTE eventValue3);

	
private:
	static CMcu8952* m_pInstance;
	CMyList <unsigned long>  m_MEventList;
	CPUB_Lock   m_ListLock;
	HANDLE  m_ThreadHandle;
	bool    m_ThreadFlag;

	//�򿪵Ĵ����豸���
	int m_fd;

	//��д������
	CPUB_Lock serialLock;

	//ָʾ�Ƶ���,Ϩ��״̬
	BYTE m_LampState;

	//ָʾ�̵���״̬
	BYTE m_Delay;

	//�澯��Ϣ
	//BYTE m_Alarm;	//kg_20080310
	BYTE m_Alarm_get_flag;
	DWORD m_Alarm;

	WORD m_Reply;
	//
	pthread_mutex_t        m_com_mutex;

	WORD m_VersionChip;		//��Ƭ���汾
	WORD m_VersionHard;		//Ӳ���汾
};


DWORD CMcu8952::GetAlarm()
{
	DWORD alarm = m_Alarm;
	m_Alarm_get_flag = 1;
	return alarm;
}

void  CMcu8952::GetMCUVersion(char* msg)
{
	assert( NULL!=msg );
	sprintf(msg,"%02d.%02d.%02d",((m_VersionChip>>9) & 0x7f), ((m_VersionChip>>5) & 0xf), (m_VersionChip & 0x1f));
}

void  CMcu8952::GetHardwareVersion(char* msg)
{
	assert( NULL!=msg );
	sprintf(msg,"%d.%d", ((m_VersionHard>>8) & 0xff), (m_VersionHard & 0xff) );
}

#endif
