/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgSensorDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgSensorDlg.h"
#include "CfgTriggerDlg.h"
#include "FuncCustom.h"

CCfgSensorDlg::CCfgSensorDlg()
{

}

CCfgSensorDlg::~CCfgSensorDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CCfgSensorDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	//int height = 340;
	//int height = m_length[LIST_ITEM_HEIGHT] * 7;
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[LIST_ITEM_HEIGHT] - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头

	ULONGLONG CH = 0;
	int localSensorInNum = s_loginSuccessReply.productInfo.sensorInputNum;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);
	CH >>= localVideoInputNum;
	CH <<= localSensorInNum;
	CH |= ALL_CHNN_MASK(localSensorInNum);

	effectChnn += localSensorInNum;
	effectChnn -= localVideoInputNum;

	int sensorInputNum = localSensorInNum + netVideoInputNum;

	int row = 0;
	for(int i = 0; i < sensorInputNum; i++)
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

	m_listCtrl.Create(CTRLID_CFG_SENSOR_LIST, x, y, width, height, this, 0, 0);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_ENABLE), width * 5 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_TYPE), width * 8 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_NAME), width * 16 / 32 - offsetEdge);

	char str[32] = {0};
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl * pCombo = NULL;
	for (int ch = 0; ch < effectChnn; ++ch)
	{
		int chnnIndex = m_rowChnnMap.find(ch)->second;
		sprintf(str, "%d", chnnIndex + 1);
		m_listCtrl.AddItem(ch, COL_CHANNEL, str);

		pWnd = m_listCtrl.AddItem(ch, COL_ENABLE, WND_TYPE_CHECK);

		pCombo = reinterpret_cast<GUI::CComboCtrl*>(m_listCtrl.AddItem(ch, COL_TYPE, WND_TYPE_COMBO));
		pCombo->AddItem(m_pStrTable->String(E_STR_ID_SENSOR_NO), SENSOR_TYPE_NO);
		pCombo->AddItem(m_pStrTable->String(E_STR_ID_SENSOR_NC), SENSOR_TYPE_NC);
		pCombo->SetCurItem(0);

		pWnd = m_listCtrl.AddItem(ch, COL_NAME, WND_TYPE_EDIT);
		pWnd->SetCaption(str, false);
	}

	m_listCtrl.CreateListFocusMap();

	GUI::CRect rect = m_listCtrl.GetClientRect();
	y += rect.Height() + 4;
	
	m_scAll.Create( GUI::CWnd::GetRandWndID(), x, y + (m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT])/2, width, m_length[STATIC_HEIGHT], this );
	m_scAll.SetCaption( m_pStrTable->String(E_STR_ID_ALL), false );

	y += m_length[LIST_ITEM_HEIGHT];
	m_listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 1, false );
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_ENABLE), width * 5 / 32);
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_TYPE), width * 8 / 32);
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_NAME), width * 16 / 32);

	{
		int ch = 0;
		pWnd = m_listAll.AddItem(ch, COL_CHANNEL, WND_TYPE_CHECK);

		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.AddItem(ch, COL_ENABLE, WND_TYPE_CHECK));
		assert(pCheck);
		pCheck->SetCheck(true, false);

		pWnd = m_listAll.AddItem(ch, COL_TYPE, WND_TYPE_COMBO);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->AddItem(m_pStrTable->String(E_STR_ID_SENSOR_NO), SENSOR_TYPE_NO);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->AddItem(m_pStrTable->String(E_STR_ID_SENSOR_NC), SENSOR_TYPE_NC);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->SetCurItem(0);
	}
	m_listAll.CreateListFocusMap();
}

void CCfgSensorDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	m_rowChnnMap.clear();
}

void CCfgSensorDlg::UpdateView(bool bRefresh)
{
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	if( pCheckAll )
	{
		pCheckAll->SetCheck( false, bRefresh );
	}

	CCfgDlg::UpdateView( bRefresh );
}


bool CCfgSensorDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	ULONGLONG CH = 0;
	int localSensorInNum = s_loginSuccessReply.productInfo.sensorInputNum;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	effectChnn += localSensorInNum;
	effectChnn -= localVideoInputNum;

	int sensorInputNum = localSensorInNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_SENSOR_IN_ENABLE:
		{
			assert(len == sizeof(unsigned long) * sensorInputNum);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			
			GUI::CCheckCtrl* pCtrl = NULL;
			for (int ch = 0; ch < effectChnn; ch++)
			{
				int index = m_rowChnnMap.find(ch)->second;
				if(index < sensorInputNum)
				{
					pCtrl = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.GetItemWnd(ch, COL_ENABLE));
					assert( NULL != pCtrl );
					pCtrl->SetCheck(pValue[index], bRefresh);
				}
			}
			
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TYPE:
		{
			assert(len == sizeof(unsigned long) * sensorInputNum);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCtrl = NULL;
			for (int ch = 0; ch < effectChnn; ch++)
			{
				int index = m_rowChnnMap.find(ch)->second;
				if(index < sensorInputNum)
				{
					pCtrl = dynamic_cast<GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(ch, COL_TYPE));
					assert(NULL != pCtrl);

					pCtrl->SetCurItemData(static_cast<LONGLONG>(pValue[index]), bRefresh);
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_NAME:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME) * sensorInputNum);
			CCfgDlg::GetConfig(NCFG_ITEM_SENSOR_IN_NAME_MAX_LEN,&pTempData,tempDataLen);
			assert(tempDataLen==sizeof(unsigned long));
			unsigned long maxLen = *reinterpret_cast<unsigned long*>(pTempData);

			const NCFG_INFO_SHORT_NAME* pValue = reinterpret_cast<const NCFG_INFO_SHORT_NAME*>(pData);

			GUI::CEditCtrl* pCtrl = NULL;
			for(int ch = 0; ch < effectChnn; ch++)
			{
				int index = m_rowChnnMap.find(ch)->second;
				if(index < sensorInputNum)
				{
					pCtrl = dynamic_cast<GUI::CEditCtrl*>( m_listCtrl.GetItemWnd(ch, COL_NAME));
					assert(NULL != pCtrl );
					pCtrl->SetMaxStringLen(maxLen);
					pCtrl->SetCaption(pValue[index].name, bRefresh);
				}
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

bool CCfgSensorDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	const GUI::CListCtrl& listAll = m_listAll;

	bool bAll = false;
	{
		const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd( 0, COL_CHANNEL ) );
		assert( pCheck );
		bAll = pCheck->IsCheck();
	}

	ULONGLONG CH = 0;
	int localSensorInNum = s_loginSuccessReply.productInfo.sensorInputNum;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	effectChnn += localSensorInNum;
	effectChnn -= localVideoInputNum;

	int sensorInputNum = localSensorInNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_SENSOR_IN_ENABLE:
		{
			assert(len == sizeof(unsigned long) * sensorInputNum);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CCheckCtrl* pCtrl = NULL;
			for (int ch = 0; ch < effectChnn; ch++)
			{
				int index = m_rowChnnMap.find(ch)->second;
				if(index < sensorInputNum)
				{
					if( bAll )
					{
						pCtrl = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd(0, COL_ENABLE) );
					}
					else
					{
						pCtrl = dynamic_cast<const GUI::CCheckCtrl*>( m_listCtrl.GetItemWnd(ch, COL_ENABLE) );
					}
					assert( NULL != pCtrl );
					pValue[index] = pCtrl->IsCheck() ? 1 : 0;
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TYPE:
		{
			assert(len == sizeof(unsigned long) * sensorInputNum);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCtrl = NULL;
			for (int ch = 0; ch < effectChnn; ch++)
			{
				int index = m_rowChnnMap.find(ch)->second;

				if(index < sensorInputNum)
				{
					if( bAll )
					{
						pCtrl = dynamic_cast<const GUI::CComboCtrl*>( listAll.GetItemWnd(0, COL_TYPE) );
					}
					else
					{
						pCtrl = dynamic_cast<const GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(ch, COL_TYPE) );
					}
					assert(NULL != pCtrl );

					pValue[index] = pCtrl->GetCurItemData();
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_NAME:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME) * sensorInputNum);
			NCFG_INFO_SHORT_NAME* pValue = reinterpret_cast<NCFG_INFO_SHORT_NAME*>(pData);

			const GUI::CEditCtrl* pCtrl = NULL;
			for (int ch = 0; ch < effectChnn; ch++)
			{
				int index = m_rowChnnMap.find(ch)->second;
				
				if(index < sensorInputNum)
				{
					pCtrl = dynamic_cast<const GUI::CEditCtrl*>( m_listCtrl.GetItemWnd(ch, COL_NAME) );
					assert(NULL != pCtrl);
					strcpy(pValue[index].name, pCtrl->GetCaption().c_str());
				}
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



BEGIN_MSG_MAP(CCfgSensorDlg, CCfgDlg)
	ON_MSG_EX(CTRLID_CFG_SENSOR_LIST, KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_listAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()

unsigned long CCfgSensorDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if( COL_ENABLE == col ||
		COL_TYPE == col )
	{
		//2010-03-03 09:23:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
		GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
		assert(pCheckAll);
		if( pCheckAll->IsCheck() )
		{
			pCheckAll->SetCheck(false, true);
		}
	}

	if (!IsModify())
	{
		switch (col)
		{
		case COL_ENABLE:
			if (IsChange(NCFG_ITEM_SENSOR_IN_ENABLE))
			{
				SetModify(true);
			}
			break;
		case COL_TYPE:
			if (IsChange(NCFG_ITEM_SENSOR_IN_TYPE))
			{
				SetModify(true);
			}
			break;
		case COL_NAME:
			if (IsChange(NCFG_ITEM_SENSOR_IN_NAME))
			{
				SetModify(true);
			}
			break;
		default:
			assert(false);
			break;
		}

		return KEY_VIRTUAL_MSG_CONTINUE;
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
}

unsigned long CCfgSensorDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = static_cast<unsigned short>((keyID >> 16)&0x7fff);
	unsigned short col = static_cast<unsigned short>(keyID&0xffff);

	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCheckAll );
	assert( pCfgDlg );
	if( pCheckAll->IsCheck() )
	{
		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			if( COL_ENABLE == col ||
				COL_TYPE == col )
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
		if( COL_CHANNEL == col )
		{
			SetModify(true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

