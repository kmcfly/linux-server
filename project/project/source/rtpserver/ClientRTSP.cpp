#include "ClientRTSP.h"

//#ifdef assert
//#undef assert
//#define assert(a) ((void)0)
//#endif

CClientRTSP::CClientRTSP()
{
	m_pRecvBuf = new char[RTP_CLIENT_RTSP_RECV_LEN];
	assert(NULL != m_pRecvBuf);
	memset(m_pRecvBuf, 0, RTP_CLIENT_RTSP_RECV_LEN);
	m_recvDataLen = 0;

	m_pSendBuf = new char [RTP_CLIENT_RTSP_SEND_LEN];
	assert(NULL != m_pSendBuf);
	memset(m_pSendBuf, 0, RTP_CLIENT_RTSP_SEND_LEN);
	m_SendDataLen = 0;

	memset(m_ServerSupportMethod, 0, sizeof(m_ServerSupportMethod));
	memset(m_ContentType, 0, sizeof(m_ContentType));
	m_ContentLen = 0;

	m_agentDes = "Tongwei Video Technology Co.,Ltd.";

	m_cseq = 0;
	m_ncount = 0;
	m_bAuthenticate = false;
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
	m_TimeOut = 65535;
}

CClientRTSP::~CClientRTSP()
{
	if (NULL != m_pRecvBuf)
	{
		delete m_pRecvBuf;
		m_pRecvBuf = NULL;
	}

	if (NULL != m_pSendBuf)
	{
		delete m_pSendBuf;
		m_pSendBuf = NULL;
	}
}

int CClientRTSP::HandleData(const char *pData, unsigned long len)
{
	int ret = 0;
	if (NULL == pData || len > RTP_CLIENT_RTSP_RECV_LEN)
	{
		assert(false);
		return -1;
	}
	m_Lock.Lock();
	m_bIsSending = false;
	memset(m_pRecvBuf, 0, RTP_CLIENT_RTSP_RECV_LEN);
	memcpy(m_pRecvBuf, pData, len);
	printf("\r\n%s\r\n", m_pRecvBuf);

	ret = ParseData(m_pRecvBuf);
	if (ret < 0)
	{
		m_Lock.UnLock();
		assert(false);
		return -1;
	}
	else if (STATUS_CODE_UNAUTHORIZED == ret)
	{
		if (0 == m_mode.compare(RTSP_AUTHENTICATE_DIGEST))
		{
			m_bAuthenticate = true;
			if (m_serverURL[0] == '\0'|| m_UserName[0] == '\0' || m_PassWd[0] == '\0' || m_nonce[0] == '\0' || m_realm[0] == '\0')
			{
				assert(false);
				m_Lock.UnLock();
				return -1;
			}
			m_HttpDigestAuth.SetDigestAuth(m_serverURL.c_str(), m_UserName.c_str(), m_PassWd.c_str(), m_nonce.c_str(), m_realm.c_str(), "", "md5");
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
				m_Lock.UnLock();
				printf("%s %d UNAUTHORIZED\n", __FILE__, __LINE__);
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
		if (!m_SetupRequest[0].empty())
		{
			CreateSetup(STREAM_LIVE_VIDEO);
			m_SetupRequest[0].clear();
			m_SendingMethod = RTSP_METHOD_SETUP;
		}
		else if (!m_SetupRequest[1].empty())
		{
			CreateSetup(STREAM_LIVE_AUDIO);
			m_SendingMethod = RTSP_METHOD_SETUP;
			m_SetupRequest[1].clear();
		}
		else
		{
			m_Lock.UnLock();
			return -1;
		}
		break;
	case RTSP_METHOD_SETUP:
		if (!m_SetupRequest[0].empty())
		{
			CreateSetup(STREAM_LIVE_VIDEO);
			m_SetupRequest[0].clear();
			break;
		}
		else if (!m_SetupRequest[1].empty())
		{
			CreateSetup(STREAM_LIVE_AUDIO);
			m_SetupRequest[1].clear();
			break;
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
	m_Lock.UnLock();
	return 0;
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
			assert(false);
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
				printf("%s:%s:%d,request seq=%d, answer seq=%d\n", __FUNCTION__, __FILE__, __LINE__, m_cseq, answerSeq);
				//assert(false);
				return -1;
			}
		}

		if (!strncasecmp(answer, "Server:", 7))
		{
		}

		if (!strncasecmp(answer, "Session:", 8))
		{
			char Session[64] = {0};
			unsigned int timeout = 65535;
			sscanf(answer, "%*s %[0-9a-zA-Z$_.+-]", Session);
			m_sessionID = Session;

			char *pPos = answer + 8;
			pPos = strstr(pPos, "timeout=");
			if (NULL != pPos)
			{
				timeout = atoi(pPos+8);
				if (m_TimeOut > timeout)
				{
					m_TimeOut = timeout;
				}
			}
		}

		if (!strncasecmp(answer, "Public:", 7))
		{
			int i = 0;
			char *pPos = answer + 7;
			char *pPosEnd = NULL;
			while(NULL != pPos)
			{
				pPos += 1;
				pPosEnd = strstr(pPos, ",");
				if (NULL == pPosEnd)
				{
					pPosEnd = strstr(answer, "\r\n");
					if (NULL == pPosEnd)
					{
						assert(false);
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
				assert(false);
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
				
// 				if ()//添加stale
// 				{
// 				}
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
				for (int i=0; i<m_SDPPaerser.GetMediaCount(); ++i)
				{
					//for test 实际上只支持H264 和 G711
					if (m_SDPPaerser.GetMediaType(i) >= RTP_PAYLOAD_TYPE_H264_ && m_SDPPaerser.GetMediaType(i) <= RTP_PAYLOAD_TYPE_H264_MAX)
					{
						m_bIsHasVideo = true;
						m_SetupRequest[0] = m_SDPPaerser.GetMediaURL(i);
						//如果给的地址只是部分地址，需要另作处理
						if (m_SetupRequest[0].length() < m_serverURL.length())
						{
							std::string tmp = m_serverURL;
							m_SetupRequest[0] = tmp + "/" + m_SetupRequest[0];
						}
					}
					else
					{
						m_bIsHasAudio = true;
						m_SetupRequest[1] = m_SDPPaerser.GetMediaURL(i);
						//如果给的地址只是部分地址，需要另作处理
						if (m_SetupRequest[1].length() < m_serverURL.length())
						{
							std::string tmp = m_serverURL;
							m_SetupRequest[1] = tmp + "/" +m_SetupRequest[1];
						}
					}
				}
			}
		}
	}

	return statusCode;
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

	if (m_bAuthenticate)
	{
		if (++m_RequestTime > 2)
		{
			m_RequestTime = 0;
			assert(false);
			return -1;
		}
		char Authorization[256] = {0};

		HASHHEX response;
		if (!m_HttpDigestAuth.GetResponse("OPTIONS", response))
		{
			assert(false);
			return -1;
		}

		snprintf(Authorization, sizeof(Authorization), "%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"OPTIONS %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\n\r\n",
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

	if (m_bAuthenticate)
	{
		HASHHEX response;
		char Authorization[256] = {0};
		if (!m_HttpDigestAuth.GetResponse("DESCRIBE", response))
		{
			assert(false);
			return -1;
		}
		snprintf(Authorization,  sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"DESCRIBE %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nAccept: application/sdp\r\n\r\n",
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
		if (m_SetupRequest[0].empty())
		{
			assert(false);
			return -1;
		}
		strcpy(requestURL, m_SetupRequest[0].c_str());
	}
	else if (streamType == STREAM_LIVE_AUDIO)
	{
		if (m_SetupRequest[1].empty())
		{
			assert(false);
			return -1;
		}
		strcpy(requestURL, m_SetupRequest[1].c_str());
	}
	else
	{
		assert(false);
		return -1;
	}

	if (0 != GetTransportInfo(transportInfo, sizeof(transportInfo)))
	{
		return -1;
	}

	if (m_bAuthenticate)
	{
		HASHHEX response;
		char Authorization[256] = {0};
		if (!m_HttpDigestAuth.GetResponse("SETUP", response))
		{
			assert(false);
			return -1;
		}
		snprintf(Authorization, sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"SETUP %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nTransport: %s\r\n",
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

	if (m_bAuthenticate)
	{
		HASHHEX response;
		char Authorization[256] = {0};
		if (!m_HttpDigestAuth.GetResponse("PLAY", response))
		{
			assert(false);
			return -1;
		}
		snprintf(Authorization,  sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"PLAY %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nSession: %s\r\n",\
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

	if (m_bAuthenticate)
	{
		HASHHEX response;
		char Authorization[256] = {0};
		if (!m_HttpDigestAuth.GetResponse("TEARDOWN", response))
		{
			assert(false);
			return -1;
		}
		snprintf(Authorization,  sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(pData, RTP_CLIENT_RTSP_SEND_LEN, 
			"TEARDOWN %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nSession: %s\r\n\r\n",
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str(), m_sessionID.c_str());

	}
	else
	{
		snprintf(pData, RTP_CLIENT_RTSP_SEND_LEN, 
			"TEARDOWN %s RTSP/1.0\r\nCSeq: %d\r\nUser-Agent: %s\r\nSession: %s\r\n\r\n",
			m_serverURL.c_str(), m_cseq, m_agentDes.c_str(), m_sessionID.c_str());
	}

	return 0;
}

int CClientRTSP::CreateTeardown()
{
	m_cseq ++;

	if (m_bAuthenticate)
	{
		HASHHEX response;
		char Authorization[256] = {0};
		if (!m_HttpDigestAuth.GetResponse("TEARDOWN", response))
		{
			assert(false);
			return -1;
		}
		snprintf(Authorization,  sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"TEARDOWN %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nSession: %d\r\n\r\n",
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

	memset(m_pSendBuf, 0, RTP_CLIENT_RTSP_SEND_LEN);
	m_SendDataLen = 0;

	m_cseq ++;

	if (m_bAuthenticate)
	{
		HASHHEX response;
		char Authorization[256] = {0};
		if (!m_HttpDigestAuth.GetResponse("GET_PARAMETER", response))
		{
			assert(false);
			return -1;
		}
		snprintf(Authorization,  sizeof(Authorization),"%s username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"",\
			m_mode.c_str(), m_UserName.c_str(), m_realm.c_str(), m_nonce.c_str(), m_serverURL.c_str(), response);

		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"GET_PARAMETER %s RTSP/1.0\r\nCSeq: %d\r\nAuthorization: %s\r\nUser-Agent: %s\r\nSession: %s\r\n\r\n",\
			m_serverURL.c_str(), m_cseq, Authorization, m_agentDes.c_str(), m_sessionID.c_str());
	}
	else
	{
		snprintf(m_pSendBuf, RTP_CLIENT_RTSP_SEND_LEN, 
			"GET_PARAMETER %s RTSP/1.0\r\nCSeq: %d\r\nUser-Agent: %s\r\nSession: %s\r\n\r\n",\
			m_serverURL.c_str(), m_cseq, m_agentDes.c_str(), m_sessionID.c_str());
	}

	m_Lock.UnLock();

	return 0;
}


int CClientRTSP::GetStatusCode(char *pStatusCode)
{
	char *pPos = strstr(pStatusCode, "RTSP/1.0");
	if (NULL == pPos)
	{
		assert(false);
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
		assert(false);
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
	printf("%s\n", m_pSendBuf);
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
	for (int i=0; i<MAX_RTSP_METHOD; ++i)
	{
		if (0 == strcmp(m_ServerSupportMethod[i], "GET_PARAMETER"))
		{
			return true;
		}
	}

	return false;
}

bool CClientRTSP::IsHasAudio()
{
	bool ret; 
	m_Lock.Lock();
	ret = m_bIsHasAudio;
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
		if (RTP_PAYLOAD_TYPE_H264_ <= m_SDPPaerser.GetMediaType(i) && RTP_PAYLOAD_TYPE_H264_MAX >= m_SDPPaerser.GetMediaType(i))
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
		if (RTP_PAYLOAD_TYPE_H264_ <= m_SDPPaerser.GetMediaType(i) && RTP_PAYLOAD_TYPE_H264_MAX >= m_SDPPaerser.GetMediaType(i))
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
		if (RTP_PAYLOAD_TYPE_H264_ <= m_SDPPaerser.GetMediaType(i) && RTP_PAYLOAD_TYPE_H264_MAX >= m_SDPPaerser.GetMediaType(i))
		{
			ret = m_SDPPaerser.GetVideoHeight(i);
			break;
		}
	}
	m_Lock.UnLock();

	return ret;
}