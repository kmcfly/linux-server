/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author			: L
** Date				: 2009-12-28
** Name			: SelectAudioDlg.h 
** Version			: 1.0
** Description	: 
** Modify Record:
1:
***********************************************************************/

#ifndef _SELECT_AUDIO_H_
#define _SELECT_AUDIO_H_

#include "Dialog.h"
#include "CheckCtrl.h"
#include "ListCtrl.h"

using namespace GUI;

class CSelectAudioDlg : public GUI::CDialog
{
public:
	CSelectAudioDlg();
	virtual ~CSelectAudioDlg();

public:
	const unsigned long CHNN_DLG_BORDER;
	const unsigned long	 CHNN_DLG_TOP;
	const unsigned long	 CHNN_DLG_LEFT;

	const unsigned long CHNN_CK_OFFSET_X;
	const unsigned long	 CHNN_CK_OFFSET_Y;

	enum
	{
		CHNN_MAX = 64,
		CHNN_MAX_ROW = 8,
	};

public:

	//���ÿ���ѡ�����Ƶͨ��,��ʼ����Ƶͨ��
	void SetChnnAudio(ULONGLONG allAudio, ULONGLONG ckedAudio);
	
	// Ĭ������±��Ի�������Ĵ�С������������SetChnnAudio�����׼ȷ��ȡĬ�ϴ�С
	GUI::CRect GetDlgDefaultRect();

	// OnOK ��ʹ�ô˽ӿڻ�ȡѡȡ��ͨ��,���ֻ��һ��ͨ����ѡ��
	//����0,��ʾ����û��ͨ����ѡ��
	ULONGLONG GetSelChnn() { return m_selChnn; };

protected:
	void OnInitial();
	void OnDestroy();
	void OnOutOfDlg(unsigned long keyID);
	void OnPaint( PAINT_STATUS status );
	void CheckUI(int chnn,bool bRefresh = false );

protected:
	DECLARE_MSG_MAP()
	unsigned long OnBtnExit();
	unsigned long OnBtnOK();
	unsigned long OnCheckClick(unsigned long ctrlID);
	unsigned long OnCheckNone();

protected:
	GUI::CCheckCtrl* m_pCheck;//ͨ��
	GUI::CCheckCtrl	m_ckNone;//����

	GUI::CButton	m_btnOK;
	GUI::CButton	m_btnExit;

protected:
	ULONGLONG		m_chnnAudio;	//ȫ��Ƶ����ͨ��
	ULONGLONG		m_selChnn;//ѡ��һ����Ƶͨ��
	int							m_maxChnn;//ȫ����Ƶ������Ŀ
};

#endif
