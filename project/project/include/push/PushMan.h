/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2013-08-26
** Version      : 1.0
** Description  : DVR对iphone客户端的消息推送
** Modify Record:
1:
***********************************************************************/
#ifndef __PUSH_MAN_H__
#define __PUSH_MAN_H__

#include "PushClient.h"
#include "PushDefine.h"

class CPushMan
{
public:
	~CPushMan();

	static CPushMan * Instance();

	bool Initial(const char * pMSN=NULL, const char * pCostomID=NULL, unsigned long ulMaxBufMsg = 200);
	void Quit();

	bool Start();
	void Stop();

	void PushData(PUSH_DATA_TYPE type, void * pData, int len, unsigned long channel=0xffffffff);
	
	int AddDeviceTokenInfo(unsigned long clientID, const DEVICE_TOKEN_INFO * deviceTokenInfo);

	void ChangeIP(const char * pdvrIP);
	void ChangePort(unsigned short port);
	void UsePush(bool bUsePush)
	{
		m_bUsePush = bUsePush;
	}
	void ChangeServerInfo(const char * pServerIP, unsigned short serverPort);

protected:
private:
	static PUB_THREAD_RESULT PUB_THREAD_CALL PushThread(void *pParam);
	void PushProc();
	CPushMan();
private:
	void DispatchPushMsg();
	void GetTipMsg(char * pTipMsg, int &tipLen, PUSH_DATA_TYPE type, void * pData, int len);
	void GetMsgValue(int &msgValue, PUSH_DATA_TYPE type, void * pData, int len);
private:
	
	bool				m_bUsePush;
	static CPushMan * m_pPushMan;

	PUB_thread_t	m_hPushThreadID;
	bool			m_bPushThreadProc;
	CPushClient		* m_pPushClient;

	std::list<PUSH_MSG_HEADER *>m_pushInfoList;
	CPUB_Lock					m_pushMsgListLock;

	bool 			m_bGetPushServerInfo;
};

#endif

