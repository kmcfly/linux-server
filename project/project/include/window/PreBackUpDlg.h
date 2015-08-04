/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2011-12-02
** Name         : PreBackUpDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CPREBACKUPDLG_H_
#define _CPREBACKUPDLG_H_

#include "Button.h"
#include "CheckCtrl.h"
#include "ComboCtrl.h"
#include "Dialog.h"
#include "ListCtrl.h"
#include "MessageMan.h"
#include "BackUpShowInfoDlg.h"
#include "PictureCtrl.h"
using namespace GUI;


typedef struct _backup__data_seq
{	
	FILE_INFO_EX_BACKUP backupinfo;
	unsigned short      hasCheck; 
	unsigned short      hasCalculate;//�Ƿ񱻹�ѡ��û�еĻ������͸���һ��ҳ��
	unsigned short      hasAudio;
	unsigned long       seq;
	unsigned long       size;
	unsigned long       changeTime;
	unsigned long       res[4];
}BACKUP_DATA_SEQ;
typedef vector<BACKUP_DATA_SEQ> BACKUP_DATA_SEQ_VECTOR;

class CPreBackUpDlg : public GUI::CDialog
{
public:
	CPreBackUpDlg();
	~CPreBackUpDlg();
	void SetSEARCHData(BACKUP_DATA_SEQ_VECTOR *pBackupData);//��������������,Ҫ��OnInitialǰ����
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();
	unsigned long OnOK();//����
	unsigned long OnCancel();//�������˳�
	unsigned long OnApply();//ɾ������
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnCheckAll();
	void EndDialog(DLG_END_VALUE result);
private:
	BACKUP_DATA_SEQ_VECTOR  *m_pBackupData;
	GUI::CListCtrl m_preBackUpList;
	
	enum LIST_COL
	{
		COL_CHECK,
		COL_CHNN,
		COL_SEQ,
		COL_STARTTIME,
		COL_ENDTIME,
		COL_AUDIO,
		COL_EDIT,//��Ƶ�༭
		COL_SIZE,
		COL_KEEP,
	};

	int m_line;//��������
	GUI::CCheckCtrl m_checkAll;

private:
	void CreateCtrl();
	void WriteCutFile();
};
#endif

