
#ifndef _FUNC_CUSTOM_DLG_H_
#define _FUNC_CUSTOM_DLG_H_

#include "Wnd.h"
#include "Dialog.h"
#include "ListCtrl.h"

class CFuncCustomDlg : public GUI::CDialog
{
public:
	CFuncCustomDlg();
	~CFuncCustomDlg();

protected:
	void OnInitial();

	unsigned long OnOK();

	GUI::CListCtrl m_listCtrl;
};

#endif

//end
