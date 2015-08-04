/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2013-11-25
** Name         : WzdNatManDlg.cpp
** Version      : 1.0
** Description  : ��͸�򵼹���Ի���
** Modify Record:
***********************************************************************/

#include "WzdNewNatManDlg.h"
#include "WzdNewNatStartDlg.h"
#include "WzdNewNatBasicManDlg.h"
#include "WzdNewNatPasswordDlg.h"
#include "WzdNewNatGetNetWorkSetUpDlg.h"
#include "WzdNatLocalEndDlg.h"
#include "WzdNewNatSelectDevDlg.h"
#include "WzdNewNatQrCodeDlg.h"
#include "WzdNewNatPCTipDlg.h"
#include "WzdNatEndDlg.h"
#include "CtrlID.h"
#include "MessageMan.h"
#include "MessageBox.h"
#include "WzdNewNatRemoteAccessTipDlg.h"
extern bool	g_bCloseWzd;


//�����޸�����
extern bool g_bWizzardChangeLanguage;
extern bool g_bNotCloseWizzard;


CWzdNewNatManDlg::CWzdNewNatManDlg() 
{
	m_curTime = 0;
	m_lastTime = 0;
	m_pNatWzdInfo = NULL;
	m_curIndex =0;
	m_maxTime = 10*60*1000000;
	m_devSelect = DEV_MOBILE;
	
}

CWzdNewNatManDlg::~CWzdNewNatManDlg()
{

}

bool CWzdNewNatManDlg::TryEnterConfig()
{
	if( !m_bEnterConfig )
	{
		m_bEnterConfig = EnterConfig();
	}

	return m_bEnterConfig;
}

void CWzdNewNatManDlg::OnInitial()
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
	//��ʼ��������
	CreateWizardInfo();
	//������ҳ��
	CreateNatWizardPage();
	//��ʼ��һҳ��
	m_curIndex = 0;
	//������,��Զ��IE���ڱ����򵼽�����ʱ,���޷�������������.
	//��ʱ��Ҫ��ʾ�û��ر�Զ�����÷��ɽ�������
	m_bEnterConfig = false;
	bool bret=TryEnterConfig();

}

void CWzdNewNatManDlg::OnPostInitial()
{
	CCfgDlg::OnPostInitial();

	m_lastTime = GetCurrTime();
	//��ʼ��������
	InitWizardInfo();
	//��ʼ������ҳ��ؼ���ʼֵ
	//��Ҫ��������m_pWizardInfo,����OnInitial�޷���Ĳ��ֿؼ�
	for (int i = 0; i < m_vecWzdPage.size(); i++)
	{
		m_vecWzdPage[i]->InitCtrlInfo();
	}
	//��ʼ����ǰ��Ҫ��ʾҳ��ֵ
	m_vecWzdPage[m_curIndex]->ShowWzdData(false);
	//��ʼ��ʾҳ��
	ShowPage(m_curIndex);

	//��ʾ��ť
	ShowBtn(m_curIndex);
}

void CWzdNewNatManDlg::OnDestroy()
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

void CWzdNewNatManDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);
}

void CWzdNewNatManDlg::EndDialog(DLG_END_VALUE result)
{

}

unsigned long CWzdNewNatManDlg::OnOK()//��һҳ
{
	switch(m_curIndex)
	{
	case PAGE_NETWORK:
		{
			GUI::CButton *pButton = NULL;
			pButton =  dynamic_cast<GUI::CButton*>(GetTopestWnd()->GetCtrl(CTRLID_DEFAULT_BTN_OK));
			if(false == pButton->IsEnable())//����ػ���һ����ť��ʱ��״̬�����������·�ҳ
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
		}
		break;
	case PAGE_DEVSELECT:
		{
			CWzdNewNatSelectDevDlg* pWzdDevSelectDlg = dynamic_cast<CWzdNewNatSelectDevDlg*>(m_vecWzdPage[PAGE_DEVSELECT]);
			assert(pWzdDevSelectDlg);
			m_devSelect = pWzdDevSelectDlg->GetDevSelectIndex();
		}
		break;
	case PAGE_QRCODE:
		{
			CWzdNewNatQrCodeDlg* pWzdQrCodeDlg = dynamic_cast<CWzdNewNatQrCodeDlg*>(m_vecWzdPage[PAGE_QRCODE]);
			assert(pWzdQrCodeDlg);
			if(pWzdQrCodeDlg->m_bSelect == true)
			{
				m_devSelect = NEXT_PC;
			}
		}
		break;
	case PAGE_PCTIP:
		{
			CWzdNewNatPCTipDlg* pWzdPcTipDlg = dynamic_cast<CWzdNewNatPCTipDlg*>(m_vecWzdPage[PAGE_PCTIP]);
			assert(pWzdPcTipDlg);
			if(pWzdPcTipDlg->m_bSelect == true)
			{
				m_devSelect = NEXT_MOBILE;
			}
		}

		break;

	case PAGE_NATPASS:
		{
			SetModify(true);
			m_vecWzdPage[PAGE_NATPASS]->GetWzdData(true);
			m_pNatWzdInfo->Modified = 1;
			if(0 != m_pNatWzdInfo->Modified)
			{

				if( TryEnterConfig() )
				{
				//�����Ч����

					/*OnWzdEnd();*/
					CCfgDlg::OnOK();
					printf("CCfgDlg::OnOk()\n");
					
				}
			}
		}
		break;


	default:
		break;
	}

	//��һҳ
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
			//PAGE_NETWORK����һҳ����m_pNatWzdInfo,ѡ��ͬҳ��
			if(m_pNatWzdInfo->natInfo.bStartNat)
			{
				m_curIndex += 1; //���ѡ���˱��ؽ������ͽ��뱾�ؽ���ҳ��
			}
		}
		//else if(PAGE_NATPASS == m_curIndex)
		//{
		//	SetModify(true);
		//	m_vecWzdPage[PAGE_NATPASS]->GetWzdData(true);
		//	m_pNatWzdInfo->Modified = 1;
		//	if(0 != m_pNatWzdInfo->Modified)
		//	{

		//		if( TryEnterConfig() )
		//		{
		//		//�����Ч����

		//			/*OnWzdEnd();*/
		//			CCfgDlg::OnOK();
		//			printf("CCfgDlg::OnOk()\n");
		//			
		//		}
		//	}
		//}
		else if(PAGE_LOCALEND == m_curIndex)//���ؽ�������һ��ֱ�ӹر���
		{
			g_bNotCloseWizzard = false;
			SaveNatWizard();
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if(PAGE_DEVSELECT == m_curIndex)
		{
			switch(m_devSelect)
			{

				case DEV_MOBILE:
					break;
				case DEV_PC:
					{
						m_curIndex += 1;//pC��ʾ����һҳ
					}
					break;
				default:
					break;
			}
		}
		else if(PAGE_PCTIP == m_curIndex)
		{
			if(m_devSelect == NEXT_MOBILE)
			{
				m_curIndex -= 2;
			}
		}
		else if(PAGE_QRCODE == m_curIndex)
		{
			if(m_devSelect == NEXT_PC)
			{
				
			} else 
			{	m_curIndex += 1;//����PC��ʾҳ��
			}
		}
		else
		{

		}
		
		m_curIndex += 1;
		if(PAGE_QRCODE == m_curIndex)
		{
			//��ά��ҳ����Ҫ�����豸����
			CWzdNewNatQrCodeDlg* pWzdQrCodeDlg = dynamic_cast<CWzdNewNatQrCodeDlg*>(m_vecWzdPage[m_curIndex]);
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

		//���ҳ���ȡ
		g_bNotCloseWizzard = false;
		SaveNatWizard();
		m_curIndex += 1;
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CWzdNewNatManDlg::OnCancel()
{
	g_bNotCloseWizzard = false;
	SaveNatWizard();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CWzdNewNatManDlg::OnApply()
{
	//��һҳ
	if( m_curIndex -1 >= 0 )
	{
		if(PAGE_LOCALEND == m_curIndex)//����ǽ��������뷵�ؼ��ipҳ��
		{
			
		}
		else if(PAGE_LOCALEND == m_curIndex-1)//���ǰһҳ�ǽ����������޸�����ҳ�棬��Ϊ����ҳ��û����һҳ
		{
			m_curIndex -= 1;
		}
		else if(PAGE_PCTIP == m_curIndex)
		{
			m_curIndex -= 1;//��ǰҳ����pc��ʾҳ�棬�ص��豸ѡ��ҳ��
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

unsigned long CWzdNewNatManDlg::SaveNatWizard()
{
	//��ȡ����
	for(int i = 0; i < m_vecWzdPage.size(); i++)
	{
		m_vecWzdPage[i]->GetWzdData(true);
		m_pNatWzdInfo->Modified = 1;
	}
	if(0 != m_pNatWzdInfo->Modified)
	{

		if( TryEnterConfig() )
		{
			//�����Ч����

			OnWzdEnd();

			CDialog::EndDialog(DLG_OK);
		}
		else
		{
			//��ʾIE�ͻ����˳�����,�Ƿ���������
			std::string str = m_pStrTable->String(E_STR_ID_MESSAGE_104);
			str.append("\\n");
			str.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_17));

			GUI::CMessageBox box(str.c_str(), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING);
			if( DLG_OK == box.DoModal(this) )
			{
				//�����ȴ�
			}
			else
			{
				//����
				CDialog::EndDialog(DLG_OK);
			}
		}
	}
	else
	{
		//û���޸�
		CDialog::EndDialog(DLG_OK);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

void CWzdNewNatManDlg::OnIdle()
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
		if(!g_bNotCloseWizzard)
		{
			SaveNatWizard();			
		}
	}
	GUI::CDialog::OnIdle();
}

unsigned long CWzdNewNatManDlg::CheckValue()
{
	return m_vecWzdPage[m_curIndex]->CheckValue();
}

void CWzdNewNatManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	//ʱ���DSTҳ������Ҫ������
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

bool CWzdNewNatManDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
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

bool CWzdNewNatManDlg::EnterConfig()
{
	//��������
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

void CWzdNewNatManDlg::ExitConfig() const
{
	//�˳�����
	CMessageMan* pMsgMan = CMessageMan::Instance();
	pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);
}

void CWzdNewNatManDlg::CreateNatWizardPage()
{
	unsigned long pageIndex = 0;
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] ;

	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

	bool bSetShow = true;

	CWzdNatPageDlg * pWzdStartDlg = CreateNewNatStartDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdStartDlg)
	{
		m_vecWzdPage.push_back(pWzdStartDlg);
	}
	++pageIndex;

	CWzdNatPageDlg * pWzdBasicManDlg = CreateNewNatTabBasicManDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdBasicManDlg)
	{
		m_vecWzdPage.push_back(pWzdBasicManDlg);
	}
	++pageIndex;

	CWzdNatPageDlg * pNatEnableDlg = CreateNewNatPasswordDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pNatEnableDlg)
	{
		m_vecWzdPage.push_back(pNatEnableDlg);
	}
	++pageIndex;

	CWzdNatPageDlg *pNewNatRemoteAccessTipDlg = CreateRemoteAccessTipDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pNewNatRemoteAccessTipDlg)
	{
		m_vecWzdPage.push_back(pNewNatRemoteAccessTipDlg);
	}
	++pageIndex;

	CWzdNatPageDlg * pWzdNetWorkSetUpDlg = CreateNewNatGetNetWorkSetUpDlg(x,y,cx,cy,false,pageIndex);
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
	
	CWzdNatPageDlg * pWzdNatSelectDevDlg = CreateNewNatSelectDevDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdNatSelectDevDlg)
	{
		m_vecWzdPage.push_back(pWzdNatSelectDevDlg);
	}
	++pageIndex;
	
	CWzdNatPageDlg * pWzdQrCodeDlg = CreateNewNatQrCodeDlg(x,y,cx,cy,false,pageIndex);
	if(NULL != pWzdQrCodeDlg)
	{
		m_vecWzdPage.push_back(pWzdQrCodeDlg);
}
	++pageIndex;

	CWzdNatPageDlg * pWzdPCTipDlg = CreateNewNatPCTipDlg(x,y,cx,cy,false,pageIndex);
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

void CWzdNewNatManDlg::CreateWizardInfo()
{
	if( !m_pNatWzdInfo )
	{
		m_pNatWzdInfo = new NAT_WIZARD_INFO;
	}
	memset(m_pNatWzdInfo, 0, sizeof(NAT_WIZARD_INFO));
}

void CWzdNewNatManDlg::InitWizardInfo()
{
	/*
	���������еõ������ݳ�ʼ�����Ƶ��򵼽ṹ����
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

	//��ȡ����
	CCfgDlg::GetConfig( NCFG_ITEM_SUPPORT_LANGUAGE, &pData, dataLen );
	if( dataLen>0)
	{
		memcpy(&m_pNatWzdInfo->Language,pData,dataLen);
		m_pNatWzdInfo->LanguageNum=dataLen/sizeof(NCFG_INFO_LANGUAGE);
	}

	//��ȡ�û���Ϣ�����Ŀ
	CCfgDlg::GetConfig( NCFG_ITEM_DEVUSER_NAME_MAX_LEN, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->userNameMaxLen= *reinterpret_cast<unsigned long*>(pData);

	CCfgDlg::GetConfig( NCFG_ITEM_DEVUSER_PWD_MAX_LEN, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pNatWzdInfo->passwordMaxLen= *reinterpret_cast<unsigned long*>(pData);

}

void CWzdNewNatManDlg::ShowPage(int index)
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

void CWzdNewNatManDlg::ShowBtn(int index)
{
	GUI::CWnd* pBtnApp = GetCtrl(CTRLID_DEFAULT_BTN_APPLY);
	GUI::CWnd* pBtnOk = GetCtrl(CTRLID_DEFAULT_BTN_OK);
	GUI::CWnd* pBtnCancel = GetCtrl(CTRLID_DEFAULT_BTN_CANCEL);


	assert(pBtnOk);
	assert(pBtnCancel);
	assert(pBtnApp);

	if( index >= m_vecWzdPage.size() - 1 )
	{
		//���һҳ
		if( m_vecWzdPage.size() > 1 )
		{
			//
			pBtnApp->SetEnable(true, false);
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
		//��һҳ
		pBtnApp->SetEnable(false, false);
		pBtnOk->SetEnable(true,false);
		pBtnOk->SetCaption(m_pStrTable->String(E_STR_ID_NEXT), false);
	}
	else
	{
		pBtnApp->SetEnable(true, false);
		pBtnCancel->SetEnable(true,false);
		//�м�ҳ��
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

void CWzdNewNatManDlg::OnWzdEnd()
{
	assert(0 != m_pNatWzdInfo->Modified);
	SetModify(true);

	//�������ڡ�ʱ��ؼ���ʾģʽ����
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

	//�޸�ϵͳʱ��
	ZONE_TIME_INFO zoneTimInfo;
	zoneTimInfo.UseTime = 1;
	zoneTimInfo.UseZone = 1;
	zoneTimInfo.TimeZone = m_pNatWzdInfo->TimeZone;

	CWzdNewNatBasicManDlg* pWzdBasic = dynamic_cast<CWzdNewNatBasicManDlg*>(m_vecWzdPage[1]);
	assert(pWzdBasic);

	if(pWzdBasic->GetCurrZoneTimeInfo(zoneTimInfo, zoneTimInfo.TimeZone))
	{
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_L_CHANGE_ZONE_TIME,(unsigned char*)&zoneTimInfo, sizeof(ZONE_TIME_INFO));
	}

	//������������
	CCfgDlg::OnOK();
}

CWzdNatPageDlg * CWzdNewNatManDlg::CreateNewNatStartDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNewNatStartDlg * pTipStart = new CWzdNewNatStartDlg();
	pTipStart->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	pTipStart->SetWzdInfo(m_pNatWzdInfo);
	return pTipStart;
}

CWzdNatPageDlg * CWzdNewNatManDlg::CreateNewNatTabBasicManDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNewNatBasicManDlg* pBasicManDlg = new CWzdNewNatBasicManDlg();
	pBasicManDlg->SetWzdInfo(m_pNatWzdInfo);
	pBasicManDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	return pBasicManDlg;
}

CWzdNatPageDlg* CWzdNewNatManDlg::CreateNewNatGetNetWorkSetUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNewNatGetNetWorkSetUpDlg * pGetNewNetWorkSetUpDlg = new CWzdNewNatGetNetWorkSetUpDlg();
	pGetNewNetWorkSetUpDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,bshow);
	pGetNewNetWorkSetUpDlg->SetWzdInfo(m_pNatWzdInfo);
	return pGetNewNetWorkSetUpDlg;
}

CWzdNatPageDlg* CWzdNewNatManDlg::CreateNewNatPasswordDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNewNatPasswordDlg * pNatEnableDlg = new CWzdNewNatPasswordDlg();
	pNatEnableDlg->SetWzdInfo(m_pNatWzdInfo);
	pNatEnableDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	return pNatEnableDlg;
}

CWzdNatPageDlg* CWzdNewNatManDlg::CreateRemoteAccessTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNewNatRemoteAccessTipDlg * pNewNatRemoteAccessTipDlg = new CWzdNewNatRemoteAccessTipDlg();
	pNewNatRemoteAccessTipDlg->SetWzdInfo(m_pNatWzdInfo);
	pNewNatRemoteAccessTipDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	return pNewNatRemoteAccessTipDlg;
}

CWzdNatPageDlg* CWzdNewNatManDlg::CreateLocalConnectEndDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNatLocalEndDlg *pNatLocalEndDlg = new CWzdNatLocalEndDlg();
	pNatLocalEndDlg->SetWzdInfo(m_pNatWzdInfo);
	pNatLocalEndDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	return pNatLocalEndDlg;
}

CWzdNatPageDlg* CWzdNewNatManDlg::CreateNewNatSelectDevDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	std::string strTip;
	// strTip.append("Q-See Scan N'VIEW");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_SELECT_DEV_02));
	strTip.append("\\n");
	// strTip.append("The next step will be to set up the network connection so you can view your cameras over your network and the Internet.");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_SELECT_DEV_03));
	strTip.append("\\n");
	// strTip.append("Choose whether you want to connect via Mobile Devices or Computer.");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_SELECT_DEV_04));
	strTip.append("\\n");


	CWzdNewNatSelectDevDlg *pNatSelectDevDlg = new CWzdNewNatSelectDevDlg();
	pNatSelectDevDlg -> SetDevSelectIndex(m_devSelect);
	pNatSelectDevDlg -> Create(GetRandWndID(),x,y,cx,cy,this,pageIndex,1,bshow);
	pNatSelectDevDlg -> SetWzdInfo(m_pNatWzdInfo);
	pNatSelectDevDlg -> SetTipInfo(strTip);
	return pNatSelectDevDlg;
}

CWzdNatPageDlg * CWzdNewNatManDlg::CreateNewNatQrCodeDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNewNatQrCodeDlg * pNatQrCodeDlg = new CWzdNewNatQrCodeDlg();
	pNatQrCodeDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	pNatQrCodeDlg->SetWzdInfo(m_pNatWzdInfo);
	return pNatQrCodeDlg;
}

CWzdNatPageDlg* CWzdNewNatManDlg::CreateNewNatPCTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	CWzdNewNatPCTipDlg *pNatTipDlg = new CWzdNewNatPCTipDlg();
	pNatTipDlg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	pNatTipDlg->SetWzdInfo(m_pNatWzdInfo);
	return pNatTipDlg;
}

CWzdNatPageDlg * CWzdNewNatManDlg::CreateNatEndUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex)
{
	string strTip;
	// strTip.append("The Q-See Start Up Wizard is finished.");
	strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_END_00));
	strTip.append("\\n");
	// strTip.append("Please disable the Start Up Wizard by unchecking the \"Open Wizard Next Time\" box ");
	// strTip.append("in the lower left corner, below, so it does not run again the next time you restart the DVR.");
	strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_END_01));
	strTip.append("\\n");
	// strTip.append("The QR code and other networking information may be found in the Information window.");
	strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_END_02));
	strTip.append("\\n");
	
	CWzdNatEndDlg *pWzdNatEndDLg = new CWzdNatEndDlg();
	pWzdNatEndDLg->Create(GetRandWndID(), x, y, cx, cy, this, pageIndex, 1, bshow);
	pWzdNatEndDLg->SetWzdInfo(m_pNatWzdInfo);
	pWzdNatEndDLg->SetTipInfo(strTip);
	return pWzdNatEndDLg;
}

BEGIN_MSG_MAP(CWzdNewNatManDlg, CCfgDlg)
	ON_MSG(CTRLID_NULL,KEY_VIRTUAL_LANGUAGE,OnChangeLanguage)
END_MSG_MAP()

unsigned long CWzdNewNatManDlg::OnChangeLanguage()
{
	g_bWizzardChangeLanguage = true;
	SaveNatWizard();
	g_bNotCloseWizzard = true;
	return KEY_VIRTUAL_MSG_STOP;
}
//end
