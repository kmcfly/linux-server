/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-09-29
** Name         : DeviceInfoDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _DEVICE_INFO_DLG_H_
#define _DEVICE_INFO_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "NetDeviceManager.h"

using namespace GUI;

class CDeviceInfoDlg : public GUI::CDialog
{
public:
	CDeviceInfoDlg();
	~CDeviceInfoDlg();

	enum DLG_TYPE
	{
		DEVICE_ADD = 0,
		DEVICE_MODIFY,
	};

	enum DLG_ERROR
	{
		DEVICE_INFO_OK = 0,
		DEVICE_INFO_PRODUCT,
		DEVICE_INFO_IP,
		DEVICE_INFO_USER_PASS,
	};

public:
	void SetDlgType( DLG_TYPE type = DEVICE_ADD );
	void SetDeviceInfo( const IP_DEVICE_INFO& deviceInfo );
	void GetDeviceInfo( IP_DEVICE_INFO& deviceInfo );
	void SetAllDeviceInfo(IP_DEVICE_INFO *pNetLinkInfo, long count);

	void SetManufacturerVar(bool var){m_bManufacturerVar = var;};
	bool GetManufacturerVar(){return m_bManufacturerVar;};

protected:
	void OnInitial();

	unsigned long OnOK();

	void DoCheckUI( bool bRefresh );

	DLG_ERROR DoCheckValue();
	void DoGetValue();
	void DoMessageError( DLG_ERROR err );

	//str 17Î» ·Ö¸î·û"-"£¬" "  unsigned char MAC[6]
	bool GetMac( const std::string& str, unsigned char pMac[] ) const;
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl		m_listCtrl;

	enum LIST_ROW
	{
		ROW_ENABLE = 0,
		ROW_CHNN,
		ROW_MANUFACTURER,
//		ROW_MODEL,
		ROW_IP,
		ROW_PORT,
		ROW_HTTP_PORT,
		ROW_CTRL_PORT,
		ROW_USERNAME,
		ROW_PASSWORD,

		ROW_NUM,
	};

private:
	DLG_TYPE				m_dlgType;
	IP_DEVICE_INFO			m_deviceInfo;
	IP_DEVICE_INFO			*m_pDeviceInfo;
	long					m_deviceCount;

	std::vector<NVR_SUPPORT_MANUFACTURER> m_vecManufacturer;

	int						m_rows[ROW_NUM];

	bool					m_bManufacturerVar;
};


#endif