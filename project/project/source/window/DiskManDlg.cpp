/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : DiskManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "DiskManDlg.h"
#include "MessageMan.h"
#include "MessageBox.h"
#include "DiskAdvDlg.h"

extern DVR_SYSTEM_STATUS g_system_status;

CDiskManDlg::CDiskManDlg()
{
	m_prgShow = false;

	m_listRowToDiskInfo = NULL;

	m_bFormating = false;

}

CDiskManDlg::~CDiskManDlg()
{

}


void CDiskManDlg::OnInitial()
{
	//m_style |= WND_NO_TITLE;  //不需要TITLE，因为格式化时不能够停止
	GUI::CDialog::OnInitial();
	//SetImage(BMP_MEM_DLG_LARGE, false);
	//SetCaption(m_pStrTable->String( E_STR_ID_TITLE_DISK_MAN ), false);
#ifdef __DVR_PROFESSIONAL__
	//SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	//SetDefaultBtnCaption( DEFAULT_BTN_APPLY,  m_pStrTable->String(E_STR_ID_APPLY));
	//SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_FORMAT));
	//SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));
#else
	//SetDefaultBtn(DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	//SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_FORMAT));
	//SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));
#endif
	//////////////////////////////////////////////////////////////////////////

	GUI::CListCtrl& list = m_diskManList;
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl* pCombo = NULL;

	//unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	//unsigned short xOff = 15, yOff = m_length[DEFAULT_BTN_BOTTOM_YPOS];

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	
	unsigned short progHeight = m_length[LIST_ITEM_HEIGHT];
	//unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	//unsigned short height = m_rect.Height() - y - progHeight - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - progHeight - m_length[DLG_OFFSET_SUB_BOTTOM] - m_length[BTN_HEIGHT] - 6;
	/////List

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_ID), width*2/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_TYPE),width*5/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_SIZE_GB),width*5/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_FREE_GB),width*5/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_STATUS),width*5/32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_PROPERTY), width * 7 / 32 );
	list.AddColumn(m_pStrTable->String(E_STR_ID_OWNER), width * 3/32 - m_length[SCROLL_WIDTH] - 2);
	
	if (CProduct::Instance()->SupportESATA())
	{
		GUI::CRect listRect =  list.GetClientRect();
		y += listRect.Height() + 2;
		width = 80;
		m_bEsataCk.Create(GUI::CWnd::GetRandWndID(), x, y, width, m_length[CHECK_HEIGHT], this, 0, 2);
		m_bEsataCk.SetCaption("e-Sata", false, false);
		if (CProduct::Instance()->GetUseEsata())
		{
			m_bEsataCk.SetCheck(true, false);
		}
		else
		{
			m_bEsataCk.SetCheck(false, false);
		}
		x += width;
		width = m_rect.Width() - m_length[DLG_OFFSET_LEFT] - x;
		m_progressDlg.Create( GUI::CWnd::GetRandWndID(), x, y, width, progHeight, this, 1, 2 );
	}
	else
	{
		GUI::CRect listRect =  list.GetClientRect();
		y += listRect.Height() + 2;
		m_progressDlg.Create( GUI::CWnd::GetRandWndID(), x, y, width, progHeight, this, 0, 2 );
	}
	m_progressDlg.Show(false, false);
	
	

	x = m_length[DLG_OFFSET_LEFT];
	y += progHeight + 2;
	height = m_length[STATIC_HEIGHT];
	width = m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 4 * m_length[BTN_WIDTH] - 3 * m_length[DEFAULT_BTN_OFFSET] - 10;
	m_stcMsg.Create( GUI::CWnd::GetRandWndID(), x, y, width, height, this, NULL);
	

#ifdef __DVR_PROFESSIONAL__	
	x = m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 4 * m_length[BTN_WIDTH] - 3 * m_length[DEFAULT_BTN_OFFSET];
	y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
    m_bntRefresh.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 0, 3, m_pStrTable->String(E_STR_ID_REFRESH));
#else
	x = m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 3 * m_length[BTN_WIDTH] - 2 * m_length[DEFAULT_BTN_OFFSET];
	y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	m_bntRefresh.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 0, 3, m_pStrTable->String(E_STR_ID_REFRESH));
#endif

#ifdef __DVR_PROFESSIONAL__
	x = m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 3 * m_length[BTN_WIDTH] - 2 * m_length[DEFAULT_BTN_OFFSET];
#else
	x = m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 2 * m_length[BTN_WIDTH] - 1 * m_length[DEFAULT_BTN_OFFSET];
#endif
	y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	m_btnBrowse.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 1, 3, m_pStrTable->String(E_STR_ID_BROWSE));

#ifdef __DVR_PROFESSIONAL__
	x += m_length[BTN_WIDTH] +  m_length[DEFAULT_BTN_OFFSET];

	m_btnApplay.Create(GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 1, 4);
	m_btnApplay.SetCaption(m_pStrTable->String(E_STR_ID_APPLY), false);
#endif

	x += m_length[BTN_WIDTH] +  m_length[DEFAULT_BTN_OFFSET];
	m_btnFormat.Create(GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 1, 5);
	m_btnFormat.SetCaption(m_pStrTable->String(E_STR_ID_FORMAT), false);
}


void CDiskManDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();

	RefreshDiskInfo(false);
	EnableApplayBtn(false, false);	
	m_curSelectRow = -1;
	m_stcMsg.Show(false, false);
	m_prgShow = false;
	m_bFormating = false;
	SetPercent(0);	
}

#ifdef __DVR_PROFESSIONAL__
unsigned long CDiskManDlg::OnApply()
{
	if (m_bFormating)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	SetPercent(0);

	int rowNum = m_diskManList.GetItemCount();
	if (rowNum <= 0)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//统计修改属性的硬盘
	int readWrtieNum = 0;
	int selectNum = 0;
	for (int row = 0; row < rowNum; ++row)
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_diskManList.GetItemWnd(row, 0));
		if (!pCheck->IsCheck())
		{
			continue;
		}
		POS pos = m_listRowToDiskInfo[row];
		DVR_DISK_INFO &diskInfo = m_diskInfoList.GetAt(pos);
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( m_diskManList.GetItemWnd(row, 5));

		//只有本机的硬盘才能修改属性
		if ((NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy) && (NET_DISK_TYPE_FIXEDDISK == diskInfo.diskType) && (NET_DISK_NORMAL == diskInfo.diskStatus))
		{
			unsigned char propertyTmp = diskInfo.diskProperty;
			if (propertyTmp != pCombo->GetCurItemData())
			{
				propertyTmp = pCombo->GetCurItemData();
				selectNum++;		
			}
			if (NET_DISK_READ_WRITE == propertyTmp)
			{
				readWrtieNum++;
			}
		}	
	}

	if (0 == selectNum)
	{
		RefreshDiskInfo();
		EnableApplayBtn(false, true);
		return KEY_VIRTUAL_MSG_STOP;
	}

	//需要添加判断读写硬盘是否都用完

	//
	/*if (0 == readWrtieNum)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_62),m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK|DEFAULT_ICON_ERROR);
		return KEY_VIRTUAL_MSG_STOP;
	}*/


	if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_63), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_CANCEL | DEFAULT_BTN_OK | DEFAULT_ICON_QUESTION))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}


	EnableBrowseBtn(false, true);
	EnableApplayBtn(false, true);
	EnableRefreshBtn(false, true);
	EnableFormatBtn(false, true);
	EnableExitBtn(false, true);

	m_stcMsg.Show(true);
	m_stcMsg.SetCaption(m_pStrTable->String(E_STR_ID_MESSAGE_64));
	m_stcMsg.SetBKColor(COLOR_DLG_CHILD_BK);

	int index = 0;
	DVR_DISK_INFO *pDiskInfo = new DVR_DISK_INFO [selectNum];
	for (int row = 0; row < rowNum; ++row)
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_diskManList.GetItemWnd(row, 0));
		if (!pCheck->IsCheck())
		{
			continue;
		}
		POS pos = m_listRowToDiskInfo[row];
		DVR_DISK_INFO &diskInfo = m_diskInfoList.GetAt(pos);
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( m_diskManList.GetItemWnd(row, 5));

		//只有本机的硬盘才能修改属性
		if ((NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy) && (NET_DISK_TYPE_FIXEDDISK == diskInfo.diskType) && (NET_DISK_NORMAL == diskInfo.diskStatus))
		{
			if (diskInfo.diskProperty != pCombo->GetCurItemData())
			{
				pDiskInfo[index] = diskInfo;
				pDiskInfo[index].disktemp = pCombo->GetCurItemData();
				index++;
			}
		}		
	}

	assert(index == selectNum);

	MESSAGE_DATA msgData;
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_CHANGE_PROPERTY, (unsigned char *)pDiskInfo, selectNum * sizeof(DVR_DISK_INFO), msgData);
	if (NULL != pDiskInfo)
	{
		delete [] pDiskInfo;
		pDiskInfo = NULL;
	}
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_DISK_CHANGE_PROPERTY_FAIL == (NET_PROTOCOL)pCmd->cmdType)
	{
		NET_CTRL_FAIL errorInfo = *reinterpret_cast<NET_CTRL_FAIL *>(msgData.pData + sizeof(PACKCMD));
		if (NETERR_FORMAT_DISK_FORMATING == errorInfo)
		{
			//MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_120), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		}
		else if (NETERR_FORMAT_DISK_BACKUP_PLAY == errorInfo)
		{	
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_121), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		}
		else if (NETERR_DISK_IO_ERR == errorInfo)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_65), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			PUB_Sleep(10000);
			g_system_status = DVR_SYSTEM_REBOOT;
		}	
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	RefreshDiskInfo();
	EnableBrowseBtn(true, true);
	EnableRefreshBtn(true, true);
	EnableFormatBtn(true, true);
	EnableExitBtn(true, true);

	m_stcMsg.Show(false);

	return KEY_VIRTUAL_MSG_STOP;
}
#endif

unsigned long CDiskManDlg::OnOK()
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
				MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_129), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
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
		NET_CTRL_FAIL errorInfo = *reinterpret_cast<NET_CTRL_FAIL *>(msgData.pData + sizeof(PACKCMD));
		if (NETERR_FORMAT_DISK_FORMATING == errorInfo)
		{
			//MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_120), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		}
		else if (NETERR_FORMAT_DISK_BACKUP_PLAY == errorInfo)
		{	
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_121), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		}			
		m_bFormating = false;
	}
	else
	{
		CMessageMan::Instance()->RefreshFormatProgress(0, 0, 0);	
		m_prgShow = true;
		m_bFormating = true;
		m_diplayPrecent = 0;
		SetPercent(0);
		m_finishDiskNum = 0;

		m_stcMsg.Show(true);
		m_stcMsg.SetCaption(m_pStrTable->String(E_STR_ID_FORMAT_DOT));
		m_stcMsg.SetBKColor(COLOR_DLG_CHILD_BK);
		m_progressDlg.Show(true, true);

		EnableApplayBtn(false, true);
		EnableRefreshBtn(false, true);
		EnableFormatBtn(false, true);
		EnableExitBtn(false, true);
		EnableBrowseBtn(false, true);
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	if (!m_bFormating)
	{
		OnRefresh();
	}

	return KEY_VIRTUAL_MSG_STOP;

#endif
}

void CDiskManDlg::OnIdle()
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
			EnableExitBtn(true, true);
			EnableBrowseBtn(true, true);
			m_progressDlg.Show(false, true);
			return;
		}

		
		if ((allDiskNum == curDiskNum) && (curDiskNum > 0) && (100 == precent))
		{
			//char fileName [512] = {0};
			//sprintf(fileName, "%d  and  %d", allDiskNum, curDiskNum);
			//MessageBox(fileName/*"format disk ok"*/, "Waring", DEFAULT_BTN_OK);
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
			EnableExitBtn(true, true);
			EnableBrowseBtn(true, true);
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


void CDiskManDlg::SetPercent( int percet )
{
	m_progressDlg.SetPercent( percet, m_prgShow );
}

void CDiskManDlg::EndProgress()
{
	m_progressDlg.Show( m_prgShow, true );;
}

void CDiskManDlg::RefreshDiskInfo(bool bRefresh/* = true*/)
{
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;	
	DVR_DISK_INFO *pDiskInfo = NULL;
	int diskNum;

	m_diskManList.RemoveAllItem(false);
	m_diskInfoList.RemoveAll();
	m_pFixedDiskInfoList->RemoveAll();
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
		NET_CTRL_FAIL errorInfo = *reinterpret_cast<NET_CTRL_FAIL *>(msgData.pData + sizeof(PACKCMD));
		if (NETERR_FORMAT_DISK_FORMATING == errorInfo )
		{
			GUI::CMessageBox Box(m_pStrTable->String(E_STR_ID_MESSAGE_120), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
			Box.DoModal(NULL, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
		}		
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
				snprintf(str, 128, "%s", m_pStrTable->String(E_STR_ID_OTHER));
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
			pCombo->SetMaxStringLen( 24 );
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
						snprintf( str, 128, "%s", m_pStrTable->String(E_STR_ID_NORMAL_DISK));	
						pCombo->SetEnable(true, false);
					}
					else if (NET_DISK_NO_FORMAT == diskInfo.diskStatus) 
					{
						snprintf(str, 128, "%s", m_pStrTable->String(E_STR_ID_UNFORMAT));
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
					snprintf( str,  128, "%s", m_pStrTable->String(E_STR_ID_NORMAL_DISK));	
					pCombo->SetEnable(false, false);					
				}
				m_pFixedDiskInfoList->AddTail(diskInfo);
			}
			else if (NET_DISK_TYPE_CDDVD == diskInfo.diskType)
			{
				assert(NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy);
				if (NET_DISK_NO_CD == diskInfo.diskStatus)
				{
					snprintf(str ,  128, "%s", m_pStrTable->String(E_STR_ID_MESSAGE_60));
				}
				else
				{
					snprintf(str,  128, "%s", m_pStrTable->String(E_STR_ID_NORMAL_DISK));
				}

				pCombo->SetEnable(false, false);
			}
			else
			{
				assert(NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy);
				snprintf(str,  128, "%s",m_pStrTable->String(E_STR_ID_NORMAL_DISK));
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
						snprintf( str,  128, "%s", m_pStrTable->String(E_STR_ID_NORMAL_DISK));	
					}
					else if (NET_DISK_NO_FORMAT == diskInfo.diskStatus) 
					{
						snprintf(str,  128, "%s",  m_pStrTable->String(E_STR_ID_UNFORMAT));
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
					snprintf( str, 128, "%s", m_pStrTable->String(E_STR_ID_NORMAL_DISK));	
				}
				m_pFixedDiskInfoList->AddTail(diskInfo);
			}
			else if (NET_DISK_TYPE_CDDVD == diskInfo.diskType)
			{
				assert(NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy);
				if (NET_DISK_NO_CD == diskInfo.diskStatus)
				{
					snprintf(str ,  128, "%s", m_pStrTable->String(E_STR_ID_MESSAGE_60));
				}
				else
				{
					snprintf(str,  128, "%s", m_pStrTable->String(E_STR_ID_NORMAL_DISK));
				}

			}
			else
			{
				assert(NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy);
				snprintf(str,  128, "%s", m_pStrTable->String(E_STR_ID_NORMAL_DISK));
			}
			list.AddItem( diskCount, 4, str );

			if (NET_DISK_READ_WRITE == diskInfo.diskProperty)
			{
				snprintf(str,  128, "%s", m_pStrTable->String(E_STR_ID_READ_WRITE));
			}
			else 
			{
				snprintf(str,  128, "%s", m_pStrTable->String(E_STR_ID_READ_ONLY));
			}	
			list.AddItem( diskCount, 5, str );			
#endif
			

			if (NET_DISK_OWNED_BY_THIS == diskInfo.diskOwnedBy)
			{
				snprintf(str, 128, m_pStrTable->String(E_STR_ID_THIS));
			}
			else
			{
				std::string strTmp = m_pStrTable->String(E_STR_ID_OTHER);
				char strNum[16]={0};
				snprintf(strNum, 16, "%d", diskInfo.diskOwnedBy);
				strTmp += strNum;
				snprintf(str, 128, "%s", strTmp.c_str());
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




void CDiskManDlg::EndDialog(DLG_END_VALUE result)
{
	if (m_bFormating)
	{
		return;
	}
	else
	{
		if (NULL != m_listRowToDiskInfo)
		{
			delete [] m_listRowToDiskInfo;
			m_listRowToDiskInfo = NULL;
		}

		m_diskInfoList.RemoveAll();

		m_diplayPrecent = 0;

		//CDialog::EndDialog(result);
	}
}

void CDiskManDlg::SetFixedDiskList(DVR_DISK_INFO_LIST *pList)
{
	m_pFixedDiskInfoList = pList;
}



BEGIN_MSG_MAP(CDiskManDlg, GUI::CDialog)
	ON_MSG_EX(m_diskManList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG(m_btnBrowse.GetID(), KEY_VIRTUAL_ENTER, OnClickBrowse)
	ON_MSG(m_bntRefresh.GetID(), KEY_VIRTUAL_ENTER, OnRefresh)
	ON_MSG(m_bEsataCk.GetID(), KEY_VIRTUAL_ENTER, OnCkEsata)
	ON_MSG(m_btnApplay.GetID(), KEY_VIRTUAL_ENTER, OnBtnApply)
	ON_MSG(m_btnFormat.GetID(), KEY_VIRTUAL_ENTER, OnBtnFormat)
END_MSG_MAP()



unsigned long CDiskManDlg::OnRefresh()
{
	if (m_bFormating)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	SetPercent(0);
	EnableRefreshBtn(false, true);
	EnableApplayBtn(false, true);
	EnableFormatBtn(false, true);
	EnableExitBtn(false, true);
	EnableBrowseBtn(false, true);

	m_stcMsg.Show(true);
	m_stcMsg.SetCaption(m_pStrTable->String(E_STR_ID_REFRESH_DOT));
	m_stcMsg.SetBKColor(COLOR_DLG_CHILD_BK);

	RefreshDiskInfo();

	m_stcMsg.Show(false);
	EnableFormatBtn(true, true);
	EnableRefreshBtn(true, true);
	EnableExitBtn(true, true);
	EnableBrowseBtn(true, true);

	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CDiskManDlg::OnClickBrowse()
{
	if (m_bFormating)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	int sum = m_diskManList.GetItemCount();
	/*m_curSelectRow = m_diskManList.GetCurSingleSelect();*/

	int selNum = 0; //选择个数
	for(int i=0; i<sum; i++)
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_diskManList.GetItemWnd(i, 0));
		if (pCheck->IsCheck())
		{
			selNum ++;
			m_curSelectRow = i;
			if (selNum > 1)
			{
				GUI::CMessageBox Box(m_pStrTable->String(E_STR_ID_MESSAGE_125), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
				Box.DoModal(NULL, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
				return 0;
			}
		}
	}
	if (selNum  == 0)
	{
		GUI::CMessageBox Box(m_pStrTable->String(E_STR_ID_MESSAGE_124), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		Box.DoModal(NULL, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
		return 0;
	}


	if (m_curSelectRow < 0)
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

	if(NET_DISK_TYPE_REMOVEABLE == diskInfo.diskType)
	{
		CUSBExploreDlg usbExploreDlg;

		usbExploreDlg.SetCurDevice(CUSBExploreDlg::DEVICENAME, diskInfo.diskname);
		usbExploreDlg.SetStyle(DISK_BROWSE_DLG);
		usbExploreDlg.SetCaptionName(m_pStrTable->String(E_STR_ID_TITLE_INFORMATION));
		usbExploreDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

		OnRefresh();
	}
	else if(NET_DISK_TYPE_FIXEDDISK == diskInfo.diskType)
	{
		CDiskInfoDlg diskInfoDlg;
		/*int row = m_diskManList.GetCurSingleSelect();*/
		POS p = m_listRowToDiskInfo[/*row*/m_curSelectRow];
		diskInfoDlg.SetDiskInfo(/*row*/m_curSelectRow, m_diskInfoList.GetAt(p));
		diskInfoDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
	}


	return KEY_VIRTUAL_MSG_STOP;

}

unsigned long CDiskManDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
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
		EnableApplayBtn(true, true);		
	}

	return KEY_VIRTUAL_MSG_STOP;

}

unsigned long CDiskManDlg::OnCkEsata()
{
	if (!CProduct::Instance()->SupportESATA())
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
   
	bool bCheck = false;
	if (m_bEsataCk.IsCheck())
	{
		bCheck = true;		
	}
	else
	{
		bCheck = false;
	}

	if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_119), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_CANCEL | DEFAULT_BTN_OK | DEFAULT_ICON_QUESTION))
	{
		m_bEsataCk.SetCheck(!bCheck, true);
		return KEY_VIRTUAL_MSG_STOP;
	}

	CProduct::Instance()->SetUseEsata(bCheck);
	PUB_Sleep(1000);
	g_system_status = DVR_SYSTEM_REBOOT;

	return KEY_VIRTUAL_MSG_STOP;	
}


void CDiskManDlg::EnableApplayBtn(bool bEnable, bool bRefresh)
{
	//GUI::CButton *pBtnApply = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
	//if (NULL != pBtnApply)
	//{
	//	pBtnApply->SetEnable(bEnable, bRefresh);
	//}
	m_btnApplay.SetEnable(bEnable, bRefresh);
}



void CDiskManDlg::EnableFormatBtn(bool bEnable, bool bRefresh)
{
	//GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_OK));
	//if (NULL != pBtn)
	//{
	//	pBtn->SetEnable(bEnable, bRefresh);
	//}
	m_btnFormat.SetEnable(bEnable, bRefresh);
}




void CDiskManDlg::EnableRefreshBtn(bool bEnable, bool bRefresh)
{
	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(m_bntRefresh.GetID()));
	if (NULL != pBtn)
	{
		pBtn->SetEnable(bEnable, bRefresh);
	}

	if (CProduct::Instance()->SupportESATA())
	{
		GUI::CCheckCtrl *pEsataCk = dynamic_cast<GUI::CCheckCtrl*>(GetCtrl(m_bEsataCk.GetID()));
		if (NULL != pEsataCk)
		{
			pEsataCk->SetEnable(bEnable, bRefresh);
		}
	}	
}

void CDiskManDlg::EnableExitBtn(bool bEnable, bool bRefresh)
{
	//GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_CANCEL));
	//if (NULL != pBtn)
	//{
	//	pBtn->SetEnable(bEnable, bRefresh);
	//}
	CDiskTableManDlg* pMan = dynamic_cast<CDiskTableManDlg*>(GetParent());

	if(pMan)
	{
		pMan->EnableExitBtn(bEnable, bRefresh);
	}
}

void CDiskManDlg::EnableBrowseBtn(bool bEnable, bool bRefresh)
{
	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(m_btnBrowse.GetID()));
	if(NULL != pBtn)
	{
		pBtn->SetEnable(bEnable, bRefresh);
	}
}

unsigned long CDiskManDlg::OnBtnFormat()
{
	return OnOK();
}

unsigned long CDiskManDlg::OnBtnApply()
{
	return OnApply();
}











