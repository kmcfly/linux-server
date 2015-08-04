#ifndef _IPV6_CTRL_H_
#define _IPV6_CTRL_H_

#include "Wnd.h"
#include "HexNumEditCtrl.h"

namespace GUI
{
	class CIPV6Ctrl: public CWnd
	{
	public:
		CIPV6Ctrl();
		virtual ~CIPV6Ctrl();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, 
			unsigned long cy,
			CWnd* pParent,
			int xFocus,
			int yFocus,
			bool bVisible = true);

		void SetIP(const unsigned char *pIP, unsigned length, bool bRefresh);
		void GetIP(unsigned char *pIPv6, int length) const;

	protected:
		void OnInitial();
		void OnPaint(PAINT_STATUS status);

		enum { FIELD_COUNT = 8};//������
		GUI::CHexNumEditCtrl m_field[FIELD_COUNT];

		int m_parition_width;//����֮��ļ������
		int m_field_width;//ÿ����ĳ���

	private:
		void CalculatePos();
		int TestDrawLen();
	};
}
#endif
