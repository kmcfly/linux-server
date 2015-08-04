
#include "CfgNetEmailDlg.h"


CCfgNetEmailDlg::CCfgNetEmailDlg()
{

}

CCfgNetEmailDlg::~CCfgNetEmailDlg()
{

}

void CCfgNetEmailDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgEmailList ;
	GUI::CWnd * pWnd = NULL;

	GUI::CEditCtrl*  pEdit = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CButton *pButton = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;
	GUI::CComboCtrl  *pCombo = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = height /m_length[LIST_ITEM_HEIGHT]; //List框带头
	if( ROW_COUNT > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - offsetEdge );

	list.AddItem(ROW_SMTP_NAME, 0, m_pStrTable->String(E_STR_ID_SMTP_SER));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_SMTP_NAME, 1, WND_TYPE_EDIT));
	pEdit->SetMaxStringLen(67);
	assert(pEdit);

	list.AddItem(ROW_SMTP_PORT, 0, m_pStrTable->String(E_STR_ID_PORT));
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(list.AddItem(ROW_SMTP_PORT, 1, WND_TYPE_NUM_EDIT));
	assert(pNumEdit);
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,65535);

	list.AddItem(ROW_SLL_CHECK, 0, m_pStrTable->String(E_STR_ID_SLL_CHECK));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_SLL_CHECK, 1, WND_TYPE_CHECK));
	assert(pCheck);

	list.AddItem(ROW_SEND_ADDR, 0, m_pStrTable->String(E_STR_ID_SEND_NAME));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_SEND_ADDR, 1, WND_TYPE_EDIT));
	pEdit->SetMaxStringLen(67);
	assert( pEdit );

	list.AddItem(ROW_SEND_PASS, 0, m_pStrTable->String(E_STR_ID_PASSWORD));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_SEND_PASS, 1, WND_TYPE_EDIT));
	assert( pEdit );
	pEdit->SetMaxStringLen(35);
	pEdit->SetPassword( true );

	char str[128]={0};
	for(int i = 0; i < MAX_MAIL_RECV_NUM; i++ )
	{
		snprintf(str, 128, "%s%d",m_pStrTable->String(E_STR_ID_RECV_EMAIL_ADDR),i+1);	
		list.AddItem(ROW_RECV_ADDR+i, 0, str);
		pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_RECV_ADDR+i, 1, WND_TYPE_EDIT));
		pEdit->SetMaxStringLen(67);
		assert( pEdit );
	}

	pButton = dynamic_cast<GUI::CButton*>(list.AddItem(ROW_TEST, 1, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]));
	assert(pButton);
	pButton->SetCaption(m_pStrTable->String( E_STR_ID_TEST ), false);

	list.AddItem(ROW_ADV, 0, m_pStrTable->String(E_STR_ID_ADVANCED_CFG));
	
	list.AddItem(ROW_IMG_CHECK, 0, m_pStrTable->String(E_STR_ID_IMG_NUM));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_IMG_CHECK, 1, WND_TYPE_CHECK));
	assert(pCheck);

	//list.AddItem(ROW_SNAP_TIME, 0, m_pStrTable->String(E_STR_ID_SNAP_INTERVAL));
	//pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_SNAP_TIME, 1, WND_TYPE_COMBO));
	//assert(pCombo);


	list.CreateListFocusMap();

}


void CCfgNetEmailDlg::DoOther()
{
	/*BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;
	int count = 0;
	GUI::CComboCtrl* pCombo = NULL;
	unsigned long *pTempValue = NULL;
	GUI::CListCtrl& list = m_cfgEmailList;
	char str[128] = {0};

	CCfgDlg::GetConfig( NCFG_ITEM_EMAIL_IMG_NUM_RANGE, &pTempData, tempDataLen);
	assert(tempDataLen > 0);
	count = tempDataLen/sizeof(unsigned long);	
	pTempValue = reinterpret_cast<unsigned long *>(pTempData);
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_IMG_NUM, 1));
	assert(pCombo);
	pCombo->RemoveAllItem(false);			
	for (int i = 0; i < count; ++i)
	{
		if (0 == pTempValue[i])
		{
			snprintf(str, 128, "%s", m_pStrTable->String(E_STR_ID_IMG_NONE));
		}
		else
		{
			snprintf(str, 128, "%d", pTempValue[i]);
		}
		pCombo->AddItem(str, pTempValue[i]);					
	}
	pCombo->SetCurItem(0, false);*/


	//CCfgDlg::GetConfig( NCFG_ITEM_EMAIL_SNAP_TIME_RANGE, &pTempData, tempDataLen);
	//assert(tempDataLen > 0);
	//count = tempDataLen/sizeof(unsigned long);
	//pTempValue = reinterpret_cast<unsigned long *>(pTempData);	
	//pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_SNAP_TIME, 1));
	//assert(pCombo);
	//pCombo->RemoveAllItem(false);			
	//for (int i = 0; i < count; ++i)
	//{
	//	sprintf(str, "%d", pTempValue[i]);
	//	pCombo->AddItem(str, pTempValue[i]);					
	//}
	//pCombo->SetCurItem(0, false);
}

void CCfgNetEmailDlg::EndOther()
{
	
}

bool CCfgNetEmailDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	
	switch (cfgID)
	{
	case NCFG_ITEM_EMAIL_SEND_INFO:	
		{
			assert(len == sizeof(MAIL_SEND_INFO));
			const MAIL_SEND_INFO *pValue = reinterpret_cast<const MAIL_SEND_INFO*>(pData);

			GUI::CCheckCtrl* pCheck = NULL;
			GUI::CEditCtrl * pEdit= NULL;
			GUI::CNumEditCtrl* pNumPort = NULL;

			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgEmailList.GetItemWnd(ROW_SMTP_NAME,1));
			assert( NULL != pEdit );
			
			pEdit->SetCaption( pValue->server, bRefresh );

			pNumPort = dynamic_cast<GUI::CNumEditCtrl*>(m_cfgEmailList.GetItemWnd(ROW_SMTP_PORT,1));
			assert(NULL!=pNumPort);
			pNumPort->SetValue( pValue->port, bRefresh );
			
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgEmailList.GetItemWnd(ROW_SLL_CHECK,1));
			assert(NULL!= pCheck);
			pCheck->SetCheck(pValue->bSSL,bRefresh);

			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgEmailList.GetItemWnd(ROW_SEND_ADDR,1));
			assert( NULL != pEdit );
			pEdit->SetCaption( pValue->name, bRefresh );
			
			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgEmailList.GetItemWnd(ROW_SEND_PASS,1));
			assert( NULL != pEdit );
			pEdit->SetCaption( pValue->passwd, bRefresh );
			
		}
		return true;
		break;
	case NCFG_ITEM_EMAIL_RECV_INFO:
		{	
			assert(len == (sizeof(MAIL_RECV_INFO)*MAX_MAIL_RECV_NUM));
			const MAIL_RECV_INFO *pValue = reinterpret_cast<const MAIL_RECV_INFO*>(pData);
			
			GUI::CEditCtrl * pEdit= NULL;
			for (int i = 0; i < MAX_MAIL_RECV_NUM; ++i)
			{
				pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgEmailList.GetItemWnd(ROW_RECV_ADDR+i,1));
				assert(NULL != pEdit);
				pEdit->SetCaption( pValue[i].recvname, bRefresh );
			}			
		}
		return true;
		break;
	case NCFG_ITEM_EMAIL_PIC_ACC:
		{
			assert(len == sizeof(unsigned long ));
			const unsigned long *pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CCheckCtrl *pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)m_cfgEmailList.GetItemWnd(ROW_IMG_CHECK, 1));
			if (pValue[0] > 0)
			{
				pCheck->SetCheck(true, bRefresh);
			}
			else
			{
				pCheck->SetCheck(false, bRefresh);
			}			
		}
		return true;
		break;
	/*case NCFG_ITEM_JPEG_PERIOD:
		{
			assert(len == (sizeof(unsigned long )* (CProduct::Instance()->LocalVideoInputNum())));
			const unsigned long *pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CComboCtrl *pComb = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)m_cfgEmailList.GetItemWnd(ROW_SNAP_TIME, 1));
			pComb->SetCurItemData(pValue[0], false);
		}
		return true;
		break;*/
	default:
		return false;
		break;
	}
	return false;
}

bool CCfgNetEmailDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_EMAIL_SEND_INFO:
		{			
			assert(len == (sizeof(MAIL_SEND_INFO)));
			MAIL_SEND_INFO *pValue = reinterpret_cast<MAIL_SEND_INFO*>(pData);

			GUI::CCheckCtrl* pCheck = NULL;
			GUI::CEditCtrl * pEdit= NULL;
			GUI::CNumEditCtrl* pNumPort = NULL;

			pEdit = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgEmailList.GetItemWnd(ROW_SMTP_NAME,1));
			assert( NULL != pEdit );
			strcpy(pValue->server,pEdit->GetCaption().c_str());
		
			pNumPort = dynamic_cast<GUI::CNumEditCtrl*>((GUI::CNumEditCtrl*)m_cfgEmailList.GetItemWnd(ROW_SMTP_PORT,1));
			assert(pNumPort); 	
			pValue->port = pNumPort->GetValue();

			pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)m_cfgEmailList.GetItemWnd(ROW_SLL_CHECK,1));
			assert(NULL!= pCheck);
			pValue->bSSL = pCheck->IsCheck()?1:0;

			pEdit = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgEmailList.GetItemWnd(ROW_SEND_ADDR,1));
			assert( NULL != pEdit );
			strcpy(pValue->name,pEdit->GetCaption().c_str());

			pEdit = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgEmailList.GetItemWnd(ROW_SEND_PASS,1));
			assert( NULL != pEdit );
			strcpy(pValue->passwd, pEdit->GetCaption().c_str());
		}
		return true;
		break;
	case NCFG_ITEM_EMAIL_RECV_INFO:
		{
			assert(len == (sizeof(MAIL_RECV_INFO)*MAX_MAIL_RECV_NUM));
			MAIL_RECV_INFO *pValue = reinterpret_cast<MAIL_RECV_INFO*>(pData);

			GUI::CEditCtrl * pEdit= NULL;
			for (int i = 0; i < MAX_MAIL_RECV_NUM; ++i)
			{
				pEdit = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgEmailList.GetItemWnd(ROW_RECV_ADDR + i,1));
				assert(pEdit!=NULL);
				strcpy(pValue[i].recvname, pEdit->GetCaption().c_str());
			}
		}
		return true;
		break;	
	case NCFG_ITEM_EMAIL_PIC_ACC:
		{
			assert(len == sizeof(unsigned long));
			unsigned long *pValue = reinterpret_cast<unsigned long*>(pData);
			GUI::CCheckCtrl * pCheck = NULL;
			unsigned long timeTemp = 0;
			pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)m_cfgEmailList.GetItemWnd(ROW_IMG_CHECK ,1));
			if (pCheck->IsCheck())
			{
				timeTemp = 1;
			}
			else
			{
				timeTemp = 0;
			}			 
			*pValue = timeTemp;
		}
		return true;
		break;
	/*case NCFG_ITEM_JPEG_PERIOD:
		{
			assert(len == (sizeof(unsigned long)*(CProduct::Instance()->LocalVideoInputNum())));
			unsigned long *pValue = reinterpret_cast<unsigned long*>(pData);
			GUI::CComboCtrl * pComb= NULL;
			pComb = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)m_cfgEmailList.GetItemWnd(ROW_SNAP_TIME ,1));
			unsigned long timeTemp = pComb->GetCurItemData();
			for (int i = 0; i < CProduct::Instance()->LocalVideoInputNum(); ++i)
			{				
				pValue[i] = timeTemp;
			}
		}
		return true;
		break;*/
	default:
		return false;
		break;
	}
	return false;
}

void CCfgNetEmailDlg::SetErrorText(unsigned char checkResult)
{
	ENUM_STRING_ID strID = E_STR_ID_NULL;

	switch(checkResult)
	{
	case EMAIL_TEST_SUCC:
		strID = E_STR_ID_MESSAGE_116;
		break;
	case EMAIL_TEST_FAIL: 
		strID = E_STR_ID_MESSAGE_117;
		break;
	default:
		strID = E_STR_ID_MESSAGE_118;		//未知错误
		break;
	}
	MessageBox(m_pStrTable->String(strID), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);

	MessageOut( GetTopestWnd(), strID, TIP_ERR );
}

void CCfgNetEmailDlg::TestEmailInfo(void)
{
	MESSAGE_DATA msgData;
	memset( &msgData, 0, sizeof( msgData ) );

	//重新获得当前的值
	unsigned long len = sizeof(MAIL_SEND_INFO)+(sizeof(MAIL_RECV_INFO)*MAX_MAIL_RECV_NUM);
	unsigned char *pdate = new unsigned char[len];
	memset(pdate, 0, sizeof(len));
	
	//发送部分信息
	MAIL_SEND_INFO sendInfo;
	memset(&sendInfo, 0, sizeof(sendInfo));
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CEditCtrl * pEdit= NULL;
	GUI::CNumEditCtrl* pNumPort = NULL;
	pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)m_cfgEmailList.GetItemWnd(ROW_SLL_CHECK,1));
	sendInfo.bSSL = pCheck->IsCheck()?1:0;
	pEdit = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgEmailList.GetItemWnd(ROW_SMTP_NAME,1));
	snprintf(sendInfo.server, 68, "%s",pEdit->GetCaption().c_str());
	pNumPort = dynamic_cast<GUI::CNumEditCtrl*>((GUI::CNumEditCtrl*)m_cfgEmailList.GetItemWnd(ROW_SMTP_PORT,1));
	sendInfo.port = pNumPort->GetValue();
	pEdit = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgEmailList.GetItemWnd(ROW_SEND_ADDR,1));
	snprintf(sendInfo.name, 68, "%s",pEdit->GetCaption().c_str());
	pEdit = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgEmailList.GetItemWnd(ROW_SEND_PASS,1));
	snprintf(sendInfo.passwd, 36, "%s", pEdit->GetCaption().c_str());
	memcpy(pdate, &sendInfo, sizeof(MAIL_SEND_INFO));
	
	//接收部分信息
	MAIL_RECV_INFO recvInfo[MAX_MAIL_RECV_NUM];
	memset(&recvInfo, 0, sizeof(recvInfo));
	for (int i = 0; i < MAX_MAIL_RECV_NUM; ++i)
	{
		pEdit = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgEmailList.GetItemWnd(ROW_RECV_ADDR + i,1));
		snprintf(recvInfo[i].recvname, 68, "%s", pEdit->GetCaption().c_str());
	}
	memcpy(pdate+sizeof(MAIL_SEND_INFO), recvInfo, sizeof(recvInfo));

	CMessageMan::Instance()->SendMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETPARAMCHECK_EMAIL, pdate , len, msgData);
	assert( msgData.pData );
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	unsigned char ret = 0;
	switch( static_cast<NET_PROTOCOL>(pCmd->cmdType) )
	{
	case CMD_REPLY_NETPARAMCHECK_EMAIL_SUCC:
		{
			ret = 1;
		}
		break;
	case CMD_REPLY_NETPARAMCHECK_EMAIL_FAIL:
		{
			ret = 2;
		}
		break;
	default:
			ret = 3;
			break;
	}

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	delete []pdate;
	pdate = NULL;	


	SetErrorText(ret);

	GetTopestWnd()->Repaint();


}

BEGIN_MSG_MAP(CCfgNetEmailDlg, CCfgDlg)
	ON_MSG_EX(m_cfgEmailList .GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgNetEmailDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{

	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	if (ROW_TEST == row)
	{
		TestEmailInfo();
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (!IsModify())
	{
		switch (row)
		{	
		case ROW_SMTP_NAME:
			if(IsChange(NCFG_ITEM_EMAIL_SEND_INFO))
			{
				SetModify(true);
			}
			break;
		case ROW_SMTP_PORT:
			if(IsChange(NCFG_ITEM_EMAIL_SEND_INFO))
			{
				SetModify(true);
			}
			break;
		case ROW_SLL_CHECK:
			if(IsChange(NCFG_ITEM_EMAIL_SEND_INFO))
			{
				SetModify(true);
			}
			break;
		case ROW_SEND_ADDR:
			if(IsChange(NCFG_ITEM_EMAIL_SEND_INFO))
			{
				SetModify(true);
			}
			break;
		case ROW_SEND_PASS:
			if(IsChange(NCFG_ITEM_EMAIL_SEND_INFO))
			{
				SetModify(true);
			}
			break;
		case ROW_IMG_CHECK:
			if(IsChange(NCFG_ITEM_EMAIL_PIC_ACC))
			{
				SetModify(true);
			}
			break;
		/*case ROW_SNAP_TIME:
			if(IsChange(NCFG_ITEM_JPEG_PERIOD))
			{
				SetModify(true);
			}
			break;	*/
		
		default:
			{
				if ((row >= ROW_RECV_ADDR) && (row < ROW_TEST))
				{
					if(IsChange(NCFG_ITEM_EMAIL_RECV_INFO))
					{
						SetModify(true);
					}
				}
				else
				{
					assert(false);
				}
			}			
			break;
		}
		return KEY_VIRTUAL_MSG_CONTINUE;
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
}


#ifdef __CUSTOM_US02_NEW__
void CCfgNetEmailDlg::SetTip()
{
	//MessageOut(GetTopestWnd(),m_pStrTable->String(E_STR_ID_NETWORK_EMAIL),TIP_MSG,true);
	TipAll(m_childList, E_STR_ID_NETWORK_EMAIL);
}

void CCfgNetEmailDlg::TipAll(WND_LIST child, ENUM_STRING_ID strId)
{
	if(child.empty()) return;

	for (WND_LIST::iterator it = child.begin(); it != child.end(); it++)
	{
		if(!(*it)->m_childList.empty())
		{
			TipAll((*it)->m_childList, strId);
		}
		if((*it)->GetParent() != this && (*it)->GetParent()->GetParent() != this) AddTip((*it)->GetID(), strId);
	}
}
#endif