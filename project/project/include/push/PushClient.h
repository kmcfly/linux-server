/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2013-08-26
** Version      : 1.0
** Description  : DVR对iphone客户端的消息推送
** Modify Record:
1:
***********************************************************************/
#ifndef __PUSH_CLIENT_H__
#define __PUSH_CLIENT_H__

#include "PushDefine.h"
#include "SWL_TypeDefine.h"
#include "SWL_Public.h"

const unsigned long PUSH_CLIENT_BUF_LEN = 16 * 1024;
#if !defined(WIN32)
const unsigned long PUSH_CLIENT_IDLE_TIME = 60;		//S
#else
const unsigned long PUSH_CLIENT_IDLE_TIME = 30;		//S
#endif
class CPushClient
{
public:
	CPushClient();
	~CPushClient();

	bool Initial(const char * pProviderId, unsigned long pidLen, const char * pdvrIP="", unsigned short webPort=80, const char * pCostomID=NULL, unsigned long ulMaxBufMsg = 200);
	void Quit();

	bool Start();
	void Stop();

	bool PushData(const unsigned char * pData, unsigned long dataLen);

	bool AddDeviceTokenInfo(const DEVICE_TOKEN_INFO_PNTS * pDTKIPnts);

	void ChangeIP(const char * pdvrIP);
	void ChangePort(unsigned short port);
	void ChangeServerInfo(const char * pServerIP, unsigned short serverPort);
	void SetPushServerInfo(bool bstatus){m_bGetPushServerInfo = bstatus;};
protected:
private:
	void PushMsg(PUSH_MSG_TYPE msgType, const unsigned char * pMsgData, unsigned long dataLen);

	int DataProc();

	//接收数据
	
	int RecvData();
	int RecvData(const unsigned char * buf, int needRecLen);
	int AnalysisRecvMsg();

	//发送数据
	int SendData();
	int SendData(const unsigned char * buf, int needSendLen);
	int SendDataOver();

	bool UpdateProviderInfo();
	bool UpdateToken();
	bool CheckStatus();
	bool ConnectPtns();
	SWL_socket_t CreateSock(unsigned short localPort, unsigned short remotePort, unsigned long remoteInAddr);

	void ClearPushMsg()
	{
		while (!m_pushMsgList.empty())
		{
			PUSH_MSG_HEADER * pushMsgHeader = m_pushMsgList.front();
			m_pushMsgList.pop_front();
			delete pushMsgHeader;
		}
	}

	void ClearDTK()
	{
		while (!m_dtkList.empty())
		{
			DEVICE_TOKEN_INFO_PNTS * pDTKIPnts = m_dtkList.front();
			m_dtkList.pop_front();
			delete pDTKIPnts;
		}
	}
private:
	static PUB_THREAD_RESULT PUB_THREAD_CALL PushClientThread(void *pParam);
	void PushClientProc();

	typedef enum __push_client_status__
	{
		PUSH_CLIENT_CLOSEED		= 0,		//关闭状态，连接后要发送设备信息到PTNS
		PUSH_CLIENT_CONNECTTING	= 1,		//收到PTNS的回应后进入这个状态，表示可以发送正常报警数据和TOKEN信息
		PUSH_CLIENT_CONNECTED	= 2,		//收到PTNS的回应后进入这个状态，表示可以发送正常报警数据和TOKEN信息
		PUSH_CLIENT_ERROR		= 3			//出错，要重连
	}PUSH_CLIENT_STATUS;

	void SetClientStatus(PUSH_CLIENT_STATUS newStatus)
	{
		m_clientStatusLock.Lock();
		m_clientStatus = newStatus;
		m_clientStatusLock.UnLock();
	}

	PUSH_CLIENT_STATUS GetClientStatus()
	{
		m_clientStatusLock.Lock();
		PUSH_CLIENT_STATUS  cs = m_clientStatus;
		m_clientStatusLock.UnLock();

		return cs;
	}

	bool IsClosed()
	{
		bool bIsCloseed = false;
		m_clientStatusLock.Lock();
		if (PUSH_CLIENT_CLOSEED == m_clientStatus)
		{
			bIsCloseed = true;
		}
		m_clientStatusLock.UnLock();

		return bIsCloseed;
	}

	bool IsConnected()
	{
		bool bIsConnected = false;
		m_clientStatusLock.Lock();
		if (PUSH_CLIENT_CONNECTED == m_clientStatus)
		{
			bIsConnected = true;
		}
		m_clientStatusLock.UnLock();

		return bIsConnected;
	}

	bool IsConnectting()
	{
		bool bIsConnectting = false;
		m_clientStatusLock.Lock();
		if (PUSH_CLIENT_CONNECTTING == m_clientStatus)
		{
			bIsConnectting = true;
		}
		m_clientStatusLock.UnLock();

		return bIsConnectting;
	}

	bool IsError()
	{
		bool bIsError = false;
		m_clientStatusLock.Lock();
		if (PUSH_CLIENT_ERROR == m_clientStatus)
		{
			bIsError = true;
		}
		m_clientStatusLock.UnLock();

		return bIsError;
	}

private:

	PUB_thread_t	m_hPushClientID;
	bool			m_bPushClientProc;

	std::list<PUSH_MSG_HEADER *>	m_pushMsgList;
	CPUB_Lock						m_pushMsgListLock;

	std::list<DEVICE_TOKEN_INFO_PNTS *>	m_dtkList;
	CPUB_Lock						m_dtkListLock;


	char				m_szServerIP[64];
	unsigned short		m_serverPot;
	unsigned long		m_iServerIp;

	PROVIDER_TYPE_INFO	m_providerInfo;
	PUSH_CLIENT_STATUS	m_clientStatus;
	CPUB_Lock			m_clientStatusLock;
	SWL_socket_t		m_sock;
	unsigned long		m_ltdTime;		//上次发送数据的时间，从这个时间开始，持续1分钟没有数据发送就可以关闭连接
	
	//接收
	PUSH_MSG_HEADER		m_msgHeader;
	PUSH_MSG_INFO		* m_pRecvMsgInfo;
	PUSH_DATA_REPLY		* m_pPDR;
	unsigned char		m_recvBuf[PUSH_CLIENT_BUF_LEN];
	unsigned long		m_recvBufLen;
	unsigned long		m_recvPos;
	bool				m_bNeedRep;

	//发送
	PUSH_MSG_INFO		* m_pSendMsgInfo;
	unsigned char		m_sendBuf[PUSH_CLIENT_BUF_LEN];
	unsigned long		m_sendBufLen;
	unsigned long		m_sendPos;
	unsigned long		m_needSendLen;
	
	unsigned long		m_maxBufMsg;

	bool				m_bGetPushServerInfo;
};

#endif
