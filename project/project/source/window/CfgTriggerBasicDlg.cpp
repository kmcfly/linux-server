/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-01-20
** Name         : CfgTriggerBasicDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgTriggerBasicDlg.h"
#include "CfgTriggerDlg.h"
#include "FuncCustom.h"

CCfgTriggerBasicDlg::CCfgTriggerBasicDlg() : m_pTriggerInfo(NULL)
{

}

CCfgTriggerBasicDlg::~CCfgTriggerBasicDlg()
{

}

void CCfgTriggerBasicDlg::OnInitial()
{
	m_vecChnn.clear();
	m_vecChnnAll.clear();

	GUI::CScrollDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CScrollWnd& wnd = GetScrollWnd();

	unsigned short border = 2;
	unsigned long width = m_rect.Width() - (border + m_length[DLG_OFFSET_SUB_LEFT]) * 2;
	unsigned long height= m_length[LIST_ITEM_HEIGHT];

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;
	//设置滚动窗口的边框和可视区域
	SetDlgBorder(border);
	SetScrollWndPos(border + m_length[DLG_OFFSET_SUB_LEFT], border + m_length[DLG_OFFSET_SUB_TOP], border + m_length[DLG_OFFSET_SUB_LEFT], border + m_length[DLG_OFFSET_SUB_BOTTOM]);

	//设置滚动窗口单元格大小和最大视图区域
	wnd.SetSizeOffset(width / 2, height);
	wnd.SetViewMaxSize(width, height * 4);

	//调整控件的偏移值
	unsigned char offset_y = m_length[LIST_ITEM_YPOS];
	unsigned char offset_x = m_length[LIST_ITEM_XPOS];

	unsigned long xx = 0, yy = 0, cx = width / 2 - 2 * offset_x, cy = height;
	int row = 0;
	GUI::CRect rect;
	int xFocus = 0, yFocus = 0;
		 
	//buzzer check
	row = ROW_BUZZER;
	wnd.GetViewPos( 0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	GUI::CWnd* pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_BUZZER), false);

	rect.SetRect(0, 0, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT]);
	wnd.GetViewPos(1, row, xx, yy);
	rect.Offset(xx + offset_x, yy + offset_y);
	wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, 0, yFocus++, SW_ITEM_ID_BUZZER);

	//full combox
	row += 1;
	char strtmp[64] = {0};
	wnd.GetViewPos( 0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
	pWnd->SetCaption(m_pStrTable->String( E_STR_ID_FULL_SCREEN ), false);

	rect.SetRect(0, 0, 120, m_length[COMBO_HEIGHT]);
	wnd.GetViewPos(1, row, xx, yy);
	rect.Offset(xx + offset_x, yy + 1);
	GUI::CComboCtrl*pCombo = dynamic_cast<GUI::CComboCtrl*>(wnd.AddWnd(WND_TYPE_COMBO, GetRandWndID(), rect, 0, yFocus++, SW_ITEM_ID_FULL_SCREEN));
	for( int i = 0; i < maxChnn; i++ )
	{
		memset(strtmp, 0 , sizeof(strtmp));	
		if((CH >> i) & 0x1)
		{
			if(i < localVideoInputNum)
			{
				sprintf(strtmp, "%d", i + 1);
			}
			else
			{
				sprintf(strtmp, "%d", i + 1 /*- localVideoInputNum*/);
			}
			pCombo->AddItem( strtmp, i );
		}
	}
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_NONE), -1);
	pCombo->SetCurItemData(-1, false);

	//alarm out
	if (IsSupportAlarmOut())
	{
		row += 1;
		wnd.GetViewPos(0, row, xx, yy);
		rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
		rect.Offset(xx + offset_x, yy + offset_y);
		pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
		pWnd->SetCaption(m_pStrTable->String( E_STR_ID_TO_ALARM_OUT ), false);
		int numAlarmOut = CCfgDlg::s_loginSuccessReply.productInfo.relayOutputNum;

		//由于报警输出的个数是不确定的,所以其ID是可变的
		xFocus = 0;
		rect.SetRect(0, 0, m_length[CHECK_WIDTH] + 24, m_length[CHECK_HEIGHT]);
		wnd.GetViewPos(1, row, xx, yy);
		rect.Offset(xx + offset_x - m_length[CHECK_WIDTH] - 24, yy + offset_y);
		for (int i = 0; i < numAlarmOut; i++)
		{
			memset(strtmp, 0 , sizeof(strtmp));	
			rect.Offset(m_length[CHECK_WIDTH] + 24, 0);
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, xFocus++, yFocus, i + SW_ITEM_ID_ALARM_OUT_0));
			sprintf(strtmp, "%d", i + 1);
			pCheck->SetCaption(strtmp, false, false);
		}
		yFocus += 1;
	}

#ifdef __DVR_BASIC__	
#if !defined(__TW01_RILI__)	
	//email setup
	row += 1;
	wnd.GetViewPos( 0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_EMAIL), false);

	rect.SetRect(0, 0, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT]);
	wnd.GetViewPos(1, row, xx, yy);
	rect.Offset(xx + offset_x, yy + offset_y);
	wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, 0, yFocus++, SW_ITEM_ID_EMAIL_EN);
#endif
#endif

	if((GUI::CWnd::m_width < 1024) && (GUI::CWnd::m_height < 768) && (maxChnn >= 32))
	{
		//高度不够,不偏移
	}
	else
	{
		row++;
	}

	//snap
	row++;
	wnd.GetViewPos(0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	if (CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum > 0)
	{
		pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
		pWnd->SetCaption(m_pStrTable->String(E_STR_ID_SNAP));
	}
	
	int chnnWidth = ((cx / 5) > (m_length[CHECK_WIDTH] + 38)) ? (cx / 5) : (m_length[CHECK_WIDTH] + 38);
	int groupWidth = (chnnWidth > (m_length[CHECK_WIDTH] + 16)) ? (m_length[CHECK_WIDTH] + 16) : chnnWidth;

	int group = 0;
	xFocus = 0;
	yFocus--;
	for(int i = 0; i < CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum; i++)
	{
		if(0 == (i % 4))
		{
			if(0 == (group % 2))
			{
				row++;
				yFocus++;
				xFocus = 0;
			}

			wnd.GetViewPos(group % 2, row, xx, yy);
			rect.SetRect(0, 0, groupWidth, m_length[CHECK_HEIGHT]);
			rect.Offset(xx + offset_x, yy + offset_y);

			pWnd = wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, xFocus++, yFocus, SW_ITEM_ID_SNAP_CH_GROUP + group);
			rect.Offset(groupWidth, 0);
			rect.SetSize(chnnWidth, m_length[CHECK_HEIGHT]);

			assert(pWnd);
			m_vecChnnAll.push_back(dynamic_cast<GUI::CCheckCtrl*>(pWnd));

			group++;
		}

		pWnd = wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, xFocus++, yFocus, SW_ITEM_ID_SNAP_CH_0 + i);
		rect.Offset(chnnWidth, 0);

		assert(pWnd);
		m_vecChnn.push_back(dynamic_cast<GUI::CCheckCtrl*>(pWnd));

		if(i < localVideoInputNum)
		{
			snprintf(strtmp, sizeof(strtmp), "%d", i + 1);
		}
		else
		{
			snprintf(strtmp, sizeof(strtmp), "%d", i + 1 /*- localVideoInputNum*/);
		}

		pWnd->SetCaption(strtmp, false);

		if(!((CH >> i) & 0x1))
		{
			pWnd->SetEnable(false, false);
		}
	}
}

void CCfgTriggerBasicDlg::OnDestroy()
{
	GUI::CScrollDialog::OnDestroy();

	m_vecChnn.clear();
	m_vecChnnAll.clear();
}

void CCfgTriggerBasicDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CScrollDialog::OnPaint(status);
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_SELECT, 2);

	GUI::CScrollWnd& wnd = GetScrollWnd();
	unsigned long maxCol = 0, maxRow = 0;
	wnd.GetPageCount(maxCol, maxRow);

	unsigned long x_left = m_rect.m_left + 2 + m_length[DLG_OFFSET_SUB_LEFT], y_top = m_rect.m_top + 2 + m_length[DLG_OFFSET_SUB_TOP];
	unsigned long center_x = 0;
	unsigned long width = 0;
	unsigned long height = m_rect.Height() - m_length[DLG_OFFSET_SUB_TOP] - m_length[DLG_OFFSET_SUB_BOTTOM] - 4;

	unsigned long xx = 0, yy = 0, cx = 0, cy = 0;
	wnd.GetViewPos(1, 0, xx, yy);
	center_x = x_left + xx;

	wnd.GetViewSize(1, 0, cx, cy);
	width = xx + cx;				//如果设置的行数大于maxRow,还需预留滚动条的位置

	unsigned long x = x_left, y = y_top - 2;
	for (int i = 0; i < maxRow - 1; i++)
	{
		y += cy;
		CWnd::DrawLine(x, y, x + width, y, COLOR_LINE_GRAY);
	}

	CWnd::DrawLine(center_x, y_top, center_x, y_top + height, COLOR_LINE_GRAY);
}

void CCfgTriggerBasicDlg::InitCtrlData(bool bRefresh)
{
	gui_assert(m_pTriggerInfo, printf("Set function \"SetAlarmTriggerInfo()\" first.\n"));
	GUI::CScrollWnd& wnd = GetScrollWnd();

	GUI::CCheckCtrl*pCkBuzzer = dynamic_cast<GUI::CCheckCtrl*>( wnd.GetWnd(SW_ITEM_ID_BUZZER) );
	GUI::CComboCtrl* pCkFullScreen = dynamic_cast<GUI::CComboCtrl*>( wnd.GetWnd(SW_ITEM_ID_FULL_SCREEN) );
	assert( pCkBuzzer );
	assert( pCkFullScreen );

	//蜂鸣器
	if( m_pTriggerInfo->Buzzer != 0 )
	{
		pCkBuzzer->SetCheck(true, false);
	}

	//大画面
	LONGLONG fullScreen = -1;
	for ( int i = 0; i < CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum; i++ )
	{
		if ( (m_pTriggerInfo->FullScreen >> i) & 0x1 )
		{
			fullScreen = i;
			break;
		}
	}
	pCkFullScreen->SetCurItemData(fullScreen, false);

	if(pCkFullScreen->GetCurItemData() != fullScreen)
	{
		pCkFullScreen->SetCurItemData(-1, false);
	}

	//寄电器报警输出
	if (IsSupportAlarmOut())
	{
		int alarmOutNum = CCfgDlg::s_loginSuccessReply.productInfo.relayOutputNum;
		for ( int i = 0; i < alarmOutNum; i++ )
		{
			GUI::CCheckCtrl* pCkAlarmOut = dynamic_cast<GUI::CCheckCtrl*>( wnd.GetWnd(i + SW_ITEM_ID_ALARM_OUT_0) );
			assert(pCkAlarmOut);
			if( (m_pTriggerInfo->AlarmOut >> i) & 0x1 )
			{
				pCkAlarmOut->SetCheck(true, false);
			}
			else
			{
				pCkAlarmOut->SetCheck(false, false);
			}
		}
	}

#ifdef __DVR_BASIC__	
#if !defined(__TW01_RILI__)
	//邮件功能开关
	GUI::CCheckCtrl* pCkEmailEn = dynamic_cast<GUI::CCheckCtrl*>( wnd.GetWnd(SW_ITEM_ID_EMAIL_EN));
	if(m_pTriggerInfo->EmailEnable)
	{
		pCkEmailEn->SetCheck(true,false);
	}
	else
	{
		pCkEmailEn->SetCheck(false,false);
	}
	
#endif
#endif

	//抓图通道
	for(int i = 0; i < CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum; i++)
	{
		if((m_pTriggerInfo->SnapCH >> i) & 0x1)
		{
			m_vecChnn[i]->SetCheck(true, false);
		}
		else
		{
			m_vecChnn[i]->SetCheck(false, false);
		}
	}

	//初始设置All是否被选中
	for(int i = 0; i < m_vecChnn.size(); i++)
	{
		if(0 == (i % 4))
		{
			OnClickChnn(i, false, false);
		}
	}

	if( bRefresh )
	{
		Repaint();
	}
}

void CCfgTriggerBasicDlg::OnClickChnn(int index, bool setValue, bool bRefresh)
{
	if (index >= m_vecChnn.size()) 
	{
		return;
	}

	int group = index / 4;
	assert( group < m_vecChnnAll.size() );

	//设置Check All的状态
	bool check = true;
	for ( int i = group * 4; i < group * 4 + 4; i++ )
	{
		if( i < m_vecChnn.size() )
		{
			if ( !m_vecChnn[i]->IsCheck() )
			{
				check = false;
				break;
			}
		}
	}
	m_vecChnnAll[group]->SetCheck(check, bRefresh);

	//修改内部变量的值
	if( setValue )
	{
		if( m_vecChnn[index]->IsCheck() )
		{
			m_pTriggerInfo->SnapCH |= ( static_cast<ULONGLONG>(0x1) << index );
		}
		else
		{
			m_pTriggerInfo->SnapCH &= ( ~(static_cast<ULONGLONG>(0x1) << index) );
		}
	}
}

void CCfgTriggerBasicDlg::OnClickChnnAll(int index, bool setValue, bool bRefresh)
{
	if( index >= m_vecChnnAll.size() ) return;

	bool check = m_vecChnnAll[index]->IsCheck();
	for ( int i = index * 4; i < index * 4 + 4; i++ )
	{
		if( i < m_vecChnn.size() )
		{
			m_vecChnn[i]->SetCheck(check, bRefresh);

			//修改内部变量的值
			if( setValue )
			{
				if( m_vecChnn[i]->IsCheck() )
				{
					m_pTriggerInfo->SnapCH |= ( static_cast<ULONGLONG>(0x1) << i );
				}
				else
				{
					m_pTriggerInfo->SnapCH &= ( ~(static_cast<ULONGLONG>(0x1) << i) );
				}
			}
		}
	}
}

BEGIN_MSG_MAP(CCfgTriggerBasicDlg, GUI::CScrollDialog)
	ON_MSG_EX(GetScrollWnd().GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgTriggerBasicDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	GUI::CScrollWnd& wnd = GetScrollWnd();
	
	int index = -1;
	unsigned long tmpIndex = (keyID >> 20) & 0x7FF;
	if( tmpIndex < 0x7FF )
	{
		index = tmpIndex;
	}
	unsigned long id = keyID & 0xFFFFF;

	//printf("(%s,line=%d)index=%d\n",__FILE__,__LINE__,index);

	if( index >= 0 )
	{
		//蜂鸣器
		if( SW_ITEM_ID_BUZZER == index)
		{
			GUI::CCheckCtrl *pckeck = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_BUZZER));
			assert(pckeck);
			if( pckeck->IsCheck() )
			{
				m_pTriggerInfo->Buzzer = 1;
			}
			else
			{
				m_pTriggerInfo->Buzzer = 0;
			}

			return KEY_VIRTUAL_MSG_STOP;
		}
		//大画面
		if(SW_ITEM_ID_FULL_SCREEN == index)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( wnd.GetWnd(SW_ITEM_ID_FULL_SCREEN) );
		
			LONGLONG chnn = pCombo->GetCurItemData();
			if( chnn >=0 && chnn < CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum )
			{
				m_pTriggerInfo->FullScreen = ( static_cast<ULONGLONG>(0x1) << chnn );
			}
			else
			{
				m_pTriggerInfo->FullScreen = 0;
			}

			return KEY_VIRTUAL_MSG_STOP;
		}

		//寄电器报警输出
		int alarmOutNum = CCfgDlg::s_loginSuccessReply.productInfo.relayOutputNum;
		if((index >= SW_ITEM_ID_ALARM_OUT_0) && (index <= (SW_ITEM_ID_ALARM_OUT_0 + alarmOutNum)))
		{
			GUI::CCheckCtrl* pCkAlarmOut = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(index));
			assert(pCkAlarmOut);
			if( pCkAlarmOut->IsCheck() )
			{
				m_pTriggerInfo->AlarmOut |= (static_cast<ULONGLONG>(0x1) << (index - SW_ITEM_ID_ALARM_OUT_0));
			}
			else
			{
				m_pTriggerInfo->AlarmOut &= (~(static_cast<ULONGLONG>(0x1) << (index - SW_ITEM_ID_ALARM_OUT_0)));
			}
	
			return KEY_VIRTUAL_MSG_STOP;
		}
#ifdef __DVR_BASIC__	
#if !defined(__TW01_RILI__)
		//email 使能
		if( index == SW_ITEM_ID_EMAIL_EN )
		{
			GUI::CCheckCtrl* pEmailEn = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_EMAIL_EN));
			assert(pEmailEn);
			if(pEmailEn->IsCheck())
			{
				m_pTriggerInfo->EmailEnable = 1;
			}
			else
			{
				m_pTriggerInfo->EmailEnable = 0;
			}
			return KEY_VIRTUAL_MSG_STOP;
		}
#endif
#endif

		//抓图通道
		int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
		if((SW_ITEM_ID_SNAP_CH_GROUP <= index) && ((SW_ITEM_ID_SNAP_CH_GROUP + (numChnn + 3) / 4) > index))
		{
			OnClickChnnAll(index - SW_ITEM_ID_SNAP_CH_GROUP, true, true);
			return KEY_VIRTUAL_MSG_STOP;
		}

		if((SW_ITEM_ID_SNAP_CH_0 <= index) && ((SW_ITEM_ID_SNAP_CH_0 + numChnn) > index))
		{
			OnClickChnn(index - SW_ITEM_ID_SNAP_CH_0, true, true);
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

//end
