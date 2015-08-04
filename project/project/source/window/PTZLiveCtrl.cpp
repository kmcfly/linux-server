/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-17
** Name         : PTZLiveCtrl.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "NetProtocol.h"
#include "PTZLiveCtrl.h"
#include "ConfigEx.h"
#include "MessageBox.h"
#include "PTZViewDlg.h"


extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
CPTZLiveCtrl::CPTZLiveCtrl() : m_viewChnn(0)
{

}

CPTZLiveCtrl::~CPTZLiveCtrl()
{

}


unsigned long CPTZLiveCtrl::ResponseHideCommond( unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	return OnCommand( ctrlID, keyID, xPos, yPos);
}

void CPTZLiveCtrl::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////
	char sz[8] = {0};
	std::string str;
	int btn_cx = 0;
	int btn_cy = 0;
	GetImageSize(BMP_GOTO, btn_cx, btn_cy);

	int static_cx = 70;
	int combo_cx = 118;
	int cy = m_length[COMBO_HEIGHT] + 8;
	int offset_x = 12;

	int base_x = 10;
	int x = base_x;
	int y = 10;
	//////////////////////////////////////////////////////////////////////////
	//设置初始控制通道
	if (0 != m_viewChnn)
	{
		int firstChnn = -1;
		int i = 0;
		for ( i = 0; i < g_p_login_succ->productInfo.videoInputNum; ++i)
		{
			if((m_viewChnn >> i) & 0x1)
			{
				if(firstChnn < 0)
				{
					firstChnn = i;
					break;
				}
			}
		}

		if( i == g_p_login_succ->productInfo.videoInputNum)
		{
			CMessageMan::Instance()->SetCurrPTZChnn(0xff);
		}
		else
		{
			CMessageMan::Instance()->SetCurrPTZChnn(firstChnn);
		}
		
		
	}
	else
	{
		CMessageMan::Instance()->SetCurrPTZChnn( 0xff);
	}


	//////////////////////////////////////////////////////////////////////////
	x = base_x + static_cx;
	m_btnBrush.Create(CTRLID_PTZCONTROL_BRUSH, x, y, 30, 25, this, 0, 1);
	x += (combo_cx + 1 + btn_cx - 60);
	m_btnLamp.Create(CRTLID_PTZCONTROL_LAMP, x, y, 30, 25, this, 1, 1);

	x = base_x;
	y += cy;
	m_scPreset.Create(CTRLID_STATIC, x, y, static_cx, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_PRESET));
	x += static_cx;
	m_comboPreset.Create(GetRandWndID(), x, y, combo_cx, m_length[COMBO_HEIGHT], this, 0, 2);
	//////////////////////////////////////////////////////////////////////////
	
	x += combo_cx + 1/*offset_x*/;
	m_btnPreset.Create(GetRandWndID(), x, y, btn_cx, btn_cy, this, 1, 2);
	m_btnPreset.SetImage(PAINT_STATUS_NORMAL, BMP_GOTO, false);
	m_btnPreset.SetImage(PAINT_STATUS_DISABLE, BMP_GOTO, false);
	m_btnPreset.SetImage(PAINT_STATUS_FOCUS, BMP_GOTO_FOCUS, false);

	////
	x = base_x;
	y += cy;
	m_scCruise.Create(CTRLID_STATIC, x, y, static_cx, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_CRUISE));
	x += static_cx;
	m_comboCruise.Create(GetRandWndID(), x, y, combo_cx, m_length[COMBO_HEIGHT], this,0, 3);
	//////////////////////////////////////////////////////////////////////////

	x += combo_cx + 1/*offset_x*/;
	m_btnCruise.Create(GetRandWndID(), x, y, btn_cx, btn_cy, this, 1, 3);
	m_btnCruise.SetImage(PAINT_STATUS_NORMAL, BMP_GOTO, false);
	m_btnCruise.SetImage(PAINT_STATUS_DISABLE, BMP_GOTO, false);
	m_btnCruise.SetImage(PAINT_STATUS_FOCUS, BMP_GOTO_FOCUS, false);

	//
	x += btn_cx + offset_x;
	y = 10;
	m_btnTrack.Create(GetRandWndID(), x, y, m_rect.Width() - x - 8, m_length[BTN_HEIGHT], this, 2, 1);

	y += cy;
	m_btnAutoScan.Create(GetRandWndID(), x, y, m_rect.Width() - x - 8, m_length[BTN_HEIGHT], this, 2, 2);

	y += cy;
	m_btnStartCruise.Create(GetRandWndID(),x,y,m_rect.Width() - x - 8, m_length[BTN_HEIGHT],this, 2, 3);


	//刷新当前通道的配置界面
	if( LoadPTZConfigInfo())
	{
		SetBtnFunction();
	}
	else
	{
		assert(false);
	}

	RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), false);
}

void CPTZLiveCtrl::OnDestroy()
{
	GUI::CDialog::OnDestroy();
}


void CPTZLiveCtrl::OnPaint(PAINT_STATUS status)
{

}

void CPTZLiveCtrl::SetChnn(ULONGLONG CH)
{
	m_viewChnn = CH;
}


void CPTZLiveCtrl::ChangeBtnCaption( bool bChnnChanged)
{
	if(bChnnChanged)
	{
		SetBtnFunction();
	}

	RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), true);
}

//加载PTZ的配置信息，包括每个通道的串口配置、预置点、巡航线
bool CPTZLiveCtrl::LoadPTZConfigInfo()
{
	BYTE *pData = NULL;
	unsigned long dataLen;
	unsigned short count;
	bool bRet = true;

	if(g_p_login_succ->productInfo.videoInputNum > 0)
	{
		CConfigBlock cfgBlock;

		CMessageMan* pMsgMan = CMessageMan::Instance();
		MESSAGE_DATA msgData;
		PACKCMD *pCmd = NULL;

		//添加要读取的信息
		dataLen = ( 1 + g_p_login_succ->productInfo.videoInputNum * 2 ) * sizeof( NCFG_ITEM_HEAD );		//要读取PTZ串口、每个通道的预置点、巡航线信息					
		pData	= new BYTE[dataLen];
		memset( pData, 0, dataLen );
		NCFG_ITEM_HEAD  *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pData);

		pItemHead->itemID = NCFG_ITEM_PTZ_SERIAL;
		pItemHead++;

		for ( int i = 0; i < g_p_login_succ->productInfo.videoInputNum; ++i, ++pItemHead )
		{
			pItemHead->itemID = NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + i;
		}

		for (int i = 0; i < g_p_login_succ->productInfo.videoInputNum; ++i, ++pItemHead )
		{
			pItemHead->itemID = NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + i;
		}


		//向服务端发送读取云台配置信息命令			
		memset( &msgData, 0, sizeof( MESSAGE_DATA ) );
		pMsgMan->SendMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_GET, pData, dataLen, msgData);
		delete [] pData;
		pData = NULL;

		assert( NULL != msgData.pData );
		pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		if (CMD_REPLY_CFG_DATA == static_cast<NET_PROTOCOL>(pCmd->cmdType))
		{
			cfgBlock.CreateBlock( reinterpret_cast<BYTE*>(msgData.pData) + sizeof(PACKCMD), pCmd->dataLen );

			//读取每个通道用的协议
			pData = NULL;
			dataLen = 0;
            

			cfgBlock.GetConfig( NCFG_ITEM_PTZ_SERIAL, &pData, dataLen );
			assert( NULL != pData );
			assert( dataLen == ( sizeof( PTZ_SERIAL_INFO ) * g_p_login_succ->productInfo.videoInputNum ) );

			count = GetDataCount( NCFG_ITEM_PTZ_SERIAL, cfgBlock );
			m_ptzConfig.clear();

			PTZ_SERIAL_INFO *pSerialInfo = reinterpret_cast<PTZ_SERIAL_INFO *>(pData);
			for( int i=0; i<count; ++i,++pSerialInfo )
			{
				m_ptzConfig.push_back( *pSerialInfo );
			}

			//读取每个通道拥有的预置点
			pData = NULL;
			dataLen = 0;

			for( std::vector< std::vector<PTZ_PRESET> >::iterator presetListIter=m_presetList.begin(); presetListIter!=m_presetList.end(); ++presetListIter )
			{
				presetListIter->clear();
			}
			m_presetList.clear();
			for( int i=0; i<g_p_login_succ->productInfo.videoInputNum; i++ )
			{
				std::vector<PTZ_PRESET> presetVec;
				cfgBlock.GetConfig( (NCFG_ITEM_ID)(NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + i ), &pData, dataLen );
				assert( NULL != pData );
				count = GetDataCount( (NCFG_ITEM_ID)(NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + i ), cfgBlock );

				PTZ_PRESET *pPreset = reinterpret_cast<PTZ_PRESET *>(pData);
				for( int i=0; i<count; ++i, ++pPreset )
				{
					presetVec.push_back( *pPreset );
				}
				m_presetList.push_back( presetVec );
				presetVec.clear();
			}

			//读取每个通道拥有的巡航线信息,只读取巡航线的头信息
			pData = NULL;
			dataLen = 0;
			for( std::vector< std::vector<NCFG_INFO_CRUISE_HEAD> >::iterator curiseListIter=m_curiseList.begin(); curiseListIter!=m_curiseList.end(); ++curiseListIter )
			{
				curiseListIter->clear();
			}
			m_curiseList.clear();
            
			for( int i = 0; i < g_p_login_succ->productInfo.videoInputNum; i++ )
			{
				std::vector<NCFG_INFO_CRUISE_HEAD> curiseVec;
				cfgBlock.GetConfig( (NCFG_ITEM_ID)(NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + i ), &pData, dataLen );
				assert( NULL != pData );
				count = GetDataCount( (NCFG_ITEM_ID)(NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + i ), cfgBlock );

				NCFG_INFO_CRUISE_HEAD *pCruiseHead = reinterpret_cast<NCFG_INFO_CRUISE_HEAD *>(pData);
				for( int i=0; i<count; ++i, ++pCruiseHead )
				{
					curiseVec.push_back( *pCruiseHead );
				}

				m_curiseList.push_back( curiseVec );
				curiseVec.clear();
			}

			pData = NULL;
			dataLen = 0;

			pMsgMan->ReleaseMsgBuff(msgData);
			bRet = true;
		}
		else
		{
			printf("%s():%s:%d, get config error\n", __FUNCTION__, __FILE__, __LINE__);
			pMsgMan->ReleaseMsgBuff(msgData);
			bRet = false;
		}
	}

#ifdef __DVR_ULTIMATE__
	/*for( int i=0; i<g_p_login_succ->productInfo.netVideoInputNum; i++ )
	{
		std::vector<PTZ_PRESET> presetVec;

		PTZ_PRESET preset = {0};
		preset.enable = 1;
		for( int j=0; j<256; ++j)
		{
			preset.index = j;
			sprintf(preset.name, "preset%03d", preset.index+1);
			presetVec.push_back(preset);
		}
		m_presetList.push_back( presetVec );
		presetVec.clear();
	}
	for( int i = 0; i < g_p_login_succ->productInfo.netVideoInputNum; i++ )
	{
		std::vector<NCFG_INFO_CRUISE_HEAD> curiseVec;

		NCFG_INFO_CRUISE_HEAD cruiseHead = {0};
		for( int j=0; j<5; ++j)
		{
			cruiseHead.index = j;
			sprintf(cruiseHead.name, "cruise %d", cruiseHead.index+1);
			curiseVec.push_back(cruiseHead);
		}

		m_curiseList.push_back( curiseVec );
		curiseVec.clear();
	}*/
#endif

	return bRet;
}

void CPTZLiveCtrl::SetBtnFunction(  )
{
	char sz[64] = {0};
	std::vector<PTZ_SERIAL_INFO>::size_type count = m_ptzConfig.size();
	//该容器中必须有所有通道的串口信息
	//assert( count == 8 );
	unsigned long chnn =  CMessageMan::Instance()->CurrPTZChnn();

	if (INVALID_INDEX(unsigned long) != chnn)
	{
		assert (chnn < g_p_login_succ->productInfo.videoInputNum);

		//刷新该通道的预置点信息

		std::vector<PTZ_PRESET> presetVec = m_presetList[chnn];
		m_comboPreset.RemoveAllItem( true );
		if( !presetVec.empty())
		{
			for(std::vector<PTZ_PRESET>::const_iterator presetIter = presetVec.begin(); presetIter != presetVec.end(); ++presetIter)
			{
				snprintf(sz, 64, "%s", presetIter->name);
				m_comboPreset.AddItem(sz, presetIter->index);
			}
			m_comboPreset.SetCurItem(0);
			m_comboPreset.Show( true );
		}

		//刷新该通道的巡航线信息
		std::vector<NCFG_INFO_CRUISE_HEAD> curiseVec = m_curiseList[chnn];
		m_comboCruise.RemoveAllItem( true );
		if( !curiseVec.empty() )
		{
			for( std::vector<NCFG_INFO_CRUISE_HEAD>::const_iterator curiseIter = curiseVec.begin(); curiseIter != curiseVec.end(); ++curiseIter )
			{
				snprintf( sz, 64, "%s", curiseIter->name );
				m_comboCruise.AddItem( sz,( curiseIter->index ) );
			}
			m_comboCruise.SetCurItem( 0 );
			m_comboCruise.Show( true );
		}

		if(chnn < g_p_login_succ->productInfo.localVideoInputNum)
		{
			unsigned char protocolID = m_ptzConfig[chnn].protocol;

			//重新设置按钮是否能被选中			
			m_btnPreset.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_PRESET ) );
			m_comboPreset.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_PRESET ) );

			m_btnCruise.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_CURISE ) );
			m_comboCruise.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_CURISE ) & (~(PTZSupportAction( protocolID, PROTOCOL_DISABLE_CONFIGURE_CRUISE))) );
			m_btnStartCruise.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_CURISE ) );

			m_btnTrack.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_TRACK ) );

			m_btnAutoScan.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_AUTOSCAN ) );

			m_btnLamp.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_AUXILIARY) );
			m_btnBrush.SetEnable( PTZSupportAction( protocolID, PROTOCOL_SUPPORT_AUXILIARY) );
		}
		else
		{
			//重新设置按钮是否能被选中
			m_btnPreset.SetEnable(true);
			m_comboPreset.SetEnable(true);

			m_btnCruise.SetEnable(true);
			m_comboCruise.SetEnable(true);
			m_btnStartCruise.SetEnable(true);
			m_btnTrack.SetEnable(false);

			m_btnTrack.SetEnable(false);

			m_btnAutoScan.SetEnable(false);
#ifndef __DVR_ULTIMATE__
			m_btnLamp.SetEnable(true);
			m_btnBrush.SetEnable(true);
#endif
		}
	}
}

//////////////////////////////获得每一项中元素的个数

unsigned short CPTZLiveCtrl::GetDataCount( NCFG_ITEM_ID itemID,  CConfigBlock & CfgBlock )
{
	std::list<NCFG_ITEM_HEAD_EX> ItemHeadExList;
	ItemHeadExList = CfgBlock.GetConfigItemList();

	for( std::list<NCFG_ITEM_HEAD_EX>::iterator iter=ItemHeadExList.begin(); iter!=ItemHeadExList.end(); ++iter)
	{
		if( itemID == iter->item_head.itemID )
		{
			return iter->item_head.num;
		}
	}
	return 0;
}


BEGIN_MSG_MAP(CPTZLiveCtrl, GUI::CDialog)
	ON_MSG(m_btnTrack.GetID(), KEY_VIRTUAL_ENTER, OnClickTrack)
	ON_MSG(m_btnAutoScan.GetID(), KEY_VIRTUAL_ENTER, OnClickScan)
	//ON_MSG(m_comboChnn.GetID(), KEY_VIRTUAL_ENTER, OnClickChnn)
	ON_MSG(m_btnPreset.GetID(), KEY_VIRTUAL_ENTER, OnClickPresetGo)
	ON_MSG(m_btnCruise.GetID(), KEY_VIRTUAL_ENTER, OnClickCruise)
	ON_MSG(m_btnStartCruise.GetID(), KEY_VIRTUAL_ENTER, OnClickCruiseRun)
	ON_MSG(m_btnBrush.GetID(), KEY_VIRTUAL_ENTER, OnClickBrush)
	ON_MSG(m_btnLamp.GetID(), KEY_VIRTUAL_ENTER, OnClickLamp)

	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_TRACK, OnClickTrack)
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_AUTO,  OnClickScan)
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_PRESET,OnPresetGo);
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_CRUISE, OnClickCruiseRun);
#ifndef __DVR_ULTIMATE__
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_BRUSH, OnClickBrush)
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_LAMP, OnClickLamp)
#endif
	
	//ON_MSG_KEY_RANGE( GUI::CTRLID_NULL, KEY_VIRTUAL_1, KEY_VIRTUAL_16, OnChangeChnn )

END_MSG_MAP()

unsigned long CPTZLiveCtrl::OnClickTrack()
{
	PTZ_CTRL ptzCtrl;
	memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();

	if(0 != m_btnTrack.GetData())
	{
		ptzCtrl.cmdType = PTZ_CMD_TRACK_START;
	}
	else
	{
		ptzCtrl.cmdType = PTZ_CMD_TRACK_STOP;
	}

	if(SendPtzCtrl(ptzCtrl))
	{
		if(ptzCtrl.chnn < g_p_login_succ->productInfo.localVideoInputNum)
		{
			RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), true);
		}
		else
		{
			RefreshBtnCaption(PTZ_ST_TRACK, true);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZLiveCtrl::OnClickScan()
{
	PTZ_CTRL ptzCtrl;
	memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.speed = CMessageMan::Instance()->CurrPTZSpeed();

	if(0 != m_btnAutoScan.GetData())
	{
		ptzCtrl.cmdType = PTZ_CMD_AUTO_SCAN_START;
	}
	else
	{
		ptzCtrl.cmdType = PTZ_CMD_AUTO_SCAN_STOP;
	}

	if(SendPtzCtrl(ptzCtrl))
	{
		if(ptzCtrl.chnn < g_p_login_succ->productInfo.localVideoInputNum)
		{
			RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), true);
		}
		else
		{
			RefreshBtnCaption(PTZ_ST_AUTOSCAN, true);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZLiveCtrl::OnClickChnn()
{
	SetBtnFunction();

	//CMessageMan::Instance()->SetCurrPTZChnn( CMessageMan::Instance()->CurrPTZChnn());

	RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), false);

	GetRootWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CPTZLiveCtrl::OnChangeChnn( unsigned long keyID )
{
	unsigned long keyIndex = keyID - KEY_VIRTUAL_1;

	if((m_viewChnn >> keyIndex) & 0x1)
	{
		//m_comboChnn.SetCurItemData(keyIndex, true );

		OnClickChnn();
	}
	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CPTZLiveCtrl::OnClickPresetGo()
{
	PTZ_CTRL ptzCtrl;
	memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
	std::vector<PTZ_PRESET> presetVec;
	std::vector<PTZ_PRESET>::iterator iter;

	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_PRESET_GO;

	//所在通道没有预置点
	if( m_presetList[ptzCtrl.chnn].empty() )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//鼠标、前面板、遥控操作
	ptzCtrl.cmdValue = m_comboPreset.GetCurItemData();

	ptzCtrl.speed = CMessageMan::Instance()->CurrPTZSpeed();

	SendPtzCtrl(ptzCtrl);

	RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), true);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZLiveCtrl::OnPresetGo()
{
	//NVR支持预置点
// 	if(0 == g_p_login_succ->productInfo.localVideoInputNum)
// 	{//NVR不支持预置点
// 		return KEY_VIRTUAL_MSG_STOP;
// 	}

	PTZ_CTRL ptzCtrl;
	memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
	std::vector<PTZ_PRESET> presetVec;
	std::vector<PTZ_PRESET>::iterator iter;
	unsigned long param;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_PRESET_GO;

	//所在通道没有预置点
	if( m_presetList[ptzCtrl.chnn].empty() )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if( (( m_CommandPram >> 8 ) == KEY_TYPE_MY1003) || (( m_CommandPram >> 8 ) == KEY_TYPE_TWSY))
	{
		//485键盘调用预置点需要检测预置点的合法性
		param = m_CommandPram & 0xff;
		for( iter=m_presetList[ptzCtrl.chnn].begin(); iter!=m_presetList[ptzCtrl.chnn].end(); ++iter )
		{
			if( (param - 1) == iter->index )
			{
				break;
			}
		}

		if( iter != m_presetList[ptzCtrl.chnn].end() )
		{
			ptzCtrl.cmdValue = ( m_CommandPram & 0xff ) - 1;

			m_comboPreset.SetCurItem( ptzCtrl.cmdValue );
			m_comboPreset.Show( true );
		}
		else
		{
			//要调用的预置点不存在直接返回
			char str[128] = {0};
			snprintf( str, 128, "%s %d!", m_pStrTable->String(E_STR_ID_MESSAGE_48), m_presetList[ptzCtrl.chnn].size() );
			GUI::CDialog::MessageBox( str,m_pStrTable->String(E_STR_ID_TITLE_ERROR),DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);
			return KEY_VIRTUAL_MSG_STOP;
		}


		ptzCtrl.speed = CMessageMan::Instance()->CurrPTZSpeed();

		SendPtzCtrl(ptzCtrl);

		RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), true);
	}


	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CPTZLiveCtrl::OnClickCruise()
{
	BYTE chnn	 = CMessageMan::Instance()->CurrPTZChnn();

	if(chnn < g_p_login_succ->productInfo.localVideoInputNum)
	{
		if (0 >= m_ptzConfig.size())    //xian
		{
			return KEY_VIRTUAL_MSG_STOP;
		}
		unsigned char protocolID = m_ptzConfig[chnn].protocol;

// 		if(protocolID == 13)			//协议为PL14时该按钮为弹出云台自带菜单
// 		{
// 			PTZ_CTRL ptzCtrl;
// 			memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
// 			ptzCtrl.chnn =  chnn;
// 			ptzCtrl.cmdType = PTZ_CMD_POP_PTZ_MENU;
// 			ptzCtrl.cmdValue = 0;
// 
// 			SendPtzCtrl(ptzCtrl);
// 		}
//		else 
		if( !m_curiseList[chnn].empty() )
		{
			PTZ_CTRL ptzCtrl;
			memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
			ptzCtrl.chnn =  chnn;
			ptzCtrl.cmdType = PTZ_CMD_CRUISE_RUN;
			ptzCtrl.cmdValue = m_comboCruise.GetCurItemData();

			if(SendPtzCtrl(ptzCtrl))
			{
				RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), true);
			}
		}
		else if(PTZSupportAction(protocolID, PROTOCOL_DISABLE_CONFIGURE_CRUISE))
		{
			PTZ_CTRL ptzCtrl;
			memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
			ptzCtrl.chnn =  chnn;
			ptzCtrl.cmdType = PTZ_CMD_CRUISE_RUN;
			ptzCtrl.cmdValue = 1;

			if(SendPtzCtrl(ptzCtrl))
			{
				RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), true);
			}
		}
		else
		{
			MessageOut( GetTopestWnd(), E_STR_ID_MESSAGE_108, TIP_MSG );
		}
	}
	else
	{
		PTZ_CTRL ptzCtrl;
		memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
		ptzCtrl.chnn =  chnn;
		ptzCtrl.cmdType = PTZ_CMD_CRUISE_RUN;
		ptzCtrl.cmdValue = m_comboCruise.GetCurItemData();

		if(SendPtzCtrl(ptzCtrl))
		{
			RefreshBtnCaption(PTZ_ST_CRUISE, true);
		}
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZLiveCtrl::OnClickCruiseRun()
{
	BYTE chnn	 = CMessageMan::Instance()->CurrPTZChnn();

	if(chnn < g_p_login_succ->productInfo.localVideoInputNum)
	{
		unsigned char protocolID = m_ptzConfig[chnn].protocol;

		if( !m_curiseList[chnn].empty() )
		{
			PTZ_CTRL ptzCtrl;
			memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
			ptzCtrl.chnn =  chnn;
			ptzCtrl.cmdValue = m_comboCruise.GetCurItemData();

			if(0 != m_btnStartCruise.GetData())
			{
				ptzCtrl.cmdType = PTZ_CMD_CRUISE_RUN;
			}
			else
			{
				ptzCtrl.cmdType = PTZ_CMD_CRUISE_STOP;
			}

			if(SendPtzCtrl(ptzCtrl))
			{
				RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), true);
			}
		}
		else if(PTZSupportAction(protocolID, PROTOCOL_DISABLE_CONFIGURE_CRUISE))
		{
			PTZ_CTRL ptzCtrl;
			memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
			ptzCtrl.chnn =  chnn;
			ptzCtrl.cmdValue = 1;

			if(0 != m_btnStartCruise.GetData())
			{
				ptzCtrl.cmdType = PTZ_CMD_CRUISE_RUN;
			}
			else
			{
				ptzCtrl.cmdType = PTZ_CMD_CRUISE_STOP;
			}

			if(SendPtzCtrl(ptzCtrl))
			{
				RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), true);
			}
		}
		else
		{
			MessageOut( GetTopestWnd(), E_STR_ID_MESSAGE_108, TIP_MSG );
		}
	}
	else
	{
		PTZ_CTRL ptzCtrl;
		memset(&ptzCtrl, 0, sizeof(PTZ_CTRL));
		ptzCtrl.chnn =  chnn;
		ptzCtrl.cmdValue = m_comboCruise.GetCurItemData();

		unsigned long ptzState = PTZ_ST_NULL;
		if(0 != m_btnStartCruise.GetData())
		{
			ptzCtrl.cmdType = PTZ_CMD_CRUISE_RUN;
			ptzState = PTZ_ST_CRUISE;
		}
		else
		{
			ptzCtrl.cmdType = PTZ_CMD_CRUISE_STOP;
		}

		if(SendPtzCtrl(ptzCtrl))
		{
			RefreshBtnCaption(ptzState, true);
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZLiveCtrl::OnClickBrush()
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

	if(SendPtzCtrl(ptzCtrl))
	{
		RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZLiveCtrl::OnClickLamp()
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

	if(SendPtzCtrl(ptzCtrl))
	{
		RefreshBtnCaption(GetPtzState(CMessageMan::Instance()->CurrPTZChnn()), true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

bool CPTZLiveCtrl::SendPtzCtrl(PTZ_CTRL& ptzCtrl)
{
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
	PACKCMD * pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
	
	if( CMD_REPLY_PTZ_SEND_CMD_SUC == pPackCmd->cmdType )
	{
		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		return true;
	}
	else if( CMD_REPLY_PTZ_SEND_CMD_FAIL )
	{
		unsigned long ptzValue = *reinterpret_cast< unsigned long * >( msgData.pData + sizeof( PACKCMD ) );
		ENUM_STRING_ID strID = CPTZViewDlg::GetPtzCtrlStrID( ptzValue );
		MessageOut( GetTopestWnd(), strID, TIP_MSG );

		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		return false;
	}
}

void CPTZLiveCtrl::RefreshBtnCaption(unsigned long ptzState, bool bRefresh)
{

	if(ptzState & PTZ_ST_TRACK)
	{
		std::string str = m_pStrTable->String(E_STR_ID_STOP_TRACK);
		m_btnTrack.SetCaption(str, bRefresh);
		m_btnTrack.SetData(0);
	}
	else
	{
		std::string str = m_pStrTable->String(E_STR_ID_START_TRACK);
		m_btnTrack.SetCaption(str, bRefresh);
		m_btnTrack.SetData(1);
	}

	if(ptzState & PTZ_ST_AUTOSCAN)
	{
		std::string str = m_pStrTable->String(E_STR_ID_STOP_AUTOSCAN);
		m_btnAutoScan.SetCaption(str, bRefresh);
		m_btnAutoScan.SetData(0);
	}
	else
	{
		std::string str = m_pStrTable->String(E_STR_ID_START_AUTOSCAN);
		m_btnAutoScan.SetCaption(str, bRefresh);
		m_btnAutoScan.SetData(1);
	}

	if(ptzState & PTZ_ST_CRUISE)
	{
		std::string str = m_pStrTable->String(E_STR_ID_STOP_CRUISE);
		m_btnStartCruise.SetCaption(str, bRefresh);
		m_btnStartCruise.SetData(0);
	}
	else
	{
		std::string str = m_pStrTable->String(E_STR_ID_START_CRUISE);
		m_btnStartCruise.SetCaption(str, bRefresh);
		m_btnStartCruise.SetData(1);
	}

	if (ptzState & PTZ_ST_BRUSH_ON)
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

	if (ptzState & PTZ_ST_LAMP_ON)
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

unsigned long CPTZLiveCtrl::GetPtzState(unsigned long chnn)
{

	if (INVALID_INDEX(unsigned long) == chnn)
	{
		return PTZ_ST_NULL;
	}

	unsigned long state = 0;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_STATUS, (unsigned char*)(&chnn), sizeof(unsigned long), msgData);
	PACKCMD * pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);

	if( CMD_REPLY_PTZ_STATUS_SUC == pPackCmd->cmdType )
	{
		state = *reinterpret_cast<unsigned long*>(msgData.pData + sizeof(PACKCMD));
	}
	else
	{
		assert(false);
	}
	CMessageMan::Instance()->ReleaseMsgBuff( msgData );

	return state;
}

//end

