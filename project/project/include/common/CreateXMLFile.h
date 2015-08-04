/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2010-04-21
** Name         : CreateXMLFile.h
** Version      : 1.0
** Description  : 
					1��������������XML�ļ���Ŀǰ���Բ�������XML�ļ����ֱ�ΪXML_FORMAT_LOG��XML_FORMAT_EVENT���͵�XML�ļ���
					2��������XML�ļ���������ҳ����ʾ���������Ϣ��
					3��ÿ��XML�ļ��м�¼100���������xmlFileName.xml�д��xml���͵���־�ļ�����
					4���������xml�ļ��󿽱�tableCtrl.html��logĿ¼��
					
					5����־Ŀ¼�ṹ��event��Ϣ������Ŀ¼�ṹ����־�Ľṹ����
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

const char _XMLdeclare_[]="<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n";					//XML�ļ�ͷ

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
		XML_BACKUP_EVENT=2,//����ʱ����¼���zxx
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

	bool AddLogTitleInfo(const char *pHeadTitle, const char **ppElementTitle);			//�����־ͷ��Ϣ
	
	bool AddLogElement(const OPERATOR_LOG *pInfor);										//���һ����־xmlԪ��

	bool AddEventTitleInfo(const char *pHeadTitle, const char **ppElementTitle);		//����¼�ͷ��Ϣ
	bool AddEventElement(const EVENT_LOG *pInfor);										//���һ���¼�xmlԪ��
	
	//�ɵ�ֻ�����4��Ԫ�أ�������Ҫ��Ӹ�����(ĿǰΪ5��������Ҫ�޸�./WebServer/backup_event_list_ctrl.html)
	bool AddBackupEventTitleInfo(const char *pHeadTitle, const char **ppElementTitle,int count);
	bool AddBackupEventElement(const EVENT_LOG *pInfor,int Elementcount);

	bool AddItem(const char *pMark, const char *pInfor, int fd=-1);						//���һ��xml��ǩ
	bool WriteStartMark(const char *pMark, bool bEnd=false, int fd=-1);					//дXML�ļ��еĿ�ʼ��ǩ
	bool WriteInforMark(const char *pInfor, int fd=-1);									//д����
	bool WriteEndMark(const char *pMark, bool bEnd=false,int fd=-1);					//д������ǩ

	bool DeleteFiles();
#ifdef __ENVIRONMENT_WIN32__
	bool DeleteDirectory(char *dirName);
#endif

	bool FileFlush( int des, const char *mode );										//д�ļ�ʱˢ�»��������ݵ��ļ�

	char m_xmlFile[512];
	int m_xmlFileFd;

	char m_xmlHeadTitle[132];
	char m_xmlElementTitle[5][132];

	char m_xmlDirName[132];								//��־�ļ���������Ŀ¼

	char m_xmlFilesPath[512];							//���xml�ļ���Ŀ¼��
	int m_xmlFileNameFd;								

	XML_FORMAT m_xmlFormat;

	ID_TO_STRING				m_pfnIdToString;
	void						*m_pCallbackParam;
	
	BUTTON_TIP_INFO				*m_pButtonTipInfo;
	int							m_tipCount;

	bool                        m_bUseCDBackup;

};

#endif
