/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2011-11-15
** Name         : ExportHtmlLog.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _EXPORT_HTML_LOG_H_
#define _EXPORT_HTML_LOG_H_
#include "StringTable.h"
#include "MessageMan.h"
#include "IDToStringID.h"
#include "CreateXMLFile.h"

class CExportHtmlLog
{
public:
	CExportHtmlLog();
	~CExportHtmlLog();
	void SetWriteFileInfo(bool bUseCDBackup, char *pHeadTitle, char **ppElementTitle, char *pData, int dataLen, int fileFormat,char desLogPath[]);
	bool ExportLog();
	void SendDirname(char *dirname);//发送xml文件的目录名；
protected:
	static const char * IdToString(unsigned long id, long xmlFormat, void *pParam);
	const char * String(unsigned long id, long xmlFormat);
private:
	//m_pHeadTitle, m_pHeadTitle, m_pData所使用的数据空间在UsbBackupProc.cpp中申请和释放
	char *	m_pHeadTitle;					
	char **	m_ppElementTitle;
	char *	m_pData;
	char m_desLogPath[128];
	int		m_dataLen;
	int		m_fileFormat;
	char m_dirname[128];
	bool m_bUseCDBackup;
};
#endif
