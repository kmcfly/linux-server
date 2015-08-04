/***********************************************************************************************************************
*Copyrights (C) tongwei video Tech.co., Ltd
*
*File name:
*
*Auther:
*
*Version:
*
*Date:
*
*History:
*
*
*
*************************************************************************************************************************/


#ifndef __CRUISE_PREVIEWDLG__
#define __CRUISE_PREVIEWDLG__

#include "Dialog.h"
#include "Static.h"
#include "Rect.h"

using namespace GUI;

//////////////////////////////////////////////////////////////////////////

class CCruisePreviewDlg:public GUI::CDialog
{

public:
	CCruisePreviewDlg();
	~CCruisePreviewDlg();

protected:

	void OnInitial();
	void OnPaint(PAINT_STATUS status);

private:

};

#endif

