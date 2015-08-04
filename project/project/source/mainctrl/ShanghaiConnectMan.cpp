#include "ShanghaiConnectMan.h"
#include "MessageMan.h"
const unsigned long maxDelayTime =75;//ͼƬ����ʱ������ӳ�75��
const unsigned long maxPicCount = 40;//����ͼƬ�������
CShanghaiConnectMan *CShanghaiConnectMan::Instance()
{
	static CShanghaiConnectMan ShanghaiConnectMan;
	return &ShanghaiConnectMan;
}

CShanghaiConnectMan::CShanghaiConnectMan()
{
	m_listCmd.clear();
	m_listMotionPic.clear();
	m_listsensorPic.clear();
	memset(&m_hear_info,0,sizeof(HEARTINFO));
	memset(m_recvTime,0,sizeof(m_recvTime));
	
	m_pReclogMan = NULL;
	m_DVrStart = 0 ;
	m_recvTimeLeap = 0;//�������յ���ʱ����
	m_heartTime = 0;
	m_benable = false;

	memset(m_agentId,0,sizeof(m_agentId));
	m_port = 0;
	memset(m_ip,0,sizeof(m_ip));

	m_sensorpicCount = 0;
	m_motionpicCount = 0;
	m_sensorAlarmStartTime = 0;
	m_motionAlarmStartTime = 0;
	m_maxpicdelayTime = maxDelayTime;

	m_triggerTime = 0;
	m_chnnBits = 0;
}

CShanghaiConnectMan::~CShanghaiConnectMan()
{

}

void CShanghaiConnectMan::Initial()
{
	m_heartID	= PUB_CREATE_THREAD_FAIL;
	m_bheartProc = false;
	m_heartsock = SWL_INVALID_SOCKET;
	
	m_bmotionHead = false;
	m_bsensorHead = false;

	m_bstartHeart = false;
}

void CShanghaiConnectMan::Quit()
{
	m_pReclogMan = NULL;
}

bool CShanghaiConnectMan::Start()
{
	if ( (PUB_CREATE_THREAD_FAIL == m_heartID) && (false == m_bheartProc) )
	{
		m_heartID = PUB_CreateThread( HeartThread, (void *)this,(bool *)(&m_bheartProc));
		if( PUB_CREATE_THREAD_FAIL == m_heartID )
		{
			PUB_PrintError(__FILE__,__LINE__);
			return false;
		}
	}
	return true;
}

void CShanghaiConnectMan::Stop()
{
	m_PTLock.Lock();
	if(SWL_INVALID_SOCKET != m_heartsock)
	{
		SWL_CloseSocket(m_heartsock);
		m_heartsock = SWL_INVALID_SOCKET;
	}
	for(std::list<OPER_INFO>::iterator it = m_listCmd.begin();it != m_listCmd.end();++it)
	{
		if(0 != it->operType )
		{
			if(NULL != it->pNote)
			{
				delete [] it->pNote;
				it->pNote = NULL;
			}
			if(NULL != it->pPic)
			{
				delete [] it->pPic;
				it->pPic = NULL;
			}
			it->operType = 0;
		}
	}

	for(std::list<OPER_INFO>::iterator it = m_listMotionPic.begin();it != m_listMotionPic.end();++it)
	{
		if(0 != it->operType )
		{
			if(NULL != it->pNote)
			{
				delete [] it->pNote;
				it->pNote = NULL;
			}
			if(NULL != it->pPic)
			{
				delete [] it->pPic;
				it->pPic = NULL;
			}
			it->operType = 0;
		}
	}

	for(std::list<OPER_INFO>::iterator it = m_listsensorPic.begin();it != m_listsensorPic.end();++it)
	{
		if(0 != it->operType )
		{
			if(NULL != it->pNote)
			{
				delete [] it->pNote;
				it->pNote = NULL;
			}
			if(NULL != it->pPic)
			{
				delete [] it->pPic;
				it->pPic = NULL;
			}
			it->operType = 0;
		}
	}

	m_listCmd.clear();
	m_listMotionPic.clear();
	m_listsensorPic.clear();
	m_PTLock.UnLock();
	PUB_ExitThread(&m_heartID, &m_bheartProc);

}


void CShanghaiConnectMan::SetPTEnabel(bool benable)
{
	m_benable = benable;
	if(!m_benable)
	{
		m_PTLock.Lock();
		if(SWL_INVALID_SOCKET != m_heartsock)
		{
			SWL_CloseSocket(m_heartsock);
			m_heartsock = SWL_INVALID_SOCKET;
		}
		//�����ȡ��ƽ̨������Ϣ����ͼƬ������б�����ȫ����գ����Ҳ��ٽ���
		for(std::list<OPER_INFO>::iterator it = m_listCmd.begin();it != m_listCmd.end();++it)
		{
			if(0 != it->operType )
			{
				if(NULL != it->pNote)
				{
					delete [] it->pNote;
					it->pNote = NULL;
				}
				if(NULL != it->pPic)
				{
					delete [] it->pPic;
					it->pPic = NULL;
				}
			}
		}

		for(std::list<OPER_INFO>::iterator it = m_listMotionPic.begin();it != m_listMotionPic.end();++it)
		{
			if(0 != it->operType )
			{
				if(NULL != it->pNote)
				{
					delete [] it->pNote;
					it->pNote = NULL;
				}
				if(NULL != it->pPic)
				{
					delete [] it->pPic;
					it->pPic = NULL;
				}
			}
		}

		for(std::list<OPER_INFO>::iterator it = m_listsensorPic.begin();it != m_listsensorPic.end();++it)
		{
			if(0 != it->operType )
			{
				if(NULL != it->pNote)
				{
					delete [] it->pNote;
					it->pNote = NULL;
				}
				if(NULL != it->pPic)
				{
					delete [] it->pPic;
					it->pPic = NULL;
				}
			}
		}

		m_listCmd.clear();
		m_listMotionPic.clear();
		m_listsensorPic.clear();
		m_PTLock.UnLock();
		m_DVrStart = 0;
		m_heartTime =0;
	}
	else
	{
	
	}
	
}

bool CShanghaiConnectMan::GetPTEnable()
{
	return m_benable;
}
void CShanghaiConnectMan::SetMotionEnable(bool benable)
{
	m_bmotionEnable = benable;
}

void CShanghaiConnectMan::SetStartHeartProc()
{
	m_bstartHeart = true;
}

void CShanghaiConnectMan::SetAgentId(const char *pAgentId)
{
	if(0 != strcmp(pAgentId,m_agentId))
	{
		strcpy(m_agentId,pAgentId);
		m_heartTime =0;//���·�������
		m_recvTimeLeap = 0;
	}
	
}

void CShanghaiConnectMan::Setport(unsigned long port)
{
	if(port != m_port)
	{
		m_port = port;
		m_heartTime =0;//���·�������
		m_recvTimeLeap = 0;
	}
	
}

void CShanghaiConnectMan::SetIp(const char *pIp)
{
	if( 0 == strcmp(pIp,"sgdzpic.3322.org"))
	{
		if(0 != strcmp(m_ip,"112.65.150.62"))
		{
			strcpy(m_ip,"112.65.150.62");
			m_heartTime =0;//���·�������
			m_recvTimeLeap = 0;
		}
	
	}
	else
	{
		if(0 != strcmp(m_ip,pIp))
		{
			strcpy(m_ip,pIp);
			m_heartTime =0;//���·�������
			m_recvTimeLeap = 0;
		}
	}
	
}

void CShanghaiConnectMan::GetHeartInfo(CReclogManEX	*pReclogMan)
{
	m_pReclogMan = pReclogMan;
}

void CShanghaiConnectMan::GetLogCmd(unsigned long type,unsigned long chnn,char *ptext,int textLen,void *pPicBuf,int picBufLen,ULONGLONG time)
{
	if(!m_benable)
	{
		return;
	}
	ULONGLONG curTime = 0;
	tm curTm;
	char tmpTime[256] = {0};
	OPER_INFO shanghaiLogInfo;
	memset(&shanghaiLogInfo,0,sizeof(OPER_INFO));
	shanghaiLogInfo.operType = type;
	shanghaiLogInfo.chnn = chnn;

	curTime = time;
	memset(&curTm,0,sizeof(tm));
	curTm = DVRTimeToTm(curTime);
	memset(tmpTime,0,sizeof(tmpTime));
	snprintf(tmpTime,256,"%d-%d-%d %d:%d:%d",curTm.tm_year+1900,curTm.tm_mon+1,curTm.tm_mday,curTm.tm_hour,curTm.tm_min,curTm.tm_sec);
	strcpy(shanghaiLogInfo.triggerTime,tmpTime);

	//��log���ֽ���ת��
	{
		if(0 != textLen)
		{
			int tmpTextLen=0;
			char tmpbuf[maxShanghaiDataSize]={0};
			tmpTextLen = to64frombits((unsigned char *)tmpbuf,(unsigned char *)ptext,textLen);
			if(0 != tmpTextLen)
			{
				shanghaiLogInfo.pNote = new unsigned char[tmpTextLen+1];
				shanghaiLogInfo.textLen = tmpTextLen;
				memcpy(shanghaiLogInfo.pNote,tmpbuf,tmpTextLen);
			}
			else
			{
				assert(false);
				shanghaiLogInfo.pNote = NULL;
				shanghaiLogInfo.textLen = 0;
			}
		}
		

	}
	//��ͼƬ����ת��
	{
		if(0 != picBufLen)
		{
			int picLen=0;
			char *picbuf = new char[picBufLen*2];
			memset(picbuf,0,picBufLen*2);
			picLen = CShanghaiConnectMan::Instance()->to64frombits((unsigned char *)picbuf,(unsigned char *)pPicBuf,picBufLen);
			if(0 != picLen)
			{
				shanghaiLogInfo.picLen = picLen;
				shanghaiLogInfo.pPic = new unsigned char[picLen];
				memcpy(shanghaiLogInfo.pPic,picbuf,picLen);
			}
			else
			{
				shanghaiLogInfo.picLen =0;
				shanghaiLogInfo.pPic = NULL;

			}
			if(NULL != picbuf)
			{
				delete []picbuf;
				picbuf = NULL;
			}
		}
		else
		{
			shanghaiLogInfo.picLen =0;
			shanghaiLogInfo.pPic = NULL;
		}
	}
	uuid_generate_time(shanghaiLogInfo.uniqueGUID);
	GetOperCmd(shanghaiLogInfo);
}



void CShanghaiConnectMan::GetMotionAlarmSnap(OPER_INFO snapInfo)
{
	if(!m_benable)
	{
		return;
	}
	if(!m_bmotionEnable)
	{
		return;
	}

	if(m_listMotionPic.size()>maxCmdCount)//����100�Ų����������
	{
		return;
	}
	OPER_INFO tmpInfo;
	memset(&tmpInfo,0,sizeof(OPER_INFO));
	tmpInfo.operType = snapInfo.operType;
	tmpInfo.chnn = snapInfo.chnn;

	strcpy(tmpInfo.triggerTime,snapInfo.triggerTime);
	tmpInfo.textLen = snapInfo.textLen;

	if(0 != tmpInfo.textLen)
	{
		tmpInfo.pNote = new unsigned char[tmpInfo.textLen];
		memcpy(tmpInfo.pNote,snapInfo.pNote,tmpInfo.textLen);
	}
	else
	{
		tmpInfo.pNote =NULL;
		tmpInfo.textLen = 0;
	}

	tmpInfo.picLen = snapInfo.picLen;

	if(0 != tmpInfo.picLen)
	{
		tmpInfo.pPic = new unsigned char[tmpInfo.picLen];
		memset(tmpInfo.pPic,0,tmpInfo.picLen);
		memcpy(tmpInfo.pPic,snapInfo.pPic,tmpInfo.picLen);
	}
	else
	{
		tmpInfo.pPic =NULL;
		tmpInfo.picLen = 0;
	}

	uuid_copy(tmpInfo.uniqueGUID,snapInfo.uniqueGUID);
	m_listMotionPic.push_back(tmpInfo);
}

void CShanghaiConnectMan::GetSensorAlarmSnap(OPER_INFO snapInfo)
{
	if(!m_benable)
	{
		return;
	}
	OPER_INFO tmpInfo;
	memset(&tmpInfo,0,sizeof(OPER_INFO));
	tmpInfo.operType = snapInfo.operType;
	tmpInfo.chnn = snapInfo.chnn;
	strcpy(tmpInfo.triggerTime,snapInfo.triggerTime);
	tmpInfo.textLen = snapInfo.textLen;

	if(0 != tmpInfo.textLen)
	{
		tmpInfo.pNote = new unsigned char[tmpInfo.textLen];
		memcpy(tmpInfo.pNote,snapInfo.pNote,tmpInfo.textLen);
	}
	else
	{
		tmpInfo.pNote =NULL;
		tmpInfo.textLen = 0;
	}

	tmpInfo.picLen = snapInfo.picLen;

	if(0 != tmpInfo.picLen)
	{
		tmpInfo.pPic = new unsigned char[tmpInfo.picLen];
		memset(tmpInfo.pPic,0,tmpInfo.picLen);
		memcpy(tmpInfo.pPic,snapInfo.pPic,tmpInfo.picLen);
	}
	else
	{
		tmpInfo.pPic =NULL;
		tmpInfo.picLen = 0;
	}

	uuid_copy(tmpInfo.uniqueGUID,snapInfo.uniqueGUID);
	m_listsensorPic.push_back(tmpInfo);
}

int CShanghaiConnectMan::to64frombits(unsigned char *out, const unsigned char *in, int inlen)
{
	static const char base64digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int iLen = inlen;
	for (; inlen >= 3; inlen -= 3)
	{
		*out++ = base64digits[in[0] >> 2];
		*out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
		*out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
		*out++ = base64digits[in[2] & 0x3f];
		in += 3;
	}

	if (inlen > 0)
	{
		unsigned char fragment;

		*out++ = base64digits[in[0] >> 2];
		fragment = (in[0] << 4) & 0x30;

		if (inlen > 1)
			fragment |= in[1] >> 4;

		*out++ = base64digits[fragment];
		*out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
		*out++ = '=';
	}
	*out   = '\0';
	return (iLen / 3 + ((inlen % 3 == 0) ? 0 : 1)) * 4;
}

void CShanghaiConnectMan::SetMotionHead(bool bhead)
{
	m_bmotionHead = bhead;
	m_motionpicCount = 0;
}

void CShanghaiConnectMan::SetSensorHead(bool bhead)
{
	m_bsensorHead = bhead;
	m_sensorpicCount = 0;
}

bool  CShanghaiConnectMan::GetMotionHead()
{
	if(m_bmotionHead)//�Ѿ���ͷ��
	{
		return false;
	}
	return true;
}

bool  CShanghaiConnectMan::GetSensorHead()
{
	if(m_bsensorHead)//�Ѿ���ͷ��
	{
		return false;
	}
	return true;
}

void CShanghaiConnectMan::SetMotionAlarmStartTime(unsigned long time)
{
	m_motionAlarmStartTime = time;
}

void CShanghaiConnectMan::SetSensorAlarmStartTime(unsigned long time)
{
	m_sensorAlarmStartTime = time;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CShanghaiConnectMan::HeartThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);

	printf("%s, %d, HeartThread pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif
	if ( NULL != pParam)
	{
		CShanghaiConnectMan *pShanghaiConnectMan = (CShanghaiConnectMan *)pParam;
		pShanghaiConnectMan->HeartProc();
	} 
	else
	{
		printf("%s:%s:%d, NULL == pParam\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
	}

	return 0;
}

void CShanghaiConnectMan::HeartProc()
{
	char dvrType[256] = {0};
	char tmpTime[256] = {0};
	char buf[recvSendBufLen] = {0};	
	TiXmlDocument *pMyDocument = NULL;

	ULONGLONG curTime = 0;
	tm curTm;
	unsigned char *pXmlContent =NULL;
	int xmlLen =0;

	//�������ˣ�ÿ�붼ȥ���
	while (m_bheartProc)
	{
		PUB_Sleep(10);

		if((m_benable)&&(m_bstartHeart))//���������ƽ̨��������������
		{
			if( (GetCurrTime32() == m_heartTime+m_recvTimeLeap*60) || (0 == m_heartTime))//����������ʱ��
			{
				HeartOnlyFunction(m_recvTimeLeap);//��ʱ��ͷ�����
			}
			else
			{
				//����Ĵ�����Ϊ���ݱȽ�С�����Ϳ죬�����ŵ���ǰ
				while(0 != m_listCmd.size())
				{
					CmdOnlyFunction(CMD_OPER);
					PUB_Sleep(100);
					if(SWL_INVALID_SOCKET != m_heartsock)
					{
						SWL_CloseSocket(m_heartsock);
						m_heartsock = SWL_INVALID_SOCKET;
					}
				}

				//�ⲿ�����¼�������������),�Ƚ���Ҫ��������λ
				CMDSensorAlarm();
				//�ƶ�����¼�
				if(m_bmotionEnable)//����������ƶ�����ϴ�
				{
					CMDMotionAlarm();	
				}
			}

			PUB_Sleep(1000);

			if(m_heartTime != 0)
			{
				if(GetCurrTime32() > m_heartTime+45*60)//����������45���ӣ�
				{
					printf("timecount>45 %s,%d\n",__FILE__,__LINE__);
					if(SWL_INVALID_SOCKET != m_heartsock)
					{
						SWL_CloseSocket(m_heartsock);
						m_heartsock = SWL_INVALID_SOCKET;
					}

					PUB_Sleep(10000);//�ر��׽ӿ�10����ټ�������
					while (1)
					{
						if(!Connectheart())
						{
							PUB_Sleep(10000);			
							continue;
						}
						m_heartTime = 0;//����ʧ�����·�������
						break;
					}
				}
			}
		}
	}
}

TiXmlElement * CShanghaiConnectMan::CreateAgent(TiXmlDocument *document,char *pdvrType)
{
	//����һ����Ԫ�ز�����
	TiXmlElement *pRootElement = new TiXmlElement("Agent");
	document->LinkEndChild(pRootElement);
	pRootElement->SetAttribute("ID",m_agentId);
	pRootElement->SetAttribute("Type","TVTDVR");
	pRootElement->SetAttribute("Ver","1.2.0.0");
	return pRootElement;
}

void CShanghaiConnectMan::CreateShanghaiHeartXml(TiXmlElement *pElement,char *pdvrType,HEARTINFO hear_info)
{
	DVR_DISK_INFO_LIST dvrDiskInfoList;
	DVR_DISK_INFO diskInfo;
	POS pos;
	memset(&diskInfo,0,sizeof(DVR_DISK_INFO));
	memset(&pos,0,sizeof(POS));
	
	TiXmlElement *pDVRHeart = new TiXmlElement("DVRHeart");
	pElement->LinkEndChild(pDVRHeart);
	pDVRHeart->SetAttribute("State","0");
	//��ȡ���̵���������ʣ������
	hear_info.TotalSpace = 0;
	hear_info.FreeSpace = 0;
	CDiskManager::Instance()->GetDeviceDiskInfo(NET_DISK_TYPE_FIXEDDISK,NULL,dvrDiskInfoList);
	//����¼��Ӳ�̵�ʣ��ռ�
	{
		POS pos = dvrDiskInfoList.GetHeadPosition();
		while (NULL != pos)
		{
			diskInfo = dvrDiskInfoList.GetAt(pos);
			if (NET_DISK_NO_FORMAT == diskInfo.diskStatus)
			{
				diskInfo.freeSpace = diskInfo.totalSpace;
			}
			else if (NET_DISK_NORMAL == diskInfo.diskStatus)
			{
				if(NULL != m_pReclogMan)
				{
					diskInfo.freeSpace = m_pReclogMan->GetDiskFreeSpace(diskInfo.diskpara, diskInfo.partitionNum);
				}
				else
				{
					assert(false);
					diskInfo.freeSpace = 0;
				}
			}
			hear_info.FreeSpace += diskInfo.freeSpace;
			hear_info.TotalSpace += diskInfo.totalSpace;
			dvrDiskInfoList.GetNext(pos);
		}
	}

	
	hear_info.TotalSpace = hear_info.TotalSpace*(ULONGLONG)1024*1024;
	hear_info.FreeSpace = hear_info.FreeSpace*(ULONGLONG)1024*1024;
	char totalSpace[256]={0};
	char freeSpace[256]={0};
	snprintf(totalSpace,256,"%lld",hear_info.TotalSpace);
	snprintf(freeSpace,256,"%lld",hear_info.FreeSpace);
	
	pDVRHeart->SetAttribute("TotalSpace",totalSpace);
	pDVRHeart->SetAttribute("FreeSpace",freeSpace);

	TiXmlText *pText = new TiXmlText(pdvrType);
	pDVRHeart->LinkEndChild(pText);

	TiXmlElement *pNext = new TiXmlElement("GetTicks");
	pElement->LinkEndChild(pNext);
}

TiXmlElement * CShanghaiConnectMan::AddOperXml(TiXmlElement *pElement,OPER_INFO operInfo)
{
	char szuuid[128]={0};
	TiXmlElement *pNext = new TiXmlElement("OperationCmd");
	pElement->LinkEndChild(pNext);
	pNext->SetAttribute("Type",operInfo.operType);
	pNext->SetAttribute("Channel",operInfo.chnn);
	pNext->SetAttribute("TriggerTime",operInfo.triggerTime);

	char *pNote = new char[operInfo.textLen+1];
	memset(pNote,0,operInfo.textLen+1);
	memcpy(pNote,operInfo.pNote,operInfo.textLen);
	pNext->SetAttribute("Note",pNote);
	
	uuid_unparse(operInfo.uniqueGUID,szuuid);//���Ψһ��׼��
	pNext->SetAttribute("GUID",szuuid);

	char *pPic = new char[operInfo.picLen+1];
	memset(pPic,0,operInfo.picLen+1);
	memcpy(pPic,operInfo.pPic,operInfo.picLen);
	TiXmlText *pText = new TiXmlText(pPic);
	pNext->LinkEndChild(pText);
	delete [] pNote;
	delete [] pPic;
	return pNext;

}
bool CShanghaiConnectMan::GetHeartRecvTimeAndLeap(char *pSrc,int &TimeValue)
{
	char *pTime = strstr(pSrc,"OK NowTime=");
	char *pEnd = NULL;
	char *pLead = NULL;
	if(NULL != pTime)
	{
		pEnd = strstr(pTime + strlen("OK NowTime="),"/></Server>");
		if(NULL != pEnd)
		{
			//��ȡ��������ʱ��
			strncpy(m_recvTime,pTime + strlen("OK NowTime=")+1,pEnd -2 -(pTime + strlen("OK NowTime=")));
		}
		else
		{
			return false;
		}

		//��ȡʱ����
		pLead = strstr(pSrc,"<Ticks Value=");
		pEnd = strstr(pLead+strlen("<Ticks Value="),"><OK NowTime=");

		if(NULL != pEnd)
		{
			char leapstr[256] = {0};
			strncpy(leapstr,pLead+strlen("<Ticks Value=")+1,pEnd- 2 -(pLead+strlen("<Ticks Value=")));
			TimeValue = atoi(leapstr);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CShanghaiConnectMan::Connectheart()
{
	m_heartsock = SWL_INVALID_SOCKET;
	m_heartsock = SWL_CreateSocket(AF_INET, SOCK_STREAM, 0);
	if (SWL_INVALID_SOCKET == m_heartsock )
	{
		perror("socket()");
		printf("%s:%s:%d, create socket error\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(struct sockaddr_in));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(m_port);
	sockAddr.sin_addr.s_addr = inet_addr(m_ip);
	
	if ( 0 != SWL_Connect(m_heartsock, (struct sockaddr *)&sockAddr ,sizeof(struct sockaddr_in), 33000))
	{
		printf("%s:%s:%d, connect() to IP:%s, port=%d error\n", __FUNCTION__, __FILE__, __LINE__,m_ip, m_port);
		SWL_CloseSocket(m_heartsock);
		m_heartsock = SWL_INVALID_SOCKET;
		return false;
	}
	return true;
}

LONGLONG CShanghaiConnectMan::GetTriggerTime()
{
	return m_triggerTime;
}

LONGLONG CShanghaiConnectMan::GetChannelBits()
{
	return m_chnnBits;
}

void CShanghaiConnectMan::ProcessGetImageXml(char *xmlbuf)
{
	char *pGetImg = strstr(xmlbuf, "GetImage");
	if (NULL != pGetImg)
	{
		printf("@@@@@GetImage %s[%d]\n", __FILE__, __LINE__);
		m_triggerTime = GetCurrTime();
		char *pChnn = strstr(xmlbuf, "<GetImage Channel=");
		char *pEnd = strstr(pChnn+strlen("<GetImage Channel="), " GUID=");
		if(NULL != pEnd)
		{
			char chnnStr[256] = {0};
			strncpy(chnnStr, pChnn+strlen("<GetImage Channel=")+1, pEnd-2-(pChnn+strlen("<GetImage Channel=")));
			std::string channel = chnnStr;	
			while (true)
			{
				int index = channel.find(',');
				if (-1 != index)
				{
					m_chnnBits |= (LONGLONG)0x01<<atoi((channel.substr(0, index)).c_str())-1;
					channel = (const char *)(channel.c_str()+index+1);
				}
				else
				{
					m_chnnBits |= (LONGLONG)0x01<<atoi(channel.c_str())-1;
					break;
				}
			}
		}
	}
}

bool CShanghaiConnectMan::SendAndRecvHeart(char *pXmlbuf,int &recvTimeLeap,bool bheart)
{
	bool bret=false;
	char buf[1024]={0};
	fd_set readSet;
	fd_set writeSet;
	timeval timeOut;
	int retval = 0;
	int count = 0;
	int iLen =0;
	int sendLen =0;

	MSG_HEAD msghead;
	memset(&msghead,0,sizeof(MSG_HEAD));
	memcpy(msghead.heart,"QWCMD:",6);
	msghead.headlen = sizeof(MSG_HEAD)+strlen(pXmlbuf);
	//printf("msghead.headlen=%d\n",msghead.headlen);

	while(iLen < msghead.headlen)
	{
		FD_ZERO(&writeSet);
		FD_SET(m_heartsock, &writeSet);

		retval = select(m_heartsock + 1, NULL, &writeSet, NULL, NULL);
		if(0 <retval)
		{
			memset(buf,0,sizeof(buf));
			if(0 == iLen)//��һ����Ҫ��ͷ
			{
				memcpy(buf,&msghead,sizeof(MSG_HEAD));
				if(msghead.headlen <= 1024)
				{
					memcpy(buf+sizeof(MSG_HEAD),pXmlbuf,msghead.headlen-sizeof(MSG_HEAD));
					sendLen = SWL_Send(m_heartsock,buf,msghead.headlen,0);
					iLen += msghead.headlen;
				}
				else
				{
					memcpy(buf+sizeof(MSG_HEAD),pXmlbuf,sizeof(buf)-sizeof(MSG_HEAD));
					sendLen = SWL_Send(m_heartsock,buf,1024,0);
					iLen += 1024;
				}
				
			}
			else if(iLen + sizeof(buf) <= msghead.headlen)
			{
				memcpy(buf,pXmlbuf+iLen-sizeof(MSG_HEAD),sizeof(buf));
				sendLen = SWL_Send(m_heartsock,buf,1024,0);
				iLen += 1024;
			}
			else
			{
				memcpy(buf,pXmlbuf+iLen-sizeof(MSG_HEAD),msghead.headlen-iLen);
				sendLen = SWL_Send(m_heartsock,buf,msghead.headlen-iLen,0);
				iLen += msghead.headlen-iLen;
			}

			if(sendLen <= 0)
			{
				perror("Send ERR");
				printf("\n");
				return false;
			}
			else
			{
				//printf("sendlen = %d,len=%d\n",sendLen,iLen);
			}
			
		}
		else if ( 0 > retval)
		{
			if (EINTR  != errno)
			{
				perror("select()");
				printf("%s:%s:%d, select()ʧ��, errno=%d\n", __FUNCTION__, __FILE__, __LINE__, errno);
				assert(false);
				return -1;
			}
			else
			{
				continue;
			}
		}
	}

	while(1)
	{
		timeOut.tv_sec = 20;
		timeOut.tv_usec = 0;
		FD_ZERO(&readSet);
		FD_SET(m_heartsock, &readSet);

		retval = select(m_heartsock + 1, &readSet, NULL, NULL, &timeOut);
		if(0 < retval)
		{
			if(FD_ISSET(m_heartsock,&readSet)) 
			{
				memset(buf,0,sizeof(buf));
				iLen = SWL_Recv(m_heartsock,buf,recvSendBufLen,0);
				if(iLen <= 0)
				{
					if (!SWL_EWOULDBLOCK())
					{
						perror("recv err");
						printf("\n");
						bret= false;
						break;
					}
				}
				else
				{
					//��ȡ��������ͷ�ĳ���
					char xmlbuf[256] ={0};
					memset(&msghead,0,sizeof(MSG_HEAD));
					memcpy(&msghead,buf,sizeof(MSG_HEAD));
					//����ͷ�ĳ��Ƚ���ʣ��Ĳ���
					memcpy(xmlbuf,buf+sizeof(MSG_HEAD),msghead.headlen-sizeof(MSG_HEAD));
					//printf("xmlbuf=%s\n",xmlbuf);
					//�����ӡû��Ļ������Ҳ�������ǰ��ϵͳʱ����´η��������ļ��������ʱ��ͬ��
					if(bheart)
					{
						GetHeartRecvTimeAndLeap(xmlbuf,recvTimeLeap);
					}

					ProcessGetImageXml(xmlbuf);
	
					bret = true;
					break;
				}
			}

		}
		if ( -1 == retval)
		{
			if (EINTR  != errno)
			{
				perror("select()");
				printf("%s:%s:%d, errno=%d, fd=%d\n", __FUNCTION__ ,__FILE__, __LINE__ ,errno, m_heartsock);
				bret =false;
				break;
			}
			else
			{
				continue;
			}
		}

		if (0 == retval)//����20���ˣ���Ϊ�������
		{
			printf("Wait too long. %s[%d]\n", __FILE__, __LINE__);
			bret =false;
			break;
		}
	}
	return bret;
}

bool CShanghaiConnectMan::GetDYRSysErrExitLog()
{
	bool bsysErr = false;
	bool bfind = false;
	unsigned long num = 0;

	NET_LOG_SEARCH searchCond;
	unsigned long condition =0;
	unsigned long curTime = GetCurrTime32();
	memset(&searchCond,0,sizeof(NET_LOG_SEARCH));
	searchCond.startTime = curTime - 24*60*60;
	searchCond.endTime = curTime;
	condition |= LOG_TYPE_SYSTEM_CTRL;
	searchCond.content = condition >> 16;
	//������־����������Ŀ
	OPERATOR_LOG_LIST list;	
	num = CLogMan::Instance()->GetOperatorLog(searchCond.startTime,searchCond.endTime,searchCond.content,list);
	if (num > 0)
	{
		unsigned long logType =0;
		unsigned long preType =0;		
		OPERATOR_LOG *pLog =  new OPERATOR_LOG[num];
		unsigned long i = 0;
		POS pos = list.GetTailPosition();
		while(NULL != pos)
		{
			OPERATOR_LOG & node = list.GetPrev(pos);
			pLog[i++] = node;
		}

		for(int i=0;i<num;i++)
		{
			logType = (pLog[i].contentID << 16) | pLog[i].type;
			if(LOG_TYPE_BOOT == logType )//�ҵ�һ����������ǰһ���ǲ�������
			{
				for(int index = i+1;index<num;index++)
				{
					preType = (pLog[index].contentID << 16) | pLog[index].type;
					if(LOG_TYPE_BOOT == preType )//������˵�����쳣�ػ�
					{
						bsysErr = true;
						bfind = true;
						break;
					}
					else if(LOG_TYPE_SHUTDOWN  == preType)//�ǹرվ��������ػ�
					{
						bsysErr = false;
						bfind = true;
						break;

					}
				}
			}
			if(bfind)
			{
				break;
			}
		}
		delete []pLog;
		pLog = NULL;
		
	}

	return bsysErr;
}

void CShanghaiConnectMan::gettmreciveTime(tm &serverTm)
{
	char tmpbuf[256]={0};
	char *pyear = NULL;
	char *pMon=NULL;
	char *pDay = NULL;
	char *pHour = NULL;
	char *pmin = NULL;
	char *psec = NULL;

	pyear =strstr(m_recvTime,"-");
	if(NULL != pyear)
	{
		strncpy(tmpbuf,m_recvTime,pyear-m_recvTime);
		serverTm.tm_year =atoi(tmpbuf)-1900;
	}

	pMon=strstr(pyear+1,"-");
	if(NULL != pMon)
	{
		memset(tmpbuf,0,sizeof(tmpbuf));
		strncpy(tmpbuf,pyear+1,pMon-pyear-1);
		serverTm.tm_mon =atoi(tmpbuf)-1;
	}

	pDay = strstr(pMon+1," ");
	if(NULL != pDay)
	{
		memset(tmpbuf,0,sizeof(tmpbuf));
		strncpy(tmpbuf,pMon+1,pDay-pMon-1);
		serverTm.tm_mday =atoi(tmpbuf);
	}

	pHour = strstr(pDay+1,":");
	if(NULL != pDay)
	{
		memset(tmpbuf,0,sizeof(tmpbuf));
		strncpy(tmpbuf,pDay+1,pHour-pDay-1);
		serverTm.tm_hour =atoi(tmpbuf);
	}

	pmin = strstr(pHour+1,":");
	if(NULL != pHour)
	{
		memset(tmpbuf,0,sizeof(tmpbuf));
		strncpy(tmpbuf,pHour+1,pmin-pHour-1);
		serverTm.tm_min =atoi(tmpbuf);
	}

	memset(tmpbuf,0,sizeof(tmpbuf));
	strcpy(tmpbuf,pmin+1);
	serverTm.tm_sec = atoi(tmpbuf);
}

void CShanghaiConnectMan::ChangeSystime()
{
	tm serverTm;
	unsigned long serverTime = 0;
	unsigned long currtime = 0;
	gettmreciveTime(serverTm);
	m_heartTime = TmToDVRTime32(serverTm);
	currtime = GetCurrTime32();
	if (((serverTime > currtime) && (serverTime - currtime) > 20) || ((serverTime < currtime) && (currtime - serverTime) > 20))
	{
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_L_CHANGE_TIME, (unsigned char *)&m_heartTime, sizeof(unsigned long));
	}
}

void CShanghaiConnectMan::GetOperCmd(OPER_INFO Opercmd)
{
	OPER_INFO tmpInfo;
	memset(&tmpInfo,0,sizeof(OPER_INFO));
	tmpInfo.operType = Opercmd.operType;
	tmpInfo.chnn = Opercmd.chnn;
	strcpy(tmpInfo.triggerTime,Opercmd.triggerTime);
	tmpInfo.textLen = Opercmd.textLen;
	if(0 != tmpInfo.textLen)
	{
		tmpInfo.pNote = new unsigned char[tmpInfo.textLen];
		memcpy(tmpInfo.pNote,Opercmd.pNote,tmpInfo.textLen);
	}
	else
	{
		tmpInfo.pNote =NULL;
		tmpInfo.textLen = 0;
	}

	tmpInfo.picLen = Opercmd.picLen;
	if(0 != tmpInfo.picLen)
	{
		tmpInfo.pPic = new unsigned char[tmpInfo.picLen];
		memset(tmpInfo.pPic,0,tmpInfo.picLen);
		memcpy(tmpInfo.pPic,Opercmd.pPic,tmpInfo.picLen);
	}
	else
	{
		tmpInfo.pPic = NULL;
		tmpInfo.picLen = 0;
	}

	uuid_copy(tmpInfo.uniqueGUID,Opercmd.uniqueGUID);
	m_listCmd.push_back(tmpInfo);
}

int CShanghaiConnectMan::HeartOnlyFunction(int &recvTimeLeap)
{
	char dvrType[256] = {0};
	char tmpTime[256] = {0};
	char buf[recvSendBufLen] = {0};
	ULONGLONG curTime = 0;
	int pBufLen =0;
	tm curTm;
	TiXmlPrinter  Printer;
	Printer.SetIndent(NULL);
	TiXmlDocument *pMyDocument = NULL;
	OPER_INFO ExitlogInfo;
	memset(&ExitlogInfo,0,sizeof(OPER_INFO));
	OPER_INFO startLogInfo;
	memset(&startLogInfo,0,sizeof(OPER_INFO));

	//��������,��װ
	pMyDocument = new TiXmlDocument();
	pBufLen = 0;
	TiXmlElement *pRootElement = CreateAgent(pMyDocument,dvrType);
	CreateShanghaiHeartXml(pRootElement,dvrType,m_hear_info);
	if(0 == m_DVrStart)
	{
		char errtext[256]={0};
		if(GetDYRSysErrExitLog())//�����ϴ��ǲ����쳣�ػ����ǵĻ������쳣�ػ���Ϣ
		{
			strcpy(errtext,"GB2312:�ϴ�ϵͳ�쳣�ػ�");							
			ExitlogInfo.operType = DVRSYSERREXIT;
			ExitlogInfo.chnn = 0;

			curTime = GetCurrTime();
			PrintfBit64(curTime);
			memset(&curTm,0,sizeof(tm));
			curTm = DVRTimeToTm(curTime);
			memset(tmpTime,0,sizeof(tmpTime));
			snprintf(tmpTime,256,"%d-%d-%d %d:%d:%d",curTm.tm_year+1900,curTm.tm_mon+1,curTm.tm_mday,curTm.tm_hour,curTm.tm_min,curTm.tm_sec);
			strcpy(ExitlogInfo.triggerTime,tmpTime);

			ExitlogInfo.pPic = NULL;
			ExitlogInfo.picLen = 0;
			uuid_generate_time(ExitlogInfo.uniqueGUID);
		
			unsigned char *pBase64Buf = new unsigned char[maxShanghaiDataSize];
			memset(pBase64Buf,0,maxShanghaiDataSize);
			//ת������
			int base64Len = to64frombits(pBase64Buf,(unsigned char*)errtext,strlen(errtext));
			ExitlogInfo.pNote = NULL;
			ExitlogInfo.pNote = new unsigned char[base64Len];
			ExitlogInfo.textLen = base64Len;

			memset(ExitlogInfo.pNote,0,base64Len);
			memcpy(ExitlogInfo.pNote,pBase64Buf,base64Len);
			delete []pBase64Buf;
			pBase64Buf = NULL;
			
			AddOperXml(pRootElement,ExitlogInfo);
			if(NULL != ExitlogInfo.pNote)
			{
				delete []ExitlogInfo.pNote;
				ExitlogInfo.pNote = NULL;
			}
		}
		//�տ������������������Ͽ�������־����
		memset(errtext,0,sizeof(errtext));
		strcpy(errtext,"GB2312:ϵͳ����");
		startLogInfo.operType = DVRSYSSTART;
		startLogInfo.chnn = 0;

		curTime = GetCurrTime();
		memset(&curTm,0,sizeof(tm));
		curTm = DVRTimeToTm(curTime);
		memset(tmpTime,0,sizeof(tmpTime));
		snprintf(tmpTime,256,"%d-%d-%d %d:%d:%d",curTm.tm_year+1900,curTm.tm_mon+1,curTm.tm_mday,curTm.tm_hour,curTm.tm_min,curTm.tm_sec);
		strcpy(startLogInfo.triggerTime,tmpTime);

		unsigned char *pBase64Buf = new unsigned char[maxShanghaiDataSize];
		memset(pBase64Buf,0,maxShanghaiDataSize);
		//ת������
		int base64Len = to64frombits(pBase64Buf,(unsigned char*)errtext,strlen(errtext));
		delete [] startLogInfo.pNote;
		startLogInfo.pNote = new unsigned char[base64Len];
		startLogInfo.textLen = base64Len;
		memset(startLogInfo.pNote,0,base64Len);
		memcpy(startLogInfo.pNote,pBase64Buf,base64Len);
		delete []pBase64Buf;
		pBase64Buf = NULL;

		startLogInfo.pPic = NULL;
		startLogInfo.picLen = 0;
		uuid_generate_time(startLogInfo.uniqueGUID);
		
		AddOperXml(pRootElement,startLogInfo);
		if(NULL != startLogInfo.pNote)
		{
			delete []startLogInfo.pNote;
			startLogInfo.pNote = NULL;
		}
		
	}
	//��װ��ϣ��������������Ƿ��д�
	
	pMyDocument->Accept(&Printer);
	pBufLen = Printer.Size();
	pBufLen += strlen(pShanghaiXmltitle);//ͷ+xml��ĳ���
	char *pXmlBuf = new char[pBufLen+1];
	memset(pXmlBuf,0,pBufLen+1);
	//�˴���Ҫ���xml��ͷ
	strcpy(pXmlBuf,pShanghaiXmltitle);
	memcpy(pXmlBuf+strlen(pShanghaiXmltitle),Printer.CStr(),Printer.Size());
	//ÿ�η�����Ϣ����Ҫȥ���Ӻͽ���
	while (m_bheartProc)
	{
		if(!Connectheart())
		{
			PUB_Sleep(1000);			
			continue;
		}
		break;
	}
	
	//���Ͳ�����
	bool bsucc =SendAndRecvHeart(pXmlBuf,recvTimeLeap,true);
	if(pXmlBuf != NULL)
	{
		delete []pXmlBuf;
		pXmlBuf = NULL;
	}
	if(bsucc)
	{
		m_DVrStart = 1;
		ChangeSystime();
	}
	
	if(NULL != pMyDocument)
	{
		delete pMyDocument;
		pMyDocument = NULL;
	}
	//������Ϻ󣬹ر��׽ӿ�
	if(SWL_INVALID_SOCKET != m_heartsock)
	{
		SWL_CloseSocket(m_heartsock);
		m_heartsock = SWL_INVALID_SOCKET;
	}
	m_heartTime = GetCurrTime32();
	return ERR_SUCC;
}

int CShanghaiConnectMan::CmdOnlyFunction(int type)
{
	if(!m_benable)
	{
		return ERR_PTENABLE;
	}
	
	char dvrType[256] = {0};
	char tmpTime[256] = {0};
	char buf[recvSendBufLen] = {0};
	ULONGLONG curTime = 0;
	int pBufLen =0;
	tm curTm;
	TiXmlPrinter  Printer;
	Printer.SetIndent(NULL);
	TiXmlDocument *pMyDocument = NULL;

	//û����������ʱ�䣬���͵�ȫ��ָ��
	m_PTLock.Lock();
	pMyDocument = new TiXmlDocument();
	TiXmlElement *pRootElement = CreateAgent(pMyDocument,dvrType);

	OPER_INFO tmpInfo;
	memset(&tmpInfo,0,sizeof(OPER_INFO));
	switch(type)
	{
	case CMD_OPER:
		{
			if(0 != m_listCmd.size())
			{
				std::list<OPER_INFO>::iterator it = m_listCmd.begin();
				tmpInfo = *it;
				if(0 != tmpInfo.operType)//operType������Ϊ0��Ϊ0˵������Ŀ�ǿյ�
				{				
					AddOperXml(pRootElement,tmpInfo);//ÿ�ζ�ֻ����һ��
				}
			}
			else
			{
				return ERR_PTENABLE;
			}
		}
		break;
	case CMD_MOTION:
		{			
			if(0 != m_listMotionPic.size())
			{
				std::list<OPER_INFO>::iterator it = m_listMotionPic.begin();
				tmpInfo = *it;		
				if(0 != tmpInfo.operType)//operType������Ϊ0��Ϊ0˵������Ŀ�ǿյ�
				{
					AddOperXml(pRootElement,tmpInfo);//ÿ�ζ�ֻ����һ��
				}
			}
			else
			{
				return ERR_PTENABLE;
			}
		}
		break;
	case CMD_SENSOR:
		{
			if(0 != m_listsensorPic.size())
			{
				std::list<OPER_INFO>::iterator it = m_listsensorPic.begin();
				tmpInfo = *it;	
				if(0 != tmpInfo.operType)//operType������Ϊ0��Ϊ0˵������Ŀ�ǿյ�
				{
					AddOperXml(pRootElement,tmpInfo);//ÿ�ζ�ֻ����һ��
				}
			}
			else
			{
				return ERR_PTENABLE;
			}
		}
		break;
	default:
		assert(false);
		return ERR_OPERTYPE;
		break;
	}

	pMyDocument->Accept(&Printer);
	pBufLen = Printer.Size();
	pBufLen += strlen(pShanghaiXmltitle);//ͷ+xml��ĳ���

	char *pOPerXmlBuf = new char[pBufLen+1];
	memset(pOPerXmlBuf,0,pBufLen+1);
	strcpy(pOPerXmlBuf,pShanghaiXmltitle);
	memcpy(pOPerXmlBuf+strlen(pShanghaiXmltitle),Printer.CStr(),Printer.Size());
	//printf("pOPerXmlBuf=%s,len = %d\n",pOPerXmlBuf,strlen(pOPerXmlBuf));
	if(NULL != pMyDocument)
	{
		delete pMyDocument;
		pMyDocument = NULL;
	}
	//��ӡ

	while (m_bheartProc)
	{
		if(!Connectheart())
		{
			PUB_Sleep(1000);			
			continue;
		}
		break;
	}
	
	int tmp=0;//��Ϊû�н��գ�����ʱ����û��
	bool bsucc = SendAndRecvHeart(pOPerXmlBuf,tmp,false);
	if(pOPerXmlBuf != NULL)
	{
		delete []pOPerXmlBuf;
		pOPerXmlBuf = NULL;
	}
	//������Ϻ󣬹ر��׽ӿ�
	if(SWL_INVALID_SOCKET != m_heartsock)
	{
		SWL_CloseSocket(m_heartsock);
		m_heartsock = SWL_INVALID_SOCKET;
	}
	if(bsucc)
	{
		switch(type)
		{
		case CMD_OPER:
			{
				if(0 != m_listCmd.size())
				{
					std::list<OPER_INFO>::iterator it = m_listCmd.begin();
					if(NULL !=it->pNote)
					{
						delete []it->pNote;
						it->pNote = NULL;
					}
					if(NULL !=it->pPic)
					{
						delete []it->pPic;
						it->pPic = NULL;
					}
					m_listCmd.pop_front();
				}
				break;
			}
		case CMD_MOTION:
			{
				if(0 != m_listMotionPic.size())
				{
					std::list<OPER_INFO>::iterator it = m_listMotionPic.begin();
					if(NULL !=it->pNote)
					{
						delete []it->pNote;
						it->pNote = NULL;
					}
					if(NULL !=it->pPic)
					{
						delete []it->pPic;
						it->pPic = NULL;
					}
					m_listMotionPic.pop_front();
				}
				break;
			}
		case CMD_SENSOR:
			{
				if(0 != m_listsensorPic.size())
				{
					std::list<OPER_INFO>::iterator it = m_listsensorPic.begin();
					if(NULL !=it->pNote)
					{
						delete []it->pNote;
						it->pNote = NULL;
					}
					if(NULL !=it->pPic)
					{
						delete []it->pPic;
						it->pPic = NULL;
					}
					m_listsensorPic.pop_front();
				}
				break;
			}
		default:
			bsucc = false;
			break;
		}		
	}
	else
	{
		//���ɹ������������б�����һ�μ�������,����ǲ���ͼƬ�Ͷ�����ͼƬ�Ƚϴ��ճ�ͼƬ������ָ���´�ѭ���ٷ���
		//����ͼƬ�Ƚ���Ҫ���������б����´η��ͣ�
		if(type == CMD_OPER)
		{
			if(0 != m_listCmd.size())
			{
				std::list<OPER_INFO>::iterator itdelay = m_listCmd.begin();
				if( (KEEPUPLOAD_IMAGE == itdelay->operType)||(TESTUPLOAD_IMAGE == itdelay->operType) )
				{
					if(NULL != itdelay->pNote)
					{
						delete []itdelay->pNote;//�ڴ���Ҫ�ͷ�
						itdelay->pNote = NULL;
					}
					if(NULL != itdelay->pPic)
					{
						delete []itdelay->pPic;
						itdelay->pPic = NULL;
					}
					m_listCmd.pop_front();
				}
			}
		}
	
		if(maxCmdCount == m_listCmd.size())
		{
			//������100�������������100����û���꣬˵�������д����������ָ��
			for(std::list<OPER_INFO>::iterator it = m_listCmd.begin();it!=m_listCmd.end();++it)
			{
				if(NULL != it->pNote)
				{
					delete []it->pNote;//�ڴ���Ҫ�ͷ�
					it->pNote = NULL;
				}
				if(NULL != it->pPic)
				{
					delete []it->pPic;
					it->pPic = NULL;
				}
			}
			m_listCmd.clear();
		}
		if(maxCmdCount == m_listMotionPic.size())
		{
			//������100�������������100����û���꣬˵�������д����������ָ��
			for(std::list<OPER_INFO>::iterator it = m_listMotionPic.begin();it!=m_listMotionPic.end();++it)
			{
				if(NULL != it->pNote)
				{
					delete []it->pNote;//�ڴ���Ҫ�ͷ�
					it->pNote = NULL;
				}
				if(NULL != it->pPic)
				{
					delete []it->pPic;
					it->pPic = NULL;
				}
			}
			m_listMotionPic.clear();
		}
		if(maxCmdCount == m_listsensorPic.size())
		{
			//������100�������������100����û���꣬˵�������д����������ָ��
			for(std::list<OPER_INFO>::iterator it = m_listsensorPic.begin();it!=m_listsensorPic.end();++it)
			{
				if(NULL != it->pNote)
				{
					delete []it->pNote;//�ڴ���Ҫ�ͷ�
					it->pNote = NULL;
				}
				if(NULL != it->pPic)
				{
					delete []it->pPic;
					it->pPic = NULL;
				}
			}
			m_listsensorPic.clear();
		}
		
	}
	m_PTLock.UnLock();
	if(bsucc)
	{
		return ERR_SUCC;
	}
	else
	{
		return ERR_NETERR;
	}
}

void CShanghaiConnectMan::CMDMotionAlarm()
{
	if(!m_benable)//ƽ̨������
	{
		return;
	}
	
	//�ƶ�����¼�
	while(m_bmotionHead)
	{
		if(0 != m_listMotionPic.size())
		{
			int succ = CmdOnlyFunction(CMD_MOTION);
			if(ERR_SUCC !=succ)
			{
				if(ERR_PTENABLE != succ)//����ƽ̨�ر�
				{
					//if(SWL_INVALID_SOCKET != m_heartsock)
					//{
					//	SWL_CloseSocket(m_heartsock);
					//	m_heartsock = SWL_INVALID_SOCKET;
					//}
					////�����жϣ����²�һ��ͷ
					//char noteText[256]="GB2312:�ƶ����ͼ��";
					//OPER_INFO info;
					//memset(&info,0,sizeof(OPER_INFO));
					//info.operType =ALARMUPLOAD_IMAGE;
					//info.chnn = 0;

					//tm curTm= DVRTimeToTm(GetCurrTime());
					//char tmpTime[256]={0};
					//memset(tmpTime,0,sizeof(tmpTime));
					//snprintf(tmpTime,256,"%d-%d-%d %d:%d:%d",curTm.tm_year+1900,curTm.tm_mon+1,curTm.tm_mday,curTm.tm_hour,curTm.tm_min,curTm.tm_sec);
					//strcpy(info.triggerTime,tmpTime);
					////�����ֽ���ת��
					//{
					//	int len=0;
					//	char tmpbuf[maxShanghaiDataSize]={0};
					//	len = to64frombits((unsigned char *)tmpbuf,(unsigned char *)noteText,strlen(noteText));
					//	if(0 != len)
					//	{
					//		info.pNote = new unsigned char[len];
					//		memcpy(info.pNote,tmpbuf,len);
					//		info.textLen = len;
					//	}
					//	else
					//	{
					//		assert(false);
					//		info.textLen =0;
					//		info.pNote = NULL;
					//	}

					//}

					//info.picLen =0;
					//info.pPic = NULL;
					//std::list<OPER_INFO>::iterator it = m_listMotionPic.begin();
					//uuid_copy(info.uniqueGUID,it->uniqueGUID);
					//m_listMotionPic.push_front(info);
				}
				else//��ƽ̨�ر������
				{
					if(SWL_INVALID_SOCKET != m_heartsock)
					{
						SWL_CloseSocket(m_heartsock);
						m_heartsock = SWL_INVALID_SOCKET;
					}
					m_bmotionHead = false;
					m_motionpicCount = 0;
					break;
				}
			}	
			else
			{
				++m_motionpicCount;
			}
			PUB_Sleep(1500);
		}
		if(maxPicCount < m_motionpicCount)//ȫ���������
		{
			if(SWL_INVALID_SOCKET != m_heartsock)//ƽ̨�ر� 
			{
				SWL_CloseSocket(m_heartsock);
				m_heartsock = SWL_INVALID_SOCKET;
			}
			m_bmotionHead = false;
			m_motionpicCount = 0;
			//�ж���Ķ������
			for(std::list<OPER_INFO>::iterator it = m_listMotionPic.begin();it!=m_listMotionPic.end();++it)
			{
				if(NULL != it->pNote)
				{
					delete []it->pNote;//�ڴ���Ҫ�ͷ�
					it->pNote = NULL;
				}
				if(NULL != it->pPic)
				{
					delete []it->pPic;
					it->pPic = NULL;
				}
			}
			m_listMotionPic.clear();
			break;
		}
		else
		{
			//if((0 == m_listsensorPic.size()) &&(0 !=m_sensorAlarmStartTime))//�б���û��ͼ���������ͼ����40�ţ�����ʱ�䳬��һ����Χ��Ҳ��ֹ����
			if(0 == m_listMotionPic.size())
			{
				if(m_bmotionHead)
				{
					unsigned long time = GetCurrTime32();
					if(time - m_motionAlarmStartTime >= m_maxpicdelayTime)
					{
						if(SWL_INVALID_SOCKET != m_heartsock)//ƽ̨�ر� 
						{
							SWL_CloseSocket(m_heartsock);
							m_heartsock = SWL_INVALID_SOCKET;
						}
						m_bmotionHead = false;
						m_motionpicCount = 0;
						break;
					}
				}
			}

		}
		
	}
}

void CShanghaiConnectMan::CMDSensorAlarm()
{
	if(!m_benable)//ƽ̨������
	{
		return;
	}
	while(m_bsensorHead)
	{
		if(0 != m_listsensorPic.size())
		{
			int succ = CmdOnlyFunction(CMD_SENSOR);
			if(ERR_SUCC !=succ)
			{
				if(ERR_PTENABLE != succ)//����ƽ̨�ر�
				{
					//printf("head is lost ***************\n");
					//SWL_CloseSocket(m_heartsock);
					//m_heartsock = SWL_INVALID_SOCKET;
					////�����жϣ����²�һ��ͷ
					//char noteText[256]="GB2312:�ⲿ����ͼ��";
					//OPER_INFO info;
					//memset(&info,0,sizeof(OPER_INFO));
					//info.operType =ALARMUPLOAD_IMAGE;
					//info.chnn = 0;

					//tm curTm= DVRTimeToTm(GetCurrTime());
					//char tmpTime[256]={0};
					//memset(tmpTime,0,sizeof(tmpTime));
					//snprintf(tmpTime,256,"%d-%d-%d %d:%d:%d",curTm.tm_year+1900,curTm.tm_mon+1,curTm.tm_mday,curTm.tm_hour,curTm.tm_min,curTm.tm_sec);
					//strcpy(info.triggerTime,tmpTime);
					////�����ֽ���ת��
					//{
					//	int len=0;
					//	char tmpbuf[maxShanghaiDataSize]={0};
					//	len = to64frombits((unsigned char *)tmpbuf,(unsigned char *)noteText,strlen(noteText));
					//	if(0 != len)
					//	{
					//		info.pNote = new unsigned char[len];
					//		memcpy(info.pNote,tmpbuf,len);
					//		info.textLen = len;
					//	}
					//	else
					//	{
					//		assert(false);
					//		info.textLen =0;
					//		info.pNote = NULL;
					//	}

					//}

					//info.picLen =0;
					//info.pPic = NULL;
					//std::list<OPER_INFO>::iterator it = m_listsensorPic.begin();
					//uuid_copy(info.uniqueGUID,it->uniqueGUID);
					//m_listsensorPic.push_front(info);
					//
				}
				else
				{
					if(SWL_INVALID_SOCKET != m_heartsock)
					{
						SWL_CloseSocket(m_heartsock);
						m_heartsock = SWL_INVALID_SOCKET;
					}
					m_bsensorHead = false;
					m_sensorpicCount = 0;//ƽ̨�رյ�ʱ��ͼƬ������0
					break;
				}
				
			}
			else
			{
				++m_sensorpicCount;//���ͳɹ���������ͼƬ����������
			}
			PUB_Sleep(1500);
		}
		if(maxPicCount < m_sensorpicCount)//ȫ���������
		{
			if(SWL_INVALID_SOCKET != m_heartsock)//ƽ̨�ر� 
			{
				SWL_CloseSocket(m_heartsock);
				m_heartsock = SWL_INVALID_SOCKET;
			}
			m_bsensorHead = false;
			m_sensorpicCount = 0;
			//�ж���Ķ������
			for(std::list<OPER_INFO>::iterator it = m_listsensorPic.begin();it!=m_listsensorPic.end();++it)
			{
				if(NULL != it->pNote)
				{
					delete []it->pNote;//�ڴ���Ҫ�ͷ�
					it->pNote = NULL;
				}
				if(NULL != it->pPic)
				{
					delete []it->pPic;
					it->pPic = NULL;
				}
			}
			m_listsensorPic.clear();
			break;

		}
		else
		{
			//�б���û��ͼ���������ͼ����40�ţ�����ʱ�䳬��һ����Χ��Ҳ��ֹ����
			if(0 == m_listsensorPic.size())
			{
				if(m_bsensorHead)
				{
					unsigned long time = GetCurrTime32();
					if(time - m_sensorAlarmStartTime >= m_maxpicdelayTime)
					{
						if(SWL_INVALID_SOCKET != m_heartsock)//ƽ̨�ر� 
						{
							SWL_CloseSocket(m_heartsock);
							m_heartsock = SWL_INVALID_SOCKET;
						}
						m_bsensorHead = false;
						m_sensorpicCount = 0;
						break;
					}
				}
			}

		}

	}
}
//end
