/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 
** Name         : 
** Version      :
** Description  :
** Modify Record:
1:
***********************************************************************/


#include "YZSnapMan.h"
#include "mylist.cpp"
#include "SnapCapabilityMan.h"
#define IMAGE_XML "<?xml version=\"1.0\"?>\r\n<root xmlns:dt=\"urn:schemas-microsoft-com:datatypes\">\r\n<ascode>%s</ascode>\r\n<image dt:dt=\"bin.base64\">%s</image>\r\n</root>"

const unsigned char Base64IdxTab[128] =
{
	255,255,255,255,  255,255,255,255,  255,255,255,255,  255,255,255,255,
	255,255,255,255,  255,255,255,255,  255,255,255,255,  255,255,255,255,
	255,255,255,255,  255,255,255,255,  255,255,255,62,   255,255,255,63,
	52,53,54,55,      56,57,58,59,      60,61,255,255,    255,255,255,255,
	255,0,1,2,        3,4,5,6,          7,8,9,10,         11,12,13,14,
	15,16,17,18,      19,20,21,22,      23,24,25,255,     255,255,255,255,
	255,26,27,28,     29,30,31,32,      33,34,35,36,      37,38,39,40,
	41,42,43,44,      45,46,47,48,      49,50,51,255,     255,255,255,255
};

const char Base64ValTab[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

CBase64::CBase64(void)
{

}

CBase64::~CBase64(void)
{

}

void CBase64::SetLineMaxLen(long lineMaxLen)
{
	m_lineMaxLen = lineMaxLen;
}

long CBase64::Encode(const unsigned char *pSrc, long srcLen, char *pDest, long destLen)
{
	int i = 0;
	int loop = 0;
	int remain = 0;
	int retLen = 0;
	int lineLen = 0;

	loop = srcLen/3;
	remain = srcLen%3;

	// also can encode native char one by one as decode method
	// but because all of char in native string  is to be encoded so encode 3-chars one time is easier.

	for (i=0; i<loop; i++)
	{
		unsigned char a1 = (pSrc[i*3] >> 2);
		unsigned char a2 = ( ((pSrc[i*3] & 0x03) << 4) | (pSrc[i*3+1] >> 4) );
		unsigned char a3 = ( ((pSrc[i*3+1] & 0x0F) << 2) | ((pSrc[i*3+2] & 0xC0) >> 6) );
		unsigned char a4 = (pSrc[i*3+2] & 0x3F);

		pDest[retLen++] = Base64ValTab[a1];
		if(++lineLen == m_lineMaxLen)
		{
			lineLen = 0;
			pDest[retLen++] = '\r';
			pDest[retLen++] = '\n';
		}

		pDest[retLen++] = Base64ValTab[a2];
		if(++lineLen == m_lineMaxLen)
		{
			lineLen = 0;
			pDest[retLen++] = '\r';
			pDest[retLen++] = '\n';
		}

		pDest[retLen++] = Base64ValTab[a3];
		if(++lineLen == m_lineMaxLen)
		{
			lineLen = 0;
			pDest[retLen++] = '\r';
			pDest[retLen++] = '\n';
		}

		pDest[retLen++] = Base64ValTab[a4];
		if(++lineLen == m_lineMaxLen)
		{
			lineLen = 0;
			pDest[retLen++] = '\r';
			pDest[retLen++] = '\n';
		}
	}

	if (remain == 1)
	{
		// should pad two equal sign
		i = srcLen-1;
		unsigned char a1 = (pSrc[i] >> 2);
		unsigned char a2 = ((pSrc[i] & 0x03) << 4);

		pDest[retLen++] = Base64ValTab[a1];
		if(++lineLen == m_lineMaxLen)
		{
			lineLen = 0;
			pDest[retLen++] = '\r';
			pDest[retLen++] = '\n';
		}

		pDest[retLen++] = Base64ValTab[a2];
		if(++lineLen == m_lineMaxLen)
		{
			lineLen = 0;
			pDest[retLen++] = '\r';
			pDest[retLen++] = '\n';
		}

		pDest[retLen++] = '=';
		if(++lineLen == m_lineMaxLen)
		{
			lineLen = 0;
			pDest[retLen++] = '\r';
			pDest[retLen++] = '\n';
		}

		pDest[retLen++] = '=';
		pDest[retLen] = 0x00;
	}
	else if (remain == 2)
	{
		// should pad one equal sign
		i = srcLen-2;
		unsigned char a1 = (pSrc[i] >> 2);
		unsigned char a2 = ( ((pSrc[i] & 0x03) << 4) | (pSrc[i+1] >> 4));
		unsigned char a3 = ( (pSrc[i+1] & 0x0F) << 2);

		pDest[retLen++] = Base64ValTab[a1];
		if(++lineLen == m_lineMaxLen)
		{
			lineLen = 0;
			pDest[retLen++] = '\r';
			pDest[retLen++] = '\n';
		}

		pDest[retLen++] = Base64ValTab[a2];
		if(++lineLen == m_lineMaxLen)
		{
			lineLen = 0;
			pDest[retLen++] = '\r';
			pDest[retLen++] = '\n';
		}

		pDest[retLen++] = Base64ValTab[a3];
		if(++lineLen == m_lineMaxLen)
		{
			lineLen = 0;
			pDest[retLen++] = '\r';
			pDest[retLen++] = '\n';
		}

		pDest[retLen++] = '=';
		pDest[retLen] = 0x00;
	}
	else
	{
		// just division by 3
		pDest[retLen] = 0x00;
	}

	return retLen;
}

long CBase64::Decode(const char *pSrc, long srcLen, unsigned char *pDest, long destLen)
{
	int i = 0;
	int iCnt = 0;

	unsigned char * p = pDest;

	for (i=0; i<srcLen; i++)
	{
		if (pSrc[i] > 127)
		{
			continue;
		}

		if (pSrc[i] == '=')
		{
			return p-pDest+1;
		}

		unsigned char a = Base64IdxTab[pSrc[i]];
		if (a == 255)
		{
			continue;
		}

		switch (iCnt)
		{
		case 0:
			{
				*p = a << 2;
				iCnt++;
			}
			break;

		case 1:
			{
				*p++ |= a >> 4;
				*p = a << 4;
				iCnt++;
			}
			break;

		case 2:
			{
				*p++ |= a >> 2;
				*p = a << 6;
				iCnt++;
			}
			break;

		case 3:
			{
				*p++ |= a;
				iCnt = 0;
			}
			break;
		} 
	}

	*p = 0x00;
	return p-pDest;
}



CYZJpegSendClinet::CYZJpegSendClinet()
{

}

CYZJpegSendClinet::~CYZJpegSendClinet()
{

}

bool CYZJpegSendClinet::Init()
{
	memset(&m_sendInfo, 0, sizeof(YZ_SNAP_INFO));
	memset(m_ascode, 0, sizeof(m_ascode));
	memset(m_serverUrl, 0, sizeof(m_serverUrl));
	memset(m_urlObject, 0, sizeof(m_urlObject));
	m_urlPort = 0;

	m_base64.SetLineMaxLen(72);
	m_pBase64Buffer = new char [YZ_MAX_SEND_JPEG_BUFFER];
	memset(m_pBase64Buffer, 0, YZ_MAX_SEND_JPEG_BUFFER);

	m_pSendXmlBuffer = new char [YZ_MAX_SEND_JPEG_BUFFER];
	memset(m_pSendXmlBuffer, 0, YZ_MAX_SEND_JPEG_BUFFER);

	return true;
}

void CYZJpegSendClinet::Quit()
{
	if (m_pBase64Buffer != NULL)
	{
		delete [] m_pBase64Buffer;
		m_pBase64Buffer = NULL;
	}

	if (m_pSendXmlBuffer != NULL)
	{
		delete [] m_pSendXmlBuffer;
		m_pSendXmlBuffer = NULL;
	}
}

void CYZJpegSendClinet::SetSendPara(YZ_SNAP_INFO info, char ascode[64], char server[128], char object[128], unsigned short port)
{
	memcpy(&m_sendInfo, &info, sizeof(YZ_SNAP_INFO));
	snprintf(m_ascode, sizeof(m_ascode), "%s", ascode);
	snprintf(m_serverUrl, sizeof(m_serverUrl), "%s", server);
	snprintf(m_urlObject, sizeof(m_urlObject), "%s", object);
	m_urlPort = port;
}


unsigned long CYZJpegSendClinet::DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName )
{	
	unsigned long ret = DDNS_IPUPDATE_OK;
	do 
	{
		if ((0 == strlen(m_ascode)) || (0 == strlen(m_serverUrl)) || (0 == strlen(m_urlObject)) || (0 == m_urlPort))
		{
			ret = DDNS_IPUPDATE_EUNKNOWN;
			break;
		}
		if ((NULL == m_sendInfo.pData) || (m_sendInfo.length > 20*1024))
		{
			ret = DDNS_IPUPDATE_EUNKNOWN;
			break;
		}
		
		struct in_addr  addrTemp;
		struct sockaddr_in ser_addr;
		if(INADDR_ANY == inet_aton(m_serverUrl, &addrTemp))
		{
			//printf("%s,%d use GetHostByName\n",__FILE__,__LINE__);
			struct hostent *phstt = CNetInterface::Instance()->GetHostByName(m_serverUrl);
			if (NULL == phstt)
			{
				/*printf("gethostbyname() error\n");
				printf("ZZZZ %s:%s:%d, 不能解析域名:%s\n", __FUNCTION__, __FILE__, __LINE__,m_serverUrl);*/
				ret = DDNS_IPUPDATE_EUNKNOWN;//无法解析得到ServerIP
				break;
			}
			ser_addr.sin_family = AF_INET;
			ser_addr.sin_port =  htons(m_urlPort);
			memcpy(&(ser_addr.sin_addr), *((struct in_addr **)phstt->h_addr_list), sizeof(struct in_addr));
			memset(ser_addr.sin_zero,0, sizeof(ser_addr.sin_zero));
			CNetInterface::Instance()->ReleaseHost(phstt);
		}
		else
		{
			//printf("is ip addr\n");
			ser_addr.sin_family = AF_INET;
			ser_addr.sin_port =  htons(m_urlPort);
			memcpy(&(ser_addr.sin_addr), &addrTemp, sizeof(struct in_addr));
			memset(ser_addr.sin_zero,0, sizeof(ser_addr.sin_zero));		
		}
		
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if( -1 == sockfd )
		{
			ret = DDNS_IPUPDATE_EUNKNOWN;
			break;
		}

		if(ConnectServer(sockfd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr), CONNECT_TIME_OUNT) != 0)
		{
			close(sockfd);
			ret = DDNS_IPUPDATE_EUNKNOWN;
			break;
		}
		
		memset(m_pBase64Buffer, 0, YZ_MAX_SEND_JPEG_BUFFER);
		long base64Len = m_base64.Encode(m_sendInfo.pData, m_sendInfo.length, m_pBase64Buffer, YZ_MAX_SEND_JPEG_BUFFER);

		memset(m_pSendXmlBuffer, 0, YZ_MAX_SEND_JPEG_BUFFER);
		snprintf(m_pSendXmlBuffer, YZ_MAX_SEND_JPEG_BUFFER, IMAGE_XML, m_ascode, m_pBase64Buffer);
		long xmlLen = strlen(m_pSendXmlBuffer);

		snprintf(m_pBase64Buffer,YZ_MAX_SEND_JPEG_BUFFER, "POST %s HTTP/1.1\r\nHost:%s\r\nAccept:*/*\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)\r\nConnection:Keep-Alive\r\nContent-Length:%d\r\n\r\n%s", m_urlObject,m_serverUrl, strlen(m_pSendXmlBuffer), m_pSendXmlBuffer);
		//send cmd
		if(send(sockfd, m_pBase64Buffer, strlen(m_pBase64Buffer), 0) == -1)
		{
			//perror("send() data to server");
			close(sockfd);
			ret = DDNS_IPUPDATE_EUNKNOWN;
			break;
		}

		//printf("m_pBase64Buffer:%s\n",m_pBase64Buffer);

		//receive
		char ReceiveBuf[1000] = {0};
		int len=sizeof(ReceiveBuf);
		ret=GetMessageFromServer(sockfd,ReceiveBuf,len);
		close(sockfd);

		//printf("%s,%d,ReceiveBuf:%s\n",__FILE__,__LINE__,ReceiveBuf);
		if(ret != DDNS_IPUPDATE_OK)
		{
			ret = DDNS_IPUPDATE_EUNKNOWN;
			break;
		}
		ret = DDNS_IPUPDATE_OK;
	} while(0);


	if (NULL != m_sendInfo.pData)
	{
		delete [] m_sendInfo.pData;
		m_sendInfo.pData = NULL;
	}

	return ret;
	
}




CYZSnapMan * CYZSnapMan::Instance()
{
	static CYZSnapMan s_yzSnapMan;
	return &s_yzSnapMan;
}

CYZSnapMan::CYZSnapMan()
{
	m_maxchnnNum=0;
}


CYZSnapMan::~CYZSnapMan()
{

}

bool CYZSnapMan::Initial(int maxchnnNum)
{
	m_maxchnnNum = maxchnnNum;
	m_bYZSnap = false;
	m_curSnapChannel = 0;
	m_lastSnapTime = 0;
	m_nMaxTime = 0;
	memset(&m_yzAscod, 0, sizeof(m_yzAscod));
	memset(&m_yzJpegServer, 0, sizeof(m_yzJpegServer));
	memset(&m_urlObject, 0, sizeof(m_urlObject));
	m_urlPort = 0;


	m_yzSendJpegID = PUB_THREAD_ID_NOINIT;
	m_bYZSendJpegProc = false;

	m_yzJpegSendClinet.Init();
	
	return true;
}


void CYZSnapMan::Quit()
{
	if (PUB_THREAD_ID_NOINIT != m_yzSendJpegID)
	{
		PUB_ExitThread(&m_yzSendJpegID, &m_bYZSendJpegProc);
	}

	POS pos = m_yzSendPicList.GetHeadPosition();
	while (pos != NULL)
	{
		YZ_SNAP_INFO node = m_yzSendPicList.RemoveHead();
		if (NULL != node.pData)
		{
			delete [] node.pData;
			node.pData = NULL;
		}
		pos = m_yzSendPicList.GetHeadPosition();
	}
	m_yzSendPicList.RemoveAll();

	m_yzJpegSendClinet.Quit();

}


bool CYZSnapMan::Start()
{
	m_yzSendJpegID = PUB_CreateThread(YZSendJpegThread, (void *)this, &m_bYZSendJpegProc);
	if( PUB_CREATE_THREAD_FAIL == m_yzSendJpegID )
	{
		printf("Initial YZSendJpegThread thread fail!");
		return false;
	}
	return true;
}


void CYZSnapMan::Stop()
{
	if (PUB_THREAD_ID_NOINIT != m_yzSendJpegID)
	{
		PUB_ExitThread(&m_yzSendJpegID, &m_bYZSendJpegProc);
	}

}

void CYZSnapMan::Action(bool bAction)
{
	m_lock.Lock();
	if (!bAction)
	{
		m_bYZSnap = false;
		m_curSnapChannel = 0;
		m_lastSnapTime = 0;
		POS pos = m_yzSendPicList.GetHeadPosition();
		while (pos != NULL)
		{
			YZ_SNAP_INFO node = m_yzSendPicList.RemoveHead();
			if (NULL != node.pData)
			{
				delete [] node.pData;
				node.pData = NULL;
			}
			pos = m_yzSendPicList.GetHeadPosition();
		}
		m_yzSendPicList.RemoveAll();
		for (int i = 0; i < m_maxchnnNum; i++)
		{
			CSnapCapabilityMan::Instance()->StopSnapJpeg(i,false);
		}	
	}
	else
	{
		m_bYZSnap = true;
		for (int i = 0; i < m_maxchnnNum; i++)
		{
			CSnapCapabilityMan::Instance()->StarSnapJpeg(i,false);
		}		
	}
	m_lock.UnLock();

}

void CYZSnapMan::SetYZSnapPara(YUANZHENG_SNAP_PARAM snapPara)
{
	m_lock.Lock();
	m_nMaxTime = snapPara.timeS;
	if ((snapPara.timeS > 0) && (snapPara.timeS < 5))
	{
		m_nMaxTime = 5;
	}
	snprintf(m_yzJpegServer, sizeof(m_yzJpegServer), "%s", snapPara.ipUrl);
	GetURLAndPort(snapPara.ipUrl, m_yzJpegServer, m_urlObject, m_urlPort);

	m_lock.UnLock();
}

void CYZSnapMan::SetYuanZhengParam(YUANZHENG_PARAM param)
{
	m_lock.Lock();
	memcpy(m_yzAscod, param.asCode, sizeof(m_yzAscod));
	m_lock.UnLock();
}

bool CYZSnapMan::SnapPicture(unsigned char *buf, int nLen, int nChannel, LONGLONG time)
{
	m_lock.Lock();

	if (!m_bYZSnap)
	{
		m_lock.UnLock();
		return true;
	}

	if (0 == m_nMaxTime)
	{
		m_lock.UnLock();
		return true;
	}

	if (0 == m_urlPort)
	{
		m_lock.UnLock();
		return true;
	}

	if ((0 == strlen(m_yzJpegServer)) || (0 == strlen(m_urlObject)) || (0 == strlen(m_yzAscod)))
	{
		m_lock.UnLock();
		return true;
	}

	if (m_curSnapChannel != nChannel)
	{
		m_lock.UnLock();
		return true;
	}

	bool bSnap = false;
	if (time < m_lastSnapTime)
	{
		bSnap = true;
	}
	else
	{
		unsigned long dd = ((time - m_lastSnapTime)/1000000);
		if(dd < m_nMaxTime)
		{
			bSnap = false;
		}
		else
		{
			bSnap = true;
		}
	}

	if (!bSnap)
	{
		m_lock.UnLock();
		return true;
	}

	//printf("chnn:%d,time:",nChannel);
	//PrintfBit64(time, false);
	YZ_SNAP_INFO node;
	memset(&node, 0, sizeof(YZ_SNAP_INFO));
	node.channel = nChannel;
	node.time = time;
	node.length = nLen;
	node.pData = new unsigned char [nLen];
	memcpy(node.pData, buf, nLen);
	
	m_yzSendPicList.AddTail(node);
    
	m_curSnapChannel = (m_curSnapChannel + 1)%m_maxchnnNum;
	m_lastSnapTime = time;
	m_lock.UnLock();
	return true;	

}

PUB_THREAD_RESULT PUB_THREAD_CALL CYZSnapMan::YZSendJpegThread(void *pParam)
{
#ifdef	__ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CYZSnapMan *pManager = reinterpret_cast<CYZSnapMan *>(pParam);
	pManager->YZSendJpegProc();
	return 0;
}



void CYZSnapMan::YZSendJpegProc()
{	
	printf("CYZSnapMan::YZSendJpegProc()\n");
	while (m_bYZSendJpegProc)
	{
		m_lock.Lock();
		if (m_yzSendPicList.GetCount() <= 0)
		{
			m_lock.UnLock();
			PUB_Sleep(2000);
			continue;
		}

		if ((100 < m_yzSendPicList.GetCount()) || (0 == m_urlPort) || (0 == strlen(m_yzJpegServer)) || (0 == strlen(m_urlObject)) || (0 == strlen(m_yzAscod)))
		{
			POS pos = m_yzSendPicList.GetHeadPosition();
			while (pos != NULL)
			{
				YZ_SNAP_INFO node = m_yzSendPicList.RemoveHead();
				if (NULL != node.pData)
				{
					delete [] node.pData;
					node.pData = NULL;
				}
				pos = m_yzSendPicList.GetHeadPosition();
			}
			m_yzSendPicList.RemoveAll();
			m_lock.UnLock();
			PUB_Sleep(2000);
			continue;
		}

		YZ_SNAP_INFO node = m_yzSendPicList.RemoveHead();
		m_yzJpegSendClinet.SetSendPara(node, m_yzAscod, m_yzJpegServer, m_urlObject, m_urlPort);
		m_lock.UnLock();

		m_yzJpegSendClinet.DdnsIPupdate(NULL,NULL,NULL);
		PUB_Sleep(2000);
	}
	
}


