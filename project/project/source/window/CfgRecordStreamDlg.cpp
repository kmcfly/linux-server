/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-12
** Name         : CfgRecordStreamDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgRecordStreamDlg.h"
#include "FuncCustom.h"
#include "LocalDevice.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
extern const unsigned long g_resolutionNum;
extern unsigned long VideoSizeToDDVideoSize(unsigned long videoSize);
CCfgRecordStreamDlg::CCfgRecordStreamDlg()
{	
	m_pChnnRecp = NULL;
	m_pVSBit = NULL;

	memset(m_supportEncInfo, 0, sizeof(SUPPORT_ENC_INFO));
	memset(m_checkEncInfo, 0, sizeof(CHECK_ENC_INFO));
	m_bModified = false;
}

CCfgRecordStreamDlg::~CCfgRecordStreamDlg()
{

}

void CCfgRecordStreamDlg::OnInitial()
{
	CCfgDlg::OnInitial();

	//////////////////////////////////////////////////////////////////////////
	m_netDevNum = 0;
	m_pNetDevInfo = NULL;
	GUI::CListCtrl& list = m_cfgListCtrl;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CStatic *pStatic = NULL;
	
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[LIST_ITEM_HEIGHT] - m_length[DLG_OFFSET_SUB_BOTTOM];

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;
	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);
	maxChnn = localVideoInputNum + netVideoInputNum;

	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头
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

	m_channelNum = maxChnn;

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
#ifdef __DVR_PROFESSIONAL__
	#ifdef __CUSTOM_US02_NEW__
		list.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 6 / 32 -10 );
		list.AddColumn(m_pStrTable->String(E_STR_ID_FPS_01), width * 6/ 32+10);
	#else
		list.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 5 / 32 -10 );
		list.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 5 / 32+10);
	#endif
	list.AddColumn(m_pStrTable->String(E_STR_ID_REC_ENCODE_TYPE), width * 6 / 32 - 8);
	list.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 5 / 32 + 8 );
	#ifdef __CUSTOM_US02_NEW__
		list.AddColumn(m_pStrTable->String(E_STR_ID_REC_MAX_BIT_STREAM), width * 6 / 32 - offsetEdge);
	#else
		list.AddColumn(m_pStrTable->String(E_STR_ID_REC_MAX_BIT_STREAM), width * 8 / 32 - offsetEdge);
	#endif
#else
	list.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 9 / 32);
	#ifdef __CUSTOM_US02_NEW__
		list.AddColumn(m_pStrTable->String(E_STR_ID_FPS_01), width * 9 / 32));
	#else
		list.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 9 / 32);
	#endif
	list.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 11 / 32 - offsetEdge);
#endif


	char str[128] = {0};

	for ( int i = 0; i < effectChnn; i++ )
	{
		int index = m_rowChnnMap.find(i)->second;
		pStatic = dynamic_cast<GUI::CStatic*>(list.AddItem( i, COL_CHNN, WND_TYPE_STATIC));

		sprintf(str, "%d", index + 1);
		pStatic->SetCaption(str, false);

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_RATE, WND_TYPE_COMBO));
		assert(pCombo);		

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_RESOLUTION, WND_TYPE_COMBO));
		assert(pCombo);
#if defined(__CHIPGM__)
		if (TD_2708TS_PL == CProduct::Instance()->ProductType())
		{
			pCombo->SetEnable(false, false);
		}
#endif
		
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_QUALITY, WND_TYPE_COMBO));
		assert(pCombo);

#ifdef __DVR_PROFESSIONAL__		
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(i,COL_STREAM_TYPE,WND_TYPE_COMBO));
		assert(pCombo);
#if defined(__CHIPGM__)
		if (TD_2704AS_SL == CProduct::Instance()->ProductType())
		{
			pCombo->SetEnable(false, false);
		}
#endif
		
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(i,COL_MAX_BIT_STREAM,WND_TYPE_COMBO));
		assert(pCombo);
#endif
	}
	list.CreateListFocusMap();

	GUI::CRect rect = list.GetWindowRect();
	y += rect.Height() + 4;

	unsigned short cx = 0, cy = 0;

#ifdef __DVR_ULTIMATE__
	x += 120;
#endif

	if (localVideoInputNum > 0)
	{
		snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_ALL ) );
		TextSize(str, strlen(str), cx, cy);

		if(!g_langRightOrder)
		{
			m_cfgScAll.Create( GUI::CWnd::GetRandWndID(), x, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, 100, m_length[STATIC_HEIGHT], this );
		}
		else
		{
			m_cfgScAll.Create( GUI::CWnd::GetRandWndID(),  m_rect.Width() - x - cx , y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, cx, m_length[STATIC_HEIGHT], this );
		}
		m_cfgScAll.SetCaption( str, false );

		x += 100;
		snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_REMAINING ) );
		TextSize(str, strlen(str), cx, cy);

		if(!g_langRightOrder)
		{
			m_cfgScRemaining.Create( GUI::CWnd::GetRandWndID(), x, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, 100, m_length[STATIC_HEIGHT], this );
		}
		else
		{
			m_cfgScRemaining.Create( GUI::CWnd::GetRandWndID(), m_rect.Width() -  x - cx, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, cx, m_length[STATIC_HEIGHT], this );
		}
			//strcpy( str, "Remaining");
		m_cfgScRemaining.SetCaption(str, false );

		x += cx;
		PRODUCT_TYPE productTypeTemp = CProduct::Instance()->ProductType();
		if ((TD_2324SE_S == productTypeTemp) || (TD_2316SE_S == productTypeTemp) || (TD_2316CDS == productTypeTemp))
		{
			strcpy(str, " : 888 (CIF).");
		}
		else
		{
	#if defined(__CUSTOM_US02__)
			strcpy(str, " : 888 (CIF), 888 (D1).");
	#else
			strcpy(str, " : 888 (CIF), 888 (HD1), 888 (D1).");
	#endif
		}
		TextSize(str, strlen(str), cx, cy);
		if(!g_langRightOrder)
		{
			m_cfgScRemainNum.Create( GUI::CWnd::GetRandWndID(), x, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, width / 2, m_length[STATIC_HEIGHT], this );

		}
		else
		{
			m_cfgScRemainNum.Create( GUI::CWnd::GetRandWndID(), m_rect.Width() -x - cx, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, cx, m_length[STATIC_HEIGHT], this );

		}
		if ((TD_2324SE_S == productTypeTemp) || (TD_2316SE_S == productTypeTemp) || (TD_2316CDS == productTypeTemp))
		{
			m_cfgScRemainNum.SetCaption( " : 0 (CIF).", false );
		}
		else
		{
#if defined(__CUSTOM_US02__)
			m_cfgScRemainNum.SetCaption( " : 0 (CIF), 0 (D1).", false );
#else
			m_cfgScRemainNum.SetCaption( " : 0 (CIF), 0 (HD1), 0 (D1).", false );
#endif
		}
		m_cfgScRemainNum.SetBKColor(COLOR_DLG_CHILD_BK);

		x = m_length[DLG_OFFSET_SUB_LEFT];
		y += m_length[LIST_ITEM_HEIGHT];
		GUI::CListCtrl& listAll = m_cfgListCtrlAll;
		listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 1, false );
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
#ifdef __DVR_PROFESSIONAL__
		#ifdef __CUSTOM_US02_NEW__
			listAll.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 6 / 32 -10 );
			listAll.AddColumn(m_pStrTable->String(E_STR_ID_FPS_01), width * 6 / 32+10);
		#else
			listAll.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 5 / 32 -10 );			
			listAll.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 5 / 32+10);
		#endif
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_REC_ENCODE_TYPE), width * 6 / 32 - 8);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 5 / 32 + 8 );
		#ifdef __CUSTOM_US02_NEW__
			listAll.AddColumn(m_pStrTable->String(E_STR_ID_REC_MAX_BIT_STREAM), width * 6 / 32 - offsetEdge);
		#else
			listAll.AddColumn(m_pStrTable->String(E_STR_ID_REC_MAX_BIT_STREAM), width * 8 / 32 - offsetEdge);			
		#endif
#else
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 9 / 32 );
		#ifdef __CUSTOM_US02_NEW__
			listAll.AddColumn(m_pStrTable->String(E_STR_ID_FPS_01), width * 9 / 32));
		#else
			listAll.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 9 / 32);
		#endif
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 11 / 32 - offsetEdge);
#endif
		{
			int i = 0;

			pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem(i,COL_CHNN,WND_TYPE_CHECK));
			assert(pCheck);

			PRODUCT_TYPE productID = CProduct::Instance()->ProductType();

			if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
				|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
				|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
				|| (TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2708AS_S_A == productID) || (TD_2716AS_S == productID)
				|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) ||(TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
				|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
				|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
				|| (TD_2004AL == productID) || (TD_2008AS == productID)
				|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
				|| (TD_2716AC_PL == productID)
				|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
			{
				pCheck->Show(false,false);
			}		


			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem( i, COL_RESOLUTION, WND_TYPE_COMBO));
			assert(pCombo);
			if (!IsSupportRecordVideoSize())
			{
				pCombo->SetEnable(false, false);
			}

			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem( i, COL_RATE, WND_TYPE_COMBO));
			assert(pCombo);
			if (!IsSupportRecordResChange())
			{
				pCombo->SetEnable(false, false);
			}

#ifdef __DVR_PROFESSIONAL__	
			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem(i,COL_STREAM_TYPE,WND_TYPE_COMBO));
			assert(pCombo);
#endif
			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem( i, COL_QUALITY, WND_TYPE_COMBO));
			assert(pCombo);
			if (!IsSupportRecordResChange())
			{
				pCombo->SetEnable(false, false);
			}
		
	#ifdef __DVR_PROFESSIONAL__
			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem(i,COL_MAX_BIT_STREAM,WND_TYPE_COMBO));
			assert(pCombo);		
	#endif
		}
		listAll.CreateListFocusMap();

		PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
// 		if (IsMixDvr() || (TD_2704AS_S == productID) ||(TD_2708AS_S == productID) || (TD_2716AS_SL == productID)
// 			|| (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
// 			|| (TD_2304SS_C == productID) || (TD_2308SS_C == productID) || (TD_2316SS_C == productID)
// 			|| (TD_2304SE_C == productID) || (TD_2308SE_C == productID) || (TD_2316SE_C == productID)
// 			|| (TD_2508HE_C_3521 == productID) || (TD_2616LE == productID))
		if (IsMixDvr() || 0 < g_p_login_succ->productInfo.netVideoInputNum)
		{
			listAll.Show(false,false);
		}

#if defined(__CHIPGM__)
		if (TD_2704AS_SL == CProduct::Instance()->ProductType())
		{
			listAll.Show(false,false);
		}
#endif
	}
	m_pQualityLevelValue = NULL;
	m_pBitStreamValue = NULL;
	//加入2个控件，时间和事件单选框
	
	if (!IsSupportRecordResChange())
	{
		for (int i = 0; i < effectChnn; i++)
		{
			pCombo = NULL;
			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(i, COL_RESOLUTION));
			pCombo->SetEnable(false, false);
			
			assert(pCombo);
			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(i, COL_RATE));
			pCombo->SetEnable(false, false);
			
#ifdef __DVR_PROFESSIONAL__
			assert(pCombo);
			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(i, COL_STREAM_TYPE));
#endif

			assert(pCombo);
			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(i, COL_QUALITY));
			pCombo->SetEnable(false, false);

#ifdef __DVR_PROFESSIONAL__
			assert(pCombo);
			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(i, COL_MAX_BIT_STREAM));
#endif
		}

		m_cfgScRemaining.Show(false, false);
		m_cfgScRemainNum.Show(false, false);
	}

	if (!IsSupportRecordVideoSize())
	{
		for (int i = 0; i < effectChnn; i++)
		{
			pCombo = NULL;
			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(i, COL_RESOLUTION));
			pCombo->SetEnable(false, false);
		}
	}


	PRODUCT_TYPE productID = CProduct::Instance()->ProductType();

	if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
		|| (TD_2304SS_C == productID) || (TD_2308SS_C == productID) || (TD_2316SS_C == productID) || (TD_2616LE == productID) 
		|| (TD_2508HE_B_3521 == productID) || (TD_2524HE_C == productID) || (TD_2532HE_C == productID)
		|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
		|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
		|| (TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2708AS_S_A == productID) || (TD_2716AS_S == productID)
		|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
		|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
		|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
		|| (TD_2004AL == productID) || (TD_2008AS == productID)
		|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
		|| (TD_2716AC_PL == productID)
		|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
	{
		m_cfgListCtrlAll.Show(false,false);
	}
	

	PRODUCT_TYPE product = CProduct::Instance()->ProductType();
	unsigned long localVideoSize = CProduct::Instance()->LocalVideoSizeMask();
	bool bHideRemain = false;
	if ((localVideoSize & DVRVIDEO_SIZE_960H) || (localVideoSize & DVRVIDEO_SIZE_HD720) || (localVideoSize & DVRVIDEO_SIZE_HD1080))
	{
		bHideRemain = true;
	}
	if (bHideRemain || (product == TD_2604LD_S)||(product == TD_2608LD_S)||(product == TD_2616LD_S)
	  ||(product == TD_2604LD)||(product == TD_2608LD)||(product == TD_2616LD )
	  ||(product == TD_2504HD_C) || (product == TD_2508HD_C)|| (product == TD_2516HD_C) ||(product == TD_2524HD_C) ||(product == TD_2532HD_C)
	  ||(product == TD_2704HD)
	  ||(product == TD_2704XE_L) || (product == TD_2708XE_L) || (product == TD_2716XE_L)
	  ||(product == TD_2704XD_L) || (product == TD_2708XD_L) || (product == TD_2716XD_L)
	  ||(product == TD_2704XD_M) || (product == TD_2708XD_M) || (product == TD_2716XD_M)
	  ||(product == TD_2704XD_P) || (product == TD_2708XD_P) || (product == TD_2716XD_P)
	  ||(product == TD_2708XE_MH)|| (product == TD_2304ME_C)
	  ||(product == TD_2708XE_P))
	{
		m_cfgScRemaining.Show(false, false);
		m_cfgScRemainNum.Show(false, false);
		m_cfgScAll.Show(false,false);
	}
}

void CCfgRecordStreamDlg::OnPostInitial()
{
	CCfgDlg::OnPostInitial();

	CProduct * pProduct = CProduct::Instance();
	if(s_loginSuccessReply.productInfo.netVideoInputNum > 0)
	{
		m_ipcSupportEncode.dwResolution = pProduct->GetDigitChannelSupportVideoSize()[s_loginSuccessReply.productInfo.localVideoInputNum];
		m_ipcSupportEncode.maxChnnRate = pProduct->IPCMaxChnnRate();
		m_ipcSupportEncode.maxTotalRateOnVGA = pProduct->IPCMaxTotalRateOnVGA();
		m_ipcSupportEncode.maxChnnRateOnVGA = pProduct->IPCMaxChnnRateOnVGA();
		m_ipcSupportEncode.maxTotalBitRate = pProduct->GetDigitMaxTotalBitRate();
		m_ipcSupportEncode.defaBitRate = pProduct->IPCDefaBitRateValue();
		m_ipcSupportEncode.defaChnnRate = pProduct->IPCDefaChnnRate();
		m_ipcSupportEncode.defaResolution = pProduct->IPCDefVideoSize();
		m_ipcSupportEncode.bLimitByBitRate = pProduct->IPCIsLimitByBitRate();
		
		if(VIDEO_FORMAT_NTSC == s_loginSuccessReply.productInfo.videoFormat)
		{
#if	!defined(__CHIP3520__)
			m_ipcSupportEncode.defaChnnRate = (m_ipcSupportEncode.defaChnnRate + 1) * 6 / 5;
			if (30 < m_ipcSupportEncode.defaChnnRate)
			{
				m_ipcSupportEncode.defaChnnRate = 30;
			}
			m_ipcSupportEncode.maxChnnRate = (m_ipcSupportEncode.maxChnnRate + 1) * 6 / 5;
			if (30 < m_ipcSupportEncode.maxChnnRate)
			{
				m_ipcSupportEncode.maxChnnRate = 30;
			}
			m_ipcSupportEncode.maxTotalRateOnVGA = (m_ipcSupportEncode.maxTotalRateOnVGA + 1) * 6 / 5;
			m_ipcSupportEncode.maxChnnRateOnVGA =  (m_ipcSupportEncode.maxChnnRateOnVGA + 1) * 6 / 5;
#endif
		}

		//CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		memset(&m_netChennelEncInfo, 0, sizeof(NET_CHANNEL_ENC_INFO) * s_loginSuccessReply.productInfo.netVideoInputNum);

		if(s_loginSuccessReply.productInfo.netVideoInputNum > 0)
		{
			MESSAGE_DATA msgData;
			unsigned long streamType = 0;
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NET_CHANNEL_SUPPORT_VIDEO_ENCODE_INFO, (unsigned char *)&streamType, sizeof(unsigned long), msgData);
			PACKCMD * pCMD = (PACKCMD *)msgData.pData;
			if (0 < pCMD->dataLen)
			{
				memcpy(&m_netChennelEncInfo, msgData.pData + sizeof(PACKCMD), sizeof(NET_CHANNEL_ENC_INFO) * s_loginSuccessReply.productInfo.netVideoInputNum);
			}

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		}
		else
		{

		}
	}
	else
	{
		memset(&m_ipcSupportEncode, 0, sizeof(NCFG_IPC_SUPPORT_ENCODE));
	}

	if((0 < s_loginSuccessReply.productInfo.localVideoInputNum) && IsSupportFunction(FUNC_SERVER_ENCODE_CHECK))
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));

		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_FIRST_SUPPORT_INFO, NULL, 0, msgData);

		assert(msgData.dataLen >= sizeof(SUPPORT_ENC_INFO) * s_loginSuccessReply.productInfo.localVideoInputNum);

		memcpy(m_supportEncInfo, msgData.pData + sizeof(PACKCMD), msgData.dataLen);

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}
}

void CCfgRecordStreamDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	m_rowChnnMap.clear();
	if(NULL != m_pChnnRecp)
	{
		delete []m_pChnnRecp;
		m_pChnnRecp = NULL;
	}
	if(NULL != m_pVSBit)
	{
		delete []m_pVSBit;
		m_pVSBit = NULL;
	}
}

void CCfgRecordStreamDlg::SetTip()
{
	if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		AddTip(m_cfgListCtrlAll.GetItemWnd(0,COL_CHNN)->GetID(), E_STR_ID_TIP_01_39);
		#ifdef __CUSTOM_US02_NEW__

			NCFG_ITEM_HEAD itemHead;
			memset(&itemHead, 0, sizeof(NCFG_ITEM_HEAD));
			itemHead.itemID = NCFG_ITEM_VIDEO_FORMAT;
			unsigned long dataLen = 0;
			unsigned char *pData = NULL;
		
			if(CConfigEx::Instance()->GetConfig((unsigned char *)&itemHead, sizeof(itemHead), &pData, dataLen))
			{
				assert(pData != NULL);
				assert(dataLen > 0);
		
				unsigned long *pVideoFormat = reinterpret_cast<unsigned long *>( pData + sizeof(NCFG_ITEM_HEAD) + sizeof(NCFG_BLOCK_HEAD));
				if (*pVideoFormat == 0)
				{
					for(int i=0; i<s_loginSuccessReply.productInfo.localVideoInputNum; i++)
					{
						AddTip(m_cfgListCtrl.GetItemWnd(i, COL_RESOLUTION)->GetID(), E_STR_ID_RESOLUTION_01);
					}
					
					AddTip(m_cfgListCtrlAll.GetItemWnd(0, 1)->GetID(), E_STR_ID_RESOLUTION_01);
		
				}
				else
				{
					for(int i=0; i<s_loginSuccessReply.productInfo.localVideoInputNum; i++)
					{
						AddTip(m_cfgListCtrl.GetItemWnd(i, COL_RESOLUTION)->GetID(), E_STR_ID_RESOLUTION_01);
					}
					AddTip(m_cfgListCtrlAll.GetItemWnd(0, 1)->GetID(), E_STR_ID_RESOLUTION_01);
		
				}
			}

			for(int i=0; i<s_loginSuccessReply.productInfo.localVideoInputNum; i++)
			{
				AddTip(m_cfgListCtrl.GetItemWnd(i, 3)->GetID(), E_STR_ID_REC_ENCODE_TYPE_01);
			}
			AddTip(m_cfgListCtrlAll.GetItemWnd(0, 3)->GetID(), E_STR_ID_REC_ENCODE_TYPE_01);
		#endif	
	}
}

void CCfgRecordStreamDlg::UpdateView(bool bRefresh)
{
	if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListCtrlAll.GetItemWnd(0, COL_CHNN));
		if( pCheckAll )
		{
			pCheckAll->SetCheck( false, bRefresh );
		}
	}

	CCfgDlg::UpdateView( bRefresh );
}

void CCfgRecordStreamDlg::DoOther()
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0;
	int netVideoInputNum = 0;
	int maxChnn = 0;
	int effectChnn = 0;
	
	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;
	if(IsMixDvr())
	{
		unsigned long mixBitCount = 0;
		CCfgDlg::GetConfig( NCFG_ITEM_BITRANGE_FOR_VIDEOSIZE, &pTempData, tempDataLen);
		assert(tempDataLen > 0);
		mixBitCount = tempDataLen/sizeof(VS_SUPBRR);
		assert(mixBitCount == g_resolutionNum);
		m_pVSBit = new VS_SUPBRR[mixBitCount];
		memset(m_pVSBit,0,sizeof(VS_SUPBRR)*mixBitCount);
		VS_SUPBRR *pVS = (reinterpret_cast<VS_SUPBRR*>(pTempData));
		memcpy(m_pVSBit,pVS,tempDataLen);
		pTempData = NULL;
		tempDataLen = 0;
	}

	//读取数据 （与分辨率，帧率相关）	
	CCfgDlg::GetConfig( NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY, &pTempData, tempDataLen);
	assert(tempDataLen > 0);
	NCFG_INFO_SUPPORT_FRAME_RATE *pFrameRateData = (reinterpret_cast<NCFG_INFO_SUPPORT_FRAME_RATE*>(pTempData));
	unsigned long m_videoFormat = CCfgDlg::s_loginSuccessReply.productInfo.videoFormat;
	m_maxChnnRate = 0;
	m_totalRate = 0;
	m_defauResolution = 0;
	m_defauChnnRateNum = 0;

	if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
	{
		m_maxChnnRate = pFrameRateData->chnnRatePalOnCif;
		assert(m_maxChnnRate <= 25 * 4 * 2);
		m_totalRate = pFrameRateData->totalRatePalOnCif;
		m_defauChnnRateNum = pFrameRateData->defauChnnRateNumPal;
		m_maxNetChnnRate = CProduct::Instance()->NetTotalRatePal();
	}
	else if (VIDEO_FORMAT_NTSC == (VIDEO_FORMAT)m_videoFormat)
	{
		m_maxChnnRate = pFrameRateData->chnnRateNtscOnCif;
		assert(m_maxChnnRate <= 30 * 4 * 2);
		m_totalRate = pFrameRateData->totalRateNtscOnCif;
		m_defauChnnRateNum = pFrameRateData->defauChnnRateNumNtsc;
		m_maxNetChnnRate = CProduct::Instance()->NetTotalRateNtsc();
	}
	else
	{
		assert(false);
		m_maxChnnRate = 25;
		m_totalRate = m_maxChnnRate*m_channelNum;
		m_defauChnnRateNum = 25;
		m_maxNetChnnRate = 25;
	}

	if (IsMixDvr())
	{
		m_pChnnRecp = new CHANNEL_RECP[m_channelNum];
		memset(m_pChnnRecp,0,sizeof(CHANNEL_RECP)*m_channelNum);
		//读取NCFG_ITEM_MAJOR_CHANNEL_RECORD_PARAM_EX
		CCfgDlg::GetConfig( NCFG_ITEM_MAJOR_CHANNEL_RECORD_PARAM_EX, &pTempData, tempDataLen);
		assert(tempDataLen > 0);
		CHANNEL_RECP * pChRecp = (reinterpret_cast<CHANNEL_RECP*>(pTempData));
		memcpy(m_pChnnRecp,pChRecp,tempDataLen);

	}
	else
	{
		m_videoResolutionMask = pFrameRateData->dwResolution;
		m_defauResolution = pFrameRateData->defaResolution;
	}

	//读取数据 （与码流相关）
	pTempData = NULL;
	tempDataLen = 0;
	m_qualityLevelNum = 0;
	CCfgDlg::GetConfig(NCFG_ITEM_MAJOR_SUPPORT_QULITY_LEVELNUM, &pTempData, tempDataLen);
	assert(tempDataLen > 0);
	m_qualityLevelNum = tempDataLen/sizeof(unsigned long) - 1;
	unsigned long *pTempValue = reinterpret_cast<unsigned long *>(pTempData);
	m_defauQualityLevel = pTempValue[m_qualityLevelNum];
	m_pQualityLevelValue = new unsigned long [m_qualityLevelNum];
	memcpy(m_pQualityLevelValue, pTempData, tempDataLen - sizeof(unsigned long));	
	pTempData = NULL;
	tempDataLen = 0;
	m_videoEncodeMode = 0;

	CCfgDlg::GetConfig(NCFG_ITEM_MAJOR_SUPPORT_ENCODEMODE, &pTempData, tempDataLen);
	assert(tempDataLen/sizeof(unsigned long) == 2);
	pTempValue = reinterpret_cast<unsigned long *>(pTempData); 
	m_videoEncodeMode = pTempValue[0];
	m_defaVideoEncodeMode = pTempValue[1];
	pTempData = NULL;
	tempDataLen = 0;
	m_bitStreamCount = 0;

	CCfgDlg::GetConfig(NCFG_ITEM_MAJOR_SUPPORT_BITRATE_RANGE, &pTempData, tempDataLen);
	m_bitStreamCount = tempDataLen/sizeof(unsigned long) - 1;
	assert(m_bitStreamCount > 0);
	pTempValue = reinterpret_cast<unsigned long *>(pTempData);
	m_defaBitStreamValue = pTempValue[m_bitStreamCount];
	m_pBitStreamValue = new unsigned long [m_bitStreamCount];
	memcpy(m_pBitStreamValue, pTempData, tempDataLen - sizeof(unsigned long));

	CCfgDlg::GetConfig(NCFG_ITEM_ENCODE_SIZE_MAJOR_ALL_CH, &pTempData, tempDataLen);
	unsigned long bAllCH = *reinterpret_cast<unsigned long *>(pTempData);

	CCfgDlg::GetConfig(NCFG_ITEM_ENCODE_MAJOR_SPEC, &pTempData, tempDataLen);
	NCFG_INFO_ENCODE_SPEC_FRAME_RATE *pSpecFrameRateData = (reinterpret_cast<NCFG_INFO_ENCODE_SPEC_FRAME_RATE*>(pTempData));
	m_bVideoSizeSpecial = pSpecFrameRateData->bSupport;
	m_videoSizeCIFChnnBits = pSpecFrameRateData->videoSizeCIFChnnBits;
	m_videoSizeD1ChnnBits = pSpecFrameRateData->videoSizeD1ChnnBits;

	//设置数据
	bool bRefresh = false;
	GUI::CListCtrl& list = m_cfgListCtrl;
	char str[128] = {0};

	//2010-02-04 19:27:00 YSW
	//这里不能使用默认分辨率来定可选帧率，加入默认为D1(6Rate)，则会在下拉列表中插入1-6.
	//当把分辨率改为CIF（此时可支持帧率到25）,想设置分辨率为25，可是下拉框中不能选择。
	//导致不能灵活配置，所以应该直接使用最大数（需要比较不同制式最大数）。
	int rate = 0;
	int * pRateArr = NULL;
	if (IsMixDvr())
	{
		pRateArr = new int[m_channelNum];
		for(int rateIndex=0;rateIndex<m_channelNum;++rateIndex)
		{
			pRateArr[rateIndex] = RealRate(m_maxChnnRate,(VIDEO_SIZE)m_pChnnRecp[rateIndex].dfVS);
			//int rate = 0;
			if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
			{
				pRateArr[rateIndex] = (pRateArr[rateIndex] <= 25) ? pRateArr[rateIndex] : 25;
			}
			else
			{
				pRateArr[rateIndex] = (pRateArr[rateIndex] <= 30) ? pRateArr[rateIndex] : 30;
			}	

		}
	}
	else
	{
		if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
		{
			rate = (m_maxChnnRate <= 25) ? m_maxChnnRate : 25;
		}
		else
		{
			rate = (m_maxChnnRate <= 30) ? m_maxChnnRate : 30;
		}
	}
	
	unsigned long supportVideoSize = m_ipcSupportEncode.dwResolution;
	{
		PRODUCT_TYPE productTemp = CProduct::Instance()->ProductType();
		if ((TD_2704XE_L == productTemp) || (TD_2708XE_L == productTemp) || (TD_2716XE_L == productTemp)
			||(TD_2704XD_L == productTemp) || (TD_2708XD_L == productTemp) || (TD_2716XD_L == productTemp)
			||(TD_2704XD_M == productTemp) || (TD_2708XD_M == productTemp) || (TD_2716XD_M == productTemp)
			||(TD_2704XD_P == productTemp) || (TD_2708XD_P == productTemp) || (TD_2716XD_P == productTemp)
			||(TD_2708XE_MH == productTemp)|| (TD_2708XE_M == productTemp) || (TD_2716XE_M == productTemp)
			||(TD_2708XE_P == productTemp)
			||(TD_2704XE_S == productTemp) || (TD_2708XE_S == productTemp) || (TD_2716XE_S == productTemp)
			||(TD_2704XD_S == productTemp) || (TD_2708XD_S == productTemp) || (TD_2716XD_S == productTemp)
			||(TD_2704XS_SL == productTemp) || (TD_2708XS_SL == productTemp) || (TD_2716XS_SL == productTemp)
			||(TD_2704AS_SL == productTemp) ||(TD_2704AS_SL_A == productTemp) ||(TD_2704AS_PL == productTemp) || (TD_2708AS_SL == productTemp) || (TD_2716AS_SL == productTemp)
			||(TD_2704AS_S == productTemp) || (TD_2708AS_S == productTemp) || (TD_2708AS_S_A == productTemp) || (TD_2716AS_S == productTemp)
			||(TD_2704TS_S == productTemp) || (TD_2708TS_S == productTemp) || (TD_2716TS_S == productTemp)
			||(TD_2704TS_PL == productTemp) || (TD_2708TS_PL == productTemp) || (TD_2716TS_PL == productTemp)
			||(TD_2704TD_P == productTemp) || (TD_2708TD_P == productTemp) || (TD_2716TD_P == productTemp)
			||(TD_2704TD_PL == productTemp) || (TD_2708TD_PL == productTemp) || (TD_2716TD_PL == productTemp) || (TD_2732TD_PL == productTemp)
			|| (TD_2704TE_M == productTemp) || (TD_2708TE_M == productTemp) || (TD_2716TE_M == productTemp)
			|| (TD_2704AE_SL == productTemp) || (TD_2708AE_SL == productTemp) || (TD_2716AE_SL == productTemp)
			|| (TD_2004AL == productTemp) || (TD_2008AS == productTemp)
			||(TD_2704AS_PL == productTemp) || (TD_2708AS_PL == productTemp) || (TD_2716AS_PL == productTemp)
			|| (TD_2716AC_PL == productTemp)
			|| (TD_2704AE_PL == productTemp) || (TD_2708AE_PL == productTemp) || (TD_2716AE_PL == productTemp))
		{
			rate = RealRate(m_maxChnnRate, (VIDEO_SIZE)m_defauResolution);
			printf("#############%s,%d,rate:%d,m_maxChnnRate:%d\n",__FILE__,__LINE__,rate,m_maxChnnRate);
		}

	}

#if defined(__CHIPGM__)
	if (TD_2708TS_PL != CProduct::Instance()->ProductType())
	{
		if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
		{
			rate = (m_maxChnnRate <= 25) ? m_maxChnnRate : 25;
		}
		else
		{
			rate = (m_maxChnnRate <= 30) ? m_maxChnnRate : 30;
		}
	}
#else
	if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
	{
		rate = (m_maxChnnRate <= 25) ? m_maxChnnRate : 25;
	}	
	else
	{
		rate = (m_maxChnnRate <= 30) ? m_maxChnnRate : 30;
	}
#endif

	for (int i = 0; i < effectChnn; ++i)
	{
		int index = m_rowChnnMap.find(i)->second;

		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RATE));

		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		if(index >= localVideoInputNum)
		{
			rate = (rate < m_ipcSupportEncode.maxChnnRate) ? rate : m_ipcSupportEncode.maxChnnRate;
		}
		if (IsMixDvr())
		{
			for (int value = 1; value < pRateArr[i] + 1; ++value)
			{
				sprintf(str, "%d", value);
				pCombo->AddItem(str, value);
			}
			pCombo->SetCurItemData(pRateArr[i], false);//
		}
		else
		{
			for (int value = 1; value < rate + 1; ++value)
			{
				sprintf(str, "%d", value);
				pCombo->AddItem(str, value);
			}
			pCombo->SetCurItemData(rate, false);//

		}

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		//2011-07-12  ZYX
		if (i>=localVideoInputNum)
		{
			int j=0;
			unsigned long videoSize = 0;
			for (; j<s_loginSuccessReply.productInfo.netVideoInputNum; j++)
			{
				if (m_netChennelEncInfo[j].chnn == index)
				{
					for (int v=0; v<32; v++)
					{
						videoSize |= m_netChennelEncInfo[j].encInfo[v].videoSize;
					}
					break;
				}
			}
			if (j<s_loginSuccessReply.productInfo.netVideoInputNum && 0 != videoSize )
			{
				AddResolutionItem(videoSize&supportVideoSize, pCombo, true);
				pCombo->SetEnable(true,false);
			}
			else
			{
				//AddResolutionItem(supportVideoSize, pCombo, true);
				AddResolutionItem(CProduct::Instance()->IPCDefVideoSize(), pCombo, true);
				pCombo->SetCurItem(0);
				pCombo->SetEnable(false,false);
			}
		}//
		else
		{
			if (TD_2704HD == CProduct::Instance()->ProductType())
			{
				if (i < 4)
				{
					AddResolutionItem(m_videoResolutionMask, pCombo,false);
				}
				else
				{
					AddResolutionItem(m_videoResolutionMask, pCombo,true);
				}
			}
			else if(IsMixDvr())
			{
				AddResolutionItem(m_pChnnRecp[i].supVS,pCombo,true);
			}
			else
			{
				AddResolutionItem(m_videoResolutionMask, pCombo,true);
			}
		}

		if (bAllCH)
		{
			pCombo->SetEnable(false, false);
		}

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_QUALITY));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		AddQualityItem(m_pQualityLevelValue, m_qualityLevelNum, pCombo);
#ifdef __DVR_ULTIMATE__
		pCombo->SetCurItem(4);//
#endif

#ifdef __DVR_PROFESSIONAL__
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_STREAM_TYPE));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		//AddEncodeModeItem(m_videoEncodeMode, pCombo);
		if(i >= localVideoInputNum)
		{
			AddEncodeModeItem(VIDEO_ENCODE_MODE_CBR, pCombo);
		}
		else
		{
			AddEncodeModeItem(m_videoEncodeMode, pCombo);
		}
#ifdef __DVR_ULTIMATE__
		pCombo->SetCurItem(1);
#endif
		{
			GUI::CComboCtrl* pComboQuality = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd( i, COL_QUALITY));
			if (VIDEO_ENCODE_MODE_VBR == m_videoEncodeMode)
			{
				if (!pComboQuality->IsEnable())
				{
					pComboQuality->SetEnable(true, bRefresh);
				}
			}
			else
			{
				if (pComboQuality->IsEnable())
				{
					pComboQuality->SetEnable(false, bRefresh);
				}
			}
		}
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_MAX_BIT_STREAM));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);

		if(i < localVideoInputNum)
		{
			if (IsMixDvr())
			{
				AddMIxBitStreamItem(pCombo, m_pChnnRecp[i].dfVS);
			}
			else
			{
				AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo, m_defauResolution);
			}

		}
		else
		{
			AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo, m_ipcSupportEncode.defaResolution, i);
		}
#ifdef __DVR_ULTIMATE__
		pCombo->SetCurItem(4);
#endif
#endif
	}
	if(NULL != pRateArr)
	{
		delete []pRateArr;
		pRateArr = NULL;
	}

	if (IsMixDvr())//混合型dvr没有All选项
	{

	}
	else if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		bool bRefresh = false;
		GUI::CListCtrl& listAll = m_cfgListCtrlAll;	
		char str[128] = {0};

		//2010-02-04 19:27:00 YSW
		//这里不能使用默认分辨率来定可选帧率，加入默认为D1(6Rate)，则会在下拉列表中插入1-6.
		//当把分辨率改为CIF（此时可支持帧率到25）,想设置分辨率为25，可是下拉框中不能选择。
		//导致不能灵活配置，所以应该直接使用最大数（需要比较不同制式最大数）。
		//int rate = RealRate(m_maxChnnRate, (VIDEO_SIZE)m_defauResolution);
		int rate = 0;
		if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
		{
			rate = (m_maxChnnRate <= 25) ? m_maxChnnRate : 25;
		}
		else
		{
			rate = (m_maxChnnRate <= 30) ? m_maxChnnRate : 30;
		}

		{
			PRODUCT_TYPE productTemp = CProduct::Instance()->ProductType();
			if ((TD_2704XE_L == productTemp) || (TD_2708XE_L == productTemp) || (TD_2716XE_L == productTemp)
				||(TD_2704XD_L == productTemp) || (TD_2708XD_L == productTemp) || (TD_2716XD_L == productTemp)
				||(TD_2704XD_M == productTemp) || (TD_2708XD_M == productTemp) || (TD_2716XD_M == productTemp)
				||(TD_2704XD_P == productTemp) || (TD_2708XD_P == productTemp) || (TD_2716XD_P == productTemp)
				||(TD_2708XE_MH == productTemp)|| (TD_2708XE_M == productTemp) || (TD_2716XE_M == productTemp)
				||(TD_2708XE_P == productTemp) 
				||(TD_2704XE_S == productTemp) || (TD_2708XE_S == productTemp) || (TD_2716XE_S == productTemp)
				||(TD_2704XD_S == productTemp) || (TD_2708XD_S == productTemp) || (TD_2716XD_S == productTemp)
				||(TD_2704XS_SL == productTemp) || (TD_2708XS_SL == productTemp) || (TD_2716XS_SL == productTemp)
				||(TD_2704AS_SL == productTemp) || (TD_2704AS_SL_A == productTemp) || (TD_2704AS_PL == productTemp) ||(TD_2708AS_SL == productTemp) || (TD_2716AS_SL == productTemp)
				||(TD_2704AS_S == productTemp) || (TD_2708AS_S == productTemp) || (TD_2708AS_S_A == productTemp) || (TD_2716AS_S == productTemp)
				||(TD_2704TS_S == productTemp) || (TD_2708TS_S == productTemp) || (TD_2716TS_S == productTemp)
				||(TD_2704TS_PL == productTemp) || (TD_2708TS_PL == productTemp) || (TD_2716TS_PL == productTemp)
				||(TD_2704TD_PL == productTemp) || (TD_2708TD_PL == productTemp) || (TD_2716TD_PL == productTemp) || (TD_2732TD_PL == productTemp)
				||(TD_2704TD_P == productTemp) || (TD_2708TD_P == productTemp) || (TD_2716TD_P == productTemp)
				|| (TD_2704TE_M == productTemp) || (TD_2708TE_M == productTemp) || (TD_2716TE_M == productTemp)
				|| (TD_2704AE_SL == productTemp) || (TD_2708AE_SL == productTemp) || (TD_2716AE_SL == productTemp)
				|| (TD_2004AL == productTemp) || (TD_2008AS == productTemp)
				||(TD_2704AS_PL == productTemp) || (TD_2708AS_PL == productTemp) || (TD_2716AS_PL == productTemp)
				|| (TD_2716AC_PL == productTemp)
				|| (TD_2704AE_PL == productTemp) || (TD_2708AE_PL == productTemp) || (TD_2716AE_PL == productTemp)
				)
			{
				rate = RealRate(m_maxChnnRate, (VIDEO_SIZE)m_defauResolution);
				printf("#############%s,%d,rate:%d,m_maxChnnRate:%d\n",__FILE__,__LINE__,rate,m_maxChnnRate);
			}

		}

	#if defined(__CHIPGM__)
		if (TD_2704AS_SL == CProduct::Instance()->ProductType())
		{
			if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
			{
				rate = (m_maxChnnRate <= 25) ? m_maxChnnRate : 25;
			}
			else
			{
				rate = (m_maxChnnRate <= 30) ? m_maxChnnRate : 30;
			}
		}
	#endif

		assert(m_defauChnnRateNum <= rate);
		int i = 0;
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd( i, COL_RATE));
		pCombo->RemoveAllItem(bRefresh);
		for (int value = 1; value < rate + 1; ++value)
		{
			sprintf(str, "%d", value);
			pCombo->AddItem(str, value);
		}
		pCombo->SetCurItemData(m_defauChnnRateNum, bRefresh);


		PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
		if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
			|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
			|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
			|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
			|| (TD_2704AS_SL_A == productID) || (TD_2708AS_SL_A == productID) || (TD_2716AS_SL_A == productID)
			|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
			|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
			|| (TD_2004AL == productID)
			|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
			|| (TD_2716AC_PL == productID)
			|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
		{
			pCombo->Show(false, false);
		}


		pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd( i, COL_RESOLUTION));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		AddResolutionItem(m_videoResolutionMask, pCombo,true);
		pCombo->SetCurItemData(m_defauResolution, bRefresh);	

		if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
			|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
			|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
			|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
			|| (TD_2704AS_SL_A == productID) || (TD_2708AS_SL_A == productID) || (TD_2716AS_SL_A == productID)
			|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
			|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
			|| (TD_2004AL == productID)
			|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
			|| (TD_2716AC_PL == productID)
			|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
		{
			pCombo->Show(false, false);
		}


		pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd( i, COL_QUALITY));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		AddQualityItem(m_pQualityLevelValue, m_qualityLevelNum, pCombo);
		pCombo->SetCurItemData(m_defauQualityLevel, bRefresh);	

		if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
			|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
			|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
			|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
			|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
			|| (TD_2704AS_SL_A == productID) || (TD_2708AS_SL_A == productID) || (TD_2716AS_SL_A == productID)
			|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
			|| (TD_2004AL == productID)
			|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
			|| (TD_2716AC_PL == productID)
			|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
		{
			pCombo->Show(false, false);
		}


	#ifdef __DVR_PROFESSIONAL__	
		pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd(i,COL_STREAM_TYPE));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		AddEncodeModeItem(m_videoEncodeMode, pCombo);
		pCombo->SetCurItemData(m_defaVideoEncodeMode, bRefresh);
		{
			GUI::CComboCtrl* pComboQuality = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd( i, COL_QUALITY));
			if (VIDEO_ENCODE_MODE_VBR == m_defaVideoEncodeMode)
			{
				if (!pComboQuality->IsEnable())
				{
					pComboQuality->SetEnable(true, bRefresh);
				}
			}
			else
			{
				if (pComboQuality->IsEnable())
				{
					pComboQuality->SetEnable(false, bRefresh);
				}
			}
		}

		if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
			|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
			|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
			|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
			|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
			|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
			|| (TD_2004AL == productID)
			|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
			|| (TD_2716AC_PL == productID)
			|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
		{
			pCombo->Show(false, false);
		}

		pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd(i,COL_MAX_BIT_STREAM));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo,m_defauResolution);
		pCombo->SetCurItemData(m_defaBitStreamValue, bRefresh);

		if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
			|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
			|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
			|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
			|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
			|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
			|| (TD_2004AL == productID)
			|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID) 
			|| (TD_2716AC_PL == productID)
			|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
		{
			pCombo->Show(false, false);
		}
	#endif
	}

	if (TD_2704HD == CProduct::Instance()->ProductType())
	{
		for (int ii = 4; ii < s_loginSuccessReply.productInfo.localVideoInputNum; ++ii)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd( ii, COL_RESOLUTION));
			pCombo->SetEnable(false,false);
		}
	}
	
	if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		m_frameRate.Initial((VIDEO_FORMAT)m_videoFormat, s_loginSuccessReply.productInfo.localVideoInputNum, m_totalRate, m_maxChnnRate, CH);
	}

	if(s_loginSuccessReply.productInfo.netVideoInputNum > 0)
	{
		m_ipcFrameEncode.Initial(s_loginSuccessReply.productInfo.netVideoInputNum, \
			m_ipcSupportEncode.maxTotalBitRate, m_ipcSupportEncode.maxTotalRateOnVGA, \
			m_pBitStreamValue, m_bitStreamCount, m_ipcSupportEncode.defaResolution, \
			m_ipcSupportEncode.defaChnnRate, m_ipcSupportEncode.defaBitRate);
	}
}

void CCfgRecordStreamDlg::EndOther()
{
	m_frameRate.Quit();
	m_ipcFrameEncode.Quit();
	if (m_pBitStreamValue != NULL)
	{
		delete [] m_pBitStreamValue;
		m_pBitStreamValue = NULL;
	}
	if (m_pQualityLevelValue != NULL)
	{
		delete [] m_pQualityLevelValue;
		m_pQualityLevelValue = NULL;
	}
// 	if (NULL != m_pNetDevInfo)
// 	{
// 		delete []m_pNetDevInfo;
// 		m_pNetDevInfo = NULL;
// 	}
}

bool CCfgRecordStreamDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	
	GUI::CListCtrl& list = m_cfgListCtrl;
	char str[128] = {0};

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_ENCODE_MAJOR:
		{
			assert(len==sizeof(ENCODE_INFO)*localVideoInputNum);

			const ENCODE_INFO* pInfo = reinterpret_cast<const ENCODE_INFO*>(pData);
			for (int i = 0; i < localVideoInputNum; ++i)
			{
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RATE));
				assert(pCombo);
				int rate = 0;	
				if (m_bVideoSizeSpecial && (((ULONGLONG)0x01 << i) & m_videoSizeD1ChnnBits))
				{
					rate = pInfo[i].rate;
				}
				else
				{
					if (UsedRate(pInfo[i].rate, (VIDEO_SIZE)(pInfo[i].resolution)) > m_maxChnnRate)
					{
						rate = RealRate(m_maxChnnRate, (VIDEO_SIZE)(pInfo[i].resolution));
					}
					else
					{
						rate = pInfo[i].rate;
					}
				}
				
				pCombo->SetCurItemData(rate,bRefresh);
				m_checkEncInfo[i].usRate = rate;
				

				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));
				assert(pCombo);
				if (IsMixDvr())
				{
					if ((pInfo[i].resolution)&(m_pChnnRecp[i].supVS))
					{
						pCombo->SetCurItemData(pInfo[i].resolution,bRefresh);
					}
					else
					{
						assert(false);
						pCombo->SetCurItem(0, bRefresh);
					}
				}
				else
				{
					if ((pInfo[i].resolution)&(m_videoResolutionMask))
					{
						pCombo->SetCurItemData(pInfo[i].resolution,bRefresh);
					}
					else
					{
						assert(false);
						pCombo->SetCurItem(0, bRefresh);
					}
				}

				m_checkEncInfo[i].ulRes = pInfo[i].resolution;
				
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_QUALITY));
				assert(pCombo);
				if (IsInQualityLevel(pInfo[i].quality))
				{
					pCombo->SetCurItemData(pInfo[i].quality,bRefresh);
				}
				else
				{
					assert(false);
					pCombo->SetCurItem(0, bRefresh);
				}

				if (VIDEO_ENCODE_MODE_VBR == pInfo[i].encodeType)
				{
					if (!pCombo->IsEnable())
					{
						pCombo->SetEnable(true, false);
					}
				}
				else
				{
					if (pCombo->IsEnable())
					{
						pCombo->SetEnable(false, false);
					}
				}

#ifdef __DVR_PROFESSIONAL__
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_STREAM_TYPE));
				assert(pCombo);
				if ((pInfo[i].encodeType) & (m_videoEncodeMode))
				{
					pCombo->SetCurItemData(pInfo[i].encodeType,bRefresh);
				}
				else
				{
					assert(false);
					pCombo->SetCurItem(0, bRefresh);
				}	

				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_MAX_BIT_STREAM));
				assert(pCombo);
				//设置码流范围之前需要重新设置对应分辨率的码流范围
				assert(pCombo);
				if(IsMixDvr())
				{
					AddMIxBitStreamItem(pCombo,pInfo[i].resolution);
				}
				if (IsInBitStreamRange(pInfo[i].hBitStream))
				{
					pCombo->SetCurItemData(pInfo[i].hBitStream,bRefresh);
				}
				else
				{
					assert(false);
					pCombo->SetCurItem(0, bRefresh);
				}
				m_checkEncInfo[i].usBitrate = pInfo[i].hBitStream;
#endif
				m_frameRate.AddRate(i, rate, static_cast<VIDEO_SIZE>(pInfo[i].resolution));
			}

			char str [128] = {0};
			unsigned long remainRate = m_frameRate.Remaining();
			PRODUCT_TYPE productTypeTemp = CProduct::Instance()->ProductType();
			if ((TD_2324SE_S == productTypeTemp) || (TD_2316SE_S == productTypeTemp) || (TD_2316CDS == productTypeTemp))
			{
				sprintf(str, " : %d (CIF).", remainRate);
			}
			else
			{
#if defined(__CUSTOM_US02__)
				sprintf(str, " : %d (CIF), %d (D1).", remainRate, remainRate/4);
#else
				sprintf(str, " : %d (CIF), %d (HD1), %d (D1).", remainRate, remainRate/2, remainRate/4);
#endif
			}
			m_cfgScRemainNum.SetCaption(str, false );
		}
		return true;
	case NCFG_ITEM_ENCODE_MAJOR_EX:
		{
#ifdef __DVR_PROFESSIONAL__
			assert(len==(sizeof(ENCODE_INFO_EX)*maxChnn));
			
			ENCODE_INFO_EX *pInfo = new ENCODE_INFO_EX[maxChnn];
			memcpy(pInfo, pData, sizeof(ENCODE_INFO_EX) * maxChnn);

			m_orignLowBitRate = pInfo[0].lBitStream;

			MESSAGE_DATA msgData;
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NET_DEVICE_LOG_GET, NULL, 0, msgData);
			PACKCMD *pPackCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
			if(CMD_REPLY_NET_DEVICE_LOG_GET_SUCC == pPackCmd->cmdType)
			{
				unsigned long count =*reinterpret_cast<unsigned long*>( msgData.pData + sizeof(PACKCMD) );
				IP_DEVICE_INFO *pIPDeviceInfo = reinterpret_cast<IP_DEVICE_INFO *>(msgData.pData+sizeof(PACKCMD)+sizeof(unsigned long));

				for(unsigned long i = 0; i < count; i++)
				{
					if(pIPDeviceInfo[i].bUseDefaultCfg && (pIPDeviceInfo[i].channel != INVALID_INDEX(unsigned char)))
					{
						pInfo[pIPDeviceInfo[i].channel].resolution = m_ipcSupportEncode.defaResolution;
						pInfo[pIPDeviceInfo[i].channel].rate = m_ipcSupportEncode.defaChnnRate;
						pInfo[pIPDeviceInfo[i].channel].hBitStream = m_ipcSupportEncode.defaBitRate;
					}
				}
			}
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#ifdef __DVR_ULTIMATE__
			for (int i = localVideoInputNum; i < effectChnn; ++i)
			{
				int index = m_rowChnnMap.find(i)->second;

				int rate = 0;
				//2011-07-12  ZYX

				int j = 0;
				unsigned long videoSize = 0;
				int maxRate = 0;

				for (; j<s_loginSuccessReply.productInfo.netVideoInputNum; j++)
				{
					if (m_netChennelEncInfo[j].chnn == index)
					{
						for (int v=0; v<32; v++)
						{
							videoSize |= m_netChennelEncInfo[j].encInfo[v].videoSize;
							if (m_netChennelEncInfo[j].encInfo[v].videoSize == pInfo[index].resolution)
							{
								maxRate = m_netChennelEncInfo[j].encInfo[v].ucRate;
							}
						}
						break;
					}
				}

				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));
				if (pInfo[index].resolution &videoSize)
				{
					pCombo->SetCurItemData(pInfo[index].resolution,bRefresh);
					pCombo->SetEnable(true,false);
				}
				else
				{
					pCombo->SetCurItem(0, bRefresh);
					pCombo->SetEnable(false,false);
				}

				bool bIPCOnline = (1 == m_netChennelEncInfo[index - localVideoInputNum].online) ? true : false;
				if (!bIPCOnline)
				{
					pCombo->SetEnable(false,bRefresh);
				}
				else
				{
					pCombo->SetEnable(true,bRefresh);
				}

				unsigned long resolution = pCombo->GetCurItemData();
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_MAX_BIT_STREAM));
				AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo, resolution, index);

				if (!bIPCOnline)
				{
					pCombo->SetEnable(false,bRefresh);
				}
				else
				{
					pCombo->SetEnable(true,bRefresh);
				}
				
				rate = pInfo[index].rate;

				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RATE));
				assert(pCombo);
				rate = (rate <= m_ipcSupportEncode.maxChnnRate) ? rate : m_ipcSupportEncode.maxChnnRate;
				if (j < s_loginSuccessReply.productInfo.netVideoInputNum)
				{
					pCombo->RemoveAllItem(bRefresh);

					if( maxRate <= 0 )
					{
						maxRate = m_ipcSupportEncode.maxChnnRate/2;
						pCombo->SetEnable(false,false);
					}
					else
					{
						pCombo->SetEnable(true,false);
					}

					if((DVRVIDEO_SIZE_HD1080 == resolution || DVRVIDEO_SIZE_SXGA == resolution 
						|| DVRVIDEO_SIZE_3M == resolution || DVRVIDEO_SIZE_16_9_3M == resolution 
						|| DVRVIDEO_SIZE_4M == resolution || DVRVIDEO_SIZE_5M == resolution) && !m_ipcSupportEncode.bLimitByBitRate)
					{
						maxRate = ( maxRate <= m_ipcSupportEncode.maxChnnRate/2) ? maxRate: m_ipcSupportEncode.maxChnnRate/2;
					}
					else
					{
						maxRate = ( maxRate <= m_ipcSupportEncode.maxChnnRate) ? maxRate: m_ipcSupportEncode.maxChnnRate;
					}

					for (int value = 1; value <= maxRate; ++value)
					{
						sprintf(str, "%d", value);
						pCombo->AddItem(str, value);
					}

					if (rate > maxRate)
					{
						rate = maxRate;
					}
				}

				pCombo->SetCurItemData(rate,bRefresh);
				if (!bIPCOnline)
				{
					pCombo->SetEnable(false,bRefresh);
				}
				else
				{
					pCombo->SetEnable(true,bRefresh);
				}


				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_QUALITY));
				assert(pCombo);
				if (IsInQualityLevel(pInfo[index].quality))
				{
					pCombo->SetCurItemData(pInfo[index].quality,bRefresh);
				}
				else
				{
					pCombo->SetCurItem(0, bRefresh);
				}

				if (VIDEO_ENCODE_MODE_VBR == pInfo[index].encodeType)
				{
					if (!pCombo->IsEnable())
					{
						pCombo->SetEnable(true, false);
					}
				}
				else
				{
					if (pCombo->IsEnable())
					{
						pCombo->SetEnable(false, false);
					}
				}

				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_STREAM_TYPE));
				assert(pCombo);
				if ((pInfo[index].encodeType) & (m_videoEncodeMode))
				{
					pCombo->SetCurItemData(pInfo[index].encodeType,bRefresh);
				}
				else
				{
					pCombo->SetCurItem(0, bRefresh);
				}

				if (!bIPCOnline)
				{
					pCombo->SetEnable(false,bRefresh);
				}
				else
				{
					pCombo->SetEnable(true,bRefresh);
				}

				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_MAX_BIT_STREAM));
				assert(pCombo);
				if (IsInBitStreamRange(pInfo[index].hBitStream))
				{
					pCombo->SetCurItemData(pInfo[index].hBitStream,bRefresh);
				}
				else
				{
					pCombo->SetCurItem(0, bRefresh);
				}

				if (!bIPCOnline)
				{
					pCombo->SetEnable(false,bRefresh);
				}
				else
				{
					pCombo->SetEnable(true,bRefresh);
				}

				m_ipcFrameEncode.AddEncode(index-localVideoInputNum, rate, static_cast<VIDEO_SIZE>(pInfo[index].resolution), pInfo[index].hBitStream);
			}
#endif

			delete [] pInfo;
#endif
		}
		return true;
	default:
		return false;
		break;
	}
	return false;
}

bool CCfgRecordStreamDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CListCtrl& list = m_cfgListCtrl;
	const GUI::CListCtrl& listAll = m_cfgListCtrlAll;

	bool bAll = false;
	if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
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
	case NCFG_ITEM_ENCODE_MAJOR:
		{
			assert(len==sizeof(ENCODE_INFO)*localVideoInputNum);
			ENCODE_INFO* pInfo = reinterpret_cast<ENCODE_INFO*>(pData);

			for (int i=0; i<localVideoInputNum; i++)
			{
				GUI::CComboCtrl* pCbRate = NULL;
				GUI::CComboCtrl* pCbResolution = NULL;
				GUI::CComboCtrl* pCbQuality = NULL;
				GUI::CComboCtrl* pCbType = NULL;
				GUI::CComboCtrl* pCbMax = NULL;

				if( bAll )
				{
					pCbRate = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_RATE));
					pCbResolution = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_RESOLUTION));
					pCbQuality = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_QUALITY));
#ifdef __DVR_PROFESSIONAL__
					pCbType = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_STREAM_TYPE));
					pCbMax = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_MAX_BIT_STREAM));
#endif
				}
				else
				{
					pCbRate = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_RATE));
					pCbResolution = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_RESOLUTION));
					pCbQuality = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_QUALITY));
#ifdef __DVR_PROFESSIONAL__
					pCbType = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_STREAM_TYPE));
					pCbMax = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_MAX_BIT_STREAM));
#endif
				}

				pInfo[i].rate = pCbRate->GetCurItemData();
				pInfo[i].resolution = pCbResolution->GetCurItemData();
				pInfo[i].quality = pCbQuality->GetCurItemData();
#ifdef __DVR_PROFESSIONAL__
				pInfo[i].encodeType = pCbType->GetCurItemData();
				pInfo[i].hBitStream = pCbMax->GetCurItemData();
#endif
			}
		}
		return true;
	case NCFG_ITEM_ENCODE_MAJOR_EX:
		{
			assert(len==(sizeof(ENCODE_INFO_EX)*maxChnn));
			ENCODE_INFO_EX* pInfo = reinterpret_cast<ENCODE_INFO_EX*>(pData);

			for (int i = 0; i< effectChnn; i++)
			{
				GUI::CComboCtrl* pCbRate = NULL;
				GUI::CComboCtrl* pCbResolution = NULL;
				GUI::CComboCtrl* pCbQuality = NULL;
				GUI::CComboCtrl* pCbType = NULL;
				GUI::CComboCtrl* pCbMax = NULL;

				int index = m_rowChnnMap.find(i)->second;

				if( bAll/* && i<localVideoInputNum*/)
				{
					pCbRate = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_RATE));
					pCbResolution = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_RESOLUTION));
					pCbQuality = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_QUALITY));
#ifdef __DVR_PROFESSIONAL__
					pCbType = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_STREAM_TYPE));
					pCbMax = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_MAX_BIT_STREAM));
#endif
				}
				else
				{
					pCbRate = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_RATE));
					pCbResolution = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_RESOLUTION));
					pCbQuality = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_QUALITY));
#ifdef __DVR_PROFESSIONAL__
					pCbType = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_STREAM_TYPE));
					pCbMax = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_MAX_BIT_STREAM));
#endif
				}

				if (index < localVideoInputNum)
				{
					pInfo[index].rate = pCbRate->GetCurItemData();
					pInfo[index].resolution = pCbResolution->GetCurItemData();
					pInfo[index].quality = pCbQuality->GetCurItemData();
#ifdef __DVR_PROFESSIONAL__
					pInfo[index].encodeType = pCbType->GetCurItemData();
					pInfo[index].hBitStream = pCbMax->GetCurItemData();
#endif
				}
				else
				{
					if (1 == m_netChennelEncInfo[index - localVideoInputNum].online)
					{
						pInfo[index].rate = pCbRate->GetCurItemData();
						pInfo[index].resolution = pCbResolution->GetCurItemData();
						pInfo[index].quality = pCbQuality->GetCurItemData();
#ifdef __DVR_PROFESSIONAL__
						pInfo[index].encodeType = pCbType->GetCurItemData();
						pInfo[index].hBitStream = pCbMax->GetCurItemData();
#endif
					}
				}
			}

// 			if(m_bModified)
// 			{
// #if defined(__DVR_ULTIMATE__)
// 				if(64 != m_orignLowBitRate)
// 				{
// 					pInfo[0].lBitStream = 64;
// 				}
// 				else
// 				{
// 					pInfo[0].lBitStream = 65;
// 				}
// #endif
// 			}
		}
		return true;
	default:
		break;
	}
	return false;
}

BEGIN_MSG_MAP(CCfgRecordStreamDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgListCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_cfgListCtrlAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()

unsigned long CCfgRecordStreamDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	unsigned long ret = KEY_VIRTUAL_MSG_STOP;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		//2010-03-03 17:57:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
		GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListCtrlAll.GetItemWnd(0, COL_CHNN));
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
			case COL_RATE:
			case COL_RESOLUTION:
			case COL_QUALITY:
#ifdef __DVR_PROFESSIONAL__
			case	 COL_STREAM_TYPE:
			case	 COL_MAX_BIT_STREAM:
#endif
				if(IsChange(NCFG_ITEM_ENCODE_MAJOR) || IsChange(NCFG_ITEM_ENCODE_MAJOR_EX))
				{
					m_bModified = true;
					SetModify(true);
				}
				else
				{
					m_bModified = false;
				}
				ret = KEY_VIRTUAL_MSG_CONTINUE;
				break;
		default:
			break;
		}
	}

#ifdef __DVR_PROFESSIONAL__
	int index = m_rowChnnMap.find(row)->second;
	assert (index < maxChnn);
	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RATE));
	assert(pCombo);
	unsigned char rate = static_cast<unsigned char>(pCombo->GetCurItemData());

	if (COL_RESOLUTION == col)
	{
		if(index >= localVideoInputNum)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RESOLUTION));
			assert(pCombo);
			VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pCombo->GetCurItemData());

			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row,COL_MAX_BIT_STREAM));
			AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo, size, index);

			//重新填充帧率
			{
				int j = 0;
				unsigned long videoSize = 0;
				int maxRate = 0;

				for (; j<s_loginSuccessReply.productInfo.netVideoInputNum; j++)
				{
					if (m_netChennelEncInfo[j].chnn == index)
					{
						for (int v=0; v<32; v++)
						{
							videoSize |= m_netChennelEncInfo[j].encInfo[v].videoSize;
							if (m_netChennelEncInfo[j].encInfo[v].videoSize == size)
							{
								maxRate = m_netChennelEncInfo[j].encInfo[v].ucRate;
							}
						}
						break;
					}
				}

				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row,COL_RATE));
				assert(pCombo);

				if( maxRate <= 0 )
				{
					maxRate = m_ipcSupportEncode.maxChnnRate/2;
					pCombo->SetEnable(false,false);
				}
				else
				{
					pCombo->SetEnable(true,false);
				}

				if (j < s_loginSuccessReply.productInfo.netVideoInputNum)
				{
					pCombo->RemoveAllItem(true);
					if((DVRVIDEO_SIZE_HD1080 == size || DVRVIDEO_SIZE_SXGA == size 
						|| DVRVIDEO_SIZE_3M == size || DVRVIDEO_SIZE_16_9_3M == size 
						|| DVRVIDEO_SIZE_4M == size || DVRVIDEO_SIZE_5M == size) && !m_ipcSupportEncode.bLimitByBitRate)
					{
						maxRate = ( maxRate <= m_ipcSupportEncode.maxChnnRate/2) ? maxRate: m_ipcSupportEncode.maxChnnRate/2;
					}
					else
					{
						maxRate = ( maxRate <= m_ipcSupportEncode.maxChnnRate) ? maxRate: m_ipcSupportEncode.maxChnnRate;
					}

					char str[128] = {0};
					for (int value = 1; value <= maxRate; ++value)
					{
						sprintf(str, "%d", value);
						pCombo->AddItem(str, value);
					}
				}

				//rate = RealRate(m_maxNetChnnRate, (VIDEO_SIZE)m_defauResolution);
				//rate = (rate <= maxRate) ? rate : maxRate;
				pCombo->SetCurItemData(maxRate, true);
			}
		}
		else
		{
			GUI::CComboCtrl* pComboR = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RESOLUTION));
			GUI::CComboCtrl* pComboRate = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RATE));
			GUI::CComboCtrl* pComboB = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_MAX_BIT_STREAM));
			VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pComboR->GetCurItemData());
			
#if defined(__CHIPGM__)
			int maxRate = 0;
			if (TD_2708TS_PL == CProduct::Instance()->ProductType())
			{
				unsigned long videoFormat = CCfgDlg::s_loginSuccessReply.productInfo.videoFormat;
				if (DVRVIDEO_SIZE_HD1080 == size)
				{
					if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)videoFormat)
					{
						maxRate = 12;
					}
					else
					{
						maxRate = 15;
					}
				}
				else if (DVRVIDEO_SIZE_HD720 == size || DVRVIDEO_SIZE_960H == size)
				{
					if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)videoFormat)
					{
						maxRate = 25;
					}
					else
					{
						maxRate = 30;
					}
				}
				char str[128] = {0};
				pComboRate->RemoveAllItem(true);
				for (int value = 1; value <= maxRate; ++value)
				{
					sprintf(str, "%d", value);
					pComboRate->AddItem(str, value);
				}
				pComboRate->SetCurItemData(maxRate, true);
			}
#endif

			if(IsMixDvr())
			{
				AddMIxBitStreamItem(pComboB, size);
			}
			
			if (DVRVIDEO_SIZE_CIF == size)
			{
				pComboB->SetCurItemData(768, true);
			}	
			else if (DVRVIDEO_SIZE_D1 == size)
			{
				pComboB->SetCurItemData(1536,true);
			}
			else if (DVRVIDEO_SIZE_HD1 == size)
			{
				pComboB->SetCurItemData(1024, true);
			}
			else if (DVRVIDEO_SIZE_HD1080 == size)
			{
				PRODUCT_TYPE product = CProduct::Instance()->ProductType();
				if ((TD_2704XD_M == product) || (TD_2708XD_M == product) || (TD_2716XD_M == product)|| (TD_2704XD_P == product) || (TD_2708XD_P == product) ||(TD_2708XE_P == product))
				{
					pComboB->SetCurItemData(10240, true);
				}
				else if ((TD_2716XD_P == product) || (TD_2708XE_M == product) || (TD_2716XE_M == product) || (TD_2708XE_MH == product) 
					|| (TD_2704AS_SL == product) || (TD_2704AS_SL_A == product) || (TD_2704AS_PL == product) || (TD_2708AS_SL == product) || (TD_2716AS_SL == product) || (TD_2708TS_S == product) || (TD_2716TS_S == product) || (TD_2708TS_PL == product) || (TD_2716TS_PL == product)
					|| (TD_2704AS_SL_A == product) || (TD_2708AS_SL_A == product) || (TD_2716AS_SL_A == product)
					|| (TD_2704TD_PL == product) || (TD_2708TD_PL == product) || (TD_2716TD_PL == product) || (TD_2732TD_PL == product) || (TD_2704TD_P == product) || (TD_2708TD_P == product) || (TD_2716TD_P == product)
					|| (TD_2704TE_M == product) || (TD_2708TE_M == product) || (TD_2716TE_M == product)
					|| (TD_2704AE_SL == product) || (TD_2708AE_SL == product) || (TD_2716AE_SL == product)
					|| (TD_2004AL == product)
					|| (TD_2704AS_PL == product) || (TD_2708AS_PL == product) || (TD_2716AS_PL == product)
					|| (TD_2716AC_PL == product)
					|| (TD_2704AE_PL == product) || (TD_2708AE_PL == product) || (TD_2716AE_PL == product))
				{
					pComboB->SetCurItemData(8192, true);
				}
				else
				{
					pComboB->SetCurItemData(12288, true);
				}
			}
			else if (DVRVIDEO_SIZE_HD720 == size)
			{
				PRODUCT_TYPE product = CProduct::Instance()->ProductType();
				if(TD_2708XD_P == product)
				{
					pComboB->SetCurItemData(12288, true);
				}
				else
				{
					pComboB->SetCurItemData(4096, true);
				}
				
			}
			else if (DVRVIDEO_SIZE_960H == size)
			{
				PRODUCT_TYPE product = CProduct::Instance()->ProductType();
				if ((TD_2504HD_C == product) ||(TD_2508HD_C == product) || (TD_2516HD_C == product) || (TD_2524HD_C == product) || (TD_2532HD_C == product) 
					|| (TD_2604LE == product) || (TD_2608LE == product) || (TD_2616LE == product) || (TD_2508HE_C_3521 == product) || (TD_2524HE_C == product) || (TD_2532HE_C == product)
					|| (TD_2304ME_C == product) || (TD_2308ME_C == product) || (TD_2316ME_C == product) || (TD_2616LD == product) 
					|| (TD_2304SS_C == product) || (TD_2308SS_C == product) || (TD_2316SS_C == product)
					|| (TD_2304SL_C == product) || (TD_2308SL_C == product)
					|| (TD_2304SE_C == product) || (TD_2308SE_C == product) || (TD_2316SE_C == product) || (TD_2304SS_SDI == product) || (TD_2308SS_SDI == product)||(TD_2304SS_SL_3515A == product)
					|| (TD_2508HE_SDI == product) || (TD_2516HS_BL == product) || (TD_2704AS_SL == product) || (TD_2704AS_SL_A == product) || (TD_2704AS_PL == product) || (TD_2704AS_S == product) || (TD_2708AS_S == product) || (TD_2708AS_S_A == product)
					|| (TD_2004AL == product) || (TD_2008AS == product)
					|| (TD_2704AS_S_A == product) || (TD_2708AS_S_A == product)
					|| (TD_2704AS_SL_A == product))
				{
					pComboB->SetCurItemData(2048,true);
				}
				else
				{
					pComboB->SetCurItemData(1792,true);
				}
			}
			else
			{

			}
		}
	}

#endif
	//2010-03-02 11:00:00 YSW
	//只有点击帧率和分辨率时，才处理资源部分
	if ((COL_RATE == col) || (COL_RESOLUTION == col))
	{
		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RESOLUTION));
		assert(pCombo);
		VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pCombo->GetCurItemData());

		if(index < localVideoInputNum)
		{
			unsigned long remainRate = 0;

			if (IsSupportFunction(FUNC_SERVER_ENCODE_CHECK))
			{
				unsigned char * pMsgData = new unsigned char [sizeof(CHECK_ENC_INFO) * localVideoInputNum + sizeof(unsigned long)];
				unsigned long * pCurrentChnn = (unsigned long *)(pMsgData);
				* pCurrentChnn = row;
				CHECK_ENC_INFO * pCheckEncInfo = (CHECK_ENC_INFO *)(pMsgData + sizeof(unsigned long));

				memcpy(pCheckEncInfo, m_checkEncInfo, sizeof(CHECK_ENC_INFO) * localVideoInputNum);

				if (COL_RATE == col)
				{
					pCheckEncInfo[row].usRate = rate;
				}

				if (COL_RESOLUTION == col)
				{
					pCheckEncInfo[row].ulRes = size;
				}

				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));

				CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_FIRST_CHECK_ENC_INFO, pMsgData, sizeof(CHECK_ENC_INFO) * localVideoInputNum + sizeof(unsigned long), msgData);

				CHECK_ENC_INFO * pCheckEncInfoChecked = reinterpret_cast<CHECK_ENC_INFO *>( msgData.pData + sizeof(PACKCMD));

				if (0 != memcmp(pCheckEncInfo, pCheckEncInfoChecked, sizeof(CHECK_ENC_INFO) * localVideoInputNum))
				{
					GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RESOLUTION));
					size = (VIDEO_SIZE)pCheckEncInfoChecked[row].ulRes;
					pCombo->SetCurItemData(size, true);

					pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RATE));
					rate = pCheckEncInfoChecked[row].usRate;
					pCombo->SetCurItemData(rate, true);

					MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				}

				if ((DVRVIDEO_SIZE_HD1080 == size) && (rate > CProduct::Instance()->LocalDefChnnRateNum()))
				{
					GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RATE));
					assert(pCombo);
					rate = CProduct::Instance()->LocalDefChnnRateNum();
					pCombo->SetCurItemData(rate, true);

					MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				}

				m_checkEncInfo[row].usRate = rate;
				m_checkEncInfo[row].ulRes = size;

				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			}
			else
			{
				if (m_bVideoSizeSpecial && (((ULONGLONG)0x01 << index) & m_videoSizeD1ChnnBits))
				{
					remainRate = m_frameRate.ModifyRate(row, rate, size, false);
				}
				else
				{
					if (!m_frameRate.TryRate(row, rate, size))
					{
						GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RESOLUTION));
						assert(pCombo);
						pCombo->SetCurItemData(size, true);

						pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RATE));
						assert(pCombo);
						pCombo->SetCurItemData(rate, true);

						MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					}

					if ((DVRVIDEO_SIZE_HD1080 == size) && (rate > CProduct::Instance()->LocalDefChnnRateNum()))
					{
						GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RATE));
						assert(pCombo);
						rate = CProduct::Instance()->LocalDefChnnRateNum();
						pCombo->SetCurItemData(rate, true);

						MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					}

					remainRate = m_frameRate.ModifyRate(row, rate, size);
				}
			}

			char str [128] = {0};
			PRODUCT_TYPE productTypeTemp = CProduct::Instance()->ProductType();
			if ((TD_2324SE_S == productTypeTemp) || (TD_2316SE_S == productTypeTemp) || (TD_2316CDS == productTypeTemp))
			{
				sprintf(str, " : %d (CIF).", remainRate);
			}
			else
			{
	#if defined(__CUSTOM_US02__)
				sprintf(str, " : %d (CIF), %d (D1).", remainRate, remainRate/4);
	#else
				sprintf(str, " : %d (CIF), %d (HD1), %d (D1).", remainRate, remainRate/2, remainRate/4);
	#endif
			}
			m_cfgScRemainNum.SetCaption(str, true);
		}
		else
		{
			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_MAX_BIT_STREAM));
			assert(pCombo);
			unsigned long bitRate = static_cast<VIDEO_SIZE>(pCombo->GetCurItemData());
			
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RATE));
			assert(pCombo);
			rate = static_cast<unsigned char>(pCombo->GetCurItemData());
			
			bool bTryRet = m_ipcFrameEncode.TryEncode(index-localVideoInputNum, rate, size, bitRate);

			if(!bTryRet)
			{
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RATE));
				assert(pCombo);
				unsigned long maxRate = pCombo->GetItemData(pCombo->GetItemCount()-1);
				rate = (rate < maxRate) ? rate : maxRate;
				pCombo->SetCurItemData(rate, true);

				MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			}

			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_MAX_BIT_STREAM));
			assert(pCombo);
			pCombo->SetCurItemData(bitRate, true);

			m_ipcFrameEncode.ModifyEncode(index-localVideoInputNum, rate, size, bitRate);
		}
	}

#ifdef __DVR_PROFESSIONAL__
	//点击码率时，处理数字信号的资源问题
	if(COL_MAX_BIT_STREAM == col)
	{
		int index = m_rowChnnMap.find(row)->second;
		assert (index < maxChnn);

		if(index >= localVideoInputNum)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RATE));
			assert(pCombo);
			unsigned char rate = static_cast<unsigned char>(pCombo->GetCurItemData());

			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_RESOLUTION));
			assert(pCombo);
			VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pCombo->GetCurItemData());
			
			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_MAX_BIT_STREAM));
			assert(pCombo);
			unsigned long bitRate = static_cast<unsigned long>(pCombo->GetCurItemData());

			bool bTryRet = m_ipcFrameEncode.TryEncode(index-localVideoInputNum, rate, size, bitRate);
			m_ipcFrameEncode.ModifyEncode(index-localVideoInputNum, rate, size, bitRate);

			if(!bTryRet)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				pCombo->SetCurItemData(bitRate, true);
			}
		}
	}

	//当选择定码流时，把画质选择置为不可选状态.
	if (COL_STREAM_TYPE == col)
	{
		//assert (row < CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum);

		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_STREAM_TYPE));
		assert(pCombo);
		unsigned long type = static_cast<unsigned long>(pCombo->GetCurItemData());

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrl.GetItemWnd(row, COL_QUALITY));
		assert(pCombo);

		if (VIDEO_ENCODE_MODE_VBR == type)
		{
			if (!pCombo->IsEnable())
			{
				pCombo->SetEnable(true, true);
			}
		}
		else
		{
			if (pCombo->IsEnable())
			{
				pCombo->SetEnable(false, true);
			}
		}
	}
#endif

	return ret;
}

unsigned long CCfgRecordStreamDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	// 点选非All，先检查是否处于All状态，处于则认为做了修改
	GUI::CCheckCtrl* pCheckAll = NULL;
	if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListCtrlAll.GetItemWnd(0, COL_CHNN));
		assert( pCheckAll );
	}
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCfgDlg );

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	unsigned long ret = KEY_VIRTUAL_MSG_STOP;
	if( (NULL != pCheckAll) && pCheckAll->IsCheck() )
	{
		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			if( (COL_RATE == col)
			  ||(COL_RESOLUTION == col)
			  ||(COL_QUALITY == col)
#ifdef __DVR_PROFESSIONAL__
			  ||(COL_STREAM_TYPE == col)
			  ||(COL_MAX_BIT_STREAM == col)
#endif
			  )
			{
				SetModify(true);
				ret = KEY_VIRTUAL_MSG_CONTINUE;
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
			SetModify(true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}

#ifdef __DVR_PROFESSIONAL__
	if (COL_RESOLUTION == col)
	{
		GUI::CComboCtrl* pComboR = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_RESOLUTION));
		GUI::CComboCtrl* pComboB = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_MAX_BIT_STREAM));

		VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pComboR->GetCurItemData());

		if(localVideoInputNum > 0)
		{
			if (DVRVIDEO_SIZE_CIF == size)
			{
				pComboB->SetCurItemData(768, true);
			}	
			else if (DVRVIDEO_SIZE_D1 == size)
			{
				pComboB->SetCurItemData(1536,true);
			}
			else if (DVRVIDEO_SIZE_HD1 == size)
			{
				pComboB->SetCurItemData(1024, true);
			}
			else if (DVRVIDEO_SIZE_HD1080 == size)
			{
				PRODUCT_TYPE product = CProduct::Instance()->ProductType();
				if ((TD_2704XE_L == product) || (TD_2704XD_M == product) || (TD_2708XD_M == product) || (TD_2716XD_M == product)
					|| (TD_2704XD_P == product) || (TD_2708XD_P == product) || (TD_2708XE_P == product))
				{
					pComboB->SetCurItemData(10240, true);
				}
				else if ((TD_2716XD_P == product) || (TD_2708XE_M == product) || (TD_2716XE_M == product) || (TD_2708XE_MH == product)
					|| (TD_2704AS_SL == product) || (TD_2704AS_SL_A == product) || (TD_2704AS_PL == product) || (TD_2708AS_SL == product) || (TD_2716AS_SL == product) || (TD_2708TS_S == product) || (TD_2716TS_S == product) || (TD_2708TS_PL == product) || (TD_2716TS_PL == product)
					|| (TD_2704TD_PL == product) || (TD_2708TD_PL == product) || (TD_2716TD_PL == product) || (TD_2732TD_PL == product) || (TD_2704TD_P == product) || (TD_2708TD_P == product) || (TD_2716TD_P == product)
					|| (TD_2704TE_M == product) || (TD_2708TE_M == product) || (TD_2716TE_M == product)
					|| (TD_2704AE_SL == product) || (TD_2708AE_SL == product) || (TD_2716AE_SL == product)
					|| (TD_2004AL == product)
					|| (TD_2704AS_SL_A == product) || (TD_2708AS_SL_A == product) || (TD_2716AS_SL_A == product)
					|| (TD_2704AS_PL == product) || (TD_2708AS_PL == product) || (TD_2716AS_PL == product)
					|| (TD_2716AC_PL == product)
					|| (TD_2704AE_PL == product) || (TD_2708AE_PL == product) || (TD_2716AE_PL == product))
				{
					pComboB->SetCurItemData(8192, true);
				}
// 				else if ((TD_2708XE_MH == product))
// 				{
// 					pComboB->SetCurItemData(6144, true);
// 				}
				else
				{
					pComboB->SetCurItemData(12288, true);
				}
			}
			else if (DVRVIDEO_SIZE_HD720 == size)
			{
				PRODUCT_TYPE product = CProduct::Instance()->ProductType();
				if(TD_2704HD == product)
				{
					pComboB->SetCurItemData(12288, true);
				}
				else
				{
					pComboB->SetCurItemData(4096, true);
				}
			}
			else if (DVRVIDEO_SIZE_960H == size)
			{
				PRODUCT_TYPE product = CProduct::Instance()->ProductType();
				if ((TD_2504HD_C == product) || (TD_2508HD_C == product) || (TD_2516HD_C == product) || (TD_2524HD_C == product) || (TD_2532HD_C == product) 
					|| (TD_2604LE == product)|| (TD_2608LE == product) || (TD_2616LE == product) || (TD_2508HE_C_3521 == product) || (TD_2524HE_C == product) || (TD_2532HE_C == product)
					|| (TD_2304ME_C == product) || (TD_2308ME_C == product) || (TD_2316ME_C == product) || (TD_2616LD == product)  
					|| (TD_2304SS_C == product) || (TD_2308SS_C == product) || (TD_2316SS_C == product)
					|| (TD_2304SL_C == product) || (TD_2308SL_C == product)
					|| (TD_2304SE_C == product) || (TD_2308SE_C == product) || (TD_2316SE_C == product) || (TD_2304SS_SDI == product) || (TD_2308SS_SDI == product)||(TD_2304SS_SL_3515A == product)
					|| (TD_2508HE_SDI == product) || (TD_2516HS_BL == product) || (TD_2704AS_SL == product) || (TD_2704AS_SL_A == product) || (TD_2704AS_PL == product) || (TD_2704AS_S == product) || (TD_2708AS_S == product) || (TD_2708AS_S_A == product)
					|| (TD_2004AL == product) || (TD_2008AS == product)
					|| (TD_2704AS_S_A == product) || (TD_2708AS_S_A == product)
					|| (TD_2704AS_SL_A == product))
				{
					pComboB->SetCurItemData(2048,true);
				}
				else
				{
					pComboB->SetCurItemData(1792,true);
				}
			}
		}
		else
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_RESOLUTION));
			assert(pCombo);
			VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pCombo->GetCurItemData());

			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row,COL_MAX_BIT_STREAM));
			AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo, size);
		}
	}
#endif

	//2010-03-02 11:00:00 YSW
	//只有点击帧率和分辨率时，才处理资源部分
	if ((COL_RATE == col) || (COL_RESOLUTION == col))
	{
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_RATE));
		assert(pCombo);
		unsigned char rate = static_cast<unsigned char>(pCombo->GetCurItemData());

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_RESOLUTION));
		assert(pCombo);
		VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pCombo->GetCurItemData());

		unsigned long maxRate;
		if(localVideoInputNum > 0)
		{
			maxRate = RealRate(m_totalRate/m_channelNum, size);
			if (m_bVideoSizeSpecial)
			{
				maxRate = RealRate(m_maxChnnRate, size);
			}
		}
		else
		{
			maxRate = m_ipcFrameEncode.GetMaxFrameRate(size)/netVideoInputNum;
		}

		if (maxRate < rate)
		{
			rate = maxRate;
			MessageBox(m_pStrTable->String(E_STR_ID_TIP_03_02), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		}

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_RATE));
		assert(pCombo);
		pCombo->SetCurItemData(rate, true);

#ifdef __DVR_PROFESSIONAL__	
		if(0 == localVideoInputNum)
		{
			unsigned long bitRate = m_ipcFrameEncode.GetSuggestBitRate(size, rate);
			GUI::CComboCtrl* pComboB = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_MAX_BIT_STREAM));
			pComboB->SetCurItemData(bitRate, true);
		}
#endif
	}

	//不管是选中全选状态还是不选状态，都需要做处理。
	//当选择定码流时，把画质选择置为不可选状态.
#ifdef __DVR_PROFESSIONAL__	
	if (COL_STREAM_TYPE == col)
	{
		//assert (row < CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum);

		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_STREAM_TYPE));
		assert(pCombo);
		unsigned long type = static_cast<unsigned long>(pCombo->GetCurItemData());

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_QUALITY));
		assert(pCombo);

		if (VIDEO_ENCODE_MODE_VBR == type)
		{
			if (!pCombo->IsEnable())
			{
				pCombo->SetEnable(true, true);
			}
		}
		else
		{
			if (pCombo->IsEnable())
			{
				pCombo->SetEnable(false, true);
			}
		}
	}

	if (COL_MAX_BIT_STREAM == col)
	{
		if(0 == localVideoInputNum)
		{
			GUI::CComboCtrl* pComboB = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_MAX_BIT_STREAM));
			unsigned long curBitRate = pComboB->GetCurItemData();
			unsigned long maxBitRate = m_ipcSupportEncode.maxTotalBitRate/netVideoInputNum;
			if(curBitRate > maxBitRate)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_TIP_03_02), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				pComboB->SetCurItemData(maxBitRate, true);
			}
		}
	}

#endif

	return ret;
}


void CCfgRecordStreamDlg::AddResolutionItem(unsigned long dwResolution, CComboCtrl *pCombo,bool bAddD1)
{
	assert(dwResolution != 0);

	unsigned long just_Cif_D1_H960 = 0;
	unsigned long in_Cif_D1_H960 = 0;
	just_Cif_D1_H960 = DVRVIDEO_SIZE_CIF + DVRVIDEO_SIZE_D1 + DVRVIDEO_SIZE_960H;
	in_Cif_D1_H960 = dwResolution & just_Cif_D1_H960;
	bool b_showDecoration = false;
	if(!(g_p_login_succ->productInfo.netVideoInputNum > 0) &&
		in_Cif_D1_H960 &&
		((dwResolution & ~just_Cif_D1_H960) == 0) &&
		in_Cif_D1_H960 != DVRVIDEO_SIZE_CIF &&
		in_Cif_D1_H960 != DVRVIDEO_SIZE_D1 &&
		in_Cif_D1_H960 != DVRVIDEO_SIZE_960H
	   )
	{
		b_showDecoration = true;
	}

	if (dwResolution & DVRVIDEO_SIZE_QCIF)
	{
		pCombo->AddItem("QCIF", DVRVIDEO_SIZE_QCIF);
	}
	
	if (dwResolution & DVRVIDEO_SIZE_CIF)
	{
		#ifdef   __CUSTOM_US02_NEW__
			if(b_showDecoration)
			{
				pCombo->AddItem(m_pStrTable->String(E_STR_ID_RECORD_QUALITY_GOOD) /*"Good (CIF)"*/, DVRVIDEO_SIZE_CIF);				
			}
			else
			{
				pCombo->AddItem("CIF", DVRVIDEO_SIZE_CIF);
			}
		#else
			pCombo->AddItem("CIF", DVRVIDEO_SIZE_CIF);
		#endif
	}
	
	if (dwResolution & DVRVIDEO_SIZE_HD1)
	{
		pCombo->AddItem("HD1", DVRVIDEO_SIZE_HD1);
	}
	
	if (dwResolution & DVRVIDEO_SIZE_D1)
	{
		if (bAddD1)
		{
			#ifdef   __CUSTOM_US02_NEW__
				if(b_showDecoration)
				{
					pCombo->AddItem(m_pStrTable->String(E_STR_ID_RECORD_QUALITY_BETTER) /*"Better (D1)"*/, DVRVIDEO_SIZE_D1);
				}
				else
				{
					pCombo->AddItem("D1", DVRVIDEO_SIZE_D1);
				}
			#else
				pCombo->AddItem("D1", DVRVIDEO_SIZE_D1);
			#endif
		}				
	}			

#if defined(__CUSTOM_IL03__) || defined(__CUSTOM_US02__)
	if (dwResolution & DVRVIDEO_SIZE_960H)
	{
		#ifdef   __CUSTOM_US02_NEW__
			if(b_showDecoration)
			{
				pCombo->AddItem(m_pStrTable->String(E_STR_ID_RECORD_QUALITY_BEST) /*"Best (960H)"*/, DVRVIDEO_SIZE_960H);
			}
			else
			{
				pCombo->AddItem("960H", DVRVIDEO_SIZE_960H);
			}
		#else
			pCombo->AddItem("960H", DVRVIDEO_SIZE_960H);							
		#endif
	}
#else
	if (dwResolution & DVRVIDEO_SIZE_960H)
	{
		pCombo->AddItem("WD1", DVRVIDEO_SIZE_960H);							
	}
#endif

	if (dwResolution & DVRVIDEO_SIZE_VGA)
	{
		pCombo->AddItem("VGA", DVRVIDEO_SIZE_VGA);
	}

	if (dwResolution & DVRVIDEO_SIZE_HD720)
	{
		pCombo->AddItem("720P", DVRVIDEO_SIZE_HD720);
	}

	if (dwResolution & DVRVIDEO_SIZE_960P)
	{
		pCombo->AddItem("960P", DVRVIDEO_SIZE_960P);
	}

	if (dwResolution & DVRVIDEO_SIZE_SXGA)
	{
		pCombo->AddItem("1.3M", DVRVIDEO_SIZE_SXGA);
	}

	if (dwResolution & DVRVIDEO_SIZE_HD1080)
	{
		pCombo->AddItem("1080P", DVRVIDEO_SIZE_HD1080);
	}

	if (dwResolution & DVRVIDEO_SIZE_3M)
	{
		pCombo->AddItem("3M", DVRVIDEO_SIZE_3M);
	}

	if (dwResolution & DVRVIDEO_SIZE_16_9_3M)
	{
		pCombo->AddItem("2304X1296", DVRVIDEO_SIZE_16_9_3M);
	}

	if (dwResolution & DVRVIDEO_SIZE_4M)
	{
		pCombo->AddItem("2688X1520", DVRVIDEO_SIZE_4M);
	}

	if (dwResolution & DVRVIDEO_SIZE_5M)
	{
		pCombo->AddItem("2592X1944", DVRVIDEO_SIZE_5M);
	}
}

void CCfgRecordStreamDlg::AddQualityItem(unsigned long *pQualityValue, unsigned long count, CComboCtrl *pCombo)
{
	char str[128] = {0};

	for (int i = 0; i < count; ++i)
	{
		if (IMAGE_QUALITY_LOWEST == pQualityValue[i] )
		{
			snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_LOWEST ) );
			pCombo->AddItem( str, pQualityValue[i]);
		}
		else if (IMAGE_QUALITY_LOWER == pQualityValue[i])
		{
			snprintf( str, 128, "%s",m_pStrTable->String( E_STR_ID_LOWER ) );
			pCombo->AddItem( str,  pQualityValue[i]);
		}
		else if (IMAGE_QUALITY_LOW == pQualityValue[i])
		{
			snprintf( str, 128, "%s",m_pStrTable->String( E_STR_ID_LOW ) );
			pCombo->AddItem(str, pQualityValue[i]);
		}
		else if (IMAGE_QUALITY_MEDIUM == pQualityValue[i])
		{
			snprintf( str, 128, "%s",m_pStrTable->String( E_STR_ID_MEDIUM ) );
			pCombo->AddItem( str, pQualityValue[i] );
		}
		else if (IMAGE_QUALITY_HEIGHTER == pQualityValue[i])
		{
			snprintf( str, 128, "%s",m_pStrTable->String( E_STR_ID_HIGHER ) );
			pCombo->AddItem( str, pQualityValue[i] );
		}
		else if (IMAGE_QUALITY_HEIGHTEST == pQualityValue[i])
		{
			snprintf( str, 128, "%s",m_pStrTable->String( E_STR_ID_HIGHEST ) );
			pCombo->AddItem( str, pQualityValue[i] );
		}	
		else 
		{
			assert(false);
		}
	}
}

void CCfgRecordStreamDlg::AddEncodeModeItem(unsigned long videoEncodeMode, CComboCtrl *pCombo)
{
	if (videoEncodeMode & VIDEO_ENCODE_MODE_VBR)
	{
		pCombo->AddItem(m_pStrTable->String(E_STR_ID_REC_VBR),VIDEO_ENCODE_MODE_VBR);
	}
	if (videoEncodeMode & VIDEO_ENCODE_MODE_CBR) 
	{
		pCombo->AddItem(m_pStrTable->String(E_STR_ID_REC_CBR),VIDEO_ENCODE_MODE_CBR);
	}	
}

void CCfgRecordStreamDlg::AddBitStreamItem(unsigned long *pBitValue, unsigned long count, CComboCtrl *pCombo, unsigned long resolution, int channel)
{
	assert(pBitValue != NULL);
	assert(count > 0);
	char str[128] = {0};

	unsigned long maxBitRate = 0;
	unsigned long minBitRate = 0;

	if(IsMixDvr())
	{
		if(0 != (resolution & DVRVIDEO_SIZE_5M))
		{
			maxBitRate = 12*1024;
			minBitRate = 512;
		}
		else if(0 != (resolution & DVRVIDEO_SIZE_4M))
		{
			maxBitRate = 12*1024;
			minBitRate = 512;
		}
		else if(0 != (resolution & DVRVIDEO_SIZE_16_9_3M))
		{
			maxBitRate = 12*1024;
			minBitRate = 512;
		}
		else if(0 != (resolution & DVRVIDEO_SIZE_3M))
		{
			maxBitRate = 12*1024;
			minBitRate = 512;
		}
		else if(0 != (resolution & DVRVIDEO_SIZE_HD1080))
		{
			maxBitRate = 12*1024;
			minBitRate = 512;
		}
		else if(0 != (resolution & DVRVIDEO_SIZE_960P))
		{
			maxBitRate = 6*1024;
			minBitRate = 512;
		}
		else if(0 != (resolution & DVRVIDEO_SIZE_HD720))
		{
			maxBitRate = 6*1024;
			minBitRate = 512;
		}
		else if(0 != (resolution & DVRVIDEO_SIZE_960H))
		{
			maxBitRate = 2*1024 + 512;
			minBitRate = 512;
		}
		else if(0 != (resolution & DVRVIDEO_SIZE_D1))
		{
			maxBitRate = 2* 1024;
		}
		else if(0 != (resolution & DVRVIDEO_SIZE_VGA))
		{
			maxBitRate = 1.5* 1024;
		}
		else if(0 != (resolution & DVRVIDEO_SIZE_CIF))
		{
			maxBitRate = 1.5* 1024;
		}
		else if(0 != (resolution & DVRVIDEO_SIZE_HD1))
		{
			maxBitRate = 1.5* 1024;
		}
		else
		{
			minBitRate = 0;
			maxBitRate = 0;
		}
	}
	else
	{
		switch(resolution)
		{
			case DVRVIDEO_SIZE_HD720:
				maxBitRate = 6*1024;
				break;
			case DVRVIDEO_SIZE_HD1080:
				maxBitRate = 12*1024;
				break;
			case DVRVIDEO_SIZE_960P:
				maxBitRate = 6*1024;
				break;
			case DVRVIDEO_SIZE_SXGA:
				maxBitRate = 6*1024;
				break;
			case DVRVIDEO_SIZE_3M:
				maxBitRate = 12*1024;
				break;
			case DVRVIDEO_SIZE_16_9_3M:
				maxBitRate = 12*1024;
				break;
			case DVRVIDEO_SIZE_4M:
				maxBitRate = 12*1024;
				break;
			case DVRVIDEO_SIZE_5M:
				maxBitRate = 12*1024;
				break;
			case DVRVIDEO_SIZE_D1:
				maxBitRate = 2*1024;
				break;
			case DVRVIDEO_SIZE_960H:
				maxBitRate = 2560;
				break;
			case DVRVIDEO_SIZE_VGA:
				maxBitRate = 1536;
				break;
			case DVRVIDEO_SIZE_HD1:
				maxBitRate = 1536;
				break;
			case DVRVIDEO_SIZE_CIF:
				maxBitRate = 1024;
				break;
			default:
				maxBitRate = 2*1024;
				break;
		}

		if (-1 < channel && channel <= g_p_login_succ->productInfo.netVideoInputNum + g_p_login_succ->productInfo.localVideoInputNum)
		{
			for (int j=0; j<32; j++)
			{
				if (resolution == m_netChennelEncInfo[channel - g_p_login_succ->productInfo.localVideoInputNum].encInfo[j].videoSize)
				{
					if (0 < m_netChennelEncInfo[channel - g_p_login_succ->productInfo.localVideoInputNum].encInfo[j].maxBitRate)
					{
						maxBitRate = m_netChennelEncInfo[channel - g_p_login_succ->productInfo.localVideoInputNum].encInfo[j].maxBitRate;
					}
				}
			}
		}
	}


	pCombo->RemoveAllItem(false);
	for (int i = 0;i < count; ++i)
	{
#if defined(__DVR_ULTIMATE__)
		if(pBitValue[i] > maxBitRate)
		{
			break;
		}
#else
		if(IsMixDvr())
		{
			if((0 != minBitRate) && (pBitValue[i] < minBitRate))
			{
				continue;
			}
		}
		
		if((0 != maxBitRate) && (pBitValue[i] > maxBitRate))
		{
			break;
		}
#endif
		if(0 == pBitValue[i])
		{
			continue;
		}
		
		sprintf(str, "%d kbps", pBitValue[i]);
		pCombo->AddItem(str, pBitValue[i]);
	}
	pCombo->SetDropDownCount(7);
	
}

void CCfgRecordStreamDlg::AddMIxBitStreamItem(CComboCtrl *pCombo, unsigned long resolution)
{
	char str[128] = {0};
	int rowCount = 0;
	int vsrow = 0;
	bool bfind =false;
	
	for(int index=0;index<g_resolutionNum;++index)
	{
		if(0 !=(resolution & m_pVSBit[index].resolution))
		{
			vsrow = index;
			bfind =true;
			break;
		}
	}
	if(!bfind)
	{
		return;
	}
	pCombo->RemoveAllItem(false);
	//计算总共有对应分辨率有几个对应的码流
	for(int i=0;i<20;++i)
	{
		if(0 != m_pVSBit[vsrow].vsforbrr[i])
		{
			++rowCount;
			sprintf(str, "%d kbps",  m_pVSBit[vsrow].vsforbrr[i]);
			pCombo->AddItem(str,  m_pVSBit[vsrow].vsforbrr[i]);
		}
		else//最后一个项目是0
		{
			break;
		}
	}
	pCombo->SetDropDownCount(7);
}
bool CCfgRecordStreamDlg::IsInQualityLevel(unsigned long value)
{
	for (int i = 0; i < m_qualityLevelNum; ++i)
	{
		if ((m_pQualityLevelValue[i] == value) && (value != 0))
		{
			return true;
		}
	}
	return false;
}


bool CCfgRecordStreamDlg::IsInBitStreamRange(unsigned long value)
{
	for (int i = 0; i< m_bitStreamCount; ++i)
	{
		if (m_pBitStreamValue[i] == value)
		{
			return true;
		}
	}
	return false;
}

bool CCfgRecordStreamDlg::IsMixDvr()
{
	PRODUCT_TYPE productTmp = CProduct::Instance()->ProductType();
	switch(productTmp)
	{
		case TD_2304SS_SDI:
		case TD_2308SS_SDI:
		case TD_2508HE_SDI:
		case TD_2512HE_SDI:
		case TD_2524HE_SDI:
			return true;
			break;
		default:
			break;
	}
	return false;
}
