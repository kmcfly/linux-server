/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-14
** Name         : CfgManDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
** --------------------------------------------------------------------------------
** 说明
** --------------------------------------------------------------------------------
1、本类模版基于配置基本类CCfgDlg,模版参数必须是继承于CCfgDlg
的窗口类,专用于带Tab的配置页面
2、作用:创建带Tab样式的页面,并处理配置方面(集成页面)需要完成
的保存配置使能,保存配置后刷新等基本操作
3、使用方式:
  a).继承使用:
  直接继承如:class b : public CCfgManDlg2<CCfgDlg,CCfgDlg>
  然后重写
  void b::OnPreInitial()
  {
	AddCaption("00").AddCaption("01").AddCaption("02");
	CCfgManDlg2<CCfgDlg,CCfgDlg>::OnPreInitial();
	//其他额外
  }
  void GetItemList(CFGITEM_LIST& itemList) const
  {
	itemList.push_back(NCFG_ITEM_ID);
	...
  }

  b).直接使用
  static void GetItemList(CCfgDlg::CFGITEM_LIST& itemList)
  {
    itemList.push_back(NCFG_ITEM_ID);
    ... 
  }

  CCfgManDlg2<CCfgDlg,CCfgDlg> b;
  b.AddCaption("00").AddCaption("01").AddCaption("02");
  b.AddGetItemProc( &GetItemList ); //设置获取NCFG_ITEM_ID
  的回调函数
  b.DoModal(this);
***********************************************************************/

#ifndef _CFG_MAN_H_
#define _CFG_MAN_H_

#include "Dialog.h"
#include "TabCtrl.h"
#include "CfgDlg.h"
#include <vector>

using namespace GUI;

typedef void (*PFunc_GetItemList)(CCfgDlg::CFGITEM_LIST& itemList);

//////////////////////////////////////////////////////////////////////////
// 2 tab 页面
//////////////////////////////////////////////////////////////////////////
template <class T1, class T2>
class CCfgManDlg2 : public CCfgDlg
{
public:
	CCfgManDlg2();
	virtual ~CCfgManDlg2();

public:
	enum PAGE_ITEM
	{
		PAGE_1,
		PAGE_2,
	};

	void SetAnchorPage(PAGE_ITEM item);

public:
	CCfgManDlg2<T1,T2>& AddCaption(const std::string& strCaption );
	void AddGetItemProc(PFunc_GetItemList pGet );
	T1& GetPage1(){return m_cfgT1;};
	T2& GetPage2(){return m_cfgT2;};

protected:
	void OnInitial();
	void OnDestroy();
	virtual void OnPreInitial();
	virtual unsigned long OnApply();
	virtual unsigned long CheckValue();
	CCfgDlg* GetPage(PAGE_ITEM item);
	void GetItemList(CFGITEM_LIST& itemList) const;

	unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickPage1();
	unsigned long OnClickPage2();

protected:
	GUI::CTabCtrl		m_tabCtrl;

private:
	std::vector<std::string>	m_strNameVec;
	PFunc_GetItemList			m_callback;

protected:
	T1						m_cfgT1;
	T2						m_cfgT2;

	PAGE_ITEM m_curPage;

	//
	PAGE_ITEM m_initAnchorPage;
};


//////////////////////////////////////////////////////////////////////////
// 3 tab 页面
//////////////////////////////////////////////////////////////////////////

template <class T1, class T2, class T3>
class CCfgManDlg3 : public CCfgDlg
{
public:
	CCfgManDlg3();
	virtual ~CCfgManDlg3();

public:
	enum PAGE_ITEM
	{
		PAGE_1,
		PAGE_2,
		PAGE_3,
	};

	void SetAnchorPage(PAGE_ITEM item);

public:
	CCfgManDlg3<T1,T2,T3>& AddCaption(const std::string& strCaption );
	void AddGetItemProc(PFunc_GetItemList pGet );
	T1& GetPage1(){return m_cfgT1;};
	T2& GetPage2(){return m_cfgT2;};
	T3& GetPage3(){return m_cfgT3;};

protected:
	void OnInitial();
	void OnDestroy();
	virtual void OnPreInitial();
	virtual unsigned long OnApply();
	virtual unsigned long CheckValue();
	CCfgDlg* GetPage(PAGE_ITEM item);
	void GetItemList(CFGITEM_LIST& itemList) const;

	unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickPage1();
	unsigned long OnClickPage2();
	unsigned long OnClickPage3();

protected:
	GUI::CTabCtrl		m_tabCtrl;

private:
	std::vector<std::string>	m_strNameVec;
	PFunc_GetItemList			m_callback;

protected:
	T1						m_cfgT1;
	T2						m_cfgT2;
	T3						m_cfgT3;

	PAGE_ITEM m_curPage;

	//
	PAGE_ITEM m_initAnchorPage;
};


//////////////////////////////////////////////////////////////////////////
// 4 tab 页面
//////////////////////////////////////////////////////////////////////////

template <class T1, class T2, class T3, class T4>
class CCfgManDlg4 : public CCfgDlg
{
public:
	CCfgManDlg4();
	virtual ~CCfgManDlg4();

public:
	enum PAGE_ITEM
	{
		PAGE_1,
		PAGE_2,
		PAGE_3,
		PAGE_4,
	};

	void SetAnchorPage(PAGE_ITEM item);

public:
	CCfgManDlg4<T1,T2,T3,T4>& AddCaption(const std::string& strCaption );
	void AddGetItemProc(PFunc_GetItemList pGet );
	T1& GetPage1(){return m_cfgT1;};
	T2& GetPage2(){return m_cfgT2;};
	T3& GetPage3(){return m_cfgT3;};
	T4& GetPage4(){return m_cfgT4;};

protected:
	void OnInitial();
	void OnDestroy();
	virtual void OnPreInitial();
	virtual unsigned long OnApply();
	virtual unsigned long CheckValue();
	CCfgDlg* GetPage(PAGE_ITEM item);
	void GetItemList(CFGITEM_LIST& itemList) const;

	unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickPage1();
	unsigned long OnClickPage2();
	unsigned long OnClickPage3();
	unsigned long OnClickPage4();

protected:
	GUI::CTabCtrl		m_tabCtrl;

private:
	std::vector<std::string>	m_strNameVec;
	PFunc_GetItemList			m_callback;

protected:
	T1						m_cfgT1;
	T2						m_cfgT2;
	T3						m_cfgT3;
	T4						m_cfgT4;

	PAGE_ITEM m_curPage;

	//
	PAGE_ITEM m_initAnchorPage;
};

#endif
