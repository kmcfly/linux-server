/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgCruiseDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_CRUISE_DLG_H_
#define _CFG_CRUISE_DLG_H_

#include "Wnd.h"

typedef std::vector<PTZ_CURISE_POINT> CRUISE_POINT_LIST;
class CRUISE_INFO 
{
public:
	unsigned long	index;	
	std::string		strName;
	CRUISE_POINT_LIST point_list;
};
typedef std::vector<CRUISE_INFO> CRUISE_INFO_LIST; 

GUI::DLG_END_VALUE PopupCruisePresetDlg(GUI::CWnd* pWnd, CRUISE_INFO_LIST& cruiseList, int curCruise );


#endif//_CFG_CRUISE_DLG_H_

