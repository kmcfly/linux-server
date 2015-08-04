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

#ifndef _DEVICE_SEARCH_DLG_POPUP_H_
#define _DEVICE_SEARCH_DLG_POPUP_H_

#include "Wnd.h"
#include "NetProtocol.h"
#include <vector>

GUI::DLG_END_VALUE PopupDeviceSearchDlg(GUI::CWnd* pWnd,std::vector<DISCOVER_DEVICE_INFO>& vecDevice);

#endif