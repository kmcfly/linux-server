/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-01-20
** Name         : CfgTriggerToRecordDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgTriggerToRecordDlg.h"
#include "FuncCustom.h"

CCfgTriggerToRecordDlg::CCfgTriggerToRecordDlg() : m_pTriggerInfo(NULL)
{

}

CCfgTriggerToRecordDlg::~CCfgTriggerToRecordDlg()
{

}

void CCfgTriggerToRecordDlg::OnInitial()
{
	m_vecChnn.clear();
	m_vecChnnAll.clear();

	//////////////////////////////////////////////////////////////////////////

	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	GUI::CListCtrl& list = m_listCtrl;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	for ( int i = 0; i < 2; i++ )
	{
		list.AddColumn("", width * 3/64 + 6);
		list.AddColumn("", width * 7/64);
		list.AddColumn("", width * 7/64);
		list.AddColumn("", width * 7/64);
		list.AddColumn("", width * 8/64 - 6);
	}

	int numChnn = maxChnn;  //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
	char str[32] = {0};

	unsigned short cx = 0, cy = 0;
	strcpy(str, "D888");
	TextSize(str, strlen(str), cx, cy);
	int row = 0;
	int col = 0;
	for ( int i = 0; i < numChnn; i++ )
	{
		if( i % 8 == 0 && i != 0 )
		{
			row += 1;
			col = 0;
		}

		if( i % 4 == 0 )
		{
			GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(row, col++, WND_TYPE_CHECK, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT]));
			assert(pCheckAll);
			m_vecChnnAll.push_back(pCheckAll);
		}

		sprintf(str, "%d", i+1);

		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(row, col++, WND_TYPE_CHECK, m_length[CHECK_WIDTH] + cx, m_length[CHECK_HEIGHT]));
		assert(pCheck);
		m_vecChnn.push_back(pCheck);
		pCheck->SetCaption(str, false, false);

		if(!((CH >> i) & 0x1))
		{
			pCheck->SetEnable(false, false);
		}
	}

	/////////////////////////
	//调整列表框绘制方式
	list.DrawLine( false );
	int itemCount  = list.GetItemCount();
	int pageCount = height / m_length[LIST_ITEM_HEIGHT];
	int maxCount = (itemCount > pageCount) ? itemCount : pageCount;
	for ( int i = 0; i < maxCount; i++ )
	{
		list.DrawLine( i, 1 );
	}

	list.CreateListFocusMap();
}

void CCfgTriggerToRecordDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	m_vecChnn.clear();
	m_vecChnnAll.clear();
}

void CCfgTriggerToRecordDlg::InitCtrlData(bool bRefresh)
{
	assert(m_vecChnn.size() == CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum);

	for ( int i = 0; i < m_vecChnn.size(); i++ )
	{
		if( (m_pTriggerInfo->Record >> i) & 0x1 )
		{
			m_vecChnn[i]->SetCheck(true, bRefresh);
		}
		else
		{
			m_vecChnn[i]->SetCheck(false, bRefresh);
		}
	}

	//初始设置All是否被选中
	for ( int i = 0; i < m_vecChnn.size(); i++ )
	{
		if( i % 4 == 0 )
		{
			OnClickChnn(i, false, bRefresh);
		}
	}
}

void CCfgTriggerToRecordDlg::OnClickChnn(int index, bool setValue, bool bRefresh)
{
	if ( index >= m_vecChnn.size() ) return;

	int group = index / 4;
	assert( group < m_vecChnnAll.size() );

	//设置Check All的状态
	bool check = true;
	for ( int i = group * 4; i < group * 4 + 4; i++ )
	{
		if( i < m_vecChnn.size() )
		{
			if( !m_vecChnn[i]->IsCheck() )
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
			m_pTriggerInfo->Record |= ( static_cast<ULONGLONG>(0x1) << index );
		}
		else
		{
			m_pTriggerInfo->Record &= ( ~(static_cast<ULONGLONG>(0x1) << index) );
		}
	}
}

void CCfgTriggerToRecordDlg::OnClickChnnAll(int index, bool setValue, bool bRefresh)
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
					m_pTriggerInfo->Record |= ( static_cast<ULONGLONG>(0x1) << i );
				}
				else
				{
					m_pTriggerInfo->Record &= ( ~(static_cast<ULONGLONG>(0x1) << i) );
				}
			}
		}
	}
}

BEGIN_MSG_MAP(CCfgTriggerToRecordDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgTriggerToRecordDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if( col == 0 )
	{
		//选中了All
		int index = row * 2;

		OnClickChnnAll(index, true, true);
	}
	else if( col == 5 )
	{
		//选中了All
		int index = row * 2 + 1;

		OnClickChnnAll(index, true, true);
	}
	else if( col < 10 )
	{
		//选中了非All
		int index = row * 8 + col;
		index = (col > 5) ? (index - 2) : (index - 1);

		OnClickChnn(index, true, true);
	}
	else
	{
		assert(false);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
