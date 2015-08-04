#ifndef _SELECT_DISK_DLG_H_
#define _SELECT_DISK_DLG_H_
#include "Button.h"
#include "Dialog.h"
#include "ComboCtrl.h"
#include "ListCtrl.h"
#include <vector>

using namespace GUI;

class CSelectDiskDlg : public GUI::CDialog
{
public:
	CSelectDiskDlg();
	~CSelectDiskDlg();

	void SetInitialInfo(DVR_DISK_OWNER_LIST *pDiskOwnerList);

	void GetSelDisk(DVR_DISK_OWNER& diskInfo);

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();

protected:
	unsigned long OnOK();

protected:
	GUI::CListCtrl				m_listCtrl;

protected:
	DVR_DISK_OWNER_LIST         *m_pDiskOwnerList;
	DVR_DISK_OWNER				m_diskInfo;

	enum LIST_COL
	{
		COL_OWNER,
		COL_DISK_COUNT,
		COL_MAC_MARK,
	};
};

#endif


