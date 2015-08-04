#include "DdnsTVTIPServerClient.h"
#include "NetInterface.h"

CDdnsTVTIPServerClient::CDdnsTVTIPServerClient()
{
	memset(&m_serverMsg, 0, sizeof(SERVER_MSG));
}

CDdnsTVTIPServerClient::~CDdnsTVTIPServerClient()
{

}

void CDdnsTVTIPServerClient::SetDeviceName(const char *pDeviceName)
{
	if (NULL != pDeviceName)
	{
		HEAT_BEAT_PACKET *pHeatBeat = (HEAT_BEAT_PACKET *)(&m_serverMsg);

		memcpy(pHeatBeat->deviceName, pDeviceName, sizeof(pHeatBeat->deviceName));
	}
	else
	{
		assert(false);
	}
}

void CDdnsTVTIPServerClient::SetHttpPort(unsigned short httpPort)
{
	HEAT_BEAT_PACKET *pHeatBeat = (HEAT_BEAT_PACKET *)(&m_serverMsg);

	//pHeatBeat->port = httpPort;	
}


void CDdnsTVTIPServerClient::SetServerPort(unsigned short serverPort)
{
	HEAT_BEAT_PACKET *pHeatBeat = (HEAT_BEAT_PACKET *)(&m_serverMsg);
	pHeatBeat->port = serverPort;	
}

unsigned long CDdnsTVTIPServerClient::DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName)
{
	struct sockaddr_in server;
	bzero( &server, sizeof(struct sockaddr_in) );

	if (inet_aton(pUserName, &server.sin_addr))
	{
		//创建套接口
		int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
		if( -1 == sockfd )
		{
			perror(" socket() error\n");
			assert( false );
			return DDNS_IPUPDATE_EUNKNOWN;
		}
		
		server.sin_family 	= AF_INET;
		server.sin_port		= htons(SERV_PORT);

		//连接到服务端
		if(0 != ConnectServer(sockfd, (struct sockaddr*)&server, sizeof(struct sockaddr), CONNECT_TIME_OUNT))
		{
			perror("connect()");
			close(sockfd);
			return DDNS_IPUPDATE_EUNKNOWN;
		}

		//发送数据到服务端
		bool getDeviceParamError = false;

		HEAT_BEAT_PACKET *pHeatBeat = (HEAT_BEAT_PACKET *)(&m_serverMsg);
		
		pHeatBeat->type = HEATBEAT_PACKET_TYPE;

		//ip、mac
		if ((!CNetInterface::Instance()->GetIPAddress(pHeatBeat->deviceIP)) || (!CNetInterface::Instance()->GetMAC((char *)pHeatBeat->mac)))
		{
			getDeviceParamError = true;
		}

		if (!getDeviceParamError)
		{
			if(-1 == send(sockfd, (unsigned char *)&m_serverMsg, sizeof(SERVER_MSG), 0))
			{
				perror("send() data to server");
				close(sockfd);
				return DDNS_IPUPDATE_EUNKNOWN;
			}
			else
			{
				int recvLen = sizeof(ANSWER_PACKET);
				unsigned long retMsg = GetMessageFromServer(sockfd, (char *)&m_answerPacket, recvLen);
				
				close(sockfd);
				
				if (DDNS_IPUPDATE_OK == retMsg)
				{
					if ((ANSWER_PACKET_TYPE == m_answerPacket.type) && (UPDATE_SUCCESS == m_answerPacket.result))
					{
						return DDNS_IPUPDATE_OK;
					}
					else
					{
						return DDNS_IPUPDATE_EUNKNOWN;
					}
				}
				else
				{
					return retMsg;
				}
			}
		}
		else
		{
			close(sockfd);
			return DDNS_IPUPDATE_EUNKNOWN;
		}
		
	}
	else
	{
		return DDNS_IPUPDATE_IP_ERROR;
	}
}

DDNS_IPUPDATE_STATUS CDdnsTVTIPServerClient::GetMessageFromServer(int sockfd, char *pbuf, int &len)
{
	assert( 0 < sockfd );

	int i = 0,  retval;
	int recvLen = 0;
	struct timeval timeout;
	fd_set ReadSet;

	for( i = 0; i < 10; i++ )
	{
		FD_ZERO(&ReadSet);
		FD_SET( sockfd, &ReadSet );

		timeout.tv_sec  = 3;
		timeout.tv_usec = 0;

		retval = select(sockfd + 1, &ReadSet, NULL, NULL, &timeout );
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
			{
				continue;
			}
			else
			{
				return DDNS_IPUPDATE_EUNKNOWN;
			}
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
			else
			{
				recvLen += retval;
			}

			if (len == recvLen)
			{
				return DDNS_IPUPDATE_OK;
			}
		}
	}

	return DDNS_IPUPDATE_EUNKNOWN;

}

