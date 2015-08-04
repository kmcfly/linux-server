/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2011-05-26
** Name         : SpecSplitDlg.h //����ָ�Ի���
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
//�ط�ʱ�򲻹���ť���϶Ի���
#ifndef _SPEC_SPLIT_DLG_H_
#define _SPEC_SPLIT_DLG_H_
#include "Dialog.h"
#include "Button.h"
#include "SelectChnnDlg.h"
using namespace GUI;


//����ָ�
typedef enum _special_split_mode_type
{
	SPEC_NONE	=0,
	SPEC_1A5	=1,
	SPEC_1A5EX	=2,//�ôηָ��ǰ�1A5EX������
	SPEC_1A7	=3,
	SPEC_1A7EX	=4,
	SPEC_1A12	=5,
	SPEC_1A12EX	=6,
}SPECTYPE;
//����ĺ궨�岻�ܱ�
typedef enum _play_ch_mode
{
	PLAY_CH_NULL	= 0,
	PLAY_CH_ONE		= 1,
	PLAY_CH_FOUR	= 2,
	PLAY_CH_SIX		= 3,
	PLAY_CH_EIGHT   =4,//�˷ָ�
	PLAY_CH_NINE	= 5,
	
	
	PLAY_CH_THIRTEEN =6,//ʮ���ָ�
	PLAY_CH_SIXTEEN = 7,
}PLAY_CH_MODE;


namespace GUI
{
	class CSpecSplitDlg : public GUI::CDialog
	{
	public:
		CSpecSplitDlg();
		virtual ~CSpecSplitDlg();

		// OnOK ��ʹ�ô˽ӿڻ�ȡ����ָ������
		int GetSPlitType() { return m_splitType; };

		//OnOK�󣬻�ȡ����Ex��ť������Ҫ��m_selCuPlayrChnn  m_currSplitMode;
		void GetExParam(PLAY_CH_MODE &splitMode,ULONGLONG &selCH);
		// Ĭ������±��Ի�������Ĵ�С������������SetChnnCount�����׼ȷ��ȡĬ�ϴ�С
		GUI::CRect GetDlgDefaultRect();
		//��ȡ��Ҫ�Ĳ������Ա��ڳ�ʼ��ʱ�������ʾʲô��ť
		//PLAY_CH_MODE maxSplitMode ���ָ�ģʽ
		//int maxchnnNum ���ͨ����
		//int rectleft playerDlg���ֵ���߾���
		//int rectright playerDlg���ֵ��ұ߾���
		//int recttop playerDlg���ֵĶ�������
		void SetPlyabackParam(PLAY_CH_MODE maxSplitMode,int maxchnnNum,int maxSelChnn);
		void SetPlayToolParam(GUI::CRect playToolRect,GUI::CRect rcWnd);//��������Χ��ز���
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
		GUI::CSelectChnnDlg		m_selChnnDlg;		//ͨ��ѡ��Ի���
	private:
		int				m_splitType;//�ָ�����
		int				m_maxChnnNum;//���ͨ��
		int				m_maxSelChnn;		//�����ѡ��طŵ�ͨ��
		ULONGLONG		m_selCuPlayrChnn;	//��ǰ���ŵ�ͨ��bit
		PLAY_CH_MODE	m_currSplitMode;
		PLAY_CH_MODE	m_maxSplitMode;
		
		ULONGLONG		m_1A5CH;
		ULONGLONG		m_1A7CH;
		ULONGLONG		m_1A12CH;
		//���������˵�����λ����Ҫ����
		GUI::CRect m_playToolRect;
		GUI::CRect m_rcWnd;

	};
}
#endif
