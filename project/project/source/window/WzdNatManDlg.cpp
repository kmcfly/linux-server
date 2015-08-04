/**********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2013-11-25
** Name         : WzdNatManDlg.cpp
** Version      : 1.0
** Description  : 穿透向导管理对话框
** Modify Record:
**********************************************************************/

#include "WzdNatManDlg.h"
#include "WzdNatStartDlg.h"
#include "WzdNatBasicManDlg.h"
#include "WzdNatPassWordDlg.h"
#include "WzdNatGetNetWorkSetUpDlg.h"
#include "WzdNatLocalEndDlg.h"
#include "WzdNatSelectDevDlg.h"
#include "WzdNatQrCodeDlg.h"
#include "WzdNatPCTipDlg.h"
#include "WzdNatEndDlg.h"
#include "CtrlID.h"
#include "MessageMan.h"
#include "MessageBox.h"
extern bool	g_bCloseWzd;

CWzdNatManDlg::CWzdNatManDlg() 
{
	m_curTime = 0;
	m_lastTime = 0;
	m_pNatWzdInfo = NULL;
	m_curIndex =0;
	m_maxTime = 10*60*1000000;
	m_devSelect = DEV_ANDROID;
	
}

CWzdNatManDlg::~CWzdNatManDlg()
{

}

bool CWzdNatManDlg::TryEnterConfig()
{
	if( !m_bEnterConfig )
	{
		m_bEnterConfig = EnterConfig();
	}

	return m_bEnterConfig;
}

void CWzdNatManDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////
	CCfgDlg::OnInitial();

	if( GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 )
	{
		SetImage(BMP_MEM_DLG_LARGE, false);
	}
	else
	{
		SetImage(BMP_MEM_DLG_LARGE, false);
	}

	SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_STARTUP), false);
	SetDefaultBtn( DEFAULT_BTN_OK |  DEFAULT_BTN_CANCEL |DEFAULT_BTN_APPLY);
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_NEXT) );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_PREV) );
	CreateTipWnd();
	//初始化向导数据
	CreateWizardInfo();
	//创建向导页面
	CreateNatWizardPage();
	//初始第一页面
	m_curIndex = 0;
	//进配置,当远程IE先于本地向导进配置时,将无法保存配置数据.
	//此时需要提示用户关闭远程配置方可进行设置
	m_bEnterConfig = false;
	bool bret=TryEnterConfig();

}

void CWzdNatManDlg::OnPostInitial()
{
	CCfgDlg::OnPostInitial();

	m_lastTime = GetCurrTime();
	//初始化向导内容
	InitWizardInfo();
	//初始化所有页面控件初始值
	//主要是依赖于m_pWizardInfo,而在OnInitial无法完的部分控件
	for (int i = 0; i < m_vecWzdPage.size(); i++)
	{
		m_vecWzdPage[i]->InitCtrlInfo();
	}
	//初始化当前需要显示页面值
	m_vecWzdPage[m_curIndex]->ShowWzdData(false);
	//初始显示页面
	ShowPage(m_curIndex);

	//显示按钮
	ShowBtn(m_curIndex);
}

void CWzdNatManDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	if( m_pNatWzdInfo )
	{
		delete m_pNatWzdInfo;
		m_pNatWzdInfo = NULL;
	}

	for ( int i = 0; i < m_vecWzdPage.size(); i++ )
	{
		if( m_vecWzdPage[i] )
		{
			delete m_vecWzdPage[i];
			m_vecWzdPage[i] = NULL;
		}
	}
	m_vecWzdPage.clear();
	if( m_bEnterConfig )
	{
		ExitConfig();
		m_bEnterConfig = false;
	}

}

void CWzdNatManDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);
}

void CWzdNatManDlg::EndDialog(DLG_END_VALUE result)
{

}

unsigned long CWzdNatManDlg::OnOK()//下一页
{
	switch(m_curIndex)
	{
	case PAGE_NETWORK:
		{
			GUI::CButton *pButton = NULL;
			pButton =  dynamic_cast<GUI::CButton*>(GetTopestWnd()->GetCtrl(CTRLID_DEFAULT_BTN_OK));
			if(false == pButton->IsEnable())//如果截获下一个按钮不时能状态，不可以往下翻页
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
		}
		break;
	case PAGE_DEVSELECT:
		{
			CWzdNatSelectDevDlg* pWzdDevSelectDlg = dynamic_cast<CWzdNatSelectDevDlg*>(m_vecWzdPage[PAGE_DEVSELECT]);
			assert(pWzdDevSelectDlg);
			m_devSelect = pWzdDevSelectDlg->GetDevSelectIndex();
		}
		break;

	case PAGE_NATPASS:
		{
			SetModify(true);
			if (reinterpret_cast<CWzdNatPassWordDlg *>(m_vecWzdPage[PAGE_NATPASS])->CheckNewPasswd())
			{
				m_vecWzdPage[PAGE_NATPASS]->GetWzdData(true);
				m_pNatWzdInfo->Modified = 1;
				if(0 != m_pNatWzdInfo->Modified)
				{

					if( TryEnterConfig() )
					{
						//完成生效处理

						/*OnWzdEnd();*/
						CCfgDlg::OnOK();
						printf("CCfgDlg::OnOk()\n");

					}
				}
			}else{
				return KEY_VIRTUAL_MSG_STOP;
			}
		}
		break;
	default:
		break;
	}

	//下一页
	if( m_curIndex +1 < m_vecWzdPage.size() )
	{
		unsigned long strID = CheckValue();
		if(0 != strID)
		{
			MessageOut( GetTopestWnd(),static_cast<ENUM_STRING_ID>(strID), TIP_ERR, false );
			Repaint();
			return KEY_VIRTUAL_MSG_STOP;
		}
		m_vecWzdPage[m_curIndex]->OnLeavePage();
		
		if(PAGE_NETWORK == m_curIndex)
		{
			//PAGE_NETWORK的下一页根据m_pNatWzdInfo,选择不同页面
			if(m_pNatWzdInfo->natInfo.bStartNat)
			{
				m_curIndex += 1; //如果选择了本地结束，就进入本地结束页面
			}
		}
		else if(PAGE_LOCALEND == m_curIndex)//本地结束的下一步直接关闭向导
		{
			SaveNatWizard();
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if(PAGE_DEVSELECT == m_curIndex)
		{
			switch(m_devSelect)
			{
				case DEV_ANDROID:
				case DEV_APPLE:
					break;
				case DEV_PC:
					{
						m_curIndex += 1;//pC显示在下一页
					}
					break;
				default:
					break;
			}
		}
		else if(PAGE_QRCODE == m_curIndex)
		{
			m_curIndex += 1;//跳过PC显示页面
		}
		else
		{

		}
		
		m_curIndex += 1;
		if(PAGE_QRCODE == m_curIndex)
		{
			//二维码页面需要设置设备代号
			CWzdNatQrCodeDlg* pWzdQrCodeDlg = dynamic_cast<CWzdNatQrCodeDlg*>(m_vecWzdPage[m_curIndex]);
			pWzdQrCodeDlg->SetDevIndex(m_devSelect);
			pWzdQrCodeDlg = NULL;
		}
		
		ShowPage(m_curIndex);
		ShowBtn(m_curIndex);
		m_vecWzdPage[m_curIndex]->ShowWzdData(false);
		Repaint();
	}
	else
	{

		//最后页面读取
		SaveNatWizard();
		m_curIndex += 1;
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CWzdNatManDlg::OnCancel()
{
	SaveNatWizard();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CWzdNatManDlg::OnApply()
{
	//上一页
	if( m_curIndex -1 >= 0 )
	{
		if(PAGE_LOCALEND == m_curIndex)//如果是结束，必须返回检查ip页面
		{
			
		}
		else if(PAGE_LOCALEND == m_curIndex-1)//如果前一页是结束，返回修改密码页面，因为结束页面没有下一页
		{
			m_curIndex -= 1;
		}
		else if(PAGE_PCTIP == m_curIndex)
		{
			m_curIndex -= 1;//当前页面是pc显示页面，回的设备选择页面
		}
		else
		{

		}
		m_curIndex -= 1;
		ShowPage(m_curIndex);
		ShowBtn(m_curIndex);
		m_vecWzdPage[m_curIndex]->ShowWzdData(false);

		Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CWzdNatManDlg::SaveNatWizard()
{
	//提取数据
	for(int i = 0; i < m_vecWzdPage.size(); i++)
	{
		m_vecWzdPage[i]->GetWzdData(true);
		m_pNatWzdInfo->Modified = 1;
	}
	if(0 != m_pNatWzdInfo->Modified)
	{

		if( TryEnterConfig() )
		{
			//完成生效处理

			OnWzdEnd();

			CDialog::EndDialog(DLG_OK);
		}
		else
		{
			//提示IE客户端退出配置,是否继续或放弃
			std::string str = m_pStrTable->String(E_STR_ID_MESSAGE_104);
			str.append("\\n");
			str.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_17));

			GUI::CMessageBox box(str.c_str(), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING);
			if( DLG_OK == box.DoModal(this) )
			{
				//继续等待
			}
			else
			{
				//放弃
				CDialog::EndDialog(DLG_OK);
			}
		}
	}
	else
	{
		//没有修改
		CDialog::EndDialog(DLG_OK);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

void CWzdNatManDlg::OnIdle()
{
	if(1 == m_pNatWzdInfo->UpDateWzdTime)
	{
		m_lastTime = GetCurrTime();
		m_curTime = m_lastTime;

		m_pNatWzdInfo->UpDateWzdTime = 0;
	}
	else
	{
		m_curTime = GetCurrTime();
	}


	if (((m_lastTime > 0) && ((m_curTime - m_lastTime) > m_maxTime)) || (g_bCloseWzd == true))
	{
		SaveNatWizard();
	}
	GUI::CDialog::OnIdle();
}

unsigned long CWzdNatManDlg::CheckValue()
{
	return m_vecWzdPage[m_curIndex]->CheckValue();
}

void CWzdNatManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	//时间和DST页面所需要的配置
	itemList.push_back(NCFG_ITEM_DATE_FORMAT);
	itemList.push_back(NCFG_ITEM_HOUR_FORMAT);
	itemList.push_back(NCFG_ITEM_LANGUAGE);
	itemList.push_back(NCFG_ITEM_TIME_ZONE);
	itemList.push_back(NCFG_ITEM_TIME_SYNC);
	itemList.push_back(NCFG_ITEM_TIME_SERVER);

	itemList.push_back(NCFG_ITEM_USER);
	itemList.push_back(NCFG_ITEM_DAYLIGHT_INFO);
	itemList.push_back(NCFG_ITEM_SHOW_WIZZARD);

	itemList.push_back(NCFG_ITEM_HTTP_PORT);
	itemList.push_back(NCFG_ITEM_SERVER_PORT);
	itemList.push_back(NCFG_ITEM_NAT_SERVER);
	itemList.push_back(NCFG_ITEM_SUPPORT_LANGUAGE);
	itemList.push_back(NCFG_ITEM_DEVUSER_NAME_MAX_LEN);
	itemList.push_back(NCFG_ITEM_DEVUSER_PWD_MAX_LEN);
	
}

bool CWzdNatManDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);
	switch (cfgID)
	{

	case NCFG_ITEM_DATE_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pNatWzdInfo->DateMode;
		}
		return true;
		break;
	case NCFG_ITEM_HOUR_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue =m_pNatWzdInfo->TimeMode;
		}
		return true;
		break;
	case NCFG_ITEM_LANGUAGE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue =m_pNatWzdInfo->CurrentLanguage;
		}
		return true;
		break;
	case NCFG_ITEM_TIME_ZONE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pNatWzdInfo->TimeZone;
		}
		return true;
		break;
	case NCFG_ITEM_TIME_SYNC:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pNatWzdInfo->sync;
		}
		return true;
		break;
	case NCFG_ITEM_TIME_SERVER:
		{
			assert(len == sizeof(NCFG_INFO_LONG_NAME));
			NCFG_INFO_LONG_NAME * pValue = reinterpret_cast<NCFG_INFO_LONG_NAME*>(pData);
			memcpy(pValue,&(m_pNatWzdInfo->timeServer),sizeof(NCFG_INFO_LONG_NAME));
		}
		return true;
		break;
	case NCFG_ITEM_USER:
		{
			assert(len == sizeof(NCFG_INFO_USER)*MAX_USER_NUM);
			NCFG_INFO_USER *pUserInfo = reinterpret_cast<NCFG_INFO_USER*>(pData);
			memcpy(pUserInfo,&(m_pNatWzdInfo->userInfo),sizeof(NCFG_INFO_USER)*MAX_USER_NUM);
		}
		return true;
		break;
	case NCFG_ITEM_DAYLIGHT_INFO:
		{
			assert(len == sizeof(DAYLIGHT_INFO));
			DAYLIGHT_INFO * pValue = reinterpret_cast<DAYLIGHT_INFO*>(pData);
			*pValue = m_pNatWzdInfo->dstInfo;
		}
		return true;
		break;
	case NCFG_ITEM_SHOW_WIZZARD:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pNatWzdInfo->ShowWzd;
		}
		return true;
		break;
	case NCFG_ITEM_HTTP_PORT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pNatWzdInfo->httpPort;
		}
		return true;
		break;
	case NCFG_ITEM_SERVER_PORT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pNatWzdInfo->serverPort;
		}
		return true;
		break;
	case NCFG_ITEM_NAT_SERVER:
		{
			assert(len == (sizeof(NAT_SERVER_INFO)));
			NAT_SERVER_INFO *pNatServerInfo = reinterpret_cast<NAT_SERVER_INFO*>(pData);
			*pNatServerInfo = m_pNatWzdInfo->natInfo;
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CWzdNatManDlg::EnterConfig()
{
	//进入配置
	int tryCount = 200;
	CMessageMan* pMsgMan = CMessageMan::Instance();
	MESSAGE_DATA msgData;

	while (0 < tryCount)
	{
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
			//GUI::CDialog::MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_26), m_pStrTable->String(E_STR_ID_TITLE_FAILED), DEFAULT_BTN_OK|DEFAULT_ICON_ERROR);
			//return false;
			tryCount--;
			PUB_Sleep(50);
		}
	}

	GUI::CDialog::MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_26), m_pStrTable->String(E_STR_ID_TITLE_FAILED), DEFAULT_BTN_OK|DEFAULT_ICON_ERROR);
	return false;
}

void CWzdNatManDlg::ExitConfig() const
{
	//退出配置
	CMessageMan* pMsgMan = CMessageMan::Instance();
	pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);
}

void CWzdNatManDlg::CreateNatWizardPage()
{
	unsigned long pageIndex = 0;
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] ;

	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

	bool bSetShow = true;

	CWzdNatPageDlg * pWzdStartDlg = CreateNatStartDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdStartDlg)
	{
		m_vecWzdPage.push_back(pWzdStartDlg);
	}
	++pageIndex;

	CWzdNatPageDlg * pWzdBasicManDlg = CreateNatTabBasicManDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdBasicManDlg)
	{
		m_vecWzdPage.push_back(pWzdBasicManDlg);
	}
	++pageIndex;

	CWzdNatPageDlg * pNatEnableDlg = CreateNatPassWordDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pNatEnableDlg)
	{
		m_vecWzdPage.push_back(pNatEnableDlg);
	}
	++pageIndex;

	CWzdNatPageDlg * pWzdNetWorkSetUpDlg = CreateNatGetNetWorkSetUpDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdNetWorkSetUpDlg)
	{
		m_vecWzdPage.push_back(pWzdNetWorkSetUpDlg);
	}
	++pageIndex;

	CWzdNatPageDlg * pNatLocalConnectEndDlg = CreateLocalConnectEndDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pNatLocalConnectEndDlg)
	{
		m_vecWzdPage.push_back(pNatLocalConnectEndDlg);
	}
	++pageIndex;
	
	CWzdNatPageDlg * pWzdNatSelectDevDlg = CreateNatSelectDevDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdNatSelectDevDlg)
	{
		m_vecWzdPage.push_back(pWzdNatSelectDevDlg);
	}
	++pageIndex;
	
	CWzdNatPageDlg * pWzdQrCodeDlg = CreateNatQrCodeDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdQrCodeDlg)
	{
		m_vecWzdPage.push_back(pWzdQrCodeDlg);
	}
	++pageIndex;

	CWzdNatPageDlg * pWzdPCTipDlg = CreateNatPCTipDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdPCTipDlg)
	{
		m_vecWzdPage.push_back(pWzdPCTipDlg);
	}
	++pageIndex;

	CWzdNatPageDlg * pWzdNatEndDlg = CreateNatEndUpDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdNatEndDlg)
	{
		m_vecWzdPage.push_back(pWzdNatEndDlg);
	}
	++pageIndex;
}

void CWzdNatManDlg::CreateWizardInfo()
{
	if( !m_pNatWzdInfo )
	{
		m_pNatWzdInfo = new NAT_WIZARD_INFO;
	}
	memset(m_pNatWzdInfo, 0, sizeof(NAT_WIZARD_INFO));
}

void CWzdNatManDlg::InitWizardInfo()
{
	/*
	将从配置中得到的数据初始化复制到向导结构体中
	*/
	unsigned char* pData = NULL;
	unsigned long dataLen = 0;
	CCfgDlg::GetConfig( NCFG_ITEM_DATE_FORMAT, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->DateMode = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_HOUR_FORMAT, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->TimeMode = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_LANGUAGE, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->CurrentLanguage = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_TIME_ZONE, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->TimeZone = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_SHOW_WIZZARD, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->ShowWzd = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_TIME_SYNC, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->sync = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_TIME_SERVER, &pData, dataLen );
	assert( dataLen == sizeof(NCFG_INFO_LONG_NAME));
	memcpy(&(m_pNatWzdInfo->timeServer),pData,sizeof(NCFG_INFO_LONG_NAME));

	CCfgDlg::GetConfig(NCFG_ITEM_USER, &pData, dataLen);
	assert( dataLen == sizeof(NCFG_INFO_USER)*MAX_USER_NUM);
	memcpy(&(m_pNatWzdInfo->userInfo),pData,dataLen);

	CCfgDlg::GetConfig( NCFG_ITEM_DAYLIGHT_INFO, &pData, dataLen );
	assert( dataLen == sizeof(DAYLIGHT_INFO));
	memcpy(&(m_pNatWzdInfo->dstInfo),pData,sizeof(DAYLIGHT_INFO));

	CCfgDlg::GetConfig(NCFG_ITEM_HTTP_PORT, &pData, dataLen);
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->httpPort = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig(NCFG_ITEM_SERVER_PORT, &pData, dataLen);
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->serverPort = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig(NCFG_ITEM_NAT_SERVER, &pData, dataLen);
	assert( dataLen == sizeof(NAT_SERVER_INFO));
	m_pNatWzdInfo->natInfo = *reinterpret_cast<NAT_SERVER_INFO*>(pData);

	//获取语言
	CCfgDlg::GetConfig( NCFG_ITEM_SUPPORT_LANGUAGE, &pData, dataLen );
	if( dataLen>0)
	{
		memcpy(&m_pNatWzdInfo->Language,pData,dataLen);
		m_pNatWzdInfo->LanguageNum=dataLen/sizeof(NCFG_INFO_LANGUAGE);
	}

	//获取用户信息相关项目
	CCfgDlg::GetConfig( NCFG_ITEM_DEVUSER_NAME_MAX_LEN, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->userNameMaxLen= *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_DEVUSER_PWD_MAX_LEN, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->passwordMaxLen= *reinterpret_cast<unsigned long*>(pData);

}

void CWzdNatManDlg::ShowPage(int index)
{
	assert(index >= 0);
	assert(index < m_vecWzdPage.size());

	for (  int i = 0; i < m_vecWzdPage.size(); i++ )
	{
		if( index == i )
		{
			if( !m_vecWzdPage[i]->IsShow() )
			{
				m_vecWzdPage[i]->Show(true, false);
			}
		}
		else
		{
			if( m_vecWzdPage[i]->IsShow() )
			{
				m_vecWzdPage[i]->Show(false, false);
			}
		}
	}
}

void CWzdNatManDlg::ShowBtn(int index)
{
	GUI::CWnd* pBtnApp = GetCtrl(CTRLID_DEFAULT_BTN_APPLY);
	GUI::CWnd* pBtnOk = GetCtrl(CTRLID_DEFAULT_BTN_OK);
	GUI::CWnd* pBtnCancel = GetCtrl(CTRLID_DEFAULT_BTN_CANCEL);


	assert(pBtnOk);
	assert(pBtnCancel);
	assert(pBtnApp);

	if( index >= m_vecWzdPage.size() - 1 )
	{
		//最后一页
		if( m_vecWzdPage.size() > 1 )
		{
			//
			pBtnApp->SetEnable(false, false);
		}
		else
		{
			pBtnApp->SetEnable(false, false);
		}
		pBtnOk->SetEnable(true, false);
		pBtnOk->SetCaption(m_pStrTable->String(E_STR_ID_FINISH), false);

	}
	else if( index <= 0 )
	{
		//第一页
		pBtnApp->SetEnable(false, false);
		pBtnOk->SetEnable(true,false);
		pBtnOk->SetCaption(m_pStrTable->String(E_STR_ID_NEXT), false);
	}
	else
	{
		pBtnApp->SetEnable(true, false);
		pBtnCancel->SetEnable(true,false);
		//中间页面
		switch(m_curIndex)
		{
		case PAGE_NETWORK:
			{
				pBtnOk->SetEnable(true, false);
				pBtnOk->SetCaption(m_pStrTable->String(E_STR_ID_NEXT), false);
			}
			break;
		case PAGE_LOCALEND:
			{
				pBtnOk->SetEnable(true, false);
				pBtnOk->SetCaption(m_pStrTable->String(E_STR_ID_FINISH), false);
			}
			break;
		default:
			pBtnOk->SetEnable(true, false);
			pBtnOk->SetCaption(m_pStrTable->String(E_STR_ID_NEXT), false);
			break;
		}

	}

}

void CWzdNatManDlg::OnWzdEnd()
{
	assert(0 != m_pNatWzdInfo->Modified);
	SetModify(true);

	//修正日期、时间控件显示模式设置
	unsigned long dateMode = CDateCtrl::DateMode(m_pNatWzdInfo->DateMode);

	if (dateMode != CDateCtrl::GetDateMode())
	{
		CDateCtrl::SetDateMode(dateMode, true);
	}

	unsigned long timeMode = CTimeCtrl::TimeMode(m_pNatWzdInfo->TimeMode);

	if (timeMode != CTimeCtrl::GetTimeMode())
	{
		CTimeCtrl::SetTimeMode(timeMode, true);
	}

	//修改系统时间
	ZONE_TIME_INFO zoneTimInfo;
	zoneTimInfo.UseTime = 1;
	zoneTimInfo.UseZone = 1;
	zoneTimInfo.TimeZone = m_pNatWzdInfo->TimeZone;

	CWzdNatBasicManDlg* pWzdBasic = dynamic_cast<CWzdNatBasicManDlg*>(m_vecWzdPage[1]);
	assert(pWzdBasic);

	if(pWzdBasic->GetCurrZoneTimeInfo(zoneTimInfo, zoneTimInfo.TimeZone))
	{
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_L_CHANGE_ZONE_TIME,(unsigned char*)&zoneTimInfo, sizeof(ZONE_TIME_INFO));
	}

	//保存其他配置
	CCfgDlg::OnOK();
}

CWzdNatPageDlg * CWzdNatManDlg::CreateNatStartDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNatStartDlg * pTipStart = new CWzdNatStartDlg();
	pTipStart->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	pTipStart->SetWzdInfo(m_pNatWzdInfo);
	return pTipStart;
}

CWzdNatPageDlg * CWzdNatManDlg::CreateNatTabBasicManDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNatBasicManDlg* pBasicManDlg = new CWzdNatBasicManDlg();
	pBasicManDlg->SetWzdInfo(m_pNatWzdInfo);
	pBasicManDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	return pBasicManDlg;
}

CWzdNatPageDlg* CWzdNatManDlg::CreateNatGetNetWorkSetUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNatGetNetWorkSetUpDlg * pGetNetWorkSetUpDlg = new CWzdNatGetNetWorkSetUpDlg();
	pGetNetWorkSetUpDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,bshow);
	pGetNetWorkSetUpDlg->SetWzdInfo(m_pNatWzdInfo);
	return pGetNetWorkSetUpDlg;
}

CWzdNatPageDlg* CWzdNatManDlg::CreateNatPassWordDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNatPassWordDlg * pNatEnableDlg = new CWzdNatPassWordDlg();
	pNatEnableDlg->SetWzdInfo(m_pNatWzdInfo);
	pNatEnableDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	return pNatEnableDlg;
}

CWzdNatPageDlg* CWzdNatManDlg::CreateLocalConnectEndDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNatLocalEndDlg *pNatLocalEndDlg = new CWzdNatLocalEndDlg();
	pNatLocalEndDlg->SetWzdInfo(m_pNatWzdInfo);
	pNatLocalEndDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	return pNatLocalEndDlg;
}

CWzdNatPageDlg* CWzdNatManDlg::CreateNatSelectDevDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	std::string strTip;
	// strTip.append("Peer-to-Peer setup");
	strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_P2P_00));
	strTip.append("\\n");
	// strTip.append("The next step will be to set up the network connection so you can view your cameras over your network and the Internet.");
	strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_P2P_01));
	strTip.append("\\n");
	// strTip.append("Choose whether you want to connect via computer, Apple or Android mobile device.");
	strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_P2P_02));
	strTip.append("\\n");
	// strTip.append("You will be able to come back to this window to choose another option.");
	strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_P2P_03));
	strTip.append("\\n");

	CWzdNatSelectDevDlg *pNatSelectDevDlg = new CWzdNatSelectDevDlg();
	pNatSelectDevDlg -> SetDevSelectIndex(m_devSelect);
	pNatSelectDevDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,bshow);
	pNatSelectDevDlg -> SetWzdInfo(m_pNatWzdInfo);
	pNatSelectDevDlg -> SetTipInfo(strTip);
	return pNatSelectDevDlg;
}

CWzdNatPageDlg * CWzdNatManDlg::CreateNatQrCodeDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNatQrCodeDlg * pNatQrCodeDlg = new CWzdNatQrCodeDlg();
	pNatQrCodeDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	pNatQrCodeDlg->SetWzdInfo(m_pNatWzdInfo);
	return pNatQrCodeDlg;
}

CWzdNatPageDlg* CWzdNatManDlg::CreateNatPCTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNatPCTipDlg *pNatTipDlg = new CWzdNatPCTipDlg();
	pNatTipDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	pNatTipDlg->SetWzdInfo(m_pNatWzdInfo);
	return pNatTipDlg;
}

CWzdNatPageDlg * CWzdNatManDlg::CreateNatEndUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	string strTip;
	// strTip.append("The Q-See Start Up Wizard is finished.");
	strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_END_00));
	strTip.append("\\n");
	// strTip.append("Please disable the Start Up Wizard by unchecking the \"Open Wizard Next Time\" box ");
	strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_END_01));
	// strTip.append("in the lower left corner, below, so it does not run again the next time you restart the DVR.");
	strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_END_02));
	strTip.append("\\n");
	// strTip.append("The QR code and other networking information may be found in the Information window.");
	strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_END_03));
	strTip.append("\\n");
	
	CWzdNatEndDlg *pWzdNatEndDLg = new CWzdNatEndDlg();
	pWzdNatEndDLg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	pWzdNatEndDLg->SetWzdInfo(m_pNatWzdInfo);
	pWzdNatEndDLg->SetTipInfo(strTip);
	return pWzdNatEndDLg;
}
//end
