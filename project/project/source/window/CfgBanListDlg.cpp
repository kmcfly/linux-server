#include "CfgBanListDlg.h"

CCfgBanListDlg::CCfgBanListDlg()
{
	
}

CCfgBanListDlg::~CCfgBanListDlg()
{
	
}

void CCfgBanListDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn(DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DEFAULT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	char tiptitle[128]={0};
	strcpy(tiptitle,m_pStrTable->String(E_STR_ID_TITLE_BLACK_WHITELIST));
	SetCaption(tiptitle, false);
	CreateTipWnd();

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned long width = m_rect.Width() - m_length[DLG_OFFSET_SUB_LEFT] * 2;
	unsigned long height = m_length[LIST_ITEM_HEIGHT]*2;
	
	unsigned short black_cx = 0, black_cy = 0, white_cx = 0, white_cy = 0;
	GUI::CWnd::TextSize(m_pStrTable->String(E_STR_ID_BLACKLIST), strlen(m_pStrTable->String(E_STR_ID_BLACKLIST)), black_cx, black_cy);
	GUI::CWnd::TextSize(m_pStrTable->String(E_STR_ID_WHITELIST), strlen(m_pStrTable->String(E_STR_ID_WHITELIST)), white_cx, white_cy);

	unsigned short ckWidth = (black_cx > white_cx) ? (black_cx + 32) : (white_cx + 32);
	unsigned short ckOffset = (width - ckWidth * 2) / 10;

	m_rdBlack.Create(GetRandWndID(),x + width / 2 - ckWidth - ckOffset,y,ckWidth,m_length[RADIO_HEIGHT],this,0,1,&m_rdGroup);
	m_rdWhite.Create(GetRandWndID(),x + width / 2 + ckOffset,y,ckWidth,m_length[RADIO_HEIGHT],this,0,2,&m_rdGroup);
	m_rdGroup.AddRadio(&m_rdBlack);
	m_rdGroup.AddRadio(&m_rdWhite);
	m_rdBlack.SetCaption(m_pStrTable->String(E_STR_ID_BLACKLIST),false,false);
	m_rdBlack.SetData(PAGE_BLACK);
	m_rdWhite.SetCaption(m_pStrTable->String(E_STR_ID_WHITELIST),false,false);
	m_rdWhite.SetData(PAGE_WHITE);

	y+=m_length[RADIO_HEIGHT]+5;
	width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	height = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];
	GUI::CListCtrl& list = m_ipList;
	GUI::CWnd * pWnd = NULL;
	GUI::CIPAddressCtrl *pIpStart=NULL;
	GUI::CIPAddressCtrl *pIpEnd=NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头
	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 3, true);
	list.AddColumn(" ", m_length[CHECK_WIDTH]+10);
	list.AddColumn((m_pStrTable->String(E_STR_ID_ID)), width * 3 / 32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_IP_FROM), width/4+70);
	list.AddColumn(m_pStrTable->String(E_STR_ID_IP_TO), width/4+30);

	//插入MAX_BAN_ALLOW_LIST_NUM行固定的黑白名单
	char str[128] = {0};
	for(int i=0;i<MAX_BAN_ALLOW_LIST_NUM;i++)
	{
		sprintf(str,"%d",i+1);
		list.AddItem(i,ROW_CHNN,str);
		pCheck=dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( i, ROW_STARTUP, WND_TYPE_CHECK));
		pIpStart = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( i,ROW_STARTIP, WND_TYPE_IP,m_length[DATE_CTRL_WIDTH],m_length[IP_CTRL_HEIGHT]));
		pIpEnd = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( i, ROW_ENDIP, WND_TYPE_IP,m_length[DATE_CTRL_WIDTH],m_length[IP_CTRL_HEIGHT]));
	}
	m_curPage=PAGE_NULL;
	list.CreateListFocusMap();
}

void CCfgBanListDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_BAN_ALLOW_TYPE);
	itemList.push_back(NCFG_ITEM_BAN_ALLOW_LIST);
}

bool CCfgBanListDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_ipList;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CIPAddressCtrl* pIpStart = NULL;
	GUI::CIPAddressCtrl* pIpEnd=NULL;
	char str[128] = {0};

	switch (cfgID)
	{
		case NCFG_ITEM_BAN_ALLOW_TYPE:
			{
				assert(len == sizeof(unsigned long));
				const unsigned long* pValue =reinterpret_cast<const unsigned long*>(pData);
				if(*pValue==PAGE_WHITE)
				{
					SetPage(PAGE_WHITE,bRefresh);//页面不切换，仅仅只是把点切换了
				}
				else if(*pValue==PAGE_BLACK)
				{
					SetPage(PAGE_BLACK,bRefresh);
				}
				return true;
				break;
			}
		case NCFG_ITEM_BAN_ALLOW_LIST://有效 开始和结束ip
		{
			assert(len == sizeof(IP_SECTION)*MAX_BAN_ALLOW_LIST_NUM);
			const IP_SECTION* pInfo = reinterpret_cast<const IP_SECTION*>(pData);
			for(int i=0;i<MAX_BAN_ALLOW_LIST_NUM;i++)
			{
				//直接加载到页面上
				pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(i,ROW_STARTUP));
				assert(pCheck);
				if(pInfo[i].enable==0)
				{
					pCheck->SetCheck(false,bRefresh);
				}
				else
				{
					pCheck->SetCheck(true,bRefresh);
				}
				//ip段的加载
				pIpStart = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(i,ROW_STARTIP));
				pIpEnd = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(i,ROW_ENDIP));
				pIpStart->SetIP(pInfo[i].start,false);
				pIpEnd->SetIP(pInfo[i].end,false);
			}
			return true;
			break;
		}	
	}

	return false;
}

void CCfgBanListDlg::SetPage(PAGE_ITEM page,bool bRefresh)
{
	if(page!=m_curPage)
	{
		m_curPage=page;
	}

	if(m_curPage==PAGE_BLACK)
	{
		m_rdBlack.SetCheck(true,false);
		
	}
	else if(m_curPage==PAGE_WHITE)
	{
		
		m_rdWhite.SetCheck(true,false);
	}
	
}

bool CCfgBanListDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CCheckCtrl* pCheck=NULL;
	GUI::CIPAddressCtrl* pIpStart = NULL;
	GUI::CIPAddressCtrl* pIpEnd=NULL;
	const GUI::CListCtrl& list =m_ipList;

	switch (cfgID)
	{
	case NCFG_ITEM_BAN_ALLOW_TYPE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pInfo =reinterpret_cast<unsigned long*>(pData);
			//获取当前页面值
			if(m_curPage==PAGE_BLACK)
			{
				*pInfo=0;
			}
			else if(m_curPage==PAGE_WHITE)
			{
				*pInfo=1;
			}
			return true;
			break;
		}
	case NCFG_ITEM_BAN_ALLOW_LIST:
		{
			assert(len == sizeof(IP_SECTION)*MAX_BAN_ALLOW_LIST_NUM);
			IP_SECTION* pInfo = reinterpret_cast<IP_SECTION*>(pData);
			for(int i=0;i < MAX_BAN_ALLOW_LIST_NUM; i++)
			{
				pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)list.GetItemWnd(i,ROW_STARTUP));
				if(pCheck->IsCheck())
				{
					pInfo[i].enable=1;
				}
				else
				{
					pInfo[i].enable=0;
				}
				pIpStart = dynamic_cast<GUI::CIPAddressCtrl*>((GUI::CIPAddressCtrl*)list.GetItemWnd(i,ROW_STARTIP));
				pIpEnd = dynamic_cast<GUI::CIPAddressCtrl*>((GUI::CIPAddressCtrl*)list.GetItemWnd(i,ROW_ENDIP));
				pInfo[i].start=pIpStart->GetIP();
				pInfo[i].end=pIpEnd->GetIP();
			}
			return true;
			break;
		}

	}

	return false;
}

BEGIN_MSG_MAP(CCfgBanListDlg, CCfgDlg)
	ON_MSG_EX(m_ipList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG(m_rdBlack.GetID(),KEY_VIRTUAL_ENTER,OnClickRadio)
	ON_MSG(m_rdWhite.GetID(),KEY_VIRTUAL_ENTER,OnClickRadio)
END_MSG_MAP()

unsigned long CCfgBanListDlg::OnClickRadio()
{
	GUI::CRadioCtrl* pRadio = m_rdGroup.GetCheckRadio();
	assert(pRadio);
	PAGE_ITEM click_page= static_cast<PAGE_ITEM>(pRadio->GetData());
	if(click_page!=m_curPage)
	{
		/*GUI::CWnd* pBtnOk = GetCtrl(CTRLID_DEFAULT_BTN_OK);
		pBtnOk->SetEnable(true,false);*/
		EnableOkBtn(true,true);
		SetModify(true);
	}

	switch (click_page)
	{
		case PAGE_WHITE:
		{
			SetPage(PAGE_WHITE,false);
			break;
		}
		case PAGE_BLACK:
		{
			SetPage(PAGE_BLACK,false);
			break;
		}
		default:
			assert(false);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CCfgBanListDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	unsigned long ret = KEY_VIRTUAL_MSG_STOP;

	if (!IsModify())
	{
		switch (col)
		{
			case ROW_STARTUP:
			case ROW_STARTIP:
			case ROW_ENDIP:
			{
				if(IsChange(NCFG_ITEM_BAN_ALLOW_LIST))
				{
					SetModify(true);
					/*GUI::CWnd* pBtnOk = GetCtrl(CTRLID_DEFAULT_BTN_OK);
					pBtnOk->SetEnable(true,false);*/
					EnableOkBtn(true,true);
				}
				ret = KEY_VIRTUAL_MSG_CONTINUE;
				break;
			}
			default:
				break;
		}
	}

	return  ret;
}

unsigned long CCfgBanListDlg::OnOK()
{
	//GUI::CWnd* pBtnOk = GetCtrl(CTRLID_DEFAULT_BTN_OK);
	//pBtnOk->SetEnable(false,false);
	EnableOkBtn(false,true);
	CCfgDlg::OnOK();
	return  KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CCfgBanListDlg::OnCancel()
{
	CCfgDlg::OnCancel();
	return  KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgBanListDlg::OnApply()
{
	CCfgDlg* pCfgDlg =this;
	assert(pCfgDlg);
	unsigned long ret = OnLoadDefault(pCfgDlg);

	if( pCfgDlg->IsModify() )
	{
		EnableOkBtn(true,true);
	}
	else
	{
		EnableOkBtn(false,true);
	}

	ENUM_STRING_ID strID = ret ? static_cast<ENUM_STRING_ID>(ret) : E_STR_ID_TIP_00_03;
	MessageOut(strID,TIP_MSG,false);

	//一次重绘全部
	Repaint();
	return  KEY_VIRTUAL_MSG_STOP;
	
}
