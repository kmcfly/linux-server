/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-24
** Name         : CfgDlg.h
** Version      : 1.0
** Description  : �������öԻ��� ��ȡ������
** Modify Record:
1:
** --------------------------------------------------------------------------------
** ˵��
** --------------------------------------------------------------------------------
1�������������ͶԻ��򶼼̳��ڱ��Ի���
2���ɱ�����������������ݵ�����ͽ��գ�ͨ�����緽ʽ����
3����Tabҳ��ĶԻ���ֻ�ж�������ӵ����������
4�����ݵı�������¼����֣�a.)��������Ƿ��޸ģ�������
Ӧ�ı�ǣ�b.)�˳����л�ҳ����ʾ�û��������� c.)�����ݱ���
��ӵ���������ݵĶԻ����d.)ͨ����������ķ�ʽ�������ݵ������
5��GetItemList���ñ�ϵ��ҳ����Ҫ��������������ID
6������������ID�봰�ڿؼ�һһ��Ӧ��ShowData������ˢ��
�����ڿؼ���SetData�����ݱ�����Block��
7�����ҳ���з����������ݶ�Ӧ�Ŀؼ���Ҫˢ�£��̳�UpdateView
����д��Ӧ��ˢ�´���
8��CheckValue�������ҳ������Ԫ���Ƿ�Ϸ�.
9��PreCheckValue�����������Ŀ��Ҫ�׳���������ʾ.
10��RecoverChange�ָ��������趨�ı�.��ҳ���в���ĳЩ��Ҫ��
�˵ò���ʱ,�ڴ˴��ָ�.
***********************************************************************/

#ifndef _CFG_DLG_H_
#define _CFG_DLG_H_

#include "MessageMan.h"

#include "Dialog.h"
#include "ConfigEx.h"
#include "ConfigBlock.h"
#include <vector>

using namespace GUI;

class CCfgDlg : public GUI::CDialog
{
public:
	CCfgDlg();
	virtual ~CCfgDlg();

	typedef std::vector<NCFG_ITEM_ID> CFGITEM_LIST;

	//��ʾ����
	virtual void UpdateView(bool bRefresh);

	//����ʾ�Ի���֮ǰ�ļ�����,���ڽ��Դ�ֵ�����������
	virtual unsigned long PreCheckValue(){return 0;}

	//���ҳ���ϵ�ֵ,�ɹ�����0, ʧ�ܷ��ش������(�����ַ�ID��)
	virtual unsigned long CheckValue(){return 0;}

	//�ָ��趨�ı�
	virtual unsigned long RecoverChange(){return 0;}
	
	//����Ƿ����˸���
	virtual void SetModify(bool bModify) { m_bModify = bModify; }
	virtual bool IsModify() const { return m_bModify; }

	//����Ƿ���Ҫ�������ݺ�ˢ��UI
	void SetRefreshView(bool refresh ) { m_bRefreshView = refresh; }
	bool IsRefreshView() const { return m_bRefreshView; }

	void SetConfigBlock(CConfigBlock* pConfigBlock) { m_pCfgBlock = pConfigBlock; }
	CConfigBlock* GetConfigBlock() { return m_pCfgBlock;}

	//��ʼ��֮��, �ڼ������ô���
	void SetSubCfgDlg(CCfgDlg* pCfgDlg, bool bRefresh = false);

	//////////////////////////////////////////////////////////////////////////
	//��¼���豸��Ϣ
	static NET_LOGIN_SUCCESS_REPLY s_loginSuccessReply;
	
protected:
	virtual void OnDestroy();
	virtual void OnPostInitial();
	//unsigned long OnApply();
	unsigned long OnOK();
	unsigned long OnCancel();
	void EndDialog(DLG_END_VALUE result);

protected:
	//��Ҫ�̳���д
	////�����Ҫ������ID
	virtual void GetItemList(CFGITEM_LIST& itemList) const{}
	////��ʾ���� ����false���������öԻ��������������true���������öԻ����ٴ���
	virtual bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh) {return false;}
	////��ȡ�������� ����false���������öԻ��������������true���������öԻ����ٴ���
	virtual bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const {return false;}
	//��һЩ��Ҫ�ڶ�ȡ���ú����Ķ���
	virtual void DoOther() {};
	//��һЩDoOther��������
	virtual void EndOther() {};
	virtual void GetSubItemInfoFromDlg( const NCFG_ITEM_ID itemID, unsigned short& subItemCount, unsigned short& subItemLen);

	////���Ա�����
	//����:�������õĴ���,�Ƿ���Ҫѡ��[Y/C]�Ի���,�Ƿ�����������,�Ƿ���Ҫ��ʱˢ��
	virtual unsigned long TrySaveChange(CCfgDlg* pCfgDlg, bool bChooseDlg=true, bool bHide=false,bool bRefresh=true);
	virtual unsigned long PostSave(){return 0;}

	//����Ҫ�̳���д
	////���ĳ������ID�Ƿ��޸���
	bool IsChange(NCFG_ITEM_ID cfgID) const;
	////����Ok��ťʹ��״̬,��Ӧ��OnOK
	void EnableOkBtn(bool bEnable, bool bRefresh);
	//////��������
	bool LoadConfig();
	//////����Ĭ������,Ĭ�����ò�����(������);�ɹ����غ�,��������Block
	bool LoadDefault(CConfigBlock** pCfgBlock, unsigned long* pError=NULL);
	//////��������
	virtual bool SaveConfig();
	//////��ȡĳһID��Ӧ����������
	bool GetConfig(NCFG_ITEM_ID itemID, unsigned char **ppData, unsigned long &dataLen) const;
	/////������������
	bool SetSubItemInfo(const NCFG_ITEM_ID itemID, unsigned short sub_count, unsigned short sub_len);
	bool GetSubItemInfo(NCFG_ITEM_ID itemID, unsigned short& sub_count, unsigned short& sub_len) const;

	//���سɹ�����0,���ɹ����ش������(�����ַ�ID��)
	//ֻ�����δ���,��ֻ�ָ�һ��ҳ��
	unsigned long OnLoadDefault(CCfgDlg* pCfgDlg);

	GUI::CButton* m_pBtnOK;
	GUI::CButton* m_pBtnDefualt;

protected:
	//ȷ�ϱ��桢�Ƿ񱣴��ˢ��ҳ��
	//void DoOk( bool bRefresh );

	bool SaveConfig(const std::list<NCFG_ITEM_HEAD_EX>& cfgItemList, const BYTE* pConfigBuffer, int len);
	bool SaveConfig(const BYTE* pData, int len);
	//��ȡ��������ͷ����
	BYTE* GetCfgItemHeadBuffer(int* pLen) const;

	////��ʾ����
	bool ShowDataProc(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	////��ȡ��������
	bool SetDataProc(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;
	//��ҳ���϶�ȡ��������,ˢ�²��������Block.   ���� false:���ò�������; true:����Block
	void RefreshChangeItem(bool refreshBlock=true, CCfgDlg *pCfgDlg = NULL );
	//��һЩ��Ҫ�ڶ�ȡ���ú����Ķ���
	void DoOtherProc();
	//��һЩDoOther��������
	void EndOtherProc();

	unsigned long m_clientID;
	unsigned long m_deviceID;

	CConfigBlock* m_pCfgBlock;
	bool m_bCreateCfgBlock;

	bool m_bModify;		//��¼������Ϣ�Ƿ�ı䣬����ȷ���Ƿ���Ҫ����
	bool m_bRefreshView;//��¼�������ݺ��Ƿ���Ҫ����ˢ��UI 

	CCfgDlg* m_pCfgParentDlg;

	typedef vector<CCfgDlg*> CFGDLG_LIST;
	CFGDLG_LIST m_cfgChildDlgList;

	std::list<NCFG_ITEM_HEAD_EX> m_saveItemList;//������������ʱ��ֻ�����������������
};


#endif // _CFG_DLG_H_

