/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-12
** Name         : CfgSystemDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgSystemDlg.h"
#include "LanguagePack.h"
#include "FuncCustom.h"

const char LANG_UTF8_HEBREW_REVERSE[]  = {0xD7,0xAA,0xD7,0x99,0xD7,0xA8,0xD7,0x91,0xD7,0xA2,0x00};
const char LANG_UTF8_PERSIAN_REVERSE[] = {0xD9,0x81,0xD8,0xA7,0xD8,0xB1,0xD8,0xB3,0xDB,0x8C,0x0};
                                       
CCfgSystemDlg::CCfgSystemDlg()
{
	memset(&m_cfgIdPos, 0, sizeof(m_cfgIdPos));
	m_maxPosNum = 0;
}

CCfgSystemDlg::~CCfgSystemDlg()
{

}

void CCfgSystemDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgSystemList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*  pEdit = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 4;
	int maxListRow = height / m_length[LIST_ITEM_HEIGHT]; //
	int numChnn = 10;
	if( numChnn >= maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - offsetEdge );

	////
	char str[256];

	unsigned long listPos = 0;
	SetCfgIDPOs(listPos, NCFG_ITEM_DEVICE_NAME);
	list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_DEV_NAME));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(listPos, 1, WND_TYPE_EDIT));
	assert( pEdit );

	listPos++;
	SetCfgIDPOs(listPos, NCFG_ITEM_DEVICE_ID);
	list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_DEV_ID));
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(list.AddItem(listPos, 1, WND_TYPE_NUM_EDIT));
	assert(pNumEdit);
	pNumEdit->SetRange(0, 65535);
	pNumEdit->SetMaxStringLen( 5 );

	listPos++;
	SetCfgIDPOs(listPos, NCFG_ITEM_VIDEO_FORMAT);
	list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_VIDEO_FORMAT));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(listPos, 1, WND_TYPE_COMBO));
	assert(pCombo);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_NTSC), VIDEO_FORMAT_NTSC);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_PAL), VIDEO_FORMAT_PAL);
	pCombo->SetCurItem( 0, false );
#if defined(__CUSTOM_RU04__) || defined(__TW01_RILI__)
	pCombo->SetEnable(false, false);
#endif

#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)
//2308SS_C，2316SS_C，2308SE_C，2316SE_C需要对编码分辨率做分开处理，不能960H和D1混编
	PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
	bool bHaveVideoMode = false;
	if ((TD_2308SL_C == productID) || (TD_2308SS_C == productID) || (TD_2316SS_C == productID) || 
		(TD_2308SE_C == productID) || (TD_2316SE_C == productID) || (TD_2308ME_C == productID) || 
		(TD_2316ME_C == productID))
	{
		bHaveVideoMode = true;
	}

#if defined(__CUSTOM_US02__)
	if (TD_2516HS_BL == productID)
	{
		bHaveVideoMode = true;
	}
#endif
	if (bHaveVideoMode)
	{
		listPos++;
		SetCfgIDPOs(listPos, NCFG_ITEM_ENCODE_960H);
		list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_960H_MODE));
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(listPos, 1, WND_TYPE_CHECK));
		assert(pCheck);
	}
#endif

	listPos++;
	SetCfgIDPOs(listPos, NCFG_ITEM_AUTHORIZATION);
	list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_PASSWORD_CHECK));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(listPos, 1, WND_TYPE_CHECK));
	assert(pCheck);

	listPos++;
	SetCfgIDPOs(listPos, NCFG_ITEM_LIVE_TIME_INFO);
	list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_SHOW_TIME));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(listPos, 1, WND_TYPE_CHECK));
	assert(pCheck);

#if !defined(__TW01_RILI__)
	listPos++;
	SetCfgIDPOs(listPos, NCFG_ITEM_MAX_CLIENT_NUM);
	list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_MAX_NETWORK_USERS));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(listPos, 1, WND_TYPE_COMBO));
	assert(pCombo);
#endif

	m_netClientNum = s_loginSuccessReply.productInfo.netClientNum;
	assert(m_netClientNum > 0);
#ifdef __SUPPORT_HRSST__
	for (int i = 10;i <= m_netClientNum; i += 10)
	{
		sprintf(str,"%d",i);
		pCombo->AddItem(str,i);
	}	
	pCombo->SetCurItemData(m_netClientNum, false);
#else
	for (int i = 1;i <= m_netClientNum; i++)
	{
		sprintf(str,"%d",i);
		pCombo->AddItem(str,i);
	}	
	pCombo->SetCurItemData(m_netClientNum, false);
#endif

	listPos++;
	SetCfgIDPOs(listPos, NCFG_ITEM_VGA_RESOLUTION);
	list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_VIDEO_OUTPUT));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(listPos, 1, WND_TYPE_COMBO));
	assert(pCombo);
	pCombo->SetMaxStringLen( 16 );
	
	//modify 2010-01-04 产品支持的分辨率由product.def中读取。
	m_displayResolutionMask = s_loginSuccessReply.productInfo.displayResolutionMask;
	assert(m_displayResolutionMask > 0);
	for (int i = 0; i < sizeof(unsigned long)*8; ++i)
	{
		unsigned long tmp = ((unsigned long)1) << i;		
		if (m_displayResolutionMask & tmp)
		{			
			GetVGAResolution(static_cast<VGA_RESOLUTION>(tmp), str);	
#if !defined(__TW01_RILI__) 
			pCombo->AddItem(str, tmp);	
#else
			if(0 != strcmp(str,"HDMI"))
			{
				pCombo->AddItem(str, tmp);
			}
#endif
		}
	}

	pCombo->SetCurItem( 0, false );
	//modify end
	
	//辅助输出,没有辅助输出时CVBS可以复用
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)||defined(__CHIP3515A__)
	if (ReuseCVBSOutput())
	{
		listPos++;
		SetCfgIDPOs(listPos, NCFG_ITEM_CVBS_TO_SPOT);
		list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_SPOT));
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(listPos, 1, WND_TYPE_COMBO));
		assert(pCombo);
		pCombo->SetMaxStringLen(16);
		//以下顺序不能变
		pCombo->AddItem("CVBS", 0);
		pCombo->AddItem("SPOT", 1);
		if (RealCVBSOutput())
		{
			pCombo->AddItem("None", 2);
		}
		pCombo->SetCurItem(0, false);
	}
#endif
	listPos++;
	SetCfgIDPOs(listPos, NCFG_ITEM_LANGUAGE);
	list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_LANGUAGE));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(listPos, 1, WND_TYPE_COMBO));
#if defined(__TW01_RILI__)
	pCombo->SetEnable(false, false);
#endif
	assert(pCombo);

	listPos++;
	SetCfgIDPOs(listPos, NCFG_ITEM_SCRENN_SAVE_TIME);
	list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_SCREENSAVER));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(listPos, 1, WND_TYPE_COMBO));
	assert(pCombo);
	pCombo->AddItem(".5", 30);
	pCombo->AddItem("1", 60);
	pCombo->AddItem("3", 180);
	pCombo->AddItem("5", 300);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_REC_NEVER), 0);
	pCombo->SetCurItemData(0, false);

#ifndef __CUSTOM_US02__ 
	if(IsSupportWzd())
	{
		listPos++;
		SetCfgIDPOs(listPos, NCFG_ITEM_SHOW_WIZZARD);
		list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_SHOW_WZD));
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(listPos, 1, WND_TYPE_CHECK));
		assert(pCheck);
	}
#else
	listPos++;
	SetCfgIDPOs(listPos, NCFG_ITEM_SHOW_WIZZARD);
	list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_SHOW_WZD));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(listPos, 1, WND_TYPE_CHECK));
	assert(pCheck);
#endif
   
#if 0
	list.AddItem(GetRealRow(ROW_TRANSPARENCY),0,m_pStrTable->String(E_STR_ID_TRANSPARENCY));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(GetRealRow(ROW_TRANSPARENCY),1,WND_TYPE_COMBO));
	assert(pCombo);
	pCombo->AddItem("0%",TRANSPARENCY_0);
	pCombo->AddItem("25%",TRANSPARENCY_25);
	pCombo->AddItem("50%",TRANSPARENCY_50);
	pCombo->AddItem("75%",TRANSPARENCY_75);
	pCombo->SetCurItem(0,false);
#endif
	listPos++;
	SetCfgIDPOs(listPos, NCFG_ITEM_CHANN_VISIBLE);
	list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_NOT_DISPLAY));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(listPos, 1, WND_TYPE_CHECK));
	assert(pCheck);

	if(isSupportCVBSVideoOut())
	{
		listPos++;
		SetCfgIDPOs(listPos, NCFG_ITEM_CVBS_VO_ADJUST);
		list.AddItem(listPos, 0, m_pStrTable->String(E_STR_ID_ADJUST_CVBS_VO));
		pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(listPos, 1, WND_TYPE_CHECK));
		assert(pCheck);

	}


	listPos++;
	m_maxPosNum = listPos;

	list.CreateListFocusMap();
}

void CCfgSystemDlg::DoOther()
{
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)
	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;
	CCfgDlg::GetConfig( NCFG_ITEM_CVBS_TO_SPOT, &pTempData, tempDataLen);
	assert(tempDataLen > 0);
	NCFG_CVBS_INFO * pFrameRateData = (reinterpret_cast<NCFG_CVBS_INFO *>(pTempData));
	pFrameRateData->last = pFrameRateData->current;
#endif
}

unsigned long CCfgSystemDlg::OnOK()
{
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)
	NCFG_CVBS_INFO tempCvbs;
	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;
	CCfgDlg::GetConfig( NCFG_ITEM_CVBS_TO_SPOT, &pTempData, tempDataLen);
	assert(tempDataLen > 0);
	if ((NULL != pTempData)&&(tempDataLen>0))
	{
		NCFG_CVBS_INFO * pFrameRateData = (reinterpret_cast<NCFG_CVBS_INFO *>(pTempData));
		tempCvbs.current = pFrameRateData->current;
		tempCvbs.last	= pFrameRateData->last;

		CMessageMan *pMessMan=CMessageMan::Instance();
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		pMessMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_CVBS, (unsigned char *)(&tempCvbs), sizeof(NCFG_CVBS_INFO), msgData);
		pMessMan->ReleaseMsgBuff(msgData);

		/*if ( (2==pFrameRateData->last)&&((0 == pFrameRateData->current)||(1 == pFrameRateData->current)) )
		{
			CMessageMan *pMessMan=CMessageMan::Instance();
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			pMessMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_CVBS, NULL, 0, msgData);
			printf("asdfsadfsda\n");
			pMessMan->ReleaseMsgBuff(msgData);
		}*/
	}
#endif
	return CCfgDlg::OnOK();
}

unsigned long CCfgSystemDlg::PreCheckValue()
{
	if(IsChange(NCFG_ITEM_VIDEO_FORMAT)) 
	{
		return E_STR_ID_TIP_01_37;
	}

	return 0;
}

void CCfgSystemDlg::SetTip()
{
	//设置显示静态tip
	GUI::CListCtrl& list = m_cfgSystemList;
	AddTip(list.GetItemWnd(GetCfgIDPos(NCFG_ITEM_DEVICE_ID),1)->GetID(),E_STR_ID_TIP_01_36);
	AddTip(list.GetItemWnd(GetCfgIDPos(NCFG_ITEM_VIDEO_FORMAT),1)->GetID(),E_STR_ID_TIP_01_37);
	AddTip(list.GetItemWnd(GetCfgIDPos(NCFG_ITEM_LANGUAGE),1)->GetID(), E_STR_ID_TIP_01_38);
	AddTip(list.GetItemWnd(GetCfgIDPos(NCFG_ITEM_VGA_RESOLUTION),1)->GetID(),E_STR_ID_TIP_01_38);
}

//void CCfgSystemDlg::OnChangeFocus(unsigned long oldID, unsigned long newID)
//{
	//设置显示动态tip
	//if( GetShowTipType(TIP_DFOCUS) )
	//{
	//	GUI::CListCtrl& list = m_cfgSystemList;

	//	GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DEVICE_NAME,1));
	//	assert(pEdit);
	//	if( newID == pEdit->GetID() )
	//	{
	//		char strTip[256]={0};
	//		int maxlen = pEdit->GetMaxStringLen();
	//		assert( 256 > (maxlen+strlen(m_pStrTable->String(E_STR_ID_TIP_01_35))+16) );
	//		sprintf(strTip,"%s%d - %s",m_pStrTable->String(E_STR_ID_TIP_01_35),maxlen, pEdit->GetCaption().c_str());
	//		MessageOut(GetTopestWnd(),strTip,TIP_DFOCUS);
	//		return;
	//	}
	//}
//
//	CCfgDlg::OnChangeFocus(oldID,newID);
//}

bool CCfgSystemDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	unsigned long listPos = GetCfgIDPos(cfgID);

	switch (cfgID)
	{
	case NCFG_ITEM_DEVICE_ID:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CWnd * pWnd = m_cfgSystemList.GetItemWnd( listPos, 1);
			GUI::CNumEditCtrl* pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(pWnd);
			assert( NULL != pNumEdit );
			pNumEdit->SetValue( *pValue, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_DEVICE_NAME:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME));

			CCfgDlg::GetConfig( NCFG_ITEM_DEVICE_NAME_MAX_LEN, &pTempData, tempDataLen );
			assert( tempDataLen == sizeof(unsigned long) );
			unsigned long max_name_len = *(reinterpret_cast<unsigned long*>(pTempData));

			const NCFG_INFO_SHORT_NAME* pShortName = reinterpret_cast<const NCFG_INFO_SHORT_NAME*>(pData);
			
			GUI::CWnd * pWnd = m_cfgSystemList.GetItemWnd( listPos, 1);
			GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(pWnd);
			assert(NULL != pEdit );
			pEdit->SetMaxStringLen( max_name_len);
			pEdit->SetCaption( pShortName->name, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_VIDEO_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( m_cfgSystemList.GetItemWnd( listPos, 1) );
			assert(NULL != pCombo );
			pCombo->SetCurItemData( *pValue, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_AUTHORIZATION:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_cfgSystemList.GetItemWnd( listPos, 1 ) );
			assert(NULL != pCheck );
			pCheck->SetCheck( *pValue, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_LANGUAGE:
		{
			CCfgDlg::GetConfig( NCFG_ITEM_SUPPORT_LANGUAGE, &pTempData, tempDataLen );
			if (tempDataLen > 0)
			{
				NCFG_INFO_LANGUAGE *pLanguage = reinterpret_cast<NCFG_INFO_LANGUAGE *>(pTempData);
				unsigned long num = tempDataLen / sizeof(NCFG_INFO_LANGUAGE);
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgSystemList.GetItemWnd(listPos,1));
				assert(pCombo);
				pCombo->RemoveAllItem(bRefresh);
				for(int i = 0; i < num;  ++i)
				{
					unsigned long langID = pLanguage[i].languageId;

					if((LANGUAGE_HEBREW == langID) && (!g_langRightOrder))
					{
						//客户端需要正常顺序,因此在此处替换
						pCombo->AddItem(LANG_UTF8_HEBREW_REVERSE, (LONGLONG)langID);
					}
					else if ((LANGUAGE_PERSIAN == langID) && (!g_langRightOrder))
					{
						//客户端需要正常顺序,因此在此处替换
						pCombo->AddItem(LANG_UTF8_PERSIAN_REVERSE, (LONGLONG)langID);
					}
					else						
					{
						pCombo->AddItem(pLanguage[i].name, (LONGLONG)langID);
					}
				}
				
				assert(len == sizeof(unsigned long));
				const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
				pCombo->SetCurItemData(*pValue, bRefresh );
			}			
		}
		return true;
		break;
	case NCFG_ITEM_LIVE_TIME_INFO:
		{
			assert(len == sizeof(OSD_INFO));
			const OSD_INFO* pValue = reinterpret_cast<const OSD_INFO*>(pData);

			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_cfgSystemList.GetItemWnd( listPos, 1 ) );
			assert(NULL != pCheck );		
			pCheck->SetCheck( pValue->bEnable, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_SCRENN_SAVE_TIME:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
			assert(NULL != pCombo);
			pCombo->SetCurItemData(*pValue, bRefresh);
		}
		return true;
		break;
	case NCFG_ITEM_MAX_CLIENT_NUM:
		{
#if !defined(__TW01_RILI__)
			assert(len==sizeof(unsigned long));
			unsigned long max = *reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgSystemList.GetItemWnd(listPos,1));
			assert(pCombo);
			pCombo->SetCurItemData(max,bRefresh);
#endif
		}
		return true;
		break;
	case NCFG_ITEM_VGA_RESOLUTION:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pLong = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( m_cfgSystemList.GetItemWnd( listPos, 1 ) );
			assert(NULL != pCombo );
			pCombo->SetCurItemData( *pLong, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_CVBS_TO_SPOT:
		{
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)||defined(__CHIP3515A__)
			//辅助输出,没有辅助输出时CVBS可以复用
			if (ReuseCVBSOutput())
			{
				const NCFG_CVBS_INFO* pLong = reinterpret_cast<const NCFG_CVBS_INFO*>(pData);
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
				assert(NULL != pCombo );
				pCombo->SetCurItemData(pLong->current, bRefresh);
			}
#endif
		}
		return true;
		break;
	case NCFG_ITEM_ENCODE_960H:
		{
#if defined(__CHIP3520A__) || defined(__CHIP3521__) || defined(__CHIP3520D__)

			//2308SS_C，2316SS_C，2308SE_C，2316SE_C需要对编码分辨率做分开处理，不能960H和D1混编
			PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
			bool bHaveVideoMode = false;
			if ((TD_2308SL_C == productID) || (TD_2308SS_C == productID) || (TD_2316SS_C == productID) || 
				(TD_2308SE_C == productID) || (TD_2316SE_C == productID) || (TD_2308ME_C == productID) || 
				(TD_2316ME_C == productID))
			{
				bHaveVideoMode = true;
			}

#if defined(__CUSTOM_US02__)
			if (TD_2516HS_BL == productID)
			{
				bHaveVideoMode = true;
			}
#endif
			if (bHaveVideoMode)
			{
				assert(len == sizeof(unsigned long));
				unsigned long value = *reinterpret_cast<const unsigned long*>(pData);
				GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
				assert(pCheck);
				if(0 != value)
				{
					pCheck->SetCheck(true, bRefresh);
				}
				else
				{
					pCheck->SetCheck(false, bRefresh);
				}
			}
#endif
		}
		return true;
		break;
#if 0
	case NCFG_ITEM_TRANSPARENCY:
		{
			assert(len==sizeof(unsigned long));
			unsigned long trans = *reinterpret_cast<const unsigned long*>(pData);
			
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgSystemList.GetItemWnd(listPos,1));
			assert(pCombo);
			pCombo->SetCurItemData(trans,bRefresh);
		}
		return true;
		break;
#endif
	case NCFG_ITEM_SHOW_WIZZARD:
		{
			assert(len == sizeof(unsigned long));
			unsigned long value = *reinterpret_cast<const unsigned long*>(pData);
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
			assert(pCheck);
			if(0 != value)
			{
				pCheck->SetCheck(true, bRefresh);
			}
			else
			{
				pCheck->SetCheck(false, bRefresh);
			}
		}
		return true;
		break;
	case NCFG_ITEM_CHANN_VISIBLE:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_cfgSystemList.GetItemWnd( listPos, 1 ) );
			assert(NULL != pCheck );
			pCheck->SetCheck( *pValue, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_CVBS_VO_ADJUST:
		{
			if(isSupportCVBSVideoOut())
			{
				assert(len == sizeof(unsigned long));
				const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

				GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_cfgSystemList.GetItemWnd(listPos, 1 ) );
				assert(NULL != pCheck );
				pCheck->SetCheck( *pValue, bRefresh );
			}
		}
		return true;
		break;
	default:
		return false;
		break;
	}
}

bool CCfgSystemDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);

	unsigned long listPos = GetCfgIDPos(cfgID);

	switch (cfgID)
	{
	case NCFG_ITEM_DEVICE_ID:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CNumEditCtrl* pNumEdit = dynamic_cast<const GUI::CNumEditCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
			assert( NULL != pNumEdit );
			*pValue = static_cast<unsigned long>(pNumEdit->GetValue());
		}
		return true;
		break;
	case NCFG_ITEM_DEVICE_NAME:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME));
			NCFG_INFO_SHORT_NAME* pShortName = reinterpret_cast<NCFG_INFO_SHORT_NAME*>(pData);

			const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
			assert(NULL != pEdit );
			strcpy(pShortName->name, pEdit->GetCaption().c_str());			
		}
		return true;
		break;
	case NCFG_ITEM_VIDEO_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			 unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
			assert(NULL != pCombo );
			*pValue = static_cast<unsigned long>(pCombo->GetCurItemData());
		}
		return true;
		break;
	case NCFG_ITEM_AUTHORIZATION:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1 ));
			assert(NULL != pCheck );

			if (pCheck->IsCheck())
			{
				*pValue = 1;
			}
			else
			{
				*pValue = 0;
			}
		}
		return true;
		break;
	case NCFG_ITEM_LANGUAGE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
			assert( NULL != pCombo );
			*pValue = static_cast<unsigned long>(pCombo->GetCurItemData());
		}
		return true;
		break;
	case NCFG_ITEM_LIVE_TIME_INFO:
		{
			assert(len == sizeof(OSD_INFO));
			OSD_INFO* pValue = reinterpret_cast<OSD_INFO*>(pData);

			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
			assert(NULL != pCheck );

			if (pCheck->IsCheck())
			{
				pValue->bEnable = 1;
			} 
			else
			{
				pValue->bEnable = 0;
			}
		}
		return true;
		break;
	case NCFG_ITEM_SCRENN_SAVE_TIME:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
			assert(pCombo);

			*pValue = static_cast<unsigned long>(pCombo->GetCurItemData());
		}
		return true;
		break;
	case NCFG_ITEM_MAX_CLIENT_NUM:
		{
#if !defined(__TW01_RILI__)
			assert(len==sizeof(unsigned long));
			unsigned long* pLong = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_cfgSystemList.GetItemWnd(listPos,1));
			assert(pCombo);
			*pLong = static_cast<unsigned long>(pCombo->GetCurItemData());
#endif
		}
		return true;
		break;
	case NCFG_ITEM_VGA_RESOLUTION:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pLong = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>( m_cfgSystemList.GetItemWnd(listPos, 1));
			assert(NULL != pCombo );
			*pLong = static_cast<unsigned long>(pCombo->GetCurItemData());
		}
		return true;
		break;
	case NCFG_ITEM_CVBS_TO_SPOT:
		{
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)||defined(__CHIP3515A__)
			//辅助输出,没有辅助输出时CVBS可以复用
			if (ReuseCVBSOutput())
			{
				assert(len == sizeof(NCFG_CVBS_INFO));
				NCFG_CVBS_INFO* pLong = reinterpret_cast<NCFG_CVBS_INFO*>(pData);

				const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
				assert(NULL != pCombo );
				if (pLong->last != pLong->current)
				{
					pLong->last = pLong->current;
				}
				pLong->current = static_cast<unsigned long>(pCombo->GetCurItemData());
			}
#endif
		}
		return true;
		break;
	case NCFG_ITEM_ENCODE_960H:
		{
#if defined(__CHIP3520A__) || defined(__CHIP3521__) || defined(__CHIP3520D__)
			//2308SS_C，2316SS_C，2308SE_C，2316SE_C需要对编码分辨率做分开处理，不能960H和D1混编
			PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
			//printf("%s:%s:%d, pproduct id=%d\n", __FUNCTION__, __FILE__, __LINE__, productID);

			bool bHaveVideoMode = false;
			if ((TD_2308SL_C == productID) || (TD_2308SS_C == productID) || (TD_2316SS_C == productID) || 
				(TD_2308SE_C == productID) || (TD_2316SE_C == productID) || (TD_2308ME_C == productID) || 
				(TD_2316ME_C == productID))
			{
				bHaveVideoMode = true;
			}

#if defined(__CUSTOM_US02__)
			if (TD_2516HS_BL == productID)
			{
				bHaveVideoMode = true;
			}
#endif
			if (bHaveVideoMode)
			{
				assert(len == sizeof(unsigned long));
				unsigned long* puLong = reinterpret_cast<unsigned long*>(pData);

				const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>(m_cfgSystemList.GetItemWnd(listPos,1));
				assert(pCheck);
				if(pCheck->IsCheck())
				{
					*puLong = 1;
				}
				else
				{
					*puLong = 0;
				}
			}
#endif
		}
		return true;
		break;
#if 0
	case NCFG_ITEM_TRANSPARENCY:
		{
			assert(len==sizeof(unsigned long));
			unsigned long* pLong = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_cfgSystemList.GetItemWnd(GetRealRow(ROW_TRANSPARENCY),1));
			assert(pCombo);
			*pLong = static_cast<unsigned long>(pCombo->GetCurItemData());
		}
		return true;
		break;
#endif
	case NCFG_ITEM_SHOW_WIZZARD:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* puLong = reinterpret_cast<unsigned long*>(pData);

			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>(m_cfgSystemList.GetItemWnd(listPos,1));
			assert(pCheck);
			if(pCheck->IsCheck())
			{
				*puLong = 1;
			}
			else
			{
				*puLong = 0;
			}
		}
		return true;
		break;
	case NCFG_ITEM_CHANN_VISIBLE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
			assert(pCheck );
			if (pCheck->IsCheck())
			{
				*pValue = 1;
			}
			else
			{
				*pValue = 0;
			}
		}
		return true;
		break;
	case NCFG_ITEM_CVBS_VO_ADJUST:
		{
			if(isSupportCVBSVideoOut())
			{
				assert(len == sizeof(unsigned long));
				unsigned long * pValue = reinterpret_cast<unsigned long*>(pData);
				const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>(m_cfgSystemList.GetItemWnd(listPos, 1));
				assert(NULL != pCheck );
				if (pCheck->IsCheck())
				{
					* pValue = 1;
				}
				else
				{
					* pValue = 0;
				}

			}
			
		}
		return true;
		break;
	default:
		return false;
		break;
	}
}

BEGIN_MSG_MAP(CCfgSystemDlg, CCfgDlg)
	ON_MSG_EX(m_cfgSystemList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgSystemDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (!IsModify())
	{
		unsigned short row = (keyID >> 16)&0x7fff;	//行
		unsigned short col = keyID&0xffff;			//列
		
		if ((0 <= row) && (row < m_maxPosNum))
		{
			if (IsChange(m_cfgIdPos[row].cfgItemID))
			{
				SetModify(true);
			}
		}

		return KEY_VIRTUAL_MSG_CONTINUE;
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	
}


bool CCfgSystemDlg::HaveSpot()const
{
#if defined(__CHIP3520A__) || defined(__CHIP3520D__)  || defined(__CHIP3521__)
	PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
	if ((TD_2304SS_B == productType) || (TD_2308SS_B == productType) || (TD_2316SS_B == productType) || (TD_2508HS_BL == productType) || (TD_2516HS_BL == productType) || 
		(TD_2304SS_C == productType) || (TD_2308SS_C == productType) || (TD_2316SS_C == productType) ||(TD_2308SS_SL == productType)||(TD_2304SS_SL_3515A == productType) ||
		(TD_2304SL_C == productType) || (TD_2308SL_C == productType) )
	{
		return false;
	}
	else
#endif
	{
		return CProduct::Instance()->HaveSpot();
	}
}

bool CCfgSystemDlg::ReuseCVBSOutput()const
{
	PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
	if ((TD_2304SS_B == productType) || (TD_2308SS_B == productType) || (TD_2316SS_B == productType) || (TD_2508HS_BL == productType) || (TD_2516HS_BL == productType) || 
		(TD_2304SS_C == productType) || (TD_2308SS_C == productType) || (TD_2316SS_C == productType) ||(TD_2308SS_SL == productType)||(TD_2304SS_SL_3515A == productType)||
		(TD_2304SL_C == productType) || (TD_2308SL_C == productType) || (TD_2708AS_S_A == productType) || (TD_2704AS_SL_A == productType))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CCfgSystemDlg::RealCVBSOutput() const
{
	PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
	if ( (TD_2704AS_SL_A == productType) || (TD_2708AS_S_A == productType) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CCfgSystemDlg::SetCfgIDPOs(unsigned long listPos, const NCFG_ITEM_ID cfgItemID)
{
	m_cfgIdPos[listPos].cfgItemID = cfgItemID;
}

unsigned long CCfgSystemDlg::GetCfgIDPos(NCFG_ITEM_ID cfgItemID) const
{
	if (NCFG_ITEM_ENCODE_960H == cfgItemID)
	{
		printf("%s:%s:%d, running at here\n", __FUNCTION__, __FILE__, __LINE__);
	}

	for (unsigned long pos=0; pos<m_maxPosNum; pos++)
	{
		if (cfgItemID == m_cfgIdPos[pos].cfgItemID)
		{
			return pos;
		}
	}

	//assert(false);

	return 0;
}

bool CCfgSystemDlg::isSupportCVBSVideoOut() const
{
#if (defined(__CHIP3531__) || defined(__CHIPGM__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3515A__)) && !defined(__DVR_ULTIMATE__)
	PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
	switch(productType)
	{
		case TD_2704XS_SL:
		case TD_2708XS_SL:
		case TD_2716XS_SL:
			{
				return false;
				break;
			}
		case TD_2704AS_SL:
		case TD_2708AS_SL:
		case TD_2716AS_SL:
		//case TD_2704AS_SL_A:
		case TD_2708AS_SL_A:
		case TD_2716AS_SL_A:
			{
				return false;
				break;
			}
		case TD_2704TS_S:
		case TD_2708TS_S:
		case TD_2716TS_S:
			{
				return false;
				break;
			}
		case TD_2704TS_PL:
		case TD_2708TS_PL:
		case TD_2716TS_PL:
			{
				return false;
				break;
			}
		case TD_2704TD_PL:
		case TD_2708TD_PL:
		case TD_2716TD_PL:
		case TD_2732TD_PL:
			{
				return false;
				break;
			}
		case TD_2704TD_P:
		case TD_2708TD_P:
		case TD_2716TD_P:
			{
				return false;
				break;
			}
		case TD_2704AS_S:
		case TD_2708AS_S:
		case TD_2716AS_S:
		case TD_2704AS_S_A:
		//case TD_2708AS_S_A:
		case TD_2716AS_S_A:
			{
				return false;
				break;
			}
		case TD_2704AE_SL:
		case TD_2708AE_SL:
		case TD_2716AE_SL:
			{
				return false;
				break;
			}
		case TD_2704TE_M:
		case TD_2708TE_M:
		case TD_2716TE_M:
			{
				return false;
				break;
			}
		case TD_2004AL:
			{
				return false;
				break;
			}
		case TD_2008AS:
			{
				return false;
				break;
			}
		case TD_2704AS_PL:
		case TD_2708AS_PL:
		case TD_2716AS_PL:
			{
				return false;
				break;
			}
		case TD_2704AC_PL:
		case TD_2708AC_PL:
		case TD_2716AC_PL:
			{
				return false;
				break;
			}
		case TD_2704AE_PL:
		case TD_2708AE_PL:
		case TD_2716AE_PL:
			{
				return false;
				break;
			}
		default:
			{
				return true;
				break;
			}
	}
#else
	return false;
#endif

}
