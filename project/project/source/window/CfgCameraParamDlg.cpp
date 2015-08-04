/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xian shi wei
* Date         : 2014-10-25
* Name         : CfgCameraParamDlg.cpp
* Version      : 1.0
* Description  : 
* Modify Record:
1:
*/

#include "CfgCameraParamDlg.h"
#include "FuncCustom.h"

void CCfgCameraParamDlg::OnInitial()
{
	CCfgDlg::OnInitial();

	GUI::CListCtrl& list = m_cfgCamParamList;

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
	for(int i = localVideoInputNum; i < maxChnn; i++){
		if((CH >> i) & 0x1){
			m_rowChnnMap.insert(std::make_pair(row++, i));
		}
	}	
	if(effectChnn > maxListRow){
		offsetEdge = m_length[SCROLL_WIDTH] + 2;}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CAMERA_PARAMTER), width * 29 / 32);

	char str[64] = {0};
	for(unsigned int i =0; i<m_rowChnnMap.size();i++)
	{
		int chnnIndex = m_rowChnnMap.find(i)->second;
		sprintf(str, "%d", chnnIndex + 1 /*- localVideoInputNum*/);

		list.AddItem(i, CAM_CHNN, str);
		GUI::CButton* pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(i, CAM_PARA, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]));
		assert(pBtn);
		pBtn->SetCaption(m_pStrTable->String(E_STR_ID_SETTING), false);
	}
	list.CreateListFocusMap();

	//create storage space
	if ( NULL != m_pCamPara){
		delete [] m_pCamPara;
	}
	m_pCamPara = new CAMERA_PARAMTER[netVideoInputNum];
	m_buffLen = netVideoInputNum * sizeof(CAMERA_PARAMTER);
	memset(m_pCamPara,0,m_buffLen);

	if ( NULL != m_pCamParaChange){
		delete [] m_pCamParaChange;
	}
	m_pCamParaChange = new CAMERA_PARAMTER[netVideoInputNum];
	memset(m_pCamParaChange,0,m_buffLen);
}

void CCfgCameraParamDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();
	if ( NULL != m_pCamPara){
		delete [] m_pCamPara;
		m_pCamPara = NULL;
	}
	if ( NULL != m_pCamParaChange){
		delete [] m_pCamParaChange;
		m_pCamParaChange = NULL;
	}
	m_rowChnnMap.clear();
}

void CCfgCameraParamDlg::SetTip()
{

}
void CCfgCameraParamDlg::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	CCfgDlg::OnChangeFocus(oldID,newID);
}

void CCfgCameraParamDlg::UpdateView(bool bRefresh)
{

}
unsigned long CCfgCameraParamDlg::RecoverChange()
{
	return 0;
}

bool CCfgCameraParamDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_cfgCamParamList;

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;
    int numChnn = netVideoInputNum;
	switch (cfgID)
	{
	case NCFG_ITEM_CAM_MIRROR:
		{
			assert(len == sizeof(unsigned char) * numChnn);
			const unsigned char *pValue = reinterpret_cast<const unsigned char *>(pData);
			assert(m_pCamPara);
			for (int i= 0; i<netVideoInputNum;i++)
			{
				m_pCamPara[i].m_mirror = *pValue++;
			}
			return true;
		}
	case NCFG_ITEM_CAM_FILP:
		{
			assert(len == sizeof(unsigned char) * numChnn);
			const unsigned char *pValue = reinterpret_cast<const unsigned char *>(pData);
			assert(m_pCamPara);
			for (int i= 0; i<netVideoInputNum;i++,pValue++)
			{
				m_pCamPara[i].m_flip = *pValue;
			}
			return true;
		}
	case NCFG_ITEM_CAM_ROTATO:
		{
			assert(len == sizeof(unsigned long)*numChnn);
			const unsigned long *pValue = reinterpret_cast<const unsigned long *>(pData);
			assert(m_pCamPara);
			for (int i= 0; i<netVideoInputNum;i++,pValue++){
				m_pCamPara[i].m_rotato = *pValue;
			}
			return true;
		}
	case NCFG_ITEM_CAM_LENS_DISTORTION:
		{
			assert(len == sizeof(LENS_DISTORTION_CONFIG) * numChnn);
			const LENS_DISTORTION_CONFIG *pValue = reinterpret_cast<const LENS_DISTORTION_CONFIG *>(pData);
			assert(m_pCamPara);
			for (int i= 0; i<netVideoInputNum;i++,pValue++)
			{
				m_pCamPara[i].m_lensDistortion.ucLensDistortion = pValue->ucLensDistortion;
				m_pCamPara[i].m_lensDistortion.ucLensDistortionValue = pValue->ucLensDistortionValue;
				m_pCamPara[i].m_lensDistortion.noUse[0] = pValue->noUse[0];
				m_pCamPara[i].m_lensDistortion.noUse[1] = pValue->noUse[1];
			}
			return true;
		}
	case NCFG_ITEM_CAM_TIME_FOCUS_MODE:
		{
			return true;
		}
	default:
		return false;
	}
	return false;
}

bool CCfgCameraParamDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CListCtrl& list = m_cfgCamParamList;
	const GUI::CListCtrl& listAll = m_cfgCamParamListAll;

	/*bool bAll = false;
	{
		const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd( 0, COL_CHNN ) );
		assert( pCheck );
		bAll = pCheck->IsCheck();
	}*/

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

    int numChnn = netVideoInputNum;
	switch (cfgID)
	{
	case NCFG_ITEM_CAM_MIRROR:
		{
			assert(len == sizeof(unsigned char) * numChnn);
			unsigned char *pValue = reinterpret_cast<unsigned char *>(pData);
			assert(m_pCamPara);
			for (int i= 0; i<netVideoInputNum;i++,pValue++)
			{
				*pValue =m_pCamPara[i].m_mirror;
			}
			return true;
		}
	case NCFG_ITEM_CAM_FILP:
		{
			assert(len == sizeof(unsigned char) * numChnn);
			unsigned char *pValue = reinterpret_cast<unsigned char *>(pData);
			assert(m_pCamPara);
			for (int i= 0; i<netVideoInputNum;i++,pValue++)
			{
				*pValue = m_pCamPara[i].m_flip;
			}
			return true;
		}
	case NCFG_ITEM_CAM_ROTATO:
		{
			assert(len == sizeof(unsigned long)*numChnn);
			unsigned long *pValue = reinterpret_cast<unsigned long *>(pData);
			assert(m_pCamPara);
			for (int i= 0; i<netVideoInputNum;i++,pValue++)
			{
				*pValue = m_pCamPara[i].m_rotato;
			}
			return true;
		}
	case NCFG_ITEM_CAM_LENS_DISTORTION:
		{
			assert(len == sizeof(LENS_DISTORTION_CONFIG) * numChnn);
			LENS_DISTORTION_CONFIG *pValue = reinterpret_cast<LENS_DISTORTION_CONFIG *>(pData);
			assert(m_pCamPara);
			for (int i= 0; i<netVideoInputNum;i++,pValue++)
			{
				pValue->ucLensDistortion =  m_pCamPara[i].m_lensDistortion.ucLensDistortion;
				pValue->ucLensDistortionValue =  m_pCamPara[i].m_lensDistortion.ucLensDistortionValue;
				pValue->noUse[0] = m_pCamPara[i].m_lensDistortion.noUse[0];
				pValue->noUse[1] = m_pCamPara[i].m_lensDistortion.noUse[1];
			}
			return true;
		}
	case NCFG_ITEM_CAM_TIME_FOCUS_MODE:
		{
			return true;
		}
	default:
		return false;
	}
	return false;
}

BEGIN_MSG_MAP(CCfgCameraParamDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgCamParamList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_cfgCamParamListAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
//	ON_MSG(m_liveRecStatus.GetID(),KEY_VIRTUAL_ENTER,OnCheck)
END_MSG_MAP()

unsigned long CCfgCameraParamDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
    unsigned short xOff = m_width - CSetCamParaDlg::GetWidth() - CSetCamParaDlg::CFG_DLG_RIGHT_X;
	GUI::DLG_END_VALUE camDlgValue = DLG_ESC;
	if( CAM_PARA == col )
	{
			ULONGLONG CH = 0;
			int localVideoInputNum = 0, netVideoInputNum = 0;
			int maxNum =0,effectChnn = 0;

			IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);
			maxNum = localVideoInputNum + netVideoInputNum;

			if (m_rowChnnMap.size() <= 0){
				return KEY_VIRTUAL_MSG_STOP;
			}
			int chnn = m_rowChnnMap.find(row)->second;

			m_cfgSetParaDlg.SetChnnInfo(m_pCamPara,chnn,netVideoInputNum,localVideoInputNum);
			camDlgValue = PopupCamParaDlg( xOff, CENTER_POS, m_cfgSetParaDlg.GetWidth(),m_cfgSetParaDlg.GetHeight(), chnn ); 
			memset( m_pCamParaChange, 0, ( sizeof( unsigned long ) * netVideoInputNum ) );

			if( camDlgValue == DLG_OK )
			{
				m_cfgSetParaDlg.GetChnnPara(m_pCamPara,netVideoInputNum);
				memcpy( m_pCamParaChange, m_pCamPara, ( sizeof( unsigned long ) * netVideoInputNum ) );
				if (!IsModify())
				{
					if (IsChange(NCFG_ITEM_CAM_MIRROR))
					{
						SetModify(true);
					}
					if (IsChange(NCFG_ITEM_CAM_FILP))
					{
						SetModify(true);
					}
					if (IsChange(NCFG_ITEM_CAM_ROTATO))
					{
						SetModify(true);
					}
					if (IsChange(NCFG_ITEM_CAM_LENS_DISTORTION))
					{
						SetModify(true);
					}
					return KEY_VIRTUAL_MSG_CONTINUE;
				}
			}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgCameraParamDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	return KEY_VIRTUAL_MSG_STOP;
}

DLG_END_VALUE CCfgCameraParamDlg::PopupCamParaDlg( int x, int y, int cx, int cy, int chnn )
{
	CMessageMan *pMsgMan = CMessageMan::Instance();
	DLG_END_VALUE retVal;

	//切换到相关通道的大画面
	NET_DISPLAY_INFO displayInfo;
	memset(&displayInfo, 0, sizeof(NET_DISPLAY_INFO));

	displayInfo.type		= NET_DISPLAY_CHANGE_DATA_PACK;
	displayInfo.time_out	= -1;
	displayInfo.rec			= 0;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	MESSAGE_DATA msgData;
	if(-1 == chnn)
	{
		unsigned char chnnNum =  maxChnn;  //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
		unsigned long splitMode = VIEW_SPLIT_1X1;
		if (4 == chnnNum)
		{
			splitMode= VIEW_SPLIT_2X2;
		}
		else if (5 == chnnNum)
		{
			splitMode= VIEW_SPLIT_2X3;
		}
		else if (9 == chnnNum)
		{
			splitMode= VIEW_SPLIT_3X3;
		}
		else if (8 == chnnNum)
		{
			splitMode= VIEW_SPLIT_3X3;
		}
		else if (16 == chnnNum)
		{
			splitMode= VIEW_SPLIT_4X4;
		}
		else if ((16 < chnnNum) && (chnnNum <= 25))
		{
			splitMode= VIEW_SPLIT_5X5;
		}
		else
		{
			splitMode= VIEW_SPLIT_6X6;
		}

		displayInfo.split_mode[DISPLAY_LAYER_ANALOG] = splitMode;
		displayInfo.split_mode[DISPLAY_LAYER_NET] = splitMode;
		for (int i = 0; i < chnnNum; i++)
		{
			if(i < localVideoInputNum)
			{
				displayInfo.data[i] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, i, i);
			}
			else
			{
				displayInfo.data[i] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, i, i - localVideoInputNum);
			}
		}
		displayInfo.data_len = chnnNum;

		pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE,(unsigned char *)&displayInfo, sizeof(NET_DISPLAY_INFO), msgData );
		pMsgMan->ReleaseMsgBuff( msgData );
	}
	else
	{
		displayInfo.split_mode[DISPLAY_LAYER_ANALOG] = VIEW_SPLIT_1X1;
		displayInfo.split_mode[DISPLAY_LAYER_NET] = VIEW_SPLIT_1X1;

		if(chnn < localVideoInputNum)
		{
			return DLG_ESC;
		}
		else
		{
			displayInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, 0, chnn - localVideoInputNum);
		}

		displayInfo.data_len = 1;

		pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char *)&displayInfo, sizeof(NET_DISPLAY_INFO), msgData );
		pMsgMan->ReleaseMsgBuff( msgData );
	}

	retVal = m_cfgSetParaDlg.DoModal( this, x , y, cx, cy, false);

	//返回之前的大画面状态
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0 , msgData );

	pMsgMan->ReleaseMsgBuff( msgData );

	return retVal;
}
