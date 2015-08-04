#ifndef _EDIT_CTRL_H_
#define _EDIT_CTRL_H_

#include "Wnd.h"

namespace GUI
{
	class CEditCtrl : public GUI::CWnd
	{
	public:
		CEditCtrl();
		virtual ~CEditCtrl();

		enum CURSOR_MODE
		{
			CURSOR_MODE_AUTO,//�Զ���ʾ���
			CURSOR_MODE_ALWAYS,//������ʾ���
			CURSOR_MODE_NO,//���ǲ���ʾ���
		};

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, 
			unsigned long cy,
			CWnd* pParent,
			int xFocus,
			int yFocus,
			bool bVisible = true,
			const char* pCaption = NULL,
			bool bBorder = true);

		virtual void SetCaption(const std::string& strCaption, bool bRefresh = true);

		virtual bool AddChar(char ch);//����ַ�
		virtual bool DeleteChar();//ɾ���ַ�


		void SetMaxStringLen(int maxLen) { m_maxLen = maxLen; }
		int GetMaxStringLen() const { return m_maxLen; }

		void SetPassword(bool bPassword) { m_bPassword = bPassword; }

		void SetCurosrMode(CURSOR_MODE mode) {m_cursorMode = mode;} 
		//���ﵽ��󳤶�ʱ���Զ����
		void AtuoClear(bool bAtuoClear) { m_bAutoClear = bAtuoClear; }

		void SetBkColor(PAINT_STATUS status, unsigned long background_color, unsigned long border_color);

		void EnableInput(bool enable=true) { m_bEnableInput = enable;}
		bool IsEnableInput() { return m_bEnableInput;}

		//�������뷨����༭��
		void SetFollowCtrl(bool follow = false) { m_bFollowCtrl = follow; }

		enum { EDIT_VALUE_INVALID = -1};

	protected:
		DECLARE_MSG_MAP()
		virtual void OnInitial();
		virtual void OnPaint(PAINT_STATUS status);

		virtual unsigned long OnKeyAdd();
		virtual unsigned long OnKeyDec();
		virtual unsigned long OnInput(unsigned long keyID);
		virtual unsigned long OnDelete();
		virtual unsigned long OnEnter();

		char GetChar(unsigned long keyID) const;
		void DrawCursor();

		int m_maxLen;//����ַ�����
		bool m_bPassword;
		bool m_bAutoClear;//���ﵽ��󳤶�ʱ���Զ����
		bool m_bEnableInput; //�ܷ��������
		bool m_bFollowCtrl;	//���õ��������뷨����༭��

		struct EDIT_COLOR 
		{
			unsigned long background_color;
			unsigned long border_color;
			unsigned long string_color;
		};
		typedef std::map<PAINT_STATUS, EDIT_COLOR> COLOR_MAP;
		COLOR_MAP m_colorMap;

		
		CURSOR_MODE m_cursorMode;//�Ƿ�������ʾ���
		
	private:
	};
}

#endif

