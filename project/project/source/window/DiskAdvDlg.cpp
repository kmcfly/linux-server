/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-12-20
** Name         : DiskAdvDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "DiskAdvDlg.h"
#include "FuncCustom.h"
#include "UserMan.h"
using namespace GUI;

CDiskAdvDlg::CDiskAdvDlg()
{

}

CDiskAdvDlg::~CDiskAdvDlg()
{

}


void CDiskAdvDlg::SetFixedDiskList(DVR_DISK_INFO_LIST *pList)
{
	m_pFixedDiskInfoList = pList;
}


void CDiskAdvDlg::OnInitial()
{
	CDialog::OnInitial();

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() -  2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_BOTTOM] - m_length[COMBO_HEIGHT] - 2 * m_length[STATIC_HEIGHT] - 6 - 8 - m_length[STATIC_HEIGHT] - 2;

	m_comboCtrl.Create(GetRandWndID(), x, y, 120, m_length[COMBO_HEIGHT], this, 0, 0);

	y += m_length[COMBO_HEIGHT] + 2;
	unsigned short titleWidth = 120;

	m_scTitle[0].Create(GetRandWndID(), x, y, titleWidth, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_DISK_MODEL));
	m_scInfo[0].Create(GetRandWndID(), x + titleWidth + 2, y, width / 2 - titleWidth - 4, m_length[STATIC_HEIGHT], this);

	m_scTitle[1].Create(GetRandWndID(), x + width / 2, y, titleWidth, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_DISK_SN));
	m_scInfo[1].Create(GetRandWndID(), x + width / 2 + titleWidth + 2, y, width / 2 - titleWidth - 4, m_length[STATIC_HEIGHT], this);

	y += m_length[STATIC_HEIGHT] + 2;

	m_scTitle[2].Create(GetRandWndID(), x, y, titleWidth, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_DISK_FIRMWARE));
	m_scInfo[2].Create(GetRandWndID(), x + titleWidth + 2, y, width / 2 - titleWidth - 4, m_length[STATIC_HEIGHT], this);

	m_scTitle[3].Create(GetRandWndID(), x + width / 2, y, 130, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_DISK_HEALTH));
	m_scInfo[3].Create(GetRandWndID(), x + width / 2 + 130 + 2, y, width / 2 - titleWidth - 4, m_length[STATIC_HEIGHT], this);

	y += m_length[STATIC_HEIGHT] + 2;

	y += 8;

	m_scSmTitle.Create(GetRandWndID(), x, y, 200, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_DISK_SMART_INFO));

	y += m_length[STATIC_HEIGHT] + 2;

	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头
	int maxRow = 20;
	if( maxRow > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	 
	m_listCtrl.Create(GetRandWndID(), x, y, width, height, this, 0, 1, true);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_SM_ID), width / 8);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_SM_CURR), width  / 6);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_SM_WORST), width / 6);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_SM_TH), width / 6);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_SM_RAW), width / 4);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_SM_STA), width / 8 - offsetEdge);

	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			m_listCtrl.AddItem(i, j, WND_TYPE_STATIC);
		}
	}

	m_listCtrl.CreateListFocusMap();
}


void CDiskAdvDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();

	//RefreshComboInfo(false);

}

void CDiskAdvDlg::RefreshComboInfo(bool bRefresh /* = true */)
{
	char str[36] = {0};

	m_comboCtrl.RemoveAllItem(false);

	int count = 0;
	POS pos = m_pFixedDiskInfoList->GetHeadPosition();
	while (pos != NULL)
	{
		DVR_DISK_INFO &diskInfo = m_pFixedDiskInfoList->GetAt(pos);		
		snprintf(str, 36, "%s-%d", m_pStrTable->String(E_STR_ID_DISK), count+1);
		m_comboCtrl.AddItem(str, (LONGLONG)pos);
		m_pFixedDiskInfoList->GetNext(pos);
		count++;
	}

	m_comboCtrl.SetCurItem(0, false);
	RefreshListInfo(false);	

	if (bRefresh)
	{
		Repaint();
	}	
}

void CDiskAdvDlg::RefreshListInfo(bool bRefresh)
{
	GUI::CStatic* pStatic = NULL;	
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;
	char str[128] = {0};

	m_scInfo[0].SetCaption(" ", false);
	m_scInfo[1].SetCaption(" ", false);
	m_scInfo[2].SetCaption(" ", false);
	m_scInfo[3].SetCaption(" ", false);

	for (int row = 0; row < 20; ++row)
	{
		for (int col = 0; col < 6; ++col)
		{
			pStatic = dynamic_cast<GUI::CStatic*>(m_listCtrl.GetItemWnd(row, col));
			pStatic->SetCaption(" ", bRefresh);
		}
	}


	if (m_pFixedDiskInfoList->GetCount() <= 0)
	{
		return;
	}

	POS pos = (POS*)m_comboCtrl.GetCurItemData();
	DVR_DISK_INFO &diskInfo = m_pFixedDiskInfoList->GetAt(pos);

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_SMART_INFO, (unsigned char *)diskInfo.diskname, sizeof(diskInfo.diskname),msgData);
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	cmdType = (NET_PROTOCOL)pCmd->cmdType;
	if (CMD_REPLY_DISK_SMART_FAIL == cmdType)
	{
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		return;
	}

	NET_DISK_SMART_INFO *pSmartInfo = reinterpret_cast<NET_DISK_SMART_INFO *>(msgData.pData + sizeof(PACKCMD));

	snprintf(str, 128, "%s", pSmartInfo->szModelNumber);
	m_scInfo[0].SetCaption(str, false);
	snprintf(str, 128, "%s", pSmartInfo->szSerialNumber);
	m_scInfo[1].SetCaption(str, false);
	snprintf(str, 128, "%s", pSmartInfo->szFirmwareRev);
	m_scInfo[2].SetCaption(str, false);	
	if (1 == pSmartInfo->diskHealth)
	{
		m_scInfo[3].SetCaption(m_pStrTable->String(E_STR_ID_SM_STA_WAR), false);
	}
	else if (2 == pSmartInfo->diskHealth)
	{
		m_scInfo[3].SetCaption(m_pStrTable->String(E_STR_ID_SM_STA_CRI), false);
	}
	else
	{
		m_scInfo[3].SetCaption(m_pStrTable->String(E_STR_ID_SM_STA_OK), false);
	}

	int count = 0;
	for (int row = 0; row < 20; ++row)
	{
		GetIDName(pSmartInfo->smartInfo[count].nIndex, str, 128);
		pStatic = dynamic_cast<GUI::CStatic*>(m_listCtrl.GetItemWnd(row, 0));
		pStatic->SetCaption(str, bRefresh);

		snprintf(str, 128, "%d", pSmartInfo->smartInfo[count].nCurrent);
		pStatic = dynamic_cast<GUI::CStatic*>(m_listCtrl.GetItemWnd(row, 1));
		pStatic->SetCaption(str, bRefresh);

		snprintf(str, 128, "%d", pSmartInfo->smartInfo[count].nWorest);
		pStatic = dynamic_cast<GUI::CStatic*>(m_listCtrl.GetItemWnd(row, 2));
		pStatic->SetCaption(str, bRefresh);

		snprintf(str, 128, "%d", pSmartInfo->smartInfo[count].nThreshold);
		pStatic = dynamic_cast<GUI::CStatic*>(m_listCtrl.GetItemWnd(row, 3));
		pStatic->SetCaption(str, bRefresh);

		snprintf(str, 128, "%d", pSmartInfo->smartInfo[count].nRawValue);
		pStatic = dynamic_cast<GUI::CStatic*>(m_listCtrl.GetItemWnd(row, 4));
		pStatic->SetCaption(str, bRefresh);

		if (pSmartInfo->smartInfo[count].nStatus > 0)
		{
			snprintf(str, 128, m_pStrTable->String(E_STR_ID_SM_STA_WAR));		
		}
		else
		{
			snprintf(str, 128, m_pStrTable->String(E_STR_ID_SM_STA_OK));
		}
		pStatic = dynamic_cast<GUI::CStatic*>(m_listCtrl.GetItemWnd(row, 5));
		pStatic->SetCaption(str, bRefresh);


		count++;
	}

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	return;	
}

void CDiskAdvDlg::GetIDName(unsigned char id, char *pStr, int len)
{
	if (0x01 == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_H01_NAME));
	}
	else if (0x03 == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_H03_NAME));
	}
	else if (0x04 == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_H04_NAME));
	}
	else if (0x05 == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_H05_NAME));
	}
	else if (0x07 == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_H07_NAME));
	}
	else if (0x09 == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_H09_NAME));
	}
	else if (0x0a == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_H0A_NAME));
	}
	else if (0x0c == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_H0C_NAME));
	}
	else if (0xc2 == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_HC2_NAME));
	}
	else if (0xc3 == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_HC3_NAME));
	}
	else if (0xc5 == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_HC5_NAME));
	}
	else if (0xc6 == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_HC6_NAME));
	}
	else if (0xc7 == id)
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_HC7_NAME));
	}
	else
	{
		snprintf(pStr, 128, "(0x%x)%s", id, m_pStrTable->String(E_STR_ID_DEF_NAME));
	}

}

BEGIN_MSG_MAP(CDiskAdvDlg, GUI::CDialog)
	ON_MSG(m_comboCtrl.GetID(), KEY_VIRTUAL_ENTER, OnClickCombo)
END_MSG_MAP()

unsigned long CDiskAdvDlg::OnClickCombo()
{
	if(m_pFixedDiskInfoList->GetCount() > 0)
	{
		RefreshListInfo(false);	
		Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CDiskTableManDlg::CDiskTableManDlg()
{

}

CDiskTableManDlg::~CDiskTableManDlg()
{

}

void CDiskTableManDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;  //不需要TITLE，因为格式化时不能够停止
	CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn(DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption(DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_DISK_MAN), false);
	//////////////////////////////////////////////////////////////////////////
	
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	m_diskManDlg.SetFixedDiskList(&m_fixedDiskInfoList);
	bool bSupport = true;
#if defined(__TW01_RILI__)
	USER_INFO userInfo;
	if(CUserMan::Instance()->GetUser(userInfo))
	{
		if (!((0 == strcmp("HITACHI", userInfo.name)) || (0 == strcmp("hitachi", userInfo.name))))
		{
			bSupport = false;
		}
	}
#endif
	if (IsSupportDiskSmart()&& bSupport)
	{

		m_diskAdvDlg.SetFixedDiskList(&m_fixedDiskInfoList);

		m_diskManDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 2);
		m_diskAdvDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 3);

		m_tabCtrl.Create(GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
		m_tabCtrl.AddItem(&m_diskManDlg, m_pStrTable->String(E_STR_ID_DISK));
		m_tabCtrl.AddItem(&m_diskAdvDlg, m_pStrTable->String(E_STR_ID_ADVANCE));

		m_tabCtrl.SetCurItem(PAGE_DISKMAN, false);
	}
	else
	{
		
		m_diskManDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 2);
	}
	
	CreateTipWnd();
}

unsigned long CDiskTableManDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (m_diskManDlg.IsFormating())
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (IsSupportDiskSmart())
	{
		if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
		{
			if((PAGE_DISKADV == m_tabCtrl.GetFocusItem()) && (PAGE_DISKADV != m_tabCtrl.GetCurItem()))
			{
				m_diskAdvDlg.RefreshComboInfo(false);
			}	
		}
	}
	else
	{

	}
	
	return keyID;
}

void CDiskTableManDlg::EnableExitBtn(bool bEnable, bool bRefresh)
{
	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_CANCEL));
	if (NULL != pBtn)
	{
		pBtn->SetEnable(bEnable, bRefresh);
	}
}

void CDiskTableManDlg::EndDialog(DLG_END_VALUE result)
{
	if(!m_diskManDlg.IsFormating())
	{
		m_diskManDlg.EndDialog(result);
		GUI::CDialog::EndDialog(result);
	}
	m_fixedDiskInfoList.RemoveAll();
}


//end
