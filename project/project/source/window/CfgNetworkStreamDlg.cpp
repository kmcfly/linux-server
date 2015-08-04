/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-14
** Name         : CfgNetworkStreamDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgNetworkStreamDlg.h"
#include "FuncCustom.h"
#include "CalculateSubEncode.h"

CCfgNetworkStreamDlg::CCfgNetworkStreamDlg()
{
	m_localVideoInputNum = 0;
	m_netVideoInputNum = 0;

	m_supportSubEncInfo = NULL;
	m_reslutionNum = 0;
}

CCfgNetworkStreamDlg::~CCfgNetworkStreamDlg()
{

}

void CCfgNetworkStreamDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgNetworkStreamList;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CCheckCtrl* pCheck = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[LIST_ITEM_HEIGHT] - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头
	
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	m_localVideoInputNum = localVideoInputNum;
	m_netVideoInputNum = netVideoInputNum;

	maxChnn = effectChnn;

	int row = 0;
	for(int i = 0,j=0; j < maxChnn; i++)
	{
		if((CH >> i) & 0x1)
		{
			m_rowChnnMap.insert(std::make_pair(row++, i));
			j++;
		}
	}

	if(effectChnn > maxListRow)
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	m_channelNum = CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
#ifdef __DVR_PROFESSIONAL__
	list.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 5 / 32 -10 );
	list.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 5 / 32+10);
	list.AddColumn(m_pStrTable->String(E_STR_ID_REC_ENCODE_TYPE), width * 6 / 32 - 8);
	list.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 5 / 32 + 8 );
	list.AddColumn(m_pStrTable->String(E_STR_ID_REC_MAX_BIT_STREAM), width * 8 / 32 - offsetEdge);
#else
	list.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 9 / 32 );
	list.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 9 / 32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 11 / 32 - offsetEdge );
#endif

	char str[128] = {0};

	for(int i = 0; i < effectChnn; i++)
	{
		std::map<int, int>::iterator it = m_rowChnnMap.find(i);
		if(it == m_rowChnnMap.end())
		{
			continue;
		}

		int chnnIndex = it->second;
		if(chnnIndex < localVideoInputNum)
		{
			sprintf(str, "%d", chnnIndex + 1);
		}
		else
		{
			//break;
			sprintf(str, "%d", chnnIndex + 1 /*- localVideoInputNum*/);
		}
		list.AddItem(i, COL_CHNN, str);

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_RATE, WND_TYPE_COMBO));		
 		assert(pCombo);
// 		for(int ii = 0; ii < 30; ii++)
// 		{
// 			sprintf( str, "%d", ii + 1 );
// 			pCombo->AddItem( str,  ii + 1 );
// 		}
		pCombo->SetCurItem( 0 ,false );	

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_RESOLUTION, WND_TYPE_COMBO));
		assert(pCombo);	

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem( i, COL_QUALITY, WND_TYPE_COMBO));
		assert(pCombo);

#ifdef __DVR_PROFESSIONAL__	
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(i,COL_STREAM_TYPE,WND_TYPE_COMBO));
		assert(pCombo);

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(i,COL_MAX_BIT_STREAM,WND_TYPE_COMBO));
		assert(pCombo);	
#endif
	}
	list.CreateListFocusMap();

	GUI::CRect rect = list.GetWindowRect();
	y += rect.Height() + 4;

	unsigned short cx = 0, cy = 0,temp = 0;
	//if (localVideoInputNum > 0)
	{
		snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_ALL ) );
		TextSize(str, strlen(str), cx, cy);

		if(!g_langRightOrder)
		{
			m_cfgScAll.Create( GUI::CWnd::GetRandWndID(), x, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, cx, m_length[STATIC_HEIGHT], this );
		}
		else
		{
			m_cfgScAll.Create( GUI::CWnd::GetRandWndID(), m_rect.Width() - x - cx, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, cx, m_length[STATIC_HEIGHT], this );
		}
		m_cfgScAll.SetCaption( str, false );

		x += 100;
		snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_REMAINING ) );
		//strcpy( str, "Remaining");
		TextSize(str, strlen(str), cx, cy);
		if(!g_langRightOrder)
		{
			m_cfgScRemaining.Create( GUI::CWnd::GetRandWndID(), x, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, 100, m_length[STATIC_HEIGHT], this );
		}
		else
		{
			m_cfgScRemaining.Create( GUI::CWnd::GetRandWndID(), m_rect.Width() - x - cx  , y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, cx, m_length[STATIC_HEIGHT], this );
		}
		m_cfgScRemaining.SetCaption(str, false );

		x += cx;
		strcpy(str, " : 888 (CIF).");
		TextSize(str, strlen(str), cx, cy);
		if(!g_langRightOrder)
		{
			m_cfgScRemainNum.Create( GUI::CWnd::GetRandWndID(), x, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, width / 2, m_length[STATIC_HEIGHT], this );

		}
		else
		{
			m_cfgScRemainNum.Create( GUI::CWnd::GetRandWndID(), m_rect.Width() - x - cx, y + ( m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT]) / 2, cx, m_length[STATIC_HEIGHT], this );

		}
		m_cfgScRemainNum.SetCaption( " : 0 (CIF).", false );
		m_cfgScRemainNum.SetBKColor(COLOR_DLG_CHILD_BK);

		x = m_length[DLG_OFFSET_SUB_LEFT];
		y += m_length[LIST_ITEM_HEIGHT];
		GUI::CListCtrl& listAll = m_cfgListAll;
		listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 1, false );
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
#ifdef __DVR_PROFESSIONAL__
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 5 / 32 -10 );
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 5 / 32+10);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_REC_ENCODE_TYPE), width * 6 / 32 - 8);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 5 / 32 + 8 );
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_REC_MAX_BIT_STREAM), width * 8 / 32);
#else
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 9 / 32 );
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 9 / 32);
		listAll.AddColumn(m_pStrTable->String(E_STR_ID_QUALITY), width * 11 / 32 );
#endif
		{
			int i = 0;

			pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem(i,COL_CHNN,WND_TYPE_CHECK));
			assert(pCheck);
			//那些DVR 子码流 不支持ALL    （假定以下产品不支持ALL）
			PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
			/*if (!IsSupportSubCode())*/
			//{
				//pCheck->Show(false,false);
			//}

			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem( i, COL_RESOLUTION, WND_TYPE_COMBO));
			assert(pCombo);
			if (!IsSupportSubCode())
			{
				pCombo->SetEnable(false, false);
			}

			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem( i, COL_RATE, WND_TYPE_COMBO));
			assert(pCombo);
			if (!IsSupportSubCode())
			{
				pCombo->SetEnable(false, false);
			}
			
			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem( i, COL_QUALITY, WND_TYPE_COMBO));
			assert(pCombo);
			if (!IsSupportSubCode())
			{
				pCombo->SetEnable(false, false);
			}

#ifdef __DVR_PROFESSIONAL__		
			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem(i,COL_STREAM_TYPE,WND_TYPE_COMBO));
			assert(pCombo);

			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.AddItem(i,COL_MAX_BIT_STREAM,WND_TYPE_COMBO));
			assert(pCombo);		
#endif
		}

		PRODUCT_TYPE product = CProduct::Instance()->ProductType();
		if ((product == TD_2604LD_S)||(product == TD_2608LD_S)||(product == TD_2616LD_S)||(product == TD_2604LD)||(product == TD_2608LD)||(product == TD_2616LD) || (product == TD_2504HD_C) || (product == TD_2508HD_C) || (product == TD_2516HD_C) ||(product == TD_2524HD_C) ||(product == TD_2532HD_C) || (product == TD_2704AS_SL) || (product == TD_2704AS_SL_A))
		{
			m_cfgScRemaining.Show(false, false);
			m_cfgScRemainNum.Show(false, false);
			m_cfgScAll.Show(false,false);
		}

		if (0 < m_netVideoInputNum)
		{
			m_cfgScRemaining.Show(false, false);
			m_cfgScRemainNum.Show(false, false);
		}

		listAll.CreateListFocusMap();

#if defined(__CHIPGM__)
		if (TD_2704AS_SL == product)
		{
			listAll.Show(false,false);
		}
#endif
	}
	m_pQualityLevelValue = NULL;
	m_pBitStreamValue = NULL;

	if (!IsSupportSubCode())
	{
		for (int i = 0; i < effectChnn; i++)
		{
			pCombo = NULL;
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i, COL_RESOLUTION));
			pCombo->SetEnable(false, false);

			assert(pCombo);
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i, COL_RATE));
			pCombo->SetEnable(false, false);

#ifdef __DVR_PROFESSIONAL__
			assert(pCombo);
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i, COL_STREAM_TYPE));
#endif

			assert(pCombo);
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i, COL_QUALITY));
			pCombo->SetEnable(false, false);

#ifdef __DVR_PROFESSIONAL__
			assert(pCombo);
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i, COL_MAX_BIT_STREAM));
#endif
		}

		m_cfgScRemaining.Show(false, false);
		m_cfgScRemainNum.Show(false, false);
	}

	if (!IsSupportSubCode())
	{
		for (int i = 0; i < effectChnn; i++)
		{
			pCombo = NULL;
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i, COL_RESOLUTION));
			pCombo->SetEnable(false, false);
		}
	}


	PRODUCT_TYPE productID = CProduct::Instance()->ProductType();

	if ((TD_2704HD == productID)/* || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)*/
		|| (TD_2304SS_C == productID) || (TD_2308SS_C == productID) || (TD_2316SS_C == productID) || (TD_2616LE == productID) 
		|| (TD_2508HE_B_3521 == productID) || (TD_2524HE_C == productID) || (TD_2532HE_C == productID)
		|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
		/*|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) */
		|| (TD_2732TD_PL == productID)
		|| (TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2716AS_S == productID)
		|| (TD_2704AS_S_A == productID) || (TD_2708AS_S_A == productID) || (TD_2716AS_S_A == productID)
		|| (TD_2704AS_SL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
		|| (TD_2704AS_SL_A == productID) || (TD_2708AS_SL_A == productID) || (TD_2716AS_SL_A == productID)
		/*|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)*/
		|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
		|| (TD_2004AL == productID) || (TD_2008AS == productID))
	{
		m_cfgListAll.Show(false,false);
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
	//m_pDeviceSubeEncodeAll = NULL;
}

void CCfgNetworkStreamDlg::OnPostInitial()
{
	//获取数字通道子码流支持的分辨率配置信息
	
	CCfgDlg::OnPostInitial();

	memset(&m_netChennelEncInfo, 0, sizeof(NET_CHANNEL_ENC_INFO) * s_loginSuccessReply.productInfo.netVideoInputNum);

	if(s_loginSuccessReply.productInfo.netVideoInputNum > 0)
	{
		MESSAGE_DATA msgData;
		unsigned long streamType = 1;
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
	if ( m_localVideoInputNum > 0 )
	{
		unsigned long subLen = 0;
		m_reslutionNum = CCalculateSubEncode::Instance()->GetResNum();
		subLen = m_reslutionNum*m_localVideoInputNum;
		assert(subLen>0);
		m_supportSubEncInfo = new SUPPORT_SUB_ENC_INFO[subLen];
		subLen *= sizeof(SUPPORT_SUB_ENC_INFO);
		memset(m_supportSubEncInfo,0,subLen);

		if((0 < s_loginSuccessReply.productInfo.localVideoInputNum) && IsSupportSubCode())
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));

			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SECOND_SUPPORT_INFO, NULL, 0, msgData);

			assert(msgData.dataLen >= sizeof(SUPPORT_SUB_ENC_INFO) * s_loginSuccessReply.productInfo.localVideoInputNum);
			assert(subLen == (msgData.dataLen-sizeof(PACKCMD)) );

			memcpy(m_supportSubEncInfo, msgData.pData + sizeof(PACKCMD), subLen);

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		}
	}
	
}


void CCfgNetworkStreamDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	m_rowChnnMap.clear();
	

    (NULL != m_supportSubEncInfo)?
		delete [] m_supportSubEncInfo,
		m_supportSubEncInfo = NULL:
		NULL;
}

void CCfgNetworkStreamDlg::UpdateView(bool bRefresh)
{
	// 除配置外的其他数据初始化数据设置
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListAll.GetItemWnd(0, COL_CHNN));
	if( pCheckAll )
	{
		pCheckAll->SetCheck( false, bRefresh );
	}

	CCfgDlg::UpdateView( bRefresh );
}


void CCfgNetworkStreamDlg::DoOther()
{
	ULONGLONG CH = 0;
	int localVideoInputNum = 0;
	int netVideoInputNum = 0;
	int maxChnn = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	//先处理模拟通道的信息
	maxChnn = effectChnn;

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	//读取数据 （与分辨率，帧率相关）
	CCfgDlg::GetConfig( NCFG_ITEM_ENCODE_MINOR_SUPPORT_PROPERTY, &pTempData, tempDataLen);
	assert(tempDataLen > 0);
	NCFG_INFO_SUPPORT_FRAME_RATE *pFrameRateData = (reinterpret_cast<NCFG_INFO_SUPPORT_FRAME_RATE*>(pTempData));

	m_videoFormat = CCfgDlg::s_loginSuccessReply.productInfo.videoFormat;
	if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
	{
		m_maxChnnRate = pFrameRateData->chnnRatePalOnCif;
		assert(m_maxChnnRate <= 25*20);
		m_totalRate = pFrameRateData->totalRatePalOnCif;
		m_defauChnnRateNum = pFrameRateData->defauChnnRateNumPal;
	}
	else if (VIDEO_FORMAT_NTSC == (VIDEO_FORMAT)m_videoFormat)
	{
		m_maxChnnRate = pFrameRateData->chnnRateNtscOnCif;
		assert(m_maxChnnRate <= 30 * 20);
		m_totalRate = pFrameRateData->totalRateNtscOnCif;
		m_defauChnnRateNum = pFrameRateData->defauChnnRateNumNtsc;
	}
	else
	{
		assert(false);
		m_maxChnnRate = 25;
		m_totalRate = m_maxChnnRate*m_channelNum;
		m_defauChnnRateNum = 25;
	}

	/*if (IsSupportSubCode())
	{
		m_pChnnSubCode = new CHANNEL_SUBCODE[m_channelNum];
		memset(m_pChnnSubCode,0,sizeof(CHANNEL_SUBCODE)*m_channelNum);
		CCfgDlg::GetConfig( NCFG_ITEM_MINOR_CHANNEL_SUBCODE_PARAM_EX, &pTempData, tempDataLen);
		assert(tempDataLen > 0);
		CHANNEL_SUBCODE * pChSubCode = (reinterpret_cast<CHANNEL_SUBCODE*>(pTempData));
		memcpy(m_pChnnSubCode,pChSubCode,tempDataLen);
	} 
	else*/
	{
		m_videoResolutionMask = pFrameRateData->dwResolution;
		m_defauResolution = pFrameRateData->defaResolution;
	}
	
	if (m_videoResolutionMask & ~(DVRVIDEO_SIZE_CIF | DVRVIDEO_SIZE_QCIF | DVRVIDEO_SIZE_QVGA))
	{
		m_cfgScRemaining.Show(false, false);
		m_cfgScRemainNum.Show(false, false);
	}

	//读取数据 （与码流相关）
	pTempData = NULL;
	tempDataLen = 0;
	CCfgDlg::GetConfig(NCFG_ITEM_MINOR_SUPPORT_QULITY_LEVELNUM, &pTempData, tempDataLen);
	assert(tempDataLen > 0);
	m_qualityLevelNum = tempDataLen/sizeof(unsigned long) - 1;
	unsigned long *pTempValue = reinterpret_cast<unsigned long *>(pTempData);
	m_defauQualityLevel = pTempValue[m_qualityLevelNum];
	m_pQualityLevelValue = new unsigned long [m_qualityLevelNum];
	memcpy(m_pQualityLevelValue, pTempData, tempDataLen - sizeof(unsigned long));	
	pTempData = NULL;
	tempDataLen = 0;
	m_videoEncodeMode = 0;
	CCfgDlg::GetConfig(NCFG_ITEM_MINOR_SUPPORT_ENCODEMODE, &pTempData, tempDataLen);
	assert(tempDataLen/sizeof(unsigned long) == 2);
	pTempValue = reinterpret_cast<unsigned long *>(pTempData); 
	m_videoEncodeMode = pTempValue[0];
	m_defaVideoEncodeMode = pTempValue[1];
	pTempData = NULL;
	tempDataLen = 0;
	m_bitStreamCount = 0;
	CCfgDlg::GetConfig(NCFG_ITEM_MINOR_SUPPORT_BITRATE_RANGE, &pTempData, tempDataLen);
	m_bitStreamCount = tempDataLen/sizeof(unsigned long) - 1;
	assert(m_bitStreamCount > 0);
	pTempValue = reinterpret_cast<unsigned long *>(pTempData);
	m_defaBitStreamValue = pTempValue[m_bitStreamCount];
	m_pBitStreamValue = new unsigned long [m_bitStreamCount];
	memcpy(m_pBitStreamValue, pTempData, tempDataLen - sizeof(unsigned long));		

	CCfgDlg::GetConfig(NCFG_ITEM_ENCODE_SIZE_MINOR_ALL_CH, &pTempData, tempDataLen);
	unsigned long bAllCH = *reinterpret_cast<unsigned long *>(pTempData);

	//int * pRateArr = NULL;
	int rate = 0;
	//设置数据
	{
		bool bRefresh = false;
		GUI::CListCtrl& list = m_cfgNetworkStreamList;
		char str[128] = {0};

		//if ( IsSupportSubCode() )
		//{
		//	//pRateArr = new int[m_channelNum];
		//	//for(int rateIndex=0;rateIndex<m_channelNum;++rateIndex)
		//	//{
		//	//	pRateArr[rateIndex] = RealRate(m_maxChnnRate,(VIDEO_SIZE)m_pChnnSubCode[rateIndex].dfVS);
		//	//	//int rate = 0;
		//	//	if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
		//	//	{
		//	//		pRateArr[rateIndex] = (pRateArr[rateIndex] <= 25) ? pRateArr[rateIndex] : 25;
		//	//	}
		//	//	else
		//	//	{
		//	//		pRateArr[rateIndex] = (pRateArr[rateIndex] <= 30) ? pRateArr[rateIndex] : 30;
		//	//	}	
		//	//}
		//} 
		//else
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

		for (int i = 0; i < localVideoInputNum; ++i)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RATE));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			if ( IsSupportSubCode() )
			{
				/*for (int value = 1; value < pRateArr[i] + 1; ++value)
				{
					sprintf(str, "%d", value);
					pCombo->AddItem(str, value);
				}*/
				unsigned int value =1;
				for (; (value <= 30)&&(value <= m_supportSubEncInfo[i*m_reslutionNum].usMaxRate); value++)
				{
					sprintf(str, "%d", value);
					pCombo->AddItem(str, value);
				}
				pCombo->SetCurItemData((value-1), false);
			}
			else
			{
				for (int value = 1; value < rate + 1; ++value)
				{
					sprintf(str, "%d", value);
					pCombo->AddItem(str, value);
				}
				pCombo->SetCurItemData(rate, false);
			}

			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			if(IsSupportSubCode())
			{
                AddSubResolutionItem(m_supportSubEncInfo+i*m_reslutionNum,pCombo);
			}
			else
			{
				AddResolutionItem(m_videoResolutionMask, pCombo);
			}
			
			if (bAllCH)
			{
				pCombo->SetEnable(false, false);
			}
#if defined(__CHIPGM__)
			if (TD_2704AS_SL == CProduct::Instance()->ProductType())
			{
				pCombo->SetEnable(false, false);
			}
#endif

			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_QUALITY));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			AddQualityItem(m_pQualityLevelValue, m_qualityLevelNum, pCombo);
			pCombo->SetEnable(true, bRefresh);

#ifdef __DVR_PROFESSIONAL__	
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_STREAM_TYPE));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			AddEncodeModeItem(m_videoEncodeMode, pCombo);
#if defined(__CHIPGM__)
			if (TD_2704AS_SL == CProduct::Instance()->ProductType())
			{
				pCombo->SetEnable(false, false);
			}
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
			
			if ( IsSupportSubCode() )
			{   //添加配置的范围以CCalculateSubEncode中的为准，范围以配置项NCFG_ITEM_MINOR_SUPPORT_BITRATE_RANGE为准
				AddSubMixBitStreamItem(pCombo,i,m_supportSubEncInfo[i*m_reslutionNum].ulDefBitrate);
			}
			else
			{
#if defined(__CHIPGM__)
				AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo, 512);
#else
				AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo);
#endif
			}
#if defined(__CHIPGM__)
			if (TD_2704AS_SL == CProduct::Instance()->ProductType())	
			{
				pCombo->SetEnable(false, false);
			}
#endif
#endif
		}
	}

	//设置网络通道
	{
		GUI::CComboCtrl* pCombo = NULL;
		bool bRefresh = false;

		char str[128] = {0};
		int rate = 30;
		if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
		{
			rate = 25;
		}

		for (int chnn=localVideoInputNum, i=chnn; chnn<localVideoInputNum + netVideoInputNum; chnn++)
		{
			if (((CH >> chnn) & 0x01))
			{
				unsigned long videoSize = 0;
				
				if (1 == m_netChennelEncInfo[chnn - localVideoInputNum].online)
				{
					for (int index=0; index<32; index++)
					{
						videoSize |= m_netChennelEncInfo[chnn - localVideoInputNum].encInfo[index].videoSize;
					}
				}
				else
				{
					videoSize = DVRVIDEO_SIZE_CIF | DVRVIDEO_SIZE_D1 | DVRVIDEO_SIZE_HD720 | DVRVIDEO_SIZE_HD1080 | DVRVIDEO_SIZE_960H | DVRVIDEO_SIZE_960P;
				}

				//分辨率
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(i,COL_RESOLUTION));
				pCombo->RemoveAllItem(bRefresh);
				AddResolutionItem(videoSize, pCombo);

				//帧率
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(i,COL_RATE));
				pCombo->RemoveAllItem(bRefresh);
				for (int value = 1; value < rate + 1; ++value)
				{
					sprintf(str, "%d", value);
					pCombo->AddItem(str, value);
				}
				pCombo->SetEnable(false, bRefresh);

				//画质
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(i,COL_QUALITY));
				assert(pCombo);
				pCombo->RemoveAllItem(bRefresh);
				AddQualityItem(m_pQualityLevelValue, m_qualityLevelNum, pCombo);
				pCombo->SetEnable(false, bRefresh);
				pCombo->SetCurItem(4, bRefresh);

				//编码类型
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(i,COL_STREAM_TYPE));
				pCombo->RemoveAllItem(bRefresh);
				AddEncodeModeItem(VIDEO_ENCODE_MODE_CBR, pCombo);

				//码流大小
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(i,COL_MAX_BIT_STREAM));
				pCombo->RemoveAllItem(bRefresh);
				AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo, 12288);

				i++;
			}
		}
	}

	//if (IsSupportSubCode())//暂时不支持 all 选项 dvr没有All选项
	//{

	//}
	//else
	//{
	//	if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	//	{
			bool bRefresh = false;
			GUI::CListCtrl& listAll = m_cfgListAll;	
			char str[128] = {0};
			rate = RealRate(m_maxChnnRate, (VIDEO_SIZE)m_defauResolution);
			if (VIDEO_FORMAT_PAL == (VIDEO_FORMAT)m_videoFormat)
			{
				rate = (rate > 25) ? 25 : rate;
			}
			else
			{
				rate = (rate > 30) ? 30 : rate;
			}
			//assert(m_defauChnnRateNum <= rate);
			int i = 0;
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd( i, COL_RATE));
			pCombo->RemoveAllItem(bRefresh);
			for (int value = 1; value < rate + 1; ++value)
			{
				if (0 < m_netVideoInputNum)
				{
					sprintf(str, "%d", value);
				}
				else
				{
					sprintf(str, "%dX%d", value, m_channelNum);
				}

				pCombo->AddItem(str, value);					
			}
			pCombo->SetCurItemData(m_defauChnnRateNum, bRefresh);
			pCombo->SetEnable(true, bRefresh);


			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd( i, COL_RESOLUTION));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			pCombo->AddItem("CIF", DVRVIDEO_SIZE_CIF);
			pCombo->SetCurItemData(DVRVIDEO_SIZE_CIF, bRefresh);
			pCombo->SetEnable(true, bRefresh);

			/*AddResolutionItem(m_videoResolutionMask, pCombo);
			pCombo->SetCurItemData(m_defauResolution, bRefresh);*/

			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd( i, COL_QUALITY));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			AddQualityItem(m_pQualityLevelValue, m_qualityLevelNum, pCombo);
			pCombo->SetCurItemData(m_defauQualityLevel, bRefresh);	
			pCombo->SetEnable(true, bRefresh);

#ifdef __DVR_PROFESSIONAL__	
			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd(i,COL_STREAM_TYPE));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			if (0 < m_netVideoInputNum)
			{
				AddEncodeModeItem(VIDEO_ENCODE_MODE_CBR, pCombo);
				pCombo->SetCurItemData(VIDEO_ENCODE_MODE_CBR, bRefresh);
			}
			else
			{
				AddEncodeModeItem(m_videoEncodeMode, pCombo);
				pCombo->SetCurItemData(m_defaVideoEncodeMode, bRefresh);
			}

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

			pCombo = dynamic_cast<GUI::CComboCtrl*>(listAll.GetItemWnd(i,COL_MAX_BIT_STREAM));
			assert(pCombo);
			pCombo->RemoveAllItem(bRefresh);
			if (0 < netVideoInputNum)
			{
				AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo, 512);
			}
			else
			{
				if ((m_videoResolutionMask & DVRVIDEO_SIZE_D1) || (m_videoResolutionMask & DVRVIDEO_SIZE_960H) || (m_videoResolutionMask & DVRVIDEO_SIZE_HD720) || (m_videoResolutionMask & DVRVIDEO_SIZE_HD1080))
				{
					AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo, m_pBitStreamValue[m_bitStreamCount - 1]);
				}
				else
				{
					AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo);
				}
			}

			pCombo->SetCurItemData(m_defaBitStreamValue, bRefresh);
#endif
		//}	
	/*}*/

	//m_frameRate.Initial((VIDEO_FORMAT)m_videoFormat, s_loginSuccessReply.productInfo.localVideoInputNum, m_totalRate, m_maxChnnRate, ((0x01 << m_localVideoInputNum) - 1));
	if (s_loginSuccessReply.productInfo.localVideoInputNum > 0)
	{
		m_frameRate.Initial((VIDEO_FORMAT)m_videoFormat,\
			s_loginSuccessReply.productInfo.localVideoInputNum,\
			m_totalRate,\
			m_maxChnnRate,\
			((0x01 << m_localVideoInputNum) - 1));
	}
	//if(s_loginSuccessReply.productInfo.netVideoInputNum > 0)
	//{
	//	m_ipcFrameEncode.Initial(s_loginSuccessReply.productInfo.netVideoInputNum, \
	//		m_ipcSupportEncode.maxTotalBitRate, m_ipcSupportEncode.maxTotalRateOnVGA, \
	//		m_pBitStreamValue, m_bitStreamCount, m_ipcSupportEncode.defaResolution, \
	//		m_ipcSupportEncode.defaChnnRate, m_ipcSupportEncode.defaBitRate);
	//}
}


void CCfgNetworkStreamDlg::EndOther()
{
	m_frameRate.Quit();
	/*m_ipcFrameEncode.Quit();*/
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
}


bool CCfgNetworkStreamDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	if (NCFG_ITEM_ENCODE_MINOR == cfgID || NCFG_ITEM_ENCODE_MINOR_EX == cfgID)
	{
		assert(NULL != pData);
		assert(len > 0);
		GUI::CListCtrl& list = m_cfgNetworkStreamList;

		ULONGLONG CH = 0;
		int localVideoInputNum = 0, netVideoInputNum = 0;
		int maxChnn = 0, effectChnn = 0;

		IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

		effectChnn = localVideoInputNum;
		maxChnn = localVideoInputNum;

		switch (cfgID)
		{
		case NCFG_ITEM_ENCODE_MINOR:
			{
				assert(len==sizeof(ENCODE_INFO)*m_channelNum);

				GUI::CComboCtrl* pCombo = NULL;
				const ENCODE_INFO* pInfo = reinterpret_cast<const ENCODE_INFO*>(pData);
                //int rate = 0;
				for (int i = 0; i < localVideoInputNum; i++)
				{
					int index = m_rowChnnMap.find(i)->second;

					pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));
					assert(pCombo);

					SUPPORT_SUB_ENC_INFO *pTempRes = m_supportSubEncInfo+i*m_reslutionNum;
					unsigned int uiRes =0;
					if (IsSupportSubCode())
					{
						for (; uiRes < m_reslutionNum; uiRes++)
						{
							pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));
							assert(pCombo);

							if ((pTempRes[uiRes].ulRes > 0 ) && ((pInfo[i].resolution)&(pTempRes[uiRes].ulRes) ) )
							{
								pCombo->SetCurItemData(pInfo[i].resolution,bRefresh);
								break;
							}
							else
							{
								//assert(false);
								pCombo->SetCurItem(0, bRefresh);
							}
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

					pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RATE));
					assert(pCombo);
					int rate = 0;
					if ( IsSupportSubCode() )
					{
						/*rate = pInfo[index].rate;*/
						pInfo[index].rate < pTempRes[uiRes].usMaxRate ?
							rate = pInfo[index].rate:
							rate = pTempRes[uiRes].usMaxRate;				
					} 
					else
					{
						if (UsedRate(pInfo[index].rate, (VIDEO_SIZE)(pInfo[index].resolution)) > m_maxChnnRate)
						{
							rate = RealRate(m_maxChnnRate, (VIDEO_SIZE)(pInfo[index].resolution));				
						}
						else
						{
							rate = pInfo[index].rate;
						}
					}
					pCombo->SetCurItemData(rate,bRefresh);
					m_checkEncInfo[i].usRate = rate;

					pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_QUALITY));
					assert(pCombo);
					if (IsInQualityLevel(pInfo[index].quality))
					{
						pCombo->SetCurItemData(pInfo[index].quality,bRefresh);
					}
					else	
					{
						assert(false);
						pCombo->SetCurItem(0, bRefresh);
					}
#ifdef __DVR_PROFESSIONAL__	
					if (VIDEO_ENCODE_MODE_VBR == pInfo[index].encodeType)
					{
						if (!pCombo->IsEnable())
						{
							pCombo->SetEnable(true, bRefresh);
						}
					}
					else
					{
						if (pCombo->IsEnable())
						{
							pCombo->SetEnable(false, bRefresh);
						}
					}
#else
					pCombo->SetEnable(true, bRefresh);
#endif

#ifdef __DVR_PROFESSIONAL__	
					pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_STREAM_TYPE));
					assert(pCombo);
					if ((pInfo[index].encodeType)&(m_videoEncodeMode))
					{
						pCombo->SetCurItemData(pInfo[index].encodeType,bRefresh);
					}
					else
					{
						assert(false);
						pCombo->SetCurItem(0, bRefresh);
					}	

					pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_MAX_BIT_STREAM));
					assert(pCombo);
					//设置码流范围之前需要重新设置对应分辨率的码流范围
					if( IsSupportSubCode() )
					{
						if(AddSubMixBitStreamItem(pCombo, i,pTempRes[uiRes].ulRes))
						{
							/*当从大分辨率切换到小分辨率时，再切换之前的最大码率大于切换之后的最大码率时，
							  这时显示异常（就出现最大码率为0的现象）*/
							if (pInfo[i].hBitStream > pTempRes[uiRes].ulDefBitrate)
							{
								pCombo->SetCurItemData(pTempRes[uiRes].ulDefBitrate,bRefresh);
							}
							else
							{
								pCombo->SetCurItemData(pInfo[i].hBitStream,bRefresh);
							}	
						}
						else
						{
							assert(false);
							pCombo->SetCurItem(0, bRefresh);
						}
					}
					else
					{
						if (IsInBitStreamRange(pInfo[i].hBitStream))
						{
							pCombo->SetCurItemData(pInfo[i].hBitStream,bRefresh);
						}
						else
						{
							assert(false);
							pCombo->SetCurItem(0, bRefresh);
						}

					}
					m_checkEncInfo[i].usBitrate = pInfo[i].hBitStream;
#endif

					m_frameRate.AddRate(index, rate, static_cast<VIDEO_SIZE>(pInfo[index].resolution));
				}

// 				for (int chnn = localVideoInputNum, i=localVideoInputNum; chnn < localVideoInputNum + netVideoInputNum; chnn++)
// 				{
// 					if ((CH >> chnn) & 0x01)
// 					{
// 						ChangeNetEncodeInfo(CH, chnn, DVRVIDEO_SIZE_CIF, bRefresh);
// 
// 						int index = m_rowChnnMap.find(i)->second;
// 
// 						GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RATE));
// 						pCombo->SetCurItemData(10,bRefresh);
// 
// 						pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));
// 						pCombo->SetCurItem(0, bRefresh);
// 
// 						pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_QUALITY));
// 						pCombo->SetCurItem(4, bRefresh);
// 
// 						pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_STREAM_TYPE));
// 						pCombo->SetCurItem(0, bRefresh);
// 
// 						pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_MAX_BIT_STREAM));
// 
// 						pCombo->SetCurItem(0, bRefresh);
// 
// 						i++;
// 					}
// 				}


				char str [128] = {0};

				unsigned long remainRate = 0;
#if defined(__CUSTOM_IN__) || defined(__CUSTOM_ZNV__) || defined(__CUSTOM_CNB__)
				PRODUCT_TYPE product = CProduct::Instance()->ProductType();
				if (TD_2716XD_M == product)
				{
					remainRate = m_frameRate.RemainingEx();
				}
				else
#endif
				{
					remainRate = m_frameRate.Remaining();
				}

				sprintf(str, " : %d (CIF).", remainRate);
				m_cfgScRemainNum.SetCaption(str, false );
			}
			return true;
		case NCFG_ITEM_ENCODE_MINOR_EX:
			{
				assert(len==sizeof(ENCODE_INFO_EX) * (m_channelNum + m_netVideoInputNum));

				const ENCODE_INFO_EX* pInfoEx = reinterpret_cast<const ENCODE_INFO_EX*>(pData);
				GUI::CComboCtrl* pCombo = NULL;

				for (int i = 0; i < localVideoInputNum; i++)
				{
					int index = m_rowChnnMap.find(i)->second;
					pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));
					assert(pCombo);
					SUPPORT_SUB_ENC_INFO *pTempRes = m_supportSubEncInfo+i*m_reslutionNum;
					unsigned int uiRes =0;

					if ( IsSupportSubCode() )
					{
						for (; uiRes < m_reslutionNum; uiRes++)
						{
							if ((pTempRes[uiRes].ulRes > 0 ) && ((pInfoEx[index].resolution)&(pTempRes[uiRes].ulRes) ) )
							{
								pCombo->SetCurItemData(pInfoEx[index].resolution,bRefresh);
								break;
							}
							else
							{
								//assert(false);
								pCombo->SetCurItem(0, bRefresh);
							}
						}

					}
					else
					{
						if ((pInfoEx[index].resolution)&(m_videoResolutionMask))
						{
							pCombo->SetCurItemData(pInfoEx[index].resolution,bRefresh);
						}
						else
						{
							assert(false);
							pCombo->SetCurItem(0, bRefresh);
						}
					}
					m_checkEncInfo[index].ulRes = pInfoEx[index].resolution;

					pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RATE));
					assert(pCombo);
					int rate = 0;
					if ( IsSupportSubCode() )
					{
						/*rate = pInfo[index].rate;*/
						pInfoEx[index].rate < pTempRes[uiRes].usMaxRate ? 
							rate = pInfoEx[index].rate:
						    rate = pTempRes[uiRes].usMaxRate;
					} 
					else
					{
						if (UsedRate(pInfoEx[index].rate, (VIDEO_SIZE)(pInfoEx[index].resolution)) > m_maxChnnRate)
						{
							rate = RealRate(m_maxChnnRate, (VIDEO_SIZE)(pInfoEx[index].resolution));				
						}
						else
						{
							rate = pInfoEx[index].rate;
						}
					}
					pCombo->SetCurItemData(rate,bRefresh);
					m_checkEncInfo[i].usRate = rate;


					pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_QUALITY));
					assert(pCombo);
					if (IsInQualityLevel(pInfoEx[index].quality))
					{
						pCombo->SetCurItemData(pInfoEx[index].quality,bRefresh);
					}
					else
					{
						assert(false);
						pCombo->SetCurItem(0, bRefresh);
					}
#ifdef __DVR_PROFESSIONAL__	
					if (VIDEO_ENCODE_MODE_VBR == pInfoEx[index].encodeType)
					{
						if (!pCombo->IsEnable())
						{
							pCombo->SetEnable(true, bRefresh);
						}
					}
					else
					{
						if (pCombo->IsEnable())
						{
							pCombo->SetEnable(false, bRefresh);
						}
					}
#else
					pCombo->SetEnable(true, bRefresh);
#endif

#ifdef __DVR_PROFESSIONAL__	
					pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_STREAM_TYPE));
					assert(pCombo);
					if ((pInfoEx[index].encodeType)&(m_videoEncodeMode))
					{
						pCombo->SetCurItemData(pInfoEx[index].encodeType,bRefresh);
					}
					else
					{
						assert(false);
						pCombo->SetCurItem(0, bRefresh);
					}	

					pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_MAX_BIT_STREAM));
					//设置码流范围之前需要重新设置对应分辨率的码流范围
					assert(pCombo);
					if( IsSupportSubCode() )
					{
						/*AddMIxBitStreamItem(pCombo,pInfo[index].resolution);*/
						if (AddSubMixBitStreamItem(pCombo,index ,pTempRes[uiRes].ulRes))
						{
							//pCombo->SetCurItemData(pTempRes[uiRes].ulDefBitrate,bRefresh);
							pCombo->SetCurItemData(pInfoEx[index].hBitStream,bRefresh);
						}
						else
						{
							assert(false);
							pCombo->SetCurItem(0, bRefresh);
						}

					}
					else
					{
						if (IsInBitStreamRange(pInfoEx[index].hBitStream))
						{
							pCombo->SetCurItemData(pInfoEx[index].hBitStream,bRefresh);
						}
						else
						{
							assert(false);
							pCombo->SetCurItem(0, bRefresh);
						}
					}
					m_checkEncInfo[index].usBitrate = pInfoEx[index].hBitStream;
#endif
					m_frameRate.AddRate(index, rate, static_cast<VIDEO_SIZE>(pInfoEx[index].resolution));
				}

				for (int chnn = localVideoInputNum, i=localVideoInputNum; chnn < localVideoInputNum + netVideoInputNum; chnn++)
				{
					if ((CH >> chnn) & 0x01)
					{
						int index = m_rowChnnMap.find(i)->second;
						bool bIPCOnline = (1 == m_netChennelEncInfo[index - localVideoInputNum].online) ? true : false;

						unsigned long resolution = pInfoEx[index].resolution;
						ChangeNetEncodeInfo(CH, index, resolution, bRefresh);

						GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RATE));
						int iCurRate = pCombo->GetCurItemData();
						if (iCurRate > pInfoEx[index].rate)
						{
							pCombo->SetCurItemData(pInfoEx[index].rate,bRefresh);
						}

						if (!bIPCOnline)
						{
							pCombo->SetEnable(false,bRefresh);
						}
						else
						{
							pCombo->SetEnable(true,bRefresh);
						}

						pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_RESOLUTION));
						pCombo->SetCurItemData(resolution, bRefresh);
						if (!bIPCOnline)
						{
							pCombo->SetEnable(false,bRefresh);
						}
						else
						{
							pCombo->SetEnable(true,bRefresh);
						}

						pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_QUALITY));
						pCombo->SetCurItemData(pInfoEx[index].quality, bRefresh);
						if (!bIPCOnline)
						{
							pCombo->SetEnable(false,bRefresh);
						}
						else
						{
							pCombo->SetEnable(true,bRefresh);
						}

						if (VIDEO_ENCODE_MODE_VBR == pInfoEx[index].encodeType)
						{
							if (!pCombo->IsEnable())
							{
								pCombo->SetEnable(true, bRefresh);
							}
						}
						else
						{
							if (pCombo->IsEnable())
							{
								pCombo->SetEnable(false, bRefresh);
							}
						}

						pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(i,COL_STREAM_TYPE));

						if ((pInfoEx[index].encodeType)&(m_videoEncodeMode))
						{
							pCombo->SetCurItemData(pInfoEx[index].encodeType, bRefresh);
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

						pCombo->SetCurItemData(pInfoEx[index].hBitStream, bRefresh);
						if (!bIPCOnline)
						{
							pCombo->SetEnable(false,bRefresh);
						}
						else
						{
							pCombo->SetEnable(true,bRefresh);
						}

						i++;
					}
				}


				char str [128] = {0};

				unsigned long remainRate = 0;
#if defined(__CUSTOM_IN__) || defined(__CUSTOM_ZNV__) || defined(__CUSTOM_CNB__)
				PRODUCT_TYPE product = CProduct::Instance()->ProductType();
				if (TD_2716XD_M == product)
				{
					remainRate = m_frameRate.RemainingEx();
				}
				else
#endif
				{
					remainRate = m_frameRate.Remaining();
				}

				sprintf(str, " : %d (CIF).", remainRate);
				m_cfgScRemainNum.SetCaption(str, false );
			}
			return true;
		default:
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CCfgNetworkStreamDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	if ((NCFG_ITEM_ENCODE_MINOR == cfgID) || (NCFG_ITEM_ENCODE_MINOR_EX == cfgID))
	{
		assert(NULL != pData);
		assert(len > 0);
		const GUI::CListCtrl& list = m_cfgNetworkStreamList;
		const GUI::CListCtrl& listAll = m_cfgListAll;

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

		effectChnn = localVideoInputNum;
		maxChnn = localVideoInputNum + netVideoInputNum;

		switch (cfgID)
		{
		case NCFG_ITEM_ENCODE_MINOR:
			{

				int numChnn = s_loginSuccessReply.productInfo.localVideoInputNum;
				assert(len==sizeof(ENCODE_INFO)*numChnn);

				ENCODE_INFO* pInfo = reinterpret_cast<ENCODE_INFO*>(pData);

				for(int i = 0; i < effectChnn; i++)
				{
					GUI::CComboCtrl* pCbRate = NULL;
					GUI::CComboCtrl* pCbResolution = NULL;
					GUI::CComboCtrl* pCbQuality = NULL;
					GUI::CComboCtrl* pCbType = NULL;
					GUI::CComboCtrl* pCbMax = NULL;

					int index = m_rowChnnMap.find(i)->second;

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

					assert(pCbRate);
					assert(pCbResolution);
					assert(pCbQuality);
#ifdef __DVR_PROFESSIONAL__	
					assert(pCbType);
					assert(pCbMax);
#endif

					pInfo[index].rate = pCbRate->GetCurItemData();
					pInfo[index].resolution = pCbResolution->GetCurItemData();
					pInfo[index].quality = pCbQuality->GetCurItemData();
#ifdef __DVR_PROFESSIONAL__	
					pInfo[index].encodeType = pCbType->GetCurItemData();
					pInfo[index].hBitStream = pCbMax->GetCurItemData();
#endif
				}
			}
			return true;
			break;
		case NCFG_ITEM_ENCODE_MINOR_EX:
			{
				int numChnn = s_loginSuccessReply.productInfo.localVideoInputNum + s_loginSuccessReply.productInfo.netVideoInputNum;
				assert(len==sizeof(ENCODE_INFO_EX) * numChnn);

				ENCODE_INFO_EX * pInfoEx = reinterpret_cast<ENCODE_INFO_EX *>(pData);

				GUI::CComboCtrl* pCbRate = NULL;
				GUI::CComboCtrl* pCbResolution = NULL;
				GUI::CComboCtrl* pCbQuality = NULL;
				GUI::CComboCtrl* pCbType = NULL;
				GUI::CComboCtrl* pCbMax = NULL;

				for(int i = 0; i < localVideoInputNum; i++)
				{
					int index = m_rowChnnMap.find(i)->second;

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

					assert(pCbRate);
					assert(pCbResolution);
					assert(pCbQuality);
#ifdef __DVR_PROFESSIONAL__	
					assert(pCbType);
					assert(pCbMax);
#endif

					pInfoEx[index].rate			= pCbRate->GetCurItemData();
					pInfoEx[index].resolution	= pCbResolution->GetCurItemData();
					pInfoEx[index].quality		= pCbQuality->GetCurItemData();
#ifdef __DVR_PROFESSIONAL__	
					pInfoEx[index].encodeType	= pCbType->GetCurItemData();
					pInfoEx[index].hBitStream	= pCbMax->GetCurItemData();
#endif
				}

				for (int chnn = localVideoInputNum, i=localVideoInputNum; chnn < localVideoInputNum + netVideoInputNum; chnn++)
				{
					if ((CH >> chnn) & 0x01)
					{
						int index = m_rowChnnMap.find(i)->second;

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

						if (1 == m_netChennelEncInfo[index - localVideoInputNum].online)
						{
							pInfoEx[index].rate			= pCbRate->GetCurItemData();
							pInfoEx[index].resolution	= pCbResolution->GetCurItemData();
							pInfoEx[index].quality		= pCbQuality->GetCurItemData();
#ifdef __DVR_PROFESSIONAL__	
							pInfoEx[index].encodeType	= pCbType->GetCurItemData();
							pInfoEx[index].hBitStream	= pCbMax->GetCurItemData();
#endif
						}

						i++;
					}
				}

			}
			return true;
			break;
		default:
			break;
		}
	}
	
	return false;
}


BEGIN_MSG_MAP(CCfgNetworkStreamDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgNetworkStreamList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_cfgListAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()

unsigned long CCfgNetworkStreamDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	unsigned long ret = KEY_VIRTUAL_MSG_STOP;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	//2010-03-03 17:57:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListAll.GetItemWnd(0, COL_CHNN));
	assert(pCheckAll);
	if( pCheckAll->IsCheck() )
	{
		pCheckAll->SetCheck(false, true);
	}

	//2010-03-02 11:00:00 YSW
	//只有点击帧率和分辨率时，才处理资源部分
	if ((COL_RATE == col) || (COL_RESOLUTION == col))
	{
		int index = m_rowChnnMap.find(row)->second;
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_RATE));
		assert(pCombo);
		unsigned char rate = static_cast<unsigned char>(pCombo->GetCurItemData());

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_RESOLUTION));
		assert(pCombo);
		VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pCombo->GetCurItemData());

		//assert (row < CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum);
		if(index < s_loginSuccessReply.productInfo.localVideoInputNum)
		{
			if (IsSupportSubCode())
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

				CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SECOND_CHECK_ENC_INFO, pMsgData, sizeof(CHECK_ENC_INFO) * localVideoInputNum + sizeof(unsigned long), msgData);

				CHECK_ENC_INFO * pCheckEncInfoChecked = reinterpret_cast<CHECK_ENC_INFO *>( msgData.pData + sizeof(PACKCMD));

				if (0 != memcmp(pCheckEncInfo, pCheckEncInfoChecked, sizeof(CHECK_ENC_INFO) * localVideoInputNum))
				{
					GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_RESOLUTION));
					size = (VIDEO_SIZE)pCheckEncInfoChecked[row].ulRes;
					pCombo->SetCurItemData(size, true);

					pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_RATE));
					rate = pCheckEncInfoChecked[row].usRate;
					pCombo->SetCurItemData(rate, true);

					//MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				}

				m_checkEncInfo[row].usRate = rate;
				m_checkEncInfo[row].ulRes = size;

				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			}
			else
			{
				bool bTryRate = false;
#if defined(__CUSTOM_IN__) || defined(__CUSTOM_ZNV__) || defined(__CUSTOM_CNB__)
				PRODUCT_TYPE product = CProduct::Instance()->ProductType();
				if (TD_2716XD_M == product)
				{
					bTryRate = m_frameRate.TryRateEx(index, rate, size);
				}
				else
#endif
				{
					bTryRate = m_frameRate.TryRate(index, rate, size);
				}

				if (!bTryRate)
				{
					GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_RESOLUTION));
					assert(pCombo);
					pCombo->SetCurItemData(size, true);

					pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_RATE));
					assert(pCombo);
					pCombo->SetCurItemData(rate, true);

					MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				}

				unsigned long remainRate = 0;

#if defined(__CUSTOM_IN__) || defined(__CUSTOM_ZNV__) || defined(__CUSTOM_CNB__)
				if (TD_2716XD_M == product)
				{
					remainRate = m_frameRate.ModifyRateEx(row, rate, size);
				}
				else
#endif
				{
					remainRate = m_frameRate.ModifyRate(row, rate, size);
				}

				char str [128] = {0};
				sprintf(str, " : %d (CIF).", remainRate);
				m_cfgScRemainNum.SetCaption(str, true);
			}
		}
		else
		{
			//ChangeNetEncodeInfo(CH, index, size, true);
		}
	}
	
#ifdef __DVR_PROFESSIONAL__
	if (COL_RESOLUTION == col)
	{
		GUI::CComboCtrl* pComboR = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_RESOLUTION));
		GUI::CComboCtrl* pComboB = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_MAX_BIT_STREAM));

		PRODUCT_TYPE product = CProduct::Instance()->ProductType();

		int index = m_rowChnnMap.find(row)->second;

		unsigned long size = static_cast<unsigned long>(pComboR->GetCurItemData());

		if (index >= localVideoInputNum)
		{
			ChangeNetEncodeInfo(CH, index, size, true);
		}

		if(IsSupportSubCode() && index < localVideoInputNum)
		{
			//AddMIxBitStreamItem(pComboB, size);
			unsigned long tempMaxBitStream=0;
			AddSubMixBitStreamItem(pComboB,index,size,&tempMaxBitStream);
			pComboB->SetCurItemData(tempMaxBitStream,true);
		}else{
			if (DVRVIDEO_SIZE_CIF == size)
			{
#if defined(__CUSTOM_IL03__)
				pComboB->SetCurItemData(256, true);
#else
				if ((TD_2704HD == product)	 || (TD_2708XE_M == product) || (TD_2716XE_M == product) || (TD_2708XE_MH == product) ||
					(TD_2704XE_L == product) || (TD_2708XD_L == product) || (TD_2716XD_L == product) || (TD_2704XD_M == product) || 
					(TD_2708XD_M == product) || (TD_2716XD_M == product) || (TD_2704XD_P == product) || (TD_2708XD_P == product) ||
					(TD_2716XD_P == product) || (TD_2704XE_S == product) || (TD_2708XE_S == product) || (TD_2716XE_S == product) || 
					(TD_2704XD_S == product) || (TD_2708XD_S == product) || (TD_2716XD_S == product) || (TD_2708XE_P == product) ||
					(TD_2704XS_SL == product) || (TD_2708XS_SL == product) || (TD_2716XS_SL == product)
					/*||(TD_2704AS_SL == product) || (TD_2708AS_SL == product) || (TD_2716AS_SL == product) ||
					(TD_2704AS_S == product) || (TD_2708AS_S == product) || (TD_2716AS_S == product) ||
					(TD_2704AS_S_A == product) || (TD_2708AS_S_A == product) || (TD_2716AS_S_A == product) ||
					(TD_2704TS_S == product) || (TD_2708TS_S == product) || (TD_2716TS_S == product) ||
					(TD_2704TS_PL == product) || (TD_2708TS_PL == product) || (TD_2716TS_PL == product) ||
					(TD_2704TD_P == product) || (TD_2708TD_P == product) || (TD_2716TD_P == product) || 
					(TD_2704TD_PL == product) || (TD_2708TD_PL == product) || (TD_2716TD_PL == product)
					|| (TD_2704TE_M == product) || (TD_2708TE_M == product) || (TD_2716TE_M == product)
					|| (TD_2704AE_SL == product) || (TD_2708AE_SL == product) || (TD_2716AE_SL == product)*/)
				{
					pComboB->SetCurItemData(768, true);
				}
				else
				{
					pComboB->SetCurItemData(512, true);
				}
#endif
			}	
			else if (DVRVIDEO_SIZE_D1 == size)
			{
				pComboB->SetCurItemData(1024,true);
			}
			else if (DVRVIDEO_SIZE_HD1 == size)
			{
				pComboB->SetCurItemData(1024, true);
			}
			else if (DVRVIDEO_SIZE_HD1080 == size)
			{
				if ((TD_2708XE_MH == product) || (TD_2716XD_P == product) || (TD_2708XE_M == product) || (TD_2716XE_M == product))
				{
					pComboB->SetCurItemData(8192, true);
				}
				else
				{
					pComboB->SetCurItemData(4096, true);
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
					pComboB->SetCurItemData(2048, true);
				}
			}
			else if (DVRVIDEO_SIZE_960H == size)
			{
				pComboB->SetCurItemData(1536,true);
			}
			else
			{

			}
		}
	}
#endif

	//当选择定码流时，把画质选择置为不可选状态.
#ifdef __DVR_PROFESSIONAL__	
	if (COL_STREAM_TYPE == col)
	{
		//assert (row < CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum);

		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_STREAM_TYPE));
		assert(pCombo);
		unsigned long type = static_cast<unsigned long>(pCombo->GetCurItemData());

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_QUALITY));
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
#else
	{
		GUI::CComboCtrl*pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_QUALITY));
		assert(pCombo);
		pCombo->SetEnable(true, true);
	}
#endif

	if (!IsModify())
	{
		switch (col)
		{
		case COL_RATE:
		case COL_RESOLUTION:
		case COL_QUALITY:
#ifdef __DVR_PROFESSIONAL__	
		case COL_STREAM_TYPE:
		case COL_MAX_BIT_STREAM:
#endif
			if(IsChange(NCFG_ITEM_ENCODE_MINOR) || IsChange(NCFG_ITEM_ENCODE_MINOR_EX))
			{
				SetModify(true);
			}
			ret = KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			break;
		}
	}

	return ret;
}

unsigned long CCfgNetworkStreamDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	// 点选非All，先检查是否处于All状态，处于则认为做了修改
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListAll.GetItemWnd(0, COL_CHNN));
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCheckAll );
	assert( pCfgDlg );

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	unsigned long ret = KEY_VIRTUAL_MSG_STOP;
	if( pCheckAll->IsCheck() )
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

	//2010-03-02 11:00:00 YSW
	//只有点击帧率和分辨率时，才处理资源部分
	if ((COL_RATE == col) || (COL_RESOLUTION == col))
	{
		//assert (row < CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum);

		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListAll.GetItemWnd(row, COL_RATE));
		assert(pCombo);
		unsigned char rate = static_cast<unsigned char>(pCombo->GetCurItemData());

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListAll.GetItemWnd(row, COL_RESOLUTION));
		assert(pCombo);
		VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pCombo->GetCurItemData());

		if ((0 == localVideoInputNum) && (0 < netVideoInputNum))
		{

		}
		else
		{
			unsigned long maxRate = RealRateEx(m_totalRate/localVideoInputNum, size);
			if (maxRate < rate)
			{
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListAll.GetItemWnd(row, COL_RATE));
				assert(pCombo);
				pCombo->SetCurItemData(maxRate, true);

				MessageBox(m_pStrTable->String(E_STR_ID_TIP_05_03), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			}
		}
	}


#ifdef __DVR_PROFESSIONAL__
	if (COL_RESOLUTION == col)
	{
		GUI::CComboCtrl* pComboR = dynamic_cast<GUI::CComboCtrl*>(m_cfgListAll.GetItemWnd(row, COL_RESOLUTION));
		GUI::CComboCtrl* pComboB = dynamic_cast<GUI::CComboCtrl*>(m_cfgListAll.GetItemWnd(row, COL_MAX_BIT_STREAM));


		PRODUCT_TYPE product = CProduct::Instance()->ProductType();

		VIDEO_SIZE size = static_cast<VIDEO_SIZE>(pComboR->GetCurItemData());
		if (DVRVIDEO_SIZE_CIF == size)
		{
#if defined(__CUSTOM_IL03__)
			pComboB->SetCurItemData(256, true);
#else
			if ((TD_2704HD == product)	 || (TD_2708XE_M == product) || (TD_2716XE_M == product) || (TD_2708XE_MH == product) ||
				(TD_2704XE_L == product) || (TD_2708XD_L == product) || (TD_2716XD_L == product) || (TD_2704XD_M == product) || 
				(TD_2708XD_M == product) || (TD_2716XD_M == product) || (TD_2704XD_P == product) || (TD_2708XD_P == product) ||
				(TD_2716XD_P == product) || (TD_2704XE_S == product) || (TD_2708XE_S == product) || (TD_2716XE_S == product) || 
				(TD_2704XD_S == product) || (TD_2708XD_S == product) || (TD_2716XD_S == product) || (TD_2708XE_P == product) ||
				(TD_2704XS_SL == product) || (TD_2708XS_SL == product) || (TD_2716XS_SL == product)
				/* || (TD_2704AS_SL == product) || (TD_2708AS_SL == product) || (TD_2716AS_SL == product) ||
				(TD_2704AS_S == product) || (TD_2708AS_S == product) || (TD_2716AS_S == product) ||
				(TD_2704TS_S == product) || (TD_2708TS_S == product) || (TD_2716TS_S == product) ||
				(TD_2704TS_PL == product) || (TD_2708TS_PL == product) || (TD_2716TS_PL == product) ||
				(TD_2704TD_P == product) || (TD_2708TD_P == product) || (TD_2716TD_P == product) || 
				(TD_2704TD_PL == product) || (TD_2708TD_PL == product) || (TD_2716TD_PL == product)
				|| (TD_2704TE_M == product) || (TD_2708TE_M == product) || (TD_2716TE_M == product)
				|| (TD_2704AE_SL == product) || (TD_2708AE_SL == product) || (TD_2716AS_SL == product)*/)
			{
				pComboB->SetCurItemData(768, true);
			}
			else
			{
				pComboB->SetCurItemData(512, true);
			}
#endif
		}	
		else if (DVRVIDEO_SIZE_D1 == size)
		{
			pComboB->SetCurItemData(1024,true);
		}
		else if (DVRVIDEO_SIZE_HD1 == size)
		{
			pComboB->SetCurItemData(1024, true);
		}
		else if (DVRVIDEO_SIZE_HD1080 == size)
		{
			if ((TD_2708XE_MH == product) || (TD_2716XD_P == product) || (TD_2708XE_M == product) || (TD_2716XE_M == product))
			{
				pComboB->SetCurItemData(8192, true);
			}
			else
			{
				pComboB->SetCurItemData(4096, true);
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
				pComboB->SetCurItemData(2048, true);
			}
		}
		else if (DVRVIDEO_SIZE_960H == size)
		{
			pComboB->SetCurItemData(1536,true);
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
		//assert (row < CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum);

		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListAll.GetItemWnd(row, COL_STREAM_TYPE));
		assert(pCombo);
		unsigned long type = static_cast<unsigned long>(pCombo->GetCurItemData());

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgListAll.GetItemWnd(row, COL_QUALITY));
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
#else
	{
		GUI::CComboCtrl*pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(row, COL_QUALITY));
		assert(pCombo);
		pCombo->SetEnable(true, true);
	}
#endif

	return ret;
}


void CCfgNetworkStreamDlg::AddResolutionItem(unsigned long dwResolution, CComboCtrl *pCombo)
{
	assert(dwResolution != 0);

// 		DVRVIDEO_SIZE_QCIF	 = 0x0001,	//QCIF
// 		DVRVIDEO_SIZE_CIF	 = 0x0002,	//CIF
// 		DVRVIDEO_SIZE_HD1	 = 0x0004,	//HD1
// 		DVRVIDEO_SIZE_D1	 = 0x0008,	//D1
// 		DVRVIDEO_SIZE_HD1080 = 0x0010,	//1920X1080
// 		DVRVIDEO_SIZE_HD720  = 0x0020,	//1280X720
// 		DVRVIDEO_SIZE_960H   = 0x0040,


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
		pCombo->AddItem("D1", DVRVIDEO_SIZE_D1);
	}			
	
	if (dwResolution & DVRVIDEO_SIZE_VGA)
	{
		pCombo->AddItem("VGA", DVRVIDEO_SIZE_VGA);
	}

	if (dwResolution & DVRVIDEO_SIZE_QVGA)
	{
		pCombo->AddItem("QVGA", DVRVIDEO_SIZE_QVGA);
	}

#ifdef __CUSTOM_US02__
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

	if (dwResolution & DVRVIDEO_SIZE_HD1080)
	{
		pCombo->AddItem("1080P", DVRVIDEO_SIZE_HD1080);
	}

// 	for (int i = 0; i < sizeof(unsigned long)*8; ++i)
// 	{
// 		unsigned long tmp = ((unsigned long)1) << i;
// 		if (dwResolution & tmp)
// 		{
// 			if (tmp & DVRVIDEO_SIZE_QCIF)
// 			{
// 				pCombo->AddItem("QCIF", DVRVIDEO_SIZE_QCIF);
// 			}
// 			else if (tmp & DVRVIDEO_SIZE_CIF)
// 			{
// 				pCombo->AddItem("CIF", DVRVIDEO_SIZE_CIF);
// 			}
// 			else if (tmp & DVRVIDEO_SIZE_HD1)
// 			{
// 				pCombo->AddItem("HD1", DVRVIDEO_SIZE_HD1);
// 			}
// 			else if (tmp & DVRVIDEO_SIZE_D1)
// 			{
// 				pCombo->AddItem("D1", DVRVIDEO_SIZE_D1);
// 			}			
// 			else if (tmp & DVRVIDEO_SIZE_HD1080)
// 			{
// 				pCombo->AddItem("1080P", DVRVIDEO_SIZE_HD1080);
// 			}
// 			else if (tmp & DVRVIDEO_SIZE_HD720)
// 			{
// 				pCombo->AddItem("720P", DVRVIDEO_SIZE_HD720);
// 			}
// 			else if (tmp & DVRVIDEO_SIZE_960H)
// 			{
// 				pCombo->AddItem("WD1", DVRVIDEO_SIZE_960H);
// 			}
// 			else if (tmp & DVRVIDEO_SIZE_VGA)
// 			{
// 				pCombo->AddItem("VGA", DVRVIDEO_SIZE_VGA);
// 			}
// 		}		
// 	}
}

void CCfgNetworkStreamDlg::AddQualityItem(unsigned long *pQualityValue, unsigned long count, CComboCtrl *pCombo)
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
			snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_LOWER ) );
			pCombo->AddItem( str,  pQualityValue[i]);
		}
		else if (IMAGE_QUALITY_LOW == pQualityValue[i])
		{
			snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_LOW ) );
			pCombo->AddItem(str, pQualityValue[i]);
		}
		else if (IMAGE_QUALITY_MEDIUM == pQualityValue[i])
		{
			snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_MEDIUM ) );
			pCombo->AddItem( str, pQualityValue[i] );
		}
		else if (IMAGE_QUALITY_HEIGHTER == pQualityValue[i])
		{
			snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_HIGHER ) );
			pCombo->AddItem( str, pQualityValue[i] );
		}
		else if (IMAGE_QUALITY_HEIGHTEST == pQualityValue[i])
		{
			snprintf( str, 128, "%s", m_pStrTable->String( E_STR_ID_HIGHEST ) );
			pCombo->AddItem( str, pQualityValue[i] );
		}	
		else 
		{
			assert(false);
		}
	}
}

void CCfgNetworkStreamDlg::AddEncodeModeItem(unsigned long videoEncodeMode, CComboCtrl *pCombo)
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
void CCfgNetworkStreamDlg::AddBitStreamLocalItem(unsigned long *pBitValue, unsigned long count, CComboCtrl *pCombo, unsigned long resolution)
{
	assert(pBitValue != NULL);
	assert(count > 0);
	char str[128] = {0};

	unsigned long maxBitRate = 0;
	unsigned long minBitRate = 0;
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
		case DVRVIDEO_SIZE_CIF:
			maxBitRate = 1024;
			break;
		default:
			maxBitRate = 2*1024;
			break;
		}

		/*if (-1 < channel && channel <= g_p_login_succ->productInfo.netVideoInputNum + g_p_login_succ->productInfo.localVideoInputNum)
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
		}*/
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
		/*if(IsMixDvr())
		{
			if((0 != minBitRate) && (pBitValue[i] < minBitRate))
			{
				continue;
			}
		}*/

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

void CCfgNetworkStreamDlg::AddBitStreamItem(unsigned long *pBitValue, unsigned long count, CComboCtrl *pCombo, unsigned long ulMaxBirRate/*=768*/)
{
	assert(pBitValue != NULL);
	assert(count > 0);
	char str[128] = {0};

	for (int i = 0; i < count; ++i)
	{
		if (ulMaxBirRate >= pBitValue[i])
		{
			sprintf(str, "%d kbps", pBitValue[i]);
			pCombo->AddItem(str, pBitValue[i]);
		}
	}
	pCombo->SetDropDownCount(7);
}

bool CCfgNetworkStreamDlg::IsInQualityLevel(unsigned long value)
{
	for (int i = 0; i < m_qualityLevelNum; ++i)
	{
		if ((m_pQualityLevelValue[i] == value)&&(value != 0))
		{
			return true;
		}
	}
	return false;
}


bool CCfgNetworkStreamDlg::IsInBitStreamRange(unsigned long value)
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

void CCfgNetworkStreamDlg::ChangeNetEncodeInfo(ULONGLONG &CH, unsigned long ulToChnn, unsigned long &toVideoSize, bool bRefresh)
{
	if (m_localVideoInputNum <= ulToChnn && ulToChnn < m_localVideoInputNum + m_netVideoInputNum)
	{
		int chnn = m_localVideoInputNum;
		int i = chnn;
		for (; chnn < m_localVideoInputNum + m_netVideoInputNum; chnn++)
		{
			if (ulToChnn == chnn)
			{
				break;
			}

			if ((CH >> chnn) & 0x01)
			{
				i++;
			}
		}

		unsigned long ulRate = 0;
		unsigned long ulMaxBitRate = 0;
		char str[120] = {0};

		if (1 == m_netChennelEncInfo[chnn- m_localVideoInputNum].online)
		{
			int j = 0;
			for (j = 0; j < 32; j++)
			{
				if (toVideoSize == m_netChennelEncInfo[chnn- m_localVideoInputNum].encInfo[j].videoSize)
				{
					ulRate = m_netChennelEncInfo[chnn - m_localVideoInputNum].encInfo[j].ucRate;
					ulMaxBitRate = m_netChennelEncInfo[chnn - m_localVideoInputNum].encInfo[j].maxBitRate;
					break;
				}
			}

			if(32 == j)
			{
				toVideoSize = m_netChennelEncInfo[chnn - m_localVideoInputNum].encInfo[0].videoSize;
				ulRate = m_netChennelEncInfo[chnn - m_localVideoInputNum].encInfo[0].ucRate;
				ulMaxBitRate = m_netChennelEncInfo[chnn - m_localVideoInputNum].encInfo[0].maxBitRate;
			}
		}
		else
		{
			ulRate = 30;
			if (VIDEO_FORMAT_PAL == g_video_format)
			{
				ulRate = 25;
			}
			ulMaxBitRate = 768;
		}

		//切换了分辨率，从新设置帧率范围和码流范围
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(i,COL_RATE));
		pCombo->RemoveAllItem(bRefresh);

		for (int value = 1; value < ulRate + 1; ++value)
		{
			sprintf(str, "%d", value);
			pCombo->AddItem(str, value);
		}
		//pCombo->SetEnable(false, bRefresh);
		if (VIDEO_FORMAT_PAL == g_video_format)
		{
			if (ulRate > 25)
			{
				ulRate = 25;
			}
		}
		else
		{
			if (ulRate > 30)
			{
				ulRate = 30;
			}
		}
		pCombo->SetCurItemData(ulRate, bRefresh);

		//码流大小
		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetworkStreamList.GetItemWnd(i,COL_MAX_BIT_STREAM));
		if (1 == m_netChennelEncInfo[chnn- m_localVideoInputNum].online)
		{
			pCombo->RemoveAllItem(bRefresh);
			AddBitStreamItem(m_pBitStreamValue, m_bitStreamCount, pCombo, ulMaxBitRate);
		}

		pCombo->SetCurItemData(ulMaxBitRate, bRefresh);
	}
}

//void CCfgNetworkStreamDlg::AddMIxBitStreamItem(CComboCtrl *pCombo, unsigned long resolution)
//{
//	char str[128] = {0};
//	int rowCount = 0;
//	int vsrow = 0;
//	bool bfind =false;
//
//	//获取当前通道所接摄像机的分辨率
//
//
//	//m_supportSubEncInfo,在CCalculateSubEncode中取出该通道所接摄像机的分辨率以下的分辨率
//
//	for(int index=0;index<g_resolutionNum;++index)
//	{
//		if(0 !=(resolution & m_pVSBit[index].resolution))
//		{
//			vsrow = index;
//			bfind =true;
//			break;
//		}
//	}
//	if(!bfind)
//	{
//		return;
//	}
//	pCombo->RemoveAllItem(false);
//	//计算总共有对应分辨率有几个对应的码流
//	for(int i=0;i<20;++i)
//	{
//		if(0 != m_pVSBit[vsrow].vsforbrr[i])
//		{
//			++rowCount;
//			sprintf(str, "%d kbps",  m_pVSBit[vsrow].vsforbrr[i]);
//			pCombo->AddItem(str,  m_pVSBit[vsrow].vsforbrr[i]);
//		}
//		else//最后一个项目是0
//		{
//			break;
//		}
//	}
//	pCombo->SetDropDownCount(7);
//}

bool CCfgNetworkStreamDlg::AddSubMixBitStreamItem(CComboCtrl *pCombo,unsigned int channel, unsigned long resolution,unsigned long *pMaxBitStream)
{
	unsigned long MaxBiteRateRange[] = {32,64,128,256,384,512,768,1024,1536,2048,2560,3072,4096,5120,6144,8192,10240,12288};
	unsigned long size_max_bite = sizeof(MaxBiteRateRange)/sizeof(unsigned long);
	char str[128] = {0};
	unsigned long ulBiteStream = 0;
	unsigned int i=0;
	if ((NULL != m_supportSubEncInfo)&&(channel< m_localVideoInputNum))
	{
		for(unsigned int i=0;i< m_reslutionNum;i++)
		{
			SUPPORT_SUB_ENC_INFO *pTemp = m_supportSubEncInfo+(channel*m_reslutionNum+i);
			if (resolution == pTemp->ulRes)
			{//把这个默认值作为比特率的最大值，当前值和默认从配置项中去NCFG_ITEM_MINOR_SUPPORT_BITRATE_RANGE
				ulBiteStream = pTemp->ulDefBitrate;
				break;
			}
		}
	}
	else if((channel >=  m_localVideoInputNum)&&(channel < m_localVideoInputNum + m_netVideoInputNum))
	{
		NET_CHANNEL_ENC_INFO *pNetTemp =   &m_netChennelEncInfo[channel - m_localVideoInputNum];
		for(unsigned int i=0;i<32;i++)
		{
			if(resolution == pNetTemp->encInfo[i].videoSize)
			{
				ulBiteStream = pNetTemp->encInfo[i].maxBitRate;
				break;
			}
		}
	}
	else
	{
		return false;
	}
	pCombo->RemoveAllItem(false);
	
	for (;i<size_max_bite;i++)
	{
        if (MaxBiteRateRange[i] <= ulBiteStream)
        {
			sprintf(str, "%d kbps", MaxBiteRateRange[i]);
			pCombo->AddItem(str,  MaxBiteRateRange[i]);
		}
		else
		{
			pCombo->SetCurItemData(ulBiteStream,false);
			if (NULL != pMaxBitStream)
			{
				*pMaxBitStream = ulBiteStream;
			}
			break;
		}
	}
	pCombo->SetDropDownCount(5);
	return true;
}


void CCfgNetworkStreamDlg::AddSubResolutionItem(SUPPORT_SUB_ENC_INFO * pSupportSubEncInfo, CComboCtrl *pCombo)
{
	assert(NULL != pSupportSubEncInfo);
	unsigned long  dwResolution =0;
	for (unsigned int i=0;i<m_reslutionNum;i++)
	{
		dwResolution = pSupportSubEncInfo[i].ulRes;

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
			pCombo->AddItem("D1", DVRVIDEO_SIZE_D1);
		}			

		if (dwResolution & DVRVIDEO_SIZE_VGA)
		{
			pCombo->AddItem("VGA", DVRVIDEO_SIZE_VGA);
		}

		if (dwResolution & DVRVIDEO_SIZE_QVGA)
		{
			pCombo->AddItem("QVGA", DVRVIDEO_SIZE_QVGA);
		}

#ifdef __CUSTOM_US02__
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

		if (dwResolution & DVRVIDEO_SIZE_HD1080)
		{
			pCombo->AddItem("1080P", DVRVIDEO_SIZE_HD1080);
		}
	}
}
