/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-14
** Name         : CfgManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_MAN_DLG_CPP_
#define _CFG_MAN_DLG_CPP_

#include "CfgManDlg.h"

//////////////////////////////////////////////////////////////////////////
// 2 tab 页面
//////////////////////////////////////////////////////////////////////////

template <class T1, class T2>
CCfgManDlg2<T1, T2>::CCfgManDlg2() : m_curPage( PAGE_1 ), m_callback(NULL), m_initAnchorPage(PAGE_1)
{

}

template <class T1, class T2>
CCfgManDlg2<T1, T2>::~CCfgManDlg2()
{

}

template <class T1, class T2>
CCfgManDlg2<T1,T2>& CCfgManDlg2<T1, T2>::AddCaption(const std::string& strCaption )
{
	m_strNameVec.push_back( strCaption );

	return *this;
}

template <class T1, class T2>
void CCfgManDlg2<T1, T2>::AddGetItemProc(PFunc_GetItemList pGet )
{
	m_callback = pGet;
}

template <class T1, class T2>
void CCfgManDlg2<T1, T2>::OnPreInitial()
{
	CCfgDlg::OnInitial();

	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_strNameVec[0], false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DEFAULT) );
}

template <class T1, class T2>
void CCfgManDlg2<T1, T2>::OnInitial()
{
	// [2010-03-03 L] 修改语言时,对话框要求可以重新初始化这些临时存储内容也应该被剔除
	m_strNameVec.clear();

	OnPreInitial();

	assert( !(m_strNameVec.size() < 3) );
	//////////////////////////////////////////////////////////////////////////

#ifndef __DVR_ANCHOR__
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	m_cfgT1.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 2);
	m_cfgT2.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 3);

	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);

	m_tabCtrl.AddItem(&m_cfgT1, m_strNameVec[1] );
	m_tabCtrl.AddItem(&m_cfgT2, m_strNameVec[2]);

	////
	m_curPage = PAGE_1;
	m_tabCtrl.SetCurItem(m_curPage , false );
	CCfgDlg::SetRefreshView( false );
#else
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	switch (m_initAnchorPage)
	{
	case PAGE_1:
		SetCaption(m_strNameVec[1], false);
		m_cfgT1.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_2:
		SetCaption(m_strNameVec[2], false);
		m_cfgT2.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	default:
		assert(false);
		break;
	}

	m_curPage = m_initAnchorPage;
	CCfgDlg::SetRefreshView( false );

#endif

	//create tip
	CreateTipWnd();
}

template <class T1, class T2>
void CCfgManDlg2<T1,T2>::OnDestroy()
{
	CCfgDlg::OnDestroy();
	m_strNameVec.clear();
}

template <class T1, class T2>
void CCfgManDlg2<T1,T2>::GetItemList(CFGITEM_LIST& itemList) const
{
	if( m_callback )
	{
		m_callback( itemList );
	}
}

template <class T1, class T2>
unsigned long CCfgManDlg2<T1,T2>::OnApply()
{
	//Load default
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	unsigned long ret = OnLoadDefault(pCfgDlg);

	if( pCfgDlg->IsModify() )
	{
		EnableOkBtn(true,false);
	}
	else
	{
		EnableOkBtn(false,false);
	}

	ENUM_STRING_ID strID = ret ? static_cast<ENUM_STRING_ID>(ret) : E_STR_ID_TIP_00_03;
	MessageOut(strID,TIP_MSG,false);

	//一次重绘全部
	Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T1, class T2>
unsigned long CCfgManDlg2<T1,T2>::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

template <class T1, class T2>
CCfgDlg* CCfgManDlg2<T1,T2>::GetPage(PAGE_ITEM item)
{
	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_1:
		pCfgDlg = &m_cfgT1;
		break;
	case PAGE_2:
		pCfgDlg = &m_cfgT2;
		break;
	default:
		assert(false);
		break;
	}
	return pCfgDlg;
}

template <class T1, class T2>
void CCfgManDlg2<T1,T2>::SetAnchorPage(PAGE_ITEM item)
{
	m_initAnchorPage = item;
}

template <class T1, class T2>
unsigned long CCfgManDlg2<T1,T2>::OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (KEY_VIRTUAL_MSG_STOP == CCfgDlg::OnCommand (ctrlID, keyID, xPos, yPos)) 
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//page1
	if ((ctrlID == m_cfgT1.GetID()) && ((keyID == KEY_VIRTUAL_ENTER) || (keyID == KEY_VIRTUAL_DBCLICK)))
	{
		keyID = OnClickPage1();
		if (KEY_VIRTUAL_MSG_STOP == keyID) return KEY_VIRTUAL_MSG_STOP;
	}

	//page2
	if ((ctrlID == m_cfgT2.GetID()) && ((keyID == KEY_VIRTUAL_ENTER) || (keyID == KEY_VIRTUAL_DBCLICK)))
	{
		keyID = OnClickPage2();
		if (KEY_VIRTUAL_MSG_STOP == keyID) return KEY_VIRTUAL_MSG_STOP;
	}

	////消息响应结束
	return keyID;
}

template <class T1, class T2>
unsigned long CCfgManDlg2<T1, T2>::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		if (m_curPage != m_tabCtrl.GetFocusItem())
		{
			unsigned long ret = 0;
			if (GetPage(m_curPage)->IsModify())
			{
				ret = TrySaveChange(GetPage(m_curPage),true,false,false);
			}

			if ( ret == 0 )
			{
				m_curPage = static_cast<PAGE_ITEM>(m_tabCtrl.GetFocusItem());
			}
			else
			{
				Repaint();

				return KEY_VIRTUAL_MSG_STOP;
			}
		}	
	}

	return keyID;
}

template <class T1, class T2>
unsigned long CCfgManDlg2<T1, T2>::OnClickPage1()
{
	if (m_cfgT1.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T1, class T2>
unsigned long CCfgManDlg2<T1, T2>::OnClickPage2()
{
	if (m_cfgT2.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}


//////////////////////////////////////////////////////////////////////////
// 3 tab 页面
//////////////////////////////////////////////////////////////////////////

template <class T1, class T2, class T3>
CCfgManDlg3<T1,T2,T3>::CCfgManDlg3() : m_curPage(PAGE_1), m_callback(NULL), m_initAnchorPage(PAGE_1)
{

}

template <class T1, class T2, class T3>
CCfgManDlg3<T1,T2,T3>::~CCfgManDlg3()
{

}

template <class T1, class T2, class T3>
CCfgManDlg3<T1,T2,T3>& CCfgManDlg3<T1,T2,T3>::AddCaption(const std::string& strCaption )
{
	m_strNameVec.push_back( strCaption );

	return *this;
}

template <class T1, class T2, class T3>
void CCfgManDlg3<T1,T2,T3>::AddGetItemProc(PFunc_GetItemList pGet )
{
	m_callback = pGet;
}

template <class T1, class T2, class T3>
void CCfgManDlg3<T1,T2,T3>::OnPreInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_strNameVec[0], false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DEFAULT) );
}

template <class T1, class T2, class T3>
void CCfgManDlg3<T1,T2,T3>::OnInitial()
{
	// [2010-03-03 L] 修改语言时,对话框要求可以重新初始化这些临时存储内容也应该被剔除
	m_strNameVec.clear();

	OnPreInitial();

	assert( !(m_strNameVec.size() < 4) );
	//////////////////////////////////////////////////////////////////////////
#ifndef __DVR_ANCHOR__
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	m_cfgT1.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 1, 1);
	m_cfgT2.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 2, 1);
	m_cfgT3.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 3, 1);

	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);

	m_tabCtrl.AddItem(&m_cfgT1, m_strNameVec[1] );
	m_tabCtrl.AddItem(&m_cfgT2, m_strNameVec[2]);
	m_tabCtrl.AddItem(&m_cfgT3, m_strNameVec[3]);

	////
	m_curPage = PAGE_1;
	m_tabCtrl.SetCurItem(m_curPage , false );
	CCfgDlg::SetRefreshView( false );
#else
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	switch (m_initAnchorPage)
	{
	case PAGE_1:
		SetCaption(m_strNameVec[1], false);
		m_cfgT1.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_2:
		SetCaption(m_strNameVec[2], false);
		m_cfgT2.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_3:
		SetCaption(m_strNameVec[3], false);
		m_cfgT3.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	default:
		assert(false);
		break;
	}

	m_curPage = m_initAnchorPage;
	CCfgDlg::SetRefreshView( false );
#endif
	//create tip
	CreateTipWnd();
}

template <class T1, class T2, class T3>
void CCfgManDlg3<T1,T2,T3>::OnDestroy()
{
	CCfgDlg::OnDestroy();

	m_strNameVec.clear();
}

template <class T1, class T2, class T3>
void CCfgManDlg3<T1,T2,T3>::GetItemList(CFGITEM_LIST& itemList) const
{
	if( m_callback )
	{
		m_callback( itemList );
	}
}

template <class T1, class T2, class T3>
unsigned long CCfgManDlg3<T1,T2,T3>::OnApply()
{
	//Load default
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	unsigned long ret = OnLoadDefault(pCfgDlg);

	if( pCfgDlg->IsModify() )
	{
		EnableOkBtn(true,false);
	}
	else
	{
		EnableOkBtn(false,false);
	}

	ENUM_STRING_ID strID = ret ? static_cast<ENUM_STRING_ID>(ret) : E_STR_ID_TIP_00_03;
	MessageOut(strID,TIP_MSG,false);

	//一次重绘全部
	Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T1, class T2, class T3>
unsigned long CCfgManDlg3<T1,T2,T3>::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

template <class T1, class T2, class T3>
CCfgDlg* CCfgManDlg3<T1,T2,T3>::GetPage(PAGE_ITEM item)
{
	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_1:
		pCfgDlg = &m_cfgT1;
		break;
	case PAGE_2:
		pCfgDlg = &m_cfgT2;
		break;
	case PAGE_3:
		pCfgDlg = &m_cfgT3;
		break;
	default:
		assert(false);
		break;
	}
	return pCfgDlg;
}

template <class T1, class T2, class T3>
void CCfgManDlg3<T1,T2,T3>::SetAnchorPage(PAGE_ITEM item)
{
	m_initAnchorPage = item;
}

template <class T1, class T2, class T3>
unsigned long CCfgManDlg3<T1,T2,T3>::OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (KEY_VIRTUAL_MSG_STOP == CCfgDlg::OnCommand (ctrlID, keyID, xPos, yPos)) 
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//page1
	if ((ctrlID == m_cfgT1.GetID()) && ((keyID == KEY_VIRTUAL_ENTER) || (keyID == KEY_VIRTUAL_DBCLICK)))
	{
		keyID = OnClickPage1();
		if (KEY_VIRTUAL_MSG_STOP == keyID) return KEY_VIRTUAL_MSG_STOP;
	}

	//page2
	if ((ctrlID == m_cfgT2.GetID()) && ((keyID == KEY_VIRTUAL_ENTER) || (keyID == KEY_VIRTUAL_DBCLICK)))
	{
		keyID = OnClickPage2();
		if (KEY_VIRTUAL_MSG_STOP == keyID) return KEY_VIRTUAL_MSG_STOP;
	}

	//page3
	if ((ctrlID == m_cfgT3.GetID()) && ((keyID == KEY_VIRTUAL_ENTER) || (keyID == KEY_VIRTUAL_DBCLICK)))
	{
		keyID = OnClickPage3();
		if (KEY_VIRTUAL_MSG_STOP == keyID) return KEY_VIRTUAL_MSG_STOP;
	}

	////消息响应结束
	return keyID;
}

template <class T1, class T2, class T3>
unsigned long CCfgManDlg3<T1,T2,T3>::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		if (m_curPage != m_tabCtrl.GetFocusItem())
		{
			unsigned long ret = 0;
			if (GetPage(m_curPage)->IsModify())
			{
				ret = TrySaveChange(GetPage(m_curPage),true,false,false);
			}

			if ( ret == 0 )
			{
				m_curPage = static_cast<PAGE_ITEM>(m_tabCtrl.GetFocusItem());
			}
			else
			{
				Repaint();

				return KEY_VIRTUAL_MSG_STOP;
			}
		}	
	}

	return keyID;
}

template <class T1, class T2, class T3>
unsigned long CCfgManDlg3<T1,T2,T3>::OnClickPage1()
{
	if (m_cfgT1.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T1, class T2, class T3>
unsigned long CCfgManDlg3<T1,T2,T3>::OnClickPage2()
{
	if (m_cfgT2.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T1, class T2, class T3>
unsigned long CCfgManDlg3<T1,T2,T3>::OnClickPage3()
{
	if (m_cfgT3.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//////////////////////////////////////////////////////////////////////////
// 4 tab 页面
//////////////////////////////////////////////////////////////////////////

template <class T1, class T2, class T3, class T4>
CCfgManDlg4<T1,T2,T3,T4>::CCfgManDlg4() : m_curPage(PAGE_1), m_callback(NULL), m_initAnchorPage(PAGE_1)
{

}

template <class T1, class T2, class T3, class T4>
CCfgManDlg4<T1,T2,T3,T4>::~CCfgManDlg4()
{

}

template <class T1, class T2, class T3, class T4>
CCfgManDlg4<T1,T2,T3,T4>& CCfgManDlg4<T1,T2,T3,T4>::AddCaption(const std::string& strCaption )
{
	m_strNameVec.push_back( strCaption );

	return *this;
}

template <class T1, class T2, class T3, class T4>
void CCfgManDlg4<T1,T2,T3,T4>::AddGetItemProc(PFunc_GetItemList pGet )
{
	m_callback = pGet;
}

template <class T1, class T2, class T3, class T4>
void CCfgManDlg4<T1,T2,T3,T4>::OnPreInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_strNameVec[0], false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DEFAULT) );
}

template <class T1, class T2, class T3, class T4>
void CCfgManDlg4<T1,T2,T3,T4>::OnInitial()
{
	// [2010-03-03 L] 修改语言时,对话框要求可以重新初始化这些临时存储内容也应该被剔除
	m_strNameVec.clear();

	OnPreInitial();

	assert( !(m_strNameVec.size() < 5) );
	//////////////////////////////////////////////////////////////////////////

#ifndef __DVR_ANCHOR__
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	m_cfgT1.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 1, 1);
	m_cfgT2.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 2, 1);
	m_cfgT3.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 3, 1);
	m_cfgT4.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 4, 1);

	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);

	m_tabCtrl.AddItem(&m_cfgT1, m_strNameVec[1] );
	m_tabCtrl.AddItem(&m_cfgT2, m_strNameVec[2]);
	m_tabCtrl.AddItem(&m_cfgT3, m_strNameVec[3]);
	m_tabCtrl.AddItem(&m_cfgT4, m_strNameVec[4]);

	////
	m_curPage = PAGE_1;
	m_tabCtrl.SetCurItem(m_curPage , false );
	CCfgDlg::SetRefreshView( false );
#else

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	switch (m_initAnchorPage)
	{
	case PAGE_1:
		SetCaption(m_strNameVec[1], false);
		m_cfgT1.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_2:
		SetCaption(m_strNameVec[2], false);
		m_cfgT2.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_3:
		SetCaption(m_strNameVec[3], false);
		m_cfgT3.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_4:
		SetCaption(m_strNameVec[4], false);
		m_cfgT4.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	default:
		assert(false);
		break;
	}

	m_curPage = m_initAnchorPage;
	CCfgDlg::SetRefreshView( false );

#endif

	//create tip
	CreateTipWnd();
}

template <class T1, class T2, class T3, class T4>
void CCfgManDlg4<T1,T2,T3,T4>::OnDestroy()
{
	CCfgDlg::OnDestroy();
	m_strNameVec.clear();
}

template <class T1, class T2, class T3, class T4>
void CCfgManDlg4<T1,T2,T3,T4>::GetItemList(CFGITEM_LIST& itemList) const
{
	if( m_callback )
	{
		m_callback( itemList );
	}
}

template <class T1, class T2, class T3, class T4>
unsigned long CCfgManDlg4<T1,T2,T3,T4>::OnApply()
{
	//Load default
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	unsigned long ret = OnLoadDefault(pCfgDlg);

	if( pCfgDlg->IsModify() )
	{
		EnableOkBtn(true,false);
	}
	else
	{
		EnableOkBtn(false,false);
	}

	ENUM_STRING_ID strID = ret ? static_cast<ENUM_STRING_ID>(ret) : E_STR_ID_TIP_00_03;
	MessageOut(strID,TIP_MSG,false);

	//一次重绘全部
	Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T1, class T2, class T3, class T4>
unsigned long CCfgManDlg4<T1,T2,T3,T4>::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

template <class T1, class T2, class T3, class T4>
CCfgDlg* CCfgManDlg4<T1,T2,T3,T4>::GetPage(PAGE_ITEM item)
{
	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_1:
		pCfgDlg = &m_cfgT1;
		break;
	case PAGE_2:
		pCfgDlg = &m_cfgT2;
		break;
	case PAGE_3:
		pCfgDlg = &m_cfgT3;
		break;
	case PAGE_4:
		pCfgDlg = &m_cfgT4;
		break;
	default:
		assert(false);
		break;
	}
	return pCfgDlg;
}

template <class T1, class T2, class T3, class T4>
void CCfgManDlg4<T1,T2,T3,T4>::SetAnchorPage(PAGE_ITEM item)
{
	m_initAnchorPage = item;
}

template <class T1, class T2, class T3, class T4>
unsigned long CCfgManDlg4<T1,T2,T3,T4>::OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (KEY_VIRTUAL_MSG_STOP == CCfgDlg::OnCommand (ctrlID, keyID, xPos, yPos)) 
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//page1
	if ((ctrlID == m_cfgT1.GetID()) && ((keyID == KEY_VIRTUAL_ENTER) || (keyID == KEY_VIRTUAL_DBCLICK)))
	{
		keyID = OnClickPage1();
		if (KEY_VIRTUAL_MSG_STOP == keyID) return KEY_VIRTUAL_MSG_STOP;
	}

	//page2
	if ((ctrlID == m_cfgT2.GetID()) && ((keyID == KEY_VIRTUAL_ENTER) || (keyID == KEY_VIRTUAL_DBCLICK)))
	{
		keyID = OnClickPage2();
		if (KEY_VIRTUAL_MSG_STOP == keyID) return KEY_VIRTUAL_MSG_STOP;
	}

	//page3
	if ((ctrlID == m_cfgT3.GetID()) && ((keyID == KEY_VIRTUAL_ENTER) || (keyID == KEY_VIRTUAL_DBCLICK)))
	{
		keyID = OnClickPage3();
		if (KEY_VIRTUAL_MSG_STOP == keyID) return KEY_VIRTUAL_MSG_STOP;
	}

	//page4
	if ((ctrlID == m_cfgT4.GetID()) && ((keyID == KEY_VIRTUAL_ENTER) || (keyID == KEY_VIRTUAL_DBCLICK)))
	{
		keyID = OnClickPage4();
		if (KEY_VIRTUAL_MSG_STOP == keyID) return KEY_VIRTUAL_MSG_STOP;
	}

	////消息响应结束
	return keyID;
}

template <class T1, class T2, class T3, class T4>
unsigned long CCfgManDlg4<T1,T2,T3,T4>::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		if (m_curPage != m_tabCtrl.GetFocusItem())
		{
			unsigned long ret = 0;
			if (GetPage(m_curPage)->IsModify())
			{
				ret = TrySaveChange(GetPage(m_curPage),true,false,false);
			}

			if ( ret == 0 )
			{
				m_curPage = static_cast<PAGE_ITEM>(m_tabCtrl.GetFocusItem());
			}
			else
			{
				Repaint();

				return KEY_VIRTUAL_MSG_STOP;
			}
		}	
	}

	return keyID;
}

template <class T1, class T2, class T3, class T4>
unsigned long CCfgManDlg4<T1,T2,T3,T4>::OnClickPage1()
{
	if (m_cfgT1.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T1, class T2, class T3, class T4>
unsigned long CCfgManDlg4<T1,T2,T3,T4>::OnClickPage2()
{
	if (m_cfgT2.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T1, class T2, class T3, class T4>
unsigned long CCfgManDlg4<T1,T2,T3,T4>::OnClickPage3()
{
	if (m_cfgT3.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

template <class T1, class T2, class T3, class T4>
unsigned long CCfgManDlg4<T1,T2,T3,T4>::OnClickPage4()
{
	if (m_cfgT4.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

#endif
