/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-15
** Name         : CfgRecordTimeDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgRecordTimeDlg.h"

#include "FuncCustom.h"

CCfgRecordTimeDlg::CCfgRecordTimeDlg()
{

}

CCfgRecordTimeDlg::~CCfgRecordTimeDlg()
{

}

void CCfgRecordTimeDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	GUI::CListCtrl& list = m_cfgList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CButton* pBtn = NULL;
	GUI::CEditCtrl* pEdit = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[LIST_ITEM_HEIGHT] - m_length[DLG_OFFSET_SUB_BOTTOM];

	/////List
	/////预算List边缘是否需要滚动框
	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头
	
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	int row = 0;
	for(int i = 0; i < maxChnn; i++)
	{
		if((CH >> i) & 0x1)
		{
			m_rowChnnMap.insert(std::make_pair(row++, i));
		}
	}

	if(effectChnn > maxListRow)
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
	list.AddColumn(m_pStrTable->String( E_STR_ID_PRE_RECORD_TIME ), width * 11 / 32 - 12);
	list.AddColumn(m_pStrTable->String( E_STR_ID_POST_RECORD_TIME ), width * 11 / 32 - 7);
	list.AddColumn(m_pStrTable->String(E_STR_ID_RECORD_HOLD_TIME), width * 7 / 32 - offsetEdge + 19);

	char str[128] = {0};
	
	for ( int i = 0; i < effectChnn; i++ )
	{
		int chnnIndex = m_rowChnnMap.find(i)->second;
		if(chnnIndex < localVideoInputNum)
		{
			sprintf(str, "%d", chnnIndex + 1);
		}
		else
		{
			sprintf(str, "%d", chnnIndex + 1 /*- localVideoInputNum*/);
		}
		list.AddItem( i, COL_CHNN, str );

		pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( i, COL_PRE_RECORD_TIME, WND_TYPE_COMBO) );
		assert( pCombo );
		
		pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( i, COL_POST_RECORD_TIME, WND_TYPE_COMBO) );
		assert( pCombo );
		strcpy( str, "10" );
		pCombo->AddItem( str , 10 );
		strcpy( str, "15" );
		pCombo->AddItem( str , 15 );
		strcpy( str, "20" );
		pCombo->AddItem( str , 20 );
		strcpy( str, "30" );
		pCombo->AddItem( str , 30 );
		strcpy( str, "60" );
		pCombo->AddItem( str , 60 );
		strcpy( str, "2X60" );
		pCombo->AddItem( str , 120 );
		strcpy( str, "3X60" );
		pCombo->AddItem( str , 180 );
		strcpy( str, "5X60" );
		pCombo->AddItem( str , 300 );
		pCombo->SetCurItemData( 10, false );

		pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem(i,COL_HOLD_TIME,WND_TYPE_COMBO));
		assert(pCombo);
#ifndef __CUSTOM_RU04__
		for (int i=1;i<=60;i++)
		{
			sprintf(str,"%d",i);
			pCombo->AddItem(str,i);
		}
#endif
		pCombo->AddItem(m_pStrTable->String(E_STR_ID_REC_NEVER), 0);
	}
	list.CreateListFocusMap();

	GUI::CRect rect = list.GetWindowRect();
	y += rect.Height() + 4;

	m_cfgScAll.Create( GUI::CWnd::GetRandWndID(), x, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, width, m_length[STATIC_HEIGHT], this );
	snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_ALL ) );
	m_cfgScAll.SetCaption( str, false );

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y += m_length[LIST_ITEM_HEIGHT];
	GUI::CListCtrl& listAll = m_cfgListAll;
	listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 2, false );
	listAll.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
	listAll.AddColumn(m_pStrTable->String(E_STR_ID_PRE_RECORD_TIME), width * 11 / 32 - 12);
	listAll.AddColumn(m_pStrTable->String(E_STR_ID_POST_RECORD_TIME), width * 11 / 32 - 7);
	listAll.AddColumn(m_pStrTable->String(E_STR_ID_RECORD_HOLD_TIME), width * 7 / 32 + 19);
	{
		int i = 0;

		pCheck = dynamic_cast<GUI::CCheckCtrl*>( listAll.AddItem( i, COL_CHNN, WND_TYPE_CHECK) );
		assert( pCheck );

		pCombo = dynamic_cast<GUI::CComboCtrl*>( listAll.AddItem( i, COL_PRE_RECORD_TIME, WND_TYPE_COMBO) );
		assert( pCombo );
		
		pCombo = dynamic_cast<GUI::CComboCtrl*>( listAll.AddItem( i, COL_POST_RECORD_TIME, WND_TYPE_COMBO) );
		assert( pCombo );
		strcpy( str, "10" );
		pCombo->AddItem( str , 10 );
		strcpy( str, "15" );
		pCombo->AddItem( str , 15 );
		strcpy( str, "20" );
		pCombo->AddItem( str , 20 );
		strcpy( str, "30" );
		pCombo->AddItem( str , 30 );
		strcpy( str, "60" );
		pCombo->AddItem( str , 60 );
		strcpy( str, "2X60" );
		pCombo->AddItem( str , 120 );
		strcpy( str, "3X60" );
		pCombo->AddItem( str , 180 );
		strcpy( str, "5X60" );
		pCombo->AddItem( str , 300 );
		pCombo->SetCurItemData( 30, false );

		pCombo = dynamic_cast<GUI::CComboCtrl*>( listAll.AddItem(i,COL_HOLD_TIME,WND_TYPE_COMBO));
		assert(pCombo);
#ifndef __CUSTOM_RU04__
		for (int i=1;i<=60;i++)
		{
			sprintf(str,"%d",i);
			pCombo->AddItem(str,i);
		}
#endif
		pCombo->AddItem(m_pStrTable->String(E_STR_ID_REC_NEVER), 0);
		pCombo->SetCurItemData(0,false);
	}
	listAll.CreateListFocusMap();
}

void CCfgRecordTimeDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	m_rowChnnMap.clear();
}

void CCfgRecordTimeDlg::SetTip()
{
	//int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;

	ULONGLONG CH = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn);

	for(int i = 0; i < effectChnn; i++)
	{
		AddTip(m_cfgList.GetItemWnd(i,COL_HOLD_TIME)->GetID(),E_STR_ID_TIP_03_17);
	}

	AddTip(m_cfgListAll.GetItemWnd(0,COL_CHNN)->GetID(), E_STR_ID_TIP_01_39);
	AddTip(m_cfgListAll.GetItemWnd(0,COL_HOLD_TIME)->GetID(),E_STR_ID_TIP_03_17);

	#ifdef __CUSTOM_US02_NEW__
		for(int i = 0; i < effectChnn; i++)
		{
			AddTip(m_cfgList.GetItemWnd(i,COL_PRE_RECORD_TIME)->GetID(),E_STR_ID_PRE_RECORD_TIME_01);
		}
		for(int i = 0; i < effectChnn; i++)
		{
			AddTip(m_cfgList.GetItemWnd(i,COL_POST_RECORD_TIME)->GetID(),E_STR_ID_POST_RECORD_TIME_01);
		}
	#endif
}

void CCfgRecordTimeDlg::UpdateView(bool bRefresh)
{
	// 除配置外的其他数据初始化数据设置
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListAll.GetItemWnd(0, COL_CHNN));
	if( pCheckAll )
	{
		pCheckAll->SetCheck( false, bRefresh );
	}

	CCfgDlg::UpdateView( bRefresh );
}

void CCfgRecordTimeDlg::DoOther()
{
	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;
	CCfgDlg::GetConfig(NCFG_ITEM_MAX_PRE_ALARM_TIME, &pTempData, tempDataLen);
	unsigned long maxPreAlarmTime = *reinterpret_cast<unsigned long *>(pTempData);
	int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
	char str[128] = {0};
	if (maxPreAlarmTime < 5)
	{
		maxPreAlarmTime = 5;
	}

	ULONGLONG CH = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn);

	{
		GUI::CListCtrl& list = m_cfgList;
		for (int i = 0; i < effectChnn; ++i)
		{
			unsigned long preAlarmTime = 5;
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_PRE_RECORD_TIME));
			for (preAlarmTime = 5; preAlarmTime <= maxPreAlarmTime; preAlarmTime += 5)
			{
				sprintf(str, "%d", preAlarmTime);
				pCombo->AddItem( str , preAlarmTime );				
			}			
			pCombo->SetCurItemData(5, false );
		}		
	}

	{
		GUI::CListCtrl& list = m_cfgListAll;
		unsigned long preAlarmTime = 5;
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(0, COL_PRE_RECORD_TIME));
		for (preAlarmTime = 5; preAlarmTime <= maxPreAlarmTime; preAlarmTime += 5)
		{
			sprintf(str, "%d", preAlarmTime);
			pCombo->AddItem( str , preAlarmTime );		
		}		
		pCombo->SetCurItemData(maxPreAlarmTime, false );
	}	
}

bool CCfgRecordTimeDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_cfgList;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_PRE_ALARM_REC_TIME:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );

			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			for ( int i = 0; i < effectChnn; i++ )
			{
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( list.GetItemWnd( i, COL_PRE_RECORD_TIME) );
				assert( pCombo );
				pCombo->SetCurItemData( pValue[m_rowChnnMap.find(i)->second], bRefresh );
			}
		}
		return true;
		break;
	case NCFG_ITEM_POST_ALARM_REC_TIME:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );

			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			for ( int i = 0; i< effectChnn; i++ )
			{
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( list.GetItemWnd( i, COL_POST_RECORD_TIME) );
				assert( pCombo );
				pCombo->SetCurItemData( pValue[m_rowChnnMap.find(i)->second], bRefresh );
			}
		}
		return true;
		break;
	case NCFG_ITEM_REC_HOLD_TIME:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );

			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			for ( int i = 0; i< effectChnn; i++ )
			{
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( list.GetItemWnd( i, COL_HOLD_TIME) );
				assert( pCombo );
				pCombo->SetCurItemData( pValue[m_rowChnnMap.find(i)->second] / ONE_DAY_SECOND_NUM, bRefresh );
			}
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

bool CCfgRecordTimeDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CListCtrl& list = m_cfgList;
	const GUI::CListCtrl& listAll = m_cfgListAll;

	bool bAll = false;
	{
		const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd( 0, COL_CHNN ) );
		assert( pCheck );
		bAll = pCheck->IsCheck();
	}

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_PRE_ALARM_REC_TIME:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );

			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			for ( int i = 0; i< effectChnn; i++ )
			{
				GUI::CComboCtrl* pCombo = NULL;
				if( bAll )
				{
					pCombo = dynamic_cast<GUI::CComboCtrl*>( (GUI::CWnd*)listAll.GetItemWnd( 0, COL_PRE_RECORD_TIME) );
				}
				else
				{
					pCombo = dynamic_cast<GUI::CComboCtrl*>( (GUI::CWnd*)list.GetItemWnd( i, COL_PRE_RECORD_TIME) );
				}
				assert( pCombo );
				pValue[m_rowChnnMap.find(i)->second] = pCombo->GetCurItemData();
			}
		}
		return true;
		break;
	case NCFG_ITEM_POST_ALARM_REC_TIME:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );

			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			for ( int i = 0; i< effectChnn; i++ )
			{
				GUI::CComboCtrl* pCombo = NULL;
				if( bAll )
				{
					pCombo = dynamic_cast<GUI::CComboCtrl*>( (GUI::CWnd*)listAll.GetItemWnd( 0, COL_POST_RECORD_TIME) );
				}
				else
				{
					pCombo = dynamic_cast<GUI::CComboCtrl*>( (GUI::CWnd*)list.GetItemWnd( i, COL_POST_RECORD_TIME) );
				}
				assert( pCombo );
				pValue[m_rowChnnMap.find(i)->second] = pCombo->GetCurItemData();
			}
		}
		return true;
		break;
	case NCFG_ITEM_REC_HOLD_TIME:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );

			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			for ( int i = 0; i< effectChnn; i++ )
			{
				GUI::CComboCtrl* pCombo = NULL;
				if( bAll )
				{
					pCombo = dynamic_cast<GUI::CComboCtrl*>( (GUI::CWnd*)listAll.GetItemWnd( 0, COL_HOLD_TIME) );
				}
				else
				{
					pCombo = dynamic_cast<GUI::CComboCtrl*>( (GUI::CWnd*)list.GetItemWnd( i, COL_HOLD_TIME) );
				}
				assert( pCombo );
				pValue[m_rowChnnMap.find(i)->second] = pCombo->GetCurItemData() * ONE_DAY_SECOND_NUM;
			}
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

BEGIN_MSG_MAP(CCfgRecordTimeDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_cfgListAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()

unsigned long CCfgRecordTimeDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	//2010-03-03 17:57:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListAll.GetItemWnd(0, COL_CHNN));
	assert(pCheckAll);
	if( pCheckAll->IsCheck() )
	{
		pCheckAll->SetCheck(false, true);
	}

	if (!IsModify())
	{

		switch (col)
		{
		case COL_PRE_RECORD_TIME:
			if(IsChange( NCFG_ITEM_PRE_ALARM_REC_TIME ))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case COL_POST_RECORD_TIME:
			if(IsChange(NCFG_ITEM_POST_ALARM_REC_TIME))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case COL_HOLD_TIME:
			if(IsChange(NCFG_ITEM_REC_HOLD_TIME))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordTimeDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	// 点选非All，先检查是否处于All状态，处于则认为做了修改
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListAll.GetItemWnd(0, COL_CHNN));
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCheckAll );
	assert( pCfgDlg );
	if( pCheckAll->IsCheck() )
	{
		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			if( COL_PRE_RECORD_TIME == col ||
				COL_POST_RECORD_TIME == col ||
				COL_HOLD_TIME == col )
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
	}
	else
	{
		pCfgDlg->SetRefreshView( false );
	}

	// 点选All 直接认为做了修改
	if (!IsModify())
	{
		if( COL_CHNN == col )
		{
			SetModify(true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;	
}

//end
