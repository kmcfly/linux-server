/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2011-07-15
** Name         : CfgLocalNetModeDlg.h//局域网模式对话框
** Version      : 1.0
** Description  : 
** Modify Record:

***********************************************************************/

#ifndef _CFG_LOCALNETMODE_DLG_H_
#define _CFG_LOCALNETMODE_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#include "dvrdvsdef.h"
class CCfgLocalNetModeDlg : public GUI::CDialog
{
public:
	CCfgLocalNetModeDlg();
	~CCfgLocalNetModeDlg();

	unsigned long CheckValue();
	void SetDataInfo(const SETUPDATAINFO &DataInfo) { m_dataInfo = DataInfo;}

	//刷新数据到界面
	void RefreshView(bool bRefresh=true);

protected:
	typedef struct _get_ip_mode_
	{
		bool bGetIpModeAuto;
		bool bGetIpModePPPOE;
	}GET_IP_MODE;

	typedef enum _refrash_ui_num
	{
		REFRASH_UI_AUTOIP  =0,
		REFRASH_UI_PPPOE   =1,
		REFRASH_UI_ALL     =2,
	}REFRASH_UI_NUM;

	typedef enum _parameter_check_status
	{
		CHECK_STATUS_NOCHANGE	=0,					//网络参数没有改变
		CHECK_STATUS_OK			=1,					//测试成功
		CHECK_STATUS_NOCHECK	=2,					//没有进行测试			
		CHECK_STATUS_FAIL		=3,					//测试失败
	}CHECK_STATUS;

protected:
	virtual void OnInitial();
	void OnPaint(PAINT_STATUS status);
	void CheckEnableUI( bool bRefresh, REFRASH_UI_NUM uiNum=REFRASH_UI_ALL );
	void OnModifyNetModeData(unsigned long cfgId);

	void DoUpdateUI( GET_IP_MODE& getIpMode, bool bRefresh,REFRASH_UI_NUM uiNum);

	unsigned long GetErrorTextNum();

	unsigned long CheckDhcpParam();
	

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	GUI::CListCtrl		m_listCtrl;

	GET_IP_MODE				m_GetIpMode;
	bool					m_bCheckParam;
	CHECK_STATUS			m_StatusIPCheckOK;

	NET_CTRL_FAIL			m_netCheckResult;

	bool					m_bAuto; //是否动态IP

	int					m_poeNum;

	enum LIST_ROW
	{
		ROW_GETIP_DHCP = 0,
		ROW_STATIC_IP = 1,
		ROW_SUB_NET_MASK = 2,
		ROW_GATEWAY = 3,
		ROW_DNS_1 = 4,
		ROW_DNS_2 = 5,

		ROW_PPPOE_ENABLE = 6,
		ROW_PPPOE_USER_NAME = 7,
		ROW_PPPOE_PASSWORD = 8,
		ROW_NET_TEST = 9,

		ROW_POESWITCHIP = 10,
		ROW_POESWITCH_SUBNETMASK = 11,

		ROW_NUM = 12,
	};
private:
	SETUPDATAINFO m_dataInfo;

};
#endif
