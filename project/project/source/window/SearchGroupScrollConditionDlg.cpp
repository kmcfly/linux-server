#if defined(__GROUY_SEARCH__)
#include "SearchGroupScrollConditionDlg.h"
#include "NetProtocol.h"
#include "MessageMan.h"
#include "MessageBox.h"
#include "UserMan.h"
#include "StringDefine.h"

CSearchGroupScrollConditionDlg::CSearchGroupScrollConditionDlg() : m_channelNum(0), m_authCH(0)
{
	m_enable = 0;
	m_curItem = 0;

	m_pCKCH = NULL;
	m_pCKGroup = NULL;
	m_pCheckGroup = NULL;
	m_pComboCtrl = NULL;
	m_pBtnAdd = NULL;
	m_pBtnDel = NULL;
	m_pBtnEdit = NULL;
	
	m_bModify=false;
	m_cfgGroupList.clear();
}

CSearchGroupScrollConditionDlg::~CSearchGroupScrollConditionDlg()
{

}

void CSearchGroupScrollConditionDlg::SetInitialInfo(unsigned long channelNum, ULONGLONG authCH)
{
	m_channelNum = channelNum;
	//m_authCH	= authCH;
	m_authCH = ALL_CHNN_MASK(m_channelNum);//搜索不需要回放权限
}

unsigned long CSearchGroupScrollConditionDlg::Time()
{
	GUI::CCalendarCtrl* pCalendar = dynamic_cast<GUI::CCalendarCtrl*>(GetScrollWnd().GetWnd(0));
	if (pCalendar == NULL)
	{
		return 0;
	}
	else
	{
		return pCalendar->Time();
	}
}

ULONGLONG CSearchGroupScrollConditionDlg::CH()
{
	ULONGLONG chnnBits = 0;
	unsigned char chnnNum = m_channelNum;
	GUI::CCheckCtrl* pCKCH;
	for (int i=0; i<chnnNum; ++i)
	{
		pCKCH = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CH + i));
		chnnBits |= ((!pCKCH->IsCheck()) ? 0 : (0x01 << i));
	}

	return chnnBits;
}


//////////////////////////////////////////////////////////////////////////
void CSearchGroupScrollConditionDlg::OnInitial()
{
	assert(m_channelNum > 0);

	GUI::CScrollDialog::OnInitial();
	m_cfgGroupList.clear();

	unsigned short x = 5, y = 5, offset = 50, border = 2;

	GUI::CScrollWnd& wnd = GetScrollWnd();
	GUI::CRect rect(x, y, x + m_length[CALENDAR_WIDTH], y + m_length[CALENDAR_HEIGHT]);

	wnd.SetSizeOffset(m_length[CALENDAR_WIDTH] + x, m_length[CALENDAR_HEIGHT] + y);
	wnd.SetViewMaxSize(m_length[CALENDAR_WIDTH] + x, m_length[CALENDAR_HEIGHT]* 3 + 3 * y );

	GUI::CCalendarCtrl* pCalendar = dynamic_cast<GUI::CCalendarCtrl*>(wnd.AddWnd(WND_TYPE_CALENDAR, CTRLID_SEARCH_CALENDAR, rect, 0, 0, ID_CALENDAR));
	assert(pCalendar!=NULL);
	//////////////////////////////////////////////////////////////////////////
	unsigned char groupNum = (m_channelNum + 3)/4;
	m_pCKCH = new GUI::CCheckCtrl *[m_channelNum];
	m_pCKGroup = new GUI::CCheckCtrl *[groupNum];
	x = 10; y += (m_length[CALENDAR_HEIGHT] + 8);
	for (int i=0; i<groupNum; ++i)
	{
		rect.SetRect(x, y + i * m_length[CHECK_HEIGHT], x + m_length[CHECK_WIDTH], y + m_length[CHECK_HEIGHT] * (i + 1));
		m_pCKGroup[i] = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, CTRLID_SEARCH_GROUP_0 + i, rect, 0, i, ID_GROUP + i));
	}

	int localChnnNum = CProduct::Instance()->LocalVideoInputNum();
	char ch [] = "32";
	GUI::CCheckCtrl* pCKCH = NULL;
	x = 40; y = (m_length[CALENDAR_HEIGHT] + 13), offset = 54;
	rect.SetRect(x, y, x + offset, y + m_length[CHECK_HEIGHT]);
	for (int i=0; i<m_channelNum; ++i)
	{
		if(i < localChnnNum)
		{
			sprintf(ch, "%d", i+1);
		}
		else
		{
			sprintf(ch, "D%d", i+1-localChnnNum);
		}
		rect.SetRect(x + (i % 4) * offset, y + (i / 4) * m_length[CHECK_HEIGHT], x + (1 + i % 4) * offset, y + (1 + i / 4) * m_length[CHECK_HEIGHT]);
		m_pCKCH[i] = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, CTRLID_SEARCH_CH_0 + i, rect, (i%4) + 1, i/4, ID_CH + i));
		m_pCKCH[i]->SetCaption(ch, false, false);
		m_pCKCH[i]->SetEnable(false,false);
	}


	//////////////////////////////////////////////////////////////////////////
	x = m_length[DLG_OFFSET_X];
	y = m_length[CALENDAR_HEIGHT] + groupNum * m_length[CHECK_HEIGHT] + 25;
	int totalcount = m_channelNum/4+1;
	m_pCheckGroup = NULL;
	rect.SetRect(x, y, x+m_length[CHECK_WIDTH], y+m_length[CHECK_HEIGHT]);
	m_pCheckGroup = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, GUI::CWnd::GetRandWndID(), rect, totalcount, totalcount, ID_GROUP_CHECK));
	m_pCheckGroup->SetCheck(true,false);
	
	x += m_length[CHECK_WIDTH] + 3;
	GUI::CStatic *pStatic =NULL;
	rect.SetRect(x,y, x+m_length[TITLE_EXIT_BTN_RIGHT_XPOS], y+m_length[STATIC_HEIGHT]);
	pStatic = dynamic_cast<GUI::CStatic*>(wnd.AddWnd(WND_TYPE_STATIC, GUI::CWnd::GetRandWndID(), rect, totalcount, totalcount+1, ID_GROUP_STATIC));
	pStatic->SetCaption(m_pStrTable->String(E_STR_ID_SEARCH_GROUP),false);
	//pStatic = NULL;

	x += m_length[TITLE_EXIT_BTN_RIGHT_XPOS] + 10;
	m_pComboCtrl = NULL;
	rect.SetRect(x,y,x+m_length[BTN_WIDTH] + 2 * m_length[COMBO_WIDTH],y+m_length[COMBO_HEIGHT]);
	m_pComboCtrl = dynamic_cast<GUI::CComboCtrl*>(wnd.AddWnd(WND_TYPE_COMBO, GUI::CWnd::GetRandWndID(), rect, totalcount, totalcount+2, ID_GROUP_COMBO));
	m_pComboCtrl->SetCaption("",false);
	m_pComboCtrl->SetEnable(false,false);

	x = m_length[DLG_OFFSET_X];
	y += m_length[RADIO_HEIGHT] + 15;
	
	m_pBtnAdd = NULL;
	rect.SetRect(x,y,x+m_length[BTN_WIDTH] - 10,y+ m_length[BTN_HEIGHT]);
	m_pBtnAdd = dynamic_cast<GUI::CButton*>(wnd.AddWnd(WND_TYPE_BUTTON, GUI::CWnd::GetRandWndID(), rect, totalcount, totalcount+3, ID_GROUP_BTNADD));
	m_pBtnAdd->SetCaption(m_pStrTable->String(E_STR_ID_ADD),false);
	m_pBtnAdd->SetEnable(false,false);

	x += m_length[BTN_WIDTH] - 5;
	rect.SetRect(x,y,x+m_length[BTN_WIDTH] - 10, y+m_length[BTN_HEIGHT]);
	m_pBtnDel = NULL;
	m_pBtnDel = dynamic_cast<GUI::CButton*>(wnd.AddWnd(WND_TYPE_BUTTON,  GUI::CWnd::GetRandWndID(), rect, totalcount, totalcount+4, ID_GROUP_BTNDEL));
	m_pBtnDel->SetCaption(m_pStrTable->String(E_STR_ID_DELETE),false);
	m_pBtnDel->SetEnable(false,false);

	x += m_length[BTN_WIDTH] - 5;
	rect.SetRect(x,y,x+m_length[BTN_WIDTH] - 10, y+m_length[BTN_HEIGHT]);
	m_pBtnEdit = NULL;
	m_pBtnEdit = dynamic_cast<GUI::CButton*>(wnd.AddWnd(WND_TYPE_BUTTON, GUI::CWnd::GetRandWndID(), rect, totalcount, totalcount+5, ID_GROUP_BTNEDIT));
	m_pBtnEdit->SetCaption(m_pStrTable->String(E_STR_ID_EDIT),false);
	m_pBtnEdit->SetEnable(false,false);
	
}

void CSearchGroupScrollConditionDlg::OnPostInitial()
{
	GUI::CScrollDialog::OnPostInitial();
	LoadGroupItemCfg();
}

int CSearchGroupScrollConditionDlg::LoadGroupItemCfg()
{
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_SEARCH_GROUP_ENABLE);
	itemList.AddTail(NCFG_ITEM_SEARCH_GROUP_ITEM);

	CConfigEx	*pConfig = CConfigEx::Instance();
	unsigned char *pConfigData = NULL;
	unsigned long dataLen = 0;
	if (!pConfig->GetConfigEx(itemList, &pConfigData, dataLen))
	{
		printf("Get config data fail!\n");
		DVRSystem("exit");
		return 0;
	}

	CConfigBlock cfgBlock;
	cfgBlock.CreateBlock(pConfigData, dataLen);

	unsigned char *pData = NULL;
	cfgBlock.GetConfig(NCFG_ITEM_SEARCH_GROUP_ENABLE, &pData, dataLen );
	ShowData(NCFG_ITEM_SEARCH_GROUP_ENABLE,pData,dataLen,false);
	
	pData = NULL;
	cfgBlock.GetConfig(NCFG_ITEM_SEARCH_GROUP_ITEM, &pData, dataLen);
	ShowData(NCFG_ITEM_SEARCH_GROUP_ITEM,pData,dataLen,false);

	cfgBlock.DestroyBlock();
	pData=NULL;
	return 0;	

}

int CSearchGroupScrollConditionDlg::SaveGroupItemCfg()
{
	std::vector<NCFG_ITEM_ID> cfgItemList;
	BYTE *pData = NULL;
	unsigned long dataLen = 0;
	NCFG_ITEM_HEAD *pItemInfo = NULL;
	int index = 0;
	BYTE *pWritePos = NULL;
	unsigned long itemDataLen = 0;
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;

	//分配空间
	GetItemList(cfgItemList);
	if (0 == cfgItemList.size())
	{
		return -1;
	}
	dataLen = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * cfgItemList.size() +  sizeof(NCFG_INFO_GROUP_ENABLEINFO)+sizeof(NCFG_INFO_GROUP_ITEM)*MAX_GROUP_ITEM_COUNT;
	pData = new BYTE[dataLen+1];
	memset(pData, 0, dataLen+1);

	//组装头信息
	NCFG_BLOCK_HEAD *pblockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(pData);
	pblockHead->netcfgver = NCFG_VERSION;
	pblockHead->biSize = sizeof(NCFG_BLOCK_HEAD);
	pblockHead->ItemNum = cfgItemList.size();
	//获取每一个项的信息
	pItemInfo = reinterpret_cast<NCFG_ITEM_HEAD *>(pData + sizeof(NCFG_BLOCK_HEAD));
	index = 0;
	unsigned long itemOffsetLen = 0;
	for(std::vector<NCFG_ITEM_ID>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
	{
		switch(*iter)
		{
		case NCFG_ITEM_SEARCH_GROUP_ENABLE:
			{
				pItemInfo[index].itemID = (*iter);
				pItemInfo[index].num = 1;
				pItemInfo[index].len = sizeof(NCFG_INFO_GROUP_ENABLEINFO);
				pItemInfo[index].subLen = sizeof(NCFG_INFO_GROUP_ENABLEINFO);
				pWritePos = pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * cfgItemList.size() + itemOffsetLen;
				SetData((NCFG_ITEM_ID)(*iter), pWritePos, itemDataLen);	
				index++;
				itemOffsetLen += sizeof(NCFG_INFO_GROUP_ENABLEINFO);
			}
			break;
		case NCFG_ITEM_SEARCH_GROUP_ITEM:
			{
				pItemInfo[index].itemID = (*iter);
				pItemInfo[index].num = MAX_GROUP_ITEM_COUNT;
				pItemInfo[index].len = sizeof(NCFG_INFO_GROUP_ITEM)*MAX_GROUP_ITEM_COUNT;
				pItemInfo[index].subLen = sizeof(NCFG_INFO_GROUP_ITEM);
				pWritePos = pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * cfgItemList.size() + itemOffsetLen;
				SetData((NCFG_ITEM_ID)(*iter), pWritePos, itemDataLen);	
				index++;
				itemOffsetLen += sizeof(NCFG_INFO_GROUP_ITEM)*MAX_GROUP_ITEM_COUNT;
			}
			break;
		default:
			gui_assert(false, printf("error.empty item id = 0x%X\n", *iter));
			break;
		}
	}

	//进入配置
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_ENTER, NULL, 0, msgData);
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		delete [] pData;
		pData = NULL;
		return -1;
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	//保存信息
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_SET, pData, dataLen, msgData);
	delete [] pData;
	pData = NULL;
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{	

	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	//退出配置
	CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);


	return 0;	

}

void CSearchGroupScrollConditionDlg::GetItemList(std::vector<NCFG_ITEM_ID>& itemList) const
{
	itemList.push_back(NCFG_ITEM_SEARCH_GROUP_ENABLE);
	itemList.push_back(NCFG_ITEM_SEARCH_GROUP_ITEM);

}

bool CSearchGroupScrollConditionDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	switch(cfgID)
	{
	case NCFG_ITEM_SEARCH_GROUP_ENABLE:
		{
			assert(len == sizeof(unsigned long));
			const NCFG_INFO_GROUP_ENABLEINFO *pInfo = reinterpret_cast<const NCFG_INFO_GROUP_ENABLEINFO *>(pData);
			m_enable = pInfo->enable;
			m_curItem = pInfo->curItem;
			SetAllCheckEnable(true);
			if(0 == m_enable)
			{
				//非group方式
				for (int i=0; i<m_channelNum; ++i)
				{
					if (m_authCH & ((ULONGLONG)(0x01) << i))
					{
						m_pCKCH[i]->SetCheck(true, false);
					}
					else
					{
						m_pCKCH[i]->SetCheck(false, false);	
					}
				}

				unsigned char groupNum = (m_channelNum + 3)/4;
				for ( int i=0; i<groupNum; i++ )
				{
					DoClickChnn( i * 4, false );
				}
				m_pCheckGroup->SetCheck(false,false);
				m_pComboCtrl->SetEnable(false, false);
				m_pBtnAdd->SetEnable(false, false);
				m_pBtnDel->SetEnable(false, false);
				m_pBtnEdit->SetEnable(false, false);

			}
			else if(1 == m_enable)
			{
				SetAllCheckEnable(true);
				//group方式，不可以被勾选，只能查看
				for (int i=0; i<m_channelNum; ++i)
				{
					m_pCKCH[i]->SetCheck(false, false);
				}
				SetAllCheckEnable(false);
			}
			return true;
			break;
		}
	case NCFG_ITEM_SEARCH_GROUP_ITEM:
		{
			assert(len == MAX_GROUP_ITEM_COUNT * sizeof(NCFG_INFO_GROUP_ITEM));
			const NCFG_INFO_GROUP_ITEM* pGroupItem = reinterpret_cast<const NCFG_INFO_GROUP_ITEM*>(pData);
			////修改后把所有check框设置为不可以使用
			m_cfgGroupList.clear();
			m_pComboCtrl->RemoveAllItem(false);
			for (int i = 0; i < MAX_GROUP_ITEM_COUNT; i++)
			{
				if(strlen(pGroupItem[i].szItem)!=0)
				{
					m_cfgGroupList.push_back(pGroupItem[i]);
					m_pComboCtrl->AddItem(pGroupItem[i].szItem);
					//printf("%s,%d szitem=%s,chanbits=%x\n",__FILE__,__LINE__,pGroupItem[i].szItem,pGroupItem[i].chanbits);
				}
			}
			if(m_enable == 1)
			{
				if(m_cfgGroupList.size()==0)
				{
					//说明没有任何配置，只是打了勾，依旧刷成没勾选的样子
					SetAllCheckEnable(true);
					SetAllCheck(true);
					m_pCheckGroup->SetCheck(false, false);
					m_pComboCtrl->SetEnable(false, false);
					m_pBtnAdd->SetEnable(false, false);
					m_pBtnDel->SetEnable(false, false);
					m_pBtnEdit->SetEnable(false, false);
					return true;
				}
				SetAllCheckEnable(true);
				
				if(strlen(pGroupItem[m_curItem].szItem)!=0)
				{
					SetChannel(pGroupItem[m_curItem].chanbits);
					m_pCheckGroup->SetCheck(true, false);
					m_pComboCtrl->SetEnable(true, false);
					m_pComboCtrl->SetCurItem(m_curItem,false);
					m_pBtnDel->SetEnable(true, false);
					m_pBtnEdit->SetEnable(true, false);
				}
				if(m_cfgGroupList.size() >=MAX_GROUP_ITEM_COUNT )
				{
					//添加按钮置灰
					m_pBtnAdd->SetEnable(false, false);
				}
				else
				{
					m_pBtnAdd->SetEnable(true, false);
				}
				SetAllCheckEnable(false);
				return true;
			}
			else if(0 == m_enable)
			{
				return true;
			}
			else
			{
				assert(false);
				return false;
			}
			break;
		}
	default:
		break;
	}
	return false;
}

bool CSearchGroupScrollConditionDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, unsigned long& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch(cfgID)
	{
	case NCFG_ITEM_SEARCH_GROUP_ENABLE:
		{
			NCFG_INFO_GROUP_ENABLEINFO *pInfo = reinterpret_cast<NCFG_INFO_GROUP_ENABLEINFO *>(pData);
			pInfo->enable=m_enable;
			pInfo->curItem=m_curItem;
			break;
		}
	case NCFG_ITEM_SEARCH_GROUP_ITEM:
		{
			assert(len == MAX_GROUP_ITEM_COUNT * sizeof(NCFG_INFO_GROUP_ITEM));
			NCFG_INFO_GROUP_ITEM* pGroupItem = reinterpret_cast< NCFG_INFO_GROUP_ITEM*>(pData);
			//把pGroupItem刷空，保存m_cfgGroupList的值
			for(int i=0;i<MAX_GROUP_ITEM_COUNT;i++)
			{
				memset(&pGroupItem[i],0,sizeof(NCFG_INFO_GROUP_ITEM));
			}
			int count=0;
			for(GROUP_ITEM_LIST::const_iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();it++)
			{
				pGroupItem[count]=*it;
				count++;
			}
			return true;
			break;
		}
	default:
		break;

	}
	return false;
}


void CSearchGroupScrollConditionDlg::OnDestroy()
{
	GUI::CScrollDialog::OnDestroy();
	
	if(m_pCKCH!=NULL)
	{
		delete []m_pCKCH;
	}
	if(m_pCKGroup)
	{
		delete []m_pCKGroup;
	}
	if(m_pCheckGroup!=NULL)
	{
		m_pCheckGroup=NULL;
	}
	if(m_pComboCtrl!=NULL)
	{
		m_pComboCtrl=NULL;
	}
	if(m_pBtnAdd!=NULL)
	{
		m_pBtnAdd=NULL;
	}
	if(m_pBtnDel!=NULL)
	{
		m_pBtnDel=NULL;
	}
	if(m_pBtnEdit!=NULL)
	{
		m_pBtnEdit=NULL;
	}
}

void CSearchGroupScrollConditionDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CScrollDialog::OnPaint(status);

	int cx, cy;
	GetCurScrollPos(cx, cy);

	//画列
	int x = m_rect.m_left + m_length[CHECK_WIDTH] + 12;
	int start = m_rect.m_top + m_length[CALENDAR_HEIGHT] + 10 - cy * (m_length[CALENDAR_HEIGHT] + 5);
	int end = start + ((m_channelNum + 3)/4)*m_length[CHECK_HEIGHT];
	int offset = 5;

	GUI::CWnd::DrawLine(m_rect.m_left+offset, start, m_rect.m_left+offset, end, COLOR_LINE_GRAY);//最左边的竖条
	GUI::CWnd::DrawLine(x, start, x, end, COLOR_LINE_GRAY);//最左边的竖条
	GUI::CWnd::DrawLine(m_rect.m_right-offset - 20, start, m_rect.m_right-offset- 20, end, COLOR_LINE_GRAY);//最左边的竖条

	//画行
	int y = start;// + m_length[CHECK_HEIGHT];
	while (y <= end)
	{
		GUI::CWnd::DrawLine(m_rect.m_left+offset, y, m_rect.m_right-offset -20, y, COLOR_LINE_GRAY);
		y += m_length[CHECK_HEIGHT];
	}

}

void CSearchGroupScrollConditionDlg::MarkDay(unsigned short year, unsigned char month, unsigned char day, unsigned long color, bool bRefresh)
{
	GUI::CCalendarCtrl* pCalendar = dynamic_cast<GUI::CCalendarCtrl*>(GetScrollWnd().GetWnd(ID_CALENDAR));
	if(pCalendar!=NULL)
	{
		pCalendar->MarkDay(year, month, day, color, bRefresh);
	}
	else
	{
		printf("NULL point 541\n");
	}
	
	//m_pCalendar->MarkDay(year, month, day, color, bRefresh);
}

BEGIN_MSG_MAP(CSearchGroupScrollConditionDlg, GUI::CScrollDialog)
	ON_MSG_EX(GetScrollWnd().GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickScrollWnd)
END_MSG_MAP()

unsigned long CSearchGroupScrollConditionDlg::OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int index = -1;
	unsigned long tmpIndex = (keyID >> 20)&0x7FF;
	if( tmpIndex < 0x7FF )
	{
		index = tmpIndex;
	}
	unsigned long id = keyID&0xFFFFF;

	printf("(%s,line=%d)index=%d\n",__FILE__,__LINE__,index);

	if (index > 0)
	{
		if ((index >= ID_CH) && (index <= ID_CH + 31))
		{
			OnClickChnn(index);
		}
		if ((index >= ID_GROUP) && (index <= ID_GROUP + 7))
		{
			OnClickGroup(index);
		}
		switch(index)
		{
			case ID_GROUP_CHECK:
				{
					OnClickCheckGroup();
					break;
				}
			case ID_GROUP_COMBO:
				{
					OnClickComboGroup();
					break;
				}
			case ID_GROUP_BTNADD:
				{
					OnClickBtnAdd();
					break;
				}
			case ID_GROUP_BTNDEL:
				{
					OnClickBtnDel();
					break;
				}
			case ID_GROUP_BTNEDIT:
				{
					OnClickBtnEdit();
					break;
				}
			default:
				break;
		}
	}
	return KEY_VIRTUAL_MSG_STOP;

}

unsigned long CSearchGroupScrollConditionDlg::OnClickCheckGroup()
{
	if (m_pCheckGroup->IsCheck())
	{
		m_pCheckGroup->SetCheck(true, false);
		m_pComboCtrl->SetEnable(true,false);
		m_pBtnAdd->SetEnable(true,false);
		m_pBtnDel->SetEnable(true,false);
		m_pBtnEdit->SetEnable(true,false);
		
		//如果有分组，刷新combo页面当前值到画面
		SetAllCheckEnable(true);
		SetAllCheck(false);
		m_pComboCtrl->SetCurItem(m_curItem,false);
		string curStr;
		m_pComboCtrl->GetItemString(m_curItem,curStr);
		for(GROUP_ITEM_LIST::iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();it++)
		{
			if(strcmp(it->szItem,curStr.c_str()) == 0)
			{
				SetChannel(it->chanbits);
				break;
			}
		}		
		if(m_cfgGroupList.size()<=0)
		{
			m_pBtnDel->SetEnable(false,false);
			m_pBtnEdit->SetEnable(false,false);
		}
		//禁用所有的通道check框
		SetAllCheckEnable(false);
		m_enable=1;
	}
	else
	{
		m_pComboCtrl->SetEnable(false,false);
		m_pBtnAdd->SetEnable(false,false);
		m_pBtnDel->SetEnable(false,false);
		m_pBtnEdit->SetEnable(false,false);
		m_pCheckGroup->SetCheck(false, false);
		SetAllCheckEnable(true);
		SetAllCheck(true);
		m_enable=0;
	}
	m_bModify = true;
	GetRootWnd()->Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupScrollConditionDlg::OnClickComboGroup()
{
	//enable所有的通道check框
	SetAllCheckEnable(true);

	//把对应的check框信息刷到屏幕
	int nCount = m_pComboCtrl->GetItemCount();
	if(nCount>0)
	{
		int curItem = m_pComboCtrl->GetCurItem();
		if(curItem!=-1)
		{	
			SetComboCurChannel(curItem);
			m_curItem = curItem;
			m_bModify=true;
		}
		else
		{
			printf("%s err\n",__FUNCTION__);
		}

	}
	//禁用所有的通道check框
	SetAllCheckEnable(false);
	GetRootWnd()->Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupScrollConditionDlg::OnClickBtnAdd()
{
	m_DlgAddGroup.SetInitialInfo(m_channelNum,m_authCH,&m_cfgGroupList);
	m_DlgAddGroup.GetEditEnable(false,"",0);
	if (DLG_OK == m_DlgAddGroup.DoModal(this))
	{		
		m_pComboCtrl->RemoveAllItem(false);
		for(GROUP_ITEM_LIST::iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();it++)
		{
			m_pComboCtrl->AddItem(it->szItem);
		}	
		int curItem=m_DlgAddGroup.SendCurnItem();
		m_pComboCtrl->SetCurItem(curItem,false);//修改combo当前值为新加的分组序号
		m_curItem = curItem;
		SetAllCheckEnable(true);
		SetComboCurChannel(curItem);
		SetAllCheckEnable(false);
		m_bModify = true;
		if (MAX_GROUP_ITEM_COUNT-1 <= m_cfgGroupList.size())
		{
			m_pBtnAdd->SetEnable(false,false);
		}
		int num=m_cfgGroupList.size();
		if (MAX_GROUP_ITEM_COUNT<= num)
		{
			m_pBtnAdd->SetEnable(false,false);
		}
		else if(num == 1)
		{
			m_pBtnDel->SetEnable(true,false);
			m_pBtnEdit->SetEnable(true,false);
		}
		GetRootWnd()->Repaint();
		return KEY_VIRTUAL_MSG_STOP;
	}	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupScrollConditionDlg::OnClickBtnDel()
{
	if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_SEARCH_DEL_GROUP), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING))
	{
		//放弃则直接返回
		return KEY_VIRTUAL_MSG_STOP;
	}

	if(MAX_GROUP_ITEM_COUNT == m_pComboCtrl->GetItemCount())
	{
		m_pBtnAdd->SetEnable(true,false);
	}
	int nCurItem = m_pComboCtrl->GetCurItem();
	int next = 0;
	string nextText;
	string text;

	if(nCurItem != -1)
	{
		if(nCurItem == m_pComboCtrl->GetItemCount()-1)
		{
			if(nCurItem == 0)//combo中只有一项
			{
				m_enable=0;
				//所有控件消失,check框设置为不勾选
				m_pComboCtrl->SetEnable(false,false);
				m_pComboCtrl->RemoveAllItem(false);
				m_pBtnAdd->SetEnable(false,false);
				m_pBtnDel->SetEnable(false,false);
				m_pBtnEdit->SetEnable(false,false);
				m_pCheckGroup->SetCheck(false, false);
				m_cfgGroupList.clear();
				
				SetAllCheckEnable(true);
				SetAllCheck(true);
				GetRootWnd()->Repaint();
				m_bModify = true;
				return KEY_VIRTUAL_MSG_STOP;
			}
			else//当前值是最后一项
			{
				//当前值为combo的第一项目
				m_pComboCtrl->GetItemString(nCurItem,text);
				m_pComboCtrl->GetItemString(0,nextText);
				//从数组中寻找匹配的字符串
				SetAllCheckEnable(true);
				m_pComboCtrl->RemoveAllItem(false);
				m_cfgGroupList.pop_back();//弹出最后一个
				for(GROUP_ITEM_LIST::iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();it++)
				{
					m_pComboCtrl->AddItem(it->szItem);
				}
				m_pComboCtrl->SetCurItem(0,false);
				m_curItem = 0;
				SetComboCurChannel(0);
				SetAllCheckEnable(false);
				GetRootWnd()->Repaint();
			}
		}
		else if(nCurItem < m_pComboCtrl->GetItemCount()-1)
		{
			next = nCurItem+1;
			m_pComboCtrl->GetItemString(nCurItem,text);
			m_pComboCtrl->GetItemString(next,nextText);

			SetAllCheckEnable(true);
			m_pComboCtrl->RemoveAllItem(false);
			for(GROUP_ITEM_LIST::iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();)
			{
				if(strcmp(it->szItem,text.c_str())==0)
				{
					it=m_cfgGroupList.erase(it);
				}
				else
				{
					it++;
				}
			}
			int count=0;
			for(GROUP_ITEM_LIST::iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();it++)
			{
				if(strcmp(it->szItem,nextText.c_str())==0)
				{
					m_curItem=count;
					SetChannel(it->chanbits);
				}
				m_pComboCtrl->AddItem(it->szItem);
				count++;
			}
			m_pComboCtrl->SetCurItem(m_curItem,false);
			SetAllCheckEnable(false);
			GetRootWnd()->Repaint();
		}
		else
		{
			printf("error\n");
			assert(false);
			return KEY_VIRTUAL_MSG_STOP;
		}
		if (0 >= m_cfgGroupList.size())
		{
			m_pBtnDel->SetEnable(false,false);
			m_pBtnEdit->SetEnable(false,false);
		}
		m_bModify = true;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupScrollConditionDlg::OnClickBtnEdit()
{
	m_DlgAddGroup.SetInitialInfo(m_channelNum,m_authCH,&m_cfgGroupList);
	int curItem = m_pComboCtrl->GetCurItem();
	if(curItem!=-1)
	{
		string curStr;
		m_pComboCtrl->GetItemString(curItem,curStr);
		m_DlgAddGroup.GetEditEnable(true,curStr,curItem);
	}
	else
	{
		printf("curitem can not be -1\n");
		return KEY_VIRTUAL_MSG_STOP;
	}
	if (DLG_OK == m_DlgAddGroup.DoModal(this))
	{
		int curItem=-1;
		curItem=m_DlgAddGroup.SendCurnItem();
		m_pComboCtrl->RemoveAllItem(false);
		for(GROUP_ITEM_LIST::iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();it++)
		{
			m_pComboCtrl->AddItem(it->szItem);
		}
		if(curItem!=-1)
		{
			m_pComboCtrl->SetCurItem(curItem,false);//修改combo当前值为新加的分组序号
			SetAllCheckEnable(true);
			SetComboCurChannel(curItem);
			SetAllCheckEnable(false);
		}
		GetRootWnd()->Repaint();
		m_bModify=true;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupScrollConditionDlg::OnClickChnn( unsigned long ctrlID )
{
	DoClickChnn( ctrlID - ID_CH , false );
	GetRootWnd()->Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupScrollConditionDlg::OnClickGroup( unsigned long ctrlID )
{
	unsigned long groupNum = ctrlID - ID_GROUP;
	GUI::CCheckCtrl* pCKGroup = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_GROUP + groupNum));
	GUI::CCheckCtrl* pCKCH;

	bool check = pCKGroup->IsCheck();
	for ( int i = 0; i < 4; i++ )
	{
		pCKCH = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CH + groupNum * 4 + i));
		if(m_channelNum <= (groupNum * 4 + i))
		{
			break;
		}

		if (m_authCH & ((ULONGLONG)(0x01) << (groupNum * 4 + i)))
		{
			if( pCKCH->IsCheck() != check )
			{
				pCKCH->SetCheck( check, false );
			}
		}
	}
	GetRootWnd()->Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupScrollConditionDlg::DoClickChnn( unsigned long index, bool bRefresh )
{
	int groupNum = index / 4;
	GUI::CCheckCtrl* pCKCH;

	bool check = true;
	for ( int i = 0; i < 4; i++ )
	{
		pCKCH = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CH + groupNum * 4 + i));
		if( groupNum * 4 + i > m_channelNum - 1 )
		{
			break;
		}

		if( !pCKCH->IsCheck() )
		{
			check = false;
			break;
		}
	}
	GUI::CCheckCtrl* pCKGroup = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_GROUP + groupNum));
	if( pCKGroup->IsCheck() != check )
	{
		pCKGroup->SetCheck( check, bRefresh );
	}

	return KEY_VIRTUAL_MSG_STOP;
}

ULONGLONG CSearchGroupScrollConditionDlg::GetCurChannel() const
{
	ULONGLONG longDate = 0;
	for (int i = 0; i < m_channelNum; i++)
	{
		if (m_pCKCH[i]->IsCheck())
		{
			longDate |= m_authCH & ((ULONGLONG)0x01 << i);
		}
	}
	return longDate;
}

void CSearchGroupScrollConditionDlg::SetChannel(const ULONGLONG chanbits)
{
	//设置具体小项
	for (int i = 0; i < m_channelNum; i++)
	{
		if ((chanbits & ((ULONGLONG)0x01 << i)) && !m_pCKCH[i]->IsCheck())
		{
			m_pCKCH[i]->SetCheck(true, false);
		}
		else if (!(chanbits & ((ULONGLONG)0x01 << i)) && m_pCKCH[i]->IsCheck())
		{
			m_pCKCH[i]->SetCheck(false, false);
		}
	}

	//设置4个小项组成的组项
	int groupNem = m_channelNum / 4;
	for (int i = 0; i < groupNem; i++)
	{
		bool bCheck = true;
		for (int j = 0; j < 4; j++)
		{
			if (!m_pCKCH[4 * i + j]->IsCheck())
			{
				bCheck = false;
				break;
			}
		}

		if (bCheck)
		{
			m_pCKGroup[i]->SetCheck(true, false);
		}
		else
		{
			m_pCKGroup[i]->SetCheck(false, false);
		}
	}
}

void  CSearchGroupScrollConditionDlg::SetAllCheckEnable(bool benable)
{
	for (int i = 0; i < m_channelNum; i++)
	{
		m_pCKCH[i]->SetEnable(benable,false);	
	}
	unsigned char groupNum = (m_channelNum + 3)/4;
	for (int i=0; i<groupNum; ++i)
	{
		m_pCKGroup[i]->SetEnable(benable,false);
	}
}

void CSearchGroupScrollConditionDlg::SetAllCheck(bool bcheck)
{
	for (int i = 0; i < m_channelNum; i++)
	{
		m_pCKCH[i]->SetCheck(bcheck,false);
	}
	unsigned char groupNum = (m_channelNum + 3)/4;
	for (int i=0; i<groupNum; ++i)
	{
		m_pCKGroup[i]->SetCheck(bcheck,false);
	}
}

void CSearchGroupScrollConditionDlg::SetComboCurChannel(int curItem)
{
	string text;
	m_pComboCtrl->GetItemString(curItem,text);
	for(GROUP_ITEM_LIST::iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();it++)
	{
		if(strcmp(it->szItem,text.c_str()) == 0)
		{
			SetChannel(it->chanbits);
			break;
		}
	}	
}


#endif

//end

