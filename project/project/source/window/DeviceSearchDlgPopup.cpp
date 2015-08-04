/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-22
** Name         : DeviceSearchDlgPopup.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "DeviceSearchDlgPopup.h"
#include "DeviceSearchDlg.h"


GUI::DLG_END_VALUE PopupDeviceSearchDlg(GUI::CWnd* pWnd,std::vector<DISCOVER_DEVICE_INFO>& vecDevice)
{
	CDeviceSearchDlg dlg;

	GUI::DLG_END_VALUE ret = dlg.DoModal(pWnd);

	if( DLG_OK == ret )
	{
		vecDevice = dlg.GetSelDeviceInfo();
	}

	return ret;
}
