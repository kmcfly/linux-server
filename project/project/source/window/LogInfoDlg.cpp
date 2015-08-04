/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : LogInfoDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "LogInfoDlg.h"
#include "ListCtrlEx.cpp"

#include "NetProtocol.h"
#include "MessageMan.h"
#include "ExportLogList.h"
#include "IDToStringID.h"
#include "DetailsInfoDlg.h"

#include <string.h>


//////////////////////////////////////////////////////////////////////////
template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	OPERATOR_LOG &it = node;
	char str [512] = {0};

	unsigned long type = (it.contentID << 16) | it.type;

	if (0 == col)
	{
		strcpy(str, m_pStrTable->String(static_cast<ENUM_STRING_ID>(LogIDToStrID(type))));
	}

	if (1 == col)
	{
		snprintf(str, 512, "%s", it.name);
	}

	if (2 == col)
	{
		//tm time = DVRTime32ToTm(it.time);
		CTimeCtrl::GetTime32Text(m_str, it.time);
		return m_str;
	}

#if !defined(__TW01_RILI__)
	if (3 == col)
	{
		sprintf(str, "%03d.%03d.%03d.%03d", it.IP&0xff, (it.IP>>8)&0xff, (it.IP>>16)&0xff, it.IP>>24);
	}
#endif

	m_str = str;
	return m_str;
}

//////////////////////////////////////////////////////////////////////////
CLogInfoDlg::CLogInfoDlg()
{
	m_pLogData = NULL;
	m_logNum = 0;
	m_pLogData = NULL;
	m_logNum = 0;
}

CLogInfoDlg::~CLogInfoDlg()
{
	if(NULL != m_pLogData)
	{
		delete [] m_pLogData;
		m_pLogData = NULL;
	}

	m_logNum = 0;
}

//////////////////////////////////////////////////////////////////////////
void CLogInfoDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_LOG_LIST), false);

#ifdef __DVR_PROFESSIONAL__
	unsigned short cx = m_rect.Width() - 4 * m_length[BTN_WIDTH] - 4 * m_length[DEFAULT_BTN_OFFSET] - 2 * m_length[DEFAULT_BTN_RIGHT_XPOS];
	SetDefaultBtn(DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_BTN_APPLY);
	SetDefaultBtnCaption(DEFAULT_BTN_CANCEL,m_pStrTable->String(E_STR_ID_EXIT));
	SetDefaultBtnCaption(DEFAULT_BTN_OK,m_pStrTable->String(E_STR_ID_SEARCH));
	SetDefaultBtnCaption(DEFAULT_BTN_APPLY,m_pStrTable->String(E_STR_ID_EXPORT));
#else
	SetDefaultBtn(DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption(DEFAULT_BTN_CANCEL,m_pStrTable->String(E_STR_ID_EXIT));
	SetDefaultBtnCaption(DEFAULT_BTN_OK,m_pStrTable->String(E_STR_ID_SEARCH));
#endif
	//////////////////////////////////////////////////////////////////////////

	if( GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 )
	{
		CreateUnfoldCtrl();
	}
	else
	{
		CreateFoldCtrl();
	}

	tm tmTime = DVRTime32ToTm(GetCurrTime32());
	//tmTime.tm_mon = tmTime.tm_mon;
	tmTime.tm_hour = 0;
	tmTime.tm_min  = 0;
	tmTime.tm_sec  = 0;
	m_logSearchConditionDlg.SetStartTime(tmTime);


	tmTime.tm_hour = 23;
	tmTime.tm_min  = 59;
	tmTime.tm_sec  = 59;
	m_logSearchConditionDlg.SetEndTime(tmTime);

	//大尺寸画面不必要提示了，列表中已经可以完整显示。
	if (m_width <= 800)
	{
#ifdef __DVR_PROFESSIONAL__
		CreateTipWnd(cx);
#else
		CreateTipWnd();
#endif
	}

	if(NULL != m_pLogData)
	{
		delete [] m_pLogData;
		m_pLogData = NULL;
	}

	m_logNum = 0;
}

void CLogInfoDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();

#ifdef __DVR_PROFESSIONAL__
	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
	assert(pBtn != NULL);
	pBtn->SetEnable(false, false);
#endif
}


void CLogInfoDlg::CreateFoldCtrl()
{
	m_searchConditionDlgWidth = m_length[CALENDAR_WIDTH] + 10;
	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

	m_listCtrlEx.Create(CTRLID_INFO_EVENT_LIST, x, y, width, height, this, 0, 1);
	m_listCtrlEx.EnableSingleSelect();
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_TYPE), 152);
#if !defined(__TW01_RILI__)
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_USER_NAME), width - 508);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_TIME), 208);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_IP), 148);
#else
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_USER_NAME), (width - 152) / 2);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_TIME), (width - 152) / 2);
#endif
//	m_listCtrlEx.EnableItemFocusNotify();

	GUI::CRect rect = m_listCtrlEx.GetClientRect();
	unsigned short yOff = ( m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] -   m_length[DLG_OFFSET_BOTTOM] - rect.Height() ) * 1 / 2;
	y += yOff;
	m_listCtrlEx.MoveWnd( m_rect.m_left + x, m_rect.m_top + y );

	x = m_length[DLG_OFFSET_LEFT] + width - m_searchConditionDlgWidth;
	m_logSearchConditionDlg.Create(GUI::CWnd::GetRandWndID(), x, y, m_searchConditionDlgWidth, rect.Height(), this, 2, 1);

	x -= m_length[FLEX_BTN_WIDTH];
	m_btnFlex.Create(GUI::CWnd::GetRandWndID(), x, (m_rect.Height()/2)-(m_length[FLEX_BTN_HEIGHT]/2), m_length[FLEX_BTN_WIDTH], m_length[FLEX_BTN_HEIGHT], this, 1, 1);

	m_listCtrlEx.SetWndSize(false, false, width - m_searchConditionDlgWidth - m_length[FLEX_BTN_WIDTH], rect.Height());

	m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_R, false);
	m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_R_F, false);

	m_bFlexSearchConditionDlg = false;

	//////////////////////////////////////////////////////////////////////////
}

void CLogInfoDlg::CreateUnfoldCtrl()
{
	m_searchConditionDlgWidth = m_length[CALENDAR_WIDTH] + 10;
	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];
	unsigned short listWidth = width - m_searchConditionDlgWidth - 4;

	m_listCtrlEx.Create(CTRLID_INFO_EVENT_LIST, x, y, listWidth, height, this, 0, 1);
	m_listCtrlEx.EnableSingleSelect();
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_TYPE), 192);
#if !defined(__TW01_RILI__)
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_USER_NAME), listWidth - 548);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_TIME), 208);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_IP), 148);
#else
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_USER_NAME), (listWidth - 192) / 2);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_TIME),  (listWidth - 192) / 2);
#endif
//	m_listCtrlEx.EnableItemFocusNotify();

	GUI::CRect rect = m_listCtrlEx.GetClientRect();
	unsigned short yOff = ( m_rect.Height()- y - m_length[DEFAULT_BTN_BOTTOM_YPOS] -  m_length[DLG_OFFSET_BOTTOM] - rect.Height() ) * 1 / 2;
	y += yOff;
	m_listCtrlEx.MoveWnd( m_rect.m_left + x, m_rect.m_top + y );

	x = m_length[DLG_OFFSET_LEFT] + width - m_searchConditionDlgWidth;
	m_logSearchConditionDlg.Create(GUI::CWnd::GetRandWndID(), x, y, m_searchConditionDlgWidth, rect.Height(), this, 2, 1);

	//////////////////////////////////////////////////////////////////////////
}

unsigned long CLogInfoDlg::OnOK()
{
	//检索
	if (0 != m_logSearchConditionDlg.GetCondition())
	{
		MESSAGE_DATA msgData;
		NET_LOG_SEARCH searchCond;

		searchCond.startTime = m_logSearchConditionDlg.TimeStart();
		searchCond.endTime = m_logSearchConditionDlg.TimeEnd()/*MAX_TIME*/;
		searchCond.content = m_logSearchConditionDlg.GetCondition();

		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOG_INFO, (unsigned char *)&searchCond, sizeof(NET_LOG_SEARCH), msgData);

		NET_LOG_INFO *pInfo = reinterpret_cast<NET_LOG_INFO *>(msgData.pData + sizeof(PACKCMD));

		OPERATOR_LOG *pDate = reinterpret_cast<OPERATOR_LOG *>(msgData.pData + sizeof(PACKCMD) + sizeof(NET_LOG_INFO));

		if(NULL != m_pLogData)
		{
			delete [] m_pLogData;
			m_pLogData=NULL;
		}

		if (pInfo->logNum <= 0)
		{
#ifdef __DVR_PROFESSIONAL__
			GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
			assert(pBtn != NULL);
			pBtn->SetEnable(false);
#endif

			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_36), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		}
		else
		{
			assert ((pInfo->logNum*sizeof(OPERATOR_LOG) + sizeof(NET_LOG_INFO) + sizeof(PACKCMD)) <= msgData.dataLen);

			OPERATOR_LOG *pLog = new OPERATOR_LOG[pInfo->logNum];
			
			memcpy(pLog, pDate, pInfo->logNum*sizeof(OPERATOR_LOG));
			m_listCtrlEx.PutData(pLog, pInfo->logNum);
			m_listCtrlEx.SetSingleSelect(0, false);	
			pLog = NULL;

			//保存搜索到的日志数据，以备备份。
			//这里存在一个隐患：有可能请求很长一段时间的数据，这时候分配的临时空间会很大从而导致系统不分配

			m_pLogData = new OPERATOR_LOG[pInfo->logNum];
			if( NULL == m_pLogData )
			{
				assert(false);
				m_logNum=0;
			}
			else
			{
				memcpy(m_pLogData, pDate, pInfo->logNum*sizeof(OPERATOR_LOG));
				m_logNum=pInfo->logNum;
				m_pDefaultBtn[2].SetEnable(true);

#ifdef __DVR_PROFESSIONAL__
				GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
				assert(pBtn != NULL);
				pBtn->SetEnable(true);
#endif
			}

		}

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	}	

	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CLogInfoDlg::OnApply()
{
	if((NULL != m_pLogData) && (0 != m_logNum))
	{
		//有日志数据，并且数据个数不为0的时候才允许生成xml文件
		char headTitle[132]={0};
		char *pElememtTitle[5]={NULL};
		pElememtTitle[0]=new char [132];
		pElememtTitle[1]=new char [132];
		pElememtTitle[2]=new char [132];
		pElememtTitle[3]=new char [132];

		strcpy(headTitle,m_pStrTable->String(E_STR_ID_TITLE_LOG_LIST));
		strcpy(pElememtTitle[0], m_pStrTable->String(E_STR_ID_TYPE));
		strcpy(pElememtTitle[1], m_pStrTable->String(E_STR_ID_USER_NAME));
		strcpy(pElememtTitle[2], m_pStrTable->String(E_STR_ID_TIME));
		strcpy(pElememtTitle[3], m_pStrTable->String(E_STR_ID_IP));
		PopupExportCtrlDlg(this, reinterpret_cast<char *>(&headTitle), reinterpret_cast<char **>(&pElememtTitle), reinterpret_cast<char *>(m_pLogData),  m_logNum*sizeof(OPERATOR_LOG), 0);

		delete [] pElememtTitle[0];
		delete [] pElememtTitle[1];
		delete [] pElememtTitle[2];
		delete [] pElememtTitle[3];
		delete [] pElememtTitle[4];
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_36), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

BEGIN_MSG_MAP(CLogInfoDlg, GUI::CDialog)
	ON_MSG(m_btnFlex.GetID(), KEY_VIRTUAL_ENTER, OnBtnFlex)
	ON_MSG_EX(m_listCtrlEx.GetID(), KEY_VIRTUAL_DBCLICK, OnListExDBClick)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_SEARCH, OnOK)
END_MSG_MAP()

unsigned long CLogInfoDlg::OnListExDBClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{

	long curSel = m_listCtrlEx.GetSingleSelect();
	std::string		str;

	if(curSel >= 0 && curSel < static_cast<unsigned long>(m_listCtrlEx.GetCount()))
	{
		const OPERATOR_LOG* pLog = m_listCtrlEx.GetListData();
		unsigned long logID = (pLog[curSel].contentID << 16) | pLog[curSel].type;
		//str = m_pStrTable->String(E_STR_ID_TYPE);
		//str += "         :    ";
		//str += m_pStrTable->String(static_cast<ENUM_STRING_ID>(LogIDToStrID(logID)));
		//str += "\\n";
		//str += m_pStrTable->String(E_STR_ID_USER_NAME);
		//str += "     :    ";
		//str += pLog[curSel].name;
		//str += "\\n";
		//str += m_pStrTable->String(E_STR_ID_TIME);
		//str += "         :    ";
		//std::string pstr;
		//CTimeCtrl::GetTime32Text(pstr, pLog[curSel].time);
		//str += pstr;
		//str += "\\n";
		//str += m_pStrTable->String(E_STR_ID_IP);
		//str += " :    ";
		//char* qstr = new char [32];
		//sprintf(qstr, "%03d.%03d.%03d.%03d",  pLog[curSel].IP&0xff, ( pLog[curSel].IP>>8)&0xff, ( pLog[curSel].IP>>16)&0xff,  pLog[curSel].IP>>24);
		//str += qstr;
		//delete qstr;
		//qstr = NULL;
		//MessageBox(str.c_str(), m_pStrTable->String(E_STR_ID_TYPE), DEFAULT_BTN_OK);
		
		CDetailsInfoDlg detailInfo;

		detailInfo.AddInfo(m_pStrTable->String(E_STR_ID_TYPE), m_pStrTable->String(static_cast<ENUM_STRING_ID>(LogIDToStrID(logID))));
		detailInfo.AddInfo(m_pStrTable->String(E_STR_ID_USER_NAME), pLog[curSel].name);
		
		std::string pstr;
		CTimeCtrl::GetTime32Text(pstr, pLog[curSel].time);
		detailInfo.AddInfo(m_pStrTable->String(E_STR_ID_TIME), pstr);

		char strIP[64] = {0};
		snprintf(strIP, 64, "%03d.%03d.%03d.%03d",  pLog[curSel].IP&0xff, (pLog[curSel].IP>>8)&0xff, (pLog[curSel].IP>>16)&0xff,  (pLog[curSel].IP>>24)&0xff);
		detailInfo.AddInfo(m_pStrTable->String(E_STR_ID_IP), strIP);

		detailInfo.DoModal(NULL);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CLogInfoDlg::OnBtnFlex()
{
	m_searchConditionDlgWidth = m_length[CALENDAR_WIDTH] + 10;
	unsigned short xLeft = m_length[DLG_OFFSET_LEFT], yTop = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];

	unsigned short x = xLeft, y = yTop;
	GUI::CRect rect = m_listCtrlEx.GetClientRect();

	if (m_bFlexSearchConditionDlg)
	{
		m_listCtrlEx.SetWndSize(false, false, width - m_searchConditionDlgWidth - m_length[FLEX_BTN_WIDTH], rect.Height() );

		m_logSearchConditionDlg.Show(true, false);

		x = m_rect.m_left + xLeft + width - m_searchConditionDlgWidth - m_length[FLEX_BTN_WIDTH];
		y = m_rect.m_top + m_rect.Height()/2 - m_length[FLEX_BTN_HEIGHT]/2;
		m_btnFlex.MoveWnd( x, y );

		m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_R, false);
		m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_R_F, false);
	}
	else
	{
		m_listCtrlEx.SetWndSize(true, false);

		m_logSearchConditionDlg.Show(false, false);

		x = m_rect.m_left + xLeft + width;
		y = m_rect.m_top + m_rect.Height()/2 - m_length[FLEX_BTN_HEIGHT]/2;
		m_btnFlex.MoveWnd( x, y );

		m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_L, false);
		m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_L_F, false);
	}

	m_bFlexSearchConditionDlg = !m_bFlexSearchConditionDlg;

	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

//end
