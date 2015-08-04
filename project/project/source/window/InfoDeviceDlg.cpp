/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2011-06-23
** Name         : InfoDeviceDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "InfoDeviceDlg.h"
#include "MessageMan.h"
#include "ConfigEx.h"
#include "FuncCustom.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

//////////////////////////////////////////////////////////////////////////
CInfoDeviceDlg::CInfoDeviceDlg()
{

}

CInfoDeviceDlg::~CInfoDeviceDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CInfoDeviceDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_MENU_CFG_RECORD), false);

	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT]+m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_BOTTOM];

	NCFG_ITEM_HEAD itemHead;
	unsigned long dataLen = 0;
	unsigned char *pData = NULL;
// 	memset(&itemHead, 0, sizeof(NCFG_ITEM_HEAD));
// 	itemHead.itemID = NCFG_ITEM_VIDEO_FORMAT;

	unsigned short cy = height - 2*m_length[LIST_ITEM_HEIGHT] - 2 - m_length[STATIC_HEIGHT];

	m_ChanelRecList.Create(GUI::CWnd::GetRandWndID(), x, y, width, cy, this, 0, 1, true);
	m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
#if defined(__TW01_RILI__)
	m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 5 / 32 );
#else
	m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 6 / 32 );
#endif
	m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_FPS), width * 5 / 32);
	if (IsSupportAlarmOut() || (g_p_login_succ->productInfo.netVideoInputNum > 0))
	{
		m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_MENU_CFG_SENSOR), width * 4 / 32 );
		m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_MENU_CFG_MOTION), width * 4 / 32 );
		m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_MANUAL), width * 4 / 32 );
#if defined(__TW01_RILI__)
		m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_REC_SCHEDULE), width * 5 / 32 );
#else
		m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_REC_SCHEDULE), width * 4 / 32 );
#endif
	}
	else
	{		
		m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_MENU_CFG_MOTION), width * 5 / 32 );
		m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_MANUAL), width * 5 / 32 );
		m_ChanelRecList.AddColumn(m_pStrTable->String(E_STR_ID_REC_SCHEDULE), width * 6 / 32 );
	}

	memset(&itemHead, 0, sizeof(NCFG_ITEM_HEAD));
	itemHead.itemID = NCFG_ITEM_ENCODE_MAJOR_EX;
	dataLen = 0;
	pData = NULL;
	if(CConfigEx::Instance()->GetConfig((unsigned char *)&itemHead, sizeof(itemHead), &pData, dataLen))
	{
		assert(pData != NULL);
		assert(dataLen > 0);

		NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pData );
		assert( pBlockHead->biSize == sizeof( NCFG_BLOCK_HEAD ) );
		assert( pBlockHead->ItemNum == 1 );
		assert( pBlockHead->netcfgver == NCFG_VERSION );

		NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pData + sizeof( NCFG_BLOCK_HEAD ) );
		assert( pItemHead->itemID == NCFG_ITEM_ENCODE_MAJOR_EX );
		assert( pItemHead->subLen == sizeof( ENCODE_INFO_EX ) );
		int videoInputNum = pItemHead->num;
		ENCODE_INFO_EX *pEncodeInfo = reinterpret_cast<ENCODE_INFO_EX *>( pData + sizeof(NCFG_ITEM_HEAD) + sizeof(NCFG_BLOCK_HEAD));
		char strTmp[50] = {0};

		ULONGLONG supportCH = 0;
		IsSupportCH(supportCH);

		for (int ch = 0, row = 0; ch < videoInputNum; ch++)
		{
			if(!(supportCH & (((ULONGLONG)0x01) << ch)))
			{
				continue;
			}

			sprintf(strTmp, "%d", ch+1);
			m_ChanelRecList.AddItem(row, 0, strTmp);

			if (DVRVIDEO_SIZE_D1 == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1, "D1");
			}
			else if(DVRVIDEO_SIZE_CIF == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1, "CIF");
			}
			else if (DVRVIDEO_SIZE_HD1 == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1, "HD1");
			}
			else if (DVRVIDEO_SIZE_HD1080 == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1, "1080P");
			}
			else if (DVRVIDEO_SIZE_HD720 == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1 , "720P");
			}
			else if (DVRVIDEO_SIZE_960H == pEncodeInfo[ch].resolution)
			{
#if defined(__CUSTOM_IL03__) || defined(__CUSTOM_US02__)
				m_ChanelRecList.AddItem(row, 1, "960H");
#else
				m_ChanelRecList.AddItem(row, 1, "WD1");
#endif
			}
			else if (DVRVIDEO_SIZE_VGA == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1, "VGA");
			}
			else if (DVRVIDEO_SIZE_960P == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1, "960P");
			}
			else if (DVRVIDEO_SIZE_SXGA == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1, "1.3M");
			}
			else if (DVRVIDEO_SIZE_3M == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1, "3M");
			}
			else if (DVRVIDEO_SIZE_16_9_3M == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1, "2304X1296");
			}
			else if (DVRVIDEO_SIZE_4M == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1, "2688X1520");
			}
			else if (DVRVIDEO_SIZE_5M == pEncodeInfo[ch].resolution)
			{
				m_ChanelRecList.AddItem(row, 1, "2592X1944");
			}


			sprintf(strTmp, "%d", pEncodeInfo[ch].rate);
			m_ChanelRecList.AddItem(row, 2, strTmp);
			unsigned long recType = RECORD_TYPE_NULL;
			recType = CMessageMan::Instance()->GetRecStatus(ch);
			if (IsSupportAlarmOut() || (g_p_login_succ->productInfo.netVideoInputNum > 0))
			{
				if (RECORD_TYPE_SENSOR & recType)
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 3, WND_TYPE_RADIO));
					pRadio->SetCheck(true, false);
				}
				else
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 3, WND_TYPE_RADIO));
					//pRadio->SetEnable(false, false);
				}

				if (RECORD_TYPE_MOTION & recType)
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 4, WND_TYPE_RADIO));
					pRadio->SetCheck(true, false);
				}
				else
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 4, WND_TYPE_RADIO));
					//pRadio->SetEnable(false, false);
				}
				if (RECORD_TYPE_MANUAL & recType)
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 5, WND_TYPE_RADIO));
					pRadio->SetCheck(true, false);
				}
				else
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 5, WND_TYPE_RADIO));
					//pRadio->SetCheck(false, false);
					//pRadio->SetEnable(false, false);
				}
				if (RECORD_TYPE_SCHEDULE & recType)
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 6, WND_TYPE_RADIO));
					pRadio->SetCheck(true, false);
				}
				else
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 6, WND_TYPE_RADIO));
					//pRadio->SetEnable(false, false);
				}
			}
			else
			{
				if (RECORD_TYPE_MOTION & recType)
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 3, WND_TYPE_RADIO));
					pRadio->SetCheck(true, false);
				}
				else
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 3, WND_TYPE_RADIO));
					//pRadio->SetEnable(false, false);
				}
				if (RECORD_TYPE_MANUAL & recType)
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 4, WND_TYPE_RADIO));
					pRadio->SetCheck(true, false);
				}
				else
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 4, WND_TYPE_RADIO));
					//pRadio->SetCheck(false, false);
					//pRadio->SetEnable(false, false);
				}
				if (RECORD_TYPE_SCHEDULE & recType)
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 5, WND_TYPE_RADIO));
					pRadio->SetCheck(true, false);
				}
				else
				{
					GUI::CRadioCtrl *pRadio = dynamic_cast<GUI::CRadioCtrl*>(m_ChanelRecList.AddItem(row, 5, WND_TYPE_RADIO));
					//pRadio->SetEnable(false, false);
				}
			}

			row++;
		}
	}


	y +=cy - m_length[DLG_OFFSET_BOTTOM];
	height -=cy;

	m_stcDisk.Create( GUI::CWnd::GetRandWndID(), x, y , width, m_length[STATIC_HEIGHT], this );
	m_stcDisk.SetCaption(m_pStrTable->String(E_STR_ID_DISK), false );

	y += m_length[STATIC_HEIGHT];
	height -= m_length[STATIC_HEIGHT];

	cy = 2*m_length[LIST_ITEM_HEIGHT]+2;

	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;	
	DVR_DISK_INFO *pDiskInfo = NULL;
	int diskNum = 0;

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_START, NULL, 0, msgData);
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	cmdType = (NET_PROTOCOL)pCmd->cmdType;
	if (CMD_REPLY_DISK_FAIL == cmdType)
	{
		diskNum = 0;		
	}
	else
	{
		pDiskInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
		diskNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
		int fixedDiskNum = 0;
		for (int i=0; i< diskNum; i++)
		{
			if (NET_DISK_TYPE_FIXEDDISK == pDiskInfo[i].diskType)
			{
				fixedDiskNum++;
			}
		}
		diskNum = fixedDiskNum;
	}
	m_DiskList.Create(GUI::CWnd::GetRandWndID(), x, y, width, cy, this, 1, 1, false);
	if ( 0 == diskNum )
	{
		m_DiskList.AddColumn(NULL, width/2);
		m_DiskList.AddColumn(NULL, width/2);
	}
	else
	{
		unsigned short subWidth = width/(diskNum+1);
		for (int i=0; i<=diskNum; i++)
		{
			m_DiskList.AddColumn(NULL, subWidth);
		}
	}
	m_DiskList.AddItem(0, 0, m_pStrTable->String(E_STR_ID_ID));
	m_DiskList.AddItem(1, 0, m_pStrTable->String(E_STR_ID_STATUS));
	char tmp[50] = {0};
	for (int i=0; i<diskNum; i++)
	{
		sprintf(tmp, "%d", i+1);//pDiskInfo[i].interfaceIndex);
		m_DiskList.AddItem(0, i+1, tmp);
		
		if (pDiskInfo[i].diskStatus == NET_DISK_NORMAL)
		{
			GUI::CProgressCtrl *pProgress =  dynamic_cast<GUI::CProgressCtrl*>(m_DiskList.AddItem(1, i+1, WND_TYPE_PROGRESS));
			if(pDiskInfo[i].totalSpace > 0)
			{
				pProgress->SetPercent(100 - pDiskInfo[i].freeSpace*100/pDiskInfo[i].totalSpace, false);
			}
			else
			{
				pProgress->SetPercent(0);
			}
		}
		else
		{
			m_DiskList.AddItem(1, i+1, m_pStrTable->String(E_STR_ID_UNFORMAT));
		}
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	
}
