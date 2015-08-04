/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-14
** Name         : CfgNetworkDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgDvrIDDlg.h"

#include "Rect.h"
#include "FuncCustom.h"
CCfgDvrIdDlg::CCfgDvrIdDlg()
{
}

CCfgDvrIdDlg::~CCfgDvrIdDlg()
{

}

void CCfgDvrIdDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgLiveList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CButton* pBtn = NULL;
	GUI::CEditCtrl* pEdit = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - 2 * m_length[LIST_ITEM_HEIGHT] - y - m_length[DLG_OFFSET_SUB_BOTTOM];

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
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	list.AddColumn("ChannelID", width * 16 / 32);	//name

	char str[256] = {0};

	for (int i = 0; i < effectChnn; i++)
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

		pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem( i, COL_CAMERA_ID, WND_TYPE_EDIT));
		assert( pEdit );
	}

	list.CreateListFocusMap();

	return;
}

void CCfgDvrIdDlg::SetTip()
{
	ULONGLONG CH = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn);
}

void CCfgDvrIdDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	m_rowChnnMap.clear();
}

bool CCfgDvrIdDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_cfgLiveList;

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_GB28181_CHANNEL_ID:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
			assert( len == sizeof(GB28181_CHANNEL_ID));

			const GB28181_CHANNEL_ID* pShortName = reinterpret_cast<const GB28181_CHANNEL_ID*>(pData);
			for(int i = 0; i< effectChnn; i++)
			{
				GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(i, COL_CAMERA_ID));
				assert( pEdit );
				pEdit->SetMaxStringLen(32);
				//pEdit->SetCaption(pShortName[m_rowChnnMap.find(i)->second].name, bRefresh);
				pEdit->SetCaption(pShortName->channel_id[i], bRefresh);
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

bool CCfgDvrIdDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CListCtrl& list = m_cfgLiveList;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_GB28181_CHANNEL_ID:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
			assert(len == sizeof(GB28181_CHANNEL_ID));

			GB28181_CHANNEL_ID* pShortName = reinterpret_cast<GB28181_CHANNEL_ID*>(pData);
			for (int i = 0; i < effectChnn; i++)
			{
				const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(i, COL_CAMERA_ID));
				assert(pEdit);
				//strcpy(pShortName[m_rowChnnMap.find(i)->second].name, pEdit->GetCaption().c_str());
				strcpy(pShortName->channel_id[i], pEdit->GetCaption().c_str());
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

BEGIN_MSG_MAP(CCfgDvrIdDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgLiveList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgDvrIdDlg::OnCheck()
{
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgDvrIdDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if (!IsModify())
	{

		switch (col)
		{
		case COL_CAMERA_ID:
			SetModify(true);
			//if (IsChange(NCFG_ITEM_DVR_INFO))
			//{
			//	SetModify(true);
			//}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CCfgDvrIdDlg::PreCheckValue()
{
	if(IsChange(NCFG_ITEM_GB28181_CHANNEL_ID)) 
	{
		return E_STR_ID_TIP_03_01;
	}

	return 0;
}


bool CCfgDvrIdDlg::IsValidID(const string &strID)
{
	if (strID == "")
	{
		return true;
	}
	char p;
	int i=0;
	for (;i<strID.length();i++)
	{
		p = strID[i];
		if ( p <'0' || p> '9')
		{
			return false;
		}
	}
	if ( i != 20)
	{
		return false;
	}
	return true;
}


const char szCaption[] = {0xE5,0x9B,0xBD,0xE6,0xA0,0x87,0xE9,0x85,0x8D,0xE7,0xBD,0xAE,0xE5,0x87,0xBA,0xE9,0x94,0x99,0x0};			//国标配置出错
const char LANG_UTF8_03[] = {0x49,0x44,0xE4,0xB8,0x8D,0xE7,0xAC,0xA6,0xE5,0x90,0x88,0xE8,0xA7,0x84,0xE8,0x8C,0x83,0x0};				//ID不符合规范

unsigned long CCfgDvrIdDlg::CheckValue()
{

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;
	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);
	maxChnn = localVideoInputNum + netVideoInputNum;

	string strID;

	if(IsChange(NCFG_ITEM_GB28181_CHANNEL_ID)) 
	{
		int numChnn = maxChnn;
		for (int i = 0; i < effectChnn; i++)
		{
			const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>(m_cfgLiveList.GetItemWnd(i, COL_CAMERA_ID));
			strID = pEdit->GetCaption().c_str();
			if (!IsValidID(strID))
			{
				MessageBox(LANG_UTF8_03,szCaption,DEFAULT_BTN_OK);
				return E_STR_ID_TIP_00_07;
			}
		}
	}
	return 0;
}

//end
