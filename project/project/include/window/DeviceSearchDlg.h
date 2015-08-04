/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-07
** Name         : DeviceSearchDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _DEVICE_SEARCH_DLG_H_
#define _DEVICE_SEARCH_DLG_H_

#include "Dialog.h"
#include "ListCtrlEx.h"
#include "NdLibMan.h"
#include <vector>
#include "NetProtocol.h"

using namespace GUI;

class CDeviceSearchDlg : public GUI::CDialog
{
public:
	CDeviceSearchDlg();
	~CDeviceSearchDlg();

	std::vector<DISCOVER_DEVICE_INFO>& GetSelDeviceInfo();

protected:
	void OnInitial();
	void OnPostInitial();

	unsigned long OnOK();
	unsigned long OnApply();

	DECLARE_MSG_MAP();
	unsigned long OnDoubleClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnEditIPCNetwork();
	
protected:
	// ��DVR����ӿڻ�ȡ�������ݸ��µ��б�
	void UpdateDataUI( bool bRefresh );
	unsigned long EditIPCNetwork();

	GUI::CListCtrlEx<DISCOVER_DEVICE_INFO>	m_listCtrlEx;

private:
	std::vector<DISCOVER_DEVICE_INFO>	m_selDeviceVec;		//����ѡ�е��豸��Ϣ

	GUI::CButton				m_btnSetupIpc;
};


#endif