#include "DdnsMintAuto.h"

#if defined(__CUSTOM_IN23__)
#include "encrypt_IN23.h"
#else
#include "encrypt.h"
#endif

CDdnsMintAuto::CDdnsMintAuto()
{
	m_bufLen	= 2048;
	m_pSendBuf	= new char [m_bufLen];
	m_pRecvBuf	= new char [m_bufLen];
	m_pTempBuf	= new char [m_bufLen];
	
	memset(m_pSendBuf, 0, m_bufLen);
	memset(m_pRecvBuf, 0, m_bufLen);
	memset(m_pTempBuf, 0, m_bufLen);

	m_sockfd = -1;
}

CDdnsMintAuto::~CDdnsMintAuto()
{
	if (NULL != m_pSendBuf)
	{
		delete [] m_pSendBuf;
	}

	if (NULL != m_pRecvBuf)
	{
		delete [] m_pRecvBuf;
	}

	if (NULL != m_pTempBuf)
	{
		delete [] m_pTempBuf;
	}

	Close();
}

unsigned long CDdnsMintAuto::DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName)
{
	char exIp[60] = {0};

	GetExternalIp(exIp);

	//static unsigned char j=10;

	//j++;

	//snprintf(exIp, 60, "210.21.229.%d", j);

	unsigned long ret = Connect(m_DdnsServerName);
	if (0 != ret)
	{
		return ret;
	}

	ret = Update(exIp);

	return ret;
}

unsigned long CDdnsMintAuto::DdnsAutoReg(const char *pmyDomainName)
{
	char exIp[60] = {0};

	GetExternalIp(exIp);
 	unsigned long ret = Connect(m_DdnsServerName);
 	if (0 != ret)
 	{
 		return ret;
 	}
#ifndef __CUSTOM_IN23__
 	RemoveDm();
#endif
 	ret = AddDm(pmyDomainName, exIp);

	return ret;
}

int CDdnsMintAuto::Connect(const char * pSeverName)
{
#if USE_LOCAL_DNSRESOLVE
	struct hostent *phstt = CNetInterface::Instance()->GetHostByName(pSeverName);
#else
	struct hostent *phstt = gethostbyname(pSeverName);
	//DWORD dwError= WSAGetLastError();
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

	Close();

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

int CDdnsMintAuto::Process(const char * pSendBuf, unsigned long sendLen)
{
	int retValue = -1;
	if(-1 == send(m_sockfd, pSendBuf, sendLen, 0))
	{
		printf("%s:%s:%d, Send data error\n", __FUNCTION__, __FILE__, __LINE__);
		return retValue;
	}

	int recvLen = m_bufLen;
	GetMessageFromServer(m_sockfd, m_pRecvBuf, recvLen);
	m_pRecvBuf[strlen(m_pRecvBuf)] = '\0';

	char * pHeadEnd = strstr(m_pRecvBuf, "\r\n\r\n");
	if (NULL != pHeadEnd)
	{
		sscanf(pHeadEnd, "\r\n\r\n%d", &retValue);
	}

	return retValue;
}

void CDdnsMintAuto::Close()
{
	if (-1 != m_sockfd)
	{
#ifndef WIN32
		close(m_sockfd);
#else
		closesocket(m_sockfd);
#endif
		m_sockfd = -1;
	}
}

void CDdnsMintAuto::SetMac(const char * pMac)
{
	if (NULL != pMac)
	{
		char macTemp[120] = {0};
		strcpy(macTemp, pMac);

#if defined(__CUSTOM_IN23__)
		char enMac[MAX_CRYPT_BUFF] = {0};
		CCrypt macCrypt;
		macCrypt.EncryptBuffer((unsigned char *)macTemp, strlen(macTemp), (unsigned char *)enMac);
#else
		char enMac[MAX_SIZE] = {0};
		MACCrypt macCrypt;
		macCrypt.CryptBuffer((unsigned char *)macTemp, strlen((const char *)macTemp), (unsigned char *)enMac);
#endif
		base64Encode(enMac, m_mac);
	}
}

bool CDdnsMintAuto::RemoveDDNS()
{
#if defined(__CUSTOM_IN23__)
	char exIp[60] = {0};

	GetExternalIp(exIp);

	unsigned long ret = Connect(m_DdnsServerName);
	if (0 != ret)
	{
		return false;
	}

	if(0 == RemoveDm())
	{
		return true;
	}
#endif
	return false;
}
int CDdnsMintAuto::AddDm(const char * pDomainName, const char * pExIp, const char *pUser/*="admin"*/, const char * pPass/*="password"*/)
{
	/*
	"GET /add.aspx?mac=xaHFuOKAmOKAmeKAuuKAmOKAmeKAuuKAmOKAuuKAuuKAmOKAuuKAuuKAmOKAuuKEog%3d%3d&dm=sunny-test&dcod=12&oemid=456&myip=210.21.229.138 HTTP/1.1"
	*/
	//发送添加请求
	snprintf(m_pTempBuf, m_bufLen, "GET /add.aspx?mac=%s&dm=%s&dcod=12&oemid=456&myip=%s HTTP/1.1", m_mac, pDomainName, pExIp);
	
	snprintf(m_pSendBuf, m_bufLen, 
		"%s\r\n"
		"Accept: text/html, application/xhtml+xml, */*\r\n"
		"Accept-Language: zh-CN\r\n"
		"User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)\r\n"
		"Accept-Encoding: gzip, deflate\r\n"
		"Host: autoddns.com\r\n"
		"Connection: Keep-Alive\r\n"
		"Cookie: CookieCheck=Enabled=True; DDNSADMIN=user=admin&pass=cGFzc3dvcmQ=\r\n"
		"\r\n",
		m_pTempBuf/*, pUser, pPass*/);

	int retCode = Process(m_pSendBuf, strlen(m_pSendBuf));

	//   901 = Invalid hostname
	//   902 = Invalid MAC
	//   903 = Registration Blocked (Unapproved Mac)
	//   904 = Invalid IP
	//   905 = Invalid Device ID
	//   906 = Invalid OEM ID
	//   907 = Hostname already in use
	//   908 = Update with no change
	//   909 = Update with records modified (Write new info to file etc...)
	//   910 = Domain Created
	//   911 = Flood Detected
	//   912 = Server Offline
	switch (retCode)
	{
	case 901:
		{
			//域名错
			return DDNS_IPUPDATE_EHOSTDOMAIN;
		}
		break;
	case 902:	//MAC地址错，可能是加密错，或者是BASE64编码错
	case 903:	//MAC地址错误，服务器不允许这个MAC地址
		{
			//MAC地址出错，服务器不允许这个MAC地址
			return DDNS_IPUPDATE_EUNKNOWN;
		}
		break;
	case 904:
		{
			//IP错误
			return DDNS_IPUPDATE_IP_ERROR;
		}
		break;
	case 905:
	case 906:
		{
			return DDNS_IPUPDATE_EUNKNOWN;
		}
		break;
	case 907:
		{
			//域名已经存在
			return DDNS_AUTOREG_DOMAIN_ALREADY_USE;
		}
		break;
	case 908:
		{
			return DDNS_IPUPDATE_OK;//域名更新成功
		}
		break;
	case 909:
		{
			return DDNS_AUTOREG_DOMAIN_REPLACE_OK;
		}
		break;
	case 910:
		{
			return DDNS_AUTOREG_DOMAIN_CREATE_OK;
		}
		break;
	case 911:
	case 912:
		{
			return DDNS_IPUPDATE_EUNKNOWN;
		}
		break;
	default:
		break;
	}

	return 0;
}

int CDdnsMintAuto::RemoveDm()
{
	/*
	http://supercircuits.contosso.com/remove.aspx?mac=xaHFuOKAmOKAmeKAuuKAmOKAmeKAuuKAmOKAuuKAuuKAmOKAuuKAuuKAmOKAuuKEog%3d%3d
	*/

	//发送删除情况

	snprintf(m_pSendBuf, m_bufLen, 
		"GET /remove.aspx?mac=%s HTTP/1.1\r\n"
		"Accept: text/html, application/xhtml+xml, */*\r\n"
		"Accept-Language: zh-CN\r\n"
		"User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)\r\n"
		"Accept-Encoding: gzip, deflate\r\n"
		"Host: autoddns.com\r\n"
		"Connection: Keep-Alive\r\n"
		"Cookie: CookieCheck=Enabled=True; DDNSADMIN=user=admin&pass=cGFzc3dvcmQ=\r\n"
		"\r\n",
		m_mac/*, pUser, pPass*/);

	int retCode = Process(m_pSendBuf, strlen(m_pSendBuf));
	//    902 = Invalid MAC
	//    903 = Mac not allowed
	//    701 = No Record Found
	//    601 = Successful Delete
	//    912 = Server Error

	switch (retCode)
	{
	case 601:
		{
			//删除成功
			return 0;
		}
		break;
	case 701:
		{
			//没有发现指定的MAC
			return DDNS_IPUPDATE_EHOSTDOMAIN;
		}
		break;
	case 902:
	case 903:	
		{
			//错误的MAC
			return DDNS_IPUPDATE_EUNKNOWN;
		}
	case 912:
		{
			//服务器错误
			return DDNS_IPUPDATE_EUNKNOWN;
		}
		break;
	}

	return 0;
}

int CDdnsMintAuto::Update(const char * pExIp)
{
	/*
	http://supercircuits.contosso.com/update.aspx?mac=xaHFuOKAmOKAmeKAuuKAmOKAmeKAuuKAmOKAuuKAuuKAmOKAuuKAuuKAmOKAuuKEog%3d%3d&myip=24.116.98.21
	*/
	//外网IP变化是发送更新请求
	snprintf(m_pSendBuf, m_bufLen, 
		"GET /update.aspx?mac=%s&myip=%s HTTP/1.1\r\n"
		"Accept: text/html, application/xhtml+xml, */*\r\n"
		"Accept-Language: zh-CN\r\n"
		"User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)\r\n"
		"Accept-Encoding: gzip, deflate\r\n"
		"Host: autoddns.com\r\n"
		"Connection: Keep-Alive\r\n"
		"Cookie: CookieCheck=Enabled=True; DDNSADMIN=user=admin&pass=cGFzc3dvcmQ=\r\n"
		"\r\n",
		m_mac, pExIp/*, pUser, pPass*/);

	int retCode = Process(m_pSendBuf, strlen(m_pSendBuf));

	//    902 = Invalid MAC
	//    904 = Invalid IP
	//    802 = No Record Found
	//    801 = Successful Update
	//    803 = Flood Detected
	//    909 = Server Error

	switch (retCode)
	{
	case 801:
		{
			//更新成功
			return 0;
		}
		break;
	case 802:
		{
			//MAC地址不存在
			return DDNS_IPUPDATE_EHOSTDOMAIN;
		}
		break;
	case 803:
		{
			//更新太频繁
			return DDNS_IPUPDATE_EUNKNOWN;
		}
		break;
	case 902:
		{
			return DDNS_IPUPDATE_EHOSTDOMAIN;
		}
		break;
	case 904:
		{
			return DDNS_IPUPDATE_IP_ERROR;
		}
		break;
	case 909:
		{
			//服务器错误
			return DDNS_IPUPDATE_EUNKNOWN;
		}
		break;
	}

	return 0;
}
