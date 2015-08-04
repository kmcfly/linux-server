/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-07-22
** Name         : LocalUIMan.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "LocalUIMan.h"
#include "ImageList.h"
#ifndef NDEBUG
#include "DebugProc.h"
#endif
#include "Font.h"

extern DVR_SYSTEM_STATUS	g_system_status;
extern bool g_langRightOrder;//gui.cpp

LANGUAGE		g_language = LANGUAGE_ENGLISH;
VGA_RESOLUTION	g_resolution = VGA_800X600;
unsigned long	g_timeMode = TIME_MODE_12;
unsigned long	g_dateMode = DATE_MODE_DMY;
unsigned long	g_showWizzard = 1;
unsigned long	g_screen_save_time = 0;
bool			g_bCloseWzd = false;
bool			g_bLogin = false;
bool			g_bChangOutDevice = false;
bool			g_b_rw_cfg = true;
NET_LOGIN_SUCCESS_REPLY *	g_p_login_succ = NULL;
WND_STARTUP_INFO*			g_p_wnd_startup_info = NULL;
WND_STARTUP_INFO*			g_p_cur_startup_info = NULL;

extern ULONGLONG g_net_channel = 0;

//////////////////////////////////////////////////////////////////////////
CLocalUIMan * CLocalUIMan::Instance()
{
	static CLocalUIMan s_LocalUIMan;
	return & s_LocalUIMan;
}

CLocalUIMan::CLocalUIMan() : m_lastKeyTime(0), m_pDisktop(NULL), m_keyProcID(PUB_THREAD_ID_NOINIT), m_bKeyProc(false)
{
	assert (NULL == g_p_login_succ);

#ifdef __ENVIRONMENT_LINUX__
	m_pLocalDeviceMan = NULL;
#endif

	g_p_login_succ = new NET_LOGIN_SUCCESS_REPLY;
	memset(g_p_login_succ, 0, sizeof(NET_LOGIN_SUCCESS_REPLY));

	g_p_wnd_startup_info = new WND_STARTUP_INFO;
	memset(g_p_wnd_startup_info, 0, sizeof(WND_STARTUP_INFO));

	g_p_cur_startup_info = new WND_STARTUP_INFO;
	memset(g_p_cur_startup_info, 0, sizeof(WND_STARTUP_INFO));
}

CLocalUIMan::~CLocalUIMan()
{
	if (NULL != g_p_login_succ)
	{
		delete g_p_login_succ;
		g_p_login_succ = NULL;
	}

	if(NULL != g_p_wnd_startup_info)
	{
		delete g_p_wnd_startup_info;
		g_p_wnd_startup_info =  NULL;
	}

	if(NULL != g_p_cur_startup_info)
	{
		delete g_p_cur_startup_info;
		g_p_cur_startup_info = NULL;
	}
}

#ifdef __ENVIRONMENT_WIN32__
bool CLocalUIMan::Initial(LANGUAGE language, unsigned short width, unsigned short height, unsigned long dateMode, unsigned long timeMode, const char *pFilePath, CDisplayCtrl *pDisplay)
{
	LoadColor(pFilePath);

	if(!m_pDisktop)
	{
		m_pDisktop = new CDisktop();
	}

	CProduct* pProduct = CProduct::Instance();

	GUI::CFBInterface::Instance()->Initial(pFilePath, pDisplay, pProduct->LocalVideoInputNum(), pProduct->VideoInputNum(), pProduct->VideoOutputNum(), pProduct->NetVideoInputNum());

	CStringTable::Instance()->Initial(language, pFilePath);

	//DVR上语言是否反序,必须先于开始录像
#if defined(__CUSTOM_IN25__) || defined(__CUSTOM_GB31__) || defined(__CUSTOM_PL14__)
	if((LANGUAGE_HEBREW == language) || (LANGUAGE_PERSIAN == language))
#else
	if((LANGUAGE_HEBREW == language) || (LANGUAGE_PERSIAN == language) || (LANGUAGE_FINNISH == language))
#endif
	{
		g_langRightOrder = true;
	}
	else
	{
		g_langRightOrder = false;
	}

	GUI::CImageList::Instance()->Initial();

	{
		unsigned long mode = CDateCtrl::DateMode(dateMode);

		CDateCtrl::SetDateMode(mode, false);
	}

	{
		unsigned long mode = CTimeCtrl::TimeMode(timeMode);

		CTimeCtrl::SetTimeMode(mode, true);
	}

	return true;
}
#else
bool CLocalUIMan::Initial(LANGUAGE language, const char *pFilePath, unsigned short & width, unsigned short & height, unsigned long dateMode, unsigned long timeMode, TRANSPARENCY trans/* = TRANSPARENCY_0*/)
{
	//
	LoadColor(pFilePath);

	if(!m_pDisktop)
	{
		m_pDisktop = new CDisktop();
	}

	//

	//
	GUI::CImage::SetTransparency(trans);
	GUI::CDrawManager::SetTransparency(trans);

	//
#ifdef __TDHISI__
	GUI::CImage::SetImageDataAddr(GUI::CFBInterface::Instance()->ImageBufAddr(0));
	GUI::CDrawManager::SetImageDataAddr(GUI::CFBInterface::Instance()->ImageBufAddr(1));
#endif

	GUI::CImageList::Instance()->Initial();

	m_pLocalDeviceMan = CLocalDevice::Instance();

	{
		unsigned long mode = CDateCtrl::DateMode(dateMode);
		CDateCtrl::SetDateMode(mode, false);
	}

	{
		unsigned long mode = CTimeCtrl::TimeMode(timeMode);
		CTimeCtrl::SetTimeMode(mode, false);
	}

	return true;
}
#endif

void CLocalUIMan::Quit()
{
	CStringTable::Instance()->Quit();
	GUI::CImageList::Instance()->Clear();
#ifdef __ENVIRONMENT_WIN32__
	GUI::CFBInterface::Instance()->Quit();
#endif
	GUI::CWnd::Quit();

	if(m_pDisktop)
	{
		delete m_pDisktop;
		m_pDisktop = NULL;
	}
}

void CLocalUIMan::SetProductInfo(const PRODUCT_INFO & productInfo)
{
	g_p_login_succ->productInfo = productInfo;
}

void CLocalUIMan::SetResolution(unsigned short width, unsigned short height)
{
	GUI::CFBInterface::Instance()->SetResolution(width, height);
	GUI::CWnd::SetSize(width, height);
	GUI::CImageList::Instance()->Load();

	//
	unsigned char *pAudio = GUI::CImageList::Instance()->GetImage(BMP_LIVE_AUDIO)->GetDate();
	unsigned char *pMotion = GUI::CImageList::Instance()->GetImage(BMP_LIVE_MOTION)->GetDate();
	unsigned char *pPTZ = GUI::CImageList::Instance()->GetImage(BMP_LIVE_PTZ)->GetDate();
	unsigned char *pWait = GUI::CImageList::Instance()->GetImage(BMP_WAITTING)->GetDate();
	unsigned char *pSnapSuccImage = GUI::CImageList::Instance()->GetImage(BMP_LIVE_SNAP_TIP)->GetDate();
	unsigned char *pSnapFailImage = GUI::CImageList::Instance()->GetImage(BMP_LIVE_SNAP_TIP_FAIL)->GetDate();

	unsigned char *pLogo = NULL;
	int logocx = 0, logocy = 0, logolen = 0;
	
#if defined(__DVR_LOGO__) || defined(__ENVIRONMENT_WIN32__)
	pLogo = GUI::CImageList::Instance()->GetImage(BMP_ICON_LOGO_TRANS)->GetDate();
	logocx = GUI::CImageList::Instance()->GetImage(BMP_ICON_LOGO_TRANS)->Width();
	logocy = GUI::CImageList::Instance()->GetImage(BMP_ICON_LOGO_TRANS)->Height();
	logolen = GUI::CImageList::Instance()->GetImage(BMP_ICON_LOGO_TRANS)->Size();
#endif

	GUI::CFBInterface::Instance()->SetImageAddr(pAudio, pMotion, pPTZ, pWait, pSnapSuccImage, pSnapFailImage, pLogo, logocx, logocy, logolen);

	GUI::CFBInterface::Instance()->Clear(0, 0, width, height);
	GUI::CFBInterface::Instance()->Refresh(0, 0, width, height);
}

bool CLocalUIMan::Start()
{
	m_keyProcID = PUB_CreateThread(KeyProcThread, this, &m_bKeyProc);
	if(m_keyProcID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	return true;
}

void CLocalUIMan::DoModal()
{
	do 
	{
		m_lastKeyTime = 0;
		g_system_status = DVR_SYSTEM_RUNING;

		m_pDisktop->DoModal();
	} while(DVR_SYSTEM_LOGOUT == g_system_status);
}

void CLocalUIMan::Stop()
{
	if (PUB_THREAD_ID_NOINIT != m_keyProcID)
	{		
		PUB_ExitThread(&m_keyProcID, &m_bKeyProc);
	}

	m_pDisktop->Quit();
}
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

PUB_THREAD_RESULT PUB_THREAD_CALL CLocalUIMan::KeyProcThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CLocalUIMan *pThis = reinterpret_cast<CLocalUIMan *>(pParam);
	pThis->KeyProc();

	return 0;
}

int CLocalUIMan::KeyProc()
{
#ifdef __ENVIRONMENT_LINUX__
	KEY_VALUE_INFO keyValue;

#if defined(__CHIPGM__)
	int keyLeftDownMoveTimes = 0;
#endif

	while (m_bKeyProc)
	{
		if (m_pLocalDeviceMan->GetKeyInput(keyValue))
		{
# if 0
			if ((KEY_TYPE_MOUSE == keyValue.type) && (E_MOUSE_KEY_LEFT_DOWN == keyValue.value))
                        {
                                printf("%s:%s:%d, key type=%d, key value=%d, mouse left down at(%d:%d)\n", __FUNCTION__, __FILE__, __LINE__, keyValue.type, keyValue.value, keyValue.xPos, keyValue.yPos);
                        }
			if ((KEY_TYPE_MOUSE == keyValue.type) && (E_MOUSE_KEY_LEFT_UP == keyValue.value))
                        {
                                printf("%s:%s:%d, key type=%d, key value=%d, mouse left up at(%d:%d)\n", __FUNCTION__, __FILE__, __LINE__, keyValue.type, keyValue.value, keyValue.xPos, keyValue.yPos);
                        }
                        if ((KEY_TYPE_MOUSE == keyValue.type) && (E_MOUSE_KEY_RIGHT_DOWN == keyValue.value))
                        {
                                printf("%s:%s:%d, key type=%d, key value=%d, mouse right down at(%d:%d)\n", __FUNCTION__, __FILE__, __LINE__, keyValue.type, keyValue.value, keyValue.xPos, keyValue.yPos);
                        }
                        if ((KEY_TYPE_MOUSE == keyValue.type) && (E_MOUSE_KEY_MOUSE_MOVE == keyValue.value))
                        {
                                static int moveCount = 0;
                                if (0 == moveCount % 100)
                                {
                                        printf("%s:%s:%d, key type=%d, key value=%d, mouse move at(%d:%d)\n", __FUNCTION__, __FILE__, __LINE__, keyValue.type, keyValue.value, keyValue.xPos, keyValue.yPos);
                                }
                                moveCount ++;
                                moveCount = moveCount % 100;
                        }
#endif
			//2014-06-06 13:46:00 ZYD
#if defined(__CHIPGM__)
			if ((KEY_TYPE_MOUSE == keyValue.type) && (E_MOUSE_KEY_LEFT_DOWN_MOVE == keyValue.value))
			{
				if (15 != keyLeftDownMoveTimes)
				{
					keyLeftDownMoveTimes ++; 
					continue;
				}
				keyLeftDownMoveTimes = 0;
				//printf("%s:%s:%d, key type=%d, key value=%d, mouse left down move at(%d:%d)\n", __FUNCTION__, __FILE__, __LINE__, keyValue.type, keyValue.value, keyValue.xPos, keyValue.yPos);
			}
#endif

			GUI::CDialog::AddKeyValue(keyValue);
                        PUB_Sleep( 10 );//控制鼠标操作的灵敏度，不能太灵敏。
                        //cout << "  type:"<< keyValue.type << "  value:"<<keyValue.value << "  x:"<<keyValue.xPos << "  y:"<<keyValue.yPos << endl;

			m_lastKeyTime = 0;

			//2010-02-03 11:44:00 YSW
			//当不是鼠标移动时，停止自动调试。可以在调试配置窗体再次开启。
#ifndef NDEBUG
			unsigned long keyID = CKeyProc::TranslateKey(KEY_MODE_UI, keyValue);
			if ((KEY_VIRTUAL_MOVE != keyID) && (KEY_VIRTUAL_ESC != keyID))
			{
				CDebugProc::Instance()->Runing(false);
			}
#endif
		}
		else
		{
			//检查一些东西的变动

			//检查权限设置的变动
			g_p_login_succ->productInfo.bPasswordCheck = CMessageMan::Instance()->CheckPassword();

			PUB_Sleep(50);
			
			/*printf("%s,%d,g_bLogin:%d, bPasswordCheck:%d,g_screen_save_time:%d,\
				   Freeze:%d,m_lastKeyTime:%d\n",
				   __FILE__,__LINE__,g_bLogin,
				   g_p_login_succ->productInfo.bPasswordCheck,
				   g_screen_save_time,
				   !CMessageMan::Instance()->CheckFreezeScreenSave(),
				   m_lastKeyTime);*/

			//如果不进行检测密码，设置的屏保无效。
			if ((g_bLogin)
			  &&(g_p_login_succ->productInfo.bPasswordCheck)
			  &&(!CMessageMan::Instance()->CheckFreezeScreenSave())
			  &&(0 < g_screen_save_time))
			{
				if ((g_screen_save_time * 20) <= m_lastKeyTime)
				{
					g_system_status = DVR_SYSTEM_LOGOUT;
				}
				else
				{
					++ m_lastKeyTime;
				}
			}
		}
	}
#else
	while (m_bKeyProc)
	{
		//检查一些东西的变动

		//检查权限设置的变动
		g_p_login_succ->productInfo.bPasswordCheck = CMessageMan::Instance()->CheckPassword();

		PUB_Sleep(50);

		/*if (g_bLogin && (0 < g_screen_save_time) && (m_lastKeyTime + g_screen_save_time) <= GetCurrTime32())
		{
			g_system_status = DVR_SYSTEM_LOGOUT;
		}*/
	}
#endif

	return 0;
}
//////////////////////////////////////////////////////////////////////////

unsigned long CLocalUIMan::GetColor(CIniReader& ini, const std::string& str_r, const std::string& str_g, const std::string& str_b) const
{
	BYTE R = 0;
	BYTE G = 0;
	BYTE B = 0;

	std::string str = ini.GetKeyValue(str_r );
	R = atoi(str.c_str());
	str = ini.GetKeyValue(str_g );
	G = atoi(str.c_str());
	str = ini.GetKeyValue(str_b );
	B = atoi(str.c_str());

	return GUI_RGB(R, G, B);
}

bool CLocalUIMan::LoadColor(const char* filepath)
{
	CIniReader ini;

	if (ini.Initial(filepath))
	{
		COLOR_TEXT_NORMAL = GetColor(ini, "COLOR_TEXT_NORMAL_R", "COLOR_TEXT_NORMAL_G", "COLOR_TEXT_NORMAL_B");
		COLOR_TEXT_TITLE = GetColor(ini, "COLOR_TEXT_TITLE_R", "COLOR_TEXT_TITLE_G", "COLOR_TEXT_TITLE_B");
		COLOR_TEXT_FOCUS = GetColor(ini, "COLOR_TEXT_FOCUS_R", "COLOR_TEXT_FOCUS_G", "COLOR_TEXT_FOCUS_B");
		COLOR_TEXT_DISABLE = GetColor(ini, "COLOR_TEXT_DISABLE_R", "COLOR_TEXT_DISABLE_G", "COLOR_TEXT_DISABLE_B");

		COLOR_TEXT_BORDER = GetColor(ini, "COLOR_TEXT_BORDER_R", "COLOR_TEXT_BORDER_G", "COLOR_TEXT_BORDER_B");
		COLOR_TEXT_BORDER_ENABLE = GetColor(ini, "COLOR_TEXT_BORDER_ENABLE_R", "COLOR_TEXT_BORDER_ENABLE_G", "COLOR_TEXT_BORDER_ENABLE_B");

		COLOR_DLG_NORMAL_BK = GetColor(ini, "COLOR_DLG_NORMAL_BK_R", "COLOR_DLG_NORMAL_BK_G", "COLOR_DLG_NORMAL_BK_B");
		COLOR_DLG_CHILD_BK = GetColor(ini, "COLOR_DLG_CHILD_BK_R", "COLOR_DLG_CHILD_BK_G", "COLOR_DLG_CHILD_BK_B");
		COLOR_DLG_BORDER_LEFT = GetColor(ini, "COLOR_DLG_BORDER_LEFT_R", "COLOR_DLG_BORDER_LEFT_G", "COLOR_DLG_BORDER_LEFT_B");
		COLOR_DLG_BORDER_BOTTOM = GetColor(ini, "COLOR_DLG_BORDER_BOTTOM_R", "COLOR_DLG_BORDER_BOTTOM_G", "COLOR_DLG_BORDER_BOTTOM_B");
		COLOR_SELECT = GetColor(ini, "COLOR_SELECT_R", "COLOR_SELECT_G", "COLOR_SELECT_B");
		COLOR_DISABLE = GetColor(ini, "COLOR_DISABLE_R", "COLOR_DISABLE_G", "COLOR_DISABLE_B");
		COLOR_EDIT = GetColor(ini, "COLOR_EDIT_R", "COLOR_EDIT_G", "COLOR_EDIT_B");

		COLOR_LINE_BLACK = GetColor(ini, "COLOR_LINE_BLACK_R", "COLOR_LINE_BLACK_G", "COLOR_LINE_BLACK_B");
		COLOR_LINE_WHITE = GetColor(ini, "COLOR_LINE_WHITE_R", "COLOR_LINE_WHITE_G", "COLOR_LINE_WHITE_B");
		COLOR_LINE_GRAY = GetColor(ini, "COLOR_LINE_GRAY_R", "COLOR_LINE_GRAY_G", "COLOR_LINE_GRAY_B");
		
		COLOR_CAL_TODAYBTN_BK = GetColor(ini, "COLOR_CAL_TODAYBTN_BK_R", "COLOR_CAL_TODAYBTN_BK_G", "COLOR_CAL_TODAYBTN_BK_B");
		COLOR_BTN_DEFAULT_MARK = GetColor(ini, "COLOR_BTN_DEFAULT_MARK_R", "COLOR_BTN_DEFAULT_MARK_G", "COLOR_BTN_DEFAULT_MARK_B");
		COLOR_BTN_BK = GetColor(ini, "COLOR_BTN_BK_R", "COLOR_BTN_BK_G", "COLOR_BTN_BK_B");

		COLOR_SCROLL_BK = GetColor(ini, "COLOR_SCROLL_BK_R", "COLOR_SCROLL_BK_G", "COLOR_SCROLL_BK_B");

		COLOR_MENU_BK = GetColor(ini, "COLOR_MENU_BK_R", "COLOR_MENU_BK_G", "COLOR_MENU_BK_B");
		COLOR_MENU_BK_FOCUS = GetColor(ini, "COLOR_MENU_BK_FOCUS_R", "COLOR_MENU_BK_FOCUS_G", "COLOR_MENU_BK_FOCUS_B");

		ini.Quit();

		bool bDrawFontBorder = true;
		if(COLOR_TEXT_BORDER_ENABLE != INVALID_INDEX(unsigned long))
		{
			bDrawFontBorder = false;
		}
		GUI::CFont::SetDrawBorder(bDrawFontBorder, COLOR_TEXT_BORDER);

		return true;	
	}
	else
	{
		assert(false);
		return false;
	}
}
//end
