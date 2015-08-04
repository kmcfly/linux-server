/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-17
** Name         : PTZTrackCtrl.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "PTZTrackCtrl.h"
#include "PTZViewDlg.h"



extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

CPTZTrackCtrl::CPTZTrackCtrl()
{

}

CPTZTrackCtrl::~CPTZTrackCtrl()
{

}


void CPTZTrackCtrl::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////
	std::string str;
	int static_cx = 70;
	//int combo_cx = 120;
	int btn_cx = m_length[BTN_WIDTH];
	int cy = m_length[COMBO_HEIGHT] + 15;
	int offset_x = 40;

	int base_x = 10;
	int x = base_x;
	int y = m_length[STATIC_HEIGHT] + 5;

	x = 0;
	y += cy;
	m_btnBrush.Create(CTRLID_PTZCONTROL_BRUSH, x, y, 30, 25, this, 0, 1);
	x = 60;
	m_btnLamp.Create(CRTLID_PTZCONTROL_LAMP, x, y, 30, 25, this, 1, 1);

	x += ( 30 + 20 );
	y = 0;
	m_groupCtrl.Create(CTRLID_STATIC, x, 5, m_rect.Width() - x - 5, m_rect.Height() - 5, this, m_pStrTable->String(E_STR_ID_TRACK));

	m_channel = CMessageMan::Instance()->CurrPTZChnn();
	//停止云台的所有动作
	unsigned long chnn = static_cast< unsigned long >( m_channel );
	MESSAGE_DATA msgData;
	memset( &msgData, 0, sizeof( MESSAGE_DATA ) );
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_STOP_CURRENT_STATUS, reinterpret_cast<unsigned char *>( &chnn ), sizeof( unsigned long ), msgData );
	CMessageMan::Instance()->ReleaseMsgBuff( msgData );

	unsigned long PTZStatus = CPTZViewDlg::GetPTZStatus( m_channel );
	if( PTZ_ST_CFG_TRACK & PTZStatus )
	{
		str = m_pStrTable->String( E_STR_ID_STOP_RECORD_TRACK );
		m_btnRecord.SetData( false );
	}
	else
	{
		str = m_pStrTable->String( E_STR_ID_START_RECORD_TRACK );
		m_btnRecord.SetData( true );
	}
	y += m_length[STATIC_HEIGHT]  + 5;
	m_btnRecord.Create(GetRandWndID(), x  + 5 , y, m_rect.Width() - x - 20, m_length[BTN_HEIGHT], this, 2, 0, str.c_str());
	

	y += cy;
	if( PTZ_ST_TRACK & PTZStatus )
	{
		str = m_pStrTable->String(E_STR_ID_STOP_TRACK);
		m_btnTrack.SetData( false );
	}
	else
	{
		str = m_pStrTable->String(E_STR_ID_START_TRACK);
		m_btnTrack.SetData( true );
	}

	m_btnTrack.Create(GetRandWndID(), x + 5, y, m_rect.Width() - x - 20, m_length[BTN_HEIGHT], this, 2, 1, str.c_str());

	RefalseBtnCaption(false);
}

void CPTZTrackCtrl::OnPaint(PAINT_STATUS status)
{
}


void CPTZTrackCtrl::OnDestroy()
{
	//退出该对话框时停止所有通道的录制轨迹操作
	for( unsigned long i=0; i<g_p_login_succ->productInfo.localVideoInputNum; i++)
	{
		unsigned long PTZStatus = CPTZViewDlg::GetPTZStatus( i );

		if( PTZ_ST_CFG_TRACK & PTZStatus )					//正在记录轨迹
		{
			//停止记录轨迹
			PTZ_CTRL ptzCtrl;
			memset( &ptzCtrl, 0, sizeof( PTZ_CTRL ) );
			ptzCtrl.chnn	= i;
			ptzCtrl.cmdType = PTZ_CMD_TRACK_DEL;

			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

			CMessageMan::Instance()->ReleaseMsgBuff( msgData );
			PUB_Sleep(30);
			PTZStatus = CPTZViewDlg::GetPTZStatus(i);
			assert( 0 == ( PTZ_ST_CFG_TRACK & PTZStatus ) );
		}
	}


	CDialog::OnDestroy();

}



BEGIN_MSG_MAP(CPTZTrackCtrl, GUI::CDialog)
	ON_MSG(m_btnTrack.GetID(), KEY_VIRTUAL_ENTER, OnClickTrack)
	ON_MSG(m_btnRecord.GetID(), KEY_VIRTUAL_ENTER, OnClickRecord)

	ON_MSG(m_btnBrush.GetID(), KEY_VIRTUAL_ENTER, OnClickBrush)
	ON_MSG(m_btnLamp.GetID(), KEY_VIRTUAL_ENTER, OnClickLamp)
#ifndef __DVR_ULTIMATE__
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_BRUSH, OnClickBrush)
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_LAMP, OnClickLamp)
#endif
END_MSG_MAP()

unsigned long CPTZTrackCtrl::OnClickRecord()
{

	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));

	if (m_btnRecord.GetData())
	{
		//记录轨迹
		ptzCtrl.cmdType = PTZ_CMD_TRACK_SET;

		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
		
	} 
	else
	{
		//停止记录轨迹
		ptzCtrl.cmdType = PTZ_CMD_TRACK_DEL;


		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	}

	ReleaseMsgBuf(msgData);

	RefalseBtnCaption();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZTrackCtrl::OnClickTrack()
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	
	if (m_btnTrack.GetData())
	{
		//播放轨迹
		ptzCtrl.cmdType = PTZ_CMD_TRACK_START;

		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
	} 
	else
	{
		//停止播放轨迹
		ptzCtrl.cmdType = PTZ_CMD_TRACK_STOP;

		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	}

	ReleaseMsgBuf(msgData);

	RefalseBtnCaption();

	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CPTZTrackCtrl::OnClickBrush()
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

	ReleaseMsgBuf(msgData);

	RefalseBtnCaption();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZTrackCtrl::OnClickLamp()
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

	ReleaseMsgBuf(msgData);

	RefalseBtnCaption();

	return KEY_VIRTUAL_MSG_STOP;
}
//客户端向服务端发云台控制命令后服务端会回应一个消息，该函数用来释放服务端产生消息时申请的内存空间
//返回true表示云台执行命令成功
//返回false表示云台执行命令不成功
bool CPTZTrackCtrl::ReleaseMsgBuf(MESSAGE_DATA & msgData)
{
	assert( NULL != msgData.pData );
	PACKCMD *pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
	if( CMD_REPLY_PTZ_SEND_CMD_SUC == pPackCmd->cmdType )
	{
		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		return true;
	}
	else if( CMD_REPLY_PTZ_SEND_CMD_FAIL )
	{
		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		return false;
	}

}

/***************************************************************************************

函数名：RefalseBtnCaption()
功能：	刷新界面上的提示字符串
作者：	ZHL 2010-02-25

*****************************************************************************************/
void CPTZTrackCtrl::RefalseBtnCaption(bool refresh)
{
	//获得通道PTZ的当前状态
	unsigned long PTZStatus = CPTZViewDlg::GetPTZStatus( CMessageMan::Instance()->CurrPTZChnn() );
	std::string str;

	if( PTZ_ST_TRACK & PTZStatus )					//正在轨迹
	{
		//按键提示"停止轨迹"
		str = m_pStrTable->String(E_STR_ID_STOP_TRACK);
		m_btnTrack.SetCaption(str.c_str(), refresh);

		m_btnTrack.SetData( false );
	}
	else
	{
		//按键提示"开始轨迹"
		str = m_pStrTable->String(E_STR_ID_START_TRACK);
		m_btnTrack.SetCaption(str.c_str(), refresh);

		m_btnTrack.SetData( true );
	}

	if( PTZ_ST_CFG_TRACK & PTZStatus )			//正在配置轨迹
	{
		//按键提示"停止记录轨迹"
		str = m_pStrTable->String( E_STR_ID_STOP_RECORD_TRACK );

		m_btnRecord.SetCaption(str.c_str(), refresh);

		m_btnRecord.SetData( false );
	}
	else
	{
		//按键提示"开始记录轨迹"
		str = m_pStrTable->String( E_STR_ID_START_RECORD_TRACK );
		m_btnRecord.SetCaption(str.c_str(), refresh);

		m_btnRecord.SetData( true );
	}

	unsigned char protocolID = m_ptzConfig[m_channel].protocol;
#ifndef __DVR_ULTIMATE__
	m_btnLamp.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_AUXILIARY), refresh );
	m_btnBrush.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_AUXILIARY), refresh );
#endif
	if (PTZStatus & PTZ_ST_BRUSH_ON)
	{	
		m_btnBrush.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_BRUSH_ON, refresh);
		m_btnBrush.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_BRUSH_ON_F, refresh);
		m_btnBrush.SetData(0);
	}
	else
	{
		m_btnBrush.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_BRUSH_OFF, refresh);
		m_btnBrush.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_BRUSH_OFF_F, refresh);
		m_btnBrush.SetData(1);
	}

	if (PTZStatus & PTZ_ST_LAMP_ON)
	{		
		m_btnLamp.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_LAMP_ON, refresh);
		m_btnLamp.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_LAMP_ON_F, refresh);
		m_btnLamp.SetData(0);
	}
	else
	{
		m_btnLamp.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_LAMP_OFF, refresh);
		m_btnLamp.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_LAMP_OFF_F, refresh);
		m_btnLamp.SetData(1);
	}

}

void CPTZTrackCtrl::ChangeBtnCaption(bool bChnnChanged)
{
	if(bChnnChanged)
	{
		if(m_channel != CMessageMan::Instance()->CurrPTZChnn())
		{
			//切换了通道，要停止前一个通道的记录巡航状态
			unsigned long PTZStatus = CPTZViewDlg::GetPTZStatus( m_channel );

			if( PTZ_ST_CFG_TRACK & PTZStatus )					//正在记录轨迹
			{
				//停止记录轨迹
				PTZ_CTRL ptzCtrl;
				memset( &ptzCtrl, 0, sizeof( PTZ_CTRL ) );
				ptzCtrl.chnn	= m_channel;
				ptzCtrl.cmdType = PTZ_CMD_TRACK_DEL;
				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

				CMessageMan::Instance()->ReleaseMsgBuff( msgData );
				PUB_Sleep(70);
				PTZStatus = CPTZViewDlg::GetPTZStatus(m_channel);
				assert( 0 == ( PTZ_ST_CFG_TRACK & PTZStatus ) );
			}

			m_channel = CMessageMan::Instance()->CurrPTZChnn();
			unsigned long chnn = m_channel;
			//停止当前通道的所有状态
			MESSAGE_DATA msgData;
			memset( &msgData, 0, sizeof( MESSAGE_DATA ) );
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_STOP_CURRENT_STATUS, reinterpret_cast<unsigned char *>( &chnn ), sizeof( unsigned long ), msgData );
			CMessageMan::Instance()->ReleaseMsgBuff( msgData );

			PUB_Sleep(70);
		}
	}
	
	RefalseBtnCaption();
}



