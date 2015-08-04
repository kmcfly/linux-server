#include "ClientRTSPSession.h"


CClientRTSPSession::CClientRTSPSession()
{
	m_bHasSetRTP = false;
	m_RecvWaitTime = 0;
}

CClientRTSPSession::~CClientRTSPSession()
{

}

void CClientRTSPSession::Initial()
{
	
}

int CClientRTSPSession::ParseURL(char const* url, char* username, char* password, char* address,int &portNum) 
{
	if (NULL == url || NULL == username || NULL == password || NULL == address)
	{
		return -1;
	}
	
	do{
		// Parse the URL as "rtsp://[<username>[:<password>]@]<server-address-or-name>[:<port>][/<stream-name>]"
		char const* prefix = "rtsp://";
		unsigned const prefixLength = 7;
		unsigned const parseBufferSize = 100;
		char const* from = &url[prefixLength];

		// Check whether "<username>[:<password>]@" occurs next.
		// We do this by checking whether '@' appears before the end of the URL, or before the first '/'.
		char const* colonPasswordStart = NULL;
		char const* p;

		for (p = from; *p != '\0' && *p != '/'; ++p) 
		{
			if (*p == ':' && colonPasswordStart == NULL) 
			{
				colonPasswordStart = p;
			} 
			else if (*p == '@') 
			{
				if (colonPasswordStart == NULL) 
				{
					colonPasswordStart = p;
				}

				char const* usernameStart = from;
				unsigned usernameLen = colonPasswordStart - usernameStart;

				for (unsigned i = 0; i < usernameLen; ++i) 
				{
					username[i] = usernameStart[i];
				}
				username[usernameLen] = '\0';

				char const* passwordStart = colonPasswordStart;
				if (passwordStart < p) 
				{
					++passwordStart; // skip over the ':'
				}

				unsigned passwordLen = p - passwordStart;
				
				for (unsigned j = 0; j < passwordLen; ++j)
				{
					password[j] = passwordStart[j];
				}
				password[passwordLen] = '\0';
				from = p + 1; // skip over the '@'
				break;
			}
		}

		// Next, parse <server-address-or-name>
		char* to = address;//parseBuffer[0];
		unsigned i;
		for (i = 0; i < parseBufferSize; ++i) 
		{
			if (*from == '\0' || *from == ':' || *from == '/') 
			{
				// We've completed parsing the address
				*to = '\0';
				break;
			}
			*to++ = *from++;
		}
		if (i == parseBufferSize)
		{
			printf("URL is too long");
			break;
		}

		char nextChar = *from;
		int portNumInt = 554;
		if (nextChar == ':') 
		{
			if (sscanf(++from, "%d", &portNumInt) != 1)
			{
				printf("%s %s %d Bad Port Num", __FILE__, __FUNCTION__, __LINE__);
				return -1;
			}
			if (portNumInt < 1 || portNumInt > 65535) 
			{
				printf("%s %s %d Bad Port Num", __FILE__, __FUNCTION__, __LINE__);
				return -1;
			}
		}
		portNum = portNumInt;
		return 1;
	}while (0);

	return -1;
}


int	CClientRTSPSession::Start(const char *pURL, const char *pUserName, const char *pPassWd)
{
	m_RTSP.CreateOptions(pURL, pUserName, pPassWd);
	m_LastGetParaTime = RTSPGetCurrTime();
	m_LastRTCPTime = m_LastGetParaTime;
	return 0;
}

int CClientRTSPSession::Stop()
{
	m_RTP.Stop();

	return 0;
}

void CClientRTSPSession::SetCallBack(RTP_CALLBACK pCallBack, void *pParam, int TVTSessionID)
{
	m_RTP.SetCallBack(pCallBack, pParam, TVTSessionID);
}

int CClientRTSPSession::GetData(const char* pData, unsigned long len)
{
/*
判断data该往哪个模块传
思想：判断第一个字节是否为0x24 
若是则记录INTERLEAVED_INFO中的长度,RTP数据接收到此长度为止
若不是0x24，则为RTSP数据，若是继续记录INTERLEAVED_INFO长度
*/

	unsigned long remainLen;
	unsigned long packetLen;
	INTERLEAVED_INFO *pInterleaved;
	const char *pPos = pData;

	if (len > 65535)
	{
		remainLen = 65535;
	}
	else
	{
		remainLen = len;
	}
	int HandleLen = remainLen;

	while(0 != remainLen)
	{
		if (INTERLEAVED_DOLLAR_SIGN == *pPos)
		{
			if (remainLen < 4)
			{
				return len - remainLen;
			}
			pInterleaved = (INTERLEAVED_INFO *)pPos;
			packetLen = ntohs(pInterleaved->datalength);
			if (remainLen - sizeof(INTERLEAVED_INFO) < packetLen)
			{
				return len - remainLen;
			}
			pPos += sizeof(INTERLEAVED_INFO);
			remainLen -= sizeof(INTERLEAVED_INFO);

			m_Lock.Lock();
			if (packetLen != m_RTP.GetRTPData(pPos, packetLen, pInterleaved->channel))
			{
				assert(false);
				m_Lock.UnLock();
				return -1;
			}
			m_Lock.UnLock();

			pPos += packetLen;
			remainLen -= packetLen;
		}
		else//RTSP
		{
			int	retVal = FindString(pPos, "\r\n\r\n", remainLen);
			if (0 > retVal)//没找到
			{
				return HandleLen - remainLen;
			}
			else
			{
				int rtspLen = retVal;
				bool bHasSDP = false;
				retVal = FindString(pPos, "Content-Length: ", rtspLen);
				if (0 > retVal)
				{
					retVal = FindString(pPos, "Content-length: ", rtspLen);//to be compatible
					if (retVal >= 0)
					{
						bHasSDP = true;
					}
				}
				else
				{
					bHasSDP = true;
				}

				if (bHasSDP)
				{
					rtspLen += atoi(pPos + retVal);
					if (remainLen < rtspLen)
					{
						return HandleLen - remainLen;
					}
				}
				m_Lock.Lock();
				if (m_RTSP.HandleData(pPos, rtspLen) < 0)
				{
					m_Lock.UnLock();
					printf("m_RTSP.HandleData err\n");
					return -1;
				}

				m_Lock.UnLock();
				pPos += rtspLen;
				remainLen -= rtspLen;
			}
		}//end of else RTSP
	}//end of while
	

	if(!m_bHasSetRTP)//RTSP得到信息后把媒体信息告诉RTP
	{
		m_Lock.Lock();
		if (RTSP_METHOD_PLAY == m_RTSP.GetCurMethod())//已经发送了 PLAY  说明已经SETUP
		{
			m_bHasSetRTP = true;
			m_RTP.SetUp(m_RTSP.IsHasVideo(), m_RTSP.IsHasAudio(), m_RTSP.GetFrameRate(), m_RTSP.GetWidth(), m_RTSP.GetHeight());
		}
		m_Lock.UnLock();
	}

	return HandleLen;
}

int CClientRTSPSession::SendTearDown(char *pData)
{
	m_RTSP.CreateTeardown(pData);

	return 0;
}

int CClientRTSPSession::SendData(char* pData, unsigned long &len)
{
	int ret = -1;
	if (NULL == pData)
	{
		return ret;
	}

	m_Lock.Lock();

	if (!m_RTSP.IsSendBufEmpty())
	{
		ret = m_RTSP.GetBufferData(pData, len);
	}

	if (RTSP_SESSION_PLAY == m_RTSP.GetStatus())
	{
		int seconds = RTSPGetCurrTime().seconds;

		//RTSP会告之服务器的超时时间  在超时时间之内使用无entity的Get_PARAMETER(相当于ping)
		if (m_RTSP.IsSupportGetParameter())
		{
			if (seconds - m_LastGetParaTime.seconds >= m_RTSP.GetTimeOut())
			{
				m_LastGetParaTime.seconds = seconds;
				ret = m_RTSP.CreateGetParameter();
				ret = m_RTSP.GetBufferData(pData, len);
				m_Lock.UnLock();
				return ret;
			}
		}

		if  (seconds - m_LastRTCPTime.seconds >= 5)
		{
			m_LastRTCPTime.seconds = seconds;
			ret = m_RTP.SendRTCPData(pData, len);
		}
	}

	m_Lock.UnLock();
	return ret;
}

int CClientRTSPSession::FindString(const char *pSrc, const char *pPattern, const int dataLen)
{
	//Sunday算法
	int patternLen = strlen(pPattern);
	int i=0,j=0;//分别为目标串和模式串当前匹配的位置
	int pe = patternLen - 1;//目标串后定位

	while(i < dataLen && j<patternLen)
	{
		if (pSrc[i] == pPattern[j])
		{
			i++;
			j++;
		}
		else//计算NEXT值
		{
			int k = patternLen - 1;
			while(k>=0 && pSrc[pe + 1] != pPattern[k])//查找模式串中是否有pe
			{
				k--;
			}
			int gap = patternLen - k;
			i += gap;
			pe = i + patternLen - 1;
			j = 0;
		}
	}
	if (j == patternLen)
	{
		return i;
	}

	return -1;
}

void CClientRTSPSession::HandleConnectionBreak()
{
	m_RTP.HandleConnectionBreak();
}