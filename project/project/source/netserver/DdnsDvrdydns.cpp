 #include "DdnsDvrdydns.h"
#include "NetInterface.h"


#define LENGTH 30

CDdnsDvrdydns::CDdnsDvrdydns( )
{
	memset(m_userName, 0, sizeof(m_userName));
	memset(m_passWord, 0, sizeof(m_passWord));
	memset(m_DdnsServerName, 0, sizeof(m_DdnsServerName));
	
	m_connectfd = -1;
}

CDdnsDvrdydns::~CDdnsDvrdydns()
{
	memset( m_userName, 0, LENGTH );
	memset( m_passWord, 0, LENGTH );
	memset( m_DdnsServerName, 0, sizeof(m_DdnsServerName) );

	if( m_connectfd > 0 )
		close(m_connectfd);

	m_connectfd = -1;
}



unsigned long CDdnsDvrdydns::DdnsIPupdate(const char *pUserName, const char *pPassword , const char *pmyDomainName )
{
	assert(NULL != pUserName);
	assert(NULL != pPassword);
	assert(NULL != pmyDomainName);

	strcpy( m_userName, pUserName );
	strcpy( m_passWord, pPassword );
	strcpy( m_myDomainName, pmyDomainName );

	char recvbuf[BUFSIZE];
	char cmdbuf[BUFSIZE];
	int recvLen = BUFSIZE;
	int retval = DDNS_IPUPDATE_OK;
	
	if( 0 == strlen( m_myDomainName ) )
	{
		//dvrdydns.com上的主机域名不能为空
		return DDNS_IPUPDATE_EHOSTDOMAIN_EMPTY;
	}

	sprintf(m_userPass, "%s:%s", m_userName, m_passWord );
	base64Encode( m_userPass,  m_auther );
	sprintf( cmdbuf, "GET /client/update.asp?host=%s HTTP/1.1\r\nUser-Agent: DDNS-Enterprise-Client-v2.0\r\nAuthorization: Basic %s\r\nHost: %s\r\n\r\n",m_myDomainName,m_auther, m_DdnsServerName);

	if( false == ConnectToServer() )
	{
		return DDNS_IPUPDATE_ECONNECT;
	}

	//printf("%s:%s:%d, the send message=%s\n", __FUNCTION__, __FILE__, __LINE__, cmdbuf);
	DDNS_IPUPDATE_STATUS retStatus = SendMessageToServer( cmdbuf );
	if( DDNS_IPUPDATE_OK == retStatus )
	{
		retStatus = GetMessageFromServer(m_connectfd, recvbuf, recvLen);

		//printf("%s:%s:%d, the receive message=%s\n", __FUNCTION__, __FILE__, __LINE__, recvbuf);
		if( DDNS_IPUPDATE_OK == retStatus )
		{
			if( strstr(recvbuf, "\r\nnodom"))
			{
				printf("( %s:%s:%d )	%s ip update user name or password error!\n", __FILE__, __FUNCTION__, __LINE__,m_DdnsServerName );
				retStatus = DDNS_IPUPDATE_EHOSTDOMAIN;
			}
			else if(strstr(recvbuf, "\r\nok"))
			{
				printf("( %s:%s:%d )	%s ip update OK!\n", __FILE__, __FUNCTION__, __LINE__,m_DdnsServerName );
				retStatus = DDNS_IPUPDATE_OK;
			}
			else if( strstr(recvbuf, "\r\n911"))
			{
				printf("(%s:%s:%d)  %s ip update password is empty\n", __FUNCTION__, __FILE__, __LINE__ ,m_DdnsServerName);
				retStatus = DDNS_IPUPDATE_ENAMEPWD;
			}
			else if (strstr(recvbuf, "disabled"))
			{
				printf("(%s:%s:%d)  ddns %s at %s is disabled!\n", __FUNCTION__, __FILE__, __LINE__ ,pmyDomainName, m_DdnsServerName);
				retStatus = DDNS_IPUPDATE_ABUSE;
			}
			else
			{
				printf("%s:%s:%d, the receive message=%s\n", __FUNCTION__, __FILE__, __LINE__, recvbuf);
				retStatus = DDNS_IPUPDATE_EUNKNOWN;
			}
		}
	}

	if( m_connectfd > 0 )
	{
		close( m_connectfd );
		m_connectfd = -1;
	}

	return retStatus;
}


bool CDdnsDvrdydns::ConnectToServer()
{
	struct sockaddr_in server;
	socklen_t socklen;
	
	struct hostent *he= NULL;

#if USE_LOCAL_DNSRESOLVE
	he = CNetInterface::Instance()->GetHostByName(m_DdnsServerName);
#else
	he = gethostbyname(m_DdnsServerName);
#endif

	if( NULL == he )
	{
		printf("gethostbyname() error\n");

		printf("%s:%s:%d, 不能解析域名:%s\n", __FUNCTION__, __FILE__, __LINE__,m_DdnsServerName);
		return false;
	}

	
	if( -1 == (m_connectfd = socket( AF_INET, SOCK_STREAM, 0)))
	{
		perror("socket()");
		return false;
	}
	
	server.sin_family = PF_INET;
	server.sin_port = htons(80);
	server.sin_addr = *( struct in_addr *)*he->h_addr_list;

#if USE_LOCAL_DNSRESOLVE
	CNetInterface::Instance()->ReleaseHost( he );
#endif


	if( 0 != ConnectServer( m_connectfd, ( struct sockaddr *) &server, sizeof( server ),CONNECT_TIME_OUNT ) )
	{
		perror("connect() to the dyndns server");
		close( m_connectfd );
		m_connectfd = -1;
		return false;
	}


	return true;
}

DDNS_IPUPDATE_STATUS CDdnsDvrdydns::SendMessageToServer( const char *pMessage )
{
	int i = 0, retval = 0;
	fd_set WriteSet;

	struct timeval timeout;

	for( i=0; i<10; i++ )
	{
		FD_ZERO( &WriteSet );
		FD_SET( m_connectfd, &WriteSet );

		timeout.tv_sec  = 3;
		timeout.tv_usec = 0;

		retval = select( m_connectfd+1, NULL, &WriteSet, NULL, &timeout );
		if( 0 == retval )
		{
			if( 9 == i )
			{
				return DDNS_IPUPDATE_ETIMEOUT;
			}
			continue;
		}
		else if( -1== retval )
		{
			if( errno == EINTR )
			{
				continue;
			}
			else
			{
				return DDNS_IPUPDATE_EUNKNOWN;
			}
		}
		if( FD_ISSET( m_connectfd, &WriteSet ) )
		{
			retval = send( m_connectfd, pMessage, strlen(pMessage), 0 );
			if( -1== retval )
			{
				return DDNS_IPUPDATE_EUNKNOWN;
			}
			return DDNS_IPUPDATE_OK;
		}
	}

	return DDNS_IPUPDATE_EUNKNOWN;
}






