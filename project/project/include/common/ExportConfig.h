/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-06-19
** Name         : ExportConfig.h
** Version      : 1.0
** Description  : DVR配置信息导出功能。配置导出时需要导出config.dat CruiseConfig.dat ScheduleConfig.dat
				  和PresetConfig.dat四个文件。把这四个文件打包导出为一个文件，其格式如下
				  -------------------------------
				  |			导出文件头			|
				  -------------------------------
				  |			config.dat			|
				  -------------------------------
				  |			PresetConfig.dat	|
				  -------------------------------
				  |			CruiseConfig.dat	|
				  -------------------------------
				  |			ScheduleConfig.dat	|
				  -------------------------------

				  导出文件头的结构为
				  -------------------------------
				  |		EXPORT_FILE_HEADER		|
				  -------------------------------
				  |	5Xsizeof(EXPORT_FILE_INFO)	|			目前可以有5个导出文件
				  -------------------------------
** Modify Record:
1:修改校验位的长度为4字节 zhl 2010-07-17
***********************************************************************/

#ifndef _EXPORT_CONFIG_H_
#define _EXPORT_CONFIG_H_

#ifdef __ENVIRONMENT_LINUX__
#include "PUB_common.h"
#endif



typedef struct _device_basic_info_
{
	unsigned long	productID;			//产品ID
	unsigned long	subProductID;		//产品子ID
	char			firmwareVersion[36];//固件版本	

	unsigned long	deviceID;			//设备ID
	char			deviceName[36];		//设备名字
	unsigned char	deviceMAC[8];		//设备物理地址

	unsigned long 	dateTime;			//操作的日期
}DEVICE_BASIC_INFO;

typedef struct _export_file_info_
{
	char					name[128];
	unsigned long			startPos;
	unsigned long			length;
	int						fileType;
}EXPORT_FILE_INFO;

typedef struct _export_file_header_
{
	DEVICE_BASIC_INFO	exportDeviceBasicInfo;			//导出设备的基本信息
	DEVICE_BASIC_INFO	importDeviceBasicInfo;			//导入设备的基本信息
	unsigned long		exportVersion;					//导出功能的版本，只有导入文件中的版本和DVR中的版本相同才允许导入
	int					exportType;						//导出类型，目前只用0来代表配置的导出
	unsigned long		headerLength;					//导出文件头信息长度
	unsigned long		exportFileLength;				//导出文件长度
	unsigned long		exportFileCount;				//导出文件个数
	unsigned long		checkSum;						//导出文件的校验，在导出时填写，导入时检查
	char				notused[64];					//预留空间
}EXPORT_FILE_HEADER;



typedef enum _export_file_type_
{
	EXPORT_FILE_TYPE_CONFIG,
	EXPORT_FILE_TYPE_PRESET,
	EXPORT_FILE_TYPE_CRUISE,
	EXPORT_FILE_TYPE_SCHEDULE,
#ifdef __DVR_ULTIMATE__
	EXPORT_FILE_TYPE_NETDEVICE,
#endif
}EXPORT_FILE_TYPE;

typedef enum _export_status_
{
	EXPORT_STATUS_NEXIM = 0,							//此时没有进行配置的导入和导出
	EXPORT_STATUS_EXPORTING,							//正在导出配置	
	EXPORT_STATUS_IMPORTING,							//正在导入配置
}EXPORT_STATUS;

//
const unsigned long EXPORTCONFIGVERSION = 2;			//导出功能的版本，如果导出文件头的结构体改变就需要更改导出功能的版本
const unsigned long CHECKDATALENGTH		= sizeof(unsigned long);		//使用的校验数据长度
const unsigned long MAX_EXPORT_FILE_COUNT = 5;
const unsigned long EXPORT_DATA_POS = sizeof(EXPORT_FILE_HEADER) + (MAX_EXPORT_FILE_COUNT * sizeof(EXPORT_FILE_INFO));
const char EXPORT_FILE_NAME1[] = "/mnt/mtd/config/config.dat";
const char EXPORT_FILE_NAME2[] = "/mnt/mtd/config/PresetConfig.dat";
const char EXPORT_FILE_NAME3[] = "/mnt/mtd/config/CruiseConfig.dat";
const char EXPORT_FILE_NAME4[] = "/mnt/mtd/config/ScheduleConfig.dat";

#ifdef __DVR_ULTIMATE__
const char EXPORT_FILE_NAME5[] = "/mnt/mtd/config/RegNetDeviceLog.dat";
#endif

const char SAVE_CONFIG_DIR_CONTENT_CMD[] = "cp -rf /mnt/mtd/config /mnt/mtd/configtmp";
const char RESTORE_CONFIG_DIRCONTENT_CMD[] = "cp -rf /mnt/mtd/configtmp/* /mnt/mtd/config/";
const char DELETE_DIRTY_FILES_CMD[] = "rm -rf /mnt/mtd/configtmp";

class CExportConfig
{
public:
	
	~CExportConfig();
	static CExportConfig *Instance();

#ifdef __ENVIRONMENT_LINUX__
	int ExportConfig(unsigned char **ppExportFileCount, unsigned long &fileLength);
	int ImportConfig(const char *pImportFileContent, unsigned long dataLen);
#endif

	int CheckImportParam(const char *pExportFileContent,unsigned long dataLen, const char *pHardWareVersion, const char *pFirmwareVersion );
protected:

#ifdef __ENVIRONMENT_LINUX__
	bool InitialHeader(int exportType, unsigned short	exportFileCount,char ** ppExFileName);
	bool WriteExportFileBody();
#endif

	unsigned long GenerateCheckSum(const char *data, unsigned long dataLen);
	bool GetDeviceBasicInfo(DEVICE_BASIC_INFO *pDeviceBasicInfo, const char *pHardWareVersion, const char *pFirmwareVersion);
	void DeleteResource();

#ifdef __ENVIRONMENT_LINUX__
	int ModifyConfig(const char *pExportFileContent);
	int ModifyBasicConfig(const char *pBasicConfig, unsigned long length);
	int ModifyPresetConfig(const char *pPresetConfig, unsigned long length);
	int ModifyCruiseConfig(const char *pCruiseConfig, unsigned long length);
	int ModifyScheduleConfig(const char *pScheduleConfig, unsigned long length);
#ifdef __DVR_ULTIMATE__
	int ModifyNetDeviceConfig(const char *pNetDeviceConfig, unsigned long length);
#endif
#endif

private:
	CExportConfig();

	EXPORT_FILE_HEADER *m_pExportFileHeader;
	char			   *m_pExportFileContent;

#ifdef __ENVIRONMENT_LINUX__
	CPUB_Lock			m_exportStatusLock;			
#endif
	EXPORT_STATUS		m_exportStatus;

};
#endif //_EXPORT_CONFIG_H_
