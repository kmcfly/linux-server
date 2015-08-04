/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-14
** Name         : CfgPtzSerialPortDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgPtzSerialPortDlg.h"
#include "FuncCustom.h"

static char s_strBaudRate[][32] = {
	{"110"},// 0),
	{"300"},// 1),
	{"600"},// 2),
	{"1200"},// 3),
	{"2400"},// 4),
	{"4800"},// 5),
	{"9600"},// 6),
	{"19200"},// 7),
	{"38400"},// 8),
	{"57600"},// 9),
	{"115200"},// 10),
	{"230400"},// 11),
	{"460800"},// 12),
	{"921600"},// 13));
};


CCfgPtzSerialPortDlg::CCfgPtzSerialPortDlg()
{

}

CCfgPtzSerialPortDlg::~CCfgPtzSerialPortDlg()
{

}

void CCfgPtzSerialPortDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	GUI::CListCtrl& list = m_listCtrl;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CButton*	 pBtn = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[LIST_ITEM_HEIGHT] - m_length[DLG_OFFSET_SUB_BOTTOM];

	/////List
	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	//////////////////////////////////////////////////////////////////////////

	int row = 0;
	for(int i = 0; i < maxChnn; i++)
	{
		if((CH >> i) & 0x1)
		{
			m_rowChnnMap.insert(std::make_pair(row++, i));
		}
	}
	if(maxChnn  > maxListRow)
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_ENABLE), width * 3/32);	//name
	list.AddColumn(m_pStrTable->String(E_STR_ID_ADDRESS), width * 6/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_BAUD_RATE), width * 6/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_PROTOCOL), width * 7/32 - offsetEdge);
	list.AddColumn(m_pStrTable->String(E_STR_ID_VIRTUAL_CRUISE), width * 7/32 - offsetEdge);
	//////////////////////////////////////////////////////////////////////////

	char str[128] = {0};

	for ( int i = 0; i < effectChnn ; i++ )
	{
		//通道
		int chnnIndex = m_rowChnnMap.find(i)->second;
		if(chnnIndex < maxChnn )
		{
			sprintf(str, "%d", chnnIndex + 1);
		}
		list.AddItem( i, COL_CHANNEL, str);
		//使能
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem( i, COL_ENABLE, WND_TYPE_CHECK));
		assert(pCheck);
		//pCheck->SetCheck( true, false );
		//地址
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_ADDRESS, WND_TYPE_COMBO));
		assert(pCombo);
		for (int address = 1; address <= 255; ++address)
		{
			sprintf( str, "%d", address );
			pCombo->AddItem( str, address);
		}
		pCombo->SetCurItem( 0 );
		//波特率
		{
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_BAUD_RATE, WND_TYPE_COMBO));
			assert(NULL != pCombo);
			int count = sizeof(s_strBaudRate) / sizeof(s_strBaudRate[0]);
			for (int i = 0; i < count; ++i)
			{
				pCombo->AddItem(s_strBaudRate[i], i);
			}
		}
		//协议
		{
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_PROTOCOL, WND_TYPE_COMBO));
			assert(NULL != pCombo);
			
			for ( int i = static_cast<int>(PROTOCOL_PELCOP); i < static_cast<int>(PROTOCOL_END); ++i)
			{
				pCombo->AddItem(PTZ_PROTOCL_NAME[i], i);
			}
		}

		//巡航线类型
		{
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem( i, COL_CURISE_TYPE, WND_TYPE_CHECK));
			assert(NULL != pCheck);
		}
	}

	list.CreateListFocusMap();

	GUI::CRect rect = list.GetClientRect();
	y += rect.Height() + 4;
	{
		height = m_length[LIST_ITEM_HEIGHT];
		m_scAll.Create( GUI::CWnd::GetRandWndID(), x, y + (m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT])/2, width, m_length[STATIC_HEIGHT], this );
		m_scAll.SetCaption( m_pStrTable->String(E_STR_ID_ALL), false );

		y += height;
		height = m_length[LIST_ITEM_HEIGHT];
		GUI::CListCtrl& listAll = m_listAll;
		listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, false );
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3/32);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_ENABLE), width * 3/32);	//name
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_ADDRESS), width * 6/32);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_BAUD_RATE), width * 6/32);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_PROTOCOL), width * 7/32- offsetEdge);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_VIRTUAL_CRUISE), width * 7/32- offsetEdge);

		{
			int i = 0;
			//通道
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem( i, COL_CHANNEL, WND_TYPE_CHECK));
			assert(pCheck);
			if (0 == localVideoInputNum)
			{
				pCheck->SetEnable(false);
			}
			//pCheck->SetCheck( true, false );
           
			//使能
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem( i, COL_ENABLE, WND_TYPE_CHECK));
			assert(pCheck);
			pCheck->SetCheck( true, false );
			if (0 == localVideoInputNum)
			{
				pCheck->SetEnable(false);
			}

			//地址
			//pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem( i, COL_ADDRESS, WND_TYPE_COMBO));
			//assert(pCombo);
			//for (int address = 0; address < 255; ++address)
			//{
			//	sprintf( str, "%d", address+1 );
			//	pCombo->AddItem( str, address);
			//}
			//pCombo->SetCurItem( 0 );

			//波特率
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem( i, COL_BAUD_RATE, WND_TYPE_COMBO));
				assert(NULL != pCombo);
				int count = sizeof(s_strBaudRate) / sizeof(s_strBaudRate[0]);
				for (int i = 0; i < count; ++i)
				{
					pCombo->AddItem(s_strBaudRate[i], i);
				}
				pCombo->SetCurItem( 6 );
				if (0 == localVideoInputNum)
				{
					pCombo->SetEnable(false);
				}
			}

			//协议
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem( i, COL_PROTOCOL, WND_TYPE_COMBO));
				assert(NULL != pCombo);
				for ( int i = static_cast<int>(PROTOCOL_PELCOP); i < static_cast<int>(PROTOCOL_END); ++i)
				{
					pCombo->AddItem(PTZ_PROTOCL_NAME[i], i);
				}
				pCombo->SetCurItemData( static_cast<int>(PROTOCOL_TVI), false );

				if (0 == localVideoInputNum)
				{
					pCombo->SetEnable(false);
				}
			}

			//巡航线类型
			{
				pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem( i, COL_CURISE_TYPE, WND_TYPE_CHECK));
				assert(NULL != pCheck);
				
				if (0 == localVideoInputNum)
				{
					pCheck->SetEnable(false);
				}
			}
		}
		listAll.CreateListFocusMap();
	}
}

void CCfgPtzSerialPortDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	m_rowChnnMap.clear();
}

void CCfgPtzSerialPortDlg::UpdateView(bool bRefresh)
{	
	CCfgDlg::UpdateView( bRefresh );

	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	if( pCheckAll )
	{
		pCheckAll->SetCheck( false, bRefresh );
	}
}

bool CCfgPtzSerialPortDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{

    ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_PTZ_SERIAL:
		{
			assert(NULL != pData);
			assert(len == sizeof(PTZ_SERIAL_INFO) * maxChnn);
			const PTZ_SERIAL_INFO* pValue = reinterpret_cast<const PTZ_SERIAL_INFO*>(pData);

			GUI::CCheckCtrl* pCheck = NULL;
			GUI::CComboCtrl* pCombo = NULL;

			for (int ch = 0; ch < effectChnn; ch++)
			{
				int index = m_rowChnnMap.find(ch)->second;

				pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_listCtrl.GetItemWnd(ch, COL_ENABLE) );
				assert( NULL != pCheck );
				pCheck->SetCheck(pValue[index].bEnable,bRefresh);

				pCombo = dynamic_cast<GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(ch, COL_ADDRESS) );
				assert( NULL != pCombo );
				pCombo->SetCurItemData(pValue[index].devAddr, bRefresh);

				pCombo = dynamic_cast<GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(ch, COL_BAUD_RATE) );
				assert( NULL != pCombo );
				pCombo->SetCurItemData(pValue[index].baudRate, bRefresh);

				pCombo = dynamic_cast<GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(ch, COL_PROTOCOL) );
				assert( NULL != pCombo );
				pCombo->SetCurItemData(pValue[index].protocol, bRefresh);
			}

			for ( int i = localVideoInputNum; i<effectChnn; i++ )
			{
				pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_listCtrl.GetItemWnd(i, COL_ENABLE) );
				assert( NULL != pCheck );
				pCheck->SetEnable(false, bRefresh);

				pCombo = dynamic_cast<GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(i, COL_ADDRESS) );
				assert( NULL != pCombo );
				pCombo->SetEnable(false, bRefresh);

				pCombo = dynamic_cast<GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(i, COL_BAUD_RATE) );
				assert( NULL != pCombo );
				pCombo->SetEnable(false, bRefresh);

				pCombo = dynamic_cast<GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(i, COL_PROTOCOL) );
				assert( NULL != pCombo );
				pCombo->SetEnable(false, bRefresh);
			}
		}
		return true;
		break;
	case NCFG_ITEM_PTZ_CURISE_TYPE:
		{
			GUI::CCheckCtrl* pCheck = NULL;
			const unsigned long * const pFakeCurise = reinterpret_cast<const unsigned long*>(pData);
			assert(len == sizeof(unsigned long) * maxChnn);
			for (int ch = 0; ch < effectChnn; ++ch, pFakeCurise)
			{
				pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_listCtrl.GetItemWnd(ch, COL_CURISE_TYPE) );
				assert( NULL != pCheck );
				pCheck->SetCheck(pFakeCurise[ch], bRefresh);
			}
			for(int i=localVideoInputNum; i<effectChnn; i++)
			{
				pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_listCtrl.GetItemWnd(i, COL_CURISE_TYPE) );
				assert( NULL != pCheck );
				pCheck->SetEnable(false,bRefresh);
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

bool CCfgPtzSerialPortDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	const GUI::CListCtrl& list = m_listCtrl;
	const GUI::CListCtrl& listAll = m_listAll;

	bool bAll = false;
	{
		const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd( 0, COL_CHANNEL ) );
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
	case NCFG_ITEM_PTZ_SERIAL:
		{
			assert(NULL != pData);
			assert(len == sizeof(PTZ_SERIAL_INFO) * maxChnn);
			PTZ_SERIAL_INFO* pValue = reinterpret_cast<PTZ_SERIAL_INFO*>(pData);

			for (int ch = 0; ch < effectChnn; ch++)
			{
				GUI::CCheckCtrl* pCheck = NULL;
				GUI::CComboCtrl* pCbAddr = NULL;
				GUI::CComboCtrl* pCbRate = NULL;
				GUI::CComboCtrl* pCbProt = NULL;

				if( bAll )
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)listAll.GetItemWnd(0, COL_ENABLE));
					//pCbAddr = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_ADDRESS));
					pCbRate = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_BAUD_RATE));
					pCbProt = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_PROTOCOL));
				}
				else
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)list.GetItemWnd(ch, COL_ENABLE));
					pCbAddr = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ch,COL_ADDRESS));
					pCbRate = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ch,COL_BAUD_RATE));
					pCbProt = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ch,COL_PROTOCOL));
				}
				assert(pCheck);
				if( !bAll ) assert(pCbAddr);
				assert(pCbRate);
				assert(pCbProt);
				
				pValue[ch].bEnable = pCheck->IsCheck() ? 1 : 0;
				if( !bAll ) pValue[ch].devAddr = pCbAddr->GetCurItemData();
				pValue[ch].baudRate = pCbRate->GetCurItemData();
				pValue[ch].protocol = pCbProt->GetCurItemData();
			}
		}
		return true;
		break;
	case NCFG_ITEM_PTZ_CURISE_TYPE:
		{
			unsigned long *pFakeCurise = reinterpret_cast<unsigned long*>(pData);
			assert(len == sizeof(unsigned long) * maxChnn);
			GUI::CCheckCtrl* pCheck = NULL;

			for (int ch = 0; ch < effectChnn; ++ch)
			{
				if (bAll)
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)listAll.GetItemWnd(0, COL_CURISE_TYPE));
				}
				else
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)list.GetItemWnd(ch, COL_CURISE_TYPE));
				}

				pFakeCurise[ch] = pCheck->IsCheck() ? 1 : 0;
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


BEGIN_MSG_MAP(CCfgPtzSerialPortDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_listAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()

unsigned long CCfgPtzSerialPortDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if( COL_ENABLE == col ||
		COL_BAUD_RATE == col ||
		COL_PROTOCOL == col )
	{
		//2010-03-03 17:57:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
		GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
		assert(pCheckAll);
		if( pCheckAll->IsCheck() )
		{
			pCheckAll->SetCheck(false, true);
		}
	}

	if (!IsModify())
	{
		switch (col)
		{
		case COL_ENABLE:
		case COL_ADDRESS:
		case COL_BAUD_RATE:
		case COL_PROTOCOL:
		case COL_CURISE_TYPE:
			if ((IsChange(NCFG_ITEM_PTZ_SERIAL)) || (IsChange(NCFG_ITEM_PTZ_CURISE_TYPE)))
			{
				SetModify(true);
			}
			break;
		default:
			assert(false);
			break;
		}

		return KEY_VIRTUAL_MSG_CONTINUE;
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
}

unsigned long CCfgPtzSerialPortDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = static_cast<unsigned short>((keyID >> 16)&0x7fff);
	unsigned short col = static_cast<unsigned short>(keyID&0xffff);

	// 点选非All，先检查是否处于All状态，处于则认为做了修改
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCheckAll );
	assert( pCfgDlg );
	if( pCheckAll->IsCheck() )
	{
		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			if( COL_ENABLE == col		||
				COL_ADDRESS == col		||
				COL_BAUD_RATE == col	||
				COL_PROTOCOL == col		||
				COL_CURISE_TYPE == col)
			{
				SetModify(true);
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
		if( COL_CHANNEL == col )
		{
			SetModify(true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}


