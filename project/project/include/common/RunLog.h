/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2010-01-19
** Name         : RunLog.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _RUN_LOG_H_
#define _RUN_LOG_H_

#include <iostream>
#include <fstream>
#ifdef __ENVIRONMENT_WIN32__
#include <string>
#else
#include <string.h>
#endif
#include <stdio.h>
using namespace std;

#include "PUB_common.h"
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#undef RUN_LOG

#ifdef  NDEBUG
#define RUN_LOG(itemID, RUN_Func1, RUN_Func2, Information) ((void)0)
#else
#define RUN_LOG(itemID, RUN_Func1, RUN_Func2, Information)\
{\
	CRunLog::m_lock.Lock();\
	strcpy(CRunLog::m_buf, Information);\
	strcat(CRunLog::m_buf, "\r\n");\
	(RUN_Func1);\
	(RUN_Func2);\
	strcat(CRunLog::m_buf, "\t\t");\
	CRunLog::Instance()->WriteLog(itemID, CRunLog::m_buf, __FILE__, __LINE__);\
	printf(Information);\
	printf("\n");\
	CRunLog::m_lock.UnLock();\
}
#endif
//////////////////////////////////////////////////////////////////////////
const unsigned long	RUN_LOG_MAX_LOG_NUM		= 100;	//ÿ���ļ���¼�������
const unsigned long	RUN_LOG_MAX_FILE_NUM	= 100;	//�ļ����ݵ������

const char LOG_FILE_XML_HEADER []			= "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
const char RUN_LOG_TAG_HEAD []				= "<Runlog>";
const char RUN_LOG_TAG_END []				= "</Runlog>";


const char RUN_LOG_FILE_INFO_TAG_HEAD []	= "<FileInfo>";
const char RUN_LOG_FILE_INFO_TAG_END []		= "</FileInfo>";
const char RUN_LOG_NUM_TAG_HEAD []			= "<Num>";
const char RUN_LOG_NUM_TAG_END []			= "</Num>";
const char RUN_LOG_START_TIME_TAG_HEAD []	= "<StartTime>";
const char RUN_LOG_START_TIME_TAG_END []	= "</StartTime>";
const char RUN_LOG_END_TIME_TAG_HEAD []		= "<EndTime>";
const char RUN_LOG_END_TIME_TAG_END []		= "</EndTime>";

const char RUN_LOG_LOG_TAG_HEAD []			= "<Log>";
const char RUN_LOG_LOG_TAG_END []			= "</Log>";
const char RUN_LOG_TYPE_TAG_HEAD []			= "<Type>";
const char RUN_LOG_TYPE_TAG_END []			= "</Type>";
const char RUN_LOG_ID_TAG_HEAD []			= "<ID>";
const char RUN_LOG_ID_TAG_END []			= "</ID>";
const char RUN_LOG_INFO_TAG_HEAD []			= "<Information>";
const char RUN_LOG_INFO_TAG_END []			= "</Information>";
const char RUN_LOG_TIME_TAG_HEAD []			= "<Time>";
const char RUN_LOG_TIME_TAG_END []			= "</Time>";
const char RUN_LOG_FILE_TAG_HEAD []			= "<File>";
const char RUN_LOG_FILE_TAG_END []			= "</File>";
const char RUN_LOG_LINE_TAG_HEAD []			= "<Line>";
const char RUN_LOG_LINE_TAG_END []			= "</Line>";
//////////////////////////////////////////////////////////////////////////
class CRunLog
{
public:
	static CRunLog * Instance();
	~CRunLog();
    
	bool Initial(DISK_LOG_INFO *pDiskStatus, unsigned char diskNum, const char *pFilePath);	
	void Quit();
	//////////////////////////////////////////////////////////////////////////
	bool WriteLog(unsigned long itemID, const char *pInfo, const char *pFile, unsigned long line);
	//��ȡ����ʹ�õķ������롣lowMask���ڱ�ʾ��1������64�������������heightMask���ڱ�ʾ��65������128�����������
	void GetUsingPartitionMask(ULONGLONG &heightMask, ULONGLONG &lowMask);

	//////////////////////////////////////////////////////////////////////////
	static CPUB_Lock m_lock;
	static char m_buf [2048];
	static void RUN_Time(const char *pStr, unsigned long t);
	static void RUN_Num(const char *pStr, unsigned long num);
protected:
private:
	CRunLog();
	bool Initial(const char *pFilePath, unsigned char diskNum);
	//////////////////////////////////////////////////////////////////////////
	bool GetFileInfo(fstream & file, unsigned long & logNum, unsigned long & startTime, unsigned long & endTime);
	void WriteFileInfo(fstream & file, unsigned long logNum, unsigned long startTime, unsigned long endTime);
	bool EraseTag(string & str);
	unsigned long Time(string & str);
	//////////////////////////////////////////////////////////////////////////
	char				*m_pFilePath;
	long				m_fileInfoPos;
	long				m_logTagEndPos;

	//���ڷ�����ע�������ֻ���Ͷ�д֮��.ֻ������д��־
	//m_diskNum��ʾ����д��־�ķ�����.
	//m_pVailDisk[i] ��ʾ�±�Ϊi��λ���ϴ�ŵ�һ����ֵ���������ֵ��һ������д��־�ķ���
	//m_diskIndex��ʾ���ǵ�ǰʹ�õ��±�
	unsigned long       *m_pVaildDisk;  
	unsigned long		m_diskNum;
	unsigned long		m_diskIndex;    
	unsigned long		m_fileIndex;
	

	unsigned long		m_logNum;
	unsigned long		m_startTime;
	unsigned long		m_endTime;

	fstream				m_file;

	
};
#endif //_RUN_LOG_H_

