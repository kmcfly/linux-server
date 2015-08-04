/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgPtzCruiseDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgCruiseDlg.h"
#include "CfgPtzCruiseDlg.h"
#include "CfgDlg.h"

#include "ListCtrlEx.cpp"
#include "CruiseOther.h"

#include "FuncCustom.h"
extern NET_LOGIN_SUCCESS_REPLY *	g_p_login_succ;

//////////////////////////////////////////////////////////////////////////
template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	CRUISE_INFO &it = node;
	
	if (0 == col)
	{
		char str [8] = {0};
		sprintf(str, "%d", it.index + 1);
		m_str = str;
	}

	if (1 == col)
	{
		m_str = it.strName;
	}

	return m_str;
}

//template <class T>
//long  GUI::CListCtrlEx<T>::ForEach(T &node, long item, long param)
//{
//	if (0 == param)//ÖØÐÂÅÅÐò
//	{
//		CRUISE_INFO &it = node;
//		it.index = item;
//	}
//
//	return 0;
//}
//////////////////////////////////////////////////////////////////////////

CCfgPtzCruiseDlg::CCfgPtzCruiseDlg() : m_pCruiseID(NULL)
{

}

CCfgPtzCruiseDlg::~CCfgPtzCruiseDlg()
{

}

void CCfgPtzCruiseDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	unsigned char chnn = CMessageMan::Instance()->CurrPTZChnn();
	char str [128] = {0};
	if (INVALID_INDEX(unsigned char) == chnn)
	{
		chnn = 0;
		CMessageMan::Instance()->SetCurrPTZChnn(chnn);
	}
	sprintf(str, "%s - %s %d",  m_pStrTable->String(E_STR_ID_TITLE_CRUISE), m_pStrTable->String(E_STR_ID_CHANNEL), chnn+1);
	SetCaption(str, false);
	
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String( E_STR_ID_EXIT ) );
	//////////////////////////////////////////////////////////////////////////
	int list_left = m_length[DLG_OFFSET_LEFT];
	int list_top = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	int list_width = m_rect.Width() - m_length[DLG_OFFSET_LEFT] * 2;
	int list_height = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS] - list_top;

	m_listCtrlEx.Create(GetRandWndID(), list_left, list_top, list_width, list_height, this, 0, 1);
	m_listCtrlEx.EnableSingleSelect(); 
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_NUMBER), 64);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_NAME), list_width-70);


	//////////////////////////////////////////////////////////////////////////
	int btn_top = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	int btn_left = m_length[DLG_OFFSET_LEFT];

	m_btnAdd.Create(GetRandWndID(), btn_left, btn_top, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 0, 2, m_pStrTable->String(E_STR_ID_ADD));

	btn_left += m_length[BTN_WIDTH] + 10;
	m_btnModify.Create(GetRandWndID(), btn_left, btn_top, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 1, 2, m_pStrTable->String(E_STR_ID_SETUP));

	btn_left += m_length[BTN_WIDTH] + 10;
	m_btnDel.Create(GetRandWndID(), btn_left, btn_top, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 2, 2, m_pStrTable->String(E_STR_ID_DELETE));

	btn_left += m_length[BTN_WIDTH] + 10;
	//m_btnClear.Create(GetRandWndID(), btn_left, btn_top, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 3, 2, m_pStrTable->String(E_STR_ID_CLEAR_ALL));

	//////////////////////////////////////////////////////////////////////////
	InitialCruiseID();
	ShowCruise(false);
}

void CCfgPtzCruiseDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
	if (NULL != m_pCruiseID)
	{
		delete [] m_pCruiseID;
		m_pCruiseID = NULL;
	}
}

void CCfgPtzCruiseDlg::ShowCruise(bool bRefresh)
{
	m_listCtrlEx.PutData(NULL, 0, false);//Çå¿Õ
	for (std::vector<CRUISE_INFO>::const_iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
	{
		m_listCtrlEx.AddItem(*iter, false);
	}

	if (bRefresh)
	{
		Repaint();
	}
}

unsigned long CCfgPtzCruiseDlg::OnOK()
{
	return GUI::CDialog::OnOK();
}

void CCfgPtzCruiseDlg::InitialCruiseID()
{
	assert(NULL == m_pCruiseID);

	m_pCruiseID = new BYTE[CCfgDlg::s_loginSuccessReply.productInfo.presetNumOneCH];
	memset(m_pCruiseID, 0, CCfgDlg::s_loginSuccessReply.productInfo.presetNumOneCH);

	int count = m_CruiseList.size();
	assert(count < CCfgDlg::s_loginSuccessReply.productInfo.presetNumOneCH);
	for (CRUISE_INFO_LIST::const_iterator iter = m_CruiseList.begin(); iter != m_CruiseList.end(); ++iter)
	{
		m_pCruiseID[iter->index] = 1; 
	}
}

unsigned long CCfgPtzCruiseDlg::GetCruiseID()
{
	assert(NULL != m_pCruiseID);
	for (int i = 0; i < CCfgDlg::s_loginSuccessReply.productInfo.cruiseNumOneCH; ++i)
	{
		if (0 == m_pCruiseID[i])
		{
			m_pCruiseID[i] = 1;
			return i;
		}
	}
	return INVALID_CRUISE_ID;
}
 
BEGIN_MSG_MAP(CCfgPtzCruiseDlg, GUI::CDialog)
	ON_MSG(m_btnAdd.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnAdd)
	ON_MSG(m_btnModify.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnModify)
	//ON_MSG(m_btnClear.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnClearAll)
	ON_MSG(m_btnDel.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnDel)
	ON_MSG(m_listCtrlEx.GetID(), KEY_VIRTUAL_DBCLICK, OnClickBtnModify)
END_MSG_MAP() 

unsigned long CCfgPtzCruiseDlg::OnClickBtnAdd()
{
	CRUISE_INFO cruiseInfo;
	cruiseInfo.index = GetCruiseID();//m_listCtrlEx.GetCount();
	if (INVALID_CRUISE_ID != cruiseInfo.index)
	{
		cruiseInfo.strName = m_pStrTable->String(E_STR_ID_CRUISE);
		m_CruiseList.push_back(cruiseInfo);

		m_listCtrlEx.AddItem(cruiseInfo, false);
		m_listCtrlEx.SetSingleSelect(m_listCtrlEx.GetCount() - 1, true); 
	}
	else
	{
		char sz[128] = {0};
		snprintf( sz, 128, "%s %d!", m_pStrTable->String(E_STR_ID_MESSAGE_49), CCfgDlg::s_loginSuccessReply.productInfo.cruiseNumOneCH );
		MessageBox( sz, m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);
	}	

	return KEY_VIRTUAL_MSG_STOP;
}
 
unsigned long CCfgPtzCruiseDlg::OnClickBtnModify()
{
long curSel = m_listCtrlEx.GetSingleSelect();
	if (curSel >= 0)
	{
		assert(curSel < CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum);

		PopupCruisePresetDlg(this, m_CruiseList, m_CruiseList[curSel].index);
		ShowCruise(true);
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CCfgPtzCruiseDlg::OnClickBtnClearAll()
{
	m_listCtrlEx.PutData(NULL, 0, true);
	m_CruiseList.clear();
	memset(m_pCruiseID, 0, CCfgDlg::s_loginSuccessReply.productInfo.presetNumOneCH);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgPtzCruiseDlg::OnClickBtnDel()
{
	long curSel = m_listCtrlEx.GetSingleSelect();
	if (curSel >= 0)
	{
		m_listCtrlEx.DelItem(curSel, false);
        CCruiseDelete * pTempCruise = CCruiseDelete::GetInstanceCruiseDel();
		int localVideoInputNum = g_p_login_succ->productInfo.localVideoInputNum;
		unsigned int channelNum = CMessageMan::Instance()->CurrPTZChnn();
		bool repetition = false;
		if (localVideoInputNum <= channelNum)
		{
			unsigned int deleteCruise = 0;
			CruiseDelData delData(deleteCruise,channelNum);
			
			for (CRUISE_INFO_LIST::iterator iter = m_CruiseList.begin();iter != m_CruiseList.end(); iter++)
			{
				if (iter->index < 8){
					if (0 != pTempCruise->m_CruiseDel.size())
					{
						std::vector<CruiseDelData>::iterator iterV = pTempCruise->m_CruiseDel.begin();
						for(;iterV != pTempCruise->m_CruiseDel.end();iterV++){
							if (iterV->m_deleteInfo == iter->index)
							{
								repetition = true;
								break;
							}
						}
						if ( false==repetition){
							delData.m_deleteInfo = iter->index;
							pTempCruise->m_CruiseDel.push_back(delData);
						}
					}else{
						delData.m_deleteInfo = iter->index;
						pTempCruise->m_CruiseDel.push_back(delData);
					}
				}      
			}
		}

		m_pCruiseID[m_CruiseList[curSel].index] = 0;
		m_CruiseList.erase(m_CruiseList.begin() + curSel);
	}

	if (curSel > 0)
	{
		m_listCtrlEx.SetSingleSelect(curSel - 1, false); 
	}

	m_listCtrlEx.Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}
