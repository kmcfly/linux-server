
#include "CfgNetServerDlg.h"


CCfgNetServerDlg::CCfgNetServerDlg()
{

}

CCfgNetServerDlg::~CCfgNetServerDlg()
{

}

void CCfgNetServerDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgNetServerList;
	GUI::CWnd * pWnd = NULL;
	GUI::CNumEditCtrl* pPort = NULL;
	GUI::CIPAddressCtrl* pIP = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - 4 );

	list.AddItem(ROW_SERVER_IP, 0, m_pStrTable->String(E_STR_ID_NETSERVER_IP));
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.AddItem(ROW_SERVER_IP, 1, WND_TYPE_IP));
	assert(pIP);

	list.AddItem(ROW_SERVER_PORT, 0, m_pStrTable->String(E_STR_ID_NETSERVER_PORT));
	pPort = dynamic_cast<GUI::CNumEditCtrl*>(list.AddItem(ROW_SERVER_PORT, 1, WND_TYPE_NUM_EDIT));
	assert( pPort );
	pPort->SetMaxStringLen( 5 );
	pPort->SetRange(0,65535);


	
	list.CreateListFocusMap();

}


bool CCfgNetServerDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{

	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_NET_SERVER:
		{
			assert(len == sizeof(MDVR_NETSERVER_CFG));
			const MDVR_NETSERVER_CFG *pValue = reinterpret_cast<const MDVR_NETSERVER_CFG*>(pData);

			GUI::CIPAddressCtrl* pIP = NULL;
			GUI::CNumEditCtrl* pPort = NULL;

			pIP = dynamic_cast<GUI::CIPAddressCtrl*>(m_cfgNetServerList.GetItemWnd(ROW_SERVER_IP,1));
			assert( NULL != pIP );
			pIP->SetIP(pValue->serverIP,bRefresh);

			pPort = dynamic_cast<GUI::CNumEditCtrl*>(m_cfgNetServerList.GetItemWnd(ROW_SERVER_PORT,1));
			assert(NULL!= pPort);
			pPort->SetValue(pValue->serverPort,bRefresh);
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}


bool CCfgNetServerDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	const GUI::CListCtrl& list = m_cfgNetServerList;

	switch (cfgID)
	{
	case NCFG_ITEM_NET_SERVER:
		{
			assert(len == sizeof(MDVR_NETSERVER_CFG));
			MDVR_NETSERVER_CFG *pValue = reinterpret_cast<MDVR_NETSERVER_CFG*>(pData);

			GUI::CIPAddressCtrl* pIP = NULL;
			GUI::CNumEditCtrl* pPort = NULL;

			pIP = dynamic_cast<GUI::CIPAddressCtrl*>((GUI::CIPAddressCtrl*)list.GetItemWnd(ROW_SERVER_IP,1));
			pPort = dynamic_cast<GUI::CNumEditCtrl*>((GUI::CNumEditCtrl*)list.GetItemWnd(ROW_SERVER_PORT,1));

			assert(pIP);
			assert(pPort);
			pValue->serverIP = pIP->GetIP();
			pValue->serverPort = pPort->GetValue();

		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

BEGIN_MSG_MAP(CCfgNetServerDlg, CCfgDlg)
	ON_MSG_EX(m_cfgNetServerList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgNetServerDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//ÐÐ
	unsigned short col = keyID&0xffff;			//ÁÐ

	if (!IsModify())
	{

		switch (row)
		{
		case ROW_SERVER_IP:
		case ROW_SERVER_PORT:
			if (IsChange(NCFG_ITEM_NET_SERVER))
			{
				SetModify(true);
			}
			break;

		default:
			assert(false);
			break;
		}

		return KEY_VIRTUAL_MSG_CONTINUE;
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

}
