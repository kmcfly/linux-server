#ifndef _DATE_CTRL_H_
#define _DATE_CTRL_H_

#include "Wnd.h"
#include "NumEditCtrl.h"
#include "Static.h"
#include "Button.h"
#include "CalendarDlg.h"
#include "Calendar.h"
#include <vector>

namespace GUI
{
	class CDateCtrl : public GUI::CWnd
	{
	public:
		CDateCtrl();
		virtual ~CDateCtrl();

		//��16λ��������,��16λ����ʱ��
		typedef enum _date_mode_
		{
			MODE_YMD		= 0x0001,		//������
			MODE_MDY		= 0x0002,		//������
			MODE_DMY		= 0x0004,		//������

			CHAR_MINUS		= 0x0100,		// '-'
			CHAR_SLANT		= 0x0200,		// '/'
			CHAR_DOT			= 0x0400,		// '.'
		}DATE_MODE;

		typedef enum _date_section_
		{
			SECTION_YEAR	= 0x0001,			//��
			SECTION_MON	= 0x0002,			//��
			SECTION_DAY	= 0x0004,			//��
			SECTION_SYM1	= 0x0008,			//��һ�ָ��
			SECTION_SYM2 = 0x0010,			//�ڶ��ָ��
			SECTION_BTN	= 0x0020,			//������ť

			SECTION_NOTIFY_NO	= 0x0100,		//ģʽ�ı��,�Ƿ��ܵõ�֪ͨ�޸Ľ���:Ĭ�ϲ�֪ͨ
			SECTION_INPUT_NOR	= 0x0200,		//�Ƿ���Ե�������̵���ͨ����:Ĭ�ϲ�����ʹ��
			SECTION_INPUT_CAL	= 0x0400,		//�Ƿ����ʹ����������:Ĭ�Ͽ���ʹ��

			SECTION_NUM	= 0x0007,			//��������
			SECTION_NULL	= 0x0000,			//
			SECTION_ALL		= ~(0x0),
		}DATE_SECTION;

		typedef enum _date_size_
		{
			WIDTH_YEAR		= 46,
			WIDTH_MON		= 26,
			WIDTH_DAY		= 26,
			WIDTH_SYM		= 6,
			WIDTH_BTN		= 24,

			WIDTH_YMD			= WIDTH_YEAR + WIDTH_MON + WIDTH_DAY + WIDTH_SYM * 2 + 14,
			WIDTH_YMD_BTN	= WIDTH_YEAR + WIDTH_MON + WIDTH_DAY + WIDTH_SYM * 2 + WIDTH_BTN + 16,
			HEIGHT				= 24,
		}DATE_SIZE;

		//////////////////////////////////////////////////////////////////////////

		//���á���ȡ���ڿؼ���ʾ��ʽ
		static void SetDateMode(unsigned long mode,bool bRefresh=true);
		static unsigned long GetDateMode();
		static unsigned long DateMode(unsigned long value);
		//////////////////////////////////////////////////////////////////////////

	protected:
		//���á���ȡ����,�ڲ�ֵ����0��ʼ;�ⲿֵ��tm�ṹ��׼
		CDateCtrl& SetDate (int value, unsigned long section, bool bRefresh);
		CDateCtrl& GetDate (int& value, unsigned long section);

	public:
		////���á���ȡ��׼��ʽ����
		//void SetDate(tm time,bool bRefresh);
		//tm GetDate(); //ע��˽ӿ�ֻ�����[��:��:��],����Ĭ��ֵ0,������tm�ṹ

		//���á���ȡ32λDVR����
		//void SetDVRDate(unsigned long time, bool bRefresh);
		//unsigned long GetDVRDate();

		//ֱ�����á���ȡ������
		void AddToNotifyList();
		void SetDate(int year,int mon,int day,bool bRefresh);
		void GetDate(int&year, int& mon, int& day);
		int GetYear();//[2008~2025]
		int GetMonth();//[0~11]
		int GetDay();//[1~31+-]

		//ˢ����ʾλ������
		void RefreshWndPos(bool bRefresh=true);

		//ˢ�����ݵ�����
		void RefreshView(unsigned long section=SECTION_NUM,bool bRefresh=true);

		//ˢ�½������ݵ��洢��
		void RefreshData(unsigned long section=SECTION_NUM);

	public:
		virtual bool Create(  unsigned long ctrlID, unsigned long x, unsigned long y,unsigned long cx, unsigned long cy,
										CWnd* pParent,int xFocus,int yFocus,unsigned long section=SECTION_ALL);

	protected:
		virtual void OnInitial();
		virtual void OnPaint(PAINT_STATUS status);
		virtual void OnDestroy();
		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);

	protected:
		//�Զ�����������ֵ�λ��
		void AutoCalculateSize(unsigned long cx,unsigned long cy);

		//������ݺϷ���
		void CheckDate(int& year, int& mon, int& day);

	//protected:
	//	//���û�������
	//	void SetDrawBg(bool draw=true);
	//	void SetDrawBorder(bool draw=true);

	protected:
		DECLARE_MSG_MAP()
		unsigned long OnChangeYear();
		unsigned long OnChangeMonth();
		unsigned long OnChangeDay();
		unsigned long OnBtnCalendar();

	protected:
		GUI::CNumEditCtrl			m_neNum[3];			//��,��,�� ������ʾ��
		GUI::CStatic						m_scSymbol[2];		// - / . ������ʾ��
		GUI::CButton					m_btnCalendar;		//�������� Button
		
		CCalendar						m_calendar;				//����
		GUI::CCalendarDlg			m_calDlg;					//�����Ի���

		GUI::CRect						m_wndRect[6];			//��-��-��[btn],,,����

	protected:
		int										m_year;					//��
		int										m_month;					//��[0~11]
		int										m_day;						//��[0~30+-1]

	private:
		bool									m_bInitRange;			//�Ƿ��ʼ���˿ؼ���Χ
		int										m_preDayRangeMax;		//ǰһ���������Χ
		unsigned long					m_ctrlSection;			//����ĸ��Ի�����,�����󲻿ɸ���

		//���·�ʽ��CDateTimeCtrl��ʹ��
		//bool									m_bDrawBg;			//�Ƿ���Ʊ���
		//bool									m_bDrawBorder;		//�Ƿ���Ʊ߿�

	private:
		static unsigned long			s_date_mode;			//���ڿؼ���ʾģʽ
		static std::vector<CDateCtrl*>	s_vec_datectrl;	//�洢�Ѿ������˵Ŀؼ�,�޸���ʾģʽʱ,��ֱ�ӱ�֪ͨ�޸�
	};
}

#endif
