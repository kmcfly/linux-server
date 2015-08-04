/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-12-01
** Name         : DetailsInfo.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _DETAILS_INFO_DLG_H_
#define _DETAILS_INFO_DLG_H_

#include "gui.h"
#include "Dialog.h"
#include <vector>

using namespace GUI;

class CDetailsInfoDlg : public GUI::CDialog
{
public:
	CDetailsInfoDlg();
	~CDetailsInfoDlg();

	void AddInfo(const std::string& str1, const std::string& str2);

protected:
	void OnInitial();
	void OnDestroy();
	virtual void OnPaint(PAINT_STATUS status);

	typedef struct _details_info 
	{
		std::string first;
		std::string second;
	}DETAILS_INFO;

	std::vector<DETAILS_INFO>		m_infoVec;

	unsigned short m_cxMaxFirst;
	unsigned short m_cxMaxSecond;
};



#endif

//end
