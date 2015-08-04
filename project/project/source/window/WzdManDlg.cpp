/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-02-26
** Name         : WzdManDlg.cpp
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/

#include "WzdManDlg.h"
#include "WzdBasicSetDlg.h"
#include "WzdTipDiskDlg.h"
#include "WzdTipNetDlg.h"
#include "MessageBox.h"
#include "CfgDlg.h"
#include "FrameRate.h"
#include "CtrlID.h"

extern bool	g_bCloseWzd;
CWzdManDlg::CWzdManDlg() : m_pWizardInfo(NULL),
m_curIndex(0),
m_sysDiskStatus(-1),
m_bEnterConfig(false),
m_curTime(0),
m_lastTime(0)
{

	m_maxTime = 10*60*1000000;
}

CWzdManDlg::~CWzdManDlg()
{

}

void CWzdManDlg::OnInitial()
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
	
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_WIZZARD), false);
	SetDefaultBtn( DEFAULT_BTN_OK |  DEFAULT_BTN_CANCEL |DEFAULT_BTN_APPLY);
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_NEXT) );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_PREV) );
	//////////////////////////////////////////////////////////////////////////
	//��ʼ��������
	CreateWizardInfo();
	//������ҳ��
	CreateWizardPage();
	//��ʼ��һҳ��
	m_curIndex = 0;
	//������,��Զ��IE���ڱ����򵼽�����ʱ,���޷�������������.
	//��ʱ��Ҫ��ʾ�û��ر�Զ�����÷��ɽ�������
	m_bEnterConfig = false;
	TryEnterConfig();

		////
	CreateTipWnd();
}

void CWzdManDlg::OnPostInitial()
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

void CWzdManDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	if( m_pWizardInfo )
	{
		delete m_pWizardInfo;
		m_pWizardInfo = NULL;
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

void CWzdManDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);
}

void CWzdManDlg::EndDialog(DLG_END_VALUE result)
{

}

void CWzdManDlg::SetDiskStatus(unsigned long status)
{
	m_sysDiskStatus = status;
}

bool CWzdManDlg::TryEnterConfig()
{
	if( !m_bEnterConfig )
	{
		m_bEnterConfig = EnterConfig();
	}

	return m_bEnterConfig;
}

unsigned long CWzdManDlg::OnOK()
{
	//��һҳ
	if( m_curIndex +1 < m_vecWzdPage.size() )
	{
		m_vecWzdPage[m_curIndex]->OnLeavePage();
		#if !defined(__SUPPORT_3G__) && !defined(__TW01_RILI__)
		if(m_curIndex == 1)
		{
			if(!dynamic_cast<CWzdTipNetDlg*>(m_vecWzdPage[1])->/*GetIpIsValid()*/CheckIPIfConflict())
			{
				MessageOut( GetTopestWnd(),static_cast<ENUM_STRING_ID>(dynamic_cast<CWzdTipNetDlg*>(m_vecWzdPage[1])->GetErrorTextNum()), TIP_ERR, true );
				m_curIndex -=1;
				printf("***** iscoltion ip error...here...%d...%s\n",__LINE__,__FILE__);
			}
		}
		#endif
		m_curIndex += 1;
		ShowPage(m_curIndex);
		ShowBtn(m_curIndex);
		m_vecWzdPage[m_curIndex]->ShowWzdData(false);
		Repaint();
	}
	else
	{
		//���ҳ���ȡ
		SaveWizard();
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CWzdManDlg::OnCancel()
{
	SaveWizard();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CWzdManDlg::OnApply()
{
	//��һҳ
	if( m_curIndex -1 >= 0 )
	{
		m_vecWzdPage[m_curIndex]->OnLeavePage();
		m_curIndex -= 1;
		ShowPage(m_curIndex);
		ShowBtn(m_curIndex);
		m_vecWzdPage[m_curIndex]->GetWzdData(false);
		m_vecWzdPage[m_curIndex]->ShowWzdData(false);

		Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

void CWzdManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	
	itemList.push_back(NCFG_ITEM_DEVICE_NAME);
	itemList.push_back(NCFG_ITEM_DATE_FORMAT);
	itemList.push_back(NCFG_ITEM_HOUR_FORMAT);
	itemList.push_back(NCFG_ITEM_LANGUAGE);
	itemList.push_back(NCFG_ITEM_TIME_ZONE);
	itemList.push_back(NCFG_ITEM_SHOW_WIZZARD);

	if(CProduct::Instance()->LocalVideoInputNum() > 0)
	{
		itemList.push_back(NCFG_ITEM_ENCODE_MAJOR);
		itemList.push_back(NCFG_ITEM_ENCODE_MAJOR_SPEC);
		itemList.push_back(NCFG_ITEM_ENCODE_MAJOR_EX);
		itemList.push_back(NCFG_ITEM_BITRANGE_FOR_VIDEOSIZE);
		itemList.push_back(NCFG_ITEM_MAJOR_CHANNEL_RECORD_PARAM_EX);
	}
	
	itemList.push_back(NCFG_ITEM_HTTP_PORT);
	itemList.push_back(NCFG_ITEM_SERVER_PORT);
	itemList.push_back(NCFG_ITEM_NETWORK_ADDR_INFO);
	itemList.push_back(NCFG_ITEM_DNS_1);
	itemList.push_back(NCFG_ITEM_DNS_2);

	itemList.push_back(NCFG_ITEM_DEVICE_NAME_MAX_LEN);
	itemList.push_back(NCFG_ITEM_SUPPORT_LANGUAGE);	

	itemList.push_back(NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY);
	itemList.push_back(NCFG_ITEM_MAJOR_SUPPORT_QULITY_LEVELNUM);
	itemList.push_back(NCFG_ITEM_MAJOR_SUPPORT_ENCODEMODE);
	itemList.push_back(NCFG_ITEM_MAJOR_SUPPORT_BITRATE_RANGE);
	itemList.push_back(NCFG_ITEM_ENCODE_SIZE_MAJOR_ALL_CH);
	
}

bool CWzdManDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);
	static bool bAuto = false;
	switch (cfgID)
	{
	case NCFG_ITEM_DEVICE_NAME:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME));
			NCFG_INFO_SHORT_NAME* pShortName = reinterpret_cast<NCFG_INFO_SHORT_NAME*>(pData);
			strcpy(pShortName->name,m_pWizardInfo->DeviceName);
		}
		return true;
		break;
	case NCFG_ITEM_LANGUAGE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue =m_pWizardInfo->CurrentLanguage;

		}
		return true;
		break;
	case NCFG_ITEM_DATE_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue =m_pWizardInfo->DateMode;
		}
		return true;
		break;
	case NCFG_ITEM_HOUR_FORMAT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue =m_pWizardInfo->TimeMode;
		}
		return true;
		break;
	case NCFG_ITEM_TIME_ZONE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pWizardInfo->TimeZone;
		}
		return true;
		break;
	case NCFG_ITEM_SHOW_WIZZARD:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue = m_pWizardInfo->ShowWzd;
		}
		return true;
		break;
	case NCFG_ITEM_HTTP_PORT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue =m_pWizardInfo->Port;
		}
		return true;
		break;
	case NCFG_ITEM_SERVER_PORT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			*pValue =m_pWizardInfo->ServerPort;
		}
		return true;
		break;
	case NCFG_ITEM_NETWORK_ADDR_INFO:
		{
			NETWORK_ADDR_INFO* pValue = reinterpret_cast<NETWORK_ADDR_INFO*>(pData);
			pValue->bDHCP =m_pWizardInfo->Addr_Info.bDHCP ;
			if (pValue->bDHCP)
			{
				NETWORK_STATUS_INFO networkStatusInfo;
				memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));
				MESSAGE_DATA msgData;
				CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETWORK_INFO, NULL, 0, msgData);

				networkStatusInfo = *reinterpret_cast<NETWORK_STATUS_INFO *>(msgData.pData+sizeof(PACKCMD));
				if (networkStatusInfo.IP != 0)
				{
					bAuto =true;
					pValue->staticIP = networkStatusInfo.IP;
					pValue->netMask= networkStatusInfo.subMask;
					pValue->gateway=networkStatusInfo.gateway;
				}
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			}
			//ip��������Ϣ�ı���
			pValue->staticIP = m_pWizardInfo->Addr_Info.staticIP;
			pValue->netMask= m_pWizardInfo->Addr_Info.netMask;
			pValue->gateway=m_pWizardInfo->Addr_Info.gateway;
		}
		return true;
		break;
	case NCFG_ITEM_DNS_1:
		{
			assert( len == sizeof(unsigned long) );
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			if (bAuto)
			{
				NETWORK_STATUS_INFO networkStatusInfo;
				memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));
				MESSAGE_DATA msgData;
				CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETWORK_INFO, NULL, 0, msgData);

				networkStatusInfo = *reinterpret_cast<NETWORK_STATUS_INFO *>(msgData.pData+sizeof(PACKCMD));
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
				if (networkStatusInfo.IP != 0)
				{
					*pValue = networkStatusInfo.DNS1;
					return true;
				}
			}
			*pValue=m_pWizardInfo->DNS_MainIP;
		}
		return true;
		break;
	case NCFG_ITEM_DNS_2:
		{
			assert( len == sizeof(unsigned long) );
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			if (bAuto)
			{
				NETWORK_STATUS_INFO networkStatusInfo;
				memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));
				MESSAGE_DATA msgData;
				CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETWORK_INFO, NULL, 0, msgData);

				networkStatusInfo = *reinterpret_cast<NETWORK_STATUS_INFO *>(msgData.pData+sizeof(PACKCMD));
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
				if (networkStatusInfo.IP != 0)
				{
					*pValue = networkStatusInfo.DNS2;
					return true;
				}
			}
			*pValue=m_pWizardInfo->DNS_SubIP;
		}
		return true;
		break;
	case NCFG_ITEM_ENCODE_MAJOR:
		{
			ENCODE_INFO* pInfo = reinterpret_cast<ENCODE_INFO*>(pData);
			for (int i=0; i<CProduct::Instance()->LocalVideoInputNum(); i++)
			{
				pInfo[i].resolution = m_pWizardInfo->InfoEx[i].resolution;
				pInfo[i].rate = m_pWizardInfo->InfoEx[i].rate;
				pInfo[i].quality = m_pWizardInfo->InfoEx[i].quality;
				pInfo[i].encodeType = m_pWizardInfo->InfoEx[i].encodeType;
				pInfo[i].hBitStream = m_pWizardInfo->InfoEx[i].hBitStream;
				pInfo[i].lBitStream = m_pWizardInfo->InfoEx[i].lBitStream;
			}
 		}
		return true;
		break;
	case NCFG_ITEM_ENCODE_MAJOR_EX:
 		{
			ENCODE_INFO_EX *pInfoEx = reinterpret_cast<ENCODE_INFO_EX*>(pData);
			for (int i = 0; i < CProduct::Instance()->VideoInputNum(); i++)
			{
				pInfoEx[i] = m_pWizardInfo->InfoEx[i];
			}
 		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CWzdManDlg::EnterConfig()
{
	//��������
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
			PUB_Sleep(50);
			tryCount --;
		}
	}

	GUI::CDialog::MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_26), m_pStrTable->String(E_STR_ID_TITLE_FAILED), DEFAULT_BTN_OK|DEFAULT_ICON_ERROR);

	return false;
}

void CWzdManDlg::ExitConfig() const
{
	//�˳�����
	CMessageMan* pMsgMan = CMessageMan::Instance();
	pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);
}

void CWzdManDlg::CreateWizardInfo()
{
	if( !m_pWizardInfo )
	{
		m_pWizardInfo = new WIZARD_INFO;
	}

	memset(m_pWizardInfo, 0, sizeof(WIZARD_INFO));
}

void CWzdManDlg::InitWizardInfo()
{
	/*
		���������еõ������ݳ�ʼ�����Ƶ��򵼽ṹ����
	*/
	unsigned char* pData = NULL;
	unsigned long dataLen = 0;
	CCfgDlg::GetConfig( NCFG_ITEM_DEVICE_NAME_MAX_LEN, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pWizardInfo->DevMaxNameLen=*reinterpret_cast<unsigned long*>(pData);
	CCfgDlg::GetConfig( NCFG_ITEM_DEVICE_NAME, &pData, dataLen );
	const NCFG_INFO_SHORT_NAME* pShortName=reinterpret_cast<const NCFG_INFO_SHORT_NAME*>(pData);
	strcpy(m_pWizardInfo->DeviceName,pShortName->name);

	//��ȡ����
	CCfgDlg::GetConfig( NCFG_ITEM_SUPPORT_LANGUAGE, &pData, dataLen );
	if( dataLen>0)
	{
		memcpy(&m_pWizardInfo->Language,pData,dataLen);
		m_pWizardInfo->LanguageNum=dataLen/sizeof(NCFG_INFO_LANGUAGE);
	}
	CCfgDlg::GetConfig( NCFG_ITEM_LANGUAGE, &pData, dataLen );
	if( dataLen>0)
	{
		m_pWizardInfo->CurrentLanguage=*reinterpret_cast<unsigned long*>(pData);
	}

	//��ȡ���ڸ�ʽ
	CCfgDlg::GetConfig(NCFG_ITEM_DATE_FORMAT, &pData, dataLen);
	m_pWizardInfo->DateMode = *reinterpret_cast<unsigned long *>(pData);

	//��ȡʱ���ʽ
	CCfgDlg::GetConfig(NCFG_ITEM_HOUR_FORMAT, &pData, dataLen);
	m_pWizardInfo->TimeMode = *reinterpret_cast<unsigned long *>(pData);

	// ��ȡʱ��
	CCfgDlg::GetConfig(NCFG_ITEM_TIME_ZONE, &pData, dataLen);
	assert( dataLen == sizeof(unsigned long) );
	m_pWizardInfo->TimeZone = *reinterpret_cast<unsigned long*>(pData);

	//�´��Ƿ���ʾ��
	CCfgDlg::GetConfig(NCFG_ITEM_SHOW_WIZZARD, &pData, dataLen);
	assert(dataLen == sizeof(unsigned long));
	m_pWizardInfo->ShowWzd = *reinterpret_cast<unsigned long*>(pData);

	//��ȡ��ǰhttp�˿ں�
	CCfgDlg::GetConfig( NCFG_ITEM_HTTP_PORT, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pWizardInfo->Port=*reinterpret_cast<unsigned long*>(pData);

	//��ȡ����˿ں�
	CCfgDlg::GetConfig( NCFG_ITEM_SERVER_PORT, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long));
	m_pWizardInfo->ServerPort=*reinterpret_cast<unsigned long*>(pData);

	//��ȡ�����ַ��Ϣ
	CCfgDlg::GetConfig( NCFG_ITEM_NETWORK_ADDR_INFO, &pData, dataLen );
	m_pWizardInfo->Addr_Info=*reinterpret_cast<const NETWORK_ADDR_INFO*>(pData);

	//��ȡ������������
	CCfgDlg::GetConfig( NCFG_ITEM_DNS_1, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long) );
	m_pWizardInfo->DNS_MainIP= *reinterpret_cast<const unsigned long*>(pData);

	//��ȡ������������
	CCfgDlg::GetConfig( NCFG_ITEM_DNS_2, &pData, dataLen );
	assert( dataLen == sizeof(unsigned long) );
	m_pWizardInfo->DNS_SubIP= *reinterpret_cast<const unsigned long*>(pData);
	
	//��ȡ���� ����ֱ��ʣ�֡����أ�
	CCfgDlg::GetConfig( NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY, &pData, dataLen);
	assert(dataLen > 0);
	m_pWizardInfo->pFrameRateData=(reinterpret_cast<NCFG_INFO_SUPPORT_FRAME_RATE*>(pData));	
	
	
	//��ȡ���� ����������أ�
	CCfgDlg::GetConfig(NCFG_ITEM_MAJOR_SUPPORT_QULITY_LEVELNUM, &pData, dataLen);
	unsigned long *pTempValue = reinterpret_cast<unsigned long *>(pData);
	m_pWizardInfo->QualityLevelNum=dataLen/sizeof(unsigned long) - 1;
	m_pWizardInfo->defauQualityLevel = pTempValue[m_pWizardInfo->QualityLevelNum];
	memcpy(m_pWizardInfo->QualityLevelValue,pData,dataLen - sizeof(unsigned long));
	CCfgDlg::GetConfig(NCFG_ITEM_MAJOR_SUPPORT_ENCODEMODE, &pData, dataLen);
	assert(dataLen/sizeof(unsigned long) == 2);
	pTempValue  = reinterpret_cast<unsigned long *>(pData); 
	m_pWizardInfo->videoEncodeMode = pTempValue[0];
	m_pWizardInfo->defaVideoEncodeMode = pTempValue[1];

	CCfgDlg::GetConfig(NCFG_ITEM_MAJOR_SUPPORT_BITRATE_RANGE, &pData, dataLen);
	m_pWizardInfo->bitStreamCount = dataLen/sizeof(unsigned long) - 1;
	assert(m_pWizardInfo->bitStreamCount> 0);
	pTempValue = reinterpret_cast<unsigned long *>(pData);
	m_pWizardInfo->defaBitStreamValue = pTempValue[m_pWizardInfo->bitStreamCount];
	memcpy(m_pWizardInfo->BitStreamValue, pData, dataLen - sizeof(unsigned long));	
	CCfgDlg::GetConfig(NCFG_ITEM_ENCODE_SIZE_MAJOR_ALL_CH, &pData, dataLen);
	m_pWizardInfo->bAllCH = *reinterpret_cast<unsigned long *>(pData);

	CCfgDlg::GetConfig(NCFG_ITEM_ENCODE_MAJOR_SPEC, &pData, dataLen);
	m_pWizardInfo->FrameRateSpec = *reinterpret_cast<NCFG_INFO_ENCODE_SPEC_FRAME_RATE*>(pData);

	//��ȡIPC��¼�����ò��� added by wn 2011-11-28
	CCfgDlg::GetConfig(NCFG_ITEM_ENCODE_MAJOR_EX, &pData, dataLen);
	memcpy(&m_pWizardInfo->InfoEx, pData, dataLen);
	
	//����ͨ����������NCFG_ITEM_ENCODE_MAJOR
	if(CProduct::Instance()->LocalVideoInputNum() > 0)
	{
		CCfgDlg::GetConfig(NCFG_ITEM_ENCODE_MAJOR, &pData, dataLen);
		for(int i = 0; i < CProduct::Instance()->LocalVideoInputNum(); i++)
		{
			ENCODE_INFO *pEncodeInfo = (ENCODE_INFO*)(pData);
			m_pWizardInfo->InfoEx[i].resolution = pEncodeInfo[i].resolution;
			m_pWizardInfo->InfoEx[i].rate = pEncodeInfo[i].rate;
			m_pWizardInfo->InfoEx[i].quality = pEncodeInfo[i].quality;
			m_pWizardInfo->InfoEx[i].encodeType = pEncodeInfo[i].encodeType;
			m_pWizardInfo->InfoEx[i].hBitStream = pEncodeInfo[i].hBitStream;
			m_pWizardInfo->InfoEx[i].lBitStream = pEncodeInfo[i].lBitStream;
		}
	}
	CCfgDlg::GetConfig( NCFG_ITEM_BITRANGE_FOR_VIDEOSIZE, &pData, dataLen);
	m_pWizardInfo->pVSBit=(reinterpret_cast<VS_SUPBRR*>(pData));	

	CCfgDlg::GetConfig( NCFG_ITEM_MAJOR_CHANNEL_RECORD_PARAM_EX, &pData, dataLen);
	m_pWizardInfo->pChRecp=(reinterpret_cast<CHANNEL_RECP*>(pData));	
}

void CWzdManDlg::CreateWizardPage()
{
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] ;

	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

	bool bSetShow = true;

	CWzdBasicSetDlg* pWzdStep1 = new CWzdBasicSetDlg();
	if( pWzdStep1 )
	{
		pWzdStep1->Create(GetRandWndID(), x, y, cx, cy, this, 0, 1, bSetShow);
		pWzdStep1->SetWzdInfo(m_pWizardInfo);
		bSetShow = false;
		m_vecWzdPage.push_back(pWzdStep1);		
	}

#if !defined(__SUPPORT_3G__) && !defined(__TW01_RILI__)
	//�����򵼵���������ҳ��̫�ʺϴ�3G���ܵİ汾�����޸ıȽ��鷳���򵥵��޸ķ����ǣ�
	//3G�汾�Ŀ������У����������繦��
	CWzdPageDlg *pWzdNet=CreateTipNetDlg(x,y,cx,cy,bSetShow);
	if( NULL != pWzdNet )
	{
		bSetShow = false;
		m_vecWzdPage.push_back(pWzdNet);
	}
#endif
	if(CProduct::Instance()->LocalVideoInputNum() > 0 && 0 == CProduct::Instance()->NetVideoInputNum())
	{
		CWzdRecordStreamDlg *pWzdRecord=CreateTipRecordDlg(x,y,cx,cy,bSetShow);
		if( NULL != pWzdRecord )
		{
			bSetShow = false;
			m_vecWzdPage.push_back(pWzdRecord);
		}
	}
	

	CWzdDiskManDlg *pWzdDiskMan=CreateDiskManDlg(x,y,cx,cy,bSetShow);
	if( NULL != pWzdDiskMan )
	{
		bSetShow = false;
		m_vecWzdPage.push_back(pWzdDiskMan);
	}

	CWzdPageDlg* pWzdPage = CreateTipDiskDlg(x, y, cx, cy, bSetShow);
	if( NULL != pWzdPage )
	{
		bSetShow = false;
		m_vecWzdPage.push_back(pWzdPage);
	}
}

void CWzdManDlg::ShowPage(int index)
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

void CWzdManDlg::ShowBtn(int index)
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
			//�м�ҳ��
			pBtnApp->SetEnable(true, false);
			pBtnOk->SetEnable(true, false);
			pBtnOk->SetEnable(true,false);
			pBtnOk->SetCaption(m_pStrTable->String(E_STR_ID_NEXT), false);
		}
	
}

void CWzdManDlg::OnWzdEnd()
{
	assert(0 != m_pWizardInfo->Modified);
	SetModify(true);

	//�������ڡ�ʱ��ؼ���ʾģʽ����
	unsigned long dateMode = CDateCtrl::DateMode(m_pWizardInfo->DateMode);

	if (dateMode != CDateCtrl::GetDateMode())
	{
		CDateCtrl::SetDateMode(dateMode, true);
	}

	unsigned long timeMode = CTimeCtrl::TimeMode(m_pWizardInfo->TimeMode);

	if (timeMode != CTimeCtrl::GetTimeMode())
	{
		CTimeCtrl::SetTimeMode(timeMode, true);
	}

	//�޸�ϵͳʱ��
	ZONE_TIME_INFO zoneTimInfo;
	zoneTimInfo.UseTime = 1;
	zoneTimInfo.UseZone = 1;
	zoneTimInfo.TimeZone = m_pWizardInfo->TimeZone;

	CWzdBasicSetDlg* pWzdBasic = dynamic_cast<CWzdBasicSetDlg*>(m_vecWzdPage[0]);
	assert(pWzdBasic);

	if(pWzdBasic->GetCurrZoneTimeInfo(zoneTimInfo, zoneTimInfo.TimeZone))
	{
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_L_CHANGE_ZONE_TIME,(unsigned char*)&zoneTimInfo, sizeof(ZONE_TIME_INFO));
	}

	//������������
	CCfgDlg::OnOK();
}

CWzdPageDlg* CWzdManDlg::CreateTipDiskDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show)
{
	unsigned long sysDiskStatus = m_sysDiskStatus;
	bool bShowWzd = false;
	std::string strTip;

	if( sysDiskStatus != -1 )
	{
		if( 0 == sysDiskStatus )
		{
			strTip = m_pStrTable->String(E_STR_ID_MESSAGE_102);
			strTip.append(m_pStrTable->String(E_STR_ID_MESSAGE_72));
			strTip.append("\\n");
			strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_08));
			strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_09));
			strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_10));
			strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_11));
			bShowWzd = true;
		}
		else
		{

			bool bHaveUnFormatedDisk = false;
			bool bHaveReadeWriteDisk = false;
			bool bHaveRedunDisk = false;
			bool bHaveOnlyReadDisk = false;
			if (sysDiskStatus & NET_DISK_HAVE_UNFORMATED_DISK)
			{
				bHaveUnFormatedDisk = true;
			}
			if (sysDiskStatus & NET_DISK_HAVE_READEWRITE_DISK)
			{
				bHaveReadeWriteDisk = true;
			}
			if (sysDiskStatus & NET_DISK_HAVE_REDUN_DISK)
			{
				bHaveRedunDisk = true;
			}
			if (sysDiskStatus & NET_DISK_HAVE_ONLYREADE_DISK)
			{
				bHaveOnlyReadDisk = true;
			}
			if (bHaveReadeWriteDisk)
			{
				if (bHaveUnFormatedDisk)
				{
					//�ж�дӲ�̲�����δ��ʽ����Ӳ��
					//MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_73), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					bShowWzd = true;
					strTip = m_pStrTable->String(E_STR_ID_MESSAGE_102);
					strTip.append(m_pStrTable->String(E_STR_ID_MESSAGE_73));
					strTip.append("\\n");
					strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_01));
					strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_02));
					strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_03));
					strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_04));
				}
				else
				{
					//�ж�дӲ�̲���û��δ��ʽ����Ӳ��
					//������ʾ
				}
			}
			else
			{
				if (bHaveRedunDisk)
				{
					if (bHaveUnFormatedDisk)
					{
						//ֻ�ܽ�������¼������δ��ʽ����Ӳ��			
						bShowWzd = true;
						strTip = m_pStrTable->String(E_STR_ID_MESSAGE_102);
						strTip.append(m_pStrTable->String(E_STR_ID_MESSAGE_73));
						strTip.append("\\n");
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_01));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_02));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_03));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_04));
					}
					else
					{
						//ֻ�ܽ�������¼����û��δ��ʽ����Ӳ��
						bShowWzd = true;
						strTip = m_pStrTable->String(E_STR_ID_MESSAGE_102);
						strTip.append(m_pStrTable->String(E_STR_ID_MESSAGE_75));
						strTip.append("\\n");
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_05));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_02));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_06));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_07));
					}
				}
				else
				{
					if (bHaveOnlyReadDisk)
					{
						//����ֻ����ʽ������δ��ʽ����Ӳ��				
						bShowWzd = true;
						strTip = m_pStrTable->String(E_STR_ID_MESSAGE_102);
						strTip.append(m_pStrTable->String(E_STR_ID_MESSAGE_73));
						strTip.append("\\n");
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_01));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_02));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_03));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_04));
					}
					else
					{
						assert(bHaveUnFormatedDisk);
						bShowWzd = true;
						strTip = m_pStrTable->String(E_STR_ID_MESSAGE_102);
						strTip.append(m_pStrTable->String(E_STR_ID_MESSAGE_73));
						strTip.append("\\n");
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_01));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_02));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_03));
						strTip.append(m_pStrTable->String(E_STR_ID_TYPESET_MSG_04));
					}
				}
			}
		}
	}

	if( bShowWzd )
	{
		CWzdTipDiskDlg* pTipDisk = new CWzdTipDiskDlg();
		pTipDisk->Create(GetRandWndID(), x, y, cx, cy, this, 4, 1, show);
		pTipDisk->SetWzdInfo(m_pWizardInfo);
		pTipDisk->SetTipDisk(strTip);

		return pTipDisk;
	}
	else
	{
		return NULL;
	}
}
CWzdTipNetDlg* CWzdManDlg::CreateTipNetDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show )
{
	CWzdTipNetDlg* pTipNet = new CWzdTipNetDlg();
	if(pTipNet!=NULL)
		{
			pTipNet->SetWzdInfo(m_pWizardInfo);
			pTipNet->Create(GetRandWndID(), x, y, cx, cy, this, 1, 1, show);
			return pTipNet;
			
		}
	else
		return NULL;

}

CWzdRecordStreamDlg *CWzdManDlg::CreateTipRecordDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show )
{
	CWzdRecordStreamDlg* pRecord = new CWzdRecordStreamDlg();
	if(pRecord!=NULL)
	{
		pRecord->SetWzdInfo(m_pWizardInfo);
		pRecord->Create(GetRandWndID(), x, y, cx, cy, this, 2, 1, show);
		return pRecord;

	}
	else
		return NULL;
}

CWzdDiskManDlg *CWzdManDlg::CreateDiskManDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show )
{
	CWzdDiskManDlg* pRecord = new CWzdDiskManDlg();
	if(pRecord!=NULL)
	{
		pRecord->SetWzdInfo(m_pWizardInfo);
		pRecord->Create(GetRandWndID(), x, y, cx, cy, this, 3, 1, show);
		return pRecord;

	}
	else
		return NULL;
}

unsigned long CWzdManDlg::SaveWizard()
{
	//��ȡ����
	for(int i = 0; i < m_curIndex; i++)
	{
		m_vecWzdPage[i]->GetWzdData(true);
		m_pWizardInfo->Modified = 1;
	}

	if(0 != m_pWizardInfo->Modified)
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

void CWzdManDlg::OnIdle()
{
	m_curTime = GetCurrTime();
	if (((m_lastTime > 0) && ((m_curTime - m_lastTime) > m_maxTime)) || (g_bCloseWzd == true))
	{
		SaveWizard();
	}
	GUI::CDialog::OnIdle();
}

