/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :	袁石维
** Date         : 2008-12-06
** Name         : Log.h
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#ifndef _LOG_H_
#define _LOG_H_

#include "PUB_common.h"
#include "mylist.cpp"

typedef struct _log_head_info
{
	unsigned long fileVer;		//文件版本号
	unsigned long headLen;		//头信息结构体长度
	unsigned long indexNum;		//元素数目
	unsigned long index;		//早前段的开始，起物理前一个则是最近段的结束。
}LOG_HEAD_INFO, *P_LOG_HEAD_INFO;

const unsigned long MAX_YEAR_LEN = 30;
const unsigned long MAX_REFRESH_HEAD_NUM = 20;
const unsigned long MAX_READ_LOG_NUM = 500;
const unsigned long LOG_MONTH_DATA_POS = sizeof(LOG_HEAD_INFO) + 32;
const unsigned long LOG_DATA_POS = LOG_MONTH_DATA_POS + sizeof(unsigned long)*MAX_YEAR_LEN*12+32;

const unsigned long LOG_FILE_VER = 0x0000;

class CLog
{
public:
	CLog(unsigned long subLen);
	virtual ~CLog();

	static bool CreateLogFile(const char *pFileName, unsigned long subLen, unsigned long num);

	bool Open (const char *pFileName, bool bReadOnly = false);
	void Close ();

	void GetTime(unsigned long & firstST, unsigned long & firstET, unsigned long & secondST, unsigned long & secondET);
protected:
	virtual bool GetLogTime(unsigned long index, unsigned long & startTime, unsigned long & endTime) = 0;
	virtual bool GetLogTime(unsigned char *pData, unsigned long num , unsigned long index, unsigned long & startTime, unsigned long & endTime) = 0;
	
	//////////////////////////////////////////////////////////////////////////////////////////
	long WriteLog(unsigned char *pData, unsigned long subLen);
	bool ReadLog(unsigned char *pData, unsigned long bufLen);
	bool ReadLog(unsigned char *pData, unsigned long bufLen, unsigned long index);
	unsigned long ReadLog(unsigned char *pData, unsigned long bufLen, unsigned long num, unsigned long index);
	//variable////////////////////////////////////////////////////////////////////////////////
	unsigned long	m_subLen;
	unsigned long	*m_pMonthBit;
	FILE			*m_pFile;
	unsigned long	m_writeNum;

	unsigned long	m_firstST;
	unsigned long	m_firstET;
	unsigned long	m_secondST;
	unsigned long	m_secondET;

	LOG_HEAD_INFO	m_headInfo;
private:
	void RefreshMonthBit(unsigned long startTime, unsigned long endTime);
	void RefreshHead();
};
#endif //_LOG_H_
