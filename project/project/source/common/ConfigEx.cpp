/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2009-03-26
** Name         : ConfigEx.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "ConfigEx.h"
#include "mylist.cpp"
#include "PTZ.h"
#include "Version.h"
#include "LocalDevice.h"
#include "FuncCustom.h"

#if defined(__SUPPORT_SNAPD1__)
#include "ShanghaiCmd.h"
#endif

extern VIDEO_FORMAT	g_video_format;
#ifdef __ENVIRONMENT_LINUX__
extern bool g_bUse1104A1108ChipColor;
#endif
const unsigned char	MOTION_AREA_GRID_WIDTH	= 22;
const unsigned char	MOTION_AREA_GRID_HEIGHT = 15;
extern const unsigned long g_resolutionNum;

extern bool g_ext_init_interface;

//////////////////////////////////////////////////////////////////////////
//2010-03-09 15:03:00 YSW
//��������֡�ʺ�ʵ��֡�ʵ�ת�����ļ����¼��Ϊ�����¼��ÿ�ζ�ȡ����д�붼����
//��ǰ����Ƶ��ʽ����ת����ԭ��Ϊȥ25��30����С�������İٷֱȡ�
unsigned long VRateToRate(unsigned long vRate, VIDEO_FORMAT videoFormat)
{
	assert ((5 <= vRate) && (vRate <= 150));

	if (VIDEO_FORMAT_NTSC == videoFormat)
	{
		unsigned long rate = (vRate + 3) / 5;
		return (0 < rate) ? rate : 1;
	}
	else
	{
		unsigned long rate = (vRate + 2) / 6;
		return (0 < rate) ? rate : 1;
	}
}

unsigned long RateToVRate(unsigned long rate, VIDEO_FORMAT videoFormat)
{
	if (VIDEO_FORMAT_NTSC == videoFormat)
	{
		assert ((1 <= rate) && (rate <= 30));

		return (rate * 5);
	}
	else
	{
		assert ((1 <= rate) && (rate <= 25));
		return (rate * 6);
	}
}
//////////////////////////////////////////////////////////////////////////
CConfigEx *CConfigEx::Instance()
{
	static CConfigEx s_Config;
	return &s_Config;
}

CConfigEx::CConfigEx():m_clientID(INVALID_CLIENT_ID), m_pFile(NULL)
{
	CProduct *pProduct = CProduct::Instance();

	m_localVideoInputNum	= pProduct->LocalVideoInputNum();
	m_audioInputNum			= pProduct->AudioInputNum();
	m_sensorOutputNum		= pProduct->SensorOutputNum();
	m_maxClientNum			= pProduct->MaxNetClientNum();
	m_netVideoInputNum		= pProduct->NetVideoInputNum();
	m_localSensorInputNum	= pProduct->SensorInputNum();

	m_videoInputNum			= m_localVideoInputNum + m_netVideoInputNum;
	m_sensorInputNum		= m_localSensorInputNum + m_netVideoInputNum;
	m_bHaveCheckRate        = false;

	memset(&m_cfgHead, 0, sizeof(CONFIG_CFG_HEAD));
}

CConfigEx::~CConfigEx()
{

}

bool CConfigEx::Initial(const char *pFilePath)
{
	m_bCreatNewFile = false;
	m_bHaveCheckRate = false;
#if 0
	if(!(m_preset.Initial(pFilePath)&& m_cruise.Initial(pFilePath)))
	{
		printf("Preset Or Cruise initial Error!\n");
		return false;
	}
#endif

	char fileName[512] = {0};
	if (NULL != pFilePath)
	{
		strcpy(fileName, pFilePath);
#ifdef __ENVIRONMENT_WIN32__
		strcat(fileName, "\\config\\config.dat");
#else
		strcat(fileName, "config/config.dat");
#endif
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		strcpy(fileName, "\\config\\config.dat");
#else
		strcpy(fileName, "config/config.dat");
#endif
	}

	m_pFile = fopen(fileName, "rb+");
	if(NULL == m_pFile)
	{
		//������ļ����ɹ����򴴽��ļ�������ʼ�������
		//load default		
		m_pFile = fopen(fileName, "wb");
		if(NULL == m_pFile)
		{
			return false;
		}

		//��ʼ�������ļ�
		if (!InitialConfig())
		{
			fclose(m_pFile);
			m_pFile = NULL;
			return false;
		}

		//��ʼ���ļ��󣬹رղ��ٴ���ֻ����ʽ�򿪡�
		fclose(m_pFile);		
		m_pFile = fopen(fileName, "rb+");
		if(NULL == m_pFile)
		{
			return false;
		}
		
	}

	//����������Ϣ
	if (!LoadConfig(fileName))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	//��ʼ��Ԥ�Ƶ�����
	if(m_videoInputNum > 0)
	{
		if (!m_presetConfig.Initial(m_videoInputNum, pFilePath))//temporary
		{
			fclose(m_pFile);
			m_pFile = NULL;
			return false;
		}
	}	

	//��ʼ��Ѳ��������	
	if(m_videoInputNum > 0)
	{
		if (!m_cruiseConfig.Initial(m_videoInputNum, pFilePath))//temporary
		{
			fclose(m_pFile);
			m_pFile = NULL;
			return false;
		}
	}

	//��ʼ�����ų�����
	if (!m_scheduleHolidayCfg.Initial(m_videoInputNum, m_sensorInputNum, m_sensorOutputNum, pFilePath))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	return true;
}

void CConfigEx::Quit()
{
	if(NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	if(!m_itemList.IsEmpty())
	{
		m_itemList.RemoveAll();
	}
	
	m_scheduleHolidayCfg.Quit();
	m_cruiseConfig.Quit();
	m_presetConfig.Quit();
}

void CConfigEx::EnterConfig(unsigned long clientID/* = LOCAL_DEVICE_ID*/)
{
	m_lock.Lock( );
	m_clientID = clientID;
	m_lock.UnLock( );
}

void CConfigEx::ExitConfig()
{
	m_lock.Lock( );
	m_clientID = INVALID_CLIENT_ID;
	m_lock.UnLock( );
}

//private
//////////////////////////////////////////////////////////////////////////
bool CConfigEx::InitialConfig ()
{
	if (!InitialHead())
	{
		return false;
	}

	if (!InitialBody())
	{
		return false;
	}

	if (!InitialPTZ())
	{
		return false;
	}

	//����ͷ
	if (0 == fseek(m_pFile, 0, SEEK_SET))
	{
		if (1 == fwrite(&m_cfgHead, sizeof(CONFIG_CFG_HEAD), 1, m_pFile))
		{
			return true;
		}
	}

	return false;
}

bool CConfigEx::LoadConfig (const char *pFilePath)
{
	if (0 != fseek(m_pFile, 0, SEEK_SET))
	{
		return false;
	}

	if (1 != fread(&m_cfgHead, sizeof(CONFIG_CFG_HEAD), 1, m_pFile))
	{
		return false;
	}

	if (!InitialConfigItemHead())
	{
		return false;
	}

	if (m_cfgHead.productID	== CProduct::Instance()->ProductType())
	{
		//��������ļ��汾���ԣ��ͰѰ汾��Ϣ��д�������ļ���
		if (m_cfgHead.version != NCFG_VERSION)
		{
			m_cfgHead.version = NCFG_VERSION;
			if (0 != fseek(m_pFile, 0, SEEK_SET))
			{
				return false;
			}

			if (1 != fwrite(&m_cfgHead, sizeof(CONFIG_CFG_HEAD), 1, m_pFile))
			{
				return false;
			}

			//���޸����е����������������ļ��в����ڵ��������ֵΪĬ��ֵ��
			bool bRet = LoadDefaultConfig(0xffffffff, false, true);
			if (!bRet)
			{
				return false;
			}
		}
	}
	else
	{
		//��ͬ��Ʒ�������ļ������´��ļ���������ֻ����ʽ�򿪣������޸ġ�
		/*fclose(m_pFile);
		m_pFile = fopen(pFilePath, "rb");
		if (m_pFile == NULL)
		{
			return false;
		}*/

		//��ͬ��Ʒ�����ô��ˣ�ȴ������û�����塣2009-09-23 YSW
		return false;
	}

	return true;
}

bool CConfigEx::InitialConfigItemHead()
{
	if (0 != fseek(m_pFile, 0, SEEK_END))
	{
		return false;
	}

	long fileLen = ftell(m_pFile);
	assert(fileLen > 0);

	unsigned char *pConfig = new unsigned char[fileLen];
	assert (NULL != pConfig);

	if (0 != fseek(m_pFile,0,SEEK_SET))
	{
		delete [] pConfig;
		return false;
	}

	memset(pConfig, 0, fileLen);

	if (1 != fread(pConfig, fileLen, 1, m_pFile))
	{
		delete [] pConfig;
		return false;
	}

	NCFG_ITEM_HEAD *pItemHead = NULL;
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	long currPos = CONFIG_DATA_POS;
	NCFG_ITEM_HEAD_EX itemHeadNode;

	while (currPos < fileLen)
	{
		pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pConfig + currPos);
		memcpy(&itemHeadNode.item_head, pItemHead, sizeof(NCFG_ITEM_HEAD));
		itemHeadNode.start_pos = currPos;

		if (m_itemList.IsEmpty())
		{
			m_itemList.AddTail(itemHeadNode);
		}
		else
		{
			if (m_itemList.GetTail().item_head.itemID < itemHeadNode.item_head.itemID)
			{
				m_itemList.AddTail(itemHeadNode);
			}
			else
			{
				POS pos = m_itemList.GetHeadPosition();
				while (NULL != pos)
				{
					NCFG_ITEM_HEAD_EX & node = m_itemList.GetAt(pos);
					if (itemHeadNode.item_head.itemID < node.item_head.itemID)
					{
						m_itemList.InsertBefore(pos, itemHeadNode);
						break;
					}

					m_itemList.GetNext(pos);
				}

				assert (false);
			}
		}

		currPos += (sizeof(NCFG_ITEM_HEAD) + pItemHead->len);
	}

	delete [] pConfig;
	pConfig = NULL;

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CConfigEx::InitialHead()
{
	m_cfgHead.version			= NCFG_VERSION;
	m_cfgHead.headLen			= sizeof(CONFIG_CFG_HEAD);
	m_cfgHead.productID			= CProduct::Instance()->ProductType();
	m_cfgHead.subProductID		= CProduct::Instance()->ProductSubID();
	m_cfgHead.firmwareVersion	= FirmwareVersion();
	m_cfgHead.fileLength		= m_cfgHead.headLen;
	m_cfgHead.itemnum			= 0;

	m_cfgHead.localVideoInputNum	= m_localVideoInputNum;
	m_cfgHead.audioInputNum			= m_audioInputNum;
	m_cfgHead.sensorInputNum		= m_sensorInputNum;
	m_cfgHead.sensorOutputNum		= m_sensorOutputNum;
	m_cfgHead.netVideoInputNum		= m_netVideoInputNum;
	m_cfgHead.videoInputNum			= m_videoInputNum;

	if (0 == fseek(m_pFile, 0, SEEK_SET))
	{
		if (1 == fwrite(&m_cfgHead, sizeof(CONFIG_CFG_HEAD), 1, m_pFile))
		{
			return true;
		}
	}

	return false;
}

bool CConfigEx::WriteConfig(unsigned char *pConfig, unsigned long len, size_t pos)
{
	assert (NULL != pConfig);
	assert (len > 0);

	//��λ�����ݿ�ʼ��
	if (0 == fseek(m_pFile, pos, SEEK_SET))
	{
		if (1 == fwrite(pConfig, len, 1, m_pFile))
		{
			return true;
		}
	}

	return false;
}

bool CConfigEx::LoadDefault(unsigned long param)
{
	return LoadDefaultConfig(param, true, false);
}

bool CConfigEx::LoadDefaultConfig(unsigned long param, bool bReWrite/* =false */, bool bAddInConfig/* =false */)
{
	unsigned char *pConfig = NULL;
	unsigned long len = 0;
	unsigned short num = 0;

	if (param & DEFAULT_CFG_SYSTEM_BASE)
	{
		//ע����ڴ棬��Ҫ���ⲿ�ͷš�
		LoadSystemCfg(&pConfig, len, num, NULL, 0, NULL, false);

		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	if (param & DEFAULT_CFG_CHANNEL_BASE)
	{
		LoadChannCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	if (param & DEFAULT_CFG_CHANNEL_REC_BASE)
	{
		LoadChannRecCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	if (param & DEFAULT_CFG_ENCODE_BASE)
	{
		LoadEncodeCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}	

#ifdef __GB28181_SERVER__
	if (param & DEFAULT_CFG_ENCODE_BASE)
	{
		LoadEncodeCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}	
#endif

	if (param & DEFAULT_CFG_SCHEDULE_BASE)
	{
		LoadScheduleCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	if (param & DEFAULT_CFG_NET_BASE)
	{
		LoadNetBaseCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	if (param & DEFAULT_CFG_ALARM_TRIGER_BASE)
	{
		LoadAlarmTrigerCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	if (param & DEFAULT_CFG_ALARM_TRIGER_SENSORBASE)
	{
		LoadAlarmTrigerSensorCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	if (param & DEFAULT_CFG_ALARM_TRIGER_MOTIONBASE)
	{
		LoadAlarmTrigerMotionCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	if (param & DEFAULT_CFG_ALARM_TRIGER_VLOSSBASE)
	{
		LoadAlarmTrigerVLossCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	if (param & DEFAULT_CFG_ALARM_TRIGER_SHELTERBASE)
	{
		LoadAlarmTrigerShelterCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}  
	}
	
	////////////////////////////////�������///////////////////////////////////
#ifdef __MOBILE_DVR__
	if (param & DEFAULT_CFG_MOBILE_BASE)
	{
		LoadMobileCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}
	
	if (param & DEFAULT_CFG_SPEED_BASE)
	{
		LoadSpeedCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	if (param & DEFAULT_CFG_HIGH_SPEED_BASE)
	{
		LoadHighSpeedCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	if (param & DEFAULT_CFG_LOW_SPEED_BASE)
	{
		LoadLowSpeedCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}
/*
	if (param & DEFAULT_CFG_WALLOP_BASE)
	{
		LoadWallopCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}*/

	if (param & DEFAULT_CFG_WIRELESS_BASE)
	{
		LoadWirelessCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}
#endif
	////////////////////////////////�������-end///////////////////////////////////

	if(param & DEFAULT_CFG_EXCEPTION_BASE)
	{
		LoadAlarmExceptionCfg(&pConfig, len, num);
		if (!SetConfigBlock(pConfig, num, len, bReWrite, bAddInConfig))
		{
			delete [] pConfig;
			pConfig = NULL;

			return false;
		}
	}

	delete [] pConfig;
	pConfig = NULL;

	//���������Ϊ��ɵĲ��֣��������Ƿ��ں��档
	if (param & DEFAULT_CFG_PRESET)
	{
		if (!m_presetConfig.LoadDefault(static_cast<unsigned char>(m_cfgHead.videoInputNum)))
		{
			return false;
		}	
	}

	if (param & DEFAULT_CFG_CRUISE)
	{
		if (!m_cruiseConfig.LoadDefault(static_cast<unsigned char>(m_cfgHead.videoInputNum)))
		{
			return false;
		}
	}

	if (param & DEFAULT_CFG_HOLIDAY_SCHEDULE)
	{
		if (!m_scheduleHolidayCfg.LoadDefault(static_cast<unsigned char>(m_cfgHead.localVideoInputNum), \
						static_cast<unsigned char>(m_cfgHead.sensorInputNum), \
						static_cast<unsigned char>(m_cfgHead.sensorOutputNum)))
		{
			return false;
		}
	}

	return true;
}

bool CConfigEx::InitialBody()
{
	size_t pos = CONFIG_DATA_POS;

	unsigned char *pConfig = NULL;
	unsigned char *pBackupConfig = NULL;
	unsigned long len = 0;
	unsigned short num = 0;
	unsigned long backupPos = 0;
	unsigned long backupLen = 0;
	
	bool bBkRet = LoadBackupConfig(&pBackupConfig, backupLen);
	if (bBkRet)
	{
		printf("##%s,%d,InitialBody: has bkc config ,no writeDefaultResolution \n",__FILE__,__LINE__);
	}
	else
	{
		printf("##%s,%d,InitialBody: is no bkc config ,writeDefaultResolution \n",__FILE__,__LINE__);
		CProduct::Instance()->writeDefaultResolution();
	}

	LoadSystemCfg(&pConfig, len, num, pBackupConfig, backupLen, &backupPos, false);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadChannCfg(&pConfig, len, num, pBackupConfig, backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadChannRecCfg(&pConfig, len, num, pBackupConfig,backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadEncodeCfg(&pConfig, len, num, false, pBackupConfig, backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadScheduleCfg(&pConfig, len, num, pBackupConfig, backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadNetBaseCfg(&pConfig, len, num, pBackupConfig, backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadAlarmTrigerCfg(&pConfig, len, num, pBackupConfig, backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadAlarmTrigerSensorCfg(&pConfig, len, num, pBackupConfig, backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadAlarmTrigerMotionCfg(&pConfig, len, num, pBackupConfig, backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadAlarmTrigerVLossCfg(&pConfig, len, num, pBackupConfig, backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadAlarmTrigerShelterCfg(&pConfig, len, num, pBackupConfig, backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

#ifdef __MOBILE_DVR__
	LoadMobileCfg(&pConfig, len, num);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadSpeedCfg(&pConfig, len, num);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadHighSpeedCfg(&pConfig, len, num);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

	LoadLowSpeedCfg(&pConfig, len, num);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}
	
	LoadWirelessCfg(&pConfig, len, num);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}
#endif

	LoadAlarmExceptionCfg(&pConfig, len, num, pBackupConfig, backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}

#ifdef __GB28181_SERVER__
	LoadGB28181Cfg(&pConfig, len, num, false, pBackupConfig, backupLen, &backupPos);
	if (!WriteConfig(pConfig, len, pos))
	{
		delete [] pConfig;
		delete [] pBackupConfig;
		pConfig = NULL;
		pBackupConfig = NULL;

		return false;
	}
	else
	{
		delete [] pConfig;
		pConfig = NULL;

		pos += len;
		m_cfgHead.itemnum += num;
	}
#endif

	m_cfgHead.fileLength = pos;	//�����ļ�����

	delete [] pBackupConfig;
	pBackupConfig = NULL;

	return true;              
}


//************************************
// Method:    SetConfigBlock
// FullName:  CConfigEx::SetConfigBlock
// Access:    private 
// Returns:   bool
// Qualifier:
// Parameter: const unsigned char * pDataBlock ��������
// Parameter: const unsigned short num  pDataBlock�а�������������
// Parameter: bool bReWrite �Ƿ���pDataBlock������д�������ֵ
// Parameter: bool bAddInConfig ����û�е�������Ƿ������ӵ������ļ�ĩβ��
// [4/17/2009 xkg]
//************************************
bool CConfigEx::SetConfigBlock(const unsigned char *pDataBlock, unsigned long num, unsigned long dataLen,\
							   bool bReWrite/* =false */, bool bAddInConfig/* =false */)
{
	assert(pDataBlock != NULL);
	assert(num > 0);

	long currPos = 0;
	const NCFG_ITEM_HEAD *pItemHead = NULL;
	POS pos = m_itemList.GetHeadPosition();
	bool bFlg = false;

	bool bChangeEncodeMode = false;
	unsigned long encodeSize = 0;

	for (unsigned long i=0; i<num; i++)
	{
		pItemHead = reinterpret_cast<const NCFG_ITEM_HEAD *>(pDataBlock + currPos);
		
		//����Ԥ�Ƶ�
		if (bReWrite && (pItemHead->itemID >= NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE) && \
			(pItemHead->itemID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND))
		{
			if (!SetPresetConfigInfo(pItemHead->itemID, pDataBlock+sizeof(NCFG_ITEM_HEAD)+currPos, pItemHead->num))
			{
				return false;
			}
		}
		else if (bReWrite && (pItemHead->itemID >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE) && \
			(pItemHead->itemID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND))//����Ѳ����
		{
			if (!SetCruiseConfigInfo(pItemHead->itemID, pDataBlock+sizeof(NCFG_ITEM_HEAD)+currPos, pItemHead->num))
			{
				return false;
			}
		}
		else if (bReWrite && (pItemHead->itemID > NCFG_ITEM_SCH_HOLIDAY_BASE) && \
			(pItemHead->itemID < NCFG_ITEM_SCH_HOLIDAY_END))//�����ų�
		{
			if (!SetHolidayScheCfgInfo(pItemHead->itemID, pDataBlock+sizeof(NCFG_ITEM_HEAD)+currPos, pItemHead->num))
			{
				return false;
			}
		}
		else//��������
		{
			while (NULL != pos)
			{
				NCFG_ITEM_HEAD_EX & node = m_itemList.GetAt(pos);
				if (bReWrite && (pItemHead->itemID == node.item_head.itemID))
				{
					assert (pItemHead->len == node.item_head.len);
					assert (pItemHead->subLen == node.item_head.subLen);

					if (0 != fseek(m_pFile, node.start_pos+sizeof(NCFG_ITEM_HEAD), SEEK_SET))
					{
						return false;
					}

					if ((NCFG_ITEM_ENCODE_MAJOR == pItemHead->itemID) || (NCFG_ITEM_ENCODE_MINOR == pItemHead->itemID) || (NCFG_ITEM_ENCODE_PHONE == pItemHead->itemID))
					{
						ENCODE_INFO *pEncode = (ENCODE_INFO *)(pDataBlock+sizeof(NCFG_ITEM_HEAD)+currPos);
						for (int i=0; i<m_cfgHead.localVideoInputNum; i++)
						{
							pEncode[i].rate = RateToVRate(pEncode[i].rate, g_video_format);
							if((NCFG_ITEM_ENCODE_MAJOR == pItemHead->itemID))
							{//main stream
								if(pEncode[i].hBitStream>=GetMainStreamMaxBitRate(pEncode[i].resolution))
								{
									pEncode[i].hBitStream = GetMainStreamMaxBitRate(pEncode[i].resolution);
								}
							}
							else if((NCFG_ITEM_ENCODE_MINOR == pItemHead->itemID))
							{//net sub stream
								if(pEncode[i].hBitStream>=GetSubStreamMaxBitRate(pEncode[i].resolution))
								{
									pEncode[i].hBitStream = GetSubStreamMaxBitRate(pEncode[i].resolution);
								}
							}
						}
						
					}
					if((NCFG_ITEM_ENCODE_MAJOR_EX == pItemHead->itemID) || (NCFG_ITEM_ENCODE_MINOR_EX == pItemHead->itemID))
					{
						ENCODE_INFO_EX *pEncode = (ENCODE_INFO_EX *)(pDataBlock+sizeof(NCFG_ITEM_HEAD)+currPos);
						for (int i=0; i<m_cfgHead.localVideoInputNum + m_cfgHead.netVideoInputNum; i++)
						{
							pEncode[i].rate = RateToVRate(pEncode[i].rate, g_video_format);
							if((NCFG_ITEM_ENCODE_MAJOR_EX == pItemHead->itemID))
							{//main stream
								if(pEncode[i].hBitStream>=GetMainStreamMaxBitRate(pEncode[i].resolution))
								{
									pEncode[i].hBitStream = GetMainStreamMaxBitRate(pEncode[i].resolution);
								}
							}
							else if((NCFG_ITEM_ENCODE_MINOR_EX == pItemHead->itemID))
							{//net sub stream
								if(pEncode[i].hBitStream>=GetSubStreamMaxBitRate(pEncode[i].resolution))
								{
									pEncode[i].hBitStream = GetSubStreamMaxBitRate(pEncode[i].resolution);
								}
							}
						}
					}

					if (NCFG_ITEM_ENCODE_960H == pItemHead->itemID)
					{
						PRODUCT_TYPE productID = CProduct::Instance()->ProductType();

						bool bHaveVideoMode = false;
						if ((TD_2308SL_C == productID) || (TD_2308SS_C == productID) || (TD_2316SS_C == productID) || 
							(TD_2308SE_C == productID) || (TD_2316SE_C == productID) || (TD_2308ME_C == productID) || 
							(TD_2316ME_C == productID))
						{
							bHaveVideoMode = true;
						}

#if defined(__CUSTOM_US02__)
						if (TD_2516HS_BL == productID)
						{
							bHaveVideoMode = true;
						}
#endif
						if (bHaveVideoMode)
						{
							bChangeEncodeMode = true;
							encodeSize = *(unsigned long *)(pDataBlock+sizeof(NCFG_ITEM_HEAD)+currPos);
							CLocalDevice::Instance()->SetEncodeSize(encodeSize);
							printf("%s:%s:%d, Change to another device mode, size=%d\n", __FUNCTION__, __FILE__, __LINE__, encodeSize);
						}
					}

					if (1 != fwrite(pDataBlock+sizeof(NCFG_ITEM_HEAD)+currPos, pItemHead->len, 1, m_pFile))
					{
						return false;
					}

					m_itemList.GetNext(pos);
					break;
				}
				else if (bAddInConfig && (pItemHead->itemID < node.item_head.itemID))
				{
					//д���ļ�β��
					if (0 != fseek(m_pFile, 0, SEEK_END))
					{
						return false;
					}

					long startPos = ftell(m_pFile);
					if (1 != fwrite(pDataBlock+sizeof(NCFG_ITEM_HEAD)+currPos, dataLen, 1, m_pFile))
					{
						return false;
					}

					//�����б�ֻ��Ҫ���뵱ǰ�ڵ��ǰ������ˡ�
					NCFG_ITEM_HEAD_EX itemHeadExNode;
					memcpy(&itemHeadExNode.item_head, pItemHead, sizeof(NCFG_ITEM_HEAD));
					itemHeadExNode.start_pos = startPos;

					m_itemList.InsertBefore(pos, itemHeadExNode);

					break;
				}
				else
				{
					m_itemList.GetNext(pos);
				}
			}

			if (bAddInConfig && (NULL == pos))
			{
				//�Ƿ�Ҫ��ӽ������ļ���ĩβ��
				bFlg = true;

				if (0 != fseek(m_pFile, 0, SEEK_END))
				{
					return false;
				}

				long startPos = ftell(m_pFile);
				if (1 != fwrite(pDataBlock+sizeof(NCFG_ITEM_HEAD)+currPos, dataLen, 1, m_pFile))
				{
					return false;
				}

				//�����б�ֻ��Ҫ���뵱ǰ�ڵ��ǰ������ˡ�
				NCFG_ITEM_HEAD_EX itemHeadExNode;
				memcpy(&itemHeadExNode.item_head, pItemHead, sizeof(NCFG_ITEM_HEAD));
				itemHeadExNode.start_pos = startPos;

				m_itemList.AddTail(itemHeadExNode);
			}
		}
		
		currPos += sizeof(NCFG_ITEM_HEAD)+pItemHead->len;
	}

	assert(dataLen == currPos);

	if (bChangeEncodeMode)
	{
		unsigned long videoSize = CProduct::Instance()->LocalDefaVideoSize();
		unsigned long hBitStream = CProduct::Instance()->LocalDefaBitRateValue();
		unsigned long encodeType = CProduct::Instance()->LocalDefauVideoEncodeMode();
		unsigned long quality = CProduct::Instance()->LocalDefQuiltyValue();
		unsigned long rate = 0;
		VIDEO_FORMAT videoFormatTemp = static_cast<VIDEO_FORMAT>(CLocalDevice::GetVideoFormatFromFlash()); //��ȡflash�����Ƶ��ʽ��Ϣ
		if (VIDEO_FORMAT_PAL == videoFormatTemp)
		{
			//����ǻָ�Ĭ�����ã�����ݵ�ǰ����ʽ������ע��NTSC�Ѿ�����Ҫ���¸�ֵ��
			rate = CProduct::Instance()->LocalDefaChnnRateNumPal();
		}
		else
		{
			rate = CProduct::Instance()->LocalDefaChnnRateNumNtsc(); 
		}

		rate = RateToVRate(rate, videoFormatTemp);

		pos = m_itemList.GetHeadPosition();
		while (NULL != pos)
		{
			NCFG_ITEM_HEAD_EX & node = m_itemList.GetAt(pos);
			if ((NCFG_ITEM_ENCODE_MAJOR == node.item_head.itemID))
			{	
				ENCODE_INFO * pEncode = new ENCODE_INFO[m_localVideoInputNum];

				memset(pEncode, 0, sizeof(ENCODE_INFO) * m_localVideoInputNum);

				for (int i = 0; i< m_localVideoInputNum; i++)
				{
					pEncode[i].resolution = videoSize;
					pEncode[i].rate = rate;
					pEncode[i].encodeType = encodeType;
					pEncode[i].quality = quality;
					pEncode[i].lBitStream = 64;
					pEncode[i].hBitStream = hBitStream;
				}

				if (0 != fseek(m_pFile, node.start_pos+sizeof(NCFG_ITEM_HEAD), SEEK_SET))
				{
					delete [] pEncode;
					return false;
				}

				if (1 != fwrite(pEncode, sizeof(ENCODE_INFO) * m_localVideoInputNum, 1, m_pFile))
				{
					delete [] pEncode;
					return false;
				}

				delete [] pEncode;

				m_itemList.GetNext(pos);

				//break;
			}
			else if ((NCFG_ITEM_ENCODE_MAJOR_EX == node.item_head.itemID))
			{
				ENCODE_INFO_EX * pEncodeEx = new ENCODE_INFO_EX[m_localVideoInputNum];

				memset(pEncodeEx, 0, sizeof(ENCODE_INFO_EX) * m_localVideoInputNum);

				for (int i = 0; i< m_localVideoInputNum; i++)
				{
					pEncodeEx[i].resolution = videoSize;
					pEncodeEx[i].rate = rate;
					pEncodeEx[i].encodeType = encodeType;
					pEncodeEx[i].quality = quality;
					pEncodeEx[i].lBitStream = 64;
					pEncodeEx[i].hBitStream = hBitStream;
				}

				if (0 != fseek(m_pFile, node.start_pos+sizeof(NCFG_ITEM_HEAD), SEEK_SET))
				{
					delete [] pEncodeEx;
					return false;
				}

				if (1 != fwrite(pEncodeEx, sizeof(ENCODE_INFO_EX) * m_localVideoInputNum, 1, m_pFile))
				{
					delete [] pEncodeEx;
					return false;
				}

				delete [] pEncodeEx;

				break;
			}
			else
			{
				m_itemList.GetNext(pos);
			}
		}
	}

	if (bAddInConfig && bFlg)
	{
		//����ͷ
		if (0 != fseek(m_pFile, 0, SEEK_SET))
		{
			return false;
		}

		if (1 != fwrite(&m_cfgHead, sizeof(CONFIG_CFG_HEAD), 1, m_pFile))
		{
			return false;
		}
	}

	//
	FileFlush(m_pFile);

#if defined(__SUPPORT_SNAPD1__)
	char tip[256]="GB2312:���ñ���";
	CShanghaiConnectMan::Instance()->GetLogCmd(DVRCFGSAVE,0,tip,strlen(tip),NULL,0,GetCurrTime());
#endif
	return true;
}

//��֤�������ǰ���ID����ġ�
bool CConfigEx::SetConfig(const unsigned char *pDataBlock, unsigned long dataLen)
{
	assert(pDataBlock != NULL);
	assert(dataLen >= (sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)));//���ٵ���һ��������,Ѳ���߿�Ϊ��

	m_lock.Lock();

	unsigned char *pData = new unsigned char[dataLen-sizeof(NCFG_BLOCK_HEAD)];
	if (pData == NULL)
	{
		assert (false);

		m_lock.UnLock();
		return false;
	}
	
	//�����紫���������ݿ�ת�����ļ���ʽ��
	const NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<const NCFG_BLOCK_HEAD*>(pDataBlock);
	unsigned long readPos = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*pBlockHead->ItemNum;
	unsigned writePos = 0;
	const NCFG_ITEM_HEAD *pItemHead = NULL;

	for (unsigned long i =0; i<pBlockHead->ItemNum; i++)
	{
		pItemHead = reinterpret_cast<const NCFG_ITEM_HEAD *>(pDataBlock + i*sizeof(NCFG_ITEM_HEAD) + sizeof(NCFG_BLOCK_HEAD));
		
		assert (writePos <= (dataLen-sizeof(NCFG_BLOCK_HEAD)-sizeof(NCFG_ITEM_HEAD)));
		memcpy(pData+writePos, pItemHead, sizeof(NCFG_ITEM_HEAD));

		writePos += sizeof(NCFG_ITEM_HEAD);
		assert (writePos <= (dataLen-sizeof(NCFG_BLOCK_HEAD)-pItemHead->len));
		assert (readPos <= (dataLen-pItemHead->len));
		memcpy(pData+writePos, pDataBlock+readPos, pItemHead->len);
		writePos += pItemHead->len;

		readPos += pItemHead->len;
	}
	assert(writePos == (dataLen-sizeof(NCFG_BLOCK_HEAD)));

	if (SetConfigBlock(pData, pBlockHead->ItemNum, dataLen-sizeof(NCFG_BLOCK_HEAD), true, false))
	{
		delete [] pData;
		pData = NULL;

		m_lock.UnLock();
		return true;
	}
	else
	{
		delete [] pData;
		pData = NULL;

		m_lock.UnLock();
		return false;
	}
}

bool CConfigEx::GetConfig(unsigned char *pHead, unsigned long headLen, unsigned char **pDataBlock, unsigned long &dataLen)
{
	assert (headLen > 0);
	assert (NULL != pHead);
	assert (NULL != pDataBlock);

	int iNum = headLen/sizeof(NCFG_ITEM_HEAD);

	CMyList <NCFG_ITEM_HEAD> headList;
	NCFG_ITEM_HEAD *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pHead);

	m_lock.Lock();

	//���������������˳��ġ�
	POS pos = m_itemList.GetHeadPosition();
	for (int i=0; i< iNum; i++)
	{
		if (pItemHead->itemID > NCFG_ITEM_ONLYREAD_BASE)
		{
			unsigned short size = 0;
			unsigned short subNum = 0;
			if (!OnlyReadInfoSize(pItemHead->itemID, size, subNum))
			{
				assert(false); //ID�����ڡ�
			}
			else
			{
				pItemHead->len		= size;
				pItemHead->num		= subNum;
				pItemHead->subLen	= size/subNum;
				headList.AddTail(*pItemHead);
			}			
		}
		else if ((pItemHead->itemID >= NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE)&& 
			(pItemHead->itemID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND))
		{
			unsigned short size = 0;
			unsigned short subNum = 0;
			if (!GetPresetConfigInfo(pItemHead->itemID, NULL, size, subNum))
			{
				continue;
			}

			pItemHead->len		= size;
			pItemHead->num		= subNum;
			pItemHead->subLen	= (subNum > 0) ? (size/subNum) : 0;
			headList.AddTail(*pItemHead);
		}
		else if ((pItemHead->itemID >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE)&& 
			(pItemHead->itemID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND))
		{
			unsigned short size = 0;
			unsigned short subNum = 0;
			if (!GetCruiseConfigInfo(pItemHead->itemID, NULL, size, subNum))
			{
				continue;
			}

			pItemHead->len		= size;
			pItemHead->num		= subNum;
			pItemHead->subLen	= -1;
			headList.AddTail(*pItemHead);
		}
		else if((pItemHead->itemID > NCFG_ITEM_SCH_HOLIDAY_BASE) && \
			(pItemHead->itemID < NCFG_ITEM_SCH_HOLIDAY_END))
		{
			unsigned short size = 0;
			unsigned short subNum = 0;
			if (!GetHolidayScheCfgInfo(pItemHead->itemID, NULL, size, subNum))
			{
				m_lock.UnLock();
				return false;
			}

			pItemHead->len		= size;
			pItemHead->num		= subNum;
			pItemHead->subLen	= size/subNum;
			headList.AddTail(*pItemHead);
		}
		else
		{
			if (NULL == pos)
			{
				pos = m_itemList.GetHeadPosition();
			}

			while (NULL != pos)
			{
				NCFG_ITEM_HEAD_EX & node = m_itemList.GetNext(pos);
				if (node.item_head.itemID == pItemHead->itemID)
				{
					memcpy(pItemHead, &node.item_head, sizeof(NCFG_ITEM_HEAD));

					headList.AddTail(*pItemHead);

					break;
				}
			}

		}

		++ pItemHead;
	}

	unsigned short itemNum;
	bool bRet = GetConfigBlock(headList, pDataBlock,  dataLen, itemNum);//headList�б��������ǻ��Զ���ա�
	
	m_lock.UnLock();
	return bRet;
}

bool CConfigEx::GetConfigEx(CMyList<NCFG_ITEM_ID> &itemList, unsigned char **pDataBlock, unsigned long &dataLen)
{
	CMyList <NCFG_ITEM_HEAD> headList;
	NCFG_ITEM_HEAD itemHead;

	m_lock.Lock();

	POS itemIDPos = itemList.GetHeadPosition();
	POS itemHeadPos = m_itemList.GetHeadPosition();

	while (itemIDPos != NULL)
	{
		NCFG_ITEM_ID & itemID = itemList.GetNext(itemIDPos);

		if (itemID > NCFG_ITEM_ONLYREAD_BASE)
		{
			unsigned short size = 0;
			unsigned short subNum = 0;
			if (!OnlyReadInfoSize(itemID, size, subNum))
			{
				assert(false); //ID�����ڡ�
			}
			else
			{
				itemHead.itemID = itemID;
				itemHead.len = size;
				itemHead.num = subNum;
				itemHead.subLen = itemHead.len/itemHead.num;
				headList.AddTail(itemHead);
			}		
		}
		else if ((itemID >= NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE)&& 
			(itemID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND))
		{
			unsigned short DataLen = 0;
			unsigned short SubNum = 0;
			if (!GetPresetConfigInfo(itemID, NULL, DataLen, SubNum))
			{
				m_lock.UnLock();
				return false;
			}

			itemHead.itemID = itemID;
			itemHead.len = DataLen;
			itemHead.num = SubNum;
			itemHead.subLen = itemHead.len/itemHead.num;
			headList.AddTail(itemHead);
		}

		else if ((itemID >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE)&& 
			(itemID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND))
		{
			unsigned short DataLen = 0;
			unsigned short SubNum = 0;
			if (!GetCruiseConfigInfo(itemID, NULL, DataLen, SubNum))
			{
				m_lock.UnLock();
				return false;
			}

			itemHead.itemID = itemID;
			itemHead.len = DataLen;
			itemHead.num = SubNum;
			itemHead.subLen = -1;//���ȳ�//(SubNum > 0) ? itemHead.len/SubNum : 0;
			headList.AddTail(itemHead);
		}

		else if((itemID > NCFG_ITEM_SCH_HOLIDAY_BASE) && \
			(itemID < NCFG_ITEM_SCH_HOLIDAY_END))
		{
			unsigned short DataLen = 0;
			unsigned short SubNum = 0;
			if (!GetHolidayScheCfgInfo(itemID, NULL, DataLen, SubNum))
			{
				m_lock.UnLock();
				return false;
			}

			itemHead.itemID = itemID;
			itemHead.len = DataLen;
			itemHead.num = SubNum;
			itemHead.subLen = itemHead.len/itemHead.num;
			headList.AddTail(itemHead);
		}
		else
		{
			if (NULL == itemHeadPos)
			{
				itemHeadPos = m_itemList.GetHeadPosition();
			}

			while (itemHeadPos != NULL)
			{
				NCFG_ITEM_HEAD_EX & node = m_itemList.GetNext(itemHeadPos);
				if (node.item_head.itemID == (unsigned short)itemID)
				{
					memcpy(&itemHead, &node.item_head, sizeof(NCFG_ITEM_HEAD));

					headList.AddTail(itemHead);

					break;
				}
			}
		}
	}

	unsigned short itemNum;
	bool bRet = GetConfigBlock(headList, pDataBlock,  dataLen, itemNum);

	m_lock.UnLock();
	return bRet;
}
 
bool CConfigEx::GetDefaultData(unsigned char *pHead, unsigned long headLen, unsigned char **pDataBlock, unsigned long &dataLen, bool bChangtoCVBS)
{
	assert (headLen > 0);
	assert (NULL != pHead);
	assert (NULL != pDataBlock);

	int succNum = 0;
	int iNum = headLen/sizeof(NCFG_ITEM_HEAD);

	m_lock.Lock();
	if (0 < iNum)
	{
		unsigned long cfgType = DEFAULT_CFG_NULL;
		//ͳ������Щ���ͣ���Ϊ����Ĭ�ϲ����ǰ�������Ϊ��λ���ġ�
		{
			NCFG_ITEM_HEAD *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pHead);		
			for (int i=0; i<iNum; ++i)
			{
				if ((NCFG_ITEM_SYSTEM_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_SYSTEM_END))
				{
					cfgType |= DEFAULT_CFG_SYSTEM_BASE;
				}
				else if ((NCFG_ITEM_CHANNEL_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_CHANNEL_END))
				{
					cfgType |= DEFAULT_CFG_CHANNEL_BASE;
				}
				else if ((NCFG_ITEM_CHANNEL_REC_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_CHANNEL_REC_END))
				{
					cfgType |= DEFAULT_CFG_CHANNEL_REC_BASE;
				}
				else if ((NCFG_ITEM_ENCODE_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_ENCODE_END))
				{
					cfgType |= DEFAULT_CFG_ENCODE_BASE;
				}
				else if ((NCFG_ITEM_SCHEDULE_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_SCHEDULE_END))
				{
					cfgType |= DEFAULT_CFG_SCHEDULE_BASE;
				}
				else if ((NCFG_ITEM_NET_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_NET_END))
				{
					cfgType |= DEFAULT_CFG_NET_BASE;
				}
				else if ((NCFG_ITEM_ALARM_TRIGER_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_ALARM_TRIGER_END))
				{
					cfgType |= DEFAULT_CFG_ALARM_TRIGER_BASE;
				}
				else if ((NCFG_ITEM_ALARM_TRIGER_SENSOR_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_ALARM_TRIGER_SENSOREND))
				{
					cfgType |= DEFAULT_CFG_ALARM_TRIGER_SENSORBASE;
				}
				else if ((NCFG_ITEM_ALARM_TRIGER_MOTION_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_ALARM_TRIGER_MOTIONEND))
				{
					cfgType |= DEFAULT_CFG_ALARM_TRIGER_MOTIONBASE;
				}
				else if ((NCFG_ITEM_ALARM_TRIGER_VLOSS_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_ALARM_TRIGER_VLOSSEND))
				{
					cfgType |= DEFAULT_CFG_ALARM_TRIGER_VLOSSBASE;
				}
				else if ((NCFG_ITEM_ALARM_TRIGER_SHELTER_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_ALARM_TRIGER_SHELTEREND))
				{
					cfgType |= DEFAULT_CFG_ALARM_TRIGER_SHELTERBASE;
				}
#ifdef __MOBILE_DVR__
				else if ((NCFG_ITEM_MOBILE_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_MOBILE_END))
				{
					cfgType |= DEFAULT_CFG_MOBILE_BASE;
				}
				else if ((NCFG_ITEM_SPEED_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_SPEED_END))
				{
					cfgType |= DEFAULT_CFG_SPEED_BASE;
				}
				else if ((NCFG_ITEM_HIGH_SPEED_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_HIGH_SPEED_END))
				{
					cfgType |= DEFAULT_CFG_HIGH_SPEED_BASE;
				}
				else if ((NCFG_ITEM_LOW_SPEED_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_LOW_SPEED_END))
				{
					cfgType |= DEFAULT_CFG_LOW_SPEED_BASE;
				}
				else if ((NCFG_ITEM_WALLOP_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_WALLOP_END))
				{
					cfgType |= DEFAULT_CFG_WALLOP_BASE;
				}
				else if ((NCFG_ITEM_WIRELESS_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_WIRELESS_END))
				{
					cfgType |= DEFAULT_CFG_WIRELESS_BASE;
				}
#endif
				else if((NCFG_ITEM_EXCEPTION_BASE <= pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_EXCEPTION_END))
				{
					cfgType |= DEFAULT_CFG_EXCEPTION_BASE;
				}
				else if ((NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE <= pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND))
				{
					cfgType |= DEFAULT_CFG_PRESET;
				}
				else if ((NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE <= pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND))
				{
					cfgType |= DEFAULT_CFG_CRUISE;
				}
				else if ((NCFG_ITEM_SCH_HOLIDAY_BASE < pItemHead[i].itemID) && (pItemHead[i].itemID < NCFG_ITEM_SCH_HOLIDAY_END))
				{
					cfgType |= DEFAULT_CFG_HOLIDAY_SCHEDULE;
				}
			}
		}

		assert (DEFAULT_CFG_NULL != cfgType);

		//ͳ�����ݳ���
		{
			NCFG_ITEM_HEAD *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pHead);
			dataLen = sizeof(NCFG_BLOCK_HEAD) + (iNum * sizeof(NCFG_ITEM_HEAD));
			int index = 0;
			POS pos = m_itemList.GetHeadPosition();
			while ((NULL != pos) && (index < iNum))
			{
				NCFG_ITEM_HEAD_EX & node = m_itemList.GetAt(pos);
				if (node.item_head.itemID == pItemHead[index].itemID)
				{
					dataLen += node.item_head.len;

					++ index;
					++ succNum;

					m_itemList.GetNext(pos);
				}
				else if (pItemHead[index].itemID < node.item_head.itemID)
				{
					//������ˣ�˵��pItemHead[index].itemID�ǲ��ṩ��
					//��������汾ʱ���ͻ��˾�����������
					++ index;
				}
				else
				{
					m_itemList.GetNext(pos);
				}
			}

			//����Ƿ�������Ԥ�õ��Ѳ���ߵ�Ĭ������
			if( ( cfgType & DEFAULT_CFG_PRESET ) || ( cfgType & DEFAULT_CFG_CRUISE ) )
			{
				int lastItemCount = iNum - index;

				if( ( cfgType & DEFAULT_CFG_PRESET ) && ( lastItemCount > 0 ) )
				{
					unsigned long j = NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE;

					while((index < iNum) && (j < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND))
					{
						if( j == pItemHead[index].itemID )
						{
							dataLen += ONE_CHNN_PRESET_LEN;
							
							++ index;
							++ j;
							++ succNum;
						}
						else if (pItemHead[index].itemID < j)
						{
							++ index;
						}
						else
						{
							++ j;
						}
					}
				}

				lastItemCount = iNum - index;
				if( ( cfgType & DEFAULT_CFG_CRUISE ) && ( lastItemCount > 0 ) )
				{
						unsigned long j=NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE;
						while((index < iNum) && (j < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND))
						{
							if( j == pItemHead[index].itemID )
							{
								dataLen += 0;
								
								++ index;
								++ j;
								++ succNum;
							}
							else if (pItemHead[index].itemID < j)
							{
								++ index;
							}
							else
							{
								++ j;
							}
						}
				}
				
			}

			//����Ƿ����е�����ҵ�����Щֻ����ڴ��ǲ���ͳ�Ƶ�
			while (index < iNum)
			{
				if ((NCFG_ITEM_ONLYREAD_BASE < pItemHead[index].itemID) && (pItemHead[index].itemID < NCFG_ITEM_ONLYREAD_END))
				{
					unsigned short size = 0;
					unsigned short subNum = 0;
					if (OnlyReadInfoSize(pItemHead[index].itemID, size, subNum))
					{
						dataLen += size;
						++ index;
						++ succNum;
					}
					else
					{
						//ʧ�ܾ�˵��ID�����ڣ����߲�֧�֡�����ͻ������ӻ����汾ʱ����Ҫ���һЩ���ǲ�֧�ֵ�
						//��᷵��ʧ�ܡ�
						++ index;
					}
				}
				else
				{
					assert (false);
				}
			}

			assert (index == iNum);
			assert (succNum <= iNum);
		}
		
		unsigned char *pData = new unsigned char [dataLen];
		assert (NULL != pData);

		//��ͷ��Ϣ	
		{
			NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(pData);
			pBlockHead->biSize		= sizeof(NCFG_BLOCK_HEAD);
			pBlockHead->ItemNum		= succNum;
			pBlockHead->netcfgver	= NCFG_VERSION;
		}
		
		//������Ϣ
		{
			int index = 0 , decIndex = 0;
			NCFG_ITEM_HEAD *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pHead);
			NCFG_ITEM_HEAD *pDecItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pData + sizeof(NCFG_BLOCK_HEAD));
			unsigned char *pItemData = reinterpret_cast<unsigned char *>(pDecItemHead + succNum);

			unsigned long len = 0;
			unsigned short num = 0;
			unsigned char *pConfig = NULL;
			while (DEFAULT_CFG_NULL != cfgType)
			{
				if (cfgType & DEFAULT_CFG_SYSTEM_BASE)
				{
					//ע����ڴ棬��Ҫ���ⲿ�ͷš�
					LoadSystemCfg(&pConfig, len, num, NULL, 0, NULL, bChangtoCVBS);	
					cfgType &= ~DEFAULT_CFG_SYSTEM_BASE;
				}
				else if (cfgType & DEFAULT_CFG_CHANNEL_BASE)
				{
					LoadChannCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_CHANNEL_BASE;
				}
				else if (cfgType & DEFAULT_CFG_CHANNEL_REC_BASE)
				{
					LoadChannRecCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_CHANNEL_REC_BASE;
				}
				else if (cfgType & DEFAULT_CFG_ENCODE_BASE)
				{
					LoadEncodeCfg(&pConfig, len, num, true);
					cfgType &= ~DEFAULT_CFG_ENCODE_BASE;
				}
				else if (cfgType & DEFAULT_CFG_SCHEDULE_BASE)
				{
					LoadScheduleCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_SCHEDULE_BASE;
				}
				else if (cfgType & DEFAULT_CFG_NET_BASE)
				{
					LoadNetBaseCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_NET_BASE;
				}
				else if (cfgType & DEFAULT_CFG_ALARM_TRIGER_BASE)
				{
					LoadAlarmTrigerCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_ALARM_TRIGER_BASE;
				}
				else if (cfgType & DEFAULT_CFG_ALARM_TRIGER_SENSORBASE)
				{
					LoadAlarmTrigerSensorCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_ALARM_TRIGER_SENSORBASE;
				}
				else if (cfgType & DEFAULT_CFG_ALARM_TRIGER_MOTIONBASE)
				{
					LoadAlarmTrigerMotionCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_ALARM_TRIGER_MOTIONBASE;
				}
				else if (cfgType & DEFAULT_CFG_ALARM_TRIGER_VLOSSBASE)
				{
					LoadAlarmTrigerVLossCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_ALARM_TRIGER_VLOSSBASE;
				}
				else if (cfgType & DEFAULT_CFG_ALARM_TRIGER_SHELTERBASE)
				{
					LoadAlarmTrigerShelterCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_ALARM_TRIGER_SHELTERBASE;
				}
#ifdef __MOBILE_DVR__
				else if (cfgType & DEFAULT_CFG_MOBILE_BASE)
				{
					LoadMobileCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_MOBILE_BASE;
				}
				
				else if (cfgType & DEFAULT_CFG_SPEED_BASE)
				{
					LoadSpeedCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_SPEED_BASE;
				}
				else if (cfgType & DEFAULT_CFG_HIGH_SPEED_BASE)
				{
					LoadHighSpeedCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_HIGH_SPEED_BASE;
				}
				else if (cfgType & DEFAULT_CFG_LOW_SPEED_BASE)
				{
					LoadLowSpeedCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_LOW_SPEED_BASE;
				}
				/*	else if (cfgType & DEFAULT_CFG_WALLOP_BASE)
				{
					LoadWallopCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_WALLOP_BASE;
				}*/
				else if (cfgType & DEFAULT_CFG_WIRELESS_BASE)
				{
					LoadWirelessCfg(&pConfig, len, num);
					cfgType &= ~DEFAULT_CFG_WIRELESS_BASE;
				}
#endif
				else if( cfgType & DEFAULT_CFG_EXCEPTION_BASE )
				{
					LoadAlarmExceptionCfg( &pConfig, len, num );
					cfgType &=  ~DEFAULT_CFG_EXCEPTION_BASE;
				}
				else if ( cfgType & DEFAULT_CFG_PRESET )
				{
					LoadPTZPresetCfg( &pConfig, len, num );
					cfgType &=  ~DEFAULT_CFG_PRESET;
				}
				else if ( cfgType & DEFAULT_CFG_CRUISE )
				{
					LoadPTZCruiseCfg( &pConfig, len, num );
					cfgType &= ~DEFAULT_CFG_CRUISE;
				}

				NCFG_ITEM_HEAD *pSrcItemHead = NULL;
				unsigned char *pSrcData = pConfig;
				int srcIndex = 0;
				while ((decIndex < succNum) && (index < iNum) && (srcIndex < num))
				{
					//
					pSrcItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pSrcData);
					if (pSrcItemHead->itemID == pItemHead[index].itemID)
					{
						memcpy(pDecItemHead+decIndex, pSrcItemHead, sizeof(NCFG_ITEM_HEAD));

						memcpy(pItemData, pSrcData+sizeof(NCFG_ITEM_HEAD), pSrcItemHead->len);

						++ index;
						++ srcIndex;
						++ decIndex;

						pItemData += pSrcItemHead->len;
						pSrcData += (sizeof(NCFG_ITEM_HEAD) + pSrcItemHead->len);
					}
					else if (pItemHead[index].itemID < pSrcItemHead->itemID)
					{
						++ index;
					}
					else
					{
						++ srcIndex;
						pSrcData += (sizeof(NCFG_ITEM_HEAD) + pSrcItemHead->len);
					}
				}

				delete [] pConfig;
				pConfig = NULL;
			}

			//���ֻ�������Ϊindex<iNum��

			while ((decIndex < succNum) && (index < iNum))
			{
				if ((NCFG_ITEM_ONLYREAD_BASE < pItemHead[index].itemID) && (pItemHead[index].itemID < NCFG_ITEM_ONLYREAD_END))
				{
					unsigned short size = 0;
					unsigned short subNum = 0;
					if (OnlyReadInfoSize(pItemHead[index].itemID, size, subNum))
					{
						pDecItemHead[decIndex].itemID = pItemHead[index].itemID;
						pDecItemHead[decIndex].len = size;
						pDecItemHead[decIndex].num = subNum;
						pDecItemHead[decIndex].subLen = size/subNum;

						OnlyReadInfo(pItemHead[index].itemID, pItemData, size);

						pItemData += size;

						++ index;
						++ decIndex;
					}
					else
					{
						++ index;
					}
				}
				else
				{
					assert (false);
				}
			}

			assert (index == iNum);
			assert (decIndex == succNum);
		}

		*pDataBlock = pData;

		m_lock.UnLock();
		return true;
	}
	else
	{
		*pDataBlock = NULL;

		m_lock.UnLock();
		return false;
	}
}

//************************************
// Method:    GetConfigBlock
// FullName:  CConfigEx::GetConfigBlock
// Access:    private 
// Returns:   bool
// Qualifier:
// Parameter: CMyList <NCFG_ITEM_HEAD> & itemHeadList  ���������ͷ�������
// Parameter: unsigned char * * pDataBlock ���ݿ�
// Parameter: unsigned long & Len ��������ݿ�ĳ���
// Parameter: unsigned short & itemNum ��������ݿ����ж�����
// [4/17/2009 xkg]
//************************************
bool CConfigEx::GetConfigBlock(CMyList <NCFG_ITEM_HEAD> & itemHeadList, unsigned char **pDataBlock, \
							   unsigned long &Len, unsigned short &itemNum)
{
	assert(itemHeadList.GetCount() > 0);

	*pDataBlock = 0;
	Len = 0;
	itemNum = 0;

	//��ͳ�ƿռ�
	unsigned short num = 0;
	unsigned long dataLen = sizeof(NCFG_BLOCK_HEAD);
	POS pos = itemHeadList.GetHeadPosition();
	while (pos != NULL)
	{
		dataLen += (sizeof(NCFG_ITEM_HEAD) + itemHeadList.GetNext(pos).len);
		num++;
	}
	
	if (0 == num)
	{
		return false;
	}

	assert (dataLen > 0);
	//����ռ�
	unsigned char *pBuf = new unsigned char[dataLen];
	if (pBuf == NULL)
	{
		assert(NULL != pBuf);
		return false;
	}

	memset(pBuf, 0, dataLen);

	NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(pBuf);
	pBlockHead->biSize		= sizeof(NCFG_BLOCK_HEAD);
	pBlockHead->ItemNum		= num;
	pBlockHead->netcfgver	= NCFG_VERSION;

	unsigned long currHeadPos = sizeof(NCFG_BLOCK_HEAD);
	unsigned long currDataPos = sizeof(NCFG_BLOCK_HEAD) + num*sizeof(NCFG_ITEM_HEAD);

	//�������
	POS itemHeadPos = itemHeadList.GetHeadPosition();
	POS itemHeadExPos = m_itemList.GetHeadPosition();
	while (NULL != itemHeadPos)
	{
		NCFG_ITEM_HEAD & itemHead = itemHeadList.GetNext(itemHeadPos);
		if (itemHead.itemID > NCFG_ITEM_ONLYREAD_BASE)
		{
			memcpy(pBuf+currHeadPos, &itemHead, sizeof(NCFG_ITEM_HEAD));
			currHeadPos += sizeof(NCFG_ITEM_HEAD);

			OnlyReadInfo(itemHead.itemID, pBuf+currDataPos, itemHead.len);
			currDataPos += itemHead.len;
		}
		//Ԥ�Ƶ�
		else if((itemHead.itemID >= NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE) && \
				(itemHead.itemID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND))
		{
			unsigned short PresetDataLen=0;
			unsigned short PresetSubNum=0;

			//�ú����Ѿ�������д����buf.
			if (!GetPresetConfigInfo(itemHead.itemID, pBuf+currDataPos, PresetDataLen, PresetSubNum))
			{
				delete [] pBuf;
				pBuf = NULL;
				return false;
			}

			NCFG_ITEM_HEAD tmpItemHead;
			tmpItemHead.itemID = itemHead.itemID;
			tmpItemHead.len = PresetDataLen;
			tmpItemHead.num = PresetSubNum;
			tmpItemHead.subLen = (tmpItemHead.num > 0) ? tmpItemHead.len/tmpItemHead.num : 0;
			
			//ͷд��buf.
			memcpy(pBuf+currHeadPos, &tmpItemHead,  sizeof(NCFG_ITEM_HEAD));
			currHeadPos += sizeof(NCFG_ITEM_HEAD);
			currDataPos += itemHead.len;
		}
		//Ѳ����
		else if ((itemHead.itemID >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE) && \
			(itemHead.itemID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND))
		{
			unsigned short DataLen=0;
			unsigned short SubNum=0;

			//�ú����Ѿ�������д����buf.
			if (!GetCruiseConfigInfo(itemHead.itemID, pBuf+currDataPos, DataLen, SubNum))
			{
				delete [] pBuf;
				pBuf = NULL;
				return false;
			}

			NCFG_ITEM_HEAD tmpItemHead;
			tmpItemHead.itemID = itemHead.itemID;
			tmpItemHead.len = DataLen;
			tmpItemHead.num = SubNum;
			tmpItemHead.subLen = -1;//���ȳ�//(SubNum > 0) ? tmpItemHead.len/SubNum : 0;

			//ͷд��buf.
			memcpy(pBuf+currHeadPos, &tmpItemHead,  sizeof(NCFG_ITEM_HEAD));
			currHeadPos += sizeof(NCFG_ITEM_HEAD);
			currDataPos += itemHead.len;
		}
		//�����ų�
		else if ((itemHead.itemID > NCFG_ITEM_SCH_HOLIDAY_BASE) && \
			(itemHead.itemID < NCFG_ITEM_SCH_HOLIDAY_END))
		{
			unsigned short DataLen=0;
			unsigned short SubNum=0;

			//�ú����Ѿ�������д����buf.
			if (!GetHolidayScheCfgInfo(itemHead.itemID, pBuf+currDataPos, DataLen, SubNum))
			{
				delete [] pBuf;
				pBuf = NULL;
				return false;
			}

			NCFG_ITEM_HEAD tmpItemHead;
			tmpItemHead.itemID = itemHead.itemID;
			tmpItemHead.len = DataLen;
			tmpItemHead.num = SubNum;
			tmpItemHead.subLen = tmpItemHead.len/tmpItemHead.num;

			//ͷд��buf.
			memcpy(pBuf+currHeadPos, &tmpItemHead,  sizeof(NCFG_ITEM_HEAD));
			currHeadPos += sizeof(NCFG_ITEM_HEAD);
			currDataPos += itemHead.len;
		}
		//����
		else
		{
			while (NULL != itemHeadExPos)
			{
				NCFG_ITEM_HEAD_EX & node = m_itemList.GetNext(itemHeadExPos);

				if (itemHead.itemID == node.item_head.itemID)
				{
					if (0 != fseek(m_pFile, node.start_pos, SEEK_SET))
					{
						delete [] pBuf;
						pBuf = NULL;
						return false;
					}

					if (1 != fread(pBuf+currHeadPos, sizeof(NCFG_ITEM_HEAD), 1, m_pFile))
					{
						delete [] pBuf;
						pBuf = NULL;
						return false;
					}

					currHeadPos += sizeof(NCFG_ITEM_HEAD);

					if(itemHead.len > 0)
					{
						if (1 != fread(pBuf+currDataPos, itemHead.len, 1, m_pFile))
						{
							delete [] pBuf;
							pBuf = NULL;
							return false;
						}
					}

					//
					if ((NCFG_ITEM_ENCODE_MAJOR == itemHead.itemID) || (NCFG_ITEM_ENCODE_MINOR == itemHead.itemID) || (NCFG_ITEM_ENCODE_PHONE == itemHead.itemID))
					{
						bool needWrite=false;
						unsigned long chnnNum = m_cfgHead.localVideoInputNum;
						unsigned long totalRate =0;
						unsigned long maxChnnRate =0;//���ÿͨ��֡��
						unsigned long maxRate =0;//֡��֮��
						unsigned long bVideoSizeSpecial = CProduct::Instance()->LocalVideoSizeSpecial();


						if (VIDEO_FORMAT_NTSC == g_video_format)
						{
							totalRate = CProduct::Instance()->LocalTotalRateNtsc();
						}
						else
						{
							totalRate = CProduct::Instance()->LocalTotalRatePal();
						}
						
						ENCODE_INFO *pEncode = reinterpret_cast<ENCODE_INFO *>(pBuf+currDataPos);
						for (int i=0; i<m_cfgHead.localVideoInputNum; i++)
						{
							pEncode[i].rate = VRateToRate(pEncode[i].rate, g_video_format);
							maxRate+= UsedRate(pEncode[i].rate,VIDEO_SIZE(pEncode[i].resolution));
						}

						//printf("%s,%d,read %X, NCFG_ITEM_ENCODE_MAJOR==%x \n",__FILE__,__LINE__,itemHead.itemID,NCFG_ITEM_ENCODE_MAJOR);
						if ((NCFG_ITEM_ENCODE_MAJOR == itemHead.itemID) && (!m_bHaveCheckRate))
						{
							printf("%s,%d, first check NCFG_ITEM_ENCODE_MAJOR maxRate:%d,totalRate:%d\n ",__FILE__,__LINE__,maxRate,totalRate);
							m_bHaveCheckRate = true;
							if(maxRate <= totalRate)
							{			
								printf("%s,%d, no need change NCFG_ITEM_ENCODE_MAJOR\n",__FILE__,__LINE__);
								needWrite =false;
							}
							else
							{
								printf("@@@@%s,%d,NCFG_ITEM_ENCODE_MAJOR totalRate out\n",__FILE__,__LINE__);
								needWrite=true;	
								if(bVideoSizeSpecial)
								{
									printf("%s,%d,use defause NCFG_ITEM_ENCODE_MAJOR\n",__FILE__,__LINE__);
									unsigned long videoSize = CProduct::Instance()->LocalDefaVideoSize();
									unsigned char encodeType = CProduct::Instance()->LocalDefauVideoEncodeMode();
									unsigned char quality = CProduct::Instance()->LocalDefQuiltyValue();
									unsigned long hBitStream = CProduct::Instance()->LocalDefaBitRateValue();
									unsigned char rate = 0;
									if (VIDEO_FORMAT_PAL == g_video_format)
									{
										rate = CProduct::Instance()->LocalDefaChnnRateNumPal();
									}
									else
									{
										rate = CProduct::Instance()->LocalDefaChnnRateNumNtsc();
									}

									for (int i=0;i<m_cfgHead.localVideoInputNum;i++)
									{
										pEncode[i].resolution = videoSize;
										pEncode[i].rate = rate;
										pEncode[i].encodeType = encodeType;
										pEncode[i].quality =  quality;
										pEncode[i].lBitStream = 64;
										pEncode[i].hBitStream = hBitStream;
									}
								}
								else
								{
									printf("###%s,%d,use curren videosize\n",__FILE__,__LINE__);
									//���֡�ʳ�����֡�ʣ���Ϊƽ��ֵ
									for (int i=0;i<m_cfgHead.localVideoInputNum;i++)
									{
										maxChnnRate= RealRate(totalRate/chnnNum,VIDEO_SIZE(pEncode[i].resolution));
										if(maxChnnRate < pEncode[i].rate)
										{
											pEncode[i].rate =maxChnnRate;
										}
									}			
								}														
							}

							if(needWrite)
							{
								ENCODE_INFO *pWite = new ENCODE_INFO [m_cfgHead.localVideoInputNum];
								memcpy(pWite,pEncode,sizeof(ENCODE_INFO)*m_cfgHead.localVideoInputNum);
								for (int i=0;i<m_cfgHead.localVideoInputNum;i++)
								{
									pWite[i].rate =RateToVRate(pWite[i].rate,g_video_format);
								}
								if (0 != fseek(m_pFile, -itemHead.len, SEEK_CUR))
								{
									printf("%s,%d,err fseek\n", __FILE__, __LINE__);								
								}

								if (1 != fwrite(pWite, itemHead.len, 1, m_pFile))
								{
									printf("write config err\n");								
								}
								delete []pWite;
							}		
						}									
					}
					
					if((NCFG_ITEM_ENCODE_MAJOR_EX == itemHead.itemID) || (NCFG_ITEM_ENCODE_MINOR_EX == itemHead.itemID))
					{
						ENCODE_INFO_EX *pEncode = reinterpret_cast<ENCODE_INFO_EX *>(pBuf+currDataPos);
						for (int i=0; i<m_cfgHead.localVideoInputNum + m_cfgHead.netVideoInputNum; i++)
						{
							pEncode[i].rate = VRateToRate(pEncode[i].rate, g_video_format);
						}
					}
					
					if (NCFG_ITEM_IPC_SUPPORT_ENCODE == itemHead.itemID)
					{
						NCFG_IPC_SUPPORT_ENCODE * pSupportEncode = reinterpret_cast<NCFG_IPC_SUPPORT_ENCODE *>(pBuf+currDataPos);
						int rateTemp = 0;
						if (VIDEO_FORMAT_NTSC == g_video_format)
						{
							rateTemp = (pSupportEncode->maxChnnRate + 1) * 6 / 5;
							if (rateTemp > 30)
							{
								rateTemp = 30;
							}
							pSupportEncode->maxChnnRate = rateTemp;
							
							rateTemp = (pSupportEncode->maxChnnRateOnVGA + 1) * 6 / 5;
							pSupportEncode->maxChnnRateOnVGA = rateTemp;
							
							rateTemp = (pSupportEncode->maxTotalRateOnVGA + 1) * 6 / 5;
							pSupportEncode->maxTotalRateOnVGA = rateTemp;
							
							rateTemp = (pSupportEncode->defaChnnRate + 1) * 6 / 5;
							if (rateTemp > 30)
							{
								rateTemp = 30;
							}
							pSupportEncode->defaChnnRate = rateTemp;
						}
					}

					currDataPos += itemHead.len;

					break;
				} 
			}
		}
	}

	assert(dataLen == currDataPos);
	Len = dataLen;
	itemNum = num;
	*pDataBlock = pBuf;

	return true;
}

bool CConfigEx::InitialPTZ()
{
	return true;
}

void CConfigEx::CalculateSystemCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//NCFG_ITEM_SYSTEM_BASE=0x100,	
	num++; len += sizeof(ULONGLONG);			//NCFG_ITEM_MANUAL_REC				//�ֶ�¼��ULONGLONG
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_DEVICE_ID,				//�豸ID��unsigned long
	num++; len += sizeof(NCFG_INFO_SHORT_NAME);	//NCFG_ITEM_DEVICE_NAME,			//�豸���ƣ�NCFG_INFO_SHORT_NAME
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_VIDEO_FORMAT,			//��Ƶ��ʽ��unsigned long ,��Ƶ��ʽ
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_AUTHORIZATION,			//�Ƿ�Ȩ�޼��, unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_LIVE_AUDIO_ENABLE,		//�ֳ���Ƶ��unsigned long 
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_LIVE_AUDIO_CHNN,		//�ֳ���Ƶͨ��,unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_LIVE_AUDIO_VOLUME,		//ϵͳ����
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_AUDIO_BROADCAST_ENABLE,	//�Ƿ����������㲥, unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_DATE_FORMAT,			//���ڸ�ʽ��unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_HOUR_FORMAT,			//ʱ���ʽ��unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_LANGUAGE,				//���ԣ�unsigned long
	num++; len += sizeof(long);					//NCFG_ITEM_TIME_ZONE,				//ʱ����long
#ifdef __DVR_BASIC__
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_TIME_SYNC,				//�Ƿ�����ʱ��ͬ����unsigned long
	num++; len += sizeof(NCFG_INFO_LONG_NAME);	//NCFG_ITEM_TIME_SERVER,			//����ʱ��ͬ����������ַ��LONG_NAME
#endif
	num++; len += sizeof(OSD_INFO);				//NCFG_ITEM_LIVE_TIME_INFO,			//DVR�ֳ�ʱ��, OSD_INFO
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_SCRENN_SAVE_ENABLE,		//�Ƿ�������, unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_SCRENN_SAVE_TIME,		//����ʱ��, unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_REC_RECYCLE,			//�Ƿ񸲸�¼��unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_VGA_RESOLUTION,			//VGA�ֱ��ʣ�unsigned long���ο�dvrdvsdef.h�е�VGA_RESOLUTIONö��
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_VGA_REFRESH,			//VGA��Ļˢ����, unsigned long
	num++; len += (sizeof(NCFG_INFO_USER)*MAX_USER_NUM);//NCFG_ITEM_USER,			//�û���Ϣ��NCFG_INFO_USER,USER [MAX_USER_NUM]
	num++; len += sizeof(DAYLIGHT_INFO);		//NCFG_ITEM_DAYLIGHT_INFO,			//����ʱ��Ϣ��DAYLIGHT_INFO
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_MAX_CLIENT_NUM,			//��������û�����unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_TRANSPARENCY,			//�˵�͸���ȣ�unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_SHOW_WIZZARD,			//�����Ƿ���ʾ��
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_DISK_ALARM_FREE,        //���̱����ռ䣨���ǵ���������ռ䱨�����������԰��������ʾ��������,unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_NTP_PORT,				//NTP�������˿�,unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_NTP_PERIOD,				//Уʱ�����unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_LIVE_REC_STATUS,        //�ֳ��Ƿ���ʾ¼��״̬��unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_DODWELL					//�Ƿ���̨,unsigned long
	num++; len += sizeof(NCFG_INFO_TOOLBAR_POS);//NCFG_ITEM_TOOLBAR_POS				//������λ��,NCFG_INFO_TOOLBAR_POS
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_CHANN_VISIBLE           //δ��¼ ͨ���Ƿ�ɼ� unsigned long
	num++; len += sizeof(NCFG_INFO_GROUP_ENABLEINFO);        //NCFG_ITEM_SEARCH_GROUP_ENABLE     //ͨ�������Ƿ�����
	num++; len += (sizeof(NCFG_INFO_GROUP_ITEM) * MAX_GROUP_ITEM_COUNT);		//NCFG_ITEM_SEARCH_GROUP_ITEM	//ͨ�����鹦��
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_CVBS_VO_ADJUST,			//����CVBS������еļ���������ڣ�0�� 0��λ��ʱ����ֻ��汻�ڵ��������
	num++; len += sizeof(NCFG_CVBS_INFO);		//NCFG_ITEM_CVBS_TO_SPOT			//CVBS��ΪSPOT�� unsigned long
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_ENCODE_960H				//3520A��3521����960H��D1���ܻ��
	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadSystemCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos, bool bChangtoCVBS)
{
	CalculateSystemCfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	unsigned long currPos = 0;
	unsigned long value = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MANUAL_REC;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DEVICE_ID;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 0;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DEVICE_NAME;
	pItemHead->len = sizeof(NCFG_INFO_SHORT_NAME);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NCFG_INFO_SHORT_NAME);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#if defined(__CUSTOM_RU04__)
		strcpy((char *)(pBuf+currPos+itemHeadLen), "Grizzly");
#else
#if defined (__CUSTOM_GB31__)
	strcpy((char *)(pBuf+currPos+itemHeadLen), "ADEMCO");
#else
	strcpy((char *)(pBuf+currPos+itemHeadLen), "EDVR");
#endif
#endif
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIDEO_FORMAT;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);	
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		VIDEO_FORMAT videoFormatTemp = static_cast<VIDEO_FORMAT>(CLocalDevice::GetVideoFormatFromFlash()); //��ȡflash�����Ƶ��ʽ��Ϣ
		value = videoFormatTemp;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_AUTHORIZATION;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#if defined(__CUSTOM_RU04__)
		value = 0;
#else
		value = 1;
#endif
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LIVE_AUDIO_ENABLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#ifdef __NETSERVER_HXHT__
		//HXHT�汾Ĭ�Ͼ���
		value = NET_LIVE_AUDIO_MUTE;
#else
		value = NET_LIVE_AUDIO_OPEN;
#endif
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LIVE_AUDIO_CHNN;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 0;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LIVE_AUDIO_VOLUME;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 60;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_AUDIO_BROADCAST_ENABLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 0;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DATE_FORMAT;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#if defined (__CUSTOM_BR27__) || defined (__CUSTOM_RU04__)
		value = DATE_MODE_DMY;
#else
#if defined(__CUSTOM_YUANZHENG__) || defined(__TW01_RILI__)
		value = DATE_MODE_YMD;
#else
		value = DATE_MODE_MDY;
#endif
#endif
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HOUR_FORMAT;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = TIME_MODE_24;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;


	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LANGUAGE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#if defined(__LANGUAGE_JAPANESE__) || defined(__TW01_RILI__)
		value = LANGUAGE_JAPANESE;
#else
		value = CLanguagePack::Instance()->GetDefault();
#endif

#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__)
		value = LANGUAGE_CHINESE_S;
#endif

#if defined(__CUSTOM_CNB__)
		value = LANGUAGE_CHINESE_S;
#endif

#if defined(__KR24_IN__) && defined(__KR24_CN__)
		value = LANGUAGE_ENGLISH;
#endif

#if defined(__KR24_IN__) && !defined(__KR24_CN__) || defined(__KOREAN__)
		value = LANGUAGE_CHINESE_S;
#endif

		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_TIME_ZONE;
	pItemHead->len = sizeof(long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		long timeZone = TIME_ZONE_GMT;
#if   defined (__CUSTOM_BR27__)
		timeZone = TIME_ZONE_GMT_D3;
#elif defined (__CUSTOM_CN__) || defined (__CUSTOM_SHANGHAI__) || defined(__ZNV_SC__)
 		timeZone = TIME_ZONE_GMT_A8;

#elif defined (__LANGUAGE_JAPANESE__) || defined (__TW01_RILI__) 
		timeZone = TIME_ZONE_GMT_A9;
#else
       timeZone = TIME_ZONE_GMT;
#endif

#if defined (__CUSTOM_IN__) && defined (__KOREAN__)
		timeZone = TIME_ZONE_GMT_A9;
#endif

#if	defined (__CUSTOM_CNB__)
		timeZone = TIME_ZONE_GMT_A8;
#endif

#if	defined (__KR24_IN__)
		timeZone = TIME_ZONE_GMT_A9;
#endif
	   
	   //long timeZone = CLocalDevice::GetTimeZoneFromFlash();
		memcpy(pBuf+currPos+itemHeadLen, &timeZone, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_TIME_SYNC;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 0;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_TIME_SERVER;
	pItemHead->len = sizeof(NCFG_INFO_LONG_NAME);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NCFG_INFO_LONG_NAME);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#if defined(__CUSTOM_RU04__)
		strcpy((char*)(pBuf+currPos+itemHeadLen), "ru.pool.ntp.org");
#else
		strcpy((char*)(pBuf+currPos+itemHeadLen), "time.windows.com");
#endif
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LIVE_TIME_INFO;
	pItemHead->len = sizeof(OSD_INFO);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(OSD_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		OSD_INFO osdInfo;
		memset(&osdInfo, 0, sizeof(OSD_INFO));
		osdInfo.bEnable = 1;
		memcpy(pBuf+currPos+itemHeadLen, &osdInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SCRENN_SAVE_ENABLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 0;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SCRENN_SAVE_TIME;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long saveTime = 0;
		memcpy(pBuf+currPos+itemHeadLen, &saveTime, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_REC_RECYCLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 1;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VGA_RESOLUTION;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		if (bChangtoCVBS)
		{
			value = VGA_640X480;
		}
		else
		{
			value = CProduct::Instance()->GetDefualtResolution();
		}
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VGA_REFRESH;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 0;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_USER;
	pItemHead->len = (sizeof(NCFG_INFO_USER)*MAX_USER_NUM);
	pItemHead->num = MAX_USER_NUM;
	pItemHead->subLen = sizeof(NCFG_INFO_USER);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		NCFG_INFO_USER *pUserInfo = new NCFG_INFO_USER[MAX_USER_NUM];
		memset(pUserInfo, 0, sizeof(NCFG_INFO_USER)*MAX_USER_NUM);
		pUserInfo[0].group = USER_GROUP_ADMIN;
		memcpy(pUserInfo[0].name, "admin", sizeof("admin"));
#ifndef __CUSTOM_BR27__
		memcpy(pUserInfo[0].password, "123456", sizeof("123456"));
#else
 		memcpy(pUserInfo[0].password, "1", sizeof("1"));
#endif

#if defined(__CUSTOM_ZNV__)
		memcpy(pUserInfo[0].password, "12345", sizeof("12345"));
#endif

#if defined(__KR24_IN__)
		memcpy(pUserInfo[0].password, "1111", sizeof("1111"));
#endif

#if defined(__CUSTOM_CNB__)
		memcpy(pUserInfo[0].name, "root", sizeof("root"));
		memcpy(pUserInfo[0].password, "admin", sizeof("admin"));
#endif

#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
		memcpy(pUserInfo[0].password, "admins", sizeof("admins"));
#endif

		pUserInfo[0].authLiveCH			= ALL_CHNN_MASK(m_videoInputNum);
		pUserInfo[0].authBackupCH		= ALL_CHNN_MASK(m_videoInputNum);
		pUserInfo[0].authPlaybackCH		= ALL_CHNN_MASK(m_videoInputNum);
		pUserInfo[0].authPTZCtrlCH		= ALL_CHNN_MASK(m_videoInputNum);
		pUserInfo[0].authRecordCH		= ALL_CHNN_MASK(m_videoInputNum);
		pUserInfo[0].authRemoteViewCH	= ALL_CHNN_MASK(m_videoInputNum);
		pUserInfo[0].authority			= AUTH_ALL;

#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
		memcpy(pUserInfo[1].name, "666666", sizeof("666666"));
		memcpy(pUserInfo[1].password, "666666", sizeof("666666"));
		pUserInfo[1].group = USER_GROUP_NORMAL;
		pUserInfo[1].authLiveCH			= ALL_CHNN_MASK(m_localVideoInputNum);
		pUserInfo[1].authBackupCH		= ALL_CHNN_MASK(m_localVideoInputNum);
		pUserInfo[1].authPlaybackCH		= ALL_CHNN_MASK(m_localVideoInputNum);
		pUserInfo[1].authPTZCtrlCH		= ALL_CHNN_MASK(m_localVideoInputNum);
		pUserInfo[1].authRecordCH		= ALL_CHNN_MASK(m_localVideoInputNum);
		pUserInfo[1].authRemoteViewCH	= ALL_CHNN_MASK(m_localVideoInputNum);
		pUserInfo[1].authority			= (AUTH_BASIC | AUTH_LIVE | AUTH_RECORD | AUTH_PLAYBACK | AUTH_BACKUP | AUTH_PTZ_CTRL | AUTH_REMOTE_VIEW | AUTH_AUDIO_TALK | AUTH_REMOTE_LOGON);
#endif
		memcpy(pBuf+currPos+itemHeadLen, pUserInfo, pItemHead->len);

		delete [] pUserInfo;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DAYLIGHT_INFO;
	pItemHead->len = sizeof(DAYLIGHT_INFO);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(DAYLIGHT_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		DAYLIGHT_INFO daylight;
		memset(&daylight, 0, sizeof(DAYLIGHT_INFO));
		daylight.InYear = 2008;
		daylight.OutYear = 2008;
		daylight.InMday = 1;
		daylight.OutMday = 1;
		daylight.offSet = 1*60*60;
		memcpy(pBuf+currPos+itemHeadLen, &daylight, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MAX_CLIENT_NUM;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = CProduct::Instance()->MaxNetClientNum();
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_TRANSPARENCY;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = TRANSPARENCY_0;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHOW_WIZZARD;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 1;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISK_ALARM_FREE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 128; //MB
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NTP_PORT;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 22;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NTP_PERIOD;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 24; //H
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LIVE_REC_STATUS;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 1; //��ʾ
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DODWELL;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 0; //��̨
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_TOOLBAR_POS;
	pItemHead->len = sizeof(NCFG_INFO_TOOLBAR_POS);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NCFG_INFO_TOOLBAR_POS);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		NCFG_INFO_TOOLBAR_POS posInfo; //������λ����Ϣ
		memset(&posInfo, 0, sizeof(NCFG_INFO_TOOLBAR_POS));
		for(int i = 0; i < 32; i++)
		{
			posInfo.ItemIndex[i] = i;
		}
		posInfo.preDelayTime = 30;
		posInfo.postDelayTime = 300;
		memcpy(pBuf+currPos+itemHeadLen, &posInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CHANN_VISIBLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 0;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SEARCH_GROUP_ENABLE;
	pItemHead->len = sizeof(NCFG_INFO_GROUP_ENABLEINFO);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NCFG_INFO_GROUP_ENABLEINFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);	
	}	
	currPos += itemHeadLen+pItemHead->len;

	//-----------added by wn--2011.7.4------------
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SEARCH_GROUP_ITEM;
	pItemHead->len = MAX_GROUP_ITEM_COUNT * sizeof(NCFG_INFO_GROUP_ITEM);
	pItemHead->num = MAX_GROUP_ITEM_COUNT;
	pItemHead->subLen = sizeof(NCFG_INFO_GROUP_ITEM);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);		
	}	
	currPos += itemHeadLen+pItemHead->len;

	//
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CVBS_VO_ADJUST;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);		
	}
	currPos += itemHeadLen + pItemHead->len;

	//
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CVBS_TO_SPOT;
	pItemHead->len = sizeof(NCFG_CVBS_INFO);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NCFG_CVBS_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		NCFG_CVBS_INFO * pCVBSToSpot = (NCFG_CVBS_INFO *)(pBuf+currPos+itemHeadLen);
		pCVBSToSpot->current = 0;
		pCVBSToSpot->last = 0;
	}
	currPos += itemHeadLen + pItemHead->len;

	//
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_ENCODE_960H;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long * pEncode960H = (unsigned long *)(pBuf+currPos+itemHeadLen);
		* pEncode960H = CLocalDevice::Instance()->GetEncodeSize();
	}
	currPos += itemHeadLen + pItemHead->len;

	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateChannCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_CHANNEL_BASE=0x200,
	num++; len += (sizeof(NCFG_INFO_SHORT_NAME) * m_videoInputNum);	//NCFG_ITEM_CAMERA_NAME,		//��������ƣ�NCFG_INFO_SHORT_NAME[m_videoInputNum]
	num++; len += (sizeof(unsigned long) * m_videoInputNum);		//NCFG_ITEM_SHOW_CAM_NAME,		//DVR�ֳ��Ƿ���ʾ��������ƣ� unsigned long [m_videoInputNum]
	num++; len += (sizeof(PTZ_SERIAL_INFO) * m_videoInputNum);		//NCFG_ITEM_PTZ_SERIAL,			//ÿ��ͨ������̨��Э�顢��ַ�������� PTZ_SERIAL_INFO[m_videoInputNum]

#ifdef __DVR_BASIC__
	num++; len += sizeof(unsigned long);						//NCFG_ITEM_VIEW_SPLIT_MODE,	unsigned long
	num++; len += sizeof(unsigned long)*32;						//NCFG_ITEM_VIEW_DWELL_TIME,	unsigned long[32]
	num++; len += sizeof(unsigned long)*(32*1*1+2);				//NCFG_ITEM_VIEW_SPLIT_1X1,
	num++; len += sizeof(unsigned long)*(8*2*2+2);				//NCFG_ITEM_VIEW_SPLIT_2X2,
	num++; len += sizeof(unsigned long)*(11*2*3+2);				//NCFG_ITEM_VIEW_SPLIT_2X3
	num++; len += sizeof(unsigned long)*(6*3*3+2);				//NCFG_ITEM_VIEW_SPLIT_3X3,
	num++; len += sizeof(unsigned long)*(4*4*4+2);				//NCFG_ITEM_VIEW_SPLIT_4X4,
	num++; len += sizeof(unsigned long)*(4*5*5+2);				//NCFG_ITEM_VIEW_SPLIT_5X5,
	num++; len += sizeof(unsigned long)*(3*6*6+2);				//NCFG_ITEM_VIEW_SPLIT_6X6,
#endif

	num++; len += sizeof(unsigned long);								//NCFG_ITEM_SPOT_SPLIT_MODE,	unsigned long
	num++; len += sizeof(unsigned long);								//NCFG_ITEM_SPOT_DWELL_TIME,	unsigned long
	num++; len += sizeof(unsigned long)*(32*1*1+2);						//NCFG_ITEM_SPOT_SPLIT_1X1,

	num++; len += (sizeof(unsigned long)* m_videoInputNum);			//NCFG_ITEM_VIDEO_COLOR,			//unsigned long
	num++; len += (sizeof(OSD_INFO)* m_videoInputNum);				//NCFG_ITEM_TIME_STAMP_INFO,		//����ʱ���, OSD_INFO[m_videoInputNum]
	num++; len += (sizeof(OSD_INFO)* m_videoInputNum);				//NCFG_ITEM_CAM_NAME_STAMP_INFO,	//����ͨ������, OSD_INFO[m_videoInputNum]
	num++; len += (sizeof(CAMERA_COVER_INFO)* m_videoInputNum);		//NCFG_ITEM_CAM_COVER_INFO,			//ͼ���ڵ���Ϣ, CAMERA_COVER_INFO[m_videoInputNum]

	num++; len += (sizeof(VIDEO_CONFIG_ROI_INFO)*m_netVideoInputNum);   //NCFG_ITEM_CAM_ROI_INFO   //xian
	num++; len += (sizeof(unsigned char)*m_netVideoInputNum);           //NCFG_ITEM_CAM_FILP
	num++; len += (sizeof(unsigned char)*m_netVideoInputNum);           //NCFG_ITEM_CAM_MIRROR,	
	num++; len += (sizeof(unsigned long)*m_netVideoInputNum); 		    //NCFG_ITEM_CAM_ROTATO
    num++; len += (sizeof(LENS_DISTORTION_CONFIG)*m_netVideoInputNum);  //NCFG_ITEM_CAM_LENS_DISTORTION
	num++; len += (sizeof(unsigned char)*m_netVideoInputNum);           //NCFG_ITEM_CAM_TIME_FOCUS_MODE
    num++; len += (sizeof(unsigned char)*m_netVideoInputNum);           //NCFG_ITEM_CAM_IRCHANGE_FOCUS
	num++; len += (sizeof(unsigned long)*m_netVideoInputNum);           //NCFG_ITEM_CAM_INTERVAL_TIME
   
#ifdef __DVR_BASIC__
	num++; len += sizeof(unsigned long)*(4*4*6+2);						//NCFG_ITEM_VIEW_SPLIT_4X6,
#endif
	num++; len += (sizeof(unsigned long) * m_videoInputNum);		//NCFG_ITEM_PTZ_CURISE_TYPE			//ͨ��Ѳ������������unsigned long [VIDEO_INPUT_NUM]

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadChannCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos)
{
	CalculateChannCfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAMERA_NAME;
	pItemHead->len = (sizeof(NCFG_INFO_SHORT_NAME)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(NCFG_INFO_SHORT_NAME);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		NCFG_INFO_SHORT_NAME *pCameraName = new NCFG_INFO_SHORT_NAME [m_videoInputNum];
		for (int i=0; i<m_videoInputNum; i++)
		{
	#if defined(__CUSTOM_RU04__)
			sprintf(pCameraName[i].name, "%s%d", "KAM#", i+1);
	#else
			if(i < m_localVideoInputNum)
			{
				sprintf(pCameraName[i].name, "%s%02d", "CAMERA", i + 1);
			}
			else
			{
				sprintf(pCameraName[i].name, "%s%02d", "DEVICE", i + 1 - m_localVideoInputNum);
			}
	#endif
		}
		memcpy(pBuf+currPos+itemHeadLen, pCameraName, pItemHead->len);
		delete [] pCameraName;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHOW_CAM_NAME;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long *pShowCameraName = new unsigned long[m_videoInputNum];
		for (int i=0; i<m_videoInputNum; i++)
		{
			pShowCameraName[i] = 1;
		}
		memcpy(pBuf+currPos+itemHeadLen, pShowCameraName, pItemHead->len);
		delete [] pShowCameraName;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_PTZ_SERIAL;
	pItemHead->len = (sizeof(PTZ_SERIAL_INFO)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(PTZ_SERIAL_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned char baudRate = SBR_9600;
		unsigned char protocol = PROTOCOL_TVI;
		
		PTZ_SERIAL_INFO *pPTZConfig = new PTZ_SERIAL_INFO[m_videoInputNum];
		for (int i=0; i<m_videoInputNum; i++)
		{
			pPTZConfig[i].bEnable = 1;
			pPTZConfig[i].baudRate = baudRate;
			pPTZConfig[i].devAddr = i+1;
			pPTZConfig[i].protocol = protocol;
		}

#if defined(__LANGUAGE_JAPANESE__)
		if (TD_2516HS_BL == CProduct::Instance()->ProductType())
		{
			pPTZConfig[0].baudRate = SBR_2400;
			pPTZConfig[0].protocol = PROTOCOL_PELCOD;
		}
#endif

		memcpy(pBuf+currPos+itemHeadLen, pPTZConfig, pItemHead->len);
		delete [] pPTZConfig;
	}
	currPos += itemHeadLen+pItemHead->len;
	//
	unsigned long value = VIEW_SPLIT_1X1;
	unsigned long chBuffLen = 128;
	unsigned long chInfo[128];

#ifdef __DVR_BASIC__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIEW_SPLIT_MODE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		int split[] = {1, 4, 6, 9, 16, 25, 36};
		unsigned long split_mode[] = {VIEW_SPLIT_1X1, VIEW_SPLIT_2X2, VIEW_SPLIT_2X3, VIEW_SPLIT_3X3, VIEW_SPLIT_4X4, VIEW_SPLIT_5X5, VIEW_SPLIT_6X6};
		int splitNum = (sizeof(split)/sizeof(int) - 1);
		unsigned char VideoInputNum = IsSupportHybridDisplay() ? m_videoInputNum:m_localVideoInputNum;		
		for(int i = splitNum; i >= 0; --i)
		{
			if(VideoInputNum <= split[i])
			{
				value = split_mode[i];
			}
		}
		//value = VIEW_SPLIT_1X1;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIEW_DWELL_TIME;
	pItemHead->len = sizeof(unsigned long) * 32;
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long) * 32;
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long dwellTime[32];
		for (int i = 0; i < 32; i++)
		{
			dwellTime[i] = 5;
		}
		memcpy(pBuf+currPos+itemHeadLen, dwellTime, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIEW_SPLIT_1X1;
	pItemHead->len = sizeof(unsigned long)*(32*1*1+2);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long)*(32*1*1+2);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		memset(&chInfo,0xFF,sizeof(unsigned long)*chBuffLen);
		chInfo[0]=32;
		chInfo[1]=m_videoInputNum;
		for (unsigned char i=0; i<m_videoInputNum; ++i)
		{
			chInfo[i+2] = i;
		}
		memcpy(pBuf+currPos+itemHeadLen, chInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIEW_SPLIT_2X2;
	pItemHead->len = sizeof(unsigned long)*(8*2*2+2);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long)*(8*2*2+2);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		memset(&chInfo,0xFF,sizeof(unsigned long)*chBuffLen);
		chInfo[0]=8;
		chInfo[1]=(m_videoInputNum + 3)/4;
		for (unsigned char i=0; i<m_videoInputNum; ++i)
		{
			chInfo[i+2] = i;
		}
		memcpy(pBuf+currPos+itemHeadLen, chInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIEW_SPLIT_2X3;
	pItemHead->len = sizeof(unsigned long)*(11*2*3+2);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long)*(11*2*3+2);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		chInfo[0]=11;
		chInfo[1]=(m_videoInputNum + 5)/6;
		for (unsigned char i=0; i<m_videoInputNum; ++i)
		{
			chInfo[i+2] = i;
		}
		memcpy(pBuf+currPos+itemHeadLen, chInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIEW_SPLIT_3X3;
	pItemHead->len = sizeof(unsigned long)*(6*3*3+2);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long)*(6*3*3+2);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		chInfo[0]=6;
		chInfo[1]=(m_videoInputNum + 8)/9;
		for (unsigned char i=0; i<m_videoInputNum; ++i)
		{
			chInfo[i+2] = i;
		}
		memcpy(pBuf+currPos+itemHeadLen, chInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIEW_SPLIT_4X4;
	pItemHead->len = sizeof(unsigned long)*(4*4*4+2);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long)*(4*4*4+2);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		chInfo[0]=4;
		chInfo[1]=(m_videoInputNum + 15)/16;
		for (unsigned char i=0; i<m_videoInputNum; ++i)
		{
			chInfo[i+2] = i;
		}
		memcpy(pBuf+currPos+itemHeadLen, chInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIEW_SPLIT_5X5;
	pItemHead->len = sizeof(unsigned long)*(4*5*5+2);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long)*(4*5*5+2);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		chInfo[0]=4;
		chInfo[1]=(m_videoInputNum + 24)/25;
		for (unsigned char i=0; i<m_videoInputNum; ++i)
		{
			chInfo[i+2] = i;
		}
		memcpy(pBuf+currPos+itemHeadLen, chInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIEW_SPLIT_6X6;
	pItemHead->len = sizeof(unsigned long)*(3*6*6+2);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long)*(3*6*6+2);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		chInfo[0]=3;
		chInfo[1]=(m_videoInputNum + 35)/36;
		for (unsigned char i=0; i<m_videoInputNum; ++i)
		{
			chInfo[i+2] = i;
		}
		memcpy(pBuf+currPos+itemHeadLen, chInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SPOT_SPLIT_MODE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = VIEW_SPLIT_1X1;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SPOT_DWELL_TIME;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		value = 5;
		memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SPOT_SPLIT_1X1;
	pItemHead->len = sizeof(unsigned long)*(32*1*1+2);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long)*(32*1*1+2);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		memset(&chInfo,0xFF,sizeof(unsigned long)*chBuffLen);
		chInfo[0]=32;
		chInfo[1]=m_videoInputNum;
		for (unsigned char i=0; i<m_videoInputNum; ++i)
		{
			chInfo[i+2] = i;
		}
		memcpy(pBuf+currPos+itemHeadLen, chInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	//
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIDEO_COLOR;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		m_bCreatNewFile = true;
		unsigned long *pValue = new unsigned long [m_videoInputNum];
		VIDEO_FORMAT videoFormatTemp = static_cast<VIDEO_FORMAT>(CLocalDevice::GetVideoFormatFromFlash()); //��ȡflash�����Ƶ��ʽ��Ϣ
		
#ifdef __ENVIRONMENT_LINUX__
#if defined(__SPECIAL_COLOR)
		if (CLocalDevice::Is2316SSNewPCB()) //2316SS������ʹ�õ���1108,
		{
			for (int i = 0; i < m_localVideoInputNum; ++i)
			{
				pValue[i] = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_1108 : DEFAULT_COLOR_PAL_1108;
			}
		}
		else
		{
			for (int i = 0; i < m_localVideoInputNum; ++i)
			{
				pValue[i] = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_25838 : DEFAULT_COLOR_PAL_25838;
			}
		}
#else
		if (g_bUse1104A1108ChipColor)
		{
			for (int i = 0; i < m_localVideoInputNum; ++i)
			{
				pValue[i] = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_1104 : DEFAULT_COLOR_PAL_1104;
			}
		}
		else
		{
			for (int i = 0; i < m_localVideoInputNum; ++i)
			{
				pValue[i] = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_2864 : DEFAULT_COLOR_PAL_2864;
			}
		}
#endif
#if defined(__CHIP3520A__) || defined(__CHIP3521__) || defined(__HISI_SDK_0BX__) || defined(__CHIPGM__) || defined(__CHIP3520D__)
		char bright = 0;
		char hue = 0;
		char sat =0;
		char contrast = 0;
		unsigned long tempValue = 0;
		printf("bright=%d, hue=%d, sat=%d, contrast=%d",bright,hue,sat,contrast);
		printf("%s,  %d",__FILE__,__LINE__);
        
		if ( true == g_ext_init_interface)
		{
#if defined(__CHIPGM__)
			if (0 == ext_get_video_Def_Color(&bright, &hue, &sat, &contrast))
#else
			bright = 128;
			contrast = 128;
			sat = 128;
			hue =128;
#endif
			{
				tempValue = (bright << 24) + (contrast << 16) + (sat << 8) + hue;
				printf("%s,  %d,  bright=%d, hue=%d, sat=%d, contrast=%d",__FILE__,__LINE__,bright,hue,sat,contrast);
				printf("%s,  %d",__FILE__,__LINE__);
			}

			for (int i = 0; i < m_localVideoInputNum; ++i)
			{
				pValue[i] = tempValue;
			}
		}
#endif
#else
		for (int i = 0; i < m_localVideoInputNum; ++i)
		{
			pValue[i] = (VIDEO_FORMAT_NTSC == videoFormatTemp) ? DEFAULT_COLOR_NTSC_2864 : DEFAULT_COLOR_PAL_2864;
		}
#endif

		if ((0 < m_localVideoInputNum) && (m_localVideoInputNum < m_videoInputNum))
		{
			for(int i = 0; i < m_localVideoInputNum; i++)
			{
				pValue[i] = (128 << 24) + (128 << 16) + (128 << 8) + 128;
			}
		}

		for(int i = m_localVideoInputNum; i < m_videoInputNum; i++)
		{
			pValue[i] = (128 << 24) + (128 << 16) + (128 << 8) + 128;
		}
		memcpy(pBuf+currPos+itemHeadLen, pValue, pItemHead->len);
		delete [] pValue;
	}
	currPos += itemHeadLen+pItemHead->len;
	
	OSD_INFO *pOSDInfo = new OSD_INFO [m_videoInputNum];
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_TIME_STAMP_INFO;
	pItemHead->len = (sizeof(OSD_INFO)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(OSD_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		memset(pOSDInfo, 0, sizeof(OSD_INFO)*m_videoInputNum);
		for (int i=0;i<m_videoInputNum;i++)
		{
			pOSDInfo[i].bEnable = 1;
#ifdef __SUPPORT_OSDTWOLINES__
			pOSDInfo[i].x = 5;
			pOSDInfo[i].y = 5;
#else
			pOSDInfo[i].x = 43;
			pOSDInfo[i].y = 93;
#endif
		}
		memcpy(pBuf+currPos+itemHeadLen, pOSDInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAM_NAME_STAMP_INFO;
	pItemHead->len = (sizeof(OSD_INFO)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(OSD_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		for (int i=0;i<m_videoInputNum;i++)
		{
			pOSDInfo[i].bEnable = 1;
#ifdef __SUPPORT_OSDTWOLINES__
			pOSDInfo[i].x = 80;
			pOSDInfo[i].y = 93;
#else
			pOSDInfo[i].x = 5;
			pOSDInfo[i].y = 5;
#endif
		}
		memcpy(pBuf+currPos+itemHeadLen, pOSDInfo, pItemHead->len);
	}

	delete [] pOSDInfo;

	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAM_COVER_INFO;
	pItemHead->len = (sizeof(CAMERA_COVER_INFO)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(CAMERA_COVER_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VIEW_SPLIT_4X6;
	pItemHead->len = sizeof(unsigned long)*(4*4*6+2);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long)*(4*4*6+2);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		chInfo[0]=4;
		chInfo[1]=(m_videoInputNum + 23)/24;
		for (unsigned char i=0; i<m_videoInputNum; ++i)
		{
			chInfo[i+2] = i;
		}
		memcpy(pBuf+currPos+itemHeadLen, chInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID	= NCFG_ITEM_PTZ_CURISE_TYPE;
	pItemHead->len		= (sizeof(unsigned long) * m_videoInputNum);
	pItemHead->num		= m_videoInputNum;
	pItemHead->subLen	= sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);			//Ĭ��ֵΪʵ��Ѳ����
	}
	currPos += itemHeadLen+pItemHead->len;

	VIDEO_CONFIG_ROI_INFO *pROI= new VIDEO_CONFIG_ROI_INFO[m_netVideoInputNum];
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAM_ROI_INFO;
	pItemHead->len = (sizeof(VIDEO_CONFIG_ROI_INFO)*m_netVideoInputNum);
	pItemHead->num = m_netVideoInputNum;
	pItemHead->subLen = sizeof(VIDEO_CONFIG_ROI_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf+currPos, ppBackupConfig, backupLen ,pos);
	if (!bSuccess)
	{
		memset(pROI, 0, pItemHead->len);
		for(unsigned int i=0;i<m_netVideoInputNum;++i){
			for(unsigned int j=0;j<MAX_VIDEO_CONFIG_ROI_NUM;++j){
				pROI[i].roi[j].iLevel = 1;
			}
		}
		memcpy(pBuf+currPos+itemHeadLen, pROI, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAM_MIRROR;
	pItemHead->len = (sizeof(unsigned char)*m_netVideoInputNum);
	pItemHead->num = m_netVideoInputNum;
	pItemHead->subLen = sizeof(unsigned char);
	bSuccess = GetItemDataFromBackupConfig(pBuf+currPos, ppBackupConfig, backupLen ,pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAM_FILP;
	pItemHead->len = (sizeof(unsigned char)*m_netVideoInputNum);
	pItemHead->num = m_netVideoInputNum;
	pItemHead->subLen = sizeof(unsigned char);
	bSuccess = GetItemDataFromBackupConfig(pBuf+currPos, ppBackupConfig, backupLen ,pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAM_ROTATO;
	pItemHead->len = (sizeof(unsigned long)*m_netVideoInputNum);
	pItemHead->num = m_netVideoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf+currPos, ppBackupConfig, backupLen ,pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAM_LENS_DISTORTION;
	pItemHead->len = (sizeof(LENS_DISTORTION_CONFIG)*m_netVideoInputNum);
	pItemHead->num = m_netVideoInputNum;
	pItemHead->subLen = sizeof(LENS_DISTORTION_CONFIG);
	bSuccess = GetItemDataFromBackupConfig(pBuf+currPos, ppBackupConfig, backupLen ,pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAM_TIME_FOCUS_MODE;
	pItemHead->len = (sizeof(unsigned char)*m_netVideoInputNum);
	pItemHead->num = m_netVideoInputNum;
	pItemHead->subLen = sizeof(unsigned char);
	bSuccess = GetItemDataFromBackupConfig(pBuf+currPos, ppBackupConfig, backupLen ,pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAM_IRCHANGE_FOCUS;
	pItemHead->len = (sizeof(unsigned char)*m_netVideoInputNum);
	pItemHead->num = m_netVideoInputNum;
	pItemHead->subLen = sizeof(unsigned char);
	bSuccess = GetItemDataFromBackupConfig(pBuf+currPos, ppBackupConfig, backupLen ,pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;  

    pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAM_INTERVAL_TIME;
	pItemHead->len = (sizeof(unsigned long)*m_netVideoInputNum);
	pItemHead->num = m_netVideoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf+currPos, ppBackupConfig, backupLen ,pos);
	if (!bSuccess)
	{
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len; 

	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateChannRecCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_CHANNEL_REC_BASE=0x240,   //ͨ��¼������
	num++; len += (sizeof(unsigned long)*m_videoInputNum);	//NCFG_ITEM_REC_ENABLE,				//¼���ܿ��أ�unsigned long [m_videoInputNum]
	num++; len += (sizeof(unsigned long)*m_videoInputNum);	//NCFG_ITEM_PRE_ALARM_REC_TIME		//Ԥ¼��ʱ�䣨�룩��unsigned long[m_videoInputNum]
	num++; len += (sizeof(unsigned long)*m_videoInputNum);	//NCFG_ITEM_POST_ALARM_REC_TIME,	//����¼�񱣳�ʱ�䣨�룩��unsigned long[m_videoInputNum]
	num++; len += (sizeof(unsigned long)*m_videoInputNum);	//NCFG_ITEM_REC_HOLD_TIME			//¼�����ݱ����ڣ��룩��unsigned long[m_videoInputNum]
	num++; len += (sizeof(unsigned long)*(m_audioInputNum+m_netVideoInputNum));	//NCFG_ITEM_REC_AUDIO,              //��ͨ���Ƿ�¼��Ƶ��unsigned long [m_audioInputNum]
	num++; len += (sizeof(unsigned long)*m_videoInputNum);	//NCFG_ITEM_REC_REDUNDANT,			//��ͨ���Ƿ�¼����¼��unsigned long [m_videoInputNum]	

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}


void CConfigEx::LoadChannRecCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos)
{
	CalculateChannRecCfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_REC_ENABLE;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long *pRecEnable = new unsigned long [m_videoInputNum];
	for (int i=0; i<m_videoInputNum; i++)
	{
		pRecEnable[i] = 1;
	}
	memcpy(pBuf+currPos+itemHeadLen, pRecEnable, pItemHead->len);
	delete [] pRecEnable;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_PRE_ALARM_REC_TIME;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long* pPreRecTime = new unsigned long[m_videoInputNum];
	for(int i=0; i<m_videoInputNum; i++)
	{
		pPreRecTime[i] = CProduct::Instance()->MaxPreAlarmTime();
	}
	memcpy(pBuf+currPos+itemHeadLen, pPreRecTime, pItemHead->len);
	delete [] pPreRecTime;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_POST_ALARM_REC_TIME;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long* pPostRecTime = new unsigned long[m_videoInputNum];
	for ( int i=0; i<m_videoInputNum; i++ )
	{
		pPostRecTime[i] = 30;//��
	}
	memcpy(pBuf+currPos+itemHeadLen, pPostRecTime, pItemHead->len);
	delete [] pPostRecTime;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_REC_HOLD_TIME;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long* pRecHoldTime = new unsigned long[m_videoInputNum];
	for ( int i=0; i<m_videoInputNum; i++ )
	{
		pRecHoldTime[i] = 0;//ONE_DAY_SECOND_NUM*7;//һ��
	}
	memcpy(pBuf+currPos+itemHeadLen, pRecHoldTime, pItemHead->len);
	delete [] pRecHoldTime;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_REC_AUDIO;
	pItemHead->len = (sizeof(unsigned long)*(m_audioInputNum+m_netVideoInputNum));
	pItemHead->num = (m_audioInputNum+m_netVideoInputNum);
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long *pRecAudio = new unsigned long [(m_audioInputNum+m_netVideoInputNum)];
	memset(pRecAudio, 1, pItemHead->len);
	for(int i=0;i<m_audioInputNum+m_netVideoInputNum;i++)
	{
		pRecAudio[i]=1;
	}
	memcpy(pBuf+currPos+itemHeadLen, pRecAudio, pItemHead->len);
	delete [] pRecAudio;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_REC_REDUNDANT;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#ifdef __CUSTOM_YUANZHENG__
		unsigned long *pRecRed = new unsigned long [m_videoInputNum];
		for (int i = 0; i < m_videoInputNum; ++i)
		{
			pRecRed[i] = 1;
		}
		memcpy(pBuf+currPos+itemHeadLen, pRecRed, pItemHead->len);
		delete [] pRecRed;
#else
	unsigned long *pRecRed = new unsigned long [m_videoInputNum];
	memset(pRecRed, 0, pItemHead->len);
	memcpy(pBuf+currPos+itemHeadLen, pRecRed, pItemHead->len);
	delete [] pRecRed;
#endif
	}
	currPos += itemHeadLen+pItemHead->len;

	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateScheduleCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_SCHEDULE_BASE=0x300,
	num++; len += (sizeof(WEEK_SCHEDULE)*m_videoInputNum);	//NCFG_ITEM_REC_SCH_SCHEDULE_WEEK,	//�ų�¼���ܼƻ��� WEEK_SCHEDULE[m_videoInputNum]
	num++; len += (sizeof(WEEK_SCHEDULE)*m_videoInputNum);	//NCFG_ITEM_REC_SCH_MOTION_WEEK,	//�ƶ����¼���ܼƻ��� WEEK_SCHEDULE[m_videoInputNum]
	num++; len += (sizeof(WEEK_SCHEDULE)*m_videoInputNum);	//NCFG_ITEM_REC_SCH_SENSOR_WEEK,	//����������¼���ܼƻ���WEEK_SCHEDULE[m_videoInputNum]
	num++; len += (sizeof(WEEK_SCHEDULE)*m_sensorInputNum);	//NCFG_ITEM_SENSOR_IN_SCH_WEEK,		//����������ܼƻ��� WEEK_SCHEDULE[m_sensorInputNum]
	num++; len += (sizeof(WEEK_SCHEDULE)*m_sensorOutputNum);//NCFG_ITEM_ALARMOUT_SCH_WEEK,		//��������ܼƻ�WEEK_SCHEDULE[m_sensorOutputNum]
	num++; len += (sizeof(WEEK_SCHEDULE)*m_videoInputNum);	//NCFG_ITEM_MOTION_SCH_WEEK,		//�ƶ�����ܼƻ�WEEK_SCHEDULE[m_videoInputNum]
	num++; len += (sizeof(WEEK_SCHEDULE)*m_videoInputNum);	//NCFG_ITEM_NET_ALARM_SCH_WEEK,		//���籨���ܼƻ�WEEK_SCHEDULE[m_videoInputNum]

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadScheduleCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos)
{
	CalculateScheduleCfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_REC_SCH_SCHEDULE_WEEK;
	pItemHead->len = (sizeof(WEEK_SCHEDULE)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(WEEK_SCHEDULE);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

		WEEK_SCHEDULE *pRecSSWeekValue = new WEEK_SCHEDULE[m_videoInputNum];
#if defined(__CUSTOM_RU04__)
		memset(pRecSSWeekValue, 0xff, sizeof(WEEK_SCHEDULE)*m_videoInputNum);	
#elif defined(__TW01_RILI__)
		memset(pRecSSWeekValue, 0xff, sizeof(WEEK_SCHEDULE)*m_videoInputNum);
#else
		memset(pRecSSWeekValue, 0, sizeof(WEEK_SCHEDULE)*m_videoInputNum);

#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
		memset(pRecSSWeekValue, 0xff, sizeof(WEEK_SCHEDULE) * 2);
#endif

#endif

#if defined(__KR24_IN__)
		memset(pRecSSWeekValue, 0xff, sizeof(WEEK_SCHEDULE)*m_videoInputNum);
#endif
		memcpy(pBuf+currPos+itemHeadLen, pRecSSWeekValue, pItemHead->len);
		delete [] pRecSSWeekValue;
		pRecSSWeekValue = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_REC_SCH_MOTION_WEEK;
	pItemHead->len = (sizeof(WEEK_SCHEDULE)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(WEEK_SCHEDULE);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	WEEK_SCHEDULE *pRecMWeekValue = new WEEK_SCHEDULE[m_videoInputNum];
	memset(pRecMWeekValue, 0xff, sizeof(WEEK_SCHEDULE)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pRecMWeekValue, pItemHead->len);
	delete [] pRecMWeekValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_REC_SCH_SENSOR_WEEK;
	pItemHead->len = (sizeof(WEEK_SCHEDULE)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(WEEK_SCHEDULE);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	WEEK_SCHEDULE *pRecSWeekValue = new WEEK_SCHEDULE[m_videoInputNum];
	memset(pRecSWeekValue, 0xff, sizeof(WEEK_SCHEDULE)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pRecSWeekValue, pItemHead->len);
	delete [] pRecSWeekValue;
	}
	currPos += itemHeadLen+pItemHead->len;
//////////////////////////////////////////////////////////////////////////
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_SCH_WEEK;
	pItemHead->len = (sizeof(WEEK_SCHEDULE)*m_sensorInputNum);
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(WEEK_SCHEDULE);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	WEEK_SCHEDULE *pSensorWeekValue = new WEEK_SCHEDULE[m_sensorInputNum];
	memset(pSensorWeekValue, 0xff, sizeof(WEEK_SCHEDULE)*m_sensorInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pSensorWeekValue, pItemHead->len);
	delete [] pSensorWeekValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_ALARMOUT_SCH_WEEK;
	pItemHead->len = (sizeof(WEEK_SCHEDULE)*m_sensorOutputNum);
	pItemHead->num = m_sensorOutputNum;
	pItemHead->subLen = sizeof(WEEK_SCHEDULE);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	WEEK_SCHEDULE *pAlarmOutWeekValue = new WEEK_SCHEDULE[m_sensorOutputNum];
	memset(pAlarmOutWeekValue, 0xff, sizeof(WEEK_SCHEDULE)*m_sensorOutputNum);
	memcpy(pBuf+currPos+itemHeadLen, pAlarmOutWeekValue, pItemHead->len);
	delete [] pAlarmOutWeekValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_SCH_WEEK;
	pItemHead->len = (sizeof(WEEK_SCHEDULE)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(WEEK_SCHEDULE);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	WEEK_SCHEDULE *pMotionWeekValue = new WEEK_SCHEDULE[m_videoInputNum];
	memset(pMotionWeekValue, 0xff, sizeof(WEEK_SCHEDULE)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pMotionWeekValue, pItemHead->len);
	delete [] pMotionWeekValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_ALARM_SCH_WEEK;
	pItemHead->len = (sizeof(WEEK_SCHEDULE)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(WEEK_SCHEDULE);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	WEEK_SCHEDULE *pNetAlarmWeekValue = new WEEK_SCHEDULE[m_videoInputNum];
	memset(pNetAlarmWeekValue, 0xff, sizeof(WEEK_SCHEDULE)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pNetAlarmWeekValue, pItemHead->len);
	delete [] pNetAlarmWeekValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateNetBaseCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	num++; len += sizeof(unsigned long);					//NCFG_ITEM_UPNP_ENABLE				//����UPNP����
	num++; len += sizeof(unsigned long);					//NCFG_ITEM_HTTP_PORT,				//HTTP �˿�, unsigned long
	num++; len += sizeof(unsigned long);					//NCFG_ITEM_SERVER_PORT,			//�������˿ڣ� unsigned long
	num++; len += sizeof(NETWORK_ADDR_INFO);				//NCFG_ITEM_NETWORK_ADDR_INFO,	//�����ַ�����Ϣ,	NETWORK_ADDR_INFO
	num++; len += sizeof(unsigned long);					//NCFG_ITEM_DNS_1,					//unsigned long
	num++; len += sizeof(unsigned long);					//NCFG_ITEM_DNS_2,					//unsigned long
	num++; len += sizeof(DDNS_INFO);						//NCFG_ITEM_DDNS_INFO��				//DDNS��Ϣ��DDNS_INFO
	num++; len += sizeof(PPPOE_INFO);						//NCFG_ITEM_PPPOE_INFO,					//PPPoE��Ϣ,	PPPOE_INFO
#ifdef __DVR_BASIC__	
	num++; len += sizeof(MAIL_SEND_INFO);					//NCFG_ITEM_EMAIL_SEND_INFO,			//�ʼ����ܷ��ͷ���Ϣ,MAIL_SEND_INFO
	num++; len += (sizeof(MAIL_RECV_INFO)*MAX_MAIL_RECV_NUM);	//NCFG_ITEM_EMAIL_RECV_INFO,			//�ʼ����ܽ��շ���Ϣ,MAIL_RECV_INFO[MAX_MAIL_RECV_NUM]
	num++; len += sizeof(unsigned long);	                //NCFG_ITEM_EMAIL_PIC_ACC,			
#endif
	num++; len += sizeof(unsigned long);					//NCFG_ITEM_DDNS_UPDATE_CYCLE,		//ddns���������˸��µ����ڣ���λΪ��

	num++; len += sizeof(unsigned long);					//NCFG_ITEM_BAN_ALLOW_TYPE��		//�ڰ���������
	num++; len += (sizeof(IP_SECTION)*MAX_BAN_ALLOW_LIST_NUM);					//NCFG_ITEM_BAN_ALLOW_LIST			//�ڰ������б�

#ifdef __SUPPORT_HRSST__
	num++; len += sizeof(unsigned long);  //NCFG_ITEM_HRSST_SERVER_IP,          //����ͨ������IP��ַ		
	num++; len += sizeof(unsigned long);  //NCFG_ITEM_HRSST_SERVER_PORT,        //����ͨ�������˿�
#endif
#ifdef __NETSERVER_HXHT__
	num++; len += sizeof(HXHT_NET_PARAM_INFO);				//NCFG_ITEM_HXHT_NET_PARAM ���Ż�ͨ�����豸����
#endif
	
	num++; len += sizeof(MDVR_WIRELESS_CFG);                   //NCFG_ITEM_NET_WIRELESS
	num++; len += sizeof(NET_AUTO_REPORT);                     //NCFG_ITEM_AUTO_REPORT,          //�Զ��ϱ�		

	num++; len += 132;										//NCFG_ITEM_MINT_DDNS_SERVER_NAME, mint ddns ���ͷ����������� 132�ֽڳ�
	num++;len += sizeof(unsigned long);						//NCFG_ITEM_USE_3G  ҳ����ʾ����ip���û�������3G

	num++; len += sizeof(USE_IPV4_IPV6);					//NCFG_ITEM_USE_IPV4_IPV6,		//IPV4��IPV6�Ƿ�����, USE_IPV4_IPV6
	num++; len += sizeof(NETWORK_ADDR_INFO_IPv6);		//NCFG_ITEM_NETWORK_ADDR_INFO_IPV6,		//IPV6������Ϣ, NETWORK_ADDR_INFO_IPv6
	num++; len += sizeof(unsigned char[40]);					//NCFG_ITEM_DNS_1_IPV6,					//unsigned char[40]
	num++; len += sizeof(unsigned char[40]);	
	num++; len += sizeof(YUANZHENG_PARAM);				//NCFG_ITEM_NET_YZ_PARAM
	num++; len += sizeof(YUANZHENG_SNAP_PARAM);			//NCFG_ITEM_NET_YZ_SNAP_PARAM
#if defined(__SUPPORT_SNAPD1__)
	num++; len += sizeof(SNAP_INFOEX)*MAX_DAILY_SNAP_NUM;				//NCFG_ITEM_NET_SHANGHAI_DAILY_IMAGE
	num++; len += (sizeof(unsigned long)*m_videoInputNum);				//NCFG_ITEM_NET_SHANGHAI_PRE_ALARM_SNAP_TIME
	num++; len += (sizeof(unsigned long)*m_videoInputNum);				//NCFG_ITEM_NET_SHANGHAI_POST_ALARM_SNAP_TIME
	num++; len += sizeof(unsigned long);								//NCFG_ITEM_NET_SHANGHAI_MOTIONUPLOAD_ENABLE
	num++; len += sizeof(unsigned long);								//NCFG_ITEM_NET_SHANGHAI_PTENABLE
	num++; len += (sizeof(char [16]));									//NCFG_ITEM_NET_SHANGHAI_AGENTID
	num++; len += (sizeof(char [20]));									//NCFG_ITEM_NET_SHANGHAI_IP
	num++; len += sizeof(unsigned long);								//NCFG_ITEM_NET_SHANGHAI_PORT
#endif
	num++; len += sizeof(TVT_FTP_CONFIG);				//NCFG_ITEM_NET_FTP
	num++; len += sizeof(NAT_SERVER_INFO);				//NCFG_ITEM_NAT_SERVER
	num++; len += sizeof(PUSH_SERVER_INFO);				//NCFG_ITEM_PUSH_SERVER

	if(CProduct::Instance()->PoeInputNum() > 0)
	{
		num++; len += sizeof(NET_SWITCH_INFO);				//NCFG_ITEM_SWICH_INFO
	}

	num++; len += sizeof(unsigned long);				//NCFG_ITEM_USE_PUSH

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadNetBaseCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos)
{
	CalculateNetBaseCfgSize(num, len);
	unsigned char *pBuf = new unsigned char[len];
	bool bSuccess;
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_UPNP_ENABLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

		unsigned long EnableUpnp = 0;
#ifdef __CUSTOM_US02__
		EnableUpnp = 1;
#endif
		memcpy(pBuf+currPos+itemHeadLen, &EnableUpnp, pItemHead->len);
	}
	currPos +=itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HTTP_PORT;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

		unsigned long HttpPortValue = 80;
#ifdef __CUSTOM_US02__
		HttpPortValue = 85;
#endif
		memcpy(pBuf+currPos+itemHeadLen, &HttpPortValue, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SERVER_PORT;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

#if defined(__CUSTOM_ZNV__)
		unsigned long serverPortValue = 10086;
#else
		unsigned long serverPortValue = 6036;
#endif
		memcpy(pBuf+currPos+itemHeadLen, &serverPortValue, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NETWORK_ADDR_INFO;
	pItemHead->len = sizeof(NETWORK_ADDR_INFO);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NETWORK_ADDR_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

		NETWORK_ADDR_INFO networkAddrInfo;
		networkAddrInfo.bDHCP = 1;
#ifndef __CUSTOM_BR27__
		networkAddrInfo.staticIP = IP_ADDRESS(192, 168, 1, 100);
		networkAddrInfo.gateway = IP_ADDRESS(192, 168, 1, 1);
		networkAddrInfo.netMask = IP_ADDRESS(255, 255, 255, 0);
#else
		networkAddrInfo.staticIP = IP_ADDRESS(192, 168, 2, 200);
		networkAddrInfo.gateway = IP_ADDRESS(192, 168, 2, 1);
		networkAddrInfo.netMask = IP_ADDRESS(255, 255, 255, 0);
#endif

#if defined(__CUSTOM_PL14__)
		networkAddrInfo.bDHCP = 0;
#endif


#if defined(__CUSTOM_ZNV__)
		networkAddrInfo.bDHCP = 0;
		networkAddrInfo.staticIP = IP_ADDRESS(192, 168, 1, 2);
		networkAddrInfo.gateway = IP_ADDRESS(192, 168, 1, 1);
		networkAddrInfo.netMask = IP_ADDRESS(255, 255, 0, 0);
#endif

#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
		networkAddrInfo.bDHCP = 0;
		networkAddrInfo.staticIP = IP_ADDRESS(192, 168, 1, 100);
		networkAddrInfo.gateway = IP_ADDRESS(192, 168, 1, 1);
		networkAddrInfo.netMask = IP_ADDRESS(255, 255, 255, 0);
#endif

#if defined(__CUSTOM_ZNV__) && defined(__ZNV_WNRL__)
		networkAddrInfo.bDHCP = 1;
		networkAddrInfo.staticIP = IP_ADDRESS(192, 168, 1, 100);
		networkAddrInfo.gateway = IP_ADDRESS(192, 168, 1, 1);
		networkAddrInfo.netMask = IP_ADDRESS(255, 255, 255, 0);
#endif
		memcpy(pBuf+currPos+itemHeadLen, &networkAddrInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DNS_1;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#if defined(__CUSTOM_RU04__)
		unsigned long DNS1Value = IP_ADDRESS(8,8,8,8);
		memcpy(pBuf+currPos+itemHeadLen, &DNS1Value, pItemHead->len);
#else
		ULONGLONG DNS1Value = 0;
		memcpy(pBuf+currPos+itemHeadLen, &DNS1Value, pItemHead->len);
#endif
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DNS_2;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#if defined(__CUSTOM_RU04__)
		unsigned long DNS2Value = IP_ADDRESS(8,8,4,4);
		memcpy(pBuf+currPos+itemHeadLen, &DNS2Value, pItemHead->len);
#else
		ULONGLONG DNS2Value = 0;
		memcpy(pBuf+currPos+itemHeadLen, &DNS2Value, pItemHead->len);
#endif
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DDNS_INFO;
	pItemHead->len = sizeof(DDNS_INFO);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(DDNS_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);

	//-----added by wn 2011.7.20-----
	bool bIsDDNSOk = false;
	if (bSuccess)
	{
		bIsDDNSOk = false;
		int nCount = CReadDdns::Instance()->GetDddnsCount();
		DDNS_INFO *pDdnsInfo = reinterpret_cast<DDNS_INFO*>(pBuf + currPos + itemHeadLen);
		for (int i = 0; i < nCount; i++)
		{
			DDNS_SERVER_INFO ServerInfo;
			CReadDdns::Instance()->GetDddnsServerInfo(i, &ServerInfo);
			if (ServerInfo.ucDDNSID == pDdnsInfo->type)
			{
				bIsDDNSOk = true;
				break;
			}
		}
	}
	else
	{
		bIsDDNSOk = false;
	}

	if (!bIsDDNSOk)
	{
		DDNS_INFO ddnsInfo;
		memset(&ddnsInfo, 0, sizeof(DDNS_INFO));
		DDNS_SERVER_INFO serverInfo;
		CReadDdns::Instance()->GetDddnsServerInfo(0, &serverInfo);
		ddnsInfo.type = serverInfo.ucDDNSID;
		memcpy(pBuf+currPos+itemHeadLen, &ddnsInfo, pItemHead->len);
	}
	//-----added end-------
// 	if (!bSuccess)
// 	{
// 		DDNS_INFO ddnsInfo;
// 		memset(&ddnsInfo, 0, sizeof(DDNS_INFO));
// 		DDNS_SERVER_INFO serverInfo;
// 		CReadDdns::Instance()->GetDddnsServerInfo(0, &serverInfo);
// 		ddnsInfo.type = serverInfo.ucDDNSID;
// 		memcpy(pBuf+currPos+itemHeadLen, &ddnsInfo, pItemHead->len);
// 	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_PPPOE_INFO;
	pItemHead->len = sizeof(PPPOE_INFO);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(PPPOE_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	PPPOE_INFO pppoeInfo;
	memset(&pppoeInfo, 0, sizeof(PPPOE_INFO));
	memcpy(pBuf+currPos+itemHeadLen, &pppoeInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_EMAIL_SEND_INFO;
	pItemHead->len = sizeof(MAIL_SEND_INFO);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(MAIL_SEND_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#if defined(__CUSTOM_RU04__)
		MAIL_SEND_INFO SendInfo;
		memset(&SendInfo, 0, sizeof(MAIL_SEND_INFO));
		SendInfo.port = 465;
		SendInfo.bSSL = 1;
		snprintf(SendInfo.server, 68, "smtp.mail.ru");
		memcpy(pBuf+currPos+itemHeadLen, &SendInfo, pItemHead->len);
#else
		MAIL_SEND_INFO SendInfo;
		memset(&SendInfo, 0, sizeof(MAIL_SEND_INFO));
		SendInfo.port = 25;
		memcpy(pBuf+currPos+itemHeadLen, &SendInfo, pItemHead->len);
#endif
	}
	currPos += itemHeadLen+pItemHead->len;
	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_EMAIL_RECV_INFO;
	pItemHead->len = sizeof(MAIL_RECV_INFO)*MAX_MAIL_RECV_NUM;
	pItemHead->num = MAX_MAIL_RECV_NUM;
	pItemHead->subLen = sizeof(MAIL_RECV_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	MAIL_RECV_INFO *pRecvInfo = new MAIL_RECV_INFO[MAX_MAIL_RECV_NUM];
	memset(pRecvInfo, 0, sizeof(MAIL_RECV_INFO)*MAX_MAIL_RECV_NUM);
	memcpy(pBuf+currPos+itemHeadLen, pRecvInfo, pItemHead->len);
	delete []pRecvInfo;
	pRecvInfo = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_EMAIL_PIC_ACC;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long picNum = 0;  //Ĭ��ͼƬ����������	
	memcpy(pBuf+currPos+itemHeadLen, &picNum, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DDNS_UPDATE_CYCLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long ddnsUpdateCycle = 3 * 60;		//Ĭ��3��Сʱ
	memcpy(pBuf+currPos+itemHeadLen, &ddnsUpdateCycle, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;


	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_BAN_ALLOW_TYPE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_BAN_ALLOW_LIST;
	pItemHead->len = sizeof(IP_SECTION)*MAX_BAN_ALLOW_LIST_NUM;
	pItemHead->num = MAX_BAN_ALLOW_LIST_NUM;
	pItemHead->subLen = sizeof(IP_SECTION);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;


#ifdef __SUPPORT_HRSST__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HRSST_SERVER_IP;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long phoneServerIp = IP_ADDRESS(0, 0, 0, 0);;
	memcpy(pBuf+currPos+itemHeadLen, &phoneServerIp, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HRSST_SERVER_PORT;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long PhoneHttpPortValue = 0;
	memcpy(pBuf+currPos+itemHeadLen, &PhoneHttpPortValue, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

#ifdef __NETSERVER_HXHT__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HXHT_NET_PARAM;
	pItemHead->len = sizeof(HXHT_NET_PARAM_INFO);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(HXHT_NET_PARAM_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	HXHT_NET_PARAM_INFO hxhtNetParamInfo;
	memset(&hxhtNetParamInfo, 0, sizeof(HXHT_NET_PARAM_INFO));
	hxhtNetParamInfo.usVDevDataPort = 4602;
	hxhtNetParamInfo.csIP = 0xa322a7c;			//124.42.50.10
	hxhtNetParamInfo.csMessagePort = 6005;
	hxhtNetParamInfo.csStortPort = 6004;
	
	memcpy(pBuf+currPos+itemHeadLen, &hxhtNetParamInfo, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_WIRELESS;
	pItemHead->len = sizeof(MDVR_WIRELESS_CFG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(MDVR_WIRELESS_CFG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	MDVR_WIRELESS_CFG netWireless;
	netWireless.enable = 0;
	netWireless.type = 1;
	memset(netWireless.dialup, 0, sizeof(MDVR_DIALUP_CFG)*5);
	strcpy(netWireless.dialup[0].ConnectPoint, "*99#");	
	strcpy(netWireless.dialup[1].ConnectPoint, "#777");
	strcpy(netWireless.dialup[1].username, "CARD");
	strcpy(netWireless.dialup[1].password, "CARD");
	memcpy(pBuf+currPos+itemHeadLen, &netWireless, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;


	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_AUTO_REPORT;
	pItemHead->len = sizeof(NET_AUTO_REPORT);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NET_AUTO_REPORT);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	NET_AUTO_REPORT netAutoReport;
	snprintf(netAutoReport.host, 256, "192.168.1.200");
	netAutoReport.dwPort = 2009;
	netAutoReport.ID = 0;
	netAutoReport.bUse = 0;
	memcpy(pBuf+currPos+itemHeadLen, &netAutoReport, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MINT_DDNS_SERVER_NAME;
	pItemHead->len = 132;
	pItemHead->num = 1;
	pItemHead->subLen = 132;
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	char mintDDNSName[132] = {0};
	memcpy(pBuf+currPos+itemHeadLen, mintDDNSName, pItemHead->len);
	}
	currPos += (itemHeadLen + pItemHead->len);

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_USE_3G;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{		
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += (itemHeadLen + pItemHead->len);
	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_USE_IPV4_IPV6;
	pItemHead->len = sizeof(USE_IPV4_IPV6);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(USE_IPV4_IPV6);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{		
		USE_IPV4_IPV6 useIPv4IPv6;
		useIPv4IPv6.bEnableIPv4 = 1;
		useIPv4IPv6.bEnableIPv6 = 1;
		memcpy(pBuf+currPos+itemHeadLen, &useIPv4IPv6, pItemHead->len);
	}
	
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NETWORK_ADDR_INFO_IPV6;
	pItemHead->len = sizeof(NETWORK_ADDR_INFO_IPv6);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NETWORK_ADDR_INFO_IPv6);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{		
		NETWORK_ADDR_INFO_IPv6 networkAddrInfoIPv6;
		memset(&networkAddrInfoIPv6, 0, sizeof(networkAddrInfoIPv6));
		networkAddrInfoIPv6.recv = 0;
		networkAddrInfoIPv6.useDHCP = 0;
		networkAddrInfoIPv6.subnetMask = 0;
		char szTemp[40] = "0000:0000:0000:0000:0000:0000:0000:0000";
		memcpy(networkAddrInfoIPv6.IP, szTemp, sizeof(szTemp));
		memset(szTemp,0,sizeof(szTemp));
		snprintf(szTemp,40,"%s","0000:0000:0000:0000:0000:0000:0000:0000");
		memcpy(networkAddrInfoIPv6.gateway, szTemp, sizeof(szTemp));
		memcpy(pBuf+currPos+itemHeadLen, &networkAddrInfoIPv6, pItemHead->len);
	}	
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DNS_1_IPV6;
	pItemHead->len = sizeof(unsigned char[40]);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned char[40]);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{		
		unsigned char DNS1ValueIPv6[40] = "0000:0000:0000:0000:0000:0000:0000:0000";
		memcpy(pBuf+currPos+itemHeadLen, &DNS1ValueIPv6, pItemHead->len);
	}	
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DNS_2_IPV6;
	pItemHead->len = sizeof(unsigned char[40]);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned char[40]);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{		
		unsigned char DNS2ValueIPv6[40] = "0000:0000:0000:0000:0000:0000:0000:0000";
		memcpy(pBuf+currPos+itemHeadLen, &DNS2ValueIPv6, pItemHead->len);
	}	
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_YZ_PARAM;
	pItemHead->len = sizeof(YUANZHENG_PARAM);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(YUANZHENG_PARAM);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{	
		YUANZHENG_PARAM yz_param;
		memset(yz_param.ipUrl,0,sizeof(yz_param.ipUrl));
		memset(yz_param.asCode,0,sizeof(yz_param.asCode));
		memcpy(pBuf+currPos+itemHeadLen, &yz_param, pItemHead->len);
	}	
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_YZ_SNAP_PARAM;
	pItemHead->len = sizeof(YUANZHENG_SNAP_PARAM);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(YUANZHENG_SNAP_PARAM);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{	
		YUANZHENG_SNAP_PARAM snapPara;
		memset(snapPara.ipUrl,0,sizeof(snapPara.ipUrl));
		snapPara.timeS=0;
		memcpy(pBuf+currPos+itemHeadLen, &snapPara, pItemHead->len);
	}	
	currPos += itemHeadLen+pItemHead->len;

#if defined(__SUPPORT_SNAPD1__)
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_SHANGHAI_DAILY_IMAGE;
	pItemHead->len = sizeof(SNAP_INFOEX)*MAX_DAILY_SNAP_NUM;
	pItemHead->num = MAX_DAILY_SNAP_NUM;
	pItemHead->subLen = sizeof(SNAP_INFOEX);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{	
		SNAP_INFOEX *pInfo = new SNAP_INFOEX[MAX_DAILY_SNAP_NUM];
		memset(pInfo,0,sizeof(SNAP_INFOEX)*MAX_DAILY_SNAP_NUM );
		memcpy(pBuf+currPos+itemHeadLen, pInfo, pItemHead->len);

		delete [] pInfo;
		pInfo = NULL;
	}	
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_SHANGHAI_PRE_ALARM_SNAP_TIME;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long* pPreRecTime = new unsigned long[m_videoInputNum];
		for(int i=0; i<m_videoInputNum; i++)
		{
			pPreRecTime[i] = 69;
		}
		memcpy(pBuf+currPos+itemHeadLen, pPreRecTime, pItemHead->len);
		delete [] pPreRecTime;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_SHANGHAI_POST_ALARM_SNAP_TIME;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

		unsigned long* pPostRecTime = new unsigned long[m_videoInputNum];
		for ( int i=0; i<m_videoInputNum; i++ )
		{
			pPostRecTime[i] = 69;//��
		}
		memcpy(pBuf+currPos+itemHeadLen, pPostRecTime, pItemHead->len);
		delete [] pPostRecTime;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_SHANGHAI_MOTIONUPLOAD_ENABLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long motionEnable = 0;
		memcpy(pBuf+currPos+itemHeadLen, &motionEnable, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_SHANGHAI_PTENABLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long ptEnable = 0;
		memcpy(pBuf+currPos+itemHeadLen, &ptEnable, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_SHANGHAI_AGENTID;
	pItemHead->len = sizeof(char [16]);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(char [16]);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		char tmpID[13] = "SSJCTVTZXX05";
		memcpy(pBuf+currPos+itemHeadLen, tmpID, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_SHANGHAI_IP;
	pItemHead->len = sizeof(char [20]);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(char [20]);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		char tmpID[20] = "sgdzpic.3322.org";
		memcpy(pBuf+currPos+itemHeadLen, tmpID, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_SHANGHAI_PORT;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long tmpPort = 5901;
		memcpy(pBuf+currPos+itemHeadLen, &tmpPort, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_FTP;
	pItemHead->len = sizeof(TVT_FTP_CONFIG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(TVT_FTP_CONFIG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		TVT_FTP_CONFIG ftpCfg;
		memset(&ftpCfg, 0, sizeof(TVT_FTP_CONFIG));

		ftpCfg.serverPort = 21;
		ftpCfg.uploadVideoLen = 32;
		ftpCfg.uploadCamNum = 4;
		memcpy(pBuf+currPos+itemHeadLen, &ftpCfg, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;


	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID =  NCFG_ITEM_NAT_SERVER;
	pItemHead->len = sizeof(NAT_SERVER_INFO);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NAT_SERVER_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		NAT_SERVER_INFO natServerCfg;
		memset(&natServerCfg, 0, sizeof(NAT_SERVER_INFO));
#if defined(__CUSTOM_US02__)
		natServerCfg.bStartNat = 1;
		snprintf(natServerCfg.szServerAddr, sizeof(natServerCfg.szServerAddr), "q-seeqtview.com");
#elif defined(__CUSTOM_CN__) || defined(__CUSTOM_SHANGHAI__) || defined(__CUSTOM_IN__) || defined(__CUSTOM_CNB__)
		natServerCfg.bStartNat = 0;
		snprintf(natServerCfg.szServerAddr, sizeof(natServerCfg.szServerAddr), "nat.autonat.com");
#elif defined(__CUSTOM_IL03__)
		natServerCfg.bStartNat = 0;
		snprintf(natServerCfg.szServerAddr, sizeof(natServerCfg.szServerAddr), "www.provisionisr-nat.com");
#else
		natServerCfg.bStartNat = 0;
		snprintf(natServerCfg.szServerAddr, sizeof(natServerCfg.szServerAddr), "nat.autonat.com");
#endif

		natServerCfg.iServerPort = 8989;
		memcpy(pBuf+currPos+itemHeadLen, &natServerCfg, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID =  NCFG_ITEM_PUSH_SERVER;
	pItemHead->len = sizeof(PUSH_SERVER_INFO);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(PUSH_SERVER_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		PUSH_SERVER_INFO natServerCfg;
		memset(&natServerCfg, 0, sizeof(PUSH_SERVER_INFO));

		snprintf(natServerCfg.szServerAddr, sizeof(natServerCfg.szServerAddr), "pnts.tvt.net.cn");
		natServerCfg.iServerPort = 7020;
		memcpy(pBuf+currPos+itemHeadLen, &natServerCfg, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	if(CProduct::Instance()->PoeInputNum() > 0)
	{
		pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
		pItemHead->itemID =  NCFG_ITEM_SWITCH_INFO;
		pItemHead->len = sizeof(NET_SWITCH_INFO);
		pItemHead->num = 1;
		pItemHead->subLen = sizeof(NET_SWITCH_INFO);
		bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
		if (!bSuccess)
		{
			NET_SWITCH_INFO switchInfoCfg;
			switchInfoCfg.switchIP = IP_ADDRESS(10, 151, 151, 1);
			switchInfoCfg.netMask = IP_ADDRESS(255, 255, 255, 0);
			memcpy(pBuf+currPos+itemHeadLen, &switchInfoCfg, pItemHead->len);
		}
		currPos += itemHeadLen+pItemHead->len;
	}

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID =  NCFG_ITEM_USE_PUSH;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long bUsePush = 1;

		memcpy(pBuf+currPos+itemHeadLen, &bUsePush, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateAlarmTrigerCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_ALARM_TRIGER_BASE=0x600,
	num++; len += sizeof(unsigned long);							//NCFG_ITEM_BUZZER_ENABLE,			//���������أ�unsigned long
	num++; len += sizeof(unsigned long);							//NCFG_ITEM_BUZZER_HOLD_TIME,			//����������ʱ�䣬unsigned long,
	num++; len += (sizeof(NCFG_INFO_SHORT_NAME)*m_sensorOutputNum);	//NCFG_ITEM_ALARM_OUT_NAME,			//�̵���������ƣ�SHORT_NAME[m_sensorOutputNum]
	num++; len += (sizeof(unsigned long)*m_sensorOutputNum);		//NCFG_ITEM_ALARM_OUT_HOLD_TIME,		//�̵����������ʱ�䣬unsigned long [m_sensorOutputNum]

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}


void CConfigEx::LoadAlarmTrigerCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos)
{
	CalculateAlarmTrigerCfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_BUZZER_ENABLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long value = 0;
#if defined(__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__) || defined(__CUSTOM_CBC__)
	value = 1;
#endif

#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
	value = 1;
#endif

	memcpy(pBuf+currPos+itemHeadLen, &value, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_BUZZER_HOLD_TIME;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
		unsigned long holdTimeValue = 0xffffffff;
#else
		unsigned long holdTimeValue = 10;
#endif
		memcpy(pBuf+currPos+itemHeadLen, &holdTimeValue, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_ALARM_OUT_NAME;
	pItemHead->len = (sizeof(NCFG_INFO_SHORT_NAME)*m_sensorOutputNum);
	pItemHead->num = m_sensorOutputNum;
	pItemHead->subLen = sizeof(NCFG_INFO_SHORT_NAME);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	NCFG_INFO_SHORT_NAME *pAlarmOutNameValue = new NCFG_INFO_SHORT_NAME[m_sensorOutputNum];
	for (int i=0; i<m_sensorOutputNum; i++)
	{
		sprintf(pAlarmOutNameValue[i].name, "ALARM OUT %d", i+1);
	}
	memcpy(pBuf+currPos+itemHeadLen, pAlarmOutNameValue, pItemHead->len);
	delete [] pAlarmOutNameValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_ALARM_OUT_HOLD_TIME;
	pItemHead->len = (sizeof(unsigned long)*m_sensorOutputNum);
	pItemHead->num = m_sensorOutputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long *pAlarmOutHoldTimeValue = new unsigned long [m_sensorOutputNum];
	for (int i=0; i<m_sensorOutputNum; i++)
	{
#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
		pAlarmOutHoldTimeValue[i] = 0xffffffff;
#else
		pAlarmOutHoldTimeValue[i] = 10;
#endif
	}
	memcpy(pBuf+currPos+itemHeadLen, pAlarmOutHoldTimeValue, pItemHead->len);
	delete [] pAlarmOutHoldTimeValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateAlarmTrigerSensorCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_ALARM_TRIGER_SENSOR_BASE=0x620,
	num++; len += (sizeof(unsigned long)*m_sensorInputNum);		//NCFG_ITEM_SENSOR_IN_ENABLE,			//���������أ�unsigned long [m_sensorInputNum]
	num++; len += (sizeof(unsigned long)*m_sensorInputNum);		//NCFG_ITEM_SENSOR_IN_TYPE,			//�������豸���ͣ�unsigned long [m_sensorInputNum]
	num++; len += (sizeof(NCFG_INFO_SHORT_NAME)*m_sensorInputNum);	//NCFG_ITEM_SENSOR_IN_NAME,			//���������ƣ�SHORT_NAME [m_sensorInputNum]
	num++; len += (sizeof(unsigned long)*m_sensorInputNum);			//NCFG_ITEM_SENSOR_IN_HOLD_TIME,//����������ʱ��, unsigned long [m_sensorInputNum]
	num++; len += (sizeof(unsigned long)*m_sensorInputNum);		//NCFG_ITEM_SENSOR_IN_BUZZ,           //����������������, unsigned long [m_sensorInputNum]
	num++; len += (sizeof(ULONGLONG)*m_sensorInputNum);			//NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT,	//�������������������ULONGLONG [m_sensorInputNum];
	num++; len += (sizeof(ULONGLONG)*m_sensorInputNum);			//NCFG_ITEM_SENSOR_IN_TO_RECORD,		//����������¼��ULONGLONG [m_sensorInputNum];
	num++; len += (sizeof(ULONGLONG)*m_sensorInputNum);			//NCFG_ITEM_SENSOR_TO_BIG_VIEW,		//�������������棬ULONGLONG [m_sensorInputNum];
#ifdef __DVR_BASIC__	
	num++; len += (sizeof(unsigned long)*m_sensorInputNum);		//NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE,//�������ܹ�����EMAIL��unsigned long
	num++; len += (sizeof(unsigned long)*m_sensorInputNum);		//NCFG_ITEM_SENSOR_TO_EMAIL_ADDR,		//����������MAIL��ַ�б�unsigned long
#endif
	num++; len += (sizeof(ULONGLONG) * m_sensorInputNum);								//NCFG_ITEM_SENSOR_IN_TO_FTP_ENABLE
	num++; len += (sizeof(TO_PTZ)*m_sensorInputNum*m_videoInputNum);//temprory	//NCFG_ITEM_SENSOR_IN_TO_PTZ,			//������������̨Ԥ�Ƶ㣬TO_PTZ [m_sensorInputNum][m_videoInputNum]

#ifdef __DVR_BASIC__
	num++; len += (sizeof(ULONGLONG)*m_sensorInputNum);  //NCFG_ITEM_SENSOR_IN_TO_SNAP ����������ץͼ��ͨ��ULONGLONG [SENSOR_INPUT_NUM]
	num++; len += (sizeof(ULONGLONG)*m_sensorInputNum);  //NCFG_ITEM_SENSOR_TO_FTP_SNAP,		//����������FTPץͼ��ULONGLONG[VIDEO_INPUT_NUM]
	num++; len += (sizeof(ULONGLONG)*m_sensorInputNum);  //NCFG_ITEM_SENSOR_TO_EMAIL_SNAP,		//����������EMAILץͼ��ULONGLONG[VIDEO_INPUT_NUM]
#endif

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}


void CConfigEx::LoadAlarmTrigerSensorCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos)
{
	CalculateAlarmTrigerSensorCfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;

	bool bHaveAlarmIn = true;
	{
		PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
		if ((TD_2304CDS == productType) || (TD_2308CDS == productType) || (TD_2304HS == productType) 
			|| (TD_2304SS_B == productType) || (TD_2308SS_B == productType) || (TD_2316SS_B == productType)  || (TD_2508HS_BL == productType) || (TD_2516HS_BL == productType)
			|| (TD_2304SS_C == productType) || (TD_2308SS_C == productType) || (TD_2316SS_C == productType)
			|| (TD_2304SL_C == productType) || (TD_2308SL_C == productType)
			|| (TD_2304SS_SL == productType)|| (TD_2308SS_SL == productType)
			|| (TD_2304SS_SDI == productType)|| (TD_2308SS_SDI == productType) ||(TD_2304SS_SL_3515A == productType) \
			|| (TD_2804NS_SL == productType) || (TD_2808NS_SL == productType) || (TD_2816NS_SL == productType) || (TD_2704XS_SL == productType) || (TD_2708XS_SL == productType) || (TD_2716XS_SL == productType)
			|| (TD_2704AS_SL == productType) || (TD_2708AS_SL == productType) || (TD_2716AS_SL == productType)
			|| (TD_2708AS_SL_A == productType) || (TD_2716AS_SL_A == productType)
			|| (TD_2704AS_S == productType) || (TD_2708AS_S == productType) || (TD_2716AS_S == productType)
			|| (TD_2704AS_S_A == productType) || (TD_2716AS_S_A == productType)
			|| (TD_2704TS_S == productType) || (TD_2708TS_S == productType) || (TD_2716TS_S == productType)
			|| (TD_2704TS_PL == productType) || (TD_2708TS_PL == productType) || (TD_2716TS_PL == productType)
			|| (TD_2804NS_C == productType ) || (TD_2808NS_C == productType ) || (TD_2816NS_C == productType ) || (TD_2804PS_C4 == productType ) || (TD_2808PS_C4 == productType ) || (TD_2808PS_C8 == productType) || (TD_2816PS_C8 == productType )
			|| (TD_2004AL == productType) || (TD_2008AS == productType))
		{
			bHaveAlarmIn = false;
		}
	}
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_ENABLE;
	pItemHead->len = (sizeof(unsigned long)*m_sensorInputNum);
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long *pSIEValue = new unsigned long [m_sensorInputNum];
#ifdef __CUSTOM_YUANZHENG__
	for (int i = 0; i < m_sensorInputNum; ++i)
		{
			pSIEValue[i] = 0;
		}
#else
		
		if (bHaveAlarmIn)
		{
			for (int i = 0; i < (m_sensorInputNum); ++i)
			{
				pSIEValue[i] = 1;
			}
		}
		else
		{
			for (int i = 0; i < (m_sensorInputNum); ++i)
			{
				pSIEValue[i] = 0;
			}
		}
#endif
		memcpy(pBuf+currPos+itemHeadLen, pSIEValue, pItemHead->len);
		delete [] pSIEValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_TYPE;
	pItemHead->len = (sizeof(unsigned long)*m_sensorInputNum);
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long *pSITValue = new unsigned long [m_sensorInputNum];
	memset(pSITValue, 0, m_sensorInputNum * sizeof(pSITValue[0]));
	memcpy(pBuf+currPos+itemHeadLen, pSITValue, pItemHead->len);
	delete [] pSITValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_NAME;
	pItemHead->len = (sizeof(NCFG_INFO_SHORT_NAME)*m_sensorInputNum);
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(NCFG_INFO_SHORT_NAME);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	NCFG_INFO_SHORT_NAME *pSINValue = new NCFG_INFO_SHORT_NAME[m_sensorInputNum];
	memset(pSINValue, 0, sizeof(NCFG_INFO_SHORT_NAME)*m_sensorInputNum);
	for (int i=0; i<m_sensorInputNum; i++)
	{
		sprintf(pSINValue[i].name, "SENSOR %d", i+1);
	}
	memcpy(pBuf+currPos+itemHeadLen, pSINValue, pItemHead->len);
	delete [] pSINValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_HOLD_TIME;
	pItemHead->len = sizeof(unsigned long)*m_sensorInputNum;
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long *pSIHoldTime = new unsigned long [m_sensorInputNum];
	for (int i = 0; i < m_sensorInputNum; ++i)
	{
		pSIHoldTime[i] = 10;
	}
	memcpy(pBuf+currPos+itemHeadLen, pSIHoldTime, pItemHead->len);
	delete [] pSIHoldTime;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_BUZZ;
	pItemHead->len = (sizeof(unsigned long)*m_sensorInputNum);
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long *pSIBValue = new unsigned long [m_sensorInputNum];
	for (int i = 0; i < m_sensorInputNum; ++i)
	{
		pSIBValue[i] = 1;
	}
	memcpy(pBuf+currPos+itemHeadLen, pSIBValue, pItemHead->len);
	delete [] pSIBValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT;
	pItemHead->len = (sizeof(ULONGLONG)*m_sensorInputNum);
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		ULONGLONG *pSITAOValue = new ULONGLONG[m_sensorInputNum];
#if defined(__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__) || defined(__CUSTOM_CBC__)
		for (int i=0; i<m_sensorInputNum; i++)
		{
			pSITAOValue[i] = (ULONGLONG)(0x01 << (i%m_sensorOutputNum));
		}
#else
		memset(pSITAOValue, 0, sizeof(ULONGLONG)*m_sensorInputNum);
#endif
	
	
		memcpy(pBuf+currPos+itemHeadLen, pSITAOValue, pItemHead->len);
		delete [] pSITAOValue;
	}

	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_TO_RECORD;
	pItemHead->len = (sizeof(ULONGLONG)*m_sensorInputNum);
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		ULONGLONG *pSITRValue = new ULONGLONG [m_sensorInputNum];
		for (int i=0; i<m_sensorInputNum; i++)
		{
			if((i < m_localVideoInputNum) && (i < m_localSensorInputNum))	//����SENSORĬ�ϴ���ģ��ͨ��¼��
			{
				pSITRValue[i] = (ULONGLONG)(0x01 << i);
			}
			else if(i >= m_localSensorInputNum)	//����SENSORĬ�ϴ�������ͨ��¼��
			{
				pSITRValue[i] = ((ULONGLONG)0x01 << (i - m_localSensorInputNum + m_localVideoInputNum));
			}
			else
			{
				pSITRValue[i] = 0;
			}
		}
		memcpy(pBuf+currPos+itemHeadLen, pSITRValue, pItemHead->len);
		delete [] pSITRValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_TO_BIG_VIEW;
	pItemHead->len = (sizeof(ULONGLONG)*m_sensorInputNum);
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		ULONGLONG *pSITBVValue = new ULONGLONG [m_sensorInputNum];
		for (int i=0; i<m_sensorInputNum; i++)
		{
			pSITBVValue[i] = 0;//(ULONGLONG)(0x01 << i);
		}
		memcpy(pBuf+currPos+itemHeadLen, pSITBVValue, pItemHead->len);
		delete [] pSITBVValue;
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE;
	pItemHead->len = (sizeof(unsigned long)) * m_sensorInputNum;
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long *pEmailEn = new unsigned long[m_sensorInputNum];
		memset(pEmailEn, 0, sizeof(unsigned long)*m_sensorInputNum);
		memcpy(pBuf+currPos+itemHeadLen, pEmailEn,pItemHead->len);
		delete []pEmailEn;
		pEmailEn = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_TO_EMAIL_ADDR;
	pItemHead->len = (sizeof(unsigned long)) * m_sensorInputNum;
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long *pEmailAddr = new unsigned long[m_sensorInputNum];
		memset(pEmailAddr, 0, sizeof(unsigned long)*m_sensorInputNum);
		memcpy(pBuf+currPos+itemHeadLen, pEmailAddr,pItemHead->len);
		delete []pEmailAddr;
		pEmailAddr = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_TO_FTP_ENABLE;
	pItemHead->len = (sizeof(ULONGLONG)*(m_sensorInputNum));
	pItemHead->num = (m_sensorInputNum);
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

		ULONGLONG *pSITRValue = new ULONGLONG [m_sensorInputNum];
		memset(pSITRValue, 0, pItemHead->len);
		memcpy(pBuf+currPos+itemHeadLen, pSITRValue, pItemHead->len);
		delete [] pSITRValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_TO_PTZ;
	pItemHead->len = (sizeof(TO_PTZ)*m_videoInputNum*m_sensorInputNum);//temprory
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = (sizeof(TO_PTZ)*m_videoInputNum);//temprory
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	TO_PTZ *psITPValue = new TO_PTZ [m_sensorInputNum*m_videoInputNum];//temprory
	memset(psITPValue, 0, sizeof(TO_PTZ)*m_sensorInputNum*m_videoInputNum);//temprory
	memcpy(pBuf+currPos+itemHeadLen, psITPValue, pItemHead->len);
	delete []psITPValue;
	}
	currPos += itemHeadLen+pItemHead->len;


#ifdef __DVR_BASIC__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_IN_TO_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_sensorInputNum);
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		ULONGLONG *pSITSNValue = new ULONGLONG [m_sensorInputNum];
		for (int i=0; i<m_sensorInputNum; i++)
		{
			if((i < m_localVideoInputNum) && (i < m_localSensorInputNum))	//����SENSORĬ�ϴ���ģ��ͨ��¼��
			{
				pSITSNValue[i] = (ULONGLONG)(0x01 << i);
			}
			else if(i >= m_localSensorInputNum)	//����SENSORĬ�ϴ�������ͨ��¼��
			{
				pSITSNValue[i] = ((ULONGLONG)0x01 << (i - m_localSensorInputNum + m_localVideoInputNum));
			}
			else
			{
				pSITSNValue[i] = 0;
			}
		}
		memcpy(pBuf+currPos+itemHeadLen, pSITSNValue, pItemHead->len);
		delete [] pSITSNValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_TO_FTP_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_sensorInputNum);
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		ULONGLONG *pSITFTValue = new ULONGLONG [m_sensorInputNum];
		for (int i=0; i<m_sensorInputNum; i++)
		{
			if((i < m_localVideoInputNum) && (i < m_localSensorInputNum))	//����SENSORĬ�ϴ���ģ��ͨ��¼��
			{
				pSITFTValue[i] = (ULONGLONG)(0x01 << i);
			}
			else if(i >= m_localSensorInputNum)	//����SENSORĬ�ϴ�������ͨ��¼��
			{
				pSITFTValue[i] = ((ULONGLONG)0x01 << (i - m_localSensorInputNum + m_localVideoInputNum));
			}
			else
			{
				pSITFTValue[i] = 0;
			}
		}
		memcpy(pBuf+currPos+itemHeadLen, pSITFTValue, pItemHead->len);
		delete [] pSITFTValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SENSOR_TO_EMAIL_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_sensorInputNum);
	pItemHead->num = m_sensorInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		ULONGLONG *pSITESValue = new ULONGLONG [m_sensorInputNum];
		for (int i=0; i<m_sensorInputNum; i++)
		{
			//pSITESValue[i] = (ULONGLONG)(0x01 << i);
			pSITESValue[i] = 0;
		}
	memcpy(pBuf+currPos+itemHeadLen, pSITESValue, pItemHead->len);
	delete [] pSITESValue;
	}
	currPos += itemHeadLen+pItemHead->len;
#endif


	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateAlarmTrigerMotionCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_ALARM_TRIGER_MOTION_BASE=0x640,
	num++; len += (sizeof(unsigned long)*m_videoInputNum);		//NCFG_ITEM_MOTION_ENABLE,			//�ƶ���⿪�أ�unsigned long [m_videoInputNum]
	num++; len += (sizeof(MOTION_DETECT_INFO)*m_videoInputNum);//NCFG_ITEM_MOTION_AREA_INFO,         //�������Ϣ��MOTION_DETECT_INFO[m_videoInputNum]
	num++; len += (sizeof(unsigned long)*m_videoInputNum);		//NCFG_ITEM_MOTION_HOLD_TIME,	//�ƶ���Ᵽ��ʱ��,unsigned long [m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);			//NCFG_ITEM_MOTION_TO_ALARM_OUT,		//�ƶ���ⴥ�����������ULONGLONG [m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);			//NCFG_ITEM_MOTION_TO_RECORD,			//�ƶ���ⴥ��¼��ULONGLONG [m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);			//NCFG_ITEM_MOTION_TO_BIG_VIEW,			//�ƶ���ⴥ�����棬ULONGLONG [m_videoInputNum]
#ifdef __DVR_BASIC__	
	num++; len += (sizeof(unsigned long)*m_videoInputNum);		//NCFG_ITEM_MOTION_TO_EMAIL_ENABLE,		//�ƶ�����ܹ�����EMAIL��unsigned long		
	num++; len += (sizeof(unsigned long)*m_videoInputNum);		//NCFG_ITEM_MOTION_TO_EMAIL_ADDR,		//�ƶ���ⴥ��MAIL��ַ�б�unsigned long
#endif	

	num++; len += (sizeof(TO_PTZ)*m_videoInputNum*m_videoInputNum);//temporary	//NCFG_ITEM_MOTION_TO_PTZ,	//�ƶ���ⴥ����̨��TO_PTZ [m_videoInputNum][m_videoInputNum]
	num++; len += (sizeof(ULONGLONG) * m_videoInputNum);		//NCFG_ITEM_MOTION_TO_FTP_ENABLE,		//�ƶ���ⴥ��FTP��unsigned long
	num++; len += (sizeof(unsigned long)*m_videoInputNum);		//NCFG_ITEM_MOTION_TO_BUZZ,           //�ƶ���ⴥ��BUZZ unsigned long[m_localVideoInputNum]

#ifdef __DVR_BASIC__
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);	//NCFG_ITEM_MOTION_TO_SNAP,           //�ƶ���ⴥ��ץ�� ULONGLONG[m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);	//NCFG_ITEM_MOTION_TO_FTP_SNAP,		//�ƶ���ⴥ��FTPץͼ��ULONGLONG[m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum); //	NCFG_ITEM_MOTION_TO_EMAIL_SNAP,		//�ƶ���ⴥ��EMAILץͼ��ULONGLONG[m_videoInputNum]
#endif
#ifdef __NETSERVER_HXHT__
	num++; len += (sizeof(HXHT_MOTION_AREA_INFO)*m_localVideoInputNum);//NCFG_ITEM_HXHT_MOTION_AREA_INFO  //���Ż�ͨ������Ϣ
#endif
	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadAlarmTrigerMotionCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos)
{
	CalculateAlarmTrigerMotionCfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_ENABLE;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long *pMEValue = new unsigned long [m_videoInputNum] ;
	for (int i = 0; i < m_videoInputNum; ++i)
	{
#if defined(__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__) || defined(__CUSTOM_CBC__) || defined(__CUSTOM_YUANZHENG__)
		pMEValue[i] = 0;
#else
		pMEValue[i] = 1;
#endif
#if defined(__CUSTOM_ZNV__)
		pMEValue[i] = 0;
#endif
	}
	memcpy(pBuf+currPos+itemHeadLen, pMEValue, pItemHead->len);
	delete [] pMEValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_AREA_INFO;
	pItemHead->len = (sizeof(MOTION_DETECT_INFO)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(MOTION_DETECT_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	MOTION_DETECT_INFO *pMAIValue = new MOTION_DETECT_INFO[m_videoInputNum];
	memset(pMAIValue, 0, sizeof(MOTION_DETECT_INFO)*m_videoInputNum);
	for(unsigned long j = 0; j < m_videoInputNum; ++j)
	{				
		pMAIValue[j].areaInfo.gridWidth = MOTION_AREA_GRID_WIDTH;
		pMAIValue[j].areaInfo.gridHeight = MOTION_AREA_GRID_HEIGHT;
		pMAIValue[j].sensitivity = 5;
		memset(pMAIValue[j].areaInfo.pData, 0xff, sizeof(pMAIValue[j].areaInfo.pData));
	}
	memcpy(pBuf+currPos+itemHeadLen, pMAIValue, pItemHead->len);
	delete [] pMAIValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_HOLD_TIME;
	pItemHead->len = sizeof(unsigned long)*m_videoInputNum;
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long *pMHoldTime = new unsigned long [m_videoInputNum];

	#ifdef __CUSTOM_US02_NEW__
		memset(pMHoldTime, 5, sizeof(unsigned long)*m_videoInputNum);
	#else
		memset(pMHoldTime, 0, sizeof(unsigned long)*m_videoInputNum);
	#endif
	for (int i=0; i<m_videoInputNum; i++)
	{
		pMHoldTime[i] = 10;
	}
	memcpy(pBuf+currPos+itemHeadLen, pMHoldTime, pItemHead->len);
	delete [] pMHoldTime;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_TO_ALARM_OUT;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	ULONGLONG *pMTAOValue = new ULONGLONG [m_videoInputNum];
#if defined(__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__) || defined(__CUSTOM_CBC__)
	for (int i=0; i<m_videoInputNum; i++)
	{
		pMTAOValue[i] = (ULONGLONG)(0x01 << (i%m_sensorOutputNum));
	}
#else
	memset(pMTAOValue, 0, sizeof(ULONGLONG)*m_videoInputNum);
#endif	
	memcpy(pBuf+currPos+itemHeadLen, pMTAOValue, pItemHead->len);
	delete [] pMTAOValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_TO_RECORD;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	ULONGLONG *pMTRValue = new ULONGLONG [m_videoInputNum];
	for (int i=0; i<m_videoInputNum; i++)
	{
		pMTRValue[i] = (ULONGLONG)(0x01 << i);
		
	}
	memcpy(pBuf+currPos+itemHeadLen, pMTRValue, pItemHead->len);
	delete [] pMTRValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_TO_BIG_VIEW;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	ULONGLONG *pMTBVValue = new ULONGLONG [m_videoInputNum];
	for (int i=0; i<m_videoInputNum; i++)
	{
		pMTBVValue[i] = 0;//(ULONGLONG)(0x01 << i);
	}
	memcpy(pBuf+currPos+itemHeadLen, pMTBVValue, pItemHead->len);
	delete [] pMTBVValue;
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_TO_EMAIL_ENABLE;
	pItemHead->len = (sizeof(unsigned long)) * m_videoInputNum;
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long *pEmailEn = new unsigned long[m_videoInputNum];
	memset(pEmailEn, 0, sizeof(unsigned long)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pEmailEn,pItemHead->len);
	delete []pEmailEn;
	pEmailEn = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_TO_EMAIL_ADDR;
	pItemHead->len = (sizeof(unsigned long)) * m_videoInputNum;
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long *pEmailAddr = new unsigned long[m_videoInputNum];
	memset(pEmailAddr, 0, sizeof(unsigned long)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pEmailAddr,pItemHead->len);
	delete []pEmailAddr;
	pEmailAddr = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_TO_PTZ;

	pItemHead->len = (sizeof(TO_PTZ) * m_videoInputNum * m_videoInputNum);//temporary
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(TO_PTZ) * m_videoInputNum;//temporary
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		TO_PTZ * pMTPValue = new TO_PTZ[m_videoInputNum*m_videoInputNum];//temporary
		memset(pMTPValue, 0, sizeof(TO_PTZ) * m_videoInputNum * m_videoInputNum);//temporary
		memcpy(pBuf+currPos+itemHeadLen, pMTPValue, pItemHead->len);
		delete [] pMTPValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_TO_FTP_ENABLE;
	pItemHead->len = (sizeof(ULONGLONG) * m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		ULONGLONG *pMTFValue = new ULONGLONG [m_videoInputNum];
		memset(pMTFValue, 0, pItemHead->len);
		memcpy(pBuf+currPos+itemHeadLen, pMTFValue, pItemHead->len);
		delete [] pMTFValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_TO_BUZZ;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long *pMTBValue = new unsigned long [m_videoInputNum];
	for (int i = 0; i < m_videoInputNum; ++i)
	{
		pMTBValue[i] = 1;
	}
	memcpy(pBuf+currPos+itemHeadLen, pMTBValue, pItemHead->len);
	delete [] pMTBValue;
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_TO_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	ULONGLONG *pMTSNValue = new ULONGLONG [m_videoInputNum];
	for (int i=0; i<m_videoInputNum; i++)
	{		
		pMTSNValue[i] = (ULONGLONG)(0x01 << i);		
	}
	memcpy(pBuf+currPos+itemHeadLen, pMTSNValue, pItemHead->len);
	delete [] pMTSNValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_TO_FTP_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	ULONGLONG *pMTFTValue = new ULONGLONG [m_videoInputNum];
	for (int i=0; i<m_videoInputNum; i++)
	{
		pMTFTValue[i] = (ULONGLONG)(0x01 << i);
	}
	memcpy(pBuf+currPos+itemHeadLen, pMTFTValue, pItemHead->len);
	delete [] pMTFTValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MOTION_TO_EMAIL_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	ULONGLONG *pMTENValue = new ULONGLONG [m_videoInputNum];
	for (int i=0; i<m_videoInputNum; i++)
	{
		//pMTENValue[i] = (ULONGLONG)(0x01 << i);
		pMTENValue[i] = 0;
	}
	memcpy(pBuf+currPos+itemHeadLen, pMTENValue, pItemHead->len);
	delete [] pMTENValue;
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

#ifdef __NETSERVER_HXHT__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HXHT_MOTION_AREA_INFO;
	pItemHead->len = sizeof(HXHT_MOTION_AREA_INFO) * m_localVideoInputNum;
	pItemHead->num = m_localVideoInputNum;
	pItemHead->subLen = sizeof(HXHT_MOTION_AREA_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{


	HXHT_MOTION_AREA_INFO * pHxhtMAInfo = reinterpret_cast<HXHT_MOTION_AREA_INFO *>(pBuf+currPos+itemHeadLen);
	memset(pHxhtMAInfo, 0, sizeof(HXHT_MOTION_AREA_INFO) * m_localVideoInputNum);
	for(int i = 0; i < m_localVideoInputNum; i++)
	{
		pHxhtMAInfo[i].sensitivity = 5;
		pHxhtMAInfo[i].areaRect[0].bEnable = 1;
		pHxhtMAInfo[i].areaRect[0].xStart = 0;
		pHxhtMAInfo[i].areaRect[0].yStart = 0;
		pHxhtMAInfo[i].areaRect[0].width = 351;
		pHxhtMAInfo[i].areaRect[0].height = 287;
	}
	}
	currPos += itemHeadLen + pItemHead->len;
#endif

	*ppConfig = pBuf;
	assert(len == currPos);
}


void CConfigEx::CalculateAlarmTrigerVLossCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_ALARM_TRIGER_VLOSS_BASE=0x660,
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);	//NCFG_ITEM_VL_TO_ALARM_OUT,			//��Ƶ��ʧ�������������ULONGLONG [m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);	//NCFG_ITEM_VL_TO_RECORD,				//��Ƶ��ʧ����¼��ULONGLONG [m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);	//NCFG_ITEM_VL_TO_BIG_VIEW,				//��Ƶ��ʧ�������棬ULONGLONG [m_videoInputNum]
#ifdef __DVR_BASIC__	
	num++; len += (sizeof(unsigned long)*m_videoInputNum); //NCFG_ITEM_VL_TO_EMAIL_ENABLE,		//��Ƶ��ʧ�ܹ�����MAIL unsigned long
	num++; len += (sizeof(unsigned long)*m_videoInputNum); //NCFG_ITEM_VL_TO_EMAIL_ADDR,		//��Ƶ��ʧ����MAIL��ַ�б�unsigned long
#endif
	num++; len += (sizeof(TO_PTZ)*m_videoInputNum*m_videoInputNum);	//NCFG_ITEM_VL_TO_PTZ, //��Ƶ��ʧ������̨��TO_PTZ [m_videoInputNum][m_videoInputNum]
	num++; len += (sizeof(ULONGLONG) * m_videoInputNum);	//NCFG_ITEM_VL_TO_FTP_ENABLE,			//��Ƶ��ʧ����FTP��unsigned long
	num++; len += (sizeof(unsigned long)*m_videoInputNum);	//NCFG_ITEM_VL_TO_BUZZ,             //��Ƶ��ʧ����BUZZ unsigned long[m_videoInputNum]

#ifdef __DVR_BASIC__
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);  	//NCFG_ITEM_VL_TO_SNAP,               //��Ƶ��ʧ����ץ�� ULONGLONG[m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);   //  	NCFG_ITEM_VL_TO_FTP_SNAP,		    //��Ƶ��ʧ����FTPץͼ��ULONGLONG[m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);  //	NCFG_ITEM_VL_TO_EMAIL_SNAP,		    //��Ƶ��ʧ����EMAILץͼ��ULONGLONG[m_videoInputNum]
#endif

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadAlarmTrigerVLossCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos)
{
	CalculateAlarmTrigerVLossCfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VL_TO_ALARM_OUT;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	ULONGLONG *pVTAOValue = new ULONGLONG[m_videoInputNum];
#if defined(__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__) || defined(__CUSTOM_CBC__)
	for (int i=0; i<m_videoInputNum; i++)
	{
		pVTAOValue[i] = (ULONGLONG)(0x01 << (i%m_sensorOutputNum));
	}
#else
	memset(pVTAOValue, 0, sizeof(ULONGLONG)*m_videoInputNum);
#endif	
	memcpy(pBuf+currPos+itemHeadLen, pVTAOValue, pItemHead->len);
	delete [] pVTAOValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VL_TO_RECORD;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	ULONGLONG *pVTRValue = new ULONGLONG[m_videoInputNum];
	for (int i=0; i<m_videoInputNum; i++)
	{
		pVTRValue[i] = (ULONGLONG)(0x01<<i);		
	}
	memcpy(pBuf+currPos+itemHeadLen, pVTRValue, pItemHead->len);
	delete [] pVTRValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VL_TO_BIG_VIEW;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	ULONGLONG *pVTBVValue = new ULONGLONG[m_videoInputNum];
	memset(pVTBVValue, 0, sizeof(ULONGLONG)*m_videoInputNum);
	for (int i=0; i<m_videoInputNum; i++)
	{
		pVTBVValue[i] = 0;//(ULONGLONG)(0x01 << i);
	}
	memcpy(pBuf+currPos+itemHeadLen, pVTBVValue, pItemHead->len);
	delete [] pVTBVValue;
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VL_TO_EMAIL_ENABLE;
	pItemHead->len = (sizeof(unsigned long)) * m_videoInputNum;
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long *pEmailEn = new unsigned long[m_videoInputNum];
	memset(pEmailEn, 0, sizeof(unsigned long)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pEmailEn,pItemHead->len);
	delete []pEmailEn;
	pEmailEn = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VL_TO_EMAIL_ADDR;
	pItemHead->len = (sizeof(unsigned long)) * m_videoInputNum;
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long *pEmailAddr = new unsigned long[m_videoInputNum];
	memset(pEmailAddr, 0, sizeof(unsigned long)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pEmailAddr,pItemHead->len);
	delete []pEmailAddr;
	pEmailAddr = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VL_TO_PTZ;
	pItemHead->len = (sizeof(TO_PTZ)*m_videoInputNum*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(TO_PTZ)*m_videoInputNum;
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	TO_PTZ *pVLTPValue = new TO_PTZ[m_videoInputNum*m_videoInputNum];
	memset(pVLTPValue, 0, sizeof(TO_PTZ)*m_videoInputNum*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pVLTPValue, pItemHead->len);
	delete [] pVLTPValue;
	}
	currPos += itemHeadLen+pItemHead->len;

 	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
 	pItemHead->itemID = NCFG_ITEM_VL_TO_FTP_ENABLE;
 	pItemHead->len = (sizeof(ULONGLONG) * m_videoInputNum);
 	pItemHead->num = m_videoInputNum;
 	pItemHead->subLen = sizeof(ULONGLONG);
 	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
 	if (!bSuccess)
 	{
 		ULONGLONG *pVLTFValue = new ULONGLONG[m_videoInputNum];
 		memset(pVLTFValue, 0, sizeof(ULONGLONG) * m_videoInputNum);
 		memcpy(pBuf+currPos+itemHeadLen, pVLTFValue, pItemHead->len);
 		delete [] pVLTFValue;
 	}
 	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VL_TO_BUZZ;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	unsigned long *pVTBValue = new unsigned long [m_videoInputNum];
	for (int i = 0; i < m_videoInputNum; ++i)
	{
#if defined(__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__) || defined(__CUSTOM_CBC__)
		pVTBValue[i] = 0;
#else
		pVTBValue[i] = 1;
#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
		if (i > 1)
		{
			pVTBValue[i] = 0;
		}
#endif
#endif
	}
	memcpy(pBuf+currPos+itemHeadLen, pVTBValue, pItemHead->len);
	delete [] pVTBValue;
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VL_TO_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	ULONGLONG *pVTSNValue = new ULONGLONG[m_videoInputNum];
	for (int i=0; i<m_videoInputNum; i++)
	{
		pVTSNValue[i] = (ULONGLONG)(0x01<<i);		
	}
	memcpy(pBuf+currPos+itemHeadLen, pVTSNValue, pItemHead->len);
	delete [] pVTSNValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VL_TO_FTP_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	ULONGLONG *pVTFTValue = new ULONGLONG[m_videoInputNum];
	for (int i=0; i<m_videoInputNum; i++)
	{
		pVTFTValue[i] = (ULONGLONG)(0x01<<i);
	}
	memcpy(pBuf+currPos+itemHeadLen, pVTFTValue, pItemHead->len);
	delete [] pVTFTValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_VL_TO_EMAIL_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	ULONGLONG *pVTESValue = new ULONGLONG[m_videoInputNum];
	for (int i=0; i<m_videoInputNum; i++)
	{
		//pVTESValue[i] = (ULONGLONG)(0x01<<i);
		pVTESValue[i] = 0;
	}
	memcpy(pBuf+currPos+itemHeadLen, pVTESValue, pItemHead->len);
	delete [] pVTESValue;
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateAlarmTrigerShelterCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_ALARM_TRIGER_SHELTER_BASE=0x670,
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);	//NCFG_ITEM_SHELTER_TO_ALARM_OUT,		//�ڵ��������������ULONGLONG [m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);	//NCFG_ITEM_SHELTER_TO_RECORD,			//�ڵ�����¼��ULONGLONG [m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);	//NCFG_ITEM_SHELTER_TO_BIG_VIEW,		//�ڵ��������棬ULONGLONG [m_videoInputNum]
#ifdef __DVR_BASIC__		
	num++; len += (sizeof(unsigned long)*m_videoInputNum); //NCFG_ITEM_SHELTER_TO_EMAIL_ENABLE,//�ڵ��ܹ�����EMAIL��unsigned long	
	num++; len += (sizeof(unsigned long)*m_videoInputNum); //NCFG_ITEM_SHELTER_TO_EMAIL_ADDR,	//�ڵ�����EMAIL��ַ��,unsigned long
	num++; len += (sizeof(unsigned long)*m_videoInputNum); //NCFG_ITEM_SHELTER_TO_FTP_ENABLE,	//�ڵ�����FTP��unsigned long
#endif
	num++; len += (sizeof(unsigned long)*m_videoInputNum);	//NCFG_ITEM_SHELTER_TO_BUZZ,			//�ڵ�����BUZZ unsigned long[m_videoInputNum]
	num++; len += (sizeof(TO_PTZ)*m_videoInputNum*m_videoInputNum);	//NCFG_ITEM_SHELTER_TO_PTZ,			//�ڵ�������̨��TO_PTZ [m_videoInputNum][m_videoInputNum]

#ifdef __DVR_BASIC__
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);  	//NCFG_ITEM_SHELTER_TO_SNAP,          //�ڵ�����ץ�� ULONGLONG[m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);     	//NCFG_ITEM_SHELTER_TO_FTP_SNAP,		//�ڵ�����FTPץͼ��ULONGLONG[m_videoInputNum]
	num++; len += (sizeof(ULONGLONG)*m_videoInputNum);   	//NCFG_ITEM_SHELTER_TO_EMAIL_SNAP,	//�ڵ�����EMAILץͼ��ULONGLONG[m_videoInputNum]
#endif

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadAlarmTrigerShelterCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos)
{
	CalculateAlarmTrigerShelterCfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHELTER_TO_ALARM_OUT;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	ULONGLONG *pSTAOValue = new ULONGLONG [m_videoInputNum];
	memset(pSTAOValue, 0, sizeof(ULONGLONG)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pSTAOValue, pItemHead->len);
	delete [] pSTAOValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHELTER_TO_RECORD;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	ULONGLONG *pSTRValue = new ULONGLONG[m_videoInputNum];
	memset(pSTRValue, 0, sizeof(ULONGLONG)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pSTRValue, pItemHead->len);
	delete [] pSTRValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHELTER_TO_BIG_VIEW;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	ULONGLONG *pSTBVValue = new ULONGLONG[m_videoInputNum];
	memset(pSTBVValue, 0, sizeof(ULONGLONG)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pSTBVValue, pItemHead->len);
	delete [] pSTBVValue;
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHELTER_TO_EMAIL_ENABLE;
	pItemHead->len = (sizeof(unsigned long)) * m_videoInputNum;
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long *pEmailEn = new unsigned long[m_videoInputNum];
	memset(pEmailEn, 0, sizeof(unsigned long)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pEmailEn,pItemHead->len);
	delete []pEmailEn;
	pEmailEn = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHELTER_TO_EMAIL_ADDR;
	pItemHead->len = (sizeof(unsigned long)) * m_videoInputNum;
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long *pEmailAddr = new unsigned long[m_videoInputNum];
	memset(pEmailAddr, 0, sizeof(unsigned long)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pEmailAddr,pItemHead->len);
	delete []pEmailAddr;
	pEmailAddr = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHELTER_TO_FTP_ENABLE;
	pItemHead->len = (sizeof(unsigned long)) * m_videoInputNum;
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long *pPTZEn = new unsigned long[m_videoInputNum];
	memset(pPTZEn, 0, sizeof(unsigned long)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pPTZEn,pItemHead->len);
	delete []pPTZEn;
	pPTZEn = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHELTER_TO_BUZZ;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
	unsigned long *pSTBValue = new unsigned long [m_videoInputNum] ;
	memset(pSTBValue, 0, m_videoInputNum * sizeof(pSTBValue[0]));
	memcpy(pBuf+currPos+itemHeadLen, pSTBValue, pItemHead->len);
	delete [] pSTBValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHELTER_TO_PTZ;
	pItemHead->len = (sizeof(TO_PTZ)*m_videoInputNum*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = (sizeof(TO_PTZ)*m_videoInputNum);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	TO_PTZ *pSTPValue = new TO_PTZ [m_videoInputNum*m_videoInputNum];
	memset(pSTPValue, 0, sizeof(TO_PTZ)*m_videoInputNum*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pSTPValue, pItemHead->len);
	delete [] pSTPValue;
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHELTER_TO_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	ULONGLONG *pSTSNValue = new ULONGLONG[m_videoInputNum];
	memset(pSTSNValue, 0, sizeof(ULONGLONG)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pSTSNValue, pItemHead->len);
	delete [] pSTSNValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHELTER_TO_FTP_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	ULONGLONG *pSTFSValue = new ULONGLONG[m_videoInputNum];
	memset(pSTFSValue, 0, sizeof(ULONGLONG)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pSTFSValue, pItemHead->len);
	delete [] pSTFSValue;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHELTER_TO_EMAIL_SNAP;
	pItemHead->len = (sizeof(ULONGLONG)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	ULONGLONG *pSTESValue = new ULONGLONG[m_videoInputNum];
	memset(pSTESValue, 0, sizeof(ULONGLONG)*m_videoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, pSTESValue, pItemHead->len);
	delete [] pSTESValue;
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateAlarmExceptionCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//NCFG_ITEM_EXCEPTION_BASE
	num++; len += (sizeof(unsigned long));	//NCFG_ITEM_DISK_FULL_BUZZER,			//�������������������� unsigned long
	num++; len += (sizeof(ULONGLONG));		//NCFG_ITEM_DISK_FULL_TO_ALARM_OUT,		//����������������� ULONGLONG
#ifdef __DVR_BASIC__
	num++; len += (sizeof(unsigned long));	//NCFG_ITEM_DISK_FULL_TO_EMAIL,			//�����������ʼ� unsigned long
#endif
	num++; len += (sizeof(unsigned long));	//NCFG_ITEM_IP_CONFLICT_BUZZER,			//�����ַ��ͻ�������������� unsigned long
	num++; len += (sizeof(ULONGLONG));		//NCFG_ITEM_IP_CONFLICT_TO_ALARM_OUT,	//�����ַ��ͻ����������� ULONGLONG
#ifdef __DVR_BASIC__	
	num++; len += (sizeof(unsigned long));	//NCFG_ITEM_IP_CONFLICT_TO_EMAIL,		//�����ַ��ͻ�����ʼ� unsigned long
#endif	
	num++; len += (sizeof(unsigned long));	//NCFG_ITEM_DISCONNECT_BUZZER,			//����Ͽ��������������� unsigned long
	num++; len += (sizeof(ULONGLONG));		//NCFG_ITEM_DISCONNECT_TO_ALARM_OUT,	//����Ͽ������������ ULONGLONG
#ifdef __DVR_BASIC__	
	num++; len += (sizeof(unsigned long));	//NCFG_ITEM_DISCONNECT_TO_EMAIL,		//����Ͽ������ʼ� unsigned long
#endif

	num++; len += (sizeof(unsigned long));  //NCFG_ITEM_DISK_WARNING_BUZZER,         //Ӳ��˥�������������� unsigned long
	num++; len += (sizeof(ULONGLONG));	    //NCFG_ITEM_DISK_WARNING_TO_ALARM_OUT,    //Ӳ��˥������������� ULONGLONG
	num++; len += (sizeof(unsigned long));	//NCFG_ITEM_DISK_WARNING_TO_EMAIL,        //Ӳ��˥�������ʼ� unsigned long

	num++; len += (sizeof(unsigned long));  //NCFG_ITEM_DISK_DISCONNECT_BUZZER,         //Ӳ�̵��ߴ��������� unsigned long
	num++; len += (sizeof(ULONGLONG));	    //NCFG_ITEM_DISK_DISCONNECT_TO_ALARM_OUT,    //Ӳ�̵��ߴ���������� ULONGLONG
	num++; len += (sizeof(unsigned long));	//NCFG_ITEM_DISK_DISCONNECT_TO_EMAIL,        //Ӳ�̵��ߴ����ʼ� unsigned long
	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadAlarmExceptionCfg(unsigned char **ppConfig, unsigned long &len, unsigned short &num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos)
{
	CalculateAlarmExceptionCfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	unsigned long currPos = 0;
	unsigned long value = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISK_FULL_BUZZER;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
		unsigned long buzzer = 1;
		memcpy(pBuf+currPos+itemHeadLen, &buzzer, pItemHead->len);
#else
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
#endif
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISK_FULL_TO_ALARM_OUT;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISK_FULL_TO_EMAIL;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_IP_CONFLICT_BUZZER;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_IP_CONFLICT_TO_ALARM_OUT;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_IP_CONFLICT_TO_EMAIL;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISCONNECT_BUZZER;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISCONNECT_TO_ALARM_OUT;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISCONNECT_TO_EMAIL;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISK_WARNING_BUZZER;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
		unsigned long buzzer = 1;
		memcpy(pBuf+currPos+itemHeadLen, &buzzer, pItemHead->len);
#else
		memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
#endif
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISK_WARNING_TO_ALARM_OUT;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISK_WARNING_TO_EMAIL;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	//zxx�¼�
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISK_DISCONNECT_BUZZER;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long buzz=1;
		memcpy(pBuf+currPos+itemHeadLen, &buzz, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISK_DISCONNECT_TO_ALARM_OUT;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DISK_DISCONNECT_TO_EMAIL;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{

	memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;
	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateEncodeCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_ENCODE_BASE,
	num++; len += (sizeof(ENCODE_INFO)*m_localVideoInputNum);	//NCFG_ITEM_ENCODE_MAJOR,		//������������unsigned long [m_videoInputNum]��ÿ��ͨ����ͬ
	num++; len += (sizeof(ENCODE_INFO)*m_localVideoInputNum);	//NCFG_ITEM_ENCODE_MINOR,		//����������, unsigned long [m_videoInputNum]��ÿ��ͨ����ͬ
	num++; len += (sizeof(ENCODE_INFO)*m_localVideoInputNum);	//NCFG_ITEM_ENCODE_PHONE,		//�ֻ��������������ENCODE_INFO [m_videoInputNum]
	num++; len += (sizeof(ENCODE_INFO)*m_localVideoInputNum);	//NCFG_ITEM_ENCODE_MAJOR_EVENT  //������(�¼�)���������ENCODE_INFO [m_videoInputNum]
	num++; len += (sizeof(unsigned long)*m_videoInputNum);	//NCFG_ITEM_JPEG_PERIOD,		//JPEGץͼ�����unsigned long [m_videoInputNum]
	num++; len += (sizeof(unsigned long)*m_videoInputNum); //NCFG_ITEM_JPEG_SIZE,                //JPEGץͼ�ֱ��ʣ�unsigned long [m_videoInputNum]
	num++; len += (sizeof(unsigned long)*m_videoInputNum); //NCFG_ITEM_JPEG_QUALITY,             //JPEGץͼ���ʣ�unsigned long [m_videoInputNum]
	num++; len += (sizeof(unsigned long)*m_videoInputNum); //NCFG_ITEM_JPEG_SNAP_NUM,            //JPEGץͼ������unsigned long [m_videoInputNum]
    num++; len += (sizeof(NCFG_INFO_ENCODE_SPEC_FRAME_RATE)*1); //NCFG_ITEM_ENCODE_MAJOR_SPEC  ¼��ķֱ��ʵ��������ã�������2432�ϣ�1,5,9,13,17,21,25,29�������ó�D1��CIF��������ͨ��ֻ����CIF
	num++; len += (sizeof(ENCODE_INFO_EX)*m_videoInputNum);		//NCFG_ITEM_ENCODE_MAJOR_EX
	if(m_netVideoInputNum > 0)
	{
		num++; len += sizeof(NCFG_IPC_SUPPORT_ENCODE);				//NCFG_ITEM_IPC_SUPPORT_ENCODE
	}
	num++; len += (sizeof(ENCODE_INFO_EX)*m_videoInputNum);		//NCFG_ITEM_ENCODE_MINOR_EX
	num++; len += (sizeof(VS_SUPBRR)*g_resolutionNum);						//NCFG_ITEM_BITRANGE_FOR_VIDEOSIZE
	num++; len += (sizeof(CHANNEL_RECP)*m_videoInputNum);	//NCFG_ITEM_MAJOR_CHANNEL_RECORD_PARAM_EX
	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadEncodeCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, bool bDefaultData/* = false*/, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos)
{
	CalculateEncodeCfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;

	ENCODE_INFO *pEMRValue = new ENCODE_INFO [m_videoInputNum];
	unsigned long videoSize, hBitStream, rate;
	unsigned char encodeType, quality;
	VIDEO_FORMAT videoFormatTemp;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_ENCODE_MAJOR;
	pItemHead->len = sizeof(ENCODE_INFO)*m_localVideoInputNum;
	pItemHead->num =  m_localVideoInputNum;
	pItemHead->subLen = sizeof(ENCODE_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		videoSize = CProduct::Instance()->LocalDefaVideoSize();
		encodeType = CProduct::Instance()->LocalDefauVideoEncodeMode();
		quality = CProduct::Instance()->LocalDefQuiltyValue();
		hBitStream = CProduct::Instance()->LocalDefaBitRateValue();

		//��ʱʵ��֡�ʣ���ΪĬ�ϲ���ΪNTSC�����԰�����ʹ��NTSC��ֵ
		rate = CProduct::Instance()->LocalDefaChnnRateNumNtsc();
		//2010-03-24 19:32:00 YSW
		//�����Ͽ��Զ�ȡĬ�����ݣ��ߵ�·����һ���ȡ���ݲ�ͬ��
		//������֡�ʵĲ��֣������ڻ�ȡ������ʱ����ת����ԭ�����ڶ�ȡĬ������ʱ������֡�ʸ����ͳ�ȥ
		//�����ڴ������жϣ������Ƕ�ȡĬ������ʱ���Ž���ת����������ʵ��֡�ʡ�
		videoFormatTemp = static_cast<VIDEO_FORMAT>(CLocalDevice::GetVideoFormatFromFlash()); //��ȡflash�����Ƶ��ʽ��Ϣ
		if (!bDefaultData)
		{
			rate = RateToVRate(rate, VIDEO_FORMAT_NTSC);
		}
		else if (VIDEO_FORMAT_PAL == videoFormatTemp)
		{
			//����ǻָ�Ĭ�����ã�����ݵ�ǰ����ʽ������ע��NTSC�Ѿ�����Ҫ���¸�ֵ��
			rate = CProduct::Instance()->LocalDefaChnnRateNumPal();
		}
		PRODUCT_TYPE productTypeTemp = CProduct::Instance()->ProductType();
		switch(productTypeTemp)
		{
			case TD_2704HD:
				{
					for (int i = 0; i< 4; i++)
					{
						pEMRValue[i].resolution = DVRVIDEO_SIZE_HD1080;
						pEMRValue[i].rate = rate;
						pEMRValue[i].encodeType = encodeType;
						pEMRValue[i].quality = quality;
						pEMRValue[i].lBitStream = 64;
						pEMRValue[i].hBitStream = hBitStream;
					}
					for (int i = 4; i< m_videoInputNum; i++)
					{
						pEMRValue[i].resolution = DVRVIDEO_SIZE_D1;
						pEMRValue[i].rate = rate;
						pEMRValue[i].encodeType = encodeType;
						pEMRValue[i].quality = quality;
						pEMRValue[i].lBitStream = 64;
						pEMRValue[i].hBitStream = 1536;
					}
				}
				break;
			case TD_2304SS_SDI:
				{
					for (int i = 0; i< 1; i++)
					{
						pEMRValue[i].resolution = DVRVIDEO_SIZE_HD720;
						pEMRValue[i].rate = rate;
						pEMRValue[i].encodeType = encodeType;
						pEMRValue[i].quality = quality;
						pEMRValue[i].lBitStream = 64;
						pEMRValue[i].hBitStream = 4096;
					}
					for (int i = 1; i< m_videoInputNum; i++)
					{
						pEMRValue[i].resolution = DVRVIDEO_SIZE_960H;
						pEMRValue[i].rate = rate;
						pEMRValue[i].encodeType = encodeType;
						pEMRValue[i].quality = quality;
						pEMRValue[i].lBitStream = 64;
						pEMRValue[i].hBitStream = 2048;
					}
				}
				break;
			case TD_2308SS_SDI:
				{
					for (int i = 0; i< 2; i++)
					{
						pEMRValue[i].resolution = DVRVIDEO_SIZE_HD720;
						pEMRValue[i].rate = rate;
						pEMRValue[i].encodeType = encodeType;
						pEMRValue[i].quality = quality;
						pEMRValue[i].lBitStream = 64;
						pEMRValue[i].hBitStream = 4096;
					}
					for (int i = 2; i< m_videoInputNum; i++)
					{
						pEMRValue[i].resolution = DVRVIDEO_SIZE_960H;
						pEMRValue[i].rate = rate;
						pEMRValue[i].encodeType = encodeType;
						pEMRValue[i].quality = quality;
						pEMRValue[i].lBitStream = 64;
						pEMRValue[i].hBitStream = 2048;
					}
				}
				break;
			case TD_2508HE_SDI:
				{
					for (int i = 0; i< 2; i++)
					{
						pEMRValue[i].resolution = DVRVIDEO_SIZE_HD720;
						pEMRValue[i].rate = rate;
						pEMRValue[i].encodeType = encodeType;
						pEMRValue[i].quality = quality;
						pEMRValue[i].lBitStream = 64;
						pEMRValue[i].hBitStream = 4096;
					}
					for (int i = 2; i< m_videoInputNum; i++)
					{
						pEMRValue[i].resolution = DVRVIDEO_SIZE_960H;
						pEMRValue[i].rate = rate;
						pEMRValue[i].encodeType = encodeType;
						pEMRValue[i].quality = quality;
						pEMRValue[i].lBitStream = 64;
						pEMRValue[i].hBitStream = 2048;
					}
				}
				break;
			case TD_2532HD_SDI:
				{
					for (int i = 0; i< 4; i++)
					{
						pEMRValue[i].resolution = DVRVIDEO_SIZE_HD1080;
						pEMRValue[i].rate = rate;
						pEMRValue[i].encodeType = encodeType;
						pEMRValue[i].quality = quality;
						pEMRValue[i].lBitStream = 64;
						pEMRValue[i].hBitStream = 4096;
					}
					for (int i = 4; i< m_videoInputNum; i++)
					{
						pEMRValue[i].resolution = DVRVIDEO_SIZE_960H;
						pEMRValue[i].rate = rate;
						pEMRValue[i].encodeType = encodeType;
						pEMRValue[i].quality = quality;
						pEMRValue[i].lBitStream = 64;
						pEMRValue[i].hBitStream = 2048;
					}
				}
				break;
			default:
				{
					for (int i = 0; i< m_localVideoInputNum; i++)
					{
						pEMRValue[i].resolution = videoSize;
						pEMRValue[i].rate = rate;
						pEMRValue[i].encodeType = encodeType;
						pEMRValue[i].quality = quality;
						pEMRValue[i].lBitStream = 64;
						pEMRValue[i].hBitStream = hBitStream;
					}
				}
				break;
		}
		memcpy(pBuf+currPos+itemHeadLen, pEMRValue, pItemHead->len);		
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_ENCODE_MINOR;
	pItemHead->len = (sizeof(ENCODE_INFO)*m_localVideoInputNum);
	pItemHead->num = m_localVideoInputNum;
	pItemHead->subLen = sizeof(ENCODE_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		videoSize = CProduct::Instance()->NetDefaVideoSize();
		encodeType = CProduct::Instance()->NetDefauVideoEncodeMode();
		quality = CProduct::Instance()->NetDefQuiltyValue();
		hBitStream = CProduct::Instance()->NetDefaBitRateValue(); 

		
		rate = CProduct::Instance()->NetDefaChnnRateNumNtsc();

		if (!bDefaultData)
		{
			rate = RateToVRate(rate, VIDEO_FORMAT_NTSC);
		}
		else if (VIDEO_FORMAT_PAL == videoFormatTemp)
		{
			//����ǻָ�Ĭ�����ã�����ݵ�ǰ����ʽ������ע��NTSC�Ѿ�����Ҫ���¸�ֵ��
			rate = CProduct::Instance()->NetDefaChnnRateNumPal();
		}

	for (int i = 0; i< m_localVideoInputNum; i++)
	{
		pEMRValue[i].resolution = videoSize;
		pEMRValue[i].rate = rate;
		pEMRValue[i].encodeType = encodeType;
		pEMRValue[i].quality = quality;
		pEMRValue[i].lBitStream = 64;
		pEMRValue[i].hBitStream = hBitStream;
	}
	memcpy(pBuf+currPos+itemHeadLen, pEMRValue, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_ENCODE_PHONE;
	pItemHead->len = (sizeof(ENCODE_INFO)*m_localVideoInputNum);
	pItemHead->num = m_localVideoInputNum;
	pItemHead->subLen = sizeof(ENCODE_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		videoSize = DVRVIDEO_SIZE_QCIF;
		encodeType = VIDEO_ENCODE_MODE_CBR;
		quality = IMAGE_QUALITY_MEDIUM;
		hBitStream = 128; 

		rate = 1;
		if (!bDefaultData)
		{
			rate = RateToVRate(rate, VIDEO_FORMAT_NTSC);
		}

		for (int i = 0; i< m_localVideoInputNum; i++)
		{
			pEMRValue[i].resolution = videoSize;
			pEMRValue[i].rate = rate;
			pEMRValue[i].encodeType = encodeType;
			pEMRValue[i].quality = quality;
			pEMRValue[i].lBitStream = 64;
			pEMRValue[i].hBitStream = hBitStream;
		}
		memcpy(pBuf+currPos+itemHeadLen, pEMRValue, pItemHead->len);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_ENCODE_MAJOR_EVENT;
	pItemHead->len = (sizeof(ENCODE_INFO)*m_localVideoInputNum);
	pItemHead->num = m_localVideoInputNum;
	pItemHead->subLen = sizeof(ENCODE_INFO);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		videoSize = DVRVIDEO_SIZE_CIF;
		encodeType = VIDEO_ENCODE_MODE_CBR;
		quality = IMAGE_QUALITY_MEDIUM;
		hBitStream = 768; 
		for (int i = 0; i< m_localVideoInputNum; i++)
		{
			pEMRValue[i].resolution = videoSize;
			pEMRValue[i].rate = 25;
			pEMRValue[i].encodeType = encodeType;
			pEMRValue[i].quality = quality;
			pEMRValue[i].lBitStream = 64;
			pEMRValue[i].hBitStream = hBitStream;
		}
		memcpy(pBuf+currPos+itemHeadLen, pEMRValue, pItemHead->len);		
	}
	currPos += itemHeadLen+pItemHead->len;	
	delete [] pEMRValue;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_JPEG_PERIOD;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long *pTimeTemp = new unsigned long [m_videoInputNum];
		for (int i = 0; i< m_videoInputNum; i++)
		{
#ifdef __SUPPORT_SNAPD1__
			//�Ϻ�ƽ̨ץͼ�������ҲҪ3��
			{
				pTimeTemp[i] = 3;
			}
#else
			pTimeTemp[i] = CProduct::Instance()->SnapDefaTimeIntervalValue();
#endif
		}
		memcpy(pBuf+currPos+itemHeadLen, pTimeTemp, pItemHead->len);
		delete [] pTimeTemp;
		pTimeTemp = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;


	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_JPEG_SIZE;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long *pJpegSizeTemp = new unsigned long [m_videoInputNum];
		for (int i = 0; i< m_videoInputNum; i++)
		{
			pJpegSizeTemp[i] = CProduct::Instance()->SnapDefPictureSize();
		}
		memcpy(pBuf+currPos+itemHeadLen, pJpegSizeTemp, pItemHead->len);
		delete [] pJpegSizeTemp;
		pJpegSizeTemp = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_JPEG_QUALITY;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long *pJpegQualityTemp = new unsigned long [m_videoInputNum];
		for (int i = 0; i< m_videoInputNum; i++)
		{
			pJpegQualityTemp[i] = CProduct::Instance()->SnapDefQuiltyValue();
		}
		memcpy(pBuf+currPos+itemHeadLen, pJpegQualityTemp, pItemHead->len);
		delete [] pJpegQualityTemp;
		pJpegQualityTemp = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_JPEG_SNAP_NUM;
	pItemHead->len = (sizeof(unsigned long)*m_videoInputNum);
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(unsigned long);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		unsigned long *pJpegNumTemp = new unsigned long [m_videoInputNum];
		for (int i = 0; i< m_videoInputNum; i++)
		{
			pJpegNumTemp[i] = SNAP_PIC_MAX_NUM; 
		}
		memcpy(pBuf+currPos+itemHeadLen, pJpegNumTemp, pItemHead->len);
		delete [] pJpegNumTemp;
		pJpegNumTemp = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_ENCODE_MAJOR_SPEC;  //�������ÿ�ζ�Ҫ��������
	pItemHead->len = (sizeof(NCFG_INFO_ENCODE_SPEC_FRAME_RATE)*1);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NCFG_INFO_ENCODE_SPEC_FRAME_RATE);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	//if (!bSuccess)
	{
		NCFG_INFO_ENCODE_SPEC_FRAME_RATE supportFrameRate;
		memset(&supportFrameRate, 0, sizeof(NCFG_INFO_ENCODE_SPEC_FRAME_RATE));
		supportFrameRate.bSupport = CProduct::Instance()->LocalVideoSizeSpecial();
		supportFrameRate.videoSizeCIFChnnBits = CProduct::Instance()->LocalVideoSizeCIFChnnBits();
		supportFrameRate.videoSizeD1ChnnBits = CProduct::Instance()->LocalVideoSizeD1ChnnBits();			
		memcpy(pBuf+currPos+itemHeadLen, &supportFrameRate, pItemHead->len);	
	}
	currPos += itemHeadLen+pItemHead->len;

	videoSize = CProduct::Instance()->LocalDefaVideoSize();
	encodeType = CProduct::Instance()->LocalDefauVideoEncodeMode();
	quality = CProduct::Instance()->LocalDefQuiltyValue();
	hBitStream = CProduct::Instance()->LocalDefaBitRateValue();
	rate = CProduct::Instance()->LocalDefaChnnRateNumNtsc();

	unsigned long ipcVideoSize = CProduct::Instance()->IPCDefVideoSize();
	unsigned long hIpcBitStream = CProduct::Instance()->IPCDefaBitRateValue();
	unsigned long ipcEncodeType = CProduct::Instance()->IPCDefauVideoEncodeMode();
	unsigned long ipcDefaRate = CProduct::Instance()->IPCDefaChnnRate();

	if(VIDEO_FORMAT_NTSC == videoFormatTemp)
	{
#ifndef __CHIP3520__
		ipcDefaRate = (ipcDefaRate + 1) * 6 / 5;
		if (30 < ipcDefaRate)
		{
			ipcDefaRate = 30;
		}
#endif
	}

	if (!bDefaultData)
	{
		rate = RateToVRate(rate, VIDEO_FORMAT_NTSC);
		ipcDefaRate = RateToVRate(ipcDefaRate, videoFormatTemp);
	}
	else if (VIDEO_FORMAT_PAL == videoFormatTemp)
	{
		//����ǻָ�Ĭ�����ã�����ݵ�ǰ����ʽ������ע��NTSC�Ѿ�����Ҫ���¸�ֵ��
		rate = CProduct::Instance()->LocalDefaChnnRateNumPal();
	}

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_ENCODE_MAJOR_EX;
	pItemHead->len = sizeof(ENCODE_INFO_EX)*m_videoInputNum;
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ENCODE_INFO_EX);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if(!bSuccess)
	{

		ENCODE_INFO_EX *pEncodeEx = new ENCODE_INFO_EX[m_videoInputNum];

		PRODUCT_TYPE productTypeTemp = CProduct::Instance()->ProductType();
		switch(productTypeTemp)
		{
		case TD_2304SS_SDI:
			{
				for (int i = 0; i< 1; i++)
				{
					pEncodeEx[i].resolution = DVRVIDEO_SIZE_HD720;
					pEncodeEx[i].rate = rate;
					pEncodeEx[i].encodeType = encodeType;
					pEncodeEx[i].quality = quality;
					pEncodeEx[i].lBitStream = 64;
					pEncodeEx[i].hBitStream = 4096;
				}
				for (int i = 1; i< m_videoInputNum; i++)
				{
					pEncodeEx[i].resolution = DVRVIDEO_SIZE_960H;
					pEncodeEx[i].rate = rate;
					pEncodeEx[i].encodeType = encodeType;
					pEncodeEx[i].quality = quality;
					pEncodeEx[i].lBitStream = 64;
					pEncodeEx[i].hBitStream = 2048;
				}
			}
			break;
		case TD_2308SS_SDI:
			{
				for (int i = 0; i< 2; i++)
				{
					pEncodeEx[i].resolution = DVRVIDEO_SIZE_HD720;
					pEncodeEx[i].rate = rate;
					pEncodeEx[i].encodeType = encodeType;
					pEncodeEx[i].quality = quality;
					pEncodeEx[i].lBitStream = 64;
					pEncodeEx[i].hBitStream = 4096;
				}
				for (int i = 2; i< m_videoInputNum; i++)
				{
					pEncodeEx[i].resolution = DVRVIDEO_SIZE_960H;
					pEncodeEx[i].rate = rate;
					pEncodeEx[i].encodeType = encodeType;
					pEncodeEx[i].quality = quality;
					pEncodeEx[i].lBitStream = 64;
					pEncodeEx[i].hBitStream = 2048;
				}
			}
			break;
		case TD_2508HE_SDI:
			{
				for (int i = 0; i< 2; i++)
				{
					pEncodeEx[i].resolution = DVRVIDEO_SIZE_HD720;
					pEncodeEx[i].rate = rate;
					pEncodeEx[i].encodeType = encodeType;
					pEncodeEx[i].quality = quality;
					pEncodeEx[i].lBitStream = 64;
					pEncodeEx[i].hBitStream = 4096;
				}
				for (int i = 2; i< m_videoInputNum; i++)
				{
					pEncodeEx[i].resolution = DVRVIDEO_SIZE_960H;
					pEncodeEx[i].rate = rate;
					pEncodeEx[i].encodeType = encodeType;
					pEncodeEx[i].quality = quality;
					pEncodeEx[i].lBitStream = 64;
					pEncodeEx[i].hBitStream = 2048;
				}
			}
			break;
		case TD_2532HD_SDI:
			{
				for (int i = 0; i< 4; i++)
				{
					pEncodeEx[i].resolution = DVRVIDEO_SIZE_HD720;
					pEncodeEx[i].rate = rate;
					pEncodeEx[i].encodeType = encodeType;
					pEncodeEx[i].quality = quality;
					pEncodeEx[i].lBitStream = 64;
					pEncodeEx[i].hBitStream = 4096;
				}
				for (int i = 4; i< m_videoInputNum; i++)
				{
					pEncodeEx[i].resolution = DVRVIDEO_SIZE_960H;
					pEncodeEx[i].rate = rate;
					pEncodeEx[i].encodeType = encodeType;
					pEncodeEx[i].quality = quality;
					pEncodeEx[i].lBitStream = 64;
					pEncodeEx[i].hBitStream = 2048;
				}
			}
			break;
		default:
			{
				for (int i = 0; i< m_videoInputNum; i++)
				{
					pEncodeEx[i].resolution = videoSize;
					pEncodeEx[i].rate = rate;
					pEncodeEx[i].encodeType = encodeType;
					pEncodeEx[i].quality = quality;
					pEncodeEx[i].lBitStream = 64;
					pEncodeEx[i].hBitStream = hBitStream;
				}
			}
			break;
		}

		for(int i = m_localVideoInputNum; i < m_videoInputNum; i++)
		{
			pEncodeEx[i].resolution = ipcVideoSize;
			pEncodeEx[i].rate = ipcDefaRate;
			pEncodeEx[i].hBitStream = hIpcBitStream;
			pEncodeEx[i].encodeType = ipcEncodeType;
		}
		memcpy(pBuf+currPos+itemHeadLen, pEncodeEx, pItemHead->len);

		delete [] pEncodeEx;
		pEncodeEx = NULL;
	}	
	currPos += itemHeadLen+pItemHead->len;

	if(m_netVideoInputNum > 0)
	{
		pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
		pItemHead->itemID = NCFG_ITEM_IPC_SUPPORT_ENCODE;
		pItemHead->len = sizeof(NCFG_IPC_SUPPORT_ENCODE);
		pItemHead->num = 1;
		pItemHead->subLen = sizeof(NCFG_IPC_SUPPORT_ENCODE);
		CProduct * pProduct = CProduct::Instance();

		NCFG_IPC_SUPPORT_ENCODE *pSupportEncode = new NCFG_IPC_SUPPORT_ENCODE;
		pSupportEncode->dwResolution = pProduct->GetDigitChannelSupportVideoSize()[m_localVideoInputNum];
		pSupportEncode->maxChnnRate = CProduct::Instance()->IPCMaxChnnRate();
		pSupportEncode->maxTotalRateOnVGA = CProduct::Instance()->IPCMaxTotalRateOnVGA();
		pSupportEncode->maxChnnRateOnVGA = CProduct::Instance()->IPCMaxChnnRateOnVGA();
		pSupportEncode->maxTotalBitRate = CProduct::Instance()->GetDigitMaxTotalBitRate();
		pSupportEncode->defaBitRate = CProduct::Instance()->IPCDefaBitRateValue();
		pSupportEncode->defaChnnRate = CProduct::Instance()->IPCDefaChnnRate();
		pSupportEncode->defaResolution = CProduct::Instance()->IPCDefVideoSize();
		pSupportEncode->bLimitByBitRate = CProduct::Instance()->IPCIsLimitByBitRate();

		memcpy(pBuf+currPos+itemHeadLen, pSupportEncode, pItemHead->len);
		currPos += itemHeadLen+pItemHead->len;

		delete pSupportEncode;
	}

	videoSize = CProduct::Instance()->NetDefaVideoSize();
	encodeType = CProduct::Instance()->NetDefauVideoEncodeMode();
	quality = CProduct::Instance()->NetDefQuiltyValue();
	hBitStream = CProduct::Instance()->NetDefaBitRateValue();
	rate = CProduct::Instance()->NetDefaChnnRateNumNtsc();

	ipcVideoSize = CProduct::Instance()->IPCNetDefVideoSize();
	hIpcBitStream = CProduct::Instance()->IPCNetDefaBitRateValue();
	ipcEncodeType = CProduct::Instance()->IPCNetDefauVideoEncodeMode();
	ipcDefaRate = CProduct::Instance()->IPCNetDefaChnnRate();

	if(VIDEO_FORMAT_NTSC == videoFormatTemp)
	{
		ipcDefaRate = ipcDefaRate * 6 / 5;
	}

	if (!bDefaultData)
	{
		rate = RateToVRate(rate, VIDEO_FORMAT_NTSC);
		ipcDefaRate = RateToVRate(ipcDefaRate, videoFormatTemp);
	}
	else if (VIDEO_FORMAT_PAL == videoFormatTemp)
	{
		//����ǻָ�Ĭ�����ã�����ݵ�ǰ����ʽ������ע��NTSC�Ѿ�����Ҫ���¸�ֵ��
		rate = CProduct::Instance()->NetDefaChnnRateNumPal();
	}

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_ENCODE_MINOR_EX;
	pItemHead->len = sizeof(ENCODE_INFO_EX)*m_videoInputNum;
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(ENCODE_INFO_EX);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if(!bSuccess)
	{
		ENCODE_INFO_EX *pEncodeEx = new ENCODE_INFO_EX[m_videoInputNum];
		for (int i = 0; i< m_videoInputNum; i++)
		{
			pEncodeEx[i].resolution = videoSize;
			pEncodeEx[i].rate = rate;
			pEncodeEx[i].encodeType = encodeType;
			pEncodeEx[i].quality = quality;
			pEncodeEx[i].lBitStream = 64;
			pEncodeEx[i].hBitStream = hBitStream;
		}
		for(int i = m_localVideoInputNum; i < m_videoInputNum; i++)
		{
			pEncodeEx[i].resolution = ipcVideoSize;
			pEncodeEx[i].rate = ipcDefaRate;
			pEncodeEx[i].hBitStream = hIpcBitStream;
			pEncodeEx[i].encodeType = ipcEncodeType;
		}

		memcpy(pBuf+currPos+itemHeadLen, pEncodeEx, pItemHead->len);
		delete [] pEncodeEx;
		pEncodeEx=NULL;

	}
	currPos += itemHeadLen+pItemHead->len;
	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_BITRANGE_FOR_VIDEOSIZE;
	pItemHead->len = sizeof(VS_SUPBRR)*g_resolutionNum;
	pItemHead->num = g_resolutionNum;
	pItemHead->subLen = sizeof(VS_SUPBRR);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if(!bSuccess)
	{
		VS_SUPBRR *pVs = new VS_SUPBRR[g_resolutionNum];
		memset(pVs,0,sizeof(VS_SUPBRR)*g_resolutionNum);
		CProduct::Instance()->GetVSforBrr(pVs);
		memcpy(pBuf+currPos+itemHeadLen, pVs, pItemHead->len);
		delete []pVs;
		pVs = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;
	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_MAJOR_CHANNEL_RECORD_PARAM_EX;
	pItemHead->len = sizeof(CHANNEL_RECP )*m_videoInputNum;
	pItemHead->num = m_videoInputNum;
	pItemHead->subLen = sizeof(CHANNEL_RECP);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if(!bSuccess)
	{
		CHANNEL_RECP *pChRecp = new CHANNEL_RECP[m_videoInputNum];
		memset(pChRecp,0,sizeof(CHANNEL_RECP)*m_videoInputNum);
		for (unsigned char ch = 0; ch < m_videoInputNum; ++ch)
		{
			CProduct::Instance()->GetChannelRecp(ch, pChRecp + ch);
		}
		memcpy(pBuf+currPos+itemHeadLen, pChRecp, pItemHead->len);
		delete []pChRecp;
		pChRecp = NULL;
	}
	currPos += itemHeadLen+pItemHead->len;
	
	*ppConfig = pBuf;
	assert(len == currPos);
}

#ifdef __GB28181_SERVER__
void CConfigEx::CalculateGB28181CfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	num++; len += sizeof(GB28181_SIP_SERVER);	//NCFG_ITEM_GB28181_SERVER_INFO,
	num++; len += sizeof(GB28181_CHANNEL_ID);	//NCFG_ITEM_GB28181_CHANNEL_ID
	num++; len += sizeof(GB28181_ALARM_ID);		//NCFG_ITEM_GB28181_ALARM_ID
	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadGB28181Cfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, bool bDefaultData /* = false */, unsigned char *ppBackupConfig /* = NULL */, unsigned long backupLen /* = 0 */, unsigned long* pos /* = NULL */)
{
	CalculateGB28181CfgSize(num, len);
	bool bSuccess = false;
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_GB28181_SERVER_INFO;
	pItemHead->len = sizeof(GB28181_SIP_SERVER);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(GB28181_SIP_SERVER);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		GB28181_SIP_SERVER * pSipServer = (GB28181_SIP_SERVER *)(pBuf+currPos+itemHeadLen);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_GB28181_CHANNEL_ID;
	pItemHead->len = sizeof(GB28181_CHANNEL_ID);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(GB28181_CHANNEL_ID);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		GB28181_CHANNEL_ID * pChannelID= (GB28181_CHANNEL_ID *)(pBuf+currPos+itemHeadLen);
	}
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_GB28181_ALARM_ID;
	pItemHead->len = sizeof(GB28181_ALARM_ID);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(GB28181_ALARM_ID);
	bSuccess = GetItemDataFromBackupConfig(pBuf + currPos, ppBackupConfig, backupLen, pos);
	if (!bSuccess)
	{
		GB28181_ALARM_ID * pChannelID= (GB28181_ALARM_ID *)(pBuf+currPos+itemHeadLen);
	}
	currPos += itemHeadLen+pItemHead->len;

	*ppConfig = pBuf;
	assert(len == currPos);
}
#endif

#ifdef __MOBILE_DVR__
void CConfigEx::CalculateMobileCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//NCFG_ITEM_MOBILE_BASE
	num++; len += sizeof(NCFG_INFO_LONG_NAME);	//NCFG_ITEM_COMPANY_NAME,					//���ӹ�˾����, NCFG_INFO_LONG_NAME
	num++; len += sizeof(NCFG_INFO_SHORT_NAME);	//NCFG_ITEM_DRIVER_NAME,					//˾������, NCFG_INFO_SHORT_NAME
	num++; len += sizeof(NCFG_INFO_SHORT_NAME);	//NCFG_ITEM_CAR_NUMBER,					//��������, NCFG_INFO_SHORT_NAME
	num++; len += sizeof(unsigned long);		//NCFG_ITEM_SHUT_DOWN_DELAY,				//�ػ���ʱ, unsigned long
	num++; len += sizeof(MDVR_NETSERVER_CFG);	//NCFG_ITEM_NET_SERVER,					//�����������MDVR_NETSERVER_CFG

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadMobileCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num)
{
	CalculateMobileCfgSize(num, len);
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_COMPANY_NAME;
	pItemHead->len = sizeof(NCFG_INFO_LONG_NAME);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NCFG_INFO_LONG_NAME);
	strcpy((char *)(pBuf+currPos+itemHeadLen), "TVTͬΪ����");
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_DRIVER_NAME;
	pItemHead->len = sizeof(NCFG_INFO_SHORT_NAME);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NCFG_INFO_SHORT_NAME);
	strcpy((char *)(pBuf+currPos+itemHeadLen),"��Զ��");
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_CAR_NUMBER;
	pItemHead->len = sizeof(NCFG_INFO_SHORT_NAME);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(NCFG_INFO_SHORT_NAME);
	//memset(pBuf+currPos+itemHeadLen, 0, pItemHead->len);
	strcpy((char *)(pBuf+currPos+itemHeadLen), "��-A5688Jk");
	currPos += itemHeadLen+pItemHead->len;
	
	unsigned long numvalue = 10;
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SHUT_DOWN_DELAY;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	memcpy(pBuf+currPos+itemHeadLen, &numvalue, pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_NET_SERVER;
	pItemHead->len = sizeof(MDVR_NETSERVER_CFG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(MDVR_NETSERVER_CFG);
	MDVR_NETSERVER_CFG *pNetServerCfg = new MDVR_NETSERVER_CFG;
	memset(pNetServerCfg, 0, sizeof(MDVR_NETSERVER_CFG));
	pNetServerCfg->serverPort = 0;		//��������
	pNetServerCfg->serverIP = 0;
	memcpy(pBuf+currPos+itemHeadLen, pNetServerCfg, pItemHead->len);
	delete pNetServerCfg;
	pNetServerCfg = NULL;
	currPos += itemHeadLen+pItemHead->len;

	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateSpeedCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_ENCODE_BASE,
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_SPEED_SOURCE,		//�ٶ���Դ��unsigned long 
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_SPEED_UNIT,			//�ٶȵ�λ, unsigned long 

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadSpeedCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num)
{
	CalculateSpeedCfgSize(num, len);
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;
	unsigned long valuetmp = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SPEED_SOURCE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 1;
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_SPEED_UNIT;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 1;
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;
	
	*ppConfig = pBuf;
	assert(len == currPos);

}

void CConfigEx::CalculateHighSpeedCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_ENCODE_BASE,
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_HIGH_SPEED_ENABLE,		//����ʹ�ܣ�unsigned long 
	num++; len += sizeof(MDVR_SPEED);		//NCFG_ITEM_HIGH_SPEED_VALUE,		//����ֵ, MDVR_SPEED
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_HIGH_SPEED_HOLD_TIME,	//����ֵ�������������ʱʱ��, unsigned long
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_HIGH_SPEED_BUZZ,		//���ٴ���������, unsigned long
	num++; len += sizeof(ULONGLONG);		//NCFG_ITEM_HIGH_SPEED_TO_ALARM_OUT,//���ٴ������������ULONGLONG
	num++; len += sizeof(ULONGLONG);		//NCFG_ITEM_HIGH_SPEED_TO_RECORD,	//���ٴ���¼��ULONGLONG 
	num++; len += sizeof(ULONGLONG);		//NCFG_ITEM_HIGH_SPEED_BIG_VIEW,	//���ٴ������棬ULONGLONG
#ifdef __DVR_BASIC__	
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ENABLE,	//�����ܹ�����EMAIL��unsigned long
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ADDR,	//���ٴ���MAIL��ַ�б�unsigned long
#endif
	num++; len += sizeof(TO_PTZ)*m_localVideoInputNum;		//NCFG_ITEM_HIGH_SPEED_TO_PTZ,		//���ٴ�����̨������TO_PTZ[VIDEO_INPUT_NUM]

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadHighSpeedCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num)
{
	CalculateHighSpeedCfgSize(num, len);
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;
	unsigned long valuetmp = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HIGH_SPEED_ENABLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 0;
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HIGH_SPEED_VALUE;
	pItemHead->len = sizeof(MDVR_SPEED);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(MDVR_SPEED);
	memset((char *)(pBuf+currPos+itemHeadLen), 0,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HIGH_SPEED_HOLD_TIME;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 10;
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HIGH_SPEED_BUZZ;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 1;
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HIGH_SPEED_TO_ALARM_OUT;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	memset((char *)(pBuf+currPos+itemHeadLen), 0, sizeof(ULONGLONG));
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HIGH_SPEED_TO_RECORD;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	valuetmp = 0xf;			//���ܲ���ȷ
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HIGH_SPEED_BIG_VIEW;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	valuetmp = 0;
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ENABLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 0;
	memcpy(pBuf+currPos+itemHeadLen, &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ADDR;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 0;
	memcpy(pBuf+currPos+itemHeadLen, &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_HIGH_SPEED_TO_PTZ;
	pItemHead->len = sizeof(TO_PTZ)*m_localVideoInputNum;
	pItemHead->num = m_localVideoInputNum;
	pItemHead->subLen = sizeof(TO_PTZ);
	TO_PTZ *psITPValue = new TO_PTZ [m_localVideoInputNum];
	memset(psITPValue, 0, sizeof(TO_PTZ)*m_localVideoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, psITPValue, pItemHead->len);
	delete [] psITPValue;
	psITPValue = NULL;
	currPos += itemHeadLen+pItemHead->len;

	*ppConfig = pBuf;
	assert(len == currPos);

}

void CConfigEx::CalculateLowSpeedCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	//	NCFG_ITEM_ENCODE_BASE,
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_LOW_SPEED_ENABLE,		//����ʹ�ܣ�unsigned long 
	num++; len += sizeof(MDVR_SPEED);		//NCFG_ITEM_LOW_SPEED_VALUE,		//����ֵ, MDVR_SPEED
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_LOW_SPEED_HOLD_TIME,	//����ֵ�������������ʱʱ��, unsigned long
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_LOW_SPEED_BUZZ,			//���ٴ���������, unsigned long
	num++; len += sizeof(ULONGLONG);		//NCFG_ITEM_LOW_SPEED_TO_ALARM_OUT, //���ٴ������������ULONGLONG
	num++; len += sizeof(ULONGLONG);		//NCFG_ITEM_LOW_SPEED_TO_RECORD,	//���ٴ���¼��ULONGLONG 
	num++; len += sizeof(ULONGLONG);		//NCFG_ITEM_LOW_SPEED_BIG_VIEW,		//���ٴ������棬ULONGLONG
#ifdef __DVR_BASIC__	
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_LOW_SPEED_TO_EMAIL_ENABLE,//�����ܹ�����EMAIL��unsigned long
	num++; len += sizeof(unsigned long);	//NCFG_ITEM_LOW_SPEED_TO_EMAIL_ADDR,  //���ٴ���MAIL��ַ�б�unsigned long
#endif
	num++; len += sizeof(TO_PTZ)*m_localVideoInputNum;					//NCFG_ITEM_HIGH_SPEED_TO_PTZ,		//���ٴ�����̨������TO_PTZ[VIDEO_INPUT_NUM]

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadLowSpeedCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num)
{
	CalculateLowSpeedCfgSize(num, len);
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0, len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;
	unsigned long valuetmp = 0;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LOW_SPEED_ENABLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 0;
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LOW_SPEED_VALUE;
	pItemHead->len = sizeof(MDVR_SPEED);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(MDVR_SPEED);
	memset((char *)(pBuf+currPos+itemHeadLen), 0,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LOW_SPEED_HOLD_TIME;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 10;
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LOW_SPEED_BUZZ;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 1;
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LOW_SPEED_TO_ALARM_OUT;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	memset((char *)(pBuf+currPos+itemHeadLen), 0, sizeof(ULONGLONG));
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LOW_SPEED_TO_RECORD;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	valuetmp = 0xf;			//���ܲ���ȷ
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LOW_SPEED_BIG_VIEW;
	pItemHead->len = sizeof(ULONGLONG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(ULONGLONG);
	valuetmp = 0;
	memcpy((char *)(pBuf+currPos+itemHeadLen), &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

#ifdef __DVR_BASIC__	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LOW_SPEED_TO_EMAIL_ENABLE;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 0;
	memcpy(pBuf+currPos+itemHeadLen, &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LOW_SPEED_TO_EMAIL_ADDR;
	pItemHead->len = sizeof(unsigned long);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(unsigned long);
	valuetmp = 0;
	memcpy(pBuf+currPos+itemHeadLen, &valuetmp,pItemHead->len);
	currPos += itemHeadLen+pItemHead->len;
#endif

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBuf + currPos);
	pItemHead->itemID = NCFG_ITEM_LOW_SPEED_TO_PTZ;
	pItemHead->len = sizeof(TO_PTZ)*m_localVideoInputNum;
	pItemHead->num = m_localVideoInputNum;
	pItemHead->subLen = sizeof(TO_PTZ);
	TO_PTZ *psITPValue = new TO_PTZ [m_localVideoInputNum];
	memset(psITPValue, 0, sizeof(TO_PTZ)*m_localVideoInputNum);
	memcpy(pBuf+currPos+itemHeadLen, psITPValue, pItemHead->len);
	delete [] psITPValue;
	psITPValue = NULL;
	currPos += itemHeadLen+pItemHead->len;

	*ppConfig = pBuf;
	assert(len == currPos);
}

void CConfigEx::CalculateWallopCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{

}

void CConfigEx::LoadWallopCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num)
{

}

void CConfigEx::CalculateWirelessCfgSize(unsigned short &itemNum, unsigned long &bufLen)
{
	int num = 0;
	int len = 0;

	num++; len += sizeof(WIFI_CONFIG)*MAX_WIFI_CFG_NUM;	//NCFG_ITEM_WIRELESS_WIFI,		//WIFI���ã�WIFI_CONFIG
	num++; len += sizeof(MDVR_WIRELESS_CFG);		//	NCFG_ITEM_WIRELESS_COMM,	//����ͨ�����ã�MDVR_WIRELESS_CFG		

	itemNum = num;
	bufLen = len + itemNum*sizeof(NCFG_ITEM_HEAD);
}

void CConfigEx::LoadWirelessCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num)
{
	CalculateWirelessCfgSize(num,len);
	unsigned char *pBuf = new unsigned char[len];
	memset(pBuf, 0 , len);
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	long currPos = 0;
	
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD*>(pBuf+currPos);
	pItemHead->itemID = NCFG_ITEM_WIRELESS_WIFI;
	pItemHead->len = sizeof(WIFI_CONFIG)*MAX_WIFI_CFG_NUM;
	pItemHead->num = MAX_WIFI_CFG_NUM;
	pItemHead->subLen = sizeof(WIFI_CONFIG);
	WIFI_CONFIG *pWifiCfg = new WIFI_CONFIG[MAX_WIFI_CFG_NUM];
	memset(pWifiCfg, 0, sizeof(WIFI_CONFIG)*MAX_WIFI_CFG_NUM);
	memcpy(pBuf+currPos+itemHeadLen, pWifiCfg, pItemHead->len);
	delete[]pWifiCfg;
	pWifiCfg = NULL;
	currPos += itemHeadLen+pItemHead->len;

	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD*>(pBuf+currPos);
	pItemHead->itemID = NCFG_ITEM_WIRELESS_COMM;
	pItemHead->len = sizeof(MDVR_WIRELESS_CFG);
	pItemHead->num = 1;
	pItemHead->subLen = sizeof(MDVR_WIRELESS_CFG);
	MDVR_WIRELESS_CFG *pWirelessCfg = new MDVR_WIRELESS_CFG;
	memset(pWirelessCfg, 0, sizeof(MDVR_WIRELESS_CFG));
	pWirelessCfg->enable = 0;
	pWirelessCfg->type = 1;
	memcpy(pBuf+currPos+itemHeadLen, pWirelessCfg, pItemHead->len);
	delete pWirelessCfg;
	pWirelessCfg = NULL;
	currPos += itemHeadLen+pItemHead->len;
	
	*ppConfig = pBuf;
	assert(len == currPos);
}
#endif
/*******************************************************************************************

������	LoadPTZPresetCfg( unsigned char **ppConfig, unsigned long &len, unsigned short &num )
���ܣ�	��ȡԤ�õ��Ĭ������
������	
		ppConfig �������ͨ��Ĭ��Ԥ�õ���Ϣ
		len		 ppConfig�����ݳ���
		num		 ����ܹ���ͨ������
����ֵ��
		ͬ����

�޸ģ�
		1��2010-02-09 ZHL 
		ppConfig����ÿ��ͨ����Ԥ�õ����ݸ�ʽΪ
				NCFG_ITEM_HEAD			ÿ��ͨ��һ��ITEM
					PTZ_PRESET
					PTZ_PRESET			ÿ��ITEM��128��Ԥ�õ�
					..........
					PTZ_PRESET
					PTZ_PRESET

				NCFG_ITEM_HEAD
					PTZ_PRESET
					PTZ_PRESET
					..........
					PTZ_PRESET
					PTZ_PRESET
				..............
					.........
*********************************************************************************************/
void CConfigEx::LoadPTZPresetCfg( unsigned char **ppConfig, unsigned long &len, unsigned short &num )
{
	num = m_videoInputNum;  //m_localVideoInputNum ��Ϊ m_videoInputNum       xian
	len = num * ( sizeof( NCFG_ITEM_HEAD ) + ONE_CHNN_PRESET_LEN );			//ÿ��ͨ��һ��ͷ��Ϣ��128��Ԥ�õ���Ϣ
	*ppConfig = new unsigned char [len];
	assert( NULL != *ppConfig );
	memset( *ppConfig, 0, len );

	for( unsigned int i=0; i<num; i++ )
	{
		NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD *>(*ppConfig + ( sizeof( NCFG_ITEM_HEAD ) + ONE_CHNN_PRESET_LEN ) * i );
		PTZ_PRESET *pPreset = reinterpret_cast<PTZ_PRESET *>( pItemHead + 1 );

		pItemHead->itemID = NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + i;
		pItemHead->len	= ONE_CHNN_PRESET_LEN;
		pItemHead->num	= MAX_PRESET_NUM;
		pItemHead->subLen = sizeof(PTZ_PRESET);
		
		//Ĭ�������ÿ��ͨ����Ԥ�õ㶼����
		for( unsigned int j=0; j<MAX_PRESET_NUM; j++, pPreset++ )
		{
#ifdef __NETSERVER_HXHT__
			pPreset->enable	= 0;						//���Ż�ͨ�汾�����Ԥ�õ��ر�
#else
			pPreset->enable	= 1;						//Ĭ�������Ԥ�õ��
#endif
			pPreset->index	= j;						//ͨ��i��Ԥ�õ����
			sprintf( pPreset->name, "%s%03d", "preset", ( j + 1 ) );
		}
	}

}

void CConfigEx::LoadPTZCruiseCfg( unsigned char **ppConfig, unsigned long &len, unsigned short &num )
{
	num = m_videoInputNum;      //m_localVideoInputNum ��Ϊ m_videoInputNum       xian
	len = num * sizeof( NCFG_ITEM_HEAD );
	*ppConfig = new unsigned char [len];

	NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD *>(*ppConfig);

	for( unsigned int i=0; i<num; i++ )
	{
		pItemHead[i].itemID = NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + i;
		pItemHead[i].len = 0;
		pItemHead[i].num = 0;
		pItemHead[i].subLen = sizeof(CRUISE_POINT_INDEX);
	}
}


bool CConfigEx::OnlyReadInfoSize(unsigned short itemID, unsigned short &len, unsigned short &subNum)
{
	unsigned char count = 0;
	switch(itemID)
	{
	//	NCFG_ITEM_ONLYREAD_BASE=0xE00,
	case NCFG_ITEM_DEVICE_NAME_MAX_LEN:      //�豸���Ƴ����޶�, unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_TIME_SERVER_MAX_LEN:      //����ʱ��ͬ����������ַ��󳤶�, unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_UPNP_FRIENDLY_NAME_MAX_LEN://UPNP�豸��ʾ��������󳤶�, unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_CAMERA_NAME_MAX_LEN:      //ͨ��������󳤶�, unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_SENSOR_IN_NAME_MAX_LEN:   //������������󳤶�,unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_ALARM_OUT_NAME_MAX_LEN:   //�̵������������󳤶�, unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_PPPOE_USER_NAME_MAX_LEN:  //PPPoE�û�����󳤶ȣ�unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_PPPOE_PASSWORD_MAX_LEN:   //PPPoE������󳤶�, unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_DDNS_USER_NAME_MAX_LEN:   //DDNS�û�����󳤶�, unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_DDNS_PASSWORD_MAX_LEN:    //DDNS������󳤶�, unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_DDNS_DOMAIN_MAX_LEN:		//DDNS����������󳤶�, unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_WIRELESS_SSID_MAX_LEN:   //����SSID��󳤶�, unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_WIRELESS_KEY_MAX_LEN:    //��Կ��󳤶�, unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_TIME_STAMP_SUPPORT_POS:	//ʱ���֧�ֵ�λ�÷�ʽ��unsigned long ����ʱ�����ʹ�ô�ID(0x01 << pos��MASK),[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_SUPPORT_VIDEOFORMAT:		//֧�ֵ���Ƶ��ʽ,unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_SUPPORT_QUADMODE:		//֧�ֵķָ�ģʽ[ֻ��],�ڼ�λΪ1�ʹ���֧����һ���ָ�ģʽ��unsigned long,[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;		
	case NCFG_ITEM_USER_MAXNUM:				//֧���û����������unsigned long[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_SUPPORT_LANGUAGE:		
		count = CLanguagePack::Instance()->GetCount();
		len = sizeof(NCFG_INFO_LANGUAGE) * count;
		subNum = 1;
		break;
	case NCFG_ITEM_SUPPORT_MAXCLIENTNUM:
		len = sizeof(unsigned long);
		subNum = 1;
	case NCFG_ITEM_HOLIDAY_MAX_NUM:			//����֧�ֵļ����ų����ڵ�����������ر��Ƿ�ѭ����ʽʱ���������ӣ�unsigned long
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_DEVUSER_NAME_MAX_LEN:	//�豸�û�����󳤶ȣ�unsigned long[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_DEVUSER_PWD_MAX_LEN:		//�豸�û�������󳤶ȣ�unsigned long[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_VGA_SUPPORT_RESOLUTION:	//VGAоƬ֧�ֵķֱ���MASK��unsigned long���ο�dvrdvsdef.h�е�VGA_RESOLUTIONö��[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	//case NCFG_ITEM_VGA_SUPPORT_REFRESH:		//VGA֧�ֵ���Ļˢ���ʣ�ÿ�ֱַ����¶�������ͬ[ֻ��]
	//	len = sizeof(unsigned long);
	//	subNum = 1;
	//	break;
	case NCFG_ITEM_DDNS_SERVER_INFO:			//DDNS�����б�֧�ֵ�DDNS����[ֻ��]
		count = CReadDdns::Instance()->GetDddnsCount();
		len = sizeof(DDNS_SERVER_INFO)*count;
		subNum = 1;
		break;
	case NCFG_ITEM_MAX_PRE_ALARM_TIME:			//Ԥ¼������ʱ�䣨�룩[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY://��ʾ֧�ֱַ��ʵĸ������ٸ����ɸ�NCFG_INFO_SUPPORT_FRAME_RATE[ֻ��]
		{
			count = 1;
			len = sizeof(NCFG_INFO_SUPPORT_FRAME_RATE)*count;
			subNum = 1;
		}
		break;
	case NCFG_ITEM_MAJOR_SUPPORT_QULITY_LEVELNUM:	//VBR֧�ּ������εĻ��ʣ��ͻ���֧��5�ֵ��Σ�����������1-n������ȡ������,unsigned long,[ֻ��]
		count = CProduct::Instance()->LocalQuiltyLevelNum() + 1;
		len = sizeof(unsigned long)*count;
		subNum = 1;
		break;
	case NCFG_ITEM_MAJOR_SUPPORT_ENCODEMODE:		//֧�ֵı���ģʽ���Ǹ�VIDEO_ENCODE_MODE��MASK��unsigned long,[ֻ��]
		count = 2;
		len = sizeof(unsigned long)*count;
		subNum = 1;
		break;
	case NCFG_ITEM_MAJOR_SUPPORT_BITRATE_RANGE:	//CBRģʽ��֧�ֵ�������Χ��NCFG_INFO_BITRATE_RANG,[ֻ��]
		count = CProduct::Instance()->LocalBitRateCount() + 1;
		len = sizeof(unsigned long )*count;
		subNum = 1;
		break;
	case NCFG_ITEM_ENCODE_MINOR_SUPPORT_PROPERTY://��ʾ֧�ֱַ��ʵĸ������ٸ����ɸ�NCFG_INFO_SUPPORT_FRAME_RATE[ֻ��]
		count = 1;
		len = sizeof(NCFG_INFO_SUPPORT_FRAME_RATE)*count;
		subNum = 1;		
		break;
	case NCFG_ITEM_MINOR_SUPPORT_QULITY_LEVELNUM:	//VBR֧�ּ������εĻ��ʣ��ͻ���֧��5�ֵ��Σ�����������1-n������ȡ������,unsigned long,[ֻ��]
		count = CProduct::Instance()->NetQuiltyLevelNum() + 1;
		len = sizeof(unsigned long)*count;
		subNum = 1;
		break;
	case NCFG_ITEM_MINOR_SUPPORT_ENCODEMODE:		//֧�ֵı���ģʽ���Ǹ�VIDEO_ENCODE_MODE��MASK��unsigned long,[ֻ��]
		count = 2;
		len = sizeof(unsigned long)*count;
		subNum = 1;
		break;
	case NCFG_ITEM_MINOR_SUPPORT_BITRATE_RANGE:		//CBRģʽ��֧�ֵ�������Χ��NCFG_INFO_BITRATE_RANG,[ֻ��]
		count = CProduct::Instance()->NetBitRateCount() + 1;
		len = sizeof(unsigned long )*count;
		subNum = 1;
		break;
	case NCFG_ITEM_ENCODE_SIZE_MAJOR_ALL_CH:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_ENCODE_SIZE_MINOR_ALL_CH:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_ENCODE_SIZE_PHONE_ALL_CH:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_DEVICE_SUPPORT_REDUNDANCY:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_CRUISE_PRESET_MAXNUM:		//ÿ��Ѳ���ߵ����Ԥ�õ�����[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_CHNN_PRESET_MAX_NUM:			//ÿ��ͨ�������Ԥ�õ�����[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_CHNN_CRUISE_MAX_NUM:			//ÿ��ͨ�������Ѳ��������[ֻ��]
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_PTZ_SUPPORT_INFO:
		{
			len = sizeof(PTZ_PROTOCOL_INFO)*(PROTOCOL_END - 1);
			subNum = (PROTOCOL_END - 1);
		}
		break;
	case NCFG_ITEM_DEFAULT_COLOR:
		{
			len = sizeof(unsigned long);
			subNum = 1;
		}
		break;
	case NCFG_ITEM_EMAIL_IMG_NUM_RANGE:
		{
			count = ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT + 1;
			len = sizeof(unsigned long)*count;
			subNum = 1;
		}
		break;
	case NCFG_ITEM_EMAIL_SNAP_TIME_RANGE:
		{
			count =  CProduct::Instance()->SnapTimeIntervalCount();
			len = sizeof(unsigned long) * count;
			subNum = 1;
		}
		break;
#ifdef __MOBILE_DVR__
	case NCFG_ITEM_SPEED_VALUE_MUX_LEN:
		{
			len = sizeof(unsigned long);
			subNum = 1;
		}
		break;
#endif
	case NCFG_ITEM_SUPPORT_SYNC_TIME:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_SUPPORT_DST:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_SUPPORT_MASK_AREA:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_SUPPORT_MAIN_MONITOR_SPORT:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_MAJOR_HAVE_ENCODE_AND_MAXBIT:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_MINOR_HAVE_ENCODE_AND_MAXBI:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_SPPORT_SIMPLE_USER_MANAGE:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_BAN_ALLOW_NUM:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_SNAP_SIZE_RANGE:
		len = sizeof(unsigned long);
		subNum = 1;
		break;
	case NCFG_ITEM_SNAP_QULITY_LEVELNUM:	//
		count = CProduct::Instance()->SnapQuiltyLevelNum();
		len = sizeof(unsigned long)*count;
		subNum = 1;
		break;
	case NCFG_ITEM_SNAP_IMG_NUM_RANGE:
		count = SNAP_PIC_MAX_NUM+1;
		len = sizeof(unsigned long)*count;
		subNum = 1;
		break;
	case NCFG_ITEM_SNAP_TIME_RANGE:
		count =  CProduct::Instance()->SnapTimeIntervalCount();
		len = sizeof(unsigned long) * count;
		subNum = 1;
		break;
	default:
		{
		   return false;
			break;
		}
	}
	return true;
}

bool CConfigEx::OnlyReadInfo(unsigned short itemID, unsigned char *pData, unsigned short len)
{
	assert(pData != NULL);
	unsigned long ucValue = 0;
	unsigned long ulValue = 0;
	unsigned long	count = 0;

	switch(itemID)
	{
		//	NCFG_ITEM_ONLYREAD_BASE=0xE00,
	case NCFG_ITEM_DEVICE_NAME_MAX_LEN:      //�豸���Ƴ����޶�, unsigned long,[ֻ��]
		ucValue = MAX_DEVICE_NAME_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_TIME_SERVER_MAX_LEN:      //����ʱ��ͬ����������ַ��󳤶�, unsigned long,[ֻ��]
		ucValue = MAX_TIME_SERVER_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_UPNP_FRIENDLY_NAME_MAX_LEN://UPNP�豸��ʾ��������󳤶�, unsigned long,[ֻ��]
		ucValue = MAX_UPNP_FRIENDLY_NAME_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_CAMERA_NAME_MAX_LEN:      //ͨ��������󳤶�, unsigned long,[ֻ��]
		ucValue = MAX_CAMERA_NAME_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_SENSOR_IN_NAME_MAX_LEN:   //������������󳤶�,unsigned long,[ֻ��]
		ucValue = MAX_SENSOR_IN_NAME_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_ALARM_OUT_NAME_MAX_LEN:   //�̵������������󳤶�, unsigned long,[ֻ��]
		ucValue = MAX_ALARM_OUT_NAME_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_PPPOE_USER_NAME_MAX_LEN:  //PPPoE�û�����󳤶ȣ�unsigned long,[ֻ��]
		ucValue = MAX_PPPOE_USER_NAME_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_PPPOE_PASSWORD_MAX_LEN:   //PPPoE������󳤶�, unsigned long,[ֻ��]
		ucValue = MAX_PPPOE_PASSWORD_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_DDNS_USER_NAME_MAX_LEN:   //DDNS�û�����󳤶�, unsigned long,[ֻ��]
		ucValue = MAX_DDNS_USER_NAME_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_DDNS_PASSWORD_MAX_LEN:    //DDNS������󳤶�, unsigned long,[ֻ��
		ucValue = MAX_DDNS_PASSWORD_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_DDNS_DOMAIN_MAX_LEN:		//DDNS����������󳤶�, unsigned long,[ֻ��]
		ucValue = MAX_DDNS_DOMAIN_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_WIRELESS_SSID_MAX_LEN:   //����SSID��󳤶�, unsigned long,[ֻ��]
		ucValue = MAX_WIRELESS_SSID_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_WIRELESS_KEY_MAX_LEN:    //��Կ��󳤶�, unsigned long,[ֻ��]
		ucValue = MAX_WIRELESS_KEY_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_TIME_STAMP_SUPPORT_POS:	//ʱ���֧�ֵ�λ�÷�ʽ��unsigned long ����ʱ�����ʹ�ô�ID(0x01 << pos��MASK),[ֻ��]
		ulValue = 0x1f;
		memcpy(pData, &ulValue, len);
		break;
	case NCFG_ITEM_SUPPORT_VIDEOFORMAT:		//֧�ֵ���Ƶ��ʽ,unsigned long,[ֻ��]
		ucValue = NCFG_ENUM_SUPPORT_VIDEOFORMAT_PAL | NCFG_ENUM_SUPPORT_VIDEOFORMAT_NTSC;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_SUPPORT_QUADMODE:		//֧�ֵķָ�ģʽ[ֻ��],�ڼ�λΪ1�ʹ���֧����һ���ָ�ģʽ��unsigned long,[ֻ��]
		ulValue = 0;
#if (1 <= m_videoInputNum)
		ulValue |= 0x01;
#if (4 <= m_videoInputNum)
		ulValue |= (0x01 << 4);
#if (8 <= m_videoInputNum)
		ulValue |=  (0x01 << 8);
#if (16 <= m_videoInputNum)
		ulValue |=  (0x01 << 16);
#endif
#endif
#endif
#endif
		memcpy(pData, &ulValue, len);
		break;
	case NCFG_ITEM_USER_MAXNUM:				//֧���û����������unsigned long[ֻ��]
		ucValue = MAX_USER_NUM;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_SUPPORT_LANGUAGE:
		count = CLanguagePack::Instance()->GetCount();
		if (count > 0)
		{
			unsigned long temp = 0;
			NCFG_INFO_LANGUAGE *pLanguage = new NCFG_INFO_LANGUAGE[count];
			memset(pLanguage, 0, sizeof(NCFG_INFO_LANGUAGE)*count);
			for (int i = 0; i < count; i++)
			{
				pLanguage[i].languageId = CLanguagePack::Instance()->GetLanguageID(i);
				CLanguagePack::Instance()->GetLanguageName(pLanguage[i].languageId, pLanguage[i].name);				
			}
			memcpy(pData, pLanguage, len);
			delete [] pLanguage;
			pLanguage = NULL;
		}
		break;
	case NCFG_ITEM_SUPPORT_MAXCLIENTNUM:    //֧�ֵ�ͬʱ���ߵ�����ͻ��˸���,����ȡֵ�ķ�Χ
		ucValue = CProduct::Instance()->MaxNetClientNum(); 
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_HOLIDAY_MAX_NUM:			//����֧�ֵļ����ų����ڵ�����������ر��Ƿ�ѭ����ʽʱ���������ӣ�unsigned long
		ucValue = MAX_HOLIDAY_NUM;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_DEVUSER_NAME_MAX_LEN:	//�豸�û�����󳤶ȣ�unsigned long[ֻ��]
		ucValue = USER_NAME_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_DEVUSER_PWD_MAX_LEN:		//�豸�û�������󳤶ȣ�unsigned long[ֻ��]
		ucValue = PASSWORD_LEN;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_VGA_SUPPORT_RESOLUTION:	//VGAоƬ֧�ֵķֱ���MASK��unsigned long���ο�dvrdvsdef.h�е�VGA_RESOLUTIONö��[ֻ��]
		ulValue = CProduct::Instance()->DisplayResolutionMask();
		memcpy(pData, &ulValue, len);
		break;
		//case NCFG_ITEM_VGA_SUPPORT_REFRESH:		//VGA֧�ֵ���Ļˢ���ʣ�ÿ�ֱַ����¶�������ͬ[ֻ��]
		//	len = sizeof(unsigned long);
		//	subNum = 1;
		//	break;
	case NCFG_ITEM_DDNS_SERVER_INFO:			//DDNS�����б�֧�ֵ�DDNS����[ֻ��]
		{
			count = CReadDdns::Instance()->GetDddnsCount();
			DDNS_SERVER_INFO *pInfo = new DDNS_SERVER_INFO [count];
			for (int i = 0; i < count; ++i)
			{
				CReadDdns::Instance()->GetDddnsServerInfo(i, &(pInfo[i]));				
			}
			memcpy(pData, pInfo, len);
			delete [] pInfo;
		}
		break;
	case NCFG_ITEM_MAX_PRE_ALARM_TIME:			//Ԥ¼������ʱ�䣨�룩[ֻ��]
		{
			count = 1;
			unsigned long maxPreAlarmTime = CProduct::Instance()->MaxPreAlarmTime();
			memcpy(pData, &maxPreAlarmTime, len);
		}
		break;
	case NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY://��ʾ֧�ֱַ��ʵĸ������ٸ����ɸ�NCFG_INFO_SUPPORT_FRAME_RATE[ֻ��]
		{
			{
						count = 1;
						NCFG_INFO_SUPPORT_FRAME_RATE supportFrameRate;
						supportFrameRate.dwResolution = CProduct::Instance()->LocalVideoSizeMask();
						supportFrameRate.totalRatePalOnCif = CProduct::Instance()->LocalTotalRatePal();
						supportFrameRate.chnnRatePalOnCif = CProduct::Instance()->LocalMaxChannelRatePal();
						supportFrameRate.totalRateNtscOnCif = CProduct::Instance()->LocalTotalRateNtsc();
						supportFrameRate.chnnRateNtscOnCif = CProduct::Instance()->LocalMaxChannelRateNtsc();
						supportFrameRate.defaResolution = CProduct::Instance()->LocalDefaVideoSize();
						supportFrameRate.defauChnnRateNumPal = CProduct::Instance()->LocalDefaChnnRateNumPal();
						supportFrameRate.defauChnnRateNumNtsc = CProduct::Instance()->LocalDefaChnnRateNumNtsc();
						memcpy(pData, &supportFrameRate, len);
			}
			
		}
		break;
	case NCFG_ITEM_MAJOR_SUPPORT_QULITY_LEVELNUM://VBR֧�ּ������εĻ��ʣ��ͻ���֧��5�ֵ��Σ�����������1-n������ȡ������,unsigned long,[ֻ��]	
		{
			count = CProduct::Instance()->LocalQuiltyLevelNum() + 1;
			unsigned long *pQulity = new unsigned long [count];
			for (int i = 0; i < count - 1; ++i)
			{
				pQulity[i] = CProduct::Instance()->LocalQuiltyLevelValue(i);
			}
			pQulity[count - 1] = CProduct::Instance()->LocalDefQuiltyValue();			
			memcpy(pData, pQulity, len);
			delete [] pQulity;
		}
		break;
	case NCFG_ITEM_MAJOR_SUPPORT_ENCODEMODE:		//֧�ֵı���ģʽ���Ǹ�VIDEO_ENCODE_MODE��MASK��unsigned long,[ֻ��]
		{
			count = 2;
			unsigned long *pEncodeMode = new unsigned long [count];
			pEncodeMode[0] = CProduct::Instance()->LocalVideoEncodeModeMask();
			pEncodeMode[1] = CProduct::Instance()->LocalDefauVideoEncodeMode();
			memcpy(pData, pEncodeMode, len);
			delete [] pEncodeMode;
		}
		break;
	case NCFG_ITEM_MAJOR_SUPPORT_BITRATE_RANGE:	//CBRģʽ��֧�ֵ�������Χ��NCFG_INFO_BITRATE_RANG,[ֻ��]
		{
			count = CProduct::Instance()->LocalBitRateCount() + 1;
			unsigned long *pBitRate = new unsigned long [count];
			for (int i = 0; i < count - 1; ++i)
			{
				pBitRate[i] = CProduct::Instance()->LocalBitRateValue(i);
			}
			pBitRate[count - 1] = CProduct::Instance()->LocalDefaBitRateValue();
			memcpy(pData, pBitRate, len);		
			delete [] pBitRate;
		}
		break;	
	case NCFG_ITEM_ENCODE_MINOR_SUPPORT_PROPERTY://��ʾ֧�ֱַ��ʵĸ������ٸ����ɸ�NCFG_INFO_SUPPORT_FRAME_RATE[ֻ��]
		{
			count = 1;
			NCFG_INFO_SUPPORT_FRAME_RATE supportFrameRate;
			supportFrameRate.dwResolution = CProduct::Instance()->NetVideoSizeMask();
			supportFrameRate.totalRatePalOnCif = CProduct::Instance()->NetTotalRatePal();
			supportFrameRate.chnnRatePalOnCif = CProduct::Instance()->NetMaxChannelRatePal();
			supportFrameRate.totalRateNtscOnCif = CProduct::Instance()->NetTotalRateNtsc();
			supportFrameRate.chnnRateNtscOnCif = CProduct::Instance()->NetMaxChannelRateNtsc();
			supportFrameRate.defaResolution = CProduct::Instance()->NetDefaVideoSize();
			supportFrameRate.defauChnnRateNumPal = CProduct::Instance()->NetDefaChnnRateNumPal();
			supportFrameRate.defauChnnRateNumNtsc = CProduct::Instance()->NetDefaChnnRateNumNtsc();
			memcpy(pData, &supportFrameRate, len);
		}
		break;		
	case NCFG_ITEM_MINOR_SUPPORT_QULITY_LEVELNUM://VBR֧�ּ������εĻ��ʣ��ͻ���֧��5�ֵ��Σ�����������1-n������ȡ������,unsigned long,[ֻ��]	
		{
			count = CProduct::Instance()->NetQuiltyLevelNum() + 1;
			unsigned long *pQulity = new unsigned long [count];
			for (int i = 0; i < count - 1; ++i)
			{
				pQulity[i] = CProduct::Instance()->NetQuiltyLevelValue(i);
			}
			pQulity[count - 1] = CProduct::Instance()->NetDefQuiltyValue();			
			memcpy(pData, pQulity, len);
			delete [] pQulity;
		}
		break;
	case NCFG_ITEM_MINOR_SUPPORT_ENCODEMODE:		//֧�ֵı���ģʽ���Ǹ�VIDEO_ENCODE_MODE��MASK��unsigned long,[ֻ��]
		{
			count = 2;
			unsigned long *pEncodeMode = new unsigned long [count];
			pEncodeMode[0] = CProduct::Instance()->NetVideoEncodeModeMask();
			pEncodeMode[1] = CProduct::Instance()->NetDefauVideoEncodeMode();
			memcpy(pData, pEncodeMode, len);
			delete [] pEncodeMode;
		}
		break;
	case NCFG_ITEM_MINOR_SUPPORT_BITRATE_RANGE:	//CBRģʽ��֧�ֵ�������Χ��NCFG_INFO_BITRATE_RANG,[ֻ��]
		{
			count = CProduct::Instance()->NetBitRateCount() + 1;
			unsigned long *pBitRate = new unsigned long [count];
			for (int i = 0; i < count - 1; ++i)
			{
				pBitRate[i] = CProduct::Instance()->NetBitRateValue(i);
			}
			pBitRate[count - 1] = CProduct::Instance()->NetDefaBitRateValue();
			memcpy(pData, pBitRate, len);		
			delete [] pBitRate;
		}
		break;
	case NCFG_ITEM_ENCODE_SIZE_MAJOR_ALL_CH:		//�������ֱ����Ƿ�֧������ͨ��һ������ unsigned long
		*reinterpret_cast<unsigned long *>(pData) = CProduct::Instance()->LocalRateSizeAllCH();						
		break;
	case NCFG_ITEM_ENCODE_SIZE_MINOR_ALL_CH:		//�������ֱ����Ƿ�֧������ͨ��һ������ unsigned long
		*reinterpret_cast<unsigned long *>(pData) = CProduct::Instance()->NetRateSizeAllCH();
		break;
	case NCFG_ITEM_ENCODE_SIZE_PHONE_ALL_CH:		//�ֻ������ֱ����Ƿ�֧������ͨ��һ������ unsigned long
		*reinterpret_cast<unsigned long *>(pData) = 1;
		break;
	case NCFG_ITEM_DEVICE_SUPPORT_REDUNDANCY:
		*reinterpret_cast<unsigned long *>(pData) = CProduct::Instance()->SupportRedunRec();
		break;
	case NCFG_ITEM_CRUISE_PRESET_MAXNUM:		//ÿ��Ѳ���ߵ����Ԥ�õ�����[ֻ��]
		ucValue = MAX_CRUISE_PRESET_NUM;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_CHNN_PRESET_MAX_NUM:			//ÿ��ͨ�������Ԥ�õ�����[ֻ��]
		ucValue = MAX_PRESET_NUM;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_CHNN_CRUISE_MAX_NUM:			//ÿ��ͨ�������Ѳ��������[ֻ��]
		ucValue = MAX_CRUISE_NUM;
		memcpy(pData, &ucValue, len);
		break;
	case NCFG_ITEM_PTZ_SUPPORT_INFO:
		{
			unsigned char num = PROTOCOL_END - 1;
			PTZ_PROTOCOL_INFO info [PROTOCOL_END - 1];
			for (int i=0; i<num; ++i)
			{
				info[i].ucprotocolID	= PROTOCOL_PELCOP + i;
				info[i].supportproperty = PTZSupportMask(info[i].ucprotocolID);
				strcpy(info[i].szProtocolName, &PTZ_PROTOCL_NAME[info[i].ucprotocolID][0]);
			}
			memcpy(pData, info, sizeof(PTZ_PROTOCOL_INFO)*num);
		}
		break;
	case NCFG_ITEM_DEFAULT_COLOR:
		{
#ifdef __ENVIRONMENT_LINUX__
#if defined(__SPECIAL_COLOR)
			if (CLocalDevice::Is2316SSNewPCB())
			{
				ucValue = (VIDEO_FORMAT_NTSC == g_video_format) ? DEFAULT_COLOR_NTSC_1108 : DEFAULT_COLOR_PAL_1108;
			}
			else
			{
				ucValue = (VIDEO_FORMAT_NTSC == g_video_format) ? DEFAULT_COLOR_NTSC_25838 : DEFAULT_COLOR_PAL_25838;
			}
#else
			if (g_bUse1104A1108ChipColor)
			{
				ucValue = (VIDEO_FORMAT_NTSC == g_video_format) ? DEFAULT_COLOR_NTSC_1104 : DEFAULT_COLOR_PAL_1104;
			}
			else
			{
				ucValue = (VIDEO_FORMAT_NTSC == g_video_format) ? DEFAULT_COLOR_NTSC_2864 : DEFAULT_COLOR_PAL_2864;
			}
#endif

#if defined(__CHIP3520A__) || defined(__CHIP3521__) || defined(__HISI_SDK_0BX__) || defined(__CHIPGM__) || defined(__CHIP3520D__)
			char bright = 0;
			char hue = 0;
			char sat =0;
			char contrast = 0;


			bright = 128;
			contrast = 128;
			sat = 128;
			hue =128;
			ucValue = (bright << 24) + (contrast << 16) + (sat << 8) + hue;
#endif

#else
			ucValue = (VIDEO_FORMAT_NTSC == g_video_format) ? DEFAULT_COLOR_NTSC_2864 : DEFAULT_COLOR_PAL_2864;
#endif
			memcpy(pData, &ucValue, len);
		}
		break;
	case NCFG_ITEM_EMAIL_IMG_NUM_RANGE:
		{
			unsigned long *pTemp = new unsigned long [ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT + 1];
			for (int i = 0; i < ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT + 1; ++i)
			{
				pTemp[i] = i;
			}
			memcpy(pData, pTemp, len);
			delete [] pTemp;
		}
		break;
	case NCFG_ITEM_EMAIL_SNAP_TIME_RANGE:
		{
			int count =  CProduct::Instance()->SnapTimeIntervalCount();
			unsigned long *pTemp = new unsigned long [count];
			for (int i = 0; i < count; ++i)
			{
				pTemp[i] =  CProduct::Instance()->SnapTimeIntervalValue(i);
			}
			memcpy(pData, pTemp, len);
			delete [] pTemp;
		}
		break;
#ifdef __MOBILE_DVR__
	case NCFG_ITEM_SPEED_VALUE_MUX_LEN:
		{
			ucValue = MAX_SPEED_VALUE_LEN;
			memcpy(pData, &ucValue, len);			
		}
		break;
#endif
	case NCFG_ITEM_SUPPORT_SYNC_TIME:
		{
#ifdef __DVR_BASIC__
			ucValue = 1;
#else
			ucValue = 0;
#endif
			memcpy(pData, &ucValue, len);
		}		
		break;
	case NCFG_ITEM_SUPPORT_DST:
		{
			ucValue = 1;
			memcpy(pData, &ucValue, len);
		}	
		break;
	case NCFG_ITEM_SUPPORT_MASK_AREA:
		{
			ucValue = 1;
			memcpy(pData, &ucValue, len);
		}	
		break;
	case NCFG_ITEM_SUPPORT_MAIN_MONITOR_SPORT:
		{
#ifdef __DVR_BASIC__
			ucValue = 1;
#else
			ucValue = 0;
#endif
			memcpy(pData, &ucValue, len);
		}	
		break;
	case NCFG_ITEM_MAJOR_HAVE_ENCODE_AND_MAXBIT:
		{
#if defined (__DVR_PROFESSIONAL__) || defined (__DVR_ULTIMATE__) 
			ucValue = 1;
#else
			ucValue = 0;
#endif
			memcpy(pData, &ucValue, len);
		}	
		break;
	case NCFG_ITEM_MINOR_HAVE_ENCODE_AND_MAXBI:
		{
#if defined (__DVR_PROFESSIONAL__) || defined (__DVR_ULTIMATE__) 
			ucValue = 1;
#else
			ucValue = 0;
#endif
			memcpy(pData, &ucValue, len);
		}	
		break;
	case NCFG_ITEM_SPPORT_SIMPLE_USER_MANAGE:
		{
#ifdef __DVR_BASIC__
			ucValue = 0;
#else
			ucValue = 1;
#endif
			memcpy(pData, &ucValue, len);
		}	
		break;
	case NCFG_ITEM_BAN_ALLOW_NUM:
		{
			ucValue = MAX_BAN_ALLOW_LIST_NUM;
			memcpy(pData, &ucValue, len);
		}
		break;
	case NCFG_ITEM_SNAP_SIZE_RANGE:
		{
			ucValue = CProduct::Instance()->SnapPictureSizeMask();
			memcpy(pData, &ucValue, len);
		}
		break;
	case NCFG_ITEM_SNAP_QULITY_LEVELNUM:
		{
			count = CProduct::Instance()->SnapQuiltyLevelNum();
			unsigned long *pQulity = new unsigned long [count];
			for (int i = 0; i < count; ++i)
			{
				pQulity[i] = CProduct::Instance()->SnapQuiltyLevelValue(i);
			}						
			memcpy(pData, pQulity, len);
			delete [] pQulity;
		}
		break;
	case NCFG_ITEM_SNAP_IMG_NUM_RANGE:
		{
			unsigned long *pTemp = new unsigned long [SNAP_PIC_MAX_NUM + 1];
			for (int i = 0; i < SNAP_PIC_MAX_NUM + 1; ++i)
			{
				pTemp[i] = i;
			}
			memcpy(pData, pTemp, len);
			delete [] pTemp;
		}
		break;
	case NCFG_ITEM_SNAP_TIME_RANGE:
		{
			int count =  CProduct::Instance()->SnapTimeIntervalCount();
			unsigned long *pTemp = new unsigned long [count];
			for (int i = 0; i < count; ++i)
			{
				pTemp[i] =  CProduct::Instance()->SnapTimeIntervalValue(i);
			}
			memcpy(pData, pTemp, len);
			delete [] pTemp;
		}
		break;
	default:
		return false;
	}
	return true;
}

bool CConfigEx::GetPresetConfigInfo(unsigned itemID, unsigned char *PData, unsigned short &dataLen, unsigned short &subNum)
{
	assert(itemID >= NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE && itemID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND);//��Χ�޶���

	unsigned short len = 0;
	CMyList <PTZ_PRESET> list;
	unsigned char chnn = itemID - NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE;
	bool bRet = m_presetConfig.GetPresetList (list, chnn);
	if (!bRet)
	{
		list.RemoveAll();
		return false;
	}
	subNum = list.GetCount();

	POS pos = list.GetHeadPosition();
	while(NULL != pos)
	{
		PTZ_PRESET & preset = list.GetNext(pos);
		if (PData != NULL)
		{
			memcpy(PData + len, &preset,sizeof(PTZ_PRESET));
		}
		len += sizeof(PTZ_PRESET);
	}

	assert(len == subNum*sizeof(PTZ_PRESET));
	dataLen = len;
	list.RemoveAll();
	return true;
}

bool CConfigEx::SetPresetConfigInfo(unsigned itemID, const unsigned char *PData,unsigned short subNum)
{
	bool bRet = false;
	CMyList <PTZ_PRESET> list;
	PTZ_PRESET preset;
	for (int i=0; i<subNum; i++)
	{
		memcpy(&preset, (PData+i*sizeof(PTZ_PRESET)), sizeof(PTZ_PRESET));
		list.AddTail(preset);
	}

	bRet = m_presetConfig.ModifyPreset(list, (itemID-NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE));
	list.RemoveAll();
	return bRet;
}

bool CConfigEx::GetCruiseConfigInfo(unsigned itemID, unsigned char *PData, unsigned short &dataLen, unsigned short &subNum)
{
	assert(itemID >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE && itemID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND);//��Χ�޶���

	ULONG offset = 0;
	CMyList <CRUISE> list;
	NCFG_INFO_CRUISE_HEAD cruiseHead;
	unsigned char chnn = itemID - NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE;
	bool bRet = m_cruiseConfig.GetCruiseList(list, chnn);
	if (!bRet)
	{
		return false;
	}
	subNum = list.GetCount();
	offset = subNum*sizeof(NCFG_INFO_CRUISE_HEAD);

	int cruiseNo = 0;
	POS cruisePos = list.GetHeadPosition();
	while(NULL != cruisePos)
	{
		CRUISE & cruise= list.GetNext(cruisePos);
		cruiseHead.index = cruise.index;
		cruiseHead.cruisepointnum = (NULL != cruise.pointList) ? cruise.pointList->GetCount() : 0;
		memcpy(cruiseHead.name, cruise.name, sizeof(cruiseHead.name));
		cruiseHead.offset = offset;
		if (PData != NULL)
		{
			memcpy((PData+cruiseNo*sizeof(NCFG_INFO_CRUISE_HEAD)), &cruiseHead, sizeof(NCFG_INFO_CRUISE_HEAD));
		}
		POS pointPos = (NULL != cruise.pointList) ? cruise.pointList->GetHeadPosition() : NULL;
		unsigned short len = 0;
		while (pointPos != NULL)
		{
			PTZ_CURISE_POINT &tmpCruise = cruise.pointList->GetAt(pointPos);
			if (PData != NULL)
			{
				memcpy((PData+cruiseHead.offset+len), &tmpCruise, sizeof(PTZ_CURISE_POINT));
			}
			len += sizeof(PTZ_CURISE_POINT);
			cruise.pointList->GetNext(pointPos);
		}
		if (NULL != cruise.pointList)
		{
			cruise.pointList->RemoveAll();
			delete cruise.pointList;
			cruise.pointList = NULL;
		}
		
		offset += cruiseHead.cruisepointnum*sizeof(PTZ_CURISE_POINT);
		cruiseNo++;
	}

	dataLen = (unsigned short)offset;
	list.RemoveAll();
	return true;
}

bool CConfigEx::SetCruiseConfigInfo(unsigned itemID, const unsigned char *PData,unsigned short subNum)
{
	bool bRet = false;
	CMyList <CRUISE> list;	 
	CRUISE cruise;
	PTZ_CURISE_POINT curisePoint;
	NCFG_INFO_CRUISE_HEAD cruiseHead;
	for (int i=0; i<subNum; i++)
	{
		memcpy(&cruiseHead, (PData+i*sizeof(NCFG_INFO_CRUISE_HEAD)), sizeof(NCFG_INFO_CRUISE_HEAD));
		cruise.index = cruiseHead.index;
		memcpy(cruise.name, cruiseHead.name, sizeof(cruise.name));
		cruise.pointList = new CMyList<PTZ_CURISE_POINT>;
		for (ULONG j=0; j<cruiseHead.cruisepointnum; j++)
		{
			memcpy(&curisePoint, PData+(cruiseHead.offset+j*sizeof(PTZ_CURISE_POINT)), sizeof(PTZ_CURISE_POINT));
			cruise.pointList->AddTail(curisePoint);			
		}
		list.AddTail(cruise);
	}

	bRet = m_cruiseConfig.ModifyCruiseList(list, (itemID - NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE));

	//��������
	POS pos = list.GetHeadPosition();
	while (pos != NULL)
	{
		cruise = list.GetAt(pos);
		cruise.pointList->RemoveAll();
		delete cruise.pointList;
		cruise.pointList = NULL;
		list.GetNext(pos);
	}
	list.RemoveAll();

	return bRet;
}

bool CConfigEx::GetHolidayScheCfgInfo(unsigned itemID, unsigned char *PData, unsigned short &dataLen, unsigned short &subNum)
{
	assert((itemID > NCFG_ITEM_SCH_HOLIDAY_BASE) && \
		(itemID < NCFG_ITEM_SCH_HOLIDAY_END));//��Χ�޶���

	ULONG offset = 0;
	CMyList <SCHEDULE_NODE> list;
	bool bRet = m_scheduleHolidayCfg.GetHolidayScheduleList(list, itemID);
	if (!bRet)
	{
		return false;
	}
	int nodeNum = list.GetCount();
	assert(nodeNum == getHolidayGroupNum(itemID));
	offset = nodeNum*sizeof(NCFG_INFO_HOLIDAY_SCHE_HEAD);

	int count = 0;
	NCFG_INFO_HOLIDAY_SCHE_HEAD holidayScheduleHead;
	POS pos = list.GetHeadPosition();
	while (pos != NULL)
	{
		SCHEDULE_NODE &scheduleNode = list.GetNext(pos);
		holidayScheduleHead.holidaynum = scheduleNode.pHolidayList->GetCount();
		holidayScheduleHead.offset = offset;
		if (PData != NULL)
		{
			memcpy((PData+count*sizeof(NCFG_INFO_HOLIDAY_SCHE_HEAD)), &holidayScheduleHead,\
				sizeof(NCFG_INFO_HOLIDAY_SCHE_HEAD));
		}

		POS holidayPos = scheduleNode.pHolidayList->GetHeadPosition();
		unsigned short len = 0;
		while (holidayPos != NULL)
		{
			HOLIDAY_SCHEDULE &holiday = scheduleNode.pHolidayList->GetNext(holidayPos);
			if (PData != NULL)
			{
				memcpy((PData+holidayScheduleHead.offset+len), &holiday, sizeof(HOLIDAY_SCHEDULE));
			}
			len += sizeof(HOLIDAY_SCHEDULE);
		}
		scheduleNode.pHolidayList->RemoveAll();
		delete scheduleNode.pHolidayList;
		scheduleNode.pHolidayList = NULL;
		count++;
		offset += holidayScheduleHead.holidaynum*sizeof(HOLIDAY_SCHEDULE);
	}
	dataLen = (unsigned short)offset;
	list.RemoveAll();
	subNum = 1;
	return true;
}

bool CConfigEx::SetHolidayScheCfgInfo(unsigned itemID, const unsigned char *PData,unsigned short subNum)
{
	bool bRet = false;
	CMyList <SCHEDULE_NODE> list;	 
	SCHEDULE_NODE scheduleNode;
	HOLIDAY_SCHEDULE holiday;
	NCFG_INFO_HOLIDAY_SCHE_HEAD holidaySchHead;
	unsigned long holidayGroupNum = getHolidayGroupNum(itemID);
	for (unsigned long i = 0; i < holidayGroupNum; i++)
	{
		memcpy(&holidaySchHead, (PData+i*sizeof(NCFG_INFO_HOLIDAY_SCHE_HEAD)), sizeof(NCFG_INFO_HOLIDAY_SCHE_HEAD));
		scheduleNode.chnn = i;
		scheduleNode.itemID = itemID;
		scheduleNode.pHolidayList = new CMyList<HOLIDAY_SCHEDULE>;

		for (ULONG j=0; j<holidaySchHead.holidaynum; j++)
		{
			memcpy(&holiday, PData+(holidaySchHead.offset+j*sizeof(HOLIDAY_SCHEDULE)), sizeof(HOLIDAY_SCHEDULE));
			scheduleNode.pHolidayList->AddTail(holiday);			
		}
		list.AddTail(scheduleNode);
	}

	bRet = m_scheduleHolidayCfg.ModifyHolidayScheduleList(list);

	//��������
	POS pos = list.GetHeadPosition();
	while (pos != NULL)
	{
		scheduleNode = list.GetAt(pos);
		scheduleNode.pHolidayList->RemoveAll();
		delete scheduleNode.pHolidayList;
		scheduleNode.pHolidayList = NULL;
		list.GetNext(pos);
	}
	list.RemoveAll();

	return bRet;

}

unsigned long CConfigEx::getHolidayGroupNum(unsigned short itemID)
{
	if((itemID == NCFG_ITEM_REC_SCH_SCHEDULE_HOLIDAY) || \
		(itemID == NCFG_ITEM_REC_SCH_MOTION_HOLIDAY) || \
		(itemID == NCFG_ITEM_REC_SCH_SENSOR_HOLIDAY) || \
		(itemID == NCFG_ITEM_MOTION_SCH_HOLIDAY) || \
		(itemID == NCFG_ITEM_NET_ALARM_SCH_HOLIDAY))
	{
		return m_videoInputNum;
	}
	else if(itemID == NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY)
	{
		return m_sensorInputNum;
	}
	else if(itemID == NCFG_ITEM_ALARMOUT_SCH_HOLIDAY)
	{
		return m_sensorOutputNum;
	}
	else
	{
		return 0;
	}
}

bool CConfigEx::bisMixDVR()
{
	bool bMix = false;
	PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
	switch(productType)
	{
		case TD_2304SS_SDI:
		case TD_2308SS_SDI:
		case TD_2508HE_SDI:
		case TD_2532HD_SDI:
			bMix = true;
		break;
		default:
			bMix = false;
	}
	return bMix;
}

bool CConfigEx::LoadBackupConfig(unsigned char **ppBackupConfig, unsigned long & len)
{
	unsigned char* pConfig;
#ifdef __ENVIRONMENT_WIN32__
	FILE* fp = fopen("E:\\XDVRT\\Bkconfig\\config.dat", "rb");
#else
	FILE* fp = fopen("/mnt/Bkconfig/config.dat", "rb");
#endif
	if (fp == NULL)
	{
		return false;
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	pConfig = new unsigned char[len];
	if (1 != fread(pConfig, len, 1, fp))
	{
		len = 0;
		fclose(fp);
		fp = NULL;
		delete [] pConfig;
		pConfig = NULL;
		return false;
	}

	*ppBackupConfig = pConfig;
	fclose(fp);
	fp = NULL;
	return true;
}

bool CConfigEx::GetItemDataFromBackupConfig(unsigned char* pConfig, unsigned char* pBackupConfig, unsigned long oldConfigLen, unsigned long *pos)
{
	if ((pConfig == NULL) || (pBackupConfig == NULL) || (pos == NULL) || (*pos > oldConfigLen))
	{
		return false;
	}
	NCFG_ITEM_HEAD *pBackupItemHead = NULL;
	unsigned long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	NCFG_ITEM_HEAD *pItemHead = NULL;
	pBackupItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBackupConfig + *pos);
	pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pConfig);

	while(pBackupItemHead->itemID <= pItemHead->itemID)
	{
		pBackupItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pBackupConfig + *pos);
		if (pBackupItemHead->itemID == pItemHead->itemID)
		{
			if(0 == memcmp(pBackupItemHead, pItemHead, itemHeadLen))
			{
		
				memcpy(pConfig+itemHeadLen, pBackupConfig+*pos+itemHeadLen, pItemHead->len);
				*pos += itemHeadLen + pBackupItemHead->len;
				return true;
			}
			else
			{
				return false;
			}
		}
		*pos += itemHeadLen + pBackupItemHead->len;
		if (*pos > oldConfigLen)
		{
			return false;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
//end
