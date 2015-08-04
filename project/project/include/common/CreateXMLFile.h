/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2010-04-21
** Name         : CreateXMLFile.h
** Version      : 1.0
** Description  : 
					1：该类用来产生XML文件，目前可以产生两种XML文件，分别为XML_FORMAT_LOG和XML_FORMAT_EVENT类型的XML文件。
					2：产生的XML文件用于在网页上显示相关数据信息。
					3：每个XML文件中记录100个数据项，在xmlFileName.xml中存放xml类型的日志文件名。
					4：如果生成xml文件后拷贝tableCtrl.html到log目录中
					
					5：日志目录结构。event信息导出的目录结构和日志的结构类似
				|---log
				|	|
				|	|----loglist.html
				|	|
				|	|---xmlfiles
				|	|   |-- xmlFileName.xml
				|	|   |-- LogList0.xml
				|	|   |-- LogList1.xml
				|	|   |-- ............
				|	|	|-- ............
				|	|   `-- LogListn.xml
** Modify Record:
1:
***********************************************************************/
#ifndef _CREATE_XML_FILE_H_
#define _CREATE_XML_FILE_H_

#include "dvrdvsdef.h"
#include "PUB_common.h"

const char _XMLdeclare_[]="<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n";					//XML文件头

#ifdef __ENVIRONMENT_WIN32__
const char _html_file_[]="../WebServer/log_list_ctrl.html";
const char _backup_event_html_file_[]="./WebServer/backup_event_list_ctrl.html";
#else
const char _html_file_[]="./WebServer/log_list_ctrl.html";
const char _backup_event_html_file_[]="./WebServer/backup_event_list_ctrl.html";
#endif

typedef const char * (*ID_TO_STRING)(unsigned long id, long xmlFormat, void *pParam);

typedef enum _button_tip_num_
{
	BUTTON_TIP_FIRST = 0,
	BUTTON_TIP_PRE,
	BUTTON_TIP_NEXT,
	BUTTON_TIP_LAST,
	BUTTON_TIP_RECORD,
	BUTTON_TIP_IS_THE_LAST,
	BUTTON_TIP_IS_THE_FIRST,
}BUTTON_TIP_NUM;

typedef struct _button_tip_info_
{
	BUTTON_TIP_NUM tipNum;
	char tipValue[64];
}BUTTON_TIP_INFO;

class CCreateXMLFile
{
public:
	CCreateXMLFile();
	~CCreateXMLFile();
	
	typedef enum _xml_format_
	{
		XML_FORMAT_LOG=0,
		XML_FORMAT_EVENT=1,
		XML_BACKUP_EVENT=2,//备份时候的事件，zxx
	}XML_FORMAT;


	void SetDescriptionString(ID_TO_STRING pfnIdToString, void *pParam);
	void SetButtonTip(BUTTON_TIP_INFO *pButtonTipInfo, int tipCount);
	void SetExportDir(const char *pExportDir);
	void SetUseCDBackup(bool bUseCDBackup);
	bool CreateXMLFile(const char *pHeadTitle, const char **ppElementTitle, const char *pData, int dataLen, XML_FORMAT format=XML_FORMAT_LOG);
	bool CopyHtmlFile(bool bIsCreateXMLFileOK);
	const char *GetExportPath();

private:
	bool InitialXMLFile(XML_FORMAT format=XML_FORMAT_LOG);
	bool AddTitle(const char *pHeadTitle, const char **ppElementTitle, int titleNum);
	
	bool AddButtonTip(const BUTTON_TIP_INFO *pButtonTipInfo , int tipCount);

	bool AddLogTitleInfo(const char *pHeadTitle, const char **ppElementTitle);			//添加日志头信息
	
	bool AddLogElement(const OPERATOR_LOG *pInfor);										//添加一个日志xml元素

	bool AddEventTitleInfo(const char *pHeadTitle, const char **ppElementTitle);		//添加事件头信息
	bool AddEventElement(const EVENT_LOG *pInfor);										//添加一个事件xml元素
	
	//旧的只能添加4个元素，现在需要添加更多列(目前为5，否则需要修改./WebServer/backup_event_list_ctrl.html)
	bool AddBackupEventTitleInfo(const char *pHeadTitle, const char **ppElementTitle,int count);
	bool AddBackupEventElement(const EVENT_LOG *pInfor,int Elementcount);

	bool AddItem(const char *pMark, const char *pInfor, int fd=-1);						//添加一个xml标签
	bool WriteStartMark(const char *pMark, bool bEnd=false, int fd=-1);					//写XML文件中的开始标签
	bool WriteInforMark(const char *pInfor, int fd=-1);									//写内容
	bool WriteEndMark(const char *pMark, bool bEnd=false,int fd=-1);					//写结束标签

	bool DeleteFiles();
#ifdef __ENVIRONMENT_WIN32__
	bool DeleteDirectory(char *dirName);
#endif

	bool FileFlush( int des, const char *mode );										//写文件时刷新缓存中数据到文件

	char m_xmlFile[512];
	int m_xmlFileFd;

	char m_xmlHeadTitle[132];
	char m_xmlElementTitle[5][132];

	char m_xmlDirName[132];								//日志文件导出的主目录

	char m_xmlFilesPath[512];							//存放xml文件的目录名
	int m_xmlFileNameFd;								

	XML_FORMAT m_xmlFormat;

	ID_TO_STRING				m_pfnIdToString;
	void						*m_pCallbackParam;
	
	BUTTON_TIP_INFO				*m_pButtonTipInfo;
	int							m_tipCount;

	bool                        m_bUseCDBackup;

};

#endif
