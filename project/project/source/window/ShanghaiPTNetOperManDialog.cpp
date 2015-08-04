#include "ShanghaiPTNetOperManDialog.h"
#include "ShanghaiConnectMan.h"
extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

CShanghaiPTNetOperManDialog::CShanghaiPTNetOperManDialog()
{
	m_codeTable = CCharCodeTable::Instance();
	m_choseType = 0;
}

CShanghaiPTNetOperManDialog::~CShanghaiPTNetOperManDialog()
{

}

void CShanghaiPTNetOperManDialog::SetChoseType(unsigned long choseType)
{
	m_choseType = choseType;
}

void CShanghaiPTNetOperManDialog::OnInitial()
{
	GUI::CDialog::OnInitial();
	std::string str;
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_EXIT) );
	CreateTipWnd();
	str.clear();
	str = ChangeChinese("用户类型");
	SetCaption(str,false);
	m_initPageMask= 0x1;
	
	str.clear();
	str = ChangeChinese("上传");
	SetDefaultBtnCaption(DEFAULT_BTN_APPLY,str.c_str());

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
	m_ShanghaiManualSnapDialog.SetInitialInfo(g_p_login_succ->productInfo.videoInputNum,m_choseType);
	m_ShanghaiManualSnapDialog.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0,2);
	
	switch(m_choseType)
	{
	case type_OPER:
		{
			//操作人员
			str.clear();
			str = ChangeChinese("测试抓图");
			m_tabCtrl.AddItem(&m_ShanghaiManualSnapDialog, str.c_str());
			str.clear();
			str = ChangeChinese("故障报修");
			m_tabCtrl.AddItem(&m_ShanghaiOtherErrDialog, str.c_str());
		}
		break;
	case type_KEEP:
		{
			str.clear();
			str = ChangeChinese("维保抓图");
			m_tabCtrl.AddItem(&m_ShanghaiManualSnapDialog, str.c_str());
			str.clear();
			str = ChangeChinese("签到");
			m_tabCtrl.AddItem(&m_ShanghaiAssignDialog, str.c_str());
		}
		break;
	case type_CHECK:
		{
			str.clear();
			str = ChangeChinese("验收抓图");
			m_tabCtrl.AddItem(&m_ShanghaiManualSnapDialog, str.c_str());
		}
		break;
	default:
		break;
	}

	m_curPage = PAGE_MANUALSNAP;
	m_tabCtrl.SetCurItem(m_curPage , false );

}

unsigned long  CShanghaiPTNetOperManDialog::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{

	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{
		{
			std::string str;
			unsigned short x = m_length[DLG_OFFSET_LEFT];
			unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
			unsigned short cx = m_rect.Width() - ( x * 2 );
			unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

			if(!((m_initPageMask >> PAGE_ASSIGNREPAIR) & 0x1) && (PAGE_ASSIGNREPAIR == m_tabCtrl.GetFocusItem()))
			{
				switch(m_choseType)
				{
				case type_OPER:
					{
						m_ShanghaiOtherErrDialog.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, PAGE_ASSIGNREPAIR + 2);
					}
					break;
				case type_KEEP:
					{
						m_ShanghaiAssignDialog.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, PAGE_ASSIGNREPAIR + 2);
					}
					break;
				default:
					break;
				}
				
				m_initPageMask |= (0x1 << PAGE_ASSIGNREPAIR);
			}
			
		}

		if (m_curPage != m_tabCtrl.GetFocusItem())
		{
			m_curPage = static_cast<PAGE_ITEM>(m_tabCtrl.GetFocusItem());
		}	
	}
	return keyID;
}

unsigned long CShanghaiPTNetOperManDialog::OnOK()
{
	GUI::CDialog::OnOK();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CShanghaiPTNetOperManDialog::OnApply()
{
	bool bsucc = false;
	bool bPTEnable = CShanghaiConnectMan::Instance()->GetPTEnable();
	if(!bPTEnable)
	{
		std::string strErr;
		strErr.clear();
		strErr = ChangeChinese("平台未启用");
		MessageOut( GetTopestWnd(),strErr.c_str(), TIP_ERR, false );
		Repaint();
		return KEY_VIRTUAL_MSG_STOP;
	}

	switch(m_choseType)
	{
	case type_OPER:
		{
			if(PAGE_ASSIGNREPAIR == m_curPage)
			{
				//故障报修页面
				m_ShanghaiOtherErrDialog.GetAssignInfo();
			}
			else
			{
				//抓图页面
				bsucc = m_ShanghaiManualSnapDialog.OnClickBtnUpload();
				if(!bsucc)
				{
					printf("%s,%d\n",__FILE__,__LINE__);
					MessageOut( GetTopestWnd(), m_pStrTable->String(E_STR_ID_MESSAGE_34), TIP_ERR, false );
					Repaint();
				}
			}
		}
		break;
	case type_KEEP:
		{
			if(PAGE_ASSIGNREPAIR == m_curPage)
			{
				m_ShanghaiAssignDialog.GetAssignInfo();
			}
			else
			{
				//抓图页面
				bsucc = m_ShanghaiManualSnapDialog.OnClickBtnUpload();
				if(!bsucc)
				{
					printf("%s,%d\n",__FILE__,__LINE__);
					MessageOut( GetTopestWnd(), m_pStrTable->String(E_STR_ID_MESSAGE_34), TIP_ERR, false );
					Repaint();
				}
			}
		}
		break;
	case type_CHECK:
		{
			bsucc = m_ShanghaiManualSnapDialog.OnClickBtnUpload();
			if(!bsucc)
			{
				printf("%s,%d\n",__FILE__,__LINE__);
				MessageOut( GetTopestWnd(), m_pStrTable->String(E_STR_ID_MESSAGE_34), TIP_ERR, false );
				Repaint();
			}
		}
		break;
	default:
		break;
	}
	return KEY_VIRTUAL_MSG_STOP;
}

string CShanghaiPTNetOperManDialog::ChangeChinese(const char *pstr)
{
	string outStr;
	if(NULL != m_codeTable)
	{
		if (-1 == m_codeTable->Convert(outStr, CCT_CODE_UTF8, pstr, CCT_CODE_GBK))
		{
			outStr = pstr;
		}
	}
	else
	{
		outStr = pstr;
	}
	return outStr;

}

