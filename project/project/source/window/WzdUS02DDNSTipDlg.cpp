/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-10-17
** Name         : WzdUS02DDNSTipDlg.cpp
** Version      : 1.0
** Description  : �򵼹���Ի���
** Modify Record:
***********************************************************************/

#include "WzdUS02DDNSTipDlg.h"
#include "MessageMan.h"
CWzdUS02DDNStipDlg::CWzdUS02DDNStipDlg()
{
	m_pWzdInfo = NULL;
	m_bCheckParam = false;
	m_StatusddnsCheckOK = CHECK_STATUS_NOCHANGE;
}

CWzdUS02DDNStipDlg::~CWzdUS02DDNStipDlg()
{

}

void CWzdUS02DDNStipDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdUS02DDNStipDlg::ShowWzdData(bool bRefresh)
{
	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}

	//ddns���õ���ʾ
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_listDDNS.GetItemWnd(ROW_DDNS_ENABLE, 1) );
	assert( pCheck );
	pCheck->SetCheck( m_pWzdInfo->ddnsInfo.bEnable, bRefresh );

	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_TYPE,1));
	assert(pCombo);
	pCombo->RemoveAllItem(bRefresh);
	pCombo->SetMaxStringLen(sizeof(DDNS_SERVER_INFO));//�˿ؼ�Ϊֻ��,���趨���ַ�������

	for( int i=0; i< m_pWzdInfo->ddnsServerCount; i++ )
	{
		if( m_pWzdInfo->pDdnsSeverInfo[i].ucDDNSID == DDNS_NONE )
		{
			continue;
		}

		pCombo->AddItem( m_pWzdInfo->pDdnsSeverInfo[i].szDDNSServerName, m_pWzdInfo->pDdnsSeverInfo[i].ucDDNSID );
	}
	pCombo->SetCurItemData(m_pWzdInfo->ddnsInfo.type,bRefresh);

	GUI::CEditCtrl*pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_SERVER_NAME,1));
	assert(pCombo);
	pEdit->SetMaxStringLen(m_pWzdInfo->maxDomainLen);

	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_USER_NAME,1));
	assert(pEdit);
	pEdit->SetMaxStringLen(m_pWzdInfo->maxUserLen);
	pEdit->SetCaption(m_pWzdInfo->ddnsInfo.user,bRefresh);

	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_PASSWORD,1));
	assert(pEdit);
	pEdit->SetMaxStringLen(m_pWzdInfo->maxPassLen);
	pEdit->SetCaption(m_pWzdInfo->ddnsInfo.password,bRefresh);

	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(pEdit);
	pEdit->SetMaxStringLen(m_pWzdInfo->maxDomainLen);
	pEdit->SetCaption(m_pWzdInfo->ddnsInfo.domain,bRefresh);
	

	if (DDNS_TVT_IP_SERVER == m_pWzdInfo->ddnsInfo.type)
	{
		FUIToIPServerUI();
	}
	else
	{
		IPServerUIToFUI();
	}
	CheckEnableUI(false);
	
	pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1));
	pCombo->SetCurItemData(m_pWzdInfo->ddnsCycle, false);

	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_SERVER_NAME,1));
	pEdit->SetCaption((char*)m_pWzdInfo->ddnsServerName,bRefresh);
	Repaint();
}

void CWzdUS02DDNStipDlg::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();

	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;
	GUI::CButton* pBtn = NULL;

	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned long cx = m_rect.Width() -  2 * x;
	unsigned long cy = 8*m_length[BTN_HEIGHT];

	m_richText.Create(GetRandWndID(),x,y,cx,cy,this);
	string str;
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_DDNS01));
	str.append("\\n");
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_DDNS02));
	str.append("\\n");
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_DDNS03));
	str.append("\\n");
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_DDNS04));
	str.append("\\n");
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_DDNS05));
	m_richText.SetText(str,false);

	y += cy;
	unsigned short height = 8*m_length[LIST_ITEM_HEIGHT];
		
	m_listDDNS.Create(GUI::CWnd::GetRandWndID(), x, y, cx, height, this, 0, 0, false);
	m_listDDNS.AddColumn(NULL, cx * 16 / 32);
	m_listDDNS.AddColumn(NULL, cx * 16 / 32 );

	int row = 0;
	m_listDDNS.AddItem( ROW_DDNS_ENABLE, 0, m_pStrTable->String(E_STR_ID_DDNS) );
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_listDDNS.AddItem( ROW_DDNS_ENABLE, 1, WND_TYPE_CHECK) );
	assert( pCheck );

	row += 1;
	m_listDDNS.AddItem( ROW_DDNS_TYPE, 0, m_pStrTable->String(E_STR_ID_DDNS_TYPE));
	pCombo = dynamic_cast<GUI::CComboCtrl*>( m_listDDNS.AddItem( ROW_DDNS_TYPE, 1, WND_TYPE_COMBO) );
	assert( pCombo );

	row += 1;
	m_listDDNS.AddItem( ROW_DDNS_SERVER_NAME, 0,  m_pStrTable->String(E_STR_ID_DDNS_SERVER));
	pEdit = dynamic_cast<GUI::CEditCtrl*>( m_listDDNS.AddItem( ROW_DDNS_SERVER_NAME, 1, WND_TYPE_EDIT) );
	assert( pCombo );

	row += 1;
	m_listDDNS.AddItem( ROW_DDNS_USER_NAME, 0, m_pStrTable->String(E_STR_ID_USER_NAME) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( m_listDDNS.AddItem( ROW_DDNS_USER_NAME, 1, WND_TYPE_EDIT) );
	assert( pEdit );

	row += 1;
	m_listDDNS.AddItem( ROW_DDNS_PASSWORD, 0,  m_pStrTable->String(E_STR_ID_PASSWORD));
	pEdit = dynamic_cast<GUI::CEditCtrl*>( m_listDDNS.AddItem( ROW_DDNS_PASSWORD, 1, WND_TYPE_EDIT) );
	assert( pEdit );
	pEdit->SetPassword( true );

	row += 1;
	m_listDDNS.AddItem( ROW_DDNS_DOMAIN_INFO, 0, m_pStrTable->String(E_STR_ID_HOST_DOMAIN) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( m_listDDNS.AddItem( ROW_DDNS_DOMAIN_INFO, 1, WND_TYPE_EDIT) );
	assert( pEdit );

	row += 1;
	m_listDDNS.AddItem( ROW_DDNS_UPDATE_CYCLE, 0,  m_pStrTable->String(E_STR_ID_UPDATE_CYCLE) );
	pCombo = dynamic_cast<GUI::CComboCtrl*>( m_listDDNS.AddItem( ROW_DDNS_UPDATE_CYCLE, 1, WND_TYPE_COMBO) );
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
	pBtn = dynamic_cast<GUI::CButton*>(m_listDDNS.AddItem(ROW_DDNS_TEST,1,WND_TYPE_BUTTON,m_length[BTN_WIDTH],m_length[BTN_HEIGHT]));
	assert(pBtn);
	pBtn->SetCaption(m_pStrTable->String(E_STR_ID_TEST),false);
	m_listDDNS.CreateListFocusMap();

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP];
	//y += height+m_length[DLG_OFFSET_SUB_TOP]+5;
	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];

	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
	
}

void CWzdUS02DDNStipDlg::OnDestroy()
{

}

unsigned long CWzdUS02DDNStipDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if( m_bCheckParam )
	{
		//�ڼ��DDNS������ʱ������������Ϣ��Ӧ
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return CDialog::OnPreCommand( ctrlID, keyID ,xPos, yPos );
	}
}

unsigned long CWzdUS02DDNStipDlg::CheckValue()
{
	//mint dns �� TVT IP Server �������з�������ַ���������͵ķ�������ַ������Ϊ��
	if ((DDNS_MINTDNS != m_pWzdInfo->ddnsInfo.type) && (DDNS_TVT_IP_SERVER != m_pWzdInfo->ddnsInfo.type))
	{
		memset(m_pWzdInfo->ddnsServerName, 0, sizeof(m_pWzdInfo->ddnsServerName));
	}
	printf("check status=%d\n",m_StatusddnsCheckOK);
	//�޸�DDNS�󱣴�ʱ����ʾ
	switch(m_StatusddnsCheckOK)
	{
	case CHECK_STATUS_NOCHANGE:							//�������û�иı�
	case CHECK_STATUS_OK:								//���Գɹ�
		break;

	case CHECK_STATUS_NOCHECK:							//û�н��в���		
		//���ѡ���DDNS������Ϊ�տ���ֱ�ӱ���
		if( DDNS_NONE == ( dynamic_cast<GUI::CComboCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_TYPE,1))->GetCurItemData() ) )
		{
			m_StatusddnsCheckOK = CHECK_STATUS_OK;
			break;
		}
		else
		{
			char errtip[256] ={0};
			unsigned long errNum = CheckDdnsParam();

			if(E_STR_ID_MESSAGE_56 == errNum || NETERR_NETPARAMCHECK_REGISTER_SUCC == errNum)
			{
				//DDNS�����Ϣ���ɹ�
				m_StatusddnsCheckOK = CHECK_STATUS_OK;
				errNum = E_STR_ID_NULL;
				break;
			}
			return errNum;
			//��ʾ����ʧ�������Ϣ
		}

	case CHECK_STATUS_FAIL:								//����ʧ��	��ʾ��DDNS������Ч���������趨
		return E_STR_ID_MESSAGE_111;

	default:
		assert(false);
		break;
	}

	return 0;
}

BEGIN_MSG_MAP(CWzdUS02DDNStipDlg, CWzdUS02PageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
	ON_MSG_EX(m_listDDNS.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long  CWzdUS02DDNStipDlg::OnCheck()
{
	if(m_checkCtrl.IsCheck())
	{
		m_pWzdInfo->ShowWzd = 1;
	}
	else
	{
		m_pWzdInfo->ShowWzd = 0;
	}
	return KEY_VIRTUAL_STOP;
}

unsigned long  CWzdUS02DDNStipDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//��
	unsigned short col = keyID&0xffff;			//��

	if( 1 != col )
	{
		printf("%s:%s:%d, row=0x%04x, col=0x%04x\n", __FUNCTION__, __FILE__, __LINE__, row, col);
		return KEY_VIRTUAL_MSG_STOP;
	}
	switch(row)
	{
	case ROW_DDNS_ENABLE:
		{
			CheckEnableUI(true);					

			if(( dynamic_cast<GUI::CCheckCtrl*>( m_listDDNS.GetItemWnd(ROW_DDNS_ENABLE, 1) )->IsCheck() ))
			{
				m_pWzdInfo->ddnsInfo.bEnable = 1;
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
			}
			else
			{
				m_pWzdInfo->ddnsInfo.bEnable = 0;
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHANGE;
			}

			break;
		}
	case ROW_DDNS_TYPE:
		{
			unsigned short oldType = m_pWzdInfo->ddnsInfo.type;

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_TYPE,1));
			unsigned short ddnsType = pCombo->GetCurItemData(); 
			assert(pCombo);
			
			m_pWzdInfo->ddnsInfo.type = ddnsType;
			for( int i=0; i<m_pWzdInfo->ddnsServerCount; i++ )
			{
				if(m_pWzdInfo->ddnsInfo.type == m_pWzdInfo->pDdnsSeverInfo[i].ucDDNSID )
				{
					m_pWzdInfo->ddnsInfo.bUseDomain = NCFG_ENUM_DDNS_SUPPORT_DOMAIN1;
					break;
				}			
			}

			//���ѡ���DDNS������Ϊ�տ���ֱ�ӱ���
			if( DDNS_NONE != m_pWzdInfo->ddnsInfo.type)
			{
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
			}
			else
			{
				m_StatusddnsCheckOK = CHECK_STATUS_NOCHANGE;
			}
			CheckEnableUI(true);
			break;
		}
	case ROW_DDNS_SERVER_NAME:
		{
			const GUI::CEditCtrl* pServerName = dynamic_cast<const GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_SERVER_NAME,1));
			assert( NULL != pServerName);
			if (DDNS_TVT_IP_SERVER == m_pWzdInfo->ddnsInfo.type)
			{
				//���������ƣ�ʹ��IPServerʱ������û�����
				strcpy(m_pWzdInfo->ddnsInfo.user,pServerName->GetCaption().c_str());
			}
			else
			{
				strcpy((char *)m_pWzdInfo->ddnsServerName, pServerName->GetCaption().c_str());
			}

			m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
			break;
		}
	case ROW_DDNS_USER_NAME:
		{
			const GUI::CEditCtrl* pUser = dynamic_cast<const GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_USER_NAME,1));
			assert( NULL != pUser);
			strcpy(m_pWzdInfo->ddnsInfo.user,pUser->GetCaption().c_str());
			m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
			break;
		}
	case ROW_DDNS_PASSWORD:
		{
			const GUI::CEditCtrl* pPass = dynamic_cast<const GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_PASSWORD,1));
			assert(pPass);
			strcpy(m_pWzdInfo->ddnsInfo.password,pPass->GetCaption().c_str());
			m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
			break;
		}
	case ROW_DDNS_DOMAIN_INFO:
		{
			const GUI::CEditCtrl* pDomain = dynamic_cast<const GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
			assert(pDomain);
			strcpy(m_pWzdInfo->ddnsInfo.domain,pDomain->GetCaption().c_str());
		}

		m_StatusddnsCheckOK = CHECK_STATUS_NOCHECK;
		break;
	case ROW_DDNS_UPDATE_CYCLE:
		{
			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1));
			assert(NULL != pCombo);
			m_pWzdInfo->ddnsCycle = pCombo->GetCurItemData();

			break;
		}
	case ROW_DDNS_TEST:
		{
			char errtip[256] ={0};
			unsigned long errNum = CheckDdnsParam();

			MessageOut( GetTopestWnd(), errtip, TIP_ERR, false );

			GetTopestWnd()->Repaint();

			break;
		}
	default:
		assert(false);
		break;
	}
	return 0;
}

void CWzdUS02DDNStipDlg::FUIToIPServerUI()
{
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;


	m_listDDNS.AddItem( ROW_DDNS_SERVER_NAME, 0, m_pStrTable->String(E_STR_ID_NETSERVER_IP));

	//�����û���
	m_listDDNS.AddItem( ROW_DDNS_USER_NAME, 0, "" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_USER_NAME,1));
	assert(pEdit);
	pEdit->Show(false, false);

	//��������
	m_listDDNS.AddItem( ROW_DDNS_PASSWORD, 0, "" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_PASSWORD,1));
	assert(pEdit);
	pEdit->Show(false, false);

	//��������
	m_listDDNS.AddItem( ROW_DDNS_DOMAIN_INFO, 0, "" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(pEdit);
	pEdit->Show(false, false);

	//��������
	m_listDDNS.AddItem( ROW_DDNS_UPDATE_CYCLE, 0, "" );
	pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1));
	assert(NULL != pCombo);
	pCombo->Show(false, false);

}

void CWzdUS02DDNStipDlg::IPServerUIToFUI()
{
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;

	//��ʾ�û���
	m_listDDNS.AddItem( ROW_DDNS_USER_NAME, 0, m_pStrTable->String(E_STR_ID_USER_NAME) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_USER_NAME,1));
	assert(pEdit);
	pEdit->Show(true, false);

	//��ʾ����
	m_listDDNS.AddItem( ROW_DDNS_PASSWORD, 0, m_pStrTable->String(E_STR_ID_PASSWORD) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_PASSWORD,1));
	assert(pEdit);
	pEdit->Show(true, false);

	//��ʾ����
	m_listDDNS.AddItem( ROW_DDNS_DOMAIN_INFO, 0,  m_pStrTable->String(E_STR_ID_HOST_DOMAIN) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1));
	assert(pEdit);
	pEdit->Show(true, false);

	//��ʾ����
	m_listDDNS.AddItem( ROW_DDNS_UPDATE_CYCLE, 0, m_pStrTable->String(E_STR_ID_UPDATE_CYCLE) );
	pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1));
	assert( pCombo );
	pCombo->Show(true, false);
}

void CWzdUS02DDNStipDlg::CheckEnableUI(bool bRefresh)
{
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_listDDNS.GetItemWnd( ROW_DDNS_ENABLE, 1 ));
	if( NULL != pCheck )
	{
		bool bEnableDdns = pCheck->IsCheck();
		m_listDDNS.GetItemWnd(ROW_DDNS_TYPE,1)->SetEnable(bEnableDdns,bRefresh);
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_TYPE,1));
		if ((DDNS_MINTDNS == pCombo->GetCurItemData()) || (DDNS_TVT_IP_SERVER == pCombo->GetCurItemData()))
		{
			//mint ���͵�DDNS��������TVT��IP Server��������DDNS���������֡��������͵ķ���������������DDNS������������
			m_listDDNS.AddItem( ROW_DDNS_SERVER_NAME, 0, m_pStrTable->String(E_STR_ID_SERVER), bRefresh );
			m_listDDNS.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->Show(true,bRefresh);
			m_listDDNS.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->SetCaption((const char *)m_pWzdInfo->ddnsServerName,bRefresh);
			m_listDDNS.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->SetEnable(bEnableDdns,bRefresh);
		}
		else
		{
			m_listDDNS.AddItem( ROW_DDNS_SERVER_NAME, 0, "", bRefresh);
			m_listDDNS.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->SetCaption("",bRefresh);
			m_listDDNS.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->SetEnable(false,bRefresh);
			m_listDDNS.GetItemWnd(ROW_DDNS_SERVER_NAME,1)->Show(false,bRefresh);
		}
		m_listDDNS.GetItemWnd(ROW_DDNS_USER_NAME,1)->SetEnable(bEnableDdns,bRefresh);
		m_listDDNS.GetItemWnd(ROW_DDNS_PASSWORD,1)->SetEnable(bEnableDdns,bRefresh);
		m_listDDNS.GetItemWnd(ROW_DDNS_DOMAIN_INFO,1)->SetEnable(bEnableDdns,bRefresh);

		if (DDNS_DYNDNS == pCombo->GetCurItemData())
		{
			//dyndns���͵ķ���������������������ڡ�ϵͳĬ��ʹ��һ���¸���һ��
			m_listDDNS.AddItem( ROW_DDNS_UPDATE_CYCLE, 0, "", bRefresh );
			m_listDDNS.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1)->SetEnable(false,bRefresh);
			m_listDDNS.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1)->Show(false,bRefresh);
		}
		else
		{
			m_listDDNS.AddItem( ROW_DDNS_UPDATE_CYCLE, 0, m_pStrTable->String(E_STR_ID_UPDATE_CYCLE), bRefresh);
			m_listDDNS.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1)->SetEnable(bEnableDdns,bRefresh);
			m_listDDNS.GetItemWnd(ROW_DDNS_UPDATE_CYCLE,1)->Show(true,bRefresh);
		}
		m_listDDNS.GetItemWnd(ROW_DDNS_TEST,1)->SetEnable(bEnableDdns,bRefresh);

		if(bEnableDdns)
		{
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listDDNS.GetItemWnd(ROW_DDNS_TYPE,1));
			if( DDNS_NONE == pCombo->GetCurItemData() )
			{
				//ѡ���DDNS������Ϊ��ʱ���ܽ���test����
				dynamic_cast<GUI::CButton*>(m_listDDNS.GetItemWnd(ROW_DDNS_TEST,1))->SetEnable( false,bRefresh);
			}
			else
			{
				dynamic_cast<GUI::CButton*>(m_listDDNS.GetItemWnd(ROW_DDNS_TEST,1))->SetEnable( bEnableDdns,bRefresh);
				//�����Ƿ���Ҫ����������Ϣ
				for( int i=0; i<m_pWzdInfo->ddnsServerCount; i++ )
				{
					if( pCombo->GetCurItemData() == m_pWzdInfo->pDdnsSeverInfo[i].ucDDNSID )
					{
						if( NCFG_ENUM_DDNS_SUPPORT_DOMAIN1 != m_pWzdInfo->pDdnsSeverInfo[i].supportproperty )
						{
							m_listDDNS.GetItemWnd( ROW_DDNS_DOMAIN_INFO, 1 )->SetEnable(false, bRefresh);
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

}

unsigned long  CWzdUS02DDNStipDlg::CheckDdnsParam()
{
	//����
	m_bCheckParam = true;

	MessageOut( GetTopestWnd(), E_STR_ID_PLEASE_WAIT, TIP_ERR );;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	DDNS_INFO_EX ddnsInfoEx;
	memcpy(&ddnsInfoEx, &m_pWzdInfo->ddnsInfo, sizeof(DDNS_INFO));
	memcpy(ddnsInfoEx.serverDNS, m_pWzdInfo->ddnsServerName, sizeof(ddnsInfoEx.serverDNS));

	CMessageMan::Instance()->SendMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETPARAMCHECK_DDNS, reinterpret_cast<unsigned char *>(&ddnsInfoEx), sizeof(DDNS_INFO_EX), msgData );

	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);

	switch( static_cast<NET_PROTOCOL>(pCmd->cmdType) )
	{
	case CMD_REPLY_NETPARAMCHECK_DDNS_SUCC:
		{
			m_StatusddnsCheckOK = CHECK_STATUS_OK;
		}
		break;
	case CMD_REPLY_NETPARAMCHECK_DDNS_FAIL:
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

unsigned long CWzdUS02DDNStipDlg::GetErrorTextNum()
{
	ENUM_STRING_ID strID = E_STR_ID_NULL;

	switch( m_netCheckResult )
	{
	case NETERR_NETPARAMCHECK_OK:						//����ͨ��
		strID = E_STR_ID_MESSAGE_56;
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
	default:
		strID = E_STR_ID_MESSAGE_58;
		break;
	}

	return strID;
}
//end

