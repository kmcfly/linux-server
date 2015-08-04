#include "YuanZhengClinet.h"

CYuanZhengClinet::CYuanZhengClinet()
{
	
}

CYuanZhengClinet::~CYuanZhengClinet()
{

}

void CYuanZhengClinet::Init(char *pMac)
{
	m_serverPort = 0;
	m_alarmID = 0;
	m_urlPort = 0;
	memset(m_dvrmac, 0, sizeof(m_dvrmac));
	memset(m_serUrl, 0, sizeof(m_serUrl));
	memset(m_urlObject, 0, sizeof(m_urlObject));
	memset(&m_yuanZhengParam, 0, sizeof(m_yuanZhengParam));
	m_yzAlarmType =0;
	m_yzVieolossChnnBits =0;

	snprintf(m_dvrmac, sizeof(m_dvrmac), "%s", pMac);
}

void CYuanZhengClinet::SetYuanZhengParam(YUANZHENG_PARAM param)
{
	memcpy(&m_yuanZhengParam, &param, sizeof(m_yuanZhengParam));
	m_urlPort = 0;
	memset(m_serUrl, 0, sizeof(m_serUrl));
	memset(m_urlObject, 0, sizeof(m_urlObject));
	GetURLAndPort(m_yuanZhengParam.ipUrl,m_serUrl, m_urlObject, m_urlPort);
	//printf("%s,%d, url:%s, urlport:%d, obj:%s\n",__FILE__,__LINE__,m_serUrl,m_urlPort,m_urlObject);
}

void CYuanZhengClinet::SetServerPort(unsigned short serverPort)
{
	m_serverPort = serverPort;
}

void CYuanZhengClinet::SetYZAlarm(unsigned long yzAlarmType,ULONGLONG   yzVieolossChnnBits)
{
	m_yzAlarmType = yzAlarmType;
	m_yzVieolossChnnBits = yzVieolossChnnBits;
}

unsigned long CYuanZhengClinet::DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName )
{
	ULONG mask = 0,route = 0;
	char ipaddr[16] = {"192.168.3.22"};
	int ret = 0;

	if (0 == m_serverPort)
	{
		printf("%s,%s,%d,没有设置端口\n", __FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	if (0 == strlen(m_yuanZhengParam.asCode))
	{
		printf("%s,%s,%d ,没有设置企业号\n",__FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	if (0 == strlen(m_yuanZhengParam.ipUrl))
	{
		printf("%s,%s,%d ,没有设置ipUrl\n",__FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	if (0 == m_urlPort)
	{
		printf("%s,%s,%d ,没有设置url port\n",__FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	if (0 == strlen(m_serUrl))
	{
		printf("%s,%s,%d ,没有设置url\n",__FUNCTION__, __FILE__, __LINE__);
		return DDNS_IPUPDATE_EUNKNOWN;
	}
		

	//if( -1 == GetExternalIp(ipaddr) )
	//{
	//	printf("%s:%s:%d, 获取外网IP失败\n", __FUNCTION__, __FILE__, __LINE__);
	//	return DDNS_IPUPDATE_EUNKNOWN;
	//}
	
	
	struct in_addr  addrTemp;
	struct sockaddr_in ser_addr;
	if(INADDR_ANY == inet_aton(m_serUrl, &addrTemp))
	{
		//printf("%s,%d use GetHostByName\n",__FILE__,__LINE__);
		struct hostent *phstt = CNetInterface::Instance()->GetHostByName(m_serUrl);
		if (NULL == phstt)
		{
			printf("gethostbyname() error\n");
			printf("%s:%s:%d, 不能解析域名:%s\n", __FUNCTION__, __FILE__, __LINE__,m_serUrl);
			return DDNS_IPUPDATE_EUNKNOWN;//无法解析得到ServerIP
		}
		ser_addr.sin_family = AF_INET;
		ser_addr.sin_port =  htons(m_urlPort);
		memcpy(&(ser_addr.sin_addr), *((struct in_addr **)phstt->h_addr_list), sizeof(struct in_addr));
		memset(ser_addr.sin_zero,0, sizeof(ser_addr.sin_zero));
		CNetInterface::Instance()->ReleaseHost(phstt);
	}
	else
	{
		//printf("is ip addr\n");
		ser_addr.sin_family = AF_INET;
		ser_addr.sin_port =  htons(m_urlPort);
		memcpy(&(ser_addr.sin_addr), &addrTemp, sizeof(struct in_addr));
		memset(ser_addr.sin_zero,0, sizeof(ser_addr.sin_zero));		
	}
		

	memset(m_xmlBuf, 0, sizeof(m_xmlBuf));
	memset(m_CmdBuf, 0, sizeof(m_CmdBuf));

	char alarmbuf[1024]={0};
	memset(alarmbuf,0,sizeof(alarmbuf));

	if(0 == m_yzAlarmType)
	{
		snprintf(alarmbuf,sizeof(alarmbuf),"<alert>0</alert>\r\n");	
	}
	else
	{
		for(int i=0;i<YZ_ALARM_TYPE_NUM;i++)
		{
			if( (m_yzAlarmType) & ( ((unsigned long)0x01) <<i) )
			{
				switch(i)
				{
				case YZ_ALARM_VIDEOLOSS:
					{
						//查找通道号
						for(int i=0;i<sizeof(m_yzVieolossChnnBits);i++)
						{
							if( (m_yzVieolossChnnBits) & ( ((ULONGLONG)0x01) <<i) )
							{
								snprintf(alarmbuf+strlen(alarmbuf),sizeof(alarmbuf),"<alert>1%02d</alert>\r\n",i+1);//从1开始计数							
							}
						}

					}
					break;
				case YZ_ALARM_DISK_FULL:
					{
						snprintf(alarmbuf+strlen(alarmbuf),sizeof(alarmbuf),"<alert>200</alert>\r\n");
					}
					break;
				case YZ_ALARM_DISK_DISCONNET:
					{
						snprintf(alarmbuf+strlen(alarmbuf),sizeof(alarmbuf),"<alert>300</alert>\r\n");
					}
					break;
				default:
					{
						assert(false);
						snprintf(alarmbuf+strlen(alarmbuf),sizeof(alarmbuf),"<alert>0</alert>\r\n");
					}
					break;
				}
			}

		}
	}
	
	snprintf(m_xmlBuf, sizeof(m_xmlBuf), "<?xml version=\"1.0\"?>\r\n<root xmlns:dt=\"urn:schemas-microsoft-com:datatypes\">\r\n<ascode>%s</ascode>\r\n<dvrmac>%s</dvrmac>\r\n<ip>%s</ip>\r\n<port>%d</port>\r\n%s<ver>1</ver>\r\n</root>\r\n", m_yuanZhengParam.asCode, m_dvrmac, ipaddr, m_serverPort,alarmbuf);
	snprintf(m_CmdBuf,sizeof(m_CmdBuf), "POST %s HTTP/1.1\r\nHost:%s\r\nAccept:*/*\r\nUser-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)\r\nConnection:Keep-Alive\r\nContent-Length:%d\r\n\r\n%s", m_urlObject,m_serUrl, strlen(m_xmlBuf), m_xmlBuf);
	//printf("m_cmdbuf=%s\n",m_CmdBuf);
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if( -1 == sockfd )
	{
		//perror(" socket() error\n");
		assert( false );
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	//link to host
	if(ConnectServer(sockfd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr), CONNECT_TIME_OUNT) != 0)
	{
		//perror("connect()");
		close(sockfd);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	//send cmd
	if(send(sockfd, m_CmdBuf, strlen(m_CmdBuf), 0) == -1)
	{
		//perror("send() data to server");
		close(sockfd);
		return DDNS_IPUPDATE_EUNKNOWN;
	}

	// receive
	char ReceiveBuf[1000] = {0};
	int len=sizeof(ReceiveBuf);
	ret=GetMessageFromServer(sockfd,ReceiveBuf,len);
	close(sockfd);

	//printf("%s,%d,ReceiveBuf:%s\n",__FILE__,__LINE__,ReceiveBuf);
	if(ret != DDNS_IPUPDATE_OK)
	{
		printf("%s,%d,ret:%d\n",__FILE__,__LINE__,ret);
		return ret;
	}
	//printf("%s,%d,ret:%d\n",__FILE__,__LINE__,ret);

	

	return DDNS_IPUPDATE_OK;
	
}


CYuanZhengMan * CYuanZhengMan::Instance()
{
	static CYuanZhengMan s_yuanZhengManager;
	return &s_yuanZhengManager;
}

CYuanZhengMan::CYuanZhengMan()
{

}


CYuanZhengMan::~CYuanZhengMan()
{

}

bool CYuanZhengMan::Inital()
{
	char mac[6] = {0};
	char szTemp[36] = {"0018AE222222"};
	int i = 0;
	m_count = 0;
	m_yzAlarmType =0;
	m_yzVieolossChnnBits =0;
	
	if (CNetInterface::Instance()->GetMAC(mac))
	{
		printf("device mac:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		snprintf(szTemp, 36, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}


	while ((szTemp[i] != 0) && (i < 36))
	{
		if (szTemp[i] >= 'a' && szTemp[i] <= 'z')
		{
			szTemp[i] -= 32;
		}
		i++;
	}
	
	m_yuanZhengClient.Init(szTemp);
	return true;
}


void CYuanZhengMan::Quit()
{

}

bool CYuanZhengMan::Start()
{
	m_hIpUpdateID = PUB_CreateThread( YuanZhengUpdateThread, (void *)this, &m_bIpUpdateProc );	
	if( PUB_CREATE_THREAD_FAIL == m_hIpUpdateID )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}	 	
	return true;
}

void CYuanZhengMan::Stop()
{
	if ( PUB_THREAD_ID_NOINIT != m_hIpUpdateID )
	{		
		PUB_ExitThread( &m_hIpUpdateID, &m_bIpUpdateProc );	
	}
}


void CYuanZhengMan::SetYuanZhengParam(YUANZHENG_PARAM param)
{
	m_lock.Lock();
	printf("%s,%d, %s,%s\n",__FILE__,__LINE__,param.ipUrl,param.asCode);
	memcpy(&m_yuanZhengParam,&param,sizeof(YUANZHENG_PARAM));
	m_lock.UnLock();
}

void CYuanZhengMan::SetServerPort(unsigned short serverPort)
{
	m_lock.Lock();
	m_serverPort = serverPort;
	m_lock.UnLock();
}

PUB_THREAD_RESULT PUB_THREAD_CALL CYuanZhengMan::YuanZhengUpdateThread(void *pParam)
{
#ifdef	__ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CYuanZhengMan *pManager = reinterpret_cast<CYuanZhengMan *>(pParam);
	pManager->YuanZhengUpdateProc();
	return 0;
}



void CYuanZhengMan::YuanZhengUpdateProc()
{
	bool bSend = false;
	YUANZHENG_PARAM yuanZhengParam;
	unsigned short serverPort;
	unsigned long  yzTmpAlarmType;
	ULONGLONG     yzTmpVieolossChnnBits;
	m_count = 0;
	while( m_bIpUpdateProc )
	{
		m_lock.Lock();
		if (m_count > YZ_UPDATE_SLEEP) 
		{
			bSend = true;
			memcpy(&yuanZhengParam,&m_yuanZhengParam,sizeof(YUANZHENG_PARAM));
			yzTmpAlarmType = m_yzAlarmType;
			yzTmpVieolossChnnBits = m_yzVieolossChnnBits;
			serverPort = m_serverPort;
			m_count = 0;
		}
		else
		{
			bSend = false;
		}
		m_count++;
		m_lock.UnLock();

		if (bSend) 
		{
			m_yuanZhengClient.SetYuanZhengParam(yuanZhengParam);
			m_yuanZhengClient.SetServerPort(serverPort);
			m_yuanZhengClient.SetYZAlarm(yzTmpAlarmType,yzTmpVieolossChnnBits);
			m_yuanZhengClient.DdnsIPupdate(NULL, NULL, NULL);
		}
		
		PUB_Sleep(10000);
	}
}


void CYuanZhengMan::SetYZAlarm(unsigned long yzAlarmType,bool bopenalarm,ULONGLONG   yzVieolossChnnBits)
{
	m_lock.Lock();

	if(yzAlarmType == YZ_ALARM_VIDEOLOSS)
	{
		m_yzVieolossChnnBits = yzVieolossChnnBits;
	}

	unsigned long prevAlarmType = m_yzAlarmType;//上一次远征报警类型
	if(bopenalarm)
	{		
		m_yzAlarmType |= static_cast<unsigned long>(0x01) << yzAlarmType;
	}
	else
	{
		m_yzAlarmType &= ~(static_cast<unsigned long>(0x01) << yzAlarmType);
	}
	
	if((prevAlarmType>0)||(m_yzAlarmType>0))
	{
		m_count = YZ_UPDATE_SLEEP+1;
	}
	
	m_lock.UnLock();
}
