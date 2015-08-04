#include "OnvifEvent.h"
#include "NetInterface.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "SWL_Public.h"
#include <assert.h>

#ifndef WIN32
#include <sys/syscall.h>
#endif


#define MULTI_THREAD_NUMBER_USED 1

CPackBitsCompress::CPackBitsCompress()
{


}

CPackBitsCompress::~CPackBitsCompress()
{

}

//note the srcLen is not more than 127 Bytes
int CPackBitsCompress::Encode(const unsigned char *src,int srcLen,unsigned char *dest)
{
	unsigned char *encodeBuffer=NULL;
	unsigned  char destLen = 0;

	const unsigned  char *beginPos = NULL;
	int j=0;
	int tmpNum = 0;
	int tmpPos = 0;

	unsigned int  *samenum = NULL;
	unsigned int  sameLen = 0;
	int i=0;

	const unsigned char *pos=NULL;
	unsigned int deltaLen = 0;

	if(src == NULL || srcLen == 0)
	{
		return 0;
	}

	if(srcLen <= 2)
	{
		return srcLen;
	}

	//the encoder buffer size,the worst case is not need compress
	encodeBuffer = new unsigned char[srcLen*2];
	if(encodeBuffer == NULL)
	{
		return 0;
	}
	memset(encodeBuffer,0, sizeof(unsigned char)*(srcLen*2));

	samenum = new unsigned int[srcLen];
	if(samenum == NULL)
	{
		delete [] encodeBuffer;
		return 0;
	}
	memset(samenum,0,sizeof(unsigned int)*srcLen);

	pos =src;
	deltaLen  = srcLen;
	//calculate the same element count
	while(i<srcLen)
	{
		sameLen = FindSameLargestStr(pos,deltaLen);
		if(sameLen == 0)
		{
			break;
		}
		deltaLen = deltaLen - sameLen;
		*(samenum+i) = sameLen;
		pos = pos+sameLen;
		i= i+sameLen;
	}
	pos =NULL;


	//code
	beginPos = src;
	for(i= 0; i< srcLen; i += tmpNum)
	{
		tmpNum = 0;
		tmpPos = 0;
		if(samenum[i] > 1)
		{
			tmpNum = samenum[i];
			encodeBuffer[destLen++] = ~(tmpNum-1)+1;
			encodeBuffer[destLen++] = *(beginPos +i);/*src[i]*/;
		}
		else if(samenum[i]==1)
		{
			tmpPos = destLen++;
			tmpNum++;
			encodeBuffer[destLen++] = *(beginPos +i);
			for(j= i+1;j<srcLen;j++)
			{
				if(samenum[j] == 1)
				{
					tmpNum++;
					encodeBuffer[destLen++] = *(beginPos +j);
				}
				else if(samenum[j] >1)
				{
					break;
				}
			}
			encodeBuffer[tmpPos] = tmpNum-1;
		}
	}

	beginPos = NULL;
	memcpy(dest,encodeBuffer,destLen);

	if(encodeBuffer != NULL)
	{
		delete []encodeBuffer;
	}
	encodeBuffer = NULL;

	if(samenum != NULL)
	{
		delete []samenum;
	}
	samenum=NULL;

	return destLen;
}

int CPackBitsCompress::Decode(unsigned char *decSrc,int decLen,unsigned char *decDest)
{
	unsigned int  destLen = 0;
	unsigned char *decBuffer =NULL;
	unsigned char *reAllocBuffer = NULL;
	unsigned char *tmpBuffer = NULL;
	unsigned char *curDecPos = NULL;

	int i= 0;
	unsigned int j= 0;
	unsigned char tmpLen = 0;
	unsigned char ctrLen=0;

	if(decSrc == NULL)
	{
		return 0;
	}

	decBuffer = new unsigned char[127];
	if(decBuffer == NULL)
	{
		return 0;
	}
	memset(decBuffer,0,sizeof(unsigned char)*127);

	curDecPos = decSrc;
	for(i= 0; i < decLen; i += tmpLen)
	{	   
		if(*curDecPos >= 0 && *curDecPos < 127)
		{
			tmpLen+=1;
			ctrLen = *curDecPos+1;
			if(destLen + ctrLen > 127)
			{
				reAllocBuffer = new unsigned char[destLen+ctrLen];
				if(reAllocBuffer == NULL)
				{			   
					break;
				}
				memset(reAllocBuffer,0 ,sizeof(unsigned char)*(destLen+ctrLen));
				memcpy(reAllocBuffer,decBuffer,destLen);
				tmpBuffer = decBuffer;
				decBuffer = reAllocBuffer;
				reAllocBuffer = NULL;
				delete[] tmpBuffer;
			}

			for(j=0;j < ctrLen;j++)
			{
				decBuffer[destLen++] = *(curDecPos +j +1);
			}
			tmpLen += *curDecPos;
		}

		if(*curDecPos >= 127 && *curDecPos < 255)
		{
			tmpLen += 2;
			ctrLen = ~(*curDecPos -1)+1;
			//ctrLen = *curDecPos;
			//ctrLen = ~(*curDecPos -1);
			if(destLen + ctrLen > 127)
			{
				reAllocBuffer = new unsigned char[destLen+ctrLen];
				if(reAllocBuffer == NULL)
				{			   
					break;
				}
				memset(reAllocBuffer,0 ,sizeof(unsigned char)*(destLen+ctrLen));
				memcpy(reAllocBuffer,decBuffer,destLen);
				tmpBuffer = decBuffer;
				decBuffer = reAllocBuffer;
				reAllocBuffer = NULL;
				delete[] tmpBuffer;
			}
			for(j = 0;j < ctrLen;j++)
			{
				decBuffer[destLen++] = *(curDecPos+1);
			}
		}
		curDecPos += tmpLen;
	}

	reAllocBuffer = NULL;
	tmpBuffer = NULL;
	curDecPos = NULL;

	memcpy(decDest,decBuffer,destLen);
	if(decBuffer != NULL)
	{
		delete[] decBuffer;
	}
	decBuffer = NULL;

	return destLen;
}

int  CPackBitsCompress::FindSameLargestStr(const unsigned char *src,unsigned int srcLen)
{
	int len = 0;
	unsigned int  i = 1;
	const unsigned char *key =NULL;
	const unsigned char *nextValue =NULL;

	if(src == NULL || srcLen == 0)
	{
		return 0;
	}

	if(srcLen == 1)
	{
		return 1;
	}

	if(srcLen == 2)
	{
		if(*src == *(src+1))
		{
			len = 2;
		}
		else
		{
			len  =1;
		}
		return len;
	}

	key = src;
	nextValue = src+1;

	//find the same sub string
	unsigned int tmpLen = 0;
	tmpLen = srcLen-1;
	while(i < tmpLen)
	{
		if(*key == *nextValue)
		{
			len++;
			nextValue++;
		}
		else
		{
			//len++;
			//if(len ==0)
			//{
			// len =1;
			//}
			break;
		}
		i++;
	}  

	if(*nextValue  == *key)
	{
		len++;
	}
	len++;

	key = NULL;
	nextValue = NULL;

	return len;
}

int   CPackBitsCompress::FindDiffLargestStr(unsigned char *src,unsigned int srcLen)
{
	int len = 0;

	unsigned int  i = 0;
	unsigned char *value1 =NULL;
	unsigned char *value2 =NULL;

	if(src == NULL || srcLen == 0)
	{
		return 0;
	}

	if(srcLen == 1)
	{
		return 1;
	}

	value1 = src;
	value2  = src+1; 
	while(i <srcLen)
	{
		if( *value1 != *value2)
		{
			len++;
		}
		value1 =value2 ;
		value2 += 1;
	}
	value1 =NULL;
	value2 =NULL;

	return len;
}

const unsigned char *CPackBitsCompress::FindLargestStr(const unsigned char *srcStr,unsigned int srcLen,const unsigned char *keyValue,bool &bSameValue)
{
	const unsigned char *prePos = NULL;
	const  unsigned char *currPos = NULL;
	const unsigned char *endPos = NULL;
	const unsigned char *tmpPos= NULL;

	unsigned int countNum = 0;
	unsigned int sameNum = 0;

	bSameValue = false;
	prePos= keyValue;
	currPos = keyValue+1;

	//check the same value
	while(countNum < srcLen)
	{
		if(*prePos == *currPos)
		{
			countNum++;
			//prePos++;
			currPos++;
		}
		else
		{
			endPos = currPos;
			break;
		}
	}

	if(countNum == srcLen)
	{
		endPos = currPos;
	}

	if(countNum >= 2)
	{
		bSameValue = true;
		return endPos;
	}

	//check the different value 
	prePos = endPos;
	while(countNum < srcLen -1)
	{
		currPos++;
		//check the same value again
		if(*prePos == *currPos)
		{
			if(sameNum == 0)
			{
				tmpPos = prePos;
			}
			sameNum++;
			if(sameNum >=2)
			{
				endPos = tmpPos;
				break;
			}
		}
		else
		{
			sameNum = 0;
			tmpPos = NULL;
		}

		prePos++;
		//  currPos++;
		countNum++;
	}

	return endPos;
}

unsigned char *FindSameValueStr(unsigned char *src,unsigned char *key)
{
	unsigned int count = 0;
	return NULL;
}

unsigned char *FindDiffValueStr(unsigned char *src,unsigned char *key)
{
	return NULL;
}

void CPackBitsCompress::Decode(unsigned char *decSrc,int decLen,unsigned char *decDest,int &destLen)
{
	unsigned char *bp   = NULL,*op= NULL;
	long  n    = 0;
	int   flag = 0;
	int   cc   = 0;

	if(decSrc == NULL || decDest== NULL)
	{
		return;
	}

	bp = decSrc;
	cc = decLen;
	op = decDest;
	while(cc > 0)
	{
		n= (long)*bp++;
		cc--;
		if(n >= 128)
		{
			n-= 256;
		}
		if(n<0)
		{
			if(n == -128)
			{
				continue;
			}
			n= 1-n;
			flag = *bp++;
			cc--;
			destLen += n;
			while(n-- >0)
			{
				*op++ = (char)flag;	
			}
		}//n<0
		else
		{
			memcpy(op,bp,++n);
			op += n;
			bp += n;
			cc -= n;
			destLen += n;
		}

		if(destLen > 127)
		{
			break;
		}
	}//while cc
}




#define ONVIF_PASSIVE_RECV

COnvifEvent::COnvifEvent()
{
	int idx;
	for( idx = 0; idx < EVENT_THREAD_PROCESS_CNT; idx++ )
	{
		m_eventProcessID[idx] = PUB_CREATE_THREAD_FAIL;
		m_bEventProcess[idx] = false;

		memset( &m_stEventProcInfo[idx], 0, sizeof(onvif_event_recv_process) );
	}
	m_iEventIndex = 0;

	m_subscribeId = PUB_CREATE_THREAD_FAIL;
	m_bSubscribe = false;
	m_eventRecvId =PUB_CREATE_THREAD_FAIL;
	m_bEventRecv = false; 
	m_eventPullMsgId = PUB_CREATE_THREAD_FAIL;
	m_bPullMsgCreate = false;


	memset(m_motionAlarm,0,sizeof(m_motionAlarm));
	memset(m_shelterAlarm,0,sizeof(m_shelterAlarm));
	memset(m_sensorAlarm,0,sizeof(m_sensorAlarm));
	memset(m_videoLossAlarm,0,sizeof(m_videoLossAlarm));
	memset(m_otherAlarm,0,sizeof(m_otherAlarm));

	//device list

	//terminate time
	memset(m_terminationTime,0,sizeof(m_terminationTime));
	strcpy(m_terminationTime,"PT60S");

	m_listenSockID = -1;
	m_port = 50099;
	memset(m_listenIp,0,sizeof(m_listenIp));
	m_bIpExist = false;
}

COnvifEvent::~COnvifEvent()
{
	m_bIpExist = false;

	m_bSubscribe = false;
	m_bEventRecv = false; 
}

COnvifEvent *COnvifEvent::EventInstance(void)
{
	static COnvifEvent eventApp;
	return &eventApp;
}

void COnvifEvent::EventOpen(void)
{
	int idx;

	if(m_subscribeId != PUB_CREATE_THREAD_FAIL 
		|| m_eventRecvId != PUB_CREATE_THREAD_FAIL
		|| m_eventPullMsgId != PUB_CREATE_THREAD_FAIL )
	{
		return;
	}

	//thread 1:listen port,and receive event data
	m_eventRecvId = PUB_CreateThread(OnvifEventProcess,this,&m_bEventRecv);
	if(m_eventRecvId == PUB_CREATE_THREAD_FAIL)
	{
		return;
	}

	//thread 2:subscribe/update/cancel event
	m_subscribeId = PUB_CreateThread(OnvifSubscribeProcess,this,&m_bSubscribe);
	if(m_subscribeId == PUB_CREATE_THREAD_FAIL)
	{
		return;
	}

	m_eventPullMsgId = PUB_CreateThread(OnvifPullMsgGetStatic,this,&m_bPullMsgCreate);
	if(m_eventPullMsgId == PUB_CREATE_THREAD_FAIL)
	{
		return;
	}

#ifdef MULTI_THREAD_NUMBER_USED
	for( idx = 0; idx < EVENT_THREAD_PROCESS_CNT; idx++ )
	{
		if( PUB_CREATE_THREAD_FAIL == m_eventProcessID[idx])
		{
			m_eventProcessID[idx] = PUB_CreateThread(OnvifRecvEventProcess,this,&m_bEventProcess[idx]);
			if(m_eventProcessID[idx] == PUB_CREATE_THREAD_FAIL)
			{
				return;
			}
		}
	}
#endif //MULTI_THREAD_NUMBER_USED
}

void COnvifEvent::EventClose(void)
{
	int tmpId = 0;
	int idx;

	m_listenAddrLock.Lock();
	tmpId = m_listenSockID;
	m_listenSockID = -1;
	m_listenAddrLock.UnLock();

	if(tmpId >= 0)
	{
		SWL_CloseSocket(tmpId);
	}

	DelAllEvent();

	//exit listen thread
	if(m_eventRecvId != PUB_CREATE_THREAD_FAIL)
	{
		PUB_ExitThread(&m_eventRecvId,&m_bEventRecv);
	}

	//exit subscribe thread
	if(m_subscribeId != PUB_CREATE_THREAD_FAIL)
	{
		PUB_ExitThread(&m_subscribeId,&m_bSubscribe);
	}

	if(m_eventPullMsgId != PUB_CREATE_THREAD_FAIL)
	{
		PUB_ExitThread(&m_eventPullMsgId,&m_bPullMsgCreate);
	}

#ifdef MULTI_THREAD_NUMBER_USED
	for( idx = 0; idx < EVENT_THREAD_PROCESS_CNT; idx++ )
	{
		PUB_ExitThread(&m_eventProcessID[idx],&m_bEventProcess[idx]);
	}
#endif //MULTI_THREAD_NUMBER_USED

}

bool COnvifEvent::GetCurDeviceIPText(char *pIPText, unsigned short textLen, const char* name/* = "eth0"*/, unsigned short len/* = 4*/)
{
#ifdef WIN32
	return false;
#else
	assert (NULL != pIPText);
	assert (textLen > 15);
	assert (len > 0);
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);
	struct   ifreq   ifr;  
	int fd = -1;

	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name,   name,   IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ- 1] = 0; 

	fd = socket (AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) 
	{
		fd = -1;
		return false;
	}

	if (!(ioctl(fd, SIOCGIFFLAGS, (char *)&ifr)))
	{
		if(IFF_UP & ifr.ifr_flags)
		{
			if (!(ioctl(fd, SIOCGIFADDR, (char *)&ifr)) )
			{
				unsigned long ip = ((sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr;
				sprintf(pIPText, "%d.%d.%d.%d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);

				if( fd > 0 )
				{
					close( fd );
					fd = -1;
				}
				return true;				
			}
			else
			{
				if( fd > 0 )
				{
					close( fd );
					fd = -1;
				}
				return false;						
			}
		}
		else
		{
			if( fd > 0 )
			{
				close( fd );
				fd = -1;
			}
			return false;
		}
	}	
	//	printf("GetIpaddress(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
	if( fd > 0 )
	{
		close( fd );
		fd = -1;
	}
	return false;	
#endif
}


void COnvifEvent::SetListenAddr(char *ipAddr,int aPort)
{
	char iptext[96];
	bool result = false;

	if(ipAddr == NULL)
	{
		return;
	}

	memset(iptext,0,sizeof(iptext));
	if(m_bIpExist)
	{
		return;
	}

#ifdef WIN32
	m_listenAddrLock.Lock();
	memset(m_listenIp,0,sizeof(m_listenIp));
	strcpy(m_listenIp,ipAddr);
	m_port = aPort;
	m_listenAddrLock.UnLock();

#else
	unsigned long len = sizeof(iptext);
	result = CNetInterface::Instance()->GetIPText(iptext, len);
	//printf("$$$$$$$ONVIF Event listen ip:%s\n",iptext);
	if(result)
	{
		m_listenAddrLock.Lock();
		memset(m_listenIp,0,sizeof(m_listenIp));
		//strcpy(m_listenIp,ipAddr); //save the alarm server address
		strcpy(m_listenIp,iptext);
		m_port = aPort;
		m_listenAddrLock.UnLock();
	}
#endif
	m_bIpExist = true;
}

void COnvifEvent::SetListenAddrByManual(char *ipAddr,int aPort)
{
	char iptext[96];
	bool result = false;

	if(ipAddr == NULL)
	{
		return;
	}

	memset(iptext,0,sizeof(iptext));
	if(m_bIpExist)
	{
		return;
	}

#ifdef WIN32
	m_listenAddrLock.Lock();
	memset(m_listenIp,0,sizeof(m_listenIp));
	strcpy(m_listenIp,ipAddr);
	m_port = aPort;
	m_listenAddrLock.UnLock();

#else
	unsigned long len = sizeof(iptext);
	result = GetCurDeviceIPText(iptext, len);
	printf("$$$$$$$ONVIF Event listen ip for only number:%s\n",iptext);
	if(result)
	{
		m_listenAddrLock.Lock();
		memset(m_listenIp,0,sizeof(m_listenIp));
		//strcpy(m_listenIp,ipAddr); //save the alarm server address
		strcpy(m_listenIp,iptext);
		m_port = aPort;
		m_listenAddrLock.UnLock();
	}
#endif
	m_bIpExist = true;
}

bool COnvifEvent::GetListenAddress( char* ipAddr, int len, const char* eth_name, unsigned int eth_len )
{
#ifdef WIN32
#else
	char iptext[96];

	memset(iptext,0,sizeof(iptext));
	bool bGet = GetCurDeviceIPText(iptext, 96, eth_name, eth_len );
	if( bGet )
	{
		memset( ipAddr, 0, len );
		strncpy(ipAddr, iptext, 96);

		return true;
	}

#endif

	return false;
}


void COnvifEvent::AddChnnEvent(int chnn,OnvifEventChnnInfo* pOnvifEvent)
{
	OnvifEventChnnInfo *pEventInfo= NULL;
	char listenIp[96];
	int port =0;

	if(chnn < 0 || chnn >gChnnTotalNum)
	{
		return;
	}

	pEventInfo = new OnvifEventChnnInfo;
	assert(NULL != pEventInfo);
	memset(listenIp,0,sizeof(listenIp)); 

	//copy the share variable
	m_listenAddrLock.Lock();
	strcpy(listenIp,m_listenIp);
	port = m_port;
	m_listenAddrLock.UnLock();

	memset(pEventInfo,0,sizeof(OnvifEventChnnInfo));
	pEventInfo->chnn = chnn;
	strcpy( pEventInfo->localIpAddr, pOnvifEvent->localIpAddr );
	strcpy(pEventInfo->userName,pOnvifEvent->userName);
	strcpy(pEventInfo->passKey,pOnvifEvent->passKey);
	strcpy(pEventInfo->servAddr,pOnvifEvent->servAddr);
	pEventInfo->checkCount = gRenewCount;
	pEventInfo->iIpcCtrlType = pOnvifEvent->iIpcCtrlType;
	strcpy(pEventInfo->devUtcServAddr,pOnvifEvent->devUtcServAddr );
	pEventInfo->status = TSUBSCRIBE_STATUS;

	pEventInfo->pullmsg_status = TSUBSCRIBE_STATUS;
	pEventInfo->pullmsg_checkcnt = gRenewCount;
	pEventInfo->bHasRecvAlarmMsg = false;

	pEventInfo->bCanProcessPullMsg = pOnvifEvent->bCanProcessPullMsg;

	pEventInfo->b_renew_subscribe_event = true;

	m_motionAlarm[chnn]=0;
	m_shelterAlarm[chnn]=0;
	m_sensorAlarm[chnn]=0;
	m_videoLossAlarm[chnn]=0;
	m_otherAlarm[chnn]=0;

	m_eventInfoLock.Lock();
	m_eventChnnInfoList.push_back(pEventInfo);
	pEventInfo= NULL;
	m_eventInfoLock.UnLock();

}

void COnvifEvent::DelChnnEvent(int chnn)
{
	m_eventInfoLock.Lock();
	OnvifEventChnnInfo *tmp=NULL;
	std::list<OnvifEventChnnInfo*>::iterator it;

	if(!m_eventChnnInfoList.empty())
	{
		for(it = m_eventChnnInfoList.begin(); it != m_eventChnnInfoList.end(); ++it)
		{
			if( ((*it) != NULL) && ((*it)->chnn == chnn) )
			{
				CancelSubscribe((*it));  
				m_motionAlarm[chnn]=0;
				m_shelterAlarm[chnn]=0;
				m_sensorAlarm[chnn]=0;
				m_videoLossAlarm[chnn]=0;
				m_otherAlarm[chnn]=0;
				delete (*it);
				(*it) =NULL;

				m_eventChnnInfoList.erase(it);

				break;
			}
		}
	}
	tmp=NULL;
	m_eventInfoLock.UnLock();

}

void COnvifEvent::DelAllEvent(void)
{ 

	m_eventInfoLock.Lock();
	OnvifEventChnnInfo *tmp=NULL;
	std::list<OnvifEventChnnInfo*>::iterator it;
	if(!m_eventChnnInfoList.empty())
	{
		for(it = m_eventChnnInfoList.begin(); it != m_eventChnnInfoList.end();++it)
		{
			if((*it) != NULL)
			{
				CancelSubscribe((*it));
				delete (*it);
			}
			(*it) = NULL;
		}
		m_eventChnnInfoList.clear();
	}
	tmp=NULL;
	memset(m_motionAlarm,0,sizeof(m_motionAlarm));
	memset(m_shelterAlarm,0,sizeof(m_shelterAlarm));
	memset(m_sensorAlarm,0,sizeof(m_sensorAlarm));
	memset(m_videoLossAlarm,0,sizeof(m_videoLossAlarm));
	memset(m_otherAlarm,0,sizeof(m_otherAlarm));
	m_eventInfoLock.UnLock();

}

void COnvifEvent::ResetChnnEvent(int aChnn,char *aServAddr)
{
	std::list<OnvifEventChnnInfo*>::iterator it;

	if(aChnn >= gChnnTotalNum || aChnn < 0)
	{
		return;
	}

	m_eventInfoLock.Lock();
	if(!m_eventChnnInfoList.empty())
	{
		for(it = m_eventChnnInfoList.begin(); it != m_eventChnnInfoList.end(); it++)
		{
			if(strstr((*it)->servAddr,aServAddr) != NULL)
			{
				(*it)->chnn = aChnn;
				m_motionAlarm[aChnn]=0;
				m_shelterAlarm[aChnn]=0;
				m_sensorAlarm[aChnn]=0;
				m_videoLossAlarm[aChnn]=0;
				m_otherAlarm[aChnn]=0;
				break;
			}
		}
	}
	m_eventInfoLock.UnLock();   
}

void COnvifEvent::SetDevLinkStatus(int chnn,bool bActive)
{

}

int COnvifEvent::GetDevAlarm(int chnn,enum EOnvifAlarmType aAlarmType)
{
	int  alarmValue = 0;

	if( chnn < 0 || chnn >= gChnnTotalNum )
	{
		return 0;
	}

	switch(aAlarmType)
	{
	case TEVENT_ALARM_MOTION:
		m_motionLock.Lock();
		alarmValue = m_motionAlarm[chnn];
		m_motionAlarm[chnn] = 0;
		m_motionLock.UnLock();
		break;
	case  TEVENT_ALARM_SHELTER:
		m_shelterLock.Lock();
		alarmValue = m_shelterAlarm[chnn];
		m_shelterAlarm[chnn] =0;
		m_shelterLock.UnLock();
		break;
	case TEVENT_ALARM_SENSOR:
		m_sensorLock.Lock();
		alarmValue = m_sensorAlarm[chnn];
		m_sensorAlarm[chnn] = 0;
		m_sensorLock.UnLock();
		break;
	case TEVENT_ALARM_VIDEO_LOSS:
		m_videoLossLock.Lock();
		alarmValue = m_videoLossAlarm[chnn];
		m_videoLossAlarm[chnn] = 0;
		m_videoLossLock.UnLock();
		break;
	case TEVENT_ALARM_OTHER:
		m_otherLock.Lock();
		alarmValue = m_otherAlarm[chnn];
		m_otherAlarm[chnn]=0;
		m_otherLock.UnLock();
		break;
	default:
		break;
	}
	return alarmValue;
}

PUB_THREAD_RESULT PUB_THREAD_CALL COnvifEvent::OnvifPullMsgGetStatic(void *param)
{
	COnvifEvent *pThis = NULL;

#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  OnvifPullMsgGetStatic\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	pThis = (COnvifEvent *)param;
	if(pThis != NULL)
	{
		return pThis->ProcessPullMsg();
	}
	return 0;
}


PUB_THREAD_RESULT PUB_THREAD_CALL COnvifEvent::OnvifSubscribeProcess(void *param)
{
	COnvifEvent *pThis = NULL;

#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  OnvifSubscribeThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	pThis = (COnvifEvent *)param;
	if(pThis != NULL)
	{
		return pThis->ProcessSubscribe();
	}
	return 0;
}

PUB_THREAD_RESULT PUB_THREAD_CALL COnvifEvent::OnvifRecvEventProcess(void *param)
{
	COnvifEvent *pThis = NULL;

#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  OnvifSubscribeThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	pThis = (COnvifEvent *)param;
	if(pThis != NULL)
	{
		return pThis->ProcessRecvEvent();
	}
	return 0;
}

PUB_THREAD_RESULT PUB_THREAD_CALL COnvifEvent::OnvifEventProcess(void *param)
{

	COnvifEvent *pThis = NULL;

#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  OnvifEventProcessThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	pThis = (COnvifEvent *)param;
	if(pThis != NULL)
	{
		return pThis->ProcessListen();
	}
	return 0;
}

PUB_THREAD_RESULT COnvifEvent::ProcessPullMsg(void)
{
	while(m_bSubscribe)
	{
		PullMsg_UpdateAlarmEvent();
		PUB_Sleep(500);
	}
	return 0;   
}


PUB_THREAD_RESULT COnvifEvent::ProcessSubscribe(void)
{
	while(m_bSubscribe)
	{
		RecvMsg_CheckChnnStatus();
		RecvMsg_CheckEventUpdate();
		PUB_Sleep(5000);
	}
	return 0;   
}

int COnvifEvent::insert_event_rect_proc( int sock, int chnn, bool brecv )
{
	int idx;
	int tmp;
	onvif_event_recv_process* p_proc_info = NULL;
	CPUB_Lock* p_lock_info = NULL;
	int pos = 0;
	int cnt = EVENT_PROCESS_CHNNS_CNT;
	int ret = -1;

	for( idx = 0; idx < EVENT_THREAD_PROCESS_CNT; idx++ )
	{
		p_proc_info = &m_stEventProcInfo[idx];
		p_lock_info = &m_lockEventProcInfo[idx];

		p_lock_info->Lock();
		if( cnt > p_proc_info->cnt )
		{
			cnt = p_proc_info->cnt;
			pos = idx;
		}
		p_lock_info->UnLock();
	}

	if( cnt >= EVENT_PROCESS_CHNNS_CNT )
	{
		return ret;
	}

	p_proc_info = &m_stEventProcInfo[pos];
	p_lock_info = &m_lockEventProcInfo[pos];

	p_lock_info->Lock();
	tmp = p_proc_info->wr;
	if( 0 == p_proc_info->sock[tmp] )
	{
		p_proc_info->sock[tmp] = sock;
		p_proc_info->channel[tmp] = chnn;
		p_proc_info->brecv[tmp] = brecv;

		if( ++p_proc_info->wr >= EVENT_PROCESS_CHNNS_CNT)
		{
			p_proc_info->wr = 0;
		}
		p_proc_info->cnt++;

		//printf( "********************sock %d, %d, %d,%d\n", pos, tmp, sock, chnn );

		ret = 0;
	}
	p_lock_info->UnLock();

	return ret;
}

PUB_THREAD_RESULT COnvifEvent::ProcessRecvEvent(void)
{
	int index = 0;
	int cnt = 0;

	int sock = 0;
	int chnn = 0;
	bool brecv = false;

	int tmp = 0;

	onvif_event_recv_process* p_proc_info = NULL;
	CPUB_Lock* p_lock_info = NULL;

	m_sensorLock.Lock();
	index = m_iEventIndex++;
	m_sensorLock.UnLock();

	p_proc_info = &m_stEventProcInfo[index];
	p_lock_info = &m_lockEventProcInfo[index];

	while(m_bSubscribe)
	{
		p_lock_info->Lock();
		cnt = p_proc_info->cnt;
		p_lock_info->UnLock();

		if( cnt > 0 )
		{
			sock = 0;
			p_lock_info->Lock();
			tmp = p_proc_info->rd;
			if( p_proc_info->sock[tmp] > 0 )
			{
				sock = p_proc_info->sock[tmp];
				chnn = p_proc_info->channel[tmp];
				brecv = p_proc_info->brecv[tmp];

				p_proc_info->sock[tmp] = 0;
				p_proc_info->cnt--;
				if( ++p_proc_info->rd >= EVENT_PROCESS_CHNNS_CNT )
				{
					p_proc_info->rd = 0;
				}
			}
			p_lock_info->UnLock();

			if( sock > 0 )
			{
				if( SOAP_INIT_FAULT == EventRecvNotify(sock,chnn, brecv) )
				{
					SWL_CloseSocket(sock);
					sock = -1;
				}
				//printf( "++++++++++++++++++++++++proc %d,%d-- %d,%d\n", index, tmp, sock, chnn );
			}

		}
		else
		{
			PUB_Sleep(300);
		}
	}
	return 0;   
}

PUB_THREAD_RESULT COnvifEvent::ProcessListen(void)
{
	int listenSock = 0;
	int sock = -1;
	int result = -1;
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);

	unsigned int chnn = 96;
	int nready = 0;
	int i= 0;
	int  maxSoketID= -1;
	bool bRecvMsg = false;

	int ret = -1;
	linger   tmpLinger;
	int  tmpListenId = -1,port = -1;
	struct timeval timeout = {0, 10000};

	m_listenAddrLock.Lock();
	port = m_port;
	m_listenAddrLock.UnLock();

	memset(&tmpLinger,0,sizeof(tmpLinger));
	tmpListenId = BindSock(port);
	if(maxSoketID <= tmpListenId)
	{
		maxSoketID = tmpListenId;
	}

	m_listenAddrLock.Lock();
	m_listenSockID=tmpListenId;
	m_listenAddrLock.UnLock();

	PUB_Sleep(3000);

	while(m_bEventRecv)
	{
		chnn = gChnnTotalNum +1;

		m_listenAddrLock.Lock();
		tmpListenId = m_listenSockID;
		m_listenAddrLock.UnLock();

		if(tmpListenId < 0)
		{
			printf("ONVIF lister thread sock id < 0!\n");
			PUB_Sleep(3000);
			continue;
		}

		maxSoketID = tmpListenId;

		FD_ZERO(&m_fdr);
		FD_SET(tmpListenId,&m_fdr);

		timeout.tv_sec = 0;
		timeout.tv_usec = 10000;
		nready =select(maxSoketID+1,&m_fdr,NULL,NULL,&timeout);

		if(nready == 0)
		{
			//printf("select timeout!\n");
			PUB_Sleep(10);
			//continue;
		}
		else if(nready < 0)
		{
			printf("select other fault!\n");
			PUB_Sleep(10);
		}
		else
		{	   
			if(FD_ISSET(tmpListenId,&m_fdr))//read
			{
				sock = -1;
				memset(&addr,0,sizeof(struct sockaddr_in));

				//accept
				sock = SWL_Accept(tmpListenId,(struct sockaddr *)&addr,(SWL_socklen_t *)&addr_len);

				if(SWL_INVALID_SOCKET == sock) 
				{
					if(SWL_EWOULDBLOCK())
					{
						printf("accept SWL_EWOULDBLOCK!\n");
					}
					else
					{

					}
					continue;
				}

				//check the IPC device IP address
				bRecvMsg = false;
				chnn = CheckEventIP((char *)inet_ntoa(addr.sin_addr), bRecvMsg );

				if(chnn >= 0 && chnn < gChnnTotalNum)
				{
					tmpLinger.l_onoff = 1;
					tmpLinger.l_linger = 0;

					ret = setsockopt(sock,SOL_SOCKET,SO_LINGER,(const char *)&tmpLinger,sizeof(tmpLinger));

#ifdef MULTI_THREAD_NUMBER_USED
					if( 0 != insert_event_rect_proc( sock, chnn, bRecvMsg ) )
					{
						printf( "------------------sock error %d,%d\n", sock, chnn );

						SWL_CloseSocket(sock);
						sock = -1;

					}
#else
					if( SOAP_INIT_FAULT == EventRecvNotify(sock,chnn, bRecvMsg) )
					{
						SWL_CloseSocket(sock);
						sock = -1;
					}
#endif
				}
				else
				{
					SWL_CloseSocket(sock);
					sock = -1;
				}
			}
		}//select read case
		//PUB_Sleep(5);
	}//while

	return 0;
}


void COnvifEvent::PullMsg_UpdateAlarmEvent(void)
{
	OnvifEventChnnInfo *pChnnInfo=NULL;
	std::list<OnvifEventChnnInfo*>::iterator it;
	EOnvifEventChnnStatus status;

	m_eventInfoLock.Lock();
	if(m_eventChnnInfoList.empty())
	{
		m_eventInfoLock.UnLock();
		return;
	}

	for(it = m_eventChnnInfoList.begin(); it != m_eventChnnInfoList.end(); ++it)
	{
		pChnnInfo = (*it);
		status = (*it)->pullmsg_status;

		//是否使能此功能
		if( !((*it)->bCanProcessPullMsg) )
		{
			continue;
		}

		//表示已经通过listen获取了事件
		if( (*it)->bHasRecvAlarmMsg )
		{
			continue;
		}

		switch( status )
		{
		case TIDLE_STATUS:
			{
				if( (*it)->pullmsg_checkcnt > 0 )
				{
					(*it)->pullmsg_checkcnt--;
				}
				else
				{
					(*it)->pullmsg_status = TRENEW_STATUS;
					(*it)->pullmsg_checkcnt = gRenewCount;

				}
				break;
			}

		case TERR_WAIT_STATUS:
			{
				if( (*it)->pullmsg_checkcnt > 0 )
				{
					(*it)->pullmsg_checkcnt--;
				}
				else
				{
					(*it)->pullmsg_status = TSUBSCRIBE_STATUS;
					(*it)->pullmsg_checkcnt = gRenewCount;

				}
				break;
			}

		case TRENEW_STATUS:
			{
				if( OPERATE_SUCCESS == PullMsg_GetAlarmMessages(pChnnInfo) )
				{
					(*it)->pullmsg_status = TIDLE_STATUS;
				}
				else
				{
					(*it)->pullmsg_status = TERR_WAIT_STATUS;
				}
				break;
			}

		case TSUBSCRIBE_STATUS:
			{
				if( OPERATE_SUCCESS == PullMsg_SetupSubscribe(pChnnInfo) )
				{
					if( OPERATE_SUCCESS == PullMsg_GetAlarmMessages(pChnnInfo) )
					{
						(*it)->pullmsg_status = TIDLE_STATUS;
					}
					else
					{
						(*it)->pullmsg_status = TERR_WAIT_STATUS;
					}
				}
				break;
			}

		default:
			{
				break;
			}
		}
	}
	m_eventInfoLock.UnLock();
}


void COnvifEvent::RecvMsg_CheckEventUpdate(void)
{
	char listenAddr[96];
	int   port=0;

	int  chnn = 0;
	OnvifEventChnnInfo *pChnnInfo=NULL;
	std::list<OnvifEventChnnInfo*>::iterator it;
	EOnvifEventChnnStatus status = TEND_STATUS;


	m_listenAddrLock.Lock();
	port = m_port;
	m_listenAddrLock.UnLock();

	//update the update list
	m_eventInfoLock.Lock();
	if(m_eventChnnInfoList.empty())
	{
		m_eventInfoLock.UnLock();
		return;
	}

	for(it = m_eventChnnInfoList.begin(); it != m_eventChnnInfoList.end(); ++it)
	{
		pChnnInfo = (*it);
		if((*it)->status == TSUBSCRIBE_STATUS)
		{
			memset(listenAddr,0,sizeof(listenAddr));
			strcpy(listenAddr,pChnnInfo->localIpAddr);

			if(RecvMsg_SetupSubscribe(pChnnInfo,listenAddr,port) == OPERATE_SUCCESS)
			{
				(*it)->status = TIDLE_STATUS;
			}
			else
			{
				(*it)->status = TERR_WAIT_STATUS;
			}
		}
		else if((*it)->status == TRENEW_STATUS)
		{
			if(RecvMsg_RenewSubscribe(pChnnInfo) == OPERATE_SUCCESS)
			{
				(*it)->status = TIDLE_STATUS;
			}
			else
			{
				(*it)->b_renew_subscribe_event = false;

				memset(listenAddr,0,sizeof(listenAddr));
				strcpy(listenAddr,pChnnInfo->localIpAddr);
				if(RecvMsg_SetupSubscribe(pChnnInfo,listenAddr,port) == OPERATE_SUCCESS)
				{
					(*it)->status = TIDLE_STATUS;
				}
				else
				{
					(*it)->status = TERR_WAIT_STATUS;
				}
			}
		}
	}
	m_eventInfoLock.UnLock();
}

void COnvifEvent::RecvMsg_CheckChnnStatus(void)
{	
	m_eventInfoLock.Lock();
	std::list<OnvifEventChnnInfo*>::iterator it;
	if(m_eventChnnInfoList.empty())
	{
		m_eventInfoLock.UnLock();
		return;
	}
	for(it = m_eventChnnInfoList.begin(); it != m_eventChnnInfoList.end(); it++)
	{	
		if((*it)->checkCount > 0)	
		{
			(*it)->checkCount--;
		}
		else
		{
			if( TIDLE_STATUS == (*it)->status )
			{
				if( true == (*it)->b_renew_subscribe_event )
				{
					(*it)->status = TRENEW_STATUS;
				}
				else
				{
					(*it)->status = TSUBSCRIBE_STATUS;
				}
			}
			else if( TERR_WAIT_STATUS == (*it)->status )
			{
				(*it)->status = TSUBSCRIBE_STATUS;
			}

			(*it)->checkCount = gSubCnt;
		}

	}
	m_eventInfoLock.UnLock();
}

void COnvifEvent::GetEventInfoFromTopic(int channel,char* AlarmTopic,char* AlarmMessage)
{
	if( channel < 0 || channel >= gChnnTotalNum || !AlarmTopic || !AlarmMessage )
	{
		return;
	}

	char strBackAlarmTopic[64] = {0};
	char strTtDataInfo[256] = {0};
	int event_position = 0;
	int event_typeval = 222;
	int tmp_posval = 0;
	char* ttDataBegin = NULL;
	char* ttDataEnd = NULL;

	strncpy(strBackAlarmTopic, AlarmTopic, 63 );
	strlwr_tvt( strBackAlarmTopic );

	if( strstr( strBackAlarmTopic, "motion") )
	{
		ttDataBegin = strstr( AlarmMessage, "<tt:Data>" );
		ttDataEnd = strstr( AlarmMessage, "</tt:Data>" );
		if( ttDataBegin && ttDataEnd && ((int)ttDataEnd - (int)ttDataBegin < 256))
		{
			strTtDataInfo[0] = '\0';
			strncpy( strTtDataInfo, ttDataBegin, (int)ttDataEnd - (int)ttDataBegin );
			strlwr_tvt( strTtDataInfo );

			if( strstr( strTtDataInfo, "\"true\"") )
			{
				m_motionLock.Lock();
				m_motionAlarm[channel]=1; 
				m_motionLock.UnLock();

				event_position = __LINE__;
				event_typeval = 0;
			}
			else if( strstr( strTtDataInfo, "\"1\"") )
			{
				m_motionLock.Lock();
				m_motionAlarm[channel]=1; 
				m_motionLock.UnLock();

				event_position = __LINE__;
				event_typeval = 0;
			}
			else
			{
				event_position = __LINE__;
			}
		}
		else
		{
			event_position = __LINE__;
		}
	}
	else if( strstr( strBackAlarmTopic, "tns1:device/trigger")
		|| (strncasecmp(AlarmTopic,"tns1:UserAlarm", 14) == 0) )
	{
		ttDataBegin = strstr( AlarmMessage, "<tt:Data>" );
		ttDataEnd = strstr( AlarmMessage, "</tt:Data>" );
		if( ttDataBegin && ttDataEnd && ((int)ttDataEnd - (int)ttDataBegin < 256))
		{
			strTtDataInfo[0] = '\0';
			strncpy( strTtDataInfo, ttDataBegin, (int)ttDataEnd - (int)ttDataBegin );
			strlwr_tvt( strTtDataInfo );

			if( strstr( strTtDataInfo, "\"true\"") )
			{
				m_sensorLock.Lock();
				m_sensorAlarm[channel] = 1;
				m_sensorLock.UnLock();

				event_position = __LINE__;
				event_typeval = 1;
			}
			else if( strstr( strTtDataInfo, "\"1\"") )
			{
				m_sensorLock.Lock();
				m_sensorAlarm[channel] = 1;
				m_sensorLock.UnLock();

				event_position = __LINE__;
				event_typeval = 1;
			}
			else if( strstr( strTtDataInfo, "\"open\"") )
			{
				m_sensorLock.Lock();
				m_sensorAlarm[channel] = 1;
				m_sensorLock.UnLock();

				event_position = __LINE__;
				event_typeval = 1;
			}
			else if( strstr( strTtDataInfo, "\"active\"") )
			{
				m_sensorLock.Lock();
				m_sensorAlarm[channel] = 1;
				m_sensorLock.UnLock();

				event_position = __LINE__;
				event_typeval = 1;
			}
			else if( strstr( strTtDataInfo, "\"on\"") )
			{
				m_sensorLock.Lock();
				m_sensorAlarm[channel] = 1;
				m_sensorLock.UnLock();

				event_position = __LINE__;
				event_typeval = 1;
			}
			else
			{
				event_position = __LINE__;
			}
		}
		else
		{
			event_position = __LINE__;
		}

	}
	else 
	{
		m_otherLock.Lock();
		m_otherAlarm[channel] = 1;
		m_otherLock.UnLock();

		event_position = __LINE__;
		event_typeval = 3;
	}

	//printf("Event_RecvNotify channel %d,%d,%d ,message %s, +++  %s\n", 
	//	event_typeval, event_position, channel, AlarmMessage, AlarmTopic );

}

//update the ip address if the ip is not in the local network
void COnvifEvent::UpdateIpAddr(char *oldAddr,char *newAddr)
{
	char tBuf[256];
	char *pos1 = NULL;
	char *pos2 = NULL;
	char *pos3 = NULL;

	if(oldAddr == NULL || newAddr == NULL)
	{
		return;
	}

	memset(tBuf,0,sizeof(tBuf));
}


int COnvifEvent::EventRecvNotify(int sock,unsigned int channel, bool bRecvMsg)
{
	int result = -1;
	int ret = -1;

	COnvifSoap  soapApp;
	enum EResultType eResult= OTHER_FAULT;

	char *AlarmMessage = NULL;
	char *AlarmTopic = NULL;

	struct wsnt__NotificationMessageHolderType *pMessage =NULL;

	struct __denc__Notify Notify;

	Notify.wsnt__Notify = NULL;

	if(channel >= gChnnTotalNum) //this channel device is not exist
	{
		return SOAP_INIT_FAULT;
	}

	soapApp.InitSoap(TDEVICE_EVENT,false);
	if(soapApp.m_pSoap == NULL)
	{
		return SOAP_INIT_FAULT;
	}
	soapApp.m_pSoap->socket = sock;

	soapApp.m_pSoap->header = NULL;

	soapApp.m_pSoap->connect_flags = SOL_SOCKET;
	soapApp.m_pSoap->bind_flags = SO_LINGER;
	soapApp.m_pSoap->linger_time = 0;

	result = soap_recv___denc__Notify(soapApp.m_pSoap,&Notify);
	if(Notify.wsnt__Notify == NULL)
	{
		soapApp.FreeSoap();
		return OPERATE_FAILED;
	}


	if(SOAP_OK == result)
	{
		if(Notify.wsnt__Notify->__sizeNotificationMessage > 0)
		{
			for(int i = 0;i < Notify.wsnt__Notify->__sizeNotificationMessage;i++)
			{
				if(Notify.wsnt__Notify->NotificationMessage[i].Message.__any == NULL)
				{
					continue;
				}
				AlarmMessage = Notify.wsnt__Notify->NotificationMessage[i].Message.__any;
				if(Notify.wsnt__Notify->NotificationMessage[i].Topic == NULL)
				{
					continue;
				}
				AlarmTopic =  Notify.wsnt__Notify->NotificationMessage[i].Topic->__any;
				if(AlarmMessage != NULL && AlarmTopic != NULL)
				{
					if( !bRecvMsg )
					{
						SetHasRecvMsgProc( channel );
					}

					GetEventInfoFromTopic( channel, AlarmTopic, AlarmMessage );
				}
			}
		}

		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	soapApp.FreeSoap();
	return eResult;
}



int COnvifEvent::RecvMsg_SetupSubscribe(OnvifEventChnnInfo *aChnnInfo,char *devIp,int port)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	COnvifSoap soapApp;

	char Address[64] = {'\0'};
	//tm nowTm;

	struct wsnt__FilterType Filter;
	struct _wsnt__Subscribe Subscribe ;
	struct _wsnt__SubscribeResponse SubscribeResp;

	Subscribe.Filter = NULL;
	Subscribe.InitialTerminationTime = m_terminationTime;
	Subscribe.SubscriptionPolicy = NULL;
	Subscribe.__size = 0;
	Subscribe.__any = NULL;
	SubscribeResp.__size = 0;

	SubscribeResp.SubscriptionReference.Address = NULL;
	SubscribeResp.SubscriptionReference.ReferenceParameters =NULL;	
	SubscribeResp.SubscriptionReference.Metadata = NULL;	
	SubscribeResp.SubscriptionReference.__size = NULL;	
	SubscribeResp.SubscriptionReference.__any = NULL;
	SubscribeResp.SubscriptionReference.__anyAttribute = NULL;	
	SubscribeResp.__any = NULL;
	SubscribeResp.CurrentTime = NULL;
	SubscribeResp.TerminationTime = NULL;

	sprintf(Address,"http://%s:%d/onvif/Events/",devIp,port);

	Subscribe.ConsumerReference.Address = Address;

	Subscribe.ConsumerReference.ReferenceParameters = NULL;
	Subscribe.ConsumerReference.Metadata = NULL;
	Subscribe.ConsumerReference.__size = 0;
	Subscribe.ConsumerReference.__any = NULL;
	Subscribe.ConsumerReference.__anyAttribute = NULL;

	Filter.__any = NULL;
	Filter.__size= 0;

	char *filterContent = "<wsnt:TopicExpression Dialect =\"http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet\">\n tns1:RuleEngine/CellMotionDetector//. | tns1:Device/Trigger//.</wsnt:TopicExpression>\n";
	Filter.__size = 1;
	Filter.__any  = &filterContent;

	Subscribe.Filter = NULL;//&Filter;

	if(aChnnInfo == NULL)
	{
		return SERVER_ADDR_FAULT;
	}
	if(aChnnInfo->chnn < 0 || aChnnInfo->chnn > gChnnTotalNum)
	{
		return PARAMETER_FAULT;
	}

	result = SOAP_OK;

	do 
	{
		soapApp.InitDevUtcTimes(false, aChnnInfo->devUtcServAddr, aChnnInfo->iIpcCtrlType );
		if(soapApp.ProduceSoapInfo(TDEVICE_EVENT,true,aChnnInfo->userName,aChnnInfo->passKey) != SOAP_INIT_SUCCESS)
		{
			result =  PARAMETER_FAULT;
			break;
		}

		result = soap_call___denf__Subscribe(soapApp.m_pSoap, aChnnInfo->servAddr, NULL,&Subscribe,&SubscribeResp);
		if(result != SOAP_OK)
		{
			soapApp.ExitSoap();
			if(soapApp.ProduceSoapInfo(TDEVICE_EVENT,false,aChnnInfo->userName,aChnnInfo->passKey) != SOAP_INIT_SUCCESS)
			{
				result =  SOAP_INIT_FAULT;
				break;
			}
			result = soap_call___denf__Subscribe(soapApp.m_pSoap, aChnnInfo->servAddr, NULL,&Subscribe,&SubscribeResp);
		}
	} while (0);


	if(SOAP_OK == result)
	{
		if(SubscribeResp.SubscriptionReference.Address != NULL)
		{
			memset(aChnnInfo->subscribeUri,0,sizeof(aChnnInfo->subscribeUri));
			strcpy(aChnnInfo->subscribeUri,SubscribeResp.SubscriptionReference.Address);

			//device not in the LAN
			//UpdateIpAddr(m_eventUri,m_subscribeResp);

		}
		//nowTm = DVRTimeToTm(GetCurrTime());

		//printf("^^^^Subscribe time:%d:%d:%d,%d:%d:%d\n",nowTm.tm_year+1900,nowTm.tm_mon,nowTm.tm_mday,nowTm.tm_hour,nowTm.tm_min,nowTm.tm_sec);
		//printf("subscribe current time:%s\n",SubscribeResp.CurrentTime);
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}

	soapApp.ExitSoap();

	return eResult;   
}

int COnvifEvent::CancelSubscribe(OnvifEventChnnInfo *aChnnInfo)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	COnvifSoap  soapApp;

	struct _wsnt__Unsubscribe Unsubscribe;
	struct _wsnt__UnsubscribeResponse UnsubscribeResponse;

	Unsubscribe.__size = 0;
	Unsubscribe.__any = NULL;
	UnsubscribeResponse.__any= NULL;
	UnsubscribeResponse.__size = 0;


	if(aChnnInfo == NULL)
	{
		return SERVER_ADDR_FAULT;
	}
	if(aChnnInfo->chnn < 0 || aChnnInfo->chnn > gChnnTotalNum)
	{
		return PARAMETER_FAULT;
	}

	soapApp.InitDevUtcTimes(false,aChnnInfo->devUtcServAddr,aChnnInfo->iIpcCtrlType);
	if(soapApp.ProduceSoapInfo(TDEVICE_EVENT,true,aChnnInfo->userName,aChnnInfo->passKey) != SOAP_INIT_SUCCESS)
	{
		return PARAMETER_FAULT;
	}

	soapApp.AddActionTo(aChnnInfo->subscribeUri);
	result = soap_call___desm__Unsubscribe(soapApp.m_pSoap, aChnnInfo->servAddr, NULL,&Unsubscribe,&UnsubscribeResponse);
	if(result != SOAP_OK)
	{
		soapApp.ExitSoap();
		if(soapApp.ProduceSoapInfo(TDEVICE_EVENT,false,aChnnInfo->userName,aChnnInfo->passKey) != SOAP_INIT_SUCCESS)
		{
			result =  SOAP_INIT_FAULT;
		}
		else
		{
			result = soap_call___desm__Unsubscribe(soapApp.m_pSoap, aChnnInfo->servAddr, NULL,&Unsubscribe,&UnsubscribeResponse);
		}
	}

	soapApp.ExitSoap();

	if(SOAP_OK == result)
	{
		//printf("cancel subscribe success!\n");
		eResult = OPERATE_SUCCESS;
	}
	else
	{

		eResult = OPERATE_FAILED;
	}

	return eResult; 
}

int COnvifEvent::RecvMsg_RenewSubscribe(OnvifEventChnnInfo *aChnnInfo)
{
	int result = -1;
	enum EResultType eResult= OTHER_FAULT;
	COnvifSoap  soapApp;

	struct _wsnt__Renew Renew;
	struct _wsnt__RenewResponse RenewResponse;

	Renew.TerminationTime = m_terminationTime;
	Renew.__size = 0;
	Renew.__any = NULL;
	RenewResponse.__size = 0;
	RenewResponse.__any = NULL;
	RenewResponse.CurrentTime = NULL;
	RenewResponse.TerminationTime = 0;


	if(aChnnInfo == NULL)
	{
		return SERVER_ADDR_FAULT;
	}

	soapApp.InitDevUtcTimes(false,aChnnInfo->devUtcServAddr,aChnnInfo->iIpcCtrlType);
	if(soapApp.ProduceSoapInfo(TDEVICE_EVENT,true,aChnnInfo->userName,aChnnInfo->passKey) != SOAP_INIT_SUCCESS)
	{
		result = PARAMETER_FAULT;
	}
	else
	{
		soapApp.AddActionTo(aChnnInfo->subscribeUri);
		result = soap_call___desm__Renew(soapApp.m_pSoap, aChnnInfo->subscribeUri, NULL,&Renew,&RenewResponse);
	}

	soapApp.ExitSoap();

	if(SOAP_OK == result)
	{
		eResult = OPERATE_SUCCESS;
	}
	else
	{
		eResult = OPERATE_FAILED;
	}
	return eResult;
}

int COnvifEvent::PullMsg_SetupSubscribe(OnvifEventChnnInfo *aChnnInfo)
{
	int result = -1;
	enum EResultType eResult= OPERATE_FAILED;
	COnvifSoap  soapApp;

	struct _tev__CreatePullPointSubscription CreatePullPointSubscription;
	struct _tev__CreatePullPointSubscriptionResponse CreatePullPointSubscriptionResp;

	if(aChnnInfo == NULL)
	{
		return SERVER_ADDR_FAULT;
	}

	soapApp.InitDevUtcTimes(false,aChnnInfo->devUtcServAddr,aChnnInfo->iIpcCtrlType);
	if(soapApp.ProduceSoapInfo(TDEVICE_EVENT,true,aChnnInfo->userName,aChnnInfo->passKey) != SOAP_INIT_SUCCESS)
	{
		result = PARAMETER_FAULT;
	}
	else
	{
		CreatePullPointSubscription.SubscriptionPolicy = NULL;
		CreatePullPointSubscription.InitialTerminationTime = m_terminationTime;
		CreatePullPointSubscription.Filter = NULL;
		CreatePullPointSubscription.__size = 0;
		CreatePullPointSubscription.__any = NULL;

		CreatePullPointSubscriptionResp.wsnt__CurrentTime = 0;
		CreatePullPointSubscriptionResp.wsnt__TerminationTime = 0;
		CreatePullPointSubscriptionResp.SubscriptionReference.ReferenceParameters = NULL;
		CreatePullPointSubscriptionResp.SubscriptionReference.Metadata = NULL;
		CreatePullPointSubscriptionResp.SubscriptionReference.Address = NULL;
		CreatePullPointSubscriptionResp.SubscriptionReference.__size = 0;
		CreatePullPointSubscriptionResp.SubscriptionReference.__anyAttribute = NULL;
		CreatePullPointSubscriptionResp.SubscriptionReference.__any = NULL;
		CreatePullPointSubscriptionResp.__size = 0;
		CreatePullPointSubscriptionResp.__any = NULL;

		soapApp.AddActionTo(aChnnInfo->subscribeUri);
		result = soap_call___dee__CreatePullPointSubscription(soapApp.m_pSoap,
			aChnnInfo->servAddr,NULL,&CreatePullPointSubscription,&CreatePullPointSubscriptionResp);
		if(result == SOAP_OK)
		{
			if(CreatePullPointSubscriptionResp.SubscriptionReference.Address != NULL)
			{
				memset(aChnnInfo->subscribeUri,0,sizeof(aChnnInfo->subscribeUri));
				strcpy(aChnnInfo->subscribeUri,CreatePullPointSubscriptionResp.SubscriptionReference.Address);
				eResult = OPERATE_SUCCESS;
			}
		}

	}

	soapApp.ExitSoap();

	return eResult;
}

int COnvifEvent::PullMsg_GetAlarmMessages(OnvifEventChnnInfo *aChnnInfo)
{
	int result = -1;
	enum EResultType eResult= OPERATE_FAILED;
	COnvifSoap  soapApp;

	int i= 0;
	int alarm_cnt = 0;
	char tUuid[64];

	struct _tev__PullMessages PullMessages;
	struct _tev__PullMessagesResponse PullMessagesResp;
	struct wsa5__EndpointReferenceType wsa5__ReplyTo;

	char *AlarmMessage = NULL;
	char *AlarmTopic = NULL;

	soapApp.InitDevUtcTimes(false,aChnnInfo->devUtcServAddr,aChnnInfo->iIpcCtrlType);
	if(soapApp.ProduceSoapInfo(TDEVICE_EVENT,true,aChnnInfo->userName,aChnnInfo->passKey) != SOAP_INIT_SUCCESS)
	{
		result = PARAMETER_FAULT;
	}
	else
	{
		memset(tUuid,0,sizeof(tUuid));
		onvif_soap_generate_uuid(tUuid);
		soapApp.m_pHeader->wsa5__MessageID = NULL;//tUuid;

		memset(&wsa5__ReplyTo,0,sizeof(struct wsa5__EndpointReferenceType));
		wsa5__ReplyTo.Address = "http://www.w3.org/2005/08/addressing/anonymous";
		soapApp.m_pHeader->wsa5__ReplyTo = NULL;//&wsa5__ReplyTo;

		soapApp.m_pHeader->wsa5__Action = "http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/PullMessagesRequest";
		soapApp.m_pHeader->wsa5__To = aChnnInfo->subscribeUri;

		PullMessages.Timeout = "PT1M";
		PullMessages.MessageLimit = 256;
		PullMessages.__size = 0;
		PullMessages.__any = NULL;

		PullMessagesResp.CurrentTime = 0;
		PullMessagesResp.TerminationTime = 0;
		PullMessagesResp.__sizeNotificationMessage = 0;
		PullMessagesResp.wsnt__NotificationMessage = NULL;

		result = soap_call___depps__PullMessages(soapApp.m_pSoap, aChnnInfo->subscribeUri, "123", &PullMessages, &PullMessagesResp);
		if(result == SOAP_OK)
		{
			if(PullMessagesResp.wsnt__NotificationMessage != NULL)
			{
				alarm_cnt = 0;
				if( PullMessagesResp.__sizeNotificationMessage > 10 )
				{
					alarm_cnt = PullMessagesResp.__sizeNotificationMessage - 10;
				}
				for( i = alarm_cnt; i<PullMessagesResp.__sizeNotificationMessage; i++ )
				{
					AlarmTopic = PullMessagesResp.wsnt__NotificationMessage[i].Topic->__any;
					AlarmMessage = PullMessagesResp.wsnt__NotificationMessage[i].Message.__any;
					if(AlarmTopic != NULL)
					{
						GetEventInfoFromTopic( aChnnInfo->chnn, AlarmTopic, AlarmMessage );
					}
				}
			}

			eResult = OPERATE_SUCCESS;

			//printf("pullmessages success!\n");
		}
		else
		{
			//printf("pullmessages failed!\n");
		}
	}

	soapApp.ExitSoap();

	return eResult;
}

//recv message handle
int COnvifEvent::BindSock(int port)
{
	int sock = -1;
	char i = 0;
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);

	sock = SWL_CreateSocket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		return -1;
	}
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(SWL_Bind(sock,(struct sockaddr *)&addr,sizeof(addr)))
	{
		return -1;
	}
	if(SWL_Listen(sock,1))
	{
		return -1;
	}
	return sock;
}


unsigned int COnvifEvent::CheckEventIP(char *ip, bool& bRecvMsg)
{
	unsigned int channel = gChnnTotalNum + 1;

	m_eventInfoLock.Lock();
	list<OnvifEventChnnInfo*>::iterator it;
	for(it = m_eventChnnInfoList.begin(); it != m_eventChnnInfoList.end(); it++)
	{
		if(strstr((*it)->servAddr,ip) != NULL)
		{
			channel = (*it)->chnn;
			bRecvMsg = (*it)->bHasRecvAlarmMsg;
			break;
		}
	}
	m_eventInfoLock.UnLock();

	return channel;
}

void COnvifEvent::SetHasRecvMsgProc(int chann)
{
	m_eventInfoLock.Lock();
	list<OnvifEventChnnInfo*>::iterator it;
	for(it = m_eventChnnInfoList.begin(); it != m_eventChnnInfoList.end(); it++)
	{
		if((*it)->chnn == chann )
		{
			(*it)->bHasRecvAlarmMsg = true;;
			break;
		}
	}
	m_eventInfoLock.UnLock();
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

const unsigned int ONVIF_SNAP_SEND_BUF_LEN = 1024;
const unsigned int ONVIF_SNAP_MAX_PIC_LEN = 2*512*1024;
const unsigned int ONVIF_SNAP_RECV_BUF_LEN = 64*1024;
const unsigned int ONVIF_SNAP_TIMEOUT = 3000;
const unsigned int ONVIF_SNAP_DEFAULT_INTERVAL = 2500;

unsigned int ONVIF_SNAP_GetTickCount()
{
#ifdef WIN32
	return GetTickCount();
#else
	struct timespec tp;
	if(clock_gettime(CLOCK_MONOTONIC, &tp) < 0)
	{
		assert(false);
	}

	return tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
#endif
}


COnvifSnapJpeg::COnvifSnapJpeg()
{
	m_bIsStart = false;
}

COnvifSnapJpeg::~COnvifSnapJpeg()
{

}

COnvifSnapJpeg * COnvifSnapJpeg::Instance()
{
	static COnvifSnapJpeg s_OnvifSnapJpeg;
	return &s_OnvifSnapJpeg;
}

bool COnvifSnapJpeg::Start()
{
	if (m_bIsStart)
	{
		return true;
	}

	m_hRecvThreadID = PUB_CreateThread(RecvThread, (void *)this, &m_bRecvThreadProc);
	m_hSendThreadID = PUB_CreateThread(SendThread, (void *)this, &m_bSendThreadProc);


	if( PUB_CREATE_THREAD_FAIL == m_hRecvThreadID )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
	if( PUB_CREATE_THREAD_FAIL == m_hSendThreadID )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	m_bIsStart = true;
}

bool COnvifSnapJpeg::Stop()
{
	if (!m_bIsStart)
	{
		return true;
	}

	if (PUB_THREAD_ID_NOINIT == m_hRecvThreadID || false == m_bRecvThreadProc)
	{
		printf("%s %d err\n", __FILE__, __LINE__);
		return false;
	}
	if (PUB_THREAD_ID_NOINIT == m_hSendThreadID || false == m_bSendThreadProc)
	{
		printf("%s %d err\n", __FILE__, __LINE__);
		return false;
	}

	PUB_ExitThread(&m_hRecvThreadID, &m_bRecvThreadProc);
	PUB_ExitThread(&m_hSendThreadID, &m_bSendThreadProc);

	m_bIsStart = false;

	CloseAll();
	std::map<int, SnapInfo*>::iterator it;
	for (it=m_mapSnapInfo.begin(); it!=m_mapSnapInfo.end();)
	{
		if ((*it).second->pRecvRes)
		{
			delete (*it).second->pRecvRes;
			(*it).second->pRecvRes = NULL;
		}
		if ((*it).second->pSendBuf)
		{
			delete (*it).second->pSendBuf;
			(*it).second->pSendBuf = NULL;
		}
		delete (*it).second;
		(*it).second = NULL;
		m_mapSnapInfo.erase(it++);
	}
}

PUB_THREAD_RESULT PUB_THREAD_CALL COnvifSnapJpeg::SendThread( void *pParam )
{
#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  COnvifSnapJpeg::SendThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	COnvifSnapJpeg *pObj = reinterpret_cast<COnvifSnapJpeg *>(pParam);
	pObj->SendProc();
	return 0;
}

PUB_THREAD_RESULT PUB_THREAD_CALL COnvifSnapJpeg::RecvThread( void *pParam )
{
#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  COnvifSnapJpeg::RecvThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	COnvifSnapJpeg *pObj = reinterpret_cast<COnvifSnapJpeg *>(pParam);
	pObj->RecvProc();
	return 0;
}

void COnvifSnapJpeg::RecvProc()
{
	struct timeval timeOut;
	fd_set FD_Set;
	SWL_socket_t MaxSockFd;
	int ret = 0;

	while(m_bRecvThreadProc)
	{
		timeOut.tv_sec = 0;
		timeOut.tv_usec = 10000;

		m_Lock.Lock();
		FD_Set = m_fdsetRead;
		MaxSockFd = m_MaxSockFd;
		m_Lock.UnLock();

		if (MaxSockFd <= 0)
		{
			PUB_Sleep(10);
			continue;
		}

		ret = select(MaxSockFd+1, &FD_Set, NULL, NULL, &timeOut);
		if (ret > 0)
		{
			Recv(FD_Set);
		}
		else if(ret < 0)
		{
			CloseAll();
		}
		else
		{
			PUB_Sleep(10);
		}

		PUB_Sleep(1);
	}
}

void COnvifSnapJpeg::SendProc()
{

	while(m_bSendThreadProc)
	{
		SnapIntervalCtrl();

		ConnectToServer();

		SendCmd();

		PUB_Sleep(10);
	}
}

int COnvifSnapJpeg::AddDevSnap( int channel )
{
	int ret = -1;
	m_Lock.Lock();
	if(m_mapSnapInfo.find(channel) == m_mapSnapInfo.end())
	{
		SnapInfo *pSnapInfo = new SnapInfo;
		if (pSnapInfo)
		{
			memset(pSnapInfo, 0, sizeof(SnapInfo));
			pSnapInfo->snapInterval = ONVIF_SNAP_DEFAULT_INTERVAL;
			pSnapInfo->defaultInterval = ONVIF_SNAP_DEFAULT_INTERVAL;
			pSnapInfo->sock = SWL_INVALID_SOCKET;
			if (!m_mapSnapInfo.insert(std::pair<int, SnapInfo*>(channel, pSnapInfo)).second)
			{
				delete pSnapInfo;
				pSnapInfo = NULL;
				ret = -1;
			}
			else
			{
				ret = 0;
			}
		}
	}
	m_Lock.UnLock();

	return ret;
}

void COnvifSnapJpeg::DelDevSnap( int channel )
{
	std::map<int, SnapInfo*>::iterator it;
	m_Lock.Lock();
	it = m_mapSnapInfo.find(channel);
	if(it != m_mapSnapInfo.end())
	{
		CloseConnect((*it).second);
		if ((*it).second->pRecvRes)
		{
			delete (*it).second->pRecvRes;
			(*it).second->pRecvRes = NULL;
		}
		if ((*it).second->pSendBuf)
		{
			delete (*it).second->pSendBuf;
			(*it).second->pSendBuf = NULL;
		}
		delete (*it).second;
		(*it).second = NULL;
		m_mapSnapInfo.erase(it);
	}
	m_Lock.UnLock();
}

void COnvifSnapJpeg::DelAllSnap()
{
	std::map<int, SnapInfo*>::iterator it;
	m_Lock.Lock();
	for(it=m_mapSnapInfo.begin(); it != m_mapSnapInfo.end();)
	{
		if ((*it).second->pRecvRes)
		{
			delete (*it).second->pRecvRes;
			(*it).second->pRecvRes = NULL;
		}
		if ((*it).second->pSendBuf)
		{
			delete (*it).second->pSendBuf;
			(*it).second->pSendBuf = NULL;
		}
		delete (*it).second;
		(*it).second = NULL;
		m_mapSnapInfo.erase(it++);
	}
	m_Lock.UnLock();
}

int COnvifSnapJpeg::StartSnap( int channel, char *pSnapUri, ONVIF_SNAP_CALLBACK CallBackFun, void *pParam, char* pServerAddress )
{
	int ret = -1;

	std::map<int, SnapInfo*>::iterator it;
	m_Lock.Lock();
	it = m_mapSnapInfo.find(channel);
	if (it != m_mapSnapInfo.end())
	{
		if (!(*it).second->pSendBuf)
		{
			(*it).second->pSendBuf = new char[ONVIF_SNAP_SEND_BUF_LEN];
		}
		if ((*it).second->pSendBuf)
		{
			if (!(*it).second->bStart)//防止多次调用修改参数
			{
				if (pSnapUri && CallBackFun && pParam)
				{
					if (ParseUri(pSnapUri, pServerAddress, (*it).second->pSendBuf, (*it).second->ip, (*it).second->port))
					{
						(*it).second->callBack = CallBackFun;
						(*it).second->pParam = pParam;
						(*it).second->bStart = true;
						ret = 0;
					}
				}
			}
			else
			{
				ret = 0;
			}
		}
	}
	m_Lock.UnLock();

	return ret;
}

int COnvifSnapJpeg::StopSnap( int channel )
{
	int ret = -1;
	std::map<int, SnapInfo*>::iterator it;
	m_Lock.Lock();
	it = m_mapSnapInfo.find(channel);
	if (it != m_mapSnapInfo.end())
	{
		CloseConnect((*it).second);
		if ((*it).second->pRecvRes)
		{
			delete (*it).second->pRecvRes;
			(*it).second->pRecvRes = NULL;
		}
		(*it).second->bStart = false;
		ret = 0;
	}

	m_Lock.UnLock();

	return ret;
}

int COnvifSnapJpeg::SetSnapPeriodParam( int channel, unsigned short period, unsigned short num, unsigned int capture_number )
{
	int ret = -1;
	std::map<int, SnapInfo*>::iterator it;
	m_Lock.Lock();
	it = m_mapSnapInfo.find(channel);
	if (it != m_mapSnapInfo.end() && num != 0)
	{
		(*it).second->snapInterval = (unsigned int)(period * 1000) / num;

		if(SNAP_JPEG_CAPTRUE_USED_DEFAULT == capture_number )
		{
			(*it).second->defaultInterval = (*it).second->snapInterval;
			(*it).second->captureNumber = 0;
		}
		else
		{
			(*it).second->captureNumber = capture_number;
		}
		ret = 0;
	}
	m_Lock.UnLock();

	return ret;
}

void COnvifSnapJpeg::ConnectToServer()
{
	std::map<int, SnapInfo*>::iterator it;
	bool bNeedConnect = false;
	SWL_socket_t sock = SWL_INVALID_SOCKET;
	unsigned long ip = 0;
	unsigned short port = 0;
	int key = 0;

	m_Lock.Lock();
	for (it=m_mapSnapInfo.begin(); it!=m_mapSnapInfo.end(); ++it)
	{
		if ((*it).second->bStart)
		{
			if (!(*it).second->bConnected && (*it).second->sendLen > 0)
			{		
				bNeedConnect = true;
				ip = (*it).second->ip;
				port = (*it).second->port;
				key = (*it).first;
				break;
			}
		}
	}
	m_Lock.UnLock();

	if (bNeedConnect)
	{
		bool ret = Connect(sock, ip, port);
		m_Lock.Lock();
		it = m_mapSnapInfo.find(key); 
		if (it == m_mapSnapInfo.end())
		{
			CloseSock(sock);
			ret = false;
		}
		if (ret)
		{
			(*it).second->bConnected = true;
			(*it).second->sock = sock;
		}
		else
		{
		}

		m_Lock.UnLock();

	}
}

bool COnvifSnapJpeg::ParseUri( const char *pUri, const char* pServerAddress, char *pRequestCmd, unsigned long &ip, unsigned short &port )
{
	char host[128] = {0};
	char location[128] = {0};
	port = 80;

	if (pUri && pRequestCmd && strlen(pUri) < 256)
	{
		do 
		{
			if (1 != sscanf(pUri, "http://%s", host))
			{
				break;
			}
			char *pPos = strstr(host, "/");
			if (!pPos)
			{
				break;
			}
			strcpy(location, pPos);
			*pPos = '\0';
			pPos = strstr(host, ":");
			if (pPos)
			{
				if (1 != sscanf(++pPos, "%d", &port))
				{
					break;
				}
				*(pPos-1) = '\0';
			}
			sprintf(pRequestCmd, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: Close\r\n\r\n", location, pServerAddress);
			printf( "uriResp ========================== %s\n", pRequestCmd );
			ip = inet_addr(pServerAddress);

			return true;

		} while(0);
	}

	printf("%s %d Uri Err host :%s\n", __FILE__, __LINE__, host);

	return false;
}

bool COnvifSnapJpeg::Connect( SWL_socket_t &sockFd, unsigned long ip, unsigned short port )
{
	if (sockFd != SWL_INVALID_SOCKET || ip == 0 || port == 0)
	{
		return false;
	}

	sockFd = SWL_CreateSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SWL_INVALID_SOCKET == sockFd)
	{
		return false;
	}

	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(struct sockaddr_in));
	sockAddr.sin_addr.s_addr = ip;
	sockAddr.sin_port = htons(port);
	sockAddr.sin_family = AF_INET;

	if (0 != SWL_Connect(sockFd, (const struct sockaddr *)&sockAddr, sizeof(struct sockaddr_in), 200))
	{
		SWL_CloseSocket(sockFd);
		sockFd = SWL_INVALID_SOCKET;

		return false;
	}

	return true;
}

void COnvifSnapJpeg::SendCmd()
{
	std::map<int, SnapInfo*>::iterator it;
	int ret = 0;
	m_Lock.Lock();
	unsigned int tickCount = ONVIF_SNAP_GetTickCount();
	for (it=m_mapSnapInfo.begin(); it!=m_mapSnapInfo.end(); ++it)
	{
		if ((*it).second->bConnected && (*it).second->sendLen > 0 && (*it).second->currSendLen < (*it).second->sendLen)
		{
			ret = SendCmd((*it).second->sock, (*it).second->pSendBuf + (*it).second->currSendLen, (*it).second->sendLen - (*it).second->currSendLen);
			if (ret > 0)
			{
				(*it).second->currSendLen += ret;
				if ((*it).second->currSendLen == (*it).second->sendLen)
				{
					(*it).second->sendLen = 0;
					(*it).second->currSendLen = 0;
					FD_SET((*it).second->sock, &m_fdsetRead);
					if (m_MaxSockFd < (*it).second->sock)
					{
						m_MaxSockFd = (*it).second->sock;
					}
					if (!(*it).second->pRecvRes)
					{
						(*it).second->pRecvRes = new char[ONVIF_SNAP_RECV_BUF_LEN + sizeof(RTSP_DATA_FRAME)];
						(*it).second->pRecvBuf = (*it).second->pRecvRes + sizeof(RTSP_DATA_FRAME);
						(*it).second->recvBufLen = ONVIF_SNAP_RECV_BUF_LEN;
					}
					(*it).second->currRecvLen = 0;
					(*it).second->contentLen = 0;
				}
			}
			else if(ret < 0)
			{
				CloseConnect((*it).second);
			}
			else
			{
				if ( tickCount >= (*it).second->startTick + ONVIF_SNAP_TIMEOUT)
				{
					CloseConnect((*it).second);
				}
			}
		}
	}
	m_Lock.UnLock();
}

int COnvifSnapJpeg::SendCmd( SWL_socket_t sockFd, const char *pData, unsigned int len )
{
	int ret = -1;
	if (sockFd != SWL_INVALID_SOCKET && pData && len > 0)
	{
		ret = SWL_Send(sockFd, pData, len, 0);
		if (ret < 0)
		{
			if (SWL_EWOULDBLOCK())
			{
				ret = 0;
			}
		}
	}

	return ret;
}

void COnvifSnapJpeg::SnapIntervalCtrl()
{
	std::map<int, SnapInfo*>::iterator it;
	m_Lock.Lock();
	unsigned int tickCount = ONVIF_SNAP_GetTickCount();
	for (it = m_mapSnapInfo.begin(); it!=m_mapSnapInfo.end(); ++it)
	{
		if ((*it).second->bStart)
		{
			unsigned int currTick = tickCount;
			if (currTick > (*it).second->snapInterval + (*it).second->startTick)
			{
				if ((*it).second->sendLen == 0)
				{
					(*it).second->startTick = currTick;
					(*it).second->sendLen = strlen((*it).second->pSendBuf);
				}
			}

			if( (*it).second->captureNumber > 0 )
			{
				(*it).second->captureNumber--;

				if( (*it).second->captureNumber <= 0 )
				{
					if( (*it).second->snapInterval != (*it).second->defaultInterval )
					{
						(*it).second->snapInterval = (*it).second->defaultInterval;
					}
				}
			}
		}	
	}
	m_Lock.UnLock();
}

void COnvifSnapJpeg::CloseSock( SWL_socket_t &sockFd )
{
	if (sockFd != SWL_INVALID_SOCKET)
	{
		FD_CLR(sockFd, &m_fdsetRead);
		SWL_CloseSocket(sockFd);
		sockFd = SWL_INVALID_SOCKET;

		std::map<int, SnapInfo*>::iterator it;
		m_MaxSockFd = 0;
		for (it=m_mapSnapInfo.begin(); it!=m_mapSnapInfo.end(); ++it)
		{
			if (SWL_INVALID_SOCKET != (*it).second->sock)
			{
				if (m_MaxSockFd < (*it).second->sock)
				{
					m_MaxSockFd = (*it).second->sock;
				}
			}
		}
	}	
}

int COnvifSnapJpeg::Recv( SWL_socket_t sockFd, char *pBuf, unsigned int len )
{
	int ret = -1;
	if (sockFd != SWL_INVALID_SOCKET && pBuf && len > 0)
	{
		ret = SWL_Recv(sockFd, pBuf, len, 0);
		if (ret == 0)
		{
			ret = -1;
		}
		else if(ret == -1)
		{
			if (SWL_EWOULDBLOCK())
			{
				ret = 0;
			}
		}
	}

	return ret;
}

void COnvifSnapJpeg::Recv( fd_set &fdSet )
{
	std::map<int, SnapInfo*>::iterator it;
	int ret = 0;
	m_Lock.Lock();
	for (it=m_mapSnapInfo.begin(); it!=m_mapSnapInfo.end(); ++it)
	{
		if ((*it).second->sock!=SWL_INVALID_SOCKET && FD_ISSET((*it).second->sock, &fdSet))
		{
			ret = Recv((*it).second->sock, (*it).second->pRecvBuf + (*it).second->currRecvLen, (*it).second->recvBufLen - (*it).second->currRecvLen);
			if (ret > 0)
			{
				(*it).second->currRecvLen += ret;

				if ((*it).second->contentLen == 0)
				{
					ret = ParseCmd((*it).second->pRecvBuf, (*it).second->currRecvLen, (*it).second->contentLen);
					if (ret > 0)
					{
						if ((*it).second->contentLen > (*it).second->recvBufLen)
						{
							if ((*it).second->contentLen > ONVIF_SNAP_MAX_PIC_LEN)
							{
								printf("%s %d pic contentLen err %lu\n", __FILE__, __LINE__, (*it).second->contentLen);
								CloseConnect((*it).second);
								continue;
							}
							char *pBuf = new char[(*it).second->contentLen + sizeof(RTSP_DATA_FRAME)];
							if (!pBuf)
							{
								CloseConnect((*it).second);
								continue;
							}
							memcpy(pBuf + sizeof(RTSP_DATA_FRAME), (*it).second->pRecvBuf + ret, (*it).second->currRecvLen - ret);
							delete (*it).second->pRecvRes;
							(*it).second->pRecvRes = pBuf;
							(*it).second->pRecvBuf = pBuf + sizeof(RTSP_DATA_FRAME);
							(*it).second->recvBufLen = (*it).second->contentLen;
						}
						else
						{
							memmove((*it).second->pRecvBuf, (*it).second->pRecvBuf + ret, (*it).second->currRecvLen - ret);
						}
						(*it).second->currRecvLen -= ret;
					}
					else if(ret < 0)
					{
						if (ret == -2)//TO DO
						{
						}
						else
						{
						}
						CloseConnect((*it).second);
						continue;
					}
					else
					{
						continue;
					}
				}
				else
				{

				}

				if ((*it).second->contentLen == (*it).second->currRecvLen)
				{
					CallCb((*it).second, (*it).first);
					CloseConnect((*it).second);
					memset((*it).second->pRecvBuf, 0, (*it).second->recvBufLen);
				}		
			}
			else if(ret < 0)
			{
				CloseConnect((*it).second);
			}
			else
			{
				if (ONVIF_SNAP_GetTickCount() >= (*it).second->startTick + ONVIF_SNAP_TIMEOUT)
				{
					CloseConnect((*it).second);
				}
			}
		}
		else
		{
			if ((*it).second->sock!=SWL_INVALID_SOCKET)
			{
				if (ONVIF_SNAP_GetTickCount() >= (*it).second->startTick + ONVIF_SNAP_TIMEOUT)
				{
					CloseConnect((*it).second);
				}
			}
		}
	}

	m_Lock.UnLock();
}

int COnvifSnapJpeg::ParseCmd( const char *pData, unsigned int len, unsigned int &contentLen )
{
	int ret = -1;
	int retCode = 0;

	if (pData && len > 0)
	{
		ret = 0;
		const char *pEnd = strstr(pData, "\r\n\r\n");
		do 
		{
			if (!pEnd)
			{
				break;
			}
			ret = -1;
			if (pEnd - pData + 4 > len)
			{
				break;
			}
			const char *pPos = strstr(pData, "HTTP/1.1");
			if (!pPos)
			{
				pPos = strstr(pData, "HTTP/1.0");
				if (!pPos)
				{
					break;
				}
			}
			if (1 != sscanf(pPos + 8 + 1, "%d", &retCode))
			{
				break;
			}
			if (retCode != HTTP_CMD_OK)
			{
				if (retCode == HTTP_CMD_UNAUTHORIZED)
				{
					ret = -2;
				}
				break;
			}
			pPos = strstr(pData, "Content-Length");
			if (!pPos)
			{
				pPos = strstr(pData, "Content-length");
				if (!pPos)
				{
					break;
				}
			}
			if (1 != sscanf(pPos+14, "%*[^0123456789]%d", &contentLen))
			{
				break;
			}

			return pEnd - pData + 4;
		} while(0);

		//if (ret == -1)
		// 		{
		// 			printf("%s %d Cmd %s\n", __FILE__, __LINE__, pData);
		// 		}
	}

	return ret;
}

void COnvifSnapJpeg::CallCb( SnapInfo* pSnapInfo, unsigned int channel )
{
	if (pSnapInfo && pSnapInfo->callBack && pSnapInfo->pParam)
	{
		RTSP_DATA_FRAME *pFrame = (RTSP_DATA_FRAME *)(pSnapInfo->pRecvRes);
		pFrame->pData = (unsigned char*)pFrame;
		pFrame->frame.length = pSnapInfo->contentLen;
		pFrame->length = pFrame->frame.length + sizeof(RTSP_DATA_FRAME);
		pFrame->frame.streamType = RTSP_STREAM_TYPE_PICTURE;
		pFrame->streamID = SNAP_JPEG_STREAM_ID_VALUE;
		pFrame->frame.time = RTSPGetCurrTime();;
		//pFrame->frame.localTime = pFrame->frame.time.LocalTime();
		pSnapInfo->callBack(pFrame, channel, pSnapInfo->pParam);
	}
}

void COnvifSnapJpeg::CloseAll()
{
	std::map<int, SnapInfo*>::iterator it;
	m_Lock.Lock();
	for (it=m_mapSnapInfo.begin(); it!=m_mapSnapInfo.end(); ++it)
	{
		CloseConnect((*it).second);
	}
	FD_ZERO(&m_fdsetRead);
	m_MaxSockFd = 0;
	m_Lock.UnLock();
}

void COnvifSnapJpeg::CloseConnect( SnapInfo *pSnapInfo )
{
	if (!pSnapInfo)
	{
		return;
	}
	CloseSock(pSnapInfo->sock);
	pSnapInfo->bConnected = false;
	pSnapInfo->sendLen = 0;
	pSnapInfo->currSendLen = 0;
	pSnapInfo->currRecvLen = 0;
	pSnapInfo->contentLen = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
