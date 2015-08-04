/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2011-12-06
** Name         : PreBackupAudioInfo.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CPREBACKUP_GETAUDIO_H_
#define _CPREBACKUP_GETAUDIO_H_

#if defined(__PRIMARY_WRDISK__)
#include "PrimaryRecReader.h"
#else
#include "RecReader.h"
#endif

class CpreBackupAudioInfo
{
public:
	CpreBackupAudioInfo();
	~CpreBackupAudioInfo();
	void Initial();
	void Quit();
	bool OpenAndCheckFile(unsigned long diskindex,unsigned long fileindex, unsigned long startTime, unsigned long endTime);
	void CloseFile();
	bool CheckfileAudio();//测试备份文件是不是带有音频
	unsigned long FileSize(unsigned long startTime, unsigned long endTime);
private:
	int RECTell(unsigned long time, unsigned long *pOffSet);

#if defined(__PRIMARY_WRDISK__)
	CPrimaryRecReader m_RecReader;
#else
	CRecReader m_RecReader;
#endif
	bool m_bOpen;
};
#endif
