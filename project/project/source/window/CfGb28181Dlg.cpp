/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-11
** Name         : CfgBasicManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfGb28181Dlg.h"
#include "FuncCustom.h"

//////////////////////////////////////////////////////////////////////////

CCfGb28281Dlg::CCfGb28281Dlg() :m_curPage(PAGE_SIPSERVER),
m_initAnchorPage(PAGE_SIPSERVER),
m_initPageMask(0)
{

}

CCfGb28281Dlg::~CCfGb28281Dlg()
{

}

void CCfGb28281Dlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	//SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));
	SetDefaultBtnCaption( DEFAULT_BTN_OK,m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY,m_pStrTable->String(E_STR_ID_DEFAULT) );
	//////////////////////////////////////////////////////////////////////////

	m_initPageMask = 0x1;

#ifndef __DVR_ANCHOR__
	SetCaption("Gb28181", false);
	////

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	m_cfgSipServerDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0,2);


	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);

	m_tabCtrl.AddItem(&m_cfgSipServerDlg, "Sip Server" );
	m_tabCtrl.AddItem(&m_cfgDvrIdDlg, "Dvr Channel" );
	
	m_tabCtrl.AddItem(&m_cfgAlarmIdDlg, "Dvr Alarm" );
	m_curPage = PAGE_SIPSERVER;
	m_tabCtrl.SetCurItem(m_curPage , false );
	CCfgDlg::SetRefreshView( false );
	/////////////////////////////////
#else
	////

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	switch (m_initAnchorPage)
	{
	case PAGE_SIPSERVER:
		SetCaption("Sip Server", false);
		m_cfgSipServerDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_CHANNEL:
		SetCaption("Dvr Channel", false);
		m_cfgDvrIdDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_ALARM:
		SetCaption("Dvr Alarm", false);
		m_cfgAlarmIdDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	default:
		assert(false);
		break;
	}

	m_curPage = m_initAnchorPage;
	CCfgDlg::SetRefreshView( false );
#endif

	CreateTipWnd();
}

unsigned long CCfGb28281Dlg::OnApply()
{
	printf("CCfGb28281Dlg::OnApply()\n");
	//Load default
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);
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

unsigned long CCfGb28281Dlg::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

void CCfGb28281Dlg::GetItemList(CFGITEM_LIST& itemList) const
{
	//if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	//{
	//	itemList.push_back(NCFG_ITEM_ENCODE_MINOR);
	//}
//#ifdef __DVR_BASIC__
//	//itemList.push_back(NCFG_ITEM_JPEG_PERIOD);	
//#endif
//	if(s_loginSuccessReply.productInfo.netVideoInputNum == 0)
//	{
//		itemList.push_back(NCFG_ITEM_UPNP_ENABLE);
//	}
//	itemList.push_back(NCFG_ITEM_HTTP_PORT);
//	itemList.push_back(NCFG_ITEM_SERVER_PORT);
//
//	itemList.push_back(NCFG_ITEM_NETWORK_ADDR_INFO);
//	itemList.push_back(NCFG_ITEM_DNS_1);
//	itemList.push_back(NCFG_ITEM_DNS_2);
//
//	itemList.push_back(NCFG_ITEM_DDNS_INFO);
//	itemList.push_back(NCFG_ITEM_PPPOE_INFO);
//#ifdef __DVR_BASIC__	
//	itemList.push_back(NCFG_ITEM_EMAIL_SEND_INFO);
//	itemList.push_back(NCFG_ITEM_EMAIL_RECV_INFO);
//	itemList.push_back(NCFG_ITEM_EMAIL_PIC_ACC);
//#endif
//
//	//DDNS更新周期
//	itemList.push_back(NCFG_ITEM_DDNS_UPDATE_CYCLE);
//
//#ifdef __SUPPORT_HRSST__	
//	itemList.push_back(NCFG_ITEM_HRSST_SERVER_IP);
//	itemList.push_back(NCFG_ITEM_HRSST_SERVER_PORT);
//#endif
//	itemList.push_back(NCFG_ITEM_NET_WIRELESS);
//
//	itemList.push_back(NCFG_ITEM_AUTO_REPORT);
//
//	itemList.push_back(NCFG_ITEM_MINT_DDNS_SERVER_NAME);
//	itemList.push_back(NCFG_ITEM_USE_3G);
//#if defined (__CUSTOM_IPV6__)
//	itemList.push_back(NCFG_ITEM_USE_IPV4_IPV6);
//	itemList.push_back(NCFG_ITEM_NETWORK_ADDR_INFO_IPV6);
//	itemList.push_back(NCFG_ITEM_DNS_1_IPV6);
//	itemList.push_back(NCFG_ITEM_DNS_2_IPV6);
//#endif
//
//#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
//	itemList.push_back(NCFG_ITEM_NET_FTP);
//#endif
//
//#ifdef __MOBILE_DVR__
//	itemList.push_back(NCFG_ITEM_NET_SERVER);
//	itemList.push_back(NCFG_ITEM_WIRELESS_WIFI);
//	itemList.push_back(NCFG_ITEM_WIRELESS_COMM);
//#endif
//
//
//	////read only
//	itemList.push_back(NCFG_ITEM_PPPOE_USER_NAME_MAX_LEN);
//	itemList.push_back(NCFG_ITEM_PPPOE_PASSWORD_MAX_LEN);
//	itemList.push_back(NCFG_ITEM_DDNS_USER_NAME_MAX_LEN);
//	itemList.push_back(NCFG_ITEM_DDNS_PASSWORD_MAX_LEN);
//	itemList.push_back(NCFG_ITEM_DDNS_DOMAIN_MAX_LEN);
//	itemList.push_back(NCFG_ITEM_DDNS_SERVER_INFO);
//
//	itemList.push_back(NCFG_ITEM_ENCODE_MINOR_SUPPORT_PROPERTY);
//	itemList.push_back(NCFG_ITEM_MINOR_SUPPORT_QULITY_LEVELNUM);
//	itemList.push_back(NCFG_ITEM_MINOR_SUPPORT_ENCODEMODE);
//	itemList.push_back(NCFG_ITEM_MINOR_SUPPORT_BITRATE_RANGE);
//	itemList.push_back(NCFG_ITEM_ENCODE_SIZE_MINOR_ALL_CH);
//
//	itemList.push_back(NCFG_ITEM_EMAIL_IMG_NUM_RANGE);
//	itemList.push_back(NCFG_ITEM_EMAIL_SNAP_TIME_RANGE);

	itemList.push_back(NCFG_ITEM_GB28181_SERVER_INFO);
	itemList.push_back(NCFG_ITEM_GB28181_CHANNEL_ID);
	itemList.push_back(NCFG_ITEM_GB28181_ALARM_ID);
}

CCfgDlg* CCfGb28281Dlg::GetPage(PAGE_ITEM item)
{
	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_SIPSERVER:
		pCfgDlg = &m_cfgSipServerDlg;
		break;
	case PAGE_CHANNEL:
		pCfgDlg = &m_cfgDvrIdDlg;
		break;
	case PAGE_ALARM:
		pCfgDlg = &m_cfgAlarmIdDlg;
		break;

	default:
		assert(false);
		break;
	}
	return pCfgDlg;
}

void CCfGb28281Dlg::OnDestroy()
{
	CCfgDlg::OnDestroy();
}
void CCfGb28281Dlg::SetAnchorPage(PAGE_ITEM item)
{
	m_initAnchorPage = item;
}

BEGIN_MSG_MAP(CCfGb28281Dlg,CCfgDlg)
	ON_MSG(m_cfgSipServerDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickSipServer)
	ON_MSG(m_cfgDvrIdDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickChannel)
	ON_MSG(m_cfgAlarmIdDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickAlarm)
END_MSG_MAP()



unsigned long CCfGb28281Dlg::TrySaveChange(CCfgDlg* pCfgDlg, bool bChooseDlg, bool bHide,bool bRefresh)
{
	gui_assert(pCfgDlg->IsModify(),printf("Make sure,the parameter pCfgDlg is modified.\n"));

	//////////刷新差异链表,并检查是否真正有变动
	RefreshChangeItem(false);

	if( m_saveItemList.size() <= 0 )
	{
		EnableOkBtn(false, true);
		for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
		{
			(*cfg_iter)->SetModify(false);
		}
		//char str[64];
		//strcpy(str,"Ok.No setting is changed.");
		MessageOut(GetTopestWnd(),m_pStrTable->String(E_STR_ID_TIP_00_05),TIP_MSG,false);

		//[2010-03-02 17:49:00 L] 尝试保存操作后, 即使没有变动也需要重新刷新页面内容
		pCfgDlg->UpdateView( false );

		if( !bHide && bRefresh )
		{
			Repaint();
		}

		return 0; //如果没有变动,就提前退出
	}

	//////////尝试保存数据

	unsigned long retValue = 0;
	//bool reset = false;
	DLG_END_VALUE retDlg = DLG_OK;
	unsigned long retKey = pCfgDlg->PreCheckValue();//此处结果仅用于重要提示与否,如果后期有更多需求,可以修改此处
	if( bChooseDlg || 0 != retKey )
	{
		//重要提示,强制性弹出
		std::string str;

		if( retKey != 0 )
		{
			str += m_pStrTable->String(static_cast<ENUM_STRING_ID>(retKey));
			str += "\\n";
		}

		if( bChooseDlg )
		{
			str += m_pStrTable->String(E_STR_ID_MESSAGE_38);
		}

		//GUI::CMessageBox box(str.c_str(), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_CANCEL|DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		//if (bHide)
		//{
		//Show(false,true);
		//}
		//retDlg = box.DoModal( this);
	}

	if ( DLG_OK == retDlg )
	{
		bool msgOut = true;
		unsigned long ret = pCfgDlg->CheckValue();

		if( 0 == ret )
		{
			//保存操作
			if (m_bCreateCfgBlock)
			{
				RefreshChangeItem(true, pCfgDlg);
				SaveConfig();
			}
			else
			{
				gui_assert(false,printf("Please create config block first!\n"));
			}

			//确认保存,且保存成功,修改页面的状态
			EnableOkBtn(false, true);
			for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
			{
				(*cfg_iter)->SetModify(false);
			}
			//if( IsRefreshView() )
			{
				pCfgDlg->UpdateView( false );
			}

			retValue = ret;
		}
		else
		{
			if( bChooseDlg )
			{
				std::string str = m_pStrTable->String(static_cast<ENUM_STRING_ID>(ret));
				str += m_pStrTable->String(E_STR_ID_TIP_00_06);
				//GUI::CMessageBox box(str.c_str(), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_CANCEL|DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
				//if (bHide)
				//{
				//	Show(false,true);
				//}

				//if( DLG_OK == box.DoModal( this) )
				//{
				//	retValue = ret;
				//}
				/*else
				{*/
				EnableOkBtn(false, true);
				for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
				{
					(*cfg_iter)->SetModify(false);
				}
				//if( IsRefreshView() )
				{
					pCfgDlg->UpdateView( false );
				}

				msgOut = false;
				retValue = 0;
				//}
			}
			else
			{
				retValue = ret;
			}
		}

		//在确认保存后.根据返回值弹出提示
		if( msgOut )
		{
			if(  ret != 0 )
			{
				MessageOut(GetTopestWnd(),static_cast<ENUM_STRING_ID>(ret),TIP_MSG,false);
			}
			else
			{
				MessageOut(GetTopestWnd(),E_STR_ID_TIP_00_01,TIP_MSG,false);
			}
		}
	}
	else
	{
		//取消保存后,修改页面的状态
		EnableOkBtn(false, true);
		for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
		{
			(*cfg_iter)->SetModify(false);
		}
		//if( IsRefreshView() )
		{
			pCfgDlg->UpdateView( false );
		}

		//恢复操作中,可以直接读取界面上的值[上一步已经将页面值恢复了]进行恢复
		unsigned long rec = pCfgDlg->RecoverChange();

		retValue = 0;
	}

	if( !bHide && bRefresh )
	{
		Repaint();
	}

	return retValue;
}

unsigned long CCfGb28281Dlg::OnClickSipServer()
{
	if (m_cfgSipServerDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CCfGb28281Dlg::OnClickChannel()
{
	if (m_cfgDvrIdDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CCfGb28281Dlg::OnClickAlarm()
{
	if (m_cfgAlarmIdDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CCfGb28281Dlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		{
			unsigned short x = m_length[DLG_OFFSET_LEFT];
			unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
			unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
			unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
			unsigned short cy = m_rect.Height() - y- offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

			if(!((m_initPageMask >> PAGE_CHANNEL) & 0x1) && (PAGE_CHANNEL == m_tabCtrl.GetFocusItem()))
			{
				m_cfgDvrIdDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_CHANNEL + 2);

				SetSubCfgDlg(&m_cfgDvrIdDlg);
				m_initPageMask |= (0x1 << PAGE_CHANNEL);
			}

			if(!((m_initPageMask >> PAGE_ALARM) & 0x1) && (PAGE_ALARM == m_tabCtrl.GetFocusItem()))
			{
				m_cfgAlarmIdDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_ALARM + 2);

				SetSubCfgDlg(&m_cfgAlarmIdDlg);
				m_initPageMask |= (0x1 << PAGE_ALARM);
			}

		}

		if (m_curPage != m_tabCtrl.GetFocusItem())
		{
			unsigned long ret = 0;
			if (GetPage(m_curPage)->IsModify())
			{
				ret = TrySaveChange(GetPage(m_curPage),true,false,false);
			}

			if ( ret == 0 )
			{
				m_curPage = static_cast<PAGE_ITEM>(m_tabCtrl.GetFocusItem());
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

