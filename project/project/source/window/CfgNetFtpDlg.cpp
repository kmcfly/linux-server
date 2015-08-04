#include "CfgNetFtpDlg.h"

CCfgNetFtpDlg::CCfgNetFtpDlg()
{

}
CCfgNetFtpDlg::~CCfgNetFtpDlg()
{

}

void CCfgNetFtpDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl & list	= m_cfgFtpList ;
	GUI::CWnd * pWnd		= NULL;

	GUI::CButton		* pButton	= NULL;
	GUI::CComboCtrl		* pCombo	= NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = height /m_length[LIST_ITEM_HEIGHT]; //List���ͷ
	if( ROW_COUNT > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - offsetEdge );

	//���á�����
	list.AddItem(ROW_FTP_ENABLE, 0, "");
	GUI::CCheckCtrl * pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_FTP_ENABLE, 1, WND_TYPE_CHECK));
	assert(pCheck);

	//��������ַ
	list.AddItem(ROW_FTP_SERVER_ADDR, 0, m_pStrTable->String(E_STR_ID_FTP_SERVICE));
	GUI::CEditCtrl * pEditIP = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_FTP_SERVER_ADDR, 1, WND_TYPE_EDIT));
	assert(pEditIP);

	//�������˿�
	list.AddItem(ROW_FTP_SERVER_PORT, 0, m_pStrTable->String(E_STR_ID_PORT));
	GUI::CNumEditCtrl * pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(list.AddItem(ROW_FTP_SERVER_PORT, 1, WND_TYPE_NUM_EDIT));
	assert(pNumEdit);

	//�Ƿ�ʹ��SSL����
	list.AddItem(ROW_FTP_SSL_CHECK, 0, m_pStrTable->String(E_STR_ID_SLL_CHECK));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_FTP_SSL_CHECK, 1, WND_TYPE_CHECK));
	assert(pCheck);

	//�Ƿ�ʹ��������¼
	list.AddItem(ROW_FTP_ANONYMITY_LOGIN, 0, m_pStrTable->String(E_STR_ID_FTP_ANONYMITY));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_FTP_ANONYMITY_LOGIN, 1, WND_TYPE_CHECK));
	assert(pCheck);

	//�û���
	list.AddItem(ROW_FTP_USER_NAME, 0, m_pStrTable->String(E_STR_ID_USER_NAME));
	GUI::CEditCtrl * pEditUset = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_FTP_USER_NAME, 1, WND_TYPE_EDIT));
	pEditUset->SetMaxStringLen(67);
	assert(pEditUset);

	//����
	list.AddItem(ROW_FTP_USER_PASS, 0, m_pStrTable->String(E_STR_ID_PASSWORD));
	GUI::CEditCtrl * pEditPassword = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_FTP_USER_PASS, 1, WND_TYPE_EDIT));
	assert(pEditPassword);
	pEditPassword->SetMaxStringLen(35);
	pEditPassword->SetPassword(true);

	//�ϴ�·��
	list.AddItem(ROW_FTP_UPDATE_URL, 0, m_pStrTable->String(E_STR_ID_FTP_REMOTE_DIR));
	GUI::CEditCtrl * pEditUpdateURL = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_FTP_UPDATE_URL, 1, WND_TYPE_EDIT));
	assert(pEditUpdateURL);

	list.CreateListFocusMap();
}

BEGIN_MSG_MAP(CCfgNetFtpDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgFtpList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

void CCfgNetFtpDlg::DoOther()
{

}

void CCfgNetFtpDlg::EndOther()
{

}

bool CCfgNetFtpDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_NET_FTP:	
		{
			assert(len == sizeof(TVT_FTP_CONFIG));
			const TVT_FTP_CONFIG * pFtpCfg = reinterpret_cast<const TVT_FTP_CONFIG*>(pData);

			//�Ƿ�����
			GUI::CCheckCtrl* pCheckEnable = dynamic_cast<GUI::CCheckCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_ENABLE,1));
			assert(NULL != pCheckEnable);
			pCheckEnable->SetCheck(pFtpCfg->enableFTP, bRefresh);

			//��������ַ
			GUI::CEditCtrl * pEditServerAddr = dynamic_cast<GUI::CEditCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_SERVER_ADDR,1));
			assert(NULL != pEditServerAddr);
			pEditServerAddr->SetCaption(pFtpCfg->serverURL, bRefresh);

			//�������˿�
			GUI::CNumEditCtrl * pNumServerPort = dynamic_cast<GUI::CNumEditCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_SERVER_PORT,1));
			assert(NULL != pNumServerPort);
			pNumServerPort->SetValue(pFtpCfg->serverPort, bRefresh);

			//�Ƿ�ʹ��SSL
			GUI::CCheckCtrl* pSSLCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_SSL_CHECK,1));
			assert(NULL!= pSSLCheck);
			pSSLCheck->SetCheck(pFtpCfg->encryption, bRefresh);

			//�Ƿ�������¼ANONYMITY
			GUI::CCheckCtrl* pAnonymityCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_ANONYMITY_LOGIN,1));
			assert(NULL!= pAnonymityCheck);
			pAnonymityCheck->SetCheck(pFtpCfg->anonymous, bRefresh);

			//�û���
			GUI::CEditCtrl * pEditUser = dynamic_cast<GUI::CEditCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_USER_NAME,1));
			assert(NULL != pEditUser);
			pEditUser->SetCaption(pFtpCfg->account, bRefresh );

			//����
			GUI::CEditCtrl * pEditPassword = dynamic_cast<GUI::CEditCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_USER_PASS,1));
			assert(NULL != pEditPassword);
			pEditPassword->SetCaption(pFtpCfg->password, bRefresh);

			//Զ��·��
			GUI::CEditCtrl * pEditRemoteUrl = dynamic_cast<GUI::CEditCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_UPDATE_URL,1));
			assert(NULL != pEditRemoteUrl);
			pEditRemoteUrl->SetCaption(pFtpCfg->remotePath, bRefresh);
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgNetFtpDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_NET_FTP:
		{			
			assert(len == (sizeof(TVT_FTP_CONFIG)));
			TVT_FTP_CONFIG * pFtpConfig = reinterpret_cast<TVT_FTP_CONFIG *>(pData);

			//�Ƿ�����
			const GUI::CCheckCtrl* pCheckEnable = dynamic_cast<const GUI::CCheckCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_ENABLE,1));
			assert(NULL != pCheckEnable);
			pFtpConfig->enableFTP = pCheckEnable->IsCheck()?1:0;

			//��������ַ
			const GUI::CEditCtrl * pEditServerAddr = dynamic_cast<const GUI::CEditCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_SERVER_ADDR,1));
			assert(NULL != pEditServerAddr);
			strcpy(pFtpConfig->serverURL, pEditServerAddr->GetCaption().c_str());

			//�������˿�
			const GUI::CNumEditCtrl * pNumServerPort = dynamic_cast<const GUI::CNumEditCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_SERVER_PORT,1));
			assert(NULL != pNumServerPort);
			pFtpConfig->serverPort = pNumServerPort->GetValue();

			//�Ƿ�ʹ��SSL
			const GUI::CCheckCtrl* pSSLCheck = dynamic_cast<const GUI::CCheckCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_SSL_CHECK,1));
			assert(NULL!= pSSLCheck);
			pFtpConfig->encryption = pSSLCheck->IsCheck()?1:0;

			//�Ƿ�������¼ANONYMITY
			const GUI::CCheckCtrl* pAnonymityCheck = dynamic_cast<const GUI::CCheckCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_ANONYMITY_LOGIN,1));
			assert(NULL!= pAnonymityCheck);
			pFtpConfig->anonymous = pAnonymityCheck->IsCheck()?1:0;

			//�û���
			const GUI::CEditCtrl * pEditUser = dynamic_cast<const GUI::CEditCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_USER_NAME,1));
			assert(NULL != pEditUser);
			strcpy(pFtpConfig->account, pEditUser->GetCaption().c_str());

			//����
			const GUI::CEditCtrl * pEditPassword = dynamic_cast<const GUI::CEditCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_USER_PASS,1));
			assert(NULL != pEditPassword);
			strcpy(pFtpConfig->password, pEditPassword->GetCaption().c_str());

			//Զ��·��
			const GUI::CEditCtrl * pEditRemoteURL = dynamic_cast<const GUI::CEditCtrl*>(m_cfgFtpList.GetItemWnd(ROW_FTP_UPDATE_URL,1));
			assert(NULL != pEditRemoteURL);
			strcpy(pFtpConfig->remotePath, pEditRemoteURL->GetCaption().c_str());
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

void CCfgNetFtpDlg::TestEmailInfo(void)
{

}

void CCfgNetFtpDlg::SetErrorText(unsigned char checkResult)
{

}


unsigned long CCfgNetFtpDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//��
	unsigned short col = keyID&0xffff;			//��

	if (!IsModify())
	{
		if(IsChange(NCFG_ITEM_NET_FTP))
		{
			SetModify(true);
		}
		
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
}