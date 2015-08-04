
#include "DdnsNoip.h"

#include "NetInterface.h"


CDdnsNoip::CDdnsNoip()
{

}

CDdnsNoip::~CDdnsNoip()
{
}

unsigned long CDdnsNoip::DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName )
{
	unsigned long retval = DDNS_IPUPDATE_ENAMEPWD;

	if ( (NULL == pUserName) || (0 == strlen(pUserName)))
	{
		assert(false);
		printf("%s:%s:%d, 用户名为空\n", __FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_ENAMEPWD;
	}

	if ( (NULL == pmyDomainName) || (0 == strlen(pmyDomainName)))
	{
		assert(false);
		printf("%s:%s:%d, 动态域名为空\n", __FUNCTION__, __FILE__, __LINE__);
		return  DDNS_IPUPDATE_EHOSTDOMAIN;
	}

	char base64src[128] = {0};
	char encodePwd[128] = {0};
	snprintf(base64src, sizeof(base64src), "%s:%s", pUserName, pPassword);
	base64Encode(base64src, encodePwd);

	char szIP[16] = {0};
	if( -1 == GetExternalIp(szIP) )
	{
		printf("%s:%s:%d, 获取外网IP失败\n", __FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	char *pCmdBuf = new char [1024];
	snprintf(pCmdBuf,1024, "GET /nic/update?hostname=%s&myip=%s HTTP/1.0\r\nHost: dynupdate.no-ip.com\r\nAuthorization: Basic %s\r\nUser-Agent: tvt-dvr-1.0\r\n\r\n",pmyDomainName,szIP,encodePwd);

#if USE_LOCAL_DNSRESOLVE
	struct hostent *phstt = CNetInterface::Instance()->GetHostByName(NOIP_DDNS_SERVER);
#else
	struct hostent *phstt = gethostbyname(NOIP_DDNS_SERVER);

#endif
	if(phstt == NULL) 
	{
		perror("gethostbyname()");
		printf("%s:%s:%d, NOIP_DDNS_SERVER=%s\n", __FUNCTION__, __FILE__, __LINE__, NOIP_DDNS_SERVER);

		if( NULL != pCmdBuf)
		{
			delete [] pCmdBuf;
			pCmdBuf = NULL;
		}

		return DDNS_IPUPDATE_EUNKNOWN;
	}
	int sockfd = -1;
	struct sockaddr_in ser_addr;
	struct servent *pServent = getservbyname("http", "tcp");

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = pServent->s_port;
	memcpy(&(ser_addr.sin_addr), *((struct in_addr **)phstt->h_addr_list), sizeof(struct in_addr));
	memset(ser_addr.sin_zero,0, sizeof(ser_addr.sin_zero));

#if USE_LOCAL_DNSRESOLVE
	CNetInterface::Instance()->ReleaseHost( phstt );
#endif

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(ConnectServer(sockfd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr), CONNECT_TIME_OUNT) != 0)
	{
		perror("connect()");
		printf("%s:%s:%d,连接服务器%s失败\n", __FUNCTION__, __FILE__, __LINE__,NOIP_DDNS_SERVER);
		close(sockfd);
		
		if( NULL != pCmdBuf)
		{
			delete [] pCmdBuf;
			pCmdBuf = NULL;
		}

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

	close(sockfd);

	if ( 0 < len )
	{
		if(strstr(pReceiveBuf,"good"))
		{
			retval =  DDNS_IPUPDATE_OK;
		}
		if(strstr(pReceiveBuf,"nochg"))
		{
			retval = DDNS_IPUPDATE_OK;
		}
		if(strstr(pReceiveBuf,"badauth"))
		{
			retval = DDNS_IPUPDATE_ENAMEPWD;
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


