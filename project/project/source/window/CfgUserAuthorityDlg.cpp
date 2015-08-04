/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-29
** Name         : CfgUserAuthorityDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgUserAuthorityDlg.h"
#include "NetProtocol.h"
#include "FuncCustom.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

CCfgUserAuthorityDlg::CCfgUserAuthorityDlg()
{
	m_maxChnn = 16;
	m_effectCH = INVALID_INDEX(ULONGLONG);
}

CCfgUserAuthorityDlg::~CCfgUserAuthorityDlg()
{

}

void CCfgUserAuthorityDlg::OnInitial()
{
	assert(0 == (m_maxChnn % 4));//检查设置最大通道数量，通道数目为4的倍数。

	GUI::CScrollDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	unsigned short border = 2;
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT]+border;
	unsigned short y = m_length[DLG_OFFSET_SUB_TOP]+border;
	unsigned short bottom = m_length[DLG_OFFSET_SUB_BOTTOM]+border+3;

	unsigned short authCH	= 6;//和通道相关的配置项

	int chRow = (m_maxChnn + 7) / 8;	//以8个通道为一行
	unsigned short rowMax = 3 + authCH + (authCH * chRow) + 2;

	if(!IsSupportPTZ())
	{
		rowMax -= (chRow + 1);
	}

	if(!IsSupportShutDown())
	{
		rowMax -= 1;
	}

	GUI::CScrollWnd& wnd = GetScrollWnd();

	unsigned long width = (m_rect.Width()- (2 * x) - m_length[SCROLL_WIDTH] - 2) / 10;
	unsigned long height = m_length[LIST_ITEM_HEIGHT];

	wnd.SetSizeOffset(width, height);
	wnd.SetViewMaxSize(width*10, height*rowMax);
	SetScrollWndPos(x, y, x, bottom);
	SetDlgBorder(border);

	//非通道绑定权限
	char str[64] = {0};
#ifdef __DVR_ULTIMATE__
	int authNoCH = 8;//和通道无关的配置项，一行放3个。
#else
	int authNoCH = 7;//和通道无关的配置项，一行放3个。
#endif
	for (int i = 0; i < authNoCH; i++)
	{
		int row_auth = i / 3;
		int col_auth = i % 3;

		if(!IsSupportShutDown() && (i > 2))
		{
			//隐藏Shut Down
			row_auth = (i - 1) / 3;
			col_auth = (i - 1) % 3;
		}

		//滚动窗口要求控件必须能被容纳在视图内
		GUI::CRect rect(0, 0, (width * 3) - 8, m_length[CHECK_HEIGHT]);
		unsigned long x, y, cx, cy;

		wnd.GetViewPos(3 * col_auth, row_auth, x, y);
		wnd.GetViewSize(3 * col_auth, row_auth, cx, cy);

		rect.Offset(x+4, y + (cy - m_length[CHECK_HEIGHT]) / 2);
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd( WND_TYPE_CHECK, GetRandWndID(), rect, col_auth, 2 + row_auth, ID_CH_NONE + i));
		assert(pCheck);
		if (i < 7)
		{
			pCheck->SetCaption(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_AUTH_LOG_VIEW + i)),false,false);
		}
		else if(7 == i)
		{
			pCheck->SetCaption(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_AUTH_DIGITAL_MAN)),false,false);
		}
			
		//不具备语音对讲权限，则不使能。
		if (!g_p_login_succ->productInfo.talkAudio && \
			(E_STR_ID_AUTH_AUDIO_TALK == static_cast<ENUM_STRING_ID>(E_STR_ID_AUTH_LOG_VIEW+i)))
		{
			pCheck->SetEnable(false, false);
		}
	}
	if (0 == g_p_login_succ->productInfo.netVideoInputNum)
	{
		wnd.ShowSubWnd(ID_CH_NONE + 7, false);
	}
	int row = 4;

	if(!IsSupportShutDown())
	{
		wnd.ShowSubWnd(ID_CH_NONE + 2, false);
		row = 3;
	}
	
	int chIndex = 0;//代表一组（以和通道相关的配置项划分，比如ID_CH_LIVE_GROUP）
	do 
	{
		//这一部分是处理每一组配置项总开关。
		{
			GUI::CRect rect(0, 0, width * 10, m_length[CHECK_HEIGHT]);
			unsigned long x, y, cx, cy;
			wnd.GetViewPos( 0, row, x, y);
			wnd.GetViewSize( 0, row, cx, cy);

			rect.Offset(x+4, y + (cy-m_length[CHECK_HEIGHT]) / 2);
			int id = ID_CH_HAVE+chIndex;
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd( WND_TYPE_CHECK,GetRandWndID(), rect, 0, (chRow+1)*chIndex + 5, id));
			assert(pCheck);
			pCheck->SetCaption(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_AUTH_LIVE_VIEW+chIndex)),false,false);
		}

		row += 1;//下移一行

		int chnn = -1;

		//处理某个配置项具体通道的。
		for ( int j=0; j<chRow; j++)
		{
			//应该以4个通道为一小组（每一小组带一个总开关），所以一行实质上有10 个。
			for (int i=0; i<10; i++)
			{
				if( chnn + 1 >= m_maxChnn ) break;//超出当前通道的总数，则跳出。

				//滚动窗口要求控件必须能被容纳在视图内
				GUI::CRect rect(0, 0, width-8, m_length[CHECK_HEIGHT]);
				unsigned long x, y, cx, cy;
				wnd.GetViewPos( i, j+row, x, y);
				wnd.GetViewSize( i, j+row, cx, cy);

				AddDrawBorderPos(i, j+row);

				if( 0 == i || 5 == i )
				{
					//这个是对应小组总开关。
					rect.SetSize(m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT]);
					rect.Offset(x+(cx-m_length[CHECK_WIDTH])/2, y+(cy-m_length[CHECK_HEIGHT])/2);
				}
				else
				{
					rect.Offset(x+4,y+(cy-m_length[CHECK_HEIGHT])/2);
				}

				int idBaseGroup = ID_CH_LIVE_GROUP;
				int idBaseCh = ID_CH_LIVE;
				switch ( chIndex )
				{
				case 0:
					idBaseGroup = ID_CH_LIVE_GROUP;
					idBaseCh = ID_CH_LIVE;
					break;
				case 1:
					idBaseGroup = ID_CH_RECORD_GROUP;
					idBaseCh = ID_CH_RECORD;
					break;
				case 2:
					idBaseGroup = ID_CH_PLAYBACK_GROUP;
					idBaseCh = ID_CH_PLAYBACK;
					break;
				case 3:
					idBaseGroup = ID_CH_BACKUP_GROUP;
					idBaseCh = ID_CH_BACKUP;
					break;
				case 4:
					idBaseGroup = ID_CH_PTZ_GROUP;
					idBaseCh = ID_CH_PTZ;
					break;
				case 5:
					idBaseGroup = ID_CH_REMOTE_GROUP;
					idBaseCh = ID_CH_REMOTE;
					break;
				default:
					assert(false);
					break;
				}

				//计算通道编号
				chnn = -1;
				int id = -1;
				if( 0 == i )
				{
					id = idBaseGroup+j*2;
				}

				if( (i > 0) && (i < 5) )
				{
					chnn = j * 8 + i-1;
					id = idBaseCh+chnn;
				}

				if( 5 == i )
				{
					id = idBaseGroup+j*2+1;
				}

				if( i > 5 )
				{
					chnn = j * 8 + i - 2;
					id = idBaseCh+chnn;
				}

				GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd( WND_TYPE_CHECK,GetRandWndID(),rect,i + 1,chIndex*(chRow+1)+5+j,id));
				assert(pCheck);
				if( (0 != i) && (5 !=i) )
				{
					sprintf(str, "%d", chnn+1);
					pCheck->SetCaption(str, false, false);
				}
				else
				{
#if defined(__TW01_RILI__)
					if (5 == chIndex)
					{
						pCheck->SetEnable(false, false);
					}
#endif
				}
			}
		}

		row += chRow;

		++chIndex;

		if(!IsSupportPTZ() && (4 == chIndex))
		{
			++chIndex;
		}
	} while (chIndex < authCH);
}

void CCfgUserAuthorityDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CScrollDialog::OnPaint(status);
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_SELECT, 2);
}

bool CCfgUserAuthorityDlg::GetInfoUserAuth( NCFG_INFO_USER& infoUser )
{
	GetAuthCommon(infoUser.authority);

	GetAuthChnn(ID_CH_LIVE,infoUser.authLiveCH);
	GetAuthChnn(ID_CH_RECORD,infoUser.authRecordCH);
	GetAuthChnn(ID_CH_PLAYBACK,infoUser.authPlaybackCH);
	GetAuthChnn(ID_CH_BACKUP,infoUser.authBackupCH);

	if(IsSupportPTZ())
	{
		GetAuthChnn(ID_CH_PTZ,infoUser.authPTZCtrlCH);
	}
	
	GetAuthChnn(ID_CH_REMOTE,infoUser.authRemoteViewCH);

	return true;
}

void CCfgUserAuthorityDlg::ShowInfoUserAuth( const NCFG_INFO_USER& infoUser, bool bRefresh )
{
	SetAuthCommon(infoUser.authority, false);

	SetAuthChnn(ID_CH_LIVE,infoUser.authLiveCH,false);
	SetAuthChnn(ID_CH_RECORD,infoUser.authRecordCH,false);
	SetAuthChnn(ID_CH_PLAYBACK,infoUser.authPlaybackCH,false);
	SetAuthChnn(ID_CH_BACKUP,infoUser.authBackupCH,false);
	
	if(IsSupportPTZ())
	{
		SetAuthChnn(ID_CH_PTZ,infoUser.authPTZCtrlCH,false);
	}
	
	SetAuthChnn(ID_CH_REMOTE,infoUser.authRemoteViewCH,false);

	//模拟点击,刷新4通道全选Check
	for ( int i=0;i<(m_maxChnn+3)/4;i++ )
	{
		OnClickChnn(ID_CH_LIVE,ID_CH_LIVE+4*i,false);
		OnClickChnn(ID_CH_RECORD,ID_CH_RECORD+4*i,false);
		OnClickChnn(ID_CH_PLAYBACK,ID_CH_PLAYBACK+4*i,false);
		OnClickChnn(ID_CH_BACKUP,ID_CH_BACKUP+4*i,false);
		if(IsSupportPTZ())
		{
			OnClickChnn(ID_CH_PTZ,ID_CH_PTZ+4*i,false);
		}
		OnClickChnn(ID_CH_REMOTE,ID_CH_REMOTE+4*i,false);
	}

	////
	CheckUIEnable(false);

	if( bRefresh )
	{
		Repaint();
	}
}

void CCfgUserAuthorityDlg::SetMaxChnn(int max, ULONGLONG effectCH)
{
	//assert(0==max%4);//检查设置最大通道数量必须为4的倍数
	m_maxChnn = max;
	m_effectCH = effectCH;
}

void CCfgUserAuthorityDlg::SetAuthChnn(int baseID, ULONGLONG auth, bool bRefresh)
{
	for(int i=0; i < m_maxChnn; i++)
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(baseID+i));
		//assert(pCheck);//查找窗口ID错误,或未创建
		if(pCheck)
		{
			if(!pCheck->IsEnable())
			{
				pCheck->SetEnable(true, false);
			}

#if defined(__TW01_RILI__)
			if (ID_CH_REMOTE == baseID)
			{
				pCheck->SetEnable(false, false);
			}
#endif
			pCheck->SetCheck(((auth >> i) & 0x1), bRefresh);
		}
	}
}

void CCfgUserAuthorityDlg::GetAuthChnn(int baseID, ULONGLONG & auth)
{
	auth = static_cast<ULONGLONG>(0x0);
	for(int i = 0; i < m_maxChnn; i++)
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(baseID+i));
		//assert(pCheck);//查找窗口ID错误,或未创建
		if(pCheck && pCheck->IsCheck())
		{
			auth |= ( static_cast<ULONGLONG>(0x1)<<i );
		}
	}
}

void CCfgUserAuthorityDlg::SetAuthCommon(unsigned long userRight,bool bRefresh)
{
	//非通道权限
	int authNoCH = 8;//和通道无关的配置项，一行放3个。
	for ( int i = 0; i < authNoCH; i++ )
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CH_NONE+i));
		//assert(pCheck);
		if(pCheck)
		{
			if(!pCheck->IsEnable())
			{
				pCheck->SetEnable(true, false);
			}

#if defined(__TW01_RILI__)
			if ((3 == i) || (6 == i))
			{
				pCheck->SetEnable(false,false);
			}
#endif

			if (i < 7)
			{
				pCheck->SetCheck(((userRight>>i) & AUTH_LOG_VIEW), bRefresh);
			}
			else if(7 == i)
			{
				pCheck->SetCheck( ((userRight>>(i+1))&AUTH_LOG_VIEW), bRefresh );
			}
		}
	}

	//通道相关权限
	for ( int i=0;i<6;i++ )
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CH_HAVE+i));
		//assert(pCheck);
		if(pCheck)
		{
			if(!pCheck->IsEnable())
			{
				pCheck->SetEnable(true, false);
			}
			pCheck->SetCheck( ((userRight >> i) & AUTH_LIVE), bRefresh );
		}
	}
}

void CCfgUserAuthorityDlg::GetAuthCommon(unsigned long& userRight)
{
	unsigned long right = AUTH_BASIC;

	//非通道权限
	int authNoCH = 8;//和通道无关的配置项，一行放3个。
	for(int i = 0; i < authNoCH; i++)
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CH_NONE+i));
		//assert(pCheck);
		if(pCheck && pCheck->IsCheck())
		{
			if (i < 7)
			{
				right |= (static_cast<unsigned long>(AUTH_LOG_VIEW) << i);
			}
			else if(7 == i)
			{
				right |= ( static_cast<unsigned long>(AUTH_LOG_VIEW) << (i+1) );
			}
		}
	}

	//通道相关权限
	for(int i = 0; i < 6; i++)
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CH_HAVE+i));
		//assert(pCheck);
		if(pCheck)
		{
			if(pCheck->IsCheck())
			{
				right |= ( static_cast<unsigned long>(AUTH_LIVE) << i );
			}
		}
	}

	userRight = right;
}

void CCfgUserAuthorityDlg::CheckUIEnable(bool bRefresh)
{
	//选中远程登录 后 才可以设置远程监视
	//选中远程监视 后 才可以设置通道权限

	int idRemoteLogin = ID_CH_NONE + 6;
	int idRemoteView = ID_CH_HAVE + 5;
	
	GUI::CCheckCtrl* pCkLogin = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(idRemoteLogin));
	GUI::CCheckCtrl* pCkView = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(idRemoteView));
	assert(pCkLogin);
	assert(pCkView);

	//远程监视 使能
	if(pCkLogin->IsCheck())
	{
		if(!pCkView->IsEnable())
		{
			pCkView->SetEnable(true, bRefresh);
		}
	}
	else
	{
		if(pCkView->IsCheck())
		{
			pCkView->SetCheck(false, false);
		}

		if(pCkView->IsEnable())
		{
			pCkView->SetEnable(false, bRefresh);
		}
	}
}

BEGIN_MSG_MAP(CCfgUserAuthorityDlg, GUI::CScrollDialog)
	ON_MSG_EX(GetScrollWnd().GetID(),KEY_VIRTUAL_MSG_DEFAULT_TAG,OnClickScrollWnd)
END_MSG_MAP()

unsigned long CCfgUserAuthorityDlg::OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int index = -1;
	unsigned long tmpIndex = (keyID >> 20)&0x7FF;
	if( tmpIndex < 0x7FF )
	{
		index = tmpIndex;
	}
	unsigned long id = keyID&0xFFFFF;

	printf("(%s,line=%d)index=%d\n",__FILE__,__LINE__,index);

	if( index >= 0 )
	{
		if( index == ID_CH_NONE + 6 ||
			index == ID_CH_HAVE + 5 )
		{
			CheckUIEnable(true);
		}

		if( index >= ID_CH_REMOTE )
		{
			OnClickChnn(ID_CH_REMOTE,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if( index >= ID_CH_REMOTE_GROUP )
		{
			OnClickGroup(ID_CH_REMOTE_GROUP,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if( index >= ID_CH_PTZ )
		{
			OnClickChnn(ID_CH_PTZ,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if( index >= ID_CH_PTZ_GROUP )
		{
			OnClickGroup(ID_CH_PTZ_GROUP,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if( index >= ID_CH_BACKUP )
		{
			OnClickChnn(ID_CH_BACKUP,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if( index >= ID_CH_BACKUP_GROUP )
		{
			OnClickGroup(ID_CH_BACKUP_GROUP,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if( index >= ID_CH_PLAYBACK )
		{
			OnClickChnn(ID_CH_PLAYBACK,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if( index >= ID_CH_PLAYBACK_GROUP )
		{
			OnClickGroup(ID_CH_PLAYBACK_GROUP,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if( index >= ID_CH_RECORD )
		{
			OnClickChnn(ID_CH_RECORD,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if( index >= ID_CH_RECORD_GROUP )
		{
			OnClickGroup(ID_CH_RECORD_GROUP,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if( index >= ID_CH_LIVE )
		{
			OnClickChnn(ID_CH_LIVE,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
		else if( index >= ID_CH_LIVE_GROUP )
		{
			OnClickGroup(ID_CH_LIVE_GROUP,index,true);
			return KEY_VIRTUAL_MSG_STOP;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

void CCfgUserAuthorityDlg::OnClickChnn(int base,int index,bool bRefresh)
{
	int ch = index - base;
	assert(ch>=0);
	assert(ch<m_maxChnn);

	int group = ch/4;
	bool check = true;
	for ( int i=0;i<4;i++)
	{
		GUI::CCheckCtrl* pCheckChnn = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(base+group*4+i));
		//assert(pCheckChnn);
		if( pCheckChnn && !pCheckChnn->IsCheck() )
		{
			check =false;
			break;
		}
	}

	GUI::CCheckCtrl* pCheckGroup = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(base+group-8));
	//assert(pCheckGroup);//检查参数id是否正确
	if( pCheckGroup && check != pCheckGroup->IsCheck() )
	{
		if(!pCheckGroup->IsEnable())
		{
			pCheckGroup->SetEnable(true, false);
		}
		pCheckGroup->SetCheck(check,bRefresh);
	}
}

void CCfgUserAuthorityDlg::OnClickGroup(int base,int index,bool bRefresh)
{
	int group = index-base;

	GUI::CCheckCtrl* pCheckGroup = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(index));
	assert(pCheckGroup);

	bool check = pCheckGroup->IsCheck();

	for ( int i=0;i<4;i++ )
	{
		//assert(group*4+i<m_maxChnn);//最大通道必须是4的整数倍
		GUI::CCheckCtrl* pCheckChnn = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(base+group*4+i+8));
		//assert(pCheckChnn);

		if(pCheckChnn && (check != pCheckChnn->IsCheck()))
		{
			pCheckChnn->SetCheck(check,bRefresh);
		}
	}
}

//end
