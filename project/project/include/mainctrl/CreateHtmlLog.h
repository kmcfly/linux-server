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
	bool CreateHtmlAction();//实际把操作、时间日志和html文件写下去
	vector<std::string> * GetAllAviFileName();
	void Quit();
	void SetLinkPath(char *plinkPath);
private:
	CCreateHtmlLog();
	//建立html文件的table节点
	//参数：
	//ptable--挂接ptable节点
	//ElementName--ptable左侧的字符串
	//ElementResult--ptable右侧的字符串
	//alignType--每行的对齐属性，只能是left,right,center取1种
	//bool blink--是否有链接属性：目前只有事件日志和操作日志具有该属性
	//char *path--链接文件路径
	void CreateNode(TiXmlElement *ptable,const char *ElementName,const char *ElementResult,char *alignType,bool bcolspan=false,bool blink=false,char *path=NULL,char *color=NULL);
	//获取文件名：如00000001.dat
	void GetFileName(char *filename,int fileNum);

	bool WriteHtmlLog(char *logname, bool bCreateOperLog, bool bCreateEventLog);//把获得的信息写入Html文件

	bool WriteOperationLoginfo();//写操作日志的信息
	bool WriteEventLoginfo();
	
	bool ExportOperationLog(OPERATOR_LOG *pLog,int LogNum);//写操作日志
	bool ExportEventLog(EVENT_LOG *pLog,int LogNum);//写事件日志
	//获得文件的大小
	unsigned long get_file_size(const char *filename);

	ULONGLONG CH();
	int LinkHtmlFile(char *pSrcFilePath,char *pDesPath);//给日志建立软连接
	
	char m_mac[64];//mac地址
	BACKUP_INITIAL_INFO *m_pBackupInitialInfo;//获取的备份信息
	char m_desDataPath[128];//获取的目标数据备份地址
	char m_operdirname[128];
	char m_eventdirname[128];
	
	bool m_bcdbackup;//光盘备份标志

	unsigned long m_backupStartTime;
	unsigned long m_backupEndTime;
	unsigned long m_allTimeOfFiles;

	static CCreateHtmlLog* m_pHtmlLog;

	vector<std::string> m_aviNameVec;//avi的文件名存放容器
	
	char *m_linkpath;

	bool m_bCreateOperLog;
	bool m_bCreatEventLog;
	char m_szUsrName[36];
};
#endif
