#include "RegNetDeviceLog.h"

CRegNetDeviceLog::CRegNetDeviceLog(void)
{
	m_logHeader.deviceTotalNum = 0;
	m_logHeader.deviceUsedNum = 0;
	m_pFileLogName = NULL;
	m_pNetDeviceInfo = NULL;
	m_clientID = INVALID_CLIENT_ID;

	m_bPoeUsedPtoPFunc = true;
}

CRegNetDeviceLog::~CRegNetDeviceLog(void)
{
}

CRegNetDeviceLog *CRegNetDeviceLog::Instance()
{
	static CRegNetDeviceLog s_regNetDeviceLog;
	return &s_regNetDeviceLog;
}

bool CRegNetDeviceLog::Initial(const char *pLogFilePath)
{
	m_pFileLogName = new char [512];
	memset(m_pFileLogName, 0, 512);
	if (NULL != m_pFileLogName)
	{
#ifdef __ENVIRONMENT_WIN32__
		sprintf(m_pFileLogName, "%s\\config\\RegNetDeviceLog.dat", pLogFilePath);
#else
		sprintf(m_pFileLogName, "%s/config/RegNetDeviceLog.dat", pLogFilePath);
#endif
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		strcpy(m_pFileLogName, "\\config\\RegNetDeviceLog.dat");
#else
		strcpy(m_pFileLogName, "config/RegNetDeviceLog.dat");
#endif
	}

	FILE *pFile = fopen(m_pFileLogName, "rb+");
	
	if(pFile == NULL)	//文件还没有创建
	{
		pFile = fopen(m_pFileLogName, "wb+");
		if(pFile == NULL)
		{
			goto ERROR_PROC;
		}
		
		m_logHeader.headFlag = NETDEVICE_LOG_HEAD_FLAG;
		m_logHeader.itemLen = sizeof(NET_DEVICE_LINK_INFO);
		m_logHeader.deviceTotalNum = MAX_NET_DEVICE_NUM;
		m_logHeader.deviceUsedNum = 0;
		
		if(1 != fwrite(&m_logHeader, sizeof(NETDEVICE_LOG_HEADER), 1, pFile))
		{
			goto ERROR_PROC;
		}
		
		m_logHeader.deviceTotalNum = MAX_NET_DEVICE_NUM;
		m_logHeader.deviceUsedNum = 0;
		
		m_pNetDeviceInfo = new NET_DEVICE_LINK_INFO[m_logHeader.deviceTotalNum];
		assert(m_pNetDeviceInfo != NULL);
		memset(m_pNetDeviceInfo, 0, sizeof(NET_DEVICE_LINK_INFO) * m_logHeader.deviceTotalNum);
		
		//for(unsigned short i = 0; i < (unsigned short)m_logHeader.deviceTotalNum; i++)
		//{
		//	m_pNetDeviceInfo[i].deviceID = i+1;
		//}
		
		if(1 != fwrite(m_pNetDeviceInfo, sizeof(NET_DEVICE_LINK_INFO) * m_logHeader.deviceTotalNum, 1, pFile))
		{
			goto ERROR_PROC;
		}

		NEDEVICE_EXT_FUNC_FLAG stExtFlag;
		memset( &stExtFlag, 0, sizeof(NEDEVICE_EXT_FUNC_FLAG));
		if(1 != fwrite( &stExtFlag, sizeof(NEDEVICE_EXT_FUNC_FLAG), 1, pFile))
		{
			goto ERROR_PROC;
		}
	}
	else				//从文件中读取网络设备信息
	{
		if(1 != fread(&m_logHeader, sizeof(NETDEVICE_LOG_HEADER), 1, pFile))
		{
			goto ERROR_PROC;
		}
		
		assert(m_logHeader.headFlag == NETDEVICE_LOG_HEAD_FLAG);
		
		m_pNetDeviceInfo = new NET_DEVICE_LINK_INFO[m_logHeader.deviceTotalNum];
		assert(m_pNetDeviceInfo != NULL);
		
		if(1 != fread(m_pNetDeviceInfo, sizeof(NET_DEVICE_LINK_INFO) * m_logHeader.deviceTotalNum, 1, pFile))
		{
			goto ERROR_PROC;
		}


		NEDEVICE_EXT_FUNC_FLAG stExtFlag;
		memset( &stExtFlag, 0, sizeof(NEDEVICE_EXT_FUNC_FLAG));
		if(1 != fread( &stExtFlag, sizeof(NEDEVICE_EXT_FUNC_FLAG), 1, pFile))
		{
			m_bPoeUsedPtoPFunc = true;
		}
		else
		{
			if( 1 == stExtFlag.flag_poeIsPtoP )
			{
				m_bPoeUsedPtoPFunc = false;
			}
			else
			{
				m_bPoeUsedPtoPFunc = true;
			}
		}
		printf( "current poe mode is == %d,%s,%d\n", stExtFlag.flag_poeIsPtoP, __FILE__, __LINE__ );
	}

	fclose(pFile);

	return true;

ERROR_PROC:
	if(m_pFileLogName != NULL)
	{
		delete [] m_pFileLogName;
		m_pFileLogName = NULL;
	}
	if(m_pNetDeviceInfo != NULL)
	{
		delete [] m_pNetDeviceInfo;
		m_pNetDeviceInfo = NULL;
	}
	if(pFile != NULL)
	{
		fclose(pFile);
	}
	assert(false);

	return false;
}

void CRegNetDeviceLog::Quit()
{
	if(NULL != m_pFileLogName)
	{
		delete [] m_pFileLogName;
		m_pFileLogName = NULL;
		delete [] m_pNetDeviceInfo;
		m_pNetDeviceInfo = NULL;
	}
}

unsigned long CRegNetDeviceLog::GetMaxDeviceCount() const
{
	return m_logHeader.deviceTotalNum;
}

bool CRegNetDeviceLog::SetNetDevice(NET_DEVICE_LINK_INFO *pNetDevice, unsigned long num)
{
	m_fileLock.Lock();

	m_IDList.clear();
	for(long i = 0; i < MAX_NET_DEVICE_NUM; i++)
	{
		m_IDList.push_back(i);
	}

	bool bRet = true;
	unsigned long index;

	for(index = 0; index < num; index++)
	{
		pNetDevice[index].enabled = false;
		if(pNetDevice[index].deviceID != INVALID_INDEX(unsigned long))
		{
			m_IDList.remove(pNetDevice[index].deviceID);
		}
	}

	//把所有设备置为不可用
	unsigned long curTime = GetCurrTime32();
	for(index = 0; index < m_logHeader.deviceTotalNum; index++)
	{
		if(m_pNetDeviceInfo[index].enabled)
		{
			m_pNetDeviceInfo[index].enabled = false;
//			m_pNetDeviceInfo[index].deleteTime = curTime;
		}
	}

	//把设备按绑定的通道号有序地写入文件
	
	unsigned long min_ch_index = -1;
	for(unsigned long temp = 0; temp < num; temp++)
	{
		min_ch_index = -1;
		for(index = 0; index < num; index++)
		{
			if(!pNetDevice[index].enabled)
			{
				if(-1 == min_ch_index)
				{
					min_ch_index = index;
				}
				else if(pNetDevice[index].channel < pNetDevice[min_ch_index].channel)
				{
					min_ch_index = index;
				}
			}
		}

		assert(min_ch_index != -1);
		pNetDevice[min_ch_index].enabled = true;
		bRet = AddOneDevice(pNetDevice[min_ch_index]);
		if(!bRet)
		{
			assert(false);
			break;
		}
	}

	//把记录项更新到配置文件中
	FILE *pFile = fopen(m_pFileLogName, "rb+");
	if(pFile == NULL)
	{
		m_fileLock.UnLock();
		assert(false);
		return false;
	}

	if(0 != fseek(pFile, sizeof(NETDEVICE_LOG_HEADER), SEEK_SET))
	{
		m_fileLock.UnLock();
		fclose(pFile);
		assert(false);
		return false;
	}
	else
	{
		if(1 != fwrite(m_pNetDeviceInfo, sizeof(NET_DEVICE_LINK_INFO)*m_logHeader.deviceTotalNum, 1, pFile))
		{
			m_fileLock.UnLock();
			fclose(pFile);
			assert(false);
			return false;
		}
	}

	m_logHeader.deviceUsedNum = num;

	if(0 != fseek(pFile, 0, SEEK_SET))
	{
		assert(false);
		bRet = false;
	}
	else
	{
		if(1 != fwrite(&m_logHeader, sizeof(NETDEVICE_LOG_HEADER), 1, pFile))
		{
			assert(false);
			bRet = false;
		}
	}

	fclose(pFile);

	m_fileLock.UnLock();

	return bRet;
}

void CRegNetDeviceLog::SetPoeUsedPtoPMode( bool bPoeUsedModePtoP )
{
	unsigned long ulPos = 0;

	if( m_bPoeUsedPtoPFunc == bPoeUsedModePtoP )
	{
		return;
	}

	m_fileLock.Lock();

	FILE *pFile = fopen(m_pFileLogName, "rb+");
	if(pFile == NULL)
	{
		m_fileLock.UnLock();
		return;
	}

	ulPos = sizeof(NETDEVICE_LOG_HEADER) + (m_logHeader.deviceTotalNum)*sizeof(NET_DEVICE_LINK_INFO);

	//因为当前有128个IPC的配置，暂时使用不了128路，故使用第128路的IPC配置信息保存POE对应模式
	if(0 == fseek(pFile, ulPos, SEEK_SET))
	{
		NEDEVICE_EXT_FUNC_FLAG stExtFlag;
		memset( &stExtFlag, 0, sizeof(NEDEVICE_EXT_FUNC_FLAG));

		if(1 == fread(&stExtFlag, sizeof(NEDEVICE_EXT_FUNC_FLAG), 1, pFile))
		{
			if(0 == fseek(pFile, ulPos, SEEK_SET))
			{
				if( bPoeUsedModePtoP )
				{
					stExtFlag.flag_poeIsPtoP = 0; 
				}
				else
				{
					stExtFlag.flag_poeIsPtoP = 1; 
				}

				if(1 == fwrite(&stExtFlag, sizeof(NEDEVICE_EXT_FUNC_FLAG), 1, pFile))
				{

				}
			}
		}
	}

	fclose(pFile);

	m_bPoeUsedPtoPFunc = bPoeUsedModePtoP;

	m_fileLock.UnLock();
}

unsigned long CRegNetDeviceLog::GetNetDeviceCount() const
{
	return m_logHeader.deviceUsedNum;
}

unsigned long CRegNetDeviceLog::GetNetDevice(NET_DEVICE_LINK_INFO *pNetDevice, unsigned long num)
{
	m_fileLock.Lock();
	unsigned long retNum = 0;
	for(unsigned long i = 0; i < m_logHeader.deviceTotalNum; i++)
	{
		if(retNum >= num)
		{
			break;
		}
		
		if(m_pNetDeviceInfo[i].enabled)
		{
			memcpy(&pNetDevice[retNum], &m_pNetDeviceInfo[i], sizeof(NET_DEVICE_LINK_INFO));
			retNum++;
		}
	}
	
	m_fileLock.UnLock();
	return retNum;
}

/*********************************************************************************
返回可用的的网络设备的列表
**********************************************************************************/
void CRegNetDeviceLog::GetChBindNetDevice(std::list<NET_DEVICE_LINK_INFO> &list)
{
	m_fileLock.Lock();
	list.clear();
	
	unsigned char invalid = INVALID_INDEX(unsigned char);
	for(unsigned long id = 0; id < m_logHeader.deviceTotalNum; id++)
	{
		if(m_pNetDeviceInfo[id].enabled && (m_pNetDeviceInfo[id].channel != invalid))
		{
			list.push_back(m_pNetDeviceInfo[id]);
		}
	}
	
	m_fileLock.UnLock();
}

///*********************************************************************************
//为指定的网络设备分配通道，只有分配到本地通道的设备才会被接入
//**********************************************************************************/
//bool CRegNetDeviceLog::AllocChannelForDevice(unsigned short deviceID, unsigned char channel)
//{
//	for(unsigned long id = 0; id < m_deviceCount; id++)
//	{
//		if(m_pNetDeviceInfo[id].deviceID == deviceID)
//		{
//			assert(m_pNetDeviceInfo[id].enabled);
//			m_pNetDeviceInfo[id].channel = channel;
//			return true;
//		}
//	}
//	
//	return false;
//}

bool CRegNetDeviceLog::AddOneDevice(NET_DEVICE_LINK_INFO &netDevice)
{
	long index = 0;
	for(index = 0; index < m_logHeader.deviceTotalNum; index++)
	{
		if(!m_pNetDeviceInfo[index].enabled)
		{
			break;
		}
	}

	memcpy(&m_pNetDeviceInfo[index], &netDevice, sizeof(NET_DEVICE_LINK_INFO));
	m_pNetDeviceInfo[index].enabled = true;
//	m_pNetDeviceInfo[index].deleteTime = 0x7FFFFFFF;

	if(netDevice.deviceID == INVALID_INDEX(unsigned long))
	{
		m_pNetDeviceInfo[index].deviceID = m_IDList.front();
		m_IDList.pop_front();
	}
	
	return true;
}

void CRegNetDeviceLog::EnterIPCamConfig(unsigned long clientID/* = LOCAL_DEVICE_ID*/)
{
	m_clientID = clientID;
}

void CRegNetDeviceLog::ExitIPCamConfig()
{
	m_clientID = INVALID_CLIENT_ID;
}
