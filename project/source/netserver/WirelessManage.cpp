#include"WirelessManage.h"
#include "NetInterface.h"

#if !defined(_WIN32)
#include <linux/netlink.h>
#endif

CWirelessManage * CWirelessManage::m_pInstance = NULL;

CWirelessManage::CWirelessManage()
{
	PUB_InitLock(&m_lock);
	m_fd = -1;
	m_fp = NULL;
	m_SetBaudrate = 0;
	m_SetBaudrateBak =0;
	m_WirelessProcID	= PUB_THREAD_ID_NOINIT;
	m_bWireless		= false;
	m_WirelessState = false;
	m_bBak = false;
	m_bCheck = false;
	m_signal = 0;
	m_bDialing = false;
	m_bhavecard = false;
	m_bWirelessInit = false;
	memset(&m_wirelesscfg, 0, sizeof(m_wirelesscfg));
	memset(m_3gDeviceName, 0, sizeof(m_3gDeviceName));


	m_WirelessHotPlugID = PUB_THREAD_ID_NOINIT;
	m_bHotPlug 		  = false;
}

CWirelessManage::~CWirelessManage()
{
	PUB_DestroyLock(&m_lock);
}

CWirelessManage* CWirelessManage::Instance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CWirelessManage;
		assert(NULL != m_pInstance);
	}
	return m_pInstance;
}

//打开3G usb 设备
bool CWirelessManage::Initial()
{
	memset(m_3gDeviceName, 0, sizeof(m_3gDeviceName));
	if (!GetSerialDevName(m_3gDeviceName, sizeof(m_3gDeviceName)))
	{
		return false;
	}
	printf("Open ttyUSB device path %s\n", m_3gDeviceName);
	m_fd = open(m_3gDeviceName,  O_RDWR | O_NONBLOCK);

	if(m_fd == -1)
	{
		printf("open 3G USB port err!\n");
		return false;
	}
	
	
	//设置usb口速度
	if (!SetUSBPortSpeed(SBR_115200)) 
	{
		close(m_fd);
		return false;
	}

	//设置usb口奇偶校验位
	if (!SetUSBPortParity(8, 1, 'N', 40)) 
	{	
		close(m_fd);
		return false;
	}

	m_bWirelessInit = true;
	return true;
}

void CWirelessManage::Quit()
{
	memset(m_3gDeviceName, 0, sizeof(m_3gDeviceName));

	if(m_fd != -1)
	{
		close(m_fd);
		m_fd = -1;
	}
}

//读串口时调用该函数
bool CWirelessManage::SetUSBPortParity(int databits, int stopbits, int parity, BYTE ttydatalength)
{
	struct termios options;
	
	if  ( tcgetattr( m_fd,&options)  !=  0)
	{
		perror("SetupUsbPort1\n");
		return false;
	}

	options.c_cflag &= ~CSIZE;
	switch (databits) /*设置数据位数*/
	{
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag |= CS8;
			break;
		default:
			perror("Unsupported usb port size\n");
			return false;
	}
	switch (parity)
	{
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB); /* 设置为奇校验*/
		options.c_iflag |= INPCK;             /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;     /* Enable parity */
		options.c_cflag &= ~PARODD;   /* 设置为偶校验*/
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;
	case 'S':
	case 's':  /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
	break;
	default:
		perror("Unsupported parity\n");
		return false;
	}

	/* 设置停止位*/
	switch (stopbits)
	{
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		perror("Unsupported stop bits\n");
		return false;
	}

	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;

	//mj 如果不是开发终端之类的话,只是串口输出数据,而不需要串口来处理,
	//mj 那么使用原始模式(Raw Mode)方式来通讯,设置方式如下: 
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG); // Input   
	options.c_oflag  &= ~OPOST;                          // Output

	tcflush(m_fd,TCIFLUSH);
	//options.c_cc[VTIME] = 5; 		
	//options.c_cc[VMIN] = 0; 		
	options.c_cc[VTIME] = 8; 		
	options.c_cc[VMIN] = 0; 		

	if (tcsetattr(m_fd,TCSANOW,&options) != 0)
	{
		perror("Setup usb Parity err\n");
		return false;
	}
	return true;
}

bool CWirelessManage::SetUSBPortSpeed(PORT_BAUDRATE bandrate)
{	
	int   status;
	unsigned long rate;
	struct termios   Opt;

	if (-1 == m_fd) 
	{
		return false;
	}

	rate = GetBaudRate(bandrate);
	tcgetattr(m_fd, &Opt);
	tcflush(m_fd, TCIOFLUSH);
	cfsetispeed(&Opt, rate);
	cfsetospeed(&Opt, rate);
	status = tcsetattr(m_fd, TCSANOW, &Opt);
	if  (status != 0)
	{
		perror("SetUSBPortSpeed \n");
		return false;
	}
	tcflush(m_fd,TCIOFLUSH);

	//设置成功更新串口波特率
	//m_SetBaudrate = bandrate;
	
	return true;

}

bool CWirelessManage::WriteToPort(const char *pBuf,int len,PORT_BAUDRATE bandrate)
{	
	//查看输入缓冲中是否还有数据， 有数据就要等待读完才可以
	//但是不能保证所有的数据都能抓到

	int iSleepTime = 10; //毫秒为单位，假设进程调度还浪费了10毫秒
	switch(bandrate)
	{
	case SBR_110:
		iSleepTime += len * 1000 * 8 / 110 ;
		break;
	case SBR_300:
		iSleepTime += len * 1000 * 8 / 300 ;
		break;
	case SBR_600:
		iSleepTime += len * 1000 * 8 / 600 ;
		break;
	case SBR_1200:
		iSleepTime += len * 1000 * 8 / 1200 ;
		break;
	case SBR_2400:
		iSleepTime += len * 1000 * 8 / 2400 ;
		break;
	case SBR_4800:
		iSleepTime += len * 1000 * 8 / 4800 ;
		break;
	case SBR_9600:
		iSleepTime += len * 1000 * 8 / 9600 ;
		break;
	case SBR_19200:
		iSleepTime += len * 1000 * 8 / 19200 ;
		break;
	case SBR_38400:
		iSleepTime += len * 1000 * 8 / 38400 ;
		break;
	case SBR_57600:
		iSleepTime += len * 1000 * 8 / 57600 ;
		break;
	case SBR_115200:
		iSleepTime += len * 1000 * 8 / 115200 ;
		break;
	case SBR_230400:
		iSleepTime += len * 1000 * 8 / 230400 ;
		break;
	case SBR_460800:
		iSleepTime += len * 1000 * 8 / 460800 ;
		break;
	case SBR_921600:
		iSleepTime += len * 1000 * 8 / 921600 ;
		break;
	default :
		iSleepTime += len * 1000 * 8 / 110 ;
		break;
	}
	int ret1 = write(m_fd, pBuf, len);

    perror("write ===============error:");
	printf("m_fd ============= %d\nret =========== %d\nret1 ========== %d\n",m_fd, len, ret1);
	if (ret1==len) 
	{
		PUB_Sleep(iSleepTime*100);
		return true;
	}
	else
	{
		PUB_Sleep(10);
		int ret2 = write(m_fd, pBuf+ret1, len-ret1);
		printf("ret2 ============ %d\n", ret2);
		PUB_Sleep(iSleepTime);
		
		if (ret2==len-ret1)
		{
			return true;
		}
		return false;
	}
}

//接收数据
int CWirelessManage::ReadDate(BYTE *pBuf,int len,PORT_BAUDRATE bandrate)
{
    fd_set serial;
    int retval = 0;
	int ret1 = 0;
	struct timeval timeout = {0,500000};     //100000
	
	//先置为读模式
	
    FD_ZERO(&serial);
    FD_SET(m_fd, &serial);
    if ((retval = select(m_fd+1,  &serial, NULL, NULL,&timeout)) <= 0)
	{
		 return 0;	
	}

//读数据写BUFFER
    if(FD_ISSET(m_fd, &serial) !=0)
	{
		memset((char *)pBuf, 0, len);
		ret1 = read(m_fd, pBuf, len);
     }
        
	return ret1;
}

bool CWirelessManage::WirelessProc()
{
	pthread_detach(pthread_self());
	
	bool ret = false;
	unsigned num = 0;
	unsigned long dailFailCount = 0;

	while(m_bWireless)
	{
		//if (!CheckWhetherHaveDevice())
		//{
		//	printf("%s, %d The 3G device not connect!\n", __FILE__, __LINE__);
		//	m_bWirelessInit = false;
		//	PUB_Sleep(3000);
		//	continue;
		//}
		if(!m_bWirelessInit)
		{
			if(!Initial())
			{
				num ++;
				printf("init wireless USb port error!!\n");
				PUB_Sleep(3000);
				if (num < 200)
				{
					continue;
				}
				else
				{
					Quit();
					break;
				}
			}
		}
		if (m_bDialing)
		{
			char strDNS[5][64]={"www.google.com", "time.nist.gov", "www.dyndns.com", "www.meibu.com", "www.dvrdydns.com"};
			//for (int i = 0; i < 3; i++)
			//{
			//	if (GetCardState() && CheckWhetherHaveSignal() && CheckWhetherHaveNet())
			//	{
			//		m_bDialing = true;
			//		break;
			//	}
			//	else
			//	{
			//		m_bDialing  = false;
			//		PUB_Sleep(300);
			//		printf("The 3G signal is bad. %s, %d\n", __FILE__, __LINE__);
			//	}
			//}
			//if (!m_bDialing)
			//{
			//	for (int i = 0; i<5; i++)
			//	{
			//		if (CNetInterface::Instance()->CanReachTheHost(strDNS[i]))
			//		{
			//			m_bDialing = true;
			//			break;
			//		}
			//		else if(i == 4)
			//		{
			//			m_bDialing = false;
			//			printf("The 3G net is disconnect! will be redial!\n");
			//		}
			//	}
			//}
			for (int i = 0; i<5; i++)
			{
				if (CNetInterface::Instance()->CanReachTheHost(strDNS[i]))
				{
					m_bDialing = true;
					break;
				}
				else if(i == 4)
				{
					m_bDialing = false;
					printf("The 3G net is disconnect! will be redial!\n");
				}
			}

			if (!m_bDialing)
			{
				m_WirelessState = false;
			}
		}
	
		if (!m_bDialing)
		{
			if (m_bWirelessInit && !m_WirelessState)
			{
				LogoutDialing();
			}

			if (5 == dailFailCount)
			{
				CNetInterface::Instance()->DownInterface("ppp0", sizeof("ppp0"));
				CNetInterface::Instance()->UpInterface("ppp0", sizeof("ppp0"));
				dailFailCount = 0;
			}

			if(SendDialingCmd())
			{
				//PUB_Sleep(10000);

				//用select定时看看会不会被中断
				int rc = 0;
				struct timeval tv;
				tv.tv_sec = 10;
				tv.tv_usec = 0;
				rc = select(0, NULL, NULL, NULL, &tv);
				//printf("select = %d\n", rc);

				for(num = 0 ;num<5; num ++ )
				{
					if(CheckDialing())
					{
						printf("3g get IP success!\n");
						ret = true;
						m_WirelessState = true;
						m_bDialing = true;
						m_bCheck = true;
						PUB_Sleep(10000);
						break;
					}
					else
					{	
						m_bDialing = false;
						m_WirelessState = false;
						printf("3g get IP err!\n");
						if(num == 4)
						{
#if !defined(__X_LINUX__)
							if (DVRSystem("rm -rf /var/lock/*") != 0)
							{
								printf("rm -rf lock err!\n");
								continue;
							}
#endif
							PUB_Sleep(200);
						}
					}
				}
			}
		}
		else
		{
			dailFailCount = 0;
		}

		if (!m_bDialing)
		{
			dailFailCount++;
		}



		PUB_Sleep(10000);
	}
	return ret;
}
void CWirelessManage::GetDNSName(char* name, unsigned short len)
{
	FILE* pfile = fopen("/etc/resolv.conf", "r+b");
	char getline[128] ={0};
	if(pfile != NULL)
	{
		while(fgets(getline, sizeof(getline), pfile))
		{	
			if(strstr(getline, "nameserver") !=NULL)
			{
				if(sscanf(getline, "nameserver %s", name) >0)
				{
					if( (strlen(name)  > 0) && (strcmp("0.0.0.0", name) != 0))
					{
						break;
					}
				}

			}
			memset(getline, 0, sizeof(getline));	
		}
		fclose(pfile);
		pfile = NULL;
	}
}
bool CWirelessManage::GetWirelessState()
{
	return  m_WirelessState;
}


bool CWirelessManage::GetWirelessThdState()
{
	return m_bHotPlug;
}

bool CWirelessManage::StartWireless()
{
	if(!m_bWireless)
	{
		m_WirelessProcID = PUB_CreateThread(WirelessThread, (void *)this, &m_bWireless);
		if( PUB_CREATE_THREAD_FAIL == m_WirelessProcID )
		{
			printf("Initial alarm detect thread fail!");
			return false;
		}
	}

	if(m_WirelessHotPlugID == PUB_CREATE_THREAD_FAIL)
	{
		m_WirelessHotPlugID = PUB_CreateThread(WirelessHotPlug, (void *)this, &m_bHotPlug);
		if(PUB_CREATE_THREAD_FAIL == m_WirelessHotPlugID)
		{
			printf("Initial wireless HotPlug thread fail!\n");
			return false;
		}
	}

	return true;
}

RESULT WINAPI CWirelessManage::WirelessThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	assert(NULL != lpParameter);

	CWirelessManage *pThis = reinterpret_cast<CWirelessManage *>(lpParameter);

	pThis->WirelessProc();

	return 0;
}

RESULT WINAPI CWirelessManage::WirelessHotPlug(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	assert(NULL != lpParameter);

	CWirelessManage *pThis = reinterpret_cast<CWirelessManage *>(lpParameter);

	pThis->HotPlug();

	return 0;
}

bool CWirelessManage::HotPlug()
{
	pthread_detach(pthread_self());

	int sockfd = -1;
    struct sockaddr_nl sa;
    int len;
    char buf[4096];
    struct iovec iov;
    struct msghdr msg;
    int i;

    memset(&sa,0,sizeof(sa));
    sa.nl_family=AF_NETLINK;
    sa.nl_groups=NETLINK_KOBJECT_UEVENT;
    sa.nl_pid = 0;//getpid(); both is ok
    memset(&msg,0,sizeof(msg));
    iov.iov_base=(void *)buf;
    iov.iov_len=sizeof(buf);
    msg.msg_name=(void *)&sa;
    msg.msg_namelen=sizeof(sa);
    msg.msg_iov=&iov;
    msg.msg_iovlen=1;
    char cmdbuf[512] = {0};
	//sprintf(cmdbuf, "/etc/ppp/huawei-ppp-off %s", m_3gDeviceName);
	sprintf(cmdbuf, "kill -KILL `ps w | grep -v grep | grep \"%s\" | grep \"pppd\" | awk '{ print $1; exit}'`", m_3gDeviceName);

	char* pos = NULL;

	sockfd=socket(AF_NETLINK,SOCK_RAW,NETLINK_KOBJECT_UEVENT);
	if(sockfd==-1)
	{
	    printf("socket creating failed:%s\n",strerror(errno));
	    return false;
	}
	
	if(bind(sockfd,(struct sockaddr *)&sa,sizeof(sa))==-1)
	{
	    printf("bind error:%s\n",strerror(errno));
	    close(sockfd);
	    return false;
	}

    while(m_bHotPlug)
    {
	    len=recvmsg(sockfd,&msg,0);
	    if(len<0)
	        printf("receive error\n");
	    else if(len<32||len>sizeof(buf))
	        printf("invalid message");
	    for(i=0;i<len;i++)
	        if(*(buf+i)=='\0')
	            buf[i]='\n';


	    if(m_3gDeviceName[0] != 0)
	    {
	    	pos = strrchr(m_3gDeviceName, '/');	
	    	printf("%s\n", pos+1);
	    }
	    if(strstr(buf, "ACTION=remove") && m_bWireless && pos != NULL && strstr(buf, pos+1) )
	    {
	    	printf("received %d bytes\n%s\n",len,buf);

			if (PUB_THREAD_ID_NOINIT != m_WirelessProcID)
			{
				pthread_cancel(m_WirelessProcID);
				m_WirelessState = false;
				m_bWireless 	= false;
				m_bWirelessInit = false;
				m_WirelessProcID = PUB_THREAD_ID_NOINIT;
				m_bDialing = false;

				LogoutDialing();
			}
	    }
	    char tmp[128];
	    if(strstr(buf, "ACTION=add") && !m_bWireless && GetSerialDevName(tmp, 128))
	    {
   		    printf("received %d bytes\n%s\n",len,buf);

	    	StartWireless();
	    }

	}
	return true;
}

void CWirelessManage::Stop()
{	
	if (PUB_THREAD_ID_NOINIT != m_WirelessProcID)
	{		
		pthread_cancel(m_WirelessProcID);		//使用PUB_ExitThread退出线程弊端：在界面关闭无线后，若快速插入3g设备，使wirelessproc中循环条件为真，导致wirelessproc无法退出，进而界面假死，无法操作
		m_WirelessProcID = PUB_THREAD_ID_NOINIT;
		m_WirelessState = false;
		m_bWireless 	= false;
		m_bWirelessInit = false;
	}
	
	//注销拨号	
	LogoutDialing();
	
 	Quit();

 	if(PUB_THREAD_ID_NOINIT != m_WirelessHotPlugID)
 	{
 		pthread_cancel(m_WirelessHotPlugID);
 		m_WirelessHotPlugID = PUB_THREAD_ID_NOINIT; 		
 		m_bHotPlug = false;
 	}
}

void  CWirelessManage::SetWirelessPara(const MDVR_WIRELESS_CFG *para)
{
	assert(para != NULL);
	memcpy(&m_wirelesscfg, para, sizeof(m_wirelesscfg));		
}

//查讯SIM 卡是否存在
bool CWirelessManage::CheckWhetherHaveCard()
{
	char cmd[20]= "AT+CPIN?\r\n";
	char buf[128] ={0};
	char info[50] ={0};
	
	if((m_wirelesscfg.type == 1) ||(m_wirelesscfg.type == 2))
	{	
		if(WriteToPort(cmd, strlen(cmd), SBR_115200))
		{
			if(ReadDate((BYTE *)buf, 40, SBR_115200) > 0)
			{	
				char *p = strtok(buf, "\r\n");
				while(p != NULL)
				{	
					if(strstr(p, "+CPIN:") != NULL)
					{
						if(sscanf(p, "+CPIN:%s", info) > 0)
						{
							if(strcmp(info, "READY") == 0)
							{
								return true;
							}
							else
							{
								return false;
							}
						}
					}
					p = strtok(NULL, "\r\n");
				}
			
			}
		}
		else
		{
			printf("witetoUsb port err!\n");
		}
	}
	return false;
}


//查询模块是否注册上 网络
bool CWirelessManage::CheckWhetherHaveNet()
{
	char cmd[20] = "AT+CREG?\r\n";
	char  buf[128] ={0};
	char info[50] ={0};

	//WCDMA
	if(m_wirelesscfg.type == 1)
	{	
		if(WriteToPort(cmd, strlen(cmd), SBR_115200))
		{
			if(ReadDate((BYTE*)buf, 40, SBR_115200) > 0)
			{
				char *p = strtok(buf, "\r\n");
				while(p != NULL)
				{
					if(strstr(p, "+CREG:") != NULL)
					{	
						if(sscanf(p, "+CREG:%s", info) > 0)
						{	
							if(info[2] == 0x31 || info[2] == 0x35)
							{
								return true;
							}
							else
							{
								return false;
							}
						}
					}
					p = strtok(NULL, "\r\n");
				}
			}
		}
		else
		{
			printf("witetoUsb port err!\n");
		}
	}
	//EVDO
	else if(m_wirelesscfg.type == 2)
	{
		return true;
	}

	return false;
}


//查询当前信号值
bool CWirelessManage::CheckWhetherHaveSignal()
{
	char cmd[20] = "AT+CSQ\r\n";
	char buf[128] ={0};
	char info[50] ={0};
	//int temp = 0;
	
	//WCDMA || EVDO
	if((m_wirelesscfg.type == 1) || (m_wirelesscfg.type == 2))
	{	
		if(WriteToPort(cmd, strlen(cmd), SBR_115200))
		{
			if(ReadDate((BYTE *)buf,50, SBR_115200) > 0)
			{
				char *p = strtok(buf, "\r\n");
				while(p != NULL)
				{
					if(strstr(p, "+CSQ:") !=NULL)
					{
						if(sscanf(p, "+CSQ:%s", info) > 0)
						{
							m_signal = (info[0]-0x30)*10+(info[1]-0x30); 
							if((m_signal != 0) && (m_signal != 99))
							{
								return true;
							}
							else
							{
								return false;
							}
						}
					}
					p = strtok(NULL, "\r\n");
				}	
			}
		}
		else
		{
			printf("witetoUsb port err!\n");
		}
	}

	return false;
}

//获取信号强度暂定为5格,返回参数1-5分别代表1-5格,若返回0:表示无信号
unsigned char CWirelessManage::GetSignal()
{
	unsigned char level = 0;

	if(m_signal != 0)
	{
		level = m_signal/6;
	}

	return level;
}

//返回值false: 表示3g模块不存在,true: 表示存在
bool CWirelessManage::Get3GState()
{
	int fd;
	bool existflg = false;
	char deviceName[128];
	GetSerialDevName(deviceName, 128);
	fd = open(deviceName,  O_RDWR);
	if(fd != -1)
	{
		existflg = true;
		close(fd);
	}
	return existflg;
}

//返回值false:表示有卡, true: 表示没有卡
bool CWirelessManage::GetCardState()
{
	bool cardexist = false;

	if(!m_bWirelessInit)
	{
		Quit();
		if(!Initial())
		{
			printf("init wireless USb port error!!\n");
			return false;
		}
	}
	
	for(int i = 0; i <10; i++)
	{
		if(!CheckWhetherHaveCard() )
		{
			//printf("the 3G whether have card\n");
			PUB_Sleep(600);
		}
		else
		{
			m_bhavecard = true;
			return true;
		}
	}
	return false;
}


//返回值false:表示拨号失败, true:表示成功
bool CWirelessManage::GetDialingState()
{
	return m_bDialing;
}

//返回值type =1: 表示WCDMA, type = 2: 表示evdo
unsigned short CWirelessManage::Get3GType()
{
	return m_wirelesscfg.type;
}
// 返回值 1开启 0关闭
unsigned short CWirelessManage::Get3GEnabled()
{
	return m_wirelesscfg.enable;
}

bool CWirelessManage::WirelessUp(const char* name, unsigned short len)
{
	CPUB_LockAction	funlock(&m_lock);
	CNetInterface *pNetInter = CNetInterface::Instance();
	bool flag = false;
	//接口up
	if(NET_INTERFACE_DOWN == pNetInter->GetInterfaceStatus(name))
	{		
		if(pNetInter->UpInterface(name, len))
		{
			flag = true;
		}
	}
	return flag;
}

bool CWirelessManage::WirelessDown(const char* name, unsigned short len)
{
	CPUB_LockAction	funlock(&m_lock);
	CNetInterface *pNetInter = CNetInterface::Instance();
	bool flag = false;
	
	if(NET_INTERFACE_UP == pNetInter->GetInterfaceStatus(name))
	{
		printf("%s, %d\n", __FILE__, __LINE__);
		if(pNetInter->DownInterface(name,len))
		{
			flag = true;
		}
	}
	return flag;
}

#if 1

//拨号文件
bool CWirelessManage::SendDialingCmd(void)
{
	//char * cmd1 = "echo "AT+CGDCONT=1,\"IP\",\"3GNET\"" > /dev/ttyUSB0";
	//char * cmd1 = "/etc/ppp/huawei-ppp-on --pn=*99# > /etc/ppp/dialingtmp.dat";
	
	char *info = "AT+CGDCONT=1,";
	//char *cmd = "/etc/ppp/huawei-ppp-on --pn=";
	char cmd[50];
	sprintf(cmd, "/etc/ppp/huawei-ppp-on --dev=%s --pn=", m_3gDeviceName);
	printf("devicename ===========================> %s\n*********************\n", cmd);
	char cmdbuf[512] = {0};
	int a = 0x22, b= 0x5c;

	if(m_wirelesscfg.type == 1)
	{
		//这个命令执行成功一次就不要再执行了
		if(!m_bBak)
		{
			sprintf(cmdbuf, "echo %c%s%c%cIP%c%c,%c%c%s%c%c%c > %s", a, info, b, a, b, a, b,a,m_wirelesscfg.dialup[0].APNNumber,b,a,a, m_3gDeviceName);
			printf("cmdbuf=%s\n", cmdbuf);	

			if(DVRSystem(cmdbuf) != 0)
			{
				printf("wcdma set ttyUsb0 error!\n");
				return false;
			}
			else
			{
				m_bBak = true;
			}

			usleep(400);
		}

		//国内默认为空
		memset(cmdbuf, 0, sizeof(cmdbuf));
		if((strcmp(m_wirelesscfg.dialup[0].password, "") == 0) && (strcmp(m_wirelesscfg.dialup[0].username, "") == 0))
		{
			sprintf(cmdbuf, "%s%s > /etc/ppp/dialingtmp.dat", cmd, m_wirelesscfg.dialup[0].ConnectPoint);
		}
		else
		{
			sprintf(cmdbuf, "%s%s --usr=%s --pwd=%s > /etc/ppp/dialingtmp.dat", cmd, m_wirelesscfg.dialup[0].ConnectPoint, m_wirelesscfg.dialup[0].username, m_wirelesscfg.dialup[0].password);
		}
		printf("cmdbuf=%s\n", cmdbuf);
		
		if(DVRSystem(cmdbuf) != 0)
		{
			printf("wcdma Dialing up error\n");
			return false;
		}
		
		//usleep(9000000);
	}
	else if(m_wirelesscfg.type == 2)
	{	
		//echo "AT^PPPCFG=\"CARD\",\"CARD\"" > /dev/ttyUSB0
		// /etc/ppp/huawei-ppp-on --pn=#777 --usr=CARD --pwd=CARD
		//这个命令执行成功一次就不要再执行了
		if(!m_bBak)
		{
			sprintf(cmdbuf, "echo %cAT^PPPCFG=%c%c%s%c%c,%c%c%s%c%c%c > %s", a,b,a, m_wirelesscfg.dialup[1].username,b, a,   b,a,m_wirelesscfg.dialup[1].password,b,a,a, m_3gDeviceName);
			printf("cmdbuf=%s\n", cmdbuf);
			
			if(DVRSystem(cmdbuf) != 0)
			{
				printf("evdo set ttyUsb0 error!\n");
				return false;
			}
			else
			{
				m_bBak = true;
			}

			usleep(400);
		}
		memset(cmdbuf , 0, sizeof(cmdbuf));
		sprintf(cmdbuf, "%s%s --usr=%s --pwd=%s > /etc/ppp/dialingtmp.dat", cmd, m_wirelesscfg.dialup[1].ConnectPoint, m_wirelesscfg.dialup[1].username, m_wirelesscfg.dialup[1].password);
		printf("cmdbuf=%s\n", cmdbuf);
		if(DVRSystem(cmdbuf) != 0)
		{
			printf("evdo Dialing up error\n");
			return false;
		}
		//usleep(9000000);
	}
	if(m_fd != -1)
	{
		close(m_fd);
		perror("close m_fd");
		m_fd = -1;
		printf("%s, %d close 3g fd\n", __FILE__, __LINE__);
	}
	return true;
	
}

//注销拨号连接
bool CWirelessManage::LogoutDialing()
{
	char cmdbuf[512] = {0};
	//sprintf(cmdbuf, "/etc/ppp/huawei-ppp-off %s", m_3gDeviceName);
	sprintf(cmdbuf, "kill -KILL `ps w | grep -v grep | grep \"%s\" | grep \"pppd\" | awk '{ print $1; exit }'`", m_3gDeviceName);
	printf("cmdbuf:%s\n", cmdbuf);
	//char *cmd ="/etc/ppp/huawei-ppp-off /dev/ttyUSB0";

	//返回值:PPP link to /dev/ttyUSB0 terminated
	if(DVRSystem(cmdbuf) != 0)
	{
		printf("LogoutDialing err!\n");
		return false;
	}
	return true;
}

//查看拨号状态
bool CWirelessManage::CheckDialing(void)
{
	CNetInterface *pNetInter = CNetInterface::Instance();
	unsigned long GetIP = 0;
	char ipbuf[20] = {0};

	if(pNetInter->GetIPText(ipbuf, 20, "ppp0", 4))
	{
		printf("ipbuf=%s\n", ipbuf);

		return true;
	}
	else 
	{
		bool ret = false;
		if(DVRSystem("ifconfig > /mnt/mtd/ipinfo.dat") == 0)
		{
			FILE *pfile = fopen("/mnt/mtd/ipinfo.dat", "rb");
			char getline[128] ={0};
			char  ipinfo[30] = {0};
			if(pfile != NULL)
			{
				while(fgets(getline, sizeof(getline), pfile))
				{	
					//printf("----getline=%s\n", getline);
					if(strstr(getline, "ppp0") !=NULL)
					{
						if(strstr(getline, "inet addr:") != NULL)
						{
							if(sscanf(getline, "inet addr:%s", ipinfo) >0)
							{
								if(strlen(ipinfo)  > 0)
								{
									ret = true;
									break;
								}
							}
						}
					}
					memset(getline, 0, sizeof(getline));	
				}
				fclose(pfile);
				pfile = NULL;
			}
		}
		
		return ret;
	}

#if 0
	else
	{
		FILE *pfile = fopen("/etc/ppp/dialingtmp.dat", "rb");
		char getline[128] ={0};
		char IPInfo[30] ={0};
		bool ret = false;
		if(pfile != NULL)
		{	
			while(fgets(getline, sizeof(getline), pfile))
			{
				//printf("getline=%s\n", getline);
				if(strstr(getline, "local  IP address") != NULL)
				{
					if(sscanf(getline, "local  IP address %s", IPInfo) > 0)
					{	
						printf("----IPInfo=%s\n", IPInfo);
						if(strlen(IPInfo) > 0 )
						{
							
							ret =  true;
							break;
						}
					}
				}
				memset(getline, 0, sizeof(getline));	
			}

			fclose(pfile);
			pfile = NULL;
		}
		return ret;
	}
#endif
	return false;
}	

//在插上3G网卡后，在/proc/tty/driver/usbserial文件中列出四行信息
//usbserinfo:1.0 driver:2.0
//0: name:"GSM modem (1-port)" vendor:12d1 product:1001 num_ports:1 port:1 path:usb-hi3515v100-ehci-2
//1: name:"GSM modem (1-port)" vendor:12d1 product:1001 num_ports:1 port:1 path:usb-hi3515v100-ehci-2
//2: name:"GSM modem (1-port)" vendor:12d1 product:1001 num_ports:1 port:1 path:usb-hi3515v100-ehci-2
//拔下3G网卡后只剩下第一行。如果程序启动了3G连接。拔下3G网卡后由于设备/dev/ttyUSB2处于打开状态，最后
//一行还存在。所以判断3G卡是否断开就通过改文件信息行数来判断。当程序启动了3G连接，如果断开3G卡，通过下
//面函数去判断是否断开，然后关掉打开/dev/ttyUSB2文件描述符，才能正确的再次连接3G网络。
bool CWirelessManage::CheckWhetherHaveDevice()
{
	if (0 != access("/dev/ttyUSB0", R_OK))
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

//获取无线网卡名称，通过四步获取
//1.mount -t usbfs none /proc/bus/usb
//2.read /proc/bus/usb/devices
//3.get ttyusb "Driver=option" "(Int.)" 
//4.umount /proc/bus/usb
bool CWirelessManage::GetSerialDevName(char* device, int len)
{
	assert(device == NULL);
	FILE* procG3 = fopen("/proc/bus/usb/devices", "r");
	if (NULL == procG3)
	{
		return false;
	}

	int index = -1;
	bool bFindStart = false;
	char str[256] = {0};
	while (fgets(str, 256, procG3))
	{
		if (NULL != strstr(str, "Driver=option"))
		{
			bFindStart = true;
			index ++;
		}
		else if ((true == bFindStart) && (NULL != strstr(str, "(Int.)")))
		{
			printf("%s:%s:%d, get the 3g usb device %d, \n", __FUNCTION__, __FILE__, __LINE__, index);
			break;
		}
		else
		{
			bFindStart = false;
		}
	}

	fclose(procG3);

	if (-1 < index)
	{
		snprintf(device, len, "/dev/ttyUSB%d", index);
		return true;
	}
	else
	{
		return false;
	}
}

NET_CTRL_FAIL CWirelessManage::CheckWireless()
{
	if (!Initial())
	{
		return	NETERR_NETPARAMCHECK_ENODEVICE;
	}
	else
	{
		m_bWirelessInit = true;
	}
	if (!CheckWhetherHaveCard())
	{
		Quit();
		return NETERR_NETPARMACHECK_ENOCARD;
	}
	if (!CheckWhetherHaveSignal())
	{
		Quit();
		return NETERR_NETPARMACHECK_ENOSIGNAL;
	}
	if (!CheckWhetherHaveNet())
	{
		Quit();
		return NETERR_NETPARMACHECK_ENETUNREACHD;
	}
	if(SendDialingCmd())
	{

		int rc = 0;
		struct timeval tv;
		tv.tv_sec = 10;
		tv.tv_usec = 0;
		rc = select(0, NULL, NULL, NULL, &tv);

		for(int num = 0 ;num<5; num ++ )
		{
			if(CheckDialing())
			{
				printf("3g get IP success! %s, %d\n", __FILE__, __LINE__);
				m_WirelessState = true;
				m_bDialing = true;
				m_bCheck = true;
				PUB_Sleep(5000);
				return	NETERR_SUCCESS;
			}
			else
			{	
				m_bDialing = false;
				printf("3g get IP err!\n");
				if(num == 4)
				{
					if (DVRSystem("rm -rf /var/lock/*") != 0)
					{
						printf("rm -rf lock err!\n");

						Quit();
						return NETERR_NETPARAMCHECK_EUNKNOWN;
					}
					PUB_Sleep(200);
				}
			}
		}

		Quit();
		return	NETERR_NETPARAMCHECK_EPARAM;

	}
	else
	{
		Quit();
		return  NETERR_NETPARAMCHECK_EUNKNOWN;
	}
}

#endif