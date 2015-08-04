/***********************************************************************
** Copyright (C)  Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2010-11-2
** Name         : CfgRecordSnapDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgRecordSnapDlg.h"
#include "FuncCustom.h"

CCfgRecordSnapDlg::CCfgRecordSnapDlg()
{
	m_bShowSnapNum = true;
}

CCfgRecordSnapDlg::~CCfgRecordSnapDlg()
{

}

void CCfgRecordSnapDlg::SetShowSnapNum(bool bShow = true)
{
	m_bShowSnapNum = bShow;
}

void CCfgRecordSnapDlg::OnInitial()
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
	list.AddItem(ROW_RESOLUTION, 0, m_pStrTable->String(E_STR_ID_RESOLUTION));
	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_RESOLUTION, 1, WND_TYPE_COMBO));
	assert( pCombo );

	list.AddItem(ROW_QUALITY, 0, m_pStrTable->String(E_STR_ID_QUALITY));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_QUALITY, 1, WND_TYPE_COMBO));
	assert(pCombo);

	list.AddItem(ROW_INTERVAL, 0, m_pStrTable->String(E_STR_ID_SNAP_INTERVAL));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_INTERVAL, 1, WND_TYPE_COMBO));
	assert(pCombo);
	
	if(m_bShowSnapNum)
	{
		list.AddItem(ROW_MAXNUM, 0,  m_pStrTable->String(E_STR_ID_SNAP_NUM));
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_MAXNUM, 1, WND_TYPE_COMBO));
	}
	
	list.CreateListFocusMap();
}

void CCfgRecordSnapDlg::DoOther()
{
	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;
	GUI::CComboCtrl* pCombo = NULL;

	bool bRefresh = false;
	GUI::CListCtrl& list = m_listCtrl;
	char str[128] = {0};

	{
#ifdef __SUPPORT_SNAPD1__
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_RESOLUTION, 1));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);	
		pCombo->AddItem("CIF", DVRVIDEO_SIZE_CIF);
#if defined(__CHIP3531__) || defined(__DVR_ULTIMATE__) || defined(__CHIPGM__) 
#if defined(__CHIPGM__)
		if (TD_2704AS_SL != CProduct::Instance()->ProductType())
		{
			pCombo->AddItem("D1", DVRVIDEO_SIZE_D1);
		}
#else
		pCombo->AddItem("D1", DVRVIDEO_SIZE_D1);
#endif
#endif
		pCombo->SetCurItemData(DVRVIDEO_SIZE_CIF,bRefresh);
		pTempData = NULL;
		tempDataLen = 0;
#else
		CCfgDlg::GetConfig(NCFG_ITEM_SNAP_SIZE_RANGE, &pTempData, tempDataLen);
		assert(tempDataLen > 0);
		unsigned long dwResolution = *(reinterpret_cast<unsigned long*>(pTempData));
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_RESOLUTION, 1));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);	
		for (int i = 0; i < sizeof(unsigned long)*8; ++i)
		{
			unsigned long tmp = ((unsigned long)1) << i;		
			if (dwResolution & tmp)
			{
				if (tmp & DVRVIDEO_SIZE_QCIF)
				{
					pCombo->AddItem("QCIF", DVRVIDEO_SIZE_QCIF);
				}
				else if (tmp & DVRVIDEO_SIZE_CIF)
				{
					pCombo->AddItem("CIF", DVRVIDEO_SIZE_CIF);
				}
				else if (tmp & DVRVIDEO_SIZE_HD1)
				{
					pCombo->AddItem("HD1", DVRVIDEO_SIZE_HD1);
				}
				else if (tmp & DVRVIDEO_SIZE_D1)
				{
#if defined(__CHIPGM__)
					if (TD_2704AS_SL != CProduct::Instance()->ProductType())
					{
						pCombo->AddItem("D1", DVRVIDEO_SIZE_D1);
					}
#else
					pCombo->AddItem("D1", DVRVIDEO_SIZE_D1);
#endif
				}	
				else if (tmp & DVRVIDEO_SIZE_HD1080)
				{
					pCombo->AddItem("1080P", DVRVIDEO_SIZE_HD1080);
				}
				else if (tmp & DVRVIDEO_SIZE_HD720)
				{
					pCombo->AddItem("720P", DVRVIDEO_SIZE_HD720);
				}
#ifdef __CUSTOM_US02__
				else if (tmp & DVRVIDEO_SIZE_960H)
				{
					pCombo->AddItem("960H", DVRVIDEO_SIZE_960H);
				}
#else
				else if (tmp & DVRVIDEO_SIZE_960H)
				{
					pCombo->AddItem("WD1", DVRVIDEO_SIZE_960H);
				}
#endif
			}		
		}
		pCombo->SetCurItemData(DVRVIDEO_SIZE_CIF,bRefresh);
		pTempData = NULL;
		tempDataLen = 0;
#endif
	}

	{
		CCfgDlg::GetConfig(NCFG_ITEM_SNAP_QULITY_LEVELNUM, &pTempData, tempDataLen);
		assert(tempDataLen > 0);
		int qualityLevelNum = tempDataLen/sizeof(unsigned long);
		unsigned long *pQulityValue = reinterpret_cast<unsigned long *>(pTempData);	
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_QUALITY, 1));
		pCombo->RemoveAllItem(bRefresh);	
		for (int i = 0; i < qualityLevelNum; ++i)
		{
			if (IMAGE_QUALITY_LOWEST == pQulityValue[i] )
			{
				snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_LOWEST ) );
				pCombo->AddItem( str, pQulityValue[i]);
			}
			else if (IMAGE_QUALITY_LOWER == pQulityValue[i])
			{
				snprintf( str, 128, "%s",m_pStrTable->String( E_STR_ID_LOWER ) );
				pCombo->AddItem( str,  pQulityValue[i]);
			}
			else if (IMAGE_QUALITY_LOW == pQulityValue[i])
			{
				snprintf( str, 128, "%s",m_pStrTable->String( E_STR_ID_LOW ) );
				pCombo->AddItem(str, pQulityValue[i]);
			}
			else if (IMAGE_QUALITY_MEDIUM == pQulityValue[i])
			{
				snprintf( str, 128, "%s",m_pStrTable->String( E_STR_ID_MEDIUM ) );
				pCombo->AddItem( str, pQulityValue[i] );
			}
			else if (IMAGE_QUALITY_HEIGHTER == pQulityValue[i])
			{
				snprintf( str, 128, "%s",m_pStrTable->String( E_STR_ID_HIGHER ) );
				pCombo->AddItem( str, pQulityValue[i] );
			}
			else if (IMAGE_QUALITY_HEIGHTEST == pQulityValue[i])
			{
				snprintf( str, 128, "%s",m_pStrTable->String( E_STR_ID_HIGHEST ) );
				pCombo->AddItem( str, pQulityValue[i] );
			}	
			else 
			{
				assert(false);
			}
		}
		pCombo->SetCurItemData(IMAGE_QUALITY_MEDIUM,bRefresh);
		pTempData = NULL;
		tempDataLen = 0;
	}

	{
		CCfgDlg::GetConfig(NCFG_ITEM_SNAP_TIME_RANGE, &pTempData, tempDataLen);
		assert(tempDataLen > 0);
		int timeNum = tempDataLen/sizeof(unsigned long);
		unsigned long *pTimeValue = reinterpret_cast<unsigned long *>(pTempData);	
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_INTERVAL, 1));
		pCombo->RemoveAllItem(bRefresh);
#ifdef __SUPPORT_SNAPD1__
		unsigned long shanghaiSnapLimit = 3;//上海平台抓图间隔必须大于3
		bool bcur = false;
		for (int i = 0; i < timeNum; ++i)
		{
			if(pTimeValue[i] >= shanghaiSnapLimit)
			{
				memset(str,0,sizeof(str));
				snprintf(str, 128, "%d", pTimeValue[i]);
				pCombo->AddItem(str, pTimeValue[i]);
				if(!bcur)
				{
					pCombo->SetCurItem(pTimeValue[i], bRefresh);
					bcur = true;
				}
			}
		}
		if(!bcur)
		{
			//没有大于3的数字就是3
			memset(str,0,sizeof(str));
			snprintf(str, 128, "%d", shanghaiSnapLimit);
			pCombo->AddItem(str, shanghaiSnapLimit);
			pCombo->SetCurItem(shanghaiSnapLimit, bRefresh);
		}
#else
		for (int i = 0; i < timeNum; ++i)
		{
			snprintf(str, 128, "%d", pTimeValue[i]);
			pCombo->AddItem(str, pTimeValue[i]);					
		}
		pCombo->SetCurItem(0, bRefresh);
#endif
		pTempData = NULL;
		tempDataLen = 0;
		pCombo = NULL;
	}

	if(m_bShowSnapNum)
	{
		CCfgDlg::GetConfig(NCFG_ITEM_SNAP_IMG_NUM_RANGE, &pTempData, tempDataLen);
		assert(tempDataLen > 0);
		int SnapNum = tempDataLen/sizeof(unsigned long);
		unsigned long *pSnapNumValue = reinterpret_cast<unsigned long *>(pTempData);	
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_MAXNUM, 1));
		pCombo->RemoveAllItem(bRefresh);	
		for (int i = 0; i < SnapNum; ++i)
		{
			if (0 == pSnapNumValue[i])
			{
				snprintf(str, 128, "%s", m_pStrTable->String(E_STR_ID_IMG_NONE));
			}
			else
			{
				snprintf(str, 128, "%d", pSnapNumValue[i]);
			}
			pCombo->AddItem(str, pSnapNumValue[i]);					
		}
		pCombo->SetCurItem(0, bRefresh);
		pTempData = NULL;
		tempDataLen = 0;
	}

}

void CCfgRecordSnapDlg::EndOther()
{

}

bool CCfgRecordSnapDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	GUI::CListCtrl& list = m_listCtrl;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_JPEG_SIZE:	
		{
			assert(len == (sizeof(unsigned long )* maxChnn));
			const unsigned long *pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CComboCtrl *pComb = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ROW_RESOLUTION, 1));
			pComb->SetCurItemData(pValue[0], bRefresh);
		}
		return true;
		break;
	case NCFG_ITEM_JPEG_QUALITY:
		{	
			assert(len == (sizeof(unsigned long )* maxChnn));
			const unsigned long *pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CComboCtrl *pComb = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ROW_QUALITY, 1));
			pComb->SetCurItemData(pValue[0], bRefresh);
				
		}
		return true;
		break;	
	case NCFG_ITEM_JPEG_PERIOD:
		{
			assert(len == (sizeof(unsigned long )* maxChnn));
			const unsigned long *pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CComboCtrl *pComb = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ROW_INTERVAL, 1));
#ifdef __SUPPORT_SNAPD1__
			if(pValue[0] < 3)//抓图间隔必须大于等于3秒
			{
				pComb->SetCurItemData(3, bRefresh);
			}
			else
			{
				pComb->SetCurItemData(pValue[0], bRefresh);	
			}
#else
			pComb->SetCurItemData(pValue[0], bRefresh);	
#endif
		}
		return true;
		break;
	case NCFG_ITEM_JPEG_SNAP_NUM:
		{	
			assert(len == (sizeof(unsigned long )* maxChnn));
			const unsigned long *pValue = reinterpret_cast<const unsigned long*>(pData);
			if(m_bShowSnapNum)
			{
				GUI::CComboCtrl *pComb = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ROW_MAXNUM, 1));
				printf("%s,%d, NCFG_ITEM_JPEG_SNAP_NUM:%d\n",__FILE__,__LINE__,pValue[0]);
				pComb->SetCurItemData(pValue[0], bRefresh);
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

bool CCfgRecordSnapDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{

	assert(NULL != pData);
	assert(len > 0);

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	const GUI::CListCtrl& list = m_listCtrl;
	switch (cfgID)
	{
	case NCFG_ITEM_JPEG_SIZE:
		{
			assert(len == (sizeof(unsigned long) * maxChnn));
			unsigned long *pValue = reinterpret_cast<unsigned long*>(pData);
			GUI::CComboCtrl * pComb= NULL;
			pComb = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ROW_RESOLUTION ,1));
			unsigned long Temp = pComb->GetCurItemData();
			for (int i = 0; i < maxChnn; ++i)
			{				
				pValue[i] = Temp;
			}				
		}
		return true;
		break;
	case NCFG_ITEM_JPEG_QUALITY:
		{
			assert(len == (sizeof(unsigned long) * maxChnn));
			unsigned long *pValue = reinterpret_cast<unsigned long*>(pData);
			GUI::CComboCtrl * pComb= NULL;
			pComb = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ROW_QUALITY ,1));
			unsigned long Temp = pComb->GetCurItemData();
			for (int i = 0; i < maxChnn; ++i)
			{				
				pValue[i] = Temp;
			}					
		}
		return true;
		break;	
	case NCFG_ITEM_JPEG_PERIOD:
		{
			assert(len == (sizeof(unsigned long) * maxChnn));
			unsigned long *pValue = reinterpret_cast<unsigned long*>(pData);
			GUI::CComboCtrl * pComb= NULL;
			pComb = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ROW_INTERVAL ,1));
			unsigned long Temp = pComb->GetCurItemData();
			for (int i = 0; i < maxChnn; ++i)
			{				
				pValue[i] = Temp;
			}		
		}
		return true;
		break;
	case NCFG_ITEM_JPEG_SNAP_NUM:
		{
			assert(len == (sizeof(unsigned long) * maxChnn));
			unsigned long *pValue = reinterpret_cast<unsigned long*>(pData);
			if(m_bShowSnapNum)
			{
				GUI::CComboCtrl * pComb= NULL;
				pComb = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ROW_MAXNUM ,1));
				unsigned long Temp = pComb->GetCurItemData();
			for (int i = 0; i < maxChnn; ++i)
				{				
					pValue[i] = Temp;
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

BEGIN_MSG_MAP(CCfgRecordSnapDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgRecordSnapDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos )
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if (!IsModify() && 1 == col)
	{
		switch (row)
		{
		case ROW_RESOLUTION:
			if(IsChange(NCFG_ITEM_JPEG_SIZE))
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;			
		case ROW_QUALITY:
			if(IsChange(NCFG_ITEM_JPEG_QUALITY))
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case ROW_INTERVAL:
			if(IsChange(NCFG_ITEM_JPEG_PERIOD))
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_MAXNUM:
			if(m_bShowSnapNum)
			{
				if(IsChange(NCFG_ITEM_JPEG_SNAP_NUM))
				{
					SetModify(true);
					return KEY_VIRTUAL_MSG_CONTINUE;
				}
			}
			break;
		default:
			assert(false);
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}


#ifdef __CUSTOM_US02_NEW__
void CCfgRecordSnapDlg::SetTip()
{
	MessageOut(GetTopestWnd(),m_pStrTable->String(E_STR_ID_RECORD_SNAP),TIP_MSG,true);
	TipAll(m_childList, E_STR_ID_RECORD_SNAP);
}

void CCfgRecordSnapDlg::TipAll(WND_LIST child, ENUM_STRING_ID strId)
{
	if(child.empty()) return;
	
	for (WND_LIST::iterator it = child.begin(); it != child.end(); it++)
	{
		if(!(*it)->m_childList.empty())
		{
			TipAll((*it)->m_childList, strId);
		}
		if((*it)->GetParent() != this && (*it)->GetParent()->GetParent() != this) AddTip((*it)->GetID(), strId);
	}
}
#endif



