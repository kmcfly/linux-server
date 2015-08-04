/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-24
** Name         : CfgDlg.cpp
** Version      : 1.0
** Description  : �������öԻ��� ��ȡ���������
** Modify Record:
1:
***********************************************************************/

#include "CfgDlg.h"
#include "MessageBox.h"
#include "CruiseOther.h"

NET_LOGIN_SUCCESS_REPLY CCfgDlg::s_loginSuccessReply;

CCfgDlg::CCfgDlg() : m_clientID(LOCAL_CLIENT_ID)
, m_deviceID(LOCAL_DEVICE_ID)
, m_bModify(false)
, m_pBtnOK(NULL)
, m_pCfgBlock(NULL)
, m_bCreateCfgBlock(false)
, m_pCfgParentDlg(NULL)
, m_bRefreshView(false)
{
	m_pBtnDefualt = NULL;
}

CCfgDlg::~CCfgDlg()
{
	
}

void CCfgDlg::OnDestroy()
{
	EndOtherProc();

	GUI::CDialog::OnDestroy();

	if (m_bCreateCfgBlock && (NULL != m_pCfgBlock))
	{
		m_pCfgBlock->DestroyBlock();
		delete m_pCfgBlock;
		m_pCfgBlock = NULL;
		m_bCreateCfgBlock = false;
	}
	
	m_bModify = false;
	m_pBtnOK = NULL;
	m_pBtnDefualt = NULL;
	m_pCfgParentDlg = NULL;

	m_cfgChildDlgList.clear();
	m_saveItemList.clear();
}

void CCfgDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();

	m_pCfgParentDlg = dynamic_cast<CCfgDlg*>(GetParent());
	CCfgDlg* pCfgDlg = NULL;
	m_cfgChildDlgList.clear();
	for (GUI::CWnd::WND_LIST::iterator iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		pCfgDlg = dynamic_cast<CCfgDlg*>(*iter);
		if (NULL != pCfgDlg)
		{
			m_cfgChildDlgList.push_back(pCfgDlg);
		}
	}

	m_pBtnOK = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_OK));
	if (NULL != m_pBtnOK)
	{
		m_pBtnOK->SetEnable(false, false);
	}
	
	if (DEFAULT_BTN_APPLY & m_defaultCtrlType)
	{
		m_pBtnDefualt = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
	}
	

	if (LoadConfig())
	{
		DoOtherProc();
		UpdateView(false);
		
	}
}

//unsigned long CCfgDlg::OnApply()
//{
//	//Apply��������Ϊ����Ĭ��ֵ
//	
//	return KEY_VIRTUAL_MSG_STOP;
//}

unsigned long CCfgDlg::OnOK()      //�м��OK����ΪӦ��
{
	//ɾ��Ѳ����
	CCruiseDelete * pTempCruise = CCruiseDelete::GetInstanceCruiseDel();
	unsigned int iDel =0;
	for (std::vector<CruiseDelData>::iterator iter = pTempCruise->m_CruiseDel.begin();
		iter != pTempCruise->m_CruiseDel.end(); iter++,iDel++ )
	{
		PTZ_CTRL ptzCtrl;
		ptzCtrl.chnn = iter->m_currchannel;
		ptzCtrl.cmdType = PTZ_CMD_CRUISE_DEL;
		ptzCtrl.speed = 0;

		//ptzCtrl.cmdValue = iter->m_deleteInfo;
		ptzCtrl.cmdValue = iDel;
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, 
			(unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
	}
	pTempCruise->m_CruiseDel.clear();

	if(IsModify())
	{
		unsigned long ret = TrySaveChange(this,false,false,true);
	}
	else
	{
		for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
		{
			if( (*cfg_iter)->IsModify() )
			{
				TrySaveChange(*cfg_iter,false,false,true);
				break;
			}
		}
	}
#ifdef __SUPPORT_3G__
	PostSave();
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgDlg::OnCancel()
{
	return GUI::CDialog::OnCancel();
}

void CCfgDlg::EndDialog(DLG_END_VALUE result)
{
	unsigned long ret = 0;

    CCruiseDelete * pTempCruise = CCruiseDelete::GetInstanceCruiseDel();
	if (pTempCruise->m_CruiseDel.size() > 0)
	{
		pTempCruise->m_CruiseDel.clear();
	}

 	if(IsModify())
	{
		ret = TrySaveChange(this,true,false,false);
	}
	else
	{
		for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
		{
			if( (*cfg_iter)->IsModify() )
			{
				ret = TrySaveChange(*cfg_iter,true,false,false);
				break;	//ֻ��һ��ѡ�񼴿�
			}
		}
	}
#ifdef __SUPPORT_3G__
	PostSave();
#endif

	if( ret == 0 )
	{
		GUI::CDialog::EndDialog( result );
	}
}

void CCfgDlg::SetSubCfgDlg(CCfgDlg* pCfgDlg, bool bRefresh)
{
	m_cfgChildDlgList.push_back(pCfgDlg);
	pCfgDlg->SetConfigBlock(m_pCfgBlock);
	pCfgDlg->DoOther();

	//ˢ�µ�ǰҳ������
	std::list<NCFG_ITEM_HEAD_EX> cfgItemList = m_pCfgBlock->GetConfigItemList();
	const BYTE* pData = m_pCfgBlock->Data();
	const BYTE* pItemData = NULL;
	int itemLen = 0;
	for(std::list<NCFG_ITEM_HEAD_EX>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
	{
		itemLen = iter->item_head.len;
		if (itemLen > 0)
		{
			pItemData = pData + iter->start_pos;
		} 
		else
		{
			pItemData = NULL;
		}

		//ʵ����ˢ������ʱ������Ҫ��ʱˢ�¡���������ʹ�����л�������ʾһҳ�����˶�γ�ˢ��
		pCfgDlg->ShowDataProc(static_cast<NCFG_ITEM_ID>(iter->item_head.itemID), pItemData, itemLen, false);
	}

	//���´��������ͼ
	CreateFocusMap();

	if(bRefresh)
	{
		Repaint();
	}
}

bool CCfgDlg::LoadConfig()
{
	int len = 0;
	BYTE * pData = GetCfgItemHeadBuffer(&len);
	if (NULL != pData)
	{
		m_bCreateCfgBlock = true;
		if (NULL != m_pCfgBlock)
		{
			m_pCfgBlock->DestroyBlock();
			delete m_pCfgBlock;
			m_pCfgBlock = NULL;
		}
		m_pCfgBlock = new CConfigBlock;
		//////////////////////////////////////////////////////////////////////////
		assert(len > 0);
		CMessageMan* pMsgMan = CMessageMan::Instance();
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));

		pMsgMan->SendMsgDataToServer(m_clientID, m_deviceID, CMD_REQUEST_CFG_GET, pData, len, msgData);
		delete [] pData;
		pData = NULL;

		assert(NULL != msgData.pData);
		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		if (CMD_REPLY_CFG_DATA == static_cast<NET_PROTOCOL>(pCmd->cmdType))
		{
			m_pCfgBlock->DestroyBlock();
			m_pCfgBlock->CreateBlock( reinterpret_cast<BYTE*>(msgData.pData) + sizeof(PACKCMD), pCmd->dataLen );

			pMsgMan->ReleaseMsgBuff(msgData);

			//���������ô���
			for (CFGDLG_LIST::iterator iter = m_cfgChildDlgList.begin(); iter != m_cfgChildDlgList.end(); ++iter)
			{
				(*iter)->SetConfigBlock(m_pCfgBlock);
			}
			return true;
		}
		else
		{
			assert(false);
			pMsgMan->ReleaseMsgBuff(msgData);
			return false;
		}
	} 
	else
	{
		return false;
	}
}

bool CCfgDlg::LoadDefault(CConfigBlock** pCfgBlock, unsigned long* pError)
{
	int len = 0;
	BYTE * pData = GetCfgItemHeadBuffer(&len);
	if (NULL != pData)
	{
		*pCfgBlock = new CConfigBlock;
		//////////////////////////////////////////////////////////////////////////
		assert(len > 0);
		CMessageMan* pMsgMan = CMessageMan::Instance();

		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		pMsgMan->SendMsgDataToServer(m_clientID, m_deviceID, CMD_REQUEST_CFG_DEF_DATA, pData, len, msgData);
		delete [] pData;
		pData = NULL;

		assert(NULL != msgData.pData);
		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		if (CMD_REPLY_CFG_DATA == static_cast<NET_PROTOCOL>(pCmd->cmdType))
		{
			(*pCfgBlock)->DestroyBlock();
			(*pCfgBlock)->CreateBlock( reinterpret_cast<BYTE*>(msgData.pData) + sizeof(PACKCMD), pCmd->dataLen );

			pMsgMan->ReleaseMsgBuff(msgData);

			return true;
		}
		else
		{
			gui_trace_np(true,printf("Load default config failed.\n"));
			//ɾ����ʱBlock
			(*pCfgBlock)->DestroyBlock();
			delete *pCfgBlock;
			*pCfgBlock = NULL;
			pMsgMan->ReleaseMsgBuff(msgData);
			return false;
		}
	} 
	else
	{
		return false;
	}
}

bool CCfgDlg::SaveConfig()
{
	if (m_bCreateCfgBlock && (NULL != m_pCfgBlock))
	{
#if 0	//ȫ������
		std::list<NCFG_ITEM_HEAD_EX> cfgItemList = m_pCfgBlock->GetConfigItemList();
		int header_len = sizeof(NCFG_BLOCK_HEAD) + cfgItemList.size() * sizeof(NCFG_ITEM_HEAD);
		return SaveConfig(cfgItemList, m_pCfgBlock->Data(), m_pCfgBlock->DataLen() - header_len);
#else	//ֻ����ı��
		if (!m_saveItemList.empty())
		{
			const BYTE* pConfigData = m_pCfgBlock->Data();
			assert(NULL != pConfigData);
			BYTE* pSaveData = new BYTE[m_pCfgBlock->DataLen()];
			int save_len = 0;
			for (std::list<NCFG_ITEM_HEAD_EX>::const_iterator iter = m_saveItemList.begin(); iter != m_saveItemList.end(); ++iter)
			{
				gui_trace_np(true,printf("Save config.item=0x%X.=%d\n",iter->item_head.itemID,iter->item_head.itemID));
				memcpy(pSaveData + save_len, pConfigData + iter->start_pos, iter->item_head.len);
				save_len += iter->item_head.len;
			}
			bool ret = SaveConfig(m_saveItemList, pSaveData, save_len);
			delete [] pSaveData;

			return ret;
		}	

#endif
		return true;
	}
	else
	{
		return false;
	}
}

//void CCfgDlg::DoOk( bool bRefresh )
//{
//	if (m_bCreateCfgBlock)
//	{
//		UpdateConfig();
//		SaveConfig();
//	}
//
//	EnableOkBtn(false, false);
//
//	for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
//	{
//		(*cfg_iter)->SetModify(false);
//	}
//
//	if( bRefresh && IsRefreshView() )
//	{
//		UpdateView( false );
//	}
//
//	//������ӵ��Ok��ť��Ӧ���Ƕ��㴰��,���������
//	MessageOut(E_STR_ID_TIP_00_01,TIP_MSG,false);
//
//	if( bRefresh )
//	{
//		Repaint();
//	}
//}

bool CCfgDlg::SaveConfig(const std::list<NCFG_ITEM_HEAD_EX>& cfgItemList, const BYTE* pConfigBuffer, int len)
{
	assert(NULL != pConfigBuffer);
	assert(!cfgItemList.empty());

	/*
		len���Ե���0,��ʾ��Ҫ�������Ŀֻ��ͷ����û����������
		���ײ���Ҫע��䳤������Ϊ�յ����
	*/

	NCFG_BLOCK_HEAD blockHead;
	blockHead.netcfgver = NCFG_VERSION;
	blockHead.biSize = sizeof(NCFG_BLOCK_HEAD);
	blockHead.ItemNum = cfgItemList.size();

	int header_len = blockHead.biSize + blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD);

	int setDataLen = header_len + len;
	unsigned char* pSetData = new unsigned char[setDataLen];
	memcpy( pSetData, &blockHead, sizeof(NCFG_BLOCK_HEAD) );
	NCFG_ITEM_HEAD* pItemHeader = reinterpret_cast<NCFG_ITEM_HEAD*>(pSetData + sizeof(NCFG_BLOCK_HEAD));

	for(std::list<NCFG_ITEM_HEAD_EX>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
	{
		*pItemHeader = iter->item_head;
		++pItemHeader;
	}
	
	if(len > 0)
	{
		memcpy( pItemHeader, pConfigBuffer, len);
	}

	//CConfigEx::Instance()->SetConfig( pSetData, setDataLen);
	bool ret = SaveConfig(pSetData, setDataLen);

	delete [] pSetData;
	return ret;
}

bool CCfgDlg::SaveConfig(const BYTE* pData, int len)
{
	CMessageMan* pMsgMan = CMessageMan::Instance();
	
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	pMsgMan->SendMsgDataToServer(m_clientID, m_deviceID, CMD_REQUEST_CFG_SET, const_cast<BYTE*>(pData), len, msgData);

	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{	
		pMsgMan->ReleaseMsgBuff(msgData);
		return true;
	}
	else
	{
		assert(false);
		pMsgMan->ReleaseMsgBuff(msgData);
		return false;
	}

	return true;
}

BYTE* CCfgDlg::GetCfgItemHeadBuffer(int* pLen) const
{
	CFGITEM_LIST cfgItemList;
	GetItemList(cfgItemList);

	int count = cfgItemList.size();
	if (count > 0)
	{
		int len = sizeof(NCFG_ITEM_HEAD) * count;
		BYTE* pOutBuffer = new BYTE[len];
		NCFG_ITEM_HEAD  *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pOutBuffer);

		int  offset = 0;
		for (CFGITEM_LIST::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
		{
			pItemHead->itemID = (*iter);
			pItemHead++;
		}

		if (NULL != pLen)
		{
			*pLen = len;
		}
		return pOutBuffer;
	} 
	else
	{
		if (NULL != pLen)
		{
			*pLen = 0;
		}
		return NULL;
	}
}

void CCfgDlg::UpdateView(bool bRefresh)
{
	assert(NULL != m_pCfgBlock);

	std::list<NCFG_ITEM_HEAD_EX> cfgItemList = m_pCfgBlock->GetConfigItemList();
	const BYTE* pData = m_pCfgBlock->Data();
	//printf("UI Get Config Len:%d\n", m_pCfgBlock->DataLen());
	const BYTE* pItemData = NULL;
	int itemLen = 0;
	for(std::list<NCFG_ITEM_HEAD_EX>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
	{
		itemLen = iter->item_head.len;
		if (itemLen > 0)
		{
			pItemData = pData + iter->start_pos;
		} 
		else
		{
			pItemData = NULL;
		}
		
		//ʵ����ˢ������ʱ������Ҫ��ʱˢ�¡���������ʹ�����л�������ʾһҳ�����˶�γ�ˢ��
		ShowDataProc(static_cast<NCFG_ITEM_ID>(iter->item_head.itemID), pItemData, itemLen, false);		
	}

	m_bModify = false;
	//EnableOkBtn(false, bRefresh);
}

void CCfgDlg::RefreshChangeItem(bool refreshBlock/*=true*/, CCfgDlg *pCfgDlg /*= NULL*/)
{
	/*
		���̷�������. 1.ֻ���²�������, 2.���²��������Block�е�����
	���������Block�е�����,���������ͱ��ٱ�����������.
	*/

	assert(NULL != m_pCfgBlock);
	if (NULL != m_pCfgBlock)
	{
		std::list<NCFG_ITEM_HEAD_EX>& cfgItemList = m_pCfgBlock->GetConfigItemList();
		BYTE* pData = m_pCfgBlock->Data();
		
		int len = 0;
		BYTE* pItemData = NULL; 
		bool bChange = false;

		//����ղ�������
		m_saveItemList.clear();
		for (std::list<NCFG_ITEM_HEAD_EX>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
		{
			if (iter->item_head.len > 0)
			{
				len = iter->item_head.len;
				pItemData = new BYTE[len];
				memcpy(pItemData, pData + iter->start_pos, len); //��Ҫ����ԭ����.ԭ��:�޸ĵ����ݿ���ֻ��ԭ���������һ��С����.ĳ��SetDataProc���̿���ֻ��עĳ��Item�Ĳ���.
				SetDataProc(static_cast<NCFG_ITEM_ID>(iter->item_head.itemID), pItemData, len);

				if (iter->item_head.len != len)
				{
					bChange = true;
				} 
				else if (0 != memcmp(pData + iter->start_pos, pItemData, len))
				{
					bChange = true;
				}
				else
				{
					bChange = false;
				}
			}
			else
			{
				pItemData = NULL;
				len = 0;
				SetDataProc(static_cast<NCFG_ITEM_ID>(iter->item_head.itemID), pItemData, len);
				bChange = (len > 0) ? true : false;
			}
		
			//////////////////////////////////////////////////////////////////////////
			//if (NULL != pItemData && refreshBlock)
			if ( bChange && refreshBlock)
			{

				if( ( static_cast< NCFG_ITEM_ID >( iter->item_head.itemID ) >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE) && \
					( static_cast< NCFG_ITEM_ID >( iter->item_head.itemID ) < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND )  )
				{
					//Ѳ���ߵ����ݳ����ǿɱ�,�ڱ���ʱҪ�ӽ�����Ѳ����Ϣ
					unsigned short subItemCount;
					unsigned short subItemLen;
					pCfgDlg->GetSubItemInfoFromDlg( static_cast< const NCFG_ITEM_ID>( iter->item_head.itemID ), subItemCount, subItemLen  );
					SetSubItemInfo( static_cast< const NCFG_ITEM_ID>( iter->item_head.itemID ), subItemCount, subItemLen );

				}

				if (iter->item_head.len == len)
				{
					m_pCfgBlock->SetConfig(static_cast<NCFG_ITEM_ID>(iter->item_head.itemID), pItemData, len);
				} 
				else
				{
					m_pCfgBlock->SetConfigEx(static_cast<NCFG_ITEM_ID>(iter->item_head.itemID), pItemData, len, iter->item_head.len);
					pData = m_pCfgBlock->Data();//���ȸı䣬���»�ȡ����
				}
			}
			//////////////////////////////////////////////////////////////////////////
			if (bChange)
			{
				m_saveItemList.push_back(*iter);
			}
			
			if (NULL != pItemData)
			{
				delete [] pItemData;
				pItemData = NULL;
			}
		}

		if( refreshBlock )
		{
			m_bModify = false;
		}
	}	
}

bool CCfgDlg::ShowDataProc(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	//printf("CCfgDlg::ShowDataProc ID:%d Len:%d\n", cfgID, len);
	if (!ShowData(cfgID, pData, len, bRefresh))
	{
		for (CFGDLG_LIST::iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
		{
			if ((*cfg_iter)->ShowData(cfgID, pData, len, bRefresh))
			{
				return true;
			}
		}
		return false;
	}

	return true;
}

bool CCfgDlg::SetDataProc(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	if (!SetData(cfgID, pData, len))
	{
		for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
		{
			if ((*cfg_iter)->SetData(cfgID, pData, len))
			{
				return true;
			}
		}
		return false;
	} 
	return true;
}

//��һЩ��Ҫ�ڶ�ȡ���ú����Ķ���
void CCfgDlg::DoOtherProc()
{
	for (CFGDLG_LIST::iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
	{
		(*cfg_iter)->DoOther();
	}
}

//��һЩDoOther��������
void CCfgDlg::EndOtherProc()
{
	for (CFGDLG_LIST::iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
	{
		(*cfg_iter)->EndOther();
	}
}
void CCfgDlg::GetSubItemInfoFromDlg( NCFG_ITEM_ID itemID, unsigned short& subItemCount, unsigned short& subItemLen)
{
	assert(NULL != m_pCfgBlock);
	std::list<NCFG_ITEM_HEAD_EX>& cfgItemList = m_pCfgBlock->GetConfigItemList();
	for (std::list<NCFG_ITEM_HEAD_EX>::iterator it = cfgItemList.begin(); it != cfgItemList.end(); ++it)
	{
		if (it->item_head.itemID == itemID)
		{
			subItemCount	= it->item_head.num;
			subItemLen		= it->item_head.subLen;

			return;
		}
	}
	
	assert( false );
	
}

//////////////////////////////////////////////////////////////////////////
unsigned long CCfgDlg::TrySaveChange(CCfgDlg* pCfgDlg,bool bChooseDlg/*=true*/,bool bHide,bool bRefresh/*=true*/)
{
	gui_assert(pCfgDlg->IsModify(),printf("Make sure,the parameter pCfgDlg is modified.\n"));

	//////////ˢ�²�������,������Ƿ������б䶯
	RefreshChangeItem(false);

	if( m_saveItemList.size() <= 0 )
	{
		EnableOkBtn(false, true);
		for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
		{
			(*cfg_iter)->SetModify(false);
		}
		//char str[64];
		//strcpy(str,"Ok.No setting is changed.");
		MessageOut(GetTopestWnd(),m_pStrTable->String(E_STR_ID_TIP_00_05),TIP_MSG,false);

		//[2010-03-02 17:49:00 L] ���Ա��������, ��ʹû�б䶯Ҳ��Ҫ����ˢ��ҳ������
		pCfgDlg->UpdateView( false );

		if( !bHide && bRefresh )
		{
			Repaint();
		}

		return 0; //���û�б䶯,����ǰ�˳�
	}

	//////////���Ա�������
	
	unsigned long retValue = 0;
	//bool reset = false;
	DLG_END_VALUE retDlg = DLG_OK;
	unsigned long retKey = pCfgDlg->PreCheckValue();//�˴������������Ҫ��ʾ���,��������и�������,�����޸Ĵ˴�
	if( bChooseDlg || 0 != retKey )
	{
		//��Ҫ��ʾ,ǿ���Ե���
		std::string str;

		if( retKey != 0 )
		{
			str += m_pStrTable->String(static_cast<ENUM_STRING_ID>(retKey));
			str += "\\n";
		}

		if( bChooseDlg )
		{
			str += m_pStrTable->String(E_STR_ID_MESSAGE_38);
		}

		GUI::CMessageBox box(str.c_str(), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_CANCEL|DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		//if (bHide)
		//{
			//Show(false,true);
		//}
		retDlg = box.DoModal( this);
	}

	if ( DLG_OK == retDlg )
	{
		bool msgOut = true;
		unsigned long ret = pCfgDlg->CheckValue();
		
		if( 0 == ret )
		{
			//�������
			if (m_bCreateCfgBlock)
			{
				RefreshChangeItem(true, pCfgDlg);
				SaveConfig();
			}
			else
			{
				gui_assert(false,printf("Please create config block first!\n"));
			}

			//ȷ�ϱ���,�ұ���ɹ�,�޸�ҳ���״̬
			EnableOkBtn(false, true);
			for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
			{
				(*cfg_iter)->SetModify(false);
			}
			//if( IsRefreshView() )
			{
				pCfgDlg->UpdateView( false );
			}

			retValue = ret;
		}
		else
		{
			if( bChooseDlg )
			{
				std::string str = m_pStrTable->String(static_cast<ENUM_STRING_ID>(ret));
				str += m_pStrTable->String(E_STR_ID_TIP_00_06);
				GUI::CMessageBox box(str.c_str(), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_CANCEL|DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
				//if (bHide)
				//{
				//	Show(false,true);
				//}

				if( DLG_OK == box.DoModal( this) )
				{
					retValue = ret;
				}
				else
				{
					EnableOkBtn(false, true);
					for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
					{
						(*cfg_iter)->SetModify(false);
					}
					//if( IsRefreshView() )
					{
						pCfgDlg->UpdateView( false );
					}

					msgOut = false;
					retValue = 0;
				}
			}
			else
			{
				retValue = ret;
			}
		}

		//��ȷ�ϱ����.���ݷ���ֵ������ʾ
		if( msgOut )
		{
			if(  ret != 0 )
			{
				MessageOut(GetTopestWnd(),static_cast<ENUM_STRING_ID>(ret),TIP_MSG,false);
			}
			else
			{
				MessageOut(GetTopestWnd(),E_STR_ID_TIP_00_01,TIP_MSG,false);
			}
		}
	}
	else
	{
		//ȡ�������,�޸�ҳ���״̬
		EnableOkBtn(false, true);
		for (CFGDLG_LIST::const_iterator cfg_iter = m_cfgChildDlgList.begin(); cfg_iter != m_cfgChildDlgList.end(); ++cfg_iter)
		{
			(*cfg_iter)->SetModify(false);
		}
		//if( IsRefreshView() )
		{
			pCfgDlg->UpdateView( false );
		}

		//�ָ�������,����ֱ�Ӷ�ȡ�����ϵ�ֵ[��һ���Ѿ���ҳ��ֵ�ָ���]���лָ�
		unsigned long rec = pCfgDlg->RecoverChange();

		retValue = 0;
	}

	if( !bHide && bRefresh )
	{
		Repaint();
	}

	return retValue;
}

bool CCfgDlg::IsChange(NCFG_ITEM_ID cfgID) const
{
	assert(NULL != m_pCfgBlock);

	const std::list<NCFG_ITEM_HEAD_EX>& cfgItemList = m_pCfgBlock->GetConfigItemList();
	const BYTE* pData = m_pCfgBlock->Data();

	for (std::list<NCFG_ITEM_HEAD_EX>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
	{
		if (cfgID == iter->item_head.itemID)
		{
			//assert(iter->item_head.len > 0);
			int len = 0;
			BYTE* pNewData = NULL;
			if (iter->item_head.len > 0)
			{
				len = iter->item_head.len;
				pNewData = new BYTE[len];
				memcpy(pNewData, pData + iter->start_pos, len);

				if(SetDataProc(cfgID, pNewData, len))
				{
					if ((iter->item_head.len == len) && (0 == memcmp(pNewData, pData + iter->start_pos, iter->item_head.len)))
					{
						delete [] pNewData;
						return false;
					} 
					else
					{
						delete [] pNewData;
						return true;
					}
				}
				else
				{
					//	assert(false);
					delete [] pNewData;
					return false;
				}
			}			
			else
			{
				if((SetDataProc(cfgID, pNewData, len) && (len > 0)))
				{
					delete [] pNewData;
					return true;
				}
				else
				{
					if (NULL != pNewData)
					{
						delete [] pNewData;
					}					
					return false;
				}
			}
		}
	}

	return false;
}

void CCfgDlg::EnableOkBtn(bool bEnable, bool bRefresh)
{
	if (NULL != m_pBtnOK)
	{
		m_pBtnOK->SetEnable(bEnable, bRefresh);
	}
}

bool CCfgDlg::GetConfig(NCFG_ITEM_ID itemID, unsigned char **ppData, unsigned long &dataLen) const
{
	if (NULL != m_pCfgBlock)
	{
		return m_pCfgBlock->GetConfig(itemID, ppData, dataLen);
	} 
	else
	{
		assert(false);
		return false;
	}
}

bool CCfgDlg::SetSubItemInfo(NCFG_ITEM_ID itemID, unsigned short sub_count, unsigned short sub_len)
{
	assert(NULL != m_pCfgBlock);
	std::list<NCFG_ITEM_HEAD_EX>& cfgItemList = m_pCfgBlock->GetConfigItemList();
	for (std::list<NCFG_ITEM_HEAD_EX>::iterator it = cfgItemList.begin(); it != cfgItemList.end(); ++it)
	{
		if (it->item_head.itemID == itemID)
		{
			it->item_head.num = sub_count;
			it->item_head.subLen = sub_len;
			
			return true;
		}
	}
	
	assert(false);
	return false;
}

bool CCfgDlg::GetSubItemInfo(NCFG_ITEM_ID itemID, unsigned short& sub_count, unsigned short& sub_len) const
{
	assert(NULL != m_pCfgBlock);
	std::list<NCFG_ITEM_HEAD_EX>& cfgItemList = m_pCfgBlock->GetConfigItemList();
	for (std::list<NCFG_ITEM_HEAD_EX>::const_iterator it = cfgItemList.begin(); it != cfgItemList.end(); ++it)
	{
		if (it->item_head.itemID == itemID)
		{
			sub_count = it->item_head.num;
			sub_len = it->item_head.subLen;

			return true;
		}
	}

	assert(false);
	return false;
}

unsigned long CCfgDlg::OnLoadDefault(CCfgDlg* pCfgDlg)
{
	/*
		��ʱ��������Block,����ȡȫ��Ĭ������;ֻ�Ե�ǰҳ������ֵ
	*/
	CConfigBlock* pCfgBlock = NULL;
	unsigned long error=0;
	if( LoadDefault(&pCfgBlock,&error) && pCfgBlock!=NULL )
	{
		std::list<NCFG_ITEM_HEAD_EX> cfgItemList = pCfgBlock->GetConfigItemList();
		const BYTE* pData = pCfgBlock->Data();
		const BYTE* pItemData = NULL;
		int itemLen = 0;
		int effect = 0;
		for(std::list<NCFG_ITEM_HEAD_EX>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
		{
			itemLen = iter->item_head.len;
			if (itemLen > 0)
			{
				pItemData = pData + iter->start_pos;
			} 
			else
			{
				pItemData = NULL;
			}

			//ʵ����ˢ������ʱ������Ҫ��ʱˢ�¡���������ʹ�����л�������ʾһҳ�����˶�γ�ˢ��
			if( pCfgDlg->ShowDataProc(static_cast<NCFG_ITEM_ID>(iter->item_head.itemID), pItemData, itemLen, false) )
			{
				effect += 1;
			}
		}

		pCfgDlg->SetModify(true);

		gui_trace_np(true,printf("On load default ok.data len=%d,items=%d,effect=%d\n",pCfgBlock->DataLen(),cfgItemList.size(),effect));

		//���� block
		pCfgBlock->DestroyBlock();
		delete pCfgBlock;

		//�ָ��޸��趨
		unsigned long ret = pCfgDlg->RecoverChange();
		gui_trace_np(ret!=0,printf("recover changer may be error!\n"));

		return 0;
	}
	else
	{
		return E_STR_ID_TIP_00_04;
		//���errorֵ,����Ӧ����ʾ
		//return ..error;
	}
}
