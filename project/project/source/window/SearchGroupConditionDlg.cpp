/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : wn
** Date         : 2009-08-01
** Name         : SearchGroupConditionDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#if defined(__GROUY_SEARCH__)
#include "SearchGroupConditionDlg.h"
#include "NetProtocol.h"
#include "MessageMan.h"
#include "MessageBox.h"
#include "UserMan.h"
#include "StringDefine.h"

//extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
CSearchGroupConditionDlg::CSearchGroupConditionDlg() : m_pCKCH(NULL), m_channelNum(0), m_authCH(0)
,m_bModify(false)
{
	m_enable = 0;
	m_curItem = 0;
	m_cfgGroupList.clear();
}

CSearchGroupConditionDlg::~CSearchGroupConditionDlg()
{

}

void CSearchGroupConditionDlg::SetInitialInfo(unsigned long channelNum, ULONGLONG authCH)
{
	m_channelNum = channelNum;
	//m_authCH	= authCH;
	m_authCH = ALL_CHNN_MASK(m_channelNum);//搜索不需要回放权限
}

unsigned long CSearchGroupConditionDlg::Time()
{
	return m_calendar.Time();
}

ULONGLONG CSearchGroupConditionDlg::CH()
{
	ULONGLONG chnnBits = 0;
	unsigned char chnnNum = m_channelNum;
	for (int i=0; i<chnnNum; ++i)
	{
		chnnBits |= ((!m_pCKCH[i].IsCheck()) ? 0 : (0x01 << i));
	}

	return chnnBits;
}


//////////////////////////////////////////////////////////////////////////
void CSearchGroupConditionDlg::OnInitial()
{
	assert(m_channelNum > 0);

	GUI::CDialog::OnInitial();
	
	unsigned short x = 5, y = 5, offset = 50;

	m_calendar.Create(CTRLID_SEARCH_CALENDAR, x, y, m_length[CALENDAR_WIDTH], m_length[CALENDAR_HEIGHT], this, 0, 0);

	//////////////////////////////////////////////////////////////////////////
	unsigned char groupNum = (m_channelNum + 3)/4;
	m_pCKGroup = new GUI::CCheckCtrl [groupNum];
	x = 10; y += (m_length[CALENDAR_HEIGHT] + 8);
	for (int i=0; i<groupNum; ++i)
	{
		m_pCKGroup[i].Create(CTRLID_SEARCH_GROUP_0 + i, x, y, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT], this, 0, i+1);
		y += m_length[CHECK_HEIGHT];
	}

	int localChnnNum = CProduct::Instance()->LocalVideoInputNum();
	char ch [] = "32";
	m_pCKCH = new GUI::CCheckCtrl [m_channelNum];
	x = 40; y = (m_length[CALENDAR_HEIGHT] + 13), offset = 54;
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
		m_pCKCH[i].Create(CTRLID_SEARCH_CH_0+i, x+(i%4)*offset, y+(i/4)*m_length[CHECK_HEIGHT], offset, m_length[CHECK_HEIGHT], this, i%4+1, 1+i/4);
		m_pCKCH[i].SetCaption(ch, false, false);
		m_pCKCH[i].SetEnable(false,false);
	}

	//--------------------2011.6.30----------------
	x = m_length[DLG_OFFSET_X];
	y = m_length[CALENDAR_HEIGHT] + groupNum * m_length[CHECK_HEIGHT] + 25;
	int totalcount=m_channelNum/4+1;
	m_checkCtrlGroup.Create(GetRandWndID(), x, y, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT], this, totalcount, totalcount);

	x += m_length[CHECK_WIDTH] + 3;
	m_staticGroup.Create(GetRandWndID(), x, y, m_length[TITLE_EXIT_BTN_RIGHT_XPOS], m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_SEARCH_GROUP));

	x += m_length[TITLE_EXIT_BTN_RIGHT_XPOS] + 10;
	m_comboCtrlGroup.Create(GetRandWndID(), x, y, m_length[BTN_WIDTH] + 2 * m_length[COMBO_WIDTH], m_length[COMBO_HEIGHT], this, totalcount, totalcount+1);
	m_comboCtrlGroup.SetCaption("",false);

	x = m_length[DLG_OFFSET_X];
	y += m_length[RADIO_HEIGHT] + 15;
	m_buttonAdd.Create(GetRandWndID(), x, y, m_length[BTN_WIDTH] - 10, m_length[BTN_HEIGHT], this, totalcount, totalcount+2, m_pStrTable->String(E_STR_ID_ADD));

	x += m_length[BTN_WIDTH] - 5;
	m_buttonDel.Create(GetRandWndID(), x, y, m_length[BTN_WIDTH] - 10, m_length[BTN_HEIGHT], this, totalcount, totalcount+3, m_pStrTable->String(E_STR_ID_DELETE));

	x += m_length[BTN_WIDTH] - 5;
	m_buttonEdit.Create(GetRandWndID(), x, y, m_length[BTN_WIDTH] - 10, m_length[BTN_HEIGHT], this, totalcount, totalcount+4, m_pStrTable->String(E_STR_ID_EDIT));
	m_checkCtrlGroup.SetCheck(false, false);
	m_checkCtrlGroup.Show(true, false);
	
	m_comboCtrlGroup.Show(false, false);
	m_buttonAdd.Show(false, false);
	m_buttonDel.Show(false, false);
	m_buttonEdit.Show(false, false);
	
}

void CSearchGroupConditionDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();
	LoadGroupItemCfg();
}

int CSearchGroupConditionDlg::LoadGroupItemCfg()
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

int CSearchGroupConditionDlg::SaveGroupItemCfg()
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

void CSearchGroupConditionDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	if (NULL != m_pCKCH)
	{
		delete [] m_pCKCH;
		m_pCKCH = NULL;
	}

	if(NULL != m_pCKGroup )
	{
		delete [] m_pCKGroup;
		m_pCKGroup = NULL;
	}
	m_cfgGroupList.clear();
}

void CSearchGroupConditionDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);

	//画列
	int x = m_rect.m_left + m_length[CHECK_WIDTH] + 12;
	int start = m_rect.m_top + m_length[CALENDAR_HEIGHT] + 10;
	int end = start + ((m_channelNum + 3)/4)*m_length[CHECK_HEIGHT];
	int offset = 5;

	GUI::CWnd::DrawLine(m_rect.m_left+offset, start, m_rect.m_left+offset, end, COLOR_LINE_GRAY);//最左边的竖条
	GUI::CWnd::DrawLine(x, start, x, end, COLOR_LINE_GRAY);//最左边的竖条
	GUI::CWnd::DrawLine(m_rect.m_right-offset, start, m_rect.m_right-offset, end, COLOR_LINE_GRAY);//最左边的竖条

	//画行
	int y = start;// + m_length[CHECK_HEIGHT];
	while (y <= end)
	{
		GUI::CWnd::DrawLine(m_rect.m_left+offset, y, m_rect.m_right-offset, y, COLOR_LINE_GRAY);
		y += m_length[CHECK_HEIGHT];
	}

}

void CSearchGroupConditionDlg::MarkDay(unsigned short year, unsigned char month, unsigned char day, unsigned long color, bool bRefresh)
{
	m_calendar.MarkDay(year, month, day, color, bRefresh);
}

BEGIN_MSG_MAP(CSearchGroupConditionDlg, GUI::CDialog)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_CH_0, CTRLID_SEARCH_CH_31, KEY_VIRTUAL_ENTER, OnClickChnn)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_GROUP_0, CTRLID_SEARCH_GROUP_7, KEY_VIRTUAL_ENTER, OnClickGroup)
	ON_MSG(m_checkCtrlGroup.GetID(), KEY_VIRTUAL_ENTER, OnClickCheckGroup)
	ON_MSG(m_comboCtrlGroup.GetID(), KEY_VIRTUAL_ENTER, OnClickComboGroup)
	ON_MSG(m_buttonAdd.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnAdd)
	ON_MSG(m_buttonDel.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnDel)
	ON_MSG(m_buttonEdit.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnEdit)
END_MSG_MAP()

unsigned long CSearchGroupConditionDlg::OnClickChnn( unsigned long ctrlID )
{
	 DoClickChnn( ctrlID - CTRLID_SEARCH_CH_0 , false );
	 GetRootWnd()->Repaint();
	 return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupConditionDlg::OnClickGroup( unsigned long ctrlID )
{
	unsigned long groupNum = ctrlID - CTRLID_SEARCH_GROUP_0;

	bool check = m_pCKGroup[groupNum].IsCheck();
	for ( int i = 0; i < 4; i++ )
	{
		if(m_channelNum <= (groupNum * 4 + i))
		{
			break;
		}
		
		if (m_authCH & ((ULONGLONG)(0x01) << (groupNum * 4 + i)))
		{
			if( m_pCKCH[groupNum * 4 + i].IsCheck() != check )
			{
				m_pCKCH[groupNum * 4 + i].SetCheck( check, false );
			}
		}
	}
	GetRootWnd()->Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupConditionDlg::DoClickChnn( unsigned long index, bool bRefresh )
{
	int groupNum = index / 4;

	bool check = true;
	for ( int i = 0; i < 4; i++ )
	{
		if( groupNum * 4 + i > m_channelNum - 1 )
		{
			break;
		}

		if( !m_pCKCH[groupNum * 4 + i].IsCheck() )
		{
			check = false;
			break;
		}
	}

	if( m_pCKGroup[groupNum].IsCheck() != check )
	{
		m_pCKGroup[groupNum].SetCheck( check, bRefresh );
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

//-------------------2011.6.30--------------------
void CSearchGroupConditionDlg::GetItemList(std::vector<NCFG_ITEM_ID>& itemList) const
{
	itemList.push_back(NCFG_ITEM_SEARCH_GROUP_ENABLE);
	itemList.push_back(NCFG_ITEM_SEARCH_GROUP_ITEM);
	
}

bool CSearchGroupConditionDlg:: ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
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
						m_pCKCH[i].SetCheck(true, false);
					}
					else
					{
						m_pCKCH[i].SetEnable(false, false);
					}
				}

				unsigned char groupNum = (m_channelNum + 3)/4;
				for ( int i=0; i<groupNum; i++ )
				{
					DoClickChnn( i * 4, false );
				}
				m_checkCtrlGroup.SetCheck(false,false);
				m_comboCtrlGroup.Show(false, false);
				m_buttonAdd.Show(false, false);
				m_buttonDel.Show(false, false);
				m_buttonEdit.Show(false, false);
			}
			else if(1 == m_enable)
			{
				//group方式，m_pCKCH不可以被勾选，只能查看
				for (int i=0; i<m_channelNum; ++i)
				{
					if (m_authCH & ((ULONGLONG)(0x01) << i))
					{
						m_pCKCH[i].SetCheck(false, false);//先把所有的设置成无勾选
					}
					else
					{
						m_pCKCH[i].SetEnable(false, false);
					}
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
			m_cfgGroupList.clear();
			m_comboCtrlGroup.RemoveAllItem(false);//移除所有项目
			for (int i = 0; i < MAX_GROUP_ITEM_COUNT; i++)
			{
				if(strlen(pGroupItem[i].szItem)!=0)
				{
					m_cfgGroupList.push_back(pGroupItem[i]);
					m_comboCtrlGroup.AddItem(pGroupItem[i].szItem);
				}
			}

			if(m_enable == 1)
			{
				if(m_cfgGroupList.size()==0)
				{
					//说明没有任何配置，只是打了勾，依旧刷成没勾选的样子
					SetAllCheckEnable(true);
					SetAllCheck(true);
					m_checkCtrlGroup.SetCheck(false, false);
					m_comboCtrlGroup.Show(false, false);
					m_buttonAdd.Show(false, false);
					m_buttonDel.Show(false, false);
					m_buttonEdit.Show(false, false);
					return true;
				}
				////修改后把所有check框设置为不可以使用
				SetAllCheckEnable(true);
				if(strlen(pGroupItem[m_curItem].szItem)!=0)
				{
					SetChannel(pGroupItem[m_curItem].chanbits);
					m_checkCtrlGroup.SetCheck(true, false);
					m_comboCtrlGroup.Show(true, false);
					m_buttonAdd.Show(true, false);
					m_buttonDel.Show(true, false);
					m_buttonEdit.Show(true, false);
					m_comboCtrlGroup.SetCurItem(m_curItem,false);
				}
				if(m_cfgGroupList.size() >=MAX_GROUP_ITEM_COUNT )
				{
					//添加按钮置灰
					m_buttonAdd.SetEnable(false, false);
				}
				else
				{
					m_buttonAdd.SetEnable(true, false);
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

bool CSearchGroupConditionDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, unsigned long& len)  const
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
				//不管m_benable是不是等于0，都要保存配置，但是不显示（m_benable=0）
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

unsigned long CSearchGroupConditionDlg::OnClickCheckGroup()
{
	if (m_checkCtrlGroup.IsCheck())
	{
		m_checkCtrlGroup.SetCheck(true, false);
		m_comboCtrlGroup.Show(true, false);
		m_buttonAdd.Show(true, false);
		m_buttonDel.Show(true, false);
		m_buttonEdit.Show(true, false);
			
		//如果有分组，刷新combo页面当前值到画面
		SetAllCheckEnable(true);
		SetAllCheck(false);
		m_comboCtrlGroup.SetCurItem(m_curItem,false);
		string curStr;
		m_comboCtrlGroup.GetItemString(m_curItem,curStr);
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
			m_buttonDel.SetEnable(false,false);
			m_buttonEdit.SetEnable(false,false);
		}
		//禁用所有的通道check框
		SetAllCheckEnable(false);
		m_enable=1;
	}
	else
	{
		m_checkCtrlGroup.SetCheck(false, false);
		m_comboCtrlGroup.Show(false, false);
		m_buttonAdd.Show(false, false);
		m_buttonDel.Show(false, false);
		m_buttonEdit.Show(false, false);
		SetAllCheckEnable(true);
		SetAllCheck(true);
		m_enable=0;
	}
	m_bModify = true;
	GetRootWnd()->Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupConditionDlg::OnClickComboGroup()
{
	//enable所有的通道check框
	SetAllCheckEnable(true);
	
	//把对应的check框信息刷到屏幕
	int nCount = m_comboCtrlGroup.GetItemCount();
	if(nCount>0)
	{
		int curItem = m_comboCtrlGroup.GetCurItem();
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

unsigned long CSearchGroupConditionDlg::OnClickBtnAdd()
{
	m_DlgAddGroup.SetInitialInfo(m_channelNum,m_authCH,&m_cfgGroupList);
	m_DlgAddGroup.GetEditEnable(false,"",0);
	if (DLG_OK == m_DlgAddGroup.DoModal(this))
	{		
		m_comboCtrlGroup.RemoveAllItem(false);
		for(GROUP_ITEM_LIST::iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();it++)
		{
			//printf("Add szitem=%s,chanbits=%x\n",it->szItem,it->chanbits);
			m_comboCtrlGroup.AddItem(it->szItem);
		}	
	   int curItem=m_DlgAddGroup.SendCurnItem();
	   m_comboCtrlGroup.SetCurItem(curItem,false);//修改combo当前值为新加的分组序号
	   m_curItem = curItem;
	   SetAllCheckEnable(true);
	   SetComboCurChannel(curItem);
	   SetAllCheckEnable(false);
	   m_bModify = true;
	   int num=m_cfgGroupList.size();
	   if (MAX_GROUP_ITEM_COUNT<= num)
	   {
		   m_buttonAdd.SetEnable(false,false);
	   }
	   else if(num == 1)
	   {
		   m_buttonDel.SetEnable(true,false);
		   m_buttonEdit.SetEnable(true,false);
	   }
	   GetRootWnd()->Repaint();
	   return KEY_VIRTUAL_MSG_STOP;
	}	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupConditionDlg::OnClickBtnDel()
{
	if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_SEARCH_DEL_GROUP), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING))
	{
		//放弃则直接返回
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	if(MAX_GROUP_ITEM_COUNT == m_comboCtrlGroup.GetItemCount())
	{
		m_buttonAdd.SetEnable(true,false);
	}

 	int nCurItem = m_comboCtrlGroup.GetCurItem();
	int next = 0;
	string nextText;
	string text;
	
	if(nCurItem != -1)
	{
		if(nCurItem == m_comboCtrlGroup.GetItemCount()-1)
		{
			if(nCurItem == 0)//combo中只有一项
			{ 
				m_enable=0;
				//所有控件消失,check框设置为全勾选
				m_checkCtrlGroup.SetCheck(false, false);
				m_comboCtrlGroup.RemoveAllItem(false);
				m_comboCtrlGroup.Show(false, false);
				m_buttonAdd.Show(false, false);
				m_buttonDel.Show(false, false);
				m_buttonEdit.Show(false, false);
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
				m_comboCtrlGroup.GetItemString(nCurItem,text);
				m_comboCtrlGroup.GetItemString(0,nextText);
			
				//从数组中寻找匹配的字符串
				SetAllCheckEnable(true);
				m_comboCtrlGroup.RemoveAllItem(false);
				m_cfgGroupList.pop_back();//弹出最后一个
				for(GROUP_ITEM_LIST::iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();it++)
				{
					m_comboCtrlGroup.AddItem(it->szItem);
				}
				m_comboCtrlGroup.SetCurItem(0,false);
				m_curItem = 0;
				SetComboCurChannel(0);
				SetAllCheckEnable(false);
				GetRootWnd()->Repaint();
			}
		}
		else if(nCurItem < m_comboCtrlGroup.GetItemCount()-1)
		{
			next = nCurItem+1;
			m_comboCtrlGroup.GetItemString(nCurItem,text);
			m_comboCtrlGroup.GetItemString(next,nextText);
			
			SetAllCheckEnable(true);
			m_comboCtrlGroup.RemoveAllItem(false);
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
				m_comboCtrlGroup.AddItem(it->szItem);
				count++;
			}
			m_comboCtrlGroup.SetCurItem(m_curItem,false);
			SetAllCheckEnable(false);
			GetRootWnd()->Repaint();
		}
		else
		{
			printf("error\n");
			assert(false);
			return KEY_VIRTUAL_MSG_STOP;
		}
		
		m_bModify = true;

		if (0 >= m_cfgGroupList.size())
		{
			m_buttonDel.SetEnable(false,false);
			m_buttonEdit.SetEnable(false,false);
		}

	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupConditionDlg::OnClickBtnEdit()
{
	m_DlgAddGroup.SetInitialInfo(m_channelNum,m_authCH,&m_cfgGroupList);

	int curItem = m_comboCtrlGroup.GetCurItem();
	if(curItem!=-1)
	{
		string curStr;
		m_comboCtrlGroup.GetItemString(curItem,curStr);
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
		m_comboCtrlGroup.RemoveAllItem(false);
		for(GROUP_ITEM_LIST::iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();it++)
		{
			m_comboCtrlGroup.AddItem(it->szItem);
		}
		if(curItem!=-1)
		{
			m_comboCtrlGroup.SetCurItem(curItem,false);//修改combo当前值为新加的分组序号
			SetAllCheckEnable(true);
			SetComboCurChannel(curItem);
			SetAllCheckEnable(false);
		}
		GetRootWnd()->Repaint();
		m_bModify=true;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

ULONGLONG CSearchGroupConditionDlg::GetCurChannel() const
{
	ULONGLONG longDate = 0;
	for (int i = 0; i < m_channelNum; i++)
	{
		if (m_pCKCH[i].IsCheck())
		{
			longDate |= ((ULONGLONG)0x01 << i);
			longDate |= m_authCH & ((ULONGLONG)0x01 << i);
			//printf("%#x\n", longDate);
		}
	}
	return longDate;
}

void CSearchGroupConditionDlg::SetChannel(const ULONGLONG chanbits)
{
	//设置具体小项
	for (int i = 0; i < m_channelNum; i++)
	{
		if ((chanbits & ((ULONGLONG)0x01 << i)) && !m_pCKCH[i].IsCheck())
		{
			m_pCKCH[i].SetCheck(true, false);
		}
		else if (!(chanbits & ((ULONGLONG)0x01 << i)) && m_pCKCH[i].IsCheck())
		{
			m_pCKCH[i].SetCheck(false, false);
		}
	}

	//设置4个小项组成的组项
	int groupNem = m_channelNum / 4;
	for (int i = 0; i < groupNem; i++)
	{
		bool bCheck = true;
		for (int j = 0; j < 4; j++)
		{
			if (!m_pCKCH[4 * i + j].IsCheck())
			{
				bCheck = false;
				break;
			}
		}

		if (bCheck)
		{
			m_pCKGroup[i].SetCheck(true, false);
		}
		else
		{
			m_pCKGroup[i].SetCheck(false, false);
		}
	}
}

void  CSearchGroupConditionDlg::SetAllCheckEnable(bool benable)
{
	for (int i = 0; i < m_channelNum; i++)
	{
		m_pCKCH[i].SetEnable(benable,false);
	}
	unsigned char groupNum = (m_channelNum + 3)/4;
	for (int i=0; i<groupNum; ++i)
	{
		m_pCKGroup[i].SetEnable(benable,false);
	}
}

void CSearchGroupConditionDlg::SetAllCheck(bool bcheck)
{
	for (int i = 0; i < m_channelNum; i++)
	{
		{
			m_pCKCH[i].SetCheck(bcheck,false);
		}
		
	}
	unsigned char groupNum = (m_channelNum + 3)/4;
	for (int i=0; i<groupNum; ++i)
	{
		m_pCKGroup[i].SetCheck(bcheck,false);
	}
}

void CSearchGroupConditionDlg::SetComboCurChannel(int curItem)
{
	string text;
	m_comboCtrlGroup.GetItemString(curItem,text);
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
//-------------------------------------------------

//end

