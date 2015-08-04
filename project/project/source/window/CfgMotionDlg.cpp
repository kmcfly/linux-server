/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgMotionDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgMotionDlg.h"
#include "MotionAreDlg.h"
#include "MessageMan.h"
#include "FuncCustom.h"

//////////////////////////////////////////////////////////////////////////

CCfgMotionManDlg::CCfgMotionManDlg()
{

}

CCfgMotionManDlg::~CCfgMotionManDlg()
{

}

void CCfgMotionManDlg::OnPreInitial()
{
#ifndef __DVR_ANCHOR__
	AddCaption(m_pStrTable->String(E_STR_ID_TITLE_MOTION));
	AddCaption(m_pStrTable->String(E_STR_ID_MOTION));
	AddCaption(m_pStrTable->String(E_STR_ID_SCHEDULE));
#else
	AddCaption(m_pStrTable->String(E_STR_ID_TITLE_MOTION));
	AddCaption(m_pStrTable->String(E_STR_ID_TITLE_MOTION));
	AddCaption(m_pStrTable->String(E_STR_ID_TITLE_SCHEDULE));
#endif

	CCfgManDlg2<CCfgMotionDlg,CCfgScheduleChildDlg>::OnPreInitial();
	GetPage2().SetMaxChnn(s_loginSuccessReply.productInfo.videoInputNum);
	GetPage2().SetScheduleCfgID(NCFG_ITEM_MOTION_SCH_WEEK, NCFG_ITEM_MOTION_SCH_HOLIDAY);
}

void CCfgMotionManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_MOTION_SCH_WEEK);

#ifdef __DVR_BASIC__
	itemList.push_back(NCFG_ITEM_EMAIL_RECV_INFO);
#endif

	itemList.push_back(NCFG_ITEM_MOTION_ENABLE);
	itemList.push_back(NCFG_ITEM_MOTION_AREA_INFO);
	#ifndef __CUSTOM_US02_NEW__
	itemList.push_back(NCFG_ITEM_MOTION_HOLD_TIME);
	#endif
	itemList.push_back(NCFG_ITEM_MOTION_TO_ALARM_OUT);
	itemList.push_back(NCFG_ITEM_MOTION_TO_RECORD);
	itemList.push_back(NCFG_ITEM_MOTION_TO_BIG_VIEW);
#ifdef __DVR_BASIC__	
	itemList.push_back(NCFG_ITEM_MOTION_TO_EMAIL_ENABLE);
	itemList.push_back(NCFG_ITEM_MOTION_TO_EMAIL_ADDR);
#endif
	if(s_loginSuccessReply.productInfo.videoInputNum > 0)
	{
		itemList.push_back(NCFG_ITEM_MOTION_TO_PTZ);
	}
	#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	itemList.push_back(NCFG_ITEM_MOTION_TO_FTP_ENABLE);
#endif
	itemList.push_back(NCFG_ITEM_MOTION_TO_BUZZ);
	itemList.push_back(NCFG_ITEM_MOTION_TO_SNAP);
#ifdef __NETSERVER_HXHT__
	itemList.push_back(NCFG_ITEM_HXHT_MOTION_AREA_INFO);
#endif
}

//////////////////////////////////////////////////////////////////////////

CCfgMotionDlg::CCfgMotionDlg() : m_pMotionInfo(NULL)
, m_pTriggerInfo(NULL)
{
#ifdef __NETSERVER_HXHT__
	m_hxhtpData=NULL;
#endif
}

CCfgMotionDlg::~CCfgMotionDlg()
{
	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
		m_pTriggerInfo = NULL;
	}

	if (NULL != m_pMotionInfo)
	{
		delete [] m_pMotionInfo;
		m_pMotionInfo = NULL;
	}
#ifdef __NETSERVER_HXHT__
	
	if(NULL!=m_hxhtpData)
	{
		delete [] m_hxhtpData;
		m_hxhtpData = NULL;

	}
	
#endif
}

//////////////////////////////////////////////////////////////////////////
void CCfgMotionDlg::OnInitial()
{
	CCfgDlg::OnInitial();

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM] - 2 * m_length[LIST_ITEM_HEIGHT] - 4;

	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List���ͷ

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

	m_listCtrl.Create(GetRandWndID(), x, y, width, height, this, 0, 0);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_ENABLE), width * 5 / 32/*70*/);
	#ifndef __CUSTOM_US02_NEW__
		m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_HOLD_TIME), width * 9 / 32/*150*/);
	#endif
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_TRIGGER), width * 7 / 32/*160*/);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_AREA), width * 8 / 32 - offsetEdge);

	char str[32] = {0};
	GUI::CWnd * pWnd = NULL;
	for (int ch = 0; ch < effectChnn; ++ch)
	{
		int chnnIndex = m_rowChnnMap.find(ch)->second;
		if(chnnIndex < localVideoInputNum)
		{
			sprintf(str, "%d", chnnIndex + 1);
		}
		else
		{
			sprintf(str, "%d", chnnIndex + 1 /*- localVideoInputNum*/);
		}
		m_listCtrl.AddItem(ch, COL_CHANNEL, str);

		pWnd = m_listCtrl.AddItem(ch, COL_ENABLE, WND_TYPE_CHECK);

		#ifndef __CUSTOM_US02_NEW__
		pWnd = m_listCtrl.AddItem(ch, COL_HOLD_TIME, WND_TYPE_COMBO);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->AddItem("5", 5);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->AddItem("10",10);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->AddItem("20", 20);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->AddItem("30", 30);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->AddItem("60", 60);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->AddItem("120", 120);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->AddItem(m_pStrTable->String(E_STR_ID_ALWAYS), -1);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->SetCurItemData(10, false);
		(reinterpret_cast<GUI::CComboCtrl*>(pWnd))->SetDropDownCount(7);
		#endif

		pWnd = m_listCtrl.AddItem(ch, COL_TRIGGER, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]);
		pWnd->SetCaption(m_pStrTable->String(E_STR_ID_SETTING), false);

		pWnd = m_listCtrl.AddItem(ch, COL_AREA, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]);
		pWnd->SetCaption(m_pStrTable->String(E_STR_ID_SETTING), false);
	}

	m_listCtrl.CreateListFocusMap();

	GUI::CRect rect = m_listCtrl.GetWindowRect();

	y += rect.Height() + 4;
	m_staticAll.Create(GetRandWndID(), x, y + (m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT])/2, width, m_length[STATIC_HEIGHT], this);
	m_staticAll.SetCaption(m_pStrTable->String(E_STR_ID_ALL), false);

	y += m_length[LIST_ITEM_HEIGHT];
	m_listCtrlAll.Create(GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 1, false);
	m_listCtrlAll.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
	m_listCtrlAll.AddColumn(m_pStrTable->String(E_STR_ID_ENABLE), width * 5 / 32/*70*/);
	#ifndef __CUSTOM_US02_NEW__
		m_listCtrlAll.AddColumn(m_pStrTable->String(E_STR_ID_HOLD_TIME), width * 9 / 32/*150*/);
	#endif
	m_listCtrlAll.AddColumn(m_pStrTable->String(E_STR_ID_TRIGGER), width * 7 / 32/*160*/);
	m_listCtrlAll.AddColumn(m_pStrTable->String(E_STR_ID_AREA), width * 8 / 32);

	m_listCtrlAll.AddItem(0, COL_CHANNEL, WND_TYPE_CHECK);
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrlAll.AddItem(0, COL_ENABLE, WND_TYPE_CHECK));
	assert(pCheck);
	pCheck->SetCheck(true, false);
	#ifndef __CUSTOM_US02_NEW__
	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listCtrlAll.AddItem(0, COL_HOLD_TIME, WND_TYPE_COMBO));
	assert(pCombo);
	pCombo->AddItem("5", 5);
	pCombo->AddItem("10",10);
	pCombo->AddItem("20", 20);
	pCombo->AddItem("30", 30);
	pCombo->AddItem("60", 60);
	pCombo->AddItem("120", 120);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_ALWAYS), -1);
	pCombo->SetCurItemData(10, false);
	pCombo->SetDropDownCount(7);
	#endif

	m_listCtrlAll.CreateListFocusMap();
	
	//////////////////////////////////////////////////////////////////////////
	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
		m_pTriggerInfo = NULL;
	}

	m_pTriggerInfo = new ALARM_TRIGGER_INFO[maxChnn];	
	memset(m_pTriggerInfo, 0, sizeof(ALARM_TRIGGER_INFO) * maxChnn);
}

void CCfgMotionDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
		m_pTriggerInfo = NULL;
	}

	if (NULL != m_pMotionInfo)
	{
		delete [] m_pMotionInfo;
		m_pMotionInfo = NULL;
	}

	m_rowChnnMap.clear();
}

void CCfgMotionDlg::UpdateView(bool bRefresh)
{
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrlAll.GetItemWnd(0, COL_CHANNEL));
	if( pCheckAll )
	{
		pCheckAll->SetCheck( false, bRefresh );
	}

	CCfgDlg::UpdateView( bRefresh );
}

unsigned long CCfgMotionDlg::RecoverChange()
{
	assert(NULL != m_pMotionInfo);

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	int numChnn = maxChnn;  //s_loginSuccessReply.productInfo.localVideoInputNum;

	NET_MOTION_INFO* pNetMotionInfo = new NET_MOTION_INFO[numChnn];
	memset(pNetMotionInfo, 0, sizeof(NET_MOTION_INFO) * numChnn);

	for (int i = 0; i < numChnn; i++ )
	{
		pNetMotionInfo[i].chnn = i;
		memcpy(&pNetMotionInfo[i].detectInfo, &m_pMotionInfo[i], sizeof(MOTION_DETECT_INFO));
	}

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_MOTION_AREA_SET, (unsigned char*)(pNetMotionInfo), sizeof(NET_MOTION_INFO) * numChnn);

	delete []pNetMotionInfo;

	return 0;
}

bool CCfgMotionDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	int count = maxChnn;  //s_loginSuccessReply.productInfo.localVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_MOTION_ENABLE:
		{
			assert(len == sizeof(unsigned long) * count);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CCheckCtrl* pCtrl = NULL;
			for (int ch = 0; ch < effectChnn; ch++)
			{
				pCtrl = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.GetItemWnd(ch, COL_ENABLE));
				assert( NULL != pCtrl );
				pCtrl->SetCheck(pValue[m_rowChnnMap.find(ch)->second], bRefresh);
			}

		}
		return true;
		break;
	case NCFG_ITEM_MOTION_AREA_INFO:
		{
			assert(len == sizeof(MOTION_DETECT_INFO) * count);
			if (NULL != m_pMotionInfo)
			{
				delete [] m_pMotionInfo;
			}
			m_pMotionInfo = new MOTION_DETECT_INFO[count];
			memcpy(m_pMotionInfo, pData, len);
		}
		return true;
		break;
#ifdef __NETSERVER_HXHT__
	case NCFG_ITEM_HXHT_MOTION_AREA_INFO:
		{
			assert(len == sizeof(HXHT_MOTION_AREA_INFO) * count);
			if(NULL!=m_hxhtpData)
			{
				delete [] m_hxhtpData;
			}
			m_hxhtpData=new HXHT_MOTION_AREA_INFO[count];
			memcpy(m_hxhtpData,pData,len);
		}
		return true;
		break;
#endif
		#ifndef __CUSTOM_US02_NEW__
	case NCFG_ITEM_MOTION_HOLD_TIME:
		{
			assert(len == sizeof(unsigned long) * count/*s_loginSuccessReply.productInfo.localVideoInputNum*/);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCombo = NULL;
			for (int ch = 0; ch < effectChnn; ++ch)
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(ch, COL_HOLD_TIME));
				assert( NULL != pCombo );
				int index = m_rowChnnMap.find(ch)->second;
				if( pValue[index] != static_cast<unsigned long>(-1) )
				{
					pCombo->SetCurItemData(pValue[index], bRefresh);
				}
				else
				{
					pCombo->SetCurItemData(-1, bRefresh);
				}
			}

		}
		return true;
		break;
		#endif
	case NCFG_ITEM_MOTION_TO_BUZZ:
		{
			assert(len == sizeof(unsigned long) * count);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].Buzzer = pValue[index];
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG) * count);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].AlarmOut = pValue[index];
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_RECORD:
		{
			assert(len == sizeof(ULONGLONG) * count);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].Record = pValue[index];
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_PTZ:
		{
			BYTE video_count = s_loginSuccessReply.productInfo.videoInputNum;//maxChnn; //temporary

			assert(len == sizeof(TO_PTZ) * count * video_count);
			const TO_PTZ* pValue = reinterpret_cast<const TO_PTZ*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				for (BYTE ch = 0; ch < video_count; ch++)
				{
					m_pTriggerInfo[index].ToPtz[ch] = pValue[index * video_count + ch];
				}				
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_FTP_ENABLE:
		{
			assert(len == sizeof(ULONGLONG) * count);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; ++i)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].ftpVideo = pValue[index];
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_SNAP:
		{
			assert(len == sizeof(ULONGLONG) * count);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			for(BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].SnapCH = pValue[index];
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_BIG_VIEW:
		{
			assert( len == sizeof(ULONGLONG) * count );
			assert( NULL != m_pTriggerInfo );
			const ULONGLONG *pValue = reinterpret_cast<const ULONGLONG *>(pData);
			for( BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].FullScreen = pValue[index];
			}
		}
		return true;
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_MOTION_TO_EMAIL_ENABLE:
		{
			assert(len == sizeof(unsigned long) * count);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].EmailEnable = pValue[index];
			}		
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_EMAIL_ADDR:
		{
			//��õ�������ǰ3���ռ��˵�ַ
			unsigned long tempDataLen = 0;
			BYTE* pTempData = NULL;
			CCfgDlg::GetConfig(NCFG_ITEM_EMAIL_RECV_INFO, &pTempData, tempDataLen);
			char *pInfo = reinterpret_cast<char *>(pTempData);
			if(pInfo != NULL)
			{
				for(BYTE i = 0; i < effectChnn; ++i)	
				{
					int index = m_rowChnnMap.find(i)->second;
					memcpy(m_pTriggerInfo[index].AddrInfo, pInfo, sizeof(m_pTriggerInfo[0].AddrInfo));
				}
			}

			assert(len == sizeof(unsigned long) * count);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].EmailAddr = pValue[index];
			}	
		}
		return true;
		break;
#endif
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgMotionDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	const GUI::CListCtrl& listAll = m_listCtrlAll;

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

	int count = maxChnn; //s_loginSuccessReply.productInfo.localVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_MOTION_ENABLE:
		{
			assert(len == sizeof(unsigned long) * count);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CCheckCtrl* pCtrl = NULL;
			for (int ch = 0; ch < effectChnn; ch++)
			{
				if(bAll)
				{
					pCtrl = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd(0, COL_ENABLE) );
				}
				else
				{
					pCtrl = dynamic_cast<const GUI::CCheckCtrl*>( m_listCtrl.GetItemWnd(ch, COL_ENABLE) );
				}

				assert( NULL != pCtrl );
				pValue[m_rowChnnMap.find(ch)->second] = pCtrl->IsCheck() ? 1 : 0;
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_AREA_INFO:
		{
			assert(len == sizeof(MOTION_DETECT_INFO) * count);
			assert(NULL != m_pMotionInfo);
			MOTION_DETECT_INFO* pInfo = reinterpret_cast<MOTION_DETECT_INFO*>(pData);

			memcpy(pInfo, m_pMotionInfo, len);
		}
		return true;
		break;
/**********************************zxx�¼�***************************************/
#ifdef __NETSERVER_HXHT__
	case NCFG_ITEM_HXHT_MOTION_AREA_INFO:
		{
			assert(len == sizeof(HXHT_MOTION_AREA_INFO) * count);
			assert(NULL != m_pMotionInfo);
			HXHT_MOTION_AREA_INFO* pInfo= reinterpret_cast<HXHT_MOTION_AREA_INFO*>(pData);
			memcpy(pInfo,m_hxhtpData,len);
			
		}
		return true;
		break;
#endif
/**********************************zxx�¼�***************************************/
	#ifndef __CUSTOM_US02_NEW__
	case NCFG_ITEM_MOTION_HOLD_TIME:
		{
			assert(len == sizeof(unsigned long) * count);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = NULL;
			for (int ch = 0; ch < effectChnn; ch++)
			{
				if(bAll)
				{
					pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_listCtrlAll.GetItemWnd(0, COL_HOLD_TIME));
				}
				else
				{
					pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(ch, COL_HOLD_TIME));
				}
				assert(NULL != pCombo );
				pValue[m_rowChnnMap.find(ch)->second] = pCombo->GetCurItemData();
			}
		}
		return true;
		break;
		#endif
	case NCFG_ITEM_MOTION_TO_BUZZ:
		{
			assert(len == sizeof(unsigned long) * count);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				pValue[index] = m_pTriggerInfo[index].Buzzer;
			}		
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG) * count);
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				pValue[index] = m_pTriggerInfo[index].AlarmOut;
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_RECORD:
		{
			assert(len == sizeof(ULONGLONG) * count);
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				pValue[index] = m_pTriggerInfo[index].Record;
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_PTZ:
		{
			BYTE video_count = s_loginSuccessReply.productInfo.videoInputNum;//maxChnn;  //temporary
			assert(len == sizeof(TO_PTZ) * count * video_count);
			TO_PTZ* pValue = reinterpret_cast<TO_PTZ*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; ++i)
			{
				int index = m_rowChnnMap.find(i)->second;
				for (BYTE ch = 0; ch < video_count; ch++)
				{
					pValue[index * video_count + ch] = m_pTriggerInfo[index].ToPtz[ch];
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_FTP_ENABLE:
		{
			ULONGLONG * pValue = reinterpret_cast<ULONGLONG *>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; ++i)
			{
				int index = m_rowChnnMap.find(i)->second;
				pValue[i] = m_pTriggerInfo[i].ftpVideo;
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_SNAP:
		{
			assert(len == sizeof(ULONGLONG) * count);
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			for(BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				pValue[index] = m_pTriggerInfo[index].SnapCH;
			}
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_BIG_VIEW:
		{
			assert( len == sizeof(ULONGLONG) * count );
			assert( NULL != m_pTriggerInfo );
			ULONGLONG *pValue = reinterpret_cast<ULONGLONG *>(pData);
			for( BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				pValue[index] = m_pTriggerInfo[index].FullScreen;
			}
		}
		return true;
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_MOTION_TO_EMAIL_ENABLE:
		{
			assert(len == sizeof(unsigned long) * count);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				pValue[index] = m_pTriggerInfo[index].EmailEnable;
			}	
		}
		return true;
		break;
	case NCFG_ITEM_MOTION_TO_EMAIL_ADDR:
		{
			assert(len == sizeof(unsigned long) * count);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				pValue[index] = m_pTriggerInfo[index].EmailAddr;
			}	
		}
		return true;
		break;
#endif
	default:
		return false;
		break;
	}

	return false;
}

BEGIN_MSG_MAP(CCfgMotionDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_listCtrlAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()


unsigned long CCfgMotionDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = static_cast<unsigned short>((keyID >> 16)&0x7fff);
	unsigned short col = static_cast<unsigned short>(keyID&0xffff);

	#ifdef __CUSTOM_US02_NEW__
	if( (COL_ENABLE == col) || 
		(COL_TRIGGER == col) ||
		(COL_AREA == col))
	#else
	if( (COL_ENABLE == col) || 
		(COL_HOLD_TIME == col) ||
		(COL_TRIGGER == col) ||
		(COL_AREA == col) )
	#endif
	{
		//����ϰ�� : �в�����All��������, �ͽ�Allѡ��ȡ��
		GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrlAll.GetItemWnd(0, COL_CHANNEL));
		assert(pCheckAll);
		if( pCheckAll->IsCheck() )
		{
			pCheckAll->SetCheck(false, true);
		}
	}

	switch (col)
	{
	case COL_ENABLE:
		if ((!IsModify()) && (IsChange(NCFG_ITEM_MOTION_ENABLE)))
		{
			SetModify(true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
		break;
	#ifndef __CUSTOM_US02_NEW__
	case COL_HOLD_TIME:
		if ((!IsModify()) && (IsChange(NCFG_ITEM_MOTION_HOLD_TIME)))
		{
			SetModify(true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
		break;
	#endif
	case COL_TRIGGER:
		{
			int index = m_rowChnnMap.find(row)->second;

			CCfgTriggerDlg dlg;
			m_pTriggerInfo[index].PtzSafeChnn = index;
			//m_pTriggerInfo[index].PtzSafeChnn = -1;
			dlg.SetTriggerInfo(m_pTriggerInfo[index], index);
			if (DLG_OK == dlg.DoModal(this))
			{
				dlg.GetTriggerInfo(m_pTriggerInfo[index]);
				if (!IsModify())
				{
					if ((IsChange(NCFG_ITEM_MOTION_TO_ALARM_OUT)) ||
						(IsChange(NCFG_ITEM_MOTION_TO_RECORD)) ||
						(IsChange(NCFG_ITEM_MOTION_TO_BIG_VIEW))||
#ifdef __DVR_BASIC__	
						(IsChange(NCFG_ITEM_MOTION_TO_EMAIL_ENABLE))||
						(IsChange(NCFG_ITEM_MOTION_TO_EMAIL_ADDR))||
#endif
						(IsChange(NCFG_ITEM_MOTION_TO_PTZ)) ||
						(IsChange(NCFG_ITEM_MOTION_TO_FTP_ENABLE)) ||
						(IsChange(NCFG_ITEM_MOTION_TO_BUZZ)) ||
						(IsChange(NCFG_ITEM_MOTION_TO_SNAP)))
					{
						SetModify(true);
						return KEY_VIRTUAL_MSG_CONTINUE;
					}
				}
			}
		}
		break;
	case COL_AREA:
		{
			int index = m_rowChnnMap.find(row)->second;

			CMotionAreDlg dlg;
			assert(NULL != m_pMotionInfo);
			//assert(row < s_loginSuccessReply.productInfo.localVideoInputNum);

			if (DLG_OK == PopupMontionAreaDlg( dlg, index ))
			{
				dlg.GetAreaInfo(m_pMotionInfo[index]);
				if ((!IsModify()) && (IsChange(NCFG_ITEM_MOTION_AREA_INFO)))
				{
#ifdef __NETSERVER_HXHT__
					CCfgHxhtMotion Hxht(&m_pMotionInfo[row]);
					CLIENT_AREA savesqure[32];//���32������
					for(int savecount=0;savecount<32;savecount++)
					{
						memset(&savesqure[savecount],0,sizeof(CLIENT_AREA));
					}
					Hxht.ChangeHxhtSqure(savesqure);
					
					for(int i=0;i<32;i++)
					{
						if(savesqure[i].enable)
						{
							m_hxhtpData[row].areaRect[i].xStart=savesqure[i].y*351/22;
							m_hxhtpData[row].areaRect[i].yStart=savesqure[i].x*287/15;
							m_hxhtpData[row].areaRect[i].width=savesqure[i].width*351/22;
							m_hxhtpData[row].areaRect[i].height=savesqure[i].height*287/15;
							m_hxhtpData[row].areaRect[i].bEnable=true;
							memset(m_hxhtpData[row].areaRect[i].resv,0,3);
						}
					}
					m_hxhtpData->sensitivity = m_pMotionInfo->sensitivity;

#endif
					SetModify(true);
					return KEY_VIRTUAL_MSG_CONTINUE;
				}
			}
		}
		break;
	default:
		break;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgMotionDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = static_cast<unsigned short>((keyID >> 16)&0x7fff);
	unsigned short col = static_cast<unsigned short>(keyID&0xffff);

	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrlAll.GetItemWnd(0, COL_CHANNEL));
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>(this->GetParent());

	if( pCheckAll->IsCheck() )
	{
		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			#ifdef __CUSTOM_US02_NEW__
			if( COL_ENABLE == col )
			#else
			if( (COL_ENABLE == col) ||
				(COL_HOLD_TIME == col) )
			#endif
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

	// ��ѡAll ֱ����Ϊ�����޸�
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


/******************************************************************************
*Function:
*		PopupMontionAreaDlg( CMotionAreDlg &motionAreadlg, int chnn )
*Description:
*		������motion����ʱ�л������ͨ���Ĵ���
*Input:
*		motionAreaDlg 
*		chnn	ͨ���ţ�Ϊ-1��ʾ����ͨ��
*Output
*		����DLG_END_VALUE ��������
*Date:
*		2010.02.04 
********************************************************************************/


DLG_END_VALUE CCfgMotionDlg::PopupMontionAreaDlg( CMotionAreDlg &motionAreadlg, int chnn )
{
	DLG_END_VALUE retVal;

	//�л������ͨ���Ĵ���
	NET_DISPLAY_INFO displayInfo;
	memset(&displayInfo, 0, sizeof(NET_DISPLAY_INFO));

	displayInfo.type		= NET_DISPLAY_CHANGE_DATA_PACK;
	displayInfo.time_out	= -1;
	displayInfo.rec			= 0;

	
	MESSAGE_DATA msgData;
	if(-1 == chnn)
	{
		assert(false);
		return DLG_CANCEL;
	}
	else
	{
		//ѡ��ͨ��ʱֻ�л������ͨ���Ĵ���
		if (chnn >= s_loginSuccessReply.productInfo.localVideoInputNum)
		{
			displayInfo.split_mode[DISPLAY_LAYER_NET] = VIEW_SPLIT_1X1;
			displayInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, 0, chnn-s_loginSuccessReply.productInfo.localVideoInputNum);
		}
		else
		{
			displayInfo.split_mode[DISPLAY_LAYER_ANALOG] = VIEW_SPLIT_1X1;
			displayInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, 0, chnn);
		}
		displayInfo.data_len = 1;
 
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char *)&displayInfo, sizeof(NET_DISPLAY_INFO), msgData );
		CMessageMan::Instance()->ReleaseMsgBuff( msgData );

		assert( NULL != m_pMotionInfo );
		motionAreadlg.SetAreaInfo(m_pMotionInfo[chnn], chnn);
	}
	
	retVal = motionAreadlg.DoModal( this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false );

	if(DLG_OK != retVal && motionAreadlg.IsTested(false))
	{
		//�����;ȡ������ԭ��Ӧ��motion����
		motionAreadlg.SetMotionArea(m_pMotionInfo[chnn]);
	}

	//����֮ǰ�Ĵ���״̬
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0 , msgData );
	CMessageMan::Instance()->ReleaseMsgBuff( msgData );

	return retVal;
}


//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

CCfgMotionManAnchorDlg::CCfgMotionManAnchorDlg()
{

}

CCfgMotionManAnchorDlg::~CCfgMotionManAnchorDlg()
{

}

void CCfgMotionManAnchorDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_MOTION), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 200;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;

	m_anMotion.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MOTION));
	m_anMotion.SetImage(BMP_ANCHOR_MOTION, BMP_ANCHOR_MOTION_F, BMP_ANCHOR_MOTION);

	y += 60;
	m_anSchedule.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_SCHEDULE));
	m_anSchedule.SetImage(BMP_ANCHOR_SCHEDULE, BMP_ANCHOR_SCHEDULE_F, BMP_ANCHOR_SCHEDULE);

	y += 60;
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
}

BEGIN_MSG_MAP(CCfgMotionManAnchorDlg, GUI::CDialog)
	ON_MSG(m_anMotion.GetID(), KEY_VIRTUAL_ENTER, OnAnchorMotion)
	ON_MSG(m_anSchedule.GetID(), KEY_VIRTUAL_ENTER, OnAnchorSchedule)
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
END_MSG_MAP()

unsigned long CCfgMotionManAnchorDlg::OnAnchorMotion()
{
	m_cfgMotionManDlg.SetAnchorPage(CCfgManDlg2<CCfgMotionDlg,CCfgScheduleChildDlg>::PAGE_1);
	m_cfgMotionManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgMotionManAnchorDlg::OnAnchorSchedule()
{
	m_cfgMotionManDlg.SetAnchorPage(CCfgManDlg2<CCfgMotionDlg,CCfgScheduleChildDlg>::PAGE_2);
	m_cfgMotionManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgMotionManAnchorDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

///*****************************zxx �¼�***************************************/
#ifdef __NETSERVER_HXHT__
CCfgHxhtMotion::CCfgHxhtMotion(MOTION_DETECT_INFO *pInfo)
{
	m_pHxhtInfo=pInfo;
}
CCfgHxhtMotion::~CCfgHxhtMotion()
{

}
///**********************************zxx�¼�***************************************/
int CCfgHxhtMotion::CheckSqure(int xstart,int ystart,int yend)
{
	for(int x=xstart;x<15;x++)
	{
		for(int y=ystart;y<yend;y++)
		{
			if(!m_big[x][y].recover)
			{
				if(m_big[x][y].enable)
				{
					m_big[x][y].recover=true;
					if((x==14)&&(y==yend-1))//ȫ��Ļ�ж�
					{
						return 15-xstart;
					}
				}
				else
				{
					if(y<yend)//����һ�С��Ѹ��еĶ����ó�û�и���
					{
						for(int y=ystart;y<yend;y++)
						{
							m_big[x][y].recover=false;
						}
					}
					return x-xstart;//��������
				}
			}
		}
	}
	return -1;
}
void  CCfgHxhtMotion::ChangeHxhtSqure(CLIENT_AREA *savesqure)
{

	//�����ؽ���ת���ɻ��Ż�ͨ��Ҫ�ľ������򲢱���
	int rowNum=22;//ÿ�ж��ٸ���,22
	int colNum=15;//ÿ�ж��ٸ���,15


	CLIENT_AREA tmpsqure;//��ʱ��¼
	memset(&tmpsqure,0,sizeof(CLIENT_AREA));
	int x=0;
	int y=0;
	int length=0;
	int width=0;
	bool firstcount=false;
	int squrecount=0;
	int num=0;//���θ�������
	//�ѷ���Ҫ��ĵ��������22*15��22��15��
	for(x=0;x<colNum;x++)
	{
		for(y=0;y<rowNum;y++)
		{
			memset(&m_big[x][y],0,sizeof(CLIENT_POINT));
		}
	}
	for(x=0; x<colNum; x++)//15
	{
		for(y=0; y<rowNum; y++)//22
		{	
			if( m_pHxhtInfo->areaInfo.pData[ (rowNum / 8 + 1) * x + y / 8] & (0x01 << (y%8) ) )//����Ч�ĸ���
			{

				m_big[x][y].enable=true;
				m_big[x][y].recover=false;
			}

		}
	}	
	for(x=0;x<15;x++)
	{
		for(y=0;y<22;y++)
		{
			if((m_big[x][y].enable)&&(y<21)&&(firstcount))//�����ȣ�������ͷ���ҿ��Ա�����
			{
				tmpsqure.width++;
			}
			if(!m_big[x][y].recover)//�ޱ�����
			{
				if((!firstcount)&&(m_big[x][y].enable))//û�е�һ��������Ч
				{
					firstcount=true;
					tmpsqure.x=x;
					tmpsqure.y=y;
					tmpsqure.width=1;
					tmpsqure.height=1;
				}
				else if((m_big[x][y].enable)&&(y==21))//��Ч���������һ������
				{
					//���Ҿ���
					int tmpHeight=0;
					tmpsqure.width++;
					tmpHeight =CheckSqure(tmpsqure.x,tmpsqure.y,y+1);
					if(tmpHeight!=-1)
					{
						tmpsqure.height=tmpHeight;
						firstcount=false;
						tmpsqure.enable=true;
						savesqure[num]=tmpsqure;
						num++;

					}
					memset(&tmpsqure,0,sizeof(CLIENT_AREA));
				}
				else if((m_big[x][y].enable==1)&&(m_big[x][y+1].recover))//��Ч���Ǻ�����һ��������
				{
					int tmpHeight=0;
					tmpHeight =CheckSqure(tmpsqure.x,tmpsqure.y,y+1);
					if(tmpHeight>0)
					{
						tmpsqure.height=tmpHeight;
						firstcount=false;
						tmpsqure.enable=true;
						savesqure[num]=tmpsqure;
						num++;

					}
					memset(&tmpsqure,0,sizeof(CLIENT_AREA));
				}
				else if(!m_big[x][y].enable)//��Ч��2�������1Ϊ���α߽磬2Ϊ�뿪���κ�Զ�ĵط� ����ж�
				{

					if(tmpsqure.width==0)
					{
						//�뿪��Զ�����ù�
					}
					else
					{
						int tmpHeight=0;
						firstcount=false;
						tmpHeight=CheckSqure(tmpsqure.x,tmpsqure.y,y);
						if(tmpHeight!=-1)
						{
							tmpsqure.height=tmpHeight;
							firstcount=false;
							tmpsqure.enable=true;
							savesqure[num]=tmpsqure;
							num++;

						}

						memset(&tmpsqure,0,sizeof(CLIENT_AREA));
					}


				}
			}
		}
	}



}
#endif
///**********************************zxx�¼�***************************************/
//end
