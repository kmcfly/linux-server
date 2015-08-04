

#include "SelectDiskDlg.h"
#include "NetProtocol.h"
#include "SearchManDlg.h"
#include "BackupDlg.h"

//////////////////////////////////////////////////////////////////////////
CSelectDiskDlg::CSelectDiskDlg() 
{
	m_pDiskOwnerList = NULL;
}

CSelectDiskDlg::~CSelectDiskDlg()
{

}

void CSelectDiskDlg::SetInitialInfo(DVR_DISK_OWNER_LIST *pDiskOwnerList)
{
	assert(pDiskOwnerList != NULL);
	m_pDiskOwnerList = pDiskOwnerList;
}

void CSelectDiskDlg::GetSelDisk(DVR_DISK_OWNER& diskInfo)
{
	memcpy(&diskInfo, &m_diskInfo, sizeof(DVR_DISK_OWNER));
}

//////////////////////////////////////////////////////////////////////////
void CSelectDiskDlg::OnInitial()
{
	assert(m_pDiskOwnerList != NULL);
	assert(m_pDiskOwnerList->GetCount() > 0);

	GUI::CDialog::OnInitial();

	SetImage(BMP_MEM_DLG_SMALL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SELECT_DISK), false);
	SetDefaultBtn(DEFAULT_BTN_OK);
	///////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - m_length[DLG_OFFSET_LEFT] * 2;
	unsigned short cy = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	m_listCtrl.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1, true);

	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_OWNER), cx * 12 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_DISK_COUNT), cx * 7 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_DISK_IDENT), cx * 13 / 32 - m_length[SCROLL_WIDTH] - 2);
	
	POS pos = m_pDiskOwnerList->GetHeadPosition();
	int row = 0;
	while (NULL != pos)
	{
		//char str[128] = {0};
		std::string strOwner;
		DVR_DISK_OWNER &owner = m_pDiskOwnerList->GetAt(pos);
		if (NET_DISK_OWNED_BY_THIS == owner.dvrIndex)
		{
			//sprintf(str, "this device");
			strOwner = m_pStrTable->String(E_STR_ID_THIS_DEVICE);
		}
		else
		{
			//sprintf(str, "other%d device", owner.dvrIndex);
			strOwner += m_pStrTable->String(E_STR_ID_OTHER);
			char tmp[16];
			sprintf(tmp, "%d ", owner.dvrIndex);
			strOwner += tmp;
			strOwner += m_pStrTable->String(E_STR_ID_DEVICE);
		}

		m_listCtrl.AddItem(row, COL_OWNER, strOwner.c_str());

		char str[64] = {0};
		sprintf(str, "%d", owner.diskNum);
		m_listCtrl.AddItem(row, COL_DISK_COUNT, str);

		sprintf(str, "%0.2X-%0.2X-%0.2X-%0.2X-%0.2X-%0.2X", owner.dvrId[0], owner.dvrId[1], owner.dvrId[2], owner.dvrId[3], owner.dvrId[4], owner.dvrId[5]);
		m_listCtrl.AddItem(row, COL_MAC_MARK, str);
		//m_vecPos.push_back(pos);

		row += 1;
		m_pDiskOwnerList->GetNext(pos);
	}

	m_listCtrl.EnableSingleSelect(true);
	m_listCtrl.SetCurSingleSelect(0, false);

	//
	memset(&m_diskInfo, 0, sizeof(DVR_DISK_OWNER));
}



void CSelectDiskDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();	
	m_pDiskOwnerList = NULL;
}

BEGIN_MSG_MAP(CSelectDiskDlg, GUI::CDialog)
	ON_MSG(m_listCtrl.GetID(), KEY_VIRTUAL_DBCLICK, OnOK)
END_MSG_MAP()

unsigned long CSelectDiskDlg::OnOK()
{
	int sel = m_listCtrl.GetCurSingleSelect();

	POS pos = m_pDiskOwnerList->GetHeadPosition();
	int index = 0;
	while (NULL != pos)
	{
		if(index == sel)
		{
			m_diskInfo = m_pDiskOwnerList->GetAt(pos);
			break;
		}

		index ++;
		m_pDiskOwnerList->GetNext(pos);
	}
	assert(index >= sel);

	return GUI::CDialog::OnOK();
}
