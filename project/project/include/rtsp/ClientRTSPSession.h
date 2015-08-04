/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : CC
** Date         : 2012-12-05
** Version      : 1.0
** Description  : 整个RTSP会话相关，包括RTSP会话的建立，RTP和RTCP数据的接收，RTCP包的发送
** Modify Record:
1:
***********************************************************************/
#ifndef __CLIENT_RTSP_SESSION__
#define __CLIENT_RTSP_SESSION__

#include <string>
#include "RTPHeader.h"
#include "RTSPHeader.h"
#include "ClientRTSP.h"
#include "ClientRTP.h"

class CClientRTSPSession
{
public:
	CClientRTSPSession();
	~CClientRTSPSession();
	
	void SetChannelNumber(int chnn, int sub_chn );
	void SetAudioEnable( bool bAudioEnable );

	void SetCallBackErrorNumAddr( unsigned long* p_err ){p_ulCallBackError = p_err;}

	int ParseURL(char const* url, char* username, char* password, char* address,int &portNum);
	int Start(const char *pURL, const char *pUserName, const char *pPassWd);
	int Stop();

	std::string GetSessionID() {return m_SeesionID;}

	void SetCallBack(RTP_CALLBACK pCallBack, void *pParamm, int TVTSessionID);
	void SetFuncProcCallback(PRTP_FUNC_PROC_CALLBACK pFuncProcCallBack, void *pParam);

	void SetGetIpcUsedInfoCallback(PGET_IPC_USED_INFO pCallfn, void *pParam );
	void HandleConnectionBreak();

	//从上层模块获取数据并分发
	int GetData(const char* pData, unsigned long len, unsigned long recv_max_buf_len );

	//从下层模块取得数据
	int SendData(char* pData, unsigned long &len);
	int SendTearDown(char *pData);

	//控制超时
	unsigned long GetRecvWaitTime() {return m_RecvWaitTime;}
	void RecvWaitTimeInc() {m_RecvWaitTime++;}
	void ClearRecvWaitTime() {m_RecvWaitTime = 0;}

	//
	void HandleSystemTimeChanged();
	bool GetIsMustResyncSystem();

	unsigned long GetLastError(){return (m_ulLastError | m_RTP.GetLastError() | m_RTSP.GetLastError());};
	void PrintRunPos(){printf("m_run_lines sessid=%d\n", m_run_lines);m_RTP.PrintRunPos();m_RTSP.PrintRunPos();}

	bool ResetObject(void);

	bool GetRtspIsPlaying(){return m_RTSP.GetIsPlaying();}

private:
	void Initial();

	int CheckErrorDataIsLastFrameNext( const char* pBuffer, int len, bool& bFindDataFlag );
	int FindString(const char *pSrc, const int dataLen, const char *pPattern, int patter_len );
	int SundayChangeSearch(const unsigned char *text, int n, const unsigned char *patt, int m);

	inline char* x_strlwr(char* str)
	{
		char* orig = str;
		for (; *str != '\0'; str++)
		{
			*str = tolower(*str);
		}
		return orig;
	}


private:
	std::string			m_SeesionID;
	bool				m_bHasSetRTP;

	CClientRTSP			m_RTSP;
	CClientRTP			m_RTP;

	CPUB_Lock			m_Lock;

	RTSP_DATE_TIME		m_LastGetParaTime;
	RTSP_DATE_TIME		m_LastRTCPTime;

	unsigned long		m_RecvWaitTime;

	int                 m_channel_num;
	int                 m_sub_chn_num;

	bool                m_bFirstRtspDataFlag;

	unsigned int        m_iFirstCheckDataError;

	bool                m_bIMSendPing;

	unsigned long*      p_ulCallBackError;
	unsigned long       m_ulLastError;
	unsigned long       m_run_lines;

	unsigned char       m_cRtspAsPingReplayStr[256];
	int                 m_cRtspAsPingRelayMemcpyLen;
	int                 m_cRtspAsPingReplayLen;
	int                 m_cRtspAsPingReplayTmp;
};

#endif

