
#include "ShanghaiSnapMan.h"
#include "SnapCapabilityMan.h"
#include "SnapMan.h"
#include <stdlib.h>
#include "ShanghaiConnectMan.h"
CShanghaiSnapMan *CShanghaiSnapMan::GetInstance()
{
	static CShanghaiSnapMan s_shanghaiSnapMan;
	return &s_shanghaiSnapMan;
}

CShanghaiSnapMan::CShanghaiSnapMan()
{
	m_pData = NULL;
	m_manualSnapTime = 0;
	m_OnDemandSnapTime = 0;
	m_bmanualSnap = false;

	m_pChnnStatus = NULL;
	m_pChnnCount = NULL;
	m_pLastTime = NULL;
	m_pChannType = NULL;
	
	m_preSec	=  NULL;
	m_postSec   =  NULL;
	m_startSnapTime = NULL;

	m_maxchnn  =  0;

	m_nMaxSnap = 0;
	m_nMaxTime = 0;
	m_manualImageType = 0;
	m_snapcount = NULL;
	memset(m_policeNote,0,sizeof(m_policeNote));

	m_pAlarmPrevTime = NULL;

	m_OnDemandChnnBits = 0;
}

CShanghaiSnapMan::~CShanghaiSnapMan()
{

}

bool CShanghaiSnapMan::Initial(unsigned char channelNum)
{
	m_nMaxSnap = 3;
	m_nMaxTime = 2;
	if (NULL == m_pChnnStatus)
	{
		m_pChnnStatus = new int [channelNum];
		memset(m_pChnnStatus, 0, sizeof(int)*channelNum);
	}

	if (NULL == m_pChnnCount)
	{
		m_pChnnCount = new int [channelNum];
		memset(m_pChnnCount, 0, sizeof(int)*channelNum);
	}

	if (NULL == m_pLastTime)
	{
		m_pLastTime = new LONGLONG [channelNum];
		memset(m_pLastTime, 0, sizeof(LONGLONG)*channelNum);
	}

	if (NULL == m_pChannType)
	{
		m_pChannType = new unsigned long[channelNum];
		memset(m_pChannType, 0, sizeof(unsigned long)*channelNum);
	}

	if(NULL == m_pAlarmPrevTime)
	{
		m_pAlarmPrevTime = new LONGLONG[channelNum];
		memset(m_pAlarmPrevTime, 0, sizeof(LONGLONG)*channelNum);
	}

	if(NULL == m_snapcount)
	{
		m_snapcount = new unsigned long[channelNum];
		memset(m_snapcount,0,sizeof(unsigned long)*channelNum);
	}

	if(NULL == m_pScreenSnapKey)
	{
		m_pScreenSnapKey = new unsigned long[channelNum];
		memset(m_pScreenSnapKey,0,sizeof(unsigned long)*channelNum);
	}

	if(m_preSec != NULL)
	{
		delete []m_preSec;
		m_preSec = NULL;
	}

	if(m_postSec != NULL)
	{
		delete []m_postSec;
		m_postSec = NULL;
	}

	if(m_startSnapTime != NULL)
	{
		delete []m_startSnapTime;
		m_startSnapTime = NULL;
	}

	if(NULL != m_Alarmtype)
	{
		delete []m_Alarmtype;
		m_Alarmtype = NULL;
	}

	m_preSec = new unsigned long[channelNum];
	memset(m_preSec,0,sizeof(unsigned long)*channelNum);

	m_postSec = new unsigned long[channelNum];
	memset(m_postSec,0,sizeof(unsigned long)*channelNum);

	m_startSnapTime = new unsigned long[channelNum];
	memset(m_startSnapTime,0,sizeof(unsigned long)*channelNum);

	m_Alarmtype = new unsigned long[channelNum];
	memset(m_Alarmtype,0,sizeof(unsigned long)*channelNum);

	m_maxchnn = channelNum;
	return true;
}

void CShanghaiSnapMan::GetDailyCfg(const SNAP_INFOEX *pData,int count)
{
	int j =0;
	int num =0;
	assert(pData != NULL);
	if(NULL != m_pData)
	{
		delete []m_pData;
		m_pData = NULL;
	}
	
	for(int i=0;i<MAX_DAILY_SNAP_NUM;++i)
	{
		if(1 == pData[i].bEnable)
		{
			num++;
		}
	}
	m_count = num;

	if(0 != m_count)
	{
		m_pData = new SNAP_INFOEX[m_count];
		for(int i=0;i<MAX_DAILY_SNAP_NUM;++i)
		{
			if(1 == pData[i].bEnable)
			{
				m_pData[j] = pData[i];
				j++;
			}
		}

	}
	
	if(0 != m_count)
	{
		if(NULL != m_repaetChnnBits)
		{
			delete []m_repaetChnnBits;
			m_repaetChnnBits = NULL;
		}
		
		m_repaetChnnBits = new ULONGLONG [m_count];
		memset(m_repaetChnnBits,0,sizeof(ULONGLONG)*m_count);

	}

}

void CShanghaiSnapMan::Quit()
{
	if(m_pData != NULL)
	{
		delete []m_pData;
		m_pData = NULL;
	}
	if (m_pChnnStatus != NULL)
	{
		delete [] m_pChnnStatus;
		m_pChnnStatus = NULL;
	}

	if (m_pChnnCount != NULL)
	{
		delete [] m_pChnnCount;
		m_pChnnCount = NULL;
	}

	if (m_pLastTime != NULL)
	{
		delete [] m_pLastTime;
		m_pLastTime = NULL;
	}

	if (NULL != m_pChannType)
	{
		delete []m_pChannType;
		m_pChannType = NULL;
	}

	if(m_preSec != NULL)
	{
		delete []m_preSec;
		m_preSec = NULL;
	}

	if(m_postSec != NULL)
	{
		delete []m_postSec;
		m_postSec = NULL;
	}

	if(m_startSnapTime != NULL)
	{
		delete []m_startSnapTime;
		m_startSnapTime = NULL;
	}

	if(NULL != m_Alarmtype)
	{
		delete []m_Alarmtype;
		m_Alarmtype = NULL;
	}

	if(NULL != m_pAlarmPrevTime)
	{
		delete []m_pAlarmPrevTime;
		m_pAlarmPrevTime = NULL;
	}

	if(NULL != m_snapcount)
	{
		delete []m_snapcount;
		m_snapcount = NULL;
	}

	if(NULL != m_repaetChnnBits)
	{
		delete []m_repaetChnnBits;
		m_repaetChnnBits = NULL;
	}

	if(NULL != m_pScreenSnapKey)
	{
		delete []m_pScreenSnapKey;
		m_pScreenSnapKey = NULL;
	}
	for(int i=0;i<32;i++)
	{
		if(0!=m_list[i].size())
		{
			for(list<PRE_SNAP_INFO>::iterator it = m_list[i].begin();it != m_list[i].end();++it)
			{
				delete []it->pData;
				it->pData = NULL;
			}
			m_list[i].clear();
		}
		
	}
}

void CShanghaiSnapMan::SetScreenSnapParam(unsigned long chnn)
{
	m_pScreenSnapKey[chnn] = 1;
}

void CShanghaiSnapMan::SetManualParam(ULONGLONG chnnBits,ULONGLONG snapTime,unsigned long imagetype)
{
	m_chnnBits = chnnBits;
	m_manualSnapTime = snapTime;
	m_bmanualSnap = true;
	m_manualImageType = imagetype;
}

bool CShanghaiSnapMan::ShanghaiDailySnapPicture(void *buf, int nLen, int nChannel, LONGLONG time,unsigned short width, unsigned short height )
{
	if(0 == m_count)
	{
		return false;
	}

	if(NULL == m_pData)
	{
		return false;
	}
	unsigned long recType = RECORD_TYPE_NULL;
	//���Ȳ鿴��ȡ��ͼƬ��ʱ���ǲ����ڴ������õ�ʱ�䷶Χ��
	for(int i=0;i<m_count;++i)
	{
		if( (1 == m_pData[i].bEnable)&&(true == CmpDailyTime(m_pData[i].startTime,time)) )
		{
			if(m_pData[i].chnnbit & (0x01<<nChannel))//�ҵ���Ӧͨ��
			{
				//�����ǲ����ظ���
				if(NULL != m_repaetChnnBits)
				{
					if(m_repaetChnnBits[i] &(0x01<<nChannel))//�ظ�
					{
						//printf("found repeat nChannel=%d,m_repaetChnnBits[i]=%x\n",nChannel,m_repaetChnnBits[i]);
						continue;
					}
				}
				recType = CMessageMan::Instance()->GetRecStatus(nChannel);
				
				if(RECORD_TYPE_NULL != recType)
				{
					//�����ճ�ץͼ�������ظ�ֵ��������
					CreateoperInfo(buf,nLen,nChannel,time,false);
					if(NULL != m_repaetChnnBits)
					{
						if(m_repaetChnnBits[i] != m_pData[i].chnnbit)
						{
							m_repaetChnnBits[i] |= (0x01<<nChannel);
						}
					//	printf("266 rec daily nChannel=%d,m_repaetChnnBits[i]=%x\n",nChannel,m_repaetChnnBits[i]);
					}
					
				}
				
			}
		}
		else
		{
			m_repaetChnnBits[i] = 0;
		}

	}
	return true;
}

void CShanghaiSnapMan::ShanghaiManualSnapPicture(void *buf, int nLen, int nChannel, LONGLONG time,unsigned short width, unsigned short height)
{
	//�鿴��õ�ͼƬ�ǲ�����chnnbits����
	
		if((m_chnnBits & (0x01<<nChannel))&&(m_bmanualSnap))
		{
			if((time - m_manualSnapTime >= 1000000)&&(time - m_manualSnapTime <= 6000000))
			{
				CreateoperInfo(buf,nLen,nChannel,time,true);
				m_chnnBits &= ~(0x01<<nChannel);//ÿ�λ�ö�Ӧͨ����ͼ�񣬴�chnnBits�аѶ�Ӧͨ��ȥ��
				if(0 == m_chnnBits)
				{
					m_bmanualSnap =false;
				}
			}
		}
}

void CShanghaiSnapMan::ShanghaiOnDemandSnapPicture(void *buf, int nLen, int nChannel, LONGLONG time)
{
	m_OnDemandSnapTime = CShanghaiConnectMan::Instance()->GetTriggerTime();
	m_OnDemandChnnBits = CShanghaiConnectMan::Instance()->GetChannelBits();
	if (m_OnDemandChnnBits & (0x01<<nChannel))
	{
		LONGLONG timeDiff = time-m_OnDemandSnapTime;
		if (timeDiff >= 0 && timeDiff <= 1000000)
		{
			char NoteText[256] = {0};
			unsigned long type = ONDEMANDUPLOAD_IMAGE;
			strcpy(NoteText, "GB2312:��������");
			CShanghaiConnectMan::Instance()->GetLogCmd(type, nChannel+1, NoteText, strlen(NoteText), (char *)buf, nLen, time);
		}
	}
}

bool CShanghaiSnapMan::SetSnapPicture(unsigned long chnnl, unsigned long type)
{
	m_lock.Lock();

	if (m_pChannType[chnnl] != 0)
	{
		m_lock.UnLock();
		return true;
	}

	m_pChannType[chnnl] = type;

	if (!m_pChnnStatus[chnnl])
	{
		m_pChnnStatus[chnnl] = 1;
		m_nCurSnapNum = m_nMaxSnap;
	}
	//���ô�ץͼͨ����
	{
		CSnapCapabilityMan::Instance()->StarSnapJpeg(chnnl, false);

	}
	m_lock.UnLock();
	return true;
}

bool CShanghaiSnapMan::ShanghaiAlarmManualSnapPicture(void *buf, int nLen, int nChannel, LONGLONG time , unsigned short width, unsigned short height, bool bLock)
{
	m_lock.Lock();
	bool bNeedHead = false;
	bool bPTEnable = false;
	if (!m_pChnnStatus[nChannel])
	{
		//if(time-m_pAlarmPrevTime[nChannel] >= (((ULONGLONG)m_nMaxTime-1)*1000000))
		if(time-m_pAlarmPrevTime[nChannel] >= (((ULONGLONG)m_nMaxTime)*1000000))
		{
			InsertPreSnapList(buf,nLen,nChannel,time,width,height,bLock);
			m_pAlarmPrevTime[nChannel] = time;
		}
		m_lock.UnLock();
		return true;
	}

	bool bAlarmSnap =false;
	if(m_pChannType[nChannel])//����type�������Ƿ񱣴浽Ӳ��
	{
		if(0 == m_startSnapTime[nChannel])//�ֶ�ץͼ�����뻺��
		{
			if (time <= m_pLastTime[nChannel])
			{
				//�����ֶ�ץͼ
				CSnapMan::GetInstance()->SaveSnapPicture(buf,nLen,m_pChannType[nChannel],nChannel,time,width,height,bLock);
				m_pChnnCount[nChannel]++;
				m_pLastTime[nChannel] = time;
			}
			else if(((time-m_pAlarmPrevTime[nChannel])/1000000) >= m_nMaxTime)
			{
				//�����ֶ�ץͼ
				CSnapMan::GetInstance()->SaveSnapPicture(buf,nLen,m_pChannType[nChannel],nChannel,time,width,height,bLock);
				m_pChnnCount[nChannel]++;
				m_pLastTime[nChannel] = time;
			}
		}
		else//��������
		{
			if (time <= m_pAlarmPrevTime[nChannel])
			{
				bAlarmSnap = true;
				m_pAlarmPrevTime[nChannel] = time;
			}
			else  if(((time-m_pAlarmPrevTime[nChannel])/1000000) >= m_nMaxTime)
			{
				bAlarmSnap = true;
				m_pAlarmPrevTime[nChannel] = time;
			}
			else
			{
				bAlarmSnap = false;
			}
			
		}

		if (bAlarmSnap)
		{
			//��������
			//������һ�ű��浽Ӳ��
			if(1 == m_pScreenSnapKey[nChannel])
			{
				//�����ֶ�ץͼ
				m_pChnnCount[nChannel]++;
				CSnapMan::GetInstance()->SaveSnapPicture(buf,nLen,m_pChannType[nChannel],nChannel,time,width,height,bLock);
			}
			bNeedHead = false;
			bPTEnable = false;
			bPTEnable = CShanghaiConnectMan::Instance()->GetPTEnable();
			if(bPTEnable)//ƽ̨�Ѿ�����
			{
				switch(m_Alarmtype[nChannel])
				{
				case DVRSYSMOTION://�ƶ����
					{
						//�����ж���Ҫ����Ҫͷ
						bNeedHead = CShanghaiConnectMan::Instance()->GetMotionHead();
						if(bNeedHead)
						{
							CShanghaiConnectMan::Instance()->SetMotionHead(true);
							uuid_clear(m_theoneMotion);
							uuid_generate_time(m_theoneMotion);
							CreateAlarmtitleInfo(m_theoneMotion,GetCurrTime(),nChannel);
							//�Ѵ���������ʱ�䴫������ͼƬ�࣬���ͼƬ����40�ţ�����ʱ�䳬����������+�ӳ�ʱ��+5�룬Ҳ���ٷ���
							CShanghaiConnectMan::Instance()->SetMotionAlarmStartTime(GetCurrTime32());
						}

						if(m_startSnapTime[nChannel] != 0)
						{
							if(0 != m_list[nChannel].size())
							{
								for(list<PRE_SNAP_INFO>::iterator it = m_list[nChannel].begin();it != m_list[nChannel].end();++it)
								{
									m_snapcount[nChannel]++;
									//�ȷ���Ԥ��ץͼ��
									CreateAlarmoperInfo(it->pData,it->nLen,it->chnn,it->time,m_theoneMotion);
									delete []it->pData;
									it->pData = NULL;
								}
								m_list[nChannel].clear();
							}
							CreateAlarmoperInfo(buf,nLen,nChannel,time,m_theoneMotion);
							m_snapcount[nChannel]++;
						}
					}
					break;
				case DVROUTSIDEALARM:
					{
						//�����ж���Ҫ����Ҫͷ
						bNeedHead = CShanghaiConnectMan::Instance()->GetSensorHead();
						if(bNeedHead)
						{
							CShanghaiConnectMan::Instance()->SetSensorHead(true);
							uuid_clear(m_theoneSensor);
							uuid_generate_time(m_theoneSensor);
							CreateAlarmtitleInfo(m_theoneSensor,GetCurrTime(),nChannel);
							//�Ѵ���������ʱ�䴫������ͼƬ�࣬���ͼƬ����40�ţ�����ʱ�䳬����������+�ӳ�ʱ��+5�룬Ҳ���ٷ���
							CShanghaiConnectMan::Instance()->SetSensorAlarmStartTime(GetCurrTime32());
						}
						
						if(m_startSnapTime[nChannel] != 0)
						{
							if(0 != m_list[nChannel].size())
							{
								for(list<PRE_SNAP_INFO>::iterator it = m_list[nChannel].begin();it != m_list[nChannel].end();++it)
								{
									//�ȷ���Ԥ��ץͼ������
									m_snapcount[nChannel]++;
									CreateAlarmoperInfo(it->pData,it->nLen,it->chnn,it->time,m_theoneSensor);
									delete []it->pData;
									it->pData = NULL;
								}
								m_list[nChannel].clear();
							}
							CreateAlarmoperInfo(buf,nLen,nChannel,time,m_theoneSensor);
							m_snapcount[nChannel]++;
						}
	
					}
					break;
				default:
					break;
				}
			}
			
		}
			
		
	}
		
	//��ɺ��ж���һ������ͼ��������
	if(m_startSnapTime[nChannel] != 0)//˵���Ǳ�������
	{
		if(1 == m_pScreenSnapKey[nChannel])//����ʱ����ֶ���ͼ
		{
			if (m_pChnnCount[nChannel] >= m_nCurSnapNum)
			{
				m_pScreenSnapKey[nChannel] = 0;//������Χ���ٱ��浽����
				m_pChnnCount[nChannel] = 0;
			}
		}
		unsigned long curTime = GetCurrTime32();
		//�˳���ͼ�������1��ͼƬ���������ͼƬ��+3,2,ʱ�䳬����Χ
		if( (m_snapcount[nChannel] >= GetChnnMAXSnap(nChannel)) || ((curTime- m_startSnapTime[nChannel]) > m_postSec[nChannel]))
		{
			m_pChnnStatus[nChannel] = 0;
			m_pChannType[nChannel] = 0;
			m_pAlarmPrevTime[nChannel] = 0;
			m_startSnapTime[nChannel] = 0;
			m_snapcount[nChannel] = 0;
		}
	}
	else//�ֶ�ץͼ����,
	{
		if (m_pChnnCount[nChannel] >= m_nCurSnapNum)
		{
			m_pChnnStatus[nChannel] = 0;
			m_pChnnCount[nChannel] = 0;
			m_pChannType[nChannel] = 0;
			m_pLastTime[nChannel] = 0;
		}

	}
	m_lock.UnLock();
	return true;
}

bool CShanghaiSnapMan::CmpDailyTime(unsigned long cfgTime,ULONGLONG time)
{
	tm tmpCfgTm = DVRTime32ToTm(cfgTime);
	tm getTm	= DVRTimeToTm(time);

	if((tmpCfgTm.tm_hour == getTm.tm_hour) && (tmpCfgTm.tm_min == getTm.tm_min) && (getTm.tm_sec -tmpCfgTm.tm_sec <= 1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CShanghaiSnapMan::GetPreAlarmCfg(const unsigned long *preSec)
{
	memcpy(m_preSec,preSec,sizeof(unsigned long)*m_maxchnn);
}

void CShanghaiSnapMan::GetPostAlarmCfg(const unsigned long *pPostSec)
{
	memcpy(m_postSec,pPostSec,sizeof(unsigned long)*m_maxchnn);
}

void CShanghaiSnapMan::GetAlarmTime(unsigned long chnn)
{
	m_startSnapTime[chnn] = GetCurrTime32();
	
}

void CShanghaiSnapMan::SetSnapTimeS(const unsigned long timeS)
{
	m_lock.Lock();
	m_nMaxTime = timeS;
	m_lock.UnLock();
}

void CShanghaiSnapMan::SetSnapImgNum(const unsigned long imgNum)
{
	m_lock.Lock();

	m_nMaxSnap = imgNum;
	m_lock.UnLock();
}

unsigned long CShanghaiSnapMan::GetSnapTime()
{
	m_lock.Lock();
	if ((0 == m_nMaxTime) || (0 == m_nMaxSnap))
	{
		m_lock.UnLock();
		return 0;
	}

	unsigned long retTime = m_nMaxTime * m_nMaxSnap;

	if (retTime > 20)
	{
		retTime = 20;
	}

	m_lock.UnLock();
	return retTime;

}

void CShanghaiSnapMan::GetAlarmType(unsigned long alarmType,unsigned long chnn)
{
	m_Alarmtype[chnn] = alarmType;
}

void CShanghaiSnapMan::SetCheckPiliceNum(const char *pNote)
{
	memset(m_policeNote,0,sizeof(m_policeNote));
	if(0 == strlen(pNote))
	{
		string outStr;
		outStr.clear();
		if (-1 == CCharCodeTable::Instance()->Convert(outStr, CCT_CODE_UTF8, "���Ų���Ϊ��", CCT_CODE_GBK))
		{
			outStr = "���Ų���Ϊ��";
		}
		
		//MessageBox(outStr, CStringTable::Instance()->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);	
	}
	else
	{
		strcpy(m_policeNote,pNote);
	}
}

void CShanghaiSnapMan::InsertPreSnapList(void *buf, int nLen, int nChannel, LONGLONG time,unsigned short width,unsigned short height,unsigned short block)
{
	//���Ȳ���list�ĸ���
	PRE_SNAP_INFO preInfo;
	int count = m_list[nChannel].size();
	memset(&preInfo,0,sizeof(PRE_SNAP_INFO));
	preInfo.pData = new unsigned char[nLen];
	memcpy(preInfo.pData,buf,nLen);
	preInfo.nLen = nLen;
	preInfo.chnn = nChannel;
	preInfo.time = time;
	preInfo.bLock = block;
	preInfo.width = width;
	preInfo.height = height;
	preInfo.type = 1;

	if(0 == count)
	{
		m_list[nChannel].push_back(preInfo);
	}
	else
	{
		int picNum = 0;

		list<PRE_SNAP_INFO>::iterator it = m_list[nChannel].begin();
		if((preInfo.time - it->time) <= (ULONGLONG)(m_preSec[nChannel])*1000000)
		{
			m_list[nChannel].push_back(preInfo);
		}
		else
		{
			//ɾ���б��һ��
			delete []it->pData;
			it->pData = NULL;
			m_list[nChannel].pop_front();

			m_list[nChannel].push_back(preInfo);
		}
	}

}

void CShanghaiSnapMan::CreateoperInfo(void *buf, int nLen, int nChannel, LONGLONG time,bool isbManual)
{
	unsigned long type = 0;
	char NoteText[256]={0};
	if(isbManual)
	{
		switch(m_manualImageType)
		{
		case KEEPUPLOAD_IMAGE:
			{
				type = KEEPUPLOAD_IMAGE;
				strcpy(NoteText,"GB2312:����ͼ���ϴ�");
			}
			break;
		case TESTUPLOAD_IMAGE:
			{
				type = TESTUPLOAD_IMAGE;
				strcpy(NoteText,"GB2312:����ͼ���ϴ�");
			}
			break;
		case CHECKUPLOAD_IMAGE:
			{
				type = CHECKUPLOAD_IMAGE;
				if(0 != strlen(m_policeNote))
				{
					snprintf(NoteText,256,"GB2312:����ͼ���ϴ�,����:%s",m_policeNote);
				}
				else
				{
					snprintf(NoteText,256,"GB2312:����ͼ���ϴ�,����Ϊ��");
				}
			}
			break;
		default:
			break;
		}
	}
	else
	{
		type = DAILYUPLOAD_IMAGE;
		strcpy(NoteText,"GB2312:�ճ�ͼ���ϴ�");
	}
	CShanghaiConnectMan::Instance()->GetLogCmd(type,nChannel+1,NoteText,strlen(NoteText),(char *)buf,nLen,time);
}

void CShanghaiSnapMan::CreateAlarmoperInfo(void *buf, int nLen, int nChannel, LONGLONG time,uuid_t theOneUUid)
{
	char alarmNote[256]={0};
	OPER_INFO info;
	char tmpTime[256] = {0};
	tm curTm;

	//Ȼ���ʹ�ͼƬ����Ϣ
	info.operType = ALARMUPLOAD_IMAGE;
	info.chnn = nChannel+1;
	curTm= DVRTimeToTm(time);

	memset(tmpTime,0,sizeof(tmpTime));
	snprintf(tmpTime,256,"%d-%d-%d %d:%d:%d",curTm.tm_year+1900,curTm.tm_mon+1,curTm.tm_mday,curTm.tm_hour,curTm.tm_min,curTm.tm_sec);
	strcpy(info.triggerTime,tmpTime);
	////����ͼƬʱ�򳤶����ޣ�ȥ��note����
	info.textLen =0;
	info.pNote = NULL;
	
	//��ͼƬ����ת��
	{
		int picLen=0;
		char *picbuf = new char[nLen*2];
		memset(picbuf,0,nLen*2);
		picLen = CShanghaiConnectMan::Instance()->to64frombits((unsigned char *)picbuf,(unsigned char *)buf,nLen);
		if(0 != picLen)
		{
			info.picLen = picLen;
			info.pPic = new unsigned char[picLen];
			memcpy(info.pPic,picbuf,picLen);
		}
		else
		{
			assert(false);
			info.picLen =0;
			info.pPic = NULL;

		}
		if(NULL != picbuf)
		{
			delete []picbuf;
			picbuf = NULL;
		}

	}

	uuid_copy(info.uniqueGUID,theOneUUid);
	
	switch(m_Alarmtype[nChannel])
	{
	case DVRSYSMOTION:
		{
			CShanghaiConnectMan::Instance()->GetMotionAlarmSnap(info);
		}
		break;
	case DVROUTSIDEALARM:
		{
			CShanghaiConnectMan::Instance()->GetSensorAlarmSnap(info);
		}
		break;
	default:
		break;
	}
	
	if(NULL != info.pPic)
	{
		delete []info.pPic;
		info.pPic = NULL;
	}
	if(NULL != info.pNote)
	{
		delete []info.pNote;
		info.pNote = NULL;
	}
}

void CShanghaiSnapMan::CreateAlarmtitleInfo(uuid_t theOneUUid,LONGLONG time,unsigned long chnn)
{
	//��������ʱ���ȷ���ûͼƬֻ�����ֵ���Ϣ
	char noteText[256]={0};
	
	OPER_INFO info;
	char tmpTime[256] = {0};
	tm curTm;
	memset(&info,0,sizeof(OPER_INFO));

	info.operType = ALARMUPLOAD_IMAGE;
	info.chnn = chnn+1;

	curTm= DVRTimeToTm(time);
	memset(tmpTime,0,sizeof(tmpTime));
	snprintf(tmpTime,256,"%d-%d-%d %d:%d:%d",curTm.tm_year+1900,curTm.tm_mon+1,curTm.tm_mday,curTm.tm_hour,curTm.tm_min,curTm.tm_sec);
	strcpy(info.triggerTime,tmpTime);

	switch(m_Alarmtype[chnn])
	{
	case DVRSYSMOTION:
		{
			memset(noteText,0,sizeof(noteText));
			strcpy(noteText,"GB2312:�ƶ����ͼ��");
		}
		break;
	case DVROUTSIDEALARM:
		{
			memset(noteText,0,sizeof(noteText));
			strcpy(noteText,"GB2312:�ⲿ����ͼ��");
		}
		break;
	default:
		break;
	}
	//�����ֽ���ת��
	{
		int len=0;
		char tmpbuf[maxpicLen]={0};
		len = CShanghaiConnectMan::Instance()->to64frombits((unsigned char *)tmpbuf,(unsigned char *)noteText,strlen(noteText));
		if(0 != len)
		{
			info.pNote = new unsigned char[len];
			memcpy(info.pNote,tmpbuf,len);
			info.textLen = len;
		}
		else
		{
			assert(false);
			info.textLen =0;
			info.pNote = NULL;
		}

	}
	info.picLen =0;
	info.pPic = NULL;
	uuid_copy(info.uniqueGUID,theOneUUid);

	switch(m_Alarmtype[chnn])
	{
	case DVRSYSMOTION:
		{
			CShanghaiConnectMan::Instance()->GetMotionAlarmSnap(info);
		}
		break;
	case DVROUTSIDEALARM:
		{
			CShanghaiConnectMan::Instance()->GetSensorAlarmSnap(info);
		}
		break;
	default:
		break;
	}

	if(NULL != info.pNote)
	{
		delete []info.pNote;
		info.pNote = NULL;
	}

}

unsigned long  CShanghaiSnapMan::GetChnnMAXSnap(unsigned long chnn)
{
	unsigned long totalSnapNum = 0;
	totalSnapNum = (m_preSec[chnn]+m_postSec[chnn])/m_nMaxTime * m_nMaxSnap +3;//���3�ţ�
	return totalSnapNum;
}
//end
