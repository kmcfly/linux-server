
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author		: lufen
** Date		: 2010-7-19
** Name		: WirelessManage.h
** Version 	: 1.0
** Description  :
** Modify Record:
***********************************************************************/

#ifndef _WIRELESS_MANAGE_DLG_H
#define _WIRELESS_MANAGE_DLG_H

#ifdef __ENVIRONMENT_LINUX__
#include <termios.h>    /*PPSIX �ն˿��ƶ���*/
#include <errno.h>      /*����Ŷ���*/
#endif

#include "PUB_common.h"
#include "NetProtocol.h"
#include "PTZ.h"
#include "NetInterface.h"

//const char wireless_start[] = "pppd call huawei &";

class CWirelessManage
{
public:
	CWirelessManage();
	~CWirelessManage();
	static CWirelessManage*Instance();
	void Quit(void);
	bool Initial();
	bool WirelessUp(const char* name, unsigned short len);
	bool  WirelessDown(const char* name, unsigned short len);	 
	bool StartWireless();	 
	bool GetWirelessState();
	void SetWirelessPara(const MDVR_WIRELESS_CFG *para);
	void Stop();
	unsigned char GetSignal();
	bool Get3GState();
	bool GetCardState();
	bool GetDialingState();
	unsigned short Get3GType();	 
	bool GetWirelessThdState();
	unsigned short Get3GEnabled();  
	NET_CTRL_FAIL CheckWireless();
private:
	bool SetUSBPortParity(int databits, int stopbits, int parity, BYTE ttydatalength);
	bool SetUSBPortSpeed(PORT_BAUDRATE bandrate);
	int ReadDate(BYTE *pBuf,int len,PORT_BAUDRATE bandrate);
	bool WriteToPort(const char *pBuf,int len,PORT_BAUDRATE bandrate);
	bool CheckDialing(void);
	bool SendDialingCmd(void);
	bool CheckWhetherHaveNet();
	bool CheckWhetherHaveCard();
	bool CheckWhetherHaveSignal();
	bool CheckWhetherHaveDevice();
	bool GetSerialDevName(char* device, int len);
	void GetDNSName(char* name, unsigned short len);
	bool WirelessProc();
	bool LogoutDialing();
	static RESULT WINAPI WirelessThread(LPVOID lpParameter);

	bool HotPlug();
	static RESULT WINAPI WirelessHotPlug(LPVOID lpParameter);

	int		m_fd;						//usb �ھ��
	PUB_lock_t	m_lock;

	static CWirelessManage* m_pInstance;		
	FILE      *m_fp;	
	//	CPUB_Lock	m_PortLock;
	unsigned int m_SetBaudrate;	
	unsigned int m_SetBaudrateBak;
	bool			m_WirelessState;
	MDVR_WIRELESS_CFG m_wirelesscfg;

	bool			m_bWireless;		
	bool 			m_bBak;
	bool			m_bCheck;
	unsigned int 	m_signal;					//��ǰ�ź�ֵ
	bool 			m_bhavecard;				//�Ƿ���sim��
	bool 			m_bDialing;				//�����Ƿ�ɹ�
	bool			m_bWirelessInit;			//3gģ���Ƿ��ʼ����
	PUB_thread_t	m_WirelessProcID;			//�߳�ID
	char			m_3gDeviceName[128];		//3G�������� /dev/ttyUSB0

	bool 			m_bHotPlug;
	PUB_thread_t	m_WirelessHotPlugID;
};



#endif
