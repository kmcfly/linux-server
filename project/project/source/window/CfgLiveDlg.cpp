/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-12
** Name         : CfgLiveDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgLiveDlg.h"
#include "Rect.h"
#include "FuncCustom.h"
CCfgLiveDlg::CCfgLiveDlg(): 
m_colorChnn(NULL),m_pColorChnnChange(NULL)
{

}

CCfgLiveDlg::~CCfgLiveDlg()
{

}

void CCfgLiveDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgLiveList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CButton* pBtn = NULL;
	GUI::CEditCtrl* pEdit = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - 2 * m_length[LIST_ITEM_HEIGHT] - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	/////预算List边缘是否需要滚动框
	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	int row = 0;
	for(int i = 0; i < maxChnn; i++)
	{
		if((CH >> i) & 0x1)
		{
			m_rowChnnMap.insert(std::make_pair(row++, i));
		}
	}
	
	if(effectChnn > maxListRow)
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CAMERA_NAME), width * 16 / 32);	//name
	list.AddColumn(m_pStrTable->String(E_STR_ID_SHOW_NAME), width * 6 / 32);
	if(IsSupportChangeLiveColor())
	{
		list.AddColumn(m_pStrTable->String(E_STR_ID_COLOR), width * 7 / 32 - offsetEdge);
	}

	char str[256] = {0};

	unsigned long noColorChangeChnn = 0;
	PRODUCT_TYPE product = CProduct::Instance()->ProductType();
	if (TD_2304SS_SDI == product)
	{
		noColorChangeChnn = 1;
	}
	else if (TD_2308SS_SDI == product || TD_2508HE_SDI == product)
	{
		noColorChangeChnn = 2;
	}
	for (int i = 0; i < effectChnn; i++)
	{
		int chnnIndex = m_rowChnnMap.find(i)->second;
		if(chnnIndex < localVideoInputNum)
		{
			sprintf(str, "%d", chnnIndex + 1);
		}
		else
		{
			sprintf(str, "%d", chnnIndex + 1 /*- localVideoInputNum*/);
		}
		list.AddItem(i, COL_CHNN, str);

		pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem( i, COL_CAMERA_NAME, WND_TYPE_EDIT));
		assert( pEdit );

		//pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem( i, COL_COVER, WND_TYPE_CHECK));
		//assert(pCheck);

		pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem( i, COL_SHOW_NAME, WND_TYPE_CHECK));
		assert(pCheck);

		if(IsSupportChangeLiveColor())
		{
			pBtn = dynamic_cast<GUI::CButton*>(list.AddItem( i, COL_COLOR, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]));
			assert(pBtn);
			pBtn->SetCaption( m_pStrTable->String( E_STR_ID_SETTING ), false);

			if (i < noColorChangeChnn)
			{
				pBtn->SetEnable(false, false);
			}
		}
	}

	list.CreateListFocusMap();

	GUI::CRect rect = list.GetClientRect();

	y += rect.Height() + 4;
	{
		height = m_length[LIST_ITEM_HEIGHT];
		GUI::CListCtrl& listAll = m_cfgLiveListAll;

		snprintf( str, 256, "%s", m_pStrTable->String( E_STR_ID_ALL ) );
		m_scAll.Create( GUI::CWnd::GetRandWndID(), x, y + (m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT])/2, width, m_length[STATIC_HEIGHT], this );
		m_scAll.SetCaption( str, false );
		y += height;
		height = m_length[LIST_ITEM_HEIGHT];
		listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0,2, false );
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_CAMERA_NAME), width * 16 / 32);	//name
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_SHOW_NAME), width * 6 / 32);
		if(IsSupportChangeLiveColor())
		{
			listAll.AddColumn(m_pStrTable->String(E_STR_ID_COLOR), width * 7 / 32);
		}
		
		{
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem( 0, COL_CHNN, WND_TYPE_CHECK));
			assert(pCheck);

			//pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem( 0, COL_COVER, WND_TYPE_CHECK));
			//assert(pCheck);

			pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem( 0, COL_SHOW_NAME, WND_TYPE_CHECK));
			assert(pCheck);
			pCheck->SetCheck(true, false);

			if(IsSupportChangeLiveColor())
			{
				pBtn = dynamic_cast<GUI::CButton*>(listAll.AddItem( 0, COL_COLOR, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]));
				assert(pBtn);
				pBtn->SetCaption( m_pStrTable->String( E_STR_ID_SETTING ), false);
			}
			
		}

		listAll.CreateListFocusMap();
	}

	//为color区域创建空间
	if( m_colorChnn )
	{
		delete [] m_colorChnn;
	}
	m_colorChnn = new unsigned long[maxChnn];
	memset(m_colorChnn, 0, maxChnn * sizeof(unsigned long));

	m_pColorChnnChange = new unsigned long [maxChnn];
	memset( m_pColorChnnChange, 0, ( sizeof( unsigned long ) * maxChnn ) );

	//增加录像状态的界面
	y-=height;
	x=m_rect.Width() - m_length[DLG_OFFSET_LEFT] * 15;
	width= m_length[CHECK_WIDTH]+150;
	m_liveRecStatus.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0,1);
	m_liveRecStatus.SetCaption(m_pStrTable->String(E_STR_ID_LIVE_REC_STATUS),true,false);
}

void CCfgLiveDlg::SetTip()
{
	ULONGLONG CH = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn);
	//int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
	if(IsSupportChangeLiveColor())
	{
		for(int i = 0; i < effectChnn; i++)
		{
			//AddTip(m_cfgLiveList.GetItemWnd(i,COL_SHOW_NAME)->GetID(),E_STR_ID_TIP_02_01);
			AddTip(m_cfgLiveList.GetItemWnd(i,COL_COLOR)->GetID(),E_STR_ID_TIP_02_02);
		}
		AddTip(m_cfgLiveListAll.GetItemWnd(0,COL_COLOR)->GetID(),E_STR_ID_TIP_02_05);
	}
	
	AddTip(m_cfgLiveListAll.GetItemWnd(0,COL_CHNN)->GetID(),E_STR_ID_TIP_01_39);
	//AddTip(m_cfgLiveListAll.GetItemWnd(0,COL_SHOW_NAME)->GetID(),E_STR_ID_TIP_02_04);
}

void CCfgLiveDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	if( m_colorChnn )
	{
		delete [] m_colorChnn;
		m_colorChnn = NULL;
	}

	if( m_pColorChnnChange )
	{
		delete [] m_pColorChnnChange;
		m_pColorChnnChange = NULL;
	}

	m_rowChnnMap.clear();
}

void CCfgLiveDlg::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	//if( GetShowTipType(TIP_DFOCUS) )
	//{
	//	int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
	//	for ( int i=0; i<numChnn; i++ )
	//	{
	//		if( newID == m_cfgLiveList.GetItemWnd(i,COL_CAMERA_NAME)->GetID() )
	//		{
	//			std::string strTip = m_pStrTable->String(E_STR_ID_TIP_02);
	//			strTip += m_cfgLiveList.GetItemWnd(i,COL_CAMERA_NAME)->GetCaption();
	//			MessageOut(GetTopestWnd(),strTip.c_str(),TIP_DFOCUS);
	//			return;
	//		}
	//	}
	//}

	CCfgDlg::OnChangeFocus(oldID,newID);
}

void CCfgLiveDlg::UpdateView(bool bRefresh)
{
	// 除配置外的其他数据初始化数据设置
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgLiveListAll.GetItemWnd(0, COL_CHNN));
	if( pCheckAll )
	{
		pCheckAll->SetCheck( false, bRefresh );
	}

	CCfgDlg::UpdateView( bRefresh );
}

//当在CfgLiveColorDlg.cpp中保存了新的颜色配置时，但在该页面直接退出而不保存配置信息到配置文件时要回复各个通道颜色参数
unsigned long CCfgLiveDlg::RecoverChange()
{
	assert( NULL != m_colorChnn );
	assert( NULL != m_pColorChnnChange );

	if (s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		m_cfgLiveColorDlg.RecoveryChnnColor( m_colorChnn, m_pColorChnnChange, s_loginSuccessReply.productInfo.videoInputNum );
	}
	else
	{
		m_cfgLiveColorDlg.RecoveryChnnColor( m_colorChnn, m_pColorChnnChange, s_loginSuccessReply.productInfo.netVideoInputNum );
	}
	
	//2010-05-24 zhl
	//在调色彩页面出现这样的情况
	//调节完色彩后点OK，在LIVE对话框点Apply,此时不退出，点Default,然后点Exit,在警告对话框中点Cancel退出，此时现场色彩被改变为默认值，但是配置中的数据不是默认值
	//在点Default和点Cannel时都会调用RecoverChange()函数把色彩从m_pColorChnnChange调整为m_colorChnn。
	//第一次时把色彩调整为默认值色彩值，第二次时由于m_pColorChnnChange中的值和m_colorChnn中的值相同而不会改变现场色彩，从而导致错误

	memcpy(m_pColorChnnChange,m_colorChnn, s_loginSuccessReply.productInfo.videoInputNum * sizeof(unsigned long));

	return 0;
}


bool CCfgLiveDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_cfgLiveList;

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_CAMERA_NAME:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
			assert( len == sizeof(NCFG_INFO_SHORT_NAME) * numChnn );

			CCfgDlg::GetConfig( NCFG_ITEM_CAMERA_NAME_MAX_LEN, &pTempData, tempDataLen );
			assert( tempDataLen == sizeof(unsigned long) );
			unsigned long max_len = *reinterpret_cast<unsigned long*>(pTempData);

			const NCFG_INFO_SHORT_NAME* pShortName = reinterpret_cast<const NCFG_INFO_SHORT_NAME*>(pData);
			for(int i = 0; i< effectChnn; i++)
			{
				GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(i, COL_CAMERA_NAME));
				assert( pEdit );
				pEdit->SetMaxStringLen(max_len);
				pEdit->SetCaption(pShortName[m_rowChnnMap.find(i)->second].name, bRefresh);
			}
		}
		return true;
		break;
	case NCFG_ITEM_SHOW_CAM_NAME:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
			assert(len == sizeof(unsigned long) * numChnn);

			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			for(int i = 0; i< effectChnn; i++)
			{
				GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(i, COL_SHOW_NAME));
				assert(pCheck);
				pCheck->SetCheck(pValue[m_rowChnnMap.find(i)->second], bRefresh);
			}
		}
		return true;
		break;
	case NCFG_ITEM_VIDEO_COLOR:
		{
			if(IsSupportChangeLiveColor())
			{
				int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
				assert(len==sizeof(unsigned long)*numChnn);
				const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
				assert(m_colorChnn);
				memcpy(m_colorChnn,pValue,sizeof(unsigned long)*numChnn);
			}
			
		}
		return true;
		break;
	case NCFG_ITEM_LIVE_REC_STATUS:
		{
			assert(len==sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			if(0 == *pValue)
			{
				m_liveRecStatus.SetCheck(false, bRefresh);
			}
			else
			{
				m_liveRecStatus.SetCheck(true, bRefresh);
			}
			return true;
			break;
		}
	default:
		return false;
		break;
	}
	
	return false;
}

bool CCfgLiveDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CListCtrl& list = m_cfgLiveList;
	const GUI::CListCtrl& listAll = m_cfgLiveListAll;

	bool bAll = false;
	{
		const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd( 0, COL_CHNN ) );
		assert( pCheck );
		bAll = pCheck->IsCheck();
	}

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_CAMERA_NAME:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
			assert(len == sizeof(NCFG_INFO_SHORT_NAME) * numChnn);

			NCFG_INFO_SHORT_NAME* pShortName = reinterpret_cast<NCFG_INFO_SHORT_NAME*>(pData);
			for (int i = 0; i < effectChnn; i++)
			{
				const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(i, COL_CAMERA_NAME));
				assert(pEdit);
				strcpy(pShortName[m_rowChnnMap.find(i)->second].name, pEdit->GetCaption().c_str());
			}
		}
		return true;
		break;
	case NCFG_ITEM_SHOW_CAM_NAME:
		{
			int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
			assert(len == sizeof(unsigned long) * numChnn);

			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			for ( int i = 0; i< effectChnn; i++ )
			{
				GUI::CCheckCtrl* pCheck = NULL;
				if( bAll )
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>( (GUI::CWnd*)listAll.GetItemWnd( 0, COL_SHOW_NAME) );
				}
				else
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>( (GUI::CWnd*)list.GetItemWnd( i, COL_SHOW_NAME) );
				}
				assert( pCheck );
				if( pCheck->IsCheck() )
				{
					pValue[m_rowChnnMap.find(i)->second] = 1;
				}
				else
				{
					pValue[m_rowChnnMap.find(i)->second] = 0;
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_VIDEO_COLOR:
		{
			if(IsSupportChangeLiveColor())
			{
				int numChnn = maxChnn; //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
				assert(len==sizeof(unsigned long)*numChnn);
				unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
				assert(m_colorChnn);
				memcpy(pValue,m_colorChnn,sizeof(unsigned long)*numChnn);
			}
		}
		return true;
		break;
	case NCFG_ITEM_LIVE_REC_STATUS:
		{
			assert(len==sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			if(m_liveRecStatus.IsCheck())
			{
				*pValue=1;
			}
			else
			{
				*pValue=0;
			}
			return true;
			break;
		}
	default:
		return false;
		break;
	}

	return false;
}

BEGIN_MSG_MAP(CCfgLiveDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgLiveList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_cfgLiveListAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
	ON_MSG(m_liveRecStatus.GetID(),KEY_VIRTUAL_ENTER,OnCheck)
END_MSG_MAP()

unsigned long CCfgLiveDlg::OnCheck()
{
	if (IsChange(NCFG_ITEM_LIVE_REC_STATUS))
	{
		SetModify(true);
		GUI::CWnd* pWndTop = GetTopestWnd();
		GUI::CWnd* pBapply = pWndTop->GetCtrl(CTRLID_DEFAULT_BTN_OK);
		assert(pBapply);
		pBapply->SetEnable(true,true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgLiveDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if( COL_SHOW_NAME == col || COL_COLOR == col )
	{
		//2010-03-03 17:57:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
		GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgLiveListAll.GetItemWnd(0, COL_CHNN));
		assert(pCheckAll);
		if( pCheckAll->IsCheck() )
		{
			pCheckAll->SetCheck(false, true);
		}
	}

	GUI::DLG_END_VALUE colorDlgValue = DLG_ESC;
	if( COL_COLOR == col )
	{
		if(IsSupportChangeLiveColor())
		{
			unsigned short xOff = m_width - CCfgLiveColorDlg::GetWidth() - CCfgLiveColorDlg::CFG_DLG_RIGHT_X;

			if (m_width < 800)
			{
				xOff -= 40;
			}

			//unsigned int localVideoInputNum = s_loginSuccessReply.productInfo.localVideoInputNum;
		
			ULONGLONG CH = 0;
			int localVideoInputNum = 0, netVideoInputNum = 0;
			int maxChnn = 0, effectChnn = 0;

			IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

			maxChnn = localVideoInputNum + netVideoInputNum;

			int chnn = m_rowChnnMap.find(row)->second;
		
			m_cfgLiveColorDlg.SetChnnInfo(m_colorChnn, maxChnn, chnn);
			colorDlgValue = PopupLiveColorDlg( xOff, CENTER_POS, m_cfgLiveColorDlg.GetWidth(), m_cfgLiveColorDlg.GetHeight(), chnn );
			memset( m_pColorChnnChange, 0, ( sizeof( unsigned long ) * maxChnn ) );

			if( colorDlgValue == DLG_OK )
			{
				m_cfgLiveColorDlg.GetChnnColor(m_colorChnn,maxChnn);
				memcpy( m_pColorChnnChange, m_colorChnn, ( sizeof( unsigned long ) * maxChnn ) );
				if (!IsModify())
				{
					SetModify(true);
					return KEY_VIRTUAL_MSG_CONTINUE;
				}
			}
		}
	}

	if (!IsModify())
	{

		switch (col)
		{
		case COL_CAMERA_NAME:
			if (IsChange(NCFG_ITEM_CAMERA_NAME))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case COL_SHOW_NAME:
			if(IsChange(NCFG_ITEM_SHOW_CAM_NAME))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgLiveDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	// 点选非All，先检查是否处于All状态，处于则认为做了修改
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgLiveListAll.GetItemWnd(0, COL_CHNN));
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCheckAll );
	assert( pCfgDlg );

	GUI::DLG_END_VALUE colorDlgValue = DLG_ESC;
	if( COL_COLOR == col && pCheckAll->IsCheck())
	{
		if(IsSupportChangeLiveColor())
		{
			unsigned short xOff = m_width - CCfgLiveColorDlg::GetWidth() - CCfgLiveColorDlg::CFG_DLG_RIGHT_X;
		
			ULONGLONG CH = 0;
			int localVideoInputNum = 0, netVideoInputNum = 0;
			int maxChnn = 0, effectChnn = 0;

			IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

			maxChnn = localVideoInputNum + netVideoInputNum;

			//unsigned int localVideoInputNum = s_loginSuccessReply.productInfo.localVideoInputNum;


			m_cfgLiveColorDlg.SetChnnInfo(m_colorChnn, maxChnn ,-1);
		
			colorDlgValue = PopupLiveColorDlg( xOff, CENTER_POS, CCfgLiveColorDlg::GetWidth(), CCfgLiveColorDlg::GetHeight(), -1 );

			memset( m_pColorChnnChange, 0, ( sizeof( unsigned long ) * localVideoInputNum ) );

			if( colorDlgValue == DLG_OK )
			{
				m_cfgLiveColorDlg.GetChnnColor(m_colorChnn,/*localVideoInputNum*/maxChnn);
				memcpy( m_pColorChnnChange, m_colorChnn, ( sizeof( unsigned long ) * localVideoInputNum ) );
				if (!IsModify())
				{
					SetModify(true);
					return KEY_VIRTUAL_MSG_CONTINUE;
				}
			}
		}
	}

	if( pCheckAll->IsCheck() )
	{
		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			if( COL_SHOW_NAME == col )
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}

			if( COL_COLOR == col && DLG_OK == colorDlgValue)
			{
				if(IsSupportChangeLiveColor())
				{
					SetModify(true);
				}
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
	}
	else
	{
		pCfgDlg->SetRefreshView( false );
	}

	// 点选All 直接认为做了修改
	if (!IsModify())
	{
		if( COL_CHNN == col )
		{
			//if(IsSupportChangeLiveColor())
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

DLG_END_VALUE CCfgLiveDlg::PopupLiveColorDlg( int x, int y, int cx, int cy, int chnn )
{
	CMessageMan *pMsgMan = CMessageMan::Instance();
	DLG_END_VALUE retVal;

	//切换到相关通道的大画面
	NET_DISPLAY_INFO displayInfo;
	memset(&displayInfo, 0, sizeof(NET_DISPLAY_INFO));

	displayInfo.type		= NET_DISPLAY_CHANGE_DATA_PACK;
	displayInfo.time_out	= -1;
	displayInfo.rec			= 0;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	//maxChnn = localVideoInputNum + netVideoInputNum;
	maxChnn =IsSupportHybridDisplay()? (localVideoInputNum + netVideoInputNum):localVideoInputNum;

	MESSAGE_DATA msgData;
	if(-1 == chnn)
	{
		unsigned char chnnNum =  maxChnn;  //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
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
		else if ((16 == chnnNum) || (12 == chnnNum))
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

		displayInfo.split_mode[DISPLAY_LAYER_ANALOG] = splitMode;
		displayInfo.split_mode[DISPLAY_LAYER_NET] = splitMode;
		for (int i = 0; i < chnnNum; i++)
		{
			if(i < localVideoInputNum)
			{
				displayInfo.data[i] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, i, i);
			}
			else
			{
				displayInfo.data[i] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, i, i - localVideoInputNum);
			}
		}
		displayInfo.data_len = chnnNum;

		pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE,(unsigned char *)&displayInfo, sizeof(NET_DISPLAY_INFO), msgData );
		pMsgMan->ReleaseMsgBuff( msgData );
	}
	else
	{
		displayInfo.split_mode[DISPLAY_LAYER_ANALOG] = VIEW_SPLIT_1X1;
		displayInfo.split_mode[DISPLAY_LAYER_NET] = VIEW_SPLIT_1X1;
		
		if(chnn < localVideoInputNum)
		{
			displayInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, 0, chnn);
		}
		else
		{
			displayInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, 0, chnn - localVideoInputNum);
		}

		displayInfo.data_len = 1;

		pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char *)&displayInfo, sizeof(NET_DISPLAY_INFO), msgData );
		pMsgMan->ReleaseMsgBuff( msgData );
	}

	retVal = m_cfgLiveColorDlg.DoModal( this, x , y, cx, cy, false);

	//返回之前的大画面状态
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0 , msgData );
	
	pMsgMan->ReleaseMsgBuff( msgData );

	return retVal;
}

//end
