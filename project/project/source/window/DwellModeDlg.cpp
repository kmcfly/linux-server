/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-16
** Name         : LiveDwellDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "DwellModeDlg.h"
#include "CfgLiveHostDlg.h"
#include "CfgLiveSpotDlg.h"
#include "GuiAssert.h"
#include "FuncCustom.h"

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CDwellModeDlg::CDwellModeDlg():m_pData(NULL), m_pDwellTime(NULL)
{
	m_maxChnn=-1;
	m_group = 0;
	m_curPage = -1;

	m_bSpot = false;
}

CDwellModeDlg::~CDwellModeDlg()
{
}

void CDwellModeDlg::OnInitial()
{
	assert(m_maxChnn>0);

	m_style |= WND_CHILD;
	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	unsigned short btnHeight = 40;
	unsigned short btnWidth = 40;
	unsigned short width = m_rect.Width();
	unsigned short height = m_rect.Height() - btnHeight - 10;
	m_rectNXN.SetRect(0,0,width,height);

	if (m_bSpot)
	{
		m_dwellModeDlg.SetMaxChnn(m_localChnnNum, m_localChnnNum);
	}
	else
	{
		m_dwellModeDlg.SetMaxChnn(m_localChnnNum+m_netChannNum, m_localChnnNum);
	}
	m_dwellModeDlg.Create(GetRandWndID(),0,0,width,height,this,0,0,true);

	unsigned short x = 15;
	unsigned short y = height + 4;
	m_btnPre.Create(GetRandWndID(),x,y,btnWidth,btnHeight,this,0,7);
	m_btnPre.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_ARROW_LEFT, false);
	m_btnPre.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_ARROW_LEFT_F, false);

	x = width - btnWidth - 15;
	m_btnNext.Create(GetRandWndID(),x,y,btnWidth,btnHeight,this,0,9);
	m_btnNext.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_PLAY, false);
	m_btnNext.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_PLAY_F, false);

	x = (width - 260)/2;
	y = height + (btnHeight-m_length[STATIC_HEIGHT])/2 + 4;
	m_scDwellTime.Create(GetRandWndID(),x,y,120,m_length[STATIC_HEIGHT],this);
	m_scDwellTime.SetCaption(m_pStrTable->String(E_STR_ID_DWELL_TIME),false);

	x += 130;
	y = height + (btnHeight-m_length[COMBO_HEIGHT])/2 + 4;
	m_cbDwellTime.Create(GetRandWndID(),x,y,120,m_length[COMBO_HEIGHT],this,0,8);

	for ( int i = 0; i <= 60; i += 5)
	{
		char str[32] = {0};
		snprintf(str,32, "%d",i);
		m_cbDwellTime.AddItem( str, i );
	}
	m_cbDwellTime.SetCurItemData(5,false);

	m_bDrag = false;
	m_bFirstDrag = false;
}

void CDwellModeDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	m_curPage = -1;		//使当前页无效
}

void CDwellModeDlg::SetTip()
{
	//AddTip(m_cbDwellTime.GetID(),E_STR_ID_TIP_02_17);
}

void CDwellModeDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_DLG_CHILD_BK);
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_GRAY, 1);
}

void CDwellModeDlg::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	if( GetShowTipType(TIP_DFOCUS) )
	{
		if( newID == m_btnPre.GetID() )
		{
#if !defined(__TW01_RILI__)
			MessageOut(GetTopestWnd(),GetGroupTip(E_STR_ID_TIP_02_15).c_str(),TIP_DFOCUS);
#endif
			return;
		}

		if( newID == m_btnNext.GetID() )
		{
#if !defined(__TW01_RILI__)
			MessageOut(GetTopestWnd(),GetGroupTip(E_STR_ID_TIP_02_16).c_str(),TIP_DFOCUS);
#endif
			return;
		}

		if( newID == m_cbDwellTime.GetID() )
		{
#if !defined(__TW01_RILI__)
			MessageOut(GetTopestWnd(),E_STR_ID_TIP_02_17,TIP_DFOCUS);
#endif
			return;
		}

		if( newID == m_dwellModeDlg.GetID() )
		{
#if !defined(__TW01_RILI__)
			MessageOut(GetTopestWnd(),GetGroupTip(E_STR_ID_NULL).c_str(),TIP_DFOCUS);
#endif
			return;
		}
	}

	GUI::CDialog::OnChangeFocus(oldID,newID);
}

CDwellModeDlgNXN* CDwellModeDlg::GetPage(int index)
{
	assert(index>=0);
	assert(index<PAGE_MAX);

	return &m_dwellModeDlg;
}

CDwellModeDlgNXN* CDwellModeDlg::GetCurPage()
{
	return CDwellModeDlg::GetPage(m_curPage);
}

GUI::CRect CDwellModeDlg::GetPageRect()
{
	GUI::CRect rect = GetWindowRect();
	rect.SetSize( m_rect.Width(),m_rect.Height()-50 );

	return rect;
}

void CDwellModeDlg::ShowPage(int index, bool bRefresh)
{
	//for ( int i=0;i<PAGE_MAX;i++)
	//{
	//	if( m_dwellModeDlg[i].IsShow() )
	//	{
	//		//原先处于false的控件再次被show(false,false)时,会刷空
	//		m_dwellModeDlg[i].Show(false,false);
	//	}
	//}
	if(index != m_curPage)
	{
		m_curPage = index;
		m_group = 0;

		//要求DLG_PAGE 和CDwellModeDlgNXN中的序号一致
		m_dwellModeDlg.SetLayoutType(static_cast<CDwellModeDlgNXN::LAYOUT_TYPE>(m_curPage), false);
	}

	ShowData(m_curPage,m_group,false);

	SendModifyGroupInfo(bRefresh);

	if( bRefresh )
	{
		Repaint();
	}
}

void CDwellModeDlg::SetMaxChnn(int maxChnn, int localChnnNun,int netChannNum)
{
	m_maxChnn = maxChnn;
	m_localChnnNum = localChnnNun;
	m_netChannNum = netChannNum;
}

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

int CDwellModeDlg::GetMaxGroup()
{
	unsigned long* pCfgNXN = reinterpret_cast<unsigned long*>(m_pData[m_curPage]);
	assert(pCfgNXN);//获取最大组的时候不容许未赋值

//	gui_trace(true,printf("Page index=%d,Total max group=%d,Effective max group=%d\n",m_curPage,*pCfgNXN,*(pCfgNXN+1)));

	//总组个数放在第一位,有效最大组放在第二位)
	if (m_bSpot)
	{
		return *(pCfgNXN+1) - g_p_login_succ->productInfo.netVideoInputNum;
	}
	else
	{
		return *(pCfgNXN+1);
	}
}

void CDwellModeDlg::SetCfgData(unsigned long* pData,unsigned long* pDwell, int dwellLen)
{
	m_pData  = pData;
	m_pDwellTime = pDwell;
	m_dwellTimeLen = dwellLen;
}

void CDwellModeDlg::SendModifyCfgData(NCFG_ITEM_ID cfgID)
{
	CCfgLiveHostDlg* pCfgLive = dynamic_cast<CCfgLiveHostDlg*>(GetParent());
	if( pCfgLive )
	{
		pCfgLive->OnModifyCfgData(cfgID);
		return;
	}

	CCfgLiveSpotDlg* pCfgSpot = dynamic_cast<CCfgLiveSpotDlg*>(GetParent());
	if( pCfgSpot )
	{
		pCfgSpot->OnModifyCfgData(cfgID);
		return;
	}

	assert(false);//其他页面
}

void CDwellModeDlg::SendModifyGroupInfo(bool bRefresh)
{
	if(NULL == m_pData)
	{
		return;
	}

	if(0 == m_pData[m_curPage])
	{
		return;
	}

	char str[64] = {0};
	snprintf(str, 64, "%d/%d", m_group + 1, GetMaxGroup());

	CCfgLiveHostDlg* pCfgLive = dynamic_cast<CCfgLiveHostDlg*>(GetParent());
	if( pCfgLive )
	{
		pCfgLive->OnModifyGroupInfo(str, bRefresh);
		return;
	}

	CCfgLiveSpotDlg* pCfgSpot = dynamic_cast<CCfgLiveSpotDlg*>(GetParent());
	if( pCfgSpot )
	{
		pCfgSpot->OnModifyGroupInfo(str, bRefresh);
		return;
	}

	assert(false);//其他页面
}

void CDwellModeDlg::ShowData(int index,int group,bool bRefresh)
{
	//注意index不要过界
	assert(index<PAGE_MAX);
	assert(m_pData);//初始化m_pData

	unsigned long* pCfgNXN = reinterpret_cast<unsigned long*>(m_pData[index]);

	if( NULL == pCfgNXN ) return;//数据没准备好不做操作

	int count = GetPageMax(index);

	//总组数和当前有效组数
	m_dwellModeDlg.SetChnnData(pCfgNXN+count*group+2,count,bRefresh);

	//Dwell Time
	int dwellIndex = (m_group < m_dwellTimeLen) ? m_group : 0;

	m_cbDwellTime.SetCurItemData(m_pDwellTime[dwellIndex], bRefresh);

	//获取当前显示状态
	memset(m_indexCurItem,0,sizeof(m_indexCurItem));
	for(int i=0;i<count;i++)
	{
		m_indexCurItem[i] =  dynamic_cast<GUI::CComboCtrl*>(m_dwellModeDlg.GetScrollWnd().GetWnd(i))->GetCurItem();
	}
}

int CDwellModeDlg::GetPageMax(int index)
{
	switch (index)
	{
	case PAGE_1X1:
		return 1;
		break;
	case PAGE_2X2:
		return 4;
		break;
	case PAGE_2X3:
		return 6;
		break;
	case PAGE_3X3:
		return 9;
		break;
	case PAGE_4X4:
		return 16;
		break;
	case PAGE_5X5:
		return 25;
		break;
	case PAGE_6X6:
		return 36;
		break;
	default:
		assert(false);
		break;
	}

	return 1;
}

std::string CDwellModeDlg::GetGroupTip(ENUM_STRING_ID preID)
{
	std::string str;
	char tmp[512];
	if( preID != E_STR_ID_NULL )
	{
		str = m_pStrTable->String(preID);
		str += ". ";
	}
	str += m_pStrTable->String(E_STR_ID_TIP_02_06);
	snprintf(tmp, 512, "%d .",GetMaxGroup());

	str += tmp;
	str += m_pStrTable->String(E_STR_ID_TIP_02_07);

	snprintf(tmp, 512, "%d .",m_group+1);
	str += tmp;

	return str;
}

BEGIN_MSG_MAP(CDwellModeDlg, GUI::CDialog)
	ON_MSG(m_btnPre.GetID(),KEY_VIRTUAL_ENTER,OnBtnPre)
	ON_MSG(m_btnNext.GetID(),KEY_VIRTUAL_ENTER,OnBtnNext)
	ON_MSG(m_cbDwellTime.GetID(),KEY_VIRTUAL_ENTER,OnDwellTime)
	ON_MSG_EX(m_dwellModeDlg.GetID(),KEY_VIRTUAL_MSG_DEFAULT_TAG,OnClickScrollWnd)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_ENTER, OnEnter)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_DRAG, OnMouseDrag)
	ON_MSG_EX(CTRLID_NULL, KEY_VIRTUAL_BLUP, OnMouseLeftUp)
END_MSG_MAP()

unsigned long CDwellModeDlg::OnBtnPre()
{
	if(0 == CheckDwell())
	{
		if(m_group-1>=0)
		{
			m_group -= 1;
			SendModifyGroupInfo(true);
			ShowData(m_curPage,m_group,true);
		}

		MessageOut(GetTopestWnd(),GetGroupTip(E_STR_ID_TIP_02_15).c_str(),TIP_DFOCUS);
	}
	else
	{
		GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_TIP_02_03), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);			
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDwellModeDlg::OnBtnNext()
{
	if(0 == CheckDwell())
	{
		if( m_group+1<GetMaxGroup() )
		{
			m_group += 1;
			SendModifyGroupInfo(true);
			ShowData(m_curPage,m_group,true);
		}

		MessageOut(GetTopestWnd(),GetGroupTip(E_STR_ID_TIP_02_16).c_str(),TIP_DFOCUS);
	}
	else
	{
		GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_TIP_02_03), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);	
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDwellModeDlg::OnDwellTime()
{
	int dwellIndex = (m_group < m_dwellTimeLen) ? m_group : 0;

	m_pDwellTime[dwellIndex] = m_cbDwellTime.GetCurItemData();

	SendModifyCfgData(m_cfgDwellTime);

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CDwellModeDlg::OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int index = -1;
	unsigned long tmpIndex = (keyID >> 20)&0x7FF;
	if( tmpIndex < 0x7FF )
	{
		index = tmpIndex;
	}
	unsigned long id = keyID&0xFFFFF;

	if( index >= 0 )
	{
		gui_trace(true,printf("cur page=%d,group=%d,index=%d,ctrl id=%d\n",m_curPage,m_group,index,id));

		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_dwellModeDlg.GetScrollWnd().GetWnd(index));
		assert(pCombo);

		int pageMax = GetPageMax(m_curPage);
		if (index >= 36)
		{
			index -= 36;
		}
		//gui_assert(pageMax>=index,printf("cur page=%d,page max=%d,index=%d,ctrl id=%d\n",m_curPage,pageMax,index,ctrlID));

		unsigned long* pCfgNXN = reinterpret_cast<unsigned long*>(m_pData[m_curPage]);
		unsigned long* pChnn = reinterpret_cast<unsigned long*>(pCfgNXN+pageMax*m_group+index+2);

		unsigned long chnn = pCombo->GetCurItemData();
		if(!IsSupportHybridDisplay())
		{//过滤混合显示
			if(chnn<m_localChnnNum+m_netChannNum)
			{//
				unsigned long chnn_temp = 0;
				for(int i=0;i<pageMax;i++)
				{
					if(i != index)
					{
						GUI::CComboCtrl* pCombo_temp = dynamic_cast<GUI::CComboCtrl*>(m_dwellModeDlg.GetScrollWnd().GetWnd(i));
						chnn_temp = pCombo_temp->GetCurItemData();
						if((chnn >= m_localChnnNum)&&(chnn<m_localChnnNum+m_netChannNum))
						{
							if(chnn_temp < m_localChnnNum)
							{
								chnn = m_indexCurItem[index];
								GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);			
								//pCombo->SetCurItem(/*m_indexCurItem[index]*/-1,true);
								pCombo->SetCurItemData(-1,true);
								break;
							}
						}
						else
						{
							if((chnn_temp >= m_localChnnNum)&&(chnn_temp < m_localChnnNum+m_netChannNum))
							{
								chnn = m_indexCurItem[index];
								GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);			
								//pCombo->SetCurItem(/*m_indexCurItem[index]*/-1,true);
								pCombo->SetCurItemData(-1,true);
								break;
							}
						}
					}
				}
			}
		}
		if( chnn >= (m_localChnnNum+m_netChannNum) )
		{
			*pChnn = -1;
		}
		else
		{
			*pChnn = chnn;
		}
		

		SendModifyCfgData(static_cast<NCFG_ITEM_ID>(m_cfgSplitBase+m_curPage));
	}

	return keyID;
}


unsigned long CDwellModeDlg::CheckDwell()
{

	int  pageMax = GetPageMax(m_curPage);
	unsigned long* pCfgNXN =reinterpret_cast<unsigned long*>(m_pData[m_curPage]);
	unsigned long* pChnn = reinterpret_cast<unsigned long*>(pCfgNXN + pageMax * m_group + 2);

	unsigned long* pTmp = pChnn;

	for(unsigned long i = 0; i < pageMax; i++)
	{  
		for (unsigned long j = i + 1; j < pageMax; j++)
		{
			if( ( *(pTmp + j) == *(pChnn + i) ) && ( *(pTmp +j) < (m_localChnnNum+m_netChannNum)))
			{
				return -1;
			}
		}
	}
	return 0;
}

unsigned long CDwellModeDlg::OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	m_enterPosX = xPos;
	m_enterPosY = yPos;

	m_bDrag = true;
	m_bFirstDrag = true;

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CDwellModeDlg::OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if(m_bDrag && m_bFirstDrag)
	{
		unsigned long offset_len = ((m_width / 30) * (m_width / 30)) + ((m_height / 30) * (m_height / 30));

		if(IsAlmostAbove(xPos, yPos, m_enterPosX, m_enterPosY, offset_len))
		{
			if(xPos > m_enterPosX)
			{
				//下一张
				OnBtnNext();
			}
			else
			{
				//上一张
				OnBtnPre();
			}

			m_bFirstDrag = false;
		}
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CDwellModeDlg::OnMouseLeftUp(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	m_bDrag = false;
	m_bFirstDrag = false;

	return KEY_VIRTUAL_MSG_CONTINUE;
}

//end
