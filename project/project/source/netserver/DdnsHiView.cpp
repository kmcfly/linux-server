#include "DdnsHiView.h"

CDdnsHiView::CDdnsHiView()
{
	m_bufLen = 4096;
	m_pSendBuf = new char [m_bufLen];
	m_pRecvBuf = new char [m_bufLen];

	m_sockfd = -1;
}

CDdnsHiView::~CDdnsHiView()
{
	delete [] m_pSendBuf;
	delete [] m_pRecvBuf;
}

unsigned long CDdnsHiView::DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName)
{
	//连接服务器
	unsigned long ret = Connect(m_DdnsServerName);
	if (ret)
	{
		return ret;
	}

	//GET /dns?hostname=<hostname>.hiview.org&username=<username>&password=<password>
	//Host:www.hiview.org
	snprintf(m_pSendBuf, m_bufLen, 
		"GET /dns?hostname=%s.hiview.org&username=%s&password=%s HTTP/1.1\r\n"
		"Host:www.hiview.org\r\n"
		"\r\n", pmyDomainName, pUserName, pPassword);

	ret = DDNS_IPUPDATE_EUNKNOWN;

	if(-1 != send(m_sockfd, m_pSendBuf, strlen(m_pSendBuf), 0))
	{
		int recvLen = m_bufLen;
		memset(m_pRecvBuf, 0, m_bufLen);
		GetMessageFromServer(m_sockfd, m_pRecvBuf, recvLen);
		m_pRecvBuf[strlen(m_pRecvBuf)] = '\0';

		char * pHeadEnd = strstr(m_pRecvBuf, "\r\n\r\n");
		if (NULL != pHeadEnd)
		{
			if(strstr(pHeadEnd,"good"))
			{
				printf("( %s:%s:%d )	dyndns.com ip update:OK!\n", __FILE__, __FUNCTION__, __LINE__ );
				ret = DDNS_IPUPDATE_OK;
			}
			else if(strstr(pHeadEnd,"badauth"))
			{
				printf("( %s:%s:%d )	dyndns.com ip update:Bad authorization (username or password)!\n", __FILE__, __FUNCTION__, __LINE__ );
				ret = DDNS_IPUPDATE_ENAMEPWD;
			}
			else
			{
			}
		}
	}

	if (-1 != m_sockfd)
	{
#ifndef WIN32
		close(m_sockfd);
#else
		closesocket(m_sockfd);
#endif
		m_sockfd = -1;
	}

	return ret;
}

int CDdnsHiView::Connect(const char * pSeverName)
{
#if USE_LOCAL_DNSRESOLVE
	struct hostent *phstt = CNetInterface::Instance()->GetHostByName(pSeverName);
#else
	struct hostent *phstt = gethostbyname(pSeverName);
#endif
	if (NULL == phstt)
	{
		printf("%s:%s:%d, 不能解析域名:%s\n", __FUNCTION__, __FILE__, __LINE__,pSeverName);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	struct sockaddr_in ser_addr;
	memset(&ser_addr, 0, sizeof(struct sockaddr_in));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port	=  htons(80);
	memcpy(&(ser_addr.sin_addr), *((struct in_addr **)phstt->h_addr_list), sizeof(struct in_addr));
	memset(ser_addr.sin_zero,0, sizeof(ser_addr.sin_zero));

#if USE_LOCAL_DNSRESOLVE
	CNetInterface::Instance()->ReleaseHost(phstt);
#endif

	m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == m_sockfd)
	{
		printf("%s:%s:%d, create socket error\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	if(ConnectServer(m_sockfd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr), CONNECT_TIME_OUNT) != 0)
	{
		printf("%s:%s:%d, connect socket error\n", __FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	return 0;
}
