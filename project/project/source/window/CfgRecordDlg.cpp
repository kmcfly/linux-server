/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-12
** Name         : CfgRecordDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgRecordDlg.h"
#include "FuncCustom.h"

CCfgRecordDlg::CCfgRecordDlg()
{

}

CCfgRecordDlg::~CCfgRecordDlg()
{

}

void CCfgRecordDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgRecordList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
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
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3/32);

	if (CProduct::Instance()->SupportRedunRec())
	{
		list.AddColumn(m_pStrTable->String(E_STR_ID_RECORD), width * 6/32);
		list.AddColumn(m_pStrTable->String(E_STR_ID_AUDIO), width * 6/32);
		list.AddColumn(m_pStrTable->String(E_STR_ID_REC_REDUNDANT),width * 17/32 - offsetEdge);
	}
	else
	{
		list.AddColumn(m_pStrTable->String(E_STR_ID_RECORD), width * 9/32);
		list.AddColumn(m_pStrTable->String(E_STR_ID_AUDIO), width * 20/32 - offsetEdge);
	}

	char str[64] = {0};

	int numAudio = CCfgDlg::s_loginSuccessReply.productInfo.audioInputNum;
	for(int i = 0; i < effectChnn; i++)
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
		list.AddItem(i, COL_CHNN, str);

		pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(i, COL_REC_ENABLE, WND_TYPE_CHECK));
		assert(pCheck);

		if((chnnIndex < numAudio) || (chnnIndex >= localVideoInputNum))
		{
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(i, COL_REC_AUDIO, WND_TYPE_CHECK));
			assert(pCheck);
		}
		
		if (CProduct::Instance()->SupportRedunRec())
		{
			pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem(i,COL_REDUNDANT,WND_TYPE_CHECK) );
			assert( pCheck );
		}
	}

	list.CreateListFocusMap();

	GUI::CRect rect = list.GetClientRect();
	y += rect.Height() + 4;

	{
		m_cfgSCAll.Create( GUI::CWnd::GetRandWndID(), x, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, width, m_length[STATIC_HEIGHT], this );
		snprintf( str, 64, "%s", m_pStrTable->String( E_STR_ID_ALL ) );
		m_cfgSCAll.SetCaption( str, false );

		height = m_length[LIST_ITEM_HEIGHT];
		y += height;
		GUI::CListCtrl& listAll = m_cfgRecordListAll;
		{
			listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, height , this, 0, 1, false );
			listAll.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3/32);
			if (CProduct::Instance()->SupportRedunRec())
			{
				listAll.AddColumn(m_pStrTable->String(E_STR_ID_RECORD), width * 6/32);
				listAll.AddColumn(m_pStrTable->String(E_STR_ID_AUDIO), width * 6/32);
				listAll.AddColumn(m_pStrTable->String(E_STR_ID_REC_REDUNDANT),width * 17/32);
			}
			else
			{
				listAll.AddColumn(m_pStrTable->String(E_STR_ID_RECORD), width * 9/32);
				listAll.AddColumn(m_pStrTable->String(E_STR_ID_AUDIO), width * 20/32);
			}

			{
				int row = 0;
				pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem( row, COL_CHNN, WND_TYPE_CHECK));
				assert(pCheck);

				pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem( row, COL_REC_ENABLE, WND_TYPE_CHECK));
				assert(pCheck);
				pCheck->SetCheck(true, false);

				pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem( row, COL_REC_AUDIO, WND_TYPE_CHECK));
				assert(pCheck);
				pCheck->SetCheck(true, false);
				
				if (CProduct::Instance()->SupportRedunRec())
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>( listAll.AddItem(row,COL_REDUNDANT,WND_TYPE_CHECK) );
					assert( pCheck );
				}
			}
			listAll.CreateListFocusMap();
		}
	}
}

void CCfgRecordDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	m_rowChnnMap.clear();
}

void CCfgRecordDlg::SetTip()
{
	AddTip(m_cfgRecordListAll.GetItemWnd(0,COL_CHNN)->GetID(), E_STR_ID_TIP_01_39);
}

void CCfgRecordDlg::DoOther()
{	
//#ifdef __DVR_ULTIMATE__
//	ULONGLONG CH = 0;
//	int localVideoInputNum = 0, netVideoInputNum = 0;
//	int maxChnn = 0, effectChnn = 0;

//	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

//	maxChnn = localVideoInputNum + netVideoInputNum;
	
//	int numChnn = maxChnn; //(CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum + CCfgDlg::s_loginSuccessReply.productInfo.netVideoInputNum);

//	BYTE* pTempData = NULL;
//	unsigned long tempDataLen = 0;
//	CCfgDlg::GetConfig(NCFG_ITEM_DEVICE_SUPPORT_REDUNDANCY, &pTempData, tempDataLen);
//	unsigned long bSupport = *reinterpret_cast<unsigned long *>(pTempData);
//	
//	{
//		GUI::CListCtrl& list = m_cfgRecordList;		
//		for (int i = 0; i < effectChnn; ++i)
//		{
//			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(i,COL_REDUNDANT));
//			if (!bSupport)
//			{
//				pCheck->SetEnable(false, false);
//			}
//		}
//	}
//
//	{
//		GUI::CListCtrl& list = m_cfgRecordListAll;		;
//		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(0,COL_REDUNDANT));
//		if (!bSupport)
//		{
//			pCheck->SetEnable(false, false);
//		}
//	}
//#endif
}

void CCfgRecordDlg::UpdateView(bool bRefresh)
{
	// 除配置外的其他数据初始化数据设置
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgRecordListAll.GetItemWnd(0, COL_CHNN));
	if( pCheck )
	{
		pCheck->SetCheck( false, bRefresh );
	}

	CCfgDlg::UpdateView( bRefresh );
}

bool CCfgRecordDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_cfgRecordList;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_REC_ENABLE:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			for ( int i = 0; i < effectChnn; i++ )
			{
				GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(i, COL_REC_ENABLE));
				assert( pCheck );
				pCheck->SetCheck(pValue[m_rowChnnMap.find(i)->second], bRefresh);
			}
		}
		return true;
		break;
	case NCFG_ITEM_REC_AUDIO:
		{
			int numAudio = CCfgDlg::s_loginSuccessReply.productInfo.audioInputNum;
			assert( len == sizeof(unsigned long) * (numAudio + netVideoInputNum) );
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			for ( int i = 0; i < effectChnn; i++ )
			{
				int chnn = m_rowChnnMap.find(i)->second;
				
				if(chnn < numAudio)
				{
					GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(i, COL_REC_AUDIO));
					assert( pCheck );
					pCheck->SetCheck( pValue[chnn], bRefresh );
				}
				else if(chnn >= localVideoInputNum)
				{
					GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(i, COL_REC_AUDIO));
					assert( pCheck );
					pCheck->SetCheck( pValue[chnn-localVideoInputNum+numAudio], bRefresh );
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_REC_REDUNDANT:
		{
			if (CProduct::Instance()->SupportRedunRec())
			{
				int numChnn = maxChnn;  //CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
				assert( len == sizeof(unsigned long) * numChnn );

				const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
				for ( int i = 0; i< effectChnn; i++ )
				{
					int chnn = m_rowChnnMap.find(i)->second;

					GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd( i, COL_REDUNDANT) );
					assert( pCheck );
					pCheck->SetCheck( pValue[chnn], bRefresh );
				}
			}			
		}
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool CCfgRecordDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CListCtrl& list = m_cfgRecordList;
	const GUI::CListCtrl& listAll = m_cfgRecordListAll;

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
	case NCFG_ITEM_REC_ENABLE:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			for ( int i = 0; i < effectChnn; i++ )
			{
				GUI::CCheckCtrl* pCheck = NULL;
				if( bAll )
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>( (GUI::CWnd*)listAll.GetItemWnd(0, COL_REC_ENABLE));
				}
				else
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>( (GUI::CWnd*)list.GetItemWnd(i, COL_REC_ENABLE));
				}
				assert( pCheck );
				if( pCheck->IsCheck() )
				{
					pValue[m_rowChnnMap.find(i)->second] = 1;
				}
				else
				{
					pValue[m_rowChnnMap.find(i)->second] = 0;
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_REC_AUDIO:
		{
			int numAudio = CCfgDlg::s_loginSuccessReply.productInfo.audioInputNum;
			assert( len == sizeof(unsigned long) * (numAudio+netVideoInputNum) );
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			for ( int i = 0; i < effectChnn; i++ )
			{
				int chnn = m_rowChnnMap.find(i)->second;

				if((chnn < numAudio) || (chnn >= localVideoInputNum))
				{
					if(chnn >= localVideoInputNum)
					{
						chnn = chnn - localVideoInputNum + numAudio;
					}
					GUI::CCheckCtrl* pCheck = NULL;
					if( bAll )
					{
						pCheck = dynamic_cast<GUI::CCheckCtrl*>( (GUI::CWnd*)listAll.GetItemWnd(0, COL_REC_AUDIO));
					}
					else
					{
						pCheck = dynamic_cast<GUI::CCheckCtrl*>( (GUI::CWnd*)list.GetItemWnd(i, COL_REC_AUDIO));
					}
					assert( pCheck );
					if( pCheck->IsCheck() )
					{
						pValue[chnn] = 1;
					}
					else
					{
						pValue[chnn] = 0;
					}
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_REC_REDUNDANT:
		{
			if (CProduct::Instance()->SupportRedunRec())
			{
				int numChnn = maxChnn;  //CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
				assert( len == sizeof(unsigned long) * numChnn );
				unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
				for ( int i = 0; i < effectChnn; i++ )
				{
					int chnn = m_rowChnnMap.find(i)->second;

					GUI::CCheckCtrl* pCheck = NULL;
					if( bAll )
					{
						pCheck = dynamic_cast<GUI::CCheckCtrl*>( (GUI::CWnd*)listAll.GetItemWnd(0, COL_REDUNDANT));
					}
					else
					{
						pCheck = dynamic_cast<GUI::CCheckCtrl*>( (GUI::CWnd*)list.GetItemWnd(i, COL_REDUNDANT));
					}
					assert( pCheck );
					if( pCheck->IsCheck() )
					{
						pValue[chnn] = 1;
					}
					else
					{
						pValue[chnn] = 0;
					}
				}
			}			
		}
		return true;
		break;
	default:
		break;
	}
	return false;
}


BEGIN_MSG_MAP(CCfgRecordDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgRecordList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_cfgRecordListAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()

unsigned long CCfgRecordDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	//2010-03-03 17:57:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgRecordListAll.GetItemWnd(0, COL_CHNN));
	assert(pCheckAll);
	if( pCheckAll->IsCheck() )
	{
		pCheckAll->SetCheck(false, true);
	}

	//cfg
	if (!IsModify())
	{
		switch (col)
		{
		case COL_REC_ENABLE:
			if (IsChange(NCFG_ITEM_REC_ENABLE))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case COL_REC_AUDIO:
			if(IsChange( NCFG_ITEM_REC_AUDIO ))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case COL_REDUNDANT:
			{
				if (CProduct::Instance()->SupportRedunRec())
				{
					if (IsChange(NCFG_ITEM_REC_REDUNDANT))
					{
						SetModify(true);
					}
				}			
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	// 点选非All，先检查是否处于All状态，处于则认为做了修改
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgRecordListAll.GetItemWnd(0, COL_CHNN));
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCheckAll );
	assert( pCfgDlg );
	if( pCheckAll->IsCheck() )
	{
		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			if( (COL_REC_ENABLE == col)
			  ||(COL_REC_AUDIO  == col)
			  ||(COL_REDUNDANT  == col))
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
