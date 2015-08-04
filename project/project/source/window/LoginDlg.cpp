/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : LoginDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "LoginDlg.h"
#include "ConfigEx.h"
#include "ConfigBlock.h"
#include "UserMan.h"
#include "FuncCustomDlg.h"
#include "BackDoorManDlg.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
bool CLoginDlg::s_bInitFirst = true;
extern DVR_SYSTEM_STATUS g_system_status;
extern bool g_bLogin;
extern CRYTOINFO_STATUS g_crytoStatus;

//////////////////////////////////////////////////////////////////////////
CLoginDlg::CLoginDlg() : m_bLogIn(false)
, m_loginErrorCode(NETERR_FAIL)
, m_lenName(0)
, m_lenPass(0)
{
}

CLoginDlg::~CLoginDlg()
{

}

void CLoginDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_editPassword.SetCaption("", false);
	SetImage(BMP_MEM_DLG_SMALL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_LOGIN), false);
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_LOGIN) );
	//////////////////////////////////////////////////////////////////////////
	int x = m_length[DEFAULT_BTN_RIGHT_XPOS];
	int y = m_length[TITLE_HEIGHT] + 35;
	int offset_y = m_length[EDIT_HEIGHT] + 25;
	int cx = m_rect.Width() - x*2;

	m_static[0].Create(CTRLID_STATIC, x, y, cx, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_USER_NAME));
	y += m_length[STATIC_HEIGHT] + 8;
	m_editUser.Create(GetRandWndID(), x, y, cx, m_length[TIME_CTRL_HEIGHT], this, 0, 1);
	m_editUser.SetFollowCtrl(true);
	if( s_bInitFirst )
	{
#if defined(__CUSTOM_CNB__)
		m_editUser.SetCaption("root", false);
#else
		m_editUser.SetCaption("admin", false);
#endif
	}

	y += offset_y;
	m_static[1].Create(CTRLID_STATIC, x, y, cx, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_PASSWORD));
	y += m_length[STATIC_HEIGHT] + 8;
	m_editPassword.Create(GetRandWndID(), x, y, cx, m_length[TIME_CTRL_HEIGHT], this, 0, 2);
	m_editPassword.SetPassword(true);
	m_editPassword.SetFollowCtrl(true);

	//2010-02-03 20:34:00 YSW
	//发布版不再显示默认密码
#ifndef __RELEASE__
	if( s_bInitFirst )
	{
		m_editPassword.SetCaption("123456", false);
	}
#endif

	y += m_length[EDIT_HEIGHT] + 16;
	CreateTipWnd(cx,x,y);
	SetTipTextColor(GUI_RGB(220,20,20),TIP_ERR);
	//m_stLoginInfo.Create(CTRLID_STATIC, x, y, cx, m_length[STATIC_HEIGHT], this);
	//m_stLoginInfo.SetBKColor(COLOR_DLG_CHILD_BK);
	//m_stLoginInfo.SetTextColor(GUI_RGB(255, 50, 50));
	//m_stLoginInfo.SetCaption("", false);

	//	y += m_length[TIME_CTRL_HEIGHT] + 5;
	//	m_static[2].Create(CTRLID_STATIC, x, y, m_rect.Width() - x * 2, 2, this);
	//	m_static[2].SetLineStyle(GUI::CStatic::HORZ_LINE);

	//获取用户名密码长度
	GetMaxLen(m_lenName, m_lenPass);
	m_editUser.SetMaxStringLen(m_lenName);
	m_editPassword.SetMaxStringLen(m_lenPass);

	if( s_bInitFirst )
	{
		s_bInitFirst = false;
	}
}

unsigned long CLoginDlg::OnOK()
{
	if (LoginProc())
	{
		return GUI::CDialog::OnOK();
	} 
	else
	{
		SetErrorText();

		m_editPassword.SetCaption("", true);
	//	return GUI::CDialog::OnCancel();
	}
	return KEY_VIRTUAL_MSG_STOP;
}

bool CLoginDlg::GetMaxLen(unsigned long& nameLen, unsigned long& passLen)
{
	//从配置中获取最大长度
	//////////////////////////////////////////////////////////////////////////
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_DEVUSER_NAME_MAX_LEN);
	itemList.AddTail(NCFG_ITEM_DEVUSER_PWD_MAX_LEN);

	unsigned char* pData = NULL;
	unsigned long len = 0;
	if (!CConfigEx::Instance()->GetConfigEx(itemList, &pData, len))
	{
		printf("Login dlg.Get config data fail!\n");
		nameLen = USER_NAME_LEN;
		passLen = PASSWORD_LEN;
		assert(false);
		return false;
	}

	CConfigBlock cfgBlock;
	cfgBlock.CreateBlock(pData, len);

	unsigned char *pCfgData = NULL;
	cfgBlock.GetConfig(NCFG_ITEM_DEVUSER_NAME_MAX_LEN, &pCfgData, len);
	assert(len == sizeof(unsigned long));
	nameLen = *reinterpret_cast<unsigned long*>(pCfgData);

	cfgBlock.GetConfig(NCFG_ITEM_DEVUSER_PWD_MAX_LEN, &pCfgData, len);
	assert(len == sizeof(unsigned long));
	passLen = *reinterpret_cast<unsigned long*>(pCfgData);

	delete [] pData;
	pData = NULL;
	cfgBlock.DestroyBlock();

	return true;
}

bool CLoginDlg::LogIn(const char *pName, const char *pPassword, bool bAuto/* = true*/)
{
	CMessageMan* pMsgMan = CMessageMan::Instance();

	NET_LOGIN_INFO loginInfo;
	memset(&loginInfo, 0, sizeof(loginInfo));

	/*
	使用字符比较拷贝、比较用户名密码，需要注意尾部的'\0'
	*/

	//比较是否进入后门
	if (!bAuto)
	{
		assert(NULL != pName);
		assert(NULL != pPassword);
		assert(strlen(pName) < sizeof(loginInfo.username));
		assert(strlen(pPassword) < sizeof(loginInfo.password));
		
		if (0 == strcmp(pName, "admin"))
		{
			if ((0 == strcmp(pPassword, "6036huanyuan")))
			{
				MESSAGE_DATA msgData;
				PACKCMD *pCmd = NULL;
				unsigned long cmdType;

				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_RESTOR_ADMIN_PASS, NULL, 0, msgData);
				pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
				cmdType = (NET_PROTOCOL)pCmd->cmdType;
				if (CMD_REPLY_CTRL_SUCC == cmdType)
				{
					printf("modify pass suc\n");
				}
				else
				{
					printf("modify pass error\n");
					DVRSystem("rm -rf /mnt/mtd/config/* ");	
					CProduct::Instance()->writeDefaultResolution();
				}
				m_loginErrorCode = NETERR_LOGIN_FAIL_USERPWD_ERR;
				m_bLogIn = false;
				g_system_status = DVR_SYSTEM_REBOOT;
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
				return false;
			}
			else  if((0 == strcmp(pPassword, "ppttzz51shezhi")))
			{
				CFuncCustomDlg customDlg;
				if(DLG_OK == customDlg.DoModal())
				{
					m_loginErrorCode = NETERR_LOGIN_FAIL_USERPWD_ERR;
					m_bLogIn = false;
					g_system_status = DVR_SYSTEM_REBOOT;
					return false;
				}
			}
			else
			{
				bool bUseBackdoor = true;
				WND_BACK_DOOR_INFO info;
				memset(&info, 0, sizeof(info));

				unsigned long allLang = 0xafffffff;
				if ((0 == strcmp(pPassword, "6036logo")))
				{
					info.BootImage = 1;
					info.Logo = 1;
					if (g_crytoStatus != CRYTOINFO_STATUS_NEW)
					{				
						info.Lang = 1;
					}
					else if (allLang == CLocalDevice::GetSupportLanguageNew())
					{	
						info.Lang = 1;
					}		

					unsigned long ddd = CLocalDevice::GetSupportLanguageNew();
				}
				else if (0 == strcmp(pPassword, "6036market"))
				{
					info.BootImage = 1;
					info.Logo = 1;
					info.DDNS = 1;
					info.Keyboard = 1;					
					info.Lang = 1;
					info.CompanyName = 1;
				}
				else if ((0 == strcmp(pPassword, "6036adws")))
				{
					info.BootImage = 1;
					info.Logo = 1;
					info.DDNS = 1;
					info.Keyboard = 1;
					info.Mac = 1;
					info.Hardware = 1;
					info.Lang = 1;
					if (g_crytoStatus != CRYTOINFO_STATUS_NEW)
					{
						info.CompanyID = 1;
					}
					info.CompanyName = 1;
				}
				else if ((0 == strcmp(pPassword, "jiamijiami6036")))
				{
					memset(&info, 1, sizeof(info));
				}
				else
				{
					bUseBackdoor = false;
				}
				
				if (bUseBackdoor)
				{
#ifndef WIN32
					CBackDoorManDlg backdoor;
					backdoor.SetBackDoorInfo(info);
					backdoor.DoModal();

					m_loginErrorCode = NETERR_LOGIN_FAIL_USERPWD_ERR;
					m_bLogIn = false;
					g_system_status = DVR_SYSTEM_REBOOT;
#endif
					return false;
				}
				


				

//				int level = -1;
//				if ((0 == strcmp(pPassword, "ttvvtt")))
//				{			
//					level = 0;				
//				}
//				else if ((0 == strcmp(pPassword, "ttvvttqwer")))
//				{
//					level = 1;				
//				}
//				else if ((0 == strcmp(pPassword, "ttvvttadws")))
//				{
//					//这个密码只能修改加密芯片一次。这个密码，大家可以知道
//					level = 2;				
//				}
//				else if ((0 == strcmp(pPassword, "tvttvtmarket")))
//				{
//					//
//					level = 254;					
//				}
//				else if ((0 == strcmp(pPassword, "jiamijiamitvt")))
//				{
//					//这个密码可以修改加密芯片多次。这个密码,不能让大家知道
//					level = 255;					
//				}
//
//
//				if (level > -1)
//				{
//					CBackDoorDlg backDoor;
//					backDoor.SetLevel(level);
//					backDoor.DoModal();
//					m_loginErrorCode = NETERR_LOGIN_FAIL_USERPWD_ERR;
//					m_bLogIn = false;
//					g_system_status = DVR_SYSTEM_REBOOT;
//					return false;
//				}
//
//				if((0 == strcmp(pPassword, "ppttzz51shezhi")))
//				{
//					CFuncCustomDlg customDlg;
//					if(DLG_OK == customDlg.DoModal())
//					{
//						m_loginErrorCode = NETERR_LOGIN_FAIL_USERPWD_ERR;
//						m_bLogIn = false;
//						g_system_status = DVR_SYSTEM_REBOOT;
//						return false;
//					}
//				}
//
//#ifdef __DVR_LOGO__
//				if((0 == strcmp(pPassword, "ttvvttadwslogo")))
//				{
//					CBackDoorLogoDlg logoDlg;
//
//					if(DLG_OK == logoDlg.DoModal())
//					{
//						m_loginErrorCode = NETERR_LOGIN_FAIL_USERPWD_ERR;
//						m_bLogIn = false;
//						g_system_status = DVR_SYSTEM_REBOOT;
//						return false;
//					}
//				}
//#endif
			}
		}	
	}

	if (bAuto)
	{
		loginInfo.connectType = true;
	}
	else
	{
		assert(NULL != pName);
		assert(NULL != pPassword);
		assert(strlen(pName) < sizeof(loginInfo.username));
		assert(strlen(pPassword) < sizeof(loginInfo.password));

		strcpy(loginInfo.username, pName);
		strcpy(loginInfo.password, pPassword);
	}

	loginInfo.IP = LOCAL_IP;

	MESSAGE_DATA msgData;
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOGIN, reinterpret_cast<BYTE*>(&loginInfo), sizeof(loginInfo), msgData);

	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_LONGIN_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{	
		assert(sizeof(NET_LOGIN_SUCCESS_REPLY) == pCmd->dataLen);
		memcpy(g_p_login_succ, msgData.pData + sizeof(PACKCMD), pCmd->dataLen);

		pMsgMan->ReleaseMsgBuff(msgData);
		m_bLogIn = true;
		g_bLogin = true;
		return true;
	}
	else if (CMD_REPLY_LONGIN_FAIL == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		assert(sizeof(m_loginErrorCode) == pCmd->dataLen);
		memcpy(&m_loginErrorCode, msgData.pData + sizeof(PACKCMD), pCmd->dataLen);

		pMsgMan->ReleaseMsgBuff(msgData);
		m_bLogIn = false;
		g_bLogin = false;
		return false;
	}
	else
	{
		printf("login exception cmd = 0x%08x\n", pCmd->cmdType);
		assert(false);
		pMsgMan->ReleaseMsgBuff(msgData);
		m_bLogIn = false;
		g_bLogin = false;
		return false;
	}
}

bool CLoginDlg::LoginProc()
{
	assert(m_lenName > 0);
	assert(m_lenPass > 0);
	assert(m_lenName <= 32);
	assert(m_lenPass <= 32);

	assert(strlen(m_editUser.GetCaption().c_str()) <= 32);
	assert(strlen(m_editPassword.GetCaption().c_str()) <= 32);

	return LogIn(m_editUser.GetCaption().c_str(), m_editPassword.GetCaption().c_str(), false);
}

bool CLoginDlg::GetLoginSuccessReply(NET_LOGIN_SUCCESS_REPLY& replyInfo) const
{
	if (m_bLogIn)
	{
		memcpy(&replyInfo, g_p_login_succ, sizeof(NET_LOGIN_SUCCESS_REPLY));
		return true;
	} 
	else
	{
		memset(&replyInfo, 0, sizeof(replyInfo));
		return false;
	}
}

bool CLoginDlg::GetErrorCode(NET_CTRL_FAIL& errorCode) const
{
	errorCode = m_loginErrorCode;
	return (!m_bLogIn);
}

void CLoginDlg::Logoff()
{
	MESSAGE_DATA msgData;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOGOUT, NULL, 0, msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	m_bLogIn = false;
	g_bLogin = false;
	m_editPassword.SetCaption("", false);
}

void CLoginDlg::SetErrorText()
{
	//std::string str;
	ENUM_STRING_ID strID = E_STR_ID_NULL;
	switch (m_loginErrorCode)
	{
	case NETERR_LOGIN_FAIL_USERPWD_ERR:
		strID = E_STR_ID_MESSAGE_37;
		break;
	case NETERR_LOGIN_FAIL_OVERFLOW:
		//本地不可能出现超过用户数目情况
		break;
	}
	//m_stLoginInfo.SetCaption(str, true);
	gui_assert( strID != static_cast<ENUM_STRING_ID>(E_STR_ID_NULL), printf("Login error. error code=%d.\n", m_loginErrorCode));
	MessageOut(strID,TIP_ERR,true,DEFAULT_BTN_OK|DEFAULT_ICON_ERROR);
}
