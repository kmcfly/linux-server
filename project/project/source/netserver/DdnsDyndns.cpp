/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2010-05
** Name         : DdnsDyndns.cpp
** Version      : 1.0
** Description  : 
					www.dyndns.com的客户端
** Modify Record:
1:
***********************************************************************/
#include "DdnsDyndns.h"
#include "NetInterface.h"

CDdnsDyndns::CDdnsDyndns()
{

}

CDdnsDyndns::~CDdnsDyndns()
{

}

unsigned long CDdnsDyndns::DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName )
{
	char *DomainName = "members.dyndns.org";

	memset(m_CmdBuf,0,sizeof(m_CmdBuf));

	if(strlen(pUserName))
	{
		memset(m_Username,0,sizeof(m_Username));	
		strcpy(m_Username,pUserName);
	}
	else
	{
		assert(false);
		return DDNS_IPUPDATE_ENAMEPWD;
	}

	if(strlen(pPassword))
	{
		memset(m_Password,0,sizeof(m_Password));	
		strcpy(m_Password,pPassword);
	}
	else
	{
		//密码可以是空
	}

	if(strlen(pmyDomainName))
	{
		memset(m_HostName,0,sizeof(m_HostName));	
		strcpy(m_HostName,pmyDomainName);
	}
	else
	{
		//主机域名也可以是空
	}

	char base64src[512] ={0};
	snprintf(base64src,(sizeof(base64src)-1), "%s:%s",m_Username,m_Password);
	memset(m_EncodePwd,0,sizeof(m_EncodePwd));
	base64Encode(base64src, m_EncodePwd);

	return KeepLinkProc();
}




int CDdnsDyndns::KeepLinkProc()
{

	ULONG mask = 0,route = 0;
	char ipaddr[16] = {0};
	int ret = 0;

	if( -1 == GetExternalIp(ipaddr) )
	{
		printf("%s:%s:%d, 获取外网IP失败\n", __FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

#if USE_LOCAL_DNSRESOLVE
	struct hostent *phstt = CNetInterface::Instance()->GetHostByName(m_DdnsServerName);
#else
	struct hostent *phstt = gethostbyname(m_DdnsServerName);
	DWORD dwError= WSAGetLastError();
#endif
	if (NULL == phstt)
	{
	
		printf("gethostbyname() error\n");

		printf("%s:%s:%d, 不能解析域名:%s\n", __FUNCTION__, __FILE__, __LINE__,m_DdnsServerName);
		return DDNS_IPUPDATE_EUNKNOWN;//无法解析得到ServerIP
	}
	
	//	sprintf(m_CmdBuf,"GET /nic/update?hostname=%s&myip=%s&uid=%s&pass=%s\r\n",m_HostName,ipaddr,m_Username,m_Password);
	//	sprintf(m_CmdBuf,"GET /nic/update?hostname=%s&myip=%s&uid=%s&pass=%s\r\nAuthorization: Basic %s\r\n\r\n",m_HostName,ipaddr,m_Username,m_Password,password);
	//	sprintf(m_CmdBuf,"GET /nic/update?system=dyndns&hostname=%s&myip=%s&wildcard=ON&mx=%s&backmx=NO&offline=NO HTTP/1.0\r\nHost: members.dyndns.org\r\nAuthorization: Basic %s\r\nUser-Agent: inadyn-mt/ bhoover@wecs.com\r\n\r\n",m_HostName,ipaddr,m_HostName,password);

	//	sprintf(m_CmdBuf,"http://members.dyndns.org/nic/update?hostname=%s&myip=%s&uid=%s&pass=%s\r\n",m_HostName,ipaddr,m_Username,m_Password);
	//	sprintf(m_CmdBuf,"GET /nic/update?hostname=%s&myip=%swildcard=NOCHG&mx=NOCHG&backmx=NOCHG HTTP/1.0\r\nHost: members.dyndns.org\r\nAuthorization: Basic %s\r\nUser-Agent: inadyn-mt/ bhoover@wecs.com\r\n\r\n",m_HostName,ipaddr,password);
	memset(m_CmdBuf,0,sizeof(m_CmdBuf));
	snprintf(m_CmdBuf,(sizeof(m_CmdBuf) - 1),"GET /nic/update?hostname=%s&myip=%s HTTP/1.0\r\nHost: %s\r\nAuthorization: Basic %s\r\nUser-Agent: tvt-dvr-1.0\r\n\r\n",m_HostName,ipaddr, m_DdnsServerName, m_EncodePwd);
	
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
		close(sockfd);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	//send cmd
	if(send(sockfd, m_CmdBuf, strlen(m_CmdBuf), 0) == -1)
	{
		perror("send() data to server");
		close(sockfd);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	// receive
	char ReceiveBuf[1000] = {0};
	int len=sizeof(ReceiveBuf);
	ret=GetMessageFromServer(sockfd,ReceiveBuf,len);
	close(sockfd);

	if(ret != DDNS_IPUPDATE_OK)
	{
		return ret;
	}

	//www.dyndns.com的返回值，参看https://www.dyndns.com/developers/specs/return.html
	if(strstr(ReceiveBuf,"good"))
	{
		printf("( %s:%s:%d )	dyndns.com ip update:OK!\n", __FILE__, __FUNCTION__, __LINE__ );
		return DDNS_IPUPDATE_OK;
	}
	if(strstr(ReceiveBuf,"nochg"))
	{
		printf("( %s:%s:%d )	dyndns.com ip update:No changes, update considered abusive!\n", __FILE__, __FUNCTION__, __LINE__ );		
		return DDNS_IPUPDATE_OK;
	}
	if(strstr(ReceiveBuf,"badauth"))
	{
		printf("( %s:%s:%d )	dyndns.com ip update:Bad authorization (username or password)!\n", __FILE__, __FUNCTION__, __LINE__ );
		return DDNS_IPUPDATE_ENAMEPWD;
	}
	if(strstr(ReceiveBuf,"donator"))
	{
		printf("(%s:%s:%d)  dyndns.com ip update:An option available only to credited users (such as offline URL) was specified, but the user is not a credited user.\n", __FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	if(strstr(ReceiveBuf,"nohost"))
	{
		printf("(%s:%s:%d)  dyndns.com ip update:The hostname specified does not exist in this user account \n", __FUNCTION__, __FILE__, __LINE__);
		//找不到主机域名
		return DDNS_IPUPDATE_NOHOST;
	}

	if(strstr(ReceiveBuf,"notfqdn"))
	{
		printf("(%s:%s:%d)  dyndns.com ip update:The hostname specified is not a fully-qualified domain name(not in the form hostname.dyndns.org or domain.com). \n", __FUNCTION__, __FILE__, __LINE__);
		//
		return DDNS_IPUPDATE_NOTFQDN;
	}

	if(strstr(ReceiveBuf,"numhost"))
	{
		printf("(%s:%s:%d)  dyndns.com ip update .Too many hosts (more than 20) specified in an update. Also returned if trying to update a round robin (which is not allowed)\n", __FUNCTION__, __FILE__, __LINE__);
		//
		return DDNS_IPUPDATE_NUMHOST;
	}
	
	if(strstr(ReceiveBuf,"abuse"))
	{
		printf("(%s:%s:%d)  dyndns.com ip update .The hostname is blocked for abuse, you must unblock from the www.dyndns.com\n", __FUNCTION__, __FILE__, __LINE__);
		//在频繁访问dyndns.com服务器的时候会出现该错误,表明主机IP被www.dyndns.com屏蔽, 有可能是频繁update造成的
		return DDNS_IPUPDATE_ABUSE;
	}

	if(strstr(ReceiveBuf,"dnserr"))
	{
		printf("(%s:%s:%d)  dyndns.com ip update .The server error www.dyndns.com\n", __FUNCTION__, __FILE__, __LINE__);
		//服务器出问题
		return DDNS_IPUPDATE_EUNKNOWN;
	}


	return DDNS_IPUPDATE_EUNKNOWN;
}





