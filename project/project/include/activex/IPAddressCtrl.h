#ifndef _IP_ADDRESS_CTRL_H_
#define _IP_ADDRESS_CTRL_H_

#include "Wnd.h"
#include "NumEditCtrl.h"

namespace GUI
{
#define IPADDRESS_FIELD(ip, field) static_cast<BYTE>(((ip>>((field)*8))<<24)>>24)
#define IPADDRESS_VALUE(field1, field2, field3, field4) \
		((static_cast<unsigned long>(field4) << 24) | \
		(static_cast<unsigned long>(field3) << 16) | \
		(static_cast<unsigned long>(field2) << 8) | \
		(static_cast<unsigned long>(field1)))

	class CIPAddressCtrl : public CWnd
	{
	public:
		CIPAddressCtrl();
		virtual ~CIPAddressCtrl();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, 
			unsigned long cy,
			CWnd* pParent,
			int xFocus,
			int yFocus,
			bool bVisible = true);

		void SetIP(unsigned long ip, bool bRefresh);
		unsigned long GetIP() const;

		void SetIP(BYTE field1, BYTE field2, BYTE field3, BYTE field4, bool bRefresh);
		void GetIP(BYTE& field1, BYTE& field2, BYTE& field3, BYTE& field4) const;

		//���IP��ַ��Ч�ԣ�����0Ϊ�ɹ�
		static int IsValidIP(DWORD ip,DWORD dwMask,DWORD route);

	protected:
		void OnInitial();
		void OnPaint(PAINT_STATUS status);

		enum { FIELD_COUNT = 4};//������
		
		GUI::CNumEditCtrl m_field[FIELD_COUNT];

		int m_parition_width;//����֮��ļ������
		int m_field_width;//ÿ����ĳ���

	private:
		void CalculatePos();
		int TestDrawLen();
	//	int GetField()
	};
}

#endif

