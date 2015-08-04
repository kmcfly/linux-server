/*****************************************************************
 Copyright (C),2009-2012, tongwei video Tech. Co., Ltd.

 File name	:SerialPort.h

 Author		: TVT-ZHL-146  
 Version		:	
 Date		:2009-09-23

 Description	:

 Others:

 Function List:

 History:
 
 	1.Date:

	  Author:

	  Modification:	
	2.
******************************************************************/


#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/

#include "Typedef.h"
#include "mylist.h"
#include "PTZ.h"
#include "PUB_common.h"


#if   defined(__CHIP3520__) || defined(__CHIP3531__) || defined(__CHIPGM__) 
const char dev2[]  = "/dev/ttyAMA2";
#elif defined (__CHIP3515__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__)
const char dev2[]  = "/dev/ttyAMA1";
#elif defined __TDFH__
const char dev2[]  = "/dev/ttyS1";
#elif defined __TDNXP__
const char dev2[]  = "/dev/ttyS2";
#elif defined __CHIP3535__
const char dev2[]  = "/dev/ttyAMA3";
#else
const char dev2[]  = "/dev/ttyS1";
#endif

const BYTE bufferLen = 100;

#define SUPPORT_PORTCTRL_DVR 

class CSerialPort
{
public:
	static CSerialPort *Instance();
	bool	Initial(bool bCtrl485CS);
	void	Quit();	
	bool	SetSerialParity(int databits, int stopbits, int parity);
	bool	SetSerialSpeed(PORT_BAUDRATE rate);   //设置串口通信速率
	bool	WriteToPort(const BYTE *pBuf,int len,PORT_BAUDRATE bandrate=SBR_1200);	
	bool	SetSerialParityRead(int readlength, int stopbits, int parity, BYTE ttydatalength);
	int GetfdCom(){ return m_fdCom;}
#ifdef SUPPORT_PORTCTRL_DVR
	int	ReadPort(BYTE *pBuf,int len,PORT_BAUDRATE bandrate=SBR_9600);
#endif
	//private:
	virtual ~CSerialPort();											

private:
	CSerialPort();

	int		m_fdCom;						//串口句柄
	CPUB_Lock	m_PortLock;
	FILE      *m_fp;
	unsigned int m_SetBaudrate;
	unsigned char m_writeflag;

	int    m_bCtrl485CS;                    //485是否需要片选.目前只2304SS 2308SS 2316SS
};

#endif


