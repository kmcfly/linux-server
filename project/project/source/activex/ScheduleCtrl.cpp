#include "ScheduleCtrl.h"

using namespace GUI;

GUI::CScheduleCtrl::CScheduleCtrl() : m_caption_width(0) , m_hour_width(0), m_titleColor(COLOR_TEXT_NORMAL)
{
	m_ctrlType = GUI::WND_TYPE_SCHEDULE;
}

GUI::CScheduleCtrl::~CScheduleCtrl()
{

}

bool GUI::CScheduleCtrl::Create(unsigned long ctrlID, 
							unsigned long x, 
							unsigned long y,
							unsigned long cx, 
							unsigned long cy,
							CWnd* pParent, 
							int xFocus, 
							int yFocus, 
							const char* pCaption /*= NULL*/,
							const char captionWidth/* = 40*/,
							bool bVisible/* = ture*/)
{
	//���öԻ�����
	unsigned long style = WND_CHILD | WND_DISABLE_FOCUS;
	if (bVisible)
	{
		style |= WND_VISIBLE;
	} 
	//��������
	m_hour_width = (cx - captionWidth) / 24;
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus))
	{
		if (NULL != pCaption)
		{
			SetCaption(pCaption, false);
		}
		m_caption_width = captionWidth;
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CScheduleCtrl::OnInitial()
{
	GUI::CWnd::OnInitial();	
}

void GUI::CScheduleCtrl::OnPaint(PAINT_STATUS status)
{
	int left = m_rect.m_left;
	int top = m_rect.m_top;
	int right = m_rect.m_right;
	int bottom = m_rect.m_bottom;
	int cx = m_caption_width + m_hour_width * 24;//m_rect.Width();
	int cy = m_rect.Height();

	GUI::CWnd::DrawRect(left, top, m_caption_width, cy, COLOR_DLG_CHILD_BK);
	GUI::CWnd::DrawRect(left+m_caption_width, top, cx-m_caption_width, cy, COLOR_EDIT);

	//������
	{
		int data_left = left + m_caption_width;
		int x1 = 0, x2 = 0, w = 0;
		int widthX1 = 0, widthX2 = 0;

		//������ʵ������Ϊm_hour_width��С��60���������ܱ�֤���ȡ�
		float stride = static_cast<float>(m_hour_width) / 60;

		DATA_NODE_LIST::const_iterator iter;
		for (iter = m_dataList.begin(); iter != m_dataList.end(); ++iter)
		{
			const DATA_NODE& node = (*iter);
			x1 = data_left + node.start_hour * m_hour_width + node.start_min * stride;
			x2 = data_left + node.end_hour * m_hour_width + node.end_min * stride;
			
			//
			if (0 == widthX1)
			{
				widthX1 = x1;
			}

			if ((0 == widthX2) || (x1 <= (widthX2 + 1)))
			{
				//2010-05-14 14:33:00 YSW
				//�������ʱ���ص������������˴���һ���ټ��㡣
				if (widthX2 < x2)
				{
					widthX2 = x2;
				}
				continue;
			}
			else
			{
				w = (widthX2 - widthX1 + 1);//���Ӧ�ü�1��
				//����ʱ���п��ܳ���wΪ�������
				//assert(w >= 0);
				if (w <= 0)
				{
					w = 1;
				}

				GUI::CWnd::DrawRect(widthX1, top + 1, w, cy - 1, COLOR_SELECT);

				widthX1 = x1;
				widthX2 = x2;
			}
		}

		//�����ͼ������ֻ�е����Σ������������ѭ�����ǲ��ܻ��ƣ�����ѭ�����油�㡣
		if ((0 < widthX1) && (0 < widthX2))
		{
			w = (widthX2 - widthX1 + 1);//���Ӧ�ü�1��
			//����ʱ���п��ܳ���wΪ�������
			//assert(w >= 0);
			if (w <= 0)
			{
				w = 1;
			}

			GUI::CWnd::DrawRect(widthX1, top + 1, w, cy - 1, COLOR_SELECT);
		}
	}

	//��ÿ��Сʱ�ָ���
	{
		int x = m_rect.m_left + m_caption_width;

		for (int h = 0; h < 24; ++h)
		{
			GUI::CWnd::DrawLine(x, top, x, bottom, COLOR_LINE_WHITE);
			x += m_hour_width;
		}
	}

	//����
	GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), left + m_length[EDIT_XPOS], top + m_length[EDIT_YPOS], \
		m_caption_width- 2 * m_length[EDIT_XPOS], m_rect.Height()-m_length[EDIT_YPOS], m_titleColor);

	GUI::CWnd::DrawRectBorder(left, top, cx, cy, COLOR_LINE_WHITE);
}

void GUI::CScheduleCtrl::AddData(BYTE start_hour, BYTE start_min, BYTE start_second, BYTE end_hour, BYTE end_min, BYTE end_second)
{
	DATA_NODE node;
	node.start_hour = start_hour;
	node.start_min = start_min;
	node.start_second = start_second;
	node.end_hour = end_hour;
	node.end_min = end_min;
	node.end_second = end_second;

	m_dataList.push_back(node);
}

void GUI::CScheduleCtrl::ClearData(bool bRefresh)
{
	m_dataList.clear();
}

void GUI::CScheduleCtrl::AdjustTime(BYTE& hour,BYTE& min,BYTE& sec)
{
	//����ʱ�����ھ�������,����������ʱ������ܽ�ʱ,����ѡ�����ȽϽ���
	//��һ������ʱ�����,Ҳ�������߷�Χ��,����ѡ����ʱ����ڵ�.
	unsigned long sor_hms = ToCalendar(hour,min,sec);
	unsigned long tmp_hms = sor_hms;

	unsigned long hem_out = 10*60;		//����ʱ���������߷�Χ
	unsigned long hem_in = 3*60;			//����ʱ����ڵ����߷�Χ
	unsigned long out_off=hem_out;
	DATA_NODE_LIST::iterator iter;
	for (iter=m_dataList.begin();iter!=m_dataList.end();iter++)
	{
		unsigned long start_hms = ToCalendar(iter->start_hour,iter->start_min,iter->start_second);
		unsigned long end_hms = ToCalendar(iter->end_hour,iter->end_min,iter->end_second);

		if( sor_hms == start_hms || sor_hms == end_hms )
		{
			return;//ֱ�ӵ���,ֱ�ӷ���
		}

		if( sor_hms<start_hms && (start_hms-sor_hms)<=out_off )
		{
			//��ǰ��
			out_off = start_hms - sor_hms;
			tmp_hms = start_hms;
			continue;
		}

		if( sor_hms>start_hms && sor_hms<end_hms )
		{
			//����֮��,����ѡǰ��
			if( sor_hms-start_hms<=hem_in )
			{
				tmp_hms = start_hms;
				break;
			}

			if( end_hms-start_hms<=hem_in )
			{
				tmp_hms = end_hms;
				break;
			}
		}

		if( sor_hms>end_hms && (sor_hms-end_hms)<=out_off )
		{
			//�����
			out_off = sor_hms - end_hms;
			tmp_hms = end_hms;
			continue;
		}
	}

	ToHms(tmp_hms,hour,min,sec);
}

void GUI::CScheduleCtrl::SetTitilColor(unsigned long color, bool bRefresh/* = true*/)
{
	m_titleColor = color;

	if (bRefresh && IsShow())
	{
		//����
		GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left + m_length[EDIT_XPOS], m_rect.m_top + m_length[EDIT_YPOS], \
			m_caption_width-m_length[EDIT_XPOS], m_rect.Height()-m_length[EDIT_YPOS], m_titleColor, true);
	}
}

void GUI::CScheduleCtrl::OnDestroy()
{
	GUI::CWnd::OnDestroy();

	m_dataList.clear();
}

unsigned long GUI::CScheduleCtrl::ToCalendar(BYTE hour,BYTE min,BYTE sec)
{
	assert(hour<24);
	assert(min<60);
	assert(sec<60);

	return hour*60*60+min*60+sec;
}

void GUI::CScheduleCtrl::ToHms(unsigned long cal, BYTE& hour,BYTE& min,BYTE& sec)
{
	assert(cal<60*60*60);
	unsigned long tmp = cal % (60*60);
	hour = cal / (60*60);
	min = tmp/60;
	sec = tmp%60;
}
