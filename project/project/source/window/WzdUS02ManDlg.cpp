/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2010-09-17
** Name         : WzdManDlg.cpp
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#include "WzdUS02ManDlg.h"
#include "WzdUS02StartDlg.h"
#include "WzdUS02BasicManDlg.h"
#include "WzdUS02ConnectTipDlg.h"
#include "WzdUS02GetNetWorkSetUpDlg.h"
#include "WzdUS02GetNetWorkSetUpDlg.h"
#include "WzdUS02CheckUpnpDlg.h"
#include "WzdUS02GetPortDlg.h"
#include "WzdUS02GetRouterIpDlg.h"
#include "WzdUS02SelectDeviceDlg.h"
#include "WzdUS02MobiletipDlg.h"
#include "WzdUS02DDNSTipDlg.h"
#include "WzdUS02DDNSConnectTipDlg.h"
#ifdef __SUPPORT_SHOW_QRCODE__
#include "WzdUS02QRShowDlg.h"
#include "QRCode.h"
#include "MessageMan.h"
#endif
#include "WzdUS02EndUpDlg.h"
#include "CtrlID.h"
#include "MessageBox.h"
#include "UserMan.h"

extern bool	g_bCloseWzd;

//在向导修改语言
extern bool g_bWizzardChangeLanguage;


CWzdUS02ManDlg::CWzdUS02ManDlg() 
{
	m_curTime = 0;
	m_lastTime = 0;
	m_pUS02WzdInfo = NULL;
	m_curIndex =0;
	m_maxTime = 10*60*1000000;
}

CWzdUS02ManDlg::~CWzdUS02ManDlg()
{

}

bool CWzdUS02ManDlg::TryEnterConfig()
{
	if( !m_bEnterConfig )
	{
		m_bEnterConfig = EnterConfig();
	}

	return m_bEnterConfig;
}

void CWzdUS02ManDlg::OnInitial()
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
		SetImage(BMP_MEM_DLG_NORMAL, false);
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
	CreateUS02WizardPage();
	//初始第一页面
	m_curIndex = 0;
	//进配置,当远程IE先于本地向导进配置时,将无法保存配置数据.
	//此时需要提示用户关闭远程配置方可进行设置
	m_bEnterConfig = false;
	bool bret=TryEnterConfig();

}

void CWzdUS02ManDlg::OnPostInitial()
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

void CWzdUS02ManDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	if( m_pUS02WzdInfo )
	{
		delete m_pUS02WzdInfo;
		m_pUS02WzdInfo = NULL;
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

void CWzdUS02ManDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);
}

void CWzdUS02ManDlg::EndDialog(DLG_END_VALUE result)
{

}

unsigned long CWzdUS02ManDlg::OnOK()//下一页
{
	switch(m_curIndex)
	{
		case PAGE_NETWORK:
		case PAGE_CHECKUPNP:
		case PAGE_ROUTERIP:
			{
				GUI::CButton *pButton = NULL;
				pButton =  dynamic_cast<GUI::CButton*>(GetTopestWnd()->GetCtrl(CTRLID_DEFAULT_BTN_OK));
				if(false == pButton->IsEnable())//如果截获下一个按钮不时能状态，不可以往下翻页
				{
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
		switch(m_curIndex)
		{
			case PAGE_ROUTERIP:
			case PAGE_MOBIELESELECT:
			{
				m_vecWzdPage[m_curIndex]->GetWzdData();
				break;
			}
			default:
				break;
		}
		m_curIndex += 1;
		ShowPage(m_curIndex);
		ShowBtn(m_curIndex);
		m_vecWzdPage[m_curIndex]->ShowWzdData(false);
		Repaint();
	}
	else
	{
		m_curIndex += 1;
		//最后页面读取
		SaveUS02Wizard();
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CWzdUS02ManDlg::OnCancel()
{
	SaveUS02Wizard();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CWzdUS02ManDlg::OnApply()
{
	//上一页
	if( m_curIndex -1 >= 0 )
	{
		m_curIndex -= 1;
		ShowPage(m_curIndex);
		ShowBtn(m_curIndex);
		m_vecWzdPage[m_curIndex]->ShowWzdData(false);

		Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CWzdUS02ManDlg::SaveUS02Wizard()
{
	//提取数据
	for(int i = 0; i < m_curIndex; i++)
	{
		m_vecWzdPage[i]->GetWzdData(true);
		m_pUS02WzdInfo->Modified = 1;
	}
	if(0 != m_pUS02WzdInfo->Modified)
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

void CWzdUS02ManDlg::OnIdle()
{
	if(1 == m_pUS02WzdInfo->UpDateWzdTime)
	{
		m_lastTime = GetCurrTime();
		m_curTime = m_lastTime;
		
		m_pUS02WzdInfo->UpDateWzdTime = 0;
	}
	else
	{
		m_curTime = GetCurrTime();
	}
	
	
	if (((m_lastTime > 0) && ((m_curTime - m_lastTime) > m_maxTime)) || (g_bCloseWzd == true))
	{
		SaveUS02Wizard();
	}
	GUI::CDialog::OnIdle();
}

unsigned long CWzdUS02ManDlg::CheckValue()
{
	if((1 == m_curIndex)||(9== m_curIndex))
	{
		return m_vecWzdPage[m_curIndex]->CheckValue();
	}
	return 0;
}

void CWzdUS02ManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	//时间和DST页面所需要的配置
	itemList.push_back(NCFG_ITEM_DATE_FORMAT);
	itemList.push_back(NCFG_ITEM_HOUR_FORMAT);
	itemList.push_back(NCFG_ITEM_LANGUAGE);
	itemList.push_back(NCFG_ITEM_TIME_ZONE);
	itemList.push_back(NCFG_ITEM_TIME_SYNC);
	itemList.push_back(NCFG_ITEM_TIME_SERVER);
	itemList.push_back(NCFG_ITEM_DAYLIGHT_INFO);
	itemList.push_back(NCFG_ITEM_SHOW_WIZZARD);

	itemList.push_back(NCFG_ITEM_HTTP_PORT);
	itemList.push_back(NCFG_ITEM_SERVER_PORT);
	itemList.push_back(NCFG_ITEM_DDNS_INFO);
	itemList.push_back(NCFG_ITEM_DDNS_UPDATE_CYCLE);
	itemList.push_back(NCFG_ITEM_MINT_DDNS_SERVER_NAME);
	itemList.push_back(NCFG_ITEM_DDNS_USER_NAME_MAX_LEN);
	itemList.push_back(NCFG_ITEM_DDNS_PASSWORD_MAX_LEN);
	itemList.push_back(NCFG_ITEM_DDNS_DOMAIN_MAX_LEN);
	itemList.push_back(NCFG_ITEM_SUPPORT_LANGUAGE);
	itemList.push_back(NCFG_ITEM_DDNS_SERVER_INFO);	

}

bool CWzdUS02ManDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);
	switch (cfgID)
	{
	
	case NCFG_ITEM_DATE_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pUS02WzdInfo->DateMode;
		}
		return true;
		break;
	case NCFG_ITEM_HOUR_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue =m_pUS02WzdInfo->TimeMode;
		}
		return true;
		break;
	case NCFG_ITEM_LANGUAGE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue =m_pUS02WzdInfo->CurrentLanguage;
		}
		return true;
		break;
	case NCFG_ITEM_TIME_ZONE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pUS02WzdInfo->TimeZone;
		}
		return true;
		break;
	case NCFG_ITEM_TIME_SYNC:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pUS02WzdInfo->sync;
		}
		return true;
		break;
	case NCFG_ITEM_TIME_SERVER:
		{
			assert(len == sizeof(NCFG_INFO_LONG_NAME));
			NCFG_INFO_LONG_NAME * pValue = reinterpret_cast<NCFG_INFO_LONG_NAME*>(pData);
			memcpy(pValue,&(m_pUS02WzdInfo->timeServer),sizeof(NCFG_INFO_LONG_NAME));
		}
		return true;
		break;
	case NCFG_ITEM_DAYLIGHT_INFO:
		{
			assert(len == sizeof(DAYLIGHT_INFO));
			DAYLIGHT_INFO * pValue = reinterpret_cast<DAYLIGHT_INFO*>(pData);
			*pValue = m_pUS02WzdInfo->dstInfo;
		}
		return true;
		break;
	case NCFG_ITEM_SHOW_WIZZARD:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pUS02WzdInfo->ShowWzd;
		}
		return true;
		break;
	case NCFG_ITEM_HTTP_PORT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pUS02WzdInfo->httpPort;
		}
		return true;
		break;
	case NCFG_ITEM_SERVER_PORT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pUS02WzdInfo->serverPort;
		}
		return true;
		break;
	case NCFG_ITEM_DDNS_INFO:
		{
			assert( len == sizeof(DDNS_INFO) );
			DDNS_INFO *pValue =  reinterpret_cast<DDNS_INFO*>(pData);
			memcpy(pValue,&(m_pUS02WzdInfo->ddnsInfo),sizeof(DDNS_INFO));
		}
		return true;
		break;
	case NCFG_ITEM_DDNS_UPDATE_CYCLE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long *pValue =  reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pUS02WzdInfo->ddnsCycle;
		}
		return true;
		break;
	case NCFG_ITEM_MINT_DDNS_SERVER_NAME:
		{
			assert(len == sizeof(unsigned char)*132);
			unsigned char *pValue =  reinterpret_cast<unsigned char*>(pData);
			memcpy(pValue,m_pUS02WzdInfo->ddnsServerName, sizeof(unsigned char)*132);
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CWzdUS02ManDlg::EnterConfig()
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

void CWzdUS02ManDlg::ExitConfig() const
{
	//退出配置
	CMessageMan* pMsgMan = CMessageMan::Instance();
	pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);
}
void CWzdUS02ManDlg::CreateUS02WizardPage()
{
	unsigned long pageIndex = 0;
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] ;

	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

	bool bSetShow = true;
	CWzdUS02PageDlg * pWzdStartDlg = CreateUS02TipStartDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdStartDlg)
	{
		m_vecWzdPage.push_back(pWzdStartDlg);
	}
	++pageIndex;

	CWzdUS02PageDlg * pWzdBasicManDlg = CreateUS02TabBasicManDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdBasicManDlg)
	{
		m_vecWzdPage.push_back(pWzdBasicManDlg);
	}
	++pageIndex;
#ifdef __SUPPORT_SHOW_QRCODE__
	CWzdUS02PageDlg * pWzdUS02QRShowDlg = CreateUS02QRShowDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdUS02QRShowDlg)
	{
		m_vecWzdPage.push_back(pWzdUS02QRShowDlg);
	}
	++pageIndex;
#endif
	CWzdUS02PageDlg *pWzdConnectTipDlg = CreateUS02ConntctTipDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdConnectTipDlg)
	{
		m_vecWzdPage.push_back(pWzdConnectTipDlg);
	}
	++pageIndex;

	CWzdUS02PageDlg *pWzdGetNetWorkSetUpDlg = CreateUS02GetNetWorkSetUpDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdGetNetWorkSetUpDlg)
	{
		m_vecWzdPage.push_back(pWzdGetNetWorkSetUpDlg);
	}
	++pageIndex;

	CWzdUS02PageDlg * pWzdUpnpdlg = CreateUS02CheckUpnpDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdUpnpdlg)
	{
		m_vecWzdPage.push_back(pWzdUpnpdlg);
	}
	++pageIndex;

	CWzdUS02PageDlg * pWzdGetPortDlg = CreateUS02GetPortDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdGetPortDlg)
	{
		m_vecWzdPage.push_back(pWzdGetPortDlg);
	}
	++pageIndex;

	CWzdUS02PageDlg * pWzdUS02GetRouterIpDlg = CreateUS02GetRouterIpDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdUS02GetRouterIpDlg)
	{
		m_vecWzdPage.push_back(pWzdUS02GetRouterIpDlg);
	}
	++pageIndex;

	CWzdUS02PageDlg * pWzdUs02SelectMobileDlg = CreateUS02SelectMobileDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdUS02GetRouterIpDlg)
	{
		m_vecWzdPage.push_back(pWzdUs02SelectMobileDlg);
	}
	++pageIndex;

	CWzdUS02PageDlg *pWzdUS02MobiletipDlg = CreateUS02MobileTipDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdUS02MobiletipDlg)
	{
		m_vecWzdPage.push_back(pWzdUS02MobiletipDlg);
	}
	++pageIndex;

	CWzdUS02PageDlg * pWzdUS02DDNSTipDlg = CreateUS02DDNSTipDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdUS02DDNSTipDlg)
	{
		m_vecWzdPage.push_back(pWzdUS02DDNSTipDlg);
	}
	++pageIndex;

	CWzdUS02PageDlg * pWzdUS02DDNSConnectTipDlg = CreateUS02DDNSConnectTipDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdUS02DDNSConnectTipDlg)
	{
		m_vecWzdPage.push_back(pWzdUS02DDNSConnectTipDlg);
	}
	++pageIndex;

	CWzdUS02PageDlg *pWzdEndupDlg = CreateUS02EndUpDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdEndupDlg)
	{
		m_vecWzdPage.push_back(pWzdEndupDlg);
	}
	++pageIndex;
}

void CWzdUS02ManDlg::CreateWizardInfo()
{
	if( !m_pUS02WzdInfo )
	{
		m_pUS02WzdInfo = new US02_WzdInfo;
	}
	memset(m_pUS02WzdInfo, 0, sizeof(US02_WzdInfo));
}

void CWzdUS02ManDlg::InitWizardInfo()
{
	/*
	将从配置中得到的数据初始化复制到向导结构体中
	*/
	unsigned char* pData = NULL;
	unsigned long dataLen = 0;
	CCfgDlg::GetConfig( NCFG_ITEM_DATE_FORMAT, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pUS02WzdInfo->DateMode = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_HOUR_FORMAT, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pUS02WzdInfo->TimeMode = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_LANGUAGE, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pUS02WzdInfo->CurrentLanguage = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_TIME_ZONE, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pUS02WzdInfo->TimeZone = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_SHOW_WIZZARD, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pUS02WzdInfo->ShowWzd = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_TIME_SYNC, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pUS02WzdInfo->sync = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_TIME_SERVER, &pData, dataLen );
	assert( dataLen == sizeof(NCFG_INFO_LONG_NAME));
	memcpy(&(m_pUS02WzdInfo->timeServer),pData,sizeof(NCFG_INFO_LONG_NAME));

	CCfgDlg::GetConfig( NCFG_ITEM_DAYLIGHT_INFO, &pData, dataLen );
	assert( dataLen == sizeof(DAYLIGHT_INFO));
	memcpy(&(m_pUS02WzdInfo->dstInfo),pData,sizeof(DAYLIGHT_INFO));

	CCfgDlg::GetConfig(NCFG_ITEM_HTTP_PORT, &pData, dataLen);
	assert( dataLen == sizeof(unsigned long));
	m_pUS02WzdInfo->httpPort = *reinterpret_cast<unsigned long*>(pData);
	
	CCfgDlg::GetConfig(NCFG_ITEM_SERVER_PORT, &pData, dataLen);
	assert( dataLen == sizeof(unsigned long));
	m_pUS02WzdInfo->serverPort = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig(NCFG_ITEM_DDNS_INFO,&pData,dataLen);
	assert( dataLen == sizeof(DDNS_INFO));
	memcpy(&(m_pUS02WzdInfo->ddnsInfo),pData,sizeof(DDNS_INFO));

	CCfgDlg::GetConfig(NCFG_ITEM_DDNS_UPDATE_CYCLE,&pData,dataLen);
	assert( dataLen == sizeof(unsigned long));
	m_pUS02WzdInfo->ddnsCycle = *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig(NCFG_ITEM_MINT_DDNS_SERVER_NAME,&pData,dataLen);
	assert( dataLen == 132);
	memcpy(&(m_pUS02WzdInfo->ddnsServerName),pData,132);

	CCfgDlg::GetConfig(NCFG_ITEM_DDNS_USER_NAME_MAX_LEN,&pData,dataLen);
	assert(dataLen==sizeof(unsigned long));
	m_pUS02WzdInfo-> maxUserLen =  *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig(NCFG_ITEM_DDNS_PASSWORD_MAX_LEN,&pData,dataLen);
	assert(dataLen==sizeof(unsigned long));
	m_pUS02WzdInfo-> maxPassLen =  *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig(NCFG_ITEM_DDNS_DOMAIN_MAX_LEN,&pData,dataLen);
	assert(dataLen==sizeof(unsigned long));
	m_pUS02WzdInfo-> maxDomainLen =  *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_DDNS_SERVER_INFO, &pData, dataLen );
	m_pUS02WzdInfo->pDdnsSeverInfo = reinterpret_cast<DDNS_SERVER_INFO*>(pData);
	assert( NULL != m_pUS02WzdInfo->pDdnsSeverInfo );
	m_pUS02WzdInfo->ddnsServerCount = dataLen / sizeof(DDNS_SERVER_INFO);
	assert( 0 == dataLen % sizeof( DDNS_SERVER_INFO ) );

	//获取语言
	CCfgDlg::GetConfig( NCFG_ITEM_SUPPORT_LANGUAGE, &pData, dataLen );
	if( dataLen>0)
	{
		memcpy(&m_pUS02WzdInfo->Language,pData,dataLen);
		m_pUS02WzdInfo->LanguageNum=dataLen/sizeof(NCFG_INFO_LANGUAGE);
	}

}

void CWzdUS02ManDlg::ShowPage(int index)
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

void CWzdUS02ManDlg::ShowBtn(int index)
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
		case PAGE_CHECKUPNP:
			break;
		default:
			pBtnOk->SetEnable(true, false);
			pBtnOk->SetCaption(m_pStrTable->String(E_STR_ID_NEXT), false);
			break;
		}
		
	}

}

void CWzdUS02ManDlg::OnWzdEnd()
{
	assert(0 != m_pUS02WzdInfo->Modified);
	SetModify(true);

	//修正日期、时间控件显示模式设置
	unsigned long dateMode = CDateCtrl::DateMode(m_pUS02WzdInfo->DateMode);

	if (dateMode != CDateCtrl::GetDateMode())
	{
		CDateCtrl::SetDateMode(dateMode, true);
	}

	unsigned long timeMode = CTimeCtrl::TimeMode(m_pUS02WzdInfo->TimeMode);

	if (timeMode != CTimeCtrl::GetTimeMode())
	{
		CTimeCtrl::SetTimeMode(timeMode, true);
	}

	//修改系统时间
	ZONE_TIME_INFO zoneTimInfo;
	zoneTimInfo.UseTime = 1;
	zoneTimInfo.UseZone = 1;
	zoneTimInfo.TimeZone = m_pUS02WzdInfo->TimeZone;

	CWzdUS02BasicManDlg* pWzdBasic = dynamic_cast<CWzdUS02BasicManDlg*>(m_vecWzdPage[1]);
	assert(pWzdBasic);

	if(pWzdBasic->GetCurrZoneTimeInfo(zoneTimInfo, zoneTimInfo.TimeZone))
	{
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_L_CHANGE_ZONE_TIME,(unsigned char*)&zoneTimInfo, sizeof(ZONE_TIME_INFO));
	}

	//保存其他配置
	CCfgDlg::OnOK();
}

CWzdUS02PageDlg* CWzdUS02ManDlg::CreateUS02TipStartDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	std::string strTip;
	strTip = m_pStrTable->String(E_STR_ID_US02_WZD_STARTUPFOR);
	strTip.append("\\n");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_FIRSTTIP01));
	strTip.append("\\n");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_FIRSTTIP02));
	strTip.append("\\n");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_FIRSTTIP03));
	strTip.append("\\n");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_FIRSTTIP04));
	strTip.append("\\n");
	//strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_FIRSTTIP05));

	CWzdUS02StartDlg* pTipStart = new CWzdUS02StartDlg();
	pTipStart->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, show);
	pTipStart->SetWzdInfo(m_pUS02WzdInfo);
	pTipStart->SetTipDisk(strTip);
	return pTipStart;
}

CWzdUS02PageDlg* CWzdUS02ManDlg::CreateUS02TabBasicManDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	CWzdUS02BasicManDlg* pBasicManDlg = new CWzdUS02BasicManDlg();
	pBasicManDlg->SetWzdInfo(m_pUS02WzdInfo);
	pBasicManDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, show);
	return pBasicManDlg;
}

CWzdUS02PageDlg* CWzdUS02ManDlg::CreateUS02ConntctTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	std::string strTip;
	strTip.clear();
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_NETWORKSETUP));
	strTip.append("\\n");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_FIFTHTIP01));
	strTip.append("\\n");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_FIFTHTIP02));
	strTip.append("\\n");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_FIFTHTIP03));
	strTip.append("\\n");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_FIFTHTIP04));
	strTip.append("\\n");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_FIFTHTIP05));

	CWzdUS02ConnectTipDlg *pConnectTip = new CWzdUS02ConnectTipDlg();
	pConnectTip -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,show);
	pConnectTip->SetWzdInfo(m_pUS02WzdInfo);
	pConnectTip->SetTipDisk(strTip);	
	return pConnectTip;
}

CWzdUS02PageDlg* CWzdUS02ManDlg::CreateUS02GetNetWorkSetUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	CWzdUS02GetNetWorkSetUpDlg * pGetNetWorkSetUpDlg = new CWzdUS02GetNetWorkSetUpDlg();
	pGetNetWorkSetUpDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,show);
	pGetNetWorkSetUpDlg->SetWzdInfo(m_pUS02WzdInfo);
	return pGetNetWorkSetUpDlg;
}

CWzdUS02PageDlg* CWzdUS02ManDlg::CreateUS02CheckUpnpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	CWzdUS02CheckUpnpDlg * pCheckUpnpDlg = new CWzdUS02CheckUpnpDlg();
	pCheckUpnpDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,show);
	pCheckUpnpDlg->SetWzdInfo(m_pUS02WzdInfo);
	return pCheckUpnpDlg;
}

CWzdUS02PageDlg * CWzdUS02ManDlg::CreateUS02GetPortDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	CWzdUS02GetPortDlg * pGetPortDlg = new CWzdUS02GetPortDlg();
	pGetPortDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,show);
	pGetPortDlg->SetWzdInfo(m_pUS02WzdInfo);
	return pGetPortDlg;
}

CWzdUS02PageDlg *CWzdUS02ManDlg::CreateUS02GetRouterIpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	CWzdUS02GetRouterIpDlg * pWzdUS02GetRouterIpDlg = new CWzdUS02GetRouterIpDlg();
	pWzdUS02GetRouterIpDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,show);
	pWzdUS02GetRouterIpDlg->SetWzdInfo(m_pUS02WzdInfo);
	return pWzdUS02GetRouterIpDlg;
}

CWzdUS02PageDlg *CWzdUS02ManDlg::CreateUS02SelectMobileDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	CWzdUS02SelectDeviceDlg * pWzdUS02SelectMobileDlg = new CWzdUS02SelectDeviceDlg();
	pWzdUS02SelectMobileDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,show);
	pWzdUS02SelectMobileDlg -> SetWzdInfo(m_pUS02WzdInfo);
	return pWzdUS02SelectMobileDlg;
}

CWzdUS02PageDlg *CWzdUS02ManDlg::CreateUS02MobileTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	CWzdUS02MobiletipDlg * pWzdUS02MobileTipDlg = new CWzdUS02MobiletipDlg();
	pWzdUS02MobileTipDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,show);
	pWzdUS02MobileTipDlg -> SetWzdInfo(m_pUS02WzdInfo);
	return pWzdUS02MobileTipDlg;
}

CWzdUS02PageDlg *CWzdUS02ManDlg::CreateUS02DDNSTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	CWzdUS02DDNStipDlg * pWzdUS02DDNSTipDlg = new CWzdUS02DDNStipDlg();
	pWzdUS02DDNSTipDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,show);
	pWzdUS02DDNSTipDlg -> SetWzdInfo(m_pUS02WzdInfo);
	return pWzdUS02DDNSTipDlg;
}

CWzdUS02PageDlg *CWzdUS02ManDlg::CreateUS02DDNSConnectTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	CWzdUS02DDNSConnectTipDlg *pWzdUS02DDNSConnectTipDlg = new CWzdUS02DDNSConnectTipDlg();
	pWzdUS02DDNSConnectTipDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,show);
	pWzdUS02DDNSConnectTipDlg -> SetWzdInfo(m_pUS02WzdInfo);
	return pWzdUS02DDNSConnectTipDlg;
}

CWzdUS02PageDlg *CWzdUS02ManDlg::CreateUS02QRShowDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
#ifdef __SUPPORT_SHOW_QRCODE__
	char user[36] = "admin";
	char passWord[36]={0};//颠倒顺序
	unsigned long len  =0;
	CWzdUS02QRShowDlg *pWzdUS02QRShowDlg = new CWzdUS02QRShowDlg();
	CQRCode codeMan;
	QrCodeOutputData qrMaix;
	qrMaix.dataMatrix =NULL;
	qrMaix.width = 0;
	//读取系统信息并组装字符串
	NETWORK_STATUS_INFO networkStatusInfo;
	memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));
#ifdef __ENVIRONMENT_LINUX__
	MESSAGE_DATA msgData;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETWORK_INFO, NULL, 0, msgData);

	networkStatusInfo = *reinterpret_cast<NETWORK_STATUS_INFO *>(msgData.pData+sizeof(PACKCMD));

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#endif
	//读取admin的用户名和密码
	len =sizeof(passWord);
	bool bsucc = CUserMan::Instance()->GetAdminUsrPassword(passWord,len);
	//颠倒顺序
	if(bsucc)
	{
		len =strlen(passWord);
		char tmp;
		for(int i=0;i<len/2;++i)
		{
			tmp  = 0;
			tmp = passWord[i];
			passWord[i] = passWord[len - i - 1];
			passWord[len -i - 1] = tmp;
		}
	}
	char str[128]={0};
	snprintf(str,128,"<sn>%02X%02X%02X%02X%02X%02X</sn>\n",networkStatusInfo.MAC[0],networkStatusInfo.MAC[1],networkStatusInfo.MAC[2],networkStatusInfo.MAC[3],networkStatusInfo.MAC[4],networkStatusInfo.MAC[5]);
	snprintf(str+strlen(str),128-strlen(str), "<user>%s</user>\n",user);
	snprintf(str+strlen(str),128-strlen(str), "<password>%s</password>\n",passWord);
	snprintf(str+strlen(str),128-strlen(str), "<ip>%03d.%03d.%03d.%03d</ip>\n",(networkStatusInfo.IP&0xff), (networkStatusInfo.IP>>8)&0xff, (networkStatusInfo.IP>>16)&0xff, (networkStatusInfo.IP>>24)&0xff);
	snprintf(str+strlen(str),128-strlen(str), "<port>%d</port>\n",networkStatusInfo.serverPort);
	codeMan.GetQRCodeMatrixData((unsigned char *)str,strlen(str),&qrMaix);
	int i= 0;
	int j= 0;
	pWzdUS02QRShowDlg->SetMaix((unsigned char *)qrMaix.dataMatrix,qrMaix.width,qrMaix.width);
	codeMan.FreeQrCodeMatrix(&qrMaix);
	std::string strTip;
	strTip.append("P2P/Cloud Setup");
	strTip.append("\\n");
	strTip.append("1.to view your cameras on the PC:");
	strTip.append("-Go to qtview.com from a web browser");
	strTip.append("\\n");
	strTip.append("-Once you get the login screen,enter this serial Number:");
	char textArr[128] = {0};
	snprintf(textArr,128,"%02X-%02X-%02X-%02X-%02X-%02X",networkStatusInfo.MAC[0],networkStatusInfo.MAC[1],networkStatusInfo.MAC[2],networkStatusInfo.MAC[3],networkStatusInfo.MAC[4],networkStatusInfo.MAC[5]);
	strTip.append(textArr);
	strTip.append("\\n");
	strTip.append("-Enter user name:admin and password 123456(default)");
	strTip.append("\\n");
	strTip.append("2.to view your cameras from iphone or andriod:");
	strTip.append("\\n");
	strTip.append("-Download QT view app from the Appstore or Google play store");
	strTip.append("\\n");
	strTip.append("Scan this QR Code:Enter the password 123456(default)");
	pWzdUS02QRShowDlg -> SetWzdInfo(m_pUS02WzdInfo);
	pWzdUS02QRShowDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,show);
	pWzdUS02QRShowDlg -> SetTipDisk(strTip);
	strTip.clear();
	strTip.append("Please change the Password. Keep it in a safe location.");
	strTip.append("\\n");
	strTip.append("Lost the password will require assistance from Q-See Tech support.");
	strTip.append("\\n");
	pWzdUS02QRShowDlg -> SetNoteText(strTip);
	return pWzdUS02QRShowDlg;
#else
	return NULL;
#endif
}

CWzdUS02PageDlg *CWzdUS02ManDlg::CreateUS02EndUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex)
{
	std::string strTip;
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_14_01));
	strTip.append("\\n");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_WZD_14_02));

	CWzdUS02EndUpDlg* pTipEnd = new CWzdUS02EndUpDlg();
	pTipEnd->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, show);
	pTipEnd->SetWzdInfo(m_pUS02WzdInfo);
	pTipEnd->SetTipDisk(strTip);
	return pTipEnd;	
}


BEGIN_MSG_MAP(CWzdUS02ManDlg, CCfgDlg)
	ON_MSG(CTRLID_NULL,KEY_VIRTUAL_LANGUAGE,OnChangeLanguage)
END_MSG_MAP()

unsigned long CWzdUS02ManDlg::OnChangeLanguage()
{
	g_bWizzardChangeLanguage = true;
	SaveUS02Wizard();
	return KEY_VIRTUAL_MSG_STOP;
}
//end
