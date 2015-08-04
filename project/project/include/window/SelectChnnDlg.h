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
		// DoModel ǰ�������趨ͨ����Ŀ=4��8��16��24��32
		// ��ѡ���ͨ������=1��4<=��8<=��16<=��24<=��32<=
		// ��ʼ�Ѿ���ѡ���ͨ��(��λ��ʾ)
		void SetChnnCount(int count, ULONGLONG authCH, int maxSel = 1, ULONGLONG chnnBit = 0);

		// Ĭ������±��Ի�������Ĵ�С������������SetChnnCount�����׼ȷ��ȡĬ�ϴ�С
		GUI::CRect GetDlgDefaultRect();

		// OnOK ��ʹ�ô˽ӿڻ�ȡѡȡ��ͨ��
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
		bool					m_uiLayOutInited;	// �Ƿ񱻳�ʼ��
		unsigned char			m_uiLayOut[CHNN_MAX_ROW][5];	// ����

		int		m_chnnCount;						//��ǰ����ͨ������
		int		m_maxSel;							//��ѡ�����ͨ����Ŀ
		bool	m_bAutoSel;							//�Ƿ��Զ�����ѡ��
		bool	m_bAllChnnEnable;				//�Ƿ�����ͨ����ѡ(�ֳ�ʱΪ�٣�ֻ��NVR�»ط�ʱΪ��������Ȩ�޵�ͨ����ѡ)

	protected:
		std::list<unsigned int> m_chnnSelRec;		//ѡ���¼�б�
		ULONGLONG				m_chnnSelect;		//Bitλ��ʾ��ǰѡ���ͨ��		
		ULONGLONG				m_authCH;			//

		//���л���ͨ��
		bool					m_bUsedMiniMap;
		int						m_chnnMiniMap;		//ͨ��
		GUI::CButton			m_btnMiniMap;		//
		GUI::CStatic			m_scMiniMap;
	};
}

#endif//_SELECT_CHNN_H_
