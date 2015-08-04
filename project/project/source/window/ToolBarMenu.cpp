
#include "ToolBarMenu.h"
#include "Product.h"
#include "NetProtocol.h"
#include "FuncCustom.h"

#include "UserMan.h"
#include "CfgDlg.h"
using namespace GUI;

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;


//////////////////////////////////////////////////////////////////////////

CToolBarMenu::CToolBarMenu() : m_maxChnn(-1),
m_xPos(0),
m_yPos(0)
{

}

CToolBarMenu::~CToolBarMenu()
{

}

void CToolBarMenu::InitMenuData()
{
	Clear();

	m_maxChnn = g_p_login_succ->productInfo.videoInputNum;

	////
	CMenuExItem* pItem = NULL;
	char str[128 + 4] = {0};

	////设置画面通道
	CMenuEx *pSubMenuSingle = new CMenuEx();
	CMenuEx *pSubMenuSingle_More = new CMenuEx();

	int maxGroup = m_maxChnn;

	//1x1
	const std::string strChnn = m_pStrTable->String(E_STR_ID_CHANNEL);

	for (int i = 0; i < m_maxChnn; i++)
	{
		snprintf(str, 128, "%s %d", strChnn.c_str(), i + 1);

		if (16 > i)
		{
			pItem = pSubMenuSingle->AddItem(str, static_cast<unsigned long>(MITEM_ID_1X1 + i));
		}
		else
		{
			pItem = pSubMenuSingle_More->AddItem(str, static_cast<unsigned long>(MITEM_ID_1X1 + i));
		}
		
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_SINGLE);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_SINGLE_F);
		pItem->SetCurState(0, false);
	}

	CMenuEx *pSubMenuPip = new CMenuEx();
	CMenuEx *pSubMenuPip_More = new CMenuEx();
	for (int i = 0; i< m_maxChnn; i++)
	{
		snprintf(str, 128, "%s %d", strChnn.c_str(), i + 1);
		if (16 > i)
		{
			pItem = pSubMenuPip->AddItem(str, static_cast<unsigned long>(MITEM_ID_P_1X1 + i));
		}
		else
		{
			pItem = pSubMenuPip_More->AddItem(str, static_cast<unsigned long>(MITEM_ID_P_1X1 + i));
		}
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_SINGLE);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_SINGLE_F);
		pItem->SetCurState(0, false);
	}

	////
	CMenuEx *pSubMenuMulti_Regular = new CMenuEx;
	CMenuEx *pSubMenuMulti_UnRegular = new CMenuEx;
	int mulitMaxChnn =IsSupportHybridDisplay()?(g_p_login_succ->productInfo.videoInputNum):g_p_login_succ->productInfo.localVideoInputNum;
	//2x2
	if(mulitMaxChnn >= 4)
	{
		maxGroup = (mulitMaxChnn + 3) / 4;
		for (int i = 0; i < maxGroup; i++)
		{
			int iMax = ((i * 4 + 4) < mulitMaxChnn)?(i * 4 + 4):mulitMaxChnn;
			snprintf(str, 128, "%s %d~%d", strChnn.c_str(), i * 4 + 1, iMax);
			pItem = pSubMenuMulti_Regular->AddItem(str, static_cast<unsigned long>(MITEM_ID_2X2 + i));
			pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_2X2);
			pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_2X2_F);
			pItem->SetCurState(0, false);
		}
	}

	//2x3
	if(mulitMaxChnn >= 5)
	{
		pSubMenuMulti_UnRegular->AddItem();
		maxGroup = (mulitMaxChnn + 5 ) / 6;
		for (int i = 0; i < maxGroup; i++)
		{
			int iMax = ((i * 6 + 6) < mulitMaxChnn)?(i * 6 + 6):mulitMaxChnn;
			snprintf(str, 128, "%s %d~%d", strChnn.c_str(), i * 6 + 1, iMax);
			pItem = pSubMenuMulti_UnRegular->AddItem(str, static_cast<unsigned long>(MITEM_ID_2X3 + i));
			pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_2X3);
			pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_2X3_F);
			pItem->SetCurState(0, false);
		}
	}

	//VIEW_SPLIT_1A5 3x3
	if(mulitMaxChnn >= 6)
	{
		pSubMenuMulti_UnRegular->AddItem();
		maxGroup = (mulitMaxChnn + 5) / 6;
		for (int i = 0; i < maxGroup; i++)
		{
			int iMax = ((i * 6+ 6) < mulitMaxChnn)?(i * 6 + 6):mulitMaxChnn;
			snprintf(str, 128, "%s %d~%d", strChnn.c_str(), i * 6 + 1, iMax);
			pItem = pSubMenuMulti_UnRegular->AddItem(str, static_cast<unsigned long>(MITEM_ID_1A5 + i));
			pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_1A5);
			pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_1A5_F);
			pItem->SetCurState(0, false);
		}
	}
	
	//VIEW_SPLIT_1A7 4x4
	if(mulitMaxChnn >= 8)
	{
		pSubMenuMulti_UnRegular->AddItem();
		maxGroup = (mulitMaxChnn + 7) / 8;
		for (int i = 0; i < maxGroup; i++)
		{
			int iMax = ((i * 8+ 8) < mulitMaxChnn)?(i * 8 + 8):mulitMaxChnn;
			snprintf(str, 128, "%s %d~%d", strChnn.c_str(), i * 8 + 1, iMax);
			pItem = pSubMenuMulti_UnRegular->AddItem(str, static_cast<unsigned long>(MITEM_ID_1A7 + i));
			pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_1A7);
			pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_1A7_F);
			pItem->SetCurState(0, false);
		}
	}

	//3x3
	if(mulitMaxChnn >= 8)
	{
		pSubMenuMulti_Regular->AddItem();
		maxGroup = (mulitMaxChnn + 8) / 9;
		for (int i = 0; i < maxGroup; i++)
		{
			int iMax = ((i * 9 + 9) < mulitMaxChnn)?(i * 9 + 9):mulitMaxChnn;
			snprintf(str, 128, "%s %d~%d", strChnn.c_str(), i * 9 + 1, iMax);
			pItem = pSubMenuMulti_Regular->AddItem(str, static_cast<unsigned long>(MITEM_ID_3X3 + i));
			pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_3X3);
			pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_3X3_F);
			pItem->SetCurState(0, false);
		}
	}

	//VIEW_SPLIT_1A12 4x4
	if(mulitMaxChnn >= 13)
	{
		pSubMenuMulti_UnRegular->AddItem();
		maxGroup = (mulitMaxChnn + 12) / 13;
		for (int i = 0; i < maxGroup; i++)
		{
			int iMax = ((i * 13+ 13) < mulitMaxChnn)?(i * 13 + 13):mulitMaxChnn;
			snprintf(str, 128, "%s %d~%d", strChnn.c_str(), i * 13 + 1, iMax);
			pItem = pSubMenuMulti_UnRegular->AddItem(str, static_cast<unsigned long>(MITEM_ID_1A12 + i));
			pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_1A12);
			pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_1A12_F);
			pItem->SetCurState(0, false);
		}
	}

	//4x4
	if(mulitMaxChnn >= 12)
	{
		pSubMenuMulti_Regular->AddItem();
		maxGroup = (mulitMaxChnn + 15) / 16;
		for (int i = 0; i < maxGroup; i++)
		{
			int iMax = ((i * 16+ 16) < mulitMaxChnn)?(i * 16 + 16):mulitMaxChnn;
			snprintf(str, 128, "%s %d~%d", strChnn.c_str(), i * 16 + 1, iMax);
			pItem = pSubMenuMulti_Regular->AddItem(str, static_cast<unsigned long>(MITEM_ID_4X4 + i));
			pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_MULTI);
			pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_MULTI_F);
			pItem->SetCurState(0, false);
		}
	}

	//5x5
	if(mulitMaxChnn >= 18)
	{
		pSubMenuMulti_Regular->AddItem();
		maxGroup = (mulitMaxChnn + 24) / 25;
		for (int i = 0; i < maxGroup; i++)
		{
			int iMax = ((i * 25+ 25) < mulitMaxChnn)?(i * 25 + 25):mulitMaxChnn;
			snprintf(str, 128, "%s %d~%d", strChnn.c_str(), i * 25 + 1, iMax);
			pItem = pSubMenuMulti_Regular->AddItem(str, static_cast<unsigned long>(MITEM_ID_5X5 + i));
			pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_MULTI_5X5);
			pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_MULTI_5X5_F);
			pItem->SetCurState(0, false);
		}
	}

	//6x6
	if(mulitMaxChnn >= 32)
	{
		pSubMenuMulti_Regular->AddItem();

		int iMax = mulitMaxChnn;
		snprintf(str, 128, "%s %d~%d", strChnn.c_str(), 1, iMax);
		pItem = pSubMenuMulti_Regular->AddItem(str, static_cast<unsigned long>(MITEM_ID_6X6));
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_MULTI_6X6);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_MULTI_6X6_F);
		pItem->SetCurState(0, false);
	}

	//单画面
	if (16 >= m_maxChnn)
	{
		pItem = AddItem(m_pStrTable->String(E_STR_ID_MENU_CHNN_SINGLE), MITEM_ID_SINGLE, pSubMenuSingle);
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_SINGLE);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_SINGLE_F);
		pItem->SetCurState(0, false);
	}
	else
	{
		char szTemp[40] = {0};
		snprintf(szTemp, sizeof(szTemp) - 1, "%s(1~16)", m_pStrTable->String(E_STR_ID_MENU_CHNN_SINGLE));
		pItem = AddItem(szTemp, MITEM_ID_SINGLE, pSubMenuSingle);
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_SINGLE);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_SINGLE_F);
		pItem->SetCurState(0, false);

		memset(szTemp, 0, sizeof(szTemp));
		snprintf(szTemp, sizeof(szTemp) - 1, "%s(17~%d)", m_pStrTable->String(E_STR_ID_MENU_CHNN_SINGLE), m_maxChnn);
		pItem = AddItem(szTemp, MITEM_ID_SINGLE_MORE, pSubMenuSingle_More);
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_SINGLE);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_SINGLE_F);
		pItem->SetCurState(0, false);
	}

	//多画面
	if (5 > mulitMaxChnn)
	{
		pItem = AddItem(m_pStrTable->String(E_STR_ID_MENU_CHNN_MULTI), MITEM_ID_MULTI, pSubMenuMulti_Regular);
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_MULTI);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_MULTI_F);
		pItem->SetCurState(0, false);
	}
	else
	{
		char szTemp[40] = {0};
		snprintf(szTemp, sizeof(szTemp) - 1, "%s", m_pStrTable->String(E_STR_ID_MENU_CHNN_MULTI));
		pItem = AddItem(szTemp, MITEM_ID_MULTI, pSubMenuMulti_Regular);
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_MULTI);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_MULTI_F);
		pItem->SetCurState(0, false);

		memset(szTemp, 0, sizeof(szTemp));
		snprintf(szTemp, sizeof(szTemp) - 1, "%s", m_pStrTable->String(E_STR_ID_MENU_CHNN_MULTI));
		pItem = AddItem(szTemp, MITEM_ID_MULTI_UNREGULAR, pSubMenuMulti_UnRegular);
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_MULTI_UNREGULAR);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_MULTI_UNREGULAR_F);
		pItem->SetCurState(0, false);
	}

	//画中画
	if (IsSupportPip())
	{
		if (16 >= m_maxChnn)
		{
			pItem = AddItem(m_pStrTable->String(E_STR_ID_MENU_PIP), MITEM_ID_PIP, pSubMenuPip);
			pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_PIP);
			pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_PIP_F);
			pItem->SetCurState(0, false);
		}
		else
		{
			char szTemp[40] = {0};
			snprintf(szTemp, sizeof(szTemp) - 1, "%s(1~16)", m_pStrTable->String(E_STR_ID_MENU_PIP));
			pItem = AddItem(szTemp, MITEM_ID_PIP, pSubMenuPip);
			pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_PIP);
			pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_PIP_F);
			pItem->SetCurState(0, false);

			memset(szTemp, 0, sizeof(szTemp));
			snprintf(szTemp, sizeof(szTemp) - 1, "%s(17~%d)", m_pStrTable->String(E_STR_ID_MENU_PIP), m_maxChnn);
			pItem = AddItem(szTemp, MITEM_ID_PIP_MORE, pSubMenuPip_More);
			pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_PIP);
			pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_PIP_F);
			pItem->SetCurState(0, false);
		}
	}
	
	
	//分割线
	AddItem();

#ifdef __DVR_BASIC__
	//跳台
	pItem = AddItem(m_pStrTable->String(E_STR_ID_MENU_DWELL), KEY_VIRTUAL_SEQ);
	pItem->SetStateImage(DWELL_OPEN, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_DWELL);
	pItem->SetStateImage(DWELL_OPEN, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_DWELL_F);
	pItem->SetStateImage(DWELL_CLOSE, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_DWELL_NO);
	pItem->SetStateImage(DWELL_CLOSE, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_DWELL_NO_F);
	pItem->SetCurState(DWELL_CLOSE, false);
#endif


	if(IsSupportChangeToolBarColor())
	{
		//调节色彩 MITEM_ID_COLOR 
		pItem = AddItem(m_pStrTable->String(E_STR_ID_COLOR), static_cast<unsigned long>(MITEM_ID_COLOR));
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_SET_COLOR);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_SET_COLOR_F);
		pItem->SetCurState(0, false);
	}
	
	{   
		//设置摄像机参数
		pItem = AddItem(m_pStrTable->String(E_STR_ID_CAM_PARA), static_cast<unsigned long>(MITEM_ID_CAM_PARA));
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_SET_CAM_PARA);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_SET_CAM_PARA_F);
		pItem->SetCurState(0, false);
	}



	pItem = AddItem(m_pStrTable->String(E_STR_ID_MENU_DIGITAL_ZOOM), KEY_VIRTUAL_ZOOM);
	pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_ZOOM);
	pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_ZOOM_F);
	pItem->SetCurState(0, false);

	if ((0 < g_p_login_succ->productInfo.audioInputNum) || (0 < g_p_login_succ->productInfo.netVideoInputNum))
	{
		//音量
		pItem = AddItem(m_pStrTable->String(E_STR_ID_MENU_AUDIO), KEY_VIRTUAL_AUDIO);
		pItem->SetStateImage(AUDIO_OPEN, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_AUDIO);
		pItem->SetStateImage(AUDIO_OPEN, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_AUDIO_F);
		pItem->SetStateImage(AUDIO_MUTE, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_AUDIO_NO);
		pItem->SetStateImage(AUDIO_MUTE, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_AUDIO_NO_F);
		pItem->SetCurState(AUDIO_OPEN, false);
	}
#if !defined(__TW01_RILI__)	
	if(IsSupportPTZ()/* && (CProduct::Instance()->LocalVideoInputNum() > 0)*/)
	{
		//云台控制
		pItem = AddItem(m_pStrTable->String(E_STR_ID_MENU_PTZ), KEY_VIRTUAL_PTZ);
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_PTZ);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_PTZ_F);
		pItem->SetCurState(0, false);
	}
#endif
	if (IsSupportSnapPic()/* && (CProduct::Instance()->LocalVideoInputNum() > 0)*/)
	{
		//抓图
		pItem = AddItem(m_pStrTable->String(E_STR_ID_SNAP),static_cast<unsigned long>(MITEM_ID_SNAP));
		pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_SNAP);
		pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_SNAP_F);
		pItem->SetCurState(0, false);
	}

	//录像
	pItem = AddItem(m_pStrTable->String(E_STR_ID_MENU_RECORD), KEY_VIRTUAL_RECORD);
	pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_REC);
	pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_REC_F);
	pItem->SetCurState(0, false);

	//回放
	pItem = AddItem(m_pStrTable->String(E_STR_ID_MENU_PLAYBACK), KEY_VIRTUAL_PLAY);
	pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_PLAYBACK);
	pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_PLAYBACK_F);
	pItem->SetCurState(0, false);



	//分割线
	AddItem();

	//主菜单
	pItem = AddItem(m_pStrTable->String(E_STR_ID_MENU_MAIN), KEY_VIRTUAL_MENU);
	pItem->SetStateImage(0, PAINT_STATUS_NORMAL, BMP_MENU_ITEM_MAIN);
	pItem->SetStateImage(0, PAINT_STATUS_FOCUS, BMP_MENU_ITEM_MAIN_F);
	pItem->SetCurState(0, false);
}

bool CToolBarMenu::GetSelChnn(ULONGLONG& CH, unsigned long& splitMode, unsigned long& n)
{
	if(m_bClickChnn)
	{
		CH = m_selChnnMask;
		splitMode = m_splitMode;
		n = m_maxN;

		return true;
	}

	return false;
}

unsigned long CToolBarMenu::PopupProc(unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	m_bClickChnn = false;
	m_selChnnMask = 0;
	m_selPipChnn = 0;

	if((xPos==INVALID_INDEX(unsigned long))&&(yPos ==INVALID_INDEX(unsigned long)))
	{
		//说明是名扬键盘
		
		unsigned short right = CFBInterface::Instance()->GetResolutionWidth()-1;
		unsigned short bottom = CFBInterface::Instance()->GetResolutionHeight()-1;
		
		m_xPos = right /2;
		m_yPos = bottom/2;

	}
	else
	{
		m_xPos = xPos;
		m_yPos = yPos;
	}
	printf("m_xpos=%d.m_ypos=%d\n",m_xPos,m_yPos);
	//菜单子项前处理
	PreUpdateUI();

	unsigned long ret = Popup(xPos, yPos);

	if(GUI::INVALID_ID != ret)
	{
		if((ret >= MITEM_ID_1X1) && (ret < MITEM_ID_NXN))
		{
			ret = OnClickNXN(ret);
		}
		else if((ret >= MITEM_ID_P_1X1) && (ret < MITEM_ID_P_1X1 + m_maxChnn))
		{
			ret = OnClickPip(ret);
		}
		else if(ret == MITEM_ID_COLOR)
		{
			ret=OnClickChange(ret);
		}
		else if(ret == MITEM_ID_SNAP)
		{
			ret = OnClickSnap(ret);
		}else if (ret == MITEM_ID_CAM_PARA)
		{
			ret=OnClickCamPara(ret);
		}
	}

	return ret;
}
unsigned long CToolBarMenu::OnClickChange(unsigned long itemID)
{
	return MITEM_ID_COLOR;

}

unsigned long CToolBarMenu::OnClickCamPara(unsigned long itemID)
{
	return MITEM_ID_CAM_PARA;
}

unsigned long CToolBarMenu::OnClickSnap(unsigned long itemID)
{
	return MITEM_ID_SNAP;
}

void CToolBarMenu::GetPtPos(unsigned long& xPos, unsigned long& yPos)
{
	xPos = m_xPos;
	yPos = m_yPos;
}
void CToolBarMenu::EnableDwell(bool bOpen)
{
#ifdef __DVR_BASIC__
	CMenuExItem* pItem = GetItem(KEY_VIRTUAL_SEQ, this);
	if (bOpen)
	{
		pItem->SetCurState(DWELL_OPEN, false);
	}
	else
	{
		pItem->SetCurState(DWELL_CLOSE, false);
	}
#endif
}

void CToolBarMenu::EnableAudio(bool bOpen)
{
	if (0 < g_p_login_succ->productInfo.audioInputNum)
	{
		CMenuExItem* pItem = GetItem(KEY_VIRTUAL_AUDIO, this);
		if (bOpen)
		{
			pItem->SetCurState(AUDIO_OPEN, false);
		}
		else
		{
			pItem->SetCurState(AUDIO_MUTE, false);
		}
	}
}

void CToolBarMenu::PreUpdateUI()
{
	
}

unsigned long CToolBarMenu::OnClickPip(unsigned long itemID)
{
	int group = 0;
	m_selPipChnn = 0;
	if(m_splitMode == VIEW_SPLIT_1X1)
	{
		group = itemID - MITEM_ID_P_1X1;

		if(group < m_maxChnn)
		{
			m_bClickChnn = true;
			m_selPipChnn =  group;
		}
		//无权限的通道返回无效ID
		ULONGLONG authCH = CUserMan::Instance()->CheckAuthority(AUTH_LIVE);
		if(((static_cast<ULONGLONG>(0x1) << m_selPipChnn) & authCH) == 0)
		{
			return GUI::INVALID_ID;
		}

		if((static_cast<ULONGLONG>(0x1) << m_selPipChnn) == m_selChnnMask)
		{
			return GUI::INVALID_ID;
		}
	}
	else
	{
		return GUI::INVALID_ID;
	}

	if(m_bClickChnn)
	{
		return MITEM_ID_P_1X1;
	}
}

unsigned long CToolBarMenu::OnClickNXN(unsigned long itemID)
{
	int group = 0;
	m_selChnnMask = 0;
	m_splitMode = 0;
	m_maxN = 0;

	if(itemID >= MITEM_ID_6X6)
	{
		group = itemID - MITEM_ID_6X6;

		for (int i = group * 36; i < m_maxChnn; i++)
		{
			if(i >= (group + 1) * 36)
			{
				break;
			}

			m_bClickChnn = true;
			m_selChnnMask |= (static_cast<ULONGLONG>(0x1) << i);
		}

		m_splitMode = MITEM_ID_6X6;
		m_maxN = ((32 < m_maxChnn) ? 36 : 32);
	}
	else if(itemID >= MITEM_ID_5X5)
	{
		group = itemID - MITEM_ID_5X5;

		for (int i = group * 25; i < m_maxChnn; i++)
		{
			if(i >= (group + 1) * 25)
			{
				break;
			}

			m_bClickChnn = true;
			m_selChnnMask |= (static_cast<ULONGLONG>(0x1) << i);
		}

		m_splitMode = MITEM_ID_5X5;
		m_maxN = ((24 < m_maxChnn) ? 25 : 24);
	}
	else if(itemID >= MITEM_ID_1A12)
	{
		group = itemID - MITEM_ID_1A12;

		for (int i = group * 13; i < m_maxChnn; i++)
		{
			if(i >= (group + 1) * 13)
			{
				break;
			}

			m_bClickChnn = true;
			m_selChnnMask |= (static_cast<ULONGLONG>(0x1) << i);
		}

		m_splitMode = VIEW_SPLIT_1A12;
		m_maxN = 13;
	}
	else if(itemID >= MITEM_ID_1A7)
	{
		group = itemID - MITEM_ID_1A7;

		for (int i = group * 8; i < m_maxChnn; i++)
		{
			if(i >= (group + 1) * 8)
			{
				break;
			}

			m_bClickChnn = true;
			m_selChnnMask |= (static_cast<ULONGLONG>(0x1) << i);
		}

		m_splitMode = VIEW_SPLIT_1A7;
		m_maxN = 8;
	}
	else if(itemID >= MITEM_ID_4X4)
	{
		group = itemID - MITEM_ID_4X4;

		for (int i = group * 16; i < m_maxChnn; i++)
		{
			if(i >= (group + 1) * 16)
			{
				break;
			}
			
			m_bClickChnn = true;
			m_selChnnMask |= (static_cast<ULONGLONG>(0x1) << i);
		}

		m_splitMode = VIEW_SPLIT_4X4;
		m_maxN = 16;
	}
	else if(itemID >= MITEM_ID_1A5)
	{
		group = itemID - MITEM_ID_1A5;

		for (int i = group * 6; i < m_maxChnn; i++)
		{
			if(i >= (group + 1) * 6)
			{
				break;
			}

			m_bClickChnn = true;
			m_selChnnMask |= (static_cast<ULONGLONG>(0x1) << i);
		}

		m_splitMode = VIEW_SPLIT_1A5;
		m_maxN = 6;
	}
	else if(itemID >= MITEM_ID_3X3)
	{
		group = itemID - MITEM_ID_3X3;

		for (int i = group * 9; i < m_maxChnn; i++)
		{
			if(i >= (group + 1) * 9)
			{
				break;
			}

			m_bClickChnn = true;
			m_selChnnMask |= (static_cast<ULONGLONG>(0x1) << i);
		}

		m_splitMode = VIEW_SPLIT_3X3;
		m_maxN = ((8 < m_maxChnn) ? 9 : 8);
	}
	else if(itemID >= MITEM_ID_2X3)
	{
		group = itemID - MITEM_ID_2X3;

		for (int i = group * 6; i < m_maxChnn; i++)
		{
			if(i >= (group + 1) * 6)
			{
				break;
			}

			m_bClickChnn = true;
			m_selChnnMask |= (static_cast<ULONGLONG>(0x1) << i);
		}

		m_splitMode = VIEW_SPLIT_2X3;
		m_maxN = 6;
	}
	else if(itemID >= MITEM_ID_2X2)
	{
		group = itemID - MITEM_ID_2X2;

		for (int i = group * 4; i < m_maxChnn; i++)
		{
			if(i >= (group + 1) * 4)
			{
				break;
			}

			m_bClickChnn = true;
			m_selChnnMask |= (static_cast<ULONGLONG>(0x1) << i);
		}

		m_splitMode = VIEW_SPLIT_2X2;
		m_maxN = 4;
	}
	else if(itemID >= MITEM_ID_1X1)
	{
		group = itemID - MITEM_ID_1X1;

		if(group < m_maxChnn)
		{
			m_bClickChnn = true;
			m_selChnnMask |= (static_cast<ULONGLONG>(0x1) << group);
			m_splitMode = VIEW_SPLIT_1X1;
			m_maxN = 1;
		}
	}

	if (m_maxN > m_maxChnn)
	{
		m_maxN = m_maxChnn;
	}
	
	////
	if(m_bClickChnn)
	{
		return MITEM_ID_NXN;
	}
	else
	{
		return GUI::INVALID_ID;
	}
}

//end
