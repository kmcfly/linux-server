#include "WzdNatPassWordDlg.h"



CWzdNatPassWordDlg::CWzdNatPassWordDlg()
{
	m_modifyName.clear();
	m_origianPass.clear();
	m_newPass.clear();
	m_confirmPass.clear();
	m_bChangePassword =false;
	m_toprow = 0;
}

CWzdNatPassWordDlg::~CWzdNatPassWordDlg()
{

}

void CWzdNatPassWordDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdNatPassWordDlg::OnLeavePage()
{

}

void CWzdNatPassWordDlg::ShowWzdData(bool bRefresh)
{


	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}

}

void CWzdNatPassWordDlg::GetWzdData(bool bNext)
{
	//读取页面修改的信息
	if(m_bChangePassword)
	{
		for(int i= 0;i<MAX_USER_NUM;++i)
		{
			if(0 == strcmp(m_pWzdInfo->userInfo[i].name,m_modifyName.c_str()))
			{
				strcpy(m_pWzdInfo->userInfo[i].password,m_newPass.c_str());
				break;//
			}
		}
	}
	
}
void CWzdNatPassWordDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	
	GUI::CEditCtrl*  pEdit = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CButton *pButton = NULL;
	

	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x-CScrollCtrl::m_length[SCROLL_WIDTH];
	unsigned long listWide = cx;
	unsigned long height =  m_rect.Height() - 2 * y - m_length[CHECK_HEIGHT];
	unsigned long listHeight = 4*m_length[LIST_ITEM_HEIGHT];
	unsigned long cy = height - listHeight -15;

	m_richTextCtrl.Create(GetRandWndID(),x,y,cx-m_length[SCROLL_WIDTH]-2,cy,this);
	if (800 >= m_width)//800x600以上分辨率不需要滚动条
	{
		m_richTextCtrl.SetScrollShow(true);
		m_scroll_v.Create(GetRandWndID(), x+cx-m_length[SCROLL_WIDTH]-2,y,cy-5,this,0,0,false);
	}
	y = y+cy+5;
	

	unsigned short offsetEdge = 0;
	int maxListRow = listHeight /m_length[LIST_ITEM_HEIGHT]; //List框带头
	if( ROW_COUNT > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	m_listCtrl.Create(GetRandWndID(),x,y,listWide,listHeight,this,0,1,false);
	m_listCtrl.AddColumn(NULL, listWide/2);
	m_listCtrl.AddColumn(NULL, listWide/2 - offsetEdge );
	
	m_listCtrl.AddItem(ROW_USER_NAME, 0, m_pStrTable->String(E_STR_ID_USER_NAME));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.AddItem(ROW_USER_NAME, 1, WND_TYPE_EDIT));
	assert(pEdit);
	pEdit->SetEnable(true,false);
	
	m_listCtrl.AddItem(ROW_ORIGIAN_PASSWORD, 0, m_pStrTable->String(E_STR_ID_PASSWORD));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.AddItem(ROW_ORIGIAN_PASSWORD, 1, WND_TYPE_EDIT));
	assert(pEdit);
	pEdit->SetEnable(true,false);
	pEdit->SetPassword(true);

	m_listCtrl.AddItem(ROW_NEW_PASSWORD, 0, m_pStrTable->String(E_STR_ID_NEW_PASSWORD));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.AddItem(ROW_NEW_PASSWORD, 1, WND_TYPE_EDIT));
	assert(pEdit);
	pEdit->SetEnable(true,false);
	pEdit->SetPassword(true);

	m_listCtrl.AddItem(ROW_CONFIRM_PASSWORD, 0, m_pStrTable->String(E_STR_ID_COMFIRM_PASSWORD));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.AddItem(ROW_CONFIRM_PASSWORD, 1, WND_TYPE_EDIT));
	assert(pEdit);
	pEdit->SetEnable(true,false);
	pEdit->SetPassword(true);

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_length[DLG_OFFSET_SUB_TOP]+height;
	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];
	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,2);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);

	SetTipInfo();
	m_listCtrl.CreateListFocusMap();
}

void CWzdNatPassWordDlg::OnDestroy()
{

}

void CWzdNatPassWordDlg::SetTipInfo()
{
	string tipInfo;
	// tipInfo.append("Before continuing, it is strongly recommended that you change your Admin default password from  \"123456\" to protect your system. " );
	tipInfo.append(m_pStrTable->String(E_STR_ID_NAT_WZD_PASSWORD_00));
	tipInfo.append("\\n");
	// tipInfo.append("If you do not wish to change your password at this time, please press Next.");
	tipInfo.append(m_pStrTable->String(E_STR_ID_NAT_WZD_PASSWORD_01));
	tipInfo.append("\\n");
	// tipInfo.append("IMPORTANT! Please keep your new password in a safe location. Lost passwords will require assistance ");
	tipInfo.append(m_pStrTable->String(E_STR_ID_NAT_WZD_PASSWORD_02));
	tipInfo.append("\\n");
	// tipInfo.append("from Q-See Tech Support. ");
	tipInfo.append(m_pStrTable->String(E_STR_ID_NAT_WZD_PASSWORD_03));
	m_richTextCtrl.SetText(tipInfo,false);
	unsigned short maxTextRow = m_richTextCtrl.GetTextRow()-1;//文本显示需要多少行
	if (800 >= m_width)//800x600以上分辨率不需要滚动条
	{
		m_scroll_v.SetRange(0,maxTextRow);
		m_scroll_v.SetCurPos(0,false);
	}
}

void CWzdNatPassWordDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();
	
}

void CWzdNatPassWordDlg::InitCtrlInfo()
{
	GUI::CEditCtrl*  pEdit = NULL;
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_USER_NAME,1));
	pEdit->SetMaxStringLen(m_pWzdInfo->userNameMaxLen);
	//读取并显示默认用户名
	for(int i= 0;i<MAX_USER_NUM;++i)
	{
		if(0 == strcmp(m_pWzdInfo->userInfo[i].name,"admin"))
		{
			//找到了需要修改的地方
			pEdit->SetCaption(m_pWzdInfo->userInfo[i].name,true);
			m_modifyName = m_pWzdInfo->userInfo[i].name;
			break;
		}

	}
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_ORIGIAN_PASSWORD,1));
	pEdit->SetMaxStringLen(m_pWzdInfo->passwordMaxLen);
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_NEW_PASSWORD,1));
	pEdit->SetMaxStringLen(m_pWzdInfo->passwordMaxLen);
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_CONFIRM_PASSWORD,1));
	pEdit->SetMaxStringLen(m_pWzdInfo->passwordMaxLen);
	pEdit = NULL;
}

BEGIN_MSG_MAP(CWzdNatPassWordDlg, CWzdNatPageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_scroll_v.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_ENTER, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_DRAG, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_BLUP, OnScrollchange)
END_MSG_MAP()

unsigned long  CWzdNatPassWordDlg::OnCheck()
{
	if(m_checkCtrl.IsCheck())
	{
		m_pWzdInfo->ShowWzd = 1;
	}
	else
	{
		m_pWzdInfo->ShowWzd = 0;
	}
	return KEY_VIRTUAL_STOP;
}

unsigned long CWzdNatPassWordDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	GUI::CEditCtrl*  pEdit = NULL;
	m_modifyName="admin";
	int currentUser;
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_USER_NAME,1));
	for(int i= 0;i<MAX_USER_NUM;++i)
	{
		if(0 == strcmp(m_pWzdInfo->userInfo[i].name,pEdit->GetCaption().c_str()))
		{
			//找到了需要修改的地方
			m_modifyName = m_pWzdInfo->userInfo[i].name;
			currentUser = i;
		}
	}
	switch(row)
	{
	case ROW_USER_NAME:
		{
			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_USER_NAME,1));
			bool bsuccFind =false;
			for(int i= 0;i<MAX_USER_NUM;++i)
			{
				if(0 == strcmp(m_pWzdInfo->userInfo[i].name,pEdit->GetCaption().c_str()))
				{
					//找到了需要修改的地方
					m_modifyName = m_pWzdInfo->userInfo[i].name;
					currentUser = i;
					bsuccFind = true;
					break;
				}
			}

			if(!bsuccFind)
			{
				//没找到，清除该行并弹出对话框提示
				pEdit->SetCaption("",false);
				MessageBox(m_pStrTable->String(E_STR_ID_NAT_WZD_PASSWORD_04), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);				
			}
			pEdit = NULL;
		}
		break;
	case ROW_ORIGIAN_PASSWORD:
		{
			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_ORIGIAN_PASSWORD,1));
			bool bsuccFind =false;
			if(0 == strcmp(m_pWzdInfo->userInfo[currentUser].password,pEdit->GetCaption().c_str() /*(0 == strcmp(m_pWzdInfo->userInfo[i].password,pEdit->GetCaption().c_str())) && (0 == strcmp(m_modifyName,pEdit->GetCaption().c_str()))*/))
			{
				//找到了需要修改的地方
				m_origianPass = pEdit->GetCaption();
				 bsuccFind = true;
				break;
			}
			if(!bsuccFind)
			{
				//没找到，清除该行并弹出对话框提示
				pEdit->SetCaption("",false);
				MessageBox(m_pStrTable->String(E_STR_ID_NAT_WZD_PASSWORD_05), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);				
			}
			pEdit = NULL;
		}
		break;
	case ROW_NEW_PASSWORD:
		{
			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_ORIGIAN_PASSWORD,1));
			bool bsuccFind =false;
			if(0 == strcmp(m_pWzdInfo->userInfo[currentUser].password,pEdit->GetCaption().c_str() /*(0 == strcmp(m_pWzdInfo->userInfo[i].password,pEdit->GetCaption().c_str())) && (0 == strcmp(m_modifyName,pEdit->GetCaption().c_str()))*/))
			{
				//找到了需要修改的地方
				m_origianPass = pEdit->GetCaption();
				 bsuccFind = true;
			}
			if(!bsuccFind)
			{
				//没找到，清除该行并弹出对话框提示
				pEdit->SetCaption("",false);
				pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_NEW_PASSWORD,1));
				pEdit->SetCaption("",false);
				MessageBox(m_pStrTable->String(E_STR_ID_NAT_WZD_PASSWORD_05), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
				break;
			}
			pEdit = NULL;

			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_NEW_PASSWORD,1)); 
		
			m_newPass = pEdit->GetCaption();
		}
		break;
	case ROW_CONFIRM_PASSWORD:
		{
			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_CONFIRM_PASSWORD,1));
			m_confirmPass = pEdit->GetCaption();
			if(m_confirmPass != m_newPass)
			{
				pEdit->SetCaption("",false);
				// pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_NEW_PASSWORD,1));
				// pEdit->SetCaption("",false);
				MessageBox(m_pStrTable->String(E_STR_ID_NAT_WZD_PASSWORD_06), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);				
			}
			else
			{
				m_bChangePassword = true;
			}
			pEdit = NULL;
		}
		break;
	default:
		assert(false);
		break;
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CWzdNatPassWordDlg::OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int pos = m_scroll_v.GetPos();
	if (pos != m_toprow)
	{
		m_toprow = pos;
		m_richTextCtrl.SetCurRow(m_toprow);
		Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP; 
}

bool CWzdNatPassWordDlg::CheckNewPasswd()
{
	if(m_confirmPass != m_newPass)
	{
		//pEdit->SetCaption("",false);
		// pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(ROW_NEW_PASSWORD,1));
		// pEdit->SetCaption("",false);
		GUI::CDialog::MessageBox(m_pStrTable->String(E_STR_ID_NAT_WZD_PASSWORD_06),\
					m_pStrTable->String(E_STR_ID_TITLE_WARNING),\
					DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return false;
	}
	return true;
}
//end