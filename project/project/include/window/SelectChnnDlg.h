/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : SelectChnnDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record: 2009-8-28,2009-10-22
1:
***********************************************************************/
#ifndef _SELECT_CHNN_H_
#define _SELECT_CHNN_H_

#include "Dialog.h"
#include "CheckCtrl.h"
#include <list>

namespace GUI
{
	class CSelectChnnDlg : public GUI::CDialog
	{
	public:
		CSelectChnnDlg();
		virtual ~CSelectChnnDlg();

	public:
		const unsigned long CHNN_DLG_BORDER;
		const unsigned long	CHNN_DLG_TOP;
		const unsigned long	CHNN_DLG_LEFT;

		const unsigned long	CHNN_CK_OFFSET_X;
		const unsigned long	CHNN_CK_OFFSET_Y;

		enum{
			CHNN_MAX_1X4 = 4,
			CHNN_MAX_2X4 = 8,
			CHNN_MAX_4X4 = 16,
			CHNN_MAX_6X4 = 24,
			CHNN_MAX_8X4 = 32,
			CHNN_MAX = 32,

			CHNN_MAX_ROW = 8,
		};

	public:
		// DoModel 前必须先设定通道数目=4、8、16、24、32
		// 能选择的通道数量=1、4<=、8<=、16<=、24<=、32<=
		// 初始已经被选择的通道(按位表示)
		void SetChnnCount(int count, ULONGLONG authCH, int maxSel = 1, ULONGLONG chnnBit = 0);

		// 默认情况下本对话框需求的大小，必须在设置SetChnnCount后才能准确获取默认大小
		GUI::CRect GetDlgDefaultRect();

		// OnOK 后，使用此接口获取选取的通道
		ULONGLONG GetSelChnn( ) { return m_chnnSelect; };
		int GetSelMiniMapChnn() { return m_chnnMiniMap; };

		void SetMiniMap(bool bUsed = false, int miniChnn = -1);

		void SetAutoSel(bool bAuto = true) {m_bAutoSel = bAuto;}
		void SetAllChnnEnable(bool bEnable = false) {m_bAllChnnEnable = bEnable;}

	protected:
		void OnInitial();
		void OnOutOfDlg(unsigned long keyID);

		void OnPaint( PAINT_STATUS status );

		DECLARE_MSG_MAP()
		unsigned long OnBtnExit();
		unsigned long OnBtnOK();

		unsigned long OnChnnClick( unsigned long ctrlID );
		unsigned long OnHeaderClick( unsigned long ctrlID );
		unsigned long OnCheckClick( unsigned long ctrlID );
		unsigned long OnBtnMiniMap();

	protected:
		void RefreshSelRec( int chnn, bool checked , bool bRefresh = true );
		void CheckUI( bool bRefresh = false );
		void GetRowCol( unsigned short& row, unsigned short& col );
		void RefreshMiniCaption(bool bRefresh = false);

	protected:
		GUI::CCheckCtrl m_ckChnn[CHNN_MAX];
		GUI::CCheckCtrl m_ckHeader[CHNN_MAX_ROW];

		GUI::CButton	m_btnOK;
		GUI::CButton	m_btnExit;

	protected:
		bool					m_uiLayOutInited;	// 是否被初始化
		unsigned char			m_uiLayOut[CHNN_MAX_ROW][5];	// 布局

		int		m_chnnCount;						//当前最大的通道数量
		int		m_maxSel;							//能选择最大通道数目
		bool	m_bAutoSel;							//是否自动补齐选择
		bool	m_bAllChnnEnable;				//是否所有通道可选(现场时为假，只有NVR下回放时为真所有有权限的通道可选)

	protected:
		std::list<unsigned int> m_chnnSelRec;		//选择记录列表
		ULONGLONG				m_chnnSelect;		//Bit位表示当前选择的通道		
		ULONGLONG				m_authCH;			//

		//画中画的通道
		bool					m_bUsedMiniMap;
		int						m_chnnMiniMap;		//通道
		GUI::CButton			m_btnMiniMap;		//
		GUI::CStatic			m_scMiniMap;
	};
}

#endif//_SELECT_CHNN_H_
