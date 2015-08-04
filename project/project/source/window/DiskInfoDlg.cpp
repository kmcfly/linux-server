/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2010-11-04
** Name         : DiskInfoDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "DiskInfoDlg.h"
#include "MessageMan.h"
#include "FuncCustom.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

CDiskInfoDlg::CDiskInfoDlg()
{

}

CDiskInfoDlg::~CDiskInfoDlg()
{

}

void CDiskInfoDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
    SetCaption(m_pStrTable->String(E_STR_ID_TITLE_INFORMATION),false);
	SetDefaultBtn(DEFAULT_BTN_CANCEL);

	GUI::CStatic *pStatic = NULL;
	GUI::CButton *pButton = NULL;
	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height()  - y - m_length[BTN_HEIGHT] - 2 * m_length[DLG_OFFSET_BOTTOM];

	unsigned short offsetEdge = 4;
	int maxListRow = height / m_length[LIST_ITEM_HEIGHT]; //
	int numChnn = (IsSupportSnapPic()/* && (g_p_login_succ->productInfo.localVideoInputNum > 0)*/)? (ROW_PIC_CLEAR + 1) : (ROW_DISK_SRC + 1);

	if( numChnn > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 4;
	}

	m_diskInfoList.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this , 0, 1, false);
	m_diskInfoList.AddColumn(NULL, width / 2);
	m_diskInfoList.AddColumn(NULL, width / 2 - offsetEdge);

	m_diskInfoList.AddItem(ROW_DISK_ID, 0, m_pStrTable->String(E_STR_ID_ID));
	m_diskInfoList.AddItem(ROW_SATA_TYPE, 0, m_pStrTable->String(E_STR_ID_TYPE));
	m_diskInfoList.AddItem(ROW_TOTAL_SPACE, 0, m_pStrTable->String(E_STR_ID_SIZE_GB));
	m_diskInfoList.AddItem(ROW_FREE_SPACE, 0, m_pStrTable->String(E_STR_ID_FREE_GB));
	m_diskInfoList.AddItem(ROW_DISK_STATUS, 0, m_pStrTable->String(E_STR_ID_STATUS));
	m_diskInfoList.AddItem(ROW_DISK_ATTR, 0, m_pStrTable->String(E_STR_ID_PROPERTY));
	m_diskInfoList.AddItem(ROW_DISK_SRC, 0, m_pStrTable->String(E_STR_ID_OWNER));
	
	if (IsSupportSnapPic() /*&& (g_p_login_succ->productInfo.localVideoInputNum > 0)*/)
	{
		m_diskInfoList.AddItem(ROW_PIC_INFO, 0, m_pStrTable->String(E_STR_ID_SNAP_CAPACITY));
		m_diskInfoList.AddItem(ROW_PIC_SPACE, 0, m_pStrTable->String(E_STR_ID_SNAP_ALL_SIZE));
		m_diskInfoList.AddItem(ROW_EXIST_PIC, 0,  m_pStrTable->String(E_STR_ID_SNAP_USED_SIZE));

		pStatic = dynamic_cast<GUI::CStatic*>(m_diskInfoList.AddItem(ROW_PIC_SPACE, 1, WND_TYPE_STATIC));	
		pStatic->SetCaption("0", false);
		pStatic = dynamic_cast<GUI::CStatic*>(m_diskInfoList.AddItem(ROW_EXIST_PIC, 1, WND_TYPE_STATIC));
		pStatic->SetCaption("0", false);

		pButton = dynamic_cast<GUI::CButton*>(m_diskInfoList.AddItem(ROW_PIC_CLEAR, 1, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]));
		pButton->SetCaption(m_pStrTable->String(E_STR_ID_CLEAR), false);
	}	

	m_diskInfoList.CreateListFocusMap();
}


void CDiskInfoDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();
	
	char str[128] ={0};

	sprintf(str, "%02d", m_seq+1);
	m_diskInfoList.AddItem(ROW_DISK_ID, 1, str);

	if (NET_DISK_IDE_INTERFACE == m_diskInfo.diskInterface)
	{
		strcpy( str, "IDE" );
	}
	else if (NET_DISK_SATA_INTERFACE == m_diskInfo.diskInterface)
	{
		strcpy( str, "SATA" );
	}
	m_diskInfoList.AddItem(ROW_SATA_TYPE, 1, str);

	sprintf(str, "%0.2f", m_diskInfo.totalSpace/(float)1024);
	m_diskInfoList.AddItem(ROW_TOTAL_SPACE, 1, str);

	sprintf(str, "%0.2f", m_diskInfo.freeSpace/(float)1024);
	m_diskInfoList.AddItem(ROW_FREE_SPACE, 1, str);

			
	if (NET_DISK_NORMAL == m_diskInfo.diskStatus)
	{
		snprintf( str, 128, "%s", m_pStrTable->String(E_STR_ID_NORMAL_DISK));	
	}
	else if (NET_DISK_NO_FORMAT == m_diskInfo.diskStatus) 
	{
		snprintf(str, 128, "%s", m_pStrTable->String(E_STR_ID_UNFORMAT));
	}
	else
	{
		snprintf( str, 128, "%s", m_pStrTable->String(E_STR_ID_NORMAL_DISK));
	}

	m_diskInfoList.AddItem(ROW_DISK_STATUS, 1, str);

	if(NET_DISK_READ_WRITE == m_diskInfo.diskProperty)
	{
		m_diskInfoList.AddItem(ROW_DISK_ATTR, 1, m_pStrTable->String(E_STR_ID_READ_WRITE));
	}
	else
	{
		m_diskInfoList.AddItem(ROW_DISK_ATTR, 1, m_pStrTable->String(E_STR_ID_READ_ONLY));
	}

	if (NET_DISK_OWNED_BY_THIS ==m_diskInfo.diskOwnedBy)
	{
		snprintf(str, 128, m_pStrTable->String(E_STR_ID_THIS));
	}
	else
	{
		std::string strTmp = m_pStrTable->String(E_STR_ID_OTHER);
		char strNum[16]={0};
		snprintf(strNum, 16, "%d", m_diskInfo.diskOwnedBy);
		strTmp += strNum;
		snprintf(str, 128, "%s", strTmp.c_str());
	}
	m_diskInfoList.AddItem(ROW_DISK_SRC, 1, str);

	if (IsSupportSnapPic() /*&& (g_p_login_succ->productInfo.localVideoInputNum > 0)*/)
	{
		unsigned long allPicNum = 0;
		unsigned long usedPicNum = 0;
		if (NET_DISK_NORMAL == m_diskInfo.diskStatus)
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			unsigned long diskIndex[4];
			diskIndex[0] = m_diskInfo.diskpara;
			diskIndex[1] = m_diskInfo.diskpara + 1;
			diskIndex[2] = m_diskInfo.diskpara + 2;
			diskIndex[3] = m_diskInfo.diskpara + 3;
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_DEVICE_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_PIC_INFO, (unsigned char *)(&(diskIndex[0])), sizeof(diskIndex), msgData);
			allPicNum = *((unsigned long *)(msgData.pData + sizeof(PACKCMD)));
			usedPicNum = *((unsigned long *)(msgData.pData + sizeof(PACKCMD)+ sizeof(unsigned long)));
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			snprintf(str, 128, "%d", allPicNum);
			GUI::CStatic *pStatic = dynamic_cast<GUI::CStatic*>(m_diskInfoList.GetItemWnd(ROW_PIC_SPACE, 1));
			
			pStatic->SetCaption(str, false);
			
			snprintf(str, 128, "%d", usedPicNum);			
			pStatic = dynamic_cast<GUI::CStatic*>(m_diskInfoList.GetItemWnd(ROW_EXIST_PIC, 1));
			pStatic->SetCaption(str, false);
		}	
	}


	if (IsSupportSnapPic()/* && (g_p_login_succ->productInfo.localVideoInputNum > 0)*/)
	{
		GUI::CButton *pButton = dynamic_cast<GUI::CButton*>(m_diskInfoList.GetItemWnd(ROW_PIC_CLEAR, 1));
		if ((NET_DISK_NORMAL == m_diskInfo.diskStatus)&&(NET_DISK_READ_WRITE == m_diskInfo.diskProperty))
		{
			pButton->SetEnable(true, false);
		}
		else
		{
			pButton->SetEnable(false, false);
		}
	}

	

}

BEGIN_MSG_MAP(CDiskInfoDlg, GUI::CDialog)
	ON_MSG_EX(m_diskInfoList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CDiskInfoDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{

	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if ((IsSupportSnapPic()/* && (g_p_login_succ->productInfo.localVideoInputNum > 0)*/) && (row == ROW_PIC_CLEAR) && (col == 1))
	{
		//删除前提示
		if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_123), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING))
		{
			//放弃则直接返回
			return KEY_VIRTUAL_MSG_STOP;
		}

		if (NET_DISK_NORMAL != m_diskInfo.diskStatus)
		{
			return KEY_VIRTUAL_MSG_STOP;
		}

		if (NET_DISK_READ_WRITE != m_diskInfo.diskProperty)
		{
			return KEY_VIRTUAL_MSG_STOP;
		}

		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			unsigned long diskIndex[4];
			diskIndex[0] = m_diskInfo.diskpara;
			diskIndex[1] = m_diskInfo.diskpara + 1;
			diskIndex[2] = m_diskInfo.diskpara + 2;
			diskIndex[3] = m_diskInfo.diskpara + 3;
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_DEVICE_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DEL_DISK_PIC, (unsigned char *)(&(diskIndex[0])), sizeof(diskIndex), msgData);
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			GetRootWnd()->Repaint();
		}

		{
			char str[128] ={0};
			unsigned long allPicNum = 0;
			unsigned long usedPicNum = 0;
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			unsigned long diskIndex[4];
			diskIndex[0] = m_diskInfo.diskpara;
			diskIndex[1] = m_diskInfo.diskpara + 1;
			diskIndex[2] = m_diskInfo.diskpara + 2;
			diskIndex[3] = m_diskInfo.diskpara + 3;
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_DEVICE_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_PIC_INFO, (unsigned char *)(&(diskIndex[0])), sizeof(diskIndex), msgData);
			allPicNum = *((unsigned long *)(msgData.pData + sizeof(PACKCMD)));
			usedPicNum = *((unsigned long *)(msgData.pData + sizeof(PACKCMD)+ sizeof(unsigned long)));
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			snprintf(str, 128, "%d", allPicNum);
			GUI::CStatic *pStatic = dynamic_cast<GUI::CStatic*>(m_diskInfoList.GetItemWnd(ROW_PIC_SPACE, 1));
			pStatic->SetCaption(str);				
		

			snprintf(str, 128, "%d", usedPicNum);
			pStatic = dynamic_cast<GUI::CStatic*>(m_diskInfoList.GetItemWnd(ROW_EXIST_PIC, 1));
			pStatic->SetCaption(str);
		}		
	}


	return KEY_VIRTUAL_MSG_STOP;
}

