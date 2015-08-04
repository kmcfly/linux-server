#include "DateCtrl.h"

//////////////////////////////////////////////////////////////////////////
// Static
//////////////////////////////////////////////////////////////////////////
unsigned long GUI::CDateCtrl::s_date_mode = CDateCtrl::MODE_YMD | CDateCtrl::CHAR_SLANT;
std::vector<GUI::CDateCtrl*>	GUI::CDateCtrl::s_vec_datectrl;

void GUI::CDateCtrl::SetDateMode(unsigned long mode,bool bRefresh/*=true*/)
{
	s_date_mode = mode;

	std::vector<CDateCtrl*>::iterator it;
	for ( it=s_vec_datectrl.begin(); it!=s_vec_datectrl.end(); it++ )
	{
		//如果这里出错,检查对应窗口的生命周期
		(*it)->RefreshWndPos(false);
		(*it)->RefreshView(SECTION_ALL,bRefresh);
	}
}

unsigned long GUI::CDateCtrl::GetDateMode()
{
	return s_date_mode;
}

unsigned long GUI::CDateCtrl::DateMode(unsigned long value)
{
	unsigned long mode = s_date_mode;
	if (DATE_MODE_YMD == value)
	{
		mode &= ~(0xf);
		mode |= CDateCtrl::MODE_YMD;
	}
	else if (DATE_MODE_MDY == value)
	{
		mode &= ~(0xf);
		mode |= CDateCtrl::MODE_MDY;
	}
	else
	{
		mode &= ~(0xf);
		mode |= CDateCtrl::MODE_DMY;
	}

	return mode;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
GUI::CDateCtrl::CDateCtrl() : 
m_year(2008),
m_month(0),
m_day(0),
m_bInitRange(false),
m_ctrlSection(SECTION_ALL),
m_preDayRangeMax(-1)
//m_bDrawBg(true),
//m_bDrawBorder(true)
{
	m_ctrlType = WND_TYPE_DATE;
}

GUI::CDateCtrl::~CDateCtrl()
{

}

void GUI::CDateCtrl::OnInitial()
{
	CWnd::OnInitial();

	unsigned long x=0,y=0;

	m_neNum[0].Create(GetRandWndID(),x,y,WIDTH_YEAR,HEIGHT,this,0,0,true,false,true);
	m_neNum[0].AtuoClear(true);

	m_neNum[1].Create(GetRandWndID(),x,y,WIDTH_MON,HEIGHT,this,1,0,true,false,true);
	m_neNum[1].AtuoClear(true);

	m_neNum[2].Create(GetRandWndID(),x,y,WIDTH_DAY,HEIGHT,this,2,0,true,false,true);
	m_neNum[2].AtuoClear(true);

	m_scSymbol[0].Create(GetRandWndID(),x,y,WIDTH_SYM,HEIGHT,this);
	m_scSymbol[0].SetAlignText(ALIGN_TEXT_CENTER);
	m_scSymbol[1].Create(GetRandWndID(),x,y,WIDTH_SYM,HEIGHT,this);
	m_scSymbol[1].SetAlignText(ALIGN_TEXT_CENTER);

	if( m_ctrlSection&SECTION_BTN )
	{
		m_btnCalendar.Create(GetRandWndID(),x,y,WIDTH_BTN,HEIGHT,this,3,0);
		m_btnCalendar.SetImage(PAINT_STATUS_NORMAL,BMP_DATE_BTN_CALENDAR,false);
		m_btnCalendar.SetImage(PAINT_STATUS_FOCUS,BMP_DATE_BTN_CALENDAR_FOCUS,false);
		m_btnCalendar.SetImage(PAINT_STATUS_DISABLE,BMP_DATE_BTN_CALENDAR_DISABLE,false);
	}

	if( m_ctrlSection&SECTION_INPUT_NOR )
	{
		m_neNum[0].SetEnable(false,false);
		m_neNum[1].SetEnable(false,false);
		m_neNum[2].SetEnable(false,false);
	}

	if( (m_ctrlSection&SECTION_BTN) && !(m_ctrlSection&SECTION_INPUT_CAL) )
	{
		m_btnCalendar.SetEnable(false,false);
	}

	//调整窗口位置
	RefreshWndPos(false);

	//检查数据
	CheckDate(m_year,m_month,m_day);

	//刷新数据
	RefreshView(SECTION_ALL,false);

	//加入被通知控件
	if( !(m_ctrlSection & SECTION_NOTIFY_NO) )
	{
		s_vec_datectrl.push_back(this);
	}
}

bool GUI::CDateCtrl::Create(unsigned long ctrlID, unsigned long x, unsigned long y,unsigned long cx, unsigned long cy,
											CWnd* pParent,int xFocus,int yFocus,unsigned long section/*=SECTION_ALL*/)
{
	unsigned long style = WND_CHILD | WND_VISIBLE;
	m_ctrlSection = section;

	cy = HEIGHT;
	if( m_ctrlSection&SECTION_BTN )
	{
		cx = (cx>=WIDTH_YMD_BTN) ? cx:WIDTH_YMD_BTN;
	}
	else
	{
		cx = (cx>=WIDTH_YMD)?cx:WIDTH_YMD;
	}

	//计算各个部分位置
	AutoCalculateSize( cx, cy );

	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CDateCtrl::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_DISABLE);
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_WHITE);
}

void GUI::CDateCtrl::OnDestroy()
{
	CWnd::OnDestroy();

	if( !(m_ctrlSection & SECTION_NOTIFY_NO) )
	{
		std::vector<CDateCtrl*>::iterator it;
		for ( it=s_vec_datectrl.begin(); it!=s_vec_datectrl.end(); it++ )
		{
			if( (*it) == this )
			{
				s_vec_datectrl.erase(it);
				break;
			}
		}
	}
}

void GUI::CDateCtrl::OnChangeFocus(unsigned long oldID, unsigned long newID)
{

}

void GUI::CDateCtrl::AddToNotifyList()
{
	m_ctrlSection &= ~SECTION_NOTIFY_NO;

	s_vec_datectrl.push_back(this);
}

GUI::CDateCtrl& GUI::CDateCtrl::SetDate (int value, unsigned long section/*=SECTION_YEAR*/, bool bRefresh/*=false*/)
{
	if( section&SECTION_YEAR )
	{
		m_year = value;
	}

	if( section&SECTION_MON )
	{
		m_month = value;
	}

	if( section&SECTION_DAY )
	{
		m_day = value;
	}

	CheckDate(m_year,m_month,m_day);
	RefreshView(SECTION_NUM,bRefresh);
	return *this;
}

GUI::CDateCtrl& GUI::CDateCtrl::GetDate (int& value, unsigned long section/*=SECTION_YEAR*/)
{
	if( section&SECTION_YEAR )
	{
		value = m_year;
		return *this;
	}

	if( section&SECTION_MON )
	{
		value = m_month;
		return *this;
	}

	if( section&SECTION_DAY )
	{
		value = m_day;
		return *this;
	}

	assert(false);
	return *this;
}

//void GUI::CDateCtrl::SetDate(tm time,bool bRefresh /*= true*/)
//{
//	SetDate(time.tm_year+1900,SECTION_YEAR,false);
//	SetDate(time.tm_mon,SECTION_MON,false);
//	SetDate(time.tm_mday-1,SECTION_DAY,bRefresh);
//}
//
//tm GUI::CDateCtrl::GetDate()
//{
//	tm time;
//	memset(&time,0,sizeof(tm));
//	
//	GetDate(time.tm_year,SECTION_YEAR);
//	GetDate(time.tm_mon,SECTION_MON);
//	GetDate(time.tm_mday,SECTION_DAY);
//	time.tm_year -= 1900;
//	time.tm_mday += 1;
//
//	return time;
//}

//void GUI::CDateCtrl::SetDVRDate(unsigned long time, bool bRefresh /*= true*/)
//{
//	SetDate(DVRTime32ToTm(time),bRefresh);
//}
//
//unsigned long GUI::CDateCtrl::GetDVRDate()
//{
//	return TmToDVRTime32(GetDate());
//}

void GUI::CDateCtrl::SetDate(int year,int mon,int day,bool bRefresh/*=true*/)
{
	assert(day>0);
	SetDate(year,SECTION_YEAR,false);
	SetDate(mon,SECTION_MON,false);
	SetDate(day-1,SECTION_DAY,bRefresh);
}

void GUI::CDateCtrl::GetDate(int&year, int& mon, int& day)
{
	int y,m,d;
	GetDate(y,SECTION_YEAR);
	GetDate(m,SECTION_MON);
	GetDate(d,SECTION_DAY);

	year = y;
	mon = m;
	day = d + 1;
}

int GUI::CDateCtrl::GetYear()
{
	int year = 2008;
	GetDate(year,SECTION_YEAR);
	return year;
}

int GUI::CDateCtrl::GetMonth()
{
	int mon = 0;
	GetDate(mon,SECTION_MON);
	return mon;
}

int GUI::CDateCtrl::GetDay()
{
	int day = 0;
	GetDate(day,SECTION_DAY);
	return day+1;
}

void GUI::CDateCtrl::RefreshWndPos(bool bRefresh/*=true*/)
{
	int x = m_rect.m_left + 1;
	int y = m_rect.m_top;
	int btn_off = 2;
	int x_off = 0;

	int y_off = 0;
	if( s_date_mode&CHAR_MINUS ) y_off = -1;
	if( s_date_mode&CHAR_SLANT ) y_off = 1;
	if( s_date_mode&CHAR_DOT ) y_off = 0;

	if( s_date_mode & MODE_YMD )
	{
		x_off = (m_wndRect[0].Width()-WIDTH_YEAR)/2;
		m_neNum[0].MoveWnd(x+x_off,y);			//年
		x += m_wndRect[0].Width();
		x += 2;

		x_off = (m_wndRect[1].Width()-WIDTH_SYM)/2;
		m_scSymbol[0].MoveWnd(x+x_off,y+y_off);		//1分隔符
		x += m_wndRect[1].Width();
		x += 1;

		x_off = (m_wndRect[2].Width()-WIDTH_MON)/2;
		m_neNum[1].MoveWnd(x+x_off,y);			//月
		x += m_wndRect[2].Width();
		x += 2;

		x_off = (m_wndRect[3].Width()-WIDTH_SYM)/2;
		m_scSymbol[1].MoveWnd(x+x_off,y+y_off);		//2分隔符
		x += m_wndRect[3].Width();
		x += 1;

		x_off = (m_wndRect[4].Width()-WIDTH_DAY)/2;
		m_neNum[2].MoveWnd(x+x_off,y);			//日
		x += m_wndRect[4].Width();
		x += 2;

		if( m_ctrlSection&SECTION_BTN )
		{
			m_btnCalendar.MoveWnd(m_rect.m_left + m_rect.Width()-2-WIDTH_BTN,y+btn_off);		//日历按钮
		}

		if( bRefresh )
		{
			Repaint();
		}
		return;
	}

	if( s_date_mode & MODE_MDY )
	{
		x_off = (m_wndRect[2].Width()-WIDTH_MON)/2;
		m_neNum[1].MoveWnd(x+x_off,y);			//月
		x += m_wndRect[2].Width();
		x += 2;

		x_off = (m_wndRect[1].Width()-WIDTH_SYM)/2;
		m_scSymbol[0].MoveWnd(x+x_off,y+y_off);		//1分隔符
		x += m_wndRect[1].Width();
		x += 1;

		x_off = (m_wndRect[4].Width()-WIDTH_DAY)/2;
		m_neNum[2].MoveWnd(x+x_off,y);			//日
		x += m_wndRect[4].Width();
		x += 2;

		x_off = (m_wndRect[3].Width()-WIDTH_SYM)/2;
		m_scSymbol[1].MoveWnd(x+x_off,y+y_off);		//2分隔符
		x += m_wndRect[3].Width();
		x += 1;

		x_off = (m_wndRect[0].Width()-WIDTH_YEAR)/2;
		m_neNum[0].MoveWnd(x+x_off,y);			//年
		x += m_wndRect[0].Width();
		x += 2;

		if( m_ctrlSection&SECTION_BTN )
		{
			m_btnCalendar.MoveWnd(m_rect.m_left + m_rect.Width()-2-WIDTH_BTN,y+btn_off);		//日历按钮
		}

		if( bRefresh )
		{
			Repaint();
		}
		return;
	}

	if( s_date_mode & MODE_DMY )
	{
		x_off = (m_wndRect[4].Width()-WIDTH_DAY)/2;
		m_neNum[2].MoveWnd(x+x_off,y);			//日
		x += m_wndRect[4].Width();
		x += 2;

		x_off = (m_wndRect[1].Width()-WIDTH_SYM)/2;
		m_scSymbol[0].MoveWnd(x+x_off,y+y_off);		//1分隔符
		x += m_wndRect[1].Width();
		x += 1;

		x_off = (m_wndRect[2].Width()-WIDTH_MON)/2;
		m_neNum[1].MoveWnd(x+x_off,y);			//月
		x += m_wndRect[2].Width();
		x += 2;

		x_off = (m_wndRect[3].Width()-WIDTH_SYM)/2;
		m_scSymbol[1].MoveWnd(x+x_off,y+y_off);		//2分隔符
		x += m_wndRect[3].Width();
		x += 1;

		x_off = (m_wndRect[0].Width()-WIDTH_YEAR)/2;
		m_neNum[0].MoveWnd(x+x_off,y);			//年
		x += m_wndRect[0].Width();
		x += 2;

		if( m_ctrlSection&SECTION_BTN )
		{
			m_btnCalendar.MoveWnd(m_rect.m_left + m_rect.Width()-2-WIDTH_BTN,y+btn_off);		//日历按钮
		}

		if( bRefresh )
		{
			Repaint();
		}
		return;
	}

	assert(false);
}

void GUI::CDateCtrl::RefreshView(unsigned long section/*=SECTION_NUM*/,bool bRefresh/*=true*/)
{
	if( section&SECTION_YEAR )
	{
		m_neNum[0].SetValue(m_year,false);
	}

	if( section&SECTION_MON )
	{
		m_neNum[1].SetValue(m_month+1,false);
	}

	if( section&SECTION_DAY )
	{
		m_neNum[2].SetValue(m_day+1,false);
	}

	if( (s_date_mode&CHAR_MINUS) )
	{
		if(section&SECTION_SYM1) m_scSymbol[0].SetCaption("-",false);
		if(section&SECTION_SYM2) m_scSymbol[1].SetCaption("-",false);
	}
	if( s_date_mode&CHAR_SLANT )
	{
		if(section&SECTION_SYM1) m_scSymbol[0].SetCaption("/",false);
		if(section&SECTION_SYM2) m_scSymbol[1].SetCaption("/",false);
	}
	if( s_date_mode&CHAR_DOT )
	{
		if(section&SECTION_SYM1) m_scSymbol[0].SetCaption(".",false);
		if(section&SECTION_SYM2) m_scSymbol[1].SetCaption(".",false);
	}

	if( bRefresh )
	{
		Repaint();
	}
}

void GUI::CDateCtrl::RefreshData(unsigned long section/*=SECTION_NUM*/)
{
	if( section&SECTION_YEAR )
	{
		m_year = m_neNum[0].GetValue();
	}

	if( section&SECTION_MON )
	{
		m_month = m_neNum[1].GetValue()-1;
	}

	if( section&SECTION_DAY )
	{
		m_day = m_neNum[2].GetValue()-1;
	}
}

void GUI::CDateCtrl::AutoCalculateSize(unsigned long cx,unsigned long cy)
{
	int len = 0;
	if( !(m_ctrlSection&SECTION_BTN) )
	{
		len = WIDTH_BTN/11;
	}

	assert(len>=0);
	m_wndRect[0].SetRect(0,0,WIDTH_YEAR+len*3,HEIGHT-1);		//年输入框大小
	m_wndRect[1].SetRect(0,0,WIDTH_SYM+len,HEIGHT);			//分隔符大小
	m_wndRect[2].SetRect(0,0,WIDTH_MON+len*3,HEIGHT-1);		//月输入框大小
	m_wndRect[3].SetRect(0,0,WIDTH_SYM+len,HEIGHT);			//分隔符大小
	m_wndRect[4].SetRect(0,0,WIDTH_DAY+len*3,HEIGHT-1);		//日输入框大小
	m_wndRect[5].SetRect(0,0,WIDTH_BTN,HEIGHT);
}

void GUI::CDateCtrl::CheckDate(int& year, int& mon, int& day)
{
	//日期范围与GUI::CalendarCtrl.cpp一致,[2008~2025]
	if( year <= 2008 ) year = 2008;
	if( year >= 2025 ) year = 2025;

	if( mon <= 0 ) mon = 0;
	if( mon >= 11 ) mon = 11;

	if( !m_bInitRange )
	{
		//放在这里设置范围主要是为了,检查方便
		m_neNum[0].SetRange(2008,2025);
		m_neNum[1].SetRange(1,12);
		m_bInitRange = true;
	}

	int max_day = m_calendar.GetMonthDays(year,mon+1);
	assert(max_day>0);

	if( day <= 0 ) day = 0;
	if( day >= max_day ) day = max_day-1;

	if( m_preDayRangeMax != max_day )
	{
		m_neNum[2].SetRange(1,max_day);
		m_preDayRangeMax = max_day;
	}
}

//void GUI::CDateCtrl::SetDrawBg(bool draw/*=true*/)
//{
//	m_bDrawBg = draw;
//}
//
//void GUI::CDateCtrl::SetDrawBorder(bool draw/*=true*/)
//{
//	m_bDrawBorder = draw;
//}

BEGIN_MSG_MAP(GUI::CDateCtrl, GUI::CWnd)
	ON_MSG(m_neNum[0].GetID(), KEY_VIRTUAL_ENTER, OnChangeYear)
	ON_MSG(m_neNum[1].GetID(), KEY_VIRTUAL_ENTER, OnChangeMonth)
	ON_MSG(m_neNum[2].GetID(), KEY_VIRTUAL_ENTER, OnChangeDay)
	ON_MSG(m_btnCalendar.GetID(), KEY_VIRTUAL_ENTER, OnBtnCalendar)
END_MSG_MAP()

unsigned long GUI::CDateCtrl::OnChangeYear()
{
	int year = 0;
	GetDate(year,SECTION_YEAR);

	if( year != m_neNum[0].GetValue() )
	{
		RefreshData(SECTION_YEAR);
		CheckDate(m_year,m_month,m_day);
		RefreshView(SECTION_NUM,true);
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CDateCtrl::OnChangeMonth()
{
	int mon = 0;
	GetDate(mon,SECTION_MON);

	if( mon != m_neNum[1].GetValue() )
	{
		RefreshData(SECTION_MON);
		CheckDate(m_year,m_month,m_day);
		RefreshView(SECTION_NUM,true);
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CDateCtrl::OnChangeDay()
{
	int day = 0;
	GetDate(day,SECTION_DAY);

	if( day != m_neNum[2].GetValue() )
	{
		RefreshData(SECTION_DAY);
		CheckDate(m_year,m_month,m_day);
		RefreshView(SECTION_NUM,true);
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CDateCtrl::OnBtnCalendar()
{
	GUI::CRect rect = GetWindowRect();
	unsigned long top = rect.m_bottom + 2;
	unsigned long left = rect.m_right + 2;

	//如果超出屏幕高度和宽度
	if (top + m_length[CALENDARDLG_HEIGHT] >= (m_height - 40))
	{
		if(rect.m_top > m_length[CALENDARDLG_HEIGHT] + 2)
		{
			top = rect.m_top - m_length[CALENDARDLG_HEIGHT]-2;
		}
		else
		{
			//CVBS 极端情况,上下均不能完全放置时,直接采用距离顶部2
			top = 2;
		}
	}
	left = (left > m_length[CALENDARDLG_WIDTH]) ? (left - m_length[CALENDARDLG_WIDTH]) : 2;

	if( DLG_OK == m_calDlg.DoModal(this, left, top, m_length[CALENDARDLG_WIDTH],m_length[CALENDARDLG_HEIGHT]) )
	{
		m_year = m_calDlg.GetCalCtrl()->GetSelYear();
		m_month = m_calDlg.GetCalCtrl()->GetSelMonth();
		m_day = m_calDlg.GetCalCtrl()->GetSelDay();

		CheckDate(m_year,m_month, m_day);
		RefreshView(SECTION_NUM, true);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}
