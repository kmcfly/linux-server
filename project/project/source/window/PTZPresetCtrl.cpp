/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-17
** Name         : PTZPresetCtrl.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "PTZPresetCtrl.h"
#include "PTZ.h"
#include "MessageMan.h"
#include "PTZViewDlg.h"

CPTZPresetCtrl::CPTZPresetCtrl() : m_curPreset(0), m_bSave(false)
{
	m_protocl = 0;
}

CPTZPresetCtrl::~CPTZPresetCtrl()
{

}


void CPTZPresetCtrl::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////
	int base_x = 10;
	int static_cx = 70;
	int edit_cx = m_rect.Width() - base_x*2 - static_cx - 5 - 105;
	int btn_cx = m_length[BTN_WIDTH];
	int cy = m_length[COMBO_HEIGHT] + 8;
	int offset_x = 40;
	char sz[32] = {0};

	int x = 0;
	int y = 10;
	y += cy;
	y += cy;
	m_btnBrush.Create(CTRLID_PTZCONTROL_BRUSH, x, y, 30, 25, this, 0, 1);
	x = 60;
	m_btnLamp.Create(CRTLID_PTZCONTROL_LAMP, x, y, 30, 25, this, 1, 1);

	x += ( 30 + 20 );
	y = 8;
	m_scNumber.Create(CTRLID_STATIC, x, y, static_cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_NUMBER));
	x += static_cx;
	m_comboIndex.Create(GetRandWndID(), x, y, edit_cx, m_length[COMBO_HEIGHT], this, 2, 0);

	for(int i=0; i<MAX_PRESET_NUM; i++)
	{
		sprintf(sz, "%d", i + 1);
		m_comboIndex.AddItem(sz, i);
	}
	m_comboIndex.SetCurItemData(m_curPreset, false);

	y += cy;
	y += cy;
	m_btnSave.Create(GetRandWndID(), x, y, edit_cx, m_length[BTN_HEIGHT], this, 2, 1, m_pStrTable->String(E_STR_ID_SAVE));

	//////////////////////////////////////////////////////////////////////////
	m_bSave = false;

	//停止云台的所有动作
	unsigned long chnn = static_cast< unsigned long >( CMessageMan::Instance()->CurrPTZChnn());
	MESSAGE_DATA msgData;
	memset( &msgData, 0, sizeof( MESSAGE_DATA ) );
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_STOP_CURRENT_STATUS, reinterpret_cast<unsigned char *>( &chnn ), sizeof( unsigned long ), msgData );
	
	CMessageMan::Instance()->ReleaseMsgBuff( msgData );
	
	RefalseBtnCaption(false);

	//改变云台位置到预置点 m_curPreset
	GoToPreset();
}

void CPTZPresetCtrl::OnPaint(PAINT_STATUS status)
{
}


unsigned long CPTZPresetCtrl::ResponseHideCommond( unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	return OnCommand( ctrlID, keyID, xPos, yPos);
}


void CPTZPresetCtrl::SetPresetInfo(int preset_index, const std::vector<PTZ_PRESET>& presetList)
{
	//assert(preset_index < presetList.size());
	m_presetList = presetList;
	m_curPreset = preset_index;
}

void CPTZPresetCtrl::ChangeBtnCaption(bool bChnnChanged)
{
	if(bChnnChanged)
	{
		////停止云台的所有动作
		//unsigned long chnn = static_cast< unsigned long >( CMessageMan::Instance()->CurrPTZChnn());
		//MESSAGE_DATA msgData;
		//memset( &msgData, 0, sizeof( MESSAGE_DATA ) );
		//CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_STOP_CURRENT_STATUS, reinterpret_cast<unsigned char *>( &chnn ), sizeof( unsigned long ), msgData );

		//CMessageMan::Instance()->ReleaseMsgBuff( msgData );

		////切换通道后要调用预置点
		//m_curPreset = chnn; 
		//m_comboIndex.SetCurItemData(m_curPreset, true);
		GoToPreset();
		PUB_Sleep(100);
	}
	else
	{

	}

	RefalseBtnCaption();
}

BEGIN_MSG_MAP(CPTZPresetCtrl, GUI::CDialog)
	ON_MSG(m_btnSave.GetID(), KEY_VIRTUAL_ENTER, OnSave)
	ON_MSG(m_comboIndex.GetID(), KEY_VIRTUAL_ENTER, OnClickComboPreset)
	ON_MSG(m_comboIndex.GetID(), KEY_VIRTUAL_ADD, OnClickComboPreset)
	ON_MSG(m_comboIndex.GetID(), KEY_VIRTUAL_DEC, OnClickComboPreset)

	ON_MSG(m_btnBrush.GetID(), KEY_VIRTUAL_ENTER, OnClickBrush)
	ON_MSG(m_btnLamp.GetID(), KEY_VIRTUAL_ENTER, OnClickLamp)

	ON_MSG( GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_SET_PRESET, OnSave )
#ifndef __DVR_ULTIMATE__
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_BRUSH, OnClickBrush)
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_LAMP, OnClickLamp)
#endif
END_MSG_MAP()

unsigned long CPTZPresetCtrl::OnSave()
{
	CMessageMan *pMessageMan = CMessageMan::Instance();
	PACKCMD *pPackCmd = NULL;
	MESSAGE_DATA msgData;
	bool bUseMY1003 = false;
	
	int savePresetNum = m_curPreset;
	if( ( m_CommandPram >> 8 ) == KEY_TYPE_MY1003 )
	{
		//名扬键盘操作
		savePresetNum = (  ( m_CommandPram & 0xff ) - 1 );
		bUseMY1003 = true;
	}


	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = pMessageMan->CurrPTZChnn();
	ptzCtrl.cmdType  = PTZ_CMD_PRESET_SET;
	ptzCtrl.cmdValue = savePresetNum; 
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	pMessageMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	assert( NULL != msgData.pData );
	pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
	if( CMD_REPLY_PTZ_SEND_CMD_FAIL == pPackCmd->cmdType )
	{	
		pMessageMan->ReleaseMsgBuff( msgData );
	}
	else if( CMD_REPLY_PTZ_SEND_CMD_SUC == pPackCmd->cmdType )
	{
		pMessageMan->ReleaseMsgBuff( msgData );

		//保存预置点成功设置该标记
		m_bSave = true;

		////名扬键盘操作预置点成功时要刷新界面
		if( bUseMY1003 )
		{
			//for (std::vector<PTZ_PRESET>::iterator iterCur = m_presetList.begin(); iterCur != m_presetList.end(); ++iterCur)
			//{
			//	if (iterCur->index == m_curPreset)
			//	{
			//		strcpy(iterCur->name,  m_editName.GetCaption().c_str());		
			//		break;
			//	}
			//}

			m_comboIndex.SetCurItemData( savePresetNum, true );
			m_curPreset = m_comboIndex.GetCurItemData();

			//for (std::vector<PTZ_PRESET>::iterator iterCur = m_presetList.begin(); iterCur != m_presetList.end(); ++iterCur)
			//{
			//	if (iterCur->index == m_curPreset)
			//	{
			//		m_editName.SetCaption(iterCur->name, true);
			//		break;
			//	}
			//}
		}
		//else
		//{
		//	//使用其他设备保存预置点成功时要保存预置点名字
		//	strcpy(iter->name, m_editName.GetCaption().c_str());
		//}

	}
	else
	{
		pMessageMan->ReleaseMsgBuff( msgData );
	}



	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZPresetCtrl::OnClickBrush()
{
	PTZ_CTRL ptzCtrl;
	memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.speed = CMessageMan::Instance()->CurrPTZSpeed();

	if(0 != m_btnBrush.GetData())
	{
		ptzCtrl.cmdType = PTZ_CMD_SET_AUXILIARY;
	}
	else
	{
		ptzCtrl.cmdType = PTZ_CMD_CLEAR_AUXILIARY;
	}
	ptzCtrl.cmdValue = AUXILIARY_BRUSH;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	assert( NULL != msgData.pData );
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
	if( CMD_REPLY_PTZ_SEND_CMD_SUC == pPackCmd->cmdType )
	{
		RefalseBtnCaption();
	}

	CMessageMan::Instance()->ReleaseMsgBuff( msgData );
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZPresetCtrl::OnClickLamp()
{
	PTZ_CTRL ptzCtrl;
	memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.speed = CMessageMan::Instance()->CurrPTZSpeed();

	if(0 != m_btnLamp.GetData())
	{
		ptzCtrl.cmdType = PTZ_CMD_SET_AUXILIARY;
	}
	else
	{
		ptzCtrl.cmdType = PTZ_CMD_CLEAR_AUXILIARY;
	}
	ptzCtrl.cmdValue = AUXILIARY_LAMP;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	assert( NULL != msgData.pData );
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
	if( CMD_REPLY_PTZ_SEND_CMD_SUC == pPackCmd->cmdType )
	{
		RefalseBtnCaption();
	}

	CMessageMan::Instance()->ReleaseMsgBuff( msgData );

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZPresetCtrl::OnClickComboPreset()
{	
	assert(m_curPreset >= 0);

	//for (std::vector<PTZ_PRESET>::iterator iter = m_presetList.begin(); iter != m_presetList.end(); ++iter)
	//{
	//	if (iter->index == m_curPreset)
	//	{
	//		strcpy(iter->name,  m_editName.GetCaption().c_str());		
	//		break;
	//	}
	//}
	m_curPreset = m_comboIndex.GetCurItemData();

	//for (std::vector<PTZ_PRESET>::iterator iter = m_presetList.begin(); iter != m_presetList.end(); ++iter)
	//{
	//	if (iter->index == m_curPreset)
	//	{
	//		m_editName.SetCaption(iter->name, true);
	//		break;
	//	}
	//}

	//改变云台位置到预置点 m_curPreset
	GoToPreset();


	return KEY_VIRTUAL_MSG_STOP;
}	



void CPTZPresetCtrl::GoToPreset()
{
	assert( 0 <= m_curPreset );
	assert( 128 > m_curPreset );

	PTZ_CTRL ptzCtrl;
	memset( &ptzCtrl, 0, sizeof( PTZ_CTRL ) );

	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_PRESET_GO;
	ptzCtrl.speed =  CMessageMan::Instance()->CurrPTZSpeed();
	ptzCtrl.cmdValue = m_curPreset;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	CMessageMan::Instance()->ReleaseMsgBuff( msgData );
}

void CPTZPresetCtrl::RefalseBtnCaption(bool bRefresh/* = true*/)
{
	//获得通道PTZ的当前状态
	unsigned char channel = CMessageMan::Instance()->CurrPTZChnn();
	unsigned long PTZStatus = CPTZViewDlg::GetPTZStatus(channel);

	unsigned char protocolID = m_ptzConfig[channel].protocol;

#ifndef __DVR_ULTIMATE__
	m_btnLamp.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_AUXILIARY), bRefresh );
	m_btnBrush.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_AUXILIARY), bRefresh );
#endif

	if (PTZStatus & PTZ_ST_BRUSH_ON)
	{	
		m_btnBrush.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_BRUSH_ON, bRefresh);
		m_btnBrush.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_BRUSH_ON_F, bRefresh);
		m_btnBrush.SetData(0);
	}
	else
	{
		m_btnBrush.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_BRUSH_OFF, bRefresh);
		m_btnBrush.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_BRUSH_OFF_F, bRefresh);
		m_btnBrush.SetData(1);
	}

	if (PTZStatus & PTZ_ST_LAMP_ON)
	{		
		m_btnLamp.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_LAMP_ON, bRefresh);
		m_btnLamp.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_LAMP_ON_F, bRefresh);
		m_btnLamp.SetData(0);
	}
	else
	{
		m_btnLamp.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_LAMP_OFF, bRefresh);
		m_btnLamp.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_LAMP_OFF_F, bRefresh);
		m_btnLamp.SetData(1);
	}
}
