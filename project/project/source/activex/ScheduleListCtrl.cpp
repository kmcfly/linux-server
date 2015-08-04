#include "ScheduleListCtrl.h"
#include "Static.h"
#include "Button.h"

using namespace GUI;

GUI::CScheduleListCtrl::CScheduleListCtrl()  : m_dotXPos(0), m_title_width(40)
, m_hour_width(0)
, m_pageSize(SHOW_ALL_ITEM)
, m_curPage(0)
{
	m_ctrlType = GUI::WND_TYPE_SCHEDULE_LIST;

	memset(m_pWndPageCtrl, 0, sizeof(m_pWndPageCtrl));
}

GUI::CScheduleListCtrl::~CScheduleListCtrl()
{

}

bool GUI::CScheduleListCtrl::Create(unsigned long ctrlID, 
							unsigned long x, 
							unsigned long y,
							unsigned long cx, 
							unsigned long cy,
							CWnd* pParent, 
							int xFocus, 
							int yFocus, 
							int titleWidth,
							const char* pCaption/* = NULL*/,
							int pageSize/* = SHOW_ALL_ITEM*/,
							bool bVisible/* = ture*/)
{
	//���öԻ�����
	unsigned long style = WND_CHILD;
	if (bVisible)
	{
		style |= WND_VISIBLE;
	} 
	m_curPage = 0;

	//��������
	m_pageSize = pageSize;
	//assert(cx >= (titleWidth + 480));
	m_title_width = titleWidth;
	m_hour_width = (cx - titleWidth - 1) / 24;

	unsigned long OffSetX = cx - ( m_title_width + m_hour_width * 24 );
	cx = m_title_width + m_hour_width * 24;//(cx < m_length[FONT_WIDTH]) ? m_length[FONT_WIDTH] : cx;
	//cy = 340;//(cy < m_length[EDIT_HEIGHT]) ? m_length[EDIT_HEIGHT] : cy;
	if (GUI::CWnd::Create(ctrlID, x + OffSetX / 2, y, cx, cy, pParent, style, xFocus, yFocus))
	{
		if (NULL != pCaption)
		{
			m_strCaption = pCaption;
		}
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CScheduleListCtrl::OnInitial()
{
	GUI::CWnd::OnInitial();	

	if (SHOW_ALL_ITEM != m_pageSize)
	{
		SetPageSize(m_pageSize);
	}
}

void GUI::CScheduleListCtrl::OnPaint(PAINT_STATUS status)
{
	//����
	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top , m_rect.Width(), m_rect.Height(), COLOR_DLG_CHILD_BK);	
	
	//�����
	{
		int x = m_rect.m_left + m_title_width;
		int top = m_rect.m_top ;

		unsigned short cx = 0, cy = 0;
		char sz[64] = {0};
		strcpy(sz, "88:88");
		TextSize(sz, strlen(sz), cx, cy);

		for (int h = 0; h < 24; ++h)
		{
			if (h % 4 == 0)
			{
				snprintf(sz, 64, "%.2d:00", h);
				GUI::CWnd::DrawText(sz, strlen(sz), x + 2, top, cx, m_length[SCHEDULE_HEADER_HEIGHT] - m_length[EDIT_YPOS]);
			
				GUI::CWnd::DrawLine(x, top + m_length[SCHEDULE_HEADER_HEIGHT] - 8, x, top + m_length[SCHEDULE_HEADER_HEIGHT], COLOR_LINE_WHITE);
			}
			x += m_hour_width;
		}
		
	}	

	if (!m_strCaption.empty())
	{
		GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left + m_length[EDIT_XPOS], m_rect.m_top + m_length[EDIT_YPOS], m_title_width, m_length[SCHEDULE_HEADER_HEIGHT]);
	}

	GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top + m_length[SCHEDULE_HEADER_HEIGHT], m_rect.m_right, m_rect.m_top + m_length[SCHEDULE_HEADER_HEIGHT], COLOR_LINE_WHITE, 1);
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), (PAINT_STATUS_FOCUS == status) ? COLOR_SELECT : COLOR_LINE_WHITE, 1);
}

void GUI::CScheduleListCtrl::OnErase(PAINT_STATUS status)
{
	if (0 != m_dotXPos)//�������ʾδ������ʾ����
	{
		int mapSize = m_dataCtrlMap.size();

		unsigned long num = (m_pageSize <= mapSize) ? m_pageSize : mapSize;
		if (m_curPage == mapSize / m_pageSize)
		{
			num = mapSize % m_pageSize;
		}
		unsigned long yOffSet = m_length[SCHEDULE_HEADER_HEIGHT] + ((m_length[SCHEDULE_ITEM_HEIGHT]-1)*num);

		if (yOffSet > m_length[SCHEDULE_HEADER_HEIGHT])
		{
			GUI::CWnd::DrawLine(m_dotXPos, m_showRect.m_top+m_length[SCHEDULE_HEADER_HEIGHT], m_dotXPos, m_showRect.m_top+yOffSet, COLOR_TEXT_FOCUS, 2);
		}
	}
}

void GUI::CScheduleListCtrl::OnChangeFocus(unsigned long oldID, unsigned long newID)
{

}

bool GUI::CScheduleListCtrl::AddData(int item, BYTE start_hour, BYTE start_min, BYTE start_second, BYTE end_hour, BYTE end_min, BYTE end_second)
{
	DATA_CTRL_MAP::iterator iter = m_dataCtrlMap.find(item);
	if (m_dataCtrlMap.end() != iter)
	{
		iter->second->AddData(start_hour, start_min, start_second, end_hour, end_min, end_second);
		
		return true;
	}
	else
	{
		assert(false);//�����ڴ���
		return false;
	}
}

void GUI::CScheduleListCtrl::ClearData(int item, bool bRefresh)
{
	DATA_CTRL_MAP::iterator iter = m_dataCtrlMap.find(item);
	if (m_dataCtrlMap.end() != iter)
	{
		iter->second->ClearData(false);

		if (bRefresh)
		{
			Repaint();
		}
	}
	else
	{
		assert(false);//�����ڴ���
	}
}

bool GUI::CScheduleListCtrl::AddItem(int item, const char* pStr, bool bRefresh)
{
	if (m_dataCtrlMap.end() != m_dataCtrlMap.find(item))
	{
		assert(false);//�Ѿ����ڴ���
		return false;
	}

	int index = m_dataCtrlMap.size();

	CScheduleCtrl* pScheduleCtrl = new CScheduleCtrl;
	unsigned long y = m_length[SCHEDULE_HEADER_HEIGHT] + (m_length[SCHEDULE_ITEM_HEIGHT] - 1)* index;
	if( (SHOW_ALL_ITEM != m_pageSize) && (item >= m_pageSize) )
	{
		y = 0;
	}
	pScheduleCtrl->Create(GetRandWndID(), 0, y, m_rect.Width(), m_length[SCHEDULE_ITEM_HEIGHT], this, 0, index, pStr, m_title_width);

	if ((SHOW_ALL_ITEM != m_pageSize) && (item >= m_pageSize))
	{
		pScheduleCtrl->Show(false, false);
	}
	
	m_dataCtrlMap[item] = pScheduleCtrl;
	
	SetPageInfo();
	if (bRefresh)
	{
		Repaint();
	}

	return true;
}

void GUI::CScheduleListCtrl::RemoveAllItem(bool bRefresh)
{
	DATA_CTRL_MAP::iterator iter;
	for (iter = m_dataCtrlMap.begin(); iter != m_dataCtrlMap.end(); ++iter)
	{
		assert(NULL != iter->second);
		iter->second->Show(false, false);
		iter->second->Destroy();
		delete iter->second;
		iter->second = NULL;
	}
	m_dataCtrlMap.clear();

	if (bRefresh)
	{
		Repaint();
	}
}

void GUI::CScheduleListCtrl::RepaintData(int item)
{
	DATA_CTRL_MAP::iterator iter = m_dataCtrlMap.find(item);
	if(iter != m_dataCtrlMap.end())
	{
		iter->second->Repaint();
	}
}

void GUI::CScheduleListCtrl::MoveFirst(bool bRefresh)
{
	if( !(m_dataCtrlMap.size() > 0) )
	{
		return;
	}

	if (SHOW_ALL_ITEM != m_pageSize )
	{
		SetCurPage(0,bRefresh);
		return;
	}
	else
	{
		return;
	}
}

bool GUI::CScheduleListCtrl::HitTestTime(unsigned long xPos,unsigned long yPos, BYTE& hour, BYTE& min,BYTE& sec)
{
	int itemIndex = -1;
	if(HitTest(xPos, yPos, itemIndex, hour, min, sec))
	{
		int item = 0;
		DATA_CTRL_MAP::iterator iter;
		for (iter = m_dataCtrlMap.begin(); iter != m_dataCtrlMap.end(); ++iter, ++item)
		{
			//����Ҫ��map��,��������״̬
			//���������������
			if( item == itemIndex )
			{
				//ֵ������
				//����������������,����10���������Զ�������Ե(�п������)
				//������������,����3���������Զ�������Ե(�п������)
				iter->second->AdjustTime(hour, min, sec);

				return true;
				break;
			}
		}
	}
	else
	{
		return false;
	}

	assert(false);
	return false;
}

bool GUI::CScheduleListCtrl::HitTest(unsigned long xPos, unsigned long yPos, int& item, BYTE& hour, BYTE& min, BYTE& sec)
{
	//�ų�����������֮�ڵ�x,y
	if( xPos < (m_rect.m_left+m_title_width) ||
		xPos > (m_rect.m_left+m_title_width+m_hour_width*24) ||
		yPos < (m_rect.m_top+m_length[SCHEDULE_HEADER_HEIGHT]) ||
		yPos > m_rect.m_bottom )
	{
		return false;	//��һ���ų�
	}

	//��ǰҳ���Ԫ������
	int countItem = SHOW_ALL_ITEM;
	if( m_pageSize != SHOW_ALL_ITEM )
	{
		countItem = m_pageSize;
	}
	else
	{
		countItem = m_dataCtrlMap.size();
	}

	//xoffֵ����ʱ��,yoffֵ�㴦���ĸ��ƻ�
	unsigned long xoff = xPos - m_rect.m_left - m_title_width;
	unsigned long yoff = yPos - m_rect.m_top - m_length[SCHEDULE_HEADER_HEIGHT];
	unsigned long height = m_length[SCHEDULE_ITEM_HEIGHT] - 1;//�ƻ��ؼ��߶�

	if( yoff > countItem * height )
	{
		return false;//�ڶ����ų�
	}

	int itemIndex = -1;//����:ǰ�պ�
	if( m_pageSize != SHOW_ALL_ITEM )
	{
		int index = yoff / height;
		itemIndex = m_curPage * m_pageSize + index;
	}
	else
	{
		itemIndex = yoff / height;
	}

	int itemIter = 0;
	DATA_CTRL_MAP::iterator iter;
	for (iter = m_dataCtrlMap.begin(); iter != m_dataCtrlMap.end(); ++iter, ++itemIter)
	{
		//����Ҫ��map��,��������״̬
		//���������������
		if( itemIter == itemIndex )
		{
			break;//�ҵ���Ҫ��iter,�Ա���
		}
	}

	BYTE tmpHour,tmpMin,tmpSec;
	if( iter != m_dataCtrlMap.end() )
	{
		tmpHour = xoff / m_hour_width;
		unsigned long uMinSec = 60*60*(xoff%m_hour_width)/m_hour_width;
		tmpMin = uMinSec/60;
		tmpSec = uMinSec%60;

		//��ֵ����
		if( tmpHour >= 24 )
		{
			tmpHour = 23;
			tmpMin = 59;
			tmpSec = 59;
		}

		//����
		hour = tmpHour;
		min = tmpMin;
		sec = tmpSec;
		item = itemIndex;

		return true;
	}
	else
	{
		return false;//���һ���ų�,������������ݵ�����
	}
}

void GUI::CScheduleListCtrl::SetMarkItem(int item, bool bMark, bool bRefresh/* = true*/)
{
	if (m_dataCtrlMap.end() != m_dataCtrlMap.find(item))
	{
		CScheduleCtrl* pScheduleCtrl = m_dataCtrlMap[item];
		pScheduleCtrl->SetTitilColor(bMark? COLOR_SELECT : COLOR_TEXT_NORMAL, bRefresh);
	}
	else
	{
		assert(false);//�����ڴ���
	}
}

void GUI::CScheduleListCtrl::SetShowDotFlag()
{
	m_dotXPos = m_showRect.m_left + m_title_width;
}

void GUI::CScheduleListCtrl::DrawDotFlag(unsigned long xPos, unsigned long yPos, bool bRefresh/* = false*/)
{
	if (0 != m_dotXPos)//�������ʾδ������ʾ����
	{
		if ((m_dotXPos != xPos) && ((m_showRect.m_left + m_title_width) <= xPos) && (xPos <= m_showRect.m_right))
		{
			unsigned long num = (m_pageSize <= m_dataCtrlMap.size()) ? m_pageSize : m_dataCtrlMap.size();
			unsigned long yOffSet = m_length[SCHEDULE_HEADER_HEIGHT] + ((m_length[SCHEDULE_ITEM_HEIGHT]-1)*num);

			if ((yOffSet > m_length[SCHEDULE_HEADER_HEIGHT]) && ((m_showRect.m_top+m_length[SCHEDULE_HEADER_HEIGHT]) <= yPos) && (yPos <= m_showRect.m_top+yOffSet))
			{
				m_dotXPos = xPos;

				if (bRefresh)
				{
					Repaint();
				}
			}
		}
	}
}

void GUI::CScheduleListCtrl::OnDestroy()
{
	GUI::CWnd::OnDestroy();

	DATA_CTRL_MAP::iterator iter;
	for (iter = m_dataCtrlMap.begin(); iter != m_dataCtrlMap.end(); ++iter)
	{
		assert(NULL != iter->second);
		delete iter->second;
		iter->second = NULL;
	}
	m_dataCtrlMap.clear();

	for (int i = 0; i < PAGECTRL_COUNT; ++i)
	{
		if (NULL != m_pWndPageCtrl[i])
		{
			delete m_pWndPageCtrl[i];
			m_pWndPageCtrl[i] = NULL;
		}
	}

	m_dotXPos = 0;
}

void GUI::CScheduleListCtrl::SetPageSize(int size)
{
	if (size > 0)
	{
		m_pageSize = size;
		CreateCtrlBtn();
	} 
	else
	{
		assert(false);
	}
}

void GUI::CScheduleListCtrl::CreateCtrlBtn()
{
	assert(m_pageSize > 0);

	int y = m_pageSize * m_length[SCHEDULE_ITEM_HEIGHT] + m_length[SCHEDULE_HEADER_HEIGHT] + 1;
	int x = m_length[EDIT_XPOS];

	assert(NULL == m_pWndPageCtrl[PAGECTRL_STATIC]);
	m_pWndPageCtrl[PAGECTRL_STATIC] = new GUI::CStatic;
	dynamic_cast<GUI::CStatic*>(m_pWndPageCtrl[PAGECTRL_STATIC])->Create(CTRLID_STATIC, x, y, 100, m_length[STATIC_HEIGHT], this, "1/1");

	unsigned long id[PAGECTRL_COUNT] = {CTRLID_STATIC, CTRLID_SCHEDULE_FIRST, CTRLID_SCHEDULE_PREV, CTRLID_SCHEDULE_NEXT, CTRLID_SCHEDULE_LAST};

	int button_width = 40;
	x = m_rect.Width() - button_width - 5;
	for (int i = PAGECTRL_BTN_LAST; i >= PAGECTRL_BTN_FIRST; --i)
	{
		assert(NULL == m_pWndPageCtrl[i]);
		m_pWndPageCtrl[i] = new GUI::CButton;
		dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[i])->Create(id[i], x, y, button_width, m_length[STATIC_HEIGHT], this, i, 0);
		x -= (button_width + 5);
	}

	//m_pWndPageCtrl[PAGECTRL_BTN_FIRST]->SetCaption("First!", false);
	//m_pWndPageCtrl[PAGECTRL_BTN_PREV]->SetCaption("Prev!", false);
	//m_pWndPageCtrl[PAGECTRL_BTN_NEXT]->SetCaption("Next!", false);
	//m_pWndPageCtrl[PAGECTRL_BTN_LAST]->SetCaption("Last!", false);

	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_FIRST])->SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_LEFTMOST,false);
	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_FIRST])->SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_LEFTMOST_FOCUS,false);
	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_FIRST])->SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_LEFTMOST_DISABLE,false);

	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_PREV])->SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_LEFT,false);
	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_PREV])->SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_LEFT_FOCUS,false);
	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_PREV])->SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_LEFT_DISABLE,false);

	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_NEXT])->SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_RIGHT,false);
	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_NEXT])->SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_RIGHT_FOCUS,false);
	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_NEXT])->SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_RIGHT_DISABLE,false);

	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_LAST])->SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_RIGHTMOST,false);
	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_LAST])->SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_RIGHTMOST_FOCUS,false);
	dynamic_cast<GUI::CButton*>(m_pWndPageCtrl[PAGECTRL_BTN_LAST])->SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_RIGHTMOST_DISABLE,false);
}

int CScheduleListCtrl::GetPageCount() const
{
	if (SHOW_ALL_ITEM != m_pageSize)
	{
		assert(m_pageSize > 0);
		int page = m_dataCtrlMap.size() / m_pageSize;
		if( 0 != m_dataCtrlMap.size() % m_pageSize ) page += 1;
		return page;
	} 
	else
	{
		return 1;
	}
}

void CScheduleListCtrl::SetCurPage(int curPage,bool bRefresh)
{
	if ((SHOW_ALL_ITEM != m_pageSize) && (curPage >= 0) && (curPage < GetPageCount()))
	{
		
		int last_begin = m_curPage * m_pageSize;
		int last_end = last_begin + m_pageSize - 1;
		int cur_begin = curPage * m_pageSize;
		int cur_end = cur_begin + m_pageSize - 1;

		int item = 0;
		DATA_CTRL_MAP::iterator iter;
		for (iter = m_dataCtrlMap.begin(); iter != m_dataCtrlMap.end(); ++iter, ++item)
		{
			if ((item >= last_begin) && (item <= last_end))
			{
				iter->second->Show(false, false);
			}

			if ((item >= cur_begin) && (item <= cur_end))
			{
				iter->second->MoveWnd(m_rect.m_left, m_rect.m_top + m_length[SCHEDULE_HEADER_HEIGHT] + (m_length[SCHEDULE_ITEM_HEIGHT] - 1)*(item - cur_begin));
				iter->second->Show(true, false);
			}
		}

		m_curPage = curPage;
		SetPageInfo();

		if( bRefresh )
		{
			Repaint();
		}
	} 
	else
	{
		assert(false);
	}
}

void CScheduleListCtrl::SetPageInfo()
{
	if (SHOW_ALL_ITEM != m_pageSize)
	{
		char sz[64] = {0};
		snprintf(sz, 64, "%d/%d", m_curPage + 1, GetPageCount());

		if (NULL != m_pWndPageCtrl[PAGECTRL_STATIC])
		{
			m_pWndPageCtrl[PAGECTRL_STATIC]->SetCaption(sz, false);
		} 
		else
		{
			assert(false);
		}
	}	
}


BEGIN_MSG_MAP(CScheduleListCtrl, GUI::CWnd)
	ON_MSG(CTRLID_SCHEDULE_FIRST, KEY_VIRTUAL_ENTER, OnClickBtnFirst)
	ON_MSG(CTRLID_SCHEDULE_PREV, KEY_VIRTUAL_ENTER, OnClickBtnPrev)
	ON_MSG(CTRLID_SCHEDULE_NEXT, KEY_VIRTUAL_ENTER, OnClickBtnNext)
	ON_MSG(CTRLID_SCHEDULE_LAST, KEY_VIRTUAL_ENTER, OnClickBtnLast)
END_MSG_MAP()

unsigned long CScheduleListCtrl::OnClickBtnFirst()
{
	if( !(m_dataCtrlMap.size() > 0) )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (SHOW_ALL_ITEM != m_pageSize )
	{
		SetCurPage(0);
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
}

unsigned long CScheduleListCtrl::OnClickBtnPrev()
{
	if (SHOW_ALL_ITEM != m_pageSize )
	{
		int page = m_curPage - 1;
		if (page >= 0)
		{
			SetCurPage(page);
		}		
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
}

unsigned long CScheduleListCtrl::OnClickBtnNext()
{
	if (SHOW_ALL_ITEM != m_pageSize )
	{
		int page = m_curPage + 1;
		if (page < GetPageCount())
		{
			SetCurPage(page);
		}		
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
}

unsigned long CScheduleListCtrl::OnClickBtnLast()
{
	if( !(m_dataCtrlMap.size() > 0) )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (SHOW_ALL_ITEM != m_pageSize )
	{
		int page = GetPageCount() - 1;
		if (page != m_curPage)
		{
			SetCurPage(page);
		}		
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
}
