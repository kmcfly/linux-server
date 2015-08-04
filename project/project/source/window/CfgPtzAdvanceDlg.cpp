/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-14
** Name         : CfgPtzAdvanceDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgPtzAdvanceDlg.h"
#include "CfgPtzPresetDlg.h"
#include "CruiseEditDlg.h"
#include "MessageMan.h"
#include "UserMan.h"
#include "FuncCustom.h"

CCfgPtzAdvanceDlg::CCfgPtzAdvanceDlg() : m_pPresetList(NULL), m_pCruiseList(NULL)
{

}

CCfgPtzAdvanceDlg::~CCfgPtzAdvanceDlg()
{

}


void CCfgPtzAdvanceDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	GUI::CListCtrl& list = m_listCtrl;
	GUI::CButton*	 pBtn = NULL;
	GUI::CCheckCtrl* pCheck = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y /*- 2 * m_length[LIST_ITEM_HEIGHT] */- m_length[DLG_OFFSET_SUB_BOTTOM];

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
	

	if(maxChnn > maxListRow)
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_PRESET), width * 9/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CRUISE), width * 10/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_TRACK), width * 10/32 - offsetEdge);

	char str[128] = {0};
	
	for ( int i = 0; i <effectChnn; i++ )
	{
		int chnnIndex = m_rowChnnMap.find(i)->second;
		if(chnnIndex < maxChnn)
		{
			sprintf(str, "%d", chnnIndex + 1);
		}
		else
		{
			sprintf(str, "%d", chnnIndex + 1 /*- localVideoInputNum*/);
		}
		list.AddItem( i, COL_CHANNEL, str);

		pBtn = dynamic_cast<GUI::CButton*>(list.AddItem( i, COL_PRESET, WND_TYPE_BUTTON, m_length[BTN_WIDTH]));
		assert(pBtn);
		pBtn->SetCaption( m_pStrTable->String(E_STR_ID_SETTING), false );

		pBtn = dynamic_cast<GUI::CButton*>(list.AddItem( i, COL_CRUISE, WND_TYPE_BUTTON, m_length[BTN_WIDTH]));
		assert(pBtn);
		pBtn->SetCaption( m_pStrTable->String(E_STR_ID_SETTING), false );

		pBtn = dynamic_cast<GUI::CButton*>(list.AddItem( i, COL_TRACK, WND_TYPE_BUTTON, m_length[BTN_WIDTH]));
		assert(pBtn);
		pBtn->SetCaption( m_pStrTable->String(E_STR_ID_SETTING), false );
	}

	list.CreateListFocusMap();

	//////////////////////////////////////////////////////////////////////////
	assert(maxChnn/*s_loginSuccessReply.productInfo.localVideoInputNum*/ > 0);
	if (NULL != m_pPresetList)
	{
		delete [] m_pPresetList;
	}
	m_pPresetList = new std::vector<PTZ_PRESET> [maxChnn];

	if (NULL != m_pCruiseList)
	{
		delete [] m_pCruiseList;
	}
	m_pCruiseList = new std::vector<CRUISE_INFO> [maxChnn];
}


void CCfgPtzAdvanceDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();
	if (NULL != m_pPresetList)
	{
		delete [] m_pPresetList;
		m_pPresetList = NULL;
	}

	if (NULL != m_pCruiseList)
	{
		delete [] m_pCruiseList;
		m_pCruiseList = NULL;
	}

	m_ptzConfigList.clear();

	m_rowChnnMap.clear();
}


bool CCfgPtzAdvanceDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	//////////////////////////////////////////////////////////////////////////

	if ((cfgID >= NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE) && (cfgID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + s_loginSuccessReply.productInfo.videoInputNum))
	{
		int chnn = cfgID - NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE;
		assert(NULL != m_pPresetList);
		assert(chnn < maxChnn);
		m_pPresetList[chnn].clear();

		if ((NULL != pData) && (len > 0))
		{
			assert(len % sizeof(PTZ_PRESET) == 0);
		
			const PTZ_PRESET* pPreset = reinterpret_cast<const PTZ_PRESET*>(pData);
			unsigned short presetCount = 0;
			unsigned short subLen = 0;
			GetSubItemInfo(cfgID, presetCount, subLen);
			for (int i = 0; i < presetCount; ++i, ++pPreset)
			{
				m_pPresetList[chnn].push_back(*pPreset);
			}
		}

		return true;
	}  
	else if ((cfgID >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE) & (cfgID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + s_loginSuccessReply.productInfo.videoInputNum))
	{
		int chnn = cfgID - NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE;
		assert(NULL != m_pCruiseList);
		assert(chnn < maxChnn);
		m_pCruiseList[chnn].clear();

		if ((NULL != pData) && (len > 0))
		{
			unsigned short curise_count = 0;
			unsigned short sub_len = 0;
			GetSubItemInfo(cfgID, curise_count, sub_len);

			const NCFG_INFO_CRUISE_HEAD* pHead = reinterpret_cast<const NCFG_INFO_CRUISE_HEAD*>(pData);
			for (int i = 0; i < curise_count; ++i, ++pHead)
			{
				//if( ( MAX_CRUISE_NUM <= pHead->index ) || ( MAX_CRUISE_PRESET_NUM <= pHead->cruisepointnum ) )
				//{
				//	continue;
				//}

				CRUISE_INFO cruise_info;
				cruise_info.index = pHead->index;
				cruise_info.strName = pHead->name;
				const PTZ_CURISE_POINT* pPoint = reinterpret_cast<const PTZ_CURISE_POINT*>(pData + pHead->offset);
				for (int j = 0; j < pHead->cruisepointnum; ++j, ++pPoint)
				{
					cruise_info.point_list.push_back(*pPoint);
				}
				m_pCruiseList[chnn].push_back(cruise_info);
			}
		}

		return true;
	} 
	else if( NCFG_ITEM_PTZ_SERIAL == cfgID )
	{
		//获得每个通道相关协议信息
		if( ( NULL != pData ) && ( len > 0 ) )
		{
			assert( 0 == ( len % sizeof( PTZ_SERIAL_INFO ) ) );
			unsigned short ConfigCount = ( len / sizeof( PTZ_SERIAL_INFO ) );
			m_ptzConfigList.clear();
			const PTZ_SERIAL_INFO *pPTZConfig = reinterpret_cast<const PTZ_SERIAL_INFO*>(pData);
			for( int i=0; i<ConfigCount; ++i, ++pPTZConfig )
			{
				m_ptzConfigList.push_back( *pPTZConfig );
			}

#if defined(__DVR_ANCHOR__)
			SetListBtn(m_ptzConfigList,false);
#endif

		}

	}
	else
	{
	}

	return false;
}

bool CCfgPtzAdvanceDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	//////////////////////////////////////////////////////////////////////////


	if ((cfgID >= NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE) & (cfgID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + s_loginSuccessReply.productInfo.videoInputNum))
	{
		int chnn = cfgID - NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE;
		assert(NULL != m_pPresetList);
		assert(chnn < maxChnn);

		if (NULL != pData)
		{
			delete [] pData;
			pData = NULL;	
		}
		len = 0;

		if (!m_pPresetList[chnn].empty())
		{
			len = m_pPresetList[chnn].size() * sizeof(PTZ_PRESET);
			pData = new BYTE[len];
			memset(pData, 0, len);

			PTZ_PRESET* pPreset = reinterpret_cast<PTZ_PRESET*>(pData);

			for (std::vector<PTZ_PRESET>::const_iterator iter = m_pPresetList[chnn].begin(); iter != m_pPresetList[chnn].end(); ++iter, ++pPreset)
			{
				*pPreset = *iter;
			}

			//////////////////////////////////////////////////////////////////////////
			//const_cast<CCfgPtzAdvanceDlg*>(this)->SetSubItemInfo(cfgID, m_pPresetList[chnn].size(), sizeof(PTZ_PRESET));
		}		
		else
		{
			//const_cast<CCfgPtzAdvanceDlg*>(this)->SetSubItemInfo(cfgID, 0, 0);
		}
		return true;
	} 
	else if ((cfgID >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE) & (cfgID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + s_loginSuccessReply.productInfo.videoInputNum))
	{
		int chnn = cfgID - NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE;
		assert(NULL != m_pCruiseList);
		assert(chnn < maxChnn);

		if (NULL != pData)
		{
			delete [] pData;
			pData = NULL;
		}

		len = 0;

		if (!m_pCruiseList[chnn].empty())
		{

			for (std::vector<CRUISE_INFO>::const_iterator iter = m_pCruiseList[chnn].begin(); iter != m_pCruiseList[chnn].end(); ++iter)
			{
				len += (iter->point_list.size() * sizeof(PTZ_CURISE_POINT) + sizeof(NCFG_INFO_CRUISE_HEAD));
			}

			pData = new BYTE[len];
			memset(pData, 0, len);

			int offset = sizeof(NCFG_INFO_CRUISE_HEAD) * m_pCruiseList[chnn].size();
			NCFG_INFO_CRUISE_HEAD* pHead = reinterpret_cast<NCFG_INFO_CRUISE_HEAD*>(pData);
	
			for (std::vector<CRUISE_INFO>::const_iterator iter = m_pCruiseList[chnn].begin(); iter != m_pCruiseList[chnn].end(); ++iter, ++pHead)
			{
				pHead->index = iter->index;
				pHead->cruisepointnum = iter->point_list.size();
				strcpy(pHead->name, iter->strName.c_str());
				pHead->offset = offset;
				
				PTZ_CURISE_POINT * pPoint = reinterpret_cast<PTZ_CURISE_POINT*>(pData + pHead->offset);
				for (CRUISE_POINT_LIST::const_iterator point_iter = iter->point_list.begin(); point_iter != iter->point_list.end(); ++point_iter, ++pPoint)
				{
					*pPoint = *point_iter;
					
					offset += sizeof(PTZ_CURISE_POINT);
				}
			}

			//////////////////////////////////////////////////////////////////////////
			//const_cast<CCfgPtzAdvanceDlg*>(this)->SetSubItemInfo(cfgID, m_pCruiseList[chnn].size(), -1);
		}
		else
		{
			//const_cast<CCfgPtzAdvanceDlg*>(this)->SetSubItemInfo(cfgID, 0, 0);
		}
		return true;
	} 
	else
	{
	}

	return false;
}


void CCfgPtzAdvanceDlg::UpdateView(bool bRefresh)
{
	CCfgDlg::UpdateView( bRefresh );
	SetListBtn(m_ptzConfigList, bRefresh);

}
void CCfgPtzAdvanceDlg::SetListBtn(std::vector<PTZ_SERIAL_INFO>& ptzConfigList, bool bRefresh)
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	//////////////////////////////////////////////////////////////////////////

	int numChnn = maxChnn;  //CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
	GUI::CListCtrl& list = m_listCtrl;
	GUI::CButton*	 pBtn = NULL;

	for ( int i = 0; i < effectChnn; i++ )
	{
		int index = m_rowChnnMap.find(i)->second;

		unsigned char PTZProtocolID = ptzConfigList[index].protocol;

		pBtn = dynamic_cast<GUI::CButton*>(list.GetItemWnd( i, COL_PRESET ) );
		assert(pBtn);
		pBtn->SetEnable( PTZSupportAction( PTZProtocolID, PROTOCOL_SUPPORT_PRESET ), bRefresh );

		pBtn = dynamic_cast<GUI::CButton*>(list.GetItemWnd( i, COL_CRUISE ) );
		assert(pBtn);
		pBtn->SetEnable( PTZSupportAction( PTZProtocolID, PROTOCOL_SUPPORT_CURISE ) & (~PTZSupportAction( PTZProtocolID, PROTOCOL_DISABLE_CONFIGURE_CRUISE)) , bRefresh );

		pBtn = dynamic_cast<GUI::CButton*>(list.GetItemWnd( i, COL_TRACK ) );
		assert(pBtn);
		if ( i < localVideoInputNum )
		{
			pBtn->SetEnable( PTZSupportAction( PTZProtocolID, PROTOCOL_SUPPORT_TRACK ), bRefresh );
		}
		else
		{
			pBtn->SetEnable(false,bRefresh);
		}
	}
}

void CCfgPtzAdvanceDlg::GetConfigList(std::vector<PTZ_SERIAL_INFO>& ptzConfigList)
{
	PTZ_SERIAL_INFO *pPTZConfig;
	unsigned long len;

	GetConfig(NCFG_ITEM_PTZ_SERIAL, reinterpret_cast<unsigned char**> (&pPTZConfig), len);

	unsigned short ConfigCount = ( len / sizeof( PTZ_SERIAL_INFO ) );
	ptzConfigList.clear();
	m_ptzConfigList.clear();
	for(int i =0; i< ConfigCount; i++, pPTZConfig++)
	{
		ptzConfigList.push_back( *pPTZConfig );
		m_ptzConfigList.push_back(*pPTZConfig);
	}
	
}

void CCfgPtzAdvanceDlg::GetSubItemInfoFromDlg( const NCFG_ITEM_ID itemID, unsigned short& subItemCount, unsigned short& subItemLen)
{
	if( ( itemID >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE ) & ( itemID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + s_loginSuccessReply.productInfo.videoInputNum ))
	{
		int chnn = itemID - NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE;
		assert(NULL != m_pCruiseList);
		assert(chnn < s_loginSuccessReply.productInfo.videoInputNum);

		subItemCount =  m_pCruiseList[chnn].size();
		subItemLen = -1;
	} 
}

unsigned long CCfgPtzAdvanceDlg::RecoverChange()
{
	//恢复所有预览过巡航线的通道
	unsigned long chnn = 0xffffffff;
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CRUISE_PREVIEW_RECOVER, reinterpret_cast< unsigned char * >( &chnn ), sizeof( unsigned long ) );

	return CCfgDlg::RecoverChange();
}

BEGIN_MSG_MAP(CCfgPtzAdvanceDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_listAll.GetID(), KEY_VIRTUAL_ENTER, OnClickListAll)
END_MSG_MAP()

unsigned long CCfgPtzAdvanceDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	//////////////////////////////////////////////////////////////////////////

	assert(row < maxChnn);

	ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_PTZ_CTRL);

	if( !((auth >> row ) & 0x01 ))
	{
		//该通道没有控制权限
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);

		GetRootWnd()->Repaint();
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	//设置可控制的通道
	CPTZViewDlg::SetCanCtrlChnn(auth);

	switch (col)
	{
	case COL_PRESET:
		{
			int index = m_rowChnnMap.find(row)->second;

			CMessageMan::Instance()->SetCurrPTZChnn(index);

			CCfgPtzPresetDlg presetDlg;
			presetDlg.SetPTZConfig(m_ptzConfigList);
			presetDlg.SetChnn(index);
			presetDlg.SetPresetList(m_pPresetList[index]);
			if (DLG_OK == presetDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false ))
			{
				presetDlg.GetPresetList(m_pPresetList[index]);

				if (!IsModify())
				{
					if (IsChange(static_cast<NCFG_ITEM_ID>(NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + index)))
					{
						SetModify(true);

						CMessageMan::Instance()->SetCurrPTZChnn(INVALID_INDEX(unsigned char));
						return KEY_VIRTUAL_MSG_CONTINUE;
					}
				}
			}
			
			CMessageMan::Instance()->SetCurrPTZChnn(INVALID_INDEX(unsigned char));
		}
		break;
	case COL_CRUISE:
		{
			int index = m_rowChnnMap.find(row)->second;

			CMessageMan::Instance()->SetCurrPTZChnn(index);

			CCfgPtzCruiseDlg cruiseDlg;
			cruiseDlg.SetCruiseData(m_pCruiseList[index]);
			CCruiseEditDlg::SetPresetList(m_pPresetList[index]);
			if (DLG_OK == cruiseDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false ))
			{
				cruiseDlg.GetCruiseData(m_pCruiseList[index]);

				if (!IsModify())
				{
					if (IsChange(static_cast<NCFG_ITEM_ID>(NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + index)))
					{
						SetModify(true);
						CMessageMan::Instance()->SetCurrPTZChnn(INVALID_INDEX(unsigned char));
						return KEY_VIRTUAL_MSG_CONTINUE;
					}
				}
			}

			CMessageMan::Instance()->SetCurrPTZChnn(INVALID_INDEX(unsigned char));
		}
		break;
	case COL_TRACK:
		{
			int index = m_rowChnnMap.find(row)->second;

			NET_DISPLAY_INFO disp;
			memset(&disp, 0, sizeof(NET_DISPLAY_INFO));
			disp.type = NET_DISPLAY_FREEZE;

			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&disp, sizeof(NET_DISPLAY_INFO), msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			CMessageMan::Instance()->SetCurrPTZChnn(index);
			m_ptzTrackDlg.SetPTZConfig(m_ptzConfigList);
			m_ptzViewDlg.SetCtrlDlg(&m_ptzTrackDlg, CPTZTrackCtrl::WIDTH, CPTZTrackCtrl::HEIGHT ,CPTZViewDlg::CURR_PTZCTRLDLG_TRACK);

			m_ptzViewDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0, msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			CMessageMan::Instance()->SetCurrPTZChnn(INVALID_INDEX(unsigned char));
		}
		break;
	default:
		break;
	}
	
	GetRootWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgPtzAdvanceDlg::OnClickListAll( unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos )
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	if (COL_CHANNEL == col)
	{
		if (pCheckAll->IsCheck())
		{
			GUI::CButton* pButton = dynamic_cast<GUI::CButton*>(m_listAll.GetItemWnd(0, COL_PRESET));
			assert(NULL != pButton);
			pButton->SetEnable(pCheckAll->IsCheck(), true );

			pButton = dynamic_cast<GUI::CButton*>(m_listAll.GetItemWnd(0, COL_CRUISE));
			assert(NULL != pButton);
			pButton->SetEnable(pCheckAll->IsCheck(), true );

			pButton = dynamic_cast<GUI::CButton*>(m_listAll.GetItemWnd(0, COL_TRACK));
			assert(NULL != pButton);
			pButton->SetEnable(pCheckAll->IsCheck(), true );
		}
	}

	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCheckAll );
	assert( pCfgDlg );
	if( pCheckAll->IsCheck() )
	{
		switch (col)
		{
		case COL_PRESET:
			{
				CCfgPtzPresetDlg presetDlg;
				std::vector<PTZ_PRESET> presetList;
				presetDlg.SetPresetList(presetList);
				if (DLG_OK == presetDlg.DoModal(this))
				{
					if (pCheckAll->IsCheck())
					{
						for (int i = 0; i < s_loginSuccessReply.productInfo.localVideoInputNum; ++i)
						{
							presetDlg.GetPresetList(m_pPresetList[i]);
						}
					} 
				}
			}
			break;
		case COL_CRUISE:
			{
				CCfgPtzCruiseDlg cruiseDlg;
				std::vector<CRUISE_INFO> cruiseList;
				cruiseDlg.SetCruiseData(cruiseList);
				if (DLG_OK == cruiseDlg.DoModal(this))
				{
					if (pCheckAll->IsCheck())
					{
						for (int i = 0; i < s_loginSuccessReply.productInfo.localVideoInputNum; ++i)
						{
							cruiseDlg.GetCruiseData(m_pCruiseList[i]);
						}
					} 
				}
			}
			break;
		case COL_TRACK:
			{
				m_ptzViewDlg.SetCtrlDlg(&m_ptzTrackDlg, CPTZTrackCtrl::WIDTH, CPTZTrackCtrl::HEIGHT);
				m_ptzViewDlg.DoModal(this);
			}
			break;
		default:
			break;
		}

		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			if( COL_PRESET == col ||
				COL_CRUISE == col ||
				COL_TRACK  == col )
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}

	}else{

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


