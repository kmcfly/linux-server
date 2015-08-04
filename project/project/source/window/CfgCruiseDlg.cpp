/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgCruiseDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgCruiseDlg.h"
#include "CfgCruisePresetDlg.h"

GUI::DLG_END_VALUE PopupCruisePresetDlg(GUI::CWnd* pWnd, CRUISE_INFO_LIST& cruiseList, int curCruise )
{
	CCfgCruisePresetDlg dlg;
	dlg.SetCruiseData(cruiseList, curCruise);

	GUI::DLG_END_VALUE ret = dlg.DoModal(pWnd);
	if(DLG_OK == ret)
	{
		dlg.GetCruiseData(cruiseList);
	}
	return ret;
}

