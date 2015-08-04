/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgCruisePresetDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgCruisePresetDlg.h"
#include "ListCtrlEx.cpp"
#include "NetConfigDefine.h"
#include "MessageMan.h"
#include "CfgDlg.h"
#include "CruisePreviewDlg.h"

//////////////////////////////////////////////////////////////////////////
template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	PTZ_CURISE_POINT &it = node;

	if (0 == col)
	{
		char str [8] = {0};
		sprintf(str, "%d", it.preset + 1);
		m_str = str;
	}

	if (1 == col)
	{
		char str [8] = {0};
		sprintf(str, "%d", it.dwellSpeed);
		m_str = str;
	}

	if (2 == col)
	{
		char str [8] = {0};
		sprintf(str, "%d", it.dwellTime);
		m_str = str;
	}

	return m_str;
}
//////////////////////////////////////////////////////////////////////////

CCfgCruisePresetDlg::CCfgCruisePresetDlg() : m_curCurise(0)
{
	
}

CCfgCruisePresetDlg::~CCfgCruisePresetDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CCfgCruisePresetDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_CRUISE_PRESET), false);
	
	if (g_p_login_succ->productInfo.localVideoInputNum > CMessageMan::Instance()->CurrPTZChnn())
	{
		SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_BTN_APPLY );
		SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_PREVIEW ));
	}
	else
	{
		SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	}
	SetDefaultBtnCaption(DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));
		
	//////////////////////////////////////////////////////////////////////////
	GUI::CRect rcClient(15, m_length[TITLE_HEIGHT] + m_length[COMBO_HEIGHT] + 25, m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS], m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS] - 5);
	int btn_cx = 0;
	int btn_cy = 0;
	GetImageSize(BMP_BTN_ADD, btn_cx, btn_cy);

//	int cruise_btn_x = rcClient.m_left;
//	int cruise_x = cruise_btn_x + btn_cx + 5;
	int preset_x = rcClient.m_left;
	int preset_btn_x = rcClient.m_left + rcClient.Width() - btn_cx;
	int btn_offset_y = btn_cx + 5;

	char sz[16] = {0};
	//////////////////////////////////////////////////////////////////////////
	int x = rcClient.m_left;
	int y = m_length[TITLE_HEIGHT] + 15;
	int cx = 120;

	
	cx = 60;
	m_stIndex.Create(CTRLID_STATIC, x, y, cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_NUMBER));

	x += cx;
	cx = 120;
	m_comboIndex.Create(GetRandWndID(), x, y, cx, m_length[COMBO_HEIGHT], this, 0, 1);
	for (std::vector<CRUISE_INFO>::const_iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
	{
		sprintf(sz, "%d", iter->index + 1);
		m_comboIndex.AddItem(sz, iter->index);
	}
	m_comboIndex.SetCurItemData(m_curCurise, false);

	x += cx + 20;
	m_stCruiseName.Create(CTRLID_STATIC, x, y, cx, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_NAME));

	cx = 80;
	x += cx;
	cx = 295;
	m_editName.Create(GetRandWndID(), x, y, cx, m_length[EDIT_HEIGHT], this, 1, 1);
	for (std::vector<CRUISE_INFO>::const_iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
	{
		if (m_curCurise == iter->index)
		{
			m_editName.SetCaption(iter->strName, false);
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Cruise Button 
	y = rcClient.m_top + m_length[LIST_HEAD_HEIGHT];
	//Preset List
	m_listCtrlEx.Create(GetRandWndID(), preset_x, rcClient.m_top, preset_btn_x - preset_x - 5, rcClient.Height(), this, 0, 9);
	m_listCtrlEx.EnableSingleSelect(); 
	
	int listPresetCol_size = (preset_btn_x - preset_x) / 3;	
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_PRESET), listPresetCol_size);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_SPEED), listPresetCol_size);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_TIME), listPresetCol_size - 6);

	//Preset Button
	y = rcClient.m_top + m_length[LIST_HEAD_HEIGHT];
	m_btnPresetAdd.Create(GetRandWndID(), preset_btn_x, y, btn_cx, btn_cy, this, 0, 2/*, m_pStrTable->String(E_STR_ID_ADD)*/);
	m_btnPresetAdd.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_ADD, false);
	m_btnPresetAdd.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_ADD_FOCUS, false);
	m_btnPresetAdd.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_ADD, false);
	
	y += btn_offset_y;
	m_btnPresetDel.Create(GetRandWndID(), preset_btn_x, y, btn_cx, btn_cy, this, 0, 3/*, m_pStrTable->String(E_STR_ID_DELETE)*/);
	m_btnPresetDel.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_DELETE, false);
	m_btnPresetDel.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_DELETE_FOCUS, false);
	m_btnPresetDel.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_DELETE, false);

	y += btn_offset_y;
	m_btnPresetModify.Create(GetRandWndID(), preset_btn_x, y, btn_cx, btn_cy, this, 0, 4/*, m_pStrTable->String(E_STR_ID_MODIFY)*/);
	m_btnPresetModify.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_MODIFY, false);
	m_btnPresetModify.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_MODIFY_FOCUS, false);
	m_btnPresetModify.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_MODIFY, false);
	
	y += btn_offset_y;
	m_btnPresetTop.Create(GetRandWndID(), preset_btn_x, y, btn_cx, btn_cy, this, 0, 5);
	m_btnPresetTop.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOP, false);
	m_btnPresetTop.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOP_FOCUS, false);
	m_btnPresetTop.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_TOP, false);

	y += btn_offset_y;
	m_btnPresetUp.Create(GetRandWndID(), preset_btn_x, y, btn_cx, btn_cy, this, 0, 6);
	m_btnPresetUp.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_UP, false);
	m_btnPresetUp.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_UP_FOCUS, false);
	m_btnPresetUp.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_UP, false);

	y += btn_offset_y;
	m_btnPresetDown.Create(GetRandWndID(), preset_btn_x, y, btn_cx, btn_cy, this, 0, 7);
	m_btnPresetDown.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_DOWN, false);
	m_btnPresetDown.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_DOWN_FOCUS, false);
	m_btnPresetDown.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_DOWN, false);

	y += btn_offset_y;
	m_btnPresetBottom.Create(GetRandWndID(), preset_btn_x, y, btn_cx, btn_cy, this, 0, 8);
	m_btnPresetBottom.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_BOTTOM, false);
	m_btnPresetBottom.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_BOTTOM_FOCUS, false);
	m_btnPresetBottom.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_BOTTOM, false);

	//////////////////////////////////////////////////////////////////////////

	//停止云台正在进行的动作
	unsigned long curChnn = CMessageMan::Instance()->CurrPTZChnn();
	MESSAGE_DATA msgData;
	memset( &msgData, 0, sizeof( MESSAGE_DATA ) );
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_STOP_CURRENT_STATUS, reinterpret_cast<unsigned char *>( &curChnn), sizeof( unsigned long ), msgData );
	CMessageMan::Instance()->ReleaseMsgBuff( msgData );

	////初始化显示
	//m_listCruise.SetCurSingleSelect(0, false);
	//ShowCuriseList(false);
	ShowPresetList(m_curCurise, false);
	
	////
	CreateTipWnd();
}

void CCfgCruisePresetDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
}

void CCfgCruisePresetDlg::EndDialog(DLG_END_VALUE result)
{
	if( DLG_ESC == result )
	{
		//先发送恢复巡航预览
		unsigned long chnn = CMessageMan::Instance()->CurrPTZChnn();
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CRUISE_PREVIEW_RECOVER, reinterpret_cast< unsigned char * >( &chnn ), sizeof( unsigned long ) );
	}

	GUI::CDialog::EndDialog(result);

}

void CCfgCruisePresetDlg::SetTip()
{
	AddTip(m_btnPresetAdd.GetID(),E_STR_ID_ADD);
	AddTip(m_btnPresetDel.GetID(),E_STR_ID_DELETE);
	AddTip(m_btnPresetModify.GetID(),E_STR_ID_SETUP);
	AddTip(m_btnPresetTop.GetID(),E_STR_ID_TOP);
	AddTip(m_btnPresetUp.GetID(),E_STR_ID_UP);
	AddTip(m_btnPresetDown.GetID(),E_STR_ID_DOWN);
	AddTip(m_btnPresetBottom.GetID(),E_STR_ID_BOTTOM);
	AddTip(m_pDefaultBtn[2].GetID(), E_STR_ID_MESSAGE_99);
}

DLG_END_VALUE CCfgCruisePresetDlg::PopEditDlg(CCruiseEditDlg& editDlg, GUI::CButton& btn)
{
	int dlg_w = 230;
	int dlg_h = 180;

	//如果超出屏幕高度和宽度
	GUI::CRect rect = btn.GetWindowRect();
	int top = rect.m_bottom + 1;
	int left = rect.m_right - dlg_w;

	DLG_END_VALUE ret = editDlg.DoModal(NULL, left, top, dlg_w, dlg_h);

	GUI::CWnd* pWndTop = GetTopestWnd();
	if (NULL != pWndTop)
	{
		pWndTop->Repaint();
	}		

	return ret;
}

void CCfgCruisePresetDlg::ShowPresetList(int index, bool bRefresh)
{
	m_listCtrlEx.PutData(NULL, 0, false);//清空

	for (CRUISE_INFO_LIST::const_iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
	{
		if (iter->index == index)
		{
			const CRUISE_POINT_LIST& point_list = iter->point_list;
			for (CRUISE_POINT_LIST::const_iterator point_iter = point_list.begin(); point_iter != point_list.end(); ++point_iter)
			{
				m_listCtrlEx.AddItem(*point_iter, false);
			}
			break;
		}		
	}

	if (bRefresh)
	{
		Repaint();
	}
}



///********************************************************************************************************************************
//*
//*Function:
//*			SendCruisePreviewCmd( CRUISE_INFO &cruiseInfo )
//*Description:
//*			发送巡航线预览命令到服务端，该命令只刷新cruiseInfo中的信息到PTZ，
//*			需 要调用StartCruise( CRUISE_INFO &cruiseInfo )后PTZ中新的巡航线才被启动
//*Input:		
//*			cruiseInfo 中存放相关巡航线信息
//*Output:
//*
//*Modify:
//********************************************************************************************************************************/

bool CCfgCruisePresetDlg::SendCruisePreviewCmd( const CRUISE_INFO &cruiseInfo )
{
	//发送到服务端巡航线预览数据格式为： 通道号|巡航线号|预置点个数|预置点信息
	int dataLen = sizeof( unsigned long ) + sizeof( unsigned long ) + sizeof( unsigned long ) + sizeof( PTZ_CURISE_POINT ) * cruiseInfo.point_list.size();
	unsigned char *pData = new unsigned char[dataLen];
	assert( NULL != pData );
	memset( pData, 0, dataLen );
	
	unsigned long *pIndex = reinterpret_cast< unsigned long * >( pData );
	*pIndex = CMessageMan::Instance()->CurrPTZChnn();			//通道号
	*(++pIndex) = cruiseInfo.index;	//巡航线号
	*(++pIndex) = cruiseInfo.point_list.size();		//该巡航线中的预置点个数

	PTZ_CURISE_POINT *pCruisePoint = reinterpret_cast< PTZ_CURISE_POINT *>( pData + sizeof( unsigned long ) * 3 );
	for( CRUISE_POINT_LIST::const_iterator iter = cruiseInfo.point_list.begin(); iter != cruiseInfo.point_list.end(); ++iter, ++pCruisePoint )
	{
		*pCruisePoint = *iter;
	}

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CRUISE_PREVIEW, pData, dataLen, msgData);
	
	if( NULL != pData )
	{
		delete [] pData;
		pData = NULL;
	}

	assert( NULL != msgData.pData );
	PACKCMD *pPackcmd = reinterpret_cast<PACKCMD*>(msgData.pData);
	if( CMD_REPLY_PTZ_CRUISE_PREVIEW_SUC == pPackcmd->cmdType )
	{
		//printf( "%s:%s:%d, 设置预览巡航点到云台成功\n", __FILE__, __FUNCTION__, __LINE__);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		return true;
	}
	else
	{
		//printf( "%s:%s:%d, 设置预览巡航点到云台失败\n", __FILE__, __FUNCTION__, __LINE__);
		MessageOut( GetTopestWnd(), E_STR_ID_MESSAGE_101, TIP_MSG );
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		return false;
	}
}

bool CCfgCruisePresetDlg::StartCruisePreview( const CRUISE_INFO &cruiseInfo )
{
	//向云台发送启动巡航命令

	PTZ_CTRL ptzCtrl;
	memset( &ptzCtrl, 0, sizeof( PTZ_CTRL ) );
	
	ptzCtrl.chnn =  CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_CRUISE_RUN;
	ptzCtrl.cmdValue = cruiseInfo.index;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	assert( NULL != msgData.pData );
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
	if( CMD_REPLY_PTZ_SEND_CMD_SUC == pPackCmd->cmdType )
	{
		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		//printf("%s:%s:%d, 开始巡航预览\n", __FILE__, __FUNCTION__, __LINE__);
		return true;
	}
	else
	{
		MessageOut( GetTopestWnd(), E_STR_ID_MESSAGE_101, TIP_MSG );

		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		//printf("%s:%s:%d, 开始巡航预览失败\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}

}


bool CCfgCruisePresetDlg::StopCruisePreview( const CRUISE_INFO &cruiseInfo )
{
	//向云台发送停止巡航命令
	PTZ_CTRL ptzCtrl;
	memset( &ptzCtrl, 0, sizeof( PTZ_CTRL ) );

	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_CRUISE_STOP;
	ptzCtrl.cmdValue =cruiseInfo.index;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	assert( NULL != msgData.pData );
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
	if( CMD_REPLY_PTZ_SEND_CMD_SUC == pPackCmd->cmdType )
	{
		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		//printf("%s:%s:%d, 停止巡航预览\n", __FILE__, __FUNCTION__, __LINE__);
		return true;
	}
	else
	{
		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		//printf("%s:%s:%d, 停止巡航预览失败\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}

}

/**************************************************************************************************

函数：		PopupCruisePreviewDlg()
功能：		隐藏该界面，弹出透明的预览对话框界面
参数：
			无
返回值：	
			无
作者：		ZHL 2010-02-27

***************************************************************************************************/

void CCfgCruisePresetDlg::PopupCruisePreviewDlg()
{
	NET_DISPLAY_INFO dispInfo;
	memset(&dispInfo,0,sizeof(NET_DISPLAY_INFO));
	dispInfo.type = NET_DISPLAY_CHANGE_DATA_PACK;
	dispInfo.split_mode[DISPLAY_LAYER_ANALOG] = VIEW_SPLIT_1X1;
	dispInfo.rec = 0;
	dispInfo.time_out = -1;
	
	if( g_p_login_succ->productInfo.localVideoInputNum <= CMessageMan::Instance()->CurrPTZChnn())
	{
		dispInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, 0, 0);
	}
	else
	{
		dispInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, 0, CMessageMan::Instance()->CurrPTZChnn());
	}
	dispInfo.data_len = 1;

	//启动单屏
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&dispInfo, sizeof(NET_DISPLAY_INFO), msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	CCruisePreviewDlg  cruisePreviewDlg;
	//printf("%s:%s:%d, 弹出巡航预览界面\n", __FILE__, __FUNCTION__, __LINE__);
	cruisePreviewDlg.DoModal(this,CENTER_POS,CENTER_POS,MAX_SCREEN_X,MAX_SCREEN_Y,false);

	//恢复屏幕
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0, msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
}


unsigned long CCfgCruisePresetDlg::OnOK()
{
	for (std::vector<CRUISE_INFO>::iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
	{
		if (m_curCurise == iter->index)
		{
			iter->strName = m_editName.GetCaption();
			break;
		}
	}

	return GUI::CDialog::OnOK();
}



unsigned long CCfgCruisePresetDlg::OnApply()
{
	//printf("%s:%s:%d, 开始巡航线预览\n", __FILE__, __FUNCTION__, __LINE__);

	for( CRUISE_INFO_LIST::const_iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter )
	{
		if (iter->index == m_curCurise )
		{
			if( 0 == iter->point_list.size())
			{
				//要预览的巡航线不能为空
				MessageOut( GetTopestWnd(), E_STR_ID_MESSAGE_100, TIP_MSG );
				return GUI::CDialog::OnApply();
			}

			//设置预览的巡航点到云台
			if( SendCruisePreviewCmd( *iter ) )
			{
				//调用设置好的预览巡航线
				if( StartCruisePreview( *iter ) )
				{
					//弹出本通道大画面
					PopupCruisePreviewDlg();

					//退出巡航
					StopCruisePreview( *iter );
				}

			}			

			break;
		}		
	}

	return GUI::CDialog::OnApply();
}


BEGIN_MSG_MAP(CCfgCruisePresetDlg, GUI::CDialog)
	ON_MSG(m_btnPresetAdd.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnAdd)
	ON_MSG(m_btnPresetDel.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnDel)
	ON_MSG(m_btnPresetTop.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnTop)
	ON_MSG(m_btnPresetBottom.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnBottom)
	ON_MSG(m_btnPresetUp.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnUp)
	ON_MSG(m_btnPresetDown.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnDown)

	ON_MSG(m_listCtrlEx.GetID(), KEY_VIRTUAL_DBCLICK, OnClickBtnModify)
	ON_MSG(m_btnPresetModify.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnModify)
	ON_MSG(m_comboIndex.GetID(), KEY_VIRTUAL_ENTER, OnChangeCruise)
	ON_MSG(m_comboIndex.GetID(), KEY_VIRTUAL_ADD, OnChangeCruise)
	ON_MSG(m_comboIndex.GetID(), KEY_VIRTUAL_DEC, OnChangeCruise)
END_MSG_MAP()

unsigned long CCfgCruisePresetDlg::OnChangeCruise()
{	
	for (std::vector<CRUISE_INFO>::iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
	{
		if (m_curCurise == iter->index)
		{
			iter->strName = m_editName.GetCaption();
			break;
		}
	}
		
	m_curCurise = m_comboIndex.GetCurItemData();
	ShowPresetList(m_curCurise, true);

	for (std::vector<CRUISE_INFO>::const_iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
	{
		if (m_curCurise == iter->index)
		{
			m_editName.SetCaption(iter->strName, true);
			break;
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgCruisePresetDlg::OnClickBtnAdd()
{
	std::vector<CRUISE_INFO>::iterator iter ;
	for (iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
	{
		if (m_curCurise == iter->index)
		{
			break;
		}
	}
	
	if( iter == m_CruiseList.end() )
	{
		assert( 0 );
		return KEY_VIRTUAL_MSG_STOP;
	}

	if( iter->point_list.size() < CCfgDlg::s_loginSuccessReply.productInfo.presetNumOneCruise )
	{
		CCruiseEditDlg editDlg;
		if (DLG_OK == PopEditDlg(editDlg, m_btnPresetAdd))
		{
			PTZ_CURISE_POINT point;
			editDlg.GetCurisePoint(point);
			//assert(m_curCurise < m_CruiseList.size());
			if (point.preset == 0xff)
			{
				return KEY_VIRTUAL_MSG_STOP;
			}

			iter->point_list.push_back(point);

			ShowPresetList(m_curCurise, false);
			m_listCtrlEx.SetSingleSelect(m_listCtrlEx.GetCount() - 1, true);
		}
	}
	else
	{
		char sz[128] = {0};
		snprintf( sz, 128, "%s %d!",  m_pStrTable->String( E_STR_ID_MESSAGE_48 ),  CCfgDlg::s_loginSuccessReply.productInfo.presetNumOneCruise );
		MessageBox( sz, m_pStrTable->String( E_STR_ID_TITLE_ERROR ), DEFAULT_BTN_OK|DEFAULT_ICON_ERROR );
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgCruisePresetDlg::OnClickBtnDel()
{
	assert(m_curCurise < m_CruiseList.size());
	int sel = m_listCtrlEx.GetSingleSelect();
	for (std::vector<CRUISE_INFO>::iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
	{
		if( m_curCurise == iter->index)
		{
			if ((sel >= 0) && (sel < m_listCtrlEx.GetCount()))
			{
				iter->point_list.erase(iter->point_list.begin() + sel);
				ShowPresetList(m_curCurise, false);

				if (sel > 0)
				{
					m_listCtrlEx.SetSingleSelect(sel - 1, true);
				} 
				else if (m_listCtrlEx.GetCount() > 0)
				{
					m_listCtrlEx.SetSingleSelect(0, true);
				}
				else
				{
					m_listCtrlEx.Repaint();
				}
			}
		}
		else
		{
			continue;
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgCruisePresetDlg::OnClickBtnTop()
{
	int sel = m_listCtrlEx.GetSingleSelect();
	if ((sel >= 0) && (sel < m_listCtrlEx.GetCount()))
	{
		//assert(m_curCurise < m_CruiseList.size());
		
		for (std::vector<CRUISE_INFO>::iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
		{
			if (m_curCurise == iter->index)
			{
				PTZ_CURISE_POINT temp = iter->point_list[sel];
				iter->point_list.erase(iter->point_list.begin() + sel);
				iter->point_list.insert(iter->point_list.begin(), temp);
				break;
			}
		}
		//.erase(cruise_point_list.begin() + sel);



		ShowPresetList(m_curCurise, false);
		m_listCtrlEx.SetSingleSelect(0, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CCfgCruisePresetDlg::OnClickBtnBottom()
{
	int sel = m_listCtrlEx.GetSingleSelect();
	if ((sel >= 0) && (sel < m_listCtrlEx.GetCount()))
	{
		//assert(m_curCurise < m_CruiseList.size());
		for (std::vector<CRUISE_INFO>::iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
		{
			if (m_curCurise == iter->index)
			{
				PTZ_CURISE_POINT temp = iter->point_list[sel];
				iter->point_list.erase(iter->point_list.begin() + sel);
				iter->point_list.push_back(temp);

				ShowPresetList(m_curCurise, false);
				m_listCtrlEx.SetSingleSelect(iter->point_list.size() - 1, true);
				break;
			}
		}
		//.erase(cruise_point_list.begin() + sel);

	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgCruisePresetDlg::OnClickBtnUp()
{
	int sel = m_listCtrlEx.GetSingleSelect();
	if ((sel > 0) && (sel < m_listCtrlEx.GetCount()))
	{
		//assert(m_curCurise < m_CruiseList.size());
		for (std::vector<CRUISE_INFO>::iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
		{
			if (m_curCurise == iter->index)
			{
				PTZ_CURISE_POINT temp = iter->point_list[sel];
				iter->point_list.erase(iter->point_list.begin() + sel);
				iter->point_list.insert(iter->point_list.begin() + sel - 1, temp);

				break;
			}
		}
		//.erase(cruise_point_list.begin() + sel);


		ShowPresetList(m_curCurise, false);
		m_listCtrlEx.SetSingleSelect(sel - 1, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgCruisePresetDlg::OnClickBtnDown()
{
	int sel = m_listCtrlEx.GetSingleSelect();
	if ((sel >= 0) && (sel < m_listCtrlEx.GetCount() - 1))
	{
		//assert(m_curCurise < m_CruiseList.size());
		for (std::vector<CRUISE_INFO>::iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
		{
			if (m_curCurise == iter->index)
			{
				PTZ_CURISE_POINT temp = iter->point_list[sel];
				iter->point_list.erase(iter->point_list.begin() + sel);
				iter->point_list.insert(iter->point_list.begin() + sel + 1, temp);

				break;
			}
		}
		//.erase(cruise_point_list.begin() + sel);


		ShowPresetList(m_curCurise, false);
		m_listCtrlEx.SetSingleSelect(sel + 1, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CCfgCruisePresetDlg::OnClickBtnModify()
{
	int sel = m_listCtrlEx.GetSingleSelect();
	if ((sel >= 0) && (sel < m_listCtrlEx.GetCount()))
	{
		CCruiseEditDlg editDlg;
		//assert(m_curCurise < m_CruiseList.size());
		
		for (std::vector<CRUISE_INFO>::iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
		{
			if (m_curCurise == iter->index)
			{
				editDlg.SetCurisePoint(iter->point_list[sel]);

				if (DLG_OK == PopEditDlg(editDlg, m_btnPresetModify))
				{
					editDlg.GetCurisePoint(iter->point_list[sel]);
					ShowPresetList(m_curCurise, true);
				}	

				break;
			}
		}	
	}
	return KEY_VIRTUAL_MSG_STOP;
}

