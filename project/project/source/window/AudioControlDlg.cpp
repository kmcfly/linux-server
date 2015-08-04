/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-18
** Name         : AudioControlDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "AudioControlDlg.h"
#include "NetProtocol.h"
#include "MessageMan.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

unsigned short CAudioControlDlg::DLG_LEFT_X = 8;
unsigned short CAudioControlDlg::DLG_RIGHT_X = 8;
unsigned short CAudioControlDlg::DLG_TOP_Y = 8;
unsigned short CAudioControlDlg::DLG_BOTTOM_Y = 8;
unsigned short CAudioControlDlg::DLG_OFF_X = 10;
unsigned short CAudioControlDlg::DLG_OFF_Y = 10;
unsigned short CAudioControlDlg::DLG_TOOLBAR_HEIGHT = 40;

bool CAudioControlDlg::m_bAutoDefa = true;
const unsigned char COMBO_CHNN_NULL = 255;

CAudioControlDlg::CAudioControlDlg() : m_bMute(true), m_volume(60)
{

}

CAudioControlDlg::~CAudioControlDlg()
{

}

unsigned short CAudioControlDlg::GetX( unsigned long xPos )
{
	unsigned short x = xPos - GetWidth() / 2; 	// 居中
	assert( x > 0 );

	if( m_width > x + GetWidth() )
	{
		return x;	//优先满足居中
	}

	if( m_width >= xPos + GetWidth() )
	{
		x = xPos;				//满足左对齐时
	}
	else
	{
		if( xPos - GetWidth() >= 0 )
		{
			x = xPos - GetWidth();//不满足左对齐，匹配右对齐
		}
	}

	return x;
}

unsigned short CAudioControlDlg::GetY( unsigned long yPos )
{
	return yPos - GetHeight();
	//return m_height - DLG_TOOLBAR_HEIGHT - GetHeight();
}

unsigned short CAudioControlDlg::GetWidth()
{
	// OnInitial()中初始化各个控件宽度
	return 70 + 120 + m_length[CHECK_WIDTH] + 40 + 4 * DLG_OFF_X + DLG_LEFT_X + DLG_RIGHT_X ;
}

unsigned short CAudioControlDlg::GetHeight()
{
	return DLG_TOP_Y + 2 * m_length[COMBO_HEIGHT] + DLG_OFF_Y + DLG_BOTTOM_Y + m_length[BTN_HEIGHT] + 8 ;
}

void CAudioControlDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////

	unsigned short xLeft = DLG_LEFT_X, yTop = DLG_TOP_Y;
	unsigned short xOff = DLG_OFF_X, yOff = DLG_OFF_Y;

	unsigned short w1 = 70, w2 = 120, w3 = m_length[CHECK_WIDTH], w4 = 60;
	unsigned short height = m_length[COMBO_HEIGHT];

	unsigned short x = xLeft, y = yTop;

	unsigned short cx = w1, cy = height;

	char str[128];
	snprintf(str, 128, "%s", m_pStrTable->String(E_STR_ID_VOLUME) );
	m_scVolume.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, str );

	x += cx + xOff;
	cx = w2;
	m_sliderVolume.Create( GUI::CWnd::GetRandWndID(), x, y + 4, cx, this, 0, 1 );
	m_sliderVolume.SetCurPos(m_volume, false);

	x += cx + xOff + xOff / 2;
	cx = w3 + w4;
	m_ckMute.Create( GUI::CWnd::GetRandWndID(), x, y + 2, cx, m_length[CHECK_HEIGHT], this, 1, 1 );
	m_ckMute.SetCaption( m_pStrTable->String(E_STR_ID_MUTE), false, false );

	//x += cx + xOff;
	//cx = w4;
	//strcpy( str, m_pStrTable->String(E_STR_ID_MUTE) );
	//m_scMute.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, str );
	

	////

	y += cy + yOff;
	x = xLeft;
	cx = w1;
	snprintf(str, 128, "%s", m_pStrTable->String(E_STR_ID_CHANNEL) );
	m_scChnn.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, str );

	x += cx + xOff;
	cx = w2;
	m_comboChnn.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 2 );

	int auChnnNum = g_p_login_succ->productInfo.audioInputNum;
	for ( int i = 0; i< auChnnNum; i++ )
	{
		sprintf( str, "%d", i + 1 );
		m_comboChnn.AddItem(str, i);
	}
	
	m_netDevNum = 0;
	m_pNetDevInfo = NULL;

	if (0 < g_p_login_succ->productInfo.netVideoInputNum)
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_NET_DEVICE_INFO, NULL, 0, msgData);
		m_netDevNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(ND_DEV_INFO);
		if (m_netDevNum > 0)
		{
			m_pNetDevInfo = new ND_DEV_INFO[m_netDevNum];
			memcpy(m_pNetDevInfo, (ND_DEV_INFO *)(msgData.pData + sizeof(PACKCMD)), m_netDevNum*sizeof(ND_DEV_INFO));	
		}
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		
		for (int i = 0; i < m_netDevNum; i++)
		{
			sprintf(str, "%d", (m_pNetDevInfo[i].chnn + 1));
			m_comboChnn.AddItem(str, m_pNetDevInfo[i].chnn);
		}

		if ((0 == auChnnNum) && (0 == m_netDevNum))
		{
			m_comboChnn.AddItem(m_pStrTable->String(E_STR_ID_NONE), COMBO_CHNN_NULL);
		}
	}

	m_comboChnn.SetCurItem( 0, false );

	x += cx + xOff + xOff / 2;
	cx = w3 + w4;
	m_ckAuto.Create( GUI::CWnd::GetRandWndID(),x,y,cx,cy,this,1,2);
	m_ckAuto.SetCaption( m_pStrTable->String(E_STR_ID_AUTO), false, false );
	m_ckAuto.SetCheck(m_bAutoDefa, false);

	y = m_rect.Height() - 30;
	cx = 30;
	x = m_rect.Width() - cx - 8;

	m_btnExit.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 2, 3);
	m_btnExit.SetImage( PAINT_STATUS_NORMAL, BMP_BTN_EXIT2, false );
	m_btnExit.SetImage( PAINT_STATUS_DISABLE, BMP_BTN_EXIT2, false );
	m_btnExit.SetImage( PAINT_STATUS_FOCUS, BMP_BTN_EXIT2_FOCUS, false );

	x = xLeft;
	y += 4;
	CreateTipWnd(m_rect.Width()-cx-2*xLeft,x,y);

	m_bModify = false;
	if (LoadAudioCfg() < 0)
	{
		//如果读取失败，就默认为开启，0通道的声音。
		m_comboChnn.SetCurItem( 0, false );
		m_ckMute.SetCheck(false);
		m_bMute = false;
	}
#ifdef __DVR_ULTIMATE__
	OnClickComboForChangeChnn();
#else
	OnCkForMute();
#endif
}

void CAudioControlDlg::OnDestroy()
{
	if (NULL != m_pNetDevInfo)
	{
		delete [] m_pNetDevInfo;
		m_pNetDevInfo = NULL;
		m_netDevNum = 0;
	}
	NET_LIVEAUDIO_CTRL_INFO audioCtrlInfo;
	audioCtrlInfo.type = NET_LIVE_AUDIO_AUTO;
	if (m_ckAuto.IsCheck())
	{
		audioCtrlInfo.audiochnnel = 1; 
	}
	else
	{
		audioCtrlInfo.audiochnnel = 0; 
	}	
	CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CONTROL_LIVE_AUDIO, (unsigned char*)(&audioCtrlInfo), sizeof(audioCtrlInfo));	

	CDialog::OnDestroy();
}

void CAudioControlDlg::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	if( GetShowTipType(TIP_MSG) )
	{
		if( m_sliderVolume.GetID() == newID )
		{
			unsigned long pos = m_sliderVolume.GetPos();

			std::string volume = m_pStrTable->String(E_STR_ID_VOLUME);
			char strPos[32] = {0};
			sprintf(strPos," : %d",pos);
			volume += strPos;
			MessageOut(volume.c_str(),TIP_MSG);
			return;
		}
	}

	GUI::CDialog::OnChangeFocus(oldID,newID);
}

int CAudioControlDlg::LoadAudioCfg()
{
	std::vector<NCFG_ITEM_ID> cfgItemList;
	GetItemList(cfgItemList);
	int count = cfgItemList.size();
	if (0 == count)
	{
		return -1;
	}
	int len = sizeof(NCFG_ITEM_HEAD) * count;
	BYTE* pData = new BYTE[len];
	NCFG_ITEM_HEAD  *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pData);
	for (std::vector<NCFG_ITEM_ID>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
	{
		pItemHead->itemID = (*iter);
		pItemHead++;
	}

	//进入配置
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_ENTER, NULL, 0, msgData);
	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		delete [] pData;
		pData = NULL;
		return -1;
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

    //获取信息
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_GET, pData, len, msgData);
	delete [] pData;
	pData = NULL;

	assert(NULL != msgData.pData);
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_DATA != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		return -1;
	}

	CConfigBlock CfgBlock;
	CfgBlock.CreateBlock( reinterpret_cast<BYTE*>(msgData.pData) + sizeof(PACKCMD), pCmd->dataLen);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	//显示信息
	for (std::vector<NCFG_ITEM_ID>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
	{
		unsigned long dataLen = 0;
		BYTE *pData = NULL;
		CfgBlock.GetConfig((NCFG_ITEM_ID)(*iter), &pData, dataLen);
		ShowData((NCFG_ITEM_ID)(*iter), pData, dataLen, false);
	}
	CfgBlock.DestroyBlock();
	
	//退出配置
	CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);

	return 0;	
}

int CAudioControlDlg::SaveAudioCfg()
{
	std::vector<NCFG_ITEM_ID> cfgItemList;
	BYTE *pData = NULL;
	unsigned long dataLen = 0;
	NCFG_ITEM_HEAD *pItemInfo = NULL;
    int index = 0;
	BYTE *pWritePos = NULL;
	unsigned long itemDataLen = 0;
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
    
	//分配空间
	GetItemList(cfgItemList);
	if (0 == cfgItemList.size())
	{
		return -1;
	}
	dataLen = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * cfgItemList.size() + sizeof(unsigned long) * cfgItemList.size();
	pData = new BYTE[dataLen];
    memset(pData, 0, dataLen);

    //组装头信息
	NCFG_BLOCK_HEAD *pblockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(pData);
	pblockHead->netcfgver = NCFG_VERSION;
	pblockHead->biSize = sizeof(NCFG_BLOCK_HEAD);
	pblockHead->ItemNum = cfgItemList.size();

	//获取每一个项的信息
	pItemInfo = reinterpret_cast<NCFG_ITEM_HEAD *>(pData + sizeof(NCFG_BLOCK_HEAD));
    index = 0;
	for (std::vector<NCFG_ITEM_ID>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
	{
		pItemInfo[index].itemID = (*iter);
		pItemInfo[index].num = 1;
		pItemInfo[index].len = sizeof(unsigned long);
		pItemInfo[index].subLen = sizeof(unsigned long);
		pWritePos = pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * cfgItemList.size() + index * sizeof(unsigned long);
		itemDataLen = sizeof(unsigned long);
		SetData((NCFG_ITEM_ID)(*iter), pWritePos, itemDataLen);	
		index++;
	}

	//进入配置
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_ENTER, NULL, 0, msgData);
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		delete [] pData;
		pData = NULL;
		return -1;
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	
	//保存信息
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_SET, pData, dataLen, msgData);
	delete [] pData;
	pData = NULL;
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{	
		
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	//退出配置
	CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);

	return 0;	
}

void CAudioControlDlg::GetItemList(std::vector<NCFG_ITEM_ID>& itemList) const
{
	itemList.push_back(NCFG_ITEM_LIVE_AUDIO_ENABLE);
	itemList.push_back(NCFG_ITEM_LIVE_AUDIO_CHNN);
	itemList.push_back(NCFG_ITEM_LIVE_AUDIO_VOLUME);
}

bool CAudioControlDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
	case NCFG_ITEM_LIVE_AUDIO_ENABLE:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pType = reinterpret_cast<const unsigned long*>(pData);

			if (NET_LIVE_AUDIO_OPEN == *pType)
			{
				m_ckMute.SetCheck(false);
				m_bMute = false;
			}
			else
			{
				m_ckMute.SetCheck(true);
				m_bMute = true;
			}			
		}
		return true;
		break;
	case NCFG_ITEM_LIVE_AUDIO_CHNN:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pChannel = reinterpret_cast<const unsigned long*>(pData);
			if (*pChannel < g_p_login_succ->productInfo.audioInputNum)
			{
				m_comboChnn.SetCurItemData(*pChannel, false);
			}
			else if(*pChannel < g_p_login_succ->productInfo.localVideoInputNum)
			{
				m_comboChnn.SetCurItemData(0, false);
			}
			else if(*pChannel < g_p_login_succ->productInfo.videoInputNum)
			{
				bool bIPChnnInConfig = false;
				for (int  i = 0; i < m_netDevNum; i++)
				{
					if (*pChannel == m_pNetDevInfo[i].chnn)
					{
						bIPChnnInConfig = true;
						break;
					}
				}

				if (bIPChnnInConfig)
				{
					m_comboChnn.SetCurItemData(*pChannel, false);
				}
				else
				{
					m_comboChnn.SetCurItem(0);
				}
			}
			else
			{
				m_comboChnn.SetCurItemData(0, false);
			}
		}
		return true;
		break;
	case NCFG_ITEM_LIVE_AUDIO_VOLUME:
		{
			assert(len == sizeof(unsigned long));
			m_volume = *reinterpret_cast<const unsigned long*>(pData);
			if (m_volume > 100)
			{
				m_volume = 100;
			}
			m_sliderVolume.SetCurPos(m_volume, false);
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	
	return true;
		 
}
bool CAudioControlDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, unsigned long& len)  const
{

	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_LIVE_AUDIO_ENABLE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			if (m_ckMute.IsCheck())
			{
				*pValue = NET_LIVE_AUDIO_MUTE;
			}
			else
			{
				*pValue = NET_LIVE_AUDIO_OPEN;
			}			
		}
		return true;
		break;
	case NCFG_ITEM_LIVE_AUDIO_CHNN:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			unsigned long chnn = (unsigned long)m_comboChnn.GetCurItemData();
			assert ((chnn < g_p_login_succ->productInfo.audioInputNum) || \
				((chnn >= g_p_login_succ->productInfo.localVideoInputNum) && (chnn < g_p_login_succ->productInfo.videoInputNum)) || \
				(chnn == COMBO_CHNN_NULL));
			*pValue = chnn;
		}
		return true;
		break;
	case NCFG_ITEM_LIVE_AUDIO_VOLUME:
		{
			assert(len == sizeof(unsigned long));
			unsigned long *pVolume = reinterpret_cast<unsigned long*>(pData);
			*pVolume = m_volume;
			if (*pVolume > 100)
			{
				*pVolume = 100;
			}
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return true;
	
}

void CAudioControlDlg::OnOutOfDlg(unsigned long keyID)
{
	GUI::CDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		EndDialog(DLG_ESC);
	}
}

void CAudioControlDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint( status );

	unsigned short left = m_rect.m_left ;
	unsigned short right = m_rect.m_left + m_rect.Width() - 3;
	unsigned short top = m_rect.m_top;
	unsigned short bottom = m_rect.m_top + m_rect.Height();

	GUI::CWnd::DrawLine(left, bottom-32, right, bottom-32 , COLOR_LINE_GRAY, 1);
}


BEGIN_MSG_MAP(CAudioControlDlg, GUI::CDialog)
	ON_MSG(m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit )
	ON_MSG(m_comboChnn.GetID(), KEY_VIRTUAL_ENTER, OnClickComboForChangeChnn)
	ON_MSG(m_ckMute.GetID(), KEY_VIRTUAL_ENTER, OnCkForMute)
	ON_MSG(m_ckAuto.GetID(), KEY_VIRTUAL_ENTER, OnCkForAuto)
	ON_MSG_EX(m_sliderVolume.GetID(),KEY_VIRTUAL_ENTER,OnSlider)
	ON_MSG_EX(m_sliderVolume.GetID(),KEY_VIRTUAL_DRAG,OnSlider)
	ON_MSG_EX(m_sliderVolume.GetID(),KEY_VIRTUAL_BLUP,OnSlider)
END_MSG_MAP()

unsigned long CAudioControlDlg::OnBtnExit()
{
	return OnCancel();
}

unsigned long CAudioControlDlg::OnClickComboForChangeChnn()
{
	m_bModify = true;
	if (m_ckMute.IsCheck())
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	unsigned char chnn = m_comboChnn.GetCurItemData();
	if (COMBO_CHNN_NULL == chnn)
	{
			return KEY_VIRTUAL_MSG_STOP;
	}
	assert ((chnn < g_p_login_succ->productInfo.audioInputNum) || \
		((chnn >= g_p_login_succ->productInfo.localVideoInputNum) && (chnn < g_p_login_succ->productInfo.videoInputNum)));
	NET_LIVEAUDIO_CTRL_INFO audioCtrlInfo;
	audioCtrlInfo.type = NET_LIVE_AUDIO_OPEN;
	audioCtrlInfo.audiochnnel = chnn;

	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CONTROL_LIVE_AUDIO, \
		(unsigned char*)(&audioCtrlInfo), sizeof(audioCtrlInfo));
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CAudioControlDlg::OnCkForMute()
{
	m_bModify = true;
	NET_LIVEAUDIO_CTRL_INFO audioCtrlInfo;
	unsigned char chnn = m_comboChnn.GetCurItemData();
	assert ((chnn < g_p_login_succ->productInfo.audioInputNum) || \
		((chnn >= g_p_login_succ->productInfo.localVideoInputNum) && (chnn < g_p_login_succ->productInfo.videoInputNum)));	

	if (m_ckMute.IsCheck())
	{
		audioCtrlInfo.type = NET_LIVE_AUDIO_MUTE;
		audioCtrlInfo.audiochnnel = chnn;
		m_bMute = true;
	}
	else
	{			
		audioCtrlInfo.type = NET_LIVE_AUDIO_OPEN;
		audioCtrlInfo.audiochnnel = chnn;
		m_bMute = false;
	}
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CONTROL_LIVE_AUDIO, (unsigned char*)(&audioCtrlInfo), sizeof(NET_LIVEAUDIO_CTRL_INFO));
	
	return KEY_VIRTUAL_MSG_STOP;	
}


unsigned long CAudioControlDlg::OnCkForAuto()
{
	NET_LIVEAUDIO_CTRL_INFO audioCtrlInfo;
	audioCtrlInfo.type = NET_LIVE_AUDIO_AUTO;
	//临时用audiochnnel，表示开启和关闭AUTO
	if (m_ckAuto.IsCheck())
	{
		audioCtrlInfo.audiochnnel = 1; 
		m_bAutoDefa = true;
	}
	else
	{
		audioCtrlInfo.audiochnnel = 0; 
		m_bAutoDefa = false;
	}	
	CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CONTROL_LIVE_AUDIO, (unsigned char*)(&audioCtrlInfo), sizeof(audioCtrlInfo));	
	return KEY_VIRTUAL_MSG_STOP;	
}

unsigned long CAudioControlDlg::OnSlider(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	m_volume = m_sliderVolume.GetPos();
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_MODIFY_AUDIO_VOLUME, (unsigned char*)(&m_volume), sizeof(unsigned long));

	if( GetShowTipType(TIP_MSG) )
	{
		std::string volume = m_pStrTable->String(E_STR_ID_VOLUME);
		char strPos[32] = {0};
		sprintf(strPos," : %d", m_volume);
		volume += strPos;
		MessageOut(volume.c_str(),TIP_MSG);
	}

	if( KEY_VIRTUAL_BLUP == keyID )
	{
		//在调节音量时，拖动slider结束时发送无效音量到主线程，主线程收到该无效音量时写“音量调节”到日志文件

		unsigned long volume = INVALID_INDEX(unsigned long);
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_MODIFY_AUDIO_VOLUME, (unsigned char*)(&volume), sizeof(unsigned long));
	}

	return KEY_VIRTUAL_MSG_STOP;
}


void CAudioControlDlg::EndDialog(DLG_END_VALUE result)
{
	
	if (m_bModify)
	{
		SaveAudioCfg();
	}
	CDialog::EndDialog(result);
}












