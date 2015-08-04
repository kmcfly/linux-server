#include "DdnsVisionica.h"
#include "NetInterface.h"

CDdnsVisionica::CDdnsVisionica()
{

}

CDdnsVisionica::~CDdnsVisionica()
{

}

unsigned long CDdnsVisionica::RegisterAccount(const char *pUserName,const char *pmyDomainName)
{
	unsigned long retval=  RegisterORUpdate(pUserName, pmyDomainName, DDNS_VISIONICA_MODE_REGISTER);
	if (DDNS_IPUPDATE_OK == retval)
	{
		return DDNS_IPUPDATE_REGISTER_SUCC;
	}
	else
	{
		return DDNS_IPUPDATE_REGISTER_FAIL;
	}
	
}

unsigned long CDdnsVisionica::DdnsIPupdate( const char *pUserName, const char *pPassword, const char *pmyDomainName )
{
	return RegisterORUpdate(pUserName, pmyDomainName, DDNS_VISIONICA_MODE_SIMPLE_UPDATE);
}


int CDdnsVisionica::ConnectToServer(const char *pServer, unsigned short serverPort)
{
	if (NULL == pServer)
	{
		assert(false);
		return -1;
	}

#if USE_LOCAL_DNSRESOLVE
	struct hostent *phstt = CNetInterface::Instance()->GetHostByName(pServer);
#else
	struct hostent *phstt = gethostbyname(pServer);

#endif
	if(phstt == NULL) 
	{
		perror("gethostbyname()");
		printf("%s:%s:%d, NOIP_DDNS_SERVER=%s\n", __FUNCTION__, __FILE__, __LINE__, pServer);
		return -1;
	}
	int sockfd = -1;
	struct sockaddr_in ser_addr;

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(serverPort);
	memcpy(&(ser_addr.sin_addr), *((struct in_addr **)phstt->h_addr_list), sizeof(struct in_addr));
	memset(ser_addr.sin_zero,0, sizeof(ser_addr.sin_zero));

#if USE_LOCAL_DNSRESOLVE
	CNetInterface::Instance()->ReleaseHost( phstt );
#endif

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if ( -1 == sockfd)
	{
		perror("socket()");
		printf("%s:%s:%d\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return -1;
	}


	if(ConnectServer(sockfd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr), CONNECT_TIME_OUNT) != 0)
	{
		perror("connect()");
		printf("%s:%s:%d,连接服务器%s失败\n", __FUNCTION__, __FILE__, __LINE__,pServer);
		close(sockfd);
		return -1;
	}
	
	return sockfd;
}

unsigned long CDdnsVisionica::RegisterORUpdate(const char *pUserName, const char *pmyDomainName, DDNS_VISIONICA_MODE mode)
{
	if ( NULL == pUserName)
	{
		return DDNS_IPUPDATE_ENAMEPWD;
	}

	if (NULL == pmyDomainName)
	{
		return DDNS_IPUPDATE_EHOSTDOMAIN_EMPTY;
	}


	char szIP[16] = {0};
	if (-1 == GetExternalIp(szIP))
	{
		printf("%s:%s:%d, 获取外网IP失败\n", __FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	char *pCmdBuf = new char [1024];
	assert(NULL != pCmdBuf);
	memset(pCmdBuf, 0, 1024);
	snprintf(pCmdBuf, 1024, "GET /nic/update?mode=%d&hostname=%s&myip=%s&platform=Linux&email=%s&devicemodel=TVT-1600B&domain=visionica.info HTTP/1.1\r\n\n", mode, pmyDomainName,szIP,pUserName);
	//printf("%s:%s:%d, pCmdBuf=%s\n", __FUNCTION__, __FILE__, __LINE__, pCmdBuf);


	int sockfd = ConnectToServer(VISIONICA_DDNS_SERVER, VISIONICA_DDNS_PORT); 
	if ( -1 == sockfd)
	{
		printf("%s:%s:%d, 连接到服务器失败\n", __FUNCTION__, __FILE__, __LINE__);
		delete [] pCmdBuf;
		pCmdBuf = NULL;
		return DDNS_IPUPDATE_ECONNECT;
	}

	if(send(sockfd, pCmdBuf, strlen(pCmdBuf), 0) == -1)
	{
		perror("send()");
		if( NULL != pCmdBuf)
		{
			delete [] pCmdBuf;
			pCmdBuf = NULL;
		}
		printf("%s:%s:%d, 发送数据失败\n", __FUNCTION__, __FILE__, __LINE__);
		close(sockfd);
		return DDNS_IPUPDATE_EUNKNOWN;
	}


	char *pReceiveBuf = new char [1024];
	assert( NULL != pReceiveBuf);
	memset(pReceiveBuf, 0, 1024);
	int len = 1024;

	GetMessageFromServer(sockfd,pReceiveBuf,len);
	//printf("接收到的应答为:%s\n",pReceiveBuf);
	close(sockfd);

	unsigned long retval = DDNS_IPUPDATE_EUNKNOWN;
	if ( 0 < len )
	{
		if(strstr(pReceiveBuf,"OK:"))
		{
			retval =  DDNS_IPUPDATE_OK;
		}

		if(strstr(pReceiveBuf,"good"))
		{
			retval =  DDNS_IPUPDATE_OK;
		}
		if(strstr(pReceiveBuf,"nochg"))
		{
			retval = DDNS_IPUPDATE_OK;
		}
		if (strstr(pReceiveBuf ,"BAD REQUEST"))
		{
			retval = DDNS_IPUPDATE_EUNKNOWN;
		}
		if (strstr(pReceiveBuf, "Unknown Error"))
		{
			retval = DDNS_IPUPDATE_EHOSTDOMAIN;
		}
		if(strstr(pReceiveBuf,"dnserr"))
		{
			retval = DDNS_IPUPDATE_ENAMEPWD;
		}
		if (strstr(pReceiveBuf ,"911"))
		{
			retval = DDNS_IPUPDATE_EUNKNOWN;
		}
	}


	if( NULL != pCmdBuf)
	{
		delete [] pCmdBuf;
		pCmdBuf = NULL;
	}

	if (NULL != pReceiveBuf)
	{
		delete [] pReceiveBuf;
		pReceiveBuf = NULL;
	}

	return retval;
}
