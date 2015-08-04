/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-24
** Name         : CfgDlg.h
** Version      : 1.0
** Description  : 关于配置对话框 读取、保存
** Modify Record:
1:
** --------------------------------------------------------------------------------
** 说明
** --------------------------------------------------------------------------------
1、所有配置类型对话框都继承于本对话框
2、由本类完成所有配置数据的请求和接收，通过网络方式请求
3、带Tab页面的对话框，只有顶级窗口拥有配置数据
4、数据的保存分如下几部分，a.)检测数据是否被修改，并做相
应的标记；b.)退出、切换页面提示用户保存数据 c.)将数据保存
至拥有配置数据的对话框里；d.)通过网络请求的方式保存数据到服务端
5、GetItemList设置本系列页面需要的所有配置数据ID
6、配置类数据ID与窗口控件一一对应：ShowData将数据刷新
到窗口控件；SetData将数据保存至Block中
7、如果页面有非配置类数据对应的控件需要刷新，继承UpdateView
并填写相应的刷新代码
8、CheckValue初步检查页面设置元素是否合法.
9、PreCheckValue特殊的配置项目需要抛出的特殊提示.
10、RecoverChange恢复或设置设定改变.在页面中操作某些需要回
退得操作时,在此处恢复.
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

	//显示配置
	virtual void UpdateView(bool bRefresh);

	//在提示对话框之前的检查操作,后期将以此值来做处理操作
	virtual unsigned long PreCheckValue(){return 0;}

	//检查页面上的值,成功返回0, 失败返回错误代码(错误字符ID号)
	virtual unsigned long CheckValue(){return 0;}

	//恢复设定改变
	virtual unsigned long RecoverChange(){return 0;}
	
	//标记是否做了更改
	virtual void SetModify(bool bModify) { m_bModify = bModify; }
	virtual bool IsModify() const { return m_bModify; }

	//标记是否需要保存数据后，刷新UI
	void SetRefreshView(bool refresh ) { m_bRefreshView = refresh; }
	bool IsRefreshView() const { return m_bRefreshView; }

	void SetConfigBlock(CConfigBlock* pConfigBlock) { m_pCfgBlock = pConfigBlock; }
	CConfigBlock* GetConfigBlock() { return m_pCfgBlock;}

	//初始化之后, 在加入配置窗口
	void SetSubCfgDlg(CCfgDlg* pCfgDlg, bool bRefresh = false);

	//////////////////////////////////////////////////////////////////////////
	//登录、设备信息
	static NET_LOGIN_SUCCESS_REPLY s_loginSuccessReply;
	
protected:
	virtual void OnDestroy();
	virtual void OnPostInitial();
	//unsigned long OnApply();
	unsigned long OnOK();
	unsigned long OnCancel();
	void EndDialog(DLG_END_VALUE result);

protected:
	//需要继承重写
	////添加需要的配置ID
	virtual void GetItemList(CFGITEM_LIST& itemList) const{}
	////显示数据 返回false，其他配置对话框继续处理，返回true，其他配置对话框不再处理
	virtual bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh) {return false;}
	////读取界面数据 返回false，其他配置对话框继续处理，返回true，其他配置对话框不再处理
	virtual bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const {return false;}
	//做一些需要在读取配置后做的动作
	virtual void DoOther() {};
	//做一些DoOther反向动作。
	virtual void EndOther() {};
	virtual void GetSubItemInfoFromDlg( const NCFG_ITEM_ID itemID, unsigned short& subItemCount, unsigned short& subItemLen);

	////尝试保存变更
	//参数:保存配置的窗体,是否需要选择[Y/C]对话框,是否隐藏主窗口,是否需要及时刷新
	virtual unsigned long TrySaveChange(CCfgDlg* pCfgDlg, bool bChooseDlg=true, bool bHide=false,bool bRefresh=true);
	virtual unsigned long PostSave(){return 0;}

	//不需要继承重写
	////检查某个配置ID是否修改了
	bool IsChange(NCFG_ITEM_ID cfgID) const;
	////设置Ok按钮使能状态,对应于OnOK
	void EnableOkBtn(bool bEnable, bool bRefresh);
	//////加载配置
	bool LoadConfig();
	//////加载默认配置,默认配置不保存(于类中);成功返回后,必须销毁Block
	bool LoadDefault(CConfigBlock** pCfgBlock, unsigned long* pError=NULL);
	//////保存配置
	virtual bool SaveConfig();
	//////获取某一ID对应的配置数据
	bool GetConfig(NCFG_ITEM_ID itemID, unsigned char **ppData, unsigned long &dataLen) const;
	/////设置子项数据
	bool SetSubItemInfo(const NCFG_ITEM_ID itemID, unsigned short sub_count, unsigned short sub_len);
	bool GetSubItemInfo(NCFG_ITEM_ID itemID, unsigned short& sub_count, unsigned short& sub_len) const;

	//加载成功返回0,不成功返回错误代号(错误字符ID号)
	//只针对入参窗口,即只恢复一个页面
	unsigned long OnLoadDefault(CCfgDlg* pCfgDlg);

	GUI::CButton* m_pBtnOK;
	GUI::CButton* m_pBtnDefualt;

protected:
	//确认保存、是否保存后刷新页面
	//void DoOk( bool bRefresh );

	bool SaveConfig(const std::list<NCFG_ITEM_HEAD_EX>& cfgItemList, const BYTE* pConfigBuffer, int len);
	bool SaveConfig(const BYTE* pData, int len);
	//获取请求配置头数据
	BYTE* GetCfgItemHeadBuffer(int* pLen) const;

	////显示数据
	bool ShowDataProc(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	////读取界面数据
	bool SetDataProc(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;
	//从页面上读取配置数据,刷新差异链表或Block.   参数 false:设置差异链表; true:更新Block
	void RefreshChangeItem(bool refreshBlock=true, CCfgDlg *pCfgDlg = NULL );
	//做一些需要在读取配置后做的动作
	void DoOtherProc();
	//做一些DoOther反向动作。
	void EndOtherProc();

	unsigned long m_clientID;
	unsigned long m_deviceID;

	CConfigBlock* m_pCfgBlock;
	bool m_bCreateCfgBlock;

	bool m_bModify;		//记录数据信息是否改变，用于确定是否需要保存
	bool m_bRefreshView;//记录保存数据后，是否需要重新刷新UI 

	CCfgDlg* m_pCfgParentDlg;

	typedef vector<CCfgDlg*> CFGDLG_LIST;
	CFGDLG_LIST m_cfgChildDlgList;

	std::list<NCFG_ITEM_HEAD_EX> m_saveItemList;//差异链表，保存时，只保存链表里的配置项
};


#endif // _CFG_DLG_H_

