/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-15
** Name         : CfgLiveManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgNewLiveManDlg.h"
#include "MessageMan.h"
#include "FuncCustom.h"
 extern unsigned char g_coverAreaCountLimit;
CCfgNewLiveManDlg::CCfgNewLiveManDlg() : m_curPage( PAGE_LIVE ),
m_initAnchorPage(PAGE_LIVE),
m_initPageMask(0)
{

}

CCfgNewLiveManDlg::~CCfgNewLiveManDlg()
{

}

void CCfgNewLiveManDlg::OnInitial()
{

	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK |  DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DEFAULT) );
	//////////////////////////////////////////////////////////////////////////

	m_initPageMask = 0x1;

#ifndef __DVR_ANCHOR__
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_LIVE), false);
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	m_cfgLiveDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 2);
//#ifdef __DVR_BASIC__
//	m_cfgLiveHostDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy,this,0,3);
//#endif
//	m_cfgLiveSpotDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy,this,0,4);
//	m_cfgCoverDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 5);

	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
	int pageIndex = 0;
	m_tabCtrl.AddItem(&m_cfgLiveDlg, m_pStrTable->String( E_STR_ID_LIVE ) );
	m_arrPages[pageIndex++] = PAGE_LIVE;
#ifdef __DVR_BASIC__
	m_tabCtrl.AddItem(&m_cfgLiveHostDlg,m_pStrTable->String(E_STR_ID_HOST_MONITOR));
	m_arrPages[pageIndex++] = PAGE_HOST;
#endif
	if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		if (IsSupportSpotOut())
		{
			m_tabCtrl.AddItem(&m_cfgLiveSpotDlg,m_pStrTable->String(E_STR_ID_SPOT));
			m_arrPages[pageIndex++] = PAGE_SPOT;
		}
	}
	
/*	if(g_coverAreaCountLimit !=0)
	{
		m_tabCtrl.AddItem(&m_cfgCoverDlg,m_pStrTable->String(E_STR_ID_MASK));
		m_arrPages[pageIndex++] = PAGE_COVER;
	}*/
	
	////
	if(s_loginSuccessReply.productInfo.netVideoInputNum > 0)
	{
		m_tabCtrl.AddItem(&m_cfgROIDlg,m_pStrTable->String(E_STR_ID_IMAGE_ROI));
		m_arrPages[pageIndex++] = PAGE_ROI;

		m_tabCtrl.AddItem(&m_cfgCamParaDlg,m_pStrTable->String(E_STR_ID_CAMERA_PARAMTER));
		m_arrPages[pageIndex++] = PAGE_CAM_PARA;
	}

	m_curPage = PAGE_LIVE;
	m_tabCtrl.SetCurItem(m_curPage , false );
	CCfgDlg::SetRefreshView( false );

#else
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	switch (m_initAnchorPage)
	{
	case PAGE_LIVE:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_LIVE), false);
		m_cfgLiveDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#ifdef __DVR_BASIC__
	case PAGE_HOST:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_MAIN_MONITOR), false);
		m_cfgLiveHostDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
#endif
	case PAGE_SPOT:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SPOT), false);
		m_cfgLiveSpotDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	/*case PAGE_COVER:
		if(0 !=  g_coverAreaCountLimit)
		{
			SetCaption(m_pStrTable->String(E_STR_ID_TITLE_MASK), false);
			m_cfgCoverDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		}
		break;*/
	case PAGE_ROI:
		if(s_loginSuccessReply.productInfo.netVideoInputNum > 0){
			SetCaption(m_pStrTable->String(E_STR_ID_IMAGE_ROI), false);
			m_cfgROIDlg.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
		}
		break;
	case PAGE_CAM_PARA:
		if(s_loginSuccessReply.productInfo.netVideoInputNum > 0){
			SetCaption(m_pStrTable->String(E_STR_ID_CAMERA_PARAMTER), false);
			m_cfgCamParaDlg.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
		}
		break;
	default:
		assert(false);
		break;
	}

	m_curPage = m_initAnchorPage;
	CCfgDlg::SetRefreshView( false );
#endif

	//create tip
	CreateTipWnd();
}


void CCfgNewLiveManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_LIVE_REC_STATUS);
	itemList.push_back(NCFG_ITEM_CAMERA_NAME);
	itemList.push_back(NCFG_ITEM_SHOW_CAM_NAME);

#ifdef __DVR_BASIC__
	itemList.push_back(NCFG_ITEM_VIEW_SPLIT_MODE);
	itemList.push_back(NCFG_ITEM_VIEW_DWELL_TIME);
	itemList.push_back(NCFG_ITEM_VIEW_SPLIT_1X1);
	itemList.push_back(NCFG_ITEM_VIEW_SPLIT_2X2);
	itemList.push_back(NCFG_ITEM_VIEW_SPLIT_2X3);
	itemList.push_back(NCFG_ITEM_VIEW_SPLIT_3X3);
	itemList.push_back(NCFG_ITEM_VIEW_SPLIT_4X4);
	itemList.push_back(NCFG_ITEM_VIEW_SPLIT_5X5);
	itemList.push_back(NCFG_ITEM_VIEW_SPLIT_6X6);
#endif

	itemList.push_back(NCFG_ITEM_SPOT_SPLIT_MODE);
	itemList.push_back(NCFG_ITEM_SPOT_DWELL_TIME);

	itemList.push_back(NCFG_ITEM_SPOT_SPLIT_1X1);

	itemList.push_back(NCFG_ITEM_VIDEO_COLOR);

	itemList.push_back(NCFG_ITEM_CAM_COVER_INFO);
	if (s_loginSuccessReply.productInfo.netVideoInputNum > 0){
		itemList.push_back(NCFG_ITEM_CAM_ROI_INFO);
		itemList.push_back(NCFG_ITEM_CAM_MIRROR);
		itemList.push_back(NCFG_ITEM_CAM_FILP);
		itemList.push_back(NCFG_ITEM_CAM_ROTATO);
		itemList.push_back(NCFG_ITEM_CAM_LENS_DISTORTION);
		itemList.push_back(NCFG_ITEM_CAM_TIME_FOCUS_MODE);
		itemList.push_back(NCFG_ITEM_CAM_IRCHANGE_FOCUS);
		itemList.push_back(NCFG_ITEM_CAM_INTERVAL_TIME);
	}
	// read only
	itemList.push_back(NCFG_ITEM_CAMERA_NAME_MAX_LEN);
}

unsigned long CCfgNewLiveManDlg::OnApply()
{
	//Load default
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	unsigned long ret = OnLoadDefault(pCfgDlg);

	if( pCfgDlg->IsModify() )
	{
		EnableOkBtn(true,false);
	}
	else
	{
		EnableOkBtn(false,false);
	}

	ENUM_STRING_ID strID = ret ? static_cast<ENUM_STRING_ID>(ret) : E_STR_ID_TIP_00_03;
	MessageOut(strID,TIP_MSG,false);

	//一次重绘全部
	Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

#ifdef __DVR_BASIC__
unsigned long CCfgNewLiveManDlg::OnOK()
{
	bool bReDisplay = false;

	if(m_curPage == PAGE_HOST)
	{
		if( IsChange(NCFG_ITEM_VIEW_SPLIT_MODE) ||
			IsChange(NCFG_ITEM_VIEW_SPLIT_1X1) ||
			IsChange(NCFG_ITEM_VIEW_SPLIT_2X2) ||
			IsChange(NCFG_ITEM_VIEW_SPLIT_2X3) ||
			IsChange(NCFG_ITEM_VIEW_SPLIT_3X3) ||
			IsChange(NCFG_ITEM_VIEW_SPLIT_4X4) ||
			IsChange(NCFG_ITEM_VIEW_SPLIT_5X5) ||
			IsChange(NCFG_ITEM_VIEW_SPLIT_6X6))
		{
			bReDisplay = true;
		}
	}

	if((m_curPage == PAGE_HOST) && (0 != m_cfgLiveHostDlg.CheckDwell()))
	{
		GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_TIP_02_03), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);
	}
	else
	{
		CCfgDlg::OnOK();

		if(bReDisplay)
		{
			//仅可以在DVR端配主输出,因此只在此处刷新显示
			bool bGetInfo = false;

			NET_DISPLAY_INFO dispInfo;
			memset(&dispInfo, 0, sizeof(NET_DISPLAY_INFO));
			dispInfo.type = NET_DISPLAY_GET_INFO;

			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_DISPLAY_INFO, (unsigned char*)&dispInfo, sizeof(NET_DISPLAY_INFO), msgData);
			assert(NULL != msgData.pData);
			PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
			if (CMD_REPLY_CTRL_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
			{
				//获取当前显示的通道
				NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));
				memcpy(&dispInfo, pDispInfo, sizeof(NET_DISPLAY_INFO));

				bGetInfo = true;
			}
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			if(bGetInfo)
			{
				dispInfo.type = NET_DISPLAY_CHANGE_DATA_PACK;

				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&dispInfo, sizeof(NET_DISPLAY_INFO), msgData);
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				CMessageMan::Instance()->RefreshLiveTrue();
			}
		}
	}

	return KEY_VIRTUAL_MSG_STOP;

}

unsigned long CCfgNewLiveManDlg::OnCancel()
{
	GUI::DLG_END_VALUE result;

	if((m_curPage == PAGE_HOST) && (0 != m_cfgLiveHostDlg.CheckDwell()))
	{
		result = GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_TIP_02_03), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK| DEFAULT_BTN_CANCEL | DEFAULT_ICON_ERROR);

		if(result != DLG_OK)
		{
			CDialog::EndDialog(result);
		}
	}
	else
	{
		return CCfgDlg::OnCancel();
	}

	return KEY_VIRTUAL_MSG_STOP;
}
#endif
unsigned long CCfgNewLiveManDlg::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

CCfgDlg* CCfgNewLiveManDlg::GetPage(PAGE_ITEM item)
{
	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_LIVE:
		pCfgDlg = &m_cfgLiveDlg;
		break;
#ifdef __DVR_BASIC__
	case PAGE_HOST:
		pCfgDlg = &m_cfgLiveHostDlg;
		break;
#endif
	case PAGE_SPOT:
		if (IsSupportSpotOut())
		{
			pCfgDlg = &m_cfgLiveSpotDlg;
		}
		/*else
		{
			if(0 !=  g_coverAreaCountLimit)
			{
				pCfgDlg = &m_cfgCoverDlg;
			}
			
		}*/
		break;
	/*case PAGE_COVER:
		if(0 !=  g_coverAreaCountLimit)
		{
			pCfgDlg = &m_cfgCoverDlg;
		}
		break;*/
	case PAGE_ROI:
		if(s_loginSuccessReply.productInfo.netVideoInputNum > 0){
			pCfgDlg = &m_cfgROIDlg;
		}
		break;
	case PAGE_CAM_PARA:
		if (s_loginSuccessReply.productInfo.netVideoInputNum > 0){
			pCfgDlg = &m_cfgCamParaDlg;
		}
		break;
	default:
		assert(false);
		break;
	}
	return pCfgDlg;
}

void CCfgNewLiveManDlg::SetAnchorPage(PAGE_ITEM item)
{
	m_initAnchorPage = item;
}

BEGIN_MSG_MAP(CCfgNewLiveManDlg,CCfgDlg)
	ON_MSG(m_cfgLiveDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickLive)
#ifdef __DVR_BASIC__
	ON_MSG(m_cfgLiveHostDlg.GetID(),KEY_VIRTUAL_ENTER,OnClickHost)
#endif
	ON_MSG(m_cfgLiveSpotDlg.GetID(),KEY_VIRTUAL_ENTER,OnClickSpot)
	// ON_MSG(m_cfgCoverDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickCover)
	ON_MSG(m_cfgROIDlg.GetID(),KEY_VIRTUAL_ENTER, OnClickROI)
	ON_MSG(m_cfgCamParaDlg.GetID(),KEY_VIRTUAL_ENTER,OnClickCamPara)
END_MSG_MAP()

unsigned long CCfgNewLiveManDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PAGE_ITEM curPage = m_curPage;
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		{
			unsigned short x = m_length[DLG_OFFSET_LEFT];
			unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
			unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
			unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

			curPage = m_arrPages[m_tabCtrl.GetFocusItem()];
#ifdef __DVR_BASIC__
			if(!((m_initPageMask >> PAGE_HOST) & 0x1) && (PAGE_HOST == curPage))
			{
				m_cfgLiveHostDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_HOST + 2);

				SetSubCfgDlg(&m_cfgLiveHostDlg);
				m_initPageMask |= (0x1 << PAGE_HOST);
			}
#endif
			if(CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum > 0)
			{
				if (IsSupportSpotOut())
				{
					if(!((m_initPageMask >> PAGE_SPOT) & 0x1) && (PAGE_SPOT == curPage))
					{
						m_cfgLiveSpotDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_SPOT + 2);

						SetSubCfgDlg(&m_cfgLiveSpotDlg);
						m_initPageMask |= (0x1 << PAGE_SPOT);
					}
				}
				/*else
				{
					if(!((m_initPageMask >> PAGE_SPOT) & 0x1) && (PAGE_SPOT == curPage))
					{
						if(g_coverAreaCountLimit !=0)
						{
							m_cfgCoverDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_SPOT + 2);
							SetSubCfgDlg(&m_cfgCoverDlg);
							m_initPageMask |= (0x1 << PAGE_SPOT);
						}
					}
				}*/
			}

			/*if(!((m_initPageMask >> PAGE_COVER) & 0x1) && (PAGE_COVER == curPage))
			{
				 if(g_coverAreaCountLimit != 0)
				 {
					 m_cfgCoverDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_COVER + 2);
					 SetSubCfgDlg(&m_cfgCoverDlg);
					 m_initPageMask |= (0x1 << PAGE_COVER);
				 }
			}*/
			if (!((m_initPageMask >> PAGE_ROI) & 0x1) && (PAGE_ROI == curPage))
			{
				if (s_loginSuccessReply.productInfo.netVideoInputNum > 0){
					m_cfgROIDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_ROI+2);
					SetSubCfgDlg(&m_cfgROIDlg);
					m_initPageMask |= (0x1 << PAGE_ROI);
				}
			}

			if (!((m_initPageMask >> PAGE_CAM_PARA) & 0x1) && (PAGE_CAM_PARA == curPage))
			{
				if (s_loginSuccessReply.productInfo.netVideoInputNum > 0){
					m_cfgCamParaDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_CAM_PARA+2);
					SetSubCfgDlg(&m_cfgCamParaDlg);
					m_initPageMask |= (0x1 << PAGE_CAM_PARA);
				}
			}
		}

		if (m_curPage != curPage)
		{
			unsigned long ret = 0;
			if (GetPage(m_curPage)->IsModify())
			{
				ret = TrySaveChange(GetPage(m_curPage),true,false,false);
			}

			if ( ret == 0 )
			{
				m_curPage = curPage;
			}
			else
			{
				Repaint();
				return KEY_VIRTUAL_MSG_STOP;
			}
		}	
	}

	return keyID;
}

unsigned long CCfgNewLiveManDlg::OnClickLive()
{
	if (m_cfgLiveDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNewLiveManDlg::OnClickHost()
{
#ifdef __DVR_BASIC__
	if( m_cfgLiveHostDlg.IsModify() )
	{
		EnableOkBtn(true, true);
	}
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNewLiveManDlg::OnClickSpot()
{
	if( m_cfgLiveSpotDlg.IsModify() )
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

/*unsigned long CCfgNewLiveManDlg::OnClickCover()
{
	 if(0 !=  g_coverAreaCountLimit)
	 {
		 if( m_cfgCoverDlg.IsModify() )
		 {
			 EnableOkBtn(true, true);
		 }
	 }
	return KEY_VIRTUAL_MSG_STOP;
}
*/

unsigned long CCfgNewLiveManDlg::OnClickROI()
{
	if (m_cfgROIDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNewLiveManDlg::OnClickCamPara()
{
	if (m_cfgCamParaDlg.IsModify())
	{
		EnableOkBtn(true,true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CCfgNewLiveManDlg::OnVirtualEsc(unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if((m_curPage == PAGE_HOST) && (0 != m_cfgLiveHostDlg.CheckDwell()))
	{
		GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_TIP_02_03), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);
		return KEY_VIRTUAL_MSG_STOP;
	}

	return GUI::CDialog::OnVirtualEsc(keyID, xPos, yPos);
}


//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

CCfgNewLiveManAnchorDlg::CCfgNewLiveManAnchorDlg()
{

}

CCfgNewLiveManAnchorDlg::~CCfgNewLiveManAnchorDlg()
{

}

void CCfgNewLiveManAnchorDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_LIVE), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 200;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;

	m_anLive.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_LIVE));
	m_anLive.SetImage(BMP_ANCHOR_LIVE, BMP_ANCHOR_LIVE_F, BMP_ANCHOR_LIVE);

#ifdef __DVR_BASIC__
	y += 50;
	m_anLiveHost.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_HOST_MONITOR));
	m_anLiveHost.SetImage(BMP_ANCHOR_MAIN_MONITOR, BMP_ANCHOR_MAIN_MONITOR_F, BMP_ANCHOR_MAIN_MONITOR);
#endif

	if (IsSupportSpotOut() && (s_loginSuccessReply.productInfo.localVideoInputNum > 0))
	{
		y += 50;
		m_anLiveSpot.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy , this, 0, yFocus++, m_pStrTable->String(E_STR_ID_SPOT));
		m_anLiveSpot.SetImage(BMP_ANCHOR_SPOT, BMP_ANCHOR_SPOT_F, BMP_ANCHOR_SPOT);
	}
	
	if(0 !=  g_coverAreaCountLimit)
	{
		y += 50;
		m_anCover.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MASK));
		m_anCover.SetImage(BMP_ANCHOR_MASK, BMP_ANCHOR_MASK_F, BMP_ANCHOR_MASK);
	}
	if (s_loginSuccessReply.productInfo.netVideoInputNum > 0){
		y += 50;
		m_anROI.Create(GUI::CWnd::GetRandWndID(),x+3,y,cx,cy,this,0, yFocus++, m_pStrTable->String(E_STR_ID_IMAGE_ROI));
		m_anROI.SetImage(BMP_ANCHOR_ROI, BMP_ANCHOR_ROI_F, BMP_ANCHOR_ROI);

		y += 50;
		m_anCamPara.Create(GUI::CWnd::GetRandWndID(),x+3,y,cx,cy,this,0,yFocus++,m_pStrTable->String(E_STR_ID_CAMERA_PARAMTER));
		m_anCamPara.SetImage(BMP_ANCHOR_CAM_PARA, BMP_ANCHOR_CAM_PARA_F, BMP_ANCHOR_CAM_PARA);
	}

	y += 50;
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
}

BEGIN_MSG_MAP(CCfgNewLiveManAnchorDlg, GUI::CDialog)
	ON_MSG(m_anLive.GetID(), KEY_VIRTUAL_ENTER, OnAnchorLive)
	ON_MSG(m_anLiveHost.GetID(), KEY_VIRTUAL_ENTER, OnAnchorLiveHost)
	ON_MSG(m_anLiveSpot.GetID(), KEY_VIRTUAL_ENTER, OnAnchorLiveSpot)
	// ON_MSG(m_anCover.GetID(), KEY_VIRTUAL_ENTER, OnAnchorCover)
	ON_MSG(m_anROI.GetID(),KEY_VIRTUAL_ENTER, OnAnchorROI)
	ON_MSG(m_anCamPara.GetID(),KEY_VIRTUAL_ENTER,OnAnchorCamPara)
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
END_MSG_MAP()

unsigned long CCfgNewLiveManAnchorDlg::OnAnchorLive()
{
	m_cfgLiveManDlg.SetAnchorPage(CCfgNewLiveManDlg::PAGE_LIVE);
	m_cfgLiveManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNewLiveManAnchorDlg::OnAnchorLiveHost()
{
#ifdef __DVR_BASIC__
	m_cfgLiveManDlg.SetAnchorPage(CCfgNewLiveManDlg::PAGE_HOST);
	m_cfgLiveManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNewLiveManAnchorDlg::OnAnchorLiveSpot()
{
	m_cfgLiveManDlg.SetAnchorPage(CCfgNewLiveManDlg::PAGE_SPOT);
	m_cfgLiveManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNewLiveManAnchorDlg::OnAnchorCover()
{
	if(0 !=  g_coverAreaCountLimit)
	{
		m_cfgLiveManDlg.SetAnchorPage(CCfgNewLiveManDlg::PAGE_COVER);
		m_cfgLiveManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNewLiveManAnchorDlg::OnAnchorROI()
{
	if (s_loginSuccessReply.productInfo.netVideoInputNum > 0){
		m_cfgLiveManDlg.SetAnchorPage(CCfgNewLiveManDlg::PAGE_ROI);
		m_cfgLiveManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNewLiveManAnchorDlg::OnAnchorCamPara()
{
	if (s_loginSuccessReply.productInfo.netVideoInputNum > 0){
		m_cfgLiveManDlg.SetAnchorPage(CCfgNewLiveManDlg::PAGE_CAM_PARA);
		m_cfgLiveManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
	}
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CCfgNewLiveManAnchorDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

//end
