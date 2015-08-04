#include "DdnsClient.h"
#include "NetInterface.h"
#include "ReadDdns.h"

#include <stdio.h>
#include <fcntl.h>
#include <time.h>

CDdnsClient::CDdnsClient()
{
	memset( m_DdnsServerName, 0, sizeof(m_DdnsServerName) );
	memset( m_szExternalIp, 0, sizeof(m_szExternalIp));
}

CDdnsClient::~CDdnsClient()
{
	
}

//
//
bool CDdnsClient::SetDDNSServerName(const char *pServerName)
{
	if(strlen(pServerName)<5)
	{
		assert(false);
		printf("%s:%s:%d,输入的域名错误, server name = %s\n", __FUNCTION__, __FILE__, __LINE__, pServerName);
		return false;
	}
	else
	{
		snprintf(m_DdnsServerName, (sizeof(m_DdnsServerName)-1), "%s", pServerName);
		return true;
	}
}

unsigned long CDdnsClient::DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName )
{
	//printf("%s():%s:%d, running at here , do nothing for ddns control\n", __FUNCTION__, __FILE__, __LINE__);
	return 0;
}

bool CDdnsClient::RemoveDDNS()
{
	return false;
}

static char table64[]=
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
void CDdnsClient::base64Encode(char *intext, char *output)
{
	unsigned char ibuf[3];
	unsigned char obuf[4];
	int i;
	int inputparts;

	while(*intext) {
		for (i = inputparts = 0; i < 3; i++) { 
			if(*intext) {
				inputparts++;
				ibuf[i] = *intext;
				intext++;
			}
			else
				ibuf[i] = 0;
		}

		obuf [0] = (ibuf [0] & 0xFC) >> 2;
		obuf [1] = ((ibuf [0] & 0x03) << 4) | ((ibuf [1] & 0xF0) >> 4);
		obuf [2] = ((ibuf [1] & 0x0F) << 2) | ((ibuf [2] & 0xC0) >> 6);
		obuf [3] = ibuf [2] & 0x3F;

		switch(inputparts) {
	  case 1: /* only one byte read */
		  sprintf(output, "%c%c==", 
			  table64[obuf[0]],
			  table64[obuf[1]]);
		  break;
	  case 2: /* two bytes read */
		  sprintf(output, "%c%c%c=", 
			  table64[obuf[0]],
			  table64[obuf[1]],
			  table64[obuf[2]]);
		  break;
	  default:
		  sprintf(output, "%c%c%c%c", 
			  table64[obuf[0]],
			  table64[obuf[1]],
			  table64[obuf[2]],
			  table64[obuf[3]] );
		  break;
		}
		output += 4;
	}
	*output=0;
}

DDNS_IPUPDATE_STATUS CDdnsClient::GetMessageFromServer(int sockfd, char *pbuf, int &len )
{
	assert( 0 < sockfd );
	int i = 0,  retval;
	int recvLen = 0;
	struct timeval timeout;
	fd_set ReadSet;

	for( i=0; i<10; i++ )
	{
		// FD_SET()应该在每次循环时都被调用， select()超时时readSet会被清空
		FD_ZERO(&ReadSet);
		FD_SET( sockfd, &ReadSet );

		//把超时时间改为1秒， 防止网络太慢时服务端返回的应答抓不到。
		//在公司服务器(www.dvrdydns.com)上遇到过这种情况：超时时间设置为10000微妙，通过外网一致得不到服务器响应，内网却可以
		//当延长超时时间后就正常了。
		timeout.tv_sec  = 3;
		timeout.tv_usec = 0;

		retval = select(sockfd+1, &ReadSet, NULL, NULL, &timeout );
		if( 0 == retval)
		{
			if( 9 == i )
			{
				return DDNS_IPUPDATE_ETIMEOUT;
			}
			continue;
		}
		else if( -1 == retval )
		{
			if( EINTR == errno )
				continue;
			else
				return DDNS_IPUPDATE_EUNKNOWN;
		}

		if( FD_ISSET( sockfd, &ReadSet ) )
		{
			if ( recvLen > len)
			{
				printf("%s:%s:%d, recvLen=%d, len=%d", __FUNCTION__, __FILE__ ,__LINE__, recvLen, len);
				assert(false);

				return DDNS_IPUPDATE_EUNKNOWN;
			}

			retval = recv( sockfd, pbuf + recvLen, len - recvLen, 0 );
			if( -1 == retval )
			{
				return DDNS_IPUPDATE_EUNKNOWN;
			}
			if( strstr(pbuf, "\r\n\r\n"))
			{
				recvLen += retval;
				pbuf[recvLen] = '\0';
				len = recvLen;
				return DDNS_IPUPDATE_OK;
			}

			recvLen += retval;

		}
	}

	return DDNS_IPUPDATE_EUNKNOWN;

}


int  CDdnsClient::GetExternalIp(char *pIPAddr )
{
	assert( NULL != pIPAddr );

	struct sockaddr_in Server;
	int serverfd = -1;

	struct hostent *pServerhe = NULL;

#if USE_LOCAL_DNSRESOLVE
	pServerhe = CNetInterface::Instance()->GetHostByName("checkip.dyndns.org");
#else
	pServerhe = gethostbyname("checkip.dyndns.org");
#endif

	if( NULL == pServerhe )
	{
		printf("gethostbyname() error\n");
		printf("%s:%s:%d, 不能解析域名:%s\n", __FUNCTION__, __FILE__, __LINE__,"checkip.dyndns.org");
		return -1;
	}

	memset(&Server, 0, sizeof(struct sockaddr_in));
	Server.sin_family = AF_INET;
	if (21 == CReadDdns::Instance()->GetCompany())
	{
		Server.sin_port =  htons(8245);
	}
	else
	{
		Server.sin_port =  htons(80);
	}

	memcpy(&(Server.sin_addr), *((struct in_addr **)pServerhe->h_addr_list), sizeof(struct in_addr));
	int addr_len =sizeof(Server);

#if USE_LOCAL_DNSRESOLVE
	CNetInterface::Instance()->ReleaseHost( pServerhe );
#endif

	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if( -1 == serverfd )
	{
		perror("socket()");
		//close( serverfd );
		return -1;
	}
	if(ConnectServer(serverfd, (struct sockaddr*)&Server, sizeof(struct sockaddr), CONNECT_TIME_OUNT) != 0)
	{
		close(serverfd);
		perror("connect():");
		return -1;
	}

	char package[200]={0};
	strcpy(package,"GET http://checkip.dyndns.org/HTTP/1.0\r\n\r\n");

	//send Http request
	if ( -1 == send(serverfd,(char*)package,strlen(package),0))
	{
		close(serverfd);
		perror("send():");
		return -1;
	}

	char *pReceiveBuf = new char [4096];
	assert( NULL != pReceiveBuf);
	memset(pReceiveBuf, 0, 4096);
	int recvLen = 4096;

	//接收从服务器发来的IP信息
	if( DDNS_IPUPDATE_OK != GetMessageFromServer(serverfd, pReceiveBuf, recvLen) )
 	{
 		close(serverfd);
 		if ( NULL != pReceiveBuf)
 		{
 			delete [] pReceiveBuf;
 			pReceiveBuf = NULL;
 		}
 		return -1;
 	}


	if ( 0 < recvLen ) 
	{
		char *pIP = strstr(pReceiveBuf ,"Current IP Address:");
		if(pIP != NULL)
		{
			pIP += 19;
			int a=0, b=0, c=0, d=0;
			sscanf(pIP, "%d.%d.%d.%d", &a, &b,&c,&d);
			snprintf(pIPAddr, 16,"%d.%d.%d.%d", a, b, c, d);
		}	
	}	

	if ( NULL != pReceiveBuf)
	{
		delete [] pReceiveBuf;
		pReceiveBuf = NULL;
	}
	close( serverfd );
	return 0;

}


//这里有个bug，当服务端不能连接到的时候connect()函数的超时时间是75s，会难以忍受，这就需要缩短超时时间。设置为30s来处理
//在网络描述符为非阻塞的时候，connect()系统调用有可能在三次握手没结束的时候返回，这时候用select()来观察是否连接成功，如果select()的30s超时则认为没有连接成功，
int CDdnsClient::ConnectServer(int connectFd, struct sockaddr *pServerSockAddr ,int sockAddrLen, unsigned long timeOut)
{
	int iSave = fcntl(connectFd,F_GETFL);
	fcntl(connectFd, F_SETFL, iSave | O_NONBLOCK);


	if( 0 != connect( connectFd, pServerSockAddr, sockAddrLen) )
	{
		timeval timeOutVal;
		timeOutVal.tv_sec = timeOut / 1000;
		timeOutVal.tv_usec = (timeOut % 1000) * 1000;

		fd_set readdSet;
		fd_set writeSet;
		FD_ZERO(&readdSet);
		FD_ZERO(&writeSet);
		FD_SET(connectFd, &readdSet);
		FD_SET(connectFd, &writeSet);
		if(0 < select(connectFd+1, &readdSet, &writeSet, NULL, &timeOutVal))
		{
			//要从新设置为阻塞模式
			//NoBlockIO=0;
			fcntl(connectFd, F_SETFL, iSave );

			return 0;
		}
		perror("connect() to the dyndns server");

		return -1;
	}

}


