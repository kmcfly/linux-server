#include "CfgHxhtNetParamCfgDlg.h"

CCfgHxhtNetParamCfgDlg::CCfgHxhtNetParamCfgDlg()
{

}

CCfgHxhtNetParamCfgDlg::~CCfgHxhtNetParamCfgDlg()
{

}

void CCfgHxhtNetParamCfgDlg::OnInitial()
{	
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_HXHT_NETWORK_PARAM), false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_APPLY) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DEFAULT) );
	//////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	//unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	//////////////////////////////////////////////////////////////////////////

	GUI::CListCtrl& list = m_cfgHxhtNetParamList;
	GUI::CWnd * pWnd = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;
	GUI::CIPAddressCtrl* pIP = NULL;

// 	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
// 	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
// 	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = cy/m_length[LIST_ITEM_HEIGHT]; //List框不带头
	if( ROW_NUM > maxListRow )
	{
		offsetEdge = m_length[SLIDER_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 0, false);
	list.AddColumn(NULL, cx * 16 / 32);
	list.AddColumn(NULL, cx * 16 / 32 - offsetEdge);

	////
	char str[128] = {0};

	//视频服务器设备ID
	list.AddItem( ROW_VIDEO_SERVER_DEVICE_ID, 0, m_pStrTable->String(E_STR_ID_VS_DEVICE_ID) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_VIDEO_SERVER_DEVICE_ID, 1, WND_TYPE_EDIT ) );
	assert( NULL != pEdit );
	

	//视频服务器数据端口
	list.AddItem( ROW_VIDEO_SERVER_DATA_PORT, 0, m_pStrTable->String(E_STR_ID_VS_DATA_PORT) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_VIDEO_SERVER_DATA_PORT, 1, WND_TYPE_NUM_EDIT) );
	assert( NULL != pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,(int)65535);


	//接入服务器IP
	list.AddItem( ROW_CONNECT_SERVER_IP, 0, m_pStrTable->String(E_STR_ID_CS_IP) );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_CONNECT_SERVER_IP, 1, WND_TYPE_IP) );
	assert( pIP );

	//接入服务器消息端口
	list.AddItem( ROW_CONNECT_SERVER_MESSAGE_PORT, 0, m_pStrTable->String(E_STR_ID_CS_MESSAGE_PORT) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_CONNECT_SERVER_MESSAGE_PORT, 1, WND_TYPE_NUM_EDIT) );
	assert( NULL != pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,(int)65535);

	//接入服务器存储端口
	list.AddItem( ROW_CONNECT_SERVER_STORT_PORT, 0, m_pStrTable->String(E_STR_ID_CS_STORE_PORT) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_CONNECT_SERVER_STORT_PORT, 1, WND_TYPE_NUM_EDIT) );
	assert( NULL != pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,(int)65535);


	list.CreateListFocusMap();

}

// unsigned long CCfgHxhtNetParamCfgDlg::OnOK()
// {
// 	return 0;
// }
// 
 unsigned long CCfgHxhtNetParamCfgDlg::OnApply()
 {
	 unsigned long ret = OnLoadDefault(this);

	 if( IsModify() )
	 {
		 EnableOkBtn(true,false);
	 }
	 else
	 {
		 EnableOkBtn(false,false);
	 }

	 ENUM_STRING_ID strID = ret ? static_cast<ENUM_STRING_ID>(ret) : E_STR_ID_TIP_00_03;
	 MessageOut(strID,TIP_MSG,false);

	 //一次重绘全部
	 Repaint();

 	return KEY_VIRTUAL_MSG_STOP;
 }


void CCfgHxhtNetParamCfgDlg::GetItemList(CFGITEM_LIST& itemList)const
{
	itemList.push_back(NCFG_ITEM_HXHT_NET_PARAM);
}

bool CCfgHxhtNetParamCfgDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	if (NCFG_ITEM_HXHT_NET_PARAM == cfgID)
	{
		if (sizeof(HXHT_NET_PARAM_INFO) == len)
		{
			const HXHT_NET_PARAM_INFO *pHxhtNetParamInfo = (const HXHT_NET_PARAM_INFO *)(pData);

			//视频服务器设备ID
			GUI::CEditCtrl *pEditCtrl = dynamic_cast<GUI::CEditCtrl*>( m_cfgHxhtNetParamList.GetItemWnd(ROW_VIDEO_SERVER_DEVICE_ID, 1) );
			assert(NULL != pEditCtrl);
			pEditCtrl->SetMaxStringLen(132);
			pEditCtrl->SetCaption(pHxhtNetParamInfo->videoDeviceID, bRefresh);

			//视频服务器端口
			GUI::CNumEditCtrl *pNumEditCtrl = dynamic_cast<GUI::CNumEditCtrl*>(m_cfgHxhtNetParamList.GetItemWnd(ROW_VIDEO_SERVER_DATA_PORT, 1));
			assert(NULL != pNumEditCtrl);
			pNumEditCtrl->SetValue(pHxhtNetParamInfo->usVDevDataPort, bRefresh);

			//接入服务器IP
			GUI::CIPAddressCtrl *pIP = dynamic_cast<GUI::CIPAddressCtrl*>(m_cfgHxhtNetParamList.GetItemWnd(ROW_CONNECT_SERVER_IP,1));
			assert(NULL != pIP);
			pIP->SetIP(pHxhtNetParamInfo->csIP, bRefresh);

			//接入服务器消息端口
			pNumEditCtrl = dynamic_cast<GUI::CNumEditCtrl*>( m_cfgHxhtNetParamList.GetItemWnd(ROW_CONNECT_SERVER_MESSAGE_PORT, 1) );
			assert(NULL != pEditCtrl);
			pNumEditCtrl->SetValue(pHxhtNetParamInfo->csMessagePort, bRefresh);

			//接入服务器存储端口
			pNumEditCtrl = dynamic_cast<GUI::CNumEditCtrl*>( m_cfgHxhtNetParamList.GetItemWnd(ROW_CONNECT_SERVER_STORT_PORT, 1) );
			assert(NULL != pEditCtrl);
			pNumEditCtrl->SetValue(pHxhtNetParamInfo->csStortPort, bRefresh);

		}
		else
		{
			assert(false);
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CCfgHxhtNetParamCfgDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);

	if (NCFG_ITEM_HXHT_NET_PARAM == cfgID)
	{
		if (sizeof(HXHT_NET_PARAM_INFO) == len)
		{
			HXHT_NET_PARAM_INFO *pHxhtNetParamInfo = (HXHT_NET_PARAM_INFO *)(pData);

			//视频服务器设备ID
			const GUI::CEditCtrl *pEditCtrl = dynamic_cast<const GUI::CEditCtrl*>( m_cfgHxhtNetParamList.GetItemWnd(ROW_VIDEO_SERVER_DEVICE_ID, 1) );
			assert(NULL != pEditCtrl);
			strcpy(pHxhtNetParamInfo->videoDeviceID, pEditCtrl->GetCaption().c_str());
	
			//视频服务器端口
			const GUI::CNumEditCtrl *pNumEditCtrl = dynamic_cast<const GUI::CNumEditCtrl*>(m_cfgHxhtNetParamList.GetItemWnd(ROW_VIDEO_SERVER_DATA_PORT, 1));
			assert(NULL != pNumEditCtrl);
			pHxhtNetParamInfo->usVDevDataPort = pNumEditCtrl->GetValue();

			//接入服务器IP
			const GUI::CIPAddressCtrl *pIP = dynamic_cast<const GUI::CIPAddressCtrl*>(m_cfgHxhtNetParamList.GetItemWnd(ROW_CONNECT_SERVER_IP,1));
			assert(NULL != pIP);
			pHxhtNetParamInfo->csIP = pIP->GetIP();

			//接入服务器消息端口
			pNumEditCtrl = dynamic_cast<const GUI::CNumEditCtrl*>( m_cfgHxhtNetParamList.GetItemWnd(ROW_CONNECT_SERVER_MESSAGE_PORT, 1) );
			assert(NULL != pEditCtrl);
			pHxhtNetParamInfo->csMessagePort = pNumEditCtrl->GetValue();

			//接入服务器存储端口
			pNumEditCtrl = dynamic_cast<const GUI::CNumEditCtrl*>( m_cfgHxhtNetParamList.GetItemWnd(ROW_CONNECT_SERVER_STORT_PORT, 1) );
			assert(NULL != pEditCtrl);
			pHxhtNetParamInfo->csStortPort = pNumEditCtrl->GetValue();

		}
		else
		{
			assert(false);
			return false;
		}
	}
	else
	{
		return false;
	}
}


BEGIN_MSG_MAP(CCfgHxhtNetParamCfgDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgHxhtNetParamList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgHxhtNetParamCfgDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	// 是否使用自动分配IP
	if (  ( (ROW_VIDEO_SERVER_DEVICE_ID == row) || (ROW_VIDEO_SERVER_DATA_PORT == row) || 
		(ROW_CONNECT_SERVER_IP == row) || (ROW_CONNECT_SERVER_MESSAGE_PORT == row) || 
		(ROW_CONNECT_SERVER_STORT_PORT == row) ) && ( 1 == col ) )
	{
		if (!IsModify())
		{
			if (IsChange(NCFG_ITEM_HXHT_NET_PARAM))
			{
				SetModify(true);
				EnableOkBtn(true, true);
			}
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}


