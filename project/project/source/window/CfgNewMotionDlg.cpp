
#include "CfgNewMotionDlg.h"
#include "MessageBox.h"

CSubCfgMotionManDlg::CSubCfgMotionManDlg()
{

}
CSubCfgMotionManDlg::~CSubCfgMotionManDlg()
{

}


void CSubCfgMotionManDlg::OnInitial()
{
	CCfgDlg::OnInitial();

	//////////////////////////////////////////////////////////////////////////

	//为加快Table页面, 采用先只初始化第一页面, 后面再陆续初始化其他页面
	//此变量记录已经初始化的页面
	// m_initPageMask = 0x1;

	// SetCaption("Motion", false);

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - ( x * 2 );
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	printf("before m_cfgScheduleChildDlg Create\n");
	m_cfgScheduleChildDlg.SetMaxChnn(s_loginSuccessReply.productInfo.videoInputNum);
	m_cfgScheduleChildDlg.SetScheduleCfgID(NCFG_ITEM_MOTION_SCH_WEEK, NCFG_ITEM_MOTION_SCH_HOLIDAY);
	m_cfgScheduleChildDlg.Create(GUI::CWnd::GetRandWndID(), x, m_length[DLG_OFFSET_TOP] + m_length[TABLE_HEIGHT], cx, cy, this, WND_CHILD, PAGE_SCHEDULE + 2);
	printf("after m_cfgScheduleChildDlg Create\n");
	m_cfgMotionDlg.Create(GUI::CWnd::GetRandWndID(), x, m_length[DLG_OFFSET_TOP] + m_length[TABLE_HEIGHT], cx, cy, this, WND_CHILD, PAGE_MOTION + 2);	
	
	
	printf("m_cfgScheduleChildDlg->MaxChnn:%d\n", s_loginSuccessReply.productInfo.videoInputNum);
	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
	m_tabCtrl.AddItem(&m_cfgMotionDlg, m_pStrTable->String(E_STR_ID_REC_MOTION));
	m_tabCtrl.AddItem(&m_cfgScheduleChildDlg, m_pStrTable->String(E_STR_ID_REC_SCHEDULE));
	
	

	m_curPage = PAGE_MOTION;
	m_tabCtrl.SetCurItem(m_curPage , false );
	CCfgDlg::SetRefreshView( false );



	/*SetSubCfgDlg(&m_cfgMotionDlg);
	SetSubCfgDlg(&m_cfgScheduleChildDlg);*/
	printf("initial CfgNewMotionDlg\n");	
	CreateTipWnd();
}



/*bool CSubCfgMotionManDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
		for (CFGDLG_LIST::const_iterator iter = m_childList.begin(); iter != m_childList.end(); ++iter)
		{
			if ((*iter)->ShowData(cfgID, pData, len, bRefresh))
			{
				return true;
			}
		}
		return false;
}

bool CSubCfgMotionManDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	for (CFGDLG_LIST::const_iterator iter = m_childList.begin(); iter != m_childList.end(); ++iter)
		{
			if ((*iter)->SetData(cfgID, pData, len))
			{
				return true;
			}
		}
		return false;
}

void DoOther()
{
	m_cfgMotionDlg.SetConfigBlock(GetConfigBlock());
	m_cfgScheduleChildDlg.SetConfigBlock(GetConfigBlock());
}*/
	

bool CSubCfgMotionManDlg::IsModify() const
{
	/*printf("(m_childList.size() -> %d\n", m_childList.size());
	if(m_childList.size()>0)
	{
		for (GUI::CWnd::WND_LIST::iterator iter = m_childList.begin(); iter != m_childList.end(); ++iter)
		{
			CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>(*iter);
			if (NULL != pCfgDlg)
			{
				if(pCfgDlg->IsModify()) return true;
			}
		}
	}
	return false;*/
	return	(m_cfgMotionDlg.IsModify() || m_cfgScheduleChildDlg.IsModify());
}

void CSubCfgMotionManDlg::SetModify(bool modify)
{
	/*if(m_childList.size()>0)
	{
		for (GUI::CWnd::WND_LIST::iterator iter = m_childList.begin(); iter != m_childList.end(); ++iter)
		{
			CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>(*iter);
			if (NULL != pCfgDlg)
			{
				pCfgDlg->SetModify(modify);
			}
		}
	}
*/

	m_cfgMotionDlg.SetModify(modify);
	m_cfgScheduleChildDlg.SetModify(modify);
}

void CSubCfgMotionManDlg::SetTip()
{
	m_cfgScheduleChildDlg.SetSetTip();
}

