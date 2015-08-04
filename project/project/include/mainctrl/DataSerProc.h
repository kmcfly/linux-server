/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : yuanshiwei
** Date         : 2009-06-12
** Name         : DataSerProc.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _DATA_SER_PROC_H_
#define _DATA_SER_PROC_H_

#ifdef __ENVIRONMENT_LINUX__ 
#include "DiskManager.h"
#endif
#include "ReclogManEx.h"
#include "LogMan.h"
#include "MessageMan.h"

#include "SnapMan.h"


class CDataSerProc
{
public:
	CDataSerProc();
	~CDataSerProc();
#ifdef __ENVIRONMENT_LINUX__
	bool Initial(CMessageMan *pMsgMan, CReclogManEX *pReclogMan, CLogMan *pLogMan, CDiskManager *pDiskMan);
#else
	bool Initial(CMessageMan *pMsgMan, CReclogManEX *pReclogMan, CLogMan *pLogMan, unsigned long channelNum);
#endif
	void Quit();
	bool Start();
	void Stop();
	//////////////////////////////////////////////////////////////////////////
	void PutMsg(const MESSAGE_DATA & msgData);
	
protected:
private:
	static PUB_THREAD_RESULT PUB_THREAD_CALL MessageThread(void *pParam);
	int MessageProc();
	void MessageProc(const MESSAGE_DATA & msgData);
	NET_PROTOCOL Protocol(const MESSAGE_DATA & msgData);
	void MsgData(MESSAGE_DATA &msgData, unsigned long clientID, long cmdType, unsigned char *pData, unsigned long dataLen);
	bool GetMsg(MESSAGE_DATA &msgData);
	void WriteLog(unsigned long clientID, LOG_TYPE type);
	//////////////////////////////////////////////////////////////////////////
	CMessageMan		*m_pMsgMan;
	CReclogManEX	*m_pReclogMan;
	CLogMan			*m_pLogMan;
#ifdef __ENVIRONMENT_LINUX__
	CDiskManager    *m_pDiskMan;
#endif
	std::list<MESSAGE_DATA>	m_msgList;
	CPUB_Lock       m_msgListLock;
	PUB_thread_t    m_msgProcID;
	bool            m_bMsgProc;
	MESSAGE_DATA    m_localMsgData;
	PACKCMD         m_cmd;
#ifdef __ENVIRONMENT_WIN32__ 
   unsigned long    m_channelNum;
#endif


   CSnapMan         *m_pSnapPicMan;

};
#endif //_DATA_SER_PROC_H_

