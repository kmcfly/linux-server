#include "DdnsMeibu.h"

#include "NetInterface.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <strings.h>
#include <errno.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <fcntl.h>
//

#define MEIBUSERVERPORT	9813

CDdnsMeibu::CDdnsMeibu()
{
	memset(m_LoginCommand, 0, sizeof(m_LoginCommand));	

	m_ServerSockfd = -1;

	memset(&m_Server, 0, sizeof(struct sockaddr_in));
}
CDdnsMeibu::~CDdnsMeibu()
{
	if (0 < m_ServerSockfd)
	{
		close(m_ServerSockfd);
		m_ServerSockfd = -1;
	}
}

unsigned long CDdnsMeibu::DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName )
{
	unsigned long retval = DDNS_IPUPDATE_OK;
	assert( NULL != pUserName );
	assert( NULL != pPassword );

	GetLoginCommand( pUserName,  pPassword );
	
	retval =  ConnectToServer( "main.meibu.com", MEIBUSERVERPORT );
	
	if( DDNS_IPUPDATE_OK == retval )
	{
		retval = ReciveMsgFromServer( NULL,  0 );
	}

	if( 0 < m_ServerSockfd )
	{
		close( m_ServerSockfd );
		m_ServerSockfd = -1;
	}

	return retval;

}

//protected:
	
	//获得登陆服务器的命令
void CDdnsMeibu::GetLoginCommand( const char *pUserName, const char *pPassword )
{
	assert( NULL != pUserName );
	assert( NULL != pPassword );

	assert( '\0' != *pUserName );
	//assert( '\0' != *pPassword );

	memset( m_LoginCommand, 0, DDNSLONGINCMDLEN );
	
	sprintf( m_LoginCommand, "meibudd08%s&&&%s&&&", pUserName, pPassword);
}



unsigned long CDdnsMeibu::ConnectToServer( const char *pDdnsServer , unsigned short sPort )
{
	assert( NULL != pDdnsServer );

#if USE_LOCAL_DNSRESOLVE
	struct hostent *he = CNetInterface::Instance()->GetHostByName( pDdnsServer );
#else
	struct hostent *he = gethostbyname( pDdnsServer );
#endif

	if( NULL == he )
	{
		perror("gethostbyname()");
	

		printf("%s:%s:%d, 不能解析域名:%s\n", __FUNCTION__, __FILE__, __LINE__, pDdnsServer);
		return DDNS_IPUPDATE_EUNKNOWN;
		
	}

	assert( AF_INET == he->h_addrtype );

	m_ServerSockfd = socket( AF_INET, SOCK_DGRAM, 0 );
	if( -1 == m_ServerSockfd )
	{
		perror(" socket() error\n");
		assert( false );
		return DDNS_IPUPDATE_EUNKNOWN;
	}
	
	if(fcntl(m_ServerSockfd, F_SETFL, O_NONBLOCK) == -1) 
	{
		perror("fcntl");
		assert(false);
	}


	bzero( &m_Server, sizeof( m_Server ) );
	m_Server.sin_family 	= AF_INET;
	m_Server.sin_port		= htons( sPort );
	m_Server.sin_addr 	= *((struct in_addr*)he->h_addr);	

#if USE_LOCAL_DNSRESOLVE
	CNetInterface::Instance()->ReleaseHost( he );
#endif

	if(sendto(m_ServerSockfd,m_LoginCommand, strlen(m_LoginCommand), 0, (struct sockaddr *)&m_Server, sizeof(m_Server)) < 0)
	{
		perror("sendto()");
		assert( false );
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	return DDNS_IPUPDATE_OK;

}

unsigned long CDdnsMeibu::ReciveMsgFromServer( char *pMessage,  int messageLen )
{
	fd_set readSet;
	timeval timeout;
	socklen_t socklen = sizeof(m_Server);

	int i = 0;
	int retval;

	while( true )
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 500000;

		FD_ZERO( &readSet );
		FD_SET( m_ServerSockfd, &readSet );

		retval = select( m_ServerSockfd+1,  &readSet, NULL, NULL, &timeout );

		if( 0 == retval )
		{
			if( i++ < 40 )	
			{
				if( 0 == ( i % 3 ) )
				{
					//如果等待10秒钟没收到服务器的回应就从新请求一次
					if(sendto(m_ServerSockfd,m_LoginCommand, strlen(m_LoginCommand), 0, (struct sockaddr *)&m_Server, sizeof(m_Server)) < 0)
					{
						perror("sendto()");
						assert( false );
						return DDNS_IPUPDATE_EUNKNOWN;
					}					
				}
				continue;
			}
			else
			{
				return DDNS_IPUPDATE_ETIMEOUT;
			}
		}
		else if( 0 < retval )
		{
			if( FD_ISSET( m_ServerSockfd,  &readSet ) )
			{
				char *pRecvMesg = pMessage;
				size_t msgLen = messageLen;

				if( NULL == pRecvMesg )
				{
					pRecvMesg = new char[4096];
					
					if( NULL != pRecvMesg )
					{
						msgLen = 4096;
						memset( pRecvMesg, 0, sizeof(char) * 4096 );
					}
					else
					{
						assert( false );
						return DDNS_IPUPDATE_EUNKNOWN;
					}
				}

				int ipupdateInfo = DDNS_IPUPDATE_OK;

				retval = recvfrom( m_ServerSockfd, pRecvMesg, msgLen, 0, (struct sockaddr*)&m_Server, &socklen);
				if( 0<retval )
				{
					if( strstr( pRecvMesg, "nzhuceyonghu"))
					{
						//用户名或密码错误
						pRecvMesg[retval]='\0';

						printf("( %s:%s:%d )	www.meibu.com ip update user name or password error!\n", __FILE__, __FUNCTION__, __LINE__ );
						//printf("%s():%s:%d, get message from server, the message is:%s\n", __FUNCTION__, __FILE__, __LINE__, pRecvMesg);

						ipupdateInfo = DDNS_IPUPDATE_ENAMEPWD;
					}
					else if( strstr( pRecvMesg, "dengluok" ) )
					{
						//更新成功
						pRecvMesg[retval]='\0';
						printf("( %s:%s:%d )	www.meibu.com ip update OK!\n", __FILE__, __FUNCTION__, __LINE__ );
						//printf("%s():%s:%d, get message from server, the message is:%s\n", __FUNCTION__, __FILE__, __LINE__, pRecvMesg);

						ipupdateInfo = DDNS_IPUPDATE_OK;
					}

				}
				else
				{
					perror("recvfrom()");
					assert( false );
					ipupdateInfo = DDNS_IPUPDATE_EUNKNOWN;
				}

				if( NULL == pMessage )
				{
					delete [] pRecvMesg;
					pRecvMesg = NULL;
				}

				return ipupdateInfo;
			}

			break;
		}

	}
	
	perror("select()");
	assert( false );
	return DDNS_IPUPDATE_EUNKNOWN;
}

