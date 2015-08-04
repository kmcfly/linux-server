/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-21
** Name         : CfgLiveSpotDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgLiveSpotDlg.h"
#include "GuiAssert.h"

CCfgLiveSpotDlg::CCfgLiveSpotDlg()
{
	for ( int i=0;i<6;i++ )
	{
		m_data[i]= NULL;
	}
}

CCfgLiveSpotDlg::~CCfgLiveSpotDlg()
{
	for ( int i=0;i<6;i++ )
	{
		if( NULL != m_data[i] )
		{
			delete [] m_data[i];
			m_data[i]=NULL;
		}
	}
}

void CCfgLiveSpotDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];

	unsigned short height = m_rect.Height() - y - 8 - m_length[DLG_OFFSET_SUB_BOTTOM] - m_length[COMBO_HEIGHT];

	x = m_length[DLG_OFFSET_SUB_LEFT] + (width - 240) / 2;
	int numChnn = s_loginSuccessReply.productInfo.videoInputNum;
	int numLocalChnn = s_loginSuccessReply.productInfo.localVideoInputNum;
	int numNetChnn = s_loginSuccessReply.productInfo.netVideoInputNum;
	
	m_scMode.Create(GetRandWndID(), x - 3, y, 120, m_length[COMBO_HEIGHT], this );
	m_scMode.SetCaption(m_pStrTable->String(E_STR_ID_SPLIT_MODE), false);


	m_scCountInfo.Create(GetRandWndID(), m_length[DLG_OFFSET_SUB_LEFT] + width - 60, y, 60, m_length[STATIC_HEIGHT], this);
	m_scCountInfo.SetAlignText(ALIGN_TEXT_RIGHT);
	m_scCountInfo.SetBKColor(COLOR_DLG_CHILD_BK);

	m_comboMode.Create(GUI::CWnd::GetRandWndID(),x+120,y,120,m_length[COMBO_HEIGHT],this,0,2);
	m_comboMode.AddItem("1X1",VIEW_SPLIT_1X1);
	m_comboMode.SetCurItemData(VIEW_SPLIT_1X1,false);
	m_comboMode.SetEnable(false,false);

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y += m_length[COMBO_HEIGHT] + 8;

	m_dwellModeDlg.SetSpot(true);
	m_dwellModeDlg.SetMaxChnn(numChnn, numLocalChnn,numNetChnn);
	m_dwellModeDlg.SetCfgDwellTime(NCFG_ITEM_SPOT_DWELL_TIME);
	m_dwellModeDlg.SetCfgSplitBase(NCFG_ITEM_SPOT_SPLIT_1X1);
	m_dwellModeDlg.SetCfgData(reinterpret_cast<unsigned long*>(m_data),&m_dwellTime, 1);
	m_dwellModeDlg.Create(GUI::CWnd::GetRandWndID(),x,y,width,height,this,0,3);
	//m_dwellModeDlg.ShowPage( m_comboMode.GetCurItemData(),false );
	//
	//////////////////////////////////////////////////////////////////////////
}

void CCfgLiveSpotDlg::OnModifyCfgData(NCFG_ITEM_ID cfgID)
{
	if( (!IsModify())&&IsChange(cfgID) )
	{
		SetModify(true);
	}
}

void CCfgLiveSpotDlg::OnModifyGroupInfo(const std::string& str, bool bRefresh)
{
	m_scCountInfo.SetCaption(str, bRefresh);
}

bool CCfgLiveSpotDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_SPOT_SPLIT_MODE:
		{
			assert(len==sizeof(unsigned long));

			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			m_comboMode.SetCurItemData(*pValue,false);
			//m_dwellModeDlg.ShowPage( 0,false );
		}
		return true;
		break;
	case NCFG_ITEM_SPOT_DWELL_TIME:
		{
			assert(len==sizeof(unsigned long));

			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			m_dwellTime = *pValue;
		}
		return true;
		break;
	case NCFG_ITEM_SPOT_SPLIT_1X1:
		{
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			assert(len ==sizeof(unsigned long)*(*pValue*1*1+2));
			if( NULL!=m_data[0])
			{
				delete [] m_data[0];
			}
			m_data[0] = new unsigned long[len];
			memcpy(m_data[0],pValue,len);

			if( VIEW_SPLIT_1X1 == m_comboMode.GetCurItemData() )
			{
				m_dwellModeDlg.ShowPage(0, bRefresh);
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

bool CCfgLiveSpotDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_SPOT_SPLIT_MODE:
		{
			assert(len==sizeof(unsigned long));

			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_comboMode.GetCurItemData();
		}
		return true;
		break;
	case NCFG_ITEM_SPOT_DWELL_TIME:
		{
			assert(len==sizeof(unsigned long));

			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_dwellTime;
		}
		return true;
		break;
	case NCFG_ITEM_SPOT_SPLIT_1X1:
		{
			assert(len ==sizeof(unsigned long)*((*m_data[0])*1*1+2));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			memcpy(pValue,m_data[0],len);
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

BEGIN_MSG_MAP(CCfgLiveSpotDlg, CCfgDlg)
	ON_MSG_EX(m_dwellModeDlg.GetID(),KEY_VIRTUAL_MSG_DEFAULT_TAG,OnClickDwellModeDlg)
END_MSG_MAP()


unsigned long CCfgLiveSpotDlg::OnClickDwellModeDlg(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
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
