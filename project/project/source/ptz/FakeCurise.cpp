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
		//停止巡航
		StopFakeCurise(chnn);

		//删除巡航信息
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
	//除了调用巡航的命令是假巡航特有的，其他的巡航命令都需要真假巡航时都执行。
	//这包括，进入巡航、设置巡航、退出巡航设置、删除巡航、停止巡航。这保证从模拟巡航<-->真实都能执行
	bool bRetVal = true;

	//是假巡航时只处理巡航相关的命令
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

				//查看改通道预置点停留时间是否到期，首次调用该条巡航线时必须到期
				if (GetCurrTime32() > m_chnnCuriseInfo[chnn].nextRunPointTime)
				{
					//停留时间到期并不表示真的到期。向后修改时间会造成这种结果发生
					if ((0 != m_chnnCuriseInfo[chnn].nextRunPointTime) && (MAX_DWELL_TIME < (GetCurrTime32() - m_chnnCuriseInfo[chnn].nextRunPointTime)))
					{
						//从新计算等待时间
						unsigned long prePos = GetPreValidPresetPos(chnn, curRunCurise, curRunPoint);
						m_chnnCuriseInfo[chnn].nextRunPointTime = GetCurrTime32() + m_chnnCuriseInfo[chnn].chnnCurisePoint[curRunCurise][prePos].dwellTime;
						PUB_Sleep(60);
						continue;
					}

					//调用置点到PTZ
					if (NULL != m_pPtzProtocl[chnn])
					{
						//掉用云台的预置点
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

					//计算调用下一个预置点的时间
					m_chnnCuriseInfo[chnn].nextRunPointTime = GetCurrTime32() + sleepTime;
					
					//切换到下一个与置点
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

	//配置与置点信息
	m_chnnCuriseInfo[channel].chnnCurisePoint[curSetCurise][curSetPoint].dwellSpeed	= speed;
	m_chnnCuriseInfo[channel].chnnCurisePoint[curSetCurise][curSetPoint].dwellTime	= waitTime;
	m_chnnCuriseInfo[channel].chnnCurisePoint[curSetCurise][curSetPoint].preset		= ucPresetID;

	//设置下一个要配置的与置点位置
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

	//清除后续与置点
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

	//当前不能正在运行巡航线
	if ((MAX_CRUISE_NUM >m_chnnCuriseInfo[channel].curRunCurise) || (ChnnCuriseIsRunning(channel)))
	{
		printf("%s:%s:%d, Channel %d is running curise %d\n", __FUNCTION__, __FILE__, __LINE__, channel, m_chnnCuriseInfo[channel].curRunCurise);
		assert(false);
		return false;
	}

	//设置当前运行的虚拟巡航线
	m_chnnCuriseInfo[channel].curRunCurise	= uCruiseId;

	//设置第一个调用的与置点
	m_chnnCuriseInfo[channel].curRunPoint	= 0;

	//初始化第一个与置点运行的时间
	m_chnnCuriseInfo[channel].nextRunPointTime = 0;

	//标记该通道开始巡航
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

	//标记该通道停止巡航
	SetChnnCuriseStatus(channel, false);

	//清除运行下一个与置点的时间
	m_chnnCuriseInfo[channel].nextRunPointTime	= 0x7fffffff;

	//清除当前调用的预置点
	m_chnnCuriseInfo[channel].curRunPoint		= 0xffffffff;

	//清除当前调用的巡航线
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


