/*
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xian shi wei
** Date         : 2014-10-15
** Name         : SetROIDlg.CPP
** Version      : 1.0
** Description  : 
** Modify Record:
*/


#include "SetROIDlg.h"

CSetROIDlg::CSetROIDlg():m_state(STATE_NULL),m_idSel(-1),m_currChnn(-1)
{
}

CSetROIDlg::~CSetROIDlg(){}

void CSetROIDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;

	int width = CCfgROIPanelDlg::WIDTH;
	int height = CCfgROIPanelDlg::HEIGHT;

	unsigned short offset_y = 3;
	if (576 == m_height)
	{
		offset_y += 20; 
	}
	m_panelDlg.Create(GetRandWndID(), (m_rect.Width() - width) / 2, m_rect.Height() - height - offset_y, width, height + 3, this, 0, 1);

	m_areaMax = MAX_VIDEO_CONFIG_ROI_NUM;
	m_arealist.clear();
	m_idSel = -1;
	m_state = STATE_NULL;
	SetModify(false);

	for(int i = 0; i < m_areaMax; i++ )
	{
		VIDEO_CONFIG_ROI& area = m_ROIInfo.roi[i];
		if( (0 != area.iEnable) && ( 0 != area.w ) && ( 0 != area.h) )
		{
			RectROI rcROI;
			rcROI.color = (8<<255);   //绿

			memcpy(&(rcROI.area), &area, sizeof(VIDEO_CONFIG_ROI));
			rcROI.old = 1;

			//四舍五入, 减少误差
			unsigned long x = (double(area.x * m_width) / 100 + double(0.5));
			unsigned long y = (double(area.y * m_height) / 100 + double(0.5));
			unsigned long cx = (double(area.w * m_width) / 100 + double(0.5));
			unsigned long cy = (double(area.h * m_height) / 100 + double(0.5));

			rcROI.id = (x<< 16) | (y);
			rcROI.rect.SetRect(x, y, x + cx, y + cy);

			m_arealist.push_back(rcROI);
		}
	}
	if (!m_arealist.empty())
	{
		m_sensitivity =m_arealist.front().area.iLevel;
		m_panelDlg.SetSenstiveValue( m_sensitivity );

		m_iEnable = m_arealist.front().area.iEnable;
		m_panelDlg.SetEnableROI( m_iEnable );
	}else{
		m_sensitivity = m_ROIInfo.roi[0].iLevel;
		m_panelDlg.SetSenstiveValue( m_sensitivity );
		
		m_iEnable = m_ROIInfo.roi[0].iEnable;
		m_panelDlg.SetEnableROI( m_iEnable );
	}
}

void CSetROIDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	m_arealist.clear();
	m_idSel = -1;
	m_panelDlg.SetValue(0,0);
}

void CSetROIDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::DrawClear(m_rect.m_left,m_rect.m_top,m_rect.Width(),m_rect.Height());

	std::list<RectROI>::iterator iter;
	for (iter = m_arealist.begin(); iter != m_arealist.end(); iter++ )
	{
		DrawRectPt2(iter->rect.m_left, iter->rect.m_top, iter->rect.m_right, iter->rect.m_bottom, true);
		if ( m_idSel == iter->id)
		{
			DrawRectSel(iter->rect, true, false);
		}
	}
}

void CSetROIDlg::EndDialog(DLG_END_VALUE result)
{
	if (m_panelDlg.IsShow())
	{
		m_panelDlg.Show(false, true);
	} 
	else
	{
		m_panelDlg.Show(true, true);
	}
}

bool CSetROIDlg::RefreshROIInfo()
{
	if(IsModify()){
		memset(&m_ROIInfo, 0, sizeof(VIDEO_CONFIG_ROI_INFO));
		for (unsigned int i=0;i<MAX_VIDEO_CONFIG_ROI_NUM;++i)
		{
			m_ROIInfo.roi[i].iLevel = m_sensitivity;
			/*m_ROIInfo.roi[i].iLevel = m_iEnable;*/
		}
		unsigned int index =0;
		std::list<RectROI>::iterator iter;
		for (iter = m_arealist.begin(); iter != m_arealist.end(); iter++,index++)
		{
			if (m_areaMax <= index){
				break;
			}
			if(!iter->old){//四舍五入, 减少误差    ,从大画面切换回小画面时,进行转换
				unsigned long x = (double(iter->rect.m_left) * 100 / m_width + double(0.5));     
				unsigned long y = (double(iter->rect.m_top) * 100 / m_height + double(0.5));
				unsigned long cx = (double(iter->rect.Width()) * 100 / m_width + double(0.5));
				unsigned long cy = (double(iter->rect.Height()) * 100 / m_height + double(0.5));
				//值域限制
				(x>=100)&&(x=99,cx=1) ,(cx<=0)&&(cx=1) ,((x+cx)>=100)&&(cx=100-x);
				(y>=100)&&(y=99,cy=1) ,(cy<=0)&&(cy=1) ,((y+cy)>=100)&&(cy=100-y);
				VIDEO_CONFIG_ROI area;
				memset(&area, 0, sizeof(VIDEO_CONFIG_ROI));	
				area.x = x,area.y = y;
				area.w = cx,area.h = cy;
				area.iChn = index+1;
				/*area.iEnable = iter->area.iEnable;
				area.iLevel = iter->area.iLevel;*/
				area.iEnable = m_iEnable;
				area.iLevel  = m_sensitivity;
				//area.szAreaName   区域名字暂时为空
				memcpy(&m_ROIInfo.roi[index], &area, sizeof(VIDEO_CONFIG_ROI));
			}else{
				iter->area.iEnable = m_iEnable;
				iter->area.iLevel  = m_sensitivity;
				memcpy(&m_ROIInfo.roi[index], &(iter->area), sizeof(VIDEO_CONFIG_ROI));
			}
		}
		return true;
	}else{
		if ((m_ROIInfo.roi[0].iEnable != m_iEnable)||(m_ROIInfo.roi[0].iLevel != m_sensitivity))
		{
			for (unsigned int i=0; i<MAX_VIDEO_CONFIG_ROI_NUM; ++i)
			{
				m_ROIInfo.roi[i].iEnable = m_iEnable;
				m_ROIInfo.roi[i].iLevel = m_sensitivity;
			}
			SetModify(true);
		}
	}
	return false;
}

void CSetROIDlg::SetROIInfo(const VIDEO_CONFIG_ROI_INFO& ROIInfo)
{
	memcpy(&m_ROIInfo, &ROIInfo, sizeof(VIDEO_CONFIG_ROI_INFO));
}
void CSetROIDlg::GetROIInfo(VIDEO_CONFIG_ROI_INFO& ROIInfo)
{
	memcpy(&ROIInfo, &m_ROIInfo, sizeof(VIDEO_CONFIG_ROI_INFO));
}
inline bool CSetROIDlg::IsModify() const
{
	return m_bModify;
}
inline void CSetROIDlg::SetModify(bool modify)
{
	m_bModify = modify;
}
inline bool CSetROIDlg::IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1>=x2)?(x1-x2):(x2-x1);
	unsigned long y = (y1>=y2)?(y1-y2):(y2-y1);

	if(((x*x) + (y*y)) <= distance)
	{
		return true;
	}
	return false;
}
inline bool CSetROIDlg::PointInArea(unsigned long xPos, unsigned long yPos, unsigned long& idHit)
{
	std::list<RectROI>::iterator iter;
	for (iter = m_arealist.begin(); iter != m_arealist.end(); iter++ )
	{
		if(iter->rect.PtInRect(xPos, yPos))
		{
			idHit = iter->id;
			return true;
		}
	}
	return false;
}

inline void CSetROIDlg::GetSingleRoiInfo(unsigned long id, RectROI& roi)
{
	std::list<RectROI>::iterator iter;
	for (iter = m_arealist.begin(); iter != m_arealist.end(); iter++ )
	{
		if(iter->id == id)
		{
			roi = *iter;
		}
	}
}

inline bool CSetROIDlg::FindAreaById(unsigned long id, GUI::CRect& rect)
{
	std::list<RectROI>::iterator iter;
	for (iter = m_arealist.begin(); iter != m_arealist.end(); iter++ )
	{
		if(iter->id == id)
		{
			rect = iter->rect;
			return true;
		}
	}
	return false;
}

inline void CSetROIDlg::SetExistAreaById(unsigned long id, GUI::CRect rect)
{
	std::list<RectROI>::iterator iter;
	for (iter = m_arealist.begin(); iter != m_arealist.end(); iter++ )
	{
		if(iter->id == id)
		{
			iter->rect = rect;
		}
	}
}

void CSetROIDlg::RefreshPt2(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2)
{
	unsigned long x, y, cx, cy;
	x = (x1 >= x2) ? x2 : x1;
	y = (y1 >= y2) ? y2 : y1;
	cx = (x1 >= x2) ? (x1 - x2) : (x2 - x1);
	cy = (y1 >= y2) ? (y1 - y2) : (y2 - y1);

	if(cx == 0 || cy == 0){
		return;
	}
	GUI::CWnd::Refresh(x, y, cx, cy);
}

void CSetROIDlg::DrawRectSel(const GUI::CRect& rect, bool bDraw, bool bImmediate)
{
	if(bDraw)
	{
		GUI::CWnd::DrawRectBorder(rect.m_left, rect.m_top, rect.Width() - 1, rect.Height() - 1, COLOR_SELECT,2, 2, bImmediate);
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		GUI::CWnd::DrawRectBorder(rect.m_left, rect.m_top, rect.Width() - 1, rect.Height() - 1, COLOR_LINE_WHITE, 2, 2, bImmediate);
#else
		GUI::CWnd::DrawRectBorder(rect.m_left, rect.m_top, rect.Width() - 1, rect.Height() - 1, COLOR_LINE_GRAY, 2, 2, bImmediate);
#endif
	}
}

void CSetROIDlg::ColourRect(const GUI::CRect& rect, bool bDraw,bool bImmediate)
{
	if (bDraw)
	{
#ifdef __ENVIRONMENT_WIN32__
		GUI::CWnd::DrawRectBorder(rect.m_left, rect.m_top, rect.Width() - 1, rect.Height() - 1, COLOR_LINE_GRAY, 2, 2, bImmediate);
#else
		GUI::CWnd::DrawRectBorder(rect.m_left, rect.m_top, rect.Width() - 1, rect.Height() - 1, COLOR_LINE_GRAY, 2, 2, bImmediate);
#endif
	}
}

void CSetROIDlg::DrawRectPt2(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, bool bDraw)
{
	unsigned long x, y, cx, cy;
	x = (x1 >= x2) ? x2 : x1;
	y = (y1 >= y2) ? y2 : y1;
	cx = (x1 >= x2) ? (x1 - x2) : (x2 - x1);
	cy = (y1 >= y2) ? (y1 - y2) : (y2 - y1);

	if(cx == 0 || cy == 0){
		return;
	}
	if(bDraw){
#ifdef __ENVIRONMENT_WIN32__
		GUI::CWnd::DrawRectBorder(x, y, cx-1, cy-1, COLOR_LINE_WHITE,2,2, true);
#else
		GUI::CWnd::DrawRectBorder(x, y, cx-1, cy-1, COLOR_LINE_GRAY,2,2, true);
#endif
	}else{
		GUI::CWnd::DrawClear(x, y, cx, cy, false);
	}
}

BEGIN_MSG_MAP(CSetROIDlg, GUI::CDialog)	
	ON_MSG_EX(GetID(), KEY_VIRTUAL_DRAG, OnMouseDrag)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_ENTER, OnClick)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_DBCLICK, OnDoubleClick)
	ON_MSG(m_panelDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickPanel)
END_MSG_MAP()

unsigned long CSetROIDlg::OnClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (m_panelDlg.IsShow())
	{
		m_panelDlg.Show(false, true);
	}
	if ((m_width <= xPos) && (m_height <= yPos))
	{
		return KEY_VIRTUAL_MSG_STOP;  //按键板的输入
	}

	unsigned long idHit;
	if (PointInArea(xPos, yPos, idHit))
	{
		/*if(m_idSel != idHit)
		{//切换选中	
			GUI::CRect rect;
			if(FindAreaById(m_idSel, rect))       //把之前的选择的还原
			{
				DrawRectSel(rect, false, true);
			}
			if(FindAreaById(idHit, rect))       //给当前选择的画色
			{
				DrawRectSel(rect, true, true);
			}
			m_idSel = idHit;
		}
		else
		{
			GUI::CRect rect;
			if(FindAreaById(idHit, rect))
			{
				DrawRectSel(rect, true, true); 
			}
		}
		m_state = STATE_NULL;
		RectROI roi;
		memset(&roi,0,sizeof(RectROI));
		GetSingleRoiInfo(m_idSel,roi);
		m_panelDlg.OnPaint(GUI::PAINT_STATUS_NORMAL,roi.area.iEnable,roi.area.iLevel);*/

		//只要是在某一个已定区域内部，则不能拖动绘制，保证不重叠。
		return KEY_VIRTUAL_MSG_STOP;
	}else{
		m_state = STATE_ADD; 
	}

	m_bFirstDrag = true;
	m_x_enter = xPos;
	m_y_enter = yPos;
    
	//RectROI roi;
	//GetSingleRoiInfo(m_idSel,roi);
	//m_panelDlg.OnPaint(GUI::PAINT_STATUS_NORMAL,roi.area.iEnable,roi.area.iLevel);

	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CSetROIDlg::OnDoubleClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (m_panelDlg.IsShow())
	{
		m_panelDlg.Show(false, true);
	}	
	if ((m_width <= xPos) && (m_height <= yPos))
	{//按键板的输入
		return KEY_VIRTUAL_MSG_STOP;
	}
	unsigned long idHit;
	GUI::CRect delRect;
	std::list<RectROI>::iterator iter;
	if (PointInArea(xPos, yPos, idHit))
	{//删除
		for (iter = m_arealist.begin(); iter != m_arealist.end(); iter++ )
		{
			if(iter->id == idHit){
				delRect = iter->rect;
				DrawRectPt2(iter->rect.m_left, iter->rect.m_top, iter->rect.m_right, iter->rect.m_bottom, false);
				RefreshPt2(iter->rect.m_left, iter->rect.m_top, iter->rect.m_right, iter->rect.m_bottom);
				10 >iter->area.iLevel ? 1 : iter->area.iLevel = 9;
	            //m_panelDlg.OnPaint(GUI::PAINT_STATUS_NORMAL,iter->area.iEnable,iter->area.iLevel);
				m_arealist.erase(iter);
				SetModify(true);
				break;
			}
		}
		//重绘
        for (iter = m_arealist.begin(); iter != m_arealist.end(); iter++ )
        {
			DrawRectPt2(iter->rect.m_left, iter->rect.m_top, iter->rect.m_right, iter->rect.m_bottom, true);
        }
		m_idSel = -1;
	}
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CSetROIDlg::OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (m_panelDlg.IsShow())
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	static unsigned long old_x_pos = 0;
	static unsigned long old_y_pos = 0;

	if ((m_width <= xPos) && (m_height <= yPos))
	{//按键板的输入
		return KEY_VIRTUAL_MSG_STOP;
	}
	if(STATE_ADD != m_state)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	if(m_bFirstDrag)
	{
		old_x_pos = m_x_enter;
		old_y_pos = m_y_enter;

		m_bFirstDrag = false;
	}
	if(xPos >= m_x_enter + 4 && yPos >= m_y_enter + 4 && !IsAlmostEqual(old_x_pos, old_y_pos, xPos, yPos, 100))
	{
		unsigned long id = (m_x_enter << 16) | m_y_enter;
		bool bFind = false;
		std::list<RectROI>::iterator iter;
		for (iter = m_arealist.begin(); iter != m_arealist.end(); iter++ )
		{  //修改
			if(iter->id == id)
			{
				iter->rect.SetRect(m_x_enter, m_y_enter, xPos, yPos);
				GetPanelParamter(iter->area.iEnable,iter->area.iLevel);
				bFind = true;
				SetModify(true);

				//m_panelDlg.OnPaint(GUI::PAINT_STATUS_NORMAL,iter->area.iEnable,iter->area.iLevel);
				break;
			}
		}

		RectROI rcROI;
		memset(&rcROI,0,sizeof(RectROI));
		if(!bFind) //添加
		{
			if(m_arealist.size() >= m_areaMax)
			{	//满了之后不能再添加
				return KEY_VIRTUAL_MSG_STOP;
			}
			memset(&rcROI, 0, sizeof(RectROI));
			rcROI.id = id;
			rcROI.area.iChn = m_arealist.size() + 1;
			GetPanelParamter(rcROI.area.iEnable,rcROI.area.iLevel);

			rcROI.rect.SetRect(m_x_enter, m_y_enter, xPos, yPos);
			m_arealist.push_back(rcROI);
			SetModify(true);

			m_panelDlg.OnPaint(GUI::PAINT_STATUS_NORMAL,rcROI.area.iEnable,rcROI.area.iLevel);
		}
        
		//防止擦除区域过大
        unsigned long c_x,c_y;
		xPos>m_x_enter? c_x = xPos-m_x_enter: c_x = m_x_enter - xPos;
		yPos>m_y_enter? c_y = yPos-m_y_enter: c_y = m_y_enter - yPos;
		GUI::CWnd::DrawClear(m_x_enter, m_y_enter, c_x, c_y, true);

		if ( (old_x_pos > xPos)||(old_y_pos > yPos) )
		{
			old_x_pos>m_x_enter? c_x = old_x_pos-m_x_enter: c_x = m_x_enter - old_x_pos;
			old_y_pos>m_y_enter? c_y = old_y_pos-m_y_enter: c_y = m_y_enter - old_y_pos;
			GUI::CWnd::DrawClear(m_x_enter, m_y_enter, c_x, c_y, true);
		}
		DrawRectPt2(m_x_enter,m_y_enter,xPos,yPos,true);

		//判断拖动的点是否在，已画的矩形框中,若在则重画所有区域
		unsigned long idh = 0;
		GUI::CRect rectNew;
		GUI::CRect rectOld;
		if ( PointInArea(xPos,yPos,idh) ){
			for (iter = m_arealist.begin(); iter != m_arealist.end(); iter++ )
			{
				DrawRectSel(iter->rect, false, true);
			}
		}

        RefreshPt2(m_x_enter,m_y_enter,xPos,yPos);

		old_x_pos = xPos;
		old_y_pos = yPos;
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSetROIDlg::OnClickPanel()
{
	CCfgROIPanelDlg::CLICK_COMMAND command = m_panelDlg.GetClickCommand();
	switch (command)
	{
	case CCfgROIPanelDlg::CLICK_SAVE:
		{
			m_sensitivity = m_panelDlg.GetSensitiveValue();
			m_iEnable     = m_panelDlg.GetEnableROI();
			RefreshROIInfo();
			GUI::CDialog::EndDialog(DLG_OK);
		}
		break;
	case CCfgROIPanelDlg::CLICK_EXIT:
		GUI::CDialog::EndDialog(DLG_ESC);
		break;
	case CCfgROIPanelDlg::CLICK_SENSI:
		break;
	default:
		break;
	}
	return KEY_VIRTUAL_MSG_STOP;
}