/***********************************************************************
** Copyright (C)  Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-03-25
** Name         : CfgCoverDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record: 
1: 
***********************************************************************/

#include "CfgCoverDlg.h"

#include "FuncCustom.h"
extern unsigned char g_coverAreaCountLimit;

CCfgCoverDlg::CCfgCoverDlg() : m_pCamCoverInfo(NULL)
,m_buffLen(0)
{

}

CCfgCoverDlg::~CCfgCoverDlg()
{
	if(NULL != m_pCamCoverInfo)
	{
		delete [] m_pCamCoverInfo;
		m_pCamCoverInfo = NULL;
	}
	m_buffLen = 0;
}

void CCfgCoverDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_listCtrl;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

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

	list.Create(GetRandWndID(), x, y, width, height, this, 0, 0, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_MASK_AREA), width * 29 / 32 - offsetEdge);

	char str[64] = {0};
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

		GUI::CButton* pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(i, COL_CAMERA_COVER, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]));
		assert(pBtn);
		pBtn->SetCaption(m_pStrTable->String(E_STR_ID_SETTING), false);
	}

	list.CreateListFocusMap();

	//准备内存区域
	if(m_buffLen < maxChnn * sizeof(CAMERA_COVER_INFO))
	{
		if(NULL != m_pCamCoverInfo)
		{
			delete [] m_pCamCoverInfo;
		}
		m_pCamCoverInfo = new CAMERA_COVER_INFO[maxChnn];
		m_buffLen = maxChnn * sizeof(CAMERA_COVER_INFO);
	}

	memset(m_pCamCoverInfo, 0, m_buffLen);
}

void CCfgCoverDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	m_rowChnnMap.clear();
}

void CCfgCoverDlg::SetTip()
{
	if(g_coverAreaCountLimit < MAX_COVER_AREA_NUM)
	{
		return;
	}
	
	ULONGLONG CH = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn);

	#ifdef  __CUSTOM_US02_NEW__
		for ( int i = 0; i < effectChnn; i++ )
		{
			AddTip(m_listCtrl.GetItemWnd(i, COL_CAMERA_COVER)->GetID(), E_STR_ID_TIP_02_01_01);
		}
	#else
		for ( int i = 0; i < effectChnn; i++ )
		{
			AddTip(m_listCtrl.GetItemWnd(i, COL_CAMERA_COVER)->GetID(), E_STR_ID_TIP_02_01);
		}
	#endif
}

bool CCfgCoverDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_CAM_COVER_INFO:
		{
			int numChnn = maxChnn;  //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
			assert(len == sizeof(CAMERA_COVER_INFO) * numChnn);
			assert(m_buffLen == len);

			memcpy(m_pCamCoverInfo, pData, len);
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgCoverDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	const GUI::CListCtrl& list = m_listCtrl;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_CAM_COVER_INFO:
		{
			int numChnn = maxChnn;  //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
			assert(len == sizeof(CAMERA_COVER_INFO) * numChnn);
			assert(m_buffLen == len);

			memcpy(pData, m_pCamCoverInfo, len);
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

DLG_END_VALUE CCfgCoverDlg::PopupSetCoverDlg(int chnn, CAMERA_COVER_INFO& coverInfo)
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	assert(chnn >= 0);
	assert(chnn < maxChnn);

	CMessageMan *pMsgMan = CMessageMan::Instance();
	DLG_END_VALUE retVal;

	//切换到相关通道的大画面
	NET_DISPLAY_INFO displayInfo;
	memset(&displayInfo, 0, sizeof(NET_DISPLAY_INFO));
	displayInfo.time_out	= INVALID_INDEX(unsigned short);
	displayInfo.split_mode[DISPLAY_LAYER_ANALOG]= VIEW_SPLIT_1X1;
	displayInfo.split_mode[DISPLAY_LAYER_NET] = VIEW_SPLIT_1X1;
	if(chnn < localVideoInputNum)
	{
		displayInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, 0, static_cast<unsigned char>(chnn));
	}
	else
	{
		displayInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, 0, chnn - localVideoInputNum);
	}
	displayInfo.data_len = 1;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char *)&displayInfo, sizeof(NET_DISPLAY_INFO), msgData );
	pMsgMan->ReleaseMsgBuff( msgData );

	m_setCoverDlg.SetCoverInfo(coverInfo);
	retVal = m_setCoverDlg.DoModal(this);
	if(DLG_OK == retVal)
	{
		m_setCoverDlg.GetCoverInfo(coverInfo);
	}

	//返回之前的大画面状态
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0 , msgData );
	pMsgMan->ReleaseMsgBuff( msgData );

	return retVal;
}

BEGIN_MSG_MAP(CCfgCoverDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgCoverDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if(COL_CAMERA_COVER == col)
	{
		int chnn = m_rowChnnMap.find(row)->second;

		if(DLG_OK == PopupSetCoverDlg(chnn, m_pCamCoverInfo[chnn]))
		{
			if (!IsModify())
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
	}

	if (!IsModify())
	{
		switch (col)
		{
		case COL_CAMERA_COVER:
			if (IsChange(NCFG_ITEM_CAM_COVER_INFO))
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

//end
