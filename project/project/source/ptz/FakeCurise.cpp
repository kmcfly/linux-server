//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "FakeCurise.h"

CFakeCurise * CFakeCurise::Instance()
{
	static CFakeCurise fakeCurise;
	return &fakeCurise;
}

CFakeCurise::CFakeCurise()
{
	m_fakeCuriseProcID = PUB_THREAD_ID_NOINIT;
	m_bFakeCuriseProc = false;

	memset(m_ISFakeCurise, 0, sizeof(m_ISFakeCurise));
	m_channelNum = 0;
	memset(&m_curRunCuriseChnn, 0, sizeof(ULONGLONG));
	memset(m_chnnCuriseInfo, 0, sizeof(m_chnnCuriseInfo));

	memset(m_ptzSerialInfo, 0, sizeof(m_ptzSerialInfo));
	memset(m_pPtzProtocl, 0, sizeof(m_pPtzProtocl));
}

CFakeCurise::~CFakeCurise()
{

}

bool CFakeCurise::Initial(unsigned long channelNum)
{
	m_channelNum = channelNum;

	for (unsigned long chnn = 0; chnn < m_channelNum; chnn++)
	{
		//ֹͣѲ��
		StopFakeCurise(chnn);

		//ɾ��Ѳ����Ϣ
		for (BYTE curise = 0; curise < MAX_CRUISE_NUM; curise++)
		{
			DelFakeCruise(chnn, curise);
		}
	}

	return true;
}

void CFakeCurise::Quit()
{

}

bool CFakeCurise::Start()
{
	m_fakeCuriseProcID = PUB_CreateThread( FakeCuriseThread, (void *)this, &m_bFakeCuriseProc );	
	if( PUB_CREATE_THREAD_FAIL == m_fakeCuriseProcID )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	return true;
}
void CFakeCurise::Stop()
{
	if ( PUB_THREAD_ID_NOINIT != m_fakeCuriseProcID )
	{		
		PUB_ExitThread( &m_fakeCuriseProcID, &m_bFakeCuriseProc );	
	}
}

bool CFakeCurise::PreAction(unsigned long channel, CPTZProtocol *pPTZProtocol, unsigned char cmdtype, unsigned long ptzParam)
{
	//���˵���Ѳ���������Ǽ�Ѳ�����еģ�������Ѳ�������Ҫ���Ѳ��ʱ��ִ�С�
	//�����������Ѳ��������Ѳ�����˳�Ѳ�����á�ɾ��Ѳ����ֹͣѲ�����Ᵽ֤��ģ��Ѳ��<-->��ʵ����ִ��
	bool bRetVal = true;

	//�Ǽ�Ѳ��ʱֻ����Ѳ����ص�����
	if ((NULL != pPTZProtocol) && (m_pPtzProtocl[channel] != pPTZProtocol))
	{
		m_pPtzProtocl[channel] = pPTZProtocol;
	}

	switch (cmdtype)
	{
	case PTZ_CMD_ENTER_CURISE_MODE:
		{
			EnterFakeCuriseMode(channel, ptzParam);

			bRetVal = false;
		}
		break;
	case PTZ_CMD_CRUISE_SET:
		{
			PTZ_CURISE_POINT *pPTZCurisePoint = (PTZ_CURISE_POINT*)ptzParam;
			if (NULL != pPTZCurisePoint)
			{
				FakeCuriseSet(channel, pPTZCurisePoint->preset, pPTZCurisePoint->dwellSpeed, pPTZCurisePoint->dwellTime);
			}
			else
			{
				printf("%s:%s:%d, The set curise preset = NULL\n", __FUNCTION__, __FILE__, __LINE__);
				assert(false);
			}

			bRetVal = false;
		}
		break;
	case PTZ_CMD_LEAVE_CURISE_MODE:
		{
			LeaveFakeCuriseMode(channel);

			bRetVal = false;
		}
		break;
	case PTZ_CMD_CRUISE_RUN:
		{
			if (IsFakeCurise(channel))
			{
				RunFakeCurise(channel, ptzParam);
			}
			else
			{
				bRetVal = false;
			}
		}
		break;
	case PTZ_CMD_CRUISE_STOP:
		{
			printf("%s:%s:%d, Stop the curise, channel=%d\n", __FUNCTION__, __FILE__, __LINE__, channel);
			StopFakeCurise(channel);

			bRetVal = false;
		}
		break;
	case PTZ_CMD_CRUISE_DEL:
		{
			DelFakeCruise(channel, ptzParam);

			bRetVal = false;
		}
		break;
	default:
		{
			bRetVal = false;
		}
		break;
	}

	return bRetVal;
}

bool CFakeCurise::InitChnnFakeCurise(unsigned long channel, const PTZ_CURISE* pCurise, const PTZ_CURISE_POINT* pCurisePt)
{
	if (channel >= m_channelNum)
	{
		printf("%s:%s:%d, The control channel = %d is over the device max channel %d\n", __FUNCTION__, __FILE__, __LINE__, channel, m_channelNum);
		assert(false);
		return false;
	}

	if (NULL == pCurise)
	{
		printf("%s:%s:%d, The pCurise=NULL\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return false;
	}

	if ((0 < pCurise->presetNum) && (NULL == pCurisePt))
	{
		printf("%s:%s:%d, pCurise->presetNum = %d, pCurisePt = %p\n", __FUNCTION__, __FILE__, __LINE__, pCurise->presetNum, pCurisePt);
		assert(false);
		return false;
	}
	else
	{
		unsigned long maxPresetNum = (pCurise->presetNum > MAX_CRUISE_PRESET_NUM) ? MAX_CRUISE_PRESET_NUM : pCurise->presetNum;
		unsigned long presetPos = 0;

		for (presetPos = 0; presetPos < maxPresetNum; presetPos++)
		{
			m_chnnCuriseInfo[channel].chnnCurisePoint[pCurise->index][presetPos] = pCurisePt[presetPos];
		}

		for (; presetPos < MAX_CRUISE_PRESET_NUM; presetPos++)
		{
			m_chnnCuriseInfo[channel].chnnCurisePoint[pCurise->index][presetPos].preset = UNUSED_PRESET;
		}
	}

	return true;
}

void CFakeCurise::SetCuriseType(const unsigned long *pIsFakeCurise, unsigned long len)
{
	if (NULL != pIsFakeCurise)
	{
		m_lock.Lock();
		assert((len / sizeof(unsigned long)) == m_channelNum );
		if (len < sizeof(m_ISFakeCurise))
		{
			memcpy(m_ISFakeCurise, pIsFakeCurise, len);
		}
		else
		{
			memcpy(m_ISFakeCurise, pIsFakeCurise, sizeof(m_ISFakeCurise));
		}
		m_lock.UnLock();

		//for (int chnn = 0; chnn < (len / sizeof(unsigned long)); chnn++)
		//{
		//	printf("%s:%s:%d, channel %d fake curise = %d\n", __FUNCTION__, __FILE__, __LINE__, chnn, pIsFakeCurise[chnn]);
		//}
	}
	else
	{
		printf("%s:%s:%d, The pIsFakeCurise = NULL\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
	}
}

void CFakeCurise::SetSerialInfo(unsigned long channel, unsigned char enable, unsigned char addr, unsigned char protocol, unsigned char rate)
{
	if (channel >= m_channelNum)
	{
		printf("%s:%s:%d, The control channel = %d is over the device max channel %d\n", __FUNCTION__, __FILE__, __LINE__, channel, m_channelNum);
		assert(false);
		return;
	}
	
	m_ptzSerialInfo[channel].protocol	= protocol;
	m_ptzSerialInfo[channel].devAddr	= addr;
	m_ptzSerialInfo[channel].bEnable	= enable;
	m_ptzSerialInfo[channel].baudRate	= rate;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CFakeCurise::FakeCuriseThread(void *pParam)
{
#ifdef	__ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CFakeCurise *pFakeCurise = reinterpret_cast<CFakeCurise *>(pParam);
	pFakeCurise->FakeCuriseProc();
	return 0;
}
void CFakeCurise::FakeCuriseProc()
{
	while (m_bFakeCuriseProc)
	{
		for (unsigned long chnn = 0; chnn < m_channelNum; chnn++)
		{
			if (ChnnCuriseIsRunning(chnn))
			{
				unsigned long curRunCurise	= m_chnnCuriseInfo[chnn].curRunCurise;
				unsigned long curRunPoint	= m_chnnCuriseInfo[chnn].curRunPoint;
				unsigned long sleepTime		= m_chnnCuriseInfo[chnn].chnnCurisePoint[curRunCurise][curRunPoint].dwellTime;
				
				if (MAX_CRUISE_NUM <= curRunCurise)
				{
					printf("%s:%s:%d, the cur run curise %d is over the MAX_CRUISE_NUM=%d\n", __FUNCTION__, __FILE__, __LINE__, curRunCurise, MAX_CRUISE_NUM);
					assert(false);
					continue;
				}

				if (MAX_CRUISE_PRESET_NUM <= curRunPoint)
				{
					printf("%s:%s:%d, the cur run preset pos %d is over the MAX_CRUISE_PRESET_NUM=%d\n", __FUNCTION__, __FILE__, __LINE__, curRunPoint, MAX_CRUISE_PRESET_NUM);
					assert(false);
					continue;
				}

				//�鿴��ͨ��Ԥ�õ�ͣ��ʱ���Ƿ��ڣ��״ε��ø���Ѳ����ʱ���뵽��
				if (GetCurrTime32() > m_chnnCuriseInfo[chnn].nextRunPointTime)
				{
					//ͣ��ʱ�䵽�ڲ�����ʾ��ĵ��ڡ�����޸�ʱ���������ֽ������
					if ((0 != m_chnnCuriseInfo[chnn].nextRunPointTime) && (MAX_DWELL_TIME < (GetCurrTime32() - m_chnnCuriseInfo[chnn].nextRunPointTime)))
					{
						//���¼���ȴ�ʱ��
						unsigned long prePos = GetPreValidPresetPos(chnn, curRunCurise, curRunPoint);
						m_chnnCuriseInfo[chnn].nextRunPointTime = GetCurrTime32() + m_chnnCuriseInfo[chnn].chnnCurisePoint[curRunCurise][prePos].dwellTime;
						PUB_Sleep(60);
						continue;
					}

					//�����õ㵽PTZ
					if (NULL != m_pPtzProtocl[chnn])
					{
						//������̨��Ԥ�õ�
						//printf("%s:%s:%d, Run preset %d, baudRate=%d, dwellTime=%d, speed=%d, addr=%d\n", __FUNCTION__, __FILE__, __LINE__, 
						//		m_chnnCuriseInfo[chnn].chnnCurisePoint[curRunCurise][curRunPoint].preset, 
						//		m_ptzSerialInfo[chnn].baudRate, 
						//		m_chnnCuriseInfo[chnn].chnnCurisePoint[curRunCurise][curRunPoint].dwellTime,
						//		m_chnnCuriseInfo[chnn].chnnCurisePoint[curRunCurise][curRunPoint].dwellSpeed,
						//		m_ptzSerialInfo[chnn].devAddr);
						m_pPtzProtocl[chnn]->Command(PTZ_CMD_PRESET_GO, m_ptzSerialInfo[chnn].devAddr, m_ptzSerialInfo[chnn].baudRate, 
														m_chnnCuriseInfo[chnn].chnnCurisePoint[curRunCurise][curRunPoint].dwellSpeed, 
														m_chnnCuriseInfo[chnn].chnnCurisePoint[curRunCurise][curRunPoint].preset,
														m_chnnCuriseInfo[chnn].chnnCurisePoint[curRunCurise][curRunPoint].dwellTime);
					}
					else
					{
						printf("%s:%s:%d, Can not find the ptz protocol for preset go\n", __FUNCTION__, __FILE__, __LINE__);
						assert(false);
					}

					//���������һ��Ԥ�õ��ʱ��
					m_chnnCuriseInfo[chnn].nextRunPointTime = GetCurrTime32() + sleepTime;
					
					//�л�����һ�����õ�
					curRunPoint += 1;
					curRunPoint %= MAX_CRUISE_PRESET_NUM;
					if (IsUnUsedPreset(m_chnnCuriseInfo[chnn].chnnCurisePoint[curRunCurise][curRunPoint].preset))
					{
						curRunPoint = 0;
					}
					m_chnnCuriseInfo[chnn].curRunPoint = curRunPoint;
				}
				else
				{
					if (MAX_DWELL_TIME < (m_chnnCuriseInfo[chnn].nextRunPointTime - GetCurrTime32()))
					{
						unsigned long prePos = GetPreValidPresetPos(chnn, curRunCurise, curRunPoint);
						m_chnnCuriseInfo[chnn].nextRunPointTime = GetCurrTime32() + m_chnnCuriseInfo[chnn].chnnCurisePoint[curRunCurise][prePos].dwellTime;
					}
				}
			}

			PUB_Sleep(60);
		}

		PUB_Sleep(60);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFakeCurise::EnterFakeCuriseMode(unsigned long channel, BYTE uCruiseId)
{
	if (channel >= m_channelNum)
	{
		printf("%s:%s:%d, The control channel = %d is over the device max channel %d\n", __FUNCTION__, __FILE__, __LINE__, channel, m_channelNum);
		assert(false);
		return false;
	}

	if (MAX_CRUISE_NUM <= uCruiseId)
	{
		printf("%s:%s:%d, The delete curise id=%d, the max curise id is 7\n", __FUNCTION__, __FILE__, __LINE__, uCruiseId);
		assert(false);
		return false;
	}

	m_chnnCuriseInfo[channel].curSetCurise	= uCruiseId;
	m_chnnCuriseInfo[channel].curSetPoint	= 0;

	return true;
}

bool CFakeCurise::FakeCuriseSet(unsigned long channel, BYTE ucPresetID, BYTE speed, BYTE waitTime)
{
	if (channel >= m_channelNum)
	{
		printf("%s:%s:%d, The control channel = %d is over the device max channel %d\n", __FUNCTION__, __FILE__, __LINE__, channel, m_channelNum);
		assert(false);
		return false;
	}

	unsigned long curSetCurise	= m_chnnCuriseInfo[channel].curSetCurise;
	unsigned long curSetPoint	= m_chnnCuriseInfo[channel].curSetPoint;

	if ((MAX_CRUISE_NUM <= curSetCurise) || (MAX_CRUISE_PRESET_NUM <= curSetPoint))
	{
		printf("%s:%s:%d. The cur set curise=%d is over 8 OR the cur set point=%d is over 16\n", __FUNCTION__, __FILE__, __LINE__, curSetCurise, curSetPoint);
		return false;
	}

	//�������õ���Ϣ
	m_chnnCuriseInfo[channel].chnnCurisePoint[curSetCurise][curSetPoint].dwellSpeed	= speed;
	m_chnnCuriseInfo[channel].chnnCurisePoint[curSetCurise][curSetPoint].dwellTime	= waitTime;
	m_chnnCuriseInfo[channel].chnnCurisePoint[curSetCurise][curSetPoint].preset		= ucPresetID;

	//������һ��Ҫ���õ����õ�λ��
	curSetPoint += 1;
	m_chnnCuriseInfo[channel].curSetPoint = curSetPoint;

	return true;
}

bool CFakeCurise::LeaveFakeCuriseMode(unsigned long channel)
{
	if (channel >= m_channelNum)
	{
		printf("%s:%s:%d, The control channel = %d is over the device max channel %d\n", __FUNCTION__, __FILE__, __LINE__, channel, m_channelNum);
		assert(false);
		return false;
	}

	//����������õ�
	unsigned long curSetCurise	= m_chnnCuriseInfo[channel].curSetCurise;
	unsigned long curSetPoint	= m_chnnCuriseInfo[channel].curSetPoint;
	for (unsigned long presetPos = curSetPoint; presetPos < MAX_CRUISE_PRESET_NUM; presetPos++)
	{
		m_chnnCuriseInfo[channel].chnnCurisePoint[curSetCurise][presetPos].preset = UNUSED_PRESET;
	}

	m_chnnCuriseInfo[channel].curSetCurise	= 0xffffffff;
	m_chnnCuriseInfo[channel].curSetPoint	= 0xffffffff;

	return true;
}

bool CFakeCurise::RunFakeCurise(unsigned long channel, BYTE uCruiseId)
{
	if (channel >= m_channelNum)
	{
		printf("%s:%s:%d, The control channel = %d is over the device max channel %d\n", __FUNCTION__, __FILE__, __LINE__, channel, m_channelNum);
		assert(false);
		return false;
	}

	if (MAX_CRUISE_NUM <= uCruiseId)
	{
		printf("%s:%s:%d, The delete curise id=%d, the max curise id is 7\n", __FUNCTION__, __FILE__, __LINE__, uCruiseId);
		assert(false);
		return false;
	}

	//��ǰ������������Ѳ����
	if ((MAX_CRUISE_NUM >m_chnnCuriseInfo[channel].curRunCurise) || (ChnnCuriseIsRunning(channel)))
	{
		printf("%s:%s:%d, Channel %d is running curise %d\n", __FUNCTION__, __FILE__, __LINE__, channel, m_chnnCuriseInfo[channel].curRunCurise);
		assert(false);
		return false;
	}

	//���õ�ǰ���е�����Ѳ����
	m_chnnCuriseInfo[channel].curRunCurise	= uCruiseId;

	//���õ�һ�����õ����õ�
	m_chnnCuriseInfo[channel].curRunPoint	= 0;

	//��ʼ����һ�����õ����е�ʱ��
	m_chnnCuriseInfo[channel].nextRunPointTime = 0;

	//��Ǹ�ͨ����ʼѲ��
	SetChnnCuriseStatus(channel, true);
	
	return true;
}

bool CFakeCurise::StopFakeCurise(unsigned long channel)
{
	if (channel >= m_channelNum)
	{
		printf("%s:%s:%d, The control channel = %d is over the device max channel %d\n", __FUNCTION__, __FILE__, __LINE__, channel, m_channelNum);
		assert(false);
		return false;
	}

	//��Ǹ�ͨ��ֹͣѲ��
	SetChnnCuriseStatus(channel, false);

	//���������һ�����õ��ʱ��
	m_chnnCuriseInfo[channel].nextRunPointTime	= 0x7fffffff;

	//�����ǰ���õ�Ԥ�õ�
	m_chnnCuriseInfo[channel].curRunPoint		= 0xffffffff;

	//�����ǰ���õ�Ѳ����
	m_chnnCuriseInfo[channel].curRunCurise		 = 0xffffffff;

	return true;
}

bool CFakeCurise::DelFakeCruise(unsigned long channel, BYTE uCruiseId)
{
	if (channel >= m_channelNum)
	{
		printf("%s:%s:%d, The control channel = %d is over the device max channel %d\n", __FUNCTION__, __FILE__, __LINE__, channel, m_channelNum);
		assert(false);
		return false;
	}

	if (MAX_CRUISE_NUM <= uCruiseId)
	{
		printf("%s:%s:%d, The delete curise id=%d, the max curise id is 7\n", __FUNCTION__, __FILE__, __LINE__, uCruiseId);
		assert(false);
		return false;
	}

	for (unsigned long presetPos = 0; presetPos < MAX_CRUISE_PRESET_NUM; presetPos++)
	{
		m_chnnCuriseInfo[channel].chnnCurisePoint[uCruiseId][presetPos].preset = UNUSED_PRESET;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long CFakeCurise::GetPreValidPresetPos(unsigned long channel, unsigned long curiseID, unsigned long presetPos)
{
	unsigned long prePos = presetPos;
	if (0 < prePos)
	{
		return --prePos;
	}
	else
	{
		for ( prePos = 1; prePos < MAX_CRUISE_PRESET_NUM; prePos++)
		{
			if (IsUnUsedPreset(m_chnnCuriseInfo[channel].chnnCurisePoint[curiseID][prePos].preset))
			{
				return --prePos;
			}
		}
	}

	return --prePos;
}


