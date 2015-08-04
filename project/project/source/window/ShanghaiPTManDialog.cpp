#include "ShanghaiPTManDialog.h"
#include "MessageMan.h"
CShanghaiPTManDialog::CShanghaiPTManDialog()
{
	m_codeTable = CCharCodeTable::Instance();
}

CShanghaiPTManDialog::~CShanghaiPTManDialog()
{

}

void CShanghaiPTManDialog::OnInitial()
{
	CCfgDlg::OnInitial();
	
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DEFAULT) );
	std::string outStr;
	outStr.clear();
	outStr = ChangeChinese("平台管理");
	SetCaption(outStr,false);
	m_initPageMask= 0x1;

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
	m_PTNETCfgDialog.Create(GUI::CWnd::GetRandWndID(),x,y,cx,cy, this,0,2);
	
	outStr.clear();
	outStr = ChangeChinese("接入平台");
	m_tabCtrl.AddItem(&m_PTNETCfgDialog, outStr.c_str());//接入平台
	m_tabCtrl.AddItem(&m_cfgRecordSnapDlg, m_pStrTable->String(E_STR_ID_SNAP));
	outStr.clear();
	outStr = ChangeChinese("日常图像");
	m_tabCtrl.AddItem(&m_shanghaiDailyImageDialog, outStr.c_str());
	outStr.clear();
	outStr = ChangeChinese("报警图像");
	m_tabCtrl.AddItem(&m_shanghaiAlarmimageDialog, outStr.c_str());

	m_curPage = PAGE_NET;
	m_tabCtrl.SetCurItem(m_curPage , false );
	CCfgDlg::SetRefreshView( false );
	CreateTipWnd();
	printf("%s,%d\n",__FILE__,__LINE__);
	bool bEnterCfg=EnterConfig();
	printf("%s,%d\n",__FILE__,__LINE__);
}

void CShanghaiPTManDialog::OnPostInitial()
{
	CCfgDlg::OnPostInitial();
	
}

void CShanghaiPTManDialog::OnDestroy()
{
	CCfgDlg::OnDestroy();
	ExitConfig();
}

void CShanghaiPTManDialog::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_JPEG_PERIOD);
	itemList.push_back(NCFG_ITEM_JPEG_SIZE);
	itemList.push_back(NCFG_ITEM_JPEG_QUALITY);
	itemList.push_back(NCFG_ITEM_JPEG_SNAP_NUM);
	itemList.push_back(NCFG_ITEM_ENCODE_MAJOR_SPEC);
	itemList.push_back(NCFG_ITEM_NET_SHANGHAI_DAILY_IMAGE);
	itemList.push_back(NCFG_ITEM_NET_SHANGHAI_PRE_ALARM_SNAP_TIME);
	itemList.push_back(NCFG_ITEM_NET_SHANGHAI_POST_ALARM_SNAP_TIME);
	itemList.push_back(NCFG_ITEM_NET_SHANGHAI_MOTIONUPLOAD_ENABLE);
	itemList.push_back(NCFG_ITEM_NET_SHANGHAI_PTENABLE);
	itemList.push_back(NCFG_ITEM_NET_SHANGHAI_AGENTID);
	itemList.push_back(NCFG_ITEM_NET_SHANGHAI_IP);
	itemList.push_back(NCFG_ITEM_NET_SHANGHAI_PORT);
	//下面是只读选项
	itemList.push_back(NCFG_ITEM_SNAP_SIZE_RANGE);
	itemList.push_back(NCFG_ITEM_SNAP_QULITY_LEVELNUM);
	itemList.push_back(NCFG_ITEM_SNAP_IMG_NUM_RANGE);
	itemList.push_back(NCFG_ITEM_SNAP_TIME_RANGE);
}

CCfgDlg* CShanghaiPTManDialog::GetPage(PAGE_ITEM item)
{

	CCfgDlg* pCfgDlg = NULL;
	switch (item)
	{
	case PAGE_NET:
		pCfgDlg = &m_PTNETCfgDialog;
		break;
	case PAGE_SNAPCFG:
		pCfgDlg = &m_cfgRecordSnapDlg;
		break;
	case PAGE_DAILYIMAGE:
		pCfgDlg =&m_shanghaiDailyImageDialog;
		break;
	case PAGE_ALARMIMAGE:
		pCfgDlg =&m_shanghaiAlarmimageDialog;
		break;
	default:
		assert(false);
		break;
	}
	return pCfgDlg;
}

unsigned long CShanghaiPTManDialog::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((ctrlID == m_tabCtrl.GetID()) && (KEY_VIRTUAL_ENTER == keyID))
	{		
		{
			unsigned short x = m_length[DLG_OFFSET_LEFT];
			unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
			unsigned short cx = m_rect.Width() - ( x * 2 );
			unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

			if(PAGE_DAILYIMAGE == m_tabCtrl.GetFocusItem())
			{
				GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
				if(NULL != pBtn)
				{
					pBtn->SetEnable(false,true);
				}
			}
			else
			{
				GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
				if(NULL != pBtn)
				{
					pBtn->SetEnable(true,true);
				}
			}

			if(!((m_initPageMask >> PAGE_SNAPCFG) & 0x1) && (PAGE_SNAPCFG == m_tabCtrl.GetFocusItem()))
			{
				m_cfgRecordSnapDlg.SetShowSnapNum(false);
				m_cfgRecordSnapDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, PAGE_SNAPCFG + 2);
				SetSubCfgDlg(&m_cfgRecordSnapDlg);
				m_initPageMask |= (0x1 << PAGE_SNAPCFG);

			}
			if(!((m_initPageMask >> PAGE_DAILYIMAGE) & 0x1) && (PAGE_DAILYIMAGE == m_tabCtrl.GetFocusItem()))
			{
				m_shanghaiDailyImageDialog.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, PAGE_DAILYIMAGE + 2);
				SetSubCfgDlg(&m_shanghaiDailyImageDialog);
				m_initPageMask |= (0x1 << PAGE_DAILYIMAGE);
			}
			if(!((m_initPageMask >> PAGE_ALARMIMAGE) & 0x1) && (PAGE_ALARMIMAGE == m_tabCtrl.GetFocusItem()))
			{
				m_shanghaiAlarmimageDialog.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, PAGE_ALARMIMAGE + 2);
				SetSubCfgDlg(&m_shanghaiAlarmimageDialog);
				m_initPageMask |= (0x1 << PAGE_ALARMIMAGE);
			}
		}

		if (m_curPage != m_tabCtrl.GetFocusItem())
		{
			unsigned long ret = 0;
			if (GetPage(m_curPage)->IsModify())
			{
				ret = TrySaveChange(GetPage(m_curPage),true,false,false);
			}

			if ( ret == 0 )
			{
				m_curPage = static_cast<PAGE_ITEM>(m_tabCtrl.GetFocusItem());
			}
			else
			{
				Repaint();

				return KEY_VIRTUAL_MSG_STOP;
			}
		}	
	}

	return keyID;
}

unsigned long CShanghaiPTManDialog::OnApply()
{
	//Load default
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);
	unsigned long ret = OnLoadDefault(pCfgDlg);

	if( pCfgDlg->IsModify() )
	{
		EnableOkBtn(true,false);
	}
	else
	{
		EnableOkBtn(false,false);
	}

	ENUM_STRING_ID strID = ret ? static_cast<ENUM_STRING_ID>(ret) : E_STR_ID_TIP_00_03;
	MessageOut(strID,TIP_MSG,false);

	//一次重绘全部
	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CShanghaiPTManDialog::CheckValue()
{
	CCfgDlg* pCfgDlg = GetPage(m_curPage);
	assert(pCfgDlg);

	return pCfgDlg->CheckValue();
}

bool CShanghaiPTManDialog::EnterConfig()
{
	//进入配置
	CMessageMan* pMsgMan = CMessageMan::Instance();

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_ENTER, NULL, 0, msgData);

	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		pMsgMan->ReleaseMsgBuff( msgData );
		return true;
	}
	else
	{
		pMsgMan->ReleaseMsgBuff( msgData );
		GUI::CDialog::MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_26), m_pStrTable->String(E_STR_ID_TITLE_FAILED), DEFAULT_BTN_OK|DEFAULT_ICON_ERROR);
		return false;
	}
}

void CShanghaiPTManDialog::ExitConfig() const
{
	//退出配置
	CMessageMan* pMsgMan = CMessageMan::Instance();
	pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);
}


BEGIN_MSG_MAP(CShanghaiPTManDialog,CCfgDlg)
	ON_MSG(m_PTNETCfgDialog.GetID(), KEY_VIRTUAL_ENTER, OnClickNET)
	ON_MSG(m_cfgRecordSnapDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickRecordSnap);
	ON_MSG(m_shanghaiDailyImageDialog.GetID(),KEY_VIRTUAL_ENTER, OnClickDailyImage);
	ON_MSG(m_shanghaiAlarmimageDialog.GetID(),KEY_VIRTUAL_ENTER, OnClickAlarmImage);
END_MSG_MAP()

unsigned long CShanghaiPTManDialog::OnClickNET()
{
	if (m_PTNETCfgDialog.IsModify())
	{
		EnableOkBtn(true, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CShanghaiPTManDialog::OnClickRecordSnap()
{
	if(m_cfgRecordSnapDlg.IsModify())
	{
		EnableOkBtn(true, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CShanghaiPTManDialog::OnClickDailyImage()
{
	if(m_shanghaiDailyImageDialog.IsModify())
	{
		EnableOkBtn(true, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CShanghaiPTManDialog::OnClickAlarmImage()
{
	if(m_shanghaiAlarmimageDialog.IsModify())
	{
		EnableOkBtn(true, true);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

string CShanghaiPTManDialog::ChangeChinese(const char *pstr)
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

//end
