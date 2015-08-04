/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-09-29
** Name         : DeviceManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _DEVICE_MAN_DLG_
#define _DEVICE_MAN_DLG_

#include "Dialog.h"
#include "ListCtrlExMulti.h"
#include "Button.h"
#include "NetDeviceManager.h"
#include "DeviceSearchDlgPopup.h"

#include "IPCFrameEncode.h"

using namespace GUI;

class CDeviceManDlg : public GUI::CDialog
{
public:
	CDeviceManDlg();
	~CDeviceManDlg();
public:

protected:
	void OnInitial();
	void OnPostInitial();

	unsigned long OnApply();
	void EndDialog(DLG_END_VALUE result);
protected:
	bool DoAdd( const IP_DEVICE_INFO & deviceInfo , bool bRefresh = true );
	bool DoModify( const IP_DEVICE_INFO & deviceInfo, long index );
	bool DoDelete( const long oldSel );

	void SetModify( bool modify = true );
	bool GetModify();

	// ��DVR����ӿڻ�ȡ�������ݸ��µ��б�
	void UpdateDataUI( bool bRefresh );
	// �ӽ��汣��ͨ������ӿڱ������ݵ�DVR��������������п��ܱ仯����Ҫ��ˢ��UI
	bool SaveData();

	int GetCurrentValidChnNum(bool bPOEDev, int channel);

protected:
	DECLARE_MSG_MAP()

	unsigned long OnAdd();
	unsigned long OnModify();
	unsigned long OnDelete();
	unsigned long OnSearch();
	unsigned long OnDeleteAll();
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrlExMulti<IP_DEVICE_INFO>	m_deviceList;
	GUI::CButton				m_btnAdd;
	GUI::CButton				m_btnModify;
	GUI::CButton				m_btnDelete;
	GUI::CButton				m_btnDeleteAll;
	GUI::CButton				m_btnSearch;

	//CDeviceSearchDlg		m_dlgDeviceSearch;

	GUI::CButton*				m_pBtnApply;

private:
	bool						m_bModify;					//���޸Ĺ�
	unsigned long				m_maxDevice;				//����豸����

	static int					m_rndID;

	NCFG_IPC_SUPPORT_ENCODE		m_ipcSupportEncode;

	unsigned long				m_bitStreamCount;
	unsigned long				*m_pBitStreamValue;

	CIPCFrameEncode				m_ipcFrameEncode;
};


#endif