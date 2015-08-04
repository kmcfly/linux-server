/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-06-19
** Name         : ExportConfig.cpp
** Version      : 1.0
** Description  : 导入导出配置信息,对配置进行完全导出和导入。导入没有检测某个配置项是否真的需要导入
** Modify Record:
1:
***********************************************************************/

#include "ExportConfig.h"
#include "NetProtocol.h"

#ifdef __ENVIRONMENT_LINUX__
#include "ConfigEx.h"
#include "LocalDevice.h"
#include "NetInterface.h"
extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
#endif

#ifdef __DVR_ULTIMATE__
#include "NetDeviceManager.h"
#endif




CExportConfig::CExportConfig()
{
	m_pExportFileHeader = NULL;
	m_pExportFileContent = NULL;

	m_exportStatus = EXPORT_STATUS_NEXIM;
	
}

CExportConfig::~CExportConfig()
{
	DeleteResource();

	m_exportStatus = EXPORT_STATUS_NEXIM;
}


CExportConfig * CExportConfig::Instance()
{
	static CExportConfig exportConfig;
	return &exportConfig;
}

#ifdef __ENVIRONMENT_LINUX__
int CExportConfig::ExportConfig(unsigned char **ppExportFileCount, unsigned long &fileLength)
{
	if( NULL == ppExportFileCount )
	{
		assert(false);
		return NETERR_EXPORT_CONFIG_ERR;
	}

	m_exportStatusLock.Lock();
	if(EXPORT_STATUS_NEXIM != m_exportStatus)
	{
		m_exportStatusLock.UnLock();
		printf("%s:%s:%d, 不能有一个以上用户同时进行导入和导出配置\n", __FUNCTION__, __FILE__, __LINE__);
		return NETERR_EXPORT_CONFIG_OVERONEUSER;
	}
	else
	{
		m_exportStatus = EXPORT_STATUS_EXPORTING;
	}
	m_exportStatusLock.UnLock();

	DeleteResource();

	//初始化导出文件头信息，只初始化导出设备和导出文件的相关信息
	unsigned short exportFileCount = 0;
#ifdef __DVR_ULTIMATE__	
	/*if (0 == g_p_login_succ->productInfo.localVideoInputNum)
	{
		printf("%s:%s:%d, Export 3 config files\n", __FUNCTION__, __FILE__, __LINE__);
		exportFileCount = 3;
	}
	else*/
	{
		exportFileCount = 5;
	}
#else
		exportFileCount = 4;
#endif
	if( !InitialHeader(0,exportFileCount,NULL) )
	{
		assert(false);
		DeleteResource();
		
		m_exportStatusLock.Lock();
		m_exportStatus = EXPORT_STATUS_NEXIM;
		m_exportStatusLock.UnLock();
		
		return NETERR_EXPORT_CONFIG_ERR;
	}

	//读取导出的文件
	if( !WriteExportFileBody() )
	{
		assert(false);
		DeleteResource();

		m_exportStatusLock.Lock();
		m_exportStatus = EXPORT_STATUS_NEXIM;
		m_exportStatusLock.UnLock();
		
		return NETERR_EXPORT_CONFIG_ERR;
	}
	
	//添加导出校验信息
	EXPORT_FILE_HEADER *pExportFileHd = (EXPORT_FILE_HEADER *)m_pExportFileContent;
	pExportFileHd->checkSum = GenerateCheckSum(m_pExportFileContent, m_pExportFileHeader->exportFileLength);


	//写导出文件
	if( NULL != *ppExportFileCount)
	{
		delete [] *ppExportFileCount;
		*ppExportFileCount = NULL;
	}
	
	*ppExportFileCount = new unsigned char [m_pExportFileHeader->exportFileLength];
	memset(*ppExportFileCount, 0, m_pExportFileHeader->exportFileLength);
	memcpy(*ppExportFileCount, m_pExportFileContent, m_pExportFileHeader->exportFileLength);
	fileLength = m_pExportFileHeader->exportFileLength;

	DeleteResource();

	m_exportStatusLock.Lock();
	m_exportStatus = EXPORT_STATUS_NEXIM;
	m_exportStatusLock.UnLock();
	
	return NETERR_EXPORT_CONFIG_OK;
}

int CExportConfig::ImportConfig(const char *pImportFileContent, unsigned long dataLen)
{

	if( (NULL == pImportFileContent) || (EXPORT_DATA_POS >= dataLen) )
	{
		assert(false);
		return NETERR_EXPORT_CONFIG_FILEERR;
	}

	m_exportStatusLock.Lock();
	if(EXPORT_STATUS_NEXIM != m_exportStatus)
	{
		m_exportStatusLock.UnLock();
		printf("%s:%s:%d, 不能有一个以上用户同时进行导入和导出配置\n", __FUNCTION__, __FILE__, __LINE__);
		return NETERR_EXPORT_CONFIG_OVERONEUSER;
	}
	else
	{
		m_exportStatus = EXPORT_STATUS_IMPORTING;
	}
	m_exportStatusLock.UnLock();

	DeleteResource();
	
	m_pExportFileContent = new char [dataLen];
	memcpy(m_pExportFileContent, pImportFileContent, dataLen);

	int retval = CheckImportParam(m_pExportFileContent,dataLen, g_p_login_succ->productInfo.hardwareVersion, g_p_login_succ->productInfo.firmwareVersion);
	if( NETERR_EXPORT_CONFIG_OK != retval )
	{
		assert(false);
		printf("%s:%s:%d, 检测导入信息错误\n", __FUNCTION__, __FILE__, __LINE__);
		DeleteResource();

		m_exportStatusLock.Lock();
		m_exportStatus = EXPORT_STATUS_NEXIM;
		m_exportStatusLock.UnLock();

		return retval;
	}

	
	//先保存DVR中的配置文件，如果导出出错再恢复配置文件
	DVRSystem(SAVE_CONFIG_DIR_CONTENT_CMD);
	//用导出文件修改配置文件
	retval = ModifyConfig(m_pExportFileContent);

	if(NETERR_EXPORT_CONFIG_OK != retval)
	{
		DVRSystem(RESTORE_CONFIG_DIRCONTENT_CMD);
		printf("%s:%s:%d, 导入配置失败\n", __FUNCTION__, __FILE__, __LINE__);
	}
	
	DVRSystem(DELETE_DIRTY_FILES_CMD);
	DeleteResource();

	m_exportStatusLock.Lock();
	m_exportStatus = EXPORT_STATUS_NEXIM;
	m_exportStatusLock.UnLock();

	return retval;
}
#endif

int CExportConfig::CheckImportParam(const char *pExportFileContent,unsigned long dataLen, const char *pHardWareVersion, const char *pFirmwareVersion)
{
	int retval = NETERR_EXPORT_CONFIG_OK;

	if( NULL == pExportFileContent )
	{
		assert(false);
		return NETERR_EXPORT_CONFIG_ERR;
	}
	
	EXPORT_FILE_HEADER *pExportFileHeader = (EXPORT_FILE_HEADER *)pExportFileContent;
	//文件长度是4字节的整数倍
	if(pExportFileHeader->exportFileLength % sizeof(unsigned long))
	{
		assert(false);
		printf("%s:%s;%d, 文件长度错误\n", __FUNCTION__, __FILE__, __LINE__);
		return NETERR_EXPORT_CONFIG_ERR;
	}

	if ( pExportFileHeader->exportFileLength != dataLen)
	{
		assert(false);

		printf("%s:%s:%d, dataLen=%d, fileLength=%d\n", __FUNCTION__, __FILE__, __LINE__, dataLen, pExportFileHeader->exportFileLength);
		return NETERR_EXPORT_CONFIG_ERR;
	}

	//检测校验信息是否错误
	unsigned long checkSum = pExportFileHeader->checkSum;
	pExportFileHeader->checkSum = 0;
	pExportFileHeader->checkSum = GenerateCheckSum(pExportFileContent,dataLen);
	if(pExportFileHeader->checkSum !=  checkSum)
	{
		assert(false);
		printf("%s:%s:%d, 文件中有错误信息, checkSum=0x%08x, pExportFileHd->checkSum=0x%08x\n", __FUNCTION__, __FILE__, __LINE__, checkSum, pExportFileHeader->checkSum);
		return NETERR_EXPORT_CONFIG_ERR;
	}

	//检测导出功能的版本是否一致
	if( EXPORTCONFIGVERSION != pExportFileHeader->exportVersion)
	{
		printf("%s:%s:%d ,导出功能的版本号不同\n", __FUNCTION__, __FILE__, __LINE__);
		return NETERR_EXPORT_CONFIG_VERSION;
	}

	//检测导出DVR的产品ID，产品子ID，和固件版本是否和导入的一致

	DEVICE_BASIC_INFO importDeviceBasicInfo;
	memset(&importDeviceBasicInfo, 0, sizeof(DEVICE_BASIC_INFO));
	if(!GetDeviceBasicInfo( &importDeviceBasicInfo, pHardWareVersion, pFirmwareVersion))
	{
		assert(false);
		printf("%s:%s:%d, 导入设备信息\n", __FUNCTION__, __FILE__, __LINE__);
		return NETERR_EXPORT_CONFIG_ERR;
	}

	if( pExportFileHeader->exportDeviceBasicInfo.productID != importDeviceBasicInfo.productID )
	{
		printf("%s:%s:%d, 导入的产品ID为:%d, 导出的产品ID为:%d\n", __FUNCTION__, __FILE__, __LINE__, pExportFileHeader->exportDeviceBasicInfo.productID, importDeviceBasicInfo.productID);
		return NETERR_EXPORT_CONFIG_DVRIDERR;
	}

	if( pExportFileHeader->exportDeviceBasicInfo.subProductID != importDeviceBasicInfo.subProductID )
	{
		printf("%s:%s:%d, 导入的产品子ID为:%d, 导出的产品子ID为:%d\n", __FUNCTION__, __FILE__, __LINE__, pExportFileHeader->exportDeviceBasicInfo.subProductID, importDeviceBasicInfo.subProductID);
		return NETERR_EXPORT_CONFIG_DVRIDERR;
	}

	if( ( strlen(pExportFileHeader->exportDeviceBasicInfo.firmwareVersion) != strlen(importDeviceBasicInfo.firmwareVersion) )
		|| ( strncmp(pExportFileHeader->exportDeviceBasicInfo.firmwareVersion, importDeviceBasicInfo.firmwareVersion, strlen(pExportFileHeader->exportDeviceBasicInfo.firmwareVersion))) 
		)
	{
		printf("%s:%s:%d, 导入的固件版本为:%s 导出的固件版本为:%s\n", __FUNCTION__, __FILE__, __LINE__, pExportFileHeader->exportDeviceBasicInfo.firmwareVersion, importDeviceBasicInfo.firmwareVersion);
		return NETERR_EXPORT_CONFIG_FIRMWAREERR;
	}

	//EXPORT_FILE_INFO *pExportFileInfo = (EXPORT_FILE_INFO *)(pExportFileHeader+1);

	//for(int i=0; i<pExportFileHeader->exportFileCount; i++, pExportFileInfo++)
	//{
	//	printf("%s:%s:%d, file name=%s, start pos=%d, file length=%d\n", __FUNCTION__, __FILE__, __LINE__, pExportFileInfo->name, pExportFileInfo->startPos, pExportFileInfo->length);

	//}

	return NETERR_EXPORT_CONFIG_OK;
}

unsigned long CExportConfig::GenerateCheckSum(const char *data, unsigned long dataLen)
{
	if( NULL == data)
	{
		assert(false);
		printf("%s:%s:%d, 没有数据\n", __FUNCTION__, __FILE__, __LINE__);
		return 0;
	}

	unsigned long count = 0;
	unsigned long checkSum = 0;
	int len = dataLen;

	const char *pIndex = data;

	while( len > (sizeof(unsigned long) - 1) )				//大于3字节才进行下次循环
	{
		checkSum += *(unsigned long*)pIndex;
		pIndex += sizeof(unsigned long);
		len -= sizeof(unsigned long);	
	}

	for ( int i=0; i<len; i++, pIndex++)
	{
		count |= *pIndex << ((sizeof(unsigned long) - 1 - i) * 8) ;
	}

	checkSum += count;
	
	checkSum = ~checkSum;
	//printf("%s:%s:%d, checkSum = 0x%08x\n", __FUNCTION__, __FILE__, __LINE__, checkSum);
	return checkSum;
}

#ifdef __ENVIRONMENT_LINUX__
bool CExportConfig::InitialHeader(int exportType, unsigned short exportFileCount,char ** ppExFileName)
{
	if(NULL != m_pExportFileHeader)
	{
		printf("%s:%s:%d, initial the export file header more than one times\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return false;

	}

	m_pExportFileHeader = (EXPORT_FILE_HEADER *)(new char[EXPORT_DATA_POS]);
	if( NULL == m_pExportFileHeader )
	{
		assert(false);
		return false;
	}
	memset(m_pExportFileHeader, 0, EXPORT_DATA_POS);

	//导出设备的基本信息
	if( !GetDeviceBasicInfo(&m_pExportFileHeader->exportDeviceBasicInfo, g_p_login_succ->productInfo.hardwareVersion, g_p_login_succ->productInfo.firmwareVersion))
	{
		return false;
	}

	m_pExportFileHeader->exportVersion		= EXPORTCONFIGVERSION;
	m_pExportFileHeader->exportType			= exportType;
	m_pExportFileHeader->exportFileCount	= exportFileCount;
	m_pExportFileHeader->exportFileLength	= EXPORT_DATA_POS;
	m_pExportFileHeader->headerLength		= EXPORT_DATA_POS;

	EXPORT_FILE_INFO *pExportFileInfo = (EXPORT_FILE_INFO*)(m_pExportFileHeader+1);
	if( NULL == ppExFileName )
	{
		//默认导出4个配置文件
		strncpy( pExportFileInfo->name, EXPORT_FILE_NAME1, sizeof(pExportFileInfo->name));
		pExportFileInfo->fileType = EXPORT_FILE_TYPE_CONFIG;
		
		//if (3 != exportFileCount)
		{
			pExportFileInfo++;
			strncpy( pExportFileInfo->name, EXPORT_FILE_NAME2, sizeof(pExportFileInfo->name));
			pExportFileInfo->fileType = EXPORT_FILE_TYPE_PRESET;

			pExportFileInfo++;
			strncpy( pExportFileInfo->name, EXPORT_FILE_NAME3, sizeof(pExportFileInfo->name));
			pExportFileInfo->fileType = EXPORT_FILE_TYPE_CRUISE;
		}

		pExportFileInfo++;
		strncpy( pExportFileInfo->name, EXPORT_FILE_NAME4, sizeof(pExportFileInfo->name));
		pExportFileInfo->fileType = EXPORT_FILE_TYPE_SCHEDULE;

#ifdef __DVR_ULTIMATE__
		pExportFileInfo++;
		strncpy(pExportFileInfo->name, EXPORT_FILE_NAME5, sizeof(pExportFileInfo->name));
		pExportFileInfo->fileType = EXPORT_FILE_TYPE_NETDEVICE;
#endif
	}
	else
	{
		//可选的导出文件
		char **ppExFileNameTemp = ppExFileName;
		for(int i=0; i<m_pExportFileHeader->exportFileCount; i++ ,pExportFileInfo++, ppExFileNameTemp++)
		{
			strncpy( pExportFileInfo->name, *ppExFileNameTemp, sizeof(pExportFileInfo->name));
		}
	}
// 

	/*printf("%s:%s:%d, export %d configure files to %s\n", __FUNCTION__, __FILE__, __LINE__,m_pExportFileHeader->exportFileCount,pExportDirPath);
	printf("%s:%s:%d, export DVR basic information:productID=%d, subproductID=%d, firmwareVersion=%s, deiveID=%d, deiveName=%s, device MAC=%s\n",
			__FUNCTION__, __FILE__, __LINE__ ,m_pExportFileHeader->exportDeviceBasicInfo.productID,
			m_pExportFileHeader->exportDeviceBasicInfo.subProductID,
			m_pExportFileHeader->exportDeviceBasicInfo.firmwareVersion,
			m_pExportFileHeader->exportDeviceBasicInfo.deviceID,
			m_pExportFileHeader->exportDeviceBasicInfo.deviceName,
			m_pExportFileHeader->exportDeviceBasicInfo.deviceMAC);*/
	return true;
}



bool CExportConfig::WriteExportFileBody()
{
	EXPORT_FILE_INFO *pExportFileInfo = (EXPORT_FILE_INFO*)(m_pExportFileHeader+1);
	unsigned long dataPos = EXPORT_DATA_POS;

	for(int i=0; i<m_pExportFileHeader->exportFileCount; i++ ,pExportFileInfo++)
	{
		FILE *pFDis = fopen(pExportFileInfo->name, "r");
		if( NULL == pFDis )
		{
			perror("fopen()");
			assert(false);
			return false;
		}

		fseek( pFDis, 0, SEEK_END );
		pExportFileInfo->length = ftell(pFDis);
		pExportFileInfo->startPos = dataPos;
		dataPos += pExportFileInfo->length;
		m_pExportFileHeader->exportFileLength += pExportFileInfo->length;
		//printf("%s:%s:%d, file name=%s, startPos=%d, file length=%d, export fileLength=%d\n", __FUNCTION__, __FILE__, __LINE__, pExportFileInfo->name, pExportFileInfo->startPos, pExportFileInfo->length, m_pExportFileHeader->exportFileLength);
		fclose(pFDis);
		pFDis = NULL;
	}
	
	//文件长度修正为4个字节的整数倍
	m_pExportFileHeader->exportFileLength += (CHECKDATALENGTH - (m_pExportFileHeader->exportFileLength % CHECKDATALENGTH));
	
	if( NULL != m_pExportFileContent )
	{
		delete [] m_pExportFileContent;
		m_pExportFileContent = NULL;
	}
	m_pExportFileContent = new char[m_pExportFileHeader->exportFileLength];
	if(NULL == m_pExportFileContent)
	{
		printf("%s:%s:%d, 导出配置时申请不到内存\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return false;
	}
	memset(m_pExportFileContent, 0, m_pExportFileHeader->exportFileLength);
	
	//导出的文件头信息
	memcpy(m_pExportFileContent, m_pExportFileHeader, EXPORT_DATA_POS);

	//导出的文件内容
	pExportFileInfo = (EXPORT_FILE_INFO*)(m_pExportFileHeader+1);
	for(int i=0; i<m_pExportFileHeader->exportFileCount; i++ ,pExportFileInfo++)
	{
		FILE *pFDis = fopen(pExportFileInfo->name, "r");
		if( 1 != fread(m_pExportFileContent+pExportFileInfo->startPos, pExportFileInfo->length, 1, pFDis))
		{
			perror("fead()");
			assert(false);
			fclose(pFDis);
			pFDis = NULL;
			return false;
		}
		fclose(pFDis);
		pFDis = NULL;
	}

	return true;
}
#endif

bool CExportConfig::GetDeviceBasicInfo(DEVICE_BASIC_INFO *pDeviceBasicInfo, const char *pHardwareVersion, const char *pFirmwareVersion)
{
	if( (NULL == pDeviceBasicInfo) || (NULL == pHardwareVersion) || (NULL == pFirmwareVersion) )
	{
		assert(false);
		printf("%s:%s:%d, GetDeviceBasicInfo() error\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}

	memset(pDeviceBasicInfo, 0, sizeof(DEVICE_BASIC_INFO));
	if( 2 != sscanf(pHardwareVersion, "%d.%d", &pDeviceBasicInfo->productID, &pDeviceBasicInfo->subProductID))
	{
		assert(false);
		printf("%s:%s:%d, 硬件版本错误:%s\n", __FUNCTION__, __FILE__, __LINE__,pHardwareVersion);
		return false;
	}
	

	memcpy(pDeviceBasicInfo->firmwareVersion, pFirmwareVersion, sizeof(pDeviceBasicInfo->firmwareVersion));		//固件版本号 类似3.1.0.P

#ifdef __ENVIRONMENT_LINUX__
	pDeviceBasicInfo->deviceID		= g_p_login_succ->productInfo.deviceID;					//设备号
	strncpy(pDeviceBasicInfo->deviceName, g_p_login_succ->productInfo.deviceName, sizeof(pDeviceBasicInfo->deviceName));					//设备名字
	memcpy(pDeviceBasicInfo->deviceMAC, g_p_login_succ->productInfo.deviceMAC, sizeof(pDeviceBasicInfo->deviceMAC));						//DVR硬件地址
#endif

	pDeviceBasicInfo->dateTime		= GetCurrTime32();										//导出时间

	return true;

}
void CExportConfig::DeleteResource()
{
	if( NULL != m_pExportFileHeader )
	{
		delete m_pExportFileHeader;
		m_pExportFileHeader = NULL;
	}


	if( NULL != m_pExportFileContent )
	{
		delete [] m_pExportFileContent;
		m_pExportFileContent = NULL;
	}


}

#ifdef __ENVIRONMENT_LINUX__
int CExportConfig::ModifyConfig( const char *pExportFileContent )
{
	int retval = NETERR_EXPORT_CONFIG_OK;

	if(NULL == pExportFileContent)
	{
		printf("%s:%s:%d, pExportFileContent==NULL\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return NETERR_EXPORT_CONFIG_ERR;
	}
	
	CConfigEx::Instance()->Quit();
	
	m_pExportFileHeader = (EXPORT_FILE_HEADER *)(new char [EXPORT_DATA_POS]);
	if(NULL == m_pExportFileHeader)
	{
		printf("%s:%s:%d, 导入时申请不到内存\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return NETERR_EXPORT_CONFIG_ERR;
	}
	memcpy(m_pExportFileHeader, m_pExportFileContent, EXPORT_DATA_POS);

//#ifdef __DVR_ULTIMATE__
//	CNetDeviceManager::Instance()->Stop();
//	CNetDeviceManager::Instance()->Quit();
//#endif

	EXPORT_FILE_INFO *pExportFileInfo = (EXPORT_FILE_INFO *)(m_pExportFileHeader + 1);
	for(int i=0; i<m_pExportFileHeader->exportFileCount; i++, pExportFileInfo++)
	{
		printf("%s:%s:%d, fileType=%d, fileName=%s, fileLength=%d, startPos=%d\n", __FUNCTION__, __FILE__, __LINE__, pExportFileInfo->fileType, pExportFileInfo->name, pExportFileInfo->length,pExportFileInfo->startPos);
		switch(pExportFileInfo->fileType)
		{
		case EXPORT_FILE_TYPE_CONFIG:
			retval = ModifyBasicConfig( (pExportFileContent + pExportFileInfo->startPos), pExportFileInfo->length);
			if(NETERR_EXPORT_CONFIG_OK != retval)
			{
				return retval;
			}
			break;

		case EXPORT_FILE_TYPE_PRESET:
			retval = ModifyPresetConfig( (pExportFileContent + pExportFileInfo->startPos), pExportFileInfo->length);
			if(NETERR_EXPORT_CONFIG_OK != retval)
			{
				return retval;
			}
			break;

		case EXPORT_FILE_TYPE_CRUISE:
			retval = ModifyCruiseConfig( (pExportFileContent + pExportFileInfo->startPos), pExportFileInfo->length);
			if(NETERR_EXPORT_CONFIG_OK != retval)
			{
				return retval;
			}
			break;

		case EXPORT_FILE_TYPE_SCHEDULE:
			retval = ModifyScheduleConfig( (pExportFileContent + pExportFileInfo->startPos), pExportFileInfo->length);
			if(NETERR_EXPORT_CONFIG_OK != retval)
			{
				return retval;
			}
			break;
#ifdef __DVR_ULTIMATE__
		case EXPORT_FILE_TYPE_NETDEVICE:
			retval = ModifyNetDeviceConfig((pExportFileContent + pExportFileInfo->startPos), pExportFileInfo->length);
			if (NETERR_EXPORT_CONFIG_OK != retval)
			{
				return retval;
			}
			break;
#endif
		default:
			break;
		}
	}

	DVRSystem("sync");

	CConfigEx::Instance()->Initial();
	
	return retval;
}

/*
暂时对每个配置文件完全导入。如果需要过滤掉某个配置项就在这四个函数中处理
*/
int CExportConfig::ModifyBasicConfig(const char *pBasicConfig, unsigned long length)
{
	printf("%s:%s:%d, modify basic config\n", __FUNCTION__, __FILE__, __LINE__);
	int retval = NETERR_EXPORT_CONFIG_ERR;

	if(NULL == pBasicConfig)
	{
		assert(false);
		printf("%s:%s:%d, 更新基本配置文件\n", __FUNCTION__, __FILE__, __LINE__);
		return retval;
	}

	
	FILE *pBCfigDis = fopen(EXPORT_FILE_NAME1,"w+");
	if(NULL == pBCfigDis)
	{
		perror("fopen");
		assert(false);
		return retval;
	}

	int writeCount = 0;

	do 
	{
		if( 1 == fwrite(pBasicConfig, length, 1, pBCfigDis))
		{
			retval = NETERR_EXPORT_CONFIG_OK;
			break;
		}
		else
		{
			printf("%s:%s:%d", __FUNCTION__, __FILE__, __LINE__);
			perror("fwrite()");
			printf("\n");
			if(++writeCount > 5)
			{
				break;
			}
		}

	} while(1);

	fflush(pBCfigDis);
	fclose(pBCfigDis);
	pBCfigDis = NULL;
	return retval;
}

int CExportConfig::ModifyPresetConfig(const char *pPresetConfig, unsigned long length)
{
	printf("%s:%s:%d, modify preset config\n", __FUNCTION__, __FILE__, __LINE__);

	int retval = NETERR_EXPORT_CONFIG_ERR;

	if(NULL == pPresetConfig)
	{
		assert(false);
		printf("%s:%s:%d, 更新预置点配置文件\n", __FUNCTION__, __FILE__, __LINE__);
		return retval;
	}



	FILE *pPCfigDis = fopen(EXPORT_FILE_NAME2, "w+");
	if(NULL == pPCfigDis)
	{
		perror("fopen");
		assert(false);
		return retval;
	}

	int writeCount = 0;

	do 
	{
		if( 1 == fwrite(pPresetConfig, length, 1, pPCfigDis))
		{
			retval = NETERR_EXPORT_CONFIG_OK;
			break;
		}
		else
		{
			printf("%s:%s:%d", __FUNCTION__, __FILE__, __LINE__);
			perror("fwrite()");
			printf("\n");
			if(++writeCount > 5)
			{
				break;
			}
		}

	} while(1);

	fflush(pPCfigDis);
	fclose(pPCfigDis);
	pPCfigDis = NULL;
	return retval;
}

int CExportConfig::ModifyCruiseConfig(const char *pCruiseConfig, unsigned long length)
{
	printf("%s:%s:%d, modify cruise config\n", __FUNCTION__, __FILE__, __LINE__);

	int retval = NETERR_EXPORT_CONFIG_ERR;

	if(NULL == pCruiseConfig)
	{
		assert(false);
		printf("%s:%s:%d, 更新巡航线配置文件\n", __FUNCTION__, __FILE__, __LINE__);
		return retval;
	}


	FILE *pCCfigDis = fopen(EXPORT_FILE_NAME3, "w+");
	if(NULL == pCCfigDis)
	{
		perror("fopen");
		assert(false);
		return retval;
	}

	int writeCount = 0;
	//printf("%s:%s:%d, file length=%d\n", __FUNCTION__, __FILE__, __LINE__, length);
	do 
	{
		if( 1 == fwrite(pCruiseConfig, length, 1, pCCfigDis))
		{
			retval = NETERR_EXPORT_CONFIG_OK;
			break;
		}
		else
		{
			printf("%s:%s:%d", __FUNCTION__, __FILE__, __LINE__);
			perror("fwrite()");
			printf("\n");
			if(++writeCount > 5)
			{
				break;
			}
		}

	} while(1);
	

	fflush(pCCfigDis);
	fclose(pCCfigDis);
	pCCfigDis = NULL;
	return retval;
}

int CExportConfig::ModifyScheduleConfig(const char *pScheduleConfig, unsigned long length)
{
	printf("%s:%s:%d, modify schedule config\n", __FUNCTION__, __FILE__, __LINE__);

	int retval = NETERR_EXPORT_CONFIG_ERR;

	if(NULL == pScheduleConfig)
	{
		assert(false);
		printf("%s:%s:%d, 更新排程配置文件\n", __FUNCTION__, __FILE__, __LINE__);
		return retval;
	}



	FILE *pSCfigDis = fopen(EXPORT_FILE_NAME4, "w+");
	if(NULL == pSCfigDis)
	{
		perror("fopen");
		assert(false);
		return retval;
	}
	
	int writeCount = 0;

	do 
	{
		if( 1 == fwrite(pScheduleConfig, length, 1, pSCfigDis))
		{
			retval = NETERR_EXPORT_CONFIG_OK;
			break;
		}
		else
		{
			printf("%s:%s:%d", __FUNCTION__, __FILE__, __LINE__);
			perror("fwrite()");
			printf("\n");
			if(++writeCount > 5)
			{
				break;
			}
		}

	} while(1);

	fflush(pSCfigDis);
	fclose(pSCfigDis);
	pSCfigDis = NULL;
	return retval; 
}

#ifdef __DVR_ULTIMATE__
int CExportConfig::ModifyNetDeviceConfig(const char *pNetDeviceConfig, unsigned long length)
{
	printf("%s:%s:%d, modify det device config\n", __FUNCTION__, __FILE__, __LINE__);

	int retval = NETERR_EXPORT_CONFIG_ERR;

	if(NULL == pNetDeviceConfig)
	{
		assert(false);
		printf("%s:%s:%d, 更新网络设备配置文件\n", __FUNCTION__, __FILE__, __LINE__);
		return retval;
	}



	FILE *pSCfigDis = fopen(EXPORT_FILE_NAME5, "w+");
	if(NULL == pSCfigDis)
	{
		perror("fopen");
		assert(false);
		return retval;
	}

	int writeCount = 0;

	do 
	{
		if( 1 == fwrite(pNetDeviceConfig, length, 1, pSCfigDis))
		{
			retval = NETERR_EXPORT_CONFIG_OK;
			break;
		}
		else
		{
			printf("%s:%s:%d", __FUNCTION__, __FILE__, __LINE__);
			perror("fwrite()");
			printf("\n");
			if(++writeCount > 5)
			{
				break;
			}
		}

	} while(1);

	fflush(pSCfigDis);
	fclose(pSCfigDis);

	pSCfigDis = NULL;
	return retval; 
}
#endif

#endif
