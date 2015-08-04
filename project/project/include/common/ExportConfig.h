/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-06-19
** Name         : ExportConfig.h
** Version      : 1.0
** Description  : DVR������Ϣ�������ܡ����õ���ʱ��Ҫ����config.dat CruiseConfig.dat ScheduleConfig.dat
				  ��PresetConfig.dat�ĸ��ļ��������ĸ��ļ��������Ϊһ���ļ������ʽ����
				  -------------------------------
				  |			�����ļ�ͷ			|
				  -------------------------------
				  |			config.dat			|
				  -------------------------------
				  |			PresetConfig.dat	|
				  -------------------------------
				  |			CruiseConfig.dat	|
				  -------------------------------
				  |			ScheduleConfig.dat	|
				  -------------------------------

				  �����ļ�ͷ�ĽṹΪ
				  -------------------------------
				  |		EXPORT_FILE_HEADER		|
				  -------------------------------
				  |	5Xsizeof(EXPORT_FILE_INFO)	|			Ŀǰ������5�������ļ�
				  -------------------------------
** Modify Record:
1:�޸�У��λ�ĳ���Ϊ4�ֽ� zhl 2010-07-17
***********************************************************************/

#ifndef _EXPORT_CONFIG_H_
#define _EXPORT_CONFIG_H_

#ifdef __ENVIRONMENT_LINUX__
#include "PUB_common.h"
#endif



typedef struct _device_basic_info_
{
	unsigned long	productID;			//��ƷID
	unsigned long	subProductID;		//��Ʒ��ID
	char			firmwareVersion[36];//�̼��汾	

	unsigned long	deviceID;			//�豸ID
	char			deviceName[36];		//�豸����
	unsigned char	deviceMAC[8];		//�豸�����ַ

	unsigned long 	dateTime;			//����������
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
	DEVICE_BASIC_INFO	exportDeviceBasicInfo;			//�����豸�Ļ�����Ϣ
	DEVICE_BASIC_INFO	importDeviceBasicInfo;			//�����豸�Ļ�����Ϣ
	unsigned long		exportVersion;					//�������ܵİ汾��ֻ�е����ļ��еİ汾��DVR�еİ汾��ͬ��������
	int					exportType;						//�������ͣ�Ŀǰֻ��0���������õĵ���
	unsigned long		headerLength;					//�����ļ�ͷ��Ϣ����
	unsigned long		exportFileLength;				//�����ļ�����
	unsigned long		exportFileCount;				//�����ļ�����
	unsigned long		checkSum;						//�����ļ���У�飬�ڵ���ʱ��д������ʱ���
	char				notused[64];					//Ԥ���ռ�
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
	EXPORT_STATUS_NEXIM = 0,							//��ʱû�н������õĵ���͵���
	EXPORT_STATUS_EXPORTING,							//���ڵ�������	
	EXPORT_STATUS_IMPORTING,							//���ڵ�������
}EXPORT_STATUS;

//
const unsigned long EXPORTCONFIGVERSION = 2;			//�������ܵİ汾����������ļ�ͷ�Ľṹ��ı����Ҫ���ĵ������ܵİ汾
const unsigned long CHECKDATALENGTH		= sizeof(unsigned long);		//ʹ�õ�У�����ݳ���
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
