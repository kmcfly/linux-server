/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZYX
** Date         : 2011-05-26
** Name         : SelectReplayTimeDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _SELECT_REPLAY_TIME_H_
#define _SELECT_REPLAY_TIME_H_

#include "Dialog.h"
#include "CheckCtrl.h"
#include "Static.h"
//#include <list>

namespace GUI
{
	class CSelectReplayTimeDlg : public GUI::CDialog
	{
	public:
		CSelectReplayTimeDlg();
		virtual ~CSelectReplayTimeDlg();

	public:
		const unsigned long CHNN_DLG_BORDER;
		const unsigned long	CHNN_DLG_TOP;
		const unsigned long	CHNN_DLG_LEFT;

		const unsigned long	CHNN_CK_OFFSET_X;
		const unsigned long	CHNN_CK_OFFSET_Y;

	public:

		// OnOK 后，使用此接口获取选取的通道
		int GetSelTime( ) { return m_selTime; };
		GUI::CRect GetDlgDefaultRect();

	protected:
		void OnInitial();
		void OnOutOfDlg(unsigned long keyID);

		void OnPaint( PAINT_STATUS status );

		DECLARE_MSG_MAP()
		unsigned long OnBtnExit();
		unsigned long OnBtnOK();

		unsigned long OnCheckClick( unsigned long ctrlID );

	protected:
		void RefreshSelRec( int chnn, bool checked , bool bRefresh = true );
		void CheckUI( bool bRefresh = false );

	protected:
		GUI::CCheckCtrl m_ckTime[3];

		GUI::CButton	m_btnOK;
		GUI::CButton	m_btnExit;

		GUI::CStatic	m_staticTip;
		int				m_selTime;
	};
}

#endif//_SELECT_REPLAY_TIME_H_

