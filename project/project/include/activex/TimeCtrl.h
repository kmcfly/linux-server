#ifndef _TIME_CTRL_H_
#define _TIME_CTRL_H_

#include "Wnd.h"
#include "NumEditCtrl.h"
#include "Static.h"
#include "Button.h"
#include <vector>

namespace GUI
{
	//////////////////////////////////////////////////////////////////////////
	//����Ĭ�ϰ�ť,���Ʒ�ʽ�޷��ʹ��������ؼ�����
	//���Э��,�ظ�д���Ʋ���
	class CApmButton : public GUI::CButton
	{
	public:
		CApmButton();
		virtual ~CApmButton();

	protected:
		virtual void OnPaint(PAINT_STATUS status);
	};

	//////////////////////////////////////////////////////////////////////////
	////
	//////////////////////////////////////////////////////////////////////////
	class CTimeCtrl : public GUI::CWnd
	{
	public:
		CTimeCtrl();
		virtual ~CTimeCtrl();

		//��16λ��������,��16λ����ʱ��
		typedef enum _time_mode_
		{
			MODE_H24		= 0x00010000,		//24Сʱ��
			MODE_H12		= 0x00020000,		//12Сʱ��

			CHAR_MINUS		= 0x02000000,		// '-'
			CHAR_COLON		= 0x01000000,		// ':'
			CHAR_DOT		= 0x04000000,		// '.'
		}TIME_MODE;

		typedef enum _time_section_
		{
			//֧����� [ʱ:��] [��:��] [ʱ:��:��]
			SECTION_HOUR	= 0x00010000,		//Сʱ
			SECTION_MIN		= 0x00020000,		//����
			SECTION_SEC		= 0x00040000,		//��
			SECTION_SYM1	= 0x00080000,		//��һ�ָ��
			SECTION_SYM2	= 0x00100000,		//�ڶ��ָ��
			SECTION_APM		= 0x00200000,		//[AM/PM]

			SECTION_NOTIFY_NO	= 0x01000000,	//ģʽ�ı��,�Ƿ��ܵõ�֪ͨ�޸Ľ���:Ĭ�ϲ�֪ͨ
			SECTION_INPUT_NOR	= 0x02000000,	//�Ƿ���Ե�������̵���ͨ����:Ĭ�Ͽ���ʹ��

			SECTION_NUM		= 0x00270000,		//��������
			SECTION_NULL	= 0x00000000,		//
			SECTION_ALL		= ~(0x0),
		}TIME_SECTION;

		typedef enum _time_size_
		{
			WIDTH_HOUR		= 26,
			WIDTH_MIN		= 26,
			WIDTH_SEC		= 26,
			WIDTH_SYM		= 6,
			WIDTH_APM		= 28,

			//��� [Сʱ:����]   [����:����]
			WIDTH_HM		= WIDTH_HOUR + WIDTH_MIN + WIDTH_SYM + WIDTH_APM+ 10,
			//Ĭ��ȫ����
			WIDTH_HMS		= WIDTH_HOUR + WIDTH_MIN + WIDTH_SEC + 2 * WIDTH_SYM + WIDTH_APM +14,
			HEIGHT			= 24,
		}TIME_SIZE;

	public:
		//////////////////////////////////////////////////////////////////////////

		//���á���ȡʱ��ؼ���ʾ��ʽ
		static void SetTimeMode(unsigned long mode,bool bRefresh=true);
		static unsigned long GetTimeMode();
		static unsigned long TimeMode(unsigned long value);

	public:
		static std::string GetTime32Text(unsigned long ultm, unsigned long section = SECTION_ALL);
		static void GetTime32Text(std::string & str, unsigned long ultm, unsigned long section = SECTION_ALL);

	public:
		//��ʱ��ת�����ַ���,���Ե����趨���ڻ�ʱ��;[��][��][��][ʱ][��][��]
		//��ʹ��ʱ��������ʱ��Ϣ,��ʹ��tm�ṹ�и���Ӧ�ֶ�
		//����ʹ������tm�ṹ�ſ�����ȷ���
		//static std::string GetTimeText(const tm& time, unsigned long section = SECTION_ALL);
		static void GetTimeText(std::string & str, const tm& time, unsigned long section = SECTION_ALL);
		static void GetTimeText(std::string & str,const tm& time,unsigned long dateMode,unsigned long timeMode,unsigned long section = SECTION_ALL);

	public:
		//���á���ȡʱ��
		CTimeCtrl& SetTime(int value, unsigned long section, bool bRefresh);
		CTimeCtrl& GetTime(int& value, unsigned long section);

		//���á���ȡ��׼��ʽʱ��
		//void SetTime(tm time,bool bRefresh);
		//tm GetTime();//ע��˽ӿ�ֻ�����[ʱ:��:��],����Ĭ��ֵ0,������tm�ṹ

		//���á���ȡ32λDVRʱ��
		//void SetDVRTime(unsigned long time, bool bRefresh);
		//unsigned long GetDVRTime();//����[ʱ:��:��]�ľ���ʱ�䳤��

		//ֱ�����á���ȡʱ����
		void AddToNotifyList();
		void SetTime(unsigned long secTime,bool bRefresh);//[0,86399]һ���ڵ�ƫ��
		void SetTime(int hour,int minute,int second,bool bRefresh);
		void GetTime(int& hour, int& minute, int& second);
		unsigned long GetTime();//[0,86399]һ���ڵ�ƫ��
		int GetHour();
		int GetMinute();
		int GetSecond();

		//ˢ����ʾλ������
		void RefreshWndPos(bool bRefresh=true);

		//ˢ�����ݵ�����
		void RefreshView(unsigned long section=SECTION_NUM,bool bRefresh=true);

		//ˢ�½������ݵ��洢��
		void RefreshData(unsigned long section=SECTION_NUM);

		//�����ܷ�ʹ�ù���,-,+��
		void EnableAddDec(bool enable = true);

		void Dump();

	public:
		virtual bool Create(unsigned long ctrlID, unsigned long x, unsigned long y,unsigned long cx, unsigned long cy,
									CWnd* pParent,int xFocus,int yFocus,unsigned long section=SECTION_ALL);

	protected:
		virtual void OnInitial();
		virtual void OnDestroy();
		virtual void OnPaint(PAINT_STATUS status);
		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);

	protected:
		//�Զ�����������ֵ�λ��,�ݲ�֧�ֶ�̬����
		void AutoCalculateSize(unsigned long cx,unsigned long cy);

		//������ݺϷ���
		void CheckTime(int& hour, int& min, int& sec);

		//��ʱ����ʽ�ı�ʱ
		void OnTimeModeChange(bool bRefresh);

	//protected:
	//	//���û�������
	//	void SetDrawBg(bool draw=true);
	//	void SetDrawBorder(bool draw=true);

	protected:
		DECLARE_MSG_MAP()
		unsigned long OnChangeHour();
		unsigned long OnChangeMinute();
		unsigned long OnChangeSecond();
		unsigned long OnChangeAPM();

		unsigned long OnInputNumHour(unsigned long keyID);

	protected:
		GUI::CNumEditCtrl			m_neNum[3];			//ʱ:��:��
		GUI::CStatic				m_scSymbol[2];		//':' '-' '.' ������ʾ����
		GUI::CRect					m_wndRect[6];		//ʱ-��-��,,,����

		GUI::CApmButton				m_btnApm;			//[AM/PM]��ʱ���

	protected:
		int							m_hour;				//Сʱ[0,23],��¼��ʵֵ
		int							m_minute;			//����[0,59]
		int							m_second;			//����[0,59]
		bool						m_bAM;				//�Ƿ�������

	private:
		bool						m_bInitRange;		//�Ƿ��ʼ���˿ؼ���Χ
		int							m_preHourRangeMax;	//ǰһ���趨��Сʱ�ؼ����仯��Χ
		unsigned long				m_ctrlSection;		//����ĸ��Ի�����,�����󲻿ɸ���

		//���·�ʽ��CDateTimeCtrl��ʹ��
		//bool						m_bDrawBg;			//�Ƿ���Ʊ���
		//bool						m_bDrawBorder;		//�Ƿ���Ʊ߿�

	private:
		static unsigned long			s_time_mode;	//ʱ��ؼ���ʾģʽ
		static std::vector<CTimeCtrl*>	s_vec_timectrl;	//�洢�Ѿ������˵Ŀؼ�,�޸���ʾģʽʱ,��ֱ�ӱ�֪ͨ�޸�
	};
}

#endif

