/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgPtzPresetDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgPtzPresetDlg.h"
#include "ListCtrlEx.cpp"
#include "CfgDlg.h"
#include "MessageMan.h"

//////////////////////////////////////////////////////////////////////////
template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	PTZ_PRESET &it = node;
	
	if (0 == col)
	{
		char str [8] = {0};
		sprintf(str, "%d", it.index + 1);
		m_str = str;
	}

	if (1 == col)
	{
		m_str = it.name;
	}

	return m_str;
}
//////////////////////////////////////////////////////////////////////////

CCfgPtzPresetDlg::CCfgPtzPresetDlg() : m_pPresetID(NULL)
, m_curChnn(0)
{

}

CCfgPtzPresetDlg::~CCfgPtzPresetDlg()
{
}

void CCfgPtzPresetDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	char str [128] = {0};
	if (INVALID_INDEX(unsigned char) == m_curChnn)
	{
		m_curChnn = 0;
	}

	snprintf(str, 128, "%s - %s %d",  m_pStrTable->String(E_STR_ID_TITLE_PRESET), m_pStrTable->String(E_STR_ID_CHANNEL), m_curChnn+1);
	SetCaption(str, false);
	
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String( E_STR_ID_EXIT ) );

	//////////////////////////////////////////////////////////////////////////
	int list_left = m_length[DLG_OFFSET_LEFT];
	int list_top = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	int list_width = m_rect.Width() - m_length[DLG_OFFSET_LEFT] * 2;
	int list_height = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS] -list_top;

	m_listCtrl.Create(GetRandWndID(), list_left, list_top, list_width, list_height, this, 0, 1, true);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_NUMBER), 64);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_ENABLE), 128);	//name
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_NAME), 252);
	m_listCtrl.AddColumn( m_pStrTable->String( E_STR_ID_PRESET ), list_width - 446 - m_length[SCROLL_WIDTH]);

	//////////////////////////////////////////////////////////////////////////
	ShowPresset(false);
}

void CCfgPtzPresetDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	if (NULL != m_pPresetID)
	{
		delete[] m_pPresetID;
		m_pPresetID = NULL;
	}
}


void CCfgPtzPresetDlg::InitialPresetID()
{
	assert(NULL == m_pPresetID);

	m_pPresetID = new BYTE[CCfgDlg::s_loginSuccessReply.productInfo.presetNumOneCH];
	memset(m_pPresetID, 0, CCfgDlg::s_loginSuccessReply.productInfo.presetNumOneCH);

	int count = m_presetList.size();
	assert(count <= CCfgDlg::s_loginSuccessReply.productInfo.presetNumOneCH);
	for (std::vector<PTZ_PRESET>::const_iterator iter = m_presetList.begin(); iter != m_presetList.end(); ++iter)
	{
		m_pPresetID[iter->index] = 1; 
	}
}

void CCfgPtzPresetDlg::ShowPresset(bool bRefresh)
{
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CButton *pButModify = NULL;
	GUI::CEditCtrl *pEditPresetName = NULL;
	char str[128] = {0};

	//在刷新界面上预置点信息时要先清空链表，然后再刷新新的预置点信息
	m_listCtrl.RemoveAllItem( false );

	for ( int i = 0; i < m_presetList.size(); i++ )
	{
		//预置点序号
		sprintf( str, "%d", m_presetList[i].index + 1 );
		m_listCtrl.AddItem( i, COL_PRESET_NUM, str );
		
		//使能
		pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_listCtrl.AddItem( i, COL_PRESET_ENABLE, WND_TYPE_CHECK ) );
		assert( pCheck );
		bool enable = ( 0 == m_presetList[i].enable )?false:true;
		pCheck->SetCheck( enable, false );
		
		//预置点名字
		pEditPresetName = dynamic_cast<GUI::CEditCtrl*>( m_listCtrl.AddItem( i, COL_PRESET_NAME, WND_TYPE_EDIT ) );
		assert(pEditPresetName);
		snprintf( str, 128, "%s", m_presetList[i].name );
		pEditPresetName->SetCaption(str, false);
		//m_listCtrl.AddItem( i, COL_PRESET_NAME, str );

		//更改预置点按钮
		pButModify	= dynamic_cast<GUI::CButton*>( m_listCtrl.AddItem( i, COL_PRESET_MODIFY, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]) );
		assert( NULL != pButModify );
		pButModify->SetCaption(m_pStrTable->String(E_STR_ID_SETTING), false);
	
	}
	

	if (bRefresh)
	{
		Repaint();
	}

	m_listCtrl.CreateListFocusMap();
}

unsigned long CCfgPtzPresetDlg::GetPresetID()
{
	assert(NULL != m_pPresetID);
	for (int i = 0; i < CCfgDlg::s_loginSuccessReply.productInfo.presetNumOneCH; ++i)
	{
		if (0 == m_pPresetID[i])
		{
			m_pPresetID[i] = 1;
			return i;
		}
	}
	return INVALID_PRESET_ID;
}

void CCfgPtzPresetDlg::SetPTZConfig(std::vector<PTZ_SERIAL_INFO> ptzConfig)
{
	m_ptzPresetDlg.SetPTZConfig(ptzConfig);
}

void CCfgPtzPresetDlg::SetChnn(int chnn)
{
	m_curChnn = chnn;
}

void CCfgPtzPresetDlg::SetPresetList(const std::vector<PTZ_PRESET> & presetList)
{
	m_presetList = presetList;
}

void CCfgPtzPresetDlg::GetPresetList(std::vector<PTZ_PRESET> & presetList) const
{
	presetList = m_presetList;
	
}
 
BEGIN_MSG_MAP(CCfgPtzPresetDlg, GUI::CDialog)
ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP() 

unsigned long CCfgPtzPresetDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	switch (col)
	{
	case COL_PRESET_ENABLE:
		{
			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.GetItemWnd( row, COL_PRESET_ENABLE ) );
			m_presetList[row].enable = pCheck->IsCheck();
		}
		break;
	case COL_PRESET_NAME:
		{
			GUI::CEditCtrl *pEditPresetName = dynamic_cast<GUI::CEditCtrl *>(m_listCtrl.GetItemWnd( row, COL_PRESET_NAME ) );;
			for (std::vector<PTZ_PRESET>::iterator iterCur = m_presetList.begin(); iterCur != m_presetList.end(); ++iterCur)
			{
				if (iterCur->index == row)
				{
					strcpy(iterCur->name,  pEditPresetName->GetCaption().c_str());		
					break;
				}
			}
		}
		break;
	case COL_PRESET_MODIFY:
		{
			if( row >= 0 )
			{
				NET_DISPLAY_INFO disp;
				memset(&disp, 0, sizeof(NET_DISPLAY_INFO));
				disp.type = NET_DISPLAY_FREEZE;

				MESSAGE_DATA msgData;
				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&disp, sizeof(NET_DISPLAY_INFO), msgData);
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				m_ptzPresetDlg.SetPresetInfo( m_presetList[row].index, m_presetList);
				m_ptzViewDlg.SetCtrlDlg(&m_ptzPresetDlg, CPTZPresetCtrl::WIDTH, CPTZPresetCtrl::HEIGHT, CPTZViewDlg::CURR_PTZCTRLDLG_PRESET);
				
				m_ptzViewDlg.DoModal( this, CENTER_POS,  CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false );

				memset(&msgData, 0, sizeof(MESSAGE_DATA));
				CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, NULL, 0, msgData);
				CMessageMan::Instance()->ReleaseMsgBuff(msgData);

				//if (m_ptzPresetDlg.GetPresetInfo(m_presetList))
				//{
				//	ShowPresset(true);
				//}
			}
		}
		break;
	default:
		assert(false);
		break;
	}

	return KEY_VIRTUAL_MSG_CONTINUE;

}
