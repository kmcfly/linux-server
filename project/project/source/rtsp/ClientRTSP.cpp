#include "ClientRTSP.h"

#define RTP_GET_PARAM_TIME_OUT_CNT 60

CClientRTSP::CClientRTSP()
{
	m_pRecvBuf = new char[RTP_CLIENT_RTSP_RECV_LEN];
	assert(NULL != m_pRecvBuf);
	memset(m_pRecvBuf, 0, RTP_CLIENT_RTSP_RECV_LEN);

	m_pSendBuf = new char[RTP_CLIENT_RTSP_SEND_LEN];
	assert(NULL != m_pSendBuf);
	memset(m_pSendBuf, 0, RTP_CLIENT_RTSP_SEND_LEN);

	m_agentDes = "Co.,Ltd";

	ResetObject();

}

CClientRTSP::~CClientRTSP()
{
	if (NULL != m_pRecvBuf)
	{
		delete []m_pRecvBuf;
		m_pRecvBuf = NULL;
	}

	if (NULL != m_pSendBuf)
	{
		delete []m_pSendBuf;
		m_pSendBuf = NULL;
	}
}

bool CClientRTSP::ResetObject(void)
{
	m_recvDataLen = 0;
	m_SendDataLen = 0;

	memset(m_ServerSupportMethod, 0, sizeof(m_ServerSupportMethod));
	memset(m_ContentType, 0, sizeof(m_ContentType));
	m_ContentLen = 0;

	m_cseq = 0;
	m_ncount = 0;
	m_Authenticate = AUTHENTICATE_NONE;
	m_bISUnicast = true;
	m_bISTCPSession = true;
	m_bIsHasAudio = false;
	m_bIsHasVideo = false;
	m_SendingMethod = RTSP_METHOD_TEARDOWN;
	m_CurStatus	= RTSP_SESSION_END;
	m_bIsSending = false;
	m_interleaveChannel = 0;
	m_PacketizationMode = 0;
	m_RequestTime = 0;
	m_TimeOut = RTP_GET_PARAM_TIME_OUT_CNT;//default

	for( int i = 0; i < SDP_MEDIA_STRAM_END; i++ )
	{
		m_MediaPayloadType[i] = (unsigned int)-1;
	}

	m_AudioDataType = (unsigned int)-1;

	m_chnnel_number = -1;

	m_bOpenAudioData = false;

	m_ulLastError = 0;
	m_run_lines = 0;

	m_pGetIpcUsedInfo = NULL;
	m_pGetIpcUsedParam = NULL;

	m_bSupportGetParamFn = false;


	m_SDPPaerser.ResetObject();
	m_HttpDigestAuth.ResetObject();

	return true;
}

int CClientRTSP::HandleData(const char *pData, unsigned long len)
{
	int ret;
	
	m_Lock.Lock();
	ret = HandleUnLockData(pData,len);
	m_Lock.UnLock();

	return ret;
}
int CClientRTSP::HandleUnLockData(const char *pData, unsigned long len)
{
	int ret = 0;
	if (NULL == pData || len > RTP_CLIENT_RTSP_RECV_LEN)
	{
		printf("%s %d err\n", __FILE__, __LINE__);
		return -1;
	}

	if( len >= RTP_CLIENT_RTSP_RECV_LEN )
	{
		len = RTP_CLIENT_RTSP_RECV_LEN -1;
	}

	m_bIsSending = false;
	memset(m_pRecvBuf, 0, RTP_CLIENT_RTSP_RECV_LEN);
	memcpy(m_pRecvBuf, pData, len);

	//printf("\r\n%s\r\n", m_pRecvBuf);

	ret = ParseData(m_pRecvBuf);
	if (ret < 0)
	{
		//printf("%s %d err\n", __FILE__, __LINE__);
		return -1;
	}
	else if (STATUS_CODE_UNAUTHORIZED == ret)
	{
		if (0 == m_mode.compare(RTSP_AUTHENTICATE_DIGEST))
		{
			m_Authenticate = AUTHENTICATE_DIGEST;
			//if (m_serverURL[0] == '\0'|| m_UserName[0] == '\0' || m_PassWd[0] == '\0' || m_nonce[0] == '\0' || m_realm[0] == '\0')
			//密码可以为空？
			if (m_serverURL[0] == '\0'|| m_UserName[0] == '\0' || m_nonce[0] == '\0' || m_realm[0] == '\0')
			{
				//printf("%s %d err\n", __FILE__, __LINE__);
				return -1;
			}
			m_HttpDigestAuth.SetDigestAuth(m_serverURL.c_str(), m_UserName.c_str(), m_PassWd.c_str(), m_nonce.c_str(), m_realm.c_str(), "", "md5");
		}
		else if (0 == m_mode.compare(RTSP_AUTHENTICATE_BasicT))
		{
			m_Authenticate = AUTHENTICATE_BASIC;
		}
		else
		{
			//使用明文的请求地址
			do 
			{
				if (-1 == m_serverURL.find("@"))
				{
					if (!m_UserName.empty() && !m_PassWd.empty())
					{
						int pos = m_serverURL.find("rtsp://");
						if (-1 != pos)
						{
							m_serverURL.insert(pos+7, m_UserName.c_str());
							m_serverURL.insert(pos+7+m_UserName.length(), ":");
							m_serverURL.insert(pos+7+m_UserName.length()+1, m_PassWd.c_str());
							m_serverURL.insert(pos+7+m_UserName.length()+1+m_PassWd.length(), "@");
							break;
						}
					}
				}
				//printf("%s %d UNAUTHORIZED\n", __FILE__, __LINE__);
				return -1;
			} while(0);
		}
		if (m_SendingMethod != RTSP_METHOD_PLAY)
		{
			m_SendingMethod -= 1;//上一次命令要重发
		}
	}

	memset(m_pSendBuf, 0, RTP_CLIENT_RTSP_SEND_LEN);
	m_SendDataLen = 0;

	switch(m_SendingMethod)
	{
	case RTSP_METHOD_OPTIONS:
		CreateDescribe();
		m_CurStatus = RTSP_SESSION_START;
		m_SendingMethod = RTSP_METHOD_DESCRIBE;
		break;
	case RTSP_METHOD_DESCRIBE:
		if (!m_SetupRequest[SDP_VIDEO_STREAM_POS].empty())
		{
			CreateSetup(STREAM_LIVE_VIDEO);
			m_SetupRequest[SDP_VIDEO_STREAM_POS].clear();
			m_SendingMethod = RTSP_METHOD_SETUP;
		}
		else if( m_bOpenAudioData ) 
		{
			if (!m_SetupRequest[SDP_AUDIO_STREAM_POS].empty())
			{
				CreateSetup(STREAM_LIVE_AUDIO);
				m_SendingMethod = RTSP_METHOD_SETUP;
				m_SetupRequest[SDP_AUDIO_STREAM_POS].clear();
			}
		}
		else
		{
			//printf("%s %d err", __FILE__, __LINE__);
			return -1;
		}
		break;
	case RTSP_METHOD_SETUP:
		if (!m_SetupRequest[SDP_VIDEO_STREAM_POS].empty())
		{
			CreateSetup(STREAM_LIVE_VIDEO);
			m_SetupRequest[SDP_VIDEO_STREAM_POS].clear();
			break;
		}
		else if( m_bOpenAudioData )
		{
			if (!m_SetupRequest[SDP_AUDIO_STREAM_POS].empty())
			{
				CreateSetup(STREAM_LIVE_AUDIO);
				m_SetupRequest[SDP_AUDIO_STREAM_POS].clear();
				break;
			}
		}
		CreatePlay();
		m_SendingMethod = RTSP_METHOD_PLAY;
	    break;
	case RTSP_METHOD_PLAY:
		//if (IsSupportGetParameter())
		//{
		//	CreateGetParameter();
		//}
		//m_SendingMethod = RTSP_METHOD_GET_PARAM;
		m_CurStatus = RTSP_SESSION_PLAY;
	    break;
	case RTSP_METHOD_TEARDOWN:
		m_CurStatus = RTSP_SESSION_TEARDOWN;
		break;

	default:
		CreateOptions(m_serverURL.c_str());

	    break;
	}
	return 0;
}


int CClientRTSP::GetVideoPayloadType()
{
	return m_MediaPayloadType[SDP_VIDEO_STREAM_POS];
}

int CClientRTSP::GetAudioPayloadType()
{
	return m_MediaPayloadType[SDP_AUDIO_STREAM_POS];
}

int CClientRTSP::GetAudioDataType()
{
	return m_AudioDataType;
}


int CClientRTSP::ParseData(char *pData)
{
	if (NULL == pData)
	{
		return -1;
	}
	bool bHasSDP = false;
	int ret = 0;
	int statusCode = 0;
	char answer[1024] = {0};
	char *pDataPos = pData;
	char *pDataPosEnd = pData;

	statusCode = GetStatusCode(pDataPos);

	while(NULL != pDataPosEnd && '\0' != *pDataPosEnd)
	{
		memset(answer, 0, sizeof(answer));
		pDataPos = pDataPosEnd;
		pDataPosEnd = strstr(pDataPos, "\r\n");
		if (NULL == pDataPosEnd)
		{
			//printf("%s %d err\n", __FILE__, __LINE__);
			break;
		}
		pDataPosEnd += 2;
		strncpy(answer, pDataPos, pDataPosEnd-pDataPos);

		if (!strncasecmp(answer, "CSeq:", 5))
		{
			int answerSeq = 0;
			sscanf(answer, "%*s %d", &answerSeq);
			if (answerSeq != m_cseq)
			{
				//printf("%s:%s:%d,request seq=%d, answer seq=%d\n", __FUNCTION__, __FILE__, __LINE__, m_cseq, answerSeq);
				//printf("%s %d err\n", __FILE__, __LINE__);
				return -1;
			}
		}

		if (!strncasecmp(answer, "Server:", 7))
		{
		}

		if (!strncasecmp(answer, "Session:", 8))
		{
			char Session[64] = {0};
			unsigned int timeout = 60;
			sscanf(answer, "%*s %[0-9a-zA-Z$_.+-]", Session);
			m_sessionID = Session;

			char *pPos = answer + 8;
			pPos = strstr(pPos, "timeout=");
			if (NULL != pPos)
			{
				timeout = atoi(pPos+8);
				if (m_TimeOut != timeout)
				{
					if( timeout > RTP_GET_PARAM_TIME_OUT_CNT )
					{
						m_TimeOut = timeout;
					}
					else
					{
						m_TimeOut = RTP_GET_PARAM_TIME_OUT_CNT;
					}
				}
			}
		}

		if (!strncasecmp(answer, "Public:", 7))
		{
			int i = 0;
			char *pPos = answer + 7;
			char *pPosEnd = NULL;
			
			m_bSupportGetParamFn = false;
			if ( strstr(pPos, "GET_PARAMETER") )
			{
				m_bSupportGetParamFn = true;
			}

			while(NULL != pPos)
			{
				pPos += 1;
				pPosEnd = strstr(pPos, ",");
				if (NULL == pPosEnd)
				{
					pPosEnd = strstr(answer, "\r\n");
					if (NULL == pPosEnd)
					{
						printf("%s %d err\n", __FILE__, __LINE__);
						return -1;
					}
				}
				strncpy(m_ServerSupportMethod[i++], pPos, pPosEnd - pPos);

				pPos = strstr(pPosEnd, " ");

			}
			
		}

		if (!strncasecmp(answer, "Content-Type: ", 14))
		{
			sscanf(answer, "%*s %[0-9a-zA-Z/$_.+-]", m_ContentType);
			if (!strncasecmp(m_ContentType, "application/sdp", 15))
			{
				bHasSDP = true;
			}
			else
			{
				printf("%s %d err\n", __FILE__, __LINE__);
			}
		}

		if (!strncasecmp(answer, "Content-Length: ", 16))
		{
			sscanf(answer, "%*s %d", &m_ContentLen);
		}

		//added  
		if (!strncasecmp(answer, "WWW-Authenticate: ", 18))
		{
			do 
			{
				char *pPos = answer + 18;
				char *pPosEnd = NULL;
				pPosEnd = strstr(pPos, " ");
				if (NULL == pPosEnd)
				{
					break;
				}
				m_mode.assign(pPos, pPosEnd-pPos);

				pPos = strstr(answer, "realm=");
				if (NULL == pPos)
				{
					break;
				}
				pPos += 6 + 1;
				pPosEnd = strstr(pPos, "\"");
				if (NULL == pPosEnd)
				{
					break;
				}
				m_realm.assign(pPos, pPosEnd-pPos);
				pPos = strstr(answer, "nonce=");
				if (NULL == pPos)
				{
					break;
				}
				pPos += 6 + 1;
				pPosEnd = strstr(pPos, "\"");
				if (NULL == pPosEnd)
				{
					break;
				}
				m_nonce.assign(pPos, pPosEnd-pPos);
				
			} while(0);
		}

		if (bHasSDP)
		{
			if (!strncasecmp(answer, "\r\n", 2))
			{
				bHasSDP = false;
				m_SDPPaerser.Initial(pDataPosEnd);
				m_PacketizationMode = m_SDPPaerser.GetPacketicationMode();
				for (int i=0; i<m_SDPPaerser.GetMediaCount(); i++)
				{
					//for test 实际上只支持H264 和 G711
					if (m_SDPPaerser.GetMediaType(i) == STREAM_LIVE_VIDEO)
					{
						m_bIsHasVideo = true;
						m_SetupRequest[SDP_VIDEO_STREAM_POS] = m_SDPPaerser.GetMediaURL(i);
						//如果给的地址只是部分地址，需要另作处理
						if (m_SetupRequest[SDP_VIDEO_STREAM_POS].length() < m_serverURL.length())
						{
							std::string tmp = m_serverURL;
							m_SetupRequest[SDP_VIDEO_STREAM_POS] = tmp + "/" + m_SetupRequest[SDP_VIDEO_STREAM_POS];
						}

						m_MediaPayloadType[SDP_VIDEO_STREAM_POS] = m_SDPPaerser.GetMediaPayload(i);
					}
					else if(m_SDPPaerser.GetMediaType(i) == STREAM_LIVE_AUDIO)
					{
						m_bIsHasAudio = true;
						m_SetupRequest[SDP_AUDIO_STREAM_POS] = m_SDPPaerser.GetMediaURL(i);
						//如果给的地址只是部分地址，需要另作处理
						if (m_SetupRequest[SDP_AUDIO_STREAM_POS].length() < m_serverURL.length())
						{
							std::string tmp = m_serverURL;
							m_SetupRequest[SDP_AUDIO_STREAM_POS] = tmp + "/" +m_SetupRequest[SDP_AUDIO_STREAM_POS];
						}
						m_MediaPayloadType[SDP_AUDIO_STREAM_POS] = m_SDPPaerser.GetMediaPayload(i);

						m_AudioDataType = m_SDPPaerser.GetMediaAudioType(i);
					}
				}
			}
		}//end of bHasSDP
	}

	return statusCode;
}

int CClientRTSP::CreateFirstCommand(const char *pURL, const char *pUserName, const char *pPassWd)
{
	int ret;

	m_RequestTime = 0;
	ret = CreateOptions( pURL, pUserName, pPassWd );

	return ret;
}

int CClientRTSP::CreateOptions(const char *pURL, const char *pUserName, const char *pPassWd)
{
	if (NULL == pURL)
	{
		return -1;
	}
	if (NULL != pUserName && NULL != pPassWd)
	{
		m_UserName.assign(pUserName);
		m_PassWd.assign(pPassWd);
	}

	m_cseq ++;
	memset(m_pSendBuf, 0, RTP_CLIENT_RTSP_SEND_LEN);
	m_serverURL.assign(pURL);

	if (++m_RequestTime > 20)
	{
		printf("current m_RequestTime = %d %s %d err\n", m_RequestTime, __FILE__, __LINE__);
		m_RequestTime = 0;
		return -1;
	}
	char Authorization[512] = {0};

	if (m_Authenticate == AUTHENTICATE_DIGEST)
	{
		HASHHEX response;
		if (!m_HttpDigestAuth.GetResponse("OPTIONS", response))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}

		snprintf(Authorization, sizeof(Authorization), "%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"OPTIONS %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\n\r\n",
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str());
		
	}
	else if(m_Authenticate == AUTHENTICATE_BASIC)
	{
		if (!m_HttpDigestAuth.GetBasicCredentials(m_UserName.c_str(), m_PassWd.c_str(), Authorization))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"OPTIONS %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: Basic %s\r\nUser-Agent: %s\r\n\r\n",
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str());
	}
	else
	{
		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"OPTIONS %s RTSP/1.0\r\nCSeq: %d\r\nUser-Agent: %s\r\n\r\n",
			m_serverURL.c_str(), m_cseq, m_agentDes.c_str());
	}

	m_SendingMethod = RTSP_METHOD_OPTIONS;

	return 0;
}

int CClientRTSP::CreateDescribe()
{
	m_cseq ++;
	char Authorization[512] = {0};

	if (m_Authenticate == AUTHENTICATE_DIGEST)
	{
		HASHHEX response;
		if (!m_HttpDigestAuth.GetResponse("DESCRIBE", response))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		snprintf(Authorization,  sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"DESCRIBE %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nAccept: application/sdp\r\n\r\n",
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str());
	}
	else if(m_Authenticate == AUTHENTICATE_BASIC)
	{
		if (!m_HttpDigestAuth.GetBasicCredentials(m_UserName.c_str(), m_PassWd.c_str(), Authorization))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"DESCRIBE %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: Basic %s\r\nUser-Agent: %s\r\nAccept: application/sdp\r\n\r\n",
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str());
	}
	else
	{
		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"DESCRIBE %s RTSP/1.0\r\nCSeq: %d\r\nUser-Agent: %s\r\nAccept: application/sdp\r\n\r\n",
			m_serverURL.c_str(), m_cseq, m_agentDes.c_str());
	}

	return 0;
}

int CClientRTSP::CreateSetup(STREAM_TYPE streamType)
{
	m_cseq ++;
	char requestURL[1024] = {0};
	char transportInfo[1024] = {0};

	if(streamType == STREAM_LIVE_VIDEO)
	{
		if (m_SetupRequest[SDP_VIDEO_STREAM_POS].empty())
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		strcpy(requestURL, m_SetupRequest[SDP_VIDEO_STREAM_POS].c_str());
	}
	else if (streamType == STREAM_LIVE_AUDIO)
	{
		if (m_SetupRequest[SDP_AUDIO_STREAM_POS].empty())
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		strcpy(requestURL, m_SetupRequest[SDP_AUDIO_STREAM_POS].c_str());
	}
	else
	{
		printf("%s %d err\n", __FILE__, __LINE__);
		return -1;
	}

	if (0 != GetTransportInfo(transportInfo, sizeof(transportInfo)))
	{
		return -1;
	}

	char Authorization[512] = {0};
	if (m_Authenticate == AUTHENTICATE_DIGEST)
	{
		HASHHEX response;
		if (!m_HttpDigestAuth.GetResponse("SETUP", response))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		snprintf(Authorization, sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"SETUP %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nTransport: %s\r\n",
			requestURL, m_cseq, Authorization, m_agentDes.c_str(), transportInfo);
	}
	else if(m_Authenticate == AUTHENTICATE_BASIC)
	{
		if (!m_HttpDigestAuth.GetBasicCredentials(m_UserName.c_str(), m_PassWd.c_str(), Authorization))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"SETUP %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: Basic %s\r\nUser-Agent: %s\r\nTransport: %s\r\n",
			requestURL, m_cseq, Authorization, m_agentDes.c_str(), transportInfo);
	}
	else
	{
		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"SETUP %s RTSP/1.0\r\nCSeq: %d\r\nUser-Agent: %s\r\nTransport: %s\r\n",
			requestURL, m_cseq, m_agentDes.c_str(), transportInfo);
	}

	if (!m_sessionID.empty())
	{
		char pSessionID[32];
		snprintf(pSessionID, sizeof(pSessionID), "Session: %s\r\n", m_sessionID.c_str());
		strcat(m_pSendBuf, pSessionID);

	}
	strcat(m_pSendBuf, "\r\n");

	return 0;
}

int CClientRTSP::CreateSetparameter()
{
	m_cseq ++;

	snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
		"OPTIONS %s RTSP/1.0\r\nCSeq: %d\r\nUser-Agent: %s\r\n\r\n",
		m_serverURL.c_str(), m_cseq, m_agentDes.c_str());

	return 0;
}

int CClientRTSP::CreatePlay()
{
	m_cseq ++;
	char Authorization[512] = {0};

	if (m_Authenticate == AUTHENTICATE_DIGEST)
	{
		HASHHEX response;
		if (!m_HttpDigestAuth.GetResponse("PLAY", response))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		snprintf(Authorization,  sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nSession: %s\r\n",\
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str(), m_sessionID.c_str());
	}
	else if(m_Authenticate == AUTHENTICATE_BASIC)
	{
		if (!m_HttpDigestAuth.GetBasicCredentials(m_UserName.c_str(), m_PassWd.c_str(), Authorization))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: Basic %s\r\nUser-Agent: %s\r\nSession: %s\r\n",\
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str(), m_sessionID.c_str());
	}
	else
	{
		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nUser-Agent: %s\r\nSession: %s\r\n",\
			m_serverURL.c_str(), m_cseq, m_agentDes.c_str(), m_sessionID.c_str());
	}
	strcat(m_pSendBuf, "Range: npt=0.000-\r\n\r\n");//默认一直播


	return 0;
}

int CClientRTSP::CreateTeardown(char *pData)
{
	if (NULL == pData)
	{
		return -1;
	}
	m_cseq ++;
	char Authorization[512] = {0};

	memset( Authorization, 0, 512 );
	if (m_Authenticate == AUTHENTICATE_DIGEST)
	{
		HASHHEX response;
		if (!m_HttpDigestAuth.GetResponse("TEARDOWN", response))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		snprintf(Authorization,  sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(pData, RTP_CLIENT_RTSP_SEND_LEN, 
			"TEARDOWN %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nSession: %s\r\n\r\n",
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str(), m_sessionID.c_str());

	}
	else if(m_Authenticate == AUTHENTICATE_BASIC)
	{
		if (!m_HttpDigestAuth.GetBasicCredentials(m_UserName.c_str(), m_PassWd.c_str(), Authorization))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		snprintf(pData, RTP_CLIENT_RTSP_SEND_LEN, 
			"TEARDOWN %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: Basic %s\r\nUser-Agent: %s\r\nSession: %s\r\n\r\n",
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str(), m_sessionID.c_str());
	}
	else
	{
		snprintf(pData, RTP_CLIENT_RTSP_SEND_LEN, 
			"TEARDOWN %s RTSP/1.0\r\nCSeq: %d\r\nUser-Agent: %s\r\nSession: %s\r\n\r\n",
			m_serverURL.c_str(), m_cseq, m_agentDes.c_str(), m_sessionID.c_str());
	}

	//printf( "cuurent channel rtsp teardown = %d,%d---%s\n", m_chnnel_number, m_sub_chn_num, pData );

	return 0;
}

int CClientRTSP::CreateTeardown()
{
	m_cseq ++;
	char Authorization[512] = {0};

	if (m_Authenticate == AUTHENTICATE_DIGEST)
	{
		HASHHEX response;
		if (!m_HttpDigestAuth.GetResponse("TEARDOWN", response))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		snprintf(Authorization,  sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"TEARDOWN %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nSession: %d\r\n\r\n",
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str(), m_sessionID.c_str());
	}
	else if(m_Authenticate == AUTHENTICATE_BASIC)
	{
		if (!m_HttpDigestAuth.GetBasicCredentials(m_UserName.c_str(), m_PassWd.c_str(), Authorization))
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return -1;
		}
		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"TEARDOWN %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: Basic %s\r\nUser-Agent: %s\r\nSession: %d\r\n\r\n",
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str(), m_sessionID.c_str());
	}
	else
	{
		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"TEARDOWN %s RTSP/1.0\r\nCSeq: %d\r\nUser-Agent: %s\r\nSession: %d\r\n\r\n",
			m_serverURL.c_str(), m_cseq, m_agentDes.c_str(), m_sessionID.c_str());
	}

	return 0;
}

int CClientRTSP::CreateGetParameter()
{
	m_Lock.Lock();

	do 
	{
		memset(m_pSendBuf, 0, RTP_CLIENT_RTSP_SEND_LEN);
		m_SendDataLen = 0;

		m_cseq ++;
		char Authorization[512] = {0};

		if (m_Authenticate == AUTHENTICATE_DIGEST)
		{
			HASHHEX response;
			if (!m_HttpDigestAuth.GetResponse("GET_PARAMETER", response))
			{
				printf("%s %d err\n", __FILE__, __LINE__);
				break;
			}
			snprintf(Authorization,  sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
				m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

			snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
				"GET_PARAMETER %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nSession: %s\r\n\r\n",\
				m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str(), m_sessionID.c_str());
		}
		else if(m_Authenticate == AUTHENTICATE_BASIC)
		{
			if (!m_HttpDigestAuth.GetBasicCredentials(m_UserName.c_str(), m_PassWd.c_str(), Authorization))
			{
				printf("%s %d err\n", __FILE__, __LINE__);
				break;
			}
			snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
				"GET_PARAMETER %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: Basic %s\r\nUser-Agent: %s\r\nSession: %s\r\n\r\n",\
				m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str(), m_sessionID.c_str());
		}
		else
		{
			snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
				"GET_PARAMETER %s RTSP/1.0\r\nCSeq: %d\r\nUser-Agent: %s\r\nSession: %s\r\n\r\n",\
				m_serverURL.c_str(), m_cseq, m_agentDes.c_str(), m_sessionID.c_str());
		}

	} while (0);

	m_Lock.UnLock();

	return 0;
}


int CClientRTSP::GetStatusCode(char *pStatusCode)
{
	char *pPos = strstr(pStatusCode, "RTSP/1.0");
	if (NULL == pPos)
	{
		printf("%s %d err\n", __FILE__, __LINE__);
		return -1;
	}

	pPos = strstr(pStatusCode, "200 OK");
	if (NULL != pPos)
	{
		return 200;
	}

	//没有检测到200 ok
	char *pErrCode = strstr(pStatusCode, "RTSP/1.0")+sizeof("RTSP/1.0");
	if (NULL != pErrCode)
	{
		char scode[10] = {0};
		strncpy(scode, pErrCode, 3);
		return atoi(scode);
	}
	else
	{
		printf("%s %d err\n", __FILE__, __LINE__);
		return -1;
	}
}


int CClientRTSP::GetTransportInfo(char *pRTransportInfo, unsigned long length)
{
	snprintf(pRTransportInfo, length, "RTP/AVP/");
	if (m_bISTCPSession)
	{
		strcat(pRTransportInfo, "TCP;");
	}
	else
	{
		strcat(pRTransportInfo, "UDP;");
	}

	if (m_bISUnicast)
	{
		strcat(pRTransportInfo, "unicast;");
	}
	else
	{
		strcat(pRTransportInfo, "broadcast;");
	}

	char interleaveInfo[36] = {0};

	snprintf(interleaveInfo, sizeof(interleaveInfo), "interleaved=%d-%d", m_interleaveChannel, m_interleaveChannel+1);
	m_interleaveChannel += 2;
	

	strcat(pRTransportInfo, interleaveInfo);

	return 0;
}


int CClientRTSP::GetBufferData(char *pBuffer, unsigned long &len)
{
	if (NULL == pBuffer)
	{
		return -1;
	}
	m_Lock.Lock();
	len = strlen(m_pSendBuf);

	memcpy(pBuffer, m_pSendBuf, len);
	//printf("%s\n", m_pSendBuf);
	memset(m_pSendBuf, 0, len);
	m_bIsSending = true;
	m_Lock.UnLock();
	return 0;
}

bool CClientRTSP::IsSendBufEmpty()
{
	bool ret;
	m_Lock.Lock();
	ret = (strlen(m_pSendBuf) == 0);
	m_Lock.UnLock();
	return ret;
}

bool CClientRTSP::IsSupportGetParameter()
{
	return m_bSupportGetParamFn;
}

bool CClientRTSP::IsHasAudio()
{
	bool ret = false; 
	m_Lock.Lock();
	if( m_bOpenAudioData )
	{
		ret = m_bIsHasAudio;
	}
	m_Lock.UnLock(); 
	return ret;
}

bool CClientRTSP::IsHasVideo()
{
	bool ret; 
	m_Lock.Lock();
	ret = m_bIsHasVideo;
	m_Lock.UnLock();
	return ret;
}

float CClientRTSP::GetFrameRate()
{
	float ret = 0;;
	m_Lock.Lock();
	for (int i=0; i < m_SDPPaerser.GetMediaCount(); ++i)
	{
		if (STREAM_LIVE_VIDEO == m_SDPPaerser.GetMediaType(i))
		{
			ret = m_SDPPaerser.GetRate(i);
			break;
		}
	}

	m_Lock.UnLock();

	return ret;
}

unsigned short CClientRTSP::GetWidth()
{
	unsigned short ret = 0;

	m_Lock.Lock();
	for (int i=0; i < m_SDPPaerser.GetMediaCount(); ++i)
	{
		if (STREAM_LIVE_VIDEO == m_SDPPaerser.GetMediaType(i))
		{
			ret = m_SDPPaerser.GetVideoWidth(i);
			break;
		}
	}
	m_Lock.UnLock();

	return ret;
}

unsigned short CClientRTSP::GetHeight()
{
	unsigned short ret = 0;;

	m_Lock.Lock();
	for (int i=0; i < m_SDPPaerser.GetMediaCount(); ++i)
	{
		if (STREAM_LIVE_VIDEO == m_SDPPaerser.GetMediaType(i))
		{
			ret = m_SDPPaerser.GetVideoHeight(i);
			break;
		}
	}
	m_Lock.UnLock();

	return ret;
}

void CClientRTSP::SetChannelNumber( int chnn, int sub_chn )
{
	m_chnnel_number = chnn;
	m_sub_chn_num = sub_chn;
}

void CClientRTSP::SetAudioEnableFlag( bool bAudioEnable )
{
	m_bOpenAudioData = bAudioEnable;
}

