/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2011-12-02
** Name         : CutKeepLog.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CUT_KEEP_LOG_H_
#define _CUT_KEEP_LOG_H_
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "PreBackUpDlg.h"
#include "PUB_common.h"
using namespace std;

#define CUT_BACKUP_INFO_PATH  "/mnt/mtd/cutKeeplog.log"
#define CUT_BACKUP_INFO_VER  0x0001
#define CUT_BACKUP_INFO_MAX_NUM 100
typedef struct _cut_backup_head_info_
{
	unsigned long version;
	unsigned long filenum;
}CUT_BACKUP_HEAD_INFO;

class CCutKeepLog//���м�¼������
{
public:

	CCutKeepLog();

	~CCutKeepLog();

	bool OpenKeepFile();

	bool WriteKeepFile(BACKUP_DATA_SEQ_VECTOR *pLogExVec);

	bool ReadKeepFile(BACKUP_DATA_SEQ_VECTOR *pLogExVec);

	//bool cmplogfile(BACKUP_DATA_SEQ_VECTOR *pLogExVec);//�Ƚϲ�ɾ���ļ�
	
	void Dellogfile();//ɾ���ļ�

	void Quit();	
private:
	FILE * m_pFile;	
};

#endif

