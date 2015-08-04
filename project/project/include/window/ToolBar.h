/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-07-24
** Name         : ToolBar.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _TOOL_BAR_H_
#define _TOOL_BAR_H_
#include "Dialog.h"
#include "Button.h"
#include "StateButton.h"
#include "DrawManager.h"
#include "Rect.h"
#include "WndStructDef.h"

using namespace GUI;

namespace GUI
{
	class CToolBar : public GUI::CDialog
	{
	public:
		CToolBar();
		~CToolBar();

		enum
		{
			TOOL_BOTTOM,
			TOOL_TOP,
			TOOL_PT,
		};

		CRect GetSubBtnWndRect(CTRLID id);

		void EnableDwell(bool bOpen, bool bRefresh = true);
		void EnableAudio(bool bOpen, bool bRefresh = true);

		bool GetCfgPos(NCFG_INFO_TOOLBAR_POS& posInfo);

		bool PtInToolbar(int xPos, int yPos);

		GUI::CRect AutoLayout(NCFG_INFO_TOOLBAR_POS* pPosInfo = NULL, bool bRefresh = false, unsigned long xPos = -1, unsigned long yPos = -1);

		bool MoveToolWnd(int x, int y);
		void LayoutSubWnd();

		void OnIdle();
		void OnEnter(unsigned long xPos, unsigned long yPos);
		void OnDBClick(unsigned long xPos, unsigned long yPos);
		void OnMouseMove(unsigned long xPos, unsigned long yPos);
		GUI::CRect OnMouseDrag(unsigned long xPos, unsigned long yPos, bool bRefresh = false);
		unsigned long OnMouseLeftUp();

		//(x1,y1) (x2,y2) 距离相差不大
		bool IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);
		//(x1,y1) (x2,y2) 距离相差很大
		bool IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);
		bool CalculatePosInfo(NCFG_INFO_TOOLBAR_POS& posInfo, const GUI::CRect& rect, int xoffset, int yoffset, bool bAutoCenter = true);

		void SetShowToolbar(bool bShow = true) {m_bShowToolBar = bShow;}
		virtual void Show(bool bShow, bool bRefresh = true);
		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);

	protected:
		DECLARE_MSG_MAP()
		void OnInitial();
		void OnDestroy();
		void OnPaint(PAINT_STATUS status);
		
	private:
		//////////////////////////////////////////////////////////////////////////
		unsigned long OnClick6X6();
		unsigned long OnClick5X5();
		unsigned long OnClick4X4();
		unsigned long OnClick1A12();
		unsigned long OnClick1A7();
		unsigned long OnClick3X3();
		unsigned long OnClick1A5();
		unsigned long OnClick2X3();
		unsigned long OnClick2X2();
		unsigned long OnClick1X1();

		unsigned long OnClick6X6Ex();
		unsigned long OnClick5X5Ex();
		unsigned long OnClick4X4Ex();
		unsigned long OnClick1A12Ex();
		unsigned long OnClick1A7Ex();
		unsigned long OnClick3X3Ex();
		unsigned long OnClick1A5Ex();
		unsigned long OnClick2X3Ex();
		unsigned long OnClick2X2Ex();
		unsigned long OnClick1X1Ex();

		unsigned long OnClickSystem();
		unsigned long OnClickSearch();
		unsigned long OnClickBackup();
		unsigned long OnClickInformation();

		unsigned long OnClickDwell();
		unsigned long OnClickZoom();
		unsigned long OnClickAudio();
		unsigned long OnClickPTZ();
		unsigned long OnClickRec();
		unsigned long OnClickPlay();
		unsigned long OnClickPlayEx();
		unsigned long OnClickShutDown();
		unsigned long OnClickStatus();
		unsigned long OnClickChange();
		unsigned long OnClickSnap();
		unsigned long OnClickPIP();
		unsigned long OnClickToolEdit(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
		unsigned long OnClickBurn();
		unsigned long OnClickBurnEx();
		unsigned long OnClickSysInfo();
		unsigned long OnClickCamPara();
		void OnBottomLayout();


		//
		#ifdef __CUSTOM_US02_NEW__
		unsigned long OnFocus();
		/*unsigned long OnOver();
		unsigned long OnOut();*/
		unsigned long OnFocus(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
		#endif
		//////////////////////////////////////////////////////////////////////////
		enum
		{
			DWELL_OPEN,
			DWELL_CLOSE
		};

		enum
		{
			AUDIO_OPEN,
			AUDIO_MUTE
		};

		enum
		{
			SPLIT_UP,
			SPLIT_DOWN
		};

		GUI::CButton	m_btnMenu;

		GUI::CButton	m_btn6X6;
		GUI::CButton	m_btn5X5;
		GUI::CButton	m_btn4X4;
		GUI::CButton	m_btn1A12;
		GUI::CButton	m_btn1A7;
		GUI::CButton	m_btn3X3;
		GUI::CButton	m_btn1A5;
		GUI::CButton	m_btn2X3;
		GUI::CButton	m_btn2X2;
		GUI::CButton	m_btn1X1;
		
		GUI::CStateButton	m_btn5X5Ex;
		GUI::CStateButton	m_btn4X4Ex;
		GUI::CStateButton	m_btn1A12Ex;
		GUI::CStateButton	m_btn1A7Ex;
		GUI::CStateButton	m_btn3X3Ex;
		GUI::CStateButton	m_btn1A5Ex;
		GUI::CStateButton	m_btn2X3Ex;
		GUI::CStateButton	m_btn2X2Ex;
		GUI::CStateButton	m_btn1X1Ex;
		GUI::CStateButton	m_btnDwell;
		GUI::CButton		m_btnZoom;
		GUI::CStateButton	m_btnAudio;
		GUI::CStateButton	m_btnPlayEx;
		GUI::CButton		m_btnPip;
		GUI::CButton		m_btnPTZ;
		GUI::CButton		m_btnRec;
		GUI::CButton		m_btnPlay;
		GUI::CButton		m_btnSnap;
		GUI::CButton		m_btnStateToolBar;
		GUI::CButton		m_btnChangeColor;
		GUI::CButton		m_btnBurn;
		GUI::CStateButton	m_btnBurnEx;
		GUI::CButton		m_btnSysInfo;
		GUI::CButton		m_btnCamPara;
		int	m_bmpBgWidth;
		int	m_bmpBgHeight;

		//////////////////////////////////////////////////////////////////////////
		struct BTN_WND_INFO
		{
			void SetData(unsigned long btnType, GUI::CWnd* pBtnWnd, unsigned short cx, unsigned short cy, unsigned short yTop)
			{
				type = btnType; pWnd = pBtnWnd; width = cx; height = cy; top = yTop;
			}

			unsigned long	type;	//0 主菜单; 1 分割模式; 2 扩展分割模式; 3 功能按钮; 4 工具条状态
			unsigned short	width;
			unsigned short	height;
			unsigned short	top;
			GUI::CWnd*		pWnd;
		};

		struct BTN_WND_LAYOUT
		{
			int			max_width;		//所有按钮总长度

			int			func_width;		//功能区长度
			int			split_width;	//分割区长度

			int			style;			//布局样式
			int			status;			//工具条状态
			int			num_row;		//分几行

			GUI::CRect	rect;
		};
	
		int						m_lenWndInfo;
		BTN_WND_INFO			m_btnWndInfo[32];
		BTN_WND_LAYOUT			m_btnWndLayout;
		NCFG_INFO_TOOLBAR_POS	m_posInfo;

		enum
		{
			BTN_ITEM_MENU,
			BTN_ITEM_SPLIT,
			BTN_ITEM_SPLIT_EX,
			BTN_ITEM_FUNC,
			BTN_ITEM_FUNC_EX,
			BTN_ITEM_STATE,
		};

		//////////////////////////////////////////////////////////////////////////

		unsigned long	m_x_enter, m_y_enter;
		unsigned long	m_x_drag_last, m_y_drag_last;	//

		bool			m_bFirstDrag;				//第一次拖动标记
		bool			m_bDrag;					//是否处于拖动状态
		bool			m_bShowToolBar;				//是否处于显示

		unsigned long	m_x_pos, m_y_pos;			//鼠标位置
		unsigned long	m_x_pos_last, m_y_pos_last;	//上一次鼠标位置
		unsigned long	m_count;					//计时显示、隐藏
		unsigned long	m_wait;						//操作等待延时

		int				m_top_offset;				//移动点,距离顶距离
		int				m_right_offset;				//移动点,距离右边距离

		bool			m_b_cvbs;					//是否是CVBS显示方式
		GUI::CRect		m_tool_show_rect;			//toolbar显示区域

	};
}

#endif //_TOOL_BAR_H_
