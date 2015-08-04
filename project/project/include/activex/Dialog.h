#ifndef _DIALOG_H_
#define _DIALOG_H_
#include <map>
#include "Wnd.h"
#include "KeyProc.h"
#include "CtrlID.h"
#include "Button.h"
#include "PictureCtrl.h"
#include "Static.h"

namespace GUI
{

class CDialog : public GUI::CWnd
{
public:
	CDialog(void);
	virtual ~CDialog(void);
	//DoModal时，不用调用Create
	virtual bool Create( unsigned long ctrlID, 
		unsigned long x, 
		unsigned long y,
		unsigned long cx, 
		unsigned long cy,
		CWnd* pParent,
		int xFocus,
		int yFocus,
		bool bVisible = true,
		const char* pCaption = NULL);

	//pWnd及pWnd的父窗口在DoModal调用后隐藏，DoModal完成后复原；为NULL时，不隐藏
	//x、y为显示对话框的位置，如果x或y为INVALID_POS，分别为水平居中、垂直居中
	DLG_END_VALUE DoModal(CWnd* pWnd = NULL, int x = CENTER_POS, int y = CENTER_POS, int cx = MAX_SCREEN_X, int cy = MAX_SCREEN_Y, bool bTopWndShow = true);
	
	//设置背景图片ID
	void SetImage(BITMAP_ID id, bool bRefresh);

	//创建Tip提示
	//设置显示那种类型Tip
	static void SetShowTipType( unsigned long type );
	//获取显示那种类型Tip,一次查询一种
	static bool GetShowTipType( unsigned long type=TIP_MSG);
	//给控件添加Tip信息(注:添加到顶层窗口)
	void AddTip(unsigned long ctrlID, unsigned long strID );
	//为顶层窗口创建Tip控件
	void CreateTipWnd(int cx=-1,int x=-1, int y=-1);
	//修改Tip的底色
	void ChangeTipBKColor(unsigned long color);
	//设置每种Tip的提示的颜色
	void SetTipTextColor(unsigned long color,unsigned long type=TIP_MSG);
	//////////////////////////////////////////////////////////////////////////
	static void AddKeyValue(const KEY_VALUE_INFO & keyValue);
	static void AddKeyValueEx(const KEY_VALUE_INFO & keyValue);

	virtual unsigned long ResponseHideCommond( unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos){return KEY_VIRTUAL_MSG_CONTINUE;}
protected:
	DECLARE_MSG_MAP()
	//////////////////////////////////////////////////////////////////////////
	virtual bool CreateDoModal(int x, int y, int cx, int cy);
	virtual void OnInitial();
	virtual void OnDestroy();
	virtual void OnPostInitial();
	virtual void OnPaint(PAINT_STATUS status);

	virtual void OnIdle();//空闲，DoModal模式下无按键
	virtual void OnOutOfDlg(unsigned long keyID){};//鼠标在对话框外操作
	virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);
	virtual void OnExitProc() {}
	//响应默认按钮
	virtual unsigned long OnOK();
	virtual unsigned long OnCancel();
	virtual unsigned long OnApply();

	//计算返回对话框尺寸，改变尺寸是重写
	virtual bool OnCalculateSize(int& cx, int& cy) const;
	//结束模式对话框 
	virtual void EndDialog(DLG_END_VALUE result);
	virtual unsigned long OnVirtualEsc(unsigned long keyID, unsigned long xPos, unsigned long yPos);

	//弹出消息对话框 type:DEFAULT_CTRL组合
	GUI::DLG_END_VALUE MessageBox(const char* szText,const char* szCaption,unsigned long type = DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	//设置默认控件, DEFAULT_CTRL组合 
	void SetDefaultBtn(unsigned long defaultBtn) { m_defaultCtrlType = defaultBtn; }
	void SetDefaultBtnCaption(DEFAULT_CTRL ctrl, const char* szCaption) { m_defaultBtnCaption[ctrl] = szCaption; }
	void SetDefaultBtnImage(DEFAULT_CTRL ctrl, BITMAP_ID normal, BITMAP_ID focus, BITMAP_ID disable);
	//设置标题栏
	//void SetTitle(bool bTitle);
	//bool IsTitle() const { return (0 == (m_style & WND_NO_TITLE)); }

	//Tip输出信息,必须先创建Tip控件
	virtual void SetTip(){}
	GUI::DLG_END_VALUE MessageOut(const char* szText,unsigned long tipType = TIP_MSG, bool bRefresh = true,unsigned long type = DEFAULT_BTN_OK|DEFAULT_ICON_QUESTION);
	GUI::DLG_END_VALUE MessageOut(ENUM_STRING_ID strID,unsigned long tipType = TIP_MSG,bool bRefresh = true,unsigned long type = DEFAULT_BTN_OK|DEFAULT_ICON_QUESTION);
	GUI::DLG_END_VALUE MessageOut(CWnd* pWnd,ENUM_STRING_ID strID,unsigned long tipType = TIP_MSG,bool bRefresh = true,unsigned long type = DEFAULT_BTN_OK|DEFAULT_ICON_QUESTION);
	GUI::DLG_END_VALUE MessageOut(CWnd* pWnd,const char* szText,unsigned long tipType = TIP_MSG,bool bRefresh = true,unsigned long type = DEFAULT_BTN_OK|DEFAULT_ICON_QUESTION);
	
	//默认按钮属性
	enum DEFAULT_BTN_ATTR
	{
		MAX_DEFAULT_BTN = 3,
	};

	GUI::CButton m_btnExit;
	GUI::CButton* m_pDefaultBtn; //默认按钮
	unsigned long m_defaultCtrlType;//默认控件类型，DEFAULT_CTRL组合

	typedef std::map<DEFAULT_CTRL, std::string> DEFAULT_BTN_CAPTION;
	DEFAULT_BTN_CAPTION m_defaultBtnCaption;//默认按钮标题

	struct DEFAULT_BTN_IMAGE
	{
		BITMAP_ID bmpNormal;
		BITMAP_ID bmpFocus;
		BITMAP_ID bmpDisable;
	};
	typedef std::map<DEFAULT_CTRL, DEFAULT_BTN_IMAGE> DEFAULT_BTN_IMAGELIST;
	DEFAULT_BTN_IMAGELIST m_defaultBtnImage;

	//Tip提示信息
	typedef std::map<unsigned long, unsigned long> TIP_MAP;
	TIP_MAP m_tipInfo;	//对控件的提示信息(控件ID,字符串ID)
	unsigned long m_tipType;//Tip的类型
	bool m_tipInit;//是否被初始化
	bool	m_tipShow;//tip的显隐
	int m_tipCount;//计时处理(暂时未设置,目前使用焦点变迁清除)
	int m_tipPosX;	//位置x
	int m_tipPosY;	//位置y
	unsigned long m_tipColor[TIP_COUNT];//tip提示颜色
	GUI::CPictureCtrl m_picTip;	//tip提示图片控件
	GUI::CStatic m_scTip;//tip字符显示控件
	BITMAP_ID	m_idBmpTip;//tip提示图片:错误,警告,询问
	static unsigned long m_cfgTip;//配置tip是否可用

	//////////////////////////////////////////////////////////////////////////
	bool m_bInModal;			//是否模式活动状态
	DLG_END_VALUE	m_endValue;	//退出对话框的返回值

	//////////////////////////////////////////////////////////////////////////
	KEY_MODE		m_keyMode;	//在云台控制、回放界面上更改其值
	bool			m_bTenAdd;	//在disktop界面处理10+功能
	MESSAGE_TYPE	m_lastMsgType;	//记录上次输入的消息类型
	unsigned long	m_tenAddTime;

	bool			m_bAddAdd;	//在disktop界面处理10+功能
	bool			m_bPreset;	//在同为丝印键盘中预置点按键状态
	MESSAGE_TYPE	m_lastMsgAddAddType;	//记录上次输入的消息类型
	unsigned long	m_addAddTime;
	bool            m_bAddFirstKey;
	unsigned long   m_addAddFirstKey;

	static  unsigned long	m_CommandPram; //存储外部键盘命令的参数，比如调预置点
	static	LONGLONG	m_showTime;
	static  LONGLONG	m_lastShowTime;
private:
	void CreateDefaultBtn();//创建默认按钮
	void ShowTip(bool bShow, const char* szText, BITMAP_ID bmpID, unsigned long color, bool bRefresh = true );
	//////////////////////////////////////////////////////////////////////////
	bool GetKeyValue(KEY_VALUE_INFO & keyValue);
	//////////////////////////////////////////////////////////////////////////
	static std::list<KEY_VALUE_INFO> m_keyValueList;
	static CPUB_Lock m_keyListLock;//按键链表锁
};


}
#endif


