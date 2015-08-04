/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-01
** Name         : SearchManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "SearchManDlg.h"
#include "NetProtocol.h"
#include "FuncCustom.h"
#include "PlaybackDlg.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

//////////////////////////////////////////////////////////////////////////
CSearchManDlg::CSearchManDlg()
{
	m_diskOwnerIndex = 0;
	m_channelNum = 0;
	m_bUnfold = true;
	m_bFlexOut = false;//
}

CSearchManDlg::~CSearchManDlg()
{

}

void CSearchManDlg::SetInitialInfo(unsigned long diskOwnerIndex, unsigned long channelNum)
{
	m_diskOwnerIndex = diskOwnerIndex;
	m_channelNum = channelNum;	

	ULONGLONG authCH = (g_p_login_succ->productInfo.bPasswordCheck) ? g_p_login_succ->authPlaybackCH : ALL_CHNN_MASK(m_channelNum);
#if defined(__GROUY_SEARCH__)
	{
		if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768)
		{
			m_searchGroupConditionDlg.SetInitialInfo(m_channelNum, authCH);
			m_searchGroupTimeDlg.SetInitialInfo(m_channelNum, false,authCH);
		}
		else
		{
			m_searchGroupScrollConditionDlg.SetInitialInfo(m_channelNum, authCH);
			m_searchGroupScrollTimeDlg.SetInitialInfo(m_channelNum,false,authCH);
		}
	}
#else
	{
		if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 || m_channelNum <= 16)
		{
			m_searchConditionDlg.SetInitialInfo(m_channelNum, authCH);
			m_searchCdn2TimtDlg.SetInitialInfo(m_channelNum, authCH);
		}
		else
		{
			m_searchCondition2Dlg.SetInitialInfo(m_channelNum, authCH);
			m_searchCdnTimtDlg.SetInitialInfo(m_channelNum, authCH);
		}
	}
#endif
	m_timeSearchDlg.SetInitialInfo(m_channelNum, g_p_login_succ->productInfo.videoOuputNum, m_diskOwnerIndex, authCH);
	m_eventSearchDlg.SetInitialInfo(m_channelNum);
	m_fileManDlg.SetInitialInfo(m_channelNum, diskOwnerIndex);
	m_pictureSearchDlg.SetInitialInfo(m_channelNum, diskOwnerIndex);
}

//////////////////////////////////////////////////////////////////////////
void CSearchManDlg::OnInitial()
{
	assert(m_channelNum > 0);

	GUI::CDialog::OnInitial();
#ifdef __TD2316SES_SPEC__
	//尝试调用WaitProc
	m_pWait=CFBInterface::Instance();
	m_pWait->WaitProc(true);
#endif
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SEARCH), false);
	//////////////////////////////////////////////////////////////////////////

	if( GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768)
	{
		CreateUnfoldCtrl();
	}
	else
	{
		CreateFoldCtrl();
	}
    
	SetCalendarInfo();
}

void CSearchManDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();
#ifdef __TD2316SES_SPEC__	
	m_pWait->WaitProc(false);
#endif
}

void CSearchManDlg::CreateFoldCtrl()
{
#if defined(__GROUY_SEARCH__)
	{
		m_searchConditionDlgWidth = m_length[CALENDAR_WIDTH]+30;
	}
#else
	{
		if (m_channelNum > 16)
		{
			m_searchConditionDlgWidth = m_length[CALENDAR_WIDTH]+30;
		}
		else
		{
			m_searchConditionDlgWidth =m_length[CALENDAR_WIDTH]+10;
		}
	}
#endif
	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT]+m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short height = m_rect.Height() - y - 2 * m_length[DLG_OFFSET_BOTTOM];
	int tab_dlg_w = m_rect.Width()-2*m_length[DLG_OFFSET_LEFT];

	m_timeSearchDlg.Create(GUI::CWnd::GetRandWndID(), x, y, tab_dlg_w, height, this, 0, 2);
	m_eventSearchDlg.Create(GUI::CWnd::GetRandWndID(), x, y, tab_dlg_w, height, this, 0, 3);
	m_fileManDlg.Create(GUI::CWnd::GetRandWndID(), x, y, tab_dlg_w, height, this, 0, 4);
	if (IsSupportSnapPic()/* && (g_p_login_succ->productInfo.localVideoInputNum > 0)*/)
	{
		m_pictureSearchDlg.Create(GUI::CWnd::GetRandWndID(), x, y, tab_dlg_w, height, this, 0, 5);
	}

#if defined(__GROUY_SEARCH__)
	m_searchGroupScrollConditionDlg.Create(GUI::CWnd::GetRandWndID(), m_rect.Width()- m_searchConditionDlgWidth - m_length[DLG_OFFSET_LEFT], y, m_searchConditionDlgWidth, height, this, 1, 2);
	m_searchGroupScrollTimeDlg.Create(GUI::CWnd::GetRandWndID(), m_rect.Width() - m_searchConditionDlgWidth - m_length[DLG_OFFSET_LEFT], y, m_searchConditionDlgWidth, height, this, 1, 3);
	m_searchGroupScrollTimeDlg.Show(false, false);
#else
	if(m_channelNum > 16)
	{
		m_searchCondition2Dlg.Create(GUI::CWnd::GetRandWndID(), m_rect.Width()- m_searchConditionDlgWidth - m_length[DLG_OFFSET_LEFT], y, m_searchConditionDlgWidth, height, this, 1, 2);
		m_searchCdnTimtDlg.Create(GUI::CWnd::GetRandWndID(), m_rect.Width() - m_searchConditionDlgWidth - m_length[DLG_OFFSET_LEFT], y, m_searchConditionDlgWidth, height, this, 1, 3);
		m_searchCdnTimtDlg.Show(false, false);
	}
	else
	{
		m_searchConditionDlg.Create(GUI::CWnd::GetRandWndID(), m_rect.Width()- m_searchConditionDlgWidth - m_length[DLG_OFFSET_LEFT], y, m_searchConditionDlgWidth, height, this, 1, 2);
		m_searchCdn2TimtDlg.Create(GUI::CWnd::GetRandWndID(), m_rect.Width() - m_searchConditionDlgWidth - m_length[DLG_OFFSET_LEFT], y, m_searchConditionDlgWidth, height, this, 1, 3);
		m_searchCdn2TimtDlg.Show(false, false);
	}
#endif
	
	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT]+m_length[DLG_OFFSET_TOP], tab_dlg_w - m_length[BTN_WIDTH], m_length[TABLE_HEIGHT], this, 0, 1);
	m_tabCtrl.AddItem(&m_timeSearchDlg, m_pStrTable->String(E_STR_ID_TIME_SEARCH));
	m_tabCtrl.AddItem(&m_eventSearchDlg, m_pStrTable->String(E_STR_ID_EVENT_SEARCH));
	m_tabCtrl.AddItem(&m_fileManDlg, m_pStrTable->String(E_STR_ID_FILE_MANAGER));
	if (IsSupportSnapPic()/* && (g_p_login_succ->productInfo.localVideoInputNum > 0)*/)
	{
		m_tabCtrl.AddItem(&m_pictureSearchDlg, m_pStrTable->String(E_STR_ID_SEARCH_IMAGES));
	}

	m_tabCtrl.SetCurItem(0, false);

	m_btnSearch.Create(GUI::CWnd::GetRandWndID(), m_rect.Width()-m_length[BTN_WIDTH]-m_length[DLG_OFFSET_LEFT], y - m_length[TABLE_HEIGHT], m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 1, 1, m_pStrTable->String(E_STR_ID_SEARCH));

	m_btnFlex.Create(GUI::CWnd::GetRandWndID(), m_rect.Width()-m_searchConditionDlgWidth-m_length[FLEX_BTN_WIDTH]-m_length[DLG_OFFSET_LEFT], m_rect.Height()/2-(m_length[FLEX_BTN_HEIGHT]/2), m_length[FLEX_BTN_WIDTH], m_length[FLEX_BTN_HEIGHT], this, 1, 2);
	m_bFlexOut = false;//
	tab_dlg_w = m_rect.Width()-m_searchConditionDlgWidth-m_length[FLEX_BTN_WIDTH]- 2 * m_length[DLG_OFFSET_LEFT];
	m_timeSearchDlg.SetWndSize(false, false, tab_dlg_w, height);
	m_eventSearchDlg.SetWndSize(false, false, tab_dlg_w, height);
	m_fileManDlg.SetWndSize(false, false, tab_dlg_w, height);
	m_pictureSearchDlg.SetWndSize(false, false, tab_dlg_w, height);
	m_tabCtrl.SetWndSize(false, false, tab_dlg_w + m_searchConditionDlgWidth - m_length[BTN_WIDTH], m_length[TABLE_HEIGHT]);

	m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_R, false);
	m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_R_F, false);

	m_bFlexSearchConditionDlg = false;
	m_bUnfold = false;

//	if(g_p_login_succ->productInfo.localVideoInputNum > 0)
	{
		tm tmTime = DVRTime32ToTm(GetCurrTime32());
		tmTime.tm_hour = 0;
		tmTime.tm_min  = 0;
		tmTime.tm_sec  = 0;
#if defined(__GROUY_SEARCH__)
		m_searchGroupScrollTimeDlg.SetStartTime(tmTime);
#else
		if (m_channelNum > 16)
		{
			m_searchCdnTimtDlg.SetStartTime(tmTime);
		}
		else
		{
			m_searchCdn2TimtDlg.SetStartTime(tmTime);
		}
#endif
		tmTime.tm_hour = 23;
		tmTime.tm_min  = 59;
		tmTime.tm_sec  = 59;
#if defined(__GROUY_SEARCH__)
		m_searchGroupScrollTimeDlg.SetEndTime(tmTime);
#else
		if (m_channelNum > 16)
		{
			m_searchCdnTimtDlg.SetEndTime(tmTime);
		}
		else
		{
			m_searchCdn2TimtDlg.SetEndTime(tmTime);
		}
#endif
	}	
}

void CSearchManDlg::CreateUnfoldCtrl()
{
	m_searchConditionDlgWidth = m_length[CALENDAR_WIDTH]+10;
	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT]+m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short height = m_rect.Height() - y - 2 * m_length[DLG_OFFSET_BOTTOM];
	int tab_dlg_w = m_rect.Width()-2*m_length[DLG_OFFSET_LEFT] - m_searchConditionDlgWidth - 4;

	m_timeSearchDlg.Create(GUI::CWnd::GetRandWndID(), x, y, tab_dlg_w, height, this, 0, 2);
	m_eventSearchDlg.Create(GUI::CWnd::GetRandWndID(), x, y, tab_dlg_w, height, this, 0, 3);
	m_fileManDlg.Create(GUI::CWnd::GetRandWndID(), x, y, tab_dlg_w, height, this, 0, 4);
	if (IsSupportSnapPic()/* && (g_p_login_succ->productInfo.localVideoInputNum > 0)*/)
	{
		m_pictureSearchDlg.Create(GUI::CWnd::GetRandWndID(), x, y, tab_dlg_w, height, this, 0, 5);
	}
#if defined(__GROUY_SEARCH__)
	m_searchGroupConditionDlg.Create(GUI::CWnd::GetRandWndID(), m_rect.Width()- m_searchConditionDlgWidth - m_length[DLG_OFFSET_LEFT], y, m_searchConditionDlgWidth, height, this, 1, 2);
	m_searchGroupTimeDlg.Create(GUI::CWnd::GetRandWndID(), m_rect.Width() - m_searchConditionDlgWidth - m_length[DLG_OFFSET_LEFT], y, m_searchConditionDlgWidth, height, this, 1, 3);
	m_searchGroupTimeDlg.Show(false, false);
#else
	m_searchConditionDlg.Create(GUI::CWnd::GetRandWndID(), m_rect.Width()- m_searchConditionDlgWidth - m_length[DLG_OFFSET_LEFT], y, m_searchConditionDlgWidth, height, this, 1, 2);
	if(g_p_login_succ->productInfo.videoInputNum > 0)
	{
		m_searchCdn2TimtDlg.Create(GUI::CWnd::GetRandWndID(), m_rect.Width() - m_searchConditionDlgWidth - m_length[DLG_OFFSET_LEFT], y, m_searchConditionDlgWidth, height, this, 1, 3);
		m_searchCdn2TimtDlg.Show(false, false);
	}
#endif
	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT] - m_length[BTN_WIDTH], m_length[TABLE_HEIGHT], this, 0, 1);
	m_tabCtrl.AddItem(&m_timeSearchDlg, m_pStrTable->String(E_STR_ID_TIME_SEARCH));
	m_tabCtrl.AddItem(&m_eventSearchDlg, m_pStrTable->String(E_STR_ID_EVENT_SEARCH));
	m_tabCtrl.AddItem(&m_fileManDlg, m_pStrTable->String(E_STR_ID_FILE_MANAGER));
	if (IsSupportSnapPic()/* && (g_p_login_succ->productInfo.localVideoInputNum > 0)*/)
	{
		m_tabCtrl.AddItem(&m_pictureSearchDlg, m_pStrTable->String(E_STR_ID_SEARCH_IMAGES));
	}

	m_tabCtrl.SetCurItem(0, false);

	m_btnSearch.Create(GUI::CWnd::GetRandWndID(), m_rect.Width()-m_length[BTN_WIDTH]-m_length[DLG_OFFSET_LEFT], y - m_length[TABLE_HEIGHT], m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 1, 1, m_pStrTable->String(E_STR_ID_SEARCH));

	m_bFlexSearchConditionDlg = false;
	m_bUnfold = true;

	tm tmTime = DVRTime32ToTm(GetCurrTime32());
	tmTime.tm_hour = 0;
	tmTime.tm_min  = 0;
	tmTime.tm_sec  = 0;
#if defined(__GROUY_SEARCH__)
	m_searchGroupTimeDlg.SetStartTime(tmTime);
#else
	m_searchCdn2TimtDlg.SetStartTime(tmTime);
#endif

	tmTime.tm_hour = 23;
	tmTime.tm_min  = 59;
	tmTime.tm_sec  = 59;
#if defined(__GROUY_SEARCH__)
	m_searchGroupTimeDlg.SetEndTime(tmTime);
#else
	m_searchCdn2TimtDlg.SetEndTime(tmTime);
#endif
}

unsigned long CSearchManDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		PAGE_ITEM curItem = static_cast<PAGE_ITEM>(m_tabCtrl.GetCurItem());
		PAGE_ITEM focusItem = static_cast<PAGE_ITEM>(m_tabCtrl.GetFocusItem());
		if(curItem != focusItem)
		{
			if(PAGE_PIC_SEARCH == focusItem)
			{
				if(m_bUnfold || !m_bFlexSearchConditionDlg)
				{
#if defined(__GROUY_SEARCH__)
					{
						if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768)
						{
							if(m_searchGroupConditionDlg.IsShow())
							{
								if(m_searchGroupConditionDlg.IsModify())
								{
									m_searchGroupConditionDlg.SaveGroupItemCfg();
								}
								m_searchGroupConditionDlg.Show(false, false);
							}

							if(!m_searchGroupTimeDlg.IsShow())
							{
								//m_searchGroupTimeDlg获取配置
								m_searchGroupTimeDlg.LoadGroupItemCfg();
								m_searchGroupTimeDlg.Show(true, false);
							}
						}
						else
						{
							if(m_searchGroupScrollConditionDlg.IsShow())
							{
								if(m_searchGroupScrollConditionDlg.IsModify())
								{
									m_searchGroupScrollConditionDlg.SaveGroupItemCfg();
								}
								m_searchGroupScrollConditionDlg.Show(false,false);
							}
							if(!m_searchGroupScrollTimeDlg.IsShow())
							{
								//m_searchGroupScrollTimeDlg获取配置
								m_searchGroupScrollTimeDlg.LoadGroupItemCfg();
								m_searchGroupScrollTimeDlg.Show(true, false);
							}
						}
					}			
#else
					{
						if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 || m_channelNum <= 16)
						{
							if(m_searchConditionDlg.IsShow())
							{
								m_searchConditionDlg.Show(false, false);
							}

							if(!m_searchCdn2TimtDlg.IsShow())
							{
								m_searchCdn2TimtDlg.Show(true, false);
							}
						}	
						else
						{
							if(m_searchCondition2Dlg.IsShow())
							{
								m_searchCondition2Dlg.Show(false, false);
							}
							if(!m_searchCdnTimtDlg.IsShow())
							{
								m_searchCdnTimtDlg.Show(true, false);
							}
						}
						
					}	
#endif	
			}
		}
			if(PAGE_PIC_SEARCH != focusItem)
			{
				if(m_bUnfold || !m_bFlexSearchConditionDlg)
				{
#if defined(__GROUY_SEARCH__)
					{	
						if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768)
						{
							if(m_searchGroupTimeDlg.IsShow())
							{
								if(m_searchGroupTimeDlg.IsModify())
								{
									m_searchGroupTimeDlg.SaveGroupItemCfg();
								}
								m_searchGroupTimeDlg.Show(false, false);
							}

							if(!m_searchGroupConditionDlg.IsShow())
							{
								m_searchGroupConditionDlg.LoadGroupItemCfg();
								m_searchGroupConditionDlg.Show(true, false);
							}
								
						}
						else
						{
							if(m_searchGroupScrollTimeDlg.IsShow())
							{
								if(m_searchGroupScrollTimeDlg.IsModify())
								{
									m_searchGroupScrollTimeDlg.SaveGroupItemCfg();
								}
								m_searchGroupScrollTimeDlg.Show(false, false);
								
							}

							if(!m_searchGroupScrollConditionDlg.IsShow())
							{
								m_searchGroupScrollConditionDlg.LoadGroupItemCfg();
								m_searchGroupScrollConditionDlg.Show(true,false);
								
							}
						}

					}
					
#else
					{
						if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 || m_channelNum <= 16)
						{
							if(!m_searchConditionDlg.IsShow())
							{
								m_searchConditionDlg.Show(true, false);
							}
							if(m_searchCdn2TimtDlg.IsShow())
							{
								m_searchCdn2TimtDlg.Show(false, false);
							}
						}
						else
						{
							if(!m_searchCondition2Dlg.IsShow())
							{
								m_searchCondition2Dlg.Show(true, false);
							}
							if(m_searchCdnTimtDlg.IsShow())
							{
								m_searchCdnTimtDlg.Show(false, false);
							}
						}
					}
					
#endif
				}
			}

			m_tabCtrl.SetCurItem(focusItem, false);

			Repaint();

			return KEY_VIRTUAL_MSG_STOP;
		}
	}

	return GUI::CDialog::OnPreCommand(ctrlID, keyID, xPos, yPos);
}

BEGIN_MSG_MAP(CSearchManDlg, GUI::CDialog)
	ON_MSG(m_btnFlex.GetID(), KEY_VIRTUAL_ENTER, OnBtnFlex)
	ON_MSG(m_btnSearch.GetID(), KEY_VIRTUAL_ENTER, OnSearch)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_SEARCH, OnSearch)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_PLAY, OnPlayback)
END_MSG_MAP()

unsigned long CSearchManDlg::OnBtnFlex()
{
	if( GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 ) 
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT]+m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short height = m_rect.Height() - y - 2 * m_length[DLG_OFFSET_BOTTOM];

	if (m_bFlexSearchConditionDlg)
	{
		int w = m_rect.Width()-m_searchConditionDlgWidth-m_length[FLEX_BTN_WIDTH]-(m_length[DLG_OFFSET_LEFT]*2);
		m_timeSearchDlg.SetWndSize(false, false, w, height);
		m_eventSearchDlg.SetWndSize(false, false, w, height);
		m_fileManDlg.SetWndSize(false, false, w, height);
		m_pictureSearchDlg.SetWndSize(false, false, w, height);
		m_tabCtrl.SetWndSize(false, false, w + m_searchConditionDlgWidth - m_length[BTN_WIDTH], m_length[TABLE_HEIGHT]);

		if(PAGE_PIC_SEARCH == static_cast<PAGE_ITEM>(m_tabCtrl.GetCurItem()))
		{
#if defined(__GROUY_SEARCH__)
			{
				if(!m_searchGroupScrollTimeDlg.IsShow())
				{
					m_searchGroupScrollTimeDlg.Show(true, false);
				}
				if(m_searchGroupScrollConditionDlg.IsShow())
				{
					m_searchGroupScrollConditionDlg.Show(false,false);
				}
			}
#else
			{
				if(m_channelNum <= 16)
				{
					if(!m_searchCdn2TimtDlg.IsShow())
					{
						m_searchCdn2TimtDlg.Show(true, false);
					}
					{
						if(m_searchConditionDlg.IsShow())
						{
							m_searchConditionDlg.Show(false, false);
						}
					}
				}
				else
				{
					if(!m_searchCdnTimtDlg.IsShow())
					{
						m_searchCdnTimtDlg.Show(true, false);
					}

					if(m_searchCondition2Dlg.IsShow())
					{
						m_searchCondition2Dlg.Show(false, false);
					}
				}
			}
#endif

		}
		else
		{
#if defined(__GROUY_SEARCH__)
			{
				if(m_searchGroupScrollTimeDlg.IsShow())
				{
					m_searchGroupScrollTimeDlg.Show(false, false);
				}

				if(!m_searchGroupScrollConditionDlg.IsShow())
				{
					m_searchGroupScrollConditionDlg.Show(true,false);
				}
			}
#else
			{
				if (m_channelNum <= 16)
				{
					if(m_searchCdn2TimtDlg.IsShow())
					{
						m_searchCdn2TimtDlg.Show(false, false);
					}
					{
						if(!m_searchConditionDlg.IsShow())
						{
							m_searchConditionDlg.Show(true, false);
						}
					}		
				}
				else
				{
					if(m_searchCdnTimtDlg.IsShow())
					{
						m_searchCdnTimtDlg.Show(false, false);
					}

					if(!m_searchCondition2Dlg.IsShow())
					{
						m_searchCondition2Dlg.Show(true, false);
					}
				}
			}

#endif
		}
		
		m_btnFlex.MoveWnd(m_rect.m_left+m_rect.Width()-m_searchConditionDlgWidth-m_length[FLEX_BTN_WIDTH]-m_length[DLG_OFFSET_LEFT], m_rect.m_top + m_rect.Height()/2-(m_length[FLEX_BTN_HEIGHT]/2) );

		m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_R, false);
		m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_R_F, false);
	}
	else
	{
		m_timeSearchDlg.SetWndSize(true, false);
		m_eventSearchDlg.SetWndSize(true, false);
		m_fileManDlg.SetWndSize(true, false);
		m_pictureSearchDlg.SetWndSize(true, false);
		m_tabCtrl.SetWndSize(true, false);
#if defined(__GROUY_SEARCH__)
		{
			if(m_searchGroupScrollTimeDlg.IsShow())
			{
				m_searchGroupScrollTimeDlg.Show(false, false);
			}
			if(m_searchGroupScrollConditionDlg.IsShow())
			{
				m_searchGroupScrollConditionDlg.Show(false,false);
			}
		}
#else
		{
			if(m_channelNum <= 16)
			{
				if(m_searchCdn2TimtDlg.IsShow())
				{
					m_searchCdn2TimtDlg.Show(false, false);
				}
				{
					if(m_searchConditionDlg.IsShow())
					{
						m_searchConditionDlg.Show(false, false);
					}
				}
			}
			else
			{
				if(m_searchCdnTimtDlg.IsShow())
				{
					m_searchCdnTimtDlg.Show(false, false);
				}
				if(m_searchCondition2Dlg.IsShow())
				{
					m_searchCondition2Dlg.Show(false, false);
				}
			}

		}
#endif

		m_btnFlex.MoveWnd(m_rect.m_left+m_rect.Width()-m_length[DLG_OFFSET_LEFT], m_rect.m_top + m_rect.Height()/2-(m_length[FLEX_BTN_HEIGHT]/2) );
		m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_L, false);
		m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_L_F, false);
	}
	
	m_bFlexSearchConditionDlg = !m_bFlexSearchConditionDlg;
	m_bFlexOut	= !m_bFlexOut;
	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchManDlg::OnPlayback()
{
	unsigned long playbackStartTime = 0;
	unsigned long playbackEndTime   = 0;
	unsigned long playbackPlayTime   = 0;
	ULONGLONG     chnnBits        = 0;
	CMyList<unsigned long>  channelList;
	int currItem = 0;
	unsigned char maxVideoOutNum = 0;

	currItem = m_tabCtrl.GetCurItem();

	if (PAGE_TIME_SEARCH == currItem)
	{
		if(KEY_VIRTUAL_MSG_STOP == m_timeSearchDlg.GetPlaybackInfo(playbackStartTime, playbackEndTime, playbackPlayTime, chnnBits))
		{
			return KEY_VIRTUAL_MSG_STOP;
		}

		maxVideoOutNum = g_p_login_succ->productInfo.videoOuputNum;

		CPlaybackDlg playbackDlg;	
		playbackDlg.SetPlayInfo(playbackStartTime, playbackEndTime, playbackPlayTime, chnnBits, m_channelNum, maxVideoOutNum, true, m_diskOwnerIndex);
		playbackDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
	}
	else if (PAGE_EVENT_SEARCH == currItem)
	{
		FILE_INFO_EX fileInfo;

		if (KEY_VIRTUAL_MSG_STOP == m_eventSearchDlg.GetPlaybackInfo(fileInfo))
		{
			return KEY_VIRTUAL_MSG_STOP;
		}
		maxVideoOutNum = g_p_login_succ->productInfo.videoOuputNum;

		CPlaybackDlg playbackDlg;

		if (RECORD_FILE_IS_UNOFFICAL(fileInfo.type))
		{
			playbackDlg.SetPlayInfo(fileInfo, m_diskOwnerIndex);
		}
		else
		{
			ULONGLONG authCH = (g_p_login_succ->productInfo.bPasswordCheck) ? g_p_login_succ->authPlaybackCH : ALL_CHNN_MASK(m_channelNum);

			ULONGLONG tmpChnnBit =(static_cast<ULONGLONG>(0x01) << fileInfo.chnn);
			if( (tmpChnnBit  & authCH) != tmpChnnBit)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_77), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				return KEY_VIRTUAL_MSG_STOP;	
			}
			else
			{
				playbackDlg.SetPlayInfo(fileInfo.startTime, fileInfo.endTime, fileInfo.startTime, \
					tmpChnnBit, m_channelNum, maxVideoOutNum, true, m_diskOwnerIndex);

				playbackDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
			}
		}	
	}
	else if(PAGE_FILE_MAN == currItem)
	{
		
		FILE_INFO_EX fileInfo;
 
		if (KEY_VIRTUAL_MSG_STOP == m_fileManDlg.GetPlaybackInfo(fileInfo))
		{
			return KEY_VIRTUAL_MSG_STOP;
		}

		maxVideoOutNum = g_p_login_succ->productInfo.videoOuputNum;

		CPlaybackDlg playbackDlg;
		
		if (RECORD_FILE_IS_UNOFFICAL(fileInfo.type))
		{
			playbackDlg.SetPlayInfo(fileInfo, m_diskOwnerIndex);
		}
		else
		{
			ULONGLONG authCH = (g_p_login_succ->productInfo.bPasswordCheck) ? g_p_login_succ->authPlaybackCH : ALL_CHNN_MASK(m_channelNum);
	
			ULONGLONG tmpChnnBit =(static_cast<ULONGLONG>(0x01) << fileInfo.chnn);
			//printf("tmpChnnBit=%x,total=%x,auth=%x\n",tmpChnnBit,tmpChnnBit  & authCH,authCH);
			if( (tmpChnnBit  & authCH) != tmpChnnBit)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_77), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				return KEY_VIRTUAL_MSG_STOP;	
			}
			else
			{
				
				playbackDlg.SetPlayInfo(fileInfo.startTime, fileInfo.endTime, fileInfo.startTime, \
					tmpChnnBit, m_channelNum, maxVideoOutNum, true, m_diskOwnerIndex);
				playbackDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
			}
			
		}

		
	}
	else if(PAGE_PIC_SEARCH == currItem)
	{
		if(!m_pictureSearchDlg.GetPlaybackInfo(playbackStartTime, playbackEndTime, playbackPlayTime, chnnBits))
		{
			return KEY_VIRTUAL_MSG_STOP;
		}

		maxVideoOutNum = g_p_login_succ->productInfo.videoOuputNum;

		CPlaybackDlg playbackDlg;	
		playbackDlg.SetPlayInfo(playbackStartTime, playbackEndTime, playbackPlayTime, chnnBits, m_channelNum, maxVideoOutNum, true, m_diskOwnerIndex);
		playbackDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
	}
	else
	{
		assert(false);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchManDlg::OnSearch()
{
#if defined(__GROUY_SEARCH__)
	{
		if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768)
		{
			int currItem = m_tabCtrl.GetCurItem();

			if(currItem != PAGE_PIC_SEARCH)
			{
				if (0 == m_searchGroupConditionDlg.CH())
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					return KEY_VIRTUAL_MSG_STOP;
				}
			}
			else
			{
				if(0 == m_searchGroupTimeDlg.CH())
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					return KEY_VIRTUAL_MSG_STOP;
				}
			}
			
			
			if(PAGE_TIME_SEARCH == currItem)
			{
				m_timeSearchDlg.OnSearch(m_searchGroupConditionDlg.Time(), m_searchGroupConditionDlg.Time()+ONE_DAY_SECOND_NUM, m_searchGroupConditionDlg.CH(), m_diskOwnerIndex, true);
			}
			else if(PAGE_EVENT_SEARCH == currItem)
			{
				m_eventSearchDlg.OnSearch(m_searchGroupConditionDlg.Time(), m_searchGroupConditionDlg.Time()+ONE_DAY_SECOND_NUM, m_searchGroupConditionDlg.CH(), m_diskOwnerIndex);
			}
			else if(PAGE_FILE_MAN == currItem)
			{
				m_fileManDlg.OnSearch(m_searchGroupConditionDlg.Time(), m_searchGroupConditionDlg.Time()+ONE_DAY_SECOND_NUM, m_searchGroupConditionDlg.CH(), m_diskOwnerIndex);
			}
			else if(PAGE_PIC_SEARCH == currItem)
			{
				m_pictureSearchDlg.OnSearch(m_searchGroupTimeDlg.TimeStart(), m_searchGroupTimeDlg.TimeEnd(), m_searchGroupTimeDlg.CH(), m_diskOwnerIndex);
			}
			else
			{
				assert(false);
			}
		}
		else
		{
			int currItem = m_tabCtrl.GetCurItem();

			if(currItem != PAGE_PIC_SEARCH)
			{
				ULONGLONG chnnBits = m_searchGroupScrollConditionDlg.CH();
				if (0 == chnnBits)
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					return KEY_VIRTUAL_MSG_STOP;
				}
			}
			else
			{
				if(0 == m_searchGroupScrollTimeDlg.CH())
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					return KEY_VIRTUAL_MSG_STOP;
				}
			}
			

			
			bool brefrsh = false;
			if(PAGE_TIME_SEARCH == currItem)
			{
				if(m_bFlexOut)//如果是展开，刷新
				{
					brefrsh = true;
				}
				else
				{
					brefrsh = false;
				}
				m_timeSearchDlg.OnSearch(m_searchGroupScrollConditionDlg.Time(), m_searchGroupScrollConditionDlg.Time()+ONE_DAY_SECOND_NUM, chnnBits, m_diskOwnerIndex, brefrsh);
			}
			else if(PAGE_EVENT_SEARCH == currItem)
			{
				m_eventSearchDlg.OnSearch(m_searchGroupScrollConditionDlg.Time(), m_searchGroupScrollConditionDlg.Time()+ONE_DAY_SECOND_NUM, chnnBits, m_diskOwnerIndex);
			}
			else if(PAGE_FILE_MAN == currItem)
			{
				m_fileManDlg.OnSearch(m_searchGroupScrollConditionDlg.Time(), m_searchGroupScrollConditionDlg.Time()+ONE_DAY_SECOND_NUM, chnnBits, m_diskOwnerIndex);
			}
			else if(PAGE_PIC_SEARCH == currItem)
			{
				m_pictureSearchDlg.OnSearch(m_searchGroupScrollTimeDlg.TimeStart(), m_searchGroupScrollTimeDlg.TimeEnd(), m_searchGroupScrollTimeDlg.CH(), m_diskOwnerIndex);
			}
			else
			{
				assert(false);
			}
		}
	}
#else
	{
		if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 || m_channelNum <= 16)
		{
			int currItem = m_tabCtrl.GetCurItem();
			if(currItem != PAGE_PIC_SEARCH)
			{
				if (0 == m_searchConditionDlg.CH())
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					return KEY_VIRTUAL_MSG_STOP;
				}
			}
			else
			{
				if (0 == m_searchCdn2TimtDlg.CH())
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					return KEY_VIRTUAL_MSG_STOP;
				}
			}
			

			
			if(PAGE_TIME_SEARCH == currItem)
			{
				bool brefrsh=false;
				if(GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768)
				{
					brefrsh = true;
				}
				else
				{
					if(m_bFlexOut)//如果是展开，刷新
					{
						brefrsh = true;
					}
					else
					{
						brefrsh = false;
					}
				}
				m_timeSearchDlg.OnSearch(m_searchConditionDlg.Time(), m_searchConditionDlg.Time()+ONE_DAY_SECOND_NUM, m_searchConditionDlg.CH(), m_diskOwnerIndex, brefrsh);

			}
			else if(PAGE_EVENT_SEARCH == currItem)
			{
				m_eventSearchDlg.OnSearch(m_searchConditionDlg.Time(), m_searchConditionDlg.Time()+ONE_DAY_SECOND_NUM, m_searchConditionDlg.CH(), m_diskOwnerIndex);
			}
			else if(PAGE_FILE_MAN == currItem)
			{
				m_fileManDlg.OnSearch(m_searchConditionDlg.Time(), m_searchConditionDlg.Time()+ONE_DAY_SECOND_NUM, m_searchConditionDlg.CH(), m_diskOwnerIndex);
			}
			else if(PAGE_PIC_SEARCH == currItem)
			{
				m_pictureSearchDlg.OnSearch(m_searchCdn2TimtDlg.TimeStart(), m_searchCdn2TimtDlg.TimeEnd(), m_searchCdn2TimtDlg.CH(), m_diskOwnerIndex);
			}
			else
			{
				assert(false);
			}
		}
		else
		{
			int currItem = m_tabCtrl.GetCurItem();
			if(currItem != PAGE_PIC_SEARCH)
			{
				if (0 == m_searchCondition2Dlg.CH())
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					return KEY_VIRTUAL_MSG_STOP;
				}
			}
			else
			{
				if (0 == m_searchCdnTimtDlg.CH())
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					return KEY_VIRTUAL_MSG_STOP;
				}
			}
			
			
			if(PAGE_TIME_SEARCH == currItem)
			{
				bool brefrsh=false;
				if(GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768)
				{
					brefrsh = true;
				}
				else
				{
					if(m_bFlexOut)//如果是展开，刷新
					{
						brefrsh = true;
					}
					else
					{
						brefrsh = false;
					}
				}
				m_timeSearchDlg.OnSearch(m_searchCondition2Dlg.Time(), m_searchCondition2Dlg.Time()+ONE_DAY_SECOND_NUM, m_searchCondition2Dlg.CH(), m_diskOwnerIndex, brefrsh);
			}
			else if(PAGE_EVENT_SEARCH == currItem)
			{
				m_eventSearchDlg.OnSearch(m_searchCondition2Dlg.Time(), m_searchCondition2Dlg.Time()+ONE_DAY_SECOND_NUM, m_searchCondition2Dlg.CH(), m_diskOwnerIndex);
			}
			else if(PAGE_FILE_MAN == currItem)
			{
				m_fileManDlg.OnSearch(m_searchCondition2Dlg.Time(), m_searchCondition2Dlg.Time()+ONE_DAY_SECOND_NUM, m_searchCondition2Dlg.CH(), m_diskOwnerIndex);
			}
			else if(PAGE_PIC_SEARCH == currItem)
			{
				m_pictureSearchDlg.OnSearch(m_searchCdnTimtDlg.TimeStart(), m_searchCdnTimtDlg.TimeEnd(), m_searchCdnTimtDlg.CH(), m_diskOwnerIndex);
			}
			else
			{
				assert(false);
			}
		}

	}
#endif
	if(!m_bFlexSearchConditionDlg) 
	{
		OnBtnFlex();
	}

	return KEY_VIRTUAL_MSG_STOP;
}



void CSearchManDlg::SetCalendarInfo()
{
	MESSAGE_DATA msgData;
	NET_DATA_SEARCH searchCond;

	searchCond.searchType = DATA_SEARCH_TYPE_YEAR;	
	searchCond.dataOwnerby = m_diskOwnerIndex;

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&searchCond, sizeof(NET_DATA_SEARCH), msgData);

	DVR_DATE *pDate = (DVR_DATE *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
	int num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(DVR_DATE);
	for (int count = 0; count < num; ++count)
	{
#if defined(__GROUY_SEARCH__)
		if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768)
		{
			m_searchGroupConditionDlg.MarkDay(pDate[count].year, pDate[count].month, pDate[count].mday, COLOR_BTN_DEFAULT_MARK, false);
		}
		else
		{
			m_searchGroupScrollConditionDlg.MarkDay(pDate[count].year, pDate[count].month, pDate[count].mday, COLOR_BTN_DEFAULT_MARK, false);
		}
#else
		if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 || m_channelNum <= 16)
		{
			m_searchConditionDlg.MarkDay(pDate[count].year, pDate[count].month, pDate[count].mday, COLOR_BTN_DEFAULT_MARK, false);
		}
		else
		{
			m_searchCondition2Dlg.MarkDay(pDate[count].year, pDate[count].month, pDate[count].mday, COLOR_BTN_DEFAULT_MARK, false);
		}
#endif
	}

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);	

}

#if defined(__GROUY_SEARCH__)
	void CSearchManDlg::EndDialog(DLG_END_VALUE result)
	{
		PAGE_ITEM focusItem = static_cast<PAGE_ITEM>(m_tabCtrl.GetFocusItem());
		if(focusItem!=PAGE_PIC_SEARCH)
		{
			if(GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 )
			{
				if (m_searchGroupConditionDlg.IsModify())
				{
					m_searchGroupConditionDlg.SaveGroupItemCfg();
				}
			}
			else
			{
				if(m_searchGroupScrollConditionDlg.IsModify())
				{
					m_searchGroupScrollConditionDlg.SaveGroupItemCfg();
				}
			}
		}
		else
		{
			if(GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 )
			{
				if (m_searchGroupTimeDlg.IsModify())
				{
					m_searchGroupTimeDlg.SaveGroupItemCfg();
				}
			}
			else
			{
				if(m_searchGroupScrollTimeDlg.IsModify())
				{
					m_searchGroupScrollTimeDlg.SaveGroupItemCfg();
				}
			}
		}
		
		return GUI::CDialog::EndDialog(result);

	}
#endif
//////////////////////////////////////////////////////////////////////////
