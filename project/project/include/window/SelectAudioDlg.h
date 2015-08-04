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

	//设置可以选择的音频通道,初始的音频通道
	void SetChnnAudio(ULONGLONG allAudio, ULONGLONG ckedAudio);
	
	// 默认情况下本对话框需求的大小，必须在设置SetChnnAudio后才能准确获取默认大小
	GUI::CRect GetDlgDefaultRect();

	// OnOK 后，使用此接口获取选取的通道,最多只有一个通道被选择
	//返回0,表示静音没有通道被选择
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
	GUI::CCheckCtrl* m_pCheck;//通道
	GUI::CCheckCtrl	m_ckNone;//静音

	GUI::CButton	m_btnOK;
	GUI::CButton	m_btnExit;

protected:
	ULONGLONG		m_chnnAudio;	//全音频输入通道
	ULONGLONG		m_selChnn;//选择单一个音频通道
	int							m_maxChnn;//全部音频道道数目
};

#endif
