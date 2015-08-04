#include "TimeCtrl.h"
#include "DateCtrl.h"
#include "GuiAssert.h"

using namespace  GUI;

//////////////////////////////////////////////////////////////////////////
// Static
//////////////////////////////////////////////////////////////////////////
unsigned long CTimeCtrl::s_time_mode = MODE_H24 | CHAR_COLON;
std::vector<CTimeCtrl*>	CTimeCtrl::s_vec_timectrl;

void CTimeCtrl::SetTimeMode(unsigned long mode,bool bRefresh/*=true*/)
{
	s_time_mode = mode;

	std::vector<CTimeCtrl*>::iterator it;
	for ( it=s_vec_timectrl.begin(); it!=s_vec_timectrl.end(); it++ )
	{
		//如果这里出错,检查对应窗口的生命周期
		(*it)->OnTimeModeChange(bRefresh);
	}
}

unsigned long CTimeCtrl::GetTimeMode()
{
	return s_time_mode;
}

unsigned long CTimeCtrl::TimeMode(unsigned long value)
{
	unsigned long mode = s_time_mode;
	if (TIME_MODE_12 == value)
	{
		mode &= ~(CTimeCtrl::MODE_H24);
		mode |= CTimeCtrl::MODE_H12;
	}
	else
	{
		mode &= ~(CTimeCtrl::MODE_H12);
		mode |= CTimeCtrl::MODE_H24;
	}

	return mode;
}

//////////////////////////////////////////////////////////////////////////
void CTimeCtrl::GetTime32Text(std::string & str, unsigned long ultm, unsigned long section)
{
	tm tmp = DVRTime32ToTm(ultm);
	GetTimeText(str,tmp,section);
}

std::string CTimeCtrl::GetTime32Text(unsigned long ultm, unsigned long section)
{
	std::string str;
	GetTime32Text(str,ultm,section);
	return str;
}

void CTimeCtrl::GetTimeText(std::string & str, const tm& time, unsigned long section)
{
	GetTimeText(str,time,CDateCtrl::GetDateMode(),GetTimeMode(),section);
}

//std::string CTimeCtrl::GetTimeText(const tm& time, unsigned long section /*= SECTION_ALL*/)
//{
//	std::string str;
//	GetTimeText(str,time,section);
//	return str;
//}

//void CTimeCtrl::GetTimeText(std::string & str,unsigned long time,unsigned long dateMode,unsigned long timeMode, unsigned long section)
//{
//	GetTimeText(str,DVRTime32ToTm(time),dateMode,timeMode,section);
//}

void CTimeCtrl::GetTimeText(std::string & str,const tm& time,unsigned long dateMode,unsigned long timeMode,unsigned long section)
{
	//str = asctime(&time);
	char buff[64] = {0};
	char dateSplit[8] = "-";
	char timeSplit[8] = ":";
	std::string formate;

	//设置间隔符号
	if( dateMode&CDateCtrl::CHAR_MINUS ) strcpy(dateSplit,"-");
	if( dateMode&CDateCtrl::CHAR_SLANT ) strcpy(dateSplit,"/");
	if( dateMode&CDateCtrl::CHAR_DOT ) strcpy(dateSplit,".");

	if( timeMode&CHAR_MINUS ) strcpy(timeSplit,"-");
	if( timeMode&CHAR_COLON ) strcpy(timeSplit,":" );
	if( timeMode&CHAR_DOT ) strcpy(timeSplit,".");

	//设置日期[年][月[日]排列方式
	if( dateMode&CDateCtrl::MODE_YMD )
	{
		if( (section&CDateCtrl::SECTION_YEAR) &&
			(section&CDateCtrl::SECTION_MON) &&
			(section&CDateCtrl::SECTION_DAY) )
		{
			formate += "%Y";
			formate += dateSplit;
			formate += "%m";
			formate += dateSplit;
			formate += "%d";
		}
		else if( (section&CDateCtrl::SECTION_YEAR) &&
					(section&CDateCtrl::SECTION_MON) )
		{
			formate += "%Y";
			formate += dateSplit;
			formate += "%m";
		}
		else if( (section&CDateCtrl::SECTION_MON) &&
					(section&CDateCtrl::SECTION_DAY) )
		{
			formate += "%m";
			formate += dateSplit;
			formate += "%d";
		}
		else
		{
			//assert(false);//检查日期组合格式
		}
	}
	else if( dateMode&CDateCtrl::MODE_MDY )
	{
		if( (section&CDateCtrl::SECTION_YEAR) &&
			(section&CDateCtrl::SECTION_MON) &&
			(section&CDateCtrl::SECTION_DAY) )
		{
			formate += "%m";
			formate += dateSplit;
			formate += "%d";
			formate += dateSplit;
			formate += "%Y";
		}
		else if( (section&CDateCtrl::SECTION_YEAR) &&
					(section&CDateCtrl::SECTION_MON) )
		{
			formate += "%m";
			formate += dateSplit;
			formate += "%Y";
		}
		else if( (section&CDateCtrl::SECTION_MON) &&
					(section&CDateCtrl::SECTION_DAY) )
		{
			formate += "%m";
			formate += dateSplit;
			formate += "%d";
		}
		else
		{
			//assert(false);//检查日期组合格式
		}
	}
	else if( dateMode&CDateCtrl::MODE_DMY )
	{
		if( (section&CDateCtrl::SECTION_YEAR) &&
			(section&CDateCtrl::SECTION_MON) &&
			(section&CDateCtrl::SECTION_DAY) )
		{
			formate += "%d";
			formate += dateSplit;
			formate += "%m";
			formate += dateSplit;
			formate += "%Y";
		}
		else if( (section&CDateCtrl::SECTION_YEAR) &&
			(section&CDateCtrl::SECTION_MON) )
		{
			formate += "%m";
			formate += dateSplit;
			formate += "%Y";
		}
		else if( (section&CDateCtrl::SECTION_MON) &&
			(section&CDateCtrl::SECTION_DAY) )
		{
			formate += "%d";
			formate += dateSplit;
			formate += "%m";
		}
		else
		{
			//assert(false);//检查日期组合格式
		}
	}
	else
	{
		assert(false);//检查输入日期模式
	}

	//日期时间中间间隔
	if( (section&CDateCtrl::SECTION_YEAR) ||
		(section&CDateCtrl::SECTION_MON) ||
		(section&CDateCtrl::SECTION_DAY) )
	{
		formate += " ";
	}

	//设置时间[时][分[秒][AM/PM]排列方式
	if( timeMode&MODE_H24 )
	{
		if( (section&CTimeCtrl::SECTION_HOUR) &&
			(section&CTimeCtrl::SECTION_MIN) &&
			(section&CTimeCtrl::SECTION_SEC) )
		{
			formate += "%H";
			formate += timeSplit;
			formate += "%M";
			formate += timeSplit;
			formate += "%S";
		}
		else if( (section&CTimeCtrl::SECTION_HOUR) &&
					(section&CTimeCtrl::SECTION_MIN) )
		{
			formate += "%H";
			formate += timeSplit;
			formate += "%M";
		}
		else if( (section&CTimeCtrl::SECTION_MIN) &&
					(section&CTimeCtrl::SECTION_SEC) )
		{
			formate += "%M";
			formate += timeSplit;
			formate += "%S";
		}
		else
		{
			//assert(false);//检查时间组合格式
		}
	}
	else
	{
		if( (section&CTimeCtrl::SECTION_HOUR) &&
			(section&CTimeCtrl::SECTION_MIN) &&
			(section&CTimeCtrl::SECTION_SEC) )
		{
			formate += "%I";
			formate += timeSplit;
			formate += "%M";
			formate += timeSplit;
			formate += "%S";
			formate += " ";
			formate += "%p";
		}
		else if( (section&CTimeCtrl::SECTION_HOUR) &&
			(section&CTimeCtrl::SECTION_MIN) )
		{
			formate += "%I";
			formate += timeSplit;
			formate += "%M";
			formate += " ";
			formate += "%p";
		}
		else if( (section&CTimeCtrl::SECTION_MIN) &&
			(section&CTimeCtrl::SECTION_SEC) )
		{
			formate += "%M";
			formate += timeSplit;
			formate += "%S";
		}
		else
		{
			//assert(false);//检查时间组合格式
		}
	}

	size_t len = strftime(buff,sizeof(buff),formate.c_str(),&time);
	assert( len <= sizeof(buff) );

	if( len > 0 )
	{
		str = buff;
	}
	else
	{
		printf("(%s,%d)time text = \"\"\n",__FILE__,__LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
CApmButton::CApmButton()
{
	m_color = COLOR_EDIT;
}

CApmButton::~CApmButton()
{

}

void CApmButton::OnPaint(PAINT_STATUS status)
{	
	if (PAINT_STATUS_FOCUS == status)
	{
		GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), m_color_focus);
	}
	else
	{
		GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), m_color);
	}

	if (m_bDrawBorder)
	{
		GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_right, m_rect.m_top, COLOR_LINE_WHITE, 1);
		GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_bottom, m_rect.m_right, m_rect.m_bottom, COLOR_LINE_WHITE, 1);
	}

	if (m_strCaption.length() > 0)
	{
		assert (PAINT_STATUS_NULL != status);

		unsigned short xOffSet = 1;
		unsigned short yOffset = 1;
		if (ALIGN_TEXT_CENTER == m_alignText)
		{
			unsigned short textWidth = 0, textHeight = 0;
			if (!GUI::CWnd::TextSize(m_strCaption.c_str(), m_strCaption.length(), textWidth, textHeight))
			{
				assert(false);
			}

			xOffSet = (textWidth < m_rect.Width()) ? ((m_rect.Width() - textWidth) / 2) : 0;
			yOffset = (textHeight < m_rect.Height()) ? ((m_rect.Height() - textHeight) / 2) : 0;
		} 
		else if (ALIGN_TEXT_RIGHT == m_alignText)
		{
			unsigned short textWidth = 0, textHeight = 0;
			if (!GUI::CWnd::TextSize(m_strCaption.c_str(), m_strCaption.length(), textWidth, textHeight))
			{
				assert(false);
			}

			xOffSet = (textWidth < m_rect.Width()) ? (m_rect.Width() - textWidth) : 0;
			yOffset = (textHeight < m_rect.Height()) ? (m_rect.Height() - textHeight) : 0;
		}

		if (PAINT_STATUS_NORMAL == status)
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left + xOffSet, m_rect.m_top + yOffset, m_rect.Width()-xOffSet, m_rect.Height()-yOffset);
		}
		else if (PAINT_STATUS_FOCUS == status)
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left + xOffSet + 1, m_rect.m_top + yOffset + 1, m_rect.Width()-xOffSet, m_rect.Height()-yOffset);
		}
		else
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left + xOffSet, m_rect.m_top + yOffset, m_rect.Width()-xOffSet, m_rect.Height()-yOffset);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

CTimeCtrl::CTimeCtrl() :
m_hour(0),
m_minute(0),
m_second(0),
m_bInitRange(false),
m_ctrlSection(SECTION_ALL),
m_bAM(true)
//m_bDrawBg(true),
//m_bDrawBorder(true)
{
	m_ctrlType = WND_TYPE_TIME;
}

CTimeCtrl::~CTimeCtrl()
{

}

bool CTimeCtrl::Create( unsigned long ctrlID, 
					unsigned long x, 
					unsigned long y,
					unsigned long cx, 
					unsigned long cy,
					CWnd* pParent,
					int xFocus,
					int yFocus,unsigned long section/*=SECTION_ALL*/)
{
	//设置对话框风格
	unsigned long style = WND_CHILD | WND_VISIBLE;
	
	m_ctrlSection = section;
	
	//创建窗口
	if( (m_ctrlSection&SECTION_HOUR) &&
		(m_ctrlSection&SECTION_MIN) &&
		(m_ctrlSection&SECTION_SEC) )
	{
		cx = (cx>WIDTH_HMS)?cx:WIDTH_HMS;
	}
	else
	{
		cx = (cx>WIDTH_HM)?cx:WIDTH_HM;
	}

	cy = HEIGHT;

	AutoCalculateSize(cx,cy);

	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CTimeCtrl::OnInitial()
{
	CWnd::OnInitial();

	unsigned long x=0, y=0;
	unsigned long cx=0,cy=HEIGHT;
	
	if( m_ctrlSection&SECTION_HOUR )
	{
		cx = WIDTH_HOUR;
		m_neNum[0].Create(GetRandWndID(),x,y,cx,cy,this,0,0,true,false,true);
		m_neNum[0].AtuoClear(true);
		m_neNum[0].EnableAddDec(true);
	}

	if( m_ctrlSection&SECTION_MIN )
	{
		cx = WIDTH_MIN;
		m_neNum[1].Create(GetRandWndID(),x,y,cx,cy,this,1,0,true,false,true);
		m_neNum[1].AtuoClear(true);
		m_neNum[1].EnableAddDec(true);
	}

	if( m_ctrlSection&SECTION_SEC )
	{
		cx = WIDTH_SEC;
		m_neNum[2].Create(GetRandWndID(),x,y,cx,cy,this,2,0,true,false,true);
		m_neNum[2].AtuoClear(true);
		m_neNum[2].EnableAddDec(true);
	}

	if( (m_ctrlSection&SECTION_HOUR) &&
		(m_ctrlSection&SECTION_MIN) &&
		(m_ctrlSection&SECTION_SEC) )
	{
		cx = WIDTH_SYM;
		m_scSymbol[0].Create(GetRandWndID(),x,y,cx,cy,this);
		m_scSymbol[0].SetAlignText(ALIGN_TEXT_CENTER);

		cx = WIDTH_SYM;
		m_scSymbol[1].Create(GetRandWndID(),x,y,cx,cy,this);
		m_scSymbol[1].SetAlignText(ALIGN_TEXT_CENTER);
	}
	else
	{
		cx = WIDTH_SYM;
		m_scSymbol[0].Create(GetRandWndID(),x,y,cx,cy,this);
		m_scSymbol[0].SetAlignText(ALIGN_TEXT_CENTER);
	}

	cx = WIDTH_APM;
	m_btnApm.Create(GetRandWndID(),x,y,cx,cy,this,3,0,NULL,WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnApm.SetCaption("AM",false);

	if( s_time_mode & MODE_H24 )
	{
		m_btnApm.Show(false, false);
	}
	else
	{
		m_btnApm.Show(true, false);
	}

	if(0 == (m_ctrlSection & SECTION_INPUT_NOR) )
	{
		m_neNum[0].SetEnable(false,false);
		m_neNum[1].SetEnable(false,false);
		m_neNum[2].SetEnable(false,false);
		m_btnApm.SetEnable(false,false);
	}

	//初始范围限定设定为无效,第一次设置时间时,即设定显示框范围
	m_bInitRange = false;
	m_preHourRangeMax = -1;

	//调整窗口位置
	RefreshWndPos(false);

	//检查数据
	CheckTime(m_hour,m_minute,m_second);

	//刷新数据
	RefreshView(SECTION_ALL,false);

	//加入被通知控件
	if( !(m_ctrlSection & SECTION_NOTIFY_NO) )
	{
		s_vec_timectrl.push_back(this);
	}
}

void CTimeCtrl::OnDestroy()
{
	GUI::CWnd::OnDestroy();

	if( !(m_ctrlSection & SECTION_NOTIFY_NO) )
	{
		std::vector<CTimeCtrl*>::iterator it;
		for ( it=s_vec_timectrl.begin(); it!=s_vec_timectrl.end(); it++ )
		{
			if( (*it) == this )
			{
				s_vec_timectrl.erase(it);
				break;
			}
		}
	}
}

void CTimeCtrl::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::OnPaint(status);

	//if( m_bDrawBg )
	//{
		GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_EDIT);
	//}

	//if( m_bDrawBorder )
	//{
		GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_WHITE);
	//}
	//else
	//{
	//	GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_right, m_rect.m_top, COLOR_LINE_WHITE);
	//	GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_bottom, m_rect.m_right, m_rect.m_bottom, COLOR_LINE_WHITE);
	//}
}

void CTimeCtrl::OnChangeFocus(unsigned long oldID, unsigned long newID)
{

}

//////////////////////////////////////////////////////////////////////////

void GUI::CTimeCtrl::AddToNotifyList()
{
	m_ctrlSection &= ~SECTION_NOTIFY_NO;

	s_vec_timectrl.push_back(this);
}


CTimeCtrl& CTimeCtrl::SetTime(int value, unsigned long section, bool bRefresh)
{
	gui_assert(value>=0,printf("param value=%d",value));
	if( section&SECTION_HOUR )
	{
		/*gui_trace_and(s_time_mode&MODE_H24,value>=24,printf("time mode=H24,param value=%d\n",value));
		gui_trace_and(s_time_mode&MODE_H12,value>12,printf("time mode=H12,param value=%d\n",value));*/
		assert(value<24);
		m_hour = value;
	}

	if( section&SECTION_MIN )
	{
		assert(value<60);
		m_minute = value;
	}

	if( section&SECTION_SEC )
	{
		assert(value<60);
		m_second = value;
	}

	CheckTime(m_hour,m_minute,m_second);
	RefreshView(SECTION_NUM,bRefresh);
	return *this;
}

CTimeCtrl& CTimeCtrl::GetTime(int& value, unsigned long section)
{
	if( section&SECTION_HOUR )
	{
		assert(m_hour<24);
		value = m_hour;
		return *this;
	}

	if( section&SECTION_MIN )
	{
		assert(m_minute<60);
		value = m_minute;
		return *this;
	}

	if( section&SECTION_SEC )
	{
		assert(m_second<60);
		value = m_second;
		return *this;
	}

	assert(false);
	return *this;
}

//设置、获取标准格式时间
//void CTimeCtrl::SetTime(tm time,bool bRefresh)
//{
//	SetTime(time.tm_hour,SECTION_HOUR,false);
//	SetTime(time.tm_min,SECTION_MIN,false);
//	SetTime(time.tm_sec,SECTION_SEC,bRefresh);
//}

//tm CTimeCtrl::GetTime()
//{
//	tm time;
//	memset(&time,0,sizeof(tm));
//
//	GetTime(time.tm_hour,SECTION_HOUR);
//	GetTime(time.tm_min,SECTION_MIN);
//	GetTime(time.tm_sec,SECTION_SEC);
//
//	return time;
//}

//设置、获取32位DVR日期
//void CTimeCtrl::SetDVRTime(unsigned long time, bool bRefresh)
//{
//	SetTime(DVRTime32ToTm(time),bRefresh);
//}

//unsigned long CTimeCtrl::GetDVRTime()
//{
//	//Windows和Linux 都是从1970年1月1日午夜开始的绝对时间
//	//这里返回的是[时:分:秒]所用的绝对时间长度值
//	//参考年为2008年1月1日
//	tm time1;
//	memset(&time1,0,sizeof(tm));
//	time1.tm_year = 2008 - 1900;
//	time1.tm_mday = 1;
//
//	tm time2 = GetTime();
//	time2.tm_year = 2008 - 1900;
//	time2.tm_mday = 1;
//	return TmToDVRTime32(time2) - TmToDVRTime32(time1);
//}

void CTimeCtrl::SetTime(unsigned long secTime,bool bRefresh)
{
	secTime %= (3600*24);
	int hour = secTime/3600;
	secTime %= 3600;
	int min = secTime/60;
	int sec = secTime%60;

	SetTime(hour,min,sec,bRefresh);
}

//直接设置、获取年月日
void CTimeCtrl::SetTime(int hour,int minute,int second,bool bRefresh)
{
	SetTime(hour,SECTION_HOUR,false);
	SetTime(minute,SECTION_MIN,false);
	SetTime(second,SECTION_SEC,bRefresh);
}

void CTimeCtrl::GetTime(int& hour, int& minute, int& second)
{
	int h,m,s;
	GetTime(h,SECTION_HOUR);
	GetTime(m,SECTION_MIN);
	GetTime(s,SECTION_SEC);

	hour = h;
	minute = m;
	second = s;
}

unsigned long CTimeCtrl::GetTime()
{
	int hour,min,sec;
	GetTime(hour,SECTION_HOUR);
	GetTime(min,SECTION_MIN);
	GetTime(sec,SECTION_SEC);
	return hour*3600+min*60+sec;
}

int CTimeCtrl::GetHour()
{
	int ret = 0;
	GetTime(ret,SECTION_HOUR);
	return ret;
}

int CTimeCtrl::GetMinute()
{
	int ret = 0;
	GetTime(ret,SECTION_MIN);
	return ret;
}

int CTimeCtrl::GetSecond()
{
	int ret = 0;
	GetTime(ret,SECTION_SEC);
	return ret;
}

//刷新显示位置区域
void CTimeCtrl::RefreshWndPos(bool bRefresh/*=true*/)
{
	int x = m_rect.m_left + 1;
	int y = m_rect.m_top;
	int x_off = 0;

	int y_off = 0;
	if( s_time_mode&CHAR_MINUS ) y_off = -1;
	if( s_time_mode&CHAR_COLON ) y_off = -1;
	if( s_time_mode&CHAR_DOT ) y_off = 0;

	//[时:分:秒]
	if( (m_ctrlSection&SECTION_HOUR) &&
		(m_ctrlSection&SECTION_MIN) &&
		(m_ctrlSection&SECTION_SEC) )
	{
		x_off = (m_wndRect[0].Width()-WIDTH_HOUR)/2;
		m_neNum[0].MoveWnd(x+x_off,y);			//时
		x += m_wndRect[0].Width();
		x += 2;

		x_off = (m_wndRect[1].Width()-WIDTH_SYM)/2;
		m_scSymbol[0].MoveWnd(x+x_off,y+y_off);		//1分隔符
		x += m_wndRect[1].Width();
		x += 1;

		x_off = (m_wndRect[2].Width()-WIDTH_MIN)/2;
		m_neNum[1].MoveWnd(x+x_off,y);			//分
		x += m_wndRect[2].Width();
		x += 2;

		x_off = (m_wndRect[3].Width()-WIDTH_SYM)/2;
		m_scSymbol[1].MoveWnd(x+x_off,y+y_off);		//2分隔符
		x += m_wndRect[3].Width();
		x += 1;

		x_off = (m_wndRect[4].Width()-WIDTH_SEC)/2;
		m_neNum[2].MoveWnd(x+x_off,y);			//秒
		x+= m_wndRect[4].Width();
		x += 1;

		if( s_time_mode&MODE_H24 )
		{
			m_btnApm.Show(false,false);
		}
		else
		{
			m_btnApm.Show(true,false);
			x_off = (m_wndRect[5].Width()-WIDTH_APM)/2;
			m_btnApm.MoveWnd(x+x_off,y);
		}

		if( bRefresh )
		{
			Repaint();
		}
		return;
	}

	//[时:分]
	if( (m_ctrlSection&SECTION_HOUR) &&
		(m_ctrlSection&SECTION_MIN) )
	{
		x_off = (m_wndRect[0].Width()-WIDTH_HOUR)/2;
		m_neNum[0].MoveWnd(x+x_off,y);			//时
		x += m_wndRect[0].Width();
		x += 2;

		x_off = (m_wndRect[1].Width()-WIDTH_SYM)/2;
		m_scSymbol[0].MoveWnd(x+x_off,y+y_off);		//1分隔符
		x += m_wndRect[1].Width();
		x += 1;

		x_off = (m_wndRect[2].Width()-WIDTH_MIN)/2;
		m_neNum[1].MoveWnd(x+x_off,y);			//分
		x += m_wndRect[2].Width();
		x += 1;

		if( s_time_mode&MODE_H24 )
		{
			m_btnApm.Show(false,true);
		}
		else
		{
			m_btnApm.Show(true,false);
			x_off = (m_wndRect[5].Width()-WIDTH_APM)/2;
			m_btnApm.MoveWnd(x+x_off,y);
		}
		
		if( bRefresh )
		{
			Repaint();
		}
		return;
	}

	//[分:秒]
	if( (m_ctrlSection&SECTION_MIN) &&
		(m_ctrlSection&SECTION_SEC) )
	{
		x_off = (m_wndRect[2].Width()-WIDTH_MIN)/2;
		m_neNum[1].MoveWnd(x+x_off,y);			//分
		x += m_wndRect[2].Width();
		x += 2;

		x_off = (m_wndRect[1].Width()-WIDTH_SYM)/2;
		m_scSymbol[0].MoveWnd(x+x_off,y+y_off);		//1分隔符
		x += m_wndRect[1].Width();
		x += 1;

		x_off = (m_wndRect[4].Width()-WIDTH_SEC)/2;
		m_neNum[2].MoveWnd(x+x_off,y);			//秒
		x += m_wndRect[4].Width();
		x += 1;

		if( s_time_mode&MODE_H24 )
		{
			m_btnApm.Show(false,true);
		}
		else
		{
			m_btnApm.Show(true,false);
			x_off = (m_wndRect[5].Width()-WIDTH_APM)/2;
			m_btnApm.MoveWnd(x+x_off,y);
		}

		if( bRefresh )
		{
			Repaint();
		}
		return;
	}

	assert(false);	//请检查组合格式是否符合以上条件
	return;
}

//刷新数据到界面
void CTimeCtrl::RefreshView(unsigned long section/*=SECTION_NUM*/,bool bRefresh/*=true*/)
{
	if( section&SECTION_HOUR )
	{
		int hour;
		if( s_time_mode&MODE_H24 )
		{
			hour = m_hour;
		}
		else
		{
			hour = (m_hour-12)>=0?(m_hour-12):m_hour;
			if( hour<=0 ) hour = 12;
		}

		m_neNum[0].SetValue(hour,false);
	}

	if( section&SECTION_MIN )
	{
		m_neNum[1].SetValue(m_minute,false);
	}

	if( section&SECTION_SEC )
	{
		m_neNum[2].SetValue(m_second,false);
	}

	if( (s_time_mode&CHAR_MINUS) )
	{
		if(section&SECTION_SYM1) m_scSymbol[0].SetCaption("-",false);
		if(section&SECTION_SYM2) m_scSymbol[1].SetCaption("-",false);
	}
	if( s_time_mode&CHAR_COLON )
	{
		if(section&SECTION_SYM1) m_scSymbol[0].SetCaption(":",false);
		if(section&SECTION_SYM2) m_scSymbol[1].SetCaption(":",false);
	}
	if( s_time_mode&CHAR_DOT )
	{
		if(section&SECTION_SYM1) m_scSymbol[0].SetCaption(".",false);
		if(section&SECTION_SYM2) m_scSymbol[1].SetCaption(".",false);
	}

	if( section&SECTION_APM )
	{
		if( m_bAM )
		{
			m_btnApm.SetCaption("AM",bRefresh);
		}
		else
		{
			m_btnApm.SetCaption("PM",bRefresh);
		}
	}

	if( bRefresh )
	{
		Repaint();
	}
}

//刷新界面数据到存储区
void CTimeCtrl::RefreshData(unsigned long section/*=SECTION_NUM*/)
{
	if( section&SECTION_HOUR )
	{
		int hour;
		if( s_time_mode&MODE_H24 )
		{
			hour = m_neNum[0].GetValue();
		}
		else
		{
			hour = m_neNum[0].GetValue();
			if ( hour>=12 && m_bAM )
			{
				hour = 0;
			}
			else if( hour>=12 && !m_bAM )
			{
				hour = 12;
			}
			else
			{
				hour = m_bAM?hour:hour+12;
			}
		}
		assert(hour>=0);
		m_hour = hour;
	}

	if( section&SECTION_MIN )
	{
		m_minute = m_neNum[1].GetValue();
	}

	if( section&SECTION_SEC )
	{
		m_second = m_neNum[2].GetValue();
	}
}

void CTimeCtrl::EnableAddDec(bool enable)
{
	m_neNum[0].EnableAddDec(enable);
	m_neNum[1].EnableAddDec(enable);
	m_neNum[2].EnableAddDec(enable);
}

void CTimeCtrl::Dump()
{
	printf("class name=\"CTimeCtrl\",id=%d.static s_time_mode=%X.\n",GetID(),s_time_mode);
	printf("m_hour=%d,m_minute=%d,m_second=%d,m_bAM=%d.\n",m_hour,m_minute,m_second,m_bAM);
	printf("m_bInitRange=%d,m_ctrlSection=0x%X.\n",m_bInitRange,m_ctrlSection);
}

void CTimeCtrl::AutoCalculateSize(unsigned long cx,unsigned long cy)
{
	//[时:分:秒]
	if( (m_ctrlSection&SECTION_HOUR) &&
		(m_ctrlSection&SECTION_MIN) &&
		(m_ctrlSection&SECTION_SEC) )
	{
		int len = 0;
		if( s_time_mode&MODE_H24 )
		{
			len = WIDTH_APM/11;
		}

		assert(len>=0);

		m_wndRect[0].SetRect(0,0,WIDTH_HOUR+len*3,HEIGHT-1);	//小时输入框大小
		m_wndRect[1].SetRect(0,0,WIDTH_SYM+len,HEIGHT);				//1分隔符大小
		m_wndRect[2].SetRect(0,0,WIDTH_MIN+len*3,HEIGHT-1);		//分钟输入框大小
		m_wndRect[3].SetRect(0,0,WIDTH_SYM+len,HEIGHT);				//2分隔符大小
		m_wndRect[4].SetRect(0,0,WIDTH_SEC+len*3,HEIGHT-1);		//秒输入框大小
		m_wndRect[5].SetRect(0,0,WIDTH_APM,HEIGHT);					//AM/PM输入框
		return;
	}

	//[时:分]
	if( (m_ctrlSection&SECTION_HOUR) &&
		(m_ctrlSection&SECTION_MIN) )
	{
		int len = 0;
		if( s_time_mode&MODE_H24 )
		{
			len = WIDTH_APM/7;
		}

		assert(len>=0);

		m_wndRect[0].SetRect(0,0,WIDTH_HOUR+len*3,HEIGHT-1);	//小时输入框大小
		m_wndRect[1].SetRect(0,0,WIDTH_SYM+len,HEIGHT);				//1分隔符大小
		m_wndRect[2].SetRect(0,0,WIDTH_MIN+len*3,HEIGHT-1);		//分钟输入框大小
		m_wndRect[5].SetRect(0,0,WIDTH_APM,HEIGHT);					//AM/PM输入框
		return;
	}

	//[分:秒]
	if( (m_ctrlSection&SECTION_MIN) &&
		(m_ctrlSection&SECTION_SEC) )
	{
		int len = 0;
		if( s_time_mode&MODE_H24 )
		{
			len = WIDTH_APM/7;
		}

		assert(len>=0);
		m_wndRect[2].SetRect(0,0,WIDTH_MIN+len*3,HEIGHT-1);		//分钟输入框大小
		m_wndRect[1].SetRect(0,0,WIDTH_SYM+len,HEIGHT);				//1分隔符大小
		m_wndRect[4].SetRect(0,0,WIDTH_SEC+len*3,HEIGHT-1);		//秒输入框大小
		m_wndRect[5].SetRect(0,0,WIDTH_APM,HEIGHT);					//AM/PM输入框
		return;
	}

	assert(false);	//请检查组合格式是否符合以上条件
	return;
}

void CTimeCtrl::CheckTime(int& hour, int& min, int& sec)
{
	if( min<0 ) min = 0;
	if( min>59 ) min = 59;

	if( sec<0 ) sec =0;
	if( sec>59 ) sec = 59;

	//调整输入框
	if( !m_bInitRange )
	{
		//放在这里设置范围主要是为了,检查方便
		m_neNum[1].SetRange(0,59);
		m_neNum[2].SetRange(0,59);
		m_bInitRange = true;
	}

	if( hour<0 ) hour = 0;
	if( hour>23 ) hour = 23;
	m_bAM = (hour>=12) ? false: true;		//是否处于上午

	//24小时制[0~23]
	//12小时制[1~12]
	if( s_time_mode&MODE_H24 )
	{
		if( m_preHourRangeMax != 23 )
		{
			m_neNum[0].SetRange(0,23);
			m_preHourRangeMax = 23;
		}
	}
	else
	{
		if( m_preHourRangeMax != 12 )
		{
			m_neNum[0].SetRange(1,12);
			m_preHourRangeMax = 12;
		}
	}
}

void CTimeCtrl::OnTimeModeChange(bool bRefresh)
{
	AutoCalculateSize(m_rect.Width(),m_rect.Height());
	RefreshWndPos(false);
	CheckTime(m_hour,m_minute,m_second);
	RefreshView(SECTION_ALL,bRefresh);
}

//void CTimeCtrl::SetDrawBg(bool draw/*=true*/)
//{
//	m_bDrawBg = draw;
//}
//
//void CTimeCtrl::SetDrawBorder(bool draw/*=true*/)
//{
//	m_bDrawBorder = draw;
//}

BEGIN_MSG_MAP(CTimeCtrl, GUI::CWnd)
	ON_MSG(m_neNum[0].GetID(), KEY_VIRTUAL_ENTER, OnChangeHour)
	ON_MSG(m_neNum[0].GetID(), KEY_VIRTUAL_ADD, OnChangeHour)
	ON_MSG(m_neNum[0].GetID(), KEY_VIRTUAL_DEC, OnChangeHour)
	ON_MSG(m_neNum[1].GetID(), KEY_VIRTUAL_ENTER, OnChangeMinute)
	ON_MSG(m_neNum[1].GetID(), KEY_VIRTUAL_ADD, OnChangeMinute)
	ON_MSG(m_neNum[1].GetID(), KEY_VIRTUAL_DEC, OnChangeMinute)
	ON_MSG(m_neNum[2].GetID(), KEY_VIRTUAL_ENTER, OnChangeSecond)
	ON_MSG(m_neNum[2].GetID(), KEY_VIRTUAL_ADD, OnChangeSecond)
	ON_MSG(m_neNum[2].GetID(), KEY_VIRTUAL_DEC, OnChangeSecond)
	//ON_MSG_KEY_RANGE(m_neNum[0].GetID(), KEY_VIRTUAL_0, KEY_VIRTUAL_9, OnInputNumHour)
	ON_MSG(m_btnApm.GetID(),KEY_VIRTUAL_ENTER,OnChangeAPM)
END_MSG_MAP()

unsigned long CTimeCtrl::OnChangeHour()
{
	int hour = 0;
	GetTime(hour,SECTION_HOUR);

	if( hour != m_neNum[0].GetValue() )
	{
		RefreshData(SECTION_HOUR);
		CheckTime(m_hour,m_minute,m_second);
		RefreshView(SECTION_NUM,true);
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CTimeCtrl::OnChangeMinute()
{
	int min = 0;
	GetTime(min,SECTION_MIN);

	if( min != m_neNum[1].GetValue() )
	{
		RefreshData(SECTION_MIN);
		CheckTime(m_hour,m_minute,m_second);
		RefreshView(SECTION_NUM,true);
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CTimeCtrl::OnChangeSecond()
{
	int sec = 0;
	GetTime(sec,SECTION_SEC);

	if( sec != m_neNum[2].GetValue() )
	{
		RefreshData(SECTION_SEC);
		CheckTime(m_hour,m_minute,m_second);
		RefreshView(SECTION_NUM,true);
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CTimeCtrl::OnChangeAPM()
{
	m_bAM = !m_bAM;
	int hour = 0;
	if( !(s_time_mode&MODE_H24) )
	{
		hour = m_neNum[0].GetValue();
		if ( hour>=12 && m_bAM )
		{ //极值,当12:09 AM 表示 二十四小时制 0:09,,与windows习惯相同
			hour = 0;
		}
		else if( hour>=12 && !m_bAM )
		{ //极值,当12:09 PM  表示 二十四小时制 12:09,,与windows习惯相同
			hour = 12;
		}
		else
		{
			hour = m_bAM?hour:hour+12;
		}
	}
	else
	{
		assert(false);
	}
	m_hour = hour;
	CheckTime(m_hour,m_minute,m_second);
	RefreshView(SECTION_NUM,true);

	return KEY_VIRTUAL_MSG_CONTINUE;
}	

unsigned long CTimeCtrl::OnInputNumHour(unsigned long keyID)
{
	int hour = m_neNum[0].GetValue();
	if( s_time_mode&MODE_H24 )
	{
		if( hour > 23 ) m_neNum[0].SetValue(23,true);
	}
	else
	{
		if( hour>12) m_neNum[0].SetValue(12,true);
		if( hour<1) m_neNum[0].SetValue(1,true);
	}

	return OnChangeHour();
}
