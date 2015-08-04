/*
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xian shi wei
** Date         : 2014-10-15
** Name         : CfgROIDlg.CPP
** Version      : 1.0
** Description  : 
** Modify Record:
*/

#include "CfgROIDlg.h"
#include "FuncCustom.h"

CCfgROIDlg::CCfgROIDlg():m_pROIInfo(NULL),m_buffLen(0),m_localInputNum(0),m_crruChnn(-1){}

CCfgROIDlg::~CCfgROIDlg()
{
	if ( NULL != m_pROIInfo)
	{
		delete [] m_pROIInfo;
	}
	m_buffLen = 0;
}

void CCfgROIDlg::OnInitial()
{
	CCfgDlg::OnInitial();

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
	maxChnn =localVideoInputNum + netVideoInputNum;
	m_localInputNum = localVideoInputNum;

	int row = 0;
	for(int i = localVideoInputNum; i < maxChnn; i++)
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
	list.AddColumn(m_pStrTable->String(E_STR_ID_IMAGE_ROI), width * 29 / 32 - offsetEdge);

	char str[64] = {0};
	//for(int i = 0; i < effectChnn; i++)
	//{
	//	if ((m_rowChnnMap.size()>0) && ((CH >> i) & 0x1))
	//	{
	//		int chnnIndex = m_rowChnnMap.find(i)->second;
	//		if(chnnIndex >= localVideoInputNum)
	//			/*{
	//			sprintf(str, "%d", chnnIndex + 1);
	//			}
	//			else*/
	//		{
	//			sprintf(str, "%d", chnnIndex + 1 /*- localVideoInputNum*/);
	//			list.AddItem(i, COL_CHNN, str);

	//			GUI::CButton* pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(i, COL_VIDEO_ROI, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]));
	//			assert(pBtn);
	//			pBtn->SetCaption(m_pStrTable->String(E_STR_ID_SETTING), false);
	//		}
	//	}
	//}
	for(unsigned int i =0; i<m_rowChnnMap.size();i++)
	{
		int chnnIndex = m_rowChnnMap.find(i)->second;
		sprintf(str, "%d", chnnIndex + 1 /*- localVideoInputNum*/);
		list.AddItem(i, COL_CHNN, str);

		GUI::CButton* pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(i, COL_VIDEO_ROI, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]));
		assert(pBtn);
		pBtn->SetCaption(m_pStrTable->String(E_STR_ID_SETTING), false);
	}

	list.CreateListFocusMap();

	if(m_buffLen < netVideoInputNum * sizeof(VIDEO_CONFIG_ROI_INFO))
	{
		if ( NULL != m_pROIInfo)
		{
		  	delete [] m_pROIInfo;
		}
		m_pROIInfo = new VIDEO_CONFIG_ROI_INFO[netVideoInputNum];
		m_buffLen = netVideoInputNum * sizeof(VIDEO_CONFIG_ROI_INFO);
	}

	memset(m_pROIInfo, 0, m_buffLen);
	for(unsigned int i=0;i<netVideoInputNum;++i){
		for(unsigned int j=0;j<MAX_VIDEO_CONFIG_ROI_NUM;++j){
			m_pROIInfo[i].roi[j].iLevel = 1;
		}
	}
}
void CCfgROIDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	m_rowChnnMap.clear();
}
void CCfgROIDlg::SetTip()
{

}

void CCfgROIDlg::DoOther()
{
	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;
	CCfgDlg::GetConfig( NCFG_ITEM_CAM_ROI_INFO, &pTempData, tempDataLen);
	assert(tempDataLen > 0);
	VIDEO_CONFIG_ROI_INFO *pRoiData = (reinterpret_cast<VIDEO_CONFIG_ROI_INFO*>(pTempData));
	memcpy(m_pROIInfo,pRoiData,tempDataLen);
}

DLG_END_VALUE CCfgROIDlg::PopupSetROIDlg(int chnn, VIDEO_CONFIG_ROI_INFO& ROIInfo)
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

	m_setROIDlg.SetCurrentChnn(chnn);
	m_setROIDlg.SetROIInfo(ROIInfo);
	retVal = m_setROIDlg.DoModal(this);
	if(DLG_OK == retVal)
	{
		m_setROIDlg.GetROIInfo(ROIInfo);
		//在SDK转换时，需要用的 localVedioNum,发送这个消息就是去处理它
		/*pMsgMan->SendMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_IMAGE_ROI,NULL,0,msgData);
		pMsgMan->ReleaseMsgBuff( msgData );*/
		pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID,LOCAL_DEVICE_ID,CMD_REQUEST_SET_IMAGE_ROI,NULL,0);

	}

	//返回之前的大画面状态
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0 , msgData );
	pMsgMan->ReleaseMsgBuff( msgData );

	return retVal;
}

BEGIN_MSG_MAP(CCfgROIDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgROIDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if(COL_VIDEO_ROI == col)
	{
		if (m_rowChnnMap.size() <= 0){
			return KEY_VIRTUAL_MSG_STOP;
		}
		int chnn = m_rowChnnMap.find(row)->second;
		int netChnn = m_rowChnnMap.find(row)->second - m_localInputNum;

		if(DLG_OK == PopupSetROIDlg(chnn, m_pROIInfo[netChnn]))
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
		case COL_VIDEO_ROI:
			if (IsChange(NCFG_ITEM_CAM_ROI_INFO))
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

bool CCfgROIDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_CAM_ROI_INFO:
		{
			int numChnn = maxChnn;
			assert(len == sizeof(VIDEO_CONFIG_ROI_INFO) * numChnn);
			assert(m_buffLen == len);

			memcpy(m_pROIInfo, pData, len);
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgROIDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);

	const GUI::CListCtrl& list = m_listCtrl;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_CAM_ROI_INFO:
		{
			int numChnn = maxChnn;  //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
			assert(len == sizeof(VIDEO_CONFIG_ROI_INFO) * numChnn);
			assert(m_buffLen == len);

			memcpy(pData, m_pROIInfo, len);
		}
		return true;
	default:
		return false;
	}
}