/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2010-02-02
** Name         : DebugSetupDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "DebugSetupDlg.h"
#include "DebugProc.h"
//
CDebugSetupDlg::CDebugSetupDlg()
{

}

CDebugSetupDlg::~CDebugSetupDlg()
{

}

void CDebugSetupDlg::OnInitial()
{
	GUI::CDialog::OnInitial();

	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption("DEBUG SETUP", false);

	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY,m_pStrTable->String(E_STR_ID_APPLY));
	SetDefaultBtnCaption( DEFAULT_BTN_OK,m_pStrTable->String(E_STR_ID_CLEAR_ALL));
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP]+40;
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_length[LIST_HEAD_HEIGHT] + m_length[LIST_ITEM_HEIGHT] * 4;

	m_stcType.Create(CTRLID_STATIC, x, y, m_rect.Width() - x, m_length[STATIC_HEIGHT], this, "Type"); 
	x += (width/3);
	m_stcTime.Create(CTRLID_STATIC, x, y, m_rect.Width() - x, m_length[STATIC_HEIGHT], this, "Time[s]"); 
	x += (width/3);
	m_stcNumber.Create(CTRLID_STATIC, x, y, m_rect.Width() - x, m_length[STATIC_HEIGHT], this, "Times"); 

	//
	x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP]+70;
	m_ckLive.Create(GetRandWndID(), x, y, (width/3)-m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT], this, 0, 1);
	m_ckLive.SetCaption("Live", false, false);

	y += 30;
	m_ckRecord.Create(GetRandWndID(), x, y, (width/3)-m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT], this, 0, 2);
	m_ckRecord.SetCaption("Record", false, false);

	y += 30;
	m_ckSearch.Create(GetRandWndID(), x, y, (width/3)-m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT], this, 0, 3);
	m_ckSearch.SetCaption("Search", false, false);

	y += 30;
	m_ckPlay.Create(GetRandWndID(), x, y, (width/3)-m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT], this, 0, 4);
	m_ckPlay.SetCaption("Playback", false, false);

	y += 30;
	m_ckInformation.Create(GetRandWndID(), x, y, (width/3)-m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT], this, 0, 5);
	m_ckInformation.SetCaption("Information", false, false);

	//
	x = m_length[DLG_OFFSET_SUB_LEFT] + (width/3), y = m_length[DLG_OFFSET_SUB_TOP]+70;
	m_neTimeLive.Create(GUI::CWnd::GetRandWndID(), x, y, (width/3)-10, m_length[EDIT_HEIGHT], this, 1, 1);

	y += 30;
	m_neTimeRecord.Create(GUI::CWnd::GetRandWndID(), x, y, (width/3)-10, m_length[EDIT_HEIGHT], this, 1, 2);

	y += 30;
	m_neTimeSearch.Create(GUI::CWnd::GetRandWndID(), x, y, (width/3)-10, m_length[EDIT_HEIGHT], this, 1, 3);

	y += 30;
	m_neTimePlay.Create(GUI::CWnd::GetRandWndID(), x, y, (width/3)-10, m_length[EDIT_HEIGHT], this, 1, 4);

	y += 30;
	m_neTimeInfo.Create(GUI::CWnd::GetRandWndID(), x, y, (width/3)-10, m_length[EDIT_HEIGHT], this, 1, 5);

	//
	x = m_length[DLG_OFFSET_SUB_LEFT] + (width/3) + (width/3), y = m_length[DLG_OFFSET_SUB_TOP]+70;
	m_neNumLive.Create(GUI::CWnd::GetRandWndID(), x, y, (width/3), m_length[EDIT_HEIGHT], this, 2, 1);

	y += 30;
	m_neNumRecord.Create(GUI::CWnd::GetRandWndID(), x, y, (width/3), m_length[EDIT_HEIGHT], this, 2, 2);

	y += 30;
	m_neNumSearch.Create(GUI::CWnd::GetRandWndID(), x, y, (width/3), m_length[EDIT_HEIGHT], this, 2, 3);

	y += 30;
	m_neNumPlay.Create(GUI::CWnd::GetRandWndID(), x, y, (width/3), m_length[EDIT_HEIGHT], this, 2, 4);

	y += 30;
	m_neNumInfo.Create(GUI::CWnd::GetRandWndID(), x, y, (width/3), m_length[EDIT_HEIGHT], this, 2, 5);

}

//////////////////////////////////////////////////////////////////////////
unsigned long CDebugSetupDlg::OnApply()
{
	CDebugProc *pDebug = CDebugProc::Instance();

	pDebug->SetDebug(CDebugProc::DEBUG_LIVE, m_ckLive.IsCheck(), m_neTimeLive.GetValue(), m_neNumLive.GetValue());
	pDebug->SetDebug(CDebugProc::DEBUG_RECORD, m_ckRecord.IsCheck(), m_neTimeRecord.GetValue(), m_neNumRecord.GetValue());
	pDebug->SetDebug(CDebugProc::DEBUG_SEARCH, m_ckSearch.IsCheck(), m_neTimeSearch.GetValue(), m_neNumSearch.GetValue());
	pDebug->SetDebug(CDebugProc::DEBUG_PLAYBACK, m_ckPlay.IsCheck(), m_neTimePlay.GetValue(), m_neNumPlay.GetValue());
	pDebug->SetDebug(CDebugProc::DEBUG_INFORMATION, m_ckInformation.IsCheck(), m_neTimeInfo.GetValue(), m_neNumInfo.GetValue());

	GUI::CDialog::EndDialog(DLG_ESC);

	pDebug->Runing(true);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDebugSetupDlg::OnOK()
{
	m_ckLive.SetCheck(false, true);
	m_ckRecord.SetCheck(false, true);
	m_ckSearch.SetCheck(false, true);
	m_ckPlay.SetCheck(false, true);
	m_ckInformation.SetCheck(false, true);

	return KEY_VIRTUAL_MSG_STOP;
}
//end
