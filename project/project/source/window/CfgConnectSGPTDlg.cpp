#include "CfgConnectSGPTDlg.h"
#include "ShanghaiPTNetOperManDialog.h"
CCfgConnectSGPTDlg::CCfgConnectSGPTDlg()
{
	m_codeTable = CCharCodeTable::Instance();
}

CCfgConnectSGPTDlg::~CCfgConnectSGPTDlg()
{

}

void CCfgConnectSGPTDlg::OnInitial()
{
	CCfgDlg::OnInitial();

	GUI::CEditCtrl*	 pEdit = NULL;
	GUI::CCheckCtrl *pCheck = NULL;
	GUI::CNumEditCtrl *pNumEdit = NULL;
	GUI::CComboCtrl *pComboCtrl = NULL;
	GUI::CButton *pBtn = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = height/m_length[LIST_ITEM_HEIGHT]; //List框不带头
	if( ROW_NUM >= maxListRow )
	{
		offsetEdge = m_length[SLIDER_WIDTH] + 2;
	}

	m_cfgSGPTList.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	m_cfgSGPTList.AddColumn(NULL, width * 16 / 32);
	m_cfgSGPTList.AddColumn(NULL, width * 16 / 32 - offsetEdge);

	char str[128] = {0};

	int row = 0;
	std::string outStr;
	outStr.clear();
	outStr = ChangeChinese("启用平台");
	m_cfgSGPTList.AddItem( ROW_SGPT, 0, outStr.c_str() );//SG平台启用
	pCheck =  dynamic_cast<GUI::CCheckCtrl*>( m_cfgSGPTList.AddItem( ROW_SGPT, 1, WND_TYPE_CHECK) );

	row += 1;
	outStr.clear();
	outStr = ChangeChinese("代理ID");
	m_cfgSGPTList.AddItem( ROW_AGENTID, 0,outStr.c_str() );//接入ＩＤ
	pEdit =  dynamic_cast<GUI::CEditCtrl*>( m_cfgSGPTList.AddItem( ROW_AGENTID, 1, WND_TYPE_EDIT) );
	pEdit->SetMaxStringLen(12);
	assert( pEdit );
	
	row += 1;
	outStr.clear();
	outStr = ChangeChinese("服务器IP");
	m_cfgSGPTList.AddItem( ROW_IPEDIT, 0,outStr.c_str() );//接入域名或者IP
	pEdit =  dynamic_cast<GUI::CEditCtrl*>( m_cfgSGPTList.AddItem( ROW_IPEDIT, 1, WND_TYPE_EDIT) );
	assert( pEdit );

	row += 1;
	outStr.clear();
	outStr = ChangeChinese("服务器端口");
	m_cfgSGPTList.AddItem( ROW_PORT, 0,outStr.c_str() );//接入端口
	pNumEdit =  dynamic_cast<GUI::CNumEditCtrl*>( m_cfgSGPTList.AddItem( ROW_PORT, 1, WND_TYPE_NUM_EDIT) );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,65535);
	assert( pNumEdit );

	row += 1;
	outStr.clear();
	outStr = ChangeChinese("用户类型");
	m_cfgSGPTList.AddItem( ROW_TYPE, 0,outStr.c_str() );
	pComboCtrl = dynamic_cast<GUI::CComboCtrl*>( m_cfgSGPTList.AddItem( ROW_TYPE, 1, WND_TYPE_COMBO,1.5*m_length[BTN_WIDTH],m_length[BTN_HEIGHT]) );
	outStr.clear();
	outStr = ChangeChinese("操作人员");
	pComboCtrl->AddItem(outStr.c_str(),ROW_OPER);
	outStr.clear();
	outStr = ChangeChinese("维保人员");
	pComboCtrl->AddItem(outStr.c_str(),ROW_KEEP);
	outStr.clear();
	outStr = ChangeChinese("验收人员");
	pComboCtrl->AddItem(outStr.c_str(),ROW_CHECK);
	pComboCtrl->SetCurItemData(ROW_OPER,false);

	m_cfgSGPTList.AddItem( ROW_BTN, 0,"");
	pBtn = dynamic_cast<GUI::CButton*>( m_cfgSGPTList.AddItem( ROW_BTN, 1, WND_TYPE_BUTTON,1.5*m_length[BTN_WIDTH],m_length[BTN_HEIGHT]) );
	outStr.clear();
	outStr = ChangeChinese("平台操作");
	pBtn->SetCaption(outStr.c_str(),false);
	m_cfgSGPTList.CreateListFocusMap();
}

bool CCfgConnectSGPTDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	if( (NULL == pData) || (0 >= len) )
	{
		assert(false);
		return false;
	}
	GUI::CCheckCtrl *pCheck = NULL;
	GUI::CEditCtrl *pEdit = NULL;
	GUI::CNumEditCtrl *pNumEdit = NULL;
	switch (cfgID)
	{
	case NCFG_ITEM_NET_SHANGHAI_PTENABLE:
		{
			const unsigned long *pEnable = reinterpret_cast<const unsigned long *>(pData);
			pCheck = dynamic_cast<GUI::CCheckCtrl *>(m_cfgSGPTList.GetItemWnd(ROW_SGPT,1));
			if(0 == *pEnable)
			{
				pCheck->SetCheck(false,false);
			}
			else if(1 == *pEnable)
			{
				pCheck->SetCheck(true,false);
			}			
		}
		return true;
		break;
	case NCFG_ITEM_NET_SHANGHAI_AGENTID:
		{
			const char *pAgent = reinterpret_cast<const char*>(pData);
			pEdit = dynamic_cast<GUI::CEditCtrl *>(m_cfgSGPTList.GetItemWnd(ROW_AGENTID,1));
			pEdit->SetCaption(pAgent,false);
			printf("ok\n");
		}
		return true;
		break;
	case NCFG_ITEM_NET_SHANGHAI_IP:
		{
			const char *pIP = reinterpret_cast<const char*>(pData);
			pEdit = dynamic_cast<GUI::CEditCtrl *>(m_cfgSGPTList.GetItemWnd(ROW_IPEDIT,1));
			pEdit->SetCaption(pIP,false);
		}
		return true;
		break; 
	case NCFG_ITEM_NET_SHANGHAI_PORT:
		{
			const unsigned long *pPort = reinterpret_cast<const unsigned long*>(pData);
			pNumEdit = dynamic_cast<GUI::CNumEditCtrl *>(m_cfgSGPTList.GetItemWnd(ROW_PORT,1));
			pNumEdit->SetValue(*pPort,false);
		}
		return true;
		break; 
	default:
		break;
	}

	return false;
}

bool CCfgConnectSGPTDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);

	const GUI::CCheckCtrl *pCheck = NULL;
	const GUI::CEditCtrl *pEdit = NULL;
	const GUI::CNumEditCtrl *pNumEdit= NULL;

	switch (cfgID)
	{
	case NCFG_ITEM_NET_SHANGHAI_PTENABLE:
		{
			assert( len == sizeof(unsigned long));
			unsigned long *pEnable =  reinterpret_cast<unsigned long*>(pData);
			pCheck=  dynamic_cast<const  GUI::CCheckCtrl *>(m_cfgSGPTList.GetItemWnd(ROW_SGPT,1));
			assert( NULL != pCheck );
			if(pCheck->IsCheck())
			{
				*pEnable = 1;
			}
			else
			{
				*pEnable = 0;
			}
		}
		return true;
		break;
	case NCFG_ITEM_NET_SHANGHAI_AGENTID:
		{
			assert( len == sizeof(char [13]));
			char *pAgent = reinterpret_cast<char*>(pData);
			pEdit = dynamic_cast<const GUI::CEditCtrl *>(m_cfgSGPTList.GetItemWnd(ROW_AGENTID,1));
			snprintf(pAgent,13,"%s",pEdit->GetCaption().c_str());
			if(0 == strcmp(pAgent," "))//空的不保存，提示错误
			{
				return false;
			}
		}
		return true;
		break;
	case NCFG_ITEM_NET_SHANGHAI_IP:
		{
			assert( len == sizeof(char [20]));
			char *pIp = reinterpret_cast<char*>(pData);
			pEdit = dynamic_cast<const GUI::CEditCtrl *>(m_cfgSGPTList.GetItemWnd(ROW_IPEDIT,1));
			snprintf(pIp,20,"%s",pEdit->GetCaption().c_str());
			if(0 == strcmp(pIp," "))//空的不保存，提示错误
			{
				return false;
			}
		}
		return true;
		break;
	case NCFG_ITEM_NET_SHANGHAI_PORT:
		{
			assert( len == sizeof(unsigned long));
			unsigned long *pPort =  reinterpret_cast<unsigned long*>(pData);
			pNumEdit =  dynamic_cast<const GUI::CNumEditCtrl *>(m_cfgSGPTList.GetItemWnd(ROW_PORT,1));
			*pPort = pNumEdit->GetValue();
			if(0 == *pPort)
			{
				return false;
			}
		}
		return true;
		break;
	default:
		break;
	}

	return false;
}


BEGIN_MSG_MAP(CCfgConnectSGPTDlg, CCfgDlg)
	ON_MSG_EX(m_cfgSGPTList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgConnectSGPTDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	unsigned long ret = KEY_VIRTUAL_MSG_STOP;

	if( 1 != col )
	{
		printf("%s:%s:%d, row=0x%04x, col=0x%04x\n", __FUNCTION__, __FILE__, __LINE__, row, col);
		return ret;
	}

	if (!IsModify())
	{	
		switch(row)
		{
		case ROW_SGPT:
			{
				if (IsChange(NCFG_ITEM_NET_SHANGHAI_PTENABLE))
				{
					SetModify(true);
				}
				ret = KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case ROW_AGENTID:
			{
				if (IsChange(NCFG_ITEM_NET_SHANGHAI_AGENTID))
				{
					SetModify(true);
				}
				ret = KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case ROW_IPEDIT:
			{
				if(IsChange(NCFG_ITEM_NET_SHANGHAI_IP))
				{
					SetModify(true);
				}
				ret = KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case ROW_PORT:
			{
				if (IsChange(NCFG_ITEM_NET_SHANGHAI_PORT))
				{
					SetModify(true);
				}
				ret = KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case ROW_BTN:
			{
				PtOperServer();
			}
			break;
		default:
			break;
		}
	}

	return ret;
}

string CCfgConnectSGPTDlg::ChangeChinese(const char *pstr)
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

void CCfgConnectSGPTDlg::PtOperServer()
{
	GUI::CComboCtrl *pComboCtrl = dynamic_cast<GUI::CComboCtrl *>(m_cfgSGPTList.GetItemWnd(ROW_TYPE,1));
	unsigned long curType = pComboCtrl->GetCurItemData();
	CShanghaiPTNetOperManDialog ptOperManDialog; 
	ptOperManDialog.SetChoseType(curType);
	ptOperManDialog.DoModal();
}
//end
