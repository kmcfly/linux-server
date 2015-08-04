/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-07-22
** Name         : Disktop.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "Disktop.h"
#include "MessageMan.h"
#include "CfgDlg.h"
#include "TimeCtrl.h"
#include "DisplayMan.h"
#include "WzdManDlg.h"
#include "FuncCustom.h"
#include "PicCreate.h"
#include "NetDeviceManager.h"
#include "SystemInfoDlg.h"
#include "PlaybackDlg.h"

bool g_bProIdequeSubProdID = true;
bool g_bHaveSnapPic = true;

//在向导修改语言
bool g_bWizzardChangeLanguage = false;
bool g_bNotCloseWizzard = false;

extern DVR_SYSTEM_STATUS	g_system_status;
extern VIDEO_FORMAT			g_video_format;
extern bool                 g_upgrade_status;
extern unsigned long		g_showWizzard;

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
extern WND_STARTUP_INFO* g_p_wnd_startup_info;
extern WND_STARTUP_INFO* g_p_cur_startup_info;
//////////////////////////////////////////////////////////////////////////
CDisktop::CDisktop() : m_pMsgMan(NULL), m_lastShowTime(0), m_showTime(0), m_bShowToolBar(true), m_pTipWindow(NULL)
{
	m_1X1CH = 0x0001;
	m_2X2CH = 0x000f;
	m_2X3CH = 0x003f;
	m_1A5CH = 0x003f;
	m_3X3CH = 0x01ff;
	m_1A7CH = 0x00ff;
	m_1A12CH =0x1fff;
	m_4X4CH = 0xffff;
	m_5X5CH	= (ULONGLONG)0x01ffffff;
	m_6X6CH	= ((ULONGLONG)0xf) << 32  | 0xffffffff;
	
	m_toolBarLastTime = 0;
	m_keyMode		= KEY_MODE_FUNC;
	m_videoFormat	= g_video_format;

	m_keyFlyLeftID = 0;
	m_keyFlyRightID = 0;

	m_countSnapTip = 0;
	m_selTime = 5 * 60;
	m_bNoMenuMsg = false;
	m_startTime = 0;
	m_burning = 0;
}

CDisktop::~CDisktop()
{
	m_keyFlyLeftID = 0;
	m_keyFlyRightID = 0;
}

void CDisktop::Quit()
{
	//只有当状态仍然为运行时，需要修改标记，好退出程序。
	if (DVR_SYSTEM_RUNING == g_system_status)
	{
		g_system_status = DVR_SYSTEM_EXIT;
	}
}
//////////////////////////////////////////////////////////////////////////
void CDisktop::OnInitial()
{
	//
	m_videoFormat = g_video_format;
	if(!IsSupportHybridDisplay())
	{
		unsigned long SplitMode = CProduct::Instance()->GetSplitMode(g_p_login_succ->productInfo.localVideoInputNum);
		if(g_p_cur_startup_info->SplitMode > SplitMode)
		{
			g_p_cur_startup_info->SplitMode = SplitMode;
		}
	}
	m_pWndStartupInfo = g_p_cur_startup_info;

	//
	m_style |= WND_NO_TITLE;

	GUI::CDialog::OnInitial();
	m_pMsgMan = CMessageMan::Instance();
	//////////////////////////////////////////////////////////////////////////

	ULONGLONG effectCH = 0;
	int videoInputNum = 0, localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	IsSupportCH(effectCH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	videoInputNum = localVideoInputNum + netVideoInputNum;

	CKeyLocal1U::s_videoInputNum = IsSupportHybridDisplay()?videoInputNum:localVideoInputNum;
	unsigned char chnnNum = videoInputNum;	
	unsigned long splitMode = 0;
	if (4 == chnnNum)
	{
		splitMode= VIEW_SPLIT_2X2;
	}
	else if (5 == chnnNum)
	{
		splitMode= VIEW_SPLIT_2X3;
	}
	else if (8 == chnnNum)
	{
		splitMode= VIEW_SPLIT_3X3;
	}
	else if (9 == chnnNum)
	{
		splitMode= VIEW_SPLIT_3X3;
	}
	else if ((12 == chnnNum) || (16 == chnnNum))
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
	
	m_currSplitMode = m_pWndStartupInfo->SplitMode;

	//SDK 必须最先一次显示全通道,否则会出错
	ChangeDisplayByCH(ALL_CHNN_MASK(chnnNum), splitMode, -1, 0, 1, true);


#ifdef  __ENVIRONMENT_LINUX__
	//如果程序里的厂家ID与机器里厂家的ID不一致，需要用户再次升级。
	if (!g_bProIdequeSubProdID)
	{
		printf("%s,%d,g_bProIdequeSubProdID:%d\n",__FILE__,__LINE__,g_bProIdequeSubProdID);
		m_update.DoModal( this , CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
		g_system_status = DVR_SYSTEM_REBOOT;
		return;
	}
#endif

	//向导页面最先出现
	if(IsSupportWzd())
	{
#if defined (__CUSTOM_US02__)
	#if defined(__CUSTOM_US02_NEW__) && defined (__SUPPORT_SHOW_QRCODE__) && defined (__TVT_NAT_SOCK__)
		OnNewNatWizzard();
	#elif defined (__SUPPORT_SHOW_QRCODE__) && defined (__TVT_NAT_SOCK__)
		OnNatWizarrd();
	#else
		OnUS02Wizzard();
	#endif
#else
	OnWizzard();
#endif
	}
	//2010-02-04 11:28:00 YSW
	//如果开启了权限检查，则要求人为登录，否则为自动登录
	if (m_pMsgMan->CheckPassword())
	{
		if (m_loginDlg.IsLogin())
		{
			m_loginDlg.GetLoginSuccessReply(CCfgDlg::s_loginSuccessReply);
		}
		else
		{
			//2010-10-25 15:32:00 LSL 取消第一次进系统的提示登录过程
			//改为开启跳台
			//GUI::CWnd* pWnd = IsShow() ? this : NULL;
			//if (DLG_OK == m_loginDlg.DoModal(pWnd))
			//{
			//	m_loginDlg.GetLoginSuccessReply(CCfgDlg::s_loginSuccessReply);
			//}
		}
		
	}
	else
	{
		if (!m_loginDlg.LogIn(NULL, NULL))
		{
			printf("Auto Login fail\n");
		}

		m_loginDlg.GetLoginSuccessReply(CCfgDlg::s_loginSuccessReply);
	}
	//////////////////////////////////////////////////////////////////////////
	unsigned short menu_width = 640;
	unsigned short menu_height = 420;

	//2010-02-03 18:46:00 YSW
	//由于某些监视器不能完整地显示整个输出，但接这样的监视器时，就会出现最下面的工具条不能看全的情况
	unsigned short yOffSet = 0;
	//此时需要特殊处理一下
	if (480 == m_height)
	{
		yOffSet += 20;
	}

	m_mainMenu.Create(GetRandWndID(), (m_rect.Width() - menu_width)/2, (m_rect.Height() - menu_height)/2 + yOffSet, menu_width, menu_height, this, 0, 1, false);

#ifdef __DVR_MENU__
	m_toolBarMenu.InitMenuData();
#else
	m_toolBar.Create(CTRLID_TOOL_BAR_DLG, 0, 0, m_rect.Width(), 40, this, 0, 2);
	m_toolBar.AutoLayout(&(m_pWndStartupInfo->ToolbarPos));
#endif


	//2010-02-03 16:57:00 YSW
	//其位置为屏幕的底部居中。
	char strTime [] = "8888-88-88 88:88:88 PM";
	GUI::CWnd::TextSize(strTime, strlen(strTime), m_timeWidth, m_timeHeight);
	//2010-02-03 17:44:00 YSW
	//由于某些监视器不能完整地显示整个输出，但接这样的监视器时，就会出现最下面的工具条不能看全的情况
	//此时需要特殊处理一下
	if (800 > m_width)
	{
		m_timeXPos = (m_width - m_timeWidth - 40);
		m_timeYPos = 30;
	}
	else
	{
		if (chnnNum >= 32)
		{
			if (m_width < 1280)
			{
				m_timeXPos = m_width - m_timeWidth - 20;
				m_timeYPos	= m_height - m_timeHeight - 80;
			}
			else
			{
				m_timeXPos = 1;
				m_timeYPos	= m_height - m_timeHeight - 45;
			}
		}
		else
		{
			m_timeXPos = 20;
			m_timeYPos	= m_height - m_timeHeight - 45;
		}
	}

	m_stcTime.Create(GetRandWndID(), m_timeXPos, m_timeYPos, m_timeWidth, m_timeHeight, this);
	m_stcTime.SetTextColor(GUI_RGB(255, 255, 0));
	if (800 > m_width)
	{
		m_stcTime.SetBKColor(0xff282828, false);
	}

	//磁盘满报警.其位置为屏幕的底部靠右。
	{
		const char *pStr = CStringTable::Instance()->String(E_STR_ID_MESSAGE_61);
		GUI::CWnd::TextSize(pStr, strlen(pStr), m_diskAlarmWidth, m_diskAlarmHeight);
		if (800 > m_width)
		{
			m_diskAlarmXPos = (m_width - m_diskAlarmWidth - 40);
			m_diskAlarmYPos = m_timeYPos + 20;			 
		}
		else
		{
			m_diskAlarmXPos	= m_width - m_diskAlarmWidth - 45;
			m_diskAlarmYPos	= m_height - m_diskAlarmHeight - 45;
		}

		m_stcDiskAlarm.Create(GetRandWndID(), m_diskAlarmXPos, m_diskAlarmYPos, m_diskAlarmWidth, m_diskAlarmHeight, this);
		m_stcDiskAlarm.SetTextColor(GUI_RGB(255, 255, 0));
	}

	// 创建拖动矩形框
	m_groupCtrl.Create(GetRandWndID(), 0, 0, m_width / 10, m_height / 10, this);
	m_groupCtrl.Show(false, false);

	m_bDragChnn = false;
	m_bFirstDragChnn = false;
	m_bDraged = false;
	m_countGroupCtrl = 0;
}

void CDisktop::OnPostInitial()
{
	CDialog::OnPostInitial();

//#ifdef __ENVIRONMENT_LINUX__

if(!g_bWizzardChangeLanguage)
{
	//登陆成功后，开启声音
	if ((g_p_login_succ->productInfo.audioInputNum > 0) || (g_p_login_succ->productInfo.netVideoInputNum > 0))
	{
		CMyList <NCFG_ITEM_ID> itemList;
		itemList.AddTail(NCFG_ITEM_LIVE_AUDIO_ENABLE);
		itemList.AddTail(NCFG_ITEM_LIVE_AUDIO_CHNN);
		itemList.AddTail(NCFG_ITEM_LIVE_AUDIO_VOLUME);

		unsigned char *pConfigData = NULL;
		unsigned long dataLen = 0;
		if (CConfigEx::Instance()->GetConfigEx(itemList, &pConfigData, dataLen))
		{
			CConfigBlock cfgBlock;
			cfgBlock.CreateBlock(pConfigData, dataLen);

			unsigned char *pItemData = NULL;
			unsigned long itemLen = 0;
			cfgBlock.GetConfig(NCFG_ITEM_LIVE_AUDIO_ENABLE, &pItemData, itemLen);
			NET_LIVEAUDIO_TYPE type = (NET_LIVEAUDIO_TYPE)(*reinterpret_cast<unsigned long *>(pItemData));
			pItemData = NULL;
			itemLen = 0;
			cfgBlock.GetConfig(NCFG_ITEM_LIVE_AUDIO_CHNN, &pItemData, itemLen);
			unsigned long chnn = *reinterpret_cast<unsigned long *>(pItemData);
			pItemData = NULL;
			itemLen = 0;
			if ((chnn >= g_p_login_succ->productInfo.audioInputNum) && (chnn < g_p_login_succ->productInfo.localVideoInputNum))
			{
				chnn = 0;
			}

			NET_LIVEAUDIO_CTRL_INFO audioCtrlInfo;
			audioCtrlInfo.audiochnnel = chnn;
			if (NET_LIVE_AUDIO_OPEN == type)
			{
				audioCtrlInfo.type = NET_LIVE_AUDIO_OPEN;
#ifdef __DVR_MENU__
				m_toolBarMenu.EnableAudio(true);
#else
				m_toolBar.EnableAudio(true, false);
#endif
			}
			else
			{
				audioCtrlInfo.type = NET_LIVE_AUDIO_MUTE;
#ifdef __DVR_MENU__
				m_toolBarMenu.EnableAudio(false);
#else
				m_toolBar.EnableAudio(false, false);
#endif
			}
			m_pMsgMan->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CONTROL_LIVE_AUDIO, (unsigned char*)(&audioCtrlInfo), sizeof(audioCtrlInfo));

			cfgBlock.GetConfig(NCFG_ITEM_LIVE_AUDIO_VOLUME, &pItemData, itemLen);
			unsigned long volume = *reinterpret_cast<unsigned long *>(pItemData);
			pItemData = NULL;
			itemLen = 0;
			CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_MODIFY_AUDIO_VOLUME, (unsigned char*)(&volume), sizeof(unsigned long));


			//默认开启auto功能
			{
				NET_LIVEAUDIO_CTRL_INFO audioCtrlInfo;
				audioCtrlInfo.type = NET_LIVE_AUDIO_AUTO;
				audioCtrlInfo.audiochnnel = 1; 
				CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CONTROL_LIVE_AUDIO, (unsigned char*)(&audioCtrlInfo), sizeof(audioCtrlInfo));	
			}
			cfgBlock.DestroyBlock();

			delete [] pConfigData;
		}		
	}

//#endif

	//2010-01-27 14:32:00 YSW
	//当开机后需要显示出现场分割线和一些状态信息，但是由于Dialog内部要检查是否RefreshLive标记才真正刷新，
	//所以在此通过刷新现场的方式来实现。
	//CMessageMan::Instance()->RefreshDisplay(ALL_CHNN_MASK(g_p_login_succ->productInfo.videoInputNum));

	//刷新屏幕到跳台第一组
	ChangeDisplayToDwellGroup();

#ifdef __DVR_BASIC__
	if(0 != m_pWndStartupInfo->DoDwell)
	{
		m_pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DODWELL_START,NULL, 0);
#ifdef __DVR_MENU__
		m_toolBarMenu.EnableDwell(true);
#else
		m_toolBar.EnableDwell(true);
#endif
	}
	else
	{
		m_pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DODWELL_STOP,NULL, 0);

#ifdef __DVR_MENU__
		m_toolBarMenu.EnableDwell(false);
#else
		m_toolBar.EnableDwell(false);
#endif
	}
#endif

	//
#ifndef __DVR_MENU__
	GUI::CRect tmRect(m_timeXPos, m_timeYPos, m_timeXPos + m_timeWidth, m_timeYPos + m_timeHeight);
	GUI::CRect tbRect = m_toolBar.GetWindowRect();

	if(tbRect.IntersectRect(tmRect))
	{
		//重叠, 则偏移位置

		if((tbRect.m_top + tbRect.Height() / 2) > m_height / 2)
		{
			//下半屏幕
			m_stcTime.MoveWnd(m_timeXPos, tbRect.m_top - m_timeHeight - 2);
		}
		else
		{
			m_stcTime.MoveWnd(m_timeXPos, tbRect.m_bottom + 6);
		}
	}
	else
	{
		m_stcTime.MoveWnd(m_timeXPos, m_timeYPos);
	}
#endif
}
}

void CDisktop::OnPaint(GUI::PAINT_STATUS status)
{
	//拦截绘制，保证不把现场分割线等刷掉。
}

void CDisktop::OnDestroy()
{
	//如果是重绘界面，则关闭回放和备份等功能
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_STOP, NULL, 0, msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_LOCAL_CLIENT_BACKUP_STOP, NULL, 0, msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	GUI::CDialog::OnDestroy();

	//[20010-04-22 20:00 L]退出对话框保证跳台被关闭
	//if(m_bDoDwell)
	//{
	//	m_pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DODWELL_STOP,NULL, 0);

	//	m_bDoDwell = false;
	//}

	m_loginDlg.Logoff();
}

//用于拦截Esc及Exit的消息
void CDisktop::EndDialog(DLG_END_VALUE result)
{
	OnHide();
}

unsigned long CDisktop::OnVirtualEsc(unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
#ifdef __DVR_MENU__
	if(m_mainMenu.IsShow())
	{
		return keyID;
	}
	else
	{
		NET_DISPLAY_INFO netDisp;
		memset(&netDisp, 0, sizeof(NET_DISPLAY_INFO));
		unsigned long curSplitMode;

		//获取当前分割模式和显示的通道信息，xPos、yPos的值没必要使用
		if(GetCurDisplay(netDisp, xPos, yPos, false))
		{
			curSplitMode = netDisp.split_mode[DISPLAY_LAYER_ANALOG];
			m_toolBarMenu.SetCurSplitMode(curSplitMode);
		}
		else
		{
			curSplitMode = 0;
		}
		if(VIEW_SPLIT_1X1 != curSplitMode)
		{
			m_toolBarMenu.SetItemVisible(CToolBarMenu::MITEM_ID_PIP, false);
			m_toolBarMenu.SetItemVisible(CToolBarMenu::MITEM_ID_PIP_MORE, false);
		}
		else
		{
			//检查大画面是否有权限			
			bool bHaveAuth = false;

			for(int i = 0; i < netDisp.data_len; i++)
			{
				if(DISPLAY_SIGNAL_ANALOG == DISPLAY_GET_SIGNAL(netDisp.data[i]))
				{
					int chnn = DISPLAY_GET_CHNN(netDisp.data[i]);
					ULONGLONG ulCH = static_cast <ULONGLONG> (0x01 <<(static_cast<int>(chnn)));

					ULONGLONG authCH = CUserMan::Instance()->CheckAuthority(AUTH_LIVE);
					if(0 != (ulCH & authCH))
					{
						bHaveAuth = true;
						break;
					}
				}
				else if( DISPLAY_SIGNAL_NET == DISPLAY_GET_SIGNAL(netDisp.data[i]) )
				{
					int chnn = DISPLAY_GET_CHNN(netDisp.data[i]);
					chnn += CProduct::Instance()->LocalVideoInputNum();
					ULONGLONG ulCH = static_cast <ULONGLONG> (0x01 <<(static_cast<int>(chnn)));

					ULONGLONG authCH = CUserMan::Instance()->CheckAuthority(AUTH_LIVE);
					if(0 != (ulCH & authCH))
					{
						bHaveAuth = true;
						break;
					}
				}
			}

			if (bHaveAuth)
			{
				m_toolBarMenu.SetItemVisible(CToolBarMenu::MITEM_ID_PIP, true);
				m_toolBarMenu.SetItemVisible(CToolBarMenu::MITEM_ID_PIP_MORE, true);
			}
			else
			{
				m_toolBarMenu.SetItemVisible(CToolBarMenu::MITEM_ID_PIP, false);
				m_toolBarMenu.SetItemVisible(CToolBarMenu::MITEM_ID_PIP_MORE, false);
			}
		}
		
		unsigned long ret = m_toolBarMenu.PopupProc(keyID, xPos, yPos);

		switch (ret)
		{
		case CToolBarMenu::MITEM_ID_NXN:
			{
				ULONGLONG CH = 0;
				unsigned long splitMode = 0, n = 0;

				if(m_toolBarMenu.GetSelChnn(CH, splitMode, n))
				{
					if(OnChangeCH(CH, splitMode, n, false))
					{
						GetRootWnd()->Repaint();
					}
				}
			}
			break;
		case CToolBarMenu::MITEM_ID_P_1X1:
			{
				if(VIEW_SPLIT_1X1 == netDisp.split_mode[DISPLAY_LAYER_ANALOG])
				{
					int PipCH = 0;
					ULONGLONG ulCH= 0;
					unsigned char chnn = 0;
					for(int i = 0; i < netDisp.data_len; i++)
					{
						if(DISPLAY_SIGNAL_ANALOG == DISPLAY_GET_SIGNAL(netDisp.data[i]))
						{
							chnn = DISPLAY_GET_CHNN(netDisp.data[i]);
							ulCH = static_cast <ULONGLONG> (0x01 <<(static_cast<int>(chnn)));
							PipCH = m_toolBarMenu.GetSelPipChnn();
							if (PipCH != chnn)
							{
								ChangeDisplayByCH(ulCH, curSplitMode, PipCH);
							}
						}
						else if(DISPLAY_SIGNAL_NET == DISPLAY_GET_SIGNAL(netDisp.data[i]))
						{
							chnn = DISPLAY_GET_CHNN(netDisp.data[i]);
							chnn += CProduct::Instance()->LocalVideoInputNum();
							ulCH = static_cast <ULONGLONG> (0x01 <<(static_cast<int>(chnn)));
							PipCH = m_toolBarMenu.GetSelPipChnn();
							if (PipCH != chnn)
							{
								ChangeDisplayByCH(ulCH, curSplitMode, PipCH);
							}
						}
					}
				}
			}
			break;
		case CToolBarMenu::MITEM_ID_COLOR:
			{
				OnChangeColor();
			}
			break;
		case CToolBarMenu::MITEM_ID_SNAP:
			{
				OnSnap();
			}
			break;
		case KEY_VIRTUAL_SEQ:
			{
				OnDwell();
			}
			break;
		case KEY_VIRTUAL_ZOOM:
			{
				OnZoom();
			}
			break;
		case KEY_VIRTUAL_AUDIO:
			{
				OnAudio();
			}
			break;
		case KEY_VIRTUAL_PTZ:
			{
				OnPTZ();
			}
			break;
		case KEY_VIRTUAL_RECORD:
			{
				OnRec();
			}
			break;
		case KEY_VIRTUAL_PLAY:
			{
				OnPlay();
			}
			break;
		case KEY_VIRTUAL_MENU:
			{
				OnMainMenu();
			}
			break;
		case CToolBarMenu::MITEM_ID_CAM_PARA:
			{
				OnChangeCamPara();
			}
			break;
		default:
			break;
		}

		return KEY_VIRTUAL_MSG_STOP;
	}
#else
	return GUI::CDialog::OnVirtualEsc(keyID, xPos, yPos);
#endif
}

#ifndef __DVR_MENU__

unsigned long CDisktop::OnClickToolBar(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{

	assert(m_toolBar.GetID() == ctrlID);
	unsigned long commandID = keyID & (~KEY_VIRTUAL_MSG_DEFAULT_TAG);
	ULONGLONG auth;

	switch (commandID)
	{
	case CTRLID_TOOL_BAR_CHANGE_COLOR:
		{
			return OnChangeColor();
		}
		break;
	case CTRLID_TOOL_BAR_STATUS:
		{
			return OnChangeToolStatus();
		}
		break;
	case CTRLID_TOOL_BAR_SNAP:
		{
			return OnSnap();			
		};
		break;
	case CTRLID_TOOL_BAR_1A12_EX:
		{
			return On1A12EX();
		}
		break;
	case CTRLID_TOOL_BAR_1A12:
		{
			return On1A12();
		}
		break;
	case CTRLID_TOOL_BAR_1A7_EX:
		{
			return On1A7Ex();
		}
		break;
	case CTRLID_TOOL_BAR_1A7:
		{
			return On1A7();
		}
		break;
	case CTRLID_TOOL_BAR_1A5_EX:
		{
			return On1A5Ex();
		}
		break;
	case CTRLID_TOOL_BAR_1A5:
		{
			return On1A5();
		}
		break;
	case CTRLID_TOOL_BAR_CAM_PARA:
		{
			return OnChangeCamPara();
		}
		break;
	default:
		return KEY_VIRTUAL_MSG_CONTINUE;
		break;
	}
}

#endif

unsigned long CDisktop::OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	bool bToolbar = false;

#ifndef __DVR_MENU__
	if(m_toolBar.IsShow())
	{
		m_toolBar.OnEnter(xPos, yPos);

		bToolbar = m_toolBar.PtInToolbar(xPos, yPos);
	}
#endif

	if(!m_mainMenu.IsShow())
	{
		m_bDragChnn = !bToolbar;
		m_bFirstDragChnn = true;

		if(m_bDragChnn)
		{
			if(GetCurDisplay(m_displayinfo, xPos, yPos, true) && (m_displayinfo.data_len > 0))
			{
				m_bDragChnn = true;

				m_enterDragPosX = xPos;
				m_enterDragPosY = yPos;
			}
			else
			{
				m_bDragChnn = false;
			}
		}
	}
	else
	{
		m_bDragChnn = false;
	}

	m_bDraged = false;

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnMouseMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	m_xPos = xPos;
	m_yPos = yPos;

#ifndef __DVR_MENU__
	m_toolBar.OnMouseMove(xPos, yPos);
#endif

#ifdef __CUSTOM_US02_NEW__
	if(m_pTipWindow != NULL && m_pTipWindow->IsShow())
	{
		m_pTipWindow->Show(false, true);
		GetRootWnd()->Repaint();
	}
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	m_xPos = xPos;
	m_yPos = yPos;

#ifndef __DVR_MENU__
	m_toolBar.OnMouseMove(xPos, yPos);
#ifdef __CUSTOM_US02_NEW__
	if(m_pTipWindow != NULL && m_pTipWindow->IsShow())
		{
			m_pTipWindow->Show(false, true);
			GetRootWnd()->Repaint();
		}
#endif
	if(m_toolBar.IsShow())
	{
		GUI::CRect rect = m_toolBar.OnMouseDrag(xPos, yPos, false);

		if((rect.Width() > 1) && (rect.Height() > 1))
		{
			GUI::CWnd::DrawClear(rect.m_left, rect.m_top, rect.Width(), rect.Height());
			m_toolBar.Show(true, false);

			GUI::CWnd::Refresh(rect.m_left, rect.m_top, rect.Width(), rect.Height());

			CMessageMan::Instance()->RefreshLiveTrue();
		}
	}
#endif

	if(m_bDragChnn)
	{
		int width = m_groupCtrl.GetClientRect().Width();
		int height = m_groupCtrl.GetClientRect().Height();

		GUI::CRect rectGroup(0, 0, width, height);

		if((xPos > width / 2) && ((xPos + width / 2) < m_width))
		{
			rectGroup.MoveToX(xPos - width / 2);
		}
		else if((xPos + width / 2) >= m_width)
		{
			rectGroup.MoveToX(m_width - width);
		}

		if((yPos > height / 2) && ((yPos + height / 2) < m_height))
		{
			rectGroup.MoveToY(yPos - height / 2);
		}
		else if((yPos + height / 2) >= m_height)
		{
			rectGroup.MoveToY(m_height - height);
		}

		if(m_bFirstDragChnn)
		{
			unsigned long offset_len = ((m_width / 20) * (m_width / 20)) + ((m_height / 20) * (m_height / 20));
			//unsigned long offset_len = 200;
			if(!IsAlmostEqual(m_lastDragPosX, m_lastDragPosY, xPos, yPos, offset_len))
			{
				//防止双击引发操作失误
				m_groupCtrl.MoveWnd(rectGroup.m_left, rectGroup.m_top);
				m_groupCtrl.Show(true, true);

				m_lastDragPosX = xPos;
				m_lastDragPosY = yPos;

				m_bFirstDragChnn = false;
				m_bDraged = true;

				m_countGroupCtrl = 0;

				CMessageMan::Instance()->RefreshLiveTrue();
			}
		}
		else
		{
			unsigned long offset_len = ((m_width / 40) * (m_width / 40)) + ((m_height / 40) * (m_height / 40));
			//unsigned long offset_len = 200;
			if(!IsAlmostEqual(m_lastDragPosX, m_lastDragPosY, xPos, yPos, offset_len))
			{
				GUI::CRect rectOld = m_groupCtrl.GetWindowRect();
				//GUI::CWnd::DrawClear(rectOld.m_left, rectOld.m_top, rectOld.Width(), rectOld.Height()); //去除残影
				
				m_groupCtrl.MoveWnd(rectGroup.m_left, rectGroup.m_top);
				m_groupCtrl.Show(true, false);

				GUI::CRect rectRefresh = rectOld.UnionRect(rectGroup);
				CWnd::Refresh(rectRefresh.m_left, rectRefresh.m_top, rectRefresh.Width(), rectRefresh.Height());

				m_bDraged = true;

				m_countGroupCtrl = 0;

				CMessageMan::Instance()->RefreshLiveTrue();

				m_lastDragPosX = xPos;
				m_lastDragPosY = yPos;
			}
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnMouseLeftUp(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
#ifndef __DVR_MENU__
	#ifdef __CUSTOM_US02_NEW__
	if(m_pTipWindow != NULL && m_pTipWindow->IsShow())
		{
			m_pTipWindow->Show(false, true);
			GetRootWnd()->Repaint();
		}
#endif
	if( m_toolBar.IsShow()&&
		(KEY_VIRTUAL_MSG_CONTINUE == m_toolBar.OnMouseLeftUp()) &&
		m_toolBar.GetCfgPos(m_pWndStartupInfo->ToolbarPos))
	{
		TrySaveStartupInfo(g_p_wnd_startup_info, m_pWndStartupInfo);

		gui_trace_np(true, printf("save startup info. dwell=%d. toolbar style=%d.(%d.%d).\n", g_p_wnd_startup_info->DoDwell, 
					g_p_wnd_startup_info->ToolbarPos.style, g_p_wnd_startup_info->ToolbarPos.xPos, g_p_wnd_startup_info->ToolbarPos.yPos));

		if(m_stcTime.IsShow())
		{
			GUI::CRect tmRect(m_timeXPos, m_timeYPos, m_timeXPos + m_timeWidth, m_timeYPos + m_timeHeight);
			GUI::CRect tbRect = m_toolBar.GetWindowRect();

			if(tbRect.IntersectRect(tmRect))
			{
				//重叠, 则偏移位置
				
				if((tbRect.m_top + tbRect.Height() / 2) > m_height / 2)
				{
					//下半屏幕
					m_stcTime.MoveWnd(m_timeXPos, tbRect.m_top - m_timeHeight - 2);
				}
				else
				{
					m_stcTime.MoveWnd(m_timeXPos, tbRect.m_bottom + 6);
				}
			}
			else
			{
				m_stcTime.MoveWnd(m_timeXPos, m_timeYPos);
			}
		}

		GetRootWnd()->Repaint();	
	}
#endif

	if(m_bDragChnn)
	{
		if(m_groupCtrl.IsShow())
		{
			m_groupCtrl.Show(false, false);
		}

		if(m_bDraged)
		{
			ChangeDisplayByTwoPt(m_enterDragPosX, m_enterDragPosY, xPos, yPos);
		}

		GetRootWnd()->Repaint();
	}

	m_bDragChnn = false;
	m_bFirstDragChnn = false;
	m_bDraged = false;
	m_countGroupCtrl = 0;

	return KEY_VIRTUAL_MSG_STOP; 
}

void CDisktop::OnIdle()
{
	//2010-02-03 16:57:00 YSW
	//现场显示时间，当有显示工具条时，则不显示时间。
	m_showTime = GetCurrTime();
#if defined(__CHIP3515__) || defined(__TDFH__)
	if (((m_showTime - m_lastShowTime) > 500000) || (m_showTime < m_lastShowTime))
#else
	if (((m_showTime - m_lastShowTime) > 250000) || (m_showTime < m_lastShowTime))
#endif
	{
		//处理鼠标拖动, 右键键值丢失情况
		//等待时间过长,则认为拖动结束,清理拖动框
		if(m_groupCtrl.IsShow())
		{
			m_countGroupCtrl++;

			if(m_countGroupCtrl > 10)
			{
				m_countGroupCtrl = 0;
				m_groupCtrl.Show(false, true);
				CMessageMan::Instance()->RefreshLiveTrue();
			}
		}
		else
		{
			m_countGroupCtrl = 0;
		}

		m_lastShowTime = m_showTime;
		
		if (m_pMsgMan->ShowTime())
		{
			if (!m_stcTime.IsShow())
			{
				m_stcTime.Show(true, false);	
			}

			GUI::CTimeCtrl::GetTime32Text(m_strTime, m_showTime/1000000);

			GUI::CRect stcRect = m_stcTime.GetWindowRect();
			GUI::CWnd::DrawClear(stcRect.m_left, stcRect.m_top, stcRect.Width(), stcRect.Height());
			m_stcTime.SetCaption(m_strTime, true);
		}
		else
		{
			if (m_stcTime.IsShow())
			{
				m_stcTime.Show(false, true);
				CMessageMan::Instance()->RefreshLiveTrue();
			}
		}

		if (m_pMsgMan->ShowDiskAlarm())
		{
			if (!m_stcDiskAlarm.IsShow())
			{
				m_stcDiskAlarm.Show(true, false);
			}

			GUI::CWnd::DrawClear(m_diskAlarmXPos, m_diskAlarmYPos, m_diskAlarmWidth, m_diskAlarmHeight);
			m_stcDiskAlarm.SetCaption(m_pStrTable->String(E_STR_ID_MESSAGE_61), true);
		}
#if defined(__TW01_RILI__)
		else if (m_pMsgMan->IsDiskError())
		{
			if (!m_stcDiskAlarm.IsShow())
			{
				m_stcDiskAlarm.Show(true, false);
			}

			GUI::CWnd::DrawClear(m_diskAlarmXPos, m_diskAlarmYPos, m_diskAlarmWidth, m_diskAlarmHeight);
			m_stcDiskAlarm.SetCaption(m_pStrTable->String(E_STR_ID_LOG_DISK_WARNING), true);
		}
#endif
		else
		{
			if (m_stcDiskAlarm.IsShow())
			{
				m_stcDiskAlarm.Show(false, true);	
				CMessageMan::Instance()->RefreshLiveTrue();
			}
		}

	}

	if(m_countSnapTip > 0)
	{
		m_countSnapTip--;

		if(m_countSnapTip <= 0)
		{
			bool bShowSnapTip = true, bSucc = true;
			m_pMsgMan->ShowSnapTip(bShowSnapTip, bSucc);

			if(bShowSnapTip)
			{
				CMessageMan::Instance()->SetShowSnapTip(false, bSucc);
			}
		}
	}
	
	//当网络设备断开后要求刷新时，对界面刷新
#ifdef __DVR_ULTIMATE__
	if(CNetDeviceManager::Instance()->IfNeedRefresh())
	{
		CNetDeviceManager::Instance()->RefreshDisplay();
	}
#endif

#if defined(__CUSTOM_US02__) && (defined(__TDFH__) || defined(__CHIP3531__) || defined(__CHIPGM__) || defined(__CHIP3535__) || defined(__NOMENU_OUT_INFO__) || defined(__CHIPTI__))
	if (m_bNoMenuMsg)
	{
		int curTime = GetCurrTime32();
		if (curTime < m_startTime)
		{
			m_startTime = curTime;
		}
		else
		{
			if (20 < (curTime - m_startTime))
			{
				//const char * pStr = CStringTable::Instance()->String(E_STR_ID_TIP_01_38);
				CPicCreate pic;
				pic.CreatePic(NULL, 600, 400, GUI_RGB(255, 255, 255));
				pic.SetPicToAuxiliaryOutputDevice();
				m_bNoMenuMsg = false;
			}
		}
	}
#endif

#ifdef __CUSTOM_US02_NEW__
	if(m_pTipWindow != NULL)
	{
		if(m_pTipWindow->IsShow())
		{
			unsigned long curTime = GetCurrTime32();
			if((curTime - m_pTipWindow->GetShowTime()) >= 5)
			{
				m_pTipWindow->Show(false, false);
			}
		}
	}
#endif
	GUI::CDialog::OnIdle();
}

void CDisktop::OnExitProc()
{
	if (DVR_SYSTEM_LOGOUT != g_system_status)
	{
		const char *pStr = NULL;

		if (m_videoFormat != g_video_format)
		{
			pStr = CStringTable::Instance()->String(E_STR_ID_MESSAGE_01);
		}
		else if (g_upgrade_status)
		{
			pStr = CStringTable::Instance()->String(E_STR_ID_MESSAGE_109);
		}
		else if (DVR_SYSTEM_REDRAW == g_system_status)
		{
			pStr = CStringTable::Instance()->String(E_STR_ID_TIP_01_38);
		}
		else
		{
			pStr = CStringTable::Instance()->String(E_STR_ID_PLEASE_WAIT);
		}

		GUI::CWnd::DrawClear(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), true);

		unsigned char zoom = (m_width < 1024) ? 1 : 2;

		unsigned long len = strlen(pStr);
		unsigned short cx = 0, cy = 0;
		GUI::CWnd::TextSize(pStr, len, cx, cy, zoom);

		unsigned short x = (m_rect.Width() - cx)/2;
		unsigned short y = (m_rect.Height() - cy)/2;
		x = (m_rect.Width() <= x) ? 0 : x;
		y = (m_rect.Height() <= y) ? 0 : y;

		GUI::CWnd::DrawRect(x, y, cx, cy, GUI_RGB(0, 0, 0), false);
		GUI::CWnd::DrawText(pStr, len, x, y, cx, cy, GUI_RGB(255, 255, 255), true, zoom);

		DVRSleep(1000);
	}
}

ULONGLONG CDisktop::CheckAuthority(USER_AUTHORITY authority, bool bReDisplay)
{

	if(m_pTipWindow != NULL && m_pTipWindow->IsShow())
	{
		m_pTipWindow->Show(false, true);
		GetRootWnd()->Repaint();
	}	
	//必需要求有人登录，可能不需要检查权限，但是还有应该登录，他可以在不
	//检查权限的情况，做其不具备权限的操作。
	if (m_loginDlg.IsLogin())
	{	
		//已经登录
		return  CUserMan::Instance()->CheckAuthority(authority);
	}
	else 
	{
		//2010-02-04 12:28:00 YSW
		//如果开启了权限检查，则要求人为登录，否则为自动登录
		if (m_pMsgMan->CheckPassword())
		{
			GUI::CWnd* pWnd = IsShow() ? this : NULL;

			if (DLG_OK == m_loginDlg.DoModal(pWnd))
			{
				m_loginDlg.GetLoginSuccessReply(CCfgDlg::s_loginSuccessReply);

				//2010-02-04 11:06:00 YSW
				//重新登录后，也需要刷新一下现场
				if ((AUTH_LIVE != authority) || bReDisplay)
				{
					//由于检查现场权限的部分呢，都在此函数返回后发送了切换显示模式的请求，所以在此要过滤掉
					//参数还是使用上次操作的结果来
					
					//2010-02-05 16:46:00 L 先获取当前显示的通道
					GetCurDisplay(m_displayinfo);
					m_displayinfo.type = NET_DISPLAY_CHANGE_DATA_PACK;

					MESSAGE_DATA msgData;
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&m_displayinfo, sizeof(NET_DISPLAY_INFO), msgData);	
					m_pMsgMan->ReleaseMsgBuff(msgData);
				}

				GetRootWnd()->Repaint();
				return CUserMan::Instance()->CheckAuthority(authority);
			}
			else
			{
				GetRootWnd()->Repaint();
				return static_cast<ULONGLONG>(0x0);
			}
		}
		else
		{
			if (!m_loginDlg.LogIn(NULL, NULL))
			{
				printf("Auto Login fail\n");
				assert(false);
				return static_cast<ULONGLONG>(0x0);
			}

			m_loginDlg.GetLoginSuccessReply(CCfgDlg::s_loginSuccessReply);

			//2010-02-04 11:06:00 YSW
			//重新登录后，也需要刷新一下现场
			if ((AUTH_LIVE != authority) || bReDisplay)
			{
				//由于检查现场权限的部分呢，都在此函数返回后发送了切换显示模式的请求，所以在此要过滤掉
				//参数还是使用上次操作的结果来
				GetCurDisplay(m_displayinfo);
				m_displayinfo.type = NET_DISPLAY_CHANGE_DATA_PACK;

				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&m_displayinfo, sizeof(NET_DISPLAY_INFO), msgData);	
				m_pMsgMan->ReleaseMsgBuff(msgData);
			}

			return CUserMan::Instance()->CheckAuthority(authority);
		}
	}
}

bool CDisktop::GetCurDisplay(NET_DISPLAY_INFO& disp, unsigned short x, unsigned short y, bool bUsePt)
{
	bool ret = false;

	NET_DISPLAY_INFO inDisp;
	memset(&inDisp, 0, sizeof(NET_DISPLAY_INFO));
	inDisp.x = x;
	inDisp.y = y;
	
	if(bUsePt)
	{
		inDisp.type = NET_DISPLAY_GET_INFO_XY;
	}
	else
	{
		inDisp.type = NET_DISPLAY_GET_INFO;
	}

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_DISPLAY_INFO, (unsigned char*)&inDisp, sizeof(NET_DISPLAY_INFO), msgData);
	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CTRL_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		//获取当前显示的通道
		NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));

		memcpy(&disp, pDispInfo, sizeof(NET_DISPLAY_INFO));

		ret = true;
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return ret;
}

void CDisktop::GetDiskOwer(DVR_DISK_OWNER_LIST &diskOwnerList)
{
	MESSAGE_DATA msgData;
	m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_OWNER, NULL, 0, msgData);
	DVR_DISK_OWNER * pDiskOwner = (DVR_DISK_OWNER *)(msgData.pData + sizeof(PACKCMD));
	int diskOwnerNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_OWNER);
	if (diskOwnerNum > 0)
	{
		for (int i = 0; i < diskOwnerNum; ++i)
		{	
			DVR_DISK_OWNER &tmp = pDiskOwner[i];
			diskOwnerList.AddTail(tmp);
		}
	}
	m_pMsgMan->ReleaseMsgBuff(msgData);
    
	return;
}

void CDisktop::OnWizzard()
{
	static bool s_wzd_b_first = true;

	bool bShowWzd = false; //是否显示向导页面
	if(s_wzd_b_first)
	{
		s_wzd_b_first = false;

		if(0 != g_showWizzard)
		{
			bShowWzd = true;
		}
	}

	if(!bShowWzd)
	{
		return;
	}

	unsigned long diskStatus = -1;

#ifdef __ENVIRONMENT_LINUX__
	MESSAGE_DATA msgData;
	m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_NUM_STATUS, NULL, 0, msgData);
	ULONG sysDiskStatus = *reinterpret_cast<ULONG *>(msgData.pData + sizeof(PACKCMD));
	m_pMsgMan->ReleaseMsgBuff(msgData);
	diskStatus = sysDiskStatus;
#else
	diskStatus = 0; //NET_DISK_HAVE_READEWRITE_DISK | NET_DISK_HAVE_UNFORMATED_DISK;
#endif

	m_wzdManDlg.SetDiskStatus(diskStatus);

	//是否启用向导,向导页面弹出需要至少一个页面
	if( bShowWzd )
	{
		m_wzdManDlg.DoModal(this);
	}
}

BEGIN_MSG_MAP(CDisktop, GUI::CDialog)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_6X6, On6X6)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_5X5, On5X5)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_4X4, On4X4)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_3X3, On3X3)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_2X3, On2X3)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_2X2, On2X2)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_1X1, On1X1)

	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_SPLIT, OnSplit)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_SPLITEX, OnSplitEx)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_GROUP, OnGroup)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_GROUPEX, OnGroupEx)
#ifndef __DVR_MENU__
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_6X6_EX, On6X6Ex)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_5X5_EX, On5X5Ex)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_4X4_EX, On4X4Ex)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_3X3_EX, On3X3Ex)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_2X3_EX, On2X3Ex)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_2X2_EX, On2X2Ex)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_1X1_EX, On1X1Ex)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_PIP, OnPIP)
#endif

	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_MENU, OnMainMenu)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_SEARCH, OnSearch)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_BACKUP, OnBackup)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_INFO, OnInfo)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_LOG, OnLog)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_LIVE_SNAP, OnSnap)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_ALARM, OnManualAlarm)

	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_SEQ, OnDwell)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_ZOOM, OnZoom)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_AUDIO, OnAudio)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_PTZ, OnPTZ)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_RECORD, OnRec)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_PLAY, OnPlay)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_POWER, OnShutDown)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_ESC, OnHide)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_HIDE, OnHide)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_STOP, OnHide)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_EXIT, OnHide)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_PLAY_EX, OnPlayEx)

#ifndef __DVR_MENU__	
	ON_MSG_EX(m_toolBar.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickToolBar)
#endif
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_MOVE, OnMouseMove)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_DRAG, OnMouseDrag)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_ENTER, OnEnter)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_BLUP, OnMouseLeftUp)

	ON_MSG_EX(m_mainMenu.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickMainMenu)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_DBCLICK, OnDoubleClick)
	
	ON_MSG_KEY_RANGE( CTRLID_NULL, KEY_VIRTUAL_0, KEY_VIRTUAL_32, OnInputChnnNum )

	ON_MSG_KEY_RANGE(CTRLID_NULL, KEY_VIRTUAL_FLY_LEFT_1, KEY_VIRTUAL_FLY_LEFT_8, OnFlyLeft)
	ON_MSG_KEY_RANGE(CTRLID_NULL, KEY_VIRTUAL_FLY_RIGHT_1, KEY_VIRTUAL_FLY_RIGHT_8, OnFlyRight)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_FLY_MEDIUM, OnFlyMedium)

	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_BURN, OnBurn)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_BURN_EX, OnBurnEx)
	ON_MSG(CTRLID_NULL,KEY_VIRTUAL_SYSINFO,OnSysInfo)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_OVER, OnOver)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_OUT, OnOut)
	#endif
END_MSG_MAP()
//////////////////////////////////////////////////////////////////////////
unsigned long  CDisktop::On6X6Ex()
{
#ifndef __DVR_MENU__
	ULONGLONG chnnBits = 0xffffffff;
	GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_6X6);
	unsigned long x = rect.m_left + rect.Width()/2;

	OnSelectCH(x, g_p_login_succ->productInfo.videoInputNum, 32, chnnBits,VIEW_SPLIT_6X6);

	GetRootWnd()->Repaint();
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::On5X5Ex()
{
#ifndef __DVR_MENU__
	ULONGLONG chnnBits = 0x1ffffff;
	GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_5X5);
	unsigned long x = rect.m_left + rect.Width()/2;
	OnSelectCH(x, g_p_login_succ->productInfo.videoInputNum, 25, chnnBits,VIEW_SPLIT_5X5);

	GetRootWnd()->Repaint();
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::On4X4Ex()
{
#ifndef __DVR_MENU__
	ULONGLONG chnnBits = 0xffff;
	GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_4X4);
	unsigned long x = rect.m_left + rect.Width()/2;
	OnSelectCH(x, g_p_login_succ->productInfo.videoInputNum, 16, chnnBits,VIEW_SPLIT_4X4);

	GetRootWnd()->Repaint();
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::On1A12EX()
{
#ifndef __DVR_MENU__
	ULONGLONG chnnBits = 0x1fff;
	GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_1A12);
	unsigned long x = rect.m_left + rect.Width()/2;
	OnSelectCH(x, g_p_login_succ->productInfo.videoInputNum, 13, chnnBits,VIEW_SPLIT_1A12);

	GetRootWnd()->Repaint();
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::On1A7Ex()
{
#ifndef __DVR_MENU__
	ULONGLONG chnnBits = 0xff;
	GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_1A7);
	unsigned long x = rect.m_left + rect.Width()/2;
	OnSelectCH(x, g_p_login_succ->productInfo.videoInputNum, 8, chnnBits,VIEW_SPLIT_1A7);

	GetRootWnd()->Repaint();
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::On3X3Ex()
{
#ifndef __DVR_MENU__
	int maxSel = (8 < g_p_login_succ->productInfo.videoInputNum) ? 9 : 8;
	ULONGLONG chnnBits = (8 < g_p_login_succ->productInfo.videoInputNum) ? 0x1ff : 0xff;

	GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_3X3);
	unsigned long x = rect.m_left + rect.Width()/2;
	OnSelectCH(x, g_p_login_succ->productInfo.videoInputNum, maxSel, chnnBits,VIEW_SPLIT_3X3);

	GetRootWnd()->Repaint();
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::On1A5Ex()
{
#ifndef __DVR_MENU__
	ULONGLONG chnnBits = 0x3f;
	GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_1A5);
	unsigned long x = rect.m_left + rect.Width()/2;
	OnSelectCH(x, g_p_login_succ->productInfo.videoInputNum, 6, chnnBits,VIEW_SPLIT_1A5);

	GetRootWnd()->Repaint();
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::On2X3Ex()
{
#ifndef __DVR_MENU__
	ULONGLONG chnnBits = 0x3f;

	GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_2X3);
	unsigned long x = rect.m_left + rect.Width()/2;
	OnSelectCH(x, g_p_login_succ->productInfo.videoInputNum, 6, chnnBits,VIEW_SPLIT_2X3);

	GetRootWnd()->Repaint();
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::On2X2Ex()
{
#ifndef __DVR_MENU__
	ULONGLONG chnnBits = 0x0f;

	GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_2X2);
	unsigned long x = rect.m_left + rect.Width()/2;
	OnSelectCH(x, g_p_login_succ->productInfo.videoInputNum, 4, chnnBits,VIEW_SPLIT_2X2);

	GetRootWnd()->Repaint();
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::On1X1Ex()
{
#ifndef __DVR_MENU__
	ULONGLONG chnnBits = 0x01;
	GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_1X1);
	unsigned long x = rect.m_left + rect.Width()/2;
	OnSelectCH(x, g_p_login_succ->productInfo.videoInputNum, 1, chnnBits,VIEW_SPLIT_1X1);

	GetRootWnd()->Repaint();
#endif
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long  CDisktop::OnPIP()
{
	int ProductType = CProduct::Instance()->ProductType();
	if (TD_2708AS_PL != ProductType && TD_2716AE_PL != ProductType && TD_2716TD_PL != ProductType && TD_2732TD_PL != ProductType)
	{
#ifndef __DVR_MENU__
		ULONGLONG chnnBits = 0x01;
		GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_PIP);
		unsigned long x = rect.m_left + rect.Width()/2;
		OnSelectCH(x, g_p_login_succ->productInfo.videoInputNum, 1, chnnBits,VIEW_SPLIT_1X1);

		GetRootWnd()->Repaint();
#endif
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

#ifdef __CUSTOM_US02_NEW__
unsigned long CDisktop::OnOver()
{
	CWnd* active = GetActiveCtrl();
	if(active == NULL) return KEY_VIRTUAL_STOP;
	if(active->GetID() != CTRLID_TOOL_BAR_DLG)
	{
		if(m_pTipWindow != NULL)
		{
			m_pTipWindow->Show(false, true);
			GetRootWnd()->Repaint();
		}
		 return KEY_VIRTUAL_STOP;
	}
	if(active != NULL)
	{
		CWnd* aactive = active->GetActiveCtrl();
		CWnd* aaactive = aactive->GetActiveCtrl();

		int x, y;
		aactive->GetFocusPos(x, y);
		if(aaactive != NULL)
		{
			aactive = aaactive;
			aaactive->GetFocusPos(x, y);
		}
		if(aactive->GetID() == CTRLID_TOOL_BAR_DLG) return KEY_VIRTUAL_STOP;
		CTRLID ctrl = aactive->GetID();
		std::string caption;
		switch(ctrl)
		{
			case CTRLID_TOOL_BAR_SYSTEM:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_MENU));
				break;			
			}
		case CTRLID_TOOL_BAR_1X1:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_1X1));
				break;
			}
		case CTRLID_TOOL_BAR_2X2:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_2X2));
				break;
			}
		case CTRLID_TOOL_BAR_2X3:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_2X3));
				break;
			}
		case CTRLID_TOOL_BAR_1A5:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_2X3));
				break;
			}
		case CTRLID_TOOL_BAR_1A7:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_8X));
				break;
			}
		case CTRLID_TOOL_BAR_3X3:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_3X3));
				break;
			}
		case CTRLID_TOOL_BAR_1A12:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_13X));
				break;
			}
		case CTRLID_TOOL_BAR_4X4:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_4X4));
				break;
			}
		case CTRLID_TOOL_BAR_5X5:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_32X));
				break;
			}
		case CTRLID_TOOL_BAR_6X6:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_64X));
				break;
			}
		case CTRLID_TOOL_BAR_PIP:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_PIP));
				break;
			}
		case CTRLID_TOOL_BAR_DWELL:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_SEQUENCE));
				break;
			}
		case CTRLID_TOOL_BAR_CHANGE_COLOR:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_COLOR));
				break;
			}
		case CTRLID_TOOL_BAR_ZOOM:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_ZOOM));
				break;
			}
		case CTRLID_TOOL_BAR_AUDIO:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_AUDIO));
				break;
			}
		case CTRLID_TOOL_BAR_PTZ:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_PTZ));
				break;
			}
		case CTRLID_TOOL_BAR_SNAP:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_SNAP));
				break;
			}
		case CTRLID_TOOL_BAR_REC:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_RECORD));
				break;
			}
		case CTRLID_TOOL_BAR_PLAY:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_PLAYBACK));
				break;
			}
		case CTRLID_TOOL_BAR_STATUS:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_MOVE));
				break;
			}
		case CTRLID_TOOL_BAR_CAM_PARA:
			{
				caption.clear();
				caption.assign(m_pStrTable->String(E_STR_ID_TOOLBAR_TIP_CAM_PARAM));
				break;
			}
		default:
			{	printf("ctrl=%d\n", ctrl);
				return KEY_VIRTUAL_STOP;
			}
		}



		GUI::CRect rect = aactive->GetWindowRect();

		int xx = (caption.size()+2)*10;
		x = rect.m_left + rect.Width()/2;
		
		GUI::CRect rootRect = GetWindowRect();
		if((rootRect.m_right-x) < xx/2)
		{
			x = rootRect.m_right - xx;
		}
		else
		{
			x = (x > xx/2) ? (x - xx/2) : rect.m_left;
		}
		if((ctrl == CTRLID_TOOL_BAR_SYSTEM) && (m_stcTime.GetWindowRect().m_bottom < rect.m_top) && (m_stcTime.GetWindowRect().m_right > rect.m_left))
		{
			y = m_stcTime.GetWindowRect().m_top - m_length[STATIC_HEIGHT] - 10;
		}
		else
		{
			y = (rect.m_top > m_height / 2) ? (rect.m_top - m_length[STATIC_HEIGHT]) : rect.m_bottom;
			if(rect.m_top > m_height / 2)
			{
				y -= 10;
			}
		}
		if(m_pTipWindow == NULL)
		{
			m_pTipWindow = new CTipWindow();
			m_pTipWindow->Create(GetRandWndID(), x, y, xx, m_length[STATIC_HEIGHT], this, caption.c_str());
		}

		m_pTipWindow->SetShowTime(GetCurrTime32());

			//m_pTipWindow->SetWndSize(false, false, xx, m_length[STATIC_HEIGHT]);
		m_pTipWindow->SetWndPos(x, y, xx, m_length[STATIC_HEIGHT]);
		m_pTipWindow->SetCaption(caption.c_str(), false);
		m_pTipWindow->MoveWnd(x, y);


		m_pTipWindow->SetAlignText(ALIGN_TEXT_CENTER);
		m_pTipWindow->SetBKColor(0x737b7b, true);
		m_pTipWindow->SetTextColor(0xffff00);
		m_pTipWindow->SetEnable(true, false);	

		
		if(!(m_pTipWindow->GetCtrl() == ctrl && m_pTipWindow->isRectEq(x, y, xx, m_length[STATIC_HEIGHT])))
		{
			m_pTipWindow->Show(true, false);
			GetRootWnd()->Repaint();
			m_pTipWindow->SetCtrl(ctrl);
			m_pTipWindow->SetRect(x, y, xx, m_length[STATIC_HEIGHT]);			
		}
		else
		{
			m_pTipWindow->Show(true, true);
		}
	}
	return KEY_VIRTUAL_STOP;
}

unsigned long CDisktop::OnOut()
{

		if(m_pTipWindow != NULL && m_pTipWindow->IsShow())
		{
			m_pTipWindow->Show(false, true);
			GetRootWnd()->Repaint();
		}
		return KEY_VIRTUAL_STOP;
}
#endif

unsigned long  CDisktop::OnPlayEx()
{
#ifndef __DVR_MENU__
	if (0 != CheckAuthority(AUTH_PLAYBACK))
	{
		GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_PLAY);
		unsigned long x = rect.m_left + rect.Width()/2;
		rect = m_toolBar.GetWindowRect();
		GUI::CRect chnnRect = m_selReplayTimeDlg.GetDlgDefaultRect();
		x = (x > chnnRect.Width()/2) ? (x - chnnRect.Width()/2) : x;
		unsigned short y = (rect.m_top > m_height / 2) ? (rect.m_top - chnnRect.Height()) : rect.m_bottom;
		if(DLG_OK == m_selReplayTimeDlg.DoModal( NULL, x, y, chnnRect.Width(), chnnRect.Height()))
		{
			m_selTime = m_selReplayTimeDlg.GetSelTime();
		}
		
	}

	GetRootWnd()->Repaint();
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnBurn()
{
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));

	if (m_burning)
	{
		m_burning = 0;
	}
	else
	{
		m_burning = 1;
	}

	m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_BURN_REC, (unsigned char*)&m_burning, sizeof(unsigned long), msgData);	
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CTRL_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		MessageBox("ERROR", "burn");
	}
	m_pMsgMan->ReleaseMsgBuff(msgData);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnBurnEx()
{
#ifndef __DVR_MENU__
	ULONGLONG chnnBits = 0x0f;

	GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_BURN);
	unsigned long x = rect.m_left + rect.Width()/2;
	OnSelectBurn(x, g_p_login_succ->productInfo.videoInputNum, 4, chnnBits);

	GetRootWnd()->Repaint();
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnSysInfo()
{
	CSystemInfoDlg	SysInfoDlg;
	SysInfoDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
	GetRootWnd()->Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}


bool CDisktop::OnSelectBurn(unsigned long xPos, int chnnNum, int maxSel, ULONGLONG & CH)
{
#ifndef __DVR_MENU__
	assert(maxSel <= chnnNum);
	//具有权限则发生,否则没有发送的必要
	if (0 == CheckAuthority(AUTH_LIVE))
	{
		return false;
	}

	//2010-02-04 10:14:00 YSW
	//具有权限则发生,否则没有发送的必要
	ULONGLONG authCH = CheckAuthority(AUTH_LIVE);
	ULONGLONG cfgCH = GetMainMonitorCfg();

	authCH &= cfgCH;

	if (0 != authCH)
	{
		m_selectChnnDlg.SetChnnCount(chnnNum , authCH, maxSel, CH);
		m_selectChnnDlg.SetMiniMap(false, -1);

		GUI::CRect rect = m_toolBar.GetWindowRect();
		GUI::CRect chnnRect = m_selectChnnDlg.GetDlgDefaultRect();
		unsigned short x = (xPos > chnnRect.Width()/2) ? (xPos - chnnRect.Width()/2) : xPos;
		unsigned short y = (rect.m_top > m_height / 2) ? (rect.m_top - chnnRect.Height()) : rect.m_bottom;

		if(DLG_OK == m_selectChnnDlg.DoModal(NULL, x, y, chnnRect.Width(), chnnRect.Height()))
		{
			CH = m_selectChnnDlg.GetSelChnn();

			int chnnMini = m_selectChnnDlg.GetSelMiniMapChnn();
			//必须选择一组才发送
			if (0 != CH)
			{
				unsigned char burnChnnInfo[64] = {0};

				for (int ch =0; ch < g_p_login_succ->productInfo.localVideoInputNum; ch++)
				{
					if ((CH>>ch) & (ULONGLONG)0x01)
					{
						burnChnnInfo[ch] = 1;
					}
				}

				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));

				m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_BURN_INFO, burnChnnInfo, sizeof(char) * g_p_login_succ->productInfo.localVideoInputNum, msgData);	
				PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
				if (CMD_REPLY_CTRL_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
				{
					//
					//获取返回值
					unsigned long burnStatus = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
					if (0 != burnStatus)
					{
						MessageBox("ERROR", "burn error");
					}
				}
				m_pMsgMan->ReleaseMsgBuff(msgData);
				return true;
			}
		}
		return false;
	}
	else
	{
		//无权限应该提示一下
		return false;
	}
#else
	return false;
#endif
}

unsigned long  CDisktop::On6X6()
{
	long windowNum = (g_p_login_succ->productInfo.videoInputNum < 36) ? g_p_login_succ->productInfo.videoInputNum : 36;
	if(OnChangeCH(m_6X6CH, VIEW_SPLIT_6X6, windowNum))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::On5X5()
{
	long windowNum = (g_p_login_succ->productInfo.videoInputNum < 25) ? g_p_login_succ->productInfo.videoInputNum : 25;
	if(OnChangeCH(m_5X5CH, VIEW_SPLIT_5X5, windowNum))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::On4X4()
{
	long windowNum = (g_p_login_succ->productInfo.videoInputNum < 16) ? g_p_login_succ->productInfo.videoInputNum : 16;
	if (OnChangeCH(m_4X4CH, VIEW_SPLIT_4X4, windowNum))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::On1A12()
{
	if (OnChangeCH(m_1A12CH, VIEW_SPLIT_1A12, 13))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::On1A7()
{
	if (OnChangeCH(m_1A7CH, VIEW_SPLIT_1A7, 8))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::On3X3()
{
	long windowNum = (g_p_login_succ->productInfo.videoInputNum < 9) ? g_p_login_succ->productInfo.videoInputNum : 9;
	if (OnChangeCH(m_3X3CH, VIEW_SPLIT_3X3, windowNum))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::On1A5()
{
	if (OnChangeCH(m_1A5CH, VIEW_SPLIT_1A5, 6))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::On2X3()
{
	long windowNum = (g_p_login_succ->productInfo.videoInputNum < 6) ? g_p_login_succ->productInfo.videoInputNum : 6;
	if (OnChangeCH(m_2X3CH, VIEW_SPLIT_2X3, windowNum))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::On2X2()
{
	long windowNum = (g_p_login_succ->productInfo.videoInputNum < 4) ? g_p_login_succ->productInfo.videoInputNum : 4;
	if (OnChangeCH(m_2X2CH, VIEW_SPLIT_2X2, windowNum))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::On1X1()
{
	if (OnChangeCH(m_1X1CH, VIEW_SPLIT_1X1, 1))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

/***************************************************************************************
函数：OnSplit()
功能：在分割模式1/4/9/16之间切换
	  只有1UTW按键板有此按键
zhl 2010-06-07
修改：
	1：切换到下一个分割模式时要检测设备是否支持该分割模式，如果不支持就回到1X1分割模式。 ZHL 2010-07-16
***************************************************************************************/
unsigned long CDisktop::OnSplit()
{
	//处于显示主菜单 则不处理
	if( m_mainMenu.IsShow() )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	ULONGLONG *pNextChnn = NULL;
	VIEW_SPLIT_MODE nextSplitMode = VIEW_SPLIT_1X1;
	unsigned long displayCount = 0;
	int videoInputNum = g_p_login_succ->productInfo.videoInputNum;
	if (!IsSupportHybridDisplay())
	{
		videoInputNum =  g_p_login_succ->productInfo.localVideoInputNum;
	}
	
	switch(m_currSplitMode)
	{
	case VIEW_SPLIT_1X1:
		{
			if ( 1 < videoInputNum)
			{
				pNextChnn = &m_2X2CH;
				nextSplitMode = VIEW_SPLIT_2X2;
				displayCount = 4;
			} 
			else
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
		}
		break;
	case VIEW_SPLIT_2X2:
		{
			if ( 4 < videoInputNum)
			{
				pNextChnn = &m_2X3CH;
				nextSplitMode = VIEW_SPLIT_2X3;
				displayCount = 6;
			}
			else
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = VIEW_SPLIT_1X1;
				displayCount = 1;
			}
		}
		break;
	case VIEW_SPLIT_2X3:
		{
			if ( 6 < videoInputNum)
			{
				pNextChnn = &m_3X3CH;
				nextSplitMode = VIEW_SPLIT_3X3;
				displayCount = (8 < g_p_login_succ->productInfo.videoInputNum) ? 9 : 8;
			}
			else
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = VIEW_SPLIT_1X1;
				displayCount = 1;
			}
		}
		break;
	case VIEW_SPLIT_3X3:
		{
			if (8 < videoInputNum)
			{
				pNextChnn = &m_4X4CH;
				nextSplitMode = VIEW_SPLIT_4X4;
				displayCount = 16;
			}
			else
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = VIEW_SPLIT_1X1;
				displayCount = 1;
			}
		}
		break;
	case VIEW_SPLIT_4X4:
		{
			if (16 < videoInputNum)
			{
				pNextChnn = &m_5X5CH;
				nextSplitMode = VIEW_SPLIT_5X5;
				displayCount = (24 < g_p_login_succ->productInfo.videoInputNum) ? 25 : 24;
			}
			else
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = VIEW_SPLIT_1X1;
				displayCount = 1;
			}
		}
		break;
	case VIEW_SPLIT_5X5:
		{
			if (24 <videoInputNum)
			{
				pNextChnn = &m_6X6CH;
				nextSplitMode = VIEW_SPLIT_6X6;
				displayCount = (32 < g_p_login_succ->productInfo.videoInputNum) ? 36 : 32;
			}
			else
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = VIEW_SPLIT_1X1;
				displayCount = 1;
			}
		}
		break;
	case VIEW_SPLIT_6X6:
		{
			if (32 < videoInputNum)
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = VIEW_SPLIT_1X1;
				displayCount = 1;
			}
			else
			{
				pNextChnn = &m_1X1CH;
				nextSplitMode = VIEW_SPLIT_1X1;
				displayCount = 1;
			}
		}
		break;
	default:
		{
			assert(false);
			pNextChnn = &m_1X1CH;
			nextSplitMode = VIEW_SPLIT_1X1;
			displayCount = 1;
		}
		break;
	}

	if (OnChangeCH(*pNextChnn, nextSplitMode, displayCount))
	{
		GetRootWnd()->Repaint();
	}
	return KEY_VIRTUAL_MSG_STOP;
}

/***************************************************************************************
函数：OnSplitEx()
功能：在分割模式16/8/4/1之间切换(OnSplit()函数的扩展，从高到低切换)
xzw 2011-05-17
***************************************************************************************/
unsigned long CDisktop::OnSplitEx()
{
	//处于显示主菜单 则不处理
	if( m_mainMenu.IsShow() )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	ULONGLONG *pNextChnn = NULL;
	VIEW_SPLIT_MODE nextSplitMode = VIEW_SPLIT_1X1;
	unsigned long displayCount = 0;

	switch(m_currSplitMode)
	{
	case VIEW_SPLIT_1X1:
		{
			return KEY_VIRTUAL_MSG_STOP;
		}
	case VIEW_SPLIT_2X2:
		{
			pNextChnn = &m_1X1CH;
			nextSplitMode = VIEW_SPLIT_1X1;
			displayCount = 1;
		}
		break;
	case VIEW_SPLIT_2X3:
		{
			pNextChnn = &m_2X2CH;
			nextSplitMode = VIEW_SPLIT_2X2;
			displayCount = 4;
		}
		break;
	case VIEW_SPLIT_3X3:
		{
			pNextChnn = &m_2X3CH;
			nextSplitMode = VIEW_SPLIT_2X3;
			displayCount = 6;
		}
		break;
	case VIEW_SPLIT_4X4:
		{
			pNextChnn = &m_3X3CH;
			nextSplitMode = VIEW_SPLIT_3X3;
			displayCount = 9;
		}
		break;
	case VIEW_SPLIT_5X5:
		{
			pNextChnn = &m_4X4CH;
			nextSplitMode = VIEW_SPLIT_4X4;
			displayCount = 16;
		}
		break;
	case VIEW_SPLIT_6X6:
		{
			pNextChnn = &m_5X5CH;
			nextSplitMode = VIEW_SPLIT_5X5;
			displayCount = 25;
		}
		break;
	default:
		{
			assert(false);
			pNextChnn = &m_1X1CH;
			nextSplitMode = VIEW_SPLIT_1X1;
			displayCount = 1;
		}
		break;
	}

	if (OnChangeCH(*pNextChnn, nextSplitMode, displayCount))
	{
		GetRootWnd()->Repaint();
	}
	return KEY_VIRTUAL_MSG_STOP;
}
/***************************************************************************************
函数：OnGroup()
功能：画面切换，在当分割模式下翻页
	  只有1UTW按键板有此按键
zhl 2010-06-07
***************************************************************************************/
unsigned long CDisktop::OnGroup()
{
	//处于显示主菜单 则不处理
	if( m_mainMenu.IsShow() )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	ULONGLONG *pCurrChnn = NULL;
	VIEW_SPLIT_MODE currSplitMode = VIEW_SPLIT_1X1;
	unsigned long displayCount = 0;

	switch(m_currSplitMode)
	{
	case VIEW_SPLIT_1X1:
		{
			pCurrChnn = &m_1X1CH;
			currSplitMode = VIEW_SPLIT_1X1;
			displayCount = 1;
		}
		break;
	case VIEW_SPLIT_2X2:
		{
			pCurrChnn = &m_2X2CH;
			currSplitMode = VIEW_SPLIT_2X2;
			displayCount = 4;
		}
		break;
	case VIEW_SPLIT_2X3:
		{
			pCurrChnn = &m_2X3CH;
			currSplitMode = VIEW_SPLIT_2X3;
			displayCount = 6;
		}
		break;
	case VIEW_SPLIT_3X3:
		{
			pCurrChnn = &m_3X3CH;
			currSplitMode = VIEW_SPLIT_3X3;
			displayCount = (8 < g_p_login_succ->productInfo.videoInputNum) ? 9 : 8;
		}
		break;
	case VIEW_SPLIT_4X4:
		{
			pCurrChnn = &m_4X4CH;
			currSplitMode = VIEW_SPLIT_4X4;
			displayCount = 16;
		}
		break;
	case VIEW_SPLIT_5X5:
		{
			pCurrChnn = &m_5X5CH;
			currSplitMode = VIEW_SPLIT_5X5;
			displayCount = (24 < g_p_login_succ->productInfo.videoInputNum) ? 25 : 24;
		}
		break;
	case VIEW_SPLIT_6X6:
		{
			pCurrChnn = &m_6X6CH;
			currSplitMode = VIEW_SPLIT_6X6;
			displayCount = (32 < g_p_login_succ->productInfo.videoInputNum) ? 36 : 32;
		}
		break;
	default:
		{
			pCurrChnn = &m_1X1CH;
			currSplitMode = VIEW_SPLIT_1X1;
			displayCount = 1;
		}
		break;
	}

	if (OnChangeCH(*pCurrChnn, currSplitMode, displayCount))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

/***************************************************************************************
函数：OnGroupEx()
功能：画面切换，在当分割模式上翻页
xzw 2011-05-17
***************************************************************************************/
unsigned long CDisktop::OnGroupEx()
{
	//处于显示主菜单 则不处理
	if( m_mainMenu.IsShow() )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	ULONGLONG *pCurrChnn = NULL;
	VIEW_SPLIT_MODE currSplitMode = VIEW_SPLIT_1X1;
	unsigned long displayCount = 0;

	switch(m_currSplitMode)
	{
	case VIEW_SPLIT_1X1:
		{
			pCurrChnn = &m_1X1CH;
			currSplitMode = VIEW_SPLIT_1X1;
			displayCount = 1;
		}
		break;
	case VIEW_SPLIT_2X2:
		{
			pCurrChnn = &m_2X2CH;
			currSplitMode = VIEW_SPLIT_2X2;
			displayCount = 4;
		}
		break;
	case VIEW_SPLIT_2X3:
		{
			pCurrChnn = &m_2X3CH;
			currSplitMode = VIEW_SPLIT_2X3;
			displayCount = 6;
		}
		break;
	case VIEW_SPLIT_3X3:
		{
			pCurrChnn = &m_3X3CH;
			currSplitMode = VIEW_SPLIT_3X3;
			displayCount = (8 < g_p_login_succ->productInfo.videoInputNum) ? 9 : 8;
		}
		break;
	case VIEW_SPLIT_4X4:
		{
			pCurrChnn = &m_4X4CH;
			currSplitMode = VIEW_SPLIT_4X4;
			displayCount = 16;
		}
		break;
	case VIEW_SPLIT_5X5:
		{
			pCurrChnn = &m_5X5CH;
			currSplitMode = VIEW_SPLIT_5X5;
			displayCount = (24 < g_p_login_succ->productInfo.videoInputNum) ? 25 : 24;
		}
		break;
	case VIEW_SPLIT_6X6:
		{
			pCurrChnn = &m_6X6CH;
			currSplitMode = VIEW_SPLIT_6X6;
			displayCount = (32 < g_p_login_succ->productInfo.videoInputNum) ? 36 : 32;
		}
		break;
	default:
		{
			pCurrChnn = &m_1X1CH;
			currSplitMode = VIEW_SPLIT_1X1;
			displayCount = 1;
		}
		break;
	}

	if (OnChangeCHEx(*pCurrChnn, currSplitMode, displayCount))
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}
bool CDisktop::OnSelectCH(unsigned long xPos, int chnnNum, int maxSel, ULONGLONG & CH,unsigned long splitMode)
{
#ifndef __DVR_MENU__
	//如果通道数目小于或者等于要选择的最大数目,则不需要弹出选择对话框了.
	if (chnnNum <= maxSel)
	{
		//2010-02-04 10:14:00 YSW
		//具有权限则发生,否则没有发送的必要
		if (0 != CheckAuthority(AUTH_LIVE))
		{
			ChangeDisplayByCH(CH, splitMode);
			m_currSplitMode = splitMode;
			return true;
		}
		else
		{
			return false;
		}		
	}

	assert(maxSel <= chnnNum);

	//2010-02-04 10:14:00 YSW
	//具有权限则发生,否则没有发送的必要
	ULONGLONG authCH = CheckAuthority(AUTH_LIVE);
	ULONGLONG cfgCH = GetMainMonitorCfg();

	authCH &= cfgCH;

	if (0 != authCH)
	{
		m_selectChnnDlg.SetChnnCount(chnnNum , authCH, maxSel, CH);
		m_selectChnnDlg.SetAllChnnEnable(false);
		if(maxSel == 1 && IsSupportPip())
		{
			m_selectChnnDlg.SetMiniMap(true, -1);
		}
		else
		{
			m_selectChnnDlg.SetMiniMap(false, -1);
		}

		GUI::CRect rect = m_toolBar.GetWindowRect();
		GUI::CRect chnnRect = m_selectChnnDlg.GetDlgDefaultRect();
		unsigned short x = (xPos > chnnRect.Width()/2) ? (xPos - chnnRect.Width()/2) : xPos;
		unsigned short y = (rect.m_top > m_height / 2) ? (rect.m_top - chnnRect.Height()) : rect.m_bottom;
		//if(rect.m_top > m_height/2 )
		//{
		//	y = rect.m_top - chnnRect.Height();
		//}
		//else
		//{
		//	y = rect.m_bottom;
		//}

		if( DLG_OK == m_selectChnnDlg.DoModal( NULL, x, y, chnnRect.Width(), chnnRect.Height()) )
		{
			CH = m_selectChnnDlg.GetSelChnn();

			int chnnMini = m_selectChnnDlg.GetSelMiniMapChnn();
			//必须选择一组才发送
			if (0 != CH)
			{
				ChangeDisplayByCH(CH, splitMode, chnnMini);

				m_currSplitMode = splitMode;
				return true;
			}
			
		}

		return false;
	}
	else
	{
		//无权限应该提示一下

		return false;
	}
#else
	return false;
#endif

}

bool CDisktop::OnChangeCH(ULONGLONG & CH, unsigned long splitMode, unsigned long n, bool bUseSubMask)
{
	ULONGLONG effectCH = 0;
	int videoInputNum = 0, localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	IsSupportCH(effectCH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	videoInputNum = localVideoInputNum + netVideoInputNum;

	if (n < videoInputNum)
	{
		if (bUseSubMask /*&& m_currSplitMode == splitMode*/)
		{
			ULONGLONG authCH = CheckAuthority(AUTH_LIVE);
			ULONGLONG cfgCH = GetMainMonitorCfg();

			authCH &= cfgCH;

			unsigned long i = 0, num = videoInputNum;
			ULONGLONG subMask = 0;
			switch(splitMode)
			{
			case VIEW_SPLIT_1X1:
				{
					num = videoInputNum;
					subMask = 0x01;
				}
				break;
			case VIEW_SPLIT_2X2:
				{
					num = videoInputNum / 4;
					subMask = 0xf;
				}
				break;
			case VIEW_SPLIT_2X3:
				{
					num = (videoInputNum + 5) / 6;
					subMask = 0x3f;
				}
				break;
			case VIEW_SPLIT_1A5:
				{
					num = (videoInputNum + 5) / 6;
					subMask = 0x003f;
				}
				break;
			case VIEW_SPLIT_3X3:
				{
					num = (videoInputNum + 8) / 9;
					subMask = 0x1ff;
				}
				break;
			case VIEW_SPLIT_1A7:
				{
					num = (videoInputNum + 7) / 8;
					subMask = 0x00ff;
				}
				break;
			case VIEW_SPLIT_1A12:
				{
					num = (videoInputNum + 12) / 13;
					subMask = 0x1fff;
				}
				break;
			case VIEW_SPLIT_4X4:
				{
					num = (videoInputNum + 15) / 16;
					subMask = 0xffff;
				}
				break;
			case VIEW_SPLIT_5X5:
				{
					num = (videoInputNum + 24) / 25;
					subMask = 0x1ffffff;
				}
				break;
			case VIEW_SPLIT_6X6:
				{
					num = (videoInputNum + 35) / 36;
					subMask = ((ULONGLONG)0xf) << 32  | 0xffffffff;
				}
				break;
			default:
				{
					return false;
				}
				break;
			}

			while ((0 != authCH) && (i < num))
			{
				CH = CH_ROL(CH, ALL_CHNN_MASK(videoInputNum), subMask, n);

				if (CH & authCH)
				{
					ChangeDisplayByCH(CH, splitMode);
					m_currSplitMode = splitMode;
					break;
				}

				++ i;
			}
		}
		else
		{
			ULONGLONG authCH = CheckAuthority(AUTH_LIVE);
			ULONGLONG cfgCH = GetMainMonitorCfg();

			authCH &= cfgCH;

			if(CH & authCH)
			{
				ChangeDisplayByCH(CH, splitMode);

				m_currSplitMode = splitMode;
			}
		}

		return true;
	}
	else if (n >= videoInputNum)
	{
		ULONGLONG authCH = CheckAuthority(AUTH_LIVE);
		ULONGLONG cfgCH = GetMainMonitorCfg();

		authCH &= cfgCH;

		if(CH & authCH)
		{
			ChangeDisplayByCH(CH, splitMode);

			m_currSplitMode = splitMode;
		}
		return true;
	}
	else
	{
		return false;
	}
}

/*
在当前分割模式下切换画面到上一组
*/
bool CDisktop::OnChangeCHEx(ULONGLONG & CH, unsigned long splitMode, unsigned long n)
{
	ULONGLONG effectCH = 0;
	int videoInputNum = 0, localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	IsSupportCH(effectCH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	videoInputNum = localVideoInputNum + netVideoInputNum;

	if (n < videoInputNum)
	{
		if (m_currSplitMode == splitMode)
		{
			ULONGLONG authCH = CheckAuthority(AUTH_LIVE);
			ULONGLONG cfgCH = GetMainMonitorCfg();

			authCH &= cfgCH;

			unsigned long i = 0, num = videoInputNum;
			ULONGLONG subMask = 0;
			switch(splitMode)
			{
			case VIEW_SPLIT_1X1:
				{
					num = videoInputNum;
					subMask = 0x01;
					subMask <<= (videoInputNum - 1);
				}
				break;
			case VIEW_SPLIT_2X2:
				{
					num = videoInputNum / 4;
					subMask = 0xf;
					if(!(videoInputNum % 4))
					{
						subMask <<= (videoInputNum - 4);
					}
					else
					{
						subMask <<= (videoInputNum - videoInputNum % 4);
					}
				}
				break;
			case VIEW_SPLIT_2X3:
				{
					num = (videoInputNum + 5) / 6;
					subMask = 0x3f;
					if(!(videoInputNum % 6 ))
					{
						subMask <<= (videoInputNum - 6) ;
					}
					else
					{
						subMask <<= (videoInputNum - videoInputNum % 6);
					}
				}
				break;
			case VIEW_SPLIT_1A5:
				{
					num = (videoInputNum + 5) / 6;
					subMask = 0x3f;
					if(!(videoInputNum % 6 ))
					{
						subMask <<= (videoInputNum - 6) ;
					}
					else
					{
						subMask <<= (videoInputNum - videoInputNum % 6);
					}
				}
				break;
			case VIEW_SPLIT_1A7:
				{
					num = (videoInputNum + 7) / 8;
					subMask = 0xff;
					if(!(videoInputNum % 8))
					{
						subMask <<= (videoInputNum - 8);
					}
					else
					{
						subMask <<= (videoInputNum - videoInputNum % 8);
					}
				}
				break;
			case VIEW_SPLIT_3X3:
				{
					num = (videoInputNum + 8) / 9;
					subMask = 0x1ff;
					if(!(videoInputNum % 9))
					{
						subMask <<= (videoInputNum - 9);
					}
					else
					{
						subMask <<= (videoInputNum - videoInputNum % 9);
					}
				}
				break;
			case VIEW_SPLIT_1A12:
				{
					num = (videoInputNum + 12) / 13;
					subMask = 0x1fff;
					if(!(videoInputNum % 13))
					{
						subMask <<= (videoInputNum - 13);
					}
					else
					{
						subMask <<= (videoInputNum - videoInputNum % 13);
					}
				}
				break;
			case VIEW_SPLIT_4X4:
				{
					num = (videoInputNum + 15) / 16;
					subMask = 0xffff;
					if(!(videoInputNum % 16))
					{
						subMask <<= (videoInputNum - 16);
					}
					else
					{
						subMask <<= (videoInputNum - videoInputNum % 16);
					}
				}
				break;
			case VIEW_SPLIT_5X5:
				{
					num = (videoInputNum + 24) / 25;
					subMask = 0x1ffffff;
					if(!(videoInputNum % 25))
					{
						subMask <<= (videoInputNum - 25);
					}
					else
					{
						subMask <<= (videoInputNum - videoInputNum % 25);
					}
				}
				break;
			case VIEW_SPLIT_6X6:
				{
					num = (videoInputNum + 35) / 36;
					subMask = 0x0f << 32 + 0xffffffff;
					if(!(videoInputNum % 36))
					{
						subMask <<= (videoInputNum - 36);
					}
					else
					{
						subMask <<= (videoInputNum - videoInputNum % 36);
					}
				}
				break;
			default:
				{
					return false;
				}
				break;
			}

			while ((0 != authCH) && (i < num))
			{

				CH = CH_LOL(CH, ALL_CHNN_MASK(videoInputNum), subMask, n);
				
				if (CH & authCH)
				{
					ChangeDisplayByCH(CH, splitMode);
					break;
				}

				++ i;
			}
		}
		else
		{
			ChangeDisplayByCH(CH, splitMode);
			m_currSplitMode = splitMode;
		}

		return true;
	}
	else if (n == videoInputNum)
	{
		ULONGLONG authCH = CheckAuthority(AUTH_LIVE);

		ChangeDisplayByCH(CH, splitMode);

		return true;
	}
	else
	{
		return false;
	}
}

/*
飞梭右旋时在当前分割模式下切换到下一组,ZHL 2010-06-23
*/
unsigned long CDisktop::OnFlyRight(unsigned long keyID)
{
	//处于显示主菜单 则不处理
	if( m_mainMenu.IsShow() )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if(m_keyFlyRightID < keyID )
	{
		ULONGLONG *pCH = NULL;
		unsigned long n = 0;

		switch(m_currSplitMode)
		{
		case VIEW_SPLIT_1X1:
			{
				pCH = &m_1X1CH;
				n = 1;
			}
			break;
		case VIEW_SPLIT_2X2:
			{
				pCH = &m_2X2CH;
				n = 4;
			}
			break;		
		case VIEW_SPLIT_2X3:
			{
				pCH = &m_2X3CH;
				n = 6;
			}
			break;
		case VIEW_SPLIT_1A5:
			{
				pCH = &m_1A5CH;
				n = 6;
			}
			break;
		case VIEW_SPLIT_1A7:
			{
				pCH = &m_1A7CH;
				n = 8;
			}
			break;
		case VIEW_SPLIT_3X3:
			{
				pCH = &m_3X3CH;
				n = (8 < g_p_login_succ->productInfo.videoInputNum) ? 9 : 8;
			}
			break;
		case VIEW_SPLIT_1A12:
			{
				pCH = &m_1A12CH;
				n = 13;
			}
			break;
		case VIEW_SPLIT_4X4:
			{
				pCH = &m_4X4CH;
				n = 16;
			}
			break;
		case VIEW_SPLIT_5X5:
			{
				pCH = &m_5X5CH;
				n = (24 < g_p_login_succ->productInfo.videoInputNum) ? 25 : 24;
			}
			break;
		case VIEW_SPLIT_6X6:
			{
				pCH = &m_6X6CH;
				n = (32 < g_p_login_succ->productInfo.videoInputNum) ? 36 : 32;
			}
			break;
		default:
			assert(false);
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
			break;
		}

		if ( OnChangeCH(*pCH, m_currSplitMode,n) ) 
		{
			GetRootWnd()->Repaint();
		}
	}
	m_keyFlyRightID = keyID;

	return KEY_VIRTUAL_MSG_STOP;
}

/*
飞梭左旋时在当前分割模式下切换到上一组， ZHL 2010-06-23
*/
unsigned long CDisktop::OnFlyLeft(unsigned long keyID)
{
	//处于显示主菜单 则不处理
	if( m_mainMenu.IsShow() )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if(m_keyFlyLeftID < keyID )
	{
		ULONGLONG *pCH = NULL;
		unsigned long n = 0;

		switch(m_currSplitMode)
		{
		case VIEW_SPLIT_1X1:
			{
				pCH = &m_1X1CH;
				n = 1;
			}
			break;
		case VIEW_SPLIT_2X2:
			{
				pCH = &m_2X2CH;
				n = 4;
			}
			break;
		case VIEW_SPLIT_2X3:
			{
				pCH = &m_2X3CH;
				n = 6;
			}
			break;
		case VIEW_SPLIT_1A5:
			{
				pCH = &m_1A5CH;
				n = 6;
			}
			break;
		case VIEW_SPLIT_1A7:
			{
				pCH = &m_1A7CH;
				n = 8;
			}
			break;
		case VIEW_SPLIT_3X3:
			{
				pCH = &m_3X3CH;
				n = (8 < g_p_login_succ->productInfo.videoInputNum) ? 9 : 8;
			}
			break;
		case VIEW_SPLIT_1A12:
			{
				pCH = &m_1A12CH;
				n = 13;
			}
			break;
		case VIEW_SPLIT_4X4:
			{
				pCH = &m_4X4CH;
				n = 16;
			}
			break;
		case VIEW_SPLIT_5X5:
			{
				pCH = &m_5X5CH;
				n = (24 < g_p_login_succ->productInfo.videoInputNum) ? 25 : 24;
			}
			break;
		case VIEW_SPLIT_6X6:
			{
				pCH = &m_6X6CH;
				n = (32 < g_p_login_succ->productInfo.videoInputNum) ? 36 : 32;
			}
			break;
		default:
			assert(false);
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
			break;
		}

		if ( OnChangeCHEx(*pCH, m_currSplitMode,n) )
		{
			GetRootWnd()->Repaint();
		}
	}
	m_keyFlyLeftID = keyID;

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnFlyMedium()
{
	m_keyFlyRightID = 0;
	m_keyFlyLeftID = 0;

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::OnDoubleClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//不管是否具备权限，只要是点击工具条都不处理
	//处于显示ToolBar 但点击位置在ToolBar上,则不处理
#ifndef __DVR_MENU__
	m_toolBar.OnDBClick(xPos, yPos);
	if(m_toolBar.IsShow() && m_toolBar.PtInToolbar(xPos, yPos))
	{
		return KEY_VIRTUAL_MSG_STOP; 
	}
#endif
	//printf("1 %s %d\n", __FILE__, __LINE__);
	//2010-03-05 15:30:00 YSW
	//在此检查权限设置
	if (0 == CheckAuthority(AUTH_LIVE))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	//printf("2 %s %d\n", __FILE__, __LINE__);

	//截获所有的鼠标点击事件,根据现场的状态来区分是否处理

	//处于显示主菜单 则不处理
	if( m_mainMenu.IsShow() )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	//printf("3 %s %d\n", __FILE__, __LINE__);

	//鼠标的单击和按键板的确定键都到这里来。
	
	ULONGLONG CH = 0;	
	
	if ((m_width <= xPos) && (m_height <= yPos))
	{
		//按键板的输入
		
	}
	else
	{
		//printf("4 %s %d\n", __FILE__, __LINE__);
		//鼠标的单击事件，
		if (xPos < m_rect.m_left)
		{
			xPos = m_rect.m_left;
		}
		else if (xPos > m_rect.m_left + m_rect.Width())
		{
			xPos = m_rect.m_left + m_rect.Width();
		}

		if (yPos < m_rect.m_top)
		{
			yPos = m_rect.m_top;
		}
		else if (yPos > m_rect.m_top + m_rect.Height())
		{
			yPos = m_rect.m_top + m_rect.Height();
		}

		bool bValidChnn = false;
		unsigned long chnn = INVALID_INDEX(unsigned long);
		unsigned long hitsChnn[MAX_CHNN_NUM] = {0};
		int hits = CDisplayMan::Instance()->HitChnn(xPos, yPos, hitsChnn, MAX_CHNN_NUM);
		for (int i = 0; i < hits; i++)
		{
			if( (DISPLAY_SIGNAL_ANALOG == DISPLAY_GET_SIGNAL(hitsChnn[i])) &&
				(DISPLAY_INVALID != DISPLAY_GET_CHNN(hitsChnn[i])))
			{
				bValidChnn = true;
				chnn = DISPLAY_GET_CHNN(hitsChnn[i]);
				break;
			}
			else if ((DISPLAY_SIGNAL_NET == DISPLAY_GET_SIGNAL(hitsChnn[i])) &&
				(DISPLAY_INVALID != DISPLAY_GET_CHNN(hitsChnn[i])))
			{
				chnn = DISPLAY_GET_CHNN(hitsChnn[i]) + g_p_login_succ->productInfo.localVideoInputNum;
			}
		}
//		assert(chnn != INVALID_INDEX(unsigned long));
/*
		int netDevNum = 0;
		ND_DEV_INFO	*pNetDevInfo = NULL;
		if (0 < g_p_login_succ->productInfo.netVideoInputNum)
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_NET_DEVICE_INFO, NULL, 0, msgData);
			netDevNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(ND_DEV_INFO);
			if (netDevNum > 0)
			{
				pNetDevInfo = new ND_DEV_INFO[netDevNum];
				memcpy(pNetDevInfo, (ND_DEV_INFO *)(msgData.pData + sizeof(PACKCMD)), netDevNum*sizeof(ND_DEV_INFO));	
			}
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		}
*/
		ULONGLONG CH = 0;
		int localVideoInputNum = 0, netVideoInputNum = 0;
		int maxChnn = 0, effectChnn = 0;

		IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

		if ((0x01 << chnn) & CH)
		{
			bValidChnn = true;
		}

		if (bValidChnn)
		{
			printf("%s %d, OK, Valid Net Channel = %d\n", __FILE__, __LINE__, chnn);
			ChangeDisplayByPt(xPos, yPos);

			GetRootWnd()->Repaint();
		}
		else
		{
			printf("%s %d, Fail, Invalid Net Channel = %d\n", __FILE__, __LINE__, chnn);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::OnMainMenu()
{
	if (!m_mainMenu.IsShow() && (0 != CheckAuthority(AUTH_BASIC)))
	{
		HideSnapTip();

		m_mainMenu.Show(true, false);

#ifndef __DVR_MENU__
		if (m_toolBar.IsShow())
		{
			m_toolBar.SetShowToolbar(false);
			m_toolBar.Show(false, false);
		}
#endif
	}	
	
	GetRootWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::OnSearch()
{
	ULONGLONG auth = CheckAuthority(AUTH_PLAYBACK);
	if (0 != auth)
	{
		DVR_DISK_OWNER_LIST diskOwnerList;
		GetDiskOwer(diskOwnerList);
		if (diskOwnerList.IsEmpty())
		{
			m_searchMan.SetInitialInfo(NET_DISK_OWNED_BY_THIS, g_p_login_succ->productInfo.videoInputNum);
			m_searchMan.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
			diskOwnerList.RemoveAll();

			GetRootWnd()->Repaint();

			return KEY_VIRTUAL_MSG_STOP;
		}

		if ((1 == diskOwnerList.GetCount()) && (NET_DISK_OWNED_BY_THIS == diskOwnerList.GetHead().dvrIndex)) 
		{
			assert(g_p_login_succ->productInfo.videoInputNum == diskOwnerList.GetHead().dvrPara);
			m_searchMan.SetInitialInfo(NET_DISK_OWNED_BY_THIS, g_p_login_succ->productInfo.videoInputNum);
			m_searchMan.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
			diskOwnerList.RemoveAll();

			GetRootWnd()->Repaint();

			return KEY_VIRTUAL_MSG_STOP;
		}

		{
			CSelectDiskDlg selectDiskDlg;
			selectDiskDlg.SetInitialInfo(&diskOwnerList);
			if(DLG_OK == selectDiskDlg.DoModal(NULL, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false))
			{
				DVR_DISK_OWNER diskOwner;
				memset(&diskOwner, 0, sizeof(DVR_DISK_OWNER));
				selectDiskDlg.GetSelDisk(diskOwner);
				m_searchMan.SetInitialInfo(diskOwner.dvrIndex, diskOwner.dvrPara);
				m_searchMan.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
			}
			diskOwnerList.RemoveAll();

			GetRootWnd()->Repaint();
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	else
	{
		//提示以后返回
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_77), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;	
	}
}

unsigned long  CDisktop::OnBackup()
{
	ULONGLONG auth = CheckAuthority(AUTH_BACKUP);
	if (0 != auth)
	{
		DVR_DISK_OWNER_LIST diskOwnerList;
		GetDiskOwer(diskOwnerList);
		if (diskOwnerList.IsEmpty())
		{
			m_backupDlg.SetInitialInfo(g_p_login_succ->productInfo.videoInputNum, NET_DISK_OWNED_BY_THIS);
			m_backupDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
			diskOwnerList.RemoveAll();

			GetRootWnd()->Repaint();

			return KEY_VIRTUAL_MSG_STOP;
		}

		if ((1 == diskOwnerList.GetCount()) && (NET_DISK_OWNED_BY_THIS == diskOwnerList.GetHead().dvrIndex)) 
		{
			assert(g_p_login_succ->productInfo.videoInputNum == diskOwnerList.GetHead().dvrPara);
			m_backupDlg.SetInitialInfo(g_p_login_succ->productInfo.videoInputNum, NET_DISK_OWNED_BY_THIS);
			m_backupDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
			diskOwnerList.RemoveAll();

			GetRootWnd()->Repaint();

			return KEY_VIRTUAL_MSG_STOP;
		}

		CSelectDiskDlg selectDiskDlg;
		selectDiskDlg.SetInitialInfo(&diskOwnerList);
		if(DLG_OK == selectDiskDlg.DoModal(NULL, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false))
		{
			DVR_DISK_OWNER diskOwner;
			memset(&diskOwner, 0, sizeof(DVR_DISK_OWNER));
			selectDiskDlg.GetSelDisk(diskOwner);
			m_backupDlg.SetInitialInfo(g_p_login_succ->productInfo.videoInputNum, diskOwner.dvrIndex);
			m_backupDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
		}
		diskOwnerList.RemoveAll();	

		GetRootWnd()->Repaint();

		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_78), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;	
	}
}

unsigned long  CDisktop::OnInfo()
{
	m_infoMan.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	GetRootWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnLog()
{
	ULONGLONG auth = CheckAuthority(AUTH_LIVE, true);
	if (0 != auth)
	{
		m_logMan.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	GetRootWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long  CDisktop::OnDwell()
{
#ifdef __DVR_BASIC__
	ULONGLONG auth = CheckAuthority(AUTH_BASIC);
	if (0 != auth)
	{
		if(0 != m_pWndStartupInfo->DoDwell)
		{
			m_pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DODWELL_STOP,NULL, 0);

			m_pWndStartupInfo->DoDwell = 0x0;

#ifdef __DVR_MENU__
			m_toolBarMenu.EnableDwell(false);
#else
			m_toolBar.EnableDwell(false);
#endif
		}
		else
		{
			m_pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DODWELL_START,NULL, 0);

			m_pWndStartupInfo->DoDwell = 0x1;

#ifdef __DVR_MENU__
			m_toolBarMenu.EnableDwell(true);
#else
			m_toolBar.EnableDwell(true);
#endif
		}

		TrySaveStartupInfo(g_p_wnd_startup_info, m_pWndStartupInfo);
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	GetRootWnd()->Repaint();
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::OnZoom()
{
	//先检查权限
	ULONGLONG auth = CheckAuthority(AUTH_LIVE, true);
	ULONGLONG cm = GetMainMonitorCfg();
	auth &= cm;

	if (0 != auth)
	{
		HideSnapTip();

		//要考虑到：回放电子放大的情况。
		NET_DISPLAY_INFO netDisp;
		memset(&netDisp, 0, sizeof(NET_DISPLAY_INFO));

		if(GetCurDisplay(netDisp))
		{
			for(int i = 0; i < netDisp.data_len; i++)
			{
				if(DISPLAY_LAYER_MINI == DISPLAY_GET_LAYER(netDisp.data[i]))
				{
					GetRootWnd()->Repaint();
					return KEY_VIRTUAL_MSG_STOP;
				}
			}
		}		
		memset(&netDisp, 0, sizeof(NET_DISPLAY_INFO));

#ifdef __DVR_MENU__
		unsigned long xPos = 0, yPos = 0;
		m_toolBarMenu.GetPtPos(xPos, yPos);
		if(GetCurDisplay(netDisp, xPos, yPos, true))
#else
		if(GetCurDisplay(netDisp))
#endif
		{
			if((netDisp.split_mode[DISPLAY_LAYER_ANALOG] == VIEW_SPLIT_1X1) || (netDisp.split_mode[DISPLAY_LAYER_NET] == VIEW_SPLIT_1X1))
			{
				unsigned char chnn = 0;
				for(int i = 0; i < netDisp.data_len; i++)
				{
					if((DISPLAY_SIGNAL_ANALOG == DISPLAY_GET_SIGNAL(netDisp.data[i])) || (DISPLAY_SIGNAL_NET == DISPLAY_GET_SIGNAL(netDisp.data[i])))
					{
						if(DISPLAY_SIGNAL_ANALOG == DISPLAY_GET_SIGNAL(netDisp.data[i]))
						{
							chnn = DISPLAY_GET_CHNN(netDisp.data[i]);
						}
						else
						{
							chnn = DISPLAY_GET_CHNN(netDisp.data[i]);
							chnn += CProduct::Instance()->LocalVideoInputNum();
						}

						NET_DISPLAY_INFO disp;
						memset(&disp, 0 , sizeof(NET_DISPLAY_INFO));
						disp.type = NET_DISPLAY_FREEZE;//冻结画面

						//
						MESSAGE_DATA msgData;
						memset(&msgData, 0, sizeof(MESSAGE_DATA));
						CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&disp, sizeof(NET_DISPLAY_INFO), msgData);
						CMessageMan::Instance()->ReleaseMsgBuff(msgData);

						m_zoomDlg.SetCHInfo(chnn, auth);
						m_zoomDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

						//解冻窗口
						memset(&msgData, 0, sizeof(MESSAGE_DATA));
						CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0, msgData);
						CMessageMan::Instance()->ReleaseMsgBuff(msgData);

						break;
					}
				}
			}
			else
			{
#ifdef __DVR_MENU__
				unsigned char hitChnn = INVALID_INDEX(unsigned char);
				if(netDisp.data_len > 0)
				{
					for(int i = 0; i < netDisp.data_len; i++)
					{
						if(DISPLAY_SIGNAL_ANALOG == DISPLAY_GET_SIGNAL(netDisp.data[i]))
						{
							hitChnn = DISPLAY_GET_CHNN(netDisp.data[i]);
							break;
						}
						else if(DISPLAY_SIGNAL_NET == DISPLAY_GET_SIGNAL(netDisp.data[i]))
						{
							hitChnn = DISPLAY_GET_CHNN(netDisp.data[i]);
							hitChnn += g_p_login_succ->productInfo.localVideoInputNum;
							break;
						}
					}
				}

				if(INVALID_INDEX(unsigned char) != hitChnn)
				{
					NET_DISPLAY_INFO disp;
					memset(&disp, 0, sizeof(NET_DISPLAY_INFO));
					disp.type = NET_DISPLAY_CHANGE_DATA_PACK;
					disp.time_out = -1;
					disp.split_mode[DISPLAY_LAYER_ANALOG] = VIEW_SPLIT_1X1;
					disp.split_mode[DISPLAY_LAYER_NET] = VIEW_SPLIT_1X1;
					if (hitChnn >= g_p_login_succ->productInfo.localVideoInputNum)
					{
						disp.data[0] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, 0, hitChnn - g_p_login_succ->productInfo.localVideoInputNum);
					}
					else
					{
						disp.data[0] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, 0, hitChnn);
					}
					disp.data_len = 1;

					//
					MESSAGE_DATA msgData;
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&disp, sizeof(NET_DISPLAY_INFO), msgData);
					CMessageMan::Instance()->ReleaseMsgBuff(msgData);

					GetRootWnd()->Repaint();

					m_zoomDlg.SetCHInfo(hitChnn, auth);
					m_zoomDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

					//解冻窗口
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0, msgData);
					CMessageMan::Instance()->ReleaseMsgBuff(msgData);
				}
#endif
			}
		}
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	GetRootWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnAudio()
{
	ULONGLONG auth = 0;
//#if defined (__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__)
	auth = CheckAuthority(AUTH_SYS_SETUP);
//#else
//	auth = CheckAuthority(AUTH_BASIC);
//#endif
	if (0 != auth)
	{
		if ((0 == g_p_login_succ->productInfo.audioInputNum) && (0 == g_p_login_succ->productInfo.netVideoInputNum))
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_81), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_CANCEL | DEFAULT_ICON_ERROR );//不支持音频功能
		}
		else
		{
			HideSnapTip();

#ifdef __DVR_MENU__
			unsigned long x = (m_rect.Width() - CAudioControlDlg::GetWidth()) / 2;
			unsigned long y = (m_rect.Height() - CAudioControlDlg::GetHeight()) / 2;
			m_audioControl.DoModal( NULL , x, y, CAudioControlDlg::GetWidth(), CAudioControlDlg::GetHeight() );
			m_toolBarMenu.EnableAudio(!m_audioControl.isMute());
#else
			GUI::CRect rcTool = m_toolBar.GetWindowRect();
			GUI::CRect rect = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_AUDIO);
			unsigned long x = rect.m_left + rect.Width()/2;
			unsigned long y = 0;
			if(rect.m_top > m_height/2)
			{
				y = rcTool.m_top - CAudioControlDlg::GetHeight();
			}
			else
			{
				y = rcTool.m_bottom;
			}
			//GetRootWnd()->Repaint();
			m_audioControl.DoModal( NULL , CAudioControlDlg::GetX(x), y, CAudioControlDlg::GetWidth(), CAudioControlDlg::GetHeight() );

			m_toolBar.EnableAudio(!m_audioControl.isMute());
#endif
		}
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	GetRootWnd()->Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::OnPTZ()
{
#if defined(__TW01_RILI__)
	return KEY_VIRTUAL_MSG_STOP;
#endif
#ifdef __DVR_ULTIMATE__
	if (0 == CProduct::Instance()->LocalVideoInputNum())
	{
//		printf("%s %d, NetDevice OnPTZ is invalid.\n", __FILE__, __LINE__);
//		return KEY_VIRTUAL_MSG_STOP;
	}
#endif
	//printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
	ULONGLONG auth = CheckAuthority(AUTH_PTZ_CTRL);

	//printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
	GetCurDisplay(m_displayinfo);
	//printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
	ULONGLONG CH = 0;
	long localChannelNum = CProduct::Instance()->LocalVideoInputNum();
	for (int i = 0; i < m_displayinfo.data_len; i++)
	{
		if( DISPLAY_SIGNAL_ANALOG == DISPLAY_GET_SIGNAL(m_displayinfo.data[i] ) )
		{
			CH |= (static_cast<ULONGLONG>(0x1) << DISPLAY_GET_CHNN(m_displayinfo.data[i]));
		}
		else
		{
			CH |= (static_cast<ULONGLONG>(0x1) << (DISPLAY_GET_CHNN(m_displayinfo.data[i])+localChannelNum));
		}
	}

	ULONGLONG tempCH;
	IsSupportCH(tempCH);

	CH &= tempCH;

	//printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
	//有控制通道权限，并且当前分割画面中至少有一路的PTZ可以控制才允许弹出PTZ控制界面
	if ((0 != auth) && (CH & auth))
	{
		//printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
		HideSnapTip();
		//printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);

		// * 以锁定模式发送请求,否则会受到跳台等其他影响
		memset(&m_displayinfo, 0, sizeof(NET_DISPLAY_INFO));
		m_displayinfo.type = NET_DISPLAY_FREEZE;

		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&m_displayinfo, sizeof(NET_DISPLAY_INFO), msgData);
		//printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
		
		NET_DISPLAY_INFO* pRetDisp = reinterpret_cast<NET_DISPLAY_INFO *>(msgData.pData + sizeof(PACKCMD));
		memcpy(&m_displayinfo, pRetDisp, sizeof(NET_DISPLAY_INFO));
		m_pMsgMan->ReleaseMsgBuff(msgData);

		//
		m_ptzLiveCtrl.SetChnn(CH & auth);
		m_ptzViewDlg.SetCanCtrlChnn(CH & auth);
		m_ptzViewDlg.SetCtrlDlg(&m_ptzLiveCtrl, CPTZLiveCtrl::WIDTH, CPTZLiveCtrl::HEIGHT, CPTZViewDlg::CURR_PTZCTRLDLG_LIVE);
		//printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
		m_ptzViewDlg.DoModal(NULL, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
		
		//恢复画面
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0, msgData);
		m_pMsgMan->ReleaseMsgBuff(msgData);

		CMessageMan::Instance()->SetCurrPTZChnn(INVALID_INDEX(unsigned char));
		GetRootWnd()->Repaint();
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_82), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::OnRec()
{
	ULONGLONG auth = CheckAuthority(AUTH_RECORD);
	if (0 != auth)
	{
		m_pMsgMan->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_MANUAL_REC, (unsigned char*)&auth, sizeof(ULONGLONG));
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		GetRootWnd()->Repaint();
		return KEY_VIRTUAL_MSG_STOP;
	}
}

unsigned long CDisktop::OnPlay()
{
	ULONGLONG auth = CheckAuthority(AUTH_PLAYBACK);
	unsigned char channelNum = g_p_login_succ->productInfo.videoInputNum;
	unsigned char maxVideoOutNum = g_p_login_succ->productInfo.videoOuputNum;
	unsigned char localChnnNum =  g_p_login_succ->productInfo.localVideoInputNum;
	unsigned char IpcMaxChnnNum = CProduct::Instance()->IPCMaxVideoOutNum();
	LONGLONG chnnBits = 0; 
	LONGLONG chnnBitsTemp = 0;
	int numTemp = 0;
	chnnBits = ALL_CHNN_MASK(channelNum);
	chnnBits &= auth;


	for (int index = 0; (index < localChnnNum)&&(numTemp < maxVideoOutNum); index++)
	{
		if (chnnBits & (((LONGLONG)0x01) << index))
		{
			chnnBitsTemp |= (((LONGLONG)0x01) << index);
			numTemp++;	
		}
	}
	if(numTemp ==0)
	{
		for (int index = localChnnNum; (index < channelNum)&&(numTemp < IpcMaxChnnNum); index++)
		{
			if (chnnBits & (((LONGLONG)0x01) << index))
			{
				chnnBitsTemp |= (((LONGLONG)0x01) << index);
				numTemp++;	
			}
		}
	}
	chnnBits = chnnBitsTemp;
	
	if (0 != chnnBits)
	{
		HideSnapTip();

		unsigned long curTime = GetCurrTime32();
		unsigned long startTime = curTime - m_selTime;
		unsigned long endTime = GetEndTime32OfDate(startTime);
			
		CPlaybackDlg playbackDlg;	
		playbackDlg.SetPlayInfo(startTime, endTime, startTime, chnnBits, channelNum, maxVideoOutNum, true, NET_DISK_OWNED_BY_THIS);
		playbackDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

		return KEY_VIRTUAL_MSG_STOP;	
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_77), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		GetRootWnd()->Repaint();
		return KEY_VIRTUAL_MSG_STOP;	
	}
}

DLG_END_VALUE CDisktop::PopupLiveColorDlg( int x, int y, int cx, int cy, int chnn )
{
	CMessageMan *pMsgMan = CMessageMan::Instance();
	DLG_END_VALUE retVal;
	
	//切换到相关通道的大画面
	MESSAGE_DATA msgData;

	if( -1 == chnn )
	{
		unsigned char chnnNum =  IsSupportHybridDisplay()?CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum:CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
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
		else if (12 == chnnNum)
		{
			splitMode= VIEW_SPLIT_4X4;
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

		ChangeDisplayByCH(ALL_CHNN_MASK(chnnNum), splitMode, DISPLAY_FREEZE, 0);

		GetRootWnd()->Repaint();
	}
	else
	{
		ChangeDisplayByCH((0x01 << chnn), VIEW_SPLIT_1X1, DISPLAY_FREEZE, 0);
		GetRootWnd()->Repaint();
	}

	retVal = m_changeColorDlg.DoModal( this, x , y, cx, cy, false);

	//返回之前的大画面状态
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0 , msgData );

	pMsgMan->ReleaseMsgBuff( msgData );

	GetRootWnd()->Repaint();
	return retVal;
}

DLG_END_VALUE CDisktop::PopupLiveSetCamParaDlg(int x, int y, int cx, int cy, int chnn )
{
	CMessageMan *pMsgMan = CMessageMan::Instance();
	DLG_END_VALUE retVal;

	//切换到相关通道的大画面
	MESSAGE_DATA msgData;

	if( -1 == chnn )
	{
		unsigned char chnnNum =  IsSupportHybridDisplay()?CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum:CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
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
		else if (12 == chnnNum)
		{
			splitMode= VIEW_SPLIT_4X4;
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

		ChangeDisplayByCH(ALL_CHNN_MASK(chnnNum), splitMode, DISPLAY_FREEZE, 0);

		GetRootWnd()->Repaint();
	}
	else
	{
		if(chnn < CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum)
		{
			return DLG_ESC;
		}
		else
		{
			ChangeDisplayByCH((0x01 << chnn), VIEW_SPLIT_1X1, DISPLAY_FREEZE, 0);
			GetRootWnd()->Repaint();
		}
	}

	retVal = m_setCamParaDlg.DoModal( this, x , y, cx, cy, false);

	//返回之前的大画面状态
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0 , msgData );

	pMsgMan->ReleaseMsgBuff( msgData );

	GetRootWnd()->Repaint();
	return retVal;
}
#ifdef __CUSTOM_US02__
void CDisktop::OnUS02Wizzard()
{
	static bool s_wzd_b_first = true;

	bool bShowWzd = false; //是否显示向导页面
	
	//向导修改语言，生效后保证向导显示而不是直接进入应用（更改s_wzd_b_first )
	if(g_bWizzardChangeLanguage)
	{
		s_wzd_b_first = true;
		g_bWizzardChangeLanguage = false;
	}

	if(s_wzd_b_first)
	{
		s_wzd_b_first = false;

		if(0 != g_showWizzard)
		{
			bShowWzd = true;
		}
	}

	if(!bShowWzd)
	{
		return;
	}
	m_WzdUS02ManDlg.DoModal(this);
}

void CDisktop::OnNewNatWizzard()
{
	static bool s_wzd_b_first = true;

	bool bShowWzd = false; //是否显示向导页面

	//向导修改语言，生效后保证向导显示而不是直接进入应用（更改s_wzd_b_first )
	if(g_bWizzardChangeLanguage)
	{
		s_wzd_b_first = true;
		g_bWizzardChangeLanguage = false;
	}

	if(s_wzd_b_first)
	{
		s_wzd_b_first = false;

		if(0 != g_showWizzard)
		{
			bShowWzd = true;
		}
	}
	printf("(bShowWzd=================%d)\n", bShowWzd);
	if(!bShowWzd)
	{
		return;
	}
	m_WzdNewNatManDlg.DoModal(this);
	printf("OnNewNatWizzard DoModal exit\n");	
}
#endif

void CDisktop::OnNatWizarrd()
{
#ifdef __SUPPORT_SHOW_QRCODE__
	static bool s_wzd_b_first = true;

	bool bShowWzd = false; //是否显示向导页面

	//向导修改语言，生效后保证向导显示而不是直接进入应用（更改s_wzd_b_first )
	if(g_bWizzardChangeLanguage)
	{
		s_wzd_b_first = true;
		g_bWizzardChangeLanguage = false;
	}

	if(s_wzd_b_first)
	{
		s_wzd_b_first = false;

		if(0 != g_showWizzard)
		{
			bShowWzd = true;
		}
	}

	if(!bShowWzd)
	{
		return;
	}
	m_wzdNatManDlg.DoModal(this);
#endif
}

unsigned long  CDisktop::OnChangeColor()
{
	ULONGLONG auth = 0;
//#if defined (__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__)
	auth = CheckAuthority(AUTH_SYS_SETUP);
//#else
//	auth = CheckAuthority(AUTH_BASIC);
//#endif
	if (0 == auth)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		
		GetRootWnd()->Repaint();

		return KEY_VIRTUAL_MSG_STOP;
	}

	HideSnapTip();

	//此处如果为真，则现场为调节VO输出的色彩，否则为调节编码色彩
#if 0

	unsigned short xOff = m_width - CCfgLiveColorDlg::GetWidth() - CCfgLiveColorDlg::CFG_DLG_RIGHT_X;
	if (m_width < 800)
	{
		xOff -= 40;
	}

	if (m_width < 800)
	{
		xOff -= 40;
	}
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_DEV_COLOR, NULL, 0, msgData);
	unsigned long color = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	m_changeDevColorDlg.SetDevColorInfo(color);

	m_changeDevColorDlg.DoModal(this, xOff , CENTER_POS, m_changeDevColorDlg.GetWidth(),m_changeDevColorDlg.GetHeight(),NULL);

#else

	unsigned int videoInputNum = g_p_login_succ->productInfo.videoInputNum;
	CMessageMan* pMsg = CMessageMan::Instance();

	//读取颜色值
	NET_CHNN_COLOR liveColor;
	memset(&liveColor, 0, sizeof(NET_CHNN_COLOR));
	liveColor.chnn = -1;
	liveColor.hide = 0;
	
	MESSAGE_DATA messageData;
	memset(&messageData, 0, sizeof(MESSAGE_DATA));
	pMsg->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_COLOR_GET, (unsigned char *)&liveColor, sizeof(NET_CHNN_COLOR), messageData);	
	
	unsigned long *pColor = new unsigned long[videoInputNum];
	memset(pColor, 0, sizeof(unsigned long) * videoInputNum);

	assert(NULL != messageData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(messageData.pData);
	if(CMD_REPLY_CHNN_CTRL_COLOR == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		NET_CHNN_COLOR* pInfo = reinterpret_cast<NET_CHNN_COLOR*>(messageData.pData + sizeof(PACKCMD));
		for(int i = 0; i < videoInputNum; i++)
		{
			memcpy(&liveColor, &pInfo[i], sizeof(NET_CHNN_COLOR));
			pColor[i] = liveColor.color;
		}
	}
	pMsg->ReleaseMsgBuff(messageData);

	//
	unsigned short xOff = m_width - CCfgLiveColorDlg::GetWidth() - CCfgLiveColorDlg::CFG_DLG_RIGHT_X;
	if (m_width < 800)
	{
		xOff -= 40;
	}

	if (m_width < 800)
	{
		xOff -= 40;
	}

	//获得当前通道
	NET_DISPLAY_INFO disp;
	memset(&messageData, 0, sizeof(MESSAGE_DATA));
#ifdef __DVR_MENU__
	//获取鼠标位置
	unsigned long x, y = 0;
	m_toolBarMenu.GetPtPos(x, y);
	disp.x = x;
	disp.y = y;
	disp.type = NET_DISPLAY_GET_INFO_XY;
#else
	disp.type = NET_DISPLAY_GET_INFO;
#endif
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_DISPLAY_INFO, (unsigned char*)&disp, sizeof(NET_DISPLAY_INFO), messageData);
	
	assert(NULL != messageData.pData);
	pCmd = reinterpret_cast<PACKCMD *>(messageData.pData);
	if(CMD_REPLY_CTRL_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(messageData.pData + sizeof(PACKCMD));
		memcpy(&disp, pDispInfo, sizeof(NET_DISPLAY_INFO));
	}
	pMsg->ReleaseMsgBuff(messageData);

	if((disp.split_mode[DISPLAY_LAYER_ANALOG] != VIEW_SPLIT_1X1) && (disp.split_mode[DISPLAY_LAYER_NET] != VIEW_SPLIT_1X1))
	{
		//设置all和各频道列表信息,-2表示 是从工具栏进入，-3是从右键菜单进入
#ifndef __DVR_MENU__
		m_changeColorDlg.SetChnnInfo(pColor, videoInputNum, -2, -2);
		PopupLiveColorDlg(xOff, CENTER_POS, m_changeColorDlg.GetWidth(), m_changeColorDlg.GetHeight(), -1);
#else
		if(disp.data_len <= 0)
		{
			m_changeColorDlg.SetChnnInfo(pColor,videoInputNum,-3,-3);
			PopupLiveColorDlg(xOff, CENTER_POS,m_changeColorDlg.GetWidth(), m_changeColorDlg.GetHeight(), -1);

		}
		else
		{
			unsigned char hit_chnn = DISPLAY_GET_CHNN(disp.data[0]);
			if(DISPLAY_LAYER_NET == DISPLAY_GET_LAYER(disp.data[0]))
			{
				hit_chnn += g_p_login_succ->productInfo.localVideoInputNum;
			}

			m_changeColorDlg.SetChnnInfo(pColor, videoInputNum, hit_chnn, -3);
			PopupLiveColorDlg(xOff, CENTER_POS, m_changeColorDlg.GetWidth(), m_changeColorDlg.GetHeight(), hit_chnn);
		}
#endif
		
	}
	else//单通道
	{
		unsigned char selChnn = 0;
		for(int i = 0; i < disp.data_len; i++)
		{
			unsigned char chnn = DISPLAY_GET_CHNN(disp.data[i]);
			if(DISPLAY_LAYER_ANALOG == DISPLAY_GET_LAYER(disp.data[i]))
			{
				selChnn = chnn;
				break;
			}
			if(DISPLAY_LAYER_NET == DISPLAY_GET_LAYER(disp.data[i]))
			{
				selChnn = chnn + g_p_login_succ->productInfo.localVideoInputNum;
				break;
			}
		}
		//设置all和各频道列表信息,-2表示 是从工具栏进入，-3是从右键菜单进入
#ifndef __DVR_MENU__
		m_changeColorDlg.SetChnnInfo(pColor, videoInputNum, selChnn, -2);
#else
		m_changeColorDlg.SetChnnInfo(pColor, videoInputNum, selChnn, -3);
#endif
		PopupLiveColorDlg(xOff, CENTER_POS, m_changeColorDlg.GetWidth(), m_changeColorDlg.GetHeight(), selChnn);
	}

	//
	delete [] pColor;
	pColor = NULL;
#endif

	GetRootWnd()->Repaint();
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnChangeCamPara()
{
	ULONGLONG CHipc = 0;
	int localVideoInputNumIpc = 0, netVideoInputNumIpc = 0;
	int maxChnnIpc = 0,effectChnnIpc = 0,count =0;

	IsSupportCH(CHipc, &effectChnnIpc, &localVideoInputNumIpc, &netVideoInputNumIpc);
	maxChnnIpc = localVideoInputNumIpc + netVideoInputNumIpc;

	for(int i = localVideoInputNumIpc; i < maxChnnIpc; i++){
		if((CHipc >> i) & 0x1){
			++count;
		}
	}
	if (0 == count)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_71), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		GetRootWnd()->Repaint();

		return KEY_VIRTUAL_MSG_STOP;
	}

	ULONGLONG auth = 0;
#if defined (__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__)
	auth = CheckAuthority(AUTH_SYS_SETUP);
#else
	auth = CheckAuthority(AUTH_BASIC);
#endif
	if (0 == auth)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);

		GetRootWnd()->Repaint();

		return KEY_VIRTUAL_MSG_STOP;
	}

	HideSnapTip();

	unsigned int netVideoInputNum = g_p_login_succ->productInfo.netVideoInputNum;
	unsigned int localVideoInputNum = g_p_login_succ->productInfo.localVideoInputNum;
	CMessageMan* pMsg = CMessageMan::Instance();

	////读取 走廊模式，镜像 ，翻转，畸变校正
	//unsigned char			ucFlip;//NCFG_ITEM_CAM_FILP
	//unsigned char           ucMirror;//NCFG_ITEM_CAM_MIRROR,	
	//unsigned long 		    ulRotato;//NCFG_ITEM_CAM_ROTATO
	//LENS_DISTORTION_CONFIG  lensDistortion;//NCFG_ITEM_CAM_LENS_DISTORTION

	CAMERA_PARAMTER * pSetCamPara;
	pSetCamPara = new CAMERA_PARAMTER[netVideoInputNum];
	memset(pSetCamPara,0,sizeof(CAMERA_PARAMTER)*netVideoInputNum);

	// flip
	NET_CHNN_FLIP flipLive;
	memset(&flipLive, 0, sizeof(NET_CHNN_FLIP));
	flipLive.chnn = -1;

	MESSAGE_DATA messageDataFlip;
	memset(&messageDataFlip, 0, sizeof(MESSAGE_DATA));
	pMsg->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_IMAGE_FILP, (unsigned char *)&flipLive, sizeof(NET_CHNN_FLIP), messageDataFlip);	

	assert(NULL != messageDataFlip.pData);
	PACKCMD *pCmdFlip = reinterpret_cast<PACKCMD *>(messageDataFlip.pData);
	if(CMD_REPLY_GET_IMAGE_FILP == static_cast<NET_PROTOCOL>(pCmdFlip->cmdType))
	{
		NET_CHNN_FLIP* pFlipInfo = reinterpret_cast<NET_CHNN_FLIP*>(messageDataFlip.pData + sizeof(PACKCMD));
		for(int i = 0; i < netVideoInputNum; i++)
		{
			memcpy(&flipLive, &pFlipInfo[i], sizeof(NET_CHNN_FLIP));
			pSetCamPara[i].m_flip = flipLive.flip;
			pSetCamPara[i].m_chnn = i;
		}
	}
	pMsg->ReleaseMsgBuff(messageDataFlip);

    // mirror
	NET_CHNN_MIRROR mirrorLive;
	memset(&mirrorLive, 0, sizeof(NET_CHNN_MIRROR));
	mirrorLive.chnn = -1;

	MESSAGE_DATA messageDataMirror;
	memset(&messageDataMirror, 0, sizeof(MESSAGE_DATA));
	pMsg->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_IMAGE_MIRROR, (unsigned char *)&mirrorLive, sizeof(NET_CHNN_MIRROR), messageDataMirror);	

	assert(NULL != messageDataMirror.pData);
	PACKCMD *pCmdMirror = reinterpret_cast<PACKCMD *>(messageDataMirror.pData);
	if(CMD_REPLY_GET_IMAGE_MIRROR == static_cast<NET_PROTOCOL>(pCmdMirror->cmdType))
	{
		NET_CHNN_MIRROR* pMirrorInfo = reinterpret_cast<NET_CHNN_MIRROR*>(messageDataMirror.pData + sizeof(PACKCMD));
		for(int i = 0; i < netVideoInputNum; i++)
		{
			memcpy(&mirrorLive, &pMirrorInfo[i], sizeof(NET_CHNN_MIRROR));
			pSetCamPara[i].m_mirror = mirrorLive.mirror;
			pSetCamPara[i].m_chnn  = i;
		}
	}
	pMsg->ReleaseMsgBuff(messageDataMirror);
    /*
	//rotato
	NET_CHNN_ROTATO rotatoLive;
	memset(&rotatoLive, 0, sizeof(NET_CHNN_ROTATO));
	rotatoLive.chnn = -1;

	MESSAGE_DATA messageDataRotato;
	memset(&messageDataRotato, 0, sizeof(MESSAGE_DATA));
	pMsg->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_IMAGE_ROTATO, (unsigned char *)&rotatoLive, sizeof(NET_CHNN_ROTATO), messageDataRotato);	

	assert(NULL != messageDataRotato.pData);
	PACKCMD *pCmdRotato = reinterpret_cast<PACKCMD *>(messageDataRotato.pData);
	if(CMD_REPLY_GET_IMAGE_ROTATO == static_cast<NET_PROTOCOL>(pCmdRotato->cmdType))
	{
		NET_CHNN_ROTATO* pRotatoInfo = reinterpret_cast<NET_CHNN_ROTATO*>(messageDataRotato.pData + sizeof(PACKCMD));
		for(int i = 0; i < netVideoInputNum; i++)
		{
			memcpy(&rotatoLive, &pRotatoInfo[i], sizeof(NET_CHNN_ROTATO));
			pSetCamPara[i].m_rotato = rotatoLive.rotato;
		}
	}
	pMsg->ReleaseMsgBuff(messageDataRotato); */
    //len distortion
	NET_CHNN_LENS_DISTORTION distortionLive;
	memset(&distortionLive, 0, sizeof(NET_CHNN_LENS_DISTORTION));
	distortionLive.chnn = -1;

	MESSAGE_DATA messageDataDistortion;
	memset(&messageDataDistortion, 0, sizeof(MESSAGE_DATA));
	pMsg->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_IMAGE_DISTORTION, (unsigned char *)&distortionLive, sizeof(NET_CHNN_LENS_DISTORTION), messageDataDistortion);	

	assert(NULL != messageDataDistortion.pData);
	PACKCMD *pCmdDistortion = reinterpret_cast<PACKCMD *>(messageDataDistortion.pData);
	if( CMD_REPLY_GET_IMAGE_DISTORTION == static_cast<NET_PROTOCOL>(pCmdDistortion->cmdType))
	{
		NET_CHNN_LENS_DISTORTION* pDistortionInfo = reinterpret_cast<NET_CHNN_LENS_DISTORTION*>(messageDataDistortion.pData + sizeof(PACKCMD));
		for(int i = 0; i < netVideoInputNum; i++)
		{
			//memcpy(&distortionLive, &pDistortionInfo[i], sizeof(NET_CHNN_LENS_DISTORTION));
			distortionLive.chnn = pDistortionInfo[i].chnn;
			distortionLive.lensDistortion.ucLensDistortion = pDistortionInfo[i].lensDistortion.ucLensDistortion;
			distortionLive.lensDistortion.ucLensDistortionValue = pDistortionInfo[i].lensDistortion.ucLensDistortionValue;
			distortionLive.lensDistortion.noUse[0] = pDistortionInfo[i].lensDistortion.noUse[0];
			distortionLive.lensDistortion.noUse[1] = pDistortionInfo[i].lensDistortion.noUse[1];

			pSetCamPara[i].m_lensDistortion.ucLensDistortion = distortionLive.lensDistortion.ucLensDistortion;
			pSetCamPara[i].m_lensDistortion.ucLensDistortionValue = distortionLive.lensDistortion.ucLensDistortionValue;
			pSetCamPara[i].m_lensDistortion.noUse[0] = distortionLive.lensDistortion.noUse[0];
			pSetCamPara[i].m_lensDistortion.noUse[1] = distortionLive.lensDistortion.noUse[1];
		}
	}
	pMsg->ReleaseMsgBuff(messageDataDistortion);

	unsigned short xOff = m_width - CSetCamParaDlg::GetWidth() - CSetCamParaDlg::CFG_DLG_RIGHT_X;
	if (m_width < 800)
	{
		xOff -= 40;
	}

	if (m_width < 800)
	{
		xOff -= 40;
	}

	//获得当前通道
	MESSAGE_DATA messageData;
	NET_DISPLAY_INFO disp;
	memset(&messageData, 0, sizeof(MESSAGE_DATA));
#ifdef __DVR_MENU__
	//获取鼠标位置
	unsigned long x, y = 0;
	m_toolBarMenu.GetPtPos(x, y);
	disp.x = x;
	disp.y = y;
	disp.type = NET_DISPLAY_GET_INFO_XY;
#else
	disp.type = NET_DISPLAY_GET_INFO;
#endif
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_DISPLAY_INFO, (unsigned char*)&disp, sizeof(NET_DISPLAY_INFO), messageData);

	assert(NULL != messageData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(messageData.pData);
	if(CMD_REPLY_CTRL_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(messageData.pData + sizeof(PACKCMD));
		memcpy(&disp, pDispInfo, sizeof(NET_DISPLAY_INFO));
	}
	pMsg->ReleaseMsgBuff(messageData);

//	// 全通道
//	if((disp.split_mode[DISPLAY_LAYER_ANALOG] != VIEW_SPLIT_1X1) && (disp.split_mode[DISPLAY_LAYER_NET] != VIEW_SPLIT_1X1))
//	{
//		//设置all和各频道列表信息,-2表示 是从工具栏进入，-3是从右键菜单进入
//#ifndef __DVR_MENU__
//		m_setCamParaDlg.SetChnnInfo(pSetCamPara,localVideoInputNum,netVideoInputNum,localVideoInputNum);
//		PopupLiveSetCamParaDlg(xOff, CENTER_POS, m_changeColorDlg.GetWidth(), m_changeColorDlg.GetHeight(), -1);
//#else
//		/*if(disp.data_len <= 0)
//		{
//			m_changeColorDlg.SetChnnInfo(pColor,videoInputNum,-3,-3);
//			PopupLiveColorDlg(xOff, CENTER_POS,m_changeColorDlg.GetWidth(), m_changeColorDlg.GetHeight(), -1);
//
//		}
//		else
//		{
//			unsigned char hit_chnn = DISPLAY_GET_CHNN(disp.data[0]);
//			if(DISPLAY_LAYER_NET == DISPLAY_GET_LAYER(disp.data[0]))
//			{
//				hit_chnn += g_p_login_succ->productInfo.localVideoInputNum;
//			}
//
//			m_changeColorDlg.SetChnnInfo(pColor, videoInputNum, hit_chnn, -3);
//			PopupLiveColorDlg(xOff, CENTER_POS, m_changeColorDlg.GetWidth(), m_changeColorDlg.GetHeight(), hit_chnn);
//		}*/
//#endif
//	}
//	else  //单通道
	{
		unsigned char selChnn = 0;
		for(int i = 0; i < disp.data_len; i++)
		{
			unsigned char chnn = DISPLAY_GET_CHNN(disp.data[i]);
			if(DISPLAY_LAYER_ANALOG == DISPLAY_GET_LAYER(disp.data[i]))
			{
				selChnn = chnn + g_p_login_succ->productInfo.localVideoInputNum;
				break;
			}
			if(DISPLAY_LAYER_NET == DISPLAY_GET_LAYER(disp.data[i]))
			{
				selChnn = chnn + g_p_login_succ->productInfo.localVideoInputNum;
				break;
			}
		}
		ULONGLONG CH = 0;
		int localVideoInputNum = 0, netVideoInputNum = 0;
		int maxChnn = 0, effectChnn = 0;

		IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);
		for(int i=localVideoInputNum;i<localVideoInputNum+netVideoInputNum;i++)
		{
			if((CH>>i)&0x01)
			{
				selChnn = i;
				break;
			}
		}
		 // 工具栏进来1    现场进来 0
#ifndef __DVR_MENU__
		m_setCamParaDlg.SetChnnInfo(pSetCamPara,selChnn,netVideoInputNum,localVideoInputNum,1);
#else
		m_setCamParaDlg.SetChnnInfo(pSetCamPara,selChnn,netVideoInputNum,localVideoInputNum,1);
#endif
		PopupLiveSetCamParaDlg(xOff, CENTER_POS, m_setCamParaDlg.GetWidth(), m_setCamParaDlg.GetHeight(), selChnn);
	}

	memset(pSetCamPara,0,sizeof(CAMERA_PARAMTER)*netVideoInputNum);
	delete [] pSetCamPara;
	pSetCamPara = NULL;

	GetRootWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnChangeToolStatus()
{
#ifndef __DVR_MENU__

	ULONGLONG auth = CheckAuthority(AUTH_BASIC);
	if (0 == auth)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);

		GetRootWnd()->Repaint();

		return KEY_VIRTUAL_MSG_STOP;
	}

	//int state = m_toolBar.GetState();

	//state++;
	//if(state > GUI::CToolBar::TOOL_UNFIX_EDIT)
	//{
	//	state = 0;
	//}
	//m_toolBar.SetState(state, true);
	//m_toolBar.ResetDrag();

	//if((GUI::CToolBar::TOOL_UNFIX_EDIT != m_toolBar.GetState()) && m_toolBar.GetCfgPos(m_pWndStartupInfo->ToolbarPos))
	//{
	//	TrySaveStartupInfo(g_p_wnd_startup_info, m_pWndStartupInfo);

	//	GetRootWnd()->Repaint();
	//}

#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::OnShutDown()
{

#if defined(__CUSTOM_BR27__)
	DLG_END_VALUE value = MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_06), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_QUESTION);
	if (DLG_OK == value)
	{
		if (DLG_OK == m_loginDlg.DoModal(this))
		{
			m_loginDlg.GetLoginSuccessReply(CCfgDlg::s_loginSuccessReply);
			ULONGLONG auth = CheckAuthority(AUTH_SHUTDOWN);
			if (0 != auth)
			{
				g_system_status = DVR_SYSTEM_EXIT;
			}
			else
			{
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_83), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			}
			
		}
	}
	else if (DLG_ESC == value)
	{
		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
#else
	if (IsSupportShutDown())
	{
		ULONGLONG auth = CheckAuthority(AUTH_SHUTDOWN);
		if (0 != auth)
		{
			DLG_END_VALUE value = MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_06), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_QUESTION);
			if (DLG_OK == value)
			{
#ifdef __SUPPORT_SNAPD1__
				char tmp[256]="GB2312:系统关机";
				CShanghaiConnectMan::Instance()->GetLogCmd(DVRSYSEXIT,0,tmp,strlen(tmp),NULL,0,GetCurrTime());
#endif
				g_system_status = DVR_SYSTEM_EXIT;
			}
			else if (DLG_ESC == value)
			{
				GetRootWnd()->Repaint();
			}

			return KEY_VIRTUAL_MSG_STOP;
		}
		else
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_83), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
#endif
}

unsigned long  CDisktop::OnHide()
{
	if (m_mainMenu.IsShow())
	{
		m_mainMenu.Show(false, false);
#ifndef __DVR_MENU__
		if (!m_toolBar.IsShow())
		{
			m_toolBar.SetShowToolbar(true);
			m_toolBar.Show(true, false);
		}
#endif
		#ifdef __CUSTOM_US02_NEW__
			if(m_pTipWindow != NULL && m_pTipWindow->IsShow())
				{
					m_pTipWindow->Show(false, true);
					GetRootWnd()->Repaint();
				}
		#endif
	} 
	else
	{
#ifndef __DVR_MENU__
		if (m_toolBar.IsShow())
		{
			m_toolBar.Show(false, false);
			#ifdef __CUSTOM_US02_NEW__
			if(m_pTipWindow != NULL && m_pTipWindow->IsShow())
				{
					m_pTipWindow->Show(false, true);
					GetRootWnd()->Repaint();
				}
			#endif
		}
		else
		{
			m_toolBar.Show(true, false);
		}
		m_bShowToolBar = m_toolBar.IsShow();
#endif
	}
	
	GetRootWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnSnap()
{
	ULONGLONG auth = CheckAuthority(AUTH_RECORD);
	if (0 != auth)
	{
		bool bShowSnapTip = true, bSucc = true;
		m_pMsgMan->ShowSnapTip(bShowSnapTip, bSucc);

		if(!bSucc || !bShowSnapTip)
		{
			unsigned long x = 0, y = 0;

#ifndef __DVR_MENU__
			GUI::CRect rectToolBar = m_toolBar.GetWindowRect();
			GUI::CRect rectSnap = m_toolBar.GetSubBtnWndRect(CTRLID_TOOL_BAR_SNAP);

			x = rectSnap.m_left + 2;
			y = (rectToolBar.m_top > m_height / 2) ? (rectToolBar.m_top - 36) : (rectToolBar.m_bottom + 4);
#else
			m_toolBarMenu.GetPtPos(x, y);
#endif

#ifndef __DVR_MENU__
			if (m_stcDiskAlarm.IsShow())
			{
				GUI::CRect recTemp = m_stcDiskAlarm.GetWindowRect();
				GUI::CRect rectTmp2(x, y, x + 32, y + 32);
				if (recTemp.IntersectRect(rectTmp2))
				{
					if (rectToolBar.m_top > m_height / 2)
					{
						y -= (recTemp.Height());
					}
					else
					{
						y += (recTemp.Height());
					}
				}				

			}
#endif
			m_pMsgMan->SetSnapTipPos(x, y);

			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			m_pMsgMan->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_MANUAL_SNAP, (unsigned char*)&auth, sizeof(ULONGLONG), msgData);
			PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
			if (CMD_REPLY_CTRL_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
			{
				//MessageBox(m_pStrTable->String(E_STR_ID_SNAP_FAILED), m_pStrTable->String(E_STR_ID_TITLE_FAILED), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				m_pMsgMan->SetShowSnapTip(true, false);
				
				m_countSnapTip = 100;
			}
			else
			{
				unsigned long sleepTime = *(reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD)));
				//printf("%s,%d,sleepTime:%d\n",__FILE__,__LINE__,sleepTime);
				//PUB_Sleep(sleepTime*1000);
				//MessageBox(m_pStrTable->String(E_STR_ID_SNAP_OK), m_pStrTable->String(E_STR_ID_TITLE_SUCCEED), DEFAULT_BTN_OK);
			
				m_pMsgMan->SetShowSnapTip(true, true);
				m_countSnapTip = sleepTime * 50;
			}
			m_pMsgMan->ReleaseMsgBuff(msgData);	
		}
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	GetRootWnd()->Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnClickMainMenu(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	assert(m_mainMenu.GetID() == ctrlID);
	unsigned long commandID = keyID & (~KEY_VIRTUAL_MSG_DEFAULT_TAG);
	switch (commandID)
	{
	case CTRLID_MAINMENU_CONFIG:
		return OnConfig();
		break;
	case CTRLID_MAINMENU_MANUAL_ALARM:
		return OnManualAlarm();
		break;
	case CTRLID_MAINMENU_DISK_MAN:
		return OnDiskMan();
		break;
	case CTRLID_MAINMENU_SYSTEM_RESOURCES:
		return OnSysRescouces();
		break;
	case CTRLID_MAINMENU_DEVICE_MAN:
		return OnDeviceMan();
		break;
	case CTRLID_MAINMENU_LOGOFF:
		return OnLogoff();
		break;
	default:
		assert(false);
		return KEY_VIRTUAL_MSG_STOP;
		break;
	}	
}

unsigned long  CDisktop::OnConfig()
{
	ULONGLONG auth = CheckAuthority(AUTH_BASIC);
	if (0 != auth)
	{
		m_cfgSystemMan.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
		GetRootWnd()->Repaint();
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::OnManualAlarm()
{
	if (IsSupportAlarmOut() || (g_p_login_succ->productInfo.netVideoInputNum > 0))
	{
		ULONGLONG auth = CheckAuthority(AUTH_BASIC);
		if (0 != auth)
		{
			m_manualAlarm.DoModal( this , CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

			GetRootWnd()->Repaint();
		}
		else
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::OnDiskMan()
{
	ULONGLONG auth = CheckAuthority(AUTH_DISK_MAN);
	if (0 != auth)
	{
		m_diskMan.DoModal( this , CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

		GetRootWnd()->Repaint();
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_85), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CDisktop::OnSysRescouces()
{
	ULONGLONG auth = CheckAuthority(AUTH_ADVANCE_MAN);
	if (0 != auth)
	{
		m_update.DoModal( this , CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

		GetRootWnd()->Repaint();
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnDeviceMan()
{
	if (g_p_login_succ->productInfo.netVideoInputNum > 0)
	{
		if (EnterConfig())
		{
			m_deviceManDlg.DoModal( this , CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
			GetRootWnd()->Repaint();
			ExitConfig();
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}


bool CDisktop::EnterConfig()
{
	//进入配置
	CMessageMan* pMsgMan = CMessageMan::Instance();

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_ENTER_IPCAM, NULL, 0, msgData);

	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		pMsgMan->ReleaseMsgBuff( msgData );
		return true;
	}
	else
	{
		pMsgMan->ReleaseMsgBuff( msgData );
		GUI::CDialog::MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_26), m_pStrTable->String(E_STR_ID_TITLE_FAILED), DEFAULT_BTN_OK|DEFAULT_ICON_ERROR);
		return false;
	}
}

void CDisktop::ExitConfig() const
{
	//退出配置
	CMessageMan* pMsgMan = CMessageMan::Instance();
	pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE_IPCAM, NULL, 0);
}


unsigned long CDisktop::OnLogoff()
{
	if( m_loginDlg.IsLogin() )
	{
		if( DLG_OK == MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_86), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_QUESTION))
		{
			m_loginDlg.Logoff();

			m_mainMenu.Show(false, false);
#ifndef __DVR_MENU__
			m_toolBar.Show(false, false);
			m_toolBar.SetShowToolbar(true);
#endif

			//2010-02-04 09:16:00 YSW
			//登出后，需要根据设定的权限来刷新现场画面
			{
				//m_displayinfo.type = 0;

				//GetCurDisplay(m_displayinfo);
				//m_displayinfo.type = NET_DISPLAY_CHANGE_DATA_PACK;

				//MESSAGE_DATA msgData;
				//memset(&msgData, 0, sizeof(MESSAGE_DATA));
				//m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&m_displayinfo, sizeof(NET_DISPLAY_INFO), msgData);	
				//m_pMsgMan->ReleaseMsgBuff(msgData);

				ChangeDisplayToDwellGroup();

				//刷新现场分割线等内容
				CMessageMan::Instance()->RefreshDisplay(0x1/*m_displayinfo.chnn*/);
			}

			GetRootWnd()->Repaint();
		}
	}
	else
	{
		assert(false);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDisktop::OnInputChnnNum( unsigned long ctrlID )
{
	printf("####%s,%d,ctrlID:%d\n",__FILE__,__LINE__,ctrlID);
	unsigned long chnn = ( ctrlID & 0x3f );
	printf("####%s,%d,chnn:%d\n",__FILE__,__LINE__,chnn);
	ULONGLONG auth = CheckAuthority(AUTH_LIVE);
	ULONGLONG cfgCH = GetMainMonitorCfg();

	auth &= cfgCH;

	if (0 == chnn)
	{
		chnn = (10 - 1);
	}
	else
	{
		chnn -= 1;
	}
	
	if((0 <= chnn) && (chnn < g_p_login_succ->productInfo.videoInputNum))
	{
		//检查权限，如果没有权限直接返回不响应。
		if(0 != (auth & ((ULONGLONG)(0x01) << chnn)))
		{
			assert (chnn < g_p_login_succ->productInfo.videoInputNum);

			ULONGLONG CH = (static_cast<ULONGLONG>(0x1) << chnn);
			ChangeDisplayByCH(CH, VIEW_SPLIT_1X1);

			GetRootWnd()->Repaint();
		}
	}
		
	return KEY_VIRTUAL_MSG_STOP;
}
/***************************************************************************************
函数：	ChangeCurDisplay( unsigned long keyID )
功能：	向服务端发送改变分割模式命令。
		该函数由按键板和485键盘上的画面分割按键触发。
		向服务端发送的信息中不包含通道信息，type为2。服务端接收到这种命令后依次刷新界面分割。
		比如连续按四画面按键，界面会依次显示1-4，5-8， 9-12，13-16的四画面分割会面
参数：	
		keyID 不同分割模式的虚拟键值
返回值：
		true 操作成功
		false 操作失败

		
zhl 2010-03-09
***************************************************************************************/
bool CDisktop::ChangeCurDisplay( unsigned long keyID )
{
	unsigned long splitMode = 0;

	switch( keyID )
	{
	case KEY_VIRTUAL_1X1:
		OnChangeCH(m_1X1CH, VIEW_SPLIT_1X1, 1);
		break;
	case KEY_VIRTUAL_2X2:
		OnChangeCH(m_2X2CH, VIEW_SPLIT_2X2, 4);
		break;
	case KEY_VIRTUAL_3X3:
		OnChangeCH(m_3X3CH, VIEW_SPLIT_3X3, 9);
		break;
	case KEY_VIRTUAL_4X4:
		OnChangeCH(m_4X4CH, VIEW_SPLIT_4X4, 16);
		break;
	case KEY_VIRTUAL_5X5:
		//OnChangeCH(m_5X5CH, VIEW_SPLIT_5X5, 25);
		break;
	case KEY_VIRTUAL_6X6:
		//OnChangeCH(m_6X6CH, VIEW_SPLIT_6X6, 36);
		break;
	case KEY_VIRTUAL_2X3:
		OnChangeCH(m_2X3CH, VIEW_SPLIT_2X3, 6);
		break;
	default:
		break;
	}

	return true;
}

void CDisktop::ChangeDisplayByCH(ULONGLONG CH, unsigned long splitMode, int chnnMini, unsigned long timeout, unsigned short bRec, unsigned short bFilter)
{
	memset(&m_displayinfo, 0 ,sizeof(NET_DISPLAY_INFO));
	if(bFilter)
	{
		m_displayinfo.type = NET_DISPLAY_FILTER;
	}
	else
	{
		m_displayinfo.type = NET_DISPLAY_CHANGE_DATA_PACK;
	}
	m_displayinfo.rec = bRec;
	m_displayinfo.time_out = timeout;
	m_displayinfo.split_mode[DISPLAY_LAYER_ANALOG] = splitMode;
	m_displayinfo.split_mode[DISPLAY_LAYER_NET] = splitMode;

	ULONGLONG effectCH = 0;
	int videoInputNum = 0, localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	IsSupportCH(effectCH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	videoInputNum = localVideoInputNum + netVideoInputNum;

	int index = 0;
	for (int i = 0; i < videoInputNum; i++)
	{
		if(CH & (static_cast<ULONGLONG>(0x1) << i))
		{
			if(i < localVideoInputNum)
			{
				m_displayinfo.data[index] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, index, i);
			}
			else
			{
				m_displayinfo.data[index] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, index, i - localVideoInputNum);
			}
			index++;
		}
	}
	
	if(chnnMini >= 0)
	{
		if(chnnMini < localVideoInputNum)
		{
			m_displayinfo.data[index] = DISPLAY_PACK(DISPLAY_LAYER_MINI, DISPLAY_SIGNAL_ANALOG, index, chnnMini);
		}
		else
		{
			m_displayinfo.data[index] = DISPLAY_PACK(DISPLAY_LAYER_MINI, DISPLAY_SIGNAL_NET, index, chnnMini - localVideoInputNum);
		}

		index++;
	}

	m_displayinfo.data_len = index;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&m_displayinfo, sizeof(NET_DISPLAY_INFO), msgData);	
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CTRL_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));
		memcpy(&m_displayinfo, pDispInfo, sizeof(NET_DISPLAY_INFO));
	}
	m_pMsgMan->ReleaseMsgBuff(msgData);
}

void CDisktop::ChangeDisplayByPt(unsigned short x, unsigned short y, unsigned long timeout, unsigned short bRec)
{
	memset(&m_displayinfo, 0 ,sizeof(NET_DISPLAY_INFO));
	m_displayinfo.type = NET_DISPLAY_CHANGE_XY_PT;
	m_displayinfo.time_out = timeout;
	m_displayinfo.rec = bRec;

	m_displayinfo.x = x;
	m_displayinfo.y = y;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&m_displayinfo, sizeof(NET_DISPLAY_INFO), msgData);	
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CTRL_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));
		memcpy(&m_displayinfo, pDispInfo, sizeof(NET_DISPLAY_INFO));
	}
	m_pMsgMan->ReleaseMsgBuff(msgData);
}

void CDisktop::ChangeDisplayByTwoPt(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long timeout, unsigned short bRec)
{
	memset(&m_displayinfo, 0 ,sizeof(NET_DISPLAY_INFO));
	m_displayinfo.type = NET_DISPLAY_EXCHANGE_TWO_PT;
	m_displayinfo.time_out = timeout;
	m_displayinfo.rec = bRec;

	m_displayinfo.data[0] = x1;
	m_displayinfo.data[1] = y1;
	m_displayinfo.data[2] = x2;
	m_displayinfo.data[3] = y2;
	m_displayinfo.data_len = 4;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&m_displayinfo, sizeof(NET_DISPLAY_INFO), msgData);	
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CTRL_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));
		memcpy(&m_displayinfo, pDispInfo, sizeof(NET_DISPLAY_INFO));
	}
	m_pMsgMan->ReleaseMsgBuff(msgData);
}

//int GetSplitIndex(unsigned long splitMode)
//{
//	int index = 0;
//
//	if((splitMode >= VIEW_SPLIT_1A11) && (splitMode <= VIEW_SPLIT_6X6))
//	{
//		index = 7;
//		return index;
//	}
//
//	if((splitMode >= VIEW_SPLIT_1A9) && (splitMode <= VIEW_SPLIT_5X5))
//	{
//		index = 6;
//		return index;
//	}
//
//	if((splitMode >= VIEW_SPLIT_2A6) && (splitMode <= VIEW_SPLIT_4X6))
//	{
//		index = 5;
//		return index;
//	}
//
//	if((splitMode >= VIEW_SPLIT_1A7) && (splitMode <= VIEW_SPLIT_4X4))
//	{
//		index = 4;
//		return index;
//	}
//
//	if((splitMode >= VIEW_SPLIT_1A5) && (splitMode <= VIEW_SPLIT_3X3))
//	{
//		index = 3;
//		return index;
//	}
//
//	if((splitMode >= VIEW_SPLIT_1A2) && (splitMode <= VIEW_SPLIT_2X3))
//	{
//		index = 2;
//		return index;
//	}
//
//	if(splitMode == VIEW_SPLIT_2X2)
//	{
//		index = 1;
//		return index;
//	}
//
//	if(splitMode == VIEW_SPLIT_1X1)
//	{
//		index = 0;
//		return index;
//	}
//
//	assert(false);
//	return index;
//}

bool CDisktop::ChangeDisplayToDwellGroup()
{
	int index_split = GetSplitIndex(m_pWndStartupInfo->SplitMode);

	unsigned long count = m_pWndStartupInfo->SplitData[index_split][0];		//总数据组数
	unsigned long maxGroup = m_pWndStartupInfo->SplitData[index_split][1];	//有效数据组数

	ULONGLONG effectCH = 0;
	int videoInputNum = 0, localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	IsSupportCH(effectCH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	videoInputNum = localVideoInputNum + netVideoInputNum;

	memset(&m_displayinfo, 0 ,sizeof(NET_DISPLAY_INFO));
	m_displayinfo.type = NET_DISPLAY_CHANGE_DATA_PACK;
	m_displayinfo.rec = 0x1;
	m_displayinfo.time_out = 0;
	m_displayinfo.split_mode[DISPLAY_LAYER_ANALOG] = m_pWndStartupInfo->SplitMode;
	m_displayinfo.split_mode[DISPLAY_LAYER_NET] = m_pWndStartupInfo->SplitMode;

	if(maxGroup > 0)
	{
		int index = 0;
		int splitNum = SplitModeToNum(static_cast<VIEW_SPLIT_MODE>(m_pWndStartupInfo->SplitMode));
		for(int i = 0; i < splitNum; i++)
		{
			unsigned long chnnValue = m_pWndStartupInfo->SplitData[index_split][i + 2];
			if(chnnValue < videoInputNum)
			{
				if(chnnValue < localVideoInputNum)
				{
					m_displayinfo.data[index] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, i, chnnValue);
				}
				else
				{
					chnnValue -= localVideoInputNum;
					m_displayinfo.data[index] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, i, chnnValue);
				}
				index++;
			}
		}
		m_displayinfo.data_len = index;
	}

	if(m_displayinfo.data_len > 0)
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		m_pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&m_displayinfo, sizeof(NET_DISPLAY_INFO), msgData);	
		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		if (CMD_REPLY_CTRL_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
		{
			NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));
			memcpy(&m_displayinfo, pDispInfo, sizeof(NET_DISPLAY_INFO));
		}
		m_pMsgMan->ReleaseMsgBuff(msgData);

		return true;
	}
	
	return false;
}

//ULONGLONG CDisktop::GetMainMonitorCfg()
//{
//	ULONGLONG authCH = 0x0;
//
//	int index_split = GetSplitIndex(m_pWndStartupInfo->SplitMode);
//
//	unsigned long count = m_pWndStartupInfo->SplitData[index_split][0];		//总数据组数
//	unsigned long maxGroup = m_pWndStartupInfo->SplitData[index_split][1];	//有效数据组数
//
//	unsigned long numGroup = SplitModeToNum(static_cast<VIEW_SPLIT_MODE>(m_pWndStartupInfo->SplitMode));	//每组个数
//
//	for(int i = 0; i < numGroup * maxGroup; i++)
//	{
//		unsigned long chnnValue = m_pWndStartupInfo->SplitData[index_split][i + 2];
//		if(chnnValue < g_p_login_succ->productInfo.videoInputNum)
//		{
//			authCH |= (static_cast<ULONGLONG>(0x1) << chnnValue);
//		}
//	}
//
//	return authCH;
//}

void CDisktop::HideSnapTip()
{
	bool bShowSnapTip = true, bSucc = true;
	CMessageMan::Instance()->ShowSnapTip(bShowSnapTip, bSucc);

	if(bShowSnapTip)
	{
		CMessageMan::Instance()->SetShowSnapTip(false, bSucc);
	}
}

bool CDisktop::IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1 >= x2) ? (x1-x2) : (x2-x1);
	unsigned long y = (y1 >= y2) ? (y1-y2) : (y2-y1);

	if(((x*x) + (y*y)) <= distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

unsigned long CDisktop::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
#if defined(__CUSTOM_US02__) && (defined(__TDFH__) || defined(__CHIP3531__) || defined(__CHIPGM__) || defined(__CHIP3535__) || defined(__NOMENU_OUT_INFO__) || defined(__CHIPTI__))
	if (KEY_VIRTUAL_MOVE != keyID)
	{
		if (!m_bNoMenuMsg)
		{
			//const char * pStr = CStringTable::Instance()->String(E_STR_ID_VIEW_MENU);
			CPicCreate pic;
			pic.CreatePic("To view menu:\nPush and hold \"STOP or ESC\" button until you hear a beep.", 600, 400, GUI_RGB(255, 255, 0));
			pic.SetPicToAuxiliaryOutputDevice();

			m_bNoMenuMsg = true;
		}
		m_startTime = GetCurrTime32();
	}
#endif

#ifdef __DVR_MENU__
	if ((KEY_VIRTUAL_1X1 <= keyID) && (keyID <= KEY_VIRTUAL_6X6) && m_mainMenu.IsShow())
#else
	if ((KEY_VIRTUAL_1X1 <= keyID) && (keyID <= KEY_VIRTUAL_6X6) && (m_toolBar.IsShow() || m_mainMenu.IsShow()))
#endif
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return keyID;
	}
}

//////////////////////////////////////////////////////////////////////////

