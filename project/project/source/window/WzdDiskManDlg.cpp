/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2010-07-21
** Name         : CWzdDiskManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
作用:磁盘管理的向导
***********************************************************************/
#include "WzdPageDlg.h"
#include "WzdDiskManDlg.h"
#include "MessageMan.h"
#include "MessageBox.h"
#include "CtrlID.h"

extern DVR_SYSTEM_STATUS g_system_status;

CWzdDiskManDlg::CWzdDiskManDlg() : m_pWzdInfo(NULL),
m_bUsed(true)
{
	m_prgShow = false;
	m_listRowToDiskInfo = NULL;
	m_bFormating = false;
}

CWzdDiskManDlg::~CWzdDiskManDlg()
{

}

void CWzdDiskManDlg::SetUsed(bool bUsed)
{
	m_bUsed = bUsed;
}

bool CWzdDiskManDlg::IsUsed()
{
	return m_bUsed;
}

void CWzdDiskManDlg::SetWzdInfo(WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void  CWzdDiskManDlg::OnInitial()
{
	GUI::CListCtrl& list = m_diskManList;
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short progHeight = m_length[LIST_ITEM_HEIGHT];
	unsigned short width = m_rect.Width() - 1.5 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - progHeight - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	//List
	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_ID), width*2/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_TYPE),width*4/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_SIZE_GB),width*5/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_FREE_GB),width*6/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_STATUS),width*4/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_PROPERTY), width * 7 / 32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_OWNER), width * 3/32+15);

	GUI::CRect listRect =  list.GetClientRect();
	y += listRect.Height() + 2;
	m_progressDlg.Create( GUI::CWnd::GetRandWndID(), x, y, width, progHeight, this, 0, 2 );
	
	m_progressDlg.Show(false, false);
	x = m_length[DLG_OFFSET_LEFT];
	y += progHeight + 2;
	height = m_length[STATIC_HEIGHT];
	width = m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 4 * m_length[BTN_WIDTH] - 3 * m_length[DEFAULT_BTN_OFFSET] - 10;
	m_stcMsg.Create( GUI::CWnd::GetRandWndID(), x, y, width, height, this, NULL);
	x = m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 2 * m_length[BTN_WIDTH];
	y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	m_bntRefresh.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 0, 3, m_pStrTable->String(E_STR_ID_REFRESH));
	x = m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS]- 0.9* m_length[BTN_WIDTH];
	y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	m_bntFormat.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 0, 4, m_pStrTable->String(E_STR_ID_FORMAT));
}

void CWzdDiskManDlg::InitCtrlInfo()
{

	RefreshDiskInfo(false);
	m_curSelectRow = -1;
	m_stcMsg.Show(false, false);
	m_prgShow = false;
	m_bFormating = false;
	SetPercent(0);	
		
}

void CWzdDiskManDlg::ShowWzdData(bool bRefresh)
{

}

void CWzdDiskManDlg::GetWzdData(bool bNext)
{

}

void CWzdDiskManDlg::RefreshDiskInfo(bool bRefresh)
{
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;	
	DVR_DISK_INFO *pDiskInfo = NULL;
	int diskNum;

	m_diskManList.RemoveAllItem(false);
	m_diskInfoList.RemoveAll();
	if (NULL != m_listRowToDiskInfo)
	{
		delete [] m_listRowToDiskInfo;
		m_listRowToDiskInfo = NULL;
	}

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_START, NULL, 0, msgData);
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	cmdType = (NET_PROTOCOL)pCmd->cmdType;
	if (CMD_REPLY_DISK_FAIL == cmdType)
	{
		diskNum = 0;
		GUI::CMessageBox Box(m_pStrTable->String(E_STR_ID_MESSAGE_120), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		Box.DoModal(NULL, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
	}
	else
	{
		pDiskInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
		diskNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
		if (diskNum > 0)
		{		
			m_listRowToDiskInfo = new POS [diskNum];
			for (int i = 0; i < diskNum; ++i)
			{
				DVR_DISK_INFO diskInfoTmp = pDiskInfo[i];
				POS p = m_diskInfoList.AddTail(diskInfoTmp);
				m_listRowToDiskInfo[i] = p;
			} 
		}
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);  

	bool bSupportRedun = CProduct::Instance()->SupportRedunRec();
	GUI::CListCtrl& list = m_diskManList;
	char str[128] = {0};     
	int diskCount = 0;

	if (NULL != m_listRowToDiskInfo )
	{
		assert(m_diskInfoList.GetCount() > 0);
		for (diskCount = 0; diskCount < m_diskInfoList.GetCount(); ++diskCount)
		{
			POS p = m_listRowToDiskInfo[diskCount];
			DVR_DISK_INFO &diskInfo = m_diskInfoList.GetAt(p);

			sprintf( str, "%02d", diskInfo.reserved+ 1 );
			/*list.AddItem( diskCount, 0, str );*/
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( diskCount, 0, WND_TYPE_CHECK ) );
			assert( pCheck );
			pCheck->SetCaption(str, false, false);

			if (NET_DISK_IDE_INTERFACE == diskInfo.diskInterface)
			{
				strcpy( str, "IDE" );
			}
			else if (NET_DISK_SATA_INTERFACE == diskInfo.diskInterface)
			{
				strcpy( str, "SATA" );
			}
			else if (NET_DISK_CDDVD_INTERFACE == diskInfo.diskInterface)
			{
				strcpy(str, "CD/DVD-ROM");
			}
			else if (NET_DISK_USB_INTERFACE == diskInfo.diskInterface)
			{
				strcpy(str, "USB");
			}
			else if (NET_DISK_REMDISK_INTERFACE == diskInfo.diskInterface)
			{
				strcpy(str, "USB");
			}
			else if (NET_DISK_ESATA_INETERFACE == diskInfo.diskInterface)
			{
				strcpy(str, "e-SATA");
			}
			else if (NET_DISK_SD_INTERFACE == diskInfo.diskInterface)
			{
				strcpy(str, "SD");
			}
			else
			{
				strcpy(str, m_pStrTable->String(E_STR_ID_OTHER));
			}
			list.AddItem( diskCount, 1, str );

			float spaceG = diskInfo.totalSpace / (float)1024;
			sprintf(str, "%0.2f", spaceG);				
			list.AddItem( diskCount, 2, str );

			spaceG = diskInfo.freeSpace / (float)1024;
			sprintf(str, "%0.2f", spaceG);			
			list.AddItem( diskCount, 3, str );

#ifdef __DVR_PROFESSIONAL__
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( diskCount, 5, WND_TYPE_COMBO ) );
			assert( pCombo );
			pCombo->SetMaxStringLen( 16 );
			pCombo->AddItem(m_pStrTable->String(E_STR_ID_READ_WRITE), NET_DISK_READ_WRITE);
			pCombo->AddItem(m_pStrTable->String(E_STR_ID_READ_ONLY), NET_DISK_ONLY_READ);
			if (bSupportRedun)
			{
				pCombo->AddItem(m_pStrTable->String(E_STR_ID_REDUNDANCY), NET_DISK_REDUNDANCY);
			}
			pCombo->SetCurItemData(diskInfo.diskProperty, false);

			if (NET_DISK_TYPE_FIXEDDISK == diskInfo.diskType)
			{		
				if (NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy)
				{					
					if (NET_DISK_NORMAL == diskInfo.diskStatus)
					{
						strcpy( str, m_pStrTable->String(E_STR_ID_NORMAL_DISK));	
						pCombo->SetEnable(true, false);
					}
					else if (NET_DISK_NO_FORMAT == diskInfo.diskStatus) 
					{
						strcpy(str, m_pStrTable->String(E_STR_ID_UNFORMAT));
						pCombo->SetEnable(false, false);
					}
					else
					{
						assert(false);
					}
				}
				else
				{
					assert(NET_DISK_NORMAL == diskInfo.diskStatus);
					assert(NET_DISK_ONLY_READ == diskInfo.diskProperty);
					strcpy( str, m_pStrTable->String(E_STR_ID_NORMAL_DISK));	
					pCombo->SetEnable(false, false);					
				}
			}
			else if (NET_DISK_TYPE_CDDVD == diskInfo.diskType)
			{
				assert(NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy);
				if (NET_DISK_NO_CD == diskInfo.diskStatus)
				{
					strcpy(str , m_pStrTable->String(E_STR_ID_MESSAGE_60));
				}
				else
				{
					strcpy(str, m_pStrTable->String(E_STR_ID_NORMAL_DISK));
				}

				pCombo->SetEnable(false, false);
			}
			else
			{
				assert(NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy);
				strcpy(str, m_pStrTable->String(E_STR_ID_NORMAL_DISK));
				pCombo->SetEnable(false, false);
			}	

			pCombo->SetEnable(false, false);

			list.AddItem( diskCount, 4, str );
#else
			if (NET_DISK_TYPE_FIXEDDISK == diskInfo.diskType)
			{		
				if (NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy)
				{					
					if (NET_DISK_NORMAL == diskInfo.diskStatus)
					{
						strcpy( str, m_pStrTable->String(E_STR_ID_NORMAL_DISK));	
					}
					else if (NET_DISK_NO_FORMAT == diskInfo.diskStatus) 
					{
						strcpy(str, m_pStrTable->String(E_STR_ID_UNFORMAT));
					}
					else
					{
						assert(false);
					}
				}
				else
				{
					assert(NET_DISK_NORMAL == diskInfo.diskStatus);
					assert(NET_DISK_ONLY_READ == diskInfo.diskProperty);
					strcpy( str, m_pStrTable->String(E_STR_ID_NORMAL_DISK));	
				}
			}
			else if (NET_DISK_TYPE_CDDVD == diskInfo.diskType)
			{
				assert(NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy);
				if (NET_DISK_NO_CD == diskInfo.diskStatus)
				{
					strcpy(str , m_pStrTable->String(E_STR_ID_MESSAGE_60));
				}
				else
				{
					strcpy(str, m_pStrTable->String(E_STR_ID_NORMAL_DISK));
				}

			}
			else
			{
				assert(NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy);
				strcpy(str, m_pStrTable->String(E_STR_ID_NORMAL_DISK));
			}
			list.AddItem( diskCount, 4, str );

			if (NET_DISK_READ_WRITE == diskInfo.diskProperty)
			{
				strcpy(str, m_pStrTable->String(E_STR_ID_READ_WRITE));
			}
			else 
			{
				strcpy(str, m_pStrTable->String(E_STR_ID_READ_ONLY));
			}	
			list.AddItem( diskCount, 5, str );			
#endif

			if (NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy)
			{
				sprintf(str, m_pStrTable->String(E_STR_ID_THIS));
			}
			else
			{
				std::string strTmp = m_pStrTable->String(E_STR_ID_OTHER);
				char strNum[16]={0};
				sprintf(strNum, "%d", diskInfo.diskOwnedBy);
				strTmp += strNum;
				strcpy(str,strTmp.c_str());
			}
			list.AddItem(diskCount, 6, str);
		}
	}


	/*list.EnableSingleSelect( true );*/
	list.SetCurSingleSelect( 0 , false );
	list.CreateListFocusMap();
	m_curSelectRow = 0;

	if (bRefresh)
	{
		list.Repaint();
	}
}

void CWzdDiskManDlg::OnIdle()
{
	int allDiskNum = 0;
	int curDiskNum = 0;
	int precent    = 0;
	char buf[128]={0};
	if (m_bFormating)
	{

		CMessageMan::Instance()->GetFormatProgress(&allDiskNum, &curDiskNum, &precent);

		if (101 == precent)
		{
			m_finishDiskNum = 0;
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_68), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
			m_bFormating = false;
			CMessageMan::Instance()->RefreshFormatProgress(0, 0, 0);
			CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_FORMAT_STOP, NULL, 0);
			EndProgress();
			m_diplayPrecent = 0;
			m_stcMsg.SetCaption(m_pStrTable->String(E_STR_ID_MESSAGE_68));
			m_stcMsg.SetBKColor(COLOR_DLG_CHILD_BK);
			EnableRefreshBtn(true, true);
			EnableFormatBtn(true, true);
			EnableDefaultBtn(true, true);
			m_progressDlg.Show(false, true);
			return;
		}

		if ((allDiskNum == curDiskNum) && (curDiskNum > 0) && (100 == precent))
		{
			//MessageBox("format disk ok", "Waring", DEFAULT_BTN_OK);
			m_finishDiskNum = 0;
			m_stcMsg.Show(true);
			m_stcMsg.SetCaption(m_pStrTable->String(E_STR_ID_MESSAGE_69));
			m_stcMsg.SetBKColor(COLOR_DLG_CHILD_BK);
			m_bFormating = false;
			CMessageMan::Instance()->RefreshFormatProgress(0, 0, 0);
			CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_FORMAT_STOP, NULL, 0);
			SetPercent(100);
			EndProgress();
			m_diplayPrecent = 0;
			RefreshDiskInfo();
			EnableRefreshBtn(true, true);
			EnableFormatBtn(true, true);
			EnableDefaultBtn(true, true);
			m_progressDlg.Show(false, true);
			return;
		}

		if (m_diplayPrecent <= precent)
		{
			SetPercent(m_diplayPrecent);
			m_diplayPrecent++;
			PUB_Sleep(200);
		}

		if ((allDiskNum > curDiskNum) && (curDiskNum > 0) && (100 == precent))
		{
			if (m_finishDiskNum >= curDiskNum)
			{
				return;
			}
			SetPercent(100);
			PUB_Sleep(200);
			CMessageMan::Instance()->RefreshFormatProgress(0, 0, 0);	
			m_diplayPrecent = 0;
			SetPercent(0);
			m_finishDiskNum = curDiskNum;
		}
		else
		{
			string diskstr;
			diskstr.clear();
			memset(buf,0,sizeof(buf));
			if((curDiskNum == 0)&&(allDiskNum == 0))
			{
				//2个数值都是0 的时候不修改属性
			}
			else
			{
				snprintf(buf,128,"%d/%d  %s",curDiskNum,allDiskNum,m_pStrTable->String(E_STR_ID_FORMAT_DOT));
				diskstr =buf;
				m_stcMsg.SetCaption(diskstr);
			}
		}
		
	}
}


void CWzdDiskManDlg::SetPercent( int percet )
{
	m_progressDlg.SetPercent( percet, m_prgShow );
}

void CWzdDiskManDlg::EndProgress()
{
	m_progressDlg.Show( m_prgShow, true );;
}

BEGIN_MSG_MAP(CWzdDiskManDlg, CWzdPageDlg)
	ON_MSG(m_bntFormat.GetID(), KEY_VIRTUAL_ENTER, OnFormat)
	ON_MSG(m_bntRefresh.GetID(), KEY_VIRTUAL_ENTER, OnRefresh)
	ON_MSG_EX(m_diskManList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CWzdDiskManDlg::OnFormat()
{
#ifdef __ENVIRONMENT_WIN32__

	ULONGLONG diskIndex = 0;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));


	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_FORMAT_START, (unsigned char *)&diskIndex, sizeof(ULONGLONG), msgData);
	
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_FORMAT_FAIL == (NET_PROTOCOL)pCmd->cmdType)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_33), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}
	else if (CMD_REPLY_FORMAT_SUCC == (NET_PROTOCOL)pCmd->cmdType)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_45), m_pStrTable->String(E_STR_ID_TITLE_SUCCEED), DEFAULT_BTN_OK);
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	g_system_status = DVR_SYSTEM_REBOOT;
	return KEY_VIRTUAL_MSG_STOP;
#else
	if (m_bFormating)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	//m_curSelectRow = m_diskManList.GetCurSingleSelect();

	if (NULL == m_listRowToDiskInfo)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	assert(m_diskInfoList.GetCount() > 0);
	int sum = m_diskManList.GetItemCount();
	int selNum = 0; //选择个数
	DVR_DISK_INFO diskInfo[sum];
	for(int i=0; i<sum; i++)
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_diskManList.GetItemWnd(i, 0));
		if (pCheck->IsCheck())
		{
			m_curSelectRow = i;
			if (m_curSelectRow < 0 || m_curSelectRow >= m_diskInfoList.GetCount())
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
			POS pos = m_listRowToDiskInfo[m_curSelectRow];
			if (NULL == pos)
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
			diskInfo[selNum] = m_diskInfoList.GetAt(pos);

			if (NET_DISK_TYPE_FIXEDDISK != diskInfo[selNum].diskType)
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
			selNum ++;
		}
	}
	if (selNum  == 0)
	{
		GUI::CMessageBox Box(m_pStrTable->String(E_STR_ID_MESSAGE_124), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		Box.DoModal(NULL, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
		return 0;
	}

	/*if (m_curSelectRow < 0)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (NULL == m_listRowToDiskInfo)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	assert(m_diskInfoList.GetCount() > 0);
	if (m_curSelectRow >= m_diskInfoList.GetCount())
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	POS pos = m_listRowToDiskInfo[m_curSelectRow];
	if (NULL == pos)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	DVR_DISK_INFO diskInfo = m_diskInfoList.GetAt(pos);

	if (NET_DISK_TYPE_FIXEDDISK != diskInfo.diskType)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}*/

	if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_16), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_BTN_CANCEL|DEFAULT_ICON_WARNING))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}


	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;	

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_FORMAT_START, reinterpret_cast<unsigned char *>(diskInfo), sizeof(DVR_DISK_INFO)*selNum,msgData);    

	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	cmdType = (NET_PROTOCOL)pCmd->cmdType;
	if (CMD_REPLY_FORMAT_FAIL == cmdType)
	{
		//NET_CTRL_FAIL errorInfo = *reinterpret_cast<NET_CTRL_FAIL *>(msgData.pData + sizeof(PACKCMD));
		//MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_68), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		m_bFormating = false;
	}
	else
	{
		CMessageMan::Instance()->RefreshFormatProgress(0, 0, 0);	
		m_prgShow = true;
		m_bFormating = true;
		m_diplayPrecent = 0;
		SetPercent(0);

		m_stcMsg.Show(true);
		m_stcMsg.SetCaption(m_pStrTable->String(E_STR_ID_FORMAT_DOT));
		m_stcMsg.SetBKColor(COLOR_DLG_CHILD_BK);
		m_progressDlg.Show(true, true);

		EnableRefreshBtn(false, true);
		EnableFormatBtn(false, true);
		EnableDefaultBtn(false, true);
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	if (!m_bFormating)
	{
		OnRefresh();
	}
	return KEY_VIRTUAL_MSG_STOP;
#endif
	return 0;
}

unsigned long CWzdDiskManDlg::OnRefresh()
{
	if (m_bFormating)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	SetPercent(0);
	EnableRefreshBtn(false, true);
	EnableFormatBtn(false, true);
	EnableDefaultBtn(false, true);
	
	m_stcMsg.Show(true);
	m_stcMsg.SetCaption(m_pStrTable->String(E_STR_ID_REFRESH_DOT));
	m_stcMsg.SetBKColor(COLOR_DLG_CHILD_BK);
	RefreshDiskInfo();
	m_stcMsg.Show(false);
	
	EnableRefreshBtn(true, true);
	EnableFormatBtn(true, true);
	EnableDefaultBtn(true, true);
	
	return KEY_VIRTUAL_MSG_STOP;
}

void CWzdDiskManDlg::EnableFormatBtn(bool bEnable, bool bRefresh)
{
	m_bntFormat.SetEnable(bEnable, bRefresh);
}

void  CWzdDiskManDlg::EnableRefreshBtn(bool bEnable, bool bRefresh)
{
	m_bntRefresh.SetEnable(bEnable, bRefresh);
}

void  CWzdDiskManDlg::EnableDefaultBtn(bool bEnable, bool bRefresh)
{
	GUI::CWnd* pWndTop = GetTopestWnd();//获取向导主窗口
	GUI::CWnd* pBtnApply = pWndTop->GetCtrl(CTRLID_DEFAULT_BTN_APPLY);
	GUI::CWnd* pBtnOk = pWndTop->GetCtrl(CTRLID_DEFAULT_BTN_OK);
	GUI::CWnd* pBtnCancel = pWndTop->GetCtrl(CTRLID_DEFAULT_BTN_CANCEL);
	assert(pBtnOk);
	assert(pBtnCancel);
	assert(pBtnApply);
	pBtnApply->SetEnable(bEnable, bRefresh);
	pBtnOk->SetEnable(bEnable, bRefresh);
	pBtnCancel->SetEnable(bEnable, bRefresh);
}

unsigned long CWzdDiskManDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (m_bFormating)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	//判断选择的行是否有效
	if (row >= m_diskInfoList.GetCount())
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	POS pos = m_listRowToDiskInfo[row]; 
	if (NULL == pos)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if ( 0 == col)
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_diskManList.GetItemWnd(row, col));
#ifdef __DVR_PROFESSIONAL__
		if (pCheck->IsCheck())
		{
			POS pos = m_listRowToDiskInfo[row];
			if (NULL != pos)
			{
				DVR_DISK_INFO diskInfo = m_diskInfoList.GetAt(pos);
				if (diskInfo.diskStatus == NET_DISK_NORMAL && NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy && NET_DISK_TYPE_FIXEDDISK == diskInfo.diskType)
				{
					GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( m_diskManList.GetItemWnd(row, 5));
					pCombo->SetEnable(true, true);
				}
			}
		}
		else
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( m_diskManList.GetItemWnd(row, 5));
			pCombo->SetEnable(false, true);
		}
#endif
	}

	if (5 == col) 
	{ 
		//只有本机的设备并且已经格式化后的硬盘才可以修改硬盘属性      
		DVR_DISK_INFO diskInfo = m_diskInfoList.GetAt(pos);

		assert(NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy); 
		assert(NET_DISK_TYPE_FIXEDDISK ==diskInfo.diskType);
		assert(NET_DISK_NORMAL == diskInfo.diskStatus);
		//GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( m_diskManList.GetItemWnd(row, col));
	}

	return KEY_VIRTUAL_MSG_STOP;

}
//end
