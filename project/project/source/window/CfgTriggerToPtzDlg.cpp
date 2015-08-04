/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-01-20
** Name         : CfgTriggerToPtzDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgTriggerToPtzDlg.h"
#include "FuncCustom.h"

CCfgTriggerToPtzDlg::CCfgTriggerToPtzDlg() : m_pTriggerInfo(NULL)
, m_pPresetList(NULL)
, m_pCruiseList(NULL)
, m_pLastToptzType(NULL)
{

}

CCfgTriggerToPtzDlg::~CCfgTriggerToPtzDlg()
{
	if ( NULL != m_pPresetList )
	{
		delete [] m_pPresetList;
		m_pPresetList = NULL;
	}

	if( NULL != m_pCruiseList )
	{
		delete [] m_pCruiseList;
		m_pCruiseList = NULL;
	}

	if( NULL != m_pLastToptzType )
	{
		delete [] m_pLastToptzType;
		m_pLastToptzType = NULL;
	}
}

void CCfgTriggerToPtzDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	GUI::CListCtrl& list = m_listCtrl;

	/////预算List边缘是否需要滚动框
	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;//temporary

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
	list.AddColumn(m_pStrTable->String(E_STR_ID_TYPE), width * 14/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_ALARM_NO), width * 15/32 - offsetEdge);

	char str[256] = {0};
	for ( int i = 0; i < effectChnn; i++ )
	{
		std::map<int, int>::iterator it = m_rowChnnMap.find(i);
		if(it == m_rowChnnMap.end())
		{
			continue;
		}
		int chnnIndex = m_rowChnnMap.find(i)->second;

		if(chnnIndex < maxChnn)
		{
			sprintf(str, "%d", chnnIndex + 1);
		}
		else
		{
			sprintf(str, "%d", chnnIndex + 1 /*- localVideoInputNum*/);
		}
		list.AddItem(i, COL_CHNN, str);

		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(i, COL_TYPE, WND_TYPE_COMBO));
		assert(pCombo);

		pCombo->AddItem(m_pStrTable->String(E_STR_ID_NO), TOPTZ_TYPE_NO);
		if( chnnIndex != m_pTriggerInfo->PtzSafeChnn )
		{
			pCombo->AddItem(m_pStrTable->String(E_STR_ID_PRESET), TOPTZ_TYPE_PRESET);
			pCombo->AddItem(m_pStrTable->String(E_STR_ID_CRUISE), TOPTZ_TYPE_CRUISE);
			if ( (0!=localVideoInputNum)&&(chnnIndex < localVideoInputNum) )
			{
				pCombo->AddItem(m_pStrTable->String(E_STR_ID_TRACK), TOPTZ_TYPE_TRACK);
			}
		}
		pCombo->SetCurItem(0, false);

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(i, COL_NO, WND_TYPE_COMBO));
		assert(pCombo);
	}

	list.CreateListFocusMap();

	//////////////////////////////////////////////////////////////////////////
	assert(maxChnn > 0);
	if (NULL != m_pPresetList)
	{
		delete [] m_pPresetList;
	}
	m_pPresetList = new std::vector<PTZ_PRESET> [maxChnn];

	if (NULL != m_pCruiseList)
	{
		delete [] m_pCruiseList;
	}
	m_pCruiseList = new std::vector<CRUISE_INFO> [maxChnn];

	if( NULL != m_pLastToptzType )
	{
		delete [] m_pLastToptzType;
	}
	m_pLastToptzType = new TOPTZ_TYPE [maxChnn];
	memset( m_pLastToptzType, 0, ( sizeof(TOPTZ_TYPE) * maxChnn ) );
}

void CCfgTriggerToPtzDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	if ( NULL != m_pPresetList )
	{
		delete [] m_pPresetList;
		m_pPresetList = NULL;
	}

	if( NULL != m_pCruiseList )
	{
		delete [] m_pCruiseList;
		m_pCruiseList = NULL;
	}

	if( NULL != m_pLastToptzType )
	{
		delete [] m_pLastToptzType;
		m_pLastToptzType = NULL;
	}

	m_rowChnnMap.clear();
}

unsigned long CCfgTriggerToPtzDlg::OnApply()
{
	//保存数据
	//To PTZ

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	GUI::CComboCtrl * pCombo = NULL;
	for (int i = 0; i < effectChnn; ++i)
	{
		std::map<int, int>::iterator it = m_rowChnnMap.find(i);
		if(it == m_rowChnnMap.end())
		{
			continue;
		}
		int index = m_rowChnnMap.find(i)->second;

		TO_PTZ& trigger_ptz = m_pTriggerInfo->ToPtz[index];
		memset(&trigger_ptz, 0, sizeof(TO_PTZ));
		//Type
		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(i, COL_TYPE));
		assert(NULL != pCombo);

		TOPTZ_TYPE toPtzType = static_cast<TOPTZ_TYPE>(pCombo->GetCurItemData());
		//Index
		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(i, COL_NO));
		pCombo->SetData(static_cast<LONGLONG>(toPtzType));
		int sel_index = pCombo->GetCurItem();
		if (sel_index >= 0)
		{
			switch (toPtzType)
			{
			case TOPTZ_TYPE_CRUISE:
				trigger_ptz.cIsEnableCruise = 1;
				trigger_ptz.ucToCruise = sel_index;
				break;
			case TOPTZ_TYPE_PRESET:
				trigger_ptz.cIsEnablePreset = 1;
				trigger_ptz.ucToPreset = sel_index;
				break;
			case TOPTZ_TYPE_TRACK:
				trigger_ptz.cIsEnableTrack = 1;
				//trigger_ptz.ucToCruise = sel_index;
				break;
			default:
				return 0;
				break;
			}
		}
	}

	return 0;
}

void CCfgTriggerToPtzDlg::InitCtrlData(bool bRefresh)
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;//temporary

	//int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;

	for ( int chnn = 0; chnn < effectChnn; chnn++ )
	{
		std::map<int, int>::iterator it = m_rowChnnMap.find(chnn);
		if(it == m_rowChnnMap.end())
		{
			continue;
		}
		int index = m_rowChnnMap.find(chnn)->second;

		GUI::CComboCtrl * pComboType = NULL;
		GUI::CComboCtrl * pComboIndex = NULL;
		char sz[8] = {0};

		TO_PTZ& trigger_ptz = m_pTriggerInfo->ToPtz[index];

		pComboType = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(chnn, COL_TYPE));
		assert(NULL != pComboType);
		pComboIndex = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(chnn, COL_NO));
		assert(NULL != pComboIndex);

		if( index == m_pTriggerInfo->PtzSafeChnn )
		{
			//通道触发自己，motion事件时不允许的操作
			pComboType->SetCurItemData(TOPTZ_TYPE_NO, bRefresh);
			pComboIndex->RemoveAllItem(false);

			m_pLastToptzType[index] = TOPTZ_TYPE_NO;
			continue;
		}

		if( trigger_ptz.cIsEnableCruise)
		{
			pComboType->SetCurItemData(TOPTZ_TYPE_CRUISE, bRefresh);
			pComboIndex->RemoveAllItem(false);	
			for (std::vector<CRUISE_INFO>::const_iterator iter = m_pCruiseList[index].begin(); iter != m_pCruiseList[index].end(); ++iter)
			{
				sprintf(sz, "%d", iter->index + 1);
				pComboIndex->AddItem(sz, iter->index);
				if (trigger_ptz.ucToCruise == iter->index)
				{
					pComboIndex->SetCurItemData(iter->index, false);
				}
			}
			if (!m_pCruiseList[index].empty())
			{
                if (-1==pComboIndex->GetCurItem())
                {
					std::vector<CRUISE_INFO>::const_iterator iter = m_pCruiseList[index].begin();
					sprintf(sz, "%d", iter->index + 1);
					pComboIndex->SetCurItemData(iter->index, false);
                }
			}
            
			m_pLastToptzType[index] = TOPTZ_TYPE_CRUISE;

		}
		else if (trigger_ptz.cIsEnablePreset)
		{
			pComboType->SetCurItemData(TOPTZ_TYPE_PRESET, bRefresh);
			pComboIndex->RemoveAllItem(false);
			for (std::vector<PTZ_PRESET>::const_iterator iter = m_pPresetList[index].begin(); iter != m_pPresetList[index].end(); ++iter)
			{
				sprintf(sz, "%d", iter->index + 1);
				pComboIndex->AddItem(sz, iter->index);
				if (trigger_ptz.ucToPreset == iter->index)
				{
					pComboIndex->SetCurItemData(iter->index, false);
				}
			}

			m_pLastToptzType[index] = TOPTZ_TYPE_PRESET;
		}
		else if (trigger_ptz.cIsEnableTrack)
		{
			pComboType->SetCurItemData(TOPTZ_TYPE_TRACK, bRefresh);
			pComboIndex->RemoveAllItem(false);
			pComboIndex->AddItem("1", 0);
			pComboIndex->SetCurItemData( 0, false);

			m_pLastToptzType[index] = TOPTZ_TYPE_TRACK;

		}
		else
		{
			pComboType->SetCurItemData(TOPTZ_TYPE_NO, bRefresh);
			pComboIndex->RemoveAllItem(false);

			m_pLastToptzType[index] = TOPTZ_TYPE_NO;
		}
	}
}

bool CCfgTriggerToPtzDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;//temporary

	if ((cfgID >= NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE) & (cfgID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND))
	{
		int chnn = cfgID - NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE;
		assert(NULL != m_pPresetList);
		assert(chnn < maxChnn);
		m_pPresetList[chnn].clear();

		if ((NULL != pData) && (len > 0))
		{
			assert(len % sizeof(PTZ_PRESET) == 0);
			int count = len / sizeof(PTZ_PRESET);
			const PTZ_PRESET* pPreset = reinterpret_cast<const PTZ_PRESET*>(pData);

			for (int i = 0; i < count; ++i, ++pPreset)
			{
				m_pPresetList[chnn].push_back(*pPreset);
			}
		}

		return true;
	} 
	else if ((cfgID >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE) & (cfgID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND))
	{
		int chnn = cfgID - NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE;
		assert(NULL != m_pCruiseList);
		assert(chnn < maxChnn);
		m_pCruiseList[chnn].clear();

		if ((NULL != pData) && (len > 0))
		{
			unsigned short curise_count = 0;
			unsigned short sub_len = 0;
			GetSubItemInfo(cfgID, curise_count, sub_len);

			const NCFG_INFO_CRUISE_HEAD* pHead = reinterpret_cast<const NCFG_INFO_CRUISE_HEAD*>(pData);
			for (int i = 0; i < curise_count; ++i, ++pHead)
			{
				CRUISE_INFO cruise_info;
				cruise_info.index = pHead->index;
				cruise_info.strName = pHead->name;
				const PTZ_CURISE_POINT* pPoint = reinterpret_cast<const PTZ_CURISE_POINT*>(pData + pHead->offset);
				for (int j = 0; j < pHead->cruisepointnum; ++j, ++pPoint)
				{
					cruise_info.point_list.push_back(*pPoint);
				}
				m_pCruiseList[chnn].push_back(cruise_info);
			}
		}

		return true;
	} 
	else
	{
	}
	return false;
}

BEGIN_MSG_MAP(CCfgTriggerToPtzDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()


unsigned long CCfgTriggerToPtzDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	GUI::CListCtrl& list = m_listCtrl;

	if (COL_TYPE == col)
	{
		ULONGLONG CH = 0;
		int localVideoInputNum = 0, netVideoInputNum = 0;
		int maxChnn = 0, effectChnn = 0;

		IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

		maxChnn = localVideoInputNum + netVideoInputNum;//temporary

		int index = m_rowChnnMap.find(row)->second;

		GUI::CComboCtrl* pComboType = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(row, COL_TYPE));
		assert(NULL != pComboType);
		GUI::CComboCtrl* pComboIndex = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(row, COL_NO));
		assert(NULL != pComboIndex);

		TOPTZ_TYPE toPtzType = static_cast<TOPTZ_TYPE>(pComboType->GetCurItemData());
		if ( toPtzType != m_pLastToptzType[index] )//两次之间操作了不同的TO PTZ项,只有不相同时才刷新界面数据项
		{
			char sz[8] = {0};
			assert(index < maxChnn);
			switch (toPtzType)
			{
			case TOPTZ_TYPE_CRUISE:
				pComboIndex->RemoveAllItem(false);
				if (!m_pCruiseList[index].empty())
				{
					for (std::vector<CRUISE_INFO>::const_iterator iter = m_pCruiseList[index].begin(); iter != m_pCruiseList[index].end(); ++iter)
					{
						sprintf(sz, "%d", iter->index + 1);
						pComboIndex->AddItem(sz, iter->index);
					}
					pComboIndex->SetCurItem(0, false);	
					pComboIndex->SetEnable(true, false);
				} 

				m_pLastToptzType[index] = TOPTZ_TYPE_CRUISE;

				break;
			case TOPTZ_TYPE_PRESET:
				pComboIndex->RemoveAllItem(false);
				if (!m_pPresetList[index].empty())
				{
					for (std::vector<PTZ_PRESET>::const_iterator iter = m_pPresetList[index].begin(); iter != m_pPresetList[index].end(); ++iter)
					{
						sprintf(sz, "%d", iter->index + 1);
						pComboIndex->AddItem(sz, iter->index);
					}
					pComboIndex->SetCurItem(0, false);
					pComboIndex->SetEnable(true, false);
				} 

				m_pLastToptzType[index] = TOPTZ_TYPE_PRESET;

				break;
			case TOPTZ_TYPE_TRACK:	
				pComboIndex->RemoveAllItem(false);
				pComboIndex->AddItem("1", 0);
				pComboIndex->SetCurItem(0, false);
				pComboIndex->SetEnable(true, false);

				m_pLastToptzType[index] = TOPTZ_TYPE_TRACK;

				break;
			default:
				pComboIndex->RemoveAllItem(false);
				pComboIndex->SetEnable(false, false);

				m_pLastToptzType[index] = TOPTZ_TYPE_NO;

				break;
			}

			pComboIndex->Repaint();
			pComboIndex->SetData(static_cast<LONGLONG>(toPtzType));
		}	
	}

	return KEY_VIRTUAL_MSG_STOP;
}
