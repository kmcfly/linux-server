#include "ShanghaiAlarmImageDialog.h"

CShanghaiAlarmImageDialog::CShanghaiAlarmImageDialog()
{
	m_codeTable = CCharCodeTable::Instance();
}

CShanghaiAlarmImageDialog::~CShanghaiAlarmImageDialog()
{

}

void CShanghaiAlarmImageDialog::OnInitial()
{
	GUI::CListCtrl& list = m_cfgList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CButton* pBtn = NULL;
	GUI::CEditCtrl* pEdit = NULL;
	GUI::CComboCtrl* pCombo = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y =m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[LIST_ITEM_HEIGHT] - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM]-m_length[LIST_ITEM_HEIGHT];

	/////List
	/////预算List边缘是否需要滚动框
	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头
	int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
	int numLocalChnn = s_loginSuccessReply.productInfo.localVideoInputNum;
	if( numChnn > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
	string outStr;
	outStr.clear();
	outStr = ChangeChinese("预截图时间");
	list.AddColumn(outStr.c_str(), width * 14 / 32);
	outStr.clear();
	outStr = ChangeChinese("截图保留时间");
	list.AddColumn(outStr.c_str(), width * 15 / 32-offsetEdge );

	char str[128] = {0};
	for ( int i = 0; i < numChnn; i++ )
	{
		if(i < numLocalChnn)
		{
			sprintf( str, "%d", i + 1 );
		}
		else
		{
			sprintf( str, "D%d", i + 1 - numLocalChnn );
		}
		list.AddItem( i, COL_CHNN, str );

		pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( i, COL_PRE_SNAP_TIME, WND_TYPE_COMBO) );
		assert( pCombo );
		strcpy( str, "63" );
		pCombo->AddItem( str , 63 );
		strcpy( str, "66" );
		pCombo->AddItem( str , 66 );
		strcpy( str, "69" );
		pCombo->AddItem( str , 69 );
		pCombo->SetCurItemData( 69, false );
		pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( i, COL_POST_SNAP_TIME, WND_TYPE_COMBO) );
		assert( pCombo );
		strcpy( str, "63" );
		pCombo->AddItem( str , 63 );
		strcpy( str, "66" );
		pCombo->AddItem( str , 66 );
		strcpy( str, "69" );
		pCombo->AddItem( str , 69 );
		pCombo->SetCurItemData( 69, false );

	}
	list.CreateListFocusMap();

	GUI::CRect rect = list.GetWindowRect();
	y += rect.Height() + 4;

	m_checkCtrl.Create(GetRandWndID(),x,y,m_length[CHECK_WIDTH],m_length[CHECK_HEIGHT],this,0,1);
	CCharCodeTable* pCode = CCharCodeTable::Instance();
	outStr.clear();
	outStr=ChangeChinese("启用移动侦测上传");
	m_staticNote.Create(GetRandWndID(),x+m_length[CHECK_WIDTH]+10,y,2*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT],this,outStr.c_str());//移动侦测上传启用

	y+= m_length[CHECK_HEIGHT]+4;
	

	m_cfgScAll.Create( GUI::CWnd::GetRandWndID(), x, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, width, m_length[STATIC_HEIGHT], this );
	snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_ALL ) );
	m_cfgScAll.SetCaption( str, false );

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y += m_length[LIST_ITEM_HEIGHT];
	GUI::CListCtrl& listAll = m_cfgListAll;
	listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 2, false );
	listAll.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
	listAll.AddColumn(m_pStrTable->String(E_STR_ID_PRE_RECORD_TIME), width * 14 / 32 - 12);
	listAll.AddColumn(m_pStrTable->String(E_STR_ID_POST_RECORD_TIME), width * 15 / 32 - 7);
	{
		int i = 0;

		pCheck = dynamic_cast<GUI::CCheckCtrl*>( listAll.AddItem( i, COL_CHNN, WND_TYPE_CHECK) );
		assert( pCheck );

		pCombo = dynamic_cast<GUI::CComboCtrl*>( listAll.AddItem( i, COL_PRE_SNAP_TIME, WND_TYPE_COMBO) );
		assert( pCombo );
		strcpy( str, "63" );
		pCombo->AddItem( str , 63 );
		strcpy( str, "66" );
		pCombo->AddItem( str , 66 );
		strcpy( str, "69" );
		pCombo->AddItem( str , 69 );
		pCombo->SetCurItemData(69, false );

		pCombo = dynamic_cast<GUI::CComboCtrl*>( listAll.AddItem( i, COL_POST_SNAP_TIME, WND_TYPE_COMBO) );
		assert( pCombo );
		strcpy( str, "63" );
		pCombo->AddItem( str , 63 );
		strcpy( str, "66" );
		pCombo->AddItem( str , 66 );
		strcpy( str, "69" );
		pCombo->AddItem( str , 69 );
		pCombo->SetCurItemData(69, false );
	}
	listAll.CreateListFocusMap();
}

void CShanghaiAlarmImageDialog::SetTip()
{
	int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
	AddTip(m_cfgListAll.GetItemWnd(0,COL_CHNN)->GetID(), E_STR_ID_TIP_01_39);
}

void CShanghaiAlarmImageDialog::UpdateView(bool bRefresh)
{
	// 除配置外的其他数据初始化数据设置
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListAll.GetItemWnd(0, COL_CHNN));
	if( pCheckAll )
	{
		pCheckAll->SetCheck( false, bRefresh );
	}

	CCfgDlg::UpdateView( bRefresh );
}

bool CShanghaiAlarmImageDialog::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_cfgList;

	switch (cfgID)
	{
	case NCFG_ITEM_NET_SHANGHAI_PRE_ALARM_SNAP_TIME:
		{
			int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );

			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			for ( int i = 0; i< numChnn; i++ )
			{
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( list.GetItemWnd( i, COL_PRE_SNAP_TIME) );
				assert( pCombo );
				pCombo->SetCurItemData( pValue[i], bRefresh );
			}
		}
		return true;
		break;
	case NCFG_ITEM_NET_SHANGHAI_POST_ALARM_SNAP_TIME:
		{
			int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );

			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			for ( int i = 0; i< numChnn; i++ )
			{
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( list.GetItemWnd( i, COL_POST_SNAP_TIME) );
				assert( pCombo );
				pCombo->SetCurItemData( pValue[i], bRefresh );
			}
		}
		return true;
		break;
	case NCFG_ITEM_NET_SHANGHAI_MOTIONUPLOAD_ENABLE:
		{
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			if(1 == *pValue)
			{
				m_checkCtrl.SetCheck(true,false);
			}
			else if(0 == *pValue)
			{
				m_checkCtrl.SetCheck(false,false);
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


bool CShanghaiAlarmImageDialog::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CListCtrl& list = m_cfgList;
	const GUI::CListCtrl& listAll = m_cfgListAll;

	bool bAll = false;
	{
		const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd( 0, COL_CHNN ) );
		assert( pCheck );
		bAll = pCheck->IsCheck();
	}

	switch (cfgID)
	{
	case NCFG_ITEM_NET_SHANGHAI_PRE_ALARM_SNAP_TIME:
		{
			int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );

			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			for ( int i = 0; i< numChnn; i++ )
			{
				GUI::CComboCtrl* pCombo = NULL;
				if( bAll )
				{
					pCombo = dynamic_cast<GUI::CComboCtrl*>( (GUI::CWnd*)listAll.GetItemWnd( 0, COL_PRE_SNAP_TIME) );
				}
				else
				{
					pCombo = dynamic_cast<GUI::CComboCtrl*>( (GUI::CWnd*)list.GetItemWnd( i, COL_PRE_SNAP_TIME) );
				}
				assert( pCombo );
				pValue[i] = pCombo->GetCurItemData();
			}
		}
		return true;
		break;
	case NCFG_ITEM_NET_SHANGHAI_POST_ALARM_SNAP_TIME:
		{
			int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert( len == sizeof(unsigned long) * numChnn );

			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			for ( int i = 0; i< numChnn; i++ )
			{
				GUI::CComboCtrl* pCombo = NULL;
				if( bAll )
				{
					pCombo = dynamic_cast<GUI::CComboCtrl*>( (GUI::CWnd*)listAll.GetItemWnd( 0, COL_PRE_SNAP_TIME) );
				}
				else
				{
					pCombo = dynamic_cast<GUI::CComboCtrl*>( (GUI::CWnd*)list.GetItemWnd( i, COL_POST_SNAP_TIME) );
				}
				assert( pCombo );
				pValue[i] = pCombo->GetCurItemData();
			}
		}
		return true;
		break;
	case NCFG_ITEM_NET_SHANGHAI_MOTIONUPLOAD_ENABLE:
		{
			unsigned long* pValue = reinterpret_cast< unsigned long*>(pData);
			if(m_checkCtrl.IsCheck())
			{
				 *pValue =1;
			}
			else
			{
				*pValue = 0;
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

BEGIN_MSG_MAP(CShanghaiAlarmImageDialog, CCfgDlg)
	ON_MSG_EX(m_cfgList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_cfgListAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
	ON_MSG(m_checkCtrl.GetID(),KEY_VIRTUAL_ENTER,OnClickCheck);
END_MSG_MAP()

unsigned long CShanghaiAlarmImageDialog::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	//2010-03-03 17:57:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListAll.GetItemWnd(0, COL_CHNN));
	assert(pCheckAll);
	if( pCheckAll->IsCheck() )
	{
		pCheckAll->SetCheck(false, true);
	}

	if (!IsModify())
	{

		switch (col)
		{
		case COL_PRE_SNAP_TIME:
			if(IsChange( NCFG_ITEM_NET_SHANGHAI_PRE_ALARM_SNAP_TIME ))
			{
				SetModify(true);
				EnableOkBtn(true,true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case COL_POST_SNAP_TIME:
			if(IsChange(NCFG_ITEM_NET_SHANGHAI_POST_ALARM_SNAP_TIME))
			{
				SetModify(true);
				EnableOkBtn(true,true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CShanghaiAlarmImageDialog::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	// 点选非All，先检查是否处于All状态，处于则认为做了修改
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListAll.GetItemWnd(0, COL_CHNN));
	if( pCheckAll->IsCheck() )
	{
		if (!IsModify())
		{
			if( COL_PRE_SNAP_TIME == col ||
				COL_POST_SNAP_TIME == col )
			{
				SetModify(true);
				EnableOkBtn(true,true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
	}


	// 点选All 直接认为做了修改
	if (!IsModify())
	{
		if( COL_CHNN == col )
		{
			SetModify(true);
			EnableOkBtn(true,true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;	
}

unsigned long CShanghaiAlarmImageDialog::OnClickCheck()
{
	if (IsChange(NCFG_ITEM_NET_SHANGHAI_MOTIONUPLOAD_ENABLE))
	{
		SetModify(true);
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
	return KEY_VIRTUAL_MSG_STOP;
}

string  CShanghaiAlarmImageDialog::ChangeChinese(const char *pstr)
{
	string outStr;
	if(NULL != m_codeTable)
	{
		if (-1 == m_codeTable->Convert(outStr, CCT_CODE_UTF8, pstr, CCT_CODE_GBK))
		{
			outStr = pstr;
		}
	}
	else
	{
		outStr = pstr;
	}
	return outStr;
}
//end
