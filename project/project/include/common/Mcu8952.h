/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author             : 周峰
** Date         	      : 2007-09-3
** Name              : Keyboard.h
** Version            : 3.0
** Description       :该类实现ARM串口抓取单片
机串口的数据，实现面板对设备的控制
** Modify Record   :
1:把类名有CKeyboard改为CMCU8952
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

//单片机8952占用了串口ttys2
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

//串口操作命令字
typedef enum _serial_operation
{
	SERIAL_SHUTDOWN 		=	1 ,     //软关机
	SERIAL_LAMP     			=	2 ,     //点灯
	SERIAL_ALARM			=	3 ,     //查询告警
	SERIAL_RELAY    			=	4 ,     //断电器
	SERIAL_BUZZER   		=	5 ,     //蜂鸣器
	SERIAL_SPOT     			=	6 ,     //SPOT功能, 在2316中没有用, 用分割芯片实现
	SERIAL_FEEDDOG  		=	7 ,		//喂狗
	SERIAL_KILLDOG  		=	8 ,		//杀狗
	SERIAL_CTRL485  		=	9 ,		//控制485方向：app向485发送前buf[2]=1；app向485发送结束后buf[2]=0
	SERIAL_GETVERSION  	=	10,	//查询单片机版本和硬件版本
	SERIAL_SETVIDEOLOSSLAMP = 0xc, //向单片机设置vedio loss 信息，单片机会点灯显示loss状态，低16位的灯
    SERIAL_SETVIDEOLOSSLAMP_H = 0xd//向单片机设置vedio loss 信息，单片机会点灯显示loss状态，高16位的灯
} SERIAL_OPERATION;

//串口操作数
//软件关机操作数
const BYTE	SERIAL_SHUTDOWN_ON =  0x00;
	
//点灯关灯操作数
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

//继电器操作数
const BYTE	SERIAL_RELAY1_ON	=  0x01;	
const BYTE	SERIAL_RELAY2_ON	=  0x02;
const BYTE	SERIAL_RELAY3_ON	=  0x04;
const BYTE	SERIAL_RELAY4_ON	=  0x08;

const BYTE	SERIAL_RELAY1_OFF	=  0x8E;	
const BYTE	SERIAL_RELAY2_OFF	=  0x8d;
const BYTE	SERIAL_RELAY3_OFF	=  0x8b;
const BYTE	SERIAL_RELAY4_OFF	=  0x87;

//蜂鸣器操作数
const BYTE	SERIAL_BUZZER_OFF	= 0x00;
const BYTE	SERIAL_BUZZER_ON	= 0x01;

//spot操作数
const BYTE	SERIAL_SPOT_CH1		= 0x0;
const BYTE	SERIAL_SPOT_CH2		= 0x1;
const BYTE	SERIAL_SPOT_CH3		= 0x2;
const BYTE	SERIAL_SPOT_CH4		= 0x3;
const BYTE	SERIAL_SPOT_CH5		= 0x4;
const BYTE	SERIAL_SPOT_CH6		= 0x5;
const BYTE	SERIAL_SPOT_CH7		= 0x6;
const BYTE	SERIAL_SPOT_CH8		= 0x7;

//喂狗操作数
const BYTE	SERIAL_FEEDDOG_ON	= 0x0;

const BYTE 	LIST_KEYVALUE_NUM		= 10;


class CMcu8952
{
public:
	static CMcu8952* Instance ();		//singleton mode
	bool Initial( DWORD &sensorInputType );						//init keyboard and start lisen thread
	void Quit();						//stop listen thread
	
	//DVR获取按键板或者遥控器的输入
	bool GetMCUKeyInfo(KEY_VALUE_INFO *pKeyInfo);

	bool Start();
	void Stop();

	//操作串口
	bool Operation(SERIAL_OPERATION opType, unsigned long opeValue);

	//查询告警信息
	DWORD inline GetAlarm();

	//查询单片机版本
	void inline GetMCUVersion(char* msg);

	//查询硬件版本
	void inline GetHardwareVersion(char* msg);

	//获取MCU 串口相关配置
	int GetMCUSerialConfig( struct termios *pOptions);

	//打印MCU串口配置信息
	void PrintMCUSerialConfig(struct termios *pOptions);

protected:
	static RESULT WINAPI ThreadKeyboardProc(LPVOID lpParame);
	
private:
	CMcu8952();
	virtual ~CMcu8952();

	//添加事件到队列
	void AddEventList(unsigned long);


	//设置串口数据位，停止位和效验位
	bool SetSerialParity(int databits, int stopbits, int parity);

	//设置串口通信速率
	bool SetSerialSpeed(PORT_BAUDRATE baudrate);

	//读串口数据
	void ReadSerial();

	//写串口操作
	bool WriteSerial(BYTE eventType, BYTE eventValue);
	
	//写串口操作
	bool WriteSerialEx(BYTE eventType1, BYTE eventValue2, BYTE eventValue3);

	
private:
	static CMcu8952* m_pInstance;
	CMyList <unsigned long>  m_MEventList;
	CPUB_Lock   m_ListLock;
	HANDLE  m_ThreadHandle;
	bool    m_ThreadFlag;

	//打开的串口设备句柄
	int m_fd;

	//读写串口锁
	CPUB_Lock serialLock;

	//指示灯点亮,熄灭状态
	BYTE m_LampState;

	//指示继电器状态
	BYTE m_Delay;

	//告警信息
	//BYTE m_Alarm;	//kg_20080310
	BYTE m_Alarm_get_flag;
	DWORD m_Alarm;

	WORD m_Reply;
	//
	pthread_mutex_t        m_com_mutex;

	WORD m_VersionChip;		//单片机版本
	WORD m_VersionHard;		//硬件版本
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
