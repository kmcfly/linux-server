#include "DdnsTecvoz.h"


#ifdef __ENVIRONMENT_LINUX__
#include "NetInterface.h"
#include "NetProtocol.h"
#include "Product.h"
#include "LocalDevice.h"
#endif


CDdnsTecvoz::CDdnsTecvoz()
{
	memset(m_cmdBuf, 0, sizeof(m_cmdBuf));
	memset(m_encodePwd, 0, sizeof(m_encodePwd));

	char macTmp[8] = {0};
#ifdef __ENVIRONMENT_LINUX__
	CNetInterface::Instance()->GetMAC(macTmp);
#endif
		
	char szTemp1[36] = {0};
	char szTemp2[36] = {0};
	char szHardwareVersion[36] = {0};
	char szKernelVersion[64] = {0};
	char szMCUVersion[36] = {0};

	CProduct::Instance()->GetHardwarePreVersion(szTemp1, 36);
	CLocalDevice::GetHardWareVersion(szTemp2, 36);				
	snprintf(szHardwareVersion, 36, "%s-%s", szTemp1, szTemp2);
	CLocalDevice::GetKernelVersion(szKernelVersion, 64);
	CLocalDevice::Instance()->GetMCUVersion(szMCUVersion, 36);		
	memset(&m_productInfo, 0, sizeof(PRODUCT_INFO));
	CProduct::Instance()->GetProductInfo(m_productInfo, macTmp, szHardwareVersion, szKernelVersion, szMCUVersion);
	
	snprintf(m_deviceMac, sizeof(m_deviceMac), "%02x%%3A%02x%%3A%02x%%3A%02x%%3A%02x%%3A%02x", macTmp[0], macTmp[1], macTmp[2], macTmp[3], macTmp[4], macTmp[5]);

	m_httpPort = 0;
	m_serverPort = 0;
}

CDdnsTecvoz::~CDdnsTecvoz()
{
}

unsigned long CDdnsTecvoz::DdnsIPupdate(const char *pUserName, const char *pPassword , const char *pDdnsServer)
{
#if USE_LOCAL_DNSRESOLVE
	struct hostent *phstt = CNetInterface::Instance()->GetHostByName(m_DdnsServerName);
#else
	struct hostent *phstt = gethostbyname(m_DdnsServerName);
	DWORD dwError= WSAGetLastError();
#endif
	if (NULL == phstt)
	{
		printf("gethostbyname() error\n");

		printf("%s:%s:%d, 不能解析域名:%s\n", __FUNCTION__, __FILE__, __LINE__, m_DdnsServerName);
		return DDNS_IPUPDATE_EUNKNOWN;//无法解析得到ServerIP
	}

	struct sockaddr_in ser_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port =  htons(80);
	memcpy(&(ser_addr.sin_addr), *((struct in_addr **)phstt->h_addr_list), sizeof(struct in_addr));
	memset(ser_addr.sin_zero,0, sizeof(ser_addr.sin_zero));

#if USE_LOCAL_DNSRESOLVE
	CNetInterface::Instance()->ReleaseHost(phstt);
#endif

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if( -1 == sockfd )
	{
		perror(" socket() error\n");
		assert( false );
		return DDNS_IPUPDATE_EUNKNOWN;
	}


	//link to host
	if(ConnectServer(sockfd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr), CONNECT_TIME_OUNT) != 0)
	{
		perror("connect()");

#ifndef WIN32
		close(sockfd);
#else
		closesocket(sockfd);
#endif
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	//GET /nic/update?myip=210.21.229.138&hostname=test1.tecvozddns.com.br&codigo=Tecvoz&model=wd894ab&mac=AB%3ACD%3AEF%3A01%3A02%3A03canais=16canais&firmware=2113.3aa&portahttp=8080&portartsp=554&portaweb=8081&portamobile=8082 HTTP/1.1
	//Accept: text/html, application/xhtml+xml, */*
	//Accept-Language: zh-CN
	//User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)
	//Accept-Encoding: gzip, deflate
	//Host: members.tecvozddns.com.br
	//Connection: Keep-Alive
	//Authorization: Basic user:pass(base64)
	char auth[200] = {0};
	snprintf(auth, sizeof(auth), "%s.tecvozddns.com.br:%s", pUserName, pPassword);
	base64Encode(auth, m_encodePwd);
	
	char ipaddr[16] = {0};
	if(-1 == GetExternalIp(ipaddr))
	{
		printf("%s:%s:%d, 获取外网IP失败\n", __FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	memset(m_cmdBuf, 0, sizeof(m_cmdBuf));


	//获取产品名称
	GetDeviceName(CProduct::Instance()->ProductType(), m_deviceName);

	printf("%s:%d, device name=%s, mac=%s\n", __FILE__, __LINE__, m_deviceName, m_deviceMac);

	//RTSP端口如果有需要加上，如果没有就去掉
	//手机端口和数据端口一样，可以去掉
	snprintf(m_cmdBuf,(sizeof(m_cmdBuf) - 1),"GET /nic/update?myip=%s&hostname=%s.tecvozddns.com.br&codigo=Tecvoz&model=%s&mac=%s&canais=%dcanais&firmware=%s&portahttp=%d&portartsp=0&portaservidor=%d&portamobile=0 HTTP/1.1\r\n"
		"Accept: text/html, application/xhtml+xml, */*\r\n"
  		"Accept-Language: zh-CN\r\n"
  		"User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)\r\n"
  		"Accept-Encoding: gzip, deflate\r\n"
  		"Host: members.tecvozddns.com.br\r\n"
		"Authorization: Basic %s\r\n\r\n", ipaddr, pUserName, m_deviceName, m_deviceMac, m_productInfo.videoInputNum, m_productInfo.firmwareVersion, m_httpPort, m_serverPort, m_encodePwd);

	if(send(sockfd, m_cmdBuf, strlen(m_cmdBuf), 0) == -1)
	{
		perror("send() data to server");
#ifndef WIN32
		close(sockfd);
#else
		closesocket(sockfd);
#endif
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	// receive
	char receiveBuf[1000] = {0};
	int len = sizeof(receiveBuf);

	int ret = GetMessageFromServer(sockfd, receiveBuf, len);

#ifndef WIN32
	close(sockfd);
#else
	closesocket(sockfd);
#endif

	if(ret != DDNS_IPUPDATE_OK)
	{
		return ret;
	}

	//www.dyndns.com的返回值，参看https://www.dyndns.com/developers/specs/return.html
	if(strstr(receiveBuf,"good"))
	{
		printf("( %s:%s:%d )	%s ip update:OK!\n", __FILE__, __FUNCTION__, __LINE__, m_DdnsServerName);
		return DDNS_IPUPDATE_OK;
	}

	if(strstr(receiveBuf,"nochg"))
	{
		printf("( %s:%s:%d )	%s ip update:No changes, update considered abusive!\n", __FILE__, __FUNCTION__, __LINE__, m_DdnsServerName);		
		return DDNS_IPUPDATE_OK;
	}

	if(strstr(receiveBuf,"badauth"))
	{
		printf("( %s:%s:%d )	%s ip update:Bad authorization (username or password)!\n", __FILE__, __FUNCTION__, __LINE__, m_DdnsServerName);
		return DDNS_IPUPDATE_ENAMEPWD;
	}

	if(strstr(receiveBuf,"donator"))
	{
		printf("(%s:%s:%d)  %s ip update:An option available only to credited users (such as offline URL) was specified, but the user is not a credited user.\n", __FUNCTION__, __FILE__, __LINE__, m_DdnsServerName);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	if(strstr(receiveBuf,"nohost"))
	{
		printf("(%s:%s:%d)  %s ip update:The hostname specified does not exist in this user account \n", __FUNCTION__, __FILE__, __LINE__, m_DdnsServerName);
		//找不到主机域名
		return DDNS_IPUPDATE_NOHOST;
	}

	if(strstr(receiveBuf,"notfqdn"))
	{
		printf("(%s:%s:%d)  %s ip update:The hostname specified is not a fully-qualified domain name(not in the form hostname.dyndns.org or domain.com). \n", __FUNCTION__, __FILE__, __LINE__, m_DdnsServerName);
		//
		return DDNS_IPUPDATE_NOTFQDN;
	}

	if(strstr(receiveBuf,"numhost"))
	{
		printf("(%s:%s:%d)  %s ip update .Too many hosts (more than 20) specified in an update. Also returned if trying to update a round robin (which is not allowed)\n", __FUNCTION__, __FILE__, __LINE__, m_DdnsServerName);
		//
		return DDNS_IPUPDATE_NUMHOST;
	}

	if(strstr(receiveBuf,"abuse"))
	{
		printf("(%s:%s:%d)  %s ip update .The hostname is blocked for abuse, you must unblock from the www.dyndns.com\n", __FUNCTION__, __FILE__, __LINE__, m_DdnsServerName);
		//在频繁访问dyndns.com服务器的时候会出现该错误,表明主机IP被www.dyndns.com屏蔽, 有可能是频繁update造成的
		return DDNS_IPUPDATE_ABUSE;
	}

	if(strstr(receiveBuf,"dnserr"))
	{
		printf("(%s:%s:%d)  dyndns.com ip update .The server error www.dyndns.com\n", __FUNCTION__, __FILE__, __LINE__);
		//服务器出问题
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	return DDNS_IPUPDATE_EUNKNOWN;
}

void CDdnsTecvoz::GetDeviceName(PRODUCT_TYPE productType, char * pDeviceName)
{
	switch (productType)
	{
	case TD_2304CDS:
		strcpy(pDeviceName, "TD-2304SS LIGHT");
		break;
	case TD_2308CDS:
		strcpy(pDeviceName, "TD-2308SS LIGHT");
		break;
	case TD_2316CDS:
		strcpy(pDeviceName, "TD-2316SS LIGHT");
		break;
	case TD_2304SE:
		strcpy(pDeviceName, "TD-2304SE PLUS");
		break;
	case TD_2308SE:
		strcpy(pDeviceName, "TD-2308SE PLUS");
		break;
	case TD_2316SE:
		strcpy(pDeviceName, "TD-2316SE PLUS");
		break;
	case TD_2304SE_S:
		strcpy(pDeviceName, "TD-2304SE PLUS");
		break;
	case TD_2304ME:
		strcpy(pDeviceName, "TD-2304ME PLUS");
		break;
	case TD_2308ME:
		strcpy(pDeviceName, "TD-2308ME PLUS");
		break;
	case TD_2316ME:
		strcpy(pDeviceName, "TD-2316ME PLUS");
		break;
	case TD_2404MD:
		strcpy(pDeviceName, "TD-2304MD FULL");
		break;
	case TD_2408MD:
		strcpy(pDeviceName, "TD-2308MD FULL");
		break;
	case TD_2416MD:
		strcpy(pDeviceName, "TD-2316MD FULL");
		break;
	case TD_2424MD:
		strcpy(pDeviceName, "TD-2324MD FULL");
		break;
	case TD_2432MD:
		strcpy(pDeviceName, "TD-2332MD FULL");
		break;
	case TD_2504FD:
		strcpy(pDeviceName, "TD-2504FD FULL D1");
		break;
	case TD_2508FD:
		strcpy(pDeviceName, "TD-2508FD FULL D1");
		break;
	case TD_2516FD:
		strcpy(pDeviceName, "TD-2516FD FULL D1");
		break;
	case TD_2304SS_B:
		strcpy(pDeviceName, "TD-2304SS LIGHT 2");
		break;
	case TD_2308SS_B:
		strcpy(pDeviceName, "TD-2308SS LIGHT 2");
		break;
	case TD_2316SS_B:
		strcpy(pDeviceName, "TD-2316SS LIGHT 2");
		break;
	case TD_2304SE_B:
		strcpy(pDeviceName, "TD-2304SE PLUS 2");
		break;
	case TD_2308SE_B:
		strcpy(pDeviceName, "TD-2308SE PLUS 2");
		break;
	case TD_2316SE_B:
		strcpy(pDeviceName, "TD-2316SE PLUS 2");
		break;
	case TD_2304ME_B:
		strcpy(pDeviceName, "TD-2304ME PLUS 2");
		break;
	case TD_2308ME_B:
		strcpy(pDeviceName, "TD-2308ME PLUS 2");
		break;
	case TD_2316ME_B:
		strcpy(pDeviceName, "TD-2316ME PLUS 2");
		break;
	case TD_2504HE_B:
		strcpy(pDeviceName, "TD-2504HE PLUS D1");
		break;
	case TD_2508HE_B:
		strcpy(pDeviceName, "TD-2508HE PLUS D1");
		break;
	case TD_2508HE_B_3521:
		strcpy(pDeviceName, "TD-2508HE PLUS D1");
		break;
	case TD_2516HE_B:
		strcpy(pDeviceName, "TD-2516HE PLUS D1");
		break;
	case TD_2516HE_B_N100:
		strcpy(pDeviceName, "TD-2516HE PLUS D1");
		break;
	case TD_2524HE_B:
		strcpy(pDeviceName, "TD-2524HE PLUS D1");
		break;
	case TD_2532HE_B:
		strcpy(pDeviceName, "TD-2532HE PLUS D1");
		break;
	case TD_2516HD_C:
		strcpy(pDeviceName, "TD-2516FD FULL D1W");
		break;
	case TD_2532HD_C:
		strcpy(pDeviceName, "TD-2432MD FULL D1W");
		break;
	case TD_2704XD_P:
		strcpy(pDeviceName, "TD-2704XE HD-SDI");
		break;
	default:
		strcpy(pDeviceName, "TD-XXXX");
		break;
	}
}
