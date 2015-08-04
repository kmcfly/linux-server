/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2011-11-16
** Name         : CreateHtmlLog.h
** Version      : 1.0
** Description  : 
** Modify Record:
***********************************************************************/

#ifndef _CREATE_HTML_LOG_H_
#define  _CREATE_HTML_LOG_H_
#include "BackupProc.h"
#include "ConfigBlock.h"
#include "ConfigEx.h"
#include "StringTable.h"
#include "NetProtocol.h"
#include "MessageMan.h"
#include "ExportHtmlLog.h"
#include "tinyxml.h"
#include "tinystr.h"
#include <string>
#include <vector>
#include <set>
#include "LocalDevice.h"
#include "device.h"
#ifdef __ENVIRONMENT_LINUX__
#include "CDOperationMan.h"
#endif
using namespace std;



class CCreateHtmlLog
{
public:
	~CCreateHtmlLog();
	
	static CCreateHtmlLog * Instance();
	void Initial(bool bcdbackup, BACKUP_INITIAL_INFO *pBackupInitialInfo, char desDataPath[]);
	bool WirteHtmlLogAction();
	bool CreateHtmlAction();//ʵ�ʰѲ�����ʱ����־��html�ļ�д��ȥ
	vector<std::string> * GetAllAviFileName();
	void Quit();
	void SetLinkPath(char *plinkPath);
private:
	CCreateHtmlLog();
	//����html�ļ���table�ڵ�
	//������
	//ptable--�ҽ�ptable�ڵ�
	//ElementName--ptable�����ַ���
	//ElementResult--ptable�Ҳ���ַ���
	//alignType--ÿ�еĶ������ԣ�ֻ����left,right,centerȡ1��
	//bool blink--�Ƿ����������ԣ�Ŀǰֻ���¼���־�Ͳ�����־���и�����
	//char *path--�����ļ�·��
	void CreateNode(TiXmlElement *ptable,const char *ElementName,const char *ElementResult,char *alignType,bool bcolspan=false,bool blink=false,char *path=NULL,char *color=NULL);
	//��ȡ�ļ�������00000001.dat
	void GetFileName(char *filename,int fileNum);

	bool WriteHtmlLog(char *logname, bool bCreateOperLog, bool bCreateEventLog);//�ѻ�õ���Ϣд��Html�ļ�

	bool WriteOperationLoginfo();//д������־����Ϣ
	bool WriteEventLoginfo();
	
	bool ExportOperationLog(OPERATOR_LOG *pLog,int LogNum);//д������־
	bool ExportEventLog(EVENT_LOG *pLog,int LogNum);//д�¼���־
	//����ļ��Ĵ�С
	unsigned long get_file_size(const char *filename);

	ULONGLONG CH();
	int LinkHtmlFile(char *pSrcFilePath,char *pDesPath);//����־����������
	
	char m_mac[64];//mac��ַ
	BACKUP_INITIAL_INFO *m_pBackupInitialInfo;//��ȡ�ı�����Ϣ
	char m_desDataPath[128];//��ȡ��Ŀ�����ݱ��ݵ�ַ
	char m_operdirname[128];
	char m_eventdirname[128];
	
	bool m_bcdbackup;//���̱��ݱ�־

	unsigned long m_backupStartTime;
	unsigned long m_backupEndTime;
	unsigned long m_allTimeOfFiles;

	static CCreateHtmlLog* m_pHtmlLog;

	vector<std::string> m_aviNameVec;//avi���ļ����������
	
	char *m_linkpath;

	bool m_bCreateOperLog;
	bool m_bCreatEventLog;
	char m_szUsrName[36];
};
#endif
