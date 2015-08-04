/***********************************************************************
** Copyright (C)  Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-04-09
** Name         : CfgRecordRecycleDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgRecordRecycleDlg.h"

CCfgRecordRecycleDlg::CCfgRecordRecycleDlg()
{

}

CCfgRecordRecycleDlg::~CCfgRecordRecycleDlg()
{

}

void CCfgRecordRecycleDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	GUI::CListCtrl& list = m_listCtrl;
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width / 2);
	list.AddColumn(NULL, width / 2 - 4 );

	////
	list.AddItem(ROW_RECORD_RECYCLE, 0, m_pStrTable->String(E_STR_ID_RECYCLE_RECORD));
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_RECORD_RECYCLE, 1, WND_TYPE_CHECK));
	assert( pCheck );


	list.CreateListFocusMap();
}

unsigned long CCfgRecordRecycleDlg::PreCheckValue()
{
	if(IsChange(NCFG_ITEM_REC_RECYCLE)) 
	{
		return E_STR_ID_TIP_03_01;
	}

	return 0;
}

bool CCfgRecordRecycleDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	GUI::CListCtrl& list = m_listCtrl;
	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
	case NCFG_ITEM_REC_RECYCLE:
		{
			assert( len == sizeof(unsigned long) );
			const unsigned long value = *reinterpret_cast<const unsigned long*>(pData);
			
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_RECORD_RECYCLE, 1));
			assert(pCheck);

			if(0 != value)
			{
				pCheck->SetCheck(true, bRefresh);
			}
			else
			{
				pCheck->SetCheck(false, bRefresh);
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

bool CCfgRecordRecycleDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	const GUI::CListCtrl& list = m_listCtrl;

	switch (cfgID)
	{
	case NCFG_ITEM_REC_RECYCLE:
		{
			assert( len == sizeof(unsigned long) );
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>(list.GetItemWnd(ROW_RECORD_RECYCLE, 1));
			assert(pCheck);

			if( pCheck->IsCheck() )
			{
				*pValue = 1;
			}
			else
			{
				*pValue = 0;
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

BEGIN_MSG_MAP(CCfgRecordRecycleDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgRecordRecycleDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos )
{
	unsigned short row = (keyID >> 16)&0x7fff;	//ÐÐ
	unsigned short col = keyID&0xffff;			//ÁÐ

	if (!IsModify() && 1 == col)
	{
		switch (row)
		{
		case ROW_RECORD_RECYCLE:
			if (IsChange(NCFG_ITEM_REC_RECYCLE))
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		default:
			assert(false);
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//end
