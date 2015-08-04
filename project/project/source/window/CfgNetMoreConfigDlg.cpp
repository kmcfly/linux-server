/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-06-9
** Name         : CfgNetMoreConfigDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:	2010-08-03
	���dynamic.visionica.com�Ķ�̬���������ܡ���DDNS��������ҪDVRȥ
	������ע�ᣬ����Ҫ�û���������,ֻ��Ҫһ���û��������macΪ�ؼ��ֵ�������
	
	��2��LIST��ʵ�֣�һ��������ͨ��DDNS����һ������visionica��ddns��
	��̬������Ϣ�������ʱ����m_ddnsInfo�У�����ʹ�õ�DDNS�������Ĳ�ͬ
	��m_ddnsInfo�е�����д�����LIST�С��ı�LIST������ʱ������������Ͱ�
	m_ddnsInfo����ӦԪ�ظ��¡��������ڵ����ݲ�����ʽ������
2:2010-09-11
	�����ͻ�������������dynamic.visionica.com��ע��Э���޸�Ϊhttp://dynamic.visionica.com:8080/nic/update?mode=0&hostname=7aeb1e3d4d2a&myip=83.37.17.242&platform=windows&email=henglong84@163.com&devicemodel=TVT-1600B&domain=visionica.info
	�ĸ�ʽ������ֻ����mac��ע������(֮ǰ���������д�ַ�����ע��)�����������urlע�������Ϊ7aeb1e3d4d2a.visionica.info��
	Ϊ�ˣ���visionica.com��ע�������ֱ����mac��ַ��д�����Ҳ������룬 ֻ���������û���
***********************************************************************/

#include "CfgNetMoreConfigDlg.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

CCfgNetMoreConfigDlg::CCfgNetMoreConfigDlg()
{
	m_bAutoReg = false;
}

CCfgNetMoreConfigDlg::~CCfgNetMoreConfigDlg()
{

}

void CCfgNetMoreConfigDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	InitialListF(false);
	InitialListS(false);

	m_cfgNetMoreListF.Show(false, false);
	m_cfgNetMoreListS.Show(false, false);
	

	m_bCheckParam = false;
	m_StatusddnsCheckOK = CHECK_STATUS_NOCHANGE;
}

unsigned long CCfgNetMoreConfigDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if( m_bCheckParam )
	{
		//�ڼ��DDNS������ʱ������������Ϣ��Ӧ
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return CCfgDlg::OnPreCommand( ctrlID, keyID ,xPos, yPos );
	}
}

void CCfgNetMoreConfigDlg::SetTip()
{
	GUI::CWnd* pWnd = m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TEST, 1);
	if( NULL != pWnd )
	{
		AddTip(pWnd->GetID(), E_STR_ID_TIP_05_02);
	}
	else
	{
		printf("%s:%s:%d, SetTip for DDNS Test error\n", __FUNCTION__,__FILE__, __LINE__);
		assert(false);
	}

	pWnd = m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_REGISTER, 1);
	if( NULL != pWnd )
	{
		AddTip(pWnd->GetID(), E_STR_ID_TIP_05);
	}
	else
	{
		printf("%s:%s:%d, SetTip for DDNS register error\n", __FUNCTION__,__FILE__, __LINE__);
		assert(false);
	}
}

bool CCfgNetMoreConfigDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	if( (NULL == pData) || (0 >= len) )
	{
		assert(false);
		return false;
	}

	switch (cfgID)
	{
	case NCFG_ITEM_UPNP_ENABLE:
		{
			GUI::CCheckCtrl *pCheck =NULL;
			m_bUpnp = *reinterpret_cast<const unsigned long*>(pData);
			
			{
				pCheck = dynamic_cast<GUI::CCheckCtrl *>(m_cfgNetMoreListF.GetItemWnd(ROW_UPNP_ENABLE,1));
				if (m_bUpnp)
				{					
					pCheck->SetCheck(true, bRefresh);
				}
				else
				{
					pCheck->SetCheck(false, bRefresh);
				}
			}

			{
				pCheck = dynamic_cast<GUI::CCheckCtrl *>(m_cfgNetMoreListS.GetItemWnd(ROW_S_UPNP_ENABLE,1));
				if (m_bUpnp)
				{					
					pCheck->SetCheck(true, bRefresh);
				}
				else
				{
					pCheck->SetCheck(false, bRefresh);
				}

			}
		}
		return true;
	case NCFG_ITEM_DDNS_INFO:
		{
			BYTE* pTempData = NULL;
			unsigned long tempDataLen = 0;

			assert( len == sizeof(DDNS_INFO) );
			CCfgDlg::GetConfig( NCFG_ITEM_DDNS_SERVER_INFO, &pTempData, tempDataLen );
			m_pDdnsSeverInfo = reinterpret_cast<DDNS_SERVER_INFO*>(pTempData);
			assert( NULL != m_pDdnsSeverInfo );

			m_ddnsServerCount = tempDataLen / sizeof(DDNS_SERVER_INFO);
			assert( 0 == tempDataLen % sizeof( DDNS_SERVER_INFO ) );

			const DDNS_INFO* pDDNS = reinterpret_cast<const DDNS_INFO*>(pData);
			m_ddnsInfo = *pDDNS;

			RefreshListF(pDDNS, bRefresh);
			
			RefreshListS(pDDNS, bRefresh);
			
			m_ddnsInfo = *pDDNS;

			if ( DDNS_VISIONICA == pDDNS->type)
			{
				SetMacToDomianInfo(pDDNS);

 				if (!m_cfgNetMoreListS.IsShow())
 				{
 					m_cfgNetMoreListS.Show(true, bRefresh);
 				}
 
 				if (m_cfgNetMoreListF.IsShow())
 				{
 					m_cfgNetMoreListF.Show(false, bRefresh);
 				}

				m_useWhichList = USE_SECOND_LIST;

				CheckEnableUIS( bRefresh );
			} 
			else
			{
				if (m_cfgNetMoreListS.IsShow())
				{
					m_cfgNetMoreListS.Show(false, bRefresh);
				}

				if (!m_cfgNetMoreListF.IsShow())
				{
					m_cfgNetMoreListF.Show(true, bRefresh);
				}

				m_useWhichList = USE_FIRST_LIST;

				CheckEnableUI( bRefresh );
			}
		
			
		}
		return true;
	case NCFG_ITEM_DDNS_UPDATE_CYCLE:
		{
			m_ddnsUpdateCycle = (*reinterpret_cast<const unsigned long *>(pData) );
			GUI::CComboCtrl* pCombo = NULL;

			if (USE_FIRST_LIST == m_useWhichList)
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1));
			} 
			else
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_UPDATE_CYCLE,1));
			}
			
			assert(NULL != pCombo);

			pCombo->SetCurItemData(m_ddnsUpdateCycle, false);
		}
		return true;
	case NCFG_ITEM_MINT_DDNS_SERVER_NAME:
		{
			GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_SERVER_NAME,1));
			assert(pEdit);
			pEdit->SetCaption((const char *)pData,bRefresh);
			strcpy((char *)m_ddnsServerName, (const char *)pData);
		}
		return true;
	default:
		break;
	}

	return false;
}

bool CCfgNetMoreConfigDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);
	
	switch (cfgID)
	{
	case NCFG_ITEM_UPNP_ENABLE:
		{
			assert( len == sizeof(unsigned long));
			memcpy(pData, &m_bUpnp, sizeof(unsigned long));
		}
		return true;
	case NCFG_ITEM_DDNS_INFO:
		{
			assert( len == sizeof(DDNS_INFO) );
			memcpy(pData, &m_ddnsInfo, sizeof(DDNS_INFO));
		}

		return true;
	case NCFG_ITEM_DDNS_UPDATE_CYCLE:
		{
			*reinterpret_cast<unsigned long *>(pData) = m_ddnsUpdateCycle;
		}

		return true;
	case NCFG_ITEM_MINT_DDNS_SERVER_NAME:
		{
			memcpy(pData, m_ddnsServerName, len);
		}
		return true;
	default:
		break;
	}

	return false;
}


void CCfgNetMoreConfigDlg::CheckEnableUI( bool bRefresh )
{
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetMoreListF.GetItemWnd( ROW_DDNS_ENABLE, 1 ));
	LONGLONG curtype = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TYPE,1))->GetCurItemData();
	if( NULL != pCheck )
	{
		bool bEnableDdns = pCheck->IsCheck();
		m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TYPE,1)->SetEnable(bEnableDdns,bRefresh);
		if ((DDNS_MINTDNS == curtype) || (DDNS_TVT_IP_SERVER == curtype))
		{
			//mint ���͵�DDNS��������TVT��IP Server��������DDNS���������֡��������͵ķ���������������DDNS������������
			m_cfgNetMoreListF.AddItem( ROW_DDNS_SERVER_NAME, 0, m_pStrTable->String(E_STR_ID_SERVER), bRefresh );
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->Show(true,bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->SetCaption((const char *)m_ddnsServerName,bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->SetEnable(bEnableDdns,bRefresh);
		}
		else
		{
			m_cfgNetMoreListF.AddItem( ROW_DDNS_SERVER_NAME, 0, "", bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->SetCaption("",bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->SetEnable(false,bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->Show(false,bRefresh);
		}
#ifdef __CUSTOM_BR27__
		GUI::CEditStaticCtrl * pEditStatic = NULL;
		pEditStatic = dynamic_cast<GUI::CEditStaticCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_USER_NAME,1));
		if(DDNS_TECVOZ == curtype)
		{
			pEditStatic->SetStr(".TecvozDDNS.com.br");
		}
		else if(DDNS_WINCO_BR27 == curtype)
		{
			pEditStatic->SetStr(".ddns.com.br");
		}
		else
		{
			pEditStatic->SetStr("");
		}
#endif
		
		if(DDNS_MINT_AUTO == curtype)
		{
			m_bAutoReg = true;
		}
		else
		{
			m_bAutoReg = false;
		}

		//IN23�л�ʱ�û����������������Ϊ��
		if(!m_bAutoReg)
		{
			if(DDNS_TECVOZ == curtype)
			{
				m_cfgNetMoreListF.AddItem( ROW_DDNS_USER_NAME, 0, m_pStrTable->String(E_STR_ID_SUBDOMAIN), bRefresh);
			}
			else
			{
				m_cfgNetMoreListF.AddItem( ROW_DDNS_USER_NAME, 0, m_pStrTable->String(E_STR_ID_USER_NAME), bRefresh);
			}
			
#if defined(__CUSTOM_IN23__)
			if(m_ddnsInfo.type != curtype)
			{
				m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_USER_NAME,1)->SetCaption("",bRefresh);
			}
#endif
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_USER_NAME,1)->SetEnable(bEnableDdns,bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_USER_NAME,1)->Show(true,bRefresh);

			m_cfgNetMoreListF.AddItem( ROW_DDNS_PASSWORD, 0, m_pStrTable->String(E_STR_ID_PASSWORD), bRefresh);
#if defined(__CUSTOM_IN23__)
			if(m_ddnsInfo.type != curtype)
			{
				m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_PASSWORD,1)->SetCaption("",bRefresh);
			}
#endif
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_PASSWORD,1)->SetEnable(bEnableDdns,bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_PASSWORD,1)->Show(true,bRefresh);
		}
		else
		{
			m_cfgNetMoreListF.AddItem( ROW_DDNS_USER_NAME, 0, "", bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_USER_NAME,1)->SetCaption("",bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_USER_NAME,1)->SetEnable(false,bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_USER_NAME,1)->Show(false,bRefresh);
			
			m_cfgNetMoreListF.AddItem( ROW_DDNS_PASSWORD, 0, "", bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_PASSWORD,1)->SetCaption("",bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_PASSWORD,1)->SetEnable(false,bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_PASSWORD,1)->Show(false,bRefresh);
		}
		
		if(DDNS_TECVOZ == curtype)
		{
			m_cfgNetMoreListF.AddItem( ROW_DDNS_DOMAIN_INFO, 0, "", bRefresh );
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1)->SetEnable(false,bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1)->Show(false,bRefresh);
		}
		else
		{
			m_cfgNetMoreListF.AddItem( ROW_DDNS_DOMAIN_INFO, 0, m_pStrTable->String(E_STR_ID_HOST_DOMAIN), bRefresh );
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1)->SetEnable(bEnableDdns,bRefresh);
#if defined(__CUSTOM_BR27__) || defined(__AUTOREG__)
			GUI::CEditStaticCtrl * pEditDomainInfo = NULL;
			pEditDomainInfo = dynamic_cast<GUI::CEditStaticCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
			if(DDNS_WINCO_BR27 == curtype)
			{
				pEditDomainInfo->SetStr(".ddns.com.br");
			}
			else if(DDNS_MINT_AUTO == curtype)
			{
#if defined(__CUSTOM_IN23__)
				pEditDomainInfo->SetStr(".cpplusddns.com");
#else
				pEditDomainInfo->SetStr(".autoddns.com");
#endif
			}
			else
			{
				pEditDomainInfo->SetStr("");
			}

#if defined(__CUSTOM_IN23__)
			if(m_ddnsInfo.type != curtype)
			{
				pEditDomainInfo->SetCaption("",bRefresh);
			}

#endif
#endif
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1)->Show(true,bRefresh);

		}

		if ((DDNS_DYNDNS == curtype) || (DDNS_TECVOZ == curtype)||(DDNS_WINCO_BR27 == curtype)||(DDNS_MINT_AUTO == curtype))
		{
			//dyndns���͵ķ���������������������ڡ�ϵͳĬ��ʹ��һ���¸���һ��
			m_cfgNetMoreListF.AddItem( ROW_DDNS_UPDATE_CYCLE, 0, "", bRefresh );
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1)->SetEnable(false,bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1)->Show(false,bRefresh);
		}
		else
		{
			m_cfgNetMoreListF.AddItem( ROW_DDNS_UPDATE_CYCLE, 0, m_pStrTable->String(E_STR_ID_UPDATE_CYCLE), bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1)->SetEnable(bEnableDdns,bRefresh);
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1)->Show(true,bRefresh);
		}
		m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TEST,1)->SetEnable(bEnableDdns,bRefresh);
		if(m_bAutoReg)
		{
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TEST,1)->SetCaption(m_pStrTable->String(E_STR_ID_REGISTER), bRefresh);
		}
		else
		{
			m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TEST,1)->SetCaption(m_pStrTable->String(E_STR_ID_TEST),bRefresh);
		}
		if(bEnableDdns)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TYPE,1));
			if( DDNS_NONE == pCombo->GetCurItemData() )
			{
				//ѡ���DDNS������Ϊ��ʱ���ܽ���test����
				dynamic_cast<GUI::CButton*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TEST,1))->SetEnable( false,bRefresh);
			}
			else
			{
				dynamic_cast<GUI::CButton*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TEST,1))->SetEnable( bEnableDdns,bRefresh);
				//�����Ƿ���Ҫ����������Ϣ
				for( int i=0; i<m_ddnsServerCount; i++ )
				{
					if( pCombo->GetCurItemData() == m_pDdnsSeverInfo[i].ucDDNSID )
					{
						if( NCFG_ENUM_DDNS_SUPPORT_DOMAIN1 != m_pDdnsSeverInfo[i].supportproperty )
						{
							m_cfgNetMoreListF.GetItemWnd( ROW_DDNS_DOMAIN_INFO, 1 )->SetEnable(false, bRefresh);
						}
						break;
					}			
				}
			}
		}
	}
	else
	{
		printf("%s:%s:%d, GetItemWnd( ROW_DDNS_ENABLE, 1 ) error\n", __FUNCTION__, __FILE__, __LINE__);
		assert( false );
	}
#if defined(__CUSTOM_IN23__)
	GUI::CButton * pDelBtn = dynamic_cast<GUI::CButton*>(m_cfgNetMoreListF.GetItemWnd(ROW_NUM,1));
	assert(NULL != pDelBtn);
	if(DDNS_MINT_AUTO == curtype)
	{
		pDelBtn->Show(true,bRefresh);

	}
	else
	{
		pDelBtn->Show(false,bRefresh);
	}
#endif

}


void CCfgNetMoreConfigDlg::CheckEnableUIS( bool bRefresh )
{
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetMoreListS.GetItemWnd( ROW_S_DDNS_ENABLE, 1 ));
	if( NULL != pCheck )
	{
		bool bEnableDdns = pCheck->IsCheck();
		m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_TYPE,1)->SetEnable(bEnableDdns,bRefresh);
		m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_MAIL_ADDR,1)->SetEnable(bEnableDdns,bRefresh);
		m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_DOMAIN_INFO,1)->SetEnable(false,bRefresh);
		m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_UPDATE_CYCLE,1)->SetEnable(bEnableDdns,bRefresh);
		m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_REGISTER,1)->SetEnable(bEnableDdns,bRefresh);

		if(bEnableDdns)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_TYPE,1));
			if( DDNS_NONE == pCombo->GetCurItemData() )
			{
				//ѡ���DDNS������Ϊ��ʱ���ܽ���test����
				assert(false);
				dynamic_cast<GUI::CButton*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_REGISTER,1))->SetEnable( false,bRefresh);
			}
			else
			{
				dynamic_cast<GUI::CButton*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_REGISTER,1))->SetEnable( bEnableDdns,bRefresh);

			}
		}
	}
	else
	{
		printf("%s:%s:%d, GetItemWnd( ROW_S_DDNS_ENABLE, 1 ) error\n", __FUNCTION__, __FILE__, __LINE__);
		assert( false );
	}

}

unsigned long CCfgNetMoreConfigDlg::CheckDdnsParam()
{
	//����
	m_bCheckParam = true;

	MessageOut( GetTopestWnd(), E_STR_ID_PLEASE_WAIT, TIP_ERR );

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	DDNS_INFO_EX ddnsInfoEx;
	memcpy(&ddnsInfoEx, &m_ddnsInfo, sizeof(DDNS_INFO));
	memcpy(ddnsInfoEx.serverDNS, m_ddnsServerName, sizeof(ddnsInfoEx.serverDNS));

	CMessageMan::Instance()->SendMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETPARAMCHECK_DDNS, reinterpret_cast<unsigned char *>(&ddnsInfoEx), sizeof(DDNS_INFO_EX), msgData );

	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	switch( static_cast<NET_PROTOCOL>(pCmd->cmdType) )
	{
	case CMD_REPLY_NETPARAMCHECK_DDNS_SUCC:
	case CMD_REPLY_IN23_DOMAIN_CREATE_SUCC:
	case CMD_REPLY_IN23_DOMAIN_REPLACE_SUCC:
		{
			m_StatusddnsCheckOK = CHECK_STATUS_OK;
		}
		break;
	case CMD_REPLY_NETPARAMCHECK_DDNS_FAIL:
	case CMD_REPLY_IN23_DOMAIN_ALREADY_USE:
		{
			m_StatusddnsCheckOK = CHECK_STATUS_FAIL;
		}
		break;
	default:
		{
			assert(false);
			m_netCheckResult = NETERR_NETPARAMCHECK_EUNKNOWN;
			break;
		}

	}

	m_netCheckResult = *reinterpret_cast< const NET_CTRL_FAIL* >( msgData.pData + sizeof(PACKCMD) );
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	m_bCheckParam = false;

	return GetErrorTextNum();
}

unsigned long CCfgNetMoreConfigDlg::GetErrorTextNum()
{
	ENUM_STRING_ID strID = E_STR_ID_NULL;

	switch( m_netCheckResult )
	{
	case NETERR_NETPARAMCHECK_OK:						//����ͨ��
		strID = E_STR_ID_MESSAGE_56;
#if defined(__CUSTOM_IN23__)
		if(DDNS_MINT_AUTO == m_ddnsInfo.type)
		{
			strID = E_STR_ID_MESSAGE_131;
		}
#endif
		break;
	case NETERR_NETPARAMCHECK_ENAMEPWD:					//�û��������������
		strID = E_STR_ID_MESSAGE_57;
		break;
	case NETERR_NETPARAMCHECK_EHOSTDOMAIN:				//dyndns.com���û������������������������
		strID = E_STR_ID_MESSAGE_88;
		break;
	case NETERR_NETPARAMCHECK_ECONNECT:					//����δ����,δ������
		strID = E_STR_ID_MESSAGE_58;
		break;
	case NETERR_NETPARMACHECK_ENETUNREACHD:				//���粻�ɴ�
		strID = E_STR_ID_MESSAGE_58;
		break;
	case NETERR_NETPARAMCHECK_ETIMEOUT:					//�������ӳ�ʱ
		strID = E_STR_ID_MESSAGE_87;
		break;
	case NETERR_NETPARAMCHECK_EUNKNOWN:					//δ֪����	
		strID = E_STR_ID_MESSAGE_59;
#if defined(__CUSTOM_IN23__)
		if(DDNS_MINT_AUTO == m_ddnsInfo.type)
		{
			strID = E_STR_ID_MESSAGE_134;
		}
#endif
		break;
	case NETERR_NETPARAMCHECK_EUSERNAMEEMPTY:			//�û�������Ϊ��
		strID = E_STR_ID_MESSAGE_89;
		break;
	case NETERR_NETPARMACHECK_EHOSTDOMAIN_EMPTY:		//������������Ϊ��
		strID = E_STR_ID_MESSAGE_90;
		break;
	case NETERR_NETPARAMCHECK_ABUSE:					//dyndns.com��ע���������block
		strID = E_STR_ID_MESSAGE_19;
		break;
	case NETERR_NETPARAMCHECK_NOHOST:					//dyndns.com���Ҳ����������
		strID = E_STR_ID_MESSAGE_20;
		break;
	case NETERR_NETPARAMCHECK_NOTFQDN:					//dyndns.com���������������Ϲ���
		strID = E_STR_ID_MESSAGE_21;
		break;
	case NETERR_NETPARAMCHECK_NUMHOST:					//dyndns.com��̫���������update
		strID = E_STR_ID_MESSAGE_22;
		break;
	case NETERR_NETPARAMCHECK_REGISTER_SUCC:			//��dynamic.visionica.com��ע��ɹ�
		strID = E_STR_ID_TIP_04_05;
		break;
	case NETERR_NETPARAMCHECK_REGISTER_FAIL:			//��dynamic.visionica.com��ע��ʧ��
		strID = E_STR_ID_MESSAGE_25;
		break;
	case NETERR_NETPARAMCHECK_DOMAIN_ALREADY_USE:		//IN23�Զ�ע�ᣬ�����Ѿ�����
		strID = E_STR_ID_MESSAGE_135;
		break;
	case NETERR_NETPARAMCHECK_DOMAIN_CREATE_OK:			//IN23�Զ�ע�ᣬ����ע��ɹ�
		strID = E_STR_ID_MESSAGE_138;
		break;
	case NETERR_NETPARAMCHECK_DOMAIN_REPLACE_OK:			//IN23�Զ�ע�ᣬ�����滻�ɹ�
		strID = E_STR_ID_MESSAGE_139;
		break;
	default:
		strID = E_STR_ID_MESSAGE_58;
		break;
	}

	return strID;
}

unsigned long CCfgNetMoreConfigDlg::CheckValue()
{
	//mint dns �� TVT IP Server �������з�������ַ���������͵ķ�������ַ������Ϊ��
	if ((DDNS_MINTDNS != m_ddnsInfo.type) && (DDNS_TVT_IP_SERVER != m_ddnsInfo.type))
	{
		memset(m_ddnsServerName, 0, sizeof(m_ddnsServerName));
	}

	//�޸�DDNS�󱣴�ʱ����ʾ
	switch(m_StatusddnsCheckOK)
	{
	case CHECK_STATUS_NOCHANGE:							//�������û�иı�
	case CHECK_STATUS_OK:								//���Գɹ�
		break;

	case CHECK_STATUS_NOCHECK:							//û�н��в���
		{
			//���ѡ���DDNS������Ϊ�տ���ֱ�ӱ���
			LONGLONG type = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TYPE,1))->GetCurItemData();
			if( (DDNS_NONE == type) || (DDNS_MINT_AUTO == type))
			{
				m_StatusddnsCheckOK = CHECK_STATUS_OK;

			}
			else
			{
				unsigned long errNum = CheckDdnsParam();

				if(E_STR_ID_MESSAGE_56 == errNum || NETERR_NETPARAMCHECK_REGISTER_SUCC == errNum)
				{
					//DDNS�����Ϣ���ɹ�
					m_StatusddnsCheckOK = CHECK_STATUS_OK;
					errNum = E_STR_ID_NULL;
					break;
				}
				return errNum;								//��ʾ����ʧ�������Ϣ
			}
			break;
		}
	case CHECK_STATUS_FAIL:								//����ʧ��	��ʾ��DDNS������Ч���������趨
		return E_STR_ID_MESSAGE_111;

	default:
		assert(false);
		break;
	}

	return 0;
}


BEGIN_MSG_MAP(CCfgNetMoreConfigDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgNetMoreListF.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_cfgNetMoreListS.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgNetMoreConfigDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//��
	unsigned short col = keyID&0xffff;			//��

	if( 1 != col )
	{
		printf("%s:%s:%d, row=0x%04x, col=0x%04x\n", __FUNCTION__, __FILE__, __LINE__, row, col);
		return KEY_VIRTUAL_MSG_STOP;
	}


	if (USE_FIRST_LIST == m_useWhichList)
	{
		ClickFirstList(row, col);
	} 
	else
	{
		ClickSecondList(row, col);
	}
	if (!IsModify())
	{	
		if (IsChange(NCFG_ITEM_UPNP_ENABLE))
		{
			SetModify(true);
		}
		if(IsChange(NCFG_ITEM_DDNS_INFO))
		{
			SetModify(true);
		}

		if(IsChange(NCFG_ITEM_DDNS_UPDATE_CYCLE))
		{
			SetModify(true);
		}

		if (IsChange(NCFG_ITEM_MINT_DDNS_SERVER_NAME))
		{
			SetModify(true);
		}

		return KEY_VIRTUAL_MSG_CONTINUE;

	}
	
	return KEY_VIRTUAL_MSG_STOP;
}


void CCfgNetMoreConfigDlg::ClickFirstList(unsigned short row, unsigned short col)
{

	switch(row)
	{
	case ROW_UPNP_ENABLE:
		{
			GUI::CCheckCtrl *pCheckF = NULL;
			GUI::CCheckCtrl *pCheckS = NULL;
			pCheckF = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_UPNP_ENABLE, 1));
			pCheckS = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_UPNP_ENABLE, 1));
			if (pCheckF->IsCheck())
			{
				m_bUpnp = 1;
				pCheckS->SetCheck(true, false);
			}
			else
			{
				m_bUpnp = 0;
				pCheckS->SetCheck(false, false);
			}
			break;
		}
	case ROW_DDNS_ENABLE:
		{
			CheckEnableUI(true);					
			
			if(( dynamic_cast<GUI::CCheckCtrl*>( m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_ENABLE, 1) )->IsCheck() ))
			{
				m_ddnsInfo.bEnable = 1;
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
			}
			else
			{
				m_ddnsInfo.bEnable = 0;
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHANGE;
			}

			break;
		}
	case ROW_DDNS_TYPE:
		{
			bool bChangeToSList = false;
			bool bHaveChangedList = false;
			unsigned short oldType = m_ddnsInfo.type;

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TYPE,1));
			unsigned short ddnsType = pCombo->GetCurItemData(); 
			assert(pCombo);
			if( (m_ddnsInfo.type !=  ddnsType) && (DDNS_VISIONICA == ddnsType) )
			{
				//Ҫ�л�LIST
				bChangeToSList = true;
			}

			for( int i=0; i<m_ddnsServerCount; i++ )
			{
				if( m_ddnsInfo.type == m_pDdnsSeverInfo[i].ucDDNSID )
				{
					m_ddnsInfo.bUseDomain = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
					break;
				}			
			}

			if ( bChangeToSList)
			{
				bHaveChangedList = ChangeToSList(true);
			}

			if (bHaveChangedList)
			{
				//�´β�����������m_cfgNetMoreListF
				m_useWhichList = USE_SECOND_LIST;

				if (m_cfgNetMoreListF.IsShow())
				{
					m_cfgNetMoreListF.Show(false, true);
				} 
				
				if (!m_cfgNetMoreListS.IsShow())
				{
					m_cfgNetMoreListS.Show(true, true);
				}

				CheckEnableUIS(true);
			}
			else
			{
				CheckEnableUI(true);

				if (DDNS_TVT_IP_SERVER == oldType)
				{
					if (DDNS_TVT_IP_SERVER != m_ddnsInfo.type)
					{
						IPServerUIToFUI(m_cfgNetMoreListF);
					}
				}
				else
				{
					if (DDNS_TVT_IP_SERVER == m_ddnsInfo.type)
					{
						FUIToIPServerUI(m_cfgNetMoreListF);
					}
				}

				m_cfgNetMoreListF.Show(true);
			}

			//���ѡ���DDNS������Ϊ�տ���ֱ�ӱ���
			if( DDNS_NONE != m_ddnsInfo.type)
			{
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
			}
			else
			{
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHANGE;
			}
			m_ddnsInfo.type = ddnsType;
			break;
		}
	case ROW_DDNS_SERVER_NAME:
		{
			const GUI::CEditCtrl* pServerName = dynamic_cast<const GUI::CEditCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_SERVER_NAME,1));
			assert( NULL != pServerName);
			if (DDNS_TVT_IP_SERVER == m_ddnsInfo.type)
			{
				//���������ƣ�ʹ��IPServerʱ������û�����
				strcpy(m_ddnsInfo.user,pServerName->GetCaption().c_str());
			}
			else
			{
				strcpy((char *)m_ddnsServerName, pServerName->GetCaption().c_str());
			}

			m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
			break;
		}
	case ROW_DDNS_USER_NAME:
		{
#ifdef __CUSTOM_BR27__
			const GUI::CEditStaticCtrl* pUser = dynamic_cast<const GUI::CEditStaticCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_USER_NAME,1));
			assert( NULL != pUser);
			strcpy(m_ddnsInfo.user,pUser->GetCaption().c_str());
#else
			const GUI::CEditCtrl* pUser = dynamic_cast<const GUI::CEditCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_USER_NAME,1));
			assert( NULL != pUser);
			strcpy(m_ddnsInfo.user,pUser->GetCaption().c_str());
#endif
			
		}

		m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
		break;
	case ROW_DDNS_PASSWORD:
		{
			const GUI::CEditCtrl* pPass = dynamic_cast<const GUI::CEditCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_PASSWORD,1));
			assert(pPass);
			strcpy(m_ddnsInfo.password,pPass->GetCaption().c_str());
		}

		m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
		break;
	case ROW_DDNS_DOMAIN_INFO:
		{
#if defined(__CUSTOM_BR27__) || defined(__AUTOREG__)
			GUI::CEditStaticCtrl* pDomain = dynamic_cast<GUI::CEditStaticCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
			assert(NULL != pDomain);
			strcpy(m_ddnsInfo.domain,pDomain->GetCaption().c_str());
			CheckDomainParam();
#else 
			GUI::CEditCtrl* pDomain = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
			assert(NULL != pDomain);
			strcpy(m_ddnsInfo.domain,pDomain->GetCaption().c_str());
#endif

		}
		m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
		break;
	case ROW_DDNS_UPDATE_CYCLE:
		{
			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1));
			assert(NULL != pCombo);
			m_ddnsUpdateCycle = pCombo->GetCurItemData();

			break;
		}
	case ROW_DDNS_TEST:
		{
			if(m_bAutoReg)
			{
				strcpy(m_ddnsInfo.user,"admin");
				strcpy(m_ddnsInfo.password,"password");
				if(!CheckDomainParam())//������������Ͳ����ddns������
				{
					GetTopestWnd()->Repaint();
					return;
				}
			}
			unsigned long errNum = CheckDdnsParam();
			
			MessageOut( GetTopestWnd(), static_cast<ENUM_STRING_ID>(errNum), TIP_ERR, false );

			GetTopestWnd()->Repaint();

			break;
		}
	case ROW_NUM:
		{
#if defined(__CUSTOM_IN23__)
			//���ɾ����
			if(DDNS_MINT_AUTO == m_ddnsInfo.type)
			{
				MessageOut( GetTopestWnd(), E_STR_ID_PLEASE_WAIT, TIP_ERR );
				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_REMOVEDDNS, NULL, 0, msgData );
				assert(NULL != msgData.pData);
				PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
				switch( static_cast<NET_PROTOCOL>(pCmd->cmdType) )
				{
				case CMD_REPLY_REMOVEDDNS_SUCC:
					{
						MessageOut( GetTopestWnd(), static_cast<ENUM_STRING_ID>(E_STR_ID_MESSAGE_136), TIP_ERR, false );
						//ɾ�����û�����������Ϊ��
						memset(m_ddnsInfo.user,0,sizeof(m_ddnsInfo.user));
						memset(m_ddnsInfo.password,0,sizeof(m_ddnsInfo.password));
						memset(m_ddnsInfo.domain,0,sizeof(m_ddnsInfo.domain));
						m_StatusddnsCheckOK = CHECK_STATUS_FAIL;//ɾ����״̬��Ϊ����ʧ�ܣ�����ֻ���ٴ��������ܱ���
						GUI::CEditStaticCtrl * pEdit = dynamic_cast<GUI::CEditStaticCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
						pEdit->SetCaption("",false);
					}
					break;
				case CMD_REPLY_REMOVEDDNS_FAIL:
					{
						MessageOut(GetTopestWnd(),static_cast<ENUM_STRING_ID>(E_STR_ID_MESSAGE_137), TIP_ERR, false );
					}
					break;
				default:
					{
						MessageOut(GetTopestWnd(),static_cast<ENUM_STRING_ID>(E_STR_ID_MESSAGE_137), TIP_ERR, false );
						break;
					}

				}
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
				GetTopestWnd()->Repaint();
			}
			
#endif
			break;
		}
	default:
		assert(false);
		break;
	}

}

bool CCfgNetMoreConfigDlg::ChangeToSList(bool bRefresh)
{
	//
	GUI::CListCtrl& list = m_cfgNetMoreListS;
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(ROW_S_DDNS_ENABLE, 1) );
	assert( NULL != pCheck );
	pCheck->SetCheck( m_ddnsInfo.bEnable, bRefresh );

	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_S_DDNS_TYPE,1));
	assert(NULL != pCombo);
	pCombo->SetCurItemData(m_ddnsInfo.type,bRefresh);

	GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_S_DDNS_MAIL_ADDR,1));
	assert(NULL != pEdit);
	pEdit->SetCaption(m_ddnsInfo.user,bRefresh);

	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_S_DDNS_DOMAIN_INFO,1));
	assert(NULL != pEdit);
	memset(m_ddnsInfo.domain, 0, sizeof(m_ddnsInfo.domain));
	snprintf(m_ddnsInfo.domain, sizeof(m_ddnsInfo.domain), "%02x%02x%02x%02x%02x%02x", 
		g_p_login_succ->productInfo.deviceMAC[0], g_p_login_succ->productInfo.deviceMAC[1],g_p_login_succ->productInfo.deviceMAC[2],
		g_p_login_succ->productInfo.deviceMAC[3],g_p_login_succ->productInfo.deviceMAC[4],g_p_login_succ->productInfo.deviceMAC[5]);

	pEdit->SetCaption(m_ddnsInfo.domain,bRefresh);
	pEdit->SetEnable(false, bRefresh);

	pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_UPDATE_CYCLE,1));
	assert(NULL != pCombo);
	pCombo->SetCurItemData(m_ddnsUpdateCycle, false);

	return true;
}

void CCfgNetMoreConfigDlg::ClickSecondList(unsigned short row, unsigned short col)
{
	assert( USE_SECOND_LIST == m_useWhichList );

	switch(row)
	{
	case ROW_S_UPNP_ENABLE:
		{
			GUI::CCheckCtrl *pCheckF = NULL;
			GUI::CCheckCtrl *pCheckS = NULL;
			pCheckF = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_UPNP_ENABLE, 1));
			pCheckS = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_UPNP_ENABLE, 1));
			if (pCheckS->IsCheck())
			{
				m_bUpnp = 1;
				pCheckF->SetCheck(true, false);
			}
			else
			{
				m_bUpnp = 0;
				pCheckF->SetCheck(false, false);
			}
			break;		
		}
	case ROW_S_DDNS_ENABLE:
		{
			CheckEnableUIS(true);					

			if(( dynamic_cast<GUI::CCheckCtrl*>( m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_ENABLE, 1) )->IsCheck() ))
			{
				m_ddnsInfo.bEnable = 1;
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
			}
			else
			{
				m_ddnsInfo.bEnable = 0;
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHANGE;
			}

			break;
		}
	case ROW_S_DDNS_TYPE:
		{
			bool bChangeToFList = false;
			bool bHaveChangedList = false;
			unsigned short oldType = m_ddnsInfo.type;

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_TYPE,1));
			assert(pCombo);
			unsigned short ddnsType = pCombo->GetCurItemData(); 
			if( m_ddnsInfo.type !=  ddnsType )
			{
				//�л���LIST
				bChangeToFList = true;
			}

			m_ddnsInfo.type = pCombo->GetCurItemData();
			
			for( int i=0; i<m_ddnsServerCount; i++ )
			{
				if( m_ddnsInfo.type == m_pDdnsSeverInfo[i].ucDDNSID )
				{
					m_ddnsInfo.bUseDomain = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
					break;
				}			
			}

			if ( bChangeToFList )
			{
				bChangeToFList = ChangeToFList(true);
			}

			if ( bChangeToFList )
			{
				m_useWhichList = USE_FIRST_LIST;
				if (!m_cfgNetMoreListF.IsShow())
				{
					if (DDNS_TVT_IP_SERVER == m_ddnsInfo.type)
					{
						FUIToIPServerUI(m_cfgNetMoreListF);
					}
					else
					{
						IPServerUIToFUI(m_cfgNetMoreListF);
					}

					m_cfgNetMoreListF.Show(true, true);
				} 

				if (m_cfgNetMoreListS.IsShow())
				{
					m_cfgNetMoreListS.Show(false, true);
				}

				CheckEnableUI(true);
			}
			else
			{
				CheckEnableUIS(true);
			}

			//���ѡ���DDNS������Ϊ�տ���ֱ�ӱ���
			if( DDNS_NONE != m_ddnsInfo.type )
			{
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
			}
			else
			{
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHANGE;
			}

			break;
		}
	case ROW_S_DDNS_MAIL_ADDR:
		{
			const GUI::CEditCtrl* pMailAddr = dynamic_cast<const GUI::CEditCtrl*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_MAIL_ADDR,1));
			assert( NULL != pMailAddr);
			strcpy(m_ddnsInfo.user,pMailAddr->GetCaption().c_str());
		}

		m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
		break;

	case ROW_S_DDNS_DOMAIN_INFO:
		{
			const GUI::CEditCtrl* pDomain = dynamic_cast<const GUI::CEditCtrl*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_DOMAIN_INFO,1));
			assert(pDomain);
			strcpy(m_ddnsInfo.domain,pDomain->GetCaption().c_str());
		}

		m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
		break;

	case ROW_S_DDNS_UPDATE_CYCLE:
		{
			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_UPDATE_CYCLE,1));
			assert(NULL != pCombo);
			m_ddnsUpdateCycle = pCombo->GetCurItemData();

			break;
		}
	case ROW_S_DDNS_REGISTER:
		{
			unsigned long errNum = CheckDdnsParam();

			MessageOut( GetTopestWnd(), static_cast<ENUM_STRING_ID>(errNum), TIP_ERR, false );

			GetTopestWnd()->Repaint();

			break;
		}
	default:
		assert(false);
		break;
	}

}

bool CCfgNetMoreConfigDlg:: ChangeToFList(bool bRefresh)
{
	GUI::CListCtrl& list = m_cfgNetMoreListF;
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(ROW_DDNS_ENABLE, 1) );
	assert( NULL != pCheck );
	pCheck->SetCheck( m_ddnsInfo.bEnable, bRefresh );

	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_DDNS_TYPE,1));
	assert(NULL != pCombo);
	pCombo->SetCurItemData(m_ddnsInfo.type,bRefresh);

	GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_SERVER_NAME,1));
	assert(NULL != pEdit);
	pEdit->SetCaption((const char *)m_ddnsServerName,bRefresh);

#ifdef __CUSTOM_BR27__
	GUI::CEditStaticCtrl* pEditStatic = dynamic_cast<GUI::CEditStaticCtrl*>(list.GetItemWnd(ROW_DDNS_USER_NAME,1));
	assert(NULL != pEditStatic);
	pEditStatic->SetCaption(m_ddnsInfo.user,bRefresh);
#else
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_USER_NAME,1));
	assert(NULL != pEdit);
	pEdit->SetCaption(m_ddnsInfo.user,bRefresh);
#endif
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_PASSWORD,1));
	assert(NULL != pEdit);
	pEdit->SetCaption(m_ddnsInfo.password,bRefresh);

#if defined(__CUSTOM_BR27__) || defined(__AUTOREG__)
	GUI::CEditStaticCtrl* pDomain = dynamic_cast<GUI::CEditStaticCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(NULL != pDomain);
	pDomain->SetCaption(m_ddnsInfo.domain,bRefresh);
#else 
	GUI::CEditCtrl* pDomain = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(NULL != pDomain);
	pDomain->SetCaption(m_ddnsInfo.domain,bRefresh);
#endif
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1));
	assert(NULL != pCombo);
	pCombo->SetCurItemData(m_ddnsUpdateCycle, false);

	return true;
}

void CCfgNetMoreConfigDlg::RefreshListF(const DDNS_INFO* pDDNS, bool bRefresh)
{
	if ( NULL == pDDNS)
	{
		assert(false);
		return;
	}

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;


	CCfgDlg::GetConfig( NCFG_ITEM_DDNS_USER_NAME_MAX_LEN, &pTempData, tempDataLen );
	assert(tempDataLen==sizeof(unsigned long));
	unsigned long maxUserLen =  *reinterpret_cast<unsigned long*>(pTempData);

	CCfgDlg::GetConfig( NCFG_ITEM_DDNS_PASSWORD_MAX_LEN, &pTempData, tempDataLen );
	assert(tempDataLen==sizeof(unsigned long));
	unsigned long maxPassLen = *reinterpret_cast<unsigned long*>(pTempData);

	CCfgDlg::GetConfig( NCFG_ITEM_DDNS_DOMAIN_MAX_LEN, &pTempData, tempDataLen );
	assert(tempDataLen==sizeof(unsigned long));
	unsigned long maxDomainLen = *reinterpret_cast<unsigned long*>(pTempData);
#ifdef __CUSTOM_IN23__
	LONGLONG curtype = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_TYPE,1))->GetCurItemData();
	if(DDNS_MINT_AUTO == curtype)
	{
		maxDomainLen = 16;
	}
#endif

	GUI::CListCtrl& list = m_cfgNetMoreListF;
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(ROW_DDNS_ENABLE, 1) );
	assert( pCheck );
	pCheck->SetCheck( pDDNS->bEnable, bRefresh );

	CCfgDlg::GetConfig( NCFG_ITEM_UPNP_ENABLE, &pTempData, tempDataLen);
	assert(tempDataLen == sizeof(unsigned long));
	unsigned long upnpEnable =*reinterpret_cast<unsigned long*>(pTempData);

	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_DDNS_TYPE,1));
	assert(pCombo);
	pCombo->RemoveAllItem(bRefresh);
	pCombo->SetMaxStringLen(sizeof(DDNS_SERVER_INFO));//�˿ؼ�Ϊֻ��,���趨���ַ�������

	for( int i=0; i<m_ddnsServerCount; i++ )
	{
		if( m_pDdnsSeverInfo[i].ucDDNSID == DDNS_NONE )
		{
			continue;
		}

		pCombo->AddItem( m_pDdnsSeverInfo[i].szDDNSServerName, m_pDdnsSeverInfo[i].ucDDNSID );
	}
	pCombo->SetCurItemData(pDDNS->type,bRefresh);


	GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_SERVER_NAME,1));
	assert(pEdit);
	pEdit->SetMaxStringLen(maxDomainLen);
	//pEdit->SetCaption(pDDNS->serverDNS,bRefresh);

#ifdef __CUSTOM_BR27__
	GUI::CEditStaticCtrl* pEditStatic = dynamic_cast<GUI::CEditStaticCtrl*>(list.GetItemWnd(ROW_DDNS_USER_NAME,1));
	assert(pEditStatic);
	pEditStatic->SetMaxStringLen(maxUserLen);
	pEditStatic->SetCaption(pDDNS->user,bRefresh);
#else
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_USER_NAME,1));
	assert(pEdit);
	pEdit->SetMaxStringLen(maxUserLen);
	pEdit->SetCaption(pDDNS->user,bRefresh);
#endif
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_PASSWORD,1));
	assert(pEdit);
#ifdef __CUSTOM_BR27__
	pEdit->SetMaxStringLen(maxPassLen);
#else
	pEdit->SetMaxStringLen(maxPassLen);
#endif
	pEdit->SetCaption(pDDNS->password,bRefresh);

#if defined(__CUSTOM_BR27__) || defined(__AUTOREG__)
	GUI::CEditStaticCtrl* pEditDomain = dynamic_cast<GUI::CEditStaticCtrl*>(list.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(pEditDomain);
	pEditDomain->SetMaxStringLen(maxDomainLen);
	pEditDomain->SetCaption(pDDNS->domain,bRefresh);
#else
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(pEdit);
	pEdit->SetMaxStringLen(maxDomainLen);
	pEdit->SetCaption(pDDNS->domain,bRefresh);
#endif
	if (DDNS_TVT_IP_SERVER == pDDNS->type)
	{
		FUIToIPServerUI(list);
	}
	else
	{
		IPServerUIToFUI(list);
	}
}

void CCfgNetMoreConfigDlg::RefreshListS(const DDNS_INFO* pDDNS, bool bRefresh)
{
	if ( NULL == pDDNS)
	{
		assert(false);
		return;
	}

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;


	CCfgDlg::GetConfig( NCFG_ITEM_DDNS_USER_NAME_MAX_LEN, &pTempData, tempDataLen );
	assert(tempDataLen==sizeof(unsigned long));
	unsigned long maxUserLen =  *reinterpret_cast<unsigned long*>(pTempData);

	CCfgDlg::GetConfig( NCFG_ITEM_DDNS_DOMAIN_MAX_LEN, &pTempData, tempDataLen );
	assert(tempDataLen==sizeof(unsigned long));
	unsigned long maxDomainLen = *reinterpret_cast<unsigned long*>(pTempData);

	CCfgDlg::GetConfig( NCFG_ITEM_UPNP_ENABLE, &pTempData, tempDataLen);
	assert(tempDataLen == sizeof(unsigned long));
	unsigned long upnpEnable =*reinterpret_cast<unsigned long*>(pTempData);

	GUI::CListCtrl& list = m_cfgNetMoreListS;
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(ROW_S_DDNS_ENABLE, 1) );
	assert( pCheck );
	pCheck->SetCheck( pDDNS->bEnable, bRefresh );

	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_S_DDNS_TYPE,1));
	assert(pCombo);
	pCombo->RemoveAllItem(bRefresh);
	pCombo->SetMaxStringLen(sizeof(DDNS_SERVER_INFO));//�˿ؼ�Ϊֻ��,���趨���ַ�������

	for( int i=0; i<m_ddnsServerCount; i++ )
	{
		if( m_pDdnsSeverInfo[i].ucDDNSID == DDNS_NONE )
		{
			continue;
		}

		pCombo->AddItem( m_pDdnsSeverInfo[i].szDDNSServerName, m_pDdnsSeverInfo[i].ucDDNSID );
	}
	pCombo->SetCurItemData(pDDNS->type,bRefresh);

	GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_S_DDNS_MAIL_ADDR,1));
	assert(pEdit);
	pEdit->SetMaxStringLen(maxUserLen);
	pEdit->SetCaption(pDDNS->user,bRefresh);

	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_S_DDNS_DOMAIN_INFO,1));
	assert(pEdit);
	pEdit->SetMaxStringLen(maxDomainLen);
	pEdit->SetCaption(pDDNS->domain,bRefresh);
}

void CCfgNetMoreConfigDlg::SetMacToDomianInfo(const DDNS_INFO *pDNS)
{
	GUI::CEditCtrl *pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetMoreListS.GetItemWnd(ROW_S_DDNS_DOMAIN_INFO,1));
	assert(pEdit);


	char szMac[16] = {0};
	snprintf(szMac, sizeof(szMac), "%02x%02x%02x%02x%02x%02x", 
		g_p_login_succ->productInfo.deviceMAC[0], g_p_login_succ->productInfo.deviceMAC[1],g_p_login_succ->productInfo.deviceMAC[2],
		g_p_login_succ->productInfo.deviceMAC[3],g_p_login_succ->productInfo.deviceMAC[4],g_p_login_succ->productInfo.deviceMAC[5]);
	if (strcmp(szMac, pDNS->domain))
	{
		snprintf(m_ddnsInfo.domain, sizeof(m_ddnsInfo), "%s", szMac);
		pEdit->SetCaption(m_ddnsInfo.domain,false);
	}
	
}

void CCfgNetMoreConfigDlg::InitialListF(bool bRefresh)
{
	GUI::CListCtrl& list = m_cfgNetMoreListF;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;
#if defined(__CUSTOM_BR27__) || defined(__AUTOREG__)
	GUI::CEditStaticCtrl * pEditStatic = NULL;
#endif
	GUI::CButton* pBtn = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = height/m_length[LIST_ITEM_HEIGHT]; //List�򲻴�ͷ
	//if( ROW_NUM >= maxListRow )
	if(GUI::CWnd::m_width <= 800)
	{
		offsetEdge = m_length[SLIDER_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width * 16 / 32);
	list.AddColumn(NULL, width * 16 / 32 - offsetEdge);

	////
	char str[128] = {0};

	int row = 0;
	list.AddItem( ROW_DDNS_ENABLE, 0, m_pStrTable->String(E_STR_ID_DDNS) );
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_DDNS_ENABLE, 1, WND_TYPE_CHECK) );
	assert( pCheck );

	row += 1;
	list.AddItem( ROW_DDNS_TYPE, 0, m_pStrTable->String(E_STR_ID_DDNS_TYPE) );
#ifdef __CUSTOM_BR27__
	pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( ROW_DDNS_TYPE, 1, WND_TYPE_COMBO) );
#else
	pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( ROW_DDNS_TYPE, 1, WND_TYPE_COMBO) );
#endif
	assert( pCombo );

	row += 1;
	list.AddItem( ROW_DDNS_SERVER_NAME, 0, m_pStrTable->String(E_STR_ID_DDNS_SERVER));
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_DDNS_SERVER_NAME, 1, WND_TYPE_EDIT) );
	assert( pEdit );

	row += 1;
	list.AddItem( ROW_DDNS_USER_NAME, 0, m_pStrTable->String( E_STR_ID_USER_NAME ) );
#ifdef __CUSTOM_BR27__
	pEditStatic = dynamic_cast<GUI::CEditStaticCtrl*>( list.AddItem( ROW_DDNS_USER_NAME, 1, WND_TYPE_EDITSTATIC) );
	assert( pEditStatic );
#else
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_DDNS_USER_NAME, 1, WND_TYPE_EDIT) );
	assert( pEdit );
#endif

	row += 1;
	list.AddItem( ROW_DDNS_PASSWORD, 0, m_pStrTable->String( E_STR_ID_PASSWORD ) );
#ifdef __CUSTOM_BR27__
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_DDNS_PASSWORD, 1, WND_TYPE_EDIT) );
#else
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_DDNS_PASSWORD, 1, WND_TYPE_EDIT) );
#endif
	assert( pEdit );
	pEdit->SetPassword( true );

	row += 1;
	list.AddItem( ROW_DDNS_DOMAIN_INFO, 0, m_pStrTable->String(E_STR_ID_HOST_DOMAIN) );
#if defined(__CUSTOM_BR27__) || defined(__AUTOREG__)	
	pEditStatic = dynamic_cast<GUI::CEditStaticCtrl*>( list.AddItem( ROW_DDNS_DOMAIN_INFO, 1, WND_TYPE_EDITSTATIC) );
	assert(NULL != pEditStatic );
#else
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_DDNS_DOMAIN_INFO, 1, WND_TYPE_EDIT) );
	assert( pEdit );
#endif
	row += 1;
	list.AddItem( ROW_DDNS_UPDATE_CYCLE, 0, m_pStrTable->String(E_STR_ID_UPDATE_CYCLE) );
	pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( ROW_DDNS_UPDATE_CYCLE, 1, WND_TYPE_COMBO) );
	assert( pCombo );
	char sz[20] = {0};

	snprintf(sz, sizeof(sz), ".5");
	pCombo->AddItem(sz,30);

	snprintf(sz, sizeof(sz), "1");
	pCombo->AddItem(sz,1 * 60);

	snprintf(sz, sizeof(sz), "3");
	pCombo->AddItem(sz, 3 * 60);

	snprintf(sz, sizeof(sz), "6");
	pCombo->AddItem(sz,6 * 60);

	snprintf(sz, sizeof(sz), "12");
	pCombo->AddItem(sz,12 * 60);

	snprintf(sz, sizeof(sz), "24");
	pCombo->AddItem(sz,24 * 60);

	row += 1;

	row += 1;
	pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(ROW_DDNS_TEST,1,WND_TYPE_BUTTON,m_length[BTN_WIDTH],m_length[BTN_HEIGHT]));
	assert(pBtn);
	pBtn->SetCaption(m_pStrTable->String(E_STR_ID_TEST),false);
	
	
	row += 1;
#if defined(__CUSTOM_IN23__)
	{
		pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(ROW_NUM,1,WND_TYPE_BUTTON,m_length[BTN_WIDTH],m_length[BTN_HEIGHT]));
		assert(pBtn);
		pBtn->SetCaption(m_pStrTable->String(E_STR_ID_DELETE),false);
		pBtn->Show(false,false);
	}
#endif
	

	//if(0 == s_loginSuccessReply.productInfo.netVideoInputNum)
	{
		row += 1;
		list.AddItem(ROW_UPNP_ENABLE, 0, m_pStrTable->String(E_STR_ID_UPNP), false);
		pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_UPNP_ENABLE, 1, WND_TYPE_CHECK) );
		assert(pCheck);
	}

	list.CreateListFocusMap();
}

void CCfgNetMoreConfigDlg:: InitialListS(bool bRefresh)
{
	GUI::CListCtrl& list = m_cfgNetMoreListS;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;
	GUI::CButton* pBtn = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = height/m_length[LIST_ITEM_HEIGHT]; //List�򲻴�ͷ
	if( ROW_S_NUM >= maxListRow )
	{
		offsetEdge = m_length[SLIDER_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 1, 0, false);
	list.AddColumn(NULL, width * 12 / 32);
	list.AddColumn(NULL, width * 10 / 32);
	list.AddColumn(NULL, width * 10 / 32  - offsetEdge);
	////
	char str[128] = {0};

	int row = 0;
	list.AddItem( ROW_S_DDNS_ENABLE, 0, m_pStrTable->String(E_STR_ID_DDNS) );
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_S_DDNS_ENABLE, 1, WND_TYPE_CHECK) );
	assert( pCheck );

	row += 1;
	list.AddItem( ROW_S_DDNS_TYPE, 0, m_pStrTable->String(E_STR_ID_DDNS_TYPE) );
	pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( ROW_S_DDNS_TYPE, 1, WND_TYPE_COMBO) );
	assert( pCombo );

	row += 1;
	list.AddItem( ROW_S_DDNS_MAIL_ADDR, 0, m_pStrTable->String( E_STR_ID_EMAIL ) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_S_DDNS_MAIL_ADDR, 1, WND_TYPE_EDIT) );
	assert( pEdit );

	row += 1;
	list.AddItem( ROW_S_DDNS_DOMAIN_INFO, 0, m_pStrTable->String(E_STR_ID_HOST_DOMAIN) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_S_DDNS_DOMAIN_INFO, 1, WND_TYPE_EDIT) );
	assert( pEdit );
	list.AddItem( ROW_S_DDNS_DOMAIN_INFO, 2, szVISIONICA_DNS_SERVER_NAME);

	row += 1;
	list.AddItem( ROW_S_DDNS_UPDATE_CYCLE, 0, m_pStrTable->String(E_STR_ID_UPDATE_CYCLE) );
	pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( ROW_S_DDNS_UPDATE_CYCLE, 1, WND_TYPE_COMBO) );
	assert( pCombo );
	char sz[20] = {0};

	snprintf(sz, sizeof(sz), ".5");
	pCombo->AddItem(sz,30);

	snprintf(sz, sizeof(sz), "1");
	pCombo->AddItem(sz,1 * 60);

	snprintf(sz, sizeof(sz), "3");
	pCombo->AddItem(sz, 3 * 60);

	snprintf(sz, sizeof(sz), "6");
	pCombo->AddItem(sz,6 * 60);

	snprintf(sz, sizeof(sz), "12");
	pCombo->AddItem(sz,12 * 60);

	snprintf(sz, sizeof(sz), "24");
	pCombo->AddItem(sz,24 * 60);

	row += 1;
	pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(ROW_S_DDNS_REGISTER,1,WND_TYPE_BUTTON,m_length[BTN_WIDTH],m_length[BTN_HEIGHT]));
	assert(pBtn);
	pBtn->SetCaption(m_pStrTable->String(E_STR_ID_REGISTER),false);

	//if(0 == s_loginSuccessReply.productInfo.netVideoInputNum)
	{
		row += 1;
		list.AddItem(ROW_S_UPNP_ENABLE, 0, m_pStrTable->String(E_STR_ID_UPNP), false);
		pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( ROW_S_UPNP_ENABLE, 1, WND_TYPE_CHECK) );
		assert(pCheck);
	}

	list.CreateListFocusMap();
}

void CCfgNetMoreConfigDlg::FUIToIPServerUI(GUI::CListCtrl& list)
{
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;


	list.AddItem( ROW_DDNS_SERVER_NAME, 0, m_pStrTable->String(E_STR_ID_NETSERVER_IP));

	//�����û���
#ifdef __CUSTOM_BR27__
	GUI::CEditStaticCtrl*	 pEditStatic = NULL;
	list.AddItem( ROW_DDNS_USER_NAME, 0, "" );
	pEditStatic = dynamic_cast<GUI::CEditStaticCtrl*>(list.GetItemWnd(ROW_DDNS_USER_NAME,1));
	assert(pEditStatic);
	pEditStatic->Show(false, false);

#else
	list.AddItem( ROW_DDNS_USER_NAME, 0, "" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_USER_NAME,1));
	assert(pEdit);
	pEdit->Show(false, false);
#endif
	//��������
	list.AddItem( ROW_DDNS_PASSWORD, 0, "" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_PASSWORD,1));
	assert(pEdit);
	pEdit->Show(false, false);

	//��������
	list.AddItem( ROW_DDNS_DOMAIN_INFO, 0, "" );
#if defined(__CUSTOM_BR27__) || defined(__AUTOREG__)
	GUI::CEditStaticCtrl* pDomain = dynamic_cast<GUI::CEditStaticCtrl*>(list.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(NULL != pDomain);
	pDomain->Show(false, false);
#else 
	GUI::CEditCtrl* pDomain = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(NULL != pDomain);
	pDomain->Show(false, false);
#endif
	
	
	//��������
	list.AddItem( ROW_DDNS_UPDATE_CYCLE, 0, "" );
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1));
	assert(NULL != pCombo);
	pCombo->Show(false, false);

}


void CCfgNetMoreConfigDlg::IPServerUIToFUI(GUI::CListCtrl& list)
{
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;

	//��ʾ�û���
#ifdef __CUSTOM_BR27__
	GUI::CEditStaticCtrl*	 pEditStatic = NULL;
	list.AddItem( ROW_DDNS_USER_NAME, 0, m_pStrTable->String( E_STR_ID_USER_NAME ) );
	pEditStatic = dynamic_cast<GUI::CEditStaticCtrl*>(list.GetItemWnd(ROW_DDNS_USER_NAME,1));
	assert(pEditStatic);
	pEditStatic->Show(true, false);
#else
	list.AddItem( ROW_DDNS_USER_NAME, 0, m_pStrTable->String( E_STR_ID_USER_NAME ) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_USER_NAME,1));
	assert(pEdit);
	pEdit->Show(true, false);
#endif
	//��ʾ����
	list.AddItem( ROW_DDNS_PASSWORD, 0, m_pStrTable->String( E_STR_ID_PASSWORD ) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_PASSWORD,1));
	assert(pEdit);
	pEdit->Show(true, false);

	//��ʾ����
	list.AddItem( ROW_DDNS_DOMAIN_INFO, 0, m_pStrTable->String( E_STR_ID_HOST_DOMAIN ) );
#if defined(__CUSTOM_BR27__) || defined(__AUTOREG__)
	GUI::CEditStaticCtrl* pDomain = dynamic_cast<GUI::CEditStaticCtrl*>(list.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(NULL != pDomain);
	pDomain->Show(true, false);
#else
	GUI::CEditCtrl* pDomain = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(NULL != pDomain);
	pDomain->Show(true, false);
#endif
	//��ʾ����
	list.AddItem( ROW_DDNS_UPDATE_CYCLE, 0, m_pStrTable->String(E_STR_ID_UPDATE_CYCLE) );
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1));
	assert( pCombo );
	pCombo->Show(true, false);
}

bool CCfgNetMoreConfigDlg::CheckDomainParam()
{
#if defined(__CUSTOM_IN23__)
	GUI::CEditStaticCtrl* pDomain = dynamic_cast<GUI::CEditStaticCtrl*>(m_cfgNetMoreListF.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(NULL != pDomain);
	if(DDNS_MINT_AUTO == m_ddnsInfo.type)
	{
		unsigned long textLen = strlen(m_ddnsInfo.domain);
		if((textLen > 16)||(textLen < 3))
		{
			//������ʾ�����
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_133), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			memset(m_ddnsInfo.domain,0,sizeof(m_ddnsInfo.domain));
			std::string str;
			str.clear();
			pDomain->SetCaption(str,false);
			return false;
		}
		else
		{

			//���������ַ������в������ֺ���ĸ�ķ���ȫ��������ʾ�����
			for(int index=0;index <textLen;++index)
			{
				if((isdigit(m_ddnsInfo.domain[index]))|| (isalpha(m_ddnsInfo.domain[index])))
				{

				}
				else
				{
					MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_132), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
					memset(m_ddnsInfo.domain,0,sizeof(m_ddnsInfo.domain));
					std::string str;
					str.clear();
					pDomain->SetCaption(str,false);
					return false;
				}
			}
		}
	}
#endif
	return true;
}

