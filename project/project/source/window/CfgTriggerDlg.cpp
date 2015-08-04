/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgTriggerDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgTriggerDlg.h"
#include "CheckCtrl.h"
#include "SelectChnnDlg.h"
#include "CfgDlg.h"
#include "NetConfigDefine.h"
#include "FuncCustom.h"
//////////////////////////////////////////////////////////////////////////

CCfgTriggerDlg::CCfgTriggerDlg() : m_showPage(PAGE_ALL_SHOW), m_channel(INVALID_INDEX(unsigned char))
{

}

CCfgTriggerDlg::~CCfgTriggerDlg()
{
}

//////////////////////////////////////////////////////////////////////////
void CCfgTriggerDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);

	char str [128] = {0};
	if (INVALID_INDEX(unsigned char) == m_channel)
	{
		snprintf(str, 128, "%s - %s",  m_pStrTable->String(E_STR_ID_TITLE_TRIGGER), m_pStrTable->String(E_STR_ID_ALL));
	}
	else
	{
		snprintf(str, 128, "%s - %s %d",  m_pStrTable->String(E_STR_ID_TITLE_TRIGGER), m_pStrTable->String(E_STR_ID_CHANNEL), \
				m_channel+1);
	}

	SetCaption(str, false);

	SetDefaultBtn(DEFAULT_BTN_APPLY | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String( E_STR_ID_EXIT ) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_OK) );
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	int index = 1;
	PAGE_ITEM pageFirst = PAGE_NULL;
	m_tabCtrl.Create(GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);

	if( m_showPage & static_cast<unsigned long>(PAGE_BASIC_SHOW) )
	{			
		//绘制窗口
		m_cfgTriggerBasicDlg.SetAlarmTriggerInfo(&m_alarmTriggerInfo, m_channel);
		m_cfgTriggerBasicDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, ++index );
		m_tabCtrl.AddItem(&m_cfgTriggerBasicDlg, m_pStrTable->String(E_STR_ID_ALARM_BASIC));
		if( pageFirst == PAGE_NULL)
		{
			pageFirst = PAGE_BASIC;
		}
	}

	if( m_showPage & static_cast<unsigned long>(PAGE_TO_RECORD_SHOW) )
	{
		m_cfgTriggerToRecordDlg.SetAlarmTriggerInfo(&m_alarmTriggerInfo);
		m_cfgTriggerToRecordDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, ++index );
		m_tabCtrl.AddItem(&m_cfgTriggerToRecordDlg, m_pStrTable->String(E_STR_ID_TO_RECORD));
		if( pageFirst == PAGE_NULL)
		{
			pageFirst = PAGE_TO_RECORD;
		}
	}

	if (s_loginSuccessReply.productInfo.videoInputNum > 0)
	{
		if(IsSupportPTZ() && (m_showPage & static_cast<unsigned long>(PAGE_TO_PTZ_SHOW)))
		{
			m_cfgTriggerToPtzDlg.SetAlarmTriggerInfo(&m_alarmTriggerInfo);
			m_cfgTriggerToPtzDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, ++index );
			m_tabCtrl.AddItem(&m_cfgTriggerToPtzDlg, m_pStrTable->String(E_STR_ID_TO_PTZ));
			if( pageFirst == PAGE_NULL)
			{
				pageFirst = PAGE_TO_PTZ;
			}
		}
	}

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	if(m_showPage & static_cast<unsigned long>(PAGE_TO_FTP_SHOW))
	{
		m_cfgTriggerToFtpDlg.SetAlarmTriggerInfo(&m_alarmTriggerInfo);
		m_cfgTriggerToFtpDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, ++index );
		m_tabCtrl.AddItem(&m_cfgTriggerToFtpDlg, m_pStrTable->String(E_STR_ID_FTP));
		if( pageFirst == PAGE_NULL)
		{
			pageFirst = PAGE_TO_FTP;
		}
	}
#endif

	m_curPage = pageFirst;
	m_tabCtrl.SetCurItem(m_curPage , false );
	//////////////////////////////////////////////////////////////////////////
}

void CCfgTriggerDlg::OnPostInitial()
{
	CCfgDlg::OnPostInitial();

	if(0 == s_loginSuccessReply.productInfo.localVideoInputNum)
	{
		if(m_showPage & static_cast<unsigned long>(PAGE_BASIC_SHOW))
		{
			m_cfgTriggerBasicDlg.InitCtrlData(false);
		}

		if(m_showPage & static_cast<unsigned long>(PAGE_TO_RECORD_SHOW))
		{
			m_cfgTriggerToRecordDlg.InitCtrlData(false);
		}
	}
}

void CCfgTriggerDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();
}

unsigned long CCfgTriggerDlg::OnApply()
{
	//由于配置基类使用OnOk作为保存按钮, 并有默认的操作.
	//这里使用Apply按钮替代

	// 读取页面数据保存
	// 这两个页面是页面数据修改直接修改内存数据
	//if( m_showPage & static_cast<unsigned long>(PAGE_BASIC_SHOW) )
	//{
	//	m_cfgTriggerBasicDlg.OnApply();
	//}

	//if( m_showPage & static_cast<unsigned long>(PAGE_TO_RECORD_SHOW) )
	//{
	//	m_cfgTriggerToRecordDlg..OnApply();
	//}

	if (s_loginSuccessReply.productInfo.videoInputNum > 0)
	{
		if(IsSupportPTZ() && (m_showPage & static_cast<unsigned long>(PAGE_TO_PTZ_SHOW)))
		{
			//To Ptz页面确定后, 需要重读数据
			m_cfgTriggerToPtzDlg.OnApply();
		}
	}
	
	//只需要返回DLG_OK
	return CDialog::OnOK();
}

void CCfgTriggerDlg::SetTriggerInfo(const ALARM_TRIGGER_INFO& triggerInfo, unsigned char chnn)
{
	memcpy(&m_alarmTriggerInfo, &triggerInfo, sizeof(ALARM_TRIGGER_INFO));
	m_channel = chnn;
}

void CCfgTriggerDlg::GetTriggerInfo(ALARM_TRIGGER_INFO& triggerInfo)
{
	memcpy(&triggerInfo, &m_alarmTriggerInfo, sizeof(ALARM_TRIGGER_INFO));
}

void CCfgTriggerDlg::UpdateView(bool bRefresh)
{
	CCfgDlg::UpdateView(bRefresh);

	/*
		页面内容初始化, 放这里。原因:有部分页面需要初始控件设置
	*/

	// 初始化各个页面的内容
	if(m_showPage & static_cast<unsigned long>(PAGE_BASIC_SHOW))
	{
		m_cfgTriggerBasicDlg.InitCtrlData(bRefresh);
	}

	if(m_showPage & static_cast<unsigned long>(PAGE_TO_RECORD_SHOW))
	{
		m_cfgTriggerToRecordDlg.InitCtrlData(bRefresh);
	}

	if (s_loginSuccessReply.productInfo.videoInputNum > 0)
	{
		if(IsSupportPTZ() && (m_showPage & static_cast<unsigned long>(PAGE_TO_PTZ_SHOW)))
		{
			m_cfgTriggerToPtzDlg.InitCtrlData(bRefresh);
		}
	}

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	if(m_showPage & static_cast<unsigned long>(PAGE_TO_FTP_SHOW))
	{
		m_cfgTriggerToFtpDlg.InitCtrlData(bRefresh);
	}
#endif

}

void CCfgTriggerDlg::GetItemList(CFGITEM_LIST& itemList) const
{

	for (int i = 0; i < s_loginSuccessReply.productInfo.videoInputNum; ++i)
	{
		assert(NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + i < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND);
		itemList.push_back(static_cast<NCFG_ITEM_ID>(NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + i));
	}

	for (int i = 0; i < s_loginSuccessReply.productInfo.videoInputNum; ++i)
	{
		assert(NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + i < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND);
		itemList.push_back(static_cast<NCFG_ITEM_ID>(NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + i));
	}

	
}


