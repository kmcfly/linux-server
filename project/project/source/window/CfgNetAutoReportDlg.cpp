#include "CfgNetAutoReportDlg.h"


CCfgNetAutoReportDlg::CCfgNetAutoReportDlg()
{

}

CCfgNetAutoReportDlg::~CCfgNetAutoReportDlg()
{

}

void CCfgNetAutoReportDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgNetAutoReportList;
	GUI::CWnd * pWnd = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;
	GUI::CEditCtrl* pHost = NULL;
	GUI::CCheckCtrl* pCheck = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - 4 );

	list.AddItem(ROW_USE, 0, m_pStrTable->String(E_STR_ID_ENABLE));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_USE, 1, WND_TYPE_CHECK));
	assert(pCheck);

	list.AddItem(ROW_SERVER_HOST, 0, m_pStrTable->String(E_STR_ID_SERVER));
	pHost = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_SERVER_HOST, 1, WND_TYPE_EDIT));
	assert(pHost);

	list.AddItem(ROW_SERVER_PORT, 0, m_pStrTable->String(E_STR_ID_SERVER_PORT));
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(list.AddItem(ROW_SERVER_PORT, 1, WND_TYPE_NUM_EDIT));
	pNumEdit->SetRange(0, 65535);
	assert(pNumEdit);

	list.AddItem(ROW_DEV_ID, 0, m_pStrTable->String(E_STR_ID_DEV_ID));
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(list.AddItem(ROW_DEV_ID, 1, WND_TYPE_NUM_EDIT));
	pNumEdit->SetRange(0, 65535);
	assert(pNumEdit);

	list.CreateListFocusMap();

}


bool CCfgNetAutoReportDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{

	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_AUTO_REPORT:
		{
			assert(len == sizeof(NET_AUTO_REPORT));
			const NET_AUTO_REPORT *pValue = reinterpret_cast<const NET_AUTO_REPORT*>(pData);

			GUI::CNumEditCtrl* pNumEdit = NULL;
			GUI::CEditCtrl* pHost = NULL;
			GUI::CCheckCtrl* pCheck = NULL;

			pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetAutoReportList.GetItemWnd(ROW_USE,1));
			assert( NULL != pCheck);
			pCheck->SetCheck(pValue->bUse, bRefresh);

			pHost = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetAutoReportList.GetItemWnd(ROW_SERVER_HOST,1));
			assert( NULL != pHost );
			pHost->SetCaption(pValue->host, bRefresh);

			pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(m_cfgNetAutoReportList.GetItemWnd(ROW_SERVER_PORT,1));
			assert(NULL!= pNumEdit);
			pNumEdit->SetValue(pValue->dwPort,bRefresh);

			pNumEdit = dynamic_cast<GUI::CNumEditCtrl *>(m_cfgNetAutoReportList.GetItemWnd(ROW_DEV_ID,1));
			assert(NULL!= pNumEdit);
			pNumEdit->SetValue(pValue->ID,bRefresh);
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}


bool CCfgNetAutoReportDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	const GUI::CListCtrl& list = m_cfgNetAutoReportList;

	switch (cfgID)
	{
	case NCFG_ITEM_AUTO_REPORT:
		{
			assert(len == sizeof(NET_AUTO_REPORT));
			NET_AUTO_REPORT *pValue = reinterpret_cast<NET_AUTO_REPORT*>(pData);

			GUI::CNumEditCtrl* pNumEdit = NULL;
			GUI::CEditCtrl* pHost = NULL;
			GUI::CCheckCtrl* pCheck = NULL;

			pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)list.GetItemWnd(ROW_USE,1));
			pHost = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)list.GetItemWnd(ROW_SERVER_HOST,1));
			pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>((GUI::CNumEditCtrl*)list.GetItemWnd(ROW_SERVER_PORT,1));

			assert(pCheck);
			assert(pNumEdit);
			assert(pHost);

			snprintf(pValue->host, 256, "%s", pHost->GetCaption().c_str());
			pValue->dwPort = pNumEdit->GetValue();

			pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>((GUI::CNumEditCtrl*)list.GetItemWnd(ROW_DEV_ID,1));

			assert(pNumEdit);
			pValue->ID = pNumEdit->GetValue();

			pValue->bUse = pCheck->IsCheck()?1:0;
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}


BEGIN_MSG_MAP(CCfgNetAutoReportDlg, CCfgDlg)
	ON_MSG_EX(m_cfgNetAutoReportList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgNetAutoReportDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//ÐÐ
	unsigned short col = keyID&0xffff;			//ÁÐ

	if (!IsModify())
	{
		switch (row)
		{
		case ROW_USE:
		case ROW_SERVER_HOST:
		case ROW_SERVER_PORT:
		case ROW_DEV_ID:
			if (IsChange(NCFG_ITEM_AUTO_REPORT))
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
