/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-15
** Name         : CfgRecordManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgRecordManDlg.h"
#include "MessageBox.h"
#include "FuncCustom.h"

extern NET_LOGIN_SUCCESS_REPLY *	g_p_login_succ;

CCfgRecordManDlg::CCfgRecordManDlg():
m_curPage(PAGE_RECORD),
m_initAnchorPage(PAGE_RECORD),
m_initPageMask(0)
{

}

CCfgRecordManDlg::~CCfgRecordManDlg()
{

}

void CCfgRecordManDlg::OnPostInitial()
{
	CCfgDlg::OnPostInitial();

#if defined(__DVR_ANCHOR__)
	if (PAGE_RECORD_STREAM == m_initAnchorPage)
	{
		PRODUCT_TYPE product = CProduct::Instance()->ProductType();

		if ((0 != g_p_login_succ->productInfo.netVideoInputNum) 
			|| (TD_2704TS_PL == product) || (TD_2708TS_PL == product) || (TD_2716TS_PL == product)
			|| (TD_2704TD_P == product) || (TD_2708TD_P == product) || (TD_2716TD_P == product)
			|| (TD_2704TD_PL == product) || (TD_2708TD_PL == product) || (TD_2716TD_PL == product) || (TD_2732TD_PL == product)
			|| (TD_2704AS_S == product) || (TD_2708AS_S == product) || (TD_2708AS_S_A == product) || (TD_2716AS_S == product)
			|| (TD_2704AS_S_A == product) || (TD_2708AS_S_A == product) || (TD_2716AS_S_A == product)
			|| (TD_2704TE_M == product) || (TD_2708TE_M == product) || (TD_2716TE_M == product)
			|| (TD_2704AE_SL == product) || (TD_2708AE_SL == product) || (TD_2716AS_SL == product)
			|| (TD_2004AL == product) || (TD_2008AS == product)
			|| (TD_2704AS_PL == product) || (TD_2708AS_PL == product) || (TD_2716AS_PL == product)
			|| (TD_2716AC_PL == product)
			|| (TD_2704AE_PL == product) || (TD_2708AE_PL == product) || (TD_2716AE_PL == product)
			|| (TD_2704AS_SL == product) || (TD_2708AS_SL == product))
		{
			if (NULL != m_pBtnDefualt)
			{
				m_pBtnDefualt->SetEnable(false, true);
			}
		}
	}
#endif
}

void CCfgRecordManDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DEFAULT) );
	//////////////////////////////////////////////////////////////////////////

	//为加快Table页面, 采用先只初始化第一页面, 后面再陆续初始化其他页面
	//此变量记录已经初始化的页面
	m_initPageMask = 0x1;

#ifndef __DVR_ANCHOR__
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_RECORD), false);

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - ( x * 2 );
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	m_cfgRecordDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 2);
	//m_cfgRecordStreamDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy,this,0,3);
	//m_cfgRecordTimeDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 4);
	//m_cfgRecordOsdDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 5);
	//m_cfgRecordRecycleDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 6);	
	//m_cfgRecordSnapDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 7);
		

	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
	m_tabCtrl.AddItem(&m_cfgRecordDlg, m_pStrTable->String( E_STR_ID_ENABLE ));
	m_tabCtrl.AddItem(&m_cfgRecordStreamDlg, m_pStrTable->String(E_STR_ID_RECORD_STREAM));
	m_tabCtrl.AddItem(&m_cfgRecordTimeDlg, m_pStrTable->String(E_STR_ID_RECORD_TIME));
	m_tabCtrl.AddItem(&m_cfgRecordOsdDlg, m_pStrTable->String(E_STR_ID_STAMP));
	m_tabCtrl.AddItem(&m_cfgRecordRecycleDlg, m_pStrTable->String(E_STR_ID_RECYCLE_RECORD));
	
	m_tabCtrl.AddItem(&m_cfgRecordSnapDlg, m_pStrTable->String(E_STR_ID_SNAP));
		
	////

	m_curPage = PAGE_RECORD;
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
	case PAGE_RECORD:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_RECORD), false);
		m_cfgRecordDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_RECORD_STREAM:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_RECORD_BITRATE), false);
		m_cfgRecordStreamDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_RECORD_TIME:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_TIME), false);
		m_cfgRecordTimeDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_RECORD_OSD:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_STAMP), false);
		m_cfgRecordOsdDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_RECORD_RECYCLE:
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_RECYCLE_RECORD), false);
		m_cfgRecordRecycleDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
		break;
	case PAGE_RECORD_SNAP:
		SetCaption(m_pStrTable->String(E_STR_ID_SNAP), false);
		m_cfgRecordSnapDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1);
				
		break;
	default:
		assert(false);
		break;
	}

	m_curPage = m_initAnchorPage;
	CCfgDlg::SetRefreshView( false );

#endif

	////Create tip
	CreateTipWnd();
}

unsigned long CCfgRecordManDlg::OnApply()
{
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

unsigned long CCfgRecordManDlg::OnOK()
{
	return CCfgDlg::OnOK();
}

unsigned long CCfgRecordManDlg::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

CCfgDlg* CCfgRecordManDlg::GetPage(PAGE_ITEM item)
{
	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_RECORD:
		pCfgDlg = &m_cfgRecordDlg;
		break;
	case PAGE_RECORD_STREAM:
		pCfgDlg = &m_cfgRecordStreamDlg;
		break;
	case PAGE_RECORD_TIME:
		pCfgDlg = &m_cfgRecordTimeDlg;
		break;
	case PAGE_RECORD_OSD:
		pCfgDlg = &m_cfgRecordOsdDlg;
		break;
	case PAGE_RECORD_RECYCLE:
		pCfgDlg = &m_cfgRecordRecycleDlg;
		break;	
	case PAGE_RECORD_SNAP:
		pCfgDlg = &m_cfgRecordSnapDlg;
		break;
	default:
		assert(false);
		break;
	}
	return pCfgDlg;
}

void CCfgRecordManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_REC_RECYCLE);
	itemList.push_back(NCFG_ITEM_CAMERA_NAME);
	itemList.push_back(NCFG_ITEM_TIME_STAMP_INFO);
	itemList.push_back(NCFG_ITEM_CAM_NAME_STAMP_INFO);
	itemList.push_back(NCFG_ITEM_REC_ENABLE);
	itemList.push_back(NCFG_ITEM_PRE_ALARM_REC_TIME);
	itemList.push_back(NCFG_ITEM_POST_ALARM_REC_TIME);
	itemList.push_back(NCFG_ITEM_REC_HOLD_TIME);
	itemList.push_back(NCFG_ITEM_REC_AUDIO);
	itemList.push_back(NCFG_ITEM_REC_REDUNDANT);
	if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		itemList.push_back(NCFG_ITEM_ENCODE_MAJOR);
		itemList.push_back(NCFG_ITEM_ENCODE_MAJOR_EVENT);
	}
 	itemList.push_back(NCFG_ITEM_JPEG_PERIOD);
 	itemList.push_back(NCFG_ITEM_JPEG_SIZE);
 	itemList.push_back(NCFG_ITEM_JPEG_QUALITY);
 	itemList.push_back(NCFG_ITEM_JPEG_SNAP_NUM);
 
 	itemList.push_back(NCFG_ITEM_ENCODE_MAJOR_SPEC);
 	itemList.push_back(NCFG_ITEM_ENCODE_MAJOR_EX);
	itemList.push_back(NCFG_ITEM_BITRANGE_FOR_VIDEOSIZE);
	itemList.push_back(NCFG_ITEM_MAJOR_CHANNEL_RECORD_PARAM_EX);
	//下面是只读选项
	itemList.push_back(NCFG_ITEM_MAX_PRE_ALARM_TIME);
	itemList.push_back(NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY);
	itemList.push_back(NCFG_ITEM_MAJOR_SUPPORT_QULITY_LEVELNUM);
	itemList.push_back(NCFG_ITEM_MAJOR_SUPPORT_ENCODEMODE);
	itemList.push_back(NCFG_ITEM_MAJOR_SUPPORT_BITRATE_RANGE);
	itemList.push_back(NCFG_ITEM_ENCODE_SIZE_MAJOR_ALL_CH);
	itemList.push_back(NCFG_ITEM_DEVICE_SUPPORT_REDUNDANCY);
	itemList.push_back(NCFG_ITEM_MAJOR_HAVE_ENCODE_AND_MAXBIT);
	itemList.push_back(NCFG_ITEM_SNAP_SIZE_RANGE);
	itemList.push_back(NCFG_ITEM_SNAP_QULITY_LEVELNUM);
	itemList.push_back(NCFG_ITEM_SNAP_IMG_NUM_RANGE);
	itemList.push_back(NCFG_ITEM_SNAP_TIME_RANGE);
	
	
}

bool CCfgRecordManDlg::SaveConfig()
{
	return CCfgDlg::SaveConfig();
}

void CCfgRecordManDlg::SetAnchorPage(PAGE_ITEM item)
{
	m_initAnchorPage = item;
}

BEGIN_MSG_MAP(CCfgRecordManDlg,CCfgDlg)
	ON_MSG(m_cfgRecordDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickRecord)
	ON_MSG(m_cfgRecordStreamDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickRecordStream)
	ON_MSG(m_cfgRecordTimeDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickRecordTime)
	ON_MSG(m_cfgRecordOsdDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickRecordOSD)
	ON_MSG(m_cfgRecordRecycleDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickRecordRecycle)
	ON_MSG(m_cfgRecordSnapDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickRecordSnap);
END_MSG_MAP()

unsigned long CCfgRecordManDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		{
			unsigned short x = m_length[DLG_OFFSET_LEFT];
			unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
			unsigned short cx = m_rect.Width() - ( x * 2 );
			unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

			PRODUCT_TYPE product = CProduct::Instance()->ProductType();

			if ((0 != g_p_login_succ->productInfo.netVideoInputNum) 
				|| (TD_2704TS_PL == product) || (TD_2708TS_PL == product) || (TD_2716TS_PL == product)
				|| (TD_2704TD_P == product) || (TD_2708TD_P == product) || (TD_2716TD_P == product)
				|| (TD_2704TD_PL == product) || (TD_2708TD_PL == product) || (TD_2716TD_PL == product) || (TD_2732TD_PL == product)
				|| (TD_2704AS_S == product) || (TD_2708AS_S == product) || (TD_2708AS_S_A == product) || (TD_2716AS_S == product)
				|| (TD_2704AS_SL == product) || (TD_2704AS_SL_A == product) || (TD_2704AS_PL == product) || (TD_2708AS_SL == product) || (TD_2716AS_SL == product)
				|| (TD_2704TE_M == product) || (TD_2708TE_M == product) || (TD_2716TE_M == product)
				|| (TD_2704AE_SL == product) || (TD_2708AE_SL == product) || (TD_2716AE_SL == product)
				|| (TD_2004AL == product) || (TD_2008AS == product)
				|| (TD_2704AS_PL == product) || (TD_2708AS_PL == product) || (TD_2716AS_PL == product)
				|| (TD_2716AC_PL == product)
				|| (TD_2704AE_PL == product) || (TD_2708AE_PL == product) || (TD_2716AE_PL == product))
			{
				if (PAGE_RECORD_STREAM == m_tabCtrl.GetFocusItem())
				{
					if (NULL != m_pBtnDefualt)
					{
						m_pBtnDefualt->SetEnable(false, true);
					}
				}
				else
				{
					if (NULL != m_pBtnDefualt)
					{
						m_pBtnDefualt->SetEnable(true, true);
					}
				}
			}

			if(!((m_initPageMask >> PAGE_RECORD_STREAM) & 0x1) && (PAGE_RECORD_STREAM == m_tabCtrl.GetFocusItem()))
			{
				m_cfgRecordStreamDlg.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0, PAGE_RECORD_STREAM + 2);

				SetSubCfgDlg(&m_cfgRecordStreamDlg);
				m_initPageMask |= (0x1 << PAGE_RECORD_STREAM);
			}
		
			if(!((m_initPageMask >> PAGE_RECORD_TIME) & 0x1) && (PAGE_RECORD_TIME == m_tabCtrl.GetFocusItem()))
			{
				m_cfgRecordTimeDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, PAGE_RECORD_TIME + 2);
				
				SetSubCfgDlg(&m_cfgRecordTimeDlg);
				m_initPageMask |= (0x1 << PAGE_RECORD_TIME);
			}

			if(!((m_initPageMask >> PAGE_RECORD_OSD) & 0x1) && (PAGE_RECORD_OSD == m_tabCtrl.GetFocusItem()))
			{
				m_cfgRecordOsdDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, PAGE_RECORD_OSD + 2);

				SetSubCfgDlg(&m_cfgRecordOsdDlg);
				m_initPageMask |= (0x1 << PAGE_RECORD_OSD);
			}

			if(!((m_initPageMask >> PAGE_RECORD_RECYCLE) & 0x1) && (PAGE_RECORD_RECYCLE == m_tabCtrl.GetFocusItem()))
			{
				m_cfgRecordRecycleDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, PAGE_RECORD_RECYCLE + 2);	

				SetSubCfgDlg(&m_cfgRecordRecycleDlg);
				m_initPageMask |= (0x1 << PAGE_RECORD_RECYCLE);
			}

			if(!((m_initPageMask >> PAGE_RECORD_SNAP) & 0x1) && (PAGE_RECORD_SNAP == m_tabCtrl.GetFocusItem()))
			{
				m_cfgRecordSnapDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, PAGE_RECORD_SNAP + 2);

				SetSubCfgDlg(&m_cfgRecordSnapDlg);
				m_initPageMask |= (0x1 << PAGE_RECORD_SNAP);
			}
		}
			
		if (m_curPage != m_tabCtrl.GetFocusItem())
		{
			unsigned long ret = 0;
			if (GetPage(m_curPage)->IsModify())
			{
				if((m_curPage == PAGE_RECORD_STREAM) && (0 != g_p_login_succ->productInfo.netVideoInputNum))
				{
					std::string str = m_pStrTable->String(E_STR_ID_MESSAGE_38);
					GUI::CMessageBox box(str.c_str(), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_CANCEL|DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
					if ( DLG_OK == box.DoModal( this))
					{
						ret = TrySaveChange(GetPage(m_curPage),false,false,false);
					}
					else
					{
						EnableOkBtn(false, true);
						m_cfgRecordStreamDlg.SetModify(false);
						m_cfgRecordStreamDlg.UpdateView(false);
						m_cfgRecordStreamDlg.RecoverChange();
					}
				}
				else
				{
					ret = TrySaveChange(GetPage(m_curPage),true,false,false);
				}
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

unsigned long CCfgRecordManDlg::OnClickRecord()
{
	if (m_cfgRecordDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordManDlg::OnClickRecordTime()
{
	if (m_cfgRecordTimeDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordManDlg::OnClickRecordStream()
{
	if (m_cfgRecordStreamDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordManDlg::OnClickRecordRecycle()
{
	if (m_cfgRecordRecycleDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordManDlg::OnClickRecordOSD()
{
	if (m_cfgRecordOsdDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordManDlg::OnClickRecordSnap()
{
	if(m_cfgRecordSnapDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

CCfgRecordManAnchorDlg::CCfgRecordManAnchorDlg()
{

}

CCfgRecordManAnchorDlg::~CCfgRecordManAnchorDlg()
{

}

void CCfgRecordManAnchorDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_RECORD), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 80;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;

	m_anRecord.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy , this, 0, yFocus++, m_pStrTable->String(E_STR_ID_RECORD));
	m_anRecord.SetImage(BMP_ANCHOR_RECORD, BMP_ANCHOR_RECORD_F, BMP_ANCHOR_RECORD);

	y += 60;
	m_anRecordStream.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_RECORD_STREAM));
	m_anRecordStream.SetImage(BMP_ANCHOR_BITRATE, BMP_ANCHOR_BITRATE_F, BMP_ANCHOR_BITRATE);

	y += 60;
	m_anRecordTime.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_RECORD_TIME));
	m_anRecordTime.SetImage(BMP_ANCHOR_TIME, BMP_ANCHOR_TIME_F, BMP_ANCHOR_TIME);

	y += 60;
	m_anRecordOSD.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_STAMP));
	m_anRecordOSD.SetImage(BMP_ANCHOR_STAMP, BMP_ANCHOR_STAMP_F, BMP_ANCHOR_STAMP);

	y += 60;
	m_anRecordRecycle.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_RECYCLE_RECORD));
	m_anRecordRecycle.SetImage(BMP_ANCHOR_RECYCLE_RECORD, BMP_ANCHOR_RECYCLE_RECORD_F, BMP_ANCHOR_RECYCLE_RECORD);

	//第二列

	x = m_rect.Width() / 2 + 40;
	y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
    
//	if (CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		m_anRecordSnap.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_SNAP));
		m_anRecordSnap.SetImage(BMP_ANCHOR_SNAP, BMP_ANCHOR_SNAP_F, BMP_ANCHOR_SNAP);

		y += 60;
	}
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
	
	
}

BEGIN_MSG_MAP(CCfgRecordManAnchorDlg, GUI::CDialog)
	ON_MSG(m_anRecord.GetID(), KEY_VIRTUAL_ENTER, OnAnchorRecord)
	ON_MSG(m_anRecordStream.GetID(), KEY_VIRTUAL_ENTER, OnAnchorRecordStream)
	ON_MSG(m_anRecordTime.GetID(), KEY_VIRTUAL_ENTER, OnAnchorRecordTime)
	ON_MSG(m_anRecordOSD.GetID(), KEY_VIRTUAL_ENTER, OnAnchorRecordOSD)
	ON_MSG(m_anRecordRecycle.GetID(), KEY_VIRTUAL_ENTER, OnAnchorRecordRecycle)
	ON_MSG(m_anRecordSnap.GetID(), KEY_VIRTUAL_ENTER, OnAnchorRecordSnap)
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
END_MSG_MAP()

unsigned long CCfgRecordManAnchorDlg::OnAnchorRecord()
{
	m_cfgRecordManDlg.SetAnchorPage(CCfgRecordManDlg::PAGE_RECORD);
	m_cfgRecordManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordManAnchorDlg::OnAnchorRecordStream()
{
	m_cfgRecordManDlg.SetAnchorPage(CCfgRecordManDlg::PAGE_RECORD_STREAM);

	m_cfgRecordManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordManAnchorDlg::OnAnchorRecordTime()
{
	m_cfgRecordManDlg.SetAnchorPage(CCfgRecordManDlg::PAGE_RECORD_TIME);
	m_cfgRecordManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordManAnchorDlg::OnAnchorRecordOSD()
{
	m_cfgRecordManDlg.SetAnchorPage(CCfgRecordManDlg::PAGE_RECORD_OSD);
	m_cfgRecordManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordManAnchorDlg::OnAnchorRecordRecycle()
{
	m_cfgRecordManDlg.SetAnchorPage(CCfgRecordManDlg::PAGE_RECORD_RECYCLE);
	m_cfgRecordManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordManAnchorDlg::OnAnchorRecordSnap()
{
	m_cfgRecordManDlg.SetAnchorPage(CCfgRecordManDlg::PAGE_RECORD_SNAP);
	m_cfgRecordManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecordManAnchorDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

//end
