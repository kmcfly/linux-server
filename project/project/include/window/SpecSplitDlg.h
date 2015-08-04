/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2011-05-26
** Name         : SpecSplitDlg.h //奇异分割对话框
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
//回放时候不规则按钮集合对话框
#ifndef _SPEC_SPLIT_DLG_H_
#define _SPEC_SPLIT_DLG_H_
#include "Dialog.h"
#include "Button.h"
#include "SelectChnnDlg.h"
using namespace GUI;


//特殊分割
typedef enum _special_split_mode_type
{
	SPEC_NONE	=0,
	SPEC_1A5	=1,
	SPEC_1A5EX	=2,//该次分割是按1A5EX产生的
	SPEC_1A7	=3,
	SPEC_1A7EX	=4,
	SPEC_1A12	=5,
	SPEC_1A12EX	=6,
}SPECTYPE;
//下面的宏定义不能变
typedef enum _play_ch_mode
{
	PLAY_CH_NULL	= 0,
	PLAY_CH_ONE		= 1,
	PLAY_CH_FOUR	= 2,
	PLAY_CH_SIX		= 3,
	PLAY_CH_EIGHT   =4,//八分割
	PLAY_CH_NINE	= 5,
	
	
	PLAY_CH_THIRTEEN =6,//十三分割
	PLAY_CH_SIXTEEN = 7,
}PLAY_CH_MODE;


namespace GUI
{
	class CSpecSplitDlg : public GUI::CDialog
	{
	public:
		CSpecSplitDlg();
		virtual ~CSpecSplitDlg();

		// OnOK 后，使用此接口获取奇异分割的类型
		int GetSPlitType() { return m_splitType; };

		//OnOK后，获取按下Ex按钮后所需要的m_selCuPlayrChnn  m_currSplitMode;
		void GetExParam(PLAY_CH_MODE &splitMode,ULONGLONG &selCH);
		// 默认情况下本对话框需求的大小，必须在设置SetChnnCount后才能准确获取默认大小
		GUI::CRect GetDlgDefaultRect();
		//获取需要的参数，以便在初始化时候决定显示什么按钮
		//PLAY_CH_MODE maxSplitMode 最大分割模式
		//int maxchnnNum 最大通道数
		//int rectleft playerDlg布局的左边距离
		//int rectright playerDlg布局的右边距离
		//int recttop playerDlg布局的顶部距离
		void SetPlyabackParam(PLAY_CH_MODE maxSplitMode,int maxchnnNum,int maxSelChnn);
		void SetPlayToolParam(GUI::CRect playToolRect,GUI::CRect rcWnd);//控制条范围相关参数
		bool OnClickSelChnn(GUI::CButton & btn, PLAY_CH_MODE splitMode, int splitChnn, ULONGLONG & selCH);
	public:
		const unsigned long CHNN_DLG_BORDER;
		const unsigned long	CHNN_DLG_TOP;
		const unsigned long	CHNN_DLG_LEFT;

		const unsigned long	CHNN_CK_OFFSET_X;
		const unsigned long	CHNN_CK_OFFSET_Y;
	protected:
		DECLARE_MSG_MAP()
		void OnInitial();
		void OnOutOfDlg(unsigned long keyID);

		void OnPaint( PAINT_STATUS status );

		unsigned long OnClickBtn1A5();
		unsigned long OnClickBtn1A5Ex();

		unsigned long OnClickBtn1A7();
		unsigned long OnClickBtn1A7Ex();

		unsigned long OnClickBtn1A12();
		unsigned long OnClickBtn1A12Ex();
	protected:
		GUI::CButton m_btn1A5;
		GUI::CButton m_btn1A5Ex;

		GUI::CButton m_btn1A7;
		GUI::CButton m_btn1A7Ex;

		GUI::CButton m_btn1A12;
		GUI::CButton m_btn1A12Ex;
	protected:
		GUI::CSelectChnnDlg		m_selChnnDlg;		//通道选择对话框
	private:
		int				m_splitType;//分割类型
		int				m_maxChnnNum;//最大通道
		int				m_maxSelChnn;		//最大能选择回放的通道
		ULONGLONG		m_selCuPlayrChnn;	//当前播放的通道bit
		PLAY_CH_MODE	m_currSplitMode;
		PLAY_CH_MODE	m_maxSplitMode;
		
		ULONGLONG		m_1A5CH;
		ULONGLONG		m_1A7CH;
		ULONGLONG		m_1A12CH;
		//二级弹出菜单计算位置需要参数
		GUI::CRect m_playToolRect;
		GUI::CRect m_rcWnd;

	};
}
#endif
