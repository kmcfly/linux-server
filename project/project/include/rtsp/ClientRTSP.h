/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2012-12-05
** Version      : 1.0
** Description  : RTSPЭ����أ�ֻ��Կͻ��ˣ�ֻ��ȡ�ͻ��˹��ĵ�����
** Modify Record:
1:
***********************************************************************/

#ifndef __CLIENT_RTSP__
#define __CLIENT_RTSP__

#include <string>
#include "RTPSock.h"
#include "RTPHeader.h"
#include "RTSPHeader.h"
#include "HttpDigestAuth.h"
#include "SDPParser.h"

const unsigned long RTP_CLIENT_RTSP_SEND_LEN = 1024*8;
const unsigned long RTP_CLIENT_RTSP_RECV_LEN = 1024*8;
const unsigned char MAX_RTSP_METHOD = 12;
const char RTSP_AUTHENTICATE_DIGEST[] = "Digest";
const char RTSP_AUTHENTICATE_BasicT[] = "Basic";

class CClientRTSP
{
	typedef enum _authenticate_type_
	{
		AUTHENTICATE_NONE,
		AUTHENTICATE_BASIC,
		AUTHENTICATE_DIGEST,
	}AUTHENTICATE_TYPE;
public:
	CClientRTSP();
	~CClientRTSP();
	
	int CreateFirstCommand(const char *pURL, const char *pUserName = NULL, const char *pPassWd = NULL);
	int CreateOptions(const char *pURL, const char *pUserName = NULL, const char *pPassWd = NULL);
	int HandleUnLockData(const char *pData, unsigned long len);//���յ�session����������
	int HandleData(const char *pData, unsigned long len);
	int GetBufferData(char *pBuffer, unsigned long &len);//�ⲿ����  ��ȡ����buff�е�����
	RTSP_SESSION_STATUS GetStatus() {return m_CurStatus;}
	bool GetIsPlaying(){bool ret; m_Lock.Lock();ret = (RTSP_SESSION_PLAY == m_CurStatus) ? true : false;m_Lock.UnLock();return ret;}
	int GetCurMethod() {int ret; m_Lock.Lock(); ret = m_SendingMethod; m_Lock.UnLock();return ret;}
	int Play();
	int TearDown();
	bool IsSendBufEmpty();
	bool IsWaitingforReply() {bool ret; m_Lock.Lock();ret = m_bIsSending;m_Lock.UnLock();return ret;}
	bool IsHasAudio();
	bool IsHasVideo();
	float GetFrameRate();
	unsigned short GetWidth();
	unsigned short GetHeight();
	unsigned int GetTimeOut() {unsigned int ret;m_Lock.Lock(); ret = m_TimeOut; m_Lock.UnLock(); return ret;}

	int CreateGetParameter();
	int CreateTeardown(char *pData);
	bool IsSupportGetParameter();


	int GetVideoPayloadType();
	int GetAudioPayloadType();
	int GetAudioDataType();

	void SetChannelNumber( int chnn, int sub_chn );
	void SetAudioEnableFlag( bool bAudioEnable );

	unsigned long GetLastError(){return m_ulLastError;}
	void PrintRunPos(){printf("m_run_lines rtsp=%d\n", m_run_lines);}

	void SetGetIpcInfoCallback(PGET_IPC_USED_INFO pCallback,void *pParam)
		{m_pGetIpcUsedInfo=pCallback;m_pGetIpcUsedParam=pParam;m_HttpDigestAuth.SetGetIpcInfoCallback(pCallback,pParam);}

	char* GetSpsPps( char *pDes, int &len ){ return m_SDPPaerser.GetSpsPps(pDes, len);}
	bool ResetObject(void);


private:
	int ParseData(char *pData);

	int CreateDescribe();
	int CreateSetup(STREAM_TYPE streamType);
	int CreateSetparameter();
	int CreatePlay();
	int CreateTeardown();
	int SendMsg(const char *pMsg, unsigned long msgLen);
	int GetStatusCode(char *pStatusCode);
	int GetTransportInfo(char *pRTransportInfo, unsigned long length);

private:
	CSDPParser			m_SDPPaerser;
	CPUB_Lock			m_Lock;
	int					m_SendingMethod;//��ǰ�Ѿ�����ȥ��ָ��
	RTSP_SESSION_STATUS m_CurStatus;
	bool				m_bIsSending;
	bool				m_bISTCPSession;
	bool				m_bISUnicast;
	bool				m_bIsHasAudio;
	bool				m_bIsHasVideo;
	std::string			m_sessionID;

	//�������
	char				*m_pRecvBuf;
	char				*m_pSendBuf;
	unsigned long		m_SendDataLen;
	unsigned long		m_recvDataLen;

	//request-line��Ϣ
	unsigned long		m_cseq;
	std::string			m_method;
	std::string			m_serverURL;
	std::string			m_rtspVersion;
	std::string			m_SetupRequest[SDP_MEDIA_STRAM_END];//0Ϊ��Ƶ 1Ϊ��Ƶ
	unsigned int        m_MediaPayloadType[SDP_MEDIA_STRAM_END];
	unsigned int        m_AudioDataType;
	unsigned long		m_interleaveChannel;
	std::string			m_agentDes;

	char				m_ContentType[64];
	unsigned int		m_ContentLen;
	unsigned char		m_PacketizationMode;
	unsigned int		m_TimeOut;

	//www-Authenticate���
	std::string			m_realm;
	std::string			m_nonce;
	std::string			m_mode;//digest
	std::string			m_UserName;
	std::string			m_PassWd;
	std::string			m_Response;
	int					m_ncount;
	CHttpDigestAuth		m_HttpDigestAuth;
	AUTHENTICATE_TYPE	m_Authenticate;

	int					m_RequestTime;
	//������֧�ֵ�ָ��
	char				m_ServerSupportMethod[MAX_RTSP_METHOD][15];

	int                m_chnnel_number;
	int                m_sub_chn_num;

	bool               m_bOpenAudioData;

	unsigned long      m_ulLastError;
	unsigned long      m_run_lines;

	PGET_IPC_USED_INFO m_pGetIpcUsedInfo;
	void *             m_pGetIpcUsedParam;

	bool               m_bSupportGetParamFn;
};

#endif
