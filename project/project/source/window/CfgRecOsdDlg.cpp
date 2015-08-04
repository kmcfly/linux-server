/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author        : L
** Date			  : 2009-08-12
** Name         : CfgLiveOsdDlg.cpp
** Version       : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgRecOsdDlg.h"
#include "TimeCtrl.h"

#include "FuncCustom.h"

CCfgRecOsdDlg::CCfgRecOsdDlg()
{
	m_pOsdCamera = NULL;
	m_pOsdTime = NULL;
}

CCfgRecOsdDlg::~CCfgRecOsdDlg()
{
	if( m_pOsdCamera )
	{
		delete [] m_pOsdCamera;
		m_pOsdCamera = NULL;
	}

	if( m_pOsdTime )
	{
		delete [] m_pOsdTime;
		m_pOsdTime = NULL;
	}
}

void CCfgRecOsdDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned long width = m_rect.Width() - m_length[DLG_OFFSET_SUB_LEFT] * 2;
	unsigned long height = m_rect.Height() - 2 * m_length[LIST_ITEM_HEIGHT] - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	GUI::CListCtrl& list = m_listCtrl;
	char str[256] = {0};

	/////预算List边缘是否需要滚动框
	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	int row = 0;
	for(int i = 0; i < maxChnn; i++)
	{
		if((CH >> i) & 0x1)
		{
			m_rowChnnMap.insert(std::make_pair(row++, i));
		}
	}

	if(effectChnn > maxListRow)
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	list.Create(GetRandWndID(),x,y,width,height,this,0,0,true);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_CAMERA_NAME), width * 9 / 32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_TIME_STAMP), width * 9 / 32);
	list.AddColumn(m_pStrTable->String(E_STR_ID_POSITION), width * 11 / 32 - offsetEdge);

	for ( int i = 0; i< effectChnn; i++ )
	{
		int chnnIndex = m_rowChnnMap.find(i)->second;
		if(chnnIndex < localVideoInputNum)
		{
			sprintf(str, "%d", chnnIndex + 1);
		}
		else
		{
			sprintf(str, "%d", chnnIndex + 1 /*- localVideoInputNum*/);
		}
		list.AddItem(i,COL_CHNN,str);

		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(i,COL_SHOW_NAME,WND_TYPE_CHECK));
		assert(pCheck);

		pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(i,COL_TIME_STAMP,WND_TYPE_CHECK));
		assert(pCheck);

		GUI::CButton* pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(i,COL_POS,WND_TYPE_BUTTON,m_length[BTN_WIDTH],m_length[BTN_HEIGHT]));
		assert(pBtn);
		pBtn->SetCaption(m_pStrTable->String(E_STR_ID_SETTING),false);
	}
	list.CreateListFocusMap();

	GUI::CRect rect = list.GetWindowRect();
	y += rect.Height() + 4;

	height = m_length[LIST_ITEM_HEIGHT];
	GUI::CListCtrl& listAll = m_listAll;

	snprintf( str, 256, "%s", m_pStrTable->String( E_STR_ID_ALL ) );
	m_scAll.Create( GUI::CWnd::GetRandWndID(), x, y + (m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT])/2, width, m_length[STATIC_HEIGHT], this );
	m_scAll.SetCaption( str, false );

	y += height;
	height = m_length[LIST_ITEM_HEIGHT];
	listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, false );
	listAll.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	listAll.AddColumn(m_pStrTable->String(E_STR_ID_CAMERA_NAME), width * 9 / 32);
	listAll.AddColumn(m_pStrTable->String(E_STR_ID_TIME_STAMP), width * 9 / 32);
	listAll.AddColumn(m_pStrTable->String(E_STR_ID_POSITION), width * 11 / 32);

	int i = 0;
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem(i,COL_CHNN,WND_TYPE_CHECK));
	assert(pCheckAll);

	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem(i,COL_SHOW_NAME,WND_TYPE_CHECK));
	assert(pCheck);
	pCheck->SetCheck(true, false);

	pCheck = dynamic_cast<GUI::CCheckCtrl*>(listAll.AddItem(i,COL_TIME_STAMP,WND_TYPE_CHECK));
	assert(pCheck);
	pCheck->SetCheck(true, false);

	GUI::CButton* pBtn = dynamic_cast<GUI::CButton*>(listAll.AddItem(i,COL_POS,WND_TYPE_BUTTON,m_length[BTN_WIDTH],m_length[BTN_HEIGHT]));
	assert(pBtn);
	pBtn->SetCaption(m_pStrTable->String(E_STR_ID_SETTING),false);

	listAll.CreateListFocusMap();

	///分配空间
	if( m_pOsdCamera )
	{
		delete [] m_pOsdCamera;
	}
	m_pOsdCamera = new OSD_INFO[maxChnn];

	if( m_pOsdTime )
	{
		delete [] m_pOsdTime;
	}
	m_pOsdTime = new OSD_INFO[maxChnn];
}

void CCfgRecOsdDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	if( m_pOsdCamera )
	{
		delete [] m_pOsdCamera;
		m_pOsdCamera = NULL;
	}

	if( m_pOsdTime )
	{
		delete [] m_pOsdTime;
		m_pOsdTime = NULL;
	}

	m_rowChnnMap.clear();
}

void CCfgRecOsdDlg::SetTip()
{
	ULONGLONG CH = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn);

	//int numChnn = s_loginSuccessReply.productInfo.localVideoInputNum;
	for ( int i = 0; i < effectChnn; i++ )
	{
		GUI::CWnd* pWnd = m_listCtrl.GetItemWnd(i, COL_POS);
		assert(pWnd);
		AddTip(pWnd->GetID(), E_STR_ID_TIP_02_08);
	}

	GUI::CWnd* pWndAll = m_listAll.GetItemWnd(0, COL_POS);
	assert(pWndAll);
	AddTip(pWndAll->GetID(), E_STR_ID_TIP_02_08);
}

void CCfgRecOsdDlg::UpdateView(bool bRefresh)
{
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHNN));
	if( pCheckAll )
	{
		pCheckAll->SetCheck( false, bRefresh );
	}

	CCfgDlg::UpdateView( bRefresh );
}

bool CCfgRecOsdDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	
	GUI::CListCtrl& list = m_listCtrl;
 
	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	unsigned long numChnn = maxChnn; //s_loginSuccessReply.productInfo.localVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_TIME_STAMP_INFO:
		{
			assert(len == sizeof(OSD_INFO) * numChnn);
			const OSD_INFO* pInfo = reinterpret_cast<const OSD_INFO*>(pData);
			memcpy(m_pOsdTime, pInfo, sizeof(OSD_INFO) * numChnn);
			
			m_allOsdTime.x = pInfo[0].x;
			m_allOsdTime.y = pInfo[0].y;
		
			for(int i = 0; i < effectChnn; i++)
			{
				GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(i,COL_TIME_STAMP));
				assert(pCheck);
				if(0 == pInfo[m_rowChnnMap.find(i)->second].bEnable)
				{
					pCheck->SetCheck(false, bRefresh);
				}
				else
				{
					pCheck->SetCheck(true, bRefresh);
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_CAM_NAME_STAMP_INFO:
		{
			assert(len == sizeof(OSD_INFO) * numChnn);
			const OSD_INFO* pInfo = reinterpret_cast<const OSD_INFO*>(pData);
			memcpy(m_pOsdCamera, pInfo, sizeof(OSD_INFO) * numChnn);

			m_allOsdCamera.x = pInfo[0].x;
			m_allOsdCamera.y = pInfo[0].y;

			for(int i = 0; i < effectChnn; i++)
			{
				GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(i, COL_SHOW_NAME));
				assert(pCheck);
				if( 0 == pInfo[m_rowChnnMap.find(i)->second].bEnable )
				{
					pCheck->SetCheck(false,bRefresh);
				}
				else
				{
					pCheck->SetCheck(true,bRefresh);
				}
			}
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgRecOsdDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	const GUI::CListCtrl& list = m_listCtrl;
	const GUI::CListCtrl& listAll = m_listAll;

	bool bAll = false;
	{
		const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd( 0, COL_CHNN ) );
		assert( pCheck );
		bAll = pCheck->IsCheck();
	}

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	unsigned long numChnn = maxChnn;  //s_loginSuccessReply.productInfo.localVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_TIME_STAMP_INFO:
		{
			assert(len == sizeof(OSD_INFO) * numChnn);
			OSD_INFO* pInfo = reinterpret_cast<OSD_INFO*>(pData);

			for(int i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				GUI::CCheckCtrl* pCheck = NULL;
				if( bAll )
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CWnd*)listAll.GetItemWnd(0,COL_TIME_STAMP));
					pInfo[index].x = m_allOsdTime.x;
					pInfo[index].y = m_allOsdTime.y;
				}
				else
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CWnd*)list.GetItemWnd(i,COL_TIME_STAMP));
					pInfo[index].x = m_pOsdTime[index].x;
					pInfo[index].y = m_pOsdTime[index].y;
				}
				assert(pCheck);

				pInfo[index].bEnable = pCheck->IsCheck() ? 1 : 0;
			}
		}
		return true;
		break;
	case NCFG_ITEM_CAM_NAME_STAMP_INFO:
		{
			assert(len == sizeof(OSD_INFO) * numChnn);
			OSD_INFO* pInfo = reinterpret_cast<OSD_INFO*>(pData);

			for(int i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				GUI::CCheckCtrl* pCheck = NULL;
				if( bAll )
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CWnd*)listAll.GetItemWnd(0,COL_SHOW_NAME));
					pInfo[index].x = m_allOsdCamera.x;
					pInfo[index].y = m_allOsdCamera.y;
				}
				else
				{
					pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CWnd*)list.GetItemWnd(i,COL_SHOW_NAME));
					pInfo[index].x = m_pOsdCamera[index].x;
					pInfo[index].y = m_pOsdCamera[index].y;
				}
				assert(pCheck);

				pInfo[index].bEnable = pCheck->IsCheck() ? 1 : 0;
			}
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}
void CCfgRecOsdDlg::PopUpSetPositionDlg(unsigned short chnn, 
								bool bCam, unsigned short inCamX, unsigned short inCamY, 
								unsigned short& outCamX, unsigned short& outCamY,
								bool bTm, unsigned short inTmX, unsigned short inTmY,
								unsigned short& outTmX, unsigned short& outTmY)
{
	int localVideoInputNum = CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;

	//
	NET_DISPLAY_INFO dispInfo;
	memset(&dispInfo,0,sizeof(NET_DISPLAY_INFO));
	dispInfo.type = NET_DISPLAY_CHANGE_DATA_PACK;
	dispInfo.split_mode[DISPLAY_LAYER_ANALOG] = VIEW_SPLIT_1X1;
	dispInfo.split_mode[DISPLAY_LAYER_NET] = VIEW_SPLIT_1X1;
	dispInfo.rec = 0;
	dispInfo.time_out = -1;
	if( static_cast<unsigned short>(-1) == chnn )
	{
		if(chnn < localVideoInputNum)
		{
			dispInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, 0, 0);
		}
		else
		{
			dispInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, 0, 0);
		}
		dispInfo.data_len = 1;
	}
	else
	{
		if(chnn < localVideoInputNum)
		{
			dispInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, 0, chnn);
		}
		else
		{
			dispInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, 0, chnn - localVideoInputNum);
		}
		dispInfo.data_len = 1;
	}

	/////////////启动单屏
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&dispInfo, sizeof(NET_DISPLAY_INFO), msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	///////////////名称从配置名称中读取,时间为当前时间
	std::string strCamera;
	std::string strTime;

	if( bCam )
	{
		if( static_cast<unsigned short>(-1) == chnn )
		{
			char str[32];
			strcpy(str,"All Camera");
			strCamera = str;
		}
		else
		{
			BYTE* pData = NULL;
			unsigned long dataLen = 0;
			CCfgDlg::GetConfig( NCFG_ITEM_CAMERA_NAME, &pData, dataLen );
			int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
			assert(chnn <= numChnn);
			assert( dataLen == sizeof(NCFG_INFO_SHORT_NAME)*numChnn );
			NCFG_INFO_SHORT_NAME* pShortName = reinterpret_cast<NCFG_INFO_SHORT_NAME*>(pData);

			strCamera = pShortName[chnn].name;

			if(strCamera.empty())
			{
				char str[32] = {0};
				sprintf(str,"Camera %d",chnn+1);
				strCamera = str;
			}
		}
	}

	if( bTm )
	{
		CTimeCtrl::GetTime32Text(strTime,GetCurrTime32());
	}

	//修正位置  从图像的相对位置放缩到当前屏幕
	int width_text = 200;
	int start_cam_x = m_width * inCamX / 100;
	int start_cam_y = m_height * inCamY / 100;
	int start_tm_x = m_width * inTmX / 100;
	int start_tm_y = m_height * inTmY / 100;

	unsigned short str_cx = static_cast<unsigned short>(width_text), str_cy;
	
	CWnd::TextSize(strCamera.c_str(), strCamera.length(), str_cx, str_cy);
	width_text = static_cast<int>(str_cx) + 8;
	width_text = (width_text <= 20) ? 20 : width_text;

#ifdef __SUPPORT_OSDTWOLINES__
	//2011-06-15  ZYX
	int height = m_length[STATIC_HEIGHT];
	int findEndl = strCamera.find('/');
	if (-1 != findEndl)
	{
		height += m_length[STATIC_HEIGHT];
		CWnd::TextSize(strCamera.c_str(), findEndl, str_cx, str_cy);
		unsigned short new_str_cx = static_cast<unsigned short>(width_text);
		CWnd::TextSize(strCamera.c_str()+findEndl+1, strCamera.length()-findEndl-1, new_str_cx, str_cy);
		if (str_cx < new_str_cx)
		{
			str_cx = new_str_cx;
		}
		width_text = static_cast<int>(str_cx) + 8;
		width_text = (width_text <= 20) ? 20 : width_text;
	}
	//
	m_setPositionDlg.SeEnableCamera(start_cam_x, start_cam_y, width_text, height, strCamera.c_str(), bCam);
#else
	m_setPositionDlg.SeEnableCamera(start_cam_x, start_cam_y, width_text, m_length[STATIC_HEIGHT], strCamera.c_str(), bCam);
#endif

	CWnd::TextSize(strTime.c_str(), strTime.length(), str_cx, str_cy);
	width_text = static_cast<int>(str_cx) + 8;
	width_text = (width_text <= 20) ? 20 : width_text;
	m_setPositionDlg.SetEnableTime(start_tm_x, start_tm_y, width_text, m_length[STATIC_HEIGHT], strTime.c_str(), bTm);
	
	m_setPositionDlg.DoModal(this,CENTER_POS,CENTER_POS,MAX_SCREEN_X,MAX_SCREEN_Y,false);
	
	//修正输出值
	if(bCam && m_setPositionDlg.IsModify())
	{
		int x,y;
		m_setPositionDlg.GetPosCamera(x,y);
		outCamX = x * 100 / m_width;
		outCamY = y * 100 / m_height;

		if(outCamX<=0) outCamX=0;
		if(outCamX>=100) outCamX=100;
		if(outCamY<=0) outCamY=0;
		if(outCamY>=100) outCamY=100;
	}

	if(bTm && m_setPositionDlg.IsModify())
	{
		int x,y;
		m_setPositionDlg.GetPosTime(x,y);
		outTmX = x * 100 / m_width;
		outTmY = y * 100 / m_height;

		if(outTmX<=0) outTmX=0;
		if(outTmX>=100) outTmX=100;
		if(outTmY<=0) outTmY=0;
		if(outTmY>=100) outTmY=100;
	}

	/////////////恢复屏幕
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0, msgData);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
}

BEGIN_MSG_MAP(CCfgRecOsdDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(),KEY_VIRTUAL_ENTER,OnClickList)
	ON_MSG_EX(m_listAll.GetID(),KEY_VIRTUAL_ENTER,OnClickListAll)
END_MSG_MAP()

unsigned long CCfgRecOsdDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	//2010-03-03 17:57:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHNN));
	assert( pCheckAll );
	if( pCheckAll->IsCheck() )
	{
		pCheckAll->SetCheck(false, true);
	}

	if( col == COL_POS && !pCheckAll->IsCheck())
	{
		GUI::CCheckCtrl* pCkCam = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.GetItemWnd(row,COL_SHOW_NAME));
		GUI::CCheckCtrl* pCkTm = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.GetItemWnd(row,COL_TIME_STAMP));
		assert(pCkCam);
		assert(pCkTm);

		bool bCam = pCkCam->IsCheck();
		bool bTm = pCkTm->IsCheck();

		if( bCam || bTm )
		{
			int chnn = m_rowChnnMap.find(row)->second;

			unsigned short out_cam_x, out_cam_y;
			unsigned short out_tm_x, out_tm_y;
			PopUpSetPositionDlg(chnn, bCam, m_pOsdCamera[chnn].x,m_pOsdCamera[chnn].y,out_cam_x,out_cam_y,
											bTm,m_pOsdTime[chnn].x,m_pOsdTime[chnn].y,out_tm_x,out_tm_y);
			if(bCam && m_setPositionDlg.IsModify())
			{
				m_pOsdCamera[chnn].x = out_cam_x;
				m_pOsdCamera[chnn].y = out_cam_y;
			}

			if(bTm && m_setPositionDlg.IsModify())
			{
				m_pOsdTime[chnn].x = out_tm_x;
				m_pOsdTime[chnn].y = out_tm_y;
			}
		}
	}

	if (!IsModify())
	{
		switch (col)
		{
		case COL_SHOW_NAME:
			{
				if(IsChange(NCFG_ITEM_CAM_NAME_STAMP_INFO))
				{
					SetModify(true);
				}
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case COL_TIME_STAMP:
			{
				if(IsChange(NCFG_ITEM_TIME_STAMP_INFO))
				{
					SetModify(true);
				}
			}
		case COL_POS:
			{
				if(IsChange(NCFG_ITEM_TIME_STAMP_INFO) || IsChange(NCFG_ITEM_CAM_NAME_STAMP_INFO))
				{
					SetModify(true);
				}
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgRecOsdDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	// 点选非All，先检查是否处于All状态，处于则认为做了修改
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHNN));
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCheckAll );
	assert( pCfgDlg );

	if( col == COL_POS && pCheckAll->IsCheck())
	{
		GUI::CCheckCtrl* pCkCam = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0,COL_SHOW_NAME));
		GUI::CCheckCtrl* pCkTm = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0,COL_TIME_STAMP));
		assert(pCkCam);
		assert(pCkTm);

		bool bCam = pCkCam->IsCheck();
		bool bTm = pCkTm->IsCheck();

		if( bCam || bTm )
		{
			unsigned short out_cam_x, out_cam_y;
			unsigned short out_tm_x, out_tm_y;
			PopUpSetPositionDlg(-1, bCam, m_allOsdCamera.x,m_allOsdCamera.y,out_cam_x,out_cam_y,
											bTm,m_allOsdTime.x,m_allOsdTime.y,out_tm_x,out_tm_y);
			if(bCam && m_setPositionDlg.IsModify())
			{
				m_allOsdCamera.x = out_cam_x;
				m_allOsdCamera.y = out_cam_y;
			}

			if(bTm && m_setPositionDlg.IsModify() )
			{
				m_allOsdTime.x = out_tm_x;
				m_allOsdTime.y = out_tm_y;
			}
		}
	}

	if( pCheckAll->IsCheck() )
	{
		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			if( COL_SHOW_NAME == col ||
				COL_TIME_STAMP == col )
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
	}
	else
	{
		pCfgDlg->SetRefreshView( false );
	}

	// 点选All 直接认为做了修改
	if (!IsModify())
	{
		if( COL_CHNN == col )
		{
			SetModify(true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}
