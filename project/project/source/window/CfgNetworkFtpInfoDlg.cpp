/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-01-20
** Name         : CfgTriggerBasicDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgNetworkFtpInfoDlg.h"
#include "CfgTriggerDlg.h"
#include "FuncCustom.h"

CCfgNetworkFtpInfoDlg::CCfgNetworkFtpInfoDlg()
{
	memset(&m_ftpCfg, 0, sizeof(TVT_FTP_CONFIG));
	m_channel = 0;
}

CCfgNetworkFtpInfoDlg::~CCfgNetworkFtpInfoDlg()
{

}

void CCfgNetworkFtpInfoDlg::OnInitial()
{
	m_vecChnn.clear();
	m_vecChnnAll.clear();

	GUI::CScrollDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CScrollWnd& wnd = GetScrollWnd();

	unsigned short border = 2;
	unsigned long width = m_rect.Width() - (border + m_length[DLG_OFFSET_SUB_LEFT]) * 2;
	unsigned long height= m_length[LIST_ITEM_HEIGHT];

	//设置滚动窗口的边框和可视区域
	SetDlgBorder(border);
	SetScrollWndPos(border + m_length[DLG_OFFSET_SUB_LEFT], border + m_length[DLG_OFFSET_SUB_TOP], border + m_length[DLG_OFFSET_SUB_LEFT], border + m_length[DLG_OFFSET_SUB_BOTTOM]);

	//设置滚动窗口单元格大小和最大视图区域
	wnd.SetSizeOffset(width / 2, height);
	wnd.SetViewMaxSize(width, height * 4);

	//调整控件的偏移值
	unsigned char offset_y = m_length[LIST_ITEM_YPOS];
	unsigned char offset_x = m_length[LIST_ITEM_XPOS];

	unsigned long xx = 0, yy = 0, cx = width / 2 - 2 * offset_x, cy = height;
	int row = 0;
	GUI::CRect rect;
	int xFocus = 0, yFocus = 0;

	//enable
	row = ROW_FTP_ENABLE;
	wnd.GetViewPos( 0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	GUI::CWnd* pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_ENABLE), false);

	
	rect.SetRect(0, 0, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT]);
	wnd.GetViewPos(1, row, xx, yy);
	rect.Offset(xx + offset_x, yy + offset_y);
	wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, 0, yFocus++, SW_ITEM_ID_FTP_ENABLE);

	//服务器IP地址
	row = ROW_FTP_SERVER_ADDR;
	int numChnnLocal = CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
	int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum;
	char strtmp[64] = {0};
	wnd.GetViewPos( 0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_FTP_SERVICE), false);

	rect.SetRect(0, 0, 240, m_length[COMBO_HEIGHT]);
	wnd.GetViewPos(1, row, xx, yy);
	rect.Offset(xx + offset_x, yy + 1);
	GUI::CEditCtrl * pEditIP = dynamic_cast<GUI::CEditCtrl*>(wnd.AddWnd(WND_TYPE_EDIT, GetRandWndID(), rect, 0, yFocus++, SW_ITEM_ID_FTP_SERVER_ADDR));
	pEditIP->SetMaxStringLen(67);

	//服务端口
	row = ROW_FTP_SERVER_PORT;
	wnd.GetViewPos( 0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_PORT), false);

	rect.SetRect(0, 0, 120, m_length[COMBO_HEIGHT]);
	wnd.GetViewPos(1, row, xx, yy);
	rect.Offset(xx + offset_x, yy + 1);
	GUI::CNumEditCtrl * pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(wnd.AddWnd(WND_TYPE_NUM_EDIT, GetRandWndID(), rect, 0, yFocus++, SW_ITEM_ID_FTP_SERVER_PORT));

	//是否使用SSL
// 	row = ROW_FTP_SSL_CHECK;
// 	wnd.GetViewPos( 0, row, xx, yy);
// 	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
// 	rect.Offset(xx + offset_x, yy + offset_y);
// 	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
// 	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_SLL_CHECK), false);
// 
// 	rect.SetRect(0, 0, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT]);
// 	wnd.GetViewPos(1, row, xx, yy);
// 	rect.Offset(xx + offset_x, yy + offset_y);
// 	wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, 0, yFocus++, SW_ITEM_ID_FTP_SSL_CHEKC);


	//匿名登录
	row = ROW_FTP_ANONYMITY_LOGIN;
	wnd.GetViewPos( 0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_FTP_ANONYMITY), false);

	rect.SetRect(0, 0, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT]);
	wnd.GetViewPos(1, row, xx, yy);
	rect.Offset(xx + offset_x, yy + offset_y);
	wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, 0, yFocus++, SW_ITEM_ID_FTP_ANONYMITY);

	//用户名
	row = ROW_FTP_USER_NAME;
	wnd.GetViewPos( 0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_USER_NAME), false);

	rect.SetRect(0, 0, 120, m_length[COMBO_HEIGHT]);
	wnd.GetViewPos(1, row, xx, yy);
	rect.Offset(xx + offset_x, yy + 1);
	GUI::CEditCtrl * pEditUser = dynamic_cast<GUI::CEditCtrl*>(wnd.AddWnd(WND_TYPE_EDIT, GetRandWndID(), rect, 0, yFocus++, SW_ITEM_ID_FTP_USER_NAME));
	pEditUser->SetMaxStringLen(67);

	//密码
	row = ROW_FTP_USER_PASS;
	wnd.GetViewPos( 0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_PASSWORD), false);

	rect.SetRect(0, 0, 120, m_length[COMBO_HEIGHT]);
	wnd.GetViewPos(1, row, xx, yy);
	rect.Offset(xx + offset_x, yy + 1);
	GUI::CEditCtrl * pEditPassword = dynamic_cast<GUI::CEditCtrl*>(wnd.AddWnd(WND_TYPE_EDIT, GetRandWndID(), rect, 0, yFocus++, SW_ITEM_ID_FTP_PASSWORD));
	pEditPassword->SetMaxStringLen(35);
	pEditPassword->SetPassword(true);
	
	//文件大小
	row = ROW_FTP_FILE_LEN;
	wnd.GetViewPos( 0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_FTP_FILE_LEN), false);

	rect.SetRect(0, 0, 120, m_length[COMBO_HEIGHT]);
	wnd.GetViewPos(1, row, xx, yy);
	rect.Offset(xx + offset_x, yy + 1);
	GUI::CNumEditCtrl* pNumEditFileLen = dynamic_cast<GUI::CNumEditCtrl*>(wnd.AddWnd(WND_TYPE_NUM_EDIT, GetRandWndID(), rect, 0, yFocus++, SW_ITEM_ID_FTP_FILE_LEN));
	pNumEditFileLen->SetRange(32, 64);

	if((GUI::CWnd::m_width < 1024) && (GUI::CWnd::m_height < 768) && (numChnn >= 32))
	{
		//高度不够,不偏移
	}
	else
	{
		row++;
	}

	//选择上传FTP的通道
	row++;
	wnd.GetViewPos(0, row, xx, yy);
	rect.SetRect(0, 0, cx, m_length[STATIC_HEIGHT]);
	rect.Offset(xx + offset_x, yy + offset_y);
	pWnd = wnd.AddWnd(WND_TYPE_STATIC, GetRandWndID(), rect, 0, 0, -1);
	pWnd->SetCaption(m_pStrTable->String(E_STR_ID_FTP_UPDATE_VIDEO),false);

	int chnnWidth = cx / 5;
	int groupWidth = (chnnWidth > (m_length[CHECK_WIDTH] + 24)) ? (m_length[CHECK_WIDTH] + 24) : chnnWidth;

	int group = 0;
	xFocus = 0;
	yFocus--;
	for(int i = 0; i < numChnnLocal; i++)
	{
		if(0 == (i % 4))
		{
			if(0 == (group % 2))
			{
				row++;
				yFocus++;
				xFocus = 0;
			}

			wnd.GetViewPos(group % 2, row, xx, yy);
			rect.SetRect(0, 0, groupWidth, m_length[CHECK_HEIGHT]);
			rect.Offset(xx + offset_x, yy + offset_y);

			pWnd = wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, xFocus++, yFocus, SW_ITEM_ID_FTP_CH_GROUP + group);
			rect.Offset(groupWidth, 0);

			assert(pWnd);
			m_vecChnnAll.push_back(dynamic_cast<GUI::CCheckCtrl*>(pWnd));

			group++;
		}

		pWnd = wnd.AddWnd(WND_TYPE_CHECK, GetRandWndID(), rect, xFocus++, yFocus, SW_ITEM_ID_FTP_CH_0 + i);
		rect.Offset(chnnWidth, 0);

		assert(pWnd);
		m_vecChnn.push_back(dynamic_cast<GUI::CCheckCtrl*>(pWnd));

		snprintf(strtmp, sizeof(strtmp), "%d", i + 1);
		pWnd->SetCaption(strtmp, false);
	}
}

void CCfgNetworkFtpInfoDlg::OnDestroy()
{
	GUI::CScrollDialog::OnDestroy();

	m_vecChnn.clear();
	m_vecChnnAll.clear();
}

void CCfgNetworkFtpInfoDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CScrollDialog::OnPaint(status);
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_SELECT, 2);

	GUI::CScrollWnd& wnd = GetScrollWnd();
	unsigned long maxCol = 0, maxRow = 0;
	wnd.GetPageCount(maxCol, maxRow);

	unsigned long x_left = m_rect.m_left + 2 + m_length[DLG_OFFSET_SUB_LEFT], y_top = m_rect.m_top + 2 + m_length[DLG_OFFSET_SUB_TOP];
	unsigned long center_x = 0;
	unsigned long width = 0;
	unsigned long height = m_rect.Height() - m_length[DLG_OFFSET_SUB_TOP] - m_length[DLG_OFFSET_SUB_BOTTOM] - 4;

	unsigned long xx = 0, yy = 0, cx = 0, cy = 0;
	wnd.GetViewPos(1, 0, xx, yy);
	center_x = x_left + xx;

	wnd.GetViewSize(1, 0, cx, cy);
	width = xx + cx;				//如果设置的行数大于maxRow,还需预留滚动条的位置

	unsigned long x = x_left, y = y_top - 2;
	for (int i = 0; i < maxRow - 1; i++)
	{
		y += cy;
		CWnd::DrawLine(x, y, x + width, y, COLOR_LINE_GRAY);
	}

	CWnd::DrawLine(center_x, y_top, center_x, y_top + height, COLOR_LINE_GRAY);
}

void CCfgNetworkFtpInfoDlg::InitCtrlData(bool bRefresh)
{
	GUI::CScrollWnd& wnd = GetScrollWnd();

	//启用FTP

	GUI::CCheckCtrl * pCkEnable = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_ENABLE));
	assert(pCkEnable);
	if(m_ftpCfg.enableFTP)
	{
		pCkEnable->SetCheck(true, bRefresh);
	}
	else
	{
		pCkEnable->SetCheck(false, bRefresh);
	}

	//服务器地址
	{
		GUI::CEditCtrl * pEditServerAddr = dynamic_cast<GUI::CEditCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_SERVER_ADDR));
		assert(NULL != pEditServerAddr);
		pEditServerAddr->SetCaption(m_ftpCfg.serverURL);
	}

	//服务器端口
	{
		GUI::CNumEditCtrl * pEditServerPort = dynamic_cast<GUI::CNumEditCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_SERVER_PORT));
		assert(NULL != pEditServerPort);
		pEditServerPort->SetValue(m_ftpCfg.serverPort);
	}

	//是否使用SSL
// 	GUI::CCheckCtrl * pSSLEnable = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_SSL_CHEKC));
// 	assert(NULL != pSSLEnable);
// 	if (m_ftpCfg.encryption)
// 	{
// 		pSSLEnable->SetCheck(true, false);
// 	}
// 	else
// 	{
// 		pSSLEnable->SetCheck(false, false);
// 	}

	//匿名登录
	GUI::CCheckCtrl * pChAnonymity = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_ANONYMITY));
	assert(NULL != pChAnonymity);
	if (m_ftpCfg.anonymous)
	{
		pChAnonymity->SetCheck(true, false);
	}
	else
	{
		pChAnonymity->SetCheck(false, false);
	}

	//用户名
	{
		GUI::CEditCtrl * pEditUser = dynamic_cast<GUI::CEditCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_USER_NAME));
		assert(NULL != pEditUser);
		pEditUser->SetCaption(m_ftpCfg.account);
	}
	
	//密码
	{
		GUI::CEditCtrl * pPassword = dynamic_cast<GUI::CEditCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_PASSWORD));
		assert(NULL != pPassword);
		pPassword->SetCaption(m_ftpCfg.password);
	}

	//上传文件大小
	{
		GUI::CNumEditCtrl * pFileLen = dynamic_cast<GUI::CNumEditCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_FILE_LEN));
		assert(NULL != pFileLen);
		pFileLen->SetValue(m_ftpCfg.uploadVideoLen);
	}
	
	//FTP报警上传图片通道
	for(int i = 0; i < m_vecChnn.size(); i++)
	{
 		if(m_ftpCfg.uploadVideo[i].enableUploadCam)
 		{
 			m_vecChnn[i]->SetCheck(true, false);
 		}
 		else
 		{
 			m_vecChnn[i]->SetCheck(false, false);
 		}
	}

	//初始设置All是否被选中
	for(int i = 0; i < m_vecChnn.size(); i++)
	{
		if(0 == (i % 4))
		{
			OnClickChnn(i, false, false);
		}
	}

	if( bRefresh )
	{
		Repaint();
	}
}

void CCfgNetworkFtpInfoDlg::OnClickChnn(int index, bool setValue, bool bRefresh)
{
	if (index >= m_vecChnn.size()) 
	{
		return;
	}

	if (IsMoreUpdateChnn())
	{
		return ;
	}

	int group = index / 4;
	assert( group < m_vecChnnAll.size() );

	//设置Check All的状态
	bool check = true;
	for (int i=group * 4; i<group*4+4; i++ )
	{
		if(i < m_vecChnn.size())
		{
			if( !m_vecChnn[i]->IsCheck() )
			{
				check = false;
				break;
			}
		}
	}
	m_vecChnnAll[group]->SetCheck(check, bRefresh);

	//修改内部变量的值
	if(setValue)
	{
 		if( m_vecChnn[index]->IsCheck() )
 		{
			m_ftpCfg.uploadVideo[index].enableUploadCam = 1;
 		}
 		else
 		{
			m_ftpCfg.uploadVideo[index].enableUploadCam = 0;
 		}
	}
}

void CCfgNetworkFtpInfoDlg::OnClickChnnAll(int index, bool setValue, bool bRefresh)
{
	if( index >= m_vecChnnAll.size())
	{
		return;
	}

	bool check = m_vecChnnAll[index]->IsCheck();
	for (int i=index * 4; i<index * 4 + 4; i++)
	{
		if(i < m_vecChnn.size())
		{
			m_vecChnn[i]->SetCheck(check, bRefresh);
		}
	}

	//检测是否超出范围
	if (IsMoreUpdateChnn())
	{
		return ;
	}

	//修改内部变量的值
	if(setValue)
	{
		for (int i=0; i<m_vecChnn.size(); i++)
		{
			if(m_vecChnn[i]->IsCheck())
			{
				m_ftpCfg.uploadVideo[i].enableUploadCam = 1;
			}
			else
			{
				m_ftpCfg.uploadVideo[i].enableUploadCam = 0;
			}
		}
	}
}

BEGIN_MSG_MAP(CCfgNetworkFtpInfoDlg, GUI::CScrollDialog)
	ON_MSG_EX(GetScrollWnd().GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgNetworkFtpInfoDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	GUI::CScrollWnd& wnd = GetScrollWnd();

	int index = -1;
	unsigned long tmpIndex = (keyID >> 20) & 0x7FF;
	if( tmpIndex < 0x7FF )
	{
		index = tmpIndex;
	}

	unsigned long id = keyID & 0xFFFFF;

	if(index >= 0)
	{
		//启用FTP
		if(SW_ITEM_ID_FTP_ENABLE == index)
		{
			GUI::CCheckCtrl *pckeck = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_ENABLE));
			assert(pckeck);
 			if(pckeck->IsCheck())
 			{
 				m_ftpCfg.enableFTP = 1;
 			}
 			else
 			{
 				m_ftpCfg.enableFTP = 0;
 			}

			return KEY_VIRTUAL_MSG_CONTINUE;
		}

		//FTP服务器地址
		if(SW_ITEM_ID_FTP_SERVER_ADDR == index)
		{
			GUI::CEditCtrl * pEditServerAddr = dynamic_cast<GUI::CEditCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_SERVER_ADDR));
			string strServerAddr = pEditServerAddr->GetCaption();
			strcpy(m_ftpCfg.serverURL, strServerAddr.c_str());
			
			return KEY_VIRTUAL_MSG_CONTINUE;
		}

		if (SW_ITEM_ID_FTP_SERVER_PORT == index)
		{
			GUI::CNumEditCtrl * pEditServerPort = dynamic_cast<GUI::CNumEditCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_SERVER_PORT));
			assert(NULL != pEditServerPort);
			m_ftpCfg.serverPort = pEditServerPort->GetValue();

			return KEY_VIRTUAL_MSG_CONTINUE;
		}

// 		if (SW_ITEM_ID_FTP_SSL_CHEKC  == index)
// 		{
// 			GUI::CCheckCtrl * pSSLEnable = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_SSL_CHEKC));
// 			assert(NULL != pSSLEnable);
// 			if (pSSLEnable->IsCheck())
// 			{
// 				m_ftpCfg.encryption = 1;
// 			}
// 			else
// 			{
// 				m_ftpCfg.encryption = 0;
// 			}
// 
// 			return KEY_VIRTUAL_MSG_CONTINUE;
// 		}

		if (SW_ITEM_ID_FTP_ANONYMITY == index)
		{
			GUI::CCheckCtrl * pChAnonymity = dynamic_cast<GUI::CCheckCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_ANONYMITY));
			assert(NULL != pChAnonymity);
			if (pChAnonymity->IsCheck())
			{
				m_ftpCfg.anonymous = 1;
			}
			else
			{
				m_ftpCfg.anonymous = 0;
			}

			return KEY_VIRTUAL_MSG_CONTINUE;
		}

		if (SW_ITEM_ID_FTP_USER_NAME == index)
		{
			GUI::CEditCtrl * pEditUser = dynamic_cast<GUI::CEditCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_USER_NAME));
			assert(NULL != pEditUser);
			string strUser = pEditUser->GetCaption();
			strcpy(m_ftpCfg.account, strUser.c_str());

			return KEY_VIRTUAL_MSG_CONTINUE;
		}

		if (SW_ITEM_ID_FTP_PASSWORD == index)
		{
			GUI::CEditCtrl * pPassword = dynamic_cast<GUI::CEditCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_PASSWORD));
			assert(NULL != pPassword);
			string strPass = pPassword->GetCaption();
			strcpy(m_ftpCfg.password, strPass.c_str());

			return KEY_VIRTUAL_MSG_CONTINUE;
		}

		if (SW_ITEM_ID_FTP_FILE_LEN == index)
		{
			GUI::CNumEditCtrl * pFileLen = dynamic_cast<GUI::CNumEditCtrl*>(wnd.GetWnd(SW_ITEM_ID_FTP_FILE_LEN));
			assert(NULL != pFileLen);
			m_ftpCfg.uploadVideoLen = pFileLen->GetValue();

			return KEY_VIRTUAL_MSG_CONTINUE;
		}

		//FTP上传通道
		int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum;
		if((SW_ITEM_ID_FTP_CH_GROUP <= index) && ((SW_ITEM_ID_FTP_CH_GROUP + (numChnn + 3) / 4) > index))
		{
			OnClickChnnAll(index - SW_ITEM_ID_FTP_CH_GROUP, true, true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}

		if((SW_ITEM_ID_FTP_CH_0 <= index) && ((SW_ITEM_ID_FTP_CH_0 + numChnn) > index))
		{
			OnClickChnn(index - SW_ITEM_ID_FTP_CH_0, true, true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

bool CCfgNetworkFtpInfoDlg::IsMoreUpdateChnn()
{
	unsigned long updateChannel = 0;

	for (int i=0; i<m_vecChnn.size(); i++)
	{
		if (m_vecChnn[i]->IsCheck())
		{
			updateChannel++;
		}
	}

	if (updateChannel > m_maxFtpUpdateChannel)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_FTP_BEYOND_UPLOAD_CHANNEL), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		//FTP报警上传图片通道
		for(int i = 0; i < m_vecChnn.size(); i++)
		{
			if(m_ftpCfg.uploadVideo[i].enableUploadCam)
			{
				m_vecChnn[i]->SetCheck(true, false);
			}
			else
			{
				m_vecChnn[i]->SetCheck(false, false);
			}
		}

		//初始设置All是否被选中
		for(int i = 0; i < m_vecChnn.size(); i++)
		{
			if(0 == (i % 4))
			{
				OnClickChnn(i, false, false);
			}
		}

		Repaint();
	}

	return false;
}

//end
