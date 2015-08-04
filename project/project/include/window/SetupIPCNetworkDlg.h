#ifndef __SETUP_IPC_NETWORK_DLG_H__
#define __SETUP_IPC_NETWORK_DLG_H__

#include "Dialog.h"
#include "ListCtrl.h"
#include "NetDeviceManager.h"

using namespace GUI;

class CSetupIpcNetworkDlg : public GUI::CDialog
{
public:
	CSetupIpcNetworkDlg(DISCOVER_DEVICE_INFO & ipcDeviceInfo);
	~CSetupIpcNetworkDlg();

protected:
	void OnInitial();

	unsigned long OnOK();

	bool DoCheckUIValue();

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

private:
	void ShowLocalNetWorkInfo();
protected:
	GUI::CListCtrl		m_listCtrl;

	enum LIST_ROW
	{
		ROW_IP_ADDRESS = 0,
		ROW_NETMASK,
		ROW_GATEWAY,
		ROW_NOT_USE,
		ROW_USER_NAME,
		ROW_PASSWORD,
		ROW_NOT_USE1,
		ROW_LOCAL_IP,
		ROW_LOCAL_NETMASK,
		ROW_LOCAL_GATEWAY,
	};

private:
	DISCOVER_DEVICE_INFO	m_ipcDeviceInfo;
	ND_DEVICE_NETWORK_INFO	m_netdeviceInfo;
};

#endif
