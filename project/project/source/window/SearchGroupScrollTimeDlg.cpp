#if defined(__GROUY_SEARCH__)
#include "SearchGroupScrollTimeDlg.h"
#include "NetProtocol.h"

CSearchGroupScrollTimeDlg::CSearchGroupScrollTimeDlg(): m_channelNum(0), m_authCH(0)
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

CSearchGroupScrollTimeDlg::~CSearchGroupScrollTimeDlg()
{

}

void CSearchGroupScrollTimeDlg::SetInitialInfo(unsigned long channelNum, bool bBackUp,ULONGLONG authCH)
{
	m_channelNum = channelNum;
	m_bBackUp  =  bBackUp;
	if(m_bBackUp)
	{
		m_authCH	= authCH;
	}
	else
	{
		m_authCH = ALL_CHNN_MASK(m_channelNum);
	}
}

void CSearchGroupScrollTimeDlg::OnInitial()
{
	assert(m_channelNum>0);
	m_cfgGroupList.clear();
	GUI::CScrollDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CRect rect;
	unsigned short xLeft = 10, yTop = 15;
	unsigned short x = xLeft, y = yTop;
	unsigned short xOff = 5, yOff = 5;
	unsigned short width = m_rect.Width() - 2 * xLeft;
	unsigned short rowH = m_length[TIME_CTRL_HEIGHT];

	GUI::CScrollWnd& wnd = GetScrollWnd();
	wnd.SetSizeOffset(m_rect.Width(), rowH * 1);
	wnd.SetViewMaxSize(m_rect.Width(), rowH * 20);

	rect.SetRect(x, y, x + 90, y + rowH);

	char str[128] = {0};
	GUI::CStatic* pStatic =dynamic_cast<GUI::CStatic*>(wnd.AddWnd(WND_TYPE_STATIC, GUI::CWnd::GetRandWndID(), rect, 0, 0, ID_START_STATIC));
	pStatic->SetCaption(m_pStrTable->String(E_STR_ID_START_TIME), false);
	x = xLeft + 94;
	rect. SetRect(x, y, x + m_length[DATE_CTRL_WIDTH], y + m_length[DATE_CTRL_HEIGHT]);
	GUI::CDateCtrl* pDate = dynamic_cast<GUI::CDateCtrl*>(wnd.AddWnd(WND_TYPE_DATE, GUI::CWnd::GetRandWndID(), rect, 0, 1, ID_DATE_START));
	
	y += m_length[DATE_CTRL_HEIGHT] + yOff;
	rect.SetRect(x, y, x + m_length[DATE_CTRL_WIDTH], y + m_length[TIME_CTRL_HEIGHT]);
	GUI::CTimeCtrl* pTime = dynamic_cast<GUI::CTimeCtrl*>(wnd.AddWnd(WND_TYPE_TIME, GUI::CWnd::GetRandWndID(), rect, 1, 1, ID_TIME_START));

	x = xLeft;
	y += rowH + yOff;
	rect.SetRect(x, y, x + 90, y + rowH);
	pStatic = dynamic_cast<GUI::CStatic*>(wnd.AddWnd(WND_TYPE_STATIC, GUI::CWnd::GetRandWndID(), rect, 0, 0, ID_END_STATIC));
	pStatic->SetCaption(m_pStrTable->String(E_STR_ID_END_TIME), false);

	x = xLeft + 94;
	rect.SetRect(x, y, x + m_length[DATE_CTRL_WIDTH], y + m_length[DATE_CTRL_HEIGHT]);
	pDate = dynamic_cast<GUI::CDateCtrl*>(wnd.AddWnd(WND_TYPE_DATE, GUI::CWnd::GetRandWndID(), rect, 0, 2, ID_DATE_END));

	y += m_length[DATE_CTRL_HEIGHT] + yOff;
	rect.SetRect(x, y, x + m_length[DATE_CTRL_WIDTH], y + m_length[DATE_CTRL_HEIGHT]);
	pTime = dynamic_cast<GUI::CTimeCtrl*>(wnd.AddWnd(WND_TYPE_TIME, GUI::CWnd::GetRandWndID(), rect, 1, 2, ID_TIME_END));

	x = xLeft;
	y += yOff + 8;
	unsigned short numChnn = m_channelNum;
	unsigned short numLocalChnn = CProduct::Instance()->LocalVideoInputNum();
	assert (numChnn >= 4);
	m_pCKCH = new GUI::CCheckCtrl *[numChnn];
	unsigned char groupNum = (m_channelNum + 3)/4;
	m_pCKGroup = new GUI::CCheckCtrl *[groupNum];
	
	int j = 0;
	unsigned short ckW = (width-m_length[CHECK_WIDTH]) / 4-5;
	unsigned short ckH = m_length[CHECK_HEIGHT]+2;
	for ( int i = 0; i < numChnn; i++ )
	{
		if( i % 4 == 0 )
		{
			x = xLeft;
			y += ckH;
			
			rect.SetRect(x, y, x + m_length[CHECK_WIDTH], y + ckH-2);
			m_pCKGroup[j] = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, CTRLID_SEARCH_CDN2_GROUP_0 + j, rect, 0, j + 4, ID_GROUP + j)); 
			j++;
			x = 40;
		}

		rect.SetRect(x, y, x + ckW, y + ckH);
		m_pCKCH[i] = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, CTRLID_SEARCH_CDN2_CH_0 + i, rect, (i % 4) + 1, j + 3, ID_CH + i));
		x += ckW;
		if(i < numLocalChnn)
		{
			sprintf( str, "%d", i + 1);
		}
		else
		{
			sprintf(str, "D%d", i + 1 - numLocalChnn);
		}
		m_pCKCH[i]->SetCaption( str, false, false );
		m_pCKCH[i]->SetEnable(false,false);
	}
	//////////////////////////////////////////////////////////////////////////
	x = m_length[DLG_OFFSET_X];
	int totalcount = (numChnn / 4)+4;
	y += ckH+15;
	m_pCheckGroup = NULL;
	rect.SetRect(x, y, x+m_length[CHECK_WIDTH], y+m_length[CHECK_HEIGHT]);
	m_pCheckGroup = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, GUI::CWnd::GetRandWndID(), rect, totalcount, totalcount, ID_GROUP_CHECK));
	m_pCheckGroup->SetCheck(true,false);

	x += m_length[CHECK_WIDTH] + 3;
	pStatic =NULL;
	rect.SetRect(x,y, x+m_length[TITLE_EXIT_BTN_RIGHT_XPOS], y+m_length[STATIC_HEIGHT]);
	pStatic = dynamic_cast<GUI::CStatic*>(wnd.AddWnd(WND_TYPE_STATIC, GUI::CWnd::GetRandWndID(), rect, totalcount, totalcount+1, ID_GROUP_STATIC));
	pStatic->SetCaption(m_pStrTable->String(E_STR_ID_SEARCH_GROUP),false);
	pStatic = NULL;

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

void CSearchGroupScrollTimeDlg::OnPostInitial()
{
	GUI::CScrollDialog::OnPostInitial();
	LoadGroupItemCfg();
}

int CSearchGroupScrollTimeDlg::LoadGroupItemCfg()
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

int CSearchGroupScrollTimeDlg::SaveGroupItemCfg()
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

	//����ռ�
	GetItemList(cfgItemList);
	if (0 == cfgItemList.size())
	{
		return -1;
	}
	dataLen = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * cfgItemList.size() +  sizeof(NCFG_INFO_GROUP_ENABLEINFO)+sizeof(NCFG_INFO_GROUP_ITEM)*MAX_GROUP_ITEM_COUNT;
	pData = new BYTE[dataLen+1];
	memset(pData, 0, dataLen+1);

	//��װͷ��Ϣ
	NCFG_BLOCK_HEAD *pblockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(pData);
	pblockHead->netcfgver = NCFG_VERSION;
	pblockHead->biSize = sizeof(NCFG_BLOCK_HEAD);
	pblockHead->ItemNum = cfgItemList.size();
	//��ȡÿһ�������Ϣ
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

	//��������
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

	//������Ϣ
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_SET, pData, dataLen, msgData);
	delete [] pData;
	pData = NULL;
	pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC != static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{	

	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	//�˳�����
	CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);


	return 0;	

}

void CSearchGroupScrollTimeDlg::GetItemList(std::vector<NCFG_ITEM_ID>& itemList) const
{
	itemList.push_back(NCFG_ITEM_SEARCH_GROUP_ENABLE);
	itemList.push_back(NCFG_ITEM_SEARCH_GROUP_ITEM);

}

bool CSearchGroupScrollTimeDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
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
				//��group��ʽ
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
				//group��ʽ�������Ա���ѡ��ֻ�ܲ鿴
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
			////�޸ĺ������check������Ϊ������ʹ��
			SetAllCheckEnable(true);
			m_cfgGroupList.clear();
			m_pComboCtrl->RemoveAllItem(false);
			for (int i = 0; i < MAX_GROUP_ITEM_COUNT; i++)
			{
				if(strlen(pGroupItem[i].szItem)!=0)
				{
					m_cfgGroupList.push_back(pGroupItem[i]);
					m_pComboCtrl->AddItem(pGroupItem[i].szItem);
					printf("%s,%d szitem=%s,chanbits=%x\n",__FILE__,__LINE__,pGroupItem[i].szItem,pGroupItem[i].chanbits);
				}
			}

			if(m_enable == 1)
			{
				if(m_cfgGroupList.size()==0)
				{
					//˵��û���κ����ã�ֻ�Ǵ��˹�������ˢ��û��ѡ������
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
					//��Ӱ�ť�û�
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

bool CSearchGroupScrollTimeDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, unsigned long& len)  const
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
			//��pGroupItemˢ�գ�����m_cfgGroupList��ֵ
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


void CSearchGroupScrollTimeDlg::OnDestroy()
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

void CSearchGroupScrollTimeDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CScrollDialog::OnPaint(status);
	int cx, cy;
	GetCurScrollPos(cx, cy);

	unsigned short borderWidth = 23;
	unsigned short xLeft = 10, yTop = 15;
	unsigned short x = xLeft, y = yTop;
	unsigned short xOff = 5, yOff = 5;
	unsigned short width = m_rect.Width() - 2 * xLeft;
	unsigned short rowH = m_length[TIME_CTRL_HEIGHT];

	unsigned short rcLeft = m_rect.m_left + xLeft - xOff;
	unsigned short rcTop = m_rect.m_top + yTop - yOff - cy * rowH;
	unsigned short rcWidth = width + 2 * xOff - borderWidth;

	unsigned short tmH = 4 * rowH + 4 * yOff + 4;
	//// �ϲ���
	{
		x = rcLeft;
		y = rcTop;

		GUI::CWnd::DrawLine(x, y, x, y + tmH, COLOR_LINE_GRAY);//����
		GUI::CWnd::DrawLine(x, y, x + rcWidth, y, COLOR_LINE_GRAY);//�Ϻ�
		GUI::CWnd::DrawLine(x + rcWidth, y, x + rcWidth, y + tmH, COLOR_LINE_GRAY); //����
		GUI::CWnd::DrawLine(x, y + tmH, x + rcWidth, y + tmH, COLOR_LINE_GRAY);//�º�

	}

	unsigned short numChnn = m_channelNum;
	unsigned short numRow = numChnn / 4;
	if( numChnn % 4 != 0 ) numRow += 1;
	unsigned short ckH = m_length[CHECK_HEIGHT] + 2;
	rcLeft = m_rect.m_left + xLeft - xOff;
	rcTop = m_rect.m_top + yTop + 4 * rowH + 5 * yOff - cy * rowH;
	rcWidth = width + 2 * xOff - borderWidth; 
	//// �²���
	{
		x = rcLeft;
		y = rcTop;
		unsigned short chnnH = numRow * ckH + 2;

		GUI::CWnd::DrawLine(x, y, x, y + chnnH, COLOR_LINE_GRAY ); // ����
		GUI::CWnd::DrawLine(x, y, x + rcWidth, y, COLOR_LINE_GRAY);//�Ϻ�
		GUI::CWnd::DrawLine(x + rcWidth, y, x + rcWidth, y + chnnH, COLOR_LINE_GRAY); //����
		GUI::CWnd::DrawLine(x, y + chnnH, x + rcWidth, y + chnnH, COLOR_LINE_GRAY);//�º�

		x = m_rect.m_left + m_length[CHECK_WIDTH] + 12;
		GUI::CWnd::DrawLine(x, y, x, y + chnnH, COLOR_LINE_GRAY ); // ����


		x = rcLeft;
		// �к���
		for ( int i = 1; i < numRow; i ++ )
		{
			y += ckH;
			GUI::CWnd::DrawLine(x, y + 2, x + rcWidth, y + 2, COLOR_LINE_GRAY);//�Ϻ�
		}
	}

}

unsigned long CSearchGroupScrollTimeDlg::TimeStart()
{
	tm tmpTm;
	memset(&tmpTm,0,sizeof(tm));
	GUI::CDateCtrl* pDateStart = dynamic_cast<GUI::CDateCtrl*>(GetScrollWnd().GetWnd(ID_DATE_START));
	GUI::CTimeCtrl* pTimeStart = dynamic_cast<GUI::CTimeCtrl*>(GetScrollWnd().GetWnd(ID_TIME_START));
	pDateStart->GetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday);
	pTimeStart->GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);

	tmpTm.tm_year -= 1900;

	return TmToDVRTime32(tmpTm);
}

unsigned long CSearchGroupScrollTimeDlg::TimeEnd()
{
	tm tmpTm;
	memset(&tmpTm,0,sizeof(tm));
	GUI::CDateCtrl* pDateEnd = dynamic_cast<GUI::CDateCtrl*>(GetScrollWnd().GetWnd(ID_DATE_END));
	GUI::CTimeCtrl* pTimeEnd = dynamic_cast<GUI::CTimeCtrl*>(GetScrollWnd().GetWnd(ID_TIME_END));
	pDateEnd->GetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday);
	pTimeEnd->GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);

	tmpTm.tm_year -= 1900;

	return TmToDVRTime32(tmpTm);
}

ULONGLONG CSearchGroupScrollTimeDlg::CH()
{
	GUI::CCheckCtrl* pCKChnn;
	ULONGLONG bit = 0;
	for ( int i=0; i< m_channelNum; i++ )
	{
		pCKChnn = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CH + i));
		if(pCKChnn->IsCheck() )
		{
			bit |= (ULONGLONG)0x1 << i;
		}
	}
	return bit;
}

void CSearchGroupScrollTimeDlg::SetStartTime(tm time)
{
	GUI::CDateCtrl* pDateStart = dynamic_cast<GUI::CDateCtrl*>(GetScrollWnd().GetWnd(ID_DATE_START));
	GUI::CTimeCtrl* pTimeStart = dynamic_cast<GUI::CTimeCtrl*>(GetScrollWnd().GetWnd(ID_TIME_START));
	pDateStart->SetDate(time.tm_year+1900,time.tm_mon,time.tm_mday,false);
	pTimeStart->SetTime(time.tm_hour,time.tm_min,time.tm_sec,false);
}

void CSearchGroupScrollTimeDlg::SetEndTime(tm time)
{
	GUI::CDateCtrl* pDateEnd = dynamic_cast<GUI::CDateCtrl*>(GetScrollWnd().GetWnd(ID_DATE_END));
	GUI::CTimeCtrl* pTimeEnd = dynamic_cast<GUI::CTimeCtrl*>(GetScrollWnd().GetWnd(ID_TIME_END));
	pDateEnd->SetDate(time.tm_year+1900,time.tm_mon,time.tm_mday,false);
	pTimeEnd->SetTime(time.tm_hour,time.tm_min,time.tm_sec,false);
}

BEGIN_MSG_MAP(CSearchGroupScrollTimeDlg, GUI::CScrollDialog)
	ON_MSG_EX(GetScrollWnd().GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickScrollWnd)
END_MSG_MAP()

unsigned long CSearchGroupScrollTimeDlg::OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
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

unsigned long CSearchGroupScrollTimeDlg::OnClickCheckGroup()
{
	if (m_pCheckGroup->IsCheck())
	{
		m_pCheckGroup->SetCheck(true, false);
		m_pComboCtrl->SetEnable(true,false);
		m_pBtnAdd->SetEnable(true,false);
		m_pBtnDel->SetEnable(true,false);
		m_pBtnEdit->SetEnable(true,false);
		
		//����з��飬ˢ��comboҳ�浱ǰֵ������
		SetAllCheckEnable(true);
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
		if (0 >= m_cfgGroupList.size())
		{
			m_pBtnDel->SetEnable(false,false);
			m_pBtnEdit->SetEnable(false,false);
		}
		//�������е�ͨ��check��
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

unsigned long CSearchGroupScrollTimeDlg::OnClickComboGroup()
{
	//enable���е�ͨ��check��
	SetAllCheckEnable(true);

	//�Ѷ�Ӧ��check����Ϣˢ����Ļ
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
	//�������е�ͨ��check��
	SetAllCheckEnable(false);
	GetRootWnd()->Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupScrollTimeDlg::OnClickBtnAdd()
{
	m_DlgAddGroup.SetInitialInfo(m_channelNum,m_authCH,&m_cfgGroupList);
	m_DlgAddGroup.GetEditEnable(false,"",0);
	if (DLG_OK == m_DlgAddGroup.DoModal(this))
	{		
		m_pComboCtrl->RemoveAllItem(false);
		for(GROUP_ITEM_LIST::iterator it=m_cfgGroupList.begin();it!=m_cfgGroupList.end();it++)
		{
			//printf("Add szitem=%s,chanbits=%x\n",it->szItem,it->chanbits);
			m_pComboCtrl->AddItem(it->szItem);
		}	
		int curItem=m_DlgAddGroup.SendCurnItem();
		m_pComboCtrl->SetCurItem(curItem,false);//�޸�combo��ǰֵΪ�¼ӵķ������
		m_curItem = curItem;
		SetAllCheckEnable(true);
		SetComboCurChannel(curItem);
		SetAllCheckEnable(false);
		if (MAX_GROUP_ITEM_COUNT-1 <= m_cfgGroupList.size())
		{
			m_pBtnAdd->SetEnable(false,false);
		}
		m_bModify = true;
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

unsigned long CSearchGroupScrollTimeDlg::OnClickBtnDel()
{
	if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_SEARCH_DEL_GROUP), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING))
	{
		//������ֱ�ӷ���
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
			if(nCurItem == 0)//combo��ֻ��һ��
			{
				m_enable=0;
				//���пؼ���ʧ,check������Ϊ����ѡ
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
			else//��ǰֵ�����һ��
			{
				//��ǰֵΪcombo�ĵ�һ��Ŀ
				m_pComboCtrl->GetItemString(nCurItem,text);
				m_pComboCtrl->GetItemString(0,nextText);
				//��������Ѱ��ƥ����ַ���
				SetAllCheckEnable(true);
				m_pComboCtrl->RemoveAllItem(false);
				m_cfgGroupList.pop_back();//�������һ��
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

unsigned long CSearchGroupScrollTimeDlg::OnClickBtnEdit()
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
			m_pComboCtrl->SetCurItem(curItem,false);//�޸�combo��ǰֵΪ�¼ӵķ������
			SetAllCheckEnable(true);
			SetComboCurChannel(curItem);
			SetAllCheckEnable(false);
		}
		GetRootWnd()->Repaint();
		m_bModify=true;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupScrollTimeDlg::OnClickChnn( unsigned long ctrlID )
{
	DoClickChnn( ctrlID - ID_CH , false );
	GetRootWnd()->Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchGroupScrollTimeDlg::OnClickGroup( unsigned long ctrlID )
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

unsigned long CSearchGroupScrollTimeDlg::DoClickChnn( unsigned long index, bool bRefresh )
{
	int groupNum = index / 4;
	GUI::CCheckCtrl* pCKCH=NULL;

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

ULONGLONG CSearchGroupScrollTimeDlg::GetCurChannel() const
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

void CSearchGroupScrollTimeDlg::SetChannel(const ULONGLONG chanbits)
{
	//���þ���С��
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

	//����4��С����ɵ�����
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

void  CSearchGroupScrollTimeDlg::SetAllCheckEnable(bool benable)
{
	for (int i = 0; i < m_channelNum; i++)
	{
		if (m_authCH & ((ULONGLONG)(0x01) << i))
		{
			m_pCKCH[i]->SetEnable(benable,false);
		}
	}
	unsigned char groupNum = (m_channelNum + 3)/4;
	for (int i=0; i<groupNum; ++i)
	{
		m_pCKGroup[i]->SetEnable(benable,false);
	}
}

void CSearchGroupScrollTimeDlg::SetAllCheck(bool bcheck)
{
	
	for (int i = 0; i < m_channelNum; i++)
	{
		if(m_bBackUp)
		{
			if (m_authCH & ((ULONGLONG)(0x01) << i))
			{
				m_pCKCH[i]->SetCheck(bcheck,false);
			}
		}
		else
		{
			m_pCKCH[i]->SetCheck(bcheck,false);
		}
		
	}

	unsigned char groupNum = (m_channelNum + 3)/4;
	for (int i=0; i<groupNum; ++i)
	{
		m_pCKGroup[i]->SetCheck(bcheck,false);
	}
}

void CSearchGroupScrollTimeDlg::SetComboCurChannel(int curItem)
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

