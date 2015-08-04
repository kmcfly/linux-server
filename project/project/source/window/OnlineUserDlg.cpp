/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : OnlineUserDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "OnlineUserDlg.h"
#include "MessageMan.h"
#include "ListCtrlEx.cpp"
//////////////////////////////////////////////////////////////////////////
template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	NET_CLIENT_INFO_EX &it = node;
	char str [512] = {0};

	if (0 == col)
	{
		snprintf(str, 512, "%s", it.User);
	}

	if (1 == col)
	{
		snprintf(str, 512, "%03d.%03d.%03d.%03d", it.IP&0xff, (it.IP>>8)&0xff, (it.IP>>16)&0xff, it.IP>>24);
	}

	if (2 == col)
	{
		if( 1 == it.status )
		{
			strcpy(str, m_pStrTable->String(E_STR_ID_ONLINE));
		}
	}

	m_str = str;
	return m_str;
}
//////////////////////////////////////////////////////////////////////////
COnlineUserDlg::COnlineUserDlg()
{

}

COnlineUserDlg::~COnlineUserDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void COnlineUserDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_ONLINE_USER_LIST), false);

	//////////////////////////////////////////////////////////////////////////
	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[DLG_OFFSET_BOTTOM] - m_length[BTN_HEIGHT];

	m_listCtrlEx.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_USER_NAME), width * 10 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_IP), width * 11 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_STATUS), width * 11 / 32);
	m_listCtrlEx.EnableSingleSelect();


	x = m_rect.Width() - m_length[DLG_OFFSET_LEFT] - 2 * m_length[BTN_WIDTH];
	y = m_rect.Height() - m_length[DLG_OFFSET_BOTTOM] - m_length[BTN_HEIGHT] - 5;

#ifdef __DVR_PROFESSIONAL__
	m_btnEvict.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH] * 2, m_length[BTN_HEIGHT], this,  2, 1, m_pStrTable->String(E_STR_ID_DISCONNECT));
	x -= ((m_length[BTN_WIDTH] * 2) + 5);
	m_btnEvict.SetEnable(false, false);
#endif

	m_btnRefresh.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH]* 2, m_length[BTN_HEIGHT], this,  1, 1, m_pStrTable->String(E_STR_ID_REFRESH));

	//获取用户列表，并加入控件。
	MESSAGE_DATA msgData;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CLIENT_INFO, NULL, 0, msgData);

	unsigned long num = *reinterpret_cast<unsigned long *>(msgData.pData+sizeof(PACKCMD));
	
	if (num > 0)
	{
		NET_CLIENT_INFO_EX *pUser = new NET_CLIENT_INFO_EX[num];
		memcpy(pUser, msgData.pData+sizeof(PACKCMD)+sizeof(unsigned long), sizeof(NET_CLIENT_INFO_EX)*num);
		m_listCtrlEx.PutData(pUser, num);
		m_listCtrlEx.SetSingleSelect(0,true);
		pUser = NULL;
		m_btnEvict.SetEnable(true, false);
	}

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
}

BEGIN_MSG_MAP(COnlineUserDlg, GUI::CDialog)
	ON_MSG(m_btnRefresh.GetID(), KEY_VIRTUAL_ENTER, OnRefresh)
#ifdef __DVR_PROFESSIONAL__
	ON_MSG(m_btnEvict.GetID(), KEY_VIRTUAL_ENTER, OnEvict)
#endif
END_MSG_MAP()

unsigned long COnlineUserDlg::OnRefresh()
{
	{
		//获取用户列表，并加入控件。
		MESSAGE_DATA msgData;
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CLIENT_INFO, NULL, 0, msgData);

		unsigned long num = *reinterpret_cast<unsigned long *>(msgData.pData+sizeof(PACKCMD));

		if (0 < num)
		{
			NET_CLIENT_INFO_EX *pUser = new NET_CLIENT_INFO_EX[num];
			memcpy(pUser, msgData.pData+sizeof(PACKCMD)+sizeof(unsigned long), sizeof(NET_CLIENT_INFO_EX)*num);
			m_listCtrlEx.PutData(pUser, num);
			pUser = NULL;
		}
		else
		{
			m_listCtrlEx.PutData(NULL, 0);
		}

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

#ifdef __DVR_PROFESSIONAL__
unsigned long COnlineUserDlg::OnEvict()
{
	ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_ADVANCE_MAN);
	if (0 == auth)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		unsigned short row = m_listCtrlEx.GetSingleSelect();
		if ((row >= m_listCtrlEx.GetCount()) || (row < 0))
		{
			return KEY_VIRTUAL_MSG_STOP;
		}

		NET_CLIENT_INFO_EX * pNode = m_listCtrlEx.GetListData();
		if (NULL == pNode)
		{
			return KEY_VIRTUAL_MSG_STOP;
		}

		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_EVICT_SOMEONE, reinterpret_cast<unsigned char *>(&(pNode[row].clientID)), sizeof(unsigned long), msgData);

		PACKCMD *pCMD = reinterpret_cast<PACKCMD *>(msgData.pData);
		if (CMD_REPLY_CTRL_SUCC == pCMD->cmdType)
		{
			//先释放
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			//再次刷新用户列表
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CLIENT_INFO, NULL, 0, msgData);

			unsigned long num = *reinterpret_cast<unsigned long *>(msgData.pData+sizeof(PACKCMD));

			if (num > 0)
			{
				NET_CLIENT_INFO_EX *pUser = new NET_CLIENT_INFO_EX[num];
				memcpy(pUser, msgData.pData+sizeof(PACKCMD)+sizeof(unsigned long), sizeof(NET_CLIENT_INFO_EX)*num);
				m_listCtrlEx.PutData(pUser, num);
				pUser = NULL;
			}
			else
			{
				m_listCtrlEx.PutData(NULL, 0);
			}

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		}
		else
		{
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		}

		return KEY_VIRTUAL_MSG_STOP;
	}
}
#endif

//end
