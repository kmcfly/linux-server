#include "CfgNetworkFtpDlg.h"

CCfgNetworkFtpDlg::CCfgNetworkFtpDlg()
{

}
CCfgNetworkFtpDlg::~CCfgNetworkFtpDlg()
{

}

void CCfgNetworkFtpDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CWnd * pWnd		= NULL;

	GUI::CButton		* pButton	= NULL;
	GUI::CComboCtrl		* pCombo	= NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = height /m_length[LIST_ITEM_HEIGHT]; //List¿ò´øÍ·

	m_triggerDlg.Create(GetRandWndID(), x, y, width, height, this, 0, 0);
}

void CCfgNetworkFtpDlg::DoOther()
{

}

void CCfgNetworkFtpDlg::EndOther()
{

}

bool CCfgNetworkFtpDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_NET_FTP:	
		{
			m_triggerDlg.SetFtpCfgInfo((TVT_FTP_CONFIG *) pData, 4);
			m_triggerDlg.InitCtrlData(false);
			RefreshLive();
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgNetworkFtpDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_NET_FTP:
		{
			TVT_FTP_CONFIG ftpCfg;
			m_triggerDlg.GetFtpCfgInfo(&ftpCfg);
			memcpy(pData, &ftpCfg, sizeof(TVT_FTP_CONFIG));
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

void CCfgNetworkFtpDlg::TestEmailInfo(void)
{

}

void CCfgNetworkFtpDlg::SetErrorText(unsigned char checkResult)
{

}

BEGIN_MSG_MAP(CCfgNetworkFtpDlg, CCfgDlg)
 ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_ENTER, OnFtpCfgInfoChanged)
END_MSG_MAP()


unsigned long CCfgNetworkFtpDlg::OnFtpCfgInfoChanged()
{
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

