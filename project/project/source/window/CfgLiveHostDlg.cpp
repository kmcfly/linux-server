/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-16
** Name         : CfgLiveHostDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgLiveHostDlg.h"
#include "FuncCustom.h"


CCfgLiveHostDlg::CCfgLiveHostDlg()
{
	for(int i = 0; i < 32; i++)
	{
		m_dwellTime[i] = 0;
	}

	for ( int i=0;i<CDwellModeDlg::PAGE_MAX;i++ )
	{
		m_data[i] = NULL;
	}
}

CCfgLiveHostDlg::~CCfgLiveHostDlg()
{
	for ( int i=0;i<CDwellModeDlg::PAGE_MAX;i++ )
	{
		if( NULL != m_data[i] )
		{
			delete [] m_data[i];
			m_data[i] = NULL;
		}
	}
}

void CCfgLiveHostDlg::OnInitial()
{
	CCfgDlg::OnInitial();

	//////////////////////////////////////////////////////////////////////////
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];

	unsigned short height = m_rect.Height() - y - 8 - m_length[DLG_OFFSET_SUB_BOTTOM] - m_length[COMBO_HEIGHT];

	x = m_length[DLG_OFFSET_SUB_LEFT] + (width - 240) / 2;
	//int numChnn = s_loginSuccessReply.productInfo.videoInputNum;
	
	int numChnn = IsSupportHybridDisplay()?s_loginSuccessReply.productInfo.videoInputNum:s_loginSuccessReply.productInfo.localVideoInputNum;
	int numLocalChnn = s_loginSuccessReply.productInfo.localVideoInputNum;
	int numNetChann = s_loginSuccessReply.productInfo.netVideoInputNum;

	#if defined(__KR24_IN__) || defined(__KOREAN__)
		m_scMode.Create(GUI::CWnd::GetRandWndID(), x - 23, y, 140, m_length[COMBO_HEIGHT], this );
		m_scMode.SetCaption(m_pStrTable->String(E_STR_ID_SPLIT_MODE), false);
	#else
		m_scMode.Create( GUI::CWnd::GetRandWndID(), x - 3,y,120,m_length[COMBO_HEIGHT],this );
		m_scMode.SetCaption(m_pStrTable->String(E_STR_ID_SPLIT_MODE), false);
	#endif

	m_comboMode.Create(GUI::CWnd::GetRandWndID(),x+120,y,120,m_length[COMBO_HEIGHT],this,0,0);
	if( numChnn>=1) 
	{
		m_comboMode.AddItem("1X1",VIEW_SPLIT_1X1);
	}

	if( numChnn>=4) 
	{
		m_comboMode.AddItem("2X2",VIEW_SPLIT_2X2);
	}

	if( numChnn>=5) 
	{
		m_comboMode.AddItem("2X3",VIEW_SPLIT_2X3);
	}

	if( numChnn>=8)
	{
		m_comboMode.AddItem("3X3",VIEW_SPLIT_3X3);
	}

	if((12 == numChnn) || (numChnn>=16))
	{
		m_comboMode.AddItem("4X4",VIEW_SPLIT_4X4);
	}

	if((17 <= numChnn) || (numChnn>=24))
	{
		m_comboMode.AddItem("5X5",VIEW_SPLIT_5X5);
	}

	if( numChnn>=32 )
	{
		m_comboMode.AddItem("6X6",VIEW_SPLIT_6X6);
	}
	m_comboMode.SetCurItem(0,false);

	x = m_length[DLG_OFFSET_SUB_LEFT] + width - 60;
	m_scGroupInfo.Create(GetRandWndID(), x, y, 60, m_length[STATIC_HEIGHT], this);
	m_scGroupInfo.SetAlignText(ALIGN_TEXT_RIGHT);
	m_scGroupInfo.SetBKColor(COLOR_DLG_CHILD_BK);

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y += m_length[COMBO_HEIGHT] + 8;

	m_dwellModeDlg.SetMaxChnn(numChnn, numLocalChnn,numNetChann);
	m_dwellModeDlg.SetCfgDwellTime(NCFG_ITEM_VIEW_DWELL_TIME);
	m_dwellModeDlg.SetCfgSplitBase(NCFG_ITEM_VIEW_SPLIT_1X1);
	m_dwellModeDlg.SetCfgData(reinterpret_cast<unsigned long*>(m_data), m_dwellTime, 32);
	m_dwellModeDlg.Create(GUI::CWnd::GetRandWndID(),x,y,width,height,this,0,1);
	//m_dwellModeDlg.ShowPage( GetPageIndex(m_comboMode.GetCurItemData()),false );
	//////////////////////////////////////////////////////////////////////////
}

void CCfgLiveHostDlg::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	//if( GetShowTipType(TIP_DFOCUS) )
	//{
	//	if( m_comboMode.GetID() == newID )
	//	{
	//		int sel = GetPageIndex(m_comboMode.GetCurItemData());
	//		MessageOut(GetTopestWnd(),m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_TIP_02_06+sel)),TIP_DFOCUS);
	//		return;
	//	}
	//}

	CCfgDlg::OnChangeFocus(oldID,newID);
}

void CCfgLiveHostDlg::OnModifyCfgData(NCFG_ITEM_ID cfgID)
{
	if( (!IsModify())&&IsChange(cfgID) )
	{
		SetModify(true);
	}
}

void CCfgLiveHostDlg::OnModifyGroupInfo(const std::string& str, bool bRefresh)
{
	m_scGroupInfo.SetCaption(str, bRefresh);
}


bool CCfgLiveHostDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_VIEW_SPLIT_MODE:
		{
			assert(len==sizeof(unsigned long));

			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			m_comboMode.SetCurItemData(*pValue,false);
			//m_dwellModeDlg.ShowPage(GetPageIndex(m_comboMode.GetCurItemData()),bRefresh);
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_DWELL_TIME:
		{
			assert(len==sizeof(unsigned long) * 32);

			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			memcpy(m_dwellTime, pValue, sizeof(unsigned long) * 32);
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_1X1:
		{
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			assert(len ==sizeof(unsigned long)*(*pValue*1*1+2));
			if( NULL!=m_data[0])
			{
				delete [] m_data[0];
			}
			m_data[0] = new unsigned long[len];
			memcpy(m_data[0],pValue,len);

			if( VIEW_SPLIT_1X1==m_comboMode.GetCurItemData() )
			{
				m_dwellModeDlg.ShowPage(GetPageIndex(VIEW_SPLIT_1X1),bRefresh);
			}
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_2X2:
		{
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			assert(len ==sizeof(unsigned long)*(*pValue*2*2+2));
			if( NULL!=m_data[1])
			{
				delete [] m_data[1];
			}
			m_data[1] = new unsigned long[len];
			memcpy(m_data[1],pValue,len);

			if( VIEW_SPLIT_2X2==m_comboMode.GetCurItemData() )
			{
				m_dwellModeDlg.ShowPage(GetPageIndex(VIEW_SPLIT_2X2),bRefresh);
			}
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_2X3:
		{
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			assert(len ==sizeof(unsigned long)*(*pValue*2*3+2));
			if( NULL!=m_data[2])
			{
				delete [] m_data[2];
			}
			m_data[2] = new unsigned long[len];
			memcpy(m_data[2],pValue,len);

			if( VIEW_SPLIT_2X3==m_comboMode.GetCurItemData() )
			{
				m_dwellModeDlg.ShowPage(GetPageIndex(VIEW_SPLIT_2X3),bRefresh);
			}
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_3X3:
		{
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			assert(len ==sizeof(unsigned long)*(*pValue*3*3+2));
			if( NULL!=m_data[3])
			{
				delete [] m_data[3];
			}
			m_data[3] = new unsigned long[len];
			memcpy(m_data[3],pValue,len);

			if( VIEW_SPLIT_3X3==m_comboMode.GetCurItemData() )
			{
				m_dwellModeDlg.ShowPage(GetPageIndex(VIEW_SPLIT_3X3),bRefresh);
			}
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_4X4:
		{
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			assert(len ==sizeof(unsigned long)*(*pValue*4*4+2));
			if( NULL!=m_data[4])
			{
				delete [] m_data[4];
			}
			m_data[4] = new unsigned long[len];
			memcpy(m_data[4],pValue,len);

			if( VIEW_SPLIT_4X4==m_comboMode.GetCurItemData() )
			{
				m_dwellModeDlg.ShowPage(GetPageIndex(VIEW_SPLIT_4X4),bRefresh);
			}
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_5X5:
		{
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			assert(len ==sizeof(unsigned long)*(*pValue*5*5+2));
			if( NULL!=m_data[5])
			{
				delete [] m_data[5];
			}
			m_data[5] = new unsigned long[len];
			memcpy(m_data[5],pValue,len);

			if( VIEW_SPLIT_5X5==m_comboMode.GetCurItemData() )
			{
				m_dwellModeDlg.ShowPage(GetPageIndex(VIEW_SPLIT_5X5),bRefresh);
			}
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_6X6:
		{
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			assert(len ==sizeof(unsigned long)*(*pValue*6*6+2));
			if( NULL!=m_data[6])
			{
				delete [] m_data[6];
			}
			m_data[6] = new unsigned long[len];
			memcpy(m_data[6],pValue,len);

			if( VIEW_SPLIT_6X6==m_comboMode.GetCurItemData() )
			{
				m_dwellModeDlg.ShowPage(GetPageIndex(VIEW_SPLIT_6X6),bRefresh);
			}
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgLiveHostDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_VIEW_SPLIT_MODE:
		{
			assert(len==sizeof(unsigned long));

			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_comboMode.GetCurItemData();
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_DWELL_TIME:
		{
			assert(len==sizeof(unsigned long) * 32);

			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			memcpy(pValue, m_dwellTime, sizeof(unsigned long) * 32);
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_1X1:
		{
			assert(len ==sizeof(unsigned long)*((*m_data[0])*1*1+2));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			memcpy(pValue,m_data[0],len);
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_2X2:
		{
			assert(len ==sizeof(unsigned long)*((*m_data[1])*2*2+2));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			memcpy(pValue,m_data[1],len);
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_2X3:
		{
			assert(len ==sizeof(unsigned long)*((*m_data[2])*2*3+2));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			memcpy(pValue,m_data[2],len);
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_3X3:
		{
			assert(len ==sizeof(unsigned long)*((*m_data[3])*3*3+2));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			memcpy(pValue,m_data[3],len);
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_4X4:
		{
			assert(len ==sizeof(unsigned long)*((*m_data[4])*4*4+2));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			memcpy(pValue,m_data[4],len);
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_5X5:
		{
			assert(len ==sizeof(unsigned long)*((*m_data[5])*5*5+2));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			memcpy(pValue,m_data[5],len);
		}
		return true;
		break;
	case NCFG_ITEM_VIEW_SPLIT_6X6:
		{
			assert(len ==sizeof(unsigned long)*((*m_data[6])*6*6+2));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			memcpy(pValue,m_data[6],len);
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

int CCfgLiveHostDlg::GetPageIndex(unsigned long mode)
{
	switch (mode)
	{
	case VIEW_SPLIT_1X1:
		return CDwellModeDlg::PAGE_1X1;
	case VIEW_SPLIT_2X2:
		return CDwellModeDlg::PAGE_2X2;
	case VIEW_SPLIT_2X3:
		return CDwellModeDlg::PAGE_2X3;
	case VIEW_SPLIT_3X3:
		return CDwellModeDlg::PAGE_3X3;
	case VIEW_SPLIT_4X4:
		return CDwellModeDlg::PAGE_4X4;
	case VIEW_SPLIT_5X5:
		return CDwellModeDlg::PAGE_5X5;
	case VIEW_SPLIT_6X6:
		return CDwellModeDlg::PAGE_6X6;
	default:
		assert(false);
		break;
	}
	return 0;
}

BEGIN_MSG_MAP(CCfgLiveHostDlg, CCfgDlg)
	ON_MSG(m_comboMode.GetID(),KEY_VIRTUAL_ENTER,OnClickComboMode)
	ON_MSG_EX(m_dwellModeDlg.GetID(),KEY_VIRTUAL_MSG_DEFAULT_TAG,OnClickDwellModeDlg)
END_MSG_MAP()

unsigned long CCfgLiveHostDlg::OnClickComboMode()
{
	int sel = GetPageIndex(m_comboMode.GetCurItemData());

	if( sel != m_dwellModeDlg.GetCurPageIndex() )
	{
		m_dwellModeDlg.ShowPage( GetPageIndex(m_comboMode.GetCurItemData()), true );
	}
	
	//通知修改
	OnModifyCfgData(NCFG_ITEM_VIEW_SPLIT_MODE);

	//提示
	//if( GetShowTipType(TIP_DFOCUS) )
	//{
	//	MessageOut(GetTopestWnd(),m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_TIP_02_06+sel)),TIP_DFOCUS);
	//}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CCfgLiveHostDlg::OnClickDwellModeDlg(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int index = -1;
	unsigned long tmpIndex = (keyID >> 20)&0x7FF;
	if( tmpIndex < 0x7FF )
	{
		index = tmpIndex;
	}
	unsigned long id = keyID&0xFFFFF;	

	//printf("(%s,line=%d) index=%d,id=%d\r\n",__FILE__,__LINE__,index,id);

	if( index >= 0 )
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
	else
	{
		return keyID;
	}
}

unsigned long CCfgLiveHostDlg::CheckDwell()
{

	return m_dwellModeDlg.CheckDwell();

}


