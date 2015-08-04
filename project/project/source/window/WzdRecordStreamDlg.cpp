/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2010-07-20
** Name         : CWzdRecordStreamDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
作用:录像码流的向导
***********************************************************************/
#include "WzdPageDlg.h"
#include "WzdRecordStreamDlg.h"
#include "CfgDlg.h"
#include "NetConfigDefine.h"
#include "FuncCustom.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
extern unsigned long VideoSizeToDDVideoSize(unsigned long videoSize);
extern const unsigned long g_resolutionNum;
CWzdRecordStreamDlg::CWzdRecordStreamDlg() : m_pWzdInfo(NULL),m_bUsed(true), m_pNetDevInfo(NULL), m_netDevNum(0)
{
	memset(m_supportEncInfo, 0, sizeof(SUPPORT_ENC_INFO));
	memset(m_checkEncInfo, 0, sizeof(CHECK_ENC_INFO));
}

CWzdRecordStreamDlg::~CWzdRecordStreamDlg()
{

}

void CWzdRecordStreamDlg::SetUsed(bool bUsed)
{
	m_bUsed = bUsed;
}

bool CWzdRecordStreamDlg::IsUsed()
{
	return m_bUsed;
}

void CWzdRecordStreamDlg::SetWzdInfo(WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;

	for (int i=0; i<32; i++)
	{
		m_checkEncInfo[i].ulRes = pWzdInfo->InfoEx[i].resolution;
		m_checkEncInfo[i].usBitrate = pWzdInfo->InfoEx[i].hBitStream;
		m_checkEncInfo[i].usRate = pWzdInfo->InfoEx[i].rate;
	}
}

void CWzdRecordStreamDlg::OnInitial()
{
	GUI::CListCtrl& list = m_cfgList;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CCheckCtrl* pCheck = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[LIST_ITEM_HEIGHT] - m_length[DLG_OFFSET_SUB_BOTTOM];

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;
	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);
	m_channelNum = localVideoInputNum + netVideoInputNum;

	if (0 == localVideoInputNum)
	{//NVR
		height += m_length[LIST_ITEM_HEIGHT];
	}

	int row = 0;
	char str[128] = {0};
	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头
	for(int i = 0; i < m_channelNum; i++)
	{
		if((CH >> i) & 0x1)
		{
			m_rowChnnMap.insert(std::make_pair(row++, i));
		}
	}

	if( effectChnn > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}
	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );

#ifdef __DVR_PROFESSIONAL__
	list.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 5 / 32 -10 );
	list.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 5 / 32+10);
	list.AddColumn(m_pStrTable->String(E_STR_ID_REC_ENCODE_TYPE), width * 6 / 32 - 8);
	list.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 5 / 32 + 8 );
	list.AddColumn(m_pStrTable->String(E_STR_ID_REC_MAX_BIT_STREAM), width * 8 / 32 - offsetEdge);
#else
	list.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 9 / 32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 9 / 32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 11 / 32 - offsetEdge);
#endif

	for ( int i = 0; i < effectChnn; i++ )
	{
		int chnnIndex = m_rowChnnMap.find(i)->second;
		sprintf(str,"%d",chnnIndex+1);
		list.AddItem(i,COL_CHNN,str);

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_RATE, WND_TYPE_COMBO));
		assert(pCombo);		

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_RESOLUTION, WND_TYPE_COMBO));
		assert(pCombo);
		
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_QUALITY, WND_TYPE_COMBO));
		assert(pCombo);

#ifdef __DVR_PROFESSIONAL__	
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i,COL_STREAM_TYPE,WND_TYPE_COMBO));
		assert(pCombo);

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i,COL_MAX_BIT_STREAM,WND_TYPE_COMBO));
		assert(pCombo);
#endif
	}
	list.CreateListFocusMap();

	GUI::CRect rect = list.GetWindowRect();
	y += rect.Height() + 4;

	if (localVideoInputNum > 0)
	{
		m_cfgScAll.Create( GUI::CWnd::GetRandWndID(), x, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, width / 2, m_length[STATIC_HEIGHT], this );
		strcpy( str, m_pStrTable->String( E_STR_ID_ALL ) );
		m_cfgScAll.SetCaption( str, false );
		x += 100;
		m_cfgScRemaining.Create( GUI::CWnd::GetRandWndID(), x, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, width / 2, m_length[STATIC_HEIGHT], this );
		strcpy( str, m_pStrTable->String( E_STR_ID_REMAINING ) );
		m_cfgScRemaining.SetCaption(str, false );
		unsigned short cx = 0, cy = 0;
		TextSize(str, strlen(str), cx, cy);
		x += cx;
		m_cfgScRemainNum.Create( GUI::CWnd::GetRandWndID(), x+4, y+( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, width / 2, m_length[STATIC_HEIGHT], this);
		PRODUCT_TYPE productTypeTemp = CProduct::Instance()->ProductType();
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
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 5 / 32 -10 );
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 5 / 32+10);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_REC_ENCODE_TYPE), width * 6 / 32 - 8);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 5 / 32 + 8 );
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_REC_MAX_BIT_STREAM), width * 8/ 32 - offsetEdge);
#else
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 9 / 32 );
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 9 / 32);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 11 / 32 - offsetEdge);
#endif

		{
			int i = 0;

			pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem(i,COL_CHNN,WND_TYPE_CHECK));
			assert(pCheck);

			PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
			if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
				|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
				|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
				|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
				|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AS_SL == productID)
				|| (TD_2004AL == productID)
				|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
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

			if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
				|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
				|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
				|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
				|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AS_SL == productID)
				|| (TD_2004AL == productID)
				|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
				|| (TD_2716AC_PL == productID)
				|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
			{
				pCombo->Show(false,false);
			}


			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem( i, COL_RATE, WND_TYPE_COMBO));
			assert(pCombo);	
			if (!IsSupportRecordResChange())
			{
				pCombo->SetEnable(false, false);
			}

			if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
				|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
				|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
				|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
				|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AS_SL == productID)
				|| (TD_2004AL == productID)
				|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
				|| (TD_2716AC_PL == productID)
				|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
			{
				pCombo->Show(false,false);
			}


#ifdef __DVR_PROFESSIONAL__	
			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem(i,COL_STREAM_TYPE,WND_TYPE_COMBO));
			assert(pCombo);

			if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
				|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
				|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
				|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
				|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AS_SL == productID)
				|| (TD_2004AL == productID)
				|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
				|| (TD_2716AC_PL == productID)
				|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
			{
				pCombo->Show(false,false);
			}

#endif
			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem( i, COL_QUALITY, WND_TYPE_COMBO));
			assert(pCombo);
			if (!IsSupportRecordResChange())
			{
				pCombo->SetEnable(false, false);
			}

			if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
				|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
				|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
				|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
				|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AS_SL == productID)
				|| (TD_2004AL == productID)
				|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
				|| (TD_2716AC_PL == productID)
				|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
			{
				pCombo->Show(false,false);
			}


#ifdef __DVR_PROFESSIONAL__	
			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem(i,COL_MAX_BIT_STREAM,WND_TYPE_COMBO));
			assert(pCombo);	

			if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
				|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
				|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
				|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
				|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AS_SL == productID)
				|| (TD_2004AL == productID)
				|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
				|| (TD_2716AC_PL == productID)
				|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
			{
				pCombo->Show(false,false);
			}

#endif
		}

		if (!IsSupportRecordResChange())
		{
			for (int i = 0; i < effectChnn; i++)
			{
				pCombo = NULL;
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(i, COL_RESOLUTION));
				pCombo->SetEnable(false, false);

				assert(pCombo);
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(i, COL_RATE));
				pCombo->SetEnable(false, false);

#ifdef __DVR_PROFESSIONAL__
				assert(pCombo);
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(i, COL_STREAM_TYPE));
#endif

				assert(pCombo);
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(i, COL_QUALITY));
				pCombo->SetEnable(false, false);

#ifdef __DVR_PROFESSIONAL__
				assert(pCombo);
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(i, COL_MAX_BIT_STREAM));
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
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(i, COL_RESOLUTION));
				pCombo->SetEnable(false, false);
			}
		}

		PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
	    if ((TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2708AS_S_A == productID) || (TD_2004AL == productID) || (TD_2008AS == productID))
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
	      ||(product == TD_2604LD)||(product == TD_2608LD)||(product == TD_2616LD) 
		  ||(product == TD_2504HD_C) ||(product == TD_2508HD_C) || (product == TD_2516HD_C) ||(product == TD_2524HD_C) ||(product == TD_2532HD_C)
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

		listAll.CreateListFocusMap();

		if (IsMixDvr())
		{
			listAll.Show(false,false);
		}
	}

	PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
	
	if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
		|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
		|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
		|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
		|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AS_SL == productID)
		|| (TD_2004AL == productID)
		|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
		|| (TD_2704AS_SL_A == productID) || (TD_2708AS_SL_A == productID) || (TD_2716AS_SL_A == productID)
		|| (TD_2304SS_C == productID) || (TD_2308SS_C == productID) || (TD_2316SS_C == productID) || (TD_2616LE == productID)
		|| (TD_2508HE_B_3521 == productID) || (TD_2524HE_C == productID) || (TD_2532HE_C == productID)
		|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
		|| (TD_2716AC_PL == productID)
		|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
//	{
// 	if ((TD_2704HD == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
// 		|| (TD_2304SS_C == productID) || (TD_2308SS_C == productID) || (TD_2316SS_C == productID) || (TD_2616LE == productID) 
// 		|| (TD_2508HE_B_3521 == productID) || (TD_2524HE_C == productID) || (TD_2532HE_C == productID))
	{
		m_cfgListCtrlAll.Show(false,false);
	}


	if(g_p_login_succ->productInfo.netVideoInputNum > 0)
	{
		MESSAGE_DATA msgData;
		NCFG_ITEM_HEAD cfgItemHead;
		cfgItemHead.itemID = NCFG_ITEM_IPC_SUPPORT_ENCODE;
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_GET, (unsigned char *)&cfgItemHead, sizeof(NCFG_ITEM_HEAD), msgData);

		memcpy(&m_ipcSupportEncode, msgData.pData + sizeof(PACKCMD) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD), sizeof(NCFG_IPC_SUPPORT_ENCODE));

		if(VIDEO_FORMAT_NTSC == g_p_login_succ->productInfo.videoFormat)
		{
#ifndef __CHIP3520__
			m_ipcSupportEncode.defaChnnRate = m_ipcSupportEncode.defaChnnRate * 6 / 5;
			m_ipcSupportEncode.maxChnnRate = m_ipcSupportEncode.maxChnnRate * 6 / 5;
			m_ipcSupportEncode.maxTotalRateOnVGA = m_ipcSupportEncode.maxTotalRateOnVGA * 6 / 5;
			m_ipcSupportEncode.maxChnnRateOnVGA = m_ipcSupportEncode.maxChnnRateOnVGA * 6 / 5;
#endif
		}

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}
	else
	{
		memset(&m_ipcSupportEncode, 0, sizeof(NCFG_IPC_SUPPORT_ENCODE));
	}
}

void CWzdRecordStreamDlg::OnPostInitial()
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;
	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	if ((0 < localVideoInputNum) && IsSupportFunction(FUNC_SERVER_ENCODE_CHECK))
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));

		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_FIRST_SUPPORT_INFO, NULL, 0, msgData);

		assert(msgData.dataLen >= sizeof(SUPPORT_ENC_INFO) * localVideoInputNum);

		memcpy(m_supportEncInfo, msgData.pData + sizeof(PACKCMD), msgData.dataLen);

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}
}

void CWzdRecordStreamDlg::InitCtrlInfo()
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	int channelNum = localVideoInputNum + netVideoInputNum;

	m_maxChnnRate = 0;
	m_totalRate = 0;
	m_defauResolution = 0;
	m_defauChnnRateNum = 0;
	unsigned long m_videoFormat = g_p_login_succ->productInfo.videoFormat; 

	for (int i=0; i<localVideoInputNum; i++)
	{
		m_checkEncInfo[i].ulRes = m_pWzdInfo->InfoEx[i].resolution;
		m_checkEncInfo[i].usBitrate = m_pWzdInfo->InfoEx[i].hBitStream;
		m_checkEncInfo[i].usRate = m_pWzdInfo->InfoEx[i].rate;
	}

	if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
	{
		m_maxChnnRate = m_pWzdInfo->pFrameRateData->chnnRatePalOnCif;
		m_totalRate = m_pWzdInfo->pFrameRateData->totalRatePalOnCif;
		m_defauChnnRateNum = m_pWzdInfo->pFrameRateData->defauChnnRateNumPal;
		
	}
	else if (VIDEO_FORMAT_NTSC == (VIDEO_FORMAT)m_videoFormat)
	{
		m_maxChnnRate =  m_pWzdInfo->pFrameRateData->chnnRateNtscOnCif;
		m_totalRate =  m_pWzdInfo->pFrameRateData->totalRateNtscOnCif;
		m_defauChnnRateNum =  m_pWzdInfo->pFrameRateData->defauChnnRateNumNtsc;
	}
	else
	{
		assert(false);
		m_maxChnnRate = 25;
		m_totalRate = m_maxChnnRate*channelNum;
		m_defauChnnRateNum = 25;
	}
	assert(m_maxChnnRate > 0);

	if (!IsMixDvr())
	{
		m_videoResolutionMask = m_pWzdInfo->pFrameRateData->dwResolution;
		m_defauResolution = m_pWzdInfo->pFrameRateData->defaResolution;
	}
	
	if (netVideoInputNum > 0)
	{
		MESSAGE_DATA msgData;
		ND_DEV_INFO *pNetDevInfo = NULL;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_NET_DEVICE_INFO, NULL, 0, msgData);
		pNetDevInfo = (ND_DEV_INFO *)(msgData.pData + sizeof(PACKCMD));
		m_netDevNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(ND_DEV_INFO);
		if (m_netDevNum > 0)
		{
			m_pNetDevInfo = new ND_DEV_INFO[m_netDevNum];
			memcpy(m_pNetDevInfo, pNetDevInfo, m_netDevNum*sizeof(ND_DEV_INFO));	
		}
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}

	//设置数据
	{
		bool bRefresh = false;
		GUI::CListCtrl& list = m_cfgList;
		char str[128] = {0};

		//2010-02-04 19:27:00 YSW
		//这里不能使用默认分辨率来定可选帧率，加入默认为D1(6Rate)，则会在下拉列表中插入1-6.
		//当把分辨率改为CIF（此时可支持帧率到25）,想设置分辨率为25，可是下拉框中不能选择。
		//导致不能灵活配置，所以应该直接使用最大数（需要比较不同制式最大数）。
		//int rate = RealRate(m_maxChnnRate, (VIDEO_SIZE)m_defauResolution);
		int rate = 0;
		int * pRateArr = NULL;
		if (IsMixDvr())
		{
			pRateArr = new int[m_channelNum];
			for(int rateIndex=0;rateIndex<m_channelNum;++rateIndex)
			{
				pRateArr[rateIndex] = RealRate(m_maxChnnRate,(VIDEO_SIZE)m_pWzdInfo->pChRecp[rateIndex].dfVS);
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
		unsigned long *pSupportVideoSize = CProduct::Instance()->GetDigitChannelSupportVideoSize();

		{
			PRODUCT_TYPE productTemp = CProduct::Instance()->ProductType();
			if ((TD_2704XE_L == productTemp) || (TD_2708XE_L == productTemp) || (TD_2716XE_L == productTemp)
				||(TD_2704XD_L == productTemp) || (TD_2708XD_L == productTemp) || (TD_2716XD_L == productTemp)
				||(TD_2704XD_M == productTemp) || (TD_2708XD_M == productTemp) || (TD_2716XD_M == productTemp)
				||(TD_2704XD_P == productTemp) || (TD_2708XD_P == productTemp) || (TD_2716XD_P == productTemp)
				||(TD_2708XE_M == productTemp) ||(TD_2716XE_M == productTemp)  ||(TD_2708XE_MH == productTemp)
				||(TD_2708XE_P == productTemp)
				||(TD_2704XE_S == productTemp) || (TD_2708XE_S == productTemp) || (TD_2716XE_S == productTemp)
				||(TD_2704XS_SL == productTemp) || (TD_2708XS_SL == productTemp) || (TD_2716XS_SL == productTemp)
				||(TD_2704AS_SL == productTemp) || (TD_2704AS_SL_A == productTemp) || (TD_2704AS_PL == productTemp) || (TD_2708AS_SL == productTemp) || (TD_2716AS_SL == productTemp)
				||(TD_2704AS_S == productTemp) || (TD_2708AS_S == productTemp) || (TD_2708AS_S_A == productTemp) || (TD_2716AS_S == productTemp)
				||(TD_2704TS_S == productTemp) || (TD_2708TS_S == productTemp) || (TD_2716TS_S == productTemp)
				||(TD_2704TS_PL == productTemp) || (TD_2708TS_PL == productTemp) || (TD_2716TS_PL == productTemp)
				|| (TD_2704TD_PL == productTemp) || (TD_2708TD_PL == productTemp) || (TD_2716TD_PL == productTemp) || (TD_2732TD_PL == productTemp)
				|| (TD_2704TD_P == productTemp) || (TD_2708TD_P == productTemp) || (TD_2716TD_P == productTemp)
				|| (TD_2704XD_S == productTemp) || (TD_2708XD_S == productTemp) || (TD_2716XD_S == productTemp)
				|| (TD_2704TE_M == productTemp) || (TD_2708TE_M == productTemp) || (TD_2716TE_M == productTemp)
				|| (TD_2704AE_SL == productTemp) || (TD_2708AE_SL == productTemp) || (TD_2716AS_SL == productTemp)
				|| (TD_2004AL == productTemp) || (TD_2008AS == productTemp)
				|| (TD_2704AS_PL == productTemp) || (TD_2708AS_PL == productTemp) || (TD_2716AS_PL == productTemp)
				|| (TD_2716AC_PL == productTemp)
				|| (TD_2704AE_PL == productTemp) || (TD_2708AE_PL == productTemp) || (TD_2716AE_PL == productTemp)
				)
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
			
			//Rate列
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RATE));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			int rateTemp1 = CProduct::Instance()->IPCMaxChnnRate();
			if(VIDEO_FORMAT_NTSC == g_video_format)
			{
				rateTemp1 = (rateTemp1 + 1) * 6 / 5;
				if (30 > rateTemp1)
				{
					rateTemp1 = 30;
				}
			}
			if(index >= localVideoInputNum)
			{
				rate = (rate < rateTemp1) ? rate : rateTemp1;
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
			
			//Resolution列
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			
			unsigned long supportVideoSize = (NULL == pSupportVideoSize) ? 0 : pSupportVideoSize[index];

			if (index < localVideoInputNum)
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
					AddResolutionItem(m_pWzdInfo->pChRecp[i].supVS,pCombo,true);
				}
				else
				{
					AddResolutionItem(m_videoResolutionMask, pCombo,true);
				}
			}
			else
			{//IPC
				assert(m_pNetDevInfo);
				int j=0;
				for (; j<m_netDevNum; j++)
				{
					if (m_pNetDevInfo[j].chnn == index)
					{
						break;
					}
				}
				if (j<m_netDevNum)
				{
					AddResolutionItem(m_pNetDevInfo[j].videoSize&supportVideoSize, pCombo, true);
				}
				else
				{
					AddResolutionItem(supportVideoSize, pCombo, true);
					pCombo->SetCurItem(0);
				}
			}
			if (m_pWzdInfo->bAllCH)
			{
				pCombo->SetEnable(false, false);
			}

			//Quality列
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_QUALITY));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			AddQualityItem(m_pWzdInfo->QualityLevelValue, m_pWzdInfo->QualityLevelNum, pCombo);
#ifdef __DVR_PROFESSIONAL__	
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_STREAM_TYPE));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			AddEncodeModeItem(m_pWzdInfo->videoEncodeMode, pCombo);
			{
				GUI::CComboCtrl* pComboQuality = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd( i, COL_QUALITY));
				if (VIDEO_ENCODE_MODE_VBR == m_pWzdInfo->videoEncodeMode)
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
			if (IsMixDvr())
			{
				AddMIxBitStreamItem(pCombo,m_pWzdInfo->pChRecp[i].dfVS);
			}
			else
			{
				AddBitStreamItem(m_pWzdInfo->BitStreamValue, m_pWzdInfo->bitStreamCount, pCombo, m_defauResolution);
			}
#endif
		}

		if(NULL != pRateArr)
		{
			delete []pRateArr;
			pRateArr = NULL;
		}
	}

	if(!IsMixDvr())
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
				||(TD_2708XE_M == productTemp) ||(TD_2716XE_M == productTemp)  ||(TD_2708XE_MH == productTemp)
				||(TD_2708XE_P == productTemp)
				||(TD_2704XE_S == productTemp) || (TD_2708XE_S == productTemp) || (TD_2716XE_S == productTemp)
				||(TD_2704XS_SL == productTemp) || (TD_2708XS_SL == productTemp) || (TD_2716XS_SL == productTemp)
				||(TD_2704AS_SL == productTemp) ||(TD_2704AS_SL_A == productTemp) ||(TD_2704AS_PL == productTemp) || (TD_2708AS_SL == productTemp) || (TD_2716AS_SL == productTemp)
				||(TD_2704AS_S == productTemp) || (TD_2708AS_S == productTemp) || (TD_2708AS_S_A == productTemp) || (TD_2716AS_S == productTemp)
				||(TD_2704TS_S == productTemp) || (TD_2708TS_S == productTemp) || (TD_2716TS_S == productTemp)
				||(TD_2704TS_PL == productTemp) || (TD_2708TS_PL == productTemp) || (TD_2716TS_PL == productTemp)
				|| (TD_2704TD_PL == productTemp) || (TD_2708TD_PL == productTemp) || (TD_2716TD_PL == productTemp) || (TD_2732TD_PL == productTemp)
				|| (TD_2704TD_P == productTemp) || (TD_2708TD_P == productTemp) || (TD_2716TD_P == productTemp)
				|| (TD_2704XD_S == productTemp) || (TD_2708XD_S == productTemp) || (TD_2716XD_S == productTemp)
				|| (TD_2704TE_M == productTemp) || (TD_2708TE_M == productTemp) || (TD_2716TE_M == productTemp)
				|| (TD_2704AE_SL == productTemp) || (TD_2708AE_SL == productTemp) || (TD_2716AS_SL == productTemp)
				|| (TD_2004AL == productTemp) || (TD_2008AS == productTemp)
				|| (TD_2704AS_PL == productTemp) || (TD_2708AS_PL == productTemp) || (TD_2716AS_PL == productTemp)
				|| (TD_2716AC_PL == productTemp)
				|| (TD_2704AE_PL == productTemp) || (TD_2708AE_PL == productTemp) || (TD_2716AE_PL == productTemp))
			{
				rate = RealRate(m_maxChnnRate, (VIDEO_SIZE)m_defauResolution);
				printf("#############%s,%d,rate:%d,m_maxChnnRate:%d\n",__FILE__,__LINE__,rate,m_maxChnnRate);
			}

		}

		if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
		{
			rate = (m_maxChnnRate <= 25) ? m_maxChnnRate : 25;
		}
		else
		{
			rate = (m_maxChnnRate <= 30) ? m_maxChnnRate : 30;
		}
		assert(m_defauChnnRateNum <= rate);
		int i = 0;
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd( i, COL_RATE));
		pCombo->RemoveAllItem(bRefresh);
		for (int value = 1; value < rate + 1; ++value)
		{
			sprintf(str, "%d", value);
			pCombo->AddItem(str, value);					
		}
		pCombo->SetCurItemData(rate,bRefresh);

		pCombo->SetCurItemData(m_defauChnnRateNum, bRefresh);
		pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd( i, COL_RESOLUTION));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		AddResolutionItem(m_videoResolutionMask, pCombo,true);
		pCombo->SetCurItemData(m_defauResolution, bRefresh);	
		pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd( i, COL_QUALITY));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		AddQualityItem(m_pWzdInfo->QualityLevelValue, m_pWzdInfo->QualityLevelNum, pCombo);
		pCombo->SetCurItemData(m_pWzdInfo->defauQualityLevel, bRefresh);
#ifdef __DVR_PROFESSIONAL__	
		pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd(i,COL_STREAM_TYPE));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		AddEncodeModeItem(m_pWzdInfo->videoEncodeMode, pCombo);
		pCombo->SetCurItemData(m_pWzdInfo->defaVideoEncodeMode, bRefresh);
		{
			GUI::CComboCtrl* pComboQuality = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd( i, COL_QUALITY));
			if (VIDEO_ENCODE_MODE_VBR == m_pWzdInfo->defaVideoEncodeMode)
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
		pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd(i,COL_MAX_BIT_STREAM));
		assert(pCombo);
		pCombo->RemoveAllItem(bRefresh);
		AddBitStreamItem(m_pWzdInfo->BitStreamValue, m_pWzdInfo->bitStreamCount, pCombo,m_defauResolution);
		pCombo->SetCurItemData(m_pWzdInfo->defaBitStreamValue, bRefresh);
#endif
	}

	m_pWzdInfo->ChannelNum=m_channelNum;

	
	m_bVideoSizeSpecial = m_pWzdInfo->FrameRateSpec.bSupport;
	m_videoSizeCIFChnnBits = m_pWzdInfo->FrameRateSpec.videoSizeCIFChnnBits;
	m_videoSizeD1ChnnBits = m_pWzdInfo->FrameRateSpec.videoSizeD1ChnnBits;

	m_frameRate.Initial((VIDEO_FORMAT)m_videoFormat, m_channelNum, m_totalRate, m_maxChnnRate, CH);

	if (TD_2704HD == CProduct::Instance()->ProductType())
	{
		for (int ii = 4; ii < m_channelNum; ++ii)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd( ii, COL_RESOLUTION));
			pCombo->SetEnable(false,false);
		}
	}
	
	if (CProduct::Instance()->LocalVideoInputNum() > 0)
	{
		m_frameRate.Initial((VIDEO_FORMAT)m_videoFormat, CProduct::Instance()->LocalVideoInputNum(), m_totalRate, m_maxChnnRate, CH);
	}

	if(CProduct::Instance()->NetVideoInputNum() > 0)
	{
		unsigned long maxBitRate = CProduct::Instance()->GetDigitMaxTotalBitRate();
		unsigned long maxFrameRate = CProduct::Instance()->IPCMaxTotalRateOnVGA();
		if(VIDEO_FORMAT_NTSC == g_video_format)
		{
			maxFrameRate = maxFrameRate * 6 / 5;
		}
		m_ipcFrameEncode.Initial(CProduct::Instance()->NetVideoInputNum(), \
			maxBitRate, maxFrameRate, m_pWzdInfo->BitStreamValue, m_pWzdInfo->bitStreamCount, \
			m_ipcSupportEncode.defaResolution, m_ipcSupportEncode.defaChnnRate, m_ipcSupportEncode.defaBitRate);
	}
}

void CWzdRecordStreamDlg::ShowWzdData(bool bRefresh)
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;
	char str [128] = {0};

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);
	maxChnn = localVideoInputNum + netVideoInputNum;

	GUI::CListCtrl& list = m_cfgList;
	for (int i = 0; i < effectChnn; ++i)
	{
		if (i>=localVideoInputNum)
		{
			break;
		}
		int index = m_rowChnnMap.find(i)->second;
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RATE));
		assert(pCombo);
		int rate = 0;
		if (m_bVideoSizeSpecial && (((ULONGLONG)0x01 << index) & m_videoSizeD1ChnnBits))
		{
			rate = m_pWzdInfo->InfoEx[index].rate;
		}
		else if (UsedRate(m_pWzdInfo->InfoEx[index].rate, (VIDEO_SIZE)(m_pWzdInfo->InfoEx[index].resolution)) > m_maxChnnRate)
		{
			rate = RealRate(m_maxChnnRate, (VIDEO_SIZE)(m_pWzdInfo->InfoEx[index].resolution));				
		}
		else
		{
			rate = m_pWzdInfo->InfoEx[index].rate;
		}
		pCombo->SetCurItemData(rate,bRefresh);
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));

		assert(pCombo);
		if (IsMixDvr())
		{
			if ((m_pWzdInfo->InfoEx[index].resolution)&(m_pWzdInfo->pChRecp[i].supVS))
			{
				pCombo->SetCurItemData(m_pWzdInfo->InfoEx[index].resolution,bRefresh);
			}
			else
			{
				assert(false);
				pCombo->SetCurItem(0, bRefresh);
			}
		}
		else if ((m_pWzdInfo->InfoEx[index].resolution)&(m_videoResolutionMask))
		{
			pCombo->SetCurItemData(m_pWzdInfo->InfoEx[index].resolution,bRefresh);
		}
		else
		{
			assert(false);
			pCombo->SetCurItem(0, bRefresh);
		}
				
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_QUALITY));
		assert(pCombo);
		if (IsInQualityLevel(m_pWzdInfo->InfoEx[index].quality))
		{
			pCombo->SetCurItemData(m_pWzdInfo->InfoEx[index].quality,bRefresh);
		}
		else
		{
			assert(false);
			pCombo->SetCurItem(0, bRefresh);
		}

		if (VIDEO_ENCODE_MODE_VBR == m_pWzdInfo->InfoEx[index].encodeType)
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

		if ((m_pWzdInfo->InfoEx[index].encodeType) & (m_pWzdInfo->videoEncodeMode))
		{
			pCombo->SetCurItemData(m_pWzdInfo->InfoEx[index].encodeType,bRefresh);
		}
		else
		{
			assert(false);
			pCombo->SetCurItem(0, bRefresh);
		}	

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_MAX_BIT_STREAM));
		assert(pCombo);
		if (IsInBitStreamRange(m_pWzdInfo->InfoEx[index].hBitStream))
		{
			pCombo->SetCurItemData(m_pWzdInfo->InfoEx[index].hBitStream,bRefresh);
		}
		else
		{
			assert(false);
			pCombo->SetCurItem(0, bRefresh);
		}
#endif
		m_frameRate.AddRate(index, rate, static_cast<VIDEO_SIZE>(m_pWzdInfo->InfoEx[index].resolution));
	}

	//在向导页面添加IPC的录像参数显示added by wn 2011-11-28
#ifdef __DVR_ULTIMATE__
	{
		int j = 0;
		int nIndex = 0;
		for (int i = localVideoInputNum; i < effectChnn; ++i)
		{
			nIndex = m_rowChnnMap.find(i)->second;
			assert(nIndex >= 0);

			assert(m_pNetDevInfo);
			int nTemp = 0;
			for (; nTemp < m_netDevNum; nTemp++)
			{
				if (m_pNetDevInfo[nTemp].chnn == nIndex)
				{
					break;
				}
			}

			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));
			assert(pCombo);
			if (m_pWzdInfo->InfoEx[nIndex].resolution & m_pNetDevInfo[nTemp].videoSize)
			{
				pCombo->SetCurItemData(m_pWzdInfo->InfoEx[nIndex].resolution,bRefresh);
				pCombo->SetEnable(true,bRefresh);
			}
			else
			{
				pCombo->SetCurItem(0, bRefresh);
				pCombo->SetEnable(false,bRefresh);
			}
			
			unsigned long resolution = pCombo->GetCurItemData();
			resolution = VideoSizeToDDVideoSize(resolution);
			int n = 1;
			while(resolution>>n)
			{
				n++;
			}
			n--;

			int rate = m_pWzdInfo->InfoEx[nIndex].rate;
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RATE));
			assert(pCombo);

			unsigned long maxChnnRate = CProduct::Instance()->IPCMaxChnnRate();
			if(VIDEO_FORMAT_NTSC == g_video_format)
			{
				maxChnnRate = (maxChnnRate + 1) * 6 / 5;
				if (30 > maxChnnRate)
				{
					maxChnnRate = 30;
				}
			}
			rate = (rate <= maxChnnRate) ? rate : maxChnnRate;

			if (NULL != m_pNetDevInfo && j < m_netDevNum)
			{
				pCombo->RemoveAllItem(bRefresh);

				int maxRate = 0;
				maxRate = (m_pNetDevInfo[j].ucRate[n] <= maxChnnRate) ? m_pNetDevInfo[j].ucRate[n] : maxChnnRate;
				if( maxRate <= 0 )
				{
					maxRate = maxChnnRate;
					pCombo->SetEnable(false,bRefresh);
				}
				else
				{
					pCombo->SetEnable(true,bRefresh);
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
			if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
			{
				rate = (m_maxChnnRate <= 25) ? m_maxChnnRate : 25;
			}
			else
			{
				rate = (m_maxChnnRate <= 30) ? m_maxChnnRate : 30;
			}

			pCombo->SetCurItemData(rate,bRefresh);

			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_QUALITY));
			assert(pCombo);
			if (IsInQualityLevel(m_pWzdInfo->InfoEx[nIndex].quality))
			{
				pCombo->SetCurItemData(m_pWzdInfo->InfoEx[nIndex].quality,bRefresh);
			}
			else
			{
				pCombo->SetCurItem(0, bRefresh);
			}

			if (VIDEO_ENCODE_MODE_VBR == m_pWzdInfo->InfoEx[nIndex].encodeType)
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
			if ((m_pWzdInfo->InfoEx[nIndex].encodeType) & (m_pWzdInfo->videoEncodeMode))
			{
				pCombo->SetCurItemData(m_pWzdInfo->InfoEx[nIndex].encodeType,bRefresh);
			}
			else
			{
				pCombo->SetCurItem(0, bRefresh);
			}	

			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_MAX_BIT_STREAM));
			assert(pCombo);
			if (IsInBitStreamRange(m_pWzdInfo->InfoEx[nIndex].hBitStream))
			{
				pCombo->SetCurItemData(m_pWzdInfo->InfoEx[nIndex].hBitStream,bRefresh);
			}
			else
			{
				pCombo->SetCurItem(0, bRefresh);
			}
#endif
			m_ipcFrameEncode.AddEncode(nIndex - localVideoInputNum, rate, static_cast<VIDEO_SIZE>(m_pWzdInfo->InfoEx[nIndex].resolution), m_pWzdInfo->InfoEx[nIndex].hBitStream);
			j++;
		}
	}
#endif
	//added end

	//IPC成员初始化
	for(int i = 0; i < localVideoInputNum + netVideoInputNum; ++i)
	{
		m_infoEx[i] = m_pWzdInfo->InfoEx[i];
	}

	if(localVideoInputNum > 0)
	{
		char str [128] = {0};
		unsigned long remainRate = m_frameRate.Remaining();//该处有错误
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
			sprintf(str, " : %d (CIF), %d (HD1) %d (D1).", remainRate, remainRate/2, remainRate/4);
	#endif
		}
		m_cfgScRemainNum.SetCaption(str, false );
	}
}

void CWzdRecordStreamDlg::AddQualityItem(unsigned long *pQualityValue, unsigned long count, CComboCtrl *pCombo)
{
	char str[128] = {0};

	for (int i = 0; i < count; ++i)
	{
		if (IMAGE_QUALITY_LOWEST == pQualityValue[i] )
		{
			strcpy( str, m_pStrTable->String( E_STR_ID_LOWEST ) );
			pCombo->AddItem( str, pQualityValue[i]);
		}
		else if (IMAGE_QUALITY_LOWER == pQualityValue[i])
		{
			strcpy( str, m_pStrTable->String( E_STR_ID_LOWER ) );
			pCombo->AddItem( str,  pQualityValue[i]);
		}
		else if (IMAGE_QUALITY_LOW == pQualityValue[i])
		{
			strcpy( str, m_pStrTable->String( E_STR_ID_LOW ) );
			pCombo->AddItem(str, pQualityValue[i]);
		}
		else if (IMAGE_QUALITY_MEDIUM == pQualityValue[i])
		{
			strcpy( str, m_pStrTable->String( E_STR_ID_MEDIUM ) );
			pCombo->AddItem( str, pQualityValue[i] );
		}
		else if (IMAGE_QUALITY_HEIGHTER == pQualityValue[i])
		{
			strcpy( str, m_pStrTable->String( E_STR_ID_HIGHER ) );
			pCombo->AddItem( str, pQualityValue[i] );
		}
		else if (IMAGE_QUALITY_HEIGHTEST == pQualityValue[i])
		{
			strcpy( str, m_pStrTable->String( E_STR_ID_HIGHEST ) );
			pCombo->AddItem( str, pQualityValue[i] );
		}	
		else 
		{
			assert(false);
		}
	}

}

void CWzdRecordStreamDlg::AddResolutionItem(unsigned long dwResolution, CComboCtrl *pCombo, bool bAddD1)
{
	assert(dwResolution != 0);

	if (dwResolution & DVRVIDEO_SIZE_QCIF)
	{
		pCombo->AddItem("QCIF", DVRVIDEO_SIZE_QCIF);
	}

	if (dwResolution & DVRVIDEO_SIZE_CIF)
	{
		pCombo->AddItem("CIF", DVRVIDEO_SIZE_CIF);
	}

	if (dwResolution & DVRVIDEO_SIZE_HD1)
	{
		pCombo->AddItem("HD1", DVRVIDEO_SIZE_HD1);
	}

	if (dwResolution & DVRVIDEO_SIZE_D1)
	{
		if (bAddD1)
		{
			pCombo->AddItem("D1", DVRVIDEO_SIZE_D1);
		}				
	}			

#if defined(__CUSTOM_IL03__)|| defined(__CUSTOM_US02__)
	if (dwResolution & DVRVIDEO_SIZE_960H)
	{
		pCombo->AddItem("960H", DVRVIDEO_SIZE_960H);							
	}
#else
	if (dwResolution & DVRVIDEO_SIZE_960H)
	{
		pCombo->AddItem("WD1", DVRVIDEO_SIZE_960H);							
	}
#endif

	if (dwResolution & DVRVIDEO_SIZE_HD720)
	{
		pCombo->AddItem("720P", DVRVIDEO_SIZE_HD720);
	}


	if (dwResolution & DVRVIDEO_SIZE_VGA)
	{
		pCombo->AddItem("VGA", DVRVIDEO_SIZE_VGA);
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

void CWzdRecordStreamDlg::AddEncodeModeItem(unsigned long videoEncodeMode, CComboCtrl *pCombo)
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

void CWzdRecordStreamDlg::AddBitStreamItem(unsigned long *pBitValue, unsigned long count, CComboCtrl *pCombo,unsigned long resolution)
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
	}

	pCombo->RemoveAllItem(false);
	
	for (int i = 0;i < count; ++i)
	{
		if(pBitValue[i] > maxBitRate)
		{
			break;
		}

		if(0 == pBitValue[i])
		{
			continue;
		}

		snprintf(str, sizeof(str), "%d kbps", pBitValue[i]);
		pCombo->AddItem(str, pBitValue[i]);
	}

	pCombo->SetDropDownCount(7);

}

bool CWzdRecordStreamDlg::IsInQualityLevel(unsigned long value)
{
	for (int i = 0; i < m_pWzdInfo->QualityLevelNum; ++i)
	{
		if (m_pWzdInfo->QualityLevelValue[i] == value)
		{
			return true;
		}
	}
	return false;
}

bool CWzdRecordStreamDlg::IsInBitStreamRange(unsigned long value)
{
	for (int i = 0; i< m_pWzdInfo->bitStreamCount; ++i)
	{
		if (m_pWzdInfo->BitStreamValue[i] == value)
		{
			return true;
		}
	}
	return false;
}

BEGIN_MSG_MAP(CWzdRecordStreamDlg, CWzdPageDlg)
	ON_MSG_EX(m_cfgList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_cfgListCtrlAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()

unsigned long CWzdRecordStreamDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	unsigned long ret = KEY_VIRTUAL_MSG_STOP;

	//2010-03-03 17:57:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListCtrlAll.GetItemWnd(0, COL_CHNN));
	assert(pCheckAll);
	if( pCheckAll->IsCheck() )
	{
		pCheckAll->SetCheck(false, true);
	}

	if (m_pWzdInfo->Modified!=1)
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
			
			ret = KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			break;
		}
	}

	int localVideoInputNum = CProduct::Instance()->LocalVideoInputNum();
	int netVideoInputNum = CProduct::Instance()->NetVideoInputNum();
	int maxChnn = localVideoInputNum + netVideoInputNum;
	//2010-03-02 11:00:00 YSW
	//只有点击帧率和分辨率时，才处理资源部分
	if ((COL_RATE == col) || (COL_RESOLUTION == col))
	{
		int index = m_rowChnnMap.find(row)->second;
		assert (index < maxChnn);

		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_RATE));
		assert(pCombo);
		unsigned char rate = static_cast<unsigned char>(pCombo->GetCurItemData());

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_RESOLUTION));
		assert(pCombo);
		VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pCombo->GetCurItemData());

		if(index < localVideoInputNum)
		{
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
					size = (VIDEO_SIZE)pCheckEncInfoChecked[row].ulRes;
					GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_RESOLUTION));
					pCombo->SetCurItemData(size, true);

					rate = pCheckEncInfoChecked[row].usRate;
					pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_RATE));
					pCombo->SetCurItemData(rate, true);

					MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				}

				m_checkEncInfo[row].usRate = rate;
				m_checkEncInfo[row].ulRes = size;

				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			}
			else
			{
				unsigned long remainRate = 0;
				if (m_bVideoSizeSpecial && (((ULONGLONG)0x01 << row) & m_videoSizeD1ChnnBits))
				{
					remainRate = m_frameRate.ModifyRate(row, rate, size, false);
				}
				else
				{
					if (!m_frameRate.TryRate(row, rate, size))
					{
						GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_RATE));
						assert(pCombo);
						pCombo->SetCurItemData(rate, true);
						MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
						//超过合理的部分不应该保存
					}

					if ((DVRVIDEO_SIZE_HD1080 == size) && (rate > CProduct::Instance()->LocalDefChnnRateNum()))
					{
						GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_RATE));
						assert(pCombo);
						rate = CProduct::Instance()->LocalDefChnnRateNum();
						pCombo->SetCurItemData(rate, true);

						MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					}

					remainRate = m_frameRate.ModifyRate(row, rate, size);
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
		}
		else
		{
#ifdef __DVR_PROFESSIONAL__
			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_MAX_BIT_STREAM));
			assert(pCombo);
			unsigned long bitRate = pCombo->GetCurItemData();

			bool bTryRet = m_ipcFrameEncode.TryEncode(index-localVideoInputNum, rate, size, bitRate);

			if(!bTryRet)
			{
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_RATE));
				assert(pCombo);
				pCombo->SetCurItemData(rate, true);

				MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			}

			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_MAX_BIT_STREAM));
			assert(pCombo);
			pCombo->SetCurItemData(bitRate, true);

			m_ipcFrameEncode.ModifyEncode(index-localVideoInputNum, rate, size, bitRate);
#endif
		}
	}
	
	#ifdef __DVR_PROFESSIONAL__
	if (COL_RESOLUTION == col)
	{
		GUI::CComboCtrl* pComboR = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_RESOLUTION));
		GUI::CComboCtrl* pComboB = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_MAX_BIT_STREAM));

		VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pComboR->GetCurItemData());
		if(IsMixDvr())
		{
			AddMIxBitStreamItem(pComboB,size);
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
			if (/*(TD_2704XE_L == product) || */(TD_2704XD_M == product) || (TD_2708XD_M == product) || (TD_2716XD_M == product)
				|| (TD_2704XD_P == product) || (TD_2708XD_P == product) || (TD_2708XE_P == product))
			{
				pComboB->SetCurItemData(10240, true);
			}
			else if ((TD_2716XD_P == product) || (TD_2708XE_M == product) || (TD_2716XE_M == product) || (TD_2708XE_MH == product)
				|| (TD_2708AS_SL == product) || (TD_2716AS_SL == product) || (TD_2708TS_S == product) || (TD_2716TS_S == product) || (TD_2708TS_PL == product) || (TD_2716TS_PL == product)
				|| (TD_2704TD_P == product) || (TD_2708TD_P == product) || (TD_2716TD_P == product) || (TD_2704TD_PL == product) || (TD_2708TD_PL == product) || (TD_2716TD_PL == product) || (TD_2732TD_PL == product)
				|| (TD_2704TE_M == product) || (TD_2708TE_M == product) || (TD_2716TE_M == product)
				|| (TD_2704AE_SL == product) || (TD_2708AE_SL == product) || (TD_2716AS_SL == product)
				|| (TD_2004AL == product)
				|| (TD_2704AS_PL == product) || (TD_2708AS_PL == product) || (TD_2716AS_PL == product)
				|| (TD_2716AC_PL == product)
				|| (TD_2704AE_PL == product) || (TD_2708AE_PL == product) || (TD_2716AE_PL == product)
				)
			{
				pComboB->SetCurItemData(8192, true);
			}
// 			else if ((TD_2708XE_MH == product))
// 			{
// 				pComboB->SetCurItemData(6144, true);
// 			}
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
				|| (TD_2304SE_C == product) || (TD_2308SE_C == product) || (TD_2316SE_C == product) || (TD_2304SS_SDI == product) || (TD_2308SS_SDI == product)|| (TD_2308SS_SDI == product) ||(TD_2304SS_SL_3515A == product)
				|| (TD_2508HE_SDI == product) || (TD_2516HS_BL == product)) 
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
#endif

#ifdef __DVR_PROFESSIONAL__
	//点击码率时，处理数字信号的资源问题
	if(COL_MAX_BIT_STREAM == col)
	{
		int index = m_rowChnnMap.find(row)->second;
		assert (index < maxChnn);

		if((index >= localVideoInputNum) && (localVideoInputNum > 0))
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_RATE));
			assert(pCombo);
			unsigned char rate = static_cast<unsigned char>(pCombo->GetCurItemData());

			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_RESOLUTION));
			assert(pCombo);
			VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pCombo->GetCurItemData());

			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_MAX_BIT_STREAM));
			assert(pCombo);
			unsigned long bitRate = pCombo->GetCurItemData();

			bool bTryRet = m_ipcFrameEncode.TryEncode(index - localVideoInputNum, rate, size, bitRate);

			if(!bTryRet)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			}

			m_ipcFrameEncode.ModifyEncode(index-localVideoInputNum, rate, size, bitRate);

		}//endif
	}

	//当选择定码流时，把画质选择置为不可选状态.
	if (COL_STREAM_TYPE == col)
	{
		assert (row < g_p_login_succ->productInfo.videoInputNum);

		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_STREAM_TYPE));
		assert(pCombo);
		unsigned long type = static_cast<unsigned long>(pCombo->GetCurItemData());

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_QUALITY));
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

	if (COL_RESOLUTION == col)
	{
		GUI::CComboCtrl* pComboR = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_RESOLUTION));
		GUI::CComboCtrl* pComboB = dynamic_cast<GUI::CComboCtrl*>(m_cfgList.GetItemWnd(row, COL_MAX_BIT_STREAM));

		VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pComboR->GetCurItemData());
		if (DVRVIDEO_SIZE_CIF == size)
		{
			pComboB->SetCurItemData(768, true);
		}	
		else if (DVRVIDEO_SIZE_D1 == size)
		{
			pComboB->SetCurItemData(1536,true);
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
				|| (TD_2508HE_SDI == product) || (TD_2516HS_BL == product))
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
#endif
	//把选择的保存到指针部分
	
	
	return ret;
}

unsigned long CWzdRecordStreamDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	// 点选非All，先检查是否处于All状态，处于则认为做了修改
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListCtrlAll.GetItemWnd(0, COL_CHNN));
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCheckAll );
	assert( pCfgDlg );

	unsigned long ret = KEY_VIRTUAL_MSG_STOP;
	if( pCheckAll->IsCheck() )
	{
		pCfgDlg->SetRefreshView( true );
		if (m_pWzdInfo->Modified!=1)
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
				pCfgDlg->SetModify(true);
				ret = KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
		else
		{
			pCfgDlg->SetRefreshView(false);
		}
	}

	 //点选All 直接认为做了修改
	if (m_pWzdInfo->Modified!=1)
	{
		if( COL_CHNN == col )
		{
			pCfgDlg->SetModify(true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}

	//2010-03-02 11:00:00 YSW
	//只有点击帧率和分辨率时，才处理资源部分
	if ((COL_RATE == col) || (COL_RESOLUTION == col))
	{
		assert (row < g_p_login_succ->productInfo.videoInputNum);

		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_RATE));
		assert(pCombo);
		unsigned char rate = static_cast<unsigned char>(pCombo->GetCurItemData());

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_RESOLUTION));
		assert(pCombo);
		VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pCombo->GetCurItemData());

		unsigned long maxRate = RealRate(m_totalRate/g_p_login_succ->productInfo.localVideoInputNum, size);
		if (m_bVideoSizeSpecial)
		{
			maxRate = RealRate(m_maxChnnRate, size);
		}

		if (maxRate < rate)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_RATE));
			assert(pCombo);
			pCombo->SetCurItemData(maxRate, true);

			MessageBox(m_pStrTable->String(E_STR_ID_TIP_03_02), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		}
	}
#ifdef __DVR_PROFESSIONAL__
	if (COL_RESOLUTION == col)
	{
		GUI::CComboCtrl* pComboR = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_RESOLUTION));
		GUI::CComboCtrl* pComboB = dynamic_cast<GUI::CComboCtrl*>(m_cfgListCtrlAll.GetItemWnd(row, COL_MAX_BIT_STREAM));

		VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pComboR->GetCurItemData());
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
			if (/*(TD_2704XE_L == product) || */(TD_2704XD_M == product) || (TD_2708XD_M == product) || (TD_2716XD_M == product)
				|| (TD_2704XD_P == product) || (TD_2708XD_P == product) || (TD_2708XE_P == product))
			{
				pComboB->SetCurItemData(10240, true);
			}
			else if ((TD_2716XD_P == product) || (TD_2708XE_M == product) || (TD_2716XE_M == product) || (TD_2708XE_MH == product)
				|| (TD_2708AS_SL == product) || (TD_2716AS_SL == product) || (TD_2708TS_S == product) || (TD_2716TS_S == product) || (TD_2708TS_PL == product) || (TD_2716TS_PL == product)
				|| (TD_2704TD_P == product) || (TD_2708TD_P == product) || (TD_2716TD_P == product) || (TD_2704TD_PL == product) || (TD_2708TD_PL == product) || (TD_2716TD_PL == product) || (TD_2732TD_PL == product)
				|| (TD_2704TE_M == product) || (TD_2708TE_M == product) || (TD_2716TE_M == product)
				|| (TD_2704AE_SL == product) || (TD_2708AE_SL == product) || (TD_2716AS_SL == product)
				|| (TD_2004AL == product)
				|| (TD_2704AS_PL == product) || (TD_2708AS_PL == product) || (TD_2716AS_PL == product)
				|| (TD_2716AC_PL == product)
				|| (TD_2704AE_PL == product) || (TD_2708AE_PL == product) || (TD_2716AE_PL == product))
			{
				pComboB->SetCurItemData(8192, true);
			}
// 			else if ((TD_2708XE_MH == product))
// 			{
// 				pComboB->SetCurItemData(6144, true);
// 			}
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
			if ((TD_2504HD_C == product)  || (TD_2508HD_C == product) || (TD_2516HD_C == product) || (TD_2524HD_C == product) || (TD_2532HD_C == product) 
				|| (TD_2604LE == product) || (TD_2608LE == product) || (TD_2616LE == product) || (TD_2508HE_C_3521 == product) || (TD_2524HE_C == product) || (TD_2532HE_C == product)
				|| (TD_2304ME_C == product) || (TD_2308ME_C == product) || (TD_2316ME_C == product) || (TD_2616LD == product) 
				|| (TD_2304SS_C == product) || (TD_2308SS_C == product) || (TD_2316SS_C == product)
				|| (TD_2304SL_C == product) || (TD_2308SL_C == product)
				|| (TD_2304SE_C == product) || (TD_2308SE_C == product) || (TD_2316SE_C == product) || (TD_2304SS_SDI == product) || (TD_2308SS_SDI == product)||(TD_2304SS_SL_3515A == product)
				|| (TD_2508HE_SDI == product) || (TD_2516HS_BL == product))
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
#endif
	//不管是选中全选状态还是不选状态，都需要做处理。
	//当选择定码流时，把画质选择置为不可选状态.

#ifdef __DVR_PROFESSIONAL__	
	if (COL_STREAM_TYPE == col)
	{
		assert (row < g_p_login_succ->productInfo.videoInputNum);

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
#endif
	
	return ret;
}

void CWzdRecordStreamDlg::GetWzdData(bool bNext)
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	const GUI::CListCtrl& list = m_cfgList;
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListCtrlAll.GetItemWnd(0, COL_CHNN));
	if(pCheckAll->IsCheck() )
	{
		const GUI::CListCtrl& listAll = m_cfgListCtrlAll;
		for (int i=0; i<effectChnn; i++)
		{
			if (i>=localVideoInputNum)
			{
				break;
			}
			int index = m_rowChnnMap.find(i)->second;
			GUI::CComboCtrl* pCbRate = NULL;
			GUI::CComboCtrl* pCbResolution = NULL;
			GUI::CComboCtrl* pCbQuality = NULL;
			GUI::CComboCtrl* pCbType = NULL;
			GUI::CComboCtrl* pCbMax = NULL;
			pCbRate = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_RATE));
			pCbResolution = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_RESOLUTION));
			pCbQuality = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_QUALITY));

	#ifdef __DVR_PROFESSIONAL__
			pCbType = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_STREAM_TYPE));
			pCbMax = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)listAll.GetItemWnd(0,COL_MAX_BIT_STREAM));
	#endif
			m_infoEx[index].rate=pCbRate->GetCurItemData();
			m_infoEx[index].resolution = pCbResolution->GetCurItemData();
			m_infoEx[index].quality = pCbQuality->GetCurItemData();
	#ifdef __DVR_PROFESSIONAL__
			m_infoEx[index].encodeType = pCbType->GetCurItemData();
			m_infoEx[index].hBitStream = pCbMax->GetCurItemData();
	#endif

		}
	}
	else
	{
			for (int i=0; i<effectChnn; i++)
			{
				if (i>=localVideoInputNum)
				{
					break;
				}
				GUI::CComboCtrl* pCbRate = NULL;
				GUI::CComboCtrl* pCbResolution = NULL;
				GUI::CComboCtrl* pCbQuality = NULL;
				GUI::CComboCtrl* pCbType = NULL;
				GUI::CComboCtrl* pCbMax = NULL;

				int index = m_rowChnnMap.find(i)->second;

				pCbRate = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_RATE));
				pCbResolution = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_RESOLUTION));
				pCbQuality = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_QUALITY));

	#ifdef __DVR_PROFESSIONAL__
				pCbType = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_STREAM_TYPE));
				pCbMax = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_MAX_BIT_STREAM));
	#endif
				m_infoEx[index].rate=pCbRate->GetCurItemData();
				m_infoEx[index].resolution = pCbResolution->GetCurItemData();
				m_infoEx[index].quality = pCbQuality->GetCurItemData();
	#ifdef __DVR_PROFESSIONAL__
				m_infoEx[index].encodeType = pCbType->GetCurItemData();
				m_infoEx[index].hBitStream = pCbMax->GetCurItemData();
	#endif
			}
	}

	//将向导页面中的IPC数据保存到内存中 added by wn 2011-11-28
#ifdef __DVR_ULTIMATE__
	for (int i = localVideoInputNum; i<effectChnn; i++)
	{
		GUI::CComboCtrl* pCbRate = NULL;
		GUI::CComboCtrl* pCbResolution = NULL;
		GUI::CComboCtrl* pCbQuality = NULL;
		GUI::CComboCtrl* pCbType = NULL;
		GUI::CComboCtrl* pCbMax = NULL;

		int index = m_rowChnnMap.find(i)->second;

		pCbRate = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_RATE));
		pCbResolution = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_RESOLUTION));
		pCbQuality = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_QUALITY));

#ifdef __DVR_PROFESSIONAL__
		pCbType = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_STREAM_TYPE));
		pCbMax = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(i,COL_MAX_BIT_STREAM));
#endif
		m_infoEx[index].rate=pCbRate->GetCurItemData();
		m_infoEx[index].resolution = pCbResolution->GetCurItemData();
		m_infoEx[index].quality = pCbQuality->GetCurItemData();
#ifdef __DVR_PROFESSIONAL__
		m_infoEx[index].encodeType = pCbType->GetCurItemData();
		m_infoEx[index].hBitStream = pCbMax->GetCurItemData();
#endif
	}
#endif
	//added end

	for(int i = 0; i < localVideoInputNum + netVideoInputNum; ++i)
	{
		m_pWzdInfo->InfoEx[i] = m_infoEx[i];
	}
}

void CWzdRecordStreamDlg::OnDestroy()
{
	if (NULL != m_pNetDevInfo)
	{
		delete [] m_pNetDevInfo;
		m_pNetDevInfo = NULL;
	}
	
	CWzdPageDlg::OnDestroy();

	m_rowChnnMap.clear();
}


bool CWzdRecordStreamDlg::IsMixDvr()
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

void CWzdRecordStreamDlg::AddMIxBitStreamItem(CComboCtrl *pCombo, unsigned long resolution)
{
	assert(pCombo);
	char str[128] = {0};
	int rowCount = 0;
	int vsrow = 0;
	bool bfind =false;

	for(int index=0;index<g_resolutionNum;++index)
	{
		if(0 != (resolution & m_pWzdInfo->pVSBit[index].resolution))
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
		if(0 != m_pWzdInfo->pVSBit[vsrow].vsforbrr[i])
		{
			++rowCount;
			sprintf(str, "%d kbps",  m_pWzdInfo->pVSBit[vsrow].vsforbrr[i]);
			pCombo->AddItem(str,  m_pWzdInfo->pVSBit[vsrow].vsforbrr[i]);
		}
		else//最后一个项目是0
		{
			break;
		}
	}
	pCombo->SetDropDownCount(7);
}
