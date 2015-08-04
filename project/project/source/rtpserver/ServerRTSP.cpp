#include "ServerRTSP.h"
//#include "V4Device.h"
#include "Base64.h"


#ifdef _DEBUG
#define  new DEBUG_NEW
#endif

CServerRTSP::CServerRTSP(int VideoType, int AudioType, int rtpPort, float FrameRate)
{
	m_CurSessionStatus = RTSP_SESSION_START;
	m_Cseq = 0;
	m_VideoType = VideoType;
	m_AudioType = AudioType;
	m_UDPServerPort = rtpPort;
	m_FrameRate = FrameRate;
	m_SetupTimes = 0;
	m_Channel = 255;

	char tmp[32] = {0};
	unsigned long unique = (unsigned long)(this);
	unsigned char t, i=0;

	//itoa
	while(0 != unique)
	{
		t = unique % 10;
		tmp[i++] = '0' + t;
		unique /= 10;
	}
	char result[32] = {0};
	t = strlen(tmp);
	for (i=0; i<t; ++i)
	{
		result[i] = tmp[t-i-1];
	}
	m_SessionID = result;

	m_bUseAuthentication = true;
	memset(m_CurrNonce, 0, sizeof(m_CurrNonce));
	memset(m_CurUserName, 0, sizeof(m_CurUserName));
	memset(m_CurPasswd, 0, sizeof(m_CurPasswd));

	m_TimeOut = 60;
	m_bIsUDP = false;
}

CServerRTSP::~CServerRTSP()
{

}

int CServerRTSP::HandleMsg(const char *pMsg, char *pBuf, unsigned long &Len)
{
	if (NULL == pBuf)
	{
		return -1;
	}
	const char *pTailPos = strstr(pMsg, HEADER_TAIL_LABEL);
	int ret = 0;
	if (NULL == pTailPos)
	{
		return -1;
	}
	int MsgLen = pTailPos - pMsg + 4;

	do 
	{
		ret = AnalyseRequestLine(pMsg);
		if (0 != ret)
		{
			return -1;
		}
		int method = GetRTSPMethodType(m_method);
		if (RTSP_METHOD_END == method)
		{
			return -1;
		}

		if(!CheckCSeq(pMsg))
		{
			return -1;
		}

		if (m_bUseAuthentication)
		{
			if (!CheckAuthentication(pMsg, RTSP_METHODS[method]))
			{
				CreateAuthenticationMsg(pBuf, Len);
				break;
			}
		}

		//OPTION不受状态影响，放在超时时间判断的外面
		if (method == RTSP_METHOD_OPTIONS)
		{
			ret = HandleOption(pMsg, pBuf, Len);
			m_Lock.Lock();
			m_CurSessionStatus = RTSP_SESSION_START;
			m_Lock.UnLock();
			break;
		}
		else
		{
			RTSP_DATE_TIME CurTime = RTSPGetCurrTime();
			m_Lock.Lock();
			RTSP_SESSION_STATUS status = m_CurSessionStatus;
			m_Lock.UnLock();
			if (status != RTSP_SESSION_PLAY)
			{
				CurTime -= RTSP_ROUND_TRIP_TIME;
				if (m_LastRequestTime < CurTime)
				{
					printf("%s %s %d : Request Time Out\n", __FILE__, __FUNCTION__, __LINE__);
					CreateRTSPCommonMsg(STATUS_CODE_REQUEST_TIME_OUT, pBuf, Len);
					return -1;
				}
			}
		}

		switch (method)
		{
		case RTSP_METHOD_DESCRIBE:
			{
				ret = HandleDescribe(pMsg, pBuf, Len);
			}
			break;
		case RTSP_METHOD_PLAY:
			{
				ret = HandlePlay(pMsg, pBuf, Len);
				m_Lock.Lock();
				m_CurSessionStatus = RTSP_SESSION_PLAY;
				m_Lock.UnLock();
			}
			break;
		case RTSP_METHOD_SETUP:
			{
				ret = HandleSetup(pMsg, pBuf, Len);
				m_Lock.Lock();
				m_CurSessionStatus = RTSP_SESSION_SETUP;
				m_Lock.UnLock();
			}
			break;
		case RTSP_METHOD_TEARDOWN:
			{
				ret = HandleTeardown(pMsg, pBuf, Len);
				m_Lock.Lock();
				m_CurSessionStatus = RTSP_SESSION_END;
				m_Lock.UnLock();
			}
			break;
		case RTSP_METHOD_GETPARAMETER:
			{
				CreateRTSPCommonMsg(STATUS_CODE_OK, pBuf, Len);
			}
			break;
		default:
			CreateRTSPCommonMsg(STATUS_CODE_NOT_IMPLEMENTED, pBuf, Len);
		}
	} while(0);

	m_LastRequestTime = RTSPGetCurrTime();

	return MsgLen;

}

int CServerRTSP::AnalyseRequestLine(const char *pRTSPMsgHeader)
{
	if (NULL == pRTSPMsgHeader)
	{
		assert(false);
		return -1;
	}

	if (3 != sscanf(pRTSPMsgHeader, "%s %s %s\r\n", m_method, m_rtspURL, m_rtspVersion))
	{
		printf("%s:%s:%d, RTSP request-line error! message header=%s\n", __FUNCTION__, __FILE__, __LINE__, pRTSPMsgHeader);
		assert(false);
		return -1;
	}

	if ('/' == m_rtspURL[strlen(m_rtspURL) - 1])
	{
		m_rtspURL[strlen(m_rtspURL) -1] = '\0';
	}

	//获取需要播放的通道
	unsigned long channel = 0;

	const char *pPos = strstr(m_rtspURL, "/live/");
	if (NULL != pPos)
	{
		pPos += 5;
		if (1 >= strlen(pPos))
		{
			return -1;
		}
		pPos++;
		channel = atoi(pPos);
		if (channel >= MAX_CH_NUM)
		{
			return -1;
		}
		m_Lock.Lock();
		m_Channel = channel;
		m_Lock.UnLock();
	}
	else
	{
		return -1;
	}

	return 0;
}

int CServerRTSP::GetRTSPMethodType(const char *pMethod)
{
	if (NULL == pMethod || 0 == strlen(pMethod))
	{
		assert(false);
		return RTSP_METHOD_END;
	}

	for (int i=0; i<RTSP_METHOD_END; i++)
	{
		if (0 == strcmp(pMethod, RTSP_METHODS[i]))
		{
			return i;
		}
	}
	printf("%s:%s:%d, Rtsp method %s error\n", __FUNCTION__, __FILE__, __LINE__, pMethod);

	return RTSP_METHOD_END;
}

int CServerRTSP::HandleOption(const char *pMsg, char *pBuf, unsigned long &Len)
{
	unsigned long statusCode = STATUS_CODE_OK;

	CreateRTSPOptionsReplayMsg(statusCode, pBuf, Len);
	
	return 0;
}

int CServerRTSP::HandleDescribe(const char *pMsg, char *pBuf, unsigned long &Len)
{
	unsigned long statusCode = STATUS_CODE_OK;
	const char *pPos = strstr(pMsg, "Accept:");
	if (NULL == pPos)
	{
		statusCode = STATUS_CODE_PARAMETER_NOT_UNDERSTOOD;
	}
	pPos += 7;
	pPos = strstr(pPos, "application/sdp");
	if (NULL == pPos)
	{
		statusCode = STATUS_CODE_PARAMETER_NOT_UNDERSTOOD;
	}
	CreateRTSPDescribeReplayMsg(statusCode, pBuf, Len);

	return 0;
}

int CServerRTSP::HandleSetup(const char *pMsg, char *pBuf, unsigned long &Len)
{
	unsigned long statusCode = STATUS_CODE_UNSUPPORTED_TRANSPORT;
	const char *pPos = strstr(pMsg, "Transport:");
	if(NULL != pPos)
	{
		if(NULL != strstr(pPos, "TCP"))
		{
			pPos = strstr(pPos, "interleaved=0-1");
			if (NULL != pPos)
			{
				m_SetupTimes = 1;
				statusCode = STATUS_CODE_OK;
			}
			else
			{
				pPos = strstr(pPos, "interleaved=2-3");
				if (NULL != pPos)
				{
					m_SetupTimes = 2;
					statusCode = STATUS_CODE_OK;
				}
			}
		}
		else//UDP
		{
			pPos = strstr(pPos, "client_port=");
			if (NULL != pPos)
			{
				m_SetupTimes++;
				if (2 == sscanf(pPos, "%*[^=]=%d-%d", &m_UDPPort[m_SetupTimes*2-2], &m_UDPPort[m_SetupTimes*2-1]))
				{
					statusCode = STATUS_CODE_OK;
					m_Lock.Lock();
					m_bIsUDP = true;
					m_Lock.UnLock();
				}
			}
		}
	}
	CreateRTSPSetupReplayMsg(statusCode, pBuf, Len);

	return 0;
}

int CServerRTSP::HandlePlay(const char *pMsg, char *pBuf, unsigned long &Len)
{
	unsigned long statusCode = STATUS_CODE_OK;
	const char *pPos = strstr(pMsg, "Session: ");
	if (NULL != pPos)
	{
		char Session[32] = {0};
		sscanf(pPos, "%*s %[0-9a-zA-Z$_.+-]", Session);
		if (0 != m_SessionID.compare(Session))
		{
			statusCode = STATUS_CODE_PARAMETER_NOT_UNDERSTOOD;
		}
	}
	else
	{
		statusCode = STATUS_CODE_PARAMETER_NOT_UNDERSTOOD;
	}
	//Range时间尚未判断

	CreateRTSPPlayReplayMsg(statusCode, pBuf, Len);

	return 0;
}

int CServerRTSP::HandleTeardown(const char *pMsg, char *pBuf, unsigned long &Len)
{
	unsigned long statusCode = STATUS_CODE_OK;
	const char *pPos = strstr(pMsg, "Session: ");
	if (NULL != pPos)
	{
		char Session[32] = {0};
		sscanf(pPos, "%*s %[0-9a-zA-Z$_.+-]", Session);
		if (0 != m_SessionID.compare(Session))
		{
			statusCode = STATUS_CODE_PARAMETER_NOT_UNDERSTOOD;
		}
	}
	else
	{
		statusCode = STATUS_CODE_PARAMETER_NOT_UNDERSTOOD;
	}
	//Range时间尚未判断

	CreateRTSPTeardownReplayMsg(statusCode, pBuf, Len);

	return 0;
}

bool CServerRTSP::CheckCSeq(const char *pMsg)
{
	const char *pPos = strstr(pMsg, "CSeq:");
	if (NULL == pPos)
	{
		return false;
	}
	unsigned long CSeq = atoi(pPos+5);

	if (0 == m_Cseq)
	{
		m_Cseq = CSeq;
		return true;
	}
	else
	{
		if (CSeq != m_Cseq + 1)
		{
			return false;
		}
	}
	m_Cseq = CSeq;
	return true;
}

void CServerRTSP::CreateRTSPOptionsReplayMsg(unsigned long statusCode, char *pBuf, unsigned long &Len)
{
	string status = CRTSPStatusCode::Instance()->GetStatusCodeReason(statusCode);

	sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\nPublic: %s\r\n\r\n\0",statusCode, status.c_str(), m_Cseq, RTSP_PUBLIC);
	Len = strlen(pBuf);
}

void CServerRTSP::CreateRTSPDescribeReplayMsg(unsigned long statusCode, char *pBuf, unsigned long &Len)
{
	string status = CRTSPStatusCode::Instance()->GetStatusCodeReason(statusCode);
	if (STATUS_CODE_OK != statusCode)
	{	
		sprintf(pBuf, "RTSP/1.0 %d %s\r\nServer: tvt.dvr.server\r\nCSeq: %d\r\n\r\n\0",statusCode, status.c_str(), m_Cseq);
	}
	else
	{
		char sdpMsg_Audio[512] = {0};
		char sdpMsg_Video[512] = {0};
		char OtherSDPInfo[512] = {0};
		char tmpBuf[256] = {0};
		sprintf(OtherSDPInfo,
				"v=0\r\n"
				//"o=- 0 0 IN IP4 192.168.3.201\r\n"
				"s=SDP\r\n"
				"t=0 0\r\n"
			);

		if (m_VideoType >=0 )
		{
				sprintf(sdpMsg_Video,
					"m=video %d RTP/AVP 98\r\n"
					"a=rtpmap:98 h264/90000\r\n"
					"a=control:trackID=2\r\n"
					"a=framerate:%.01f\r\n"
					, m_UDPServerPort, m_FrameRate
					);
		}
		if(m_AudioType >=0 )
		{
			sprintf(sdpMsg_Audio,
				"m=audio %d RTP/AVP 8\r\n"
				"a=rtpmap:8 PCMA/8000\r\n"
				"a=control:trackID=1\r\n"
				,m_UDPServerPort
				);
		}

		sprintf(tmpBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\nContent-Type: application/sdp\r\nContent-Length: \0"\
			,statusCode, status.c_str(), m_Cseq);
		unsigned long ContentLen = strlen(sdpMsg_Video) + strlen(sdpMsg_Audio) + strlen(OtherSDPInfo);
	
		sprintf(pBuf, "%s%d\r\n\r\n%s%s%s\0", tmpBuf, ContentLen, OtherSDPInfo, sdpMsg_Video, sdpMsg_Audio);
	}
	Len = strlen(pBuf);
}

void CServerRTSP::CreateRTSPSetupReplayMsg(unsigned long statusCode, char *pBuf, unsigned long &Len)
{
	string status = CRTSPStatusCode::Instance()->GetStatusCodeReason(statusCode);
	if (STATUS_CODE_OK != statusCode)
	{
		sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\n\r\n\0", statusCode, status.c_str(), m_Cseq);
	}
	else
	{
		char Date[64] = {0};
		tm time = RTSPGetCurrTime().TM();
		sprintf(Date, "%s, %02d %s %4d %02d:%02d:%02d GMT", \
			RTSP_DATE_DAY_ABBR[time.tm_wday], time.tm_mday, RTSP_DATE_MONTH_ABBR[time.tm_mon],\
			time.tm_year+1900, time.tm_hour, time.tm_min, time.tm_sec);
		m_Lock.Lock();
		if (m_bIsUDP)
		{
			m_Lock.UnLock();
			if (1 <= m_SetupTimes && m_SetupTimes <= 2)
			{
				sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\nDate: %s\r\nExpires: %s\r\nSession: %s;timeout=%d\r\nTransport: RTP/AVP;unicast;client_port=%d-%d;server_port=%d-%d\r\n\r\n\0"\
					, statusCode, status.c_str(), m_Cseq, Date, Date, m_SessionID.c_str(), m_TimeOut\
					, m_UDPPort[m_SetupTimes*2-2], m_UDPPort[m_SetupTimes*2-2]+1, m_UDPServerPort+2*m_SetupTimes-2, m_UDPServerPort+2*m_SetupTimes-2+1);
			}
			else
			{
				assert(false);
				status = CRTSPStatusCode::Instance()->GetStatusCodeReason(STATUS_CODE_PARAMETER_NOT_UNDERSTOOD);
				sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\n\r\n\0", STATUS_CODE_PARAMETER_NOT_UNDERSTOOD, status.c_str(), m_Cseq);
			}
		}
		else
		{
			m_Lock.UnLock();
			if (1 == m_SetupTimes)
			{
				sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\nDate: %s\r\nExpires: %s\r\nSession: %s;timeout=%d\r\nTransport: RTP/AVP/TCP;unicast;interleaved=0-1\r\n\r\n\0"\
					, statusCode, status.c_str(), m_Cseq, Date, Date, m_SessionID.c_str(), m_TimeOut);
			}
			else if(2 == m_SetupTimes)
			{
				sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\nDate: %s\r\nExpires: %s\r\nTransport: RTP/AVP/TCP;unicast;interleaved=2-3\r\nSession: %s;timeout=%d\r\n\r\n\0", \
					statusCode, status.c_str(), m_Cseq, Date, Date, m_SessionID.c_str(), m_TimeOut);
			}
			else
			{
				assert(false);
				status = CRTSPStatusCode::Instance()->GetStatusCodeReason(STATUS_CODE_PARAMETER_NOT_UNDERSTOOD);
				sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\n\r\n\0", STATUS_CODE_PARAMETER_NOT_UNDERSTOOD, status.c_str(), m_Cseq);
			}
		}
	}
	Len = strlen(pBuf);
}

void CServerRTSP::CreateRTSPPlayReplayMsg(unsigned long statusCode, char *pBuf, unsigned long &Len)
{
	string status = CRTSPStatusCode::Instance()->GetStatusCodeReason(statusCode);
	if (STATUS_CODE_OK != statusCode)
	{
		sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\n\r\n\0", statusCode, status.c_str(), m_Cseq);
	}
	else
	{
		char Date[64] = {0};
		tm time = RTSPGetCurrTime().TM();
		sprintf(Date, "%s, %02d %s %4d %02d:%02d:%02d GMT", \
			RTSP_DATE_DAY_ABBR[time.tm_wday], time.tm_mday, RTSP_DATE_MONTH_ABBR[time.tm_mon],\
			time.tm_year+1900, time.tm_hour, time.tm_min, time.tm_sec);
		sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\nDate: %s\r\nExpires: %s\r\nSession: %s;timeout=%d\r\n\r\n\0",\
			statusCode, status.c_str(), m_Cseq, Date, Date, m_SessionID.c_str(), m_TimeOut);
	}
	Len = strlen(pBuf);
}

void CServerRTSP::CreateRTSPTeardownReplayMsg(unsigned long statusCode, char *pBuf, unsigned long &Len)
{
	string status = CRTSPStatusCode::Instance()->GetStatusCodeReason(statusCode);
	sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\nSession: %s;timeout=%d\r\n\r\n\0",\
		statusCode, status.c_str(), m_Cseq, m_SessionID.c_str(), m_TimeOut);
	Len = strlen(pBuf);
}

void CServerRTSP::CreateRTSPCommonMsg(unsigned long statusCode, char *pBuf, unsigned long &Len)
{
	string status = CRTSPStatusCode::Instance()->GetStatusCodeReason(statusCode);

	char Date[64] = {0};
	tm time = RTSPGetCurrTime().TM();
	sprintf(Date, "%s, %02d %s %4d %02d:%02d:%02d GMT", \
		RTSP_DATE_DAY_ABBR[time.tm_wday], time.tm_mday, RTSP_DATE_MONTH_ABBR[time.tm_mon],\
		time.tm_year+1900, time.tm_hour, time.tm_min, time.tm_sec);

	sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\nDate: %s\r\nExpires: %s\r\nSession: %s\r\n\r\n\0",\
		statusCode, status.c_str(), m_Cseq, Date, Date, m_SessionID.c_str());
	Len = strlen(pBuf);
}

int CServerRTSP::GetStatus()
{
	int ret;
	m_Lock.Lock();
	ret = m_CurSessionStatus;
	m_Lock.UnLock();
	return ret;
}

int CServerRTSP::GetChannel()
{
	int ret;
	m_Lock.Lock();
	ret = m_Channel;
	m_Lock.UnLock();
	return ret;
}


//RFC 2617    3.2.1 
void CServerRTSP::NonceGenerator()
{
	CBase64 Base64Obj;

	char szSrc[32] = {0};
	char szDest[36] = {0};
	RTSP_LOCAL_TIME localTime = RTSPGetLocalTime();
	sprintf(szSrc, "%04d%02d%02d%02d%02d%02d%03dL", localTime.year, localTime.month, localTime.mday, \
		localTime.hour, localTime.minute, localTime.second, localTime.microsecond/1000);
	Base64Obj.Encode((unsigned char *)szSrc, strlen(szSrc), szDest, 36);
	strcpy(szSrc, szDest);
	Base64Obj.Encode((unsigned char *)szSrc, strlen(szSrc), szDest, 36);
	//strlwr(szDest);

	strncpy(m_CurrNonce, szDest, 32);
}


bool CServerRTSP::CheckAuthentication(const char *pMsg, const char *pMethod)
{
	if (m_CurrNonce[0] == '\0')
	{
		return false;
	}

	if (0 == strcmp(pMethod, "OPTIONS"))
	{
		char *pPos = strstr((char *)pMsg, "Authorization:");
		if (NULL == pPos)
		{
			printf("%s %s %d : Authorization do not found\n", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}

		pPos = strstr(pPos, "username=");
		if (NULL == pPos)
		{
			printf("%s %s %d : username do not found\n", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}

		memset(m_CurUserName, 0, sizeof(m_CurUserName));
		if (1 != sscanf(pPos, "%*[^\"]\"%[^\"]", m_CurUserName))
		{
			printf("%s %s %d : username do not found\n", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}
		
// 		memset(m_CurPasswd, 0, sizeof(m_CurPasswd));
// 		if (!FindAccount(m_CurUserName, m_CurPasswd))
// 		{
// 			printf("%s %s %d : account do not found\n", __FILE__, __FUNCTION__, __LINE__);
// 			return false;
// 		}

		HASHHEX nonce = {0};
		pPos = strstr((char *)pMsg, "nonce=");
		if (NULL == pPos)
		{
			printf("%s %s %d : nonce do not found\n", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}
		if (1 != sscanf(pPos, "%*[^\"]\"%[^\"]", nonce))
		{
			printf("%s %s %d : nonce do not found\n", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}
		if (0 != strcmp(nonce, m_CurrNonce))
		{
			printf("%s %s %d : nonce do not match\n", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}
	}

//get client response
	HASHHEX client_response;
	HASHHEX response;
	char *pPos = strstr((char *)pMsg, "response=");
	if (NULL == pPos)
	{
		printf("%s %s %d : response do not found\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	if (1 != sscanf(pPos, "%*[^\"]\"%[^\"]", client_response))
	{
		printf("%s %s %d : response do not found\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}

//check uri
	pPos = strstr((char *)pMsg, "uri=");
	if (NULL == pPos)
	{
		printf("%s %s %d : uri do not found\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	char URI[128] = {0};
	if (1 != sscanf(pPos, "%*[^\"]\"%[^\"]", URI))
	{
		printf("%s %s %d : uri do not found\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}

//generate response
	CHttpDigestAuth HttpDigestAuthBoj;
	HttpDigestAuthBoj.SetDigestAuth(URI, m_CurUserName, m_CurPasswd, m_CurrNonce, WWW_AUTHENTICATION_REALM, "", "md5");
	if (!HttpDigestAuthBoj.GetResponse(pMethod, response))
	{
		return false;
	}

//check response
	if (0 != strcmp(client_response, response))
	{
		printf("%s %s %d : response do not match\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	

	return true;
}

void CServerRTSP::CreateAuthenticationMsg(char *pBuf, unsigned long &Len)
{
	string status = CRTSPStatusCode::Instance()->GetStatusCodeReason(STATUS_CODE_UNAUTHORIZED);
	NonceGenerator();

	char Date[64] = {0};
	tm time = RTSPGetCurrTime().TM();
	sprintf(Date, "%s, %02d %s %4d %02d:%02d:%02d GMT", \
		RTSP_DATE_DAY_ABBR[time.tm_wday], time.tm_mday, RTSP_DATE_MONTH_ABBR[time.tm_mon],\
		time.tm_year+1900, time.tm_hour, time.tm_min, time.tm_sec);

	char Authenticate[128] = {0};
	sprintf(Authenticate, "WWW-Authenticate: Digest realm=\"%s\", nonce=\"%s\"",\
		WWW_AUTHENTICATION_REALM, m_CurrNonce);

	sprintf(pBuf, "RTSP/1.0 %d %s\r\nCSeq: %d\r\nDate: %s\r\nExpires: %s\r\n%s\r\n\r\n\0",\
		STATUS_CODE_UNAUTHORIZED, status.c_str(), m_Cseq, Date, Date, Authenticate);
	
	Len = strlen(pBuf);
}

// bool CServerRTSP::FindAccount(const char *pUserName, char *pPassWd)
// {
// 	TVT_CONFIG_ITEM_ID ID = TVT_CONFIG_ITEM_ACCOUNT;
// 	unsigned char *pDataBlock = NULL;
// 	unsigned long dataLen = 0;
// 	bool bExist = false;
// 	int i=0;
// 
// 	setup_Get(&ID, 1, &pDataBlock, dataLen);
// 	if(NULL != pDataBlock)
// 	{
// 		TVT_ACCOUNT_CONFIG *pConf = (TVT_ACCOUNT_CONFIG*)(pDataBlock + sizeof(TVT_CONFIG_ITEM_HEAD) + sizeof(TVT_CONFIG_BLOCK_HEAD));
// 		for (; i<pConf->supportAccountNum ;i++)
// 		{
// 			if (0 == strcmp(pConf->user[i].name, pUserName))
// 			{
// 				strcpy(pPassWd, pConf->user[i].password);
// 				bExist = true;
// 				break;
// 			}
// 		}
// 		setup_Release(&pDataBlock);
// 
// 		if (!bExist)
// 		{
// 			return false;
// 		}
// 
// 		return true;
// 	}
// 	
// 	return false;
// }

unsigned short CServerRTSP::GetUDPServerPort()
{
	return m_UDPServerPort;
}

unsigned short *CServerRTSP::GetUDPClientPort() 
{
	return m_UDPPort;
}

bool CServerRTSP::IsTransOverUDP() 
{
	bool ret;
	m_Lock.Lock();
	ret = m_bIsUDP;
	m_Lock.UnLock();
	return ret;
}

int CServerRTSP::GetSetUpCount() 
{
	return m_SetupTimes;
}