/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : MotionAreDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "MotionAreDlg.h"
#include "MessageMan.h"
#include "ImageList.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
CMotionAreDlg::CMotionAreDlg() : m_cellCount_x(0)
, m_cellCount_y(0)
, m_cell_cx(0)
, m_cell_cy(0)
, m_bAddCell(true)
, m_chnn(-1)
, m_pImgMotion(NULL)
, m_pMtInfoSor(NULL)
{

}

CMotionAreDlg::~CMotionAreDlg()
{
	m_cellList.clear();
}

//////////////////////////////////////////////////////////////////////////
void CMotionAreDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////
	int width = CMotionArePanelDlg::WIDTH;
	int height = CMotionArePanelDlg::HEIGHT;
	
	m_panelDlg.SetSenstiveValue( m_sensitivity );

	unsigned short offset_y = 3;
	if (576 == m_height)
	{
		offset_y += 20; 
	}

	if(m_chnn >= g_p_login_succ->productInfo.localVideoInputNum)
	{
		m_panelDlg.EnableTest(false);
	}

	m_panelDlg.Create(GetRandWndID(), (m_rect.Width() - width) / 2, m_rect.Height() - height - offset_y, width, height + 3, this, 0, 1);

	assert(m_chnn >= 0);
	m_bTested = false;
	m_bModify = false;

	if (m_width <= 800)
	{
		m_pImgMotion = GUI::CImageList::Instance()->GetImage(BMP_LIVE_MOTION)->GetDate();
	}
	else
	{
		m_pImgMotion = GUI::CImageList::Instance()->GetImage(BMP_LIVE_MOTION_BIG)->GetDate();
	}
	
	m_bDrawMotion = false;
	m_bReDraw = false;
	m_bMotion = true;
}

void CMotionAreDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
}

void CMotionAreDlg::SetAreaInfo(const MOTION_DETECT_INFO& motionDetectInfo, int chnn)
{
	m_pMtInfoSor = (MOTION_DETECT_INFO*)(&motionDetectInfo);

	m_cellCount_x = motionDetectInfo.areaInfo.gridWidth;
	m_cellCount_y = motionDetectInfo.areaInfo.gridHeight;

	assert(m_cellCount_x > 0);
	assert(m_cellCount_y > 0);
	m_cell_cx = (m_width / m_cellCount_x) + 1;
	m_cell_cy = (m_height / m_cellCount_y) + 1;

	//填充数据
	CELL cell;
	m_cellList.clear();
	for(int i=0; i<m_cellCount_y; i++)
	{
		for(int j=0; j<m_cellCount_x; j++)
		{	
			if( motionDetectInfo.areaInfo.pData[ (m_cellCount_x / 8 + 1) * i + j / 8] & (0x01 << (j%8) ) )
			{
				cell.x = j;
				cell.y = i;
				m_cellList.push_back(cell);
			}
		}
	}	

	//设置灵敏度大小
	m_sensitivity = motionDetectInfo.sensitivity;

	//
	m_chnn = chnn;
}

void CMotionAreDlg::GetAreaInfo(MOTION_DETECT_INFO& motionDetectInfo) const
{
	memcpy(&motionDetectInfo, m_pMtInfoSor, sizeof(MOTION_DETECT_INFO));

	//Clear
	for(int i=0; i<m_cellCount_y; i++)
	{
		for(int j=0; j<m_cellCount_x; j++)
		{	
			motionDetectInfo.areaInfo.pData[ (m_cellCount_x / 8 + 1) * i + j / 8] &= (~(0x01 << (j%8) )) ;
		}
	}

	//Set
	for (CELL_LIST::const_iterator iter = m_cellList.begin(); iter != m_cellList.end(); ++iter)
	{
		motionDetectInfo.areaInfo.pData[ (m_cellCount_x / 8 + 1) * iter->y + iter->x / 8] |= (0x01 << (iter->x % 8));
	}

	//灵敏度
	motionDetectInfo.sensitivity = m_sensitivity;
}

void CMotionAreDlg::OnPaint(PAINT_STATUS status)
{
#if 0
	unsigned long color = GUI_RGB(128, 128, 128);
	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), color);
#else
	GUI::CWnd::DrawClear(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());
#endif

	DrawCellList();

#if 0
	int left = 0;
	int top = 0;
	int right = m_width;
	int bottom = m_height;

	for (int i = 1; i < (m_cellCount_x-1); ++i)
	{
		left += m_cell_cx;
		GUI::CWnd::DrawLine(left, top, left, bottom, color);
	}

	left = 0;
	for (int i = 1; i < (m_cellCount_y-1); ++i)
	{
		top += m_cell_cy;
		GUI::CWnd::DrawLine(left, top, right, top, color);
	}
#endif

	m_bReDraw = true;
}

void CMotionAreDlg::OnIdle()
{
	GUI::CDialog::OnIdle();

#ifdef __ENVIRONMENT_WIN32__
	CMessageMan::Instance()->RefreshMotion(0xFFFF);
#endif

	static unsigned long s_count = 0;

	s_count ++;
	
	if(s_count >= 20)
	{
		s_count = 0;

		ULONGLONG motion = CMessageMan::Instance()->Motion();
		if((motion >> m_chnn) & 0x1)
		{
			DrawMotion(true, true);
		}
		else
		{
			DrawMotion(false, true);
		}
	}
}

void CMotionAreDlg::DrawCell(const CELL & cell, bool bImmediate) 
{
	assert(cell.x < m_cellCount_x);
	assert(cell.y < m_cellCount_y);

	int x = cell.x * m_cell_cx;
	int y = cell.y * m_cell_cy;

#if 0
	GUI::CWnd::DrawClear(x + 1, y + 1, m_cell_cx - 1, m_cell_cy - 1, bImmediate);
#else
	unsigned long color = GUI_RGB(20, 20, 220);
	GUI::CWnd::DrawRectBorder(x, y, m_cell_cx, m_cell_cy, color, (m_width < 800) ? 2 : 1, 1, bImmediate);
#endif
}

void CMotionAreDlg::DrawCellDel(const CELL& cell, bool bImmediate)
{
	assert(cell.x < m_cellCount_x);
	assert(cell.y < m_cellCount_y);

	int x = cell.x * m_cell_cx;
	int y = cell.y * m_cell_cy;

#if 0
	unsigned long color = GUI_RGB(128, 128, 128);
	GUI::CWnd::DrawRect(x, y, m_cell_cx, m_cell_cy, color, bImmediate);
#else
	GUI::CWnd::DrawClear(x, y, m_cell_cx, m_cell_cy, bImmediate);
#endif
}

void CMotionAreDlg::DrawCellList()
{
	for (CELL_LIST::const_iterator iter = m_cellList.begin(); iter != m_cellList.end(); ++iter)
	{
		DrawCell(*iter, false);
	}
}

CMotionAreDlg::CELL CMotionAreDlg::GetCell(int xPos, int yPos) const
{
	CELL cell;
	cell.x = xPos / m_cell_cx;
	cell.y = yPos / m_cell_cy;

	return cell;
}

void CMotionAreDlg::EndDialog(DLG_END_VALUE result)
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

bool CMotionAreDlg::IsCellInList(const CELL& cell) const
{
	for (CELL_LIST::const_iterator iter = m_cellList.begin(); iter != m_cellList.end(); ++iter)
	{
		if ((iter->x == cell.x) && (iter->y == cell.y))
		{
			return true;
		}
	}
	return false;
}

void CMotionAreDlg::AddCell(const CELL& cell)
{
	if (!IsCellInList(cell))
	{
		if ((cell.x < m_cellCount_x) && (cell.y < m_cellCount_y))
		{
			m_cellList.push_back(cell);
			DrawCell(cell, true);

			m_bTested = false;
			m_bModify = true;

			TestReDraw(cell);

			m_bMotion = false;
		}
	}	
}

void CMotionAreDlg::DelCell(const CELL& cell)
{
	bool bDel = false;

	for (CELL_LIST::iterator iter = m_cellList.begin(); iter != m_cellList.end(); ++iter)
	{
		if ((iter->x == cell.x) && (iter->y == cell.y))
		{
			m_cellList.erase(iter);
			bDel = true;
			break;
		}
	}

	if( bDel )
	{
		DrawCellDel(cell, true);

		m_bTested = false;
		m_bModify = true;

		TestReDraw(cell);

		m_bMotion = false;
	}
}

void CMotionAreDlg::HitCell(const CELL& cell)
{
	if( m_bAddCell )
	{
		AddCell(cell);
	}
	else
	{
		DelCell(cell);
	}
}

bool CMotionAreDlg::SetMotionArea(const MOTION_DETECT_INFO& motion)
{
	bool ret = true;

	NET_MOTION_INFO netMtInfo;
	memset(&netMtInfo, 0, sizeof(NET_MOTION_INFO));

	netMtInfo.chnn = m_chnn;
	memcpy(&netMtInfo.detectInfo, &motion, sizeof(MOTION_DETECT_INFO));

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_MOTION_AREA_SET, (unsigned char*)(&netMtInfo), sizeof(NET_MOTION_INFO));

	return ret;
}

BEGIN_MSG_MAP(CMotionAreDlg, GUI::CDialog)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_DRAG, OnMouseDrag)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_ENTER, OnClick)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_DBCLICK, OnDoubleClick)
	ON_MSG(m_panelDlg.GetID(), KEY_VIRTUAL_ENTER, OnClickPanel)
END_MSG_MAP()

unsigned long CMotionAreDlg::OnClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (m_panelDlg.IsShow())
	{
		m_panelDlg.Show(false, true);
	}	

	CELL cell = GetCell(static_cast<int>(xPos), static_cast<int>(yPos));

	if( IsCellInList(cell) )
	{
		m_bAddCell = false;
	}
	else
	{
		m_bAddCell = true;
	}

	//	m_bStartDrag = true;
	m_startDragCell.x = cell.x;
	m_startDragCell.y = cell.y;

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CMotionAreDlg::OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (!m_panelDlg.IsShow())
	{
		const CELL cell = GetCell(static_cast<int>(xPos), static_cast<int>(yPos));
		int offset_x = cell.x - m_startDragCell.x;
		int offset_y = cell.y - m_startDragCell.y;

		if ((0 != offset_x) && (0 != offset_y))
		{
			int start_x = (offset_x > 0) ? m_startDragCell.x : cell.x;
			int start_y = (offset_y > 0) ? m_startDragCell.y : cell.y;
			int end_x = (offset_x > 0) ? cell.x : m_startDragCell.x;
			int end_y = (offset_y > 0) ? cell.y : m_startDragCell.y;
			
			CELL tempCell;

			tempCell.x = cell.x;
			for (int i = start_y; i <= end_y; ++i)
			{
				tempCell.y = i;
				HitCell(tempCell);
			}
			
			tempCell.y = cell.y;
			for (int i = start_x; i <= end_x; ++i)
			{
				tempCell.x = i;
				HitCell(tempCell);
			}
		} 
		
		HitCell(cell);
	}	
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CMotionAreDlg::OnDoubleClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (m_panelDlg.IsShow())
	{
		m_panelDlg.Show(false, true);
	}	

	CELL cell = GetCell(static_cast<int>(xPos), static_cast<int>(yPos));

	if( IsCellInList(cell) )
	{
		m_bAddCell = false;
	}
	else
	{
		m_bAddCell = true;
	}

	HitCell(cell);

	//	m_bStartDrag = true;
	m_startDragCell.x = cell.x;
	m_startDragCell.y = cell.y;

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CMotionAreDlg::OnClickPanel()
{
	CMotionArePanelDlg::CLICK_COMMAND command = m_panelDlg.GetClickCommand();
	switch (command)
	{
	case CMotionArePanelDlg::CLICK_ALL:
		{
			CELL cell;
			m_cellList.clear();
			for (cell.y = 0; cell.y < m_cellCount_y; ++ cell.y)
			{
				for (cell.x = 0; cell.x < m_cellCount_x; ++ cell.x)
				{
					m_cellList.push_back(cell);	
				}
			}
			m_bReDraw = true;
			m_bMotion = false;
			Repaint();
		}
		break;
	case CMotionArePanelDlg::CLICK_TEST:
		{
			MOTION_DETECT_INFO motion;
			memset(&motion, 0, sizeof(MOTION_DETECT_INFO));

			m_sensitivity = m_panelDlg.GetSensitiveValue();
			GetAreaInfo(motion);
			SetMotionArea(motion);
			m_bTested = true;
			m_bMotion = true;
		}
		break;
	case CMotionArePanelDlg::CLICK_CLEAR:
		{
			m_cellList.clear();
			m_bReDraw = true;
			m_bMotion = false;
			Repaint();
		}
		break;
	case CMotionArePanelDlg::CLICK_SAVE:
//		SaveMotionInfo();
		//if(!m_bModify || IsTested(true))
		{
			if(m_bModify && !IsTested(false))
			{
				//保存之前必须通过测试
				MOTION_DETECT_INFO motion;
				memset(&motion, 0, sizeof(MOTION_DETECT_INFO));

				m_sensitivity = m_panelDlg.GetSensitiveValue();
				GetAreaInfo(motion);
				SetMotionArea(motion);
				m_bTested = true;
				m_bMotion = true;
			}

			GUI::CDialog::EndDialog(DLG_OK);
		}
		//else
		//{
		//	m_panelDlg.Repaint();
		//}
		break;
	case CMotionArePanelDlg::CLICK_EXIT:
		GUI::CDialog::EndDialog(DLG_ESC);
		break;
	case CMotionArePanelDlg::CLICK_SENSI:
		m_bModify = true;
		m_bReDraw = true;
		m_bMotion = false;
		break;
	default:
		break;
	}
	return KEY_VIRTUAL_MSG_STOP;
}

bool CMotionAreDlg::IsTested(bool bRefresh)
{
	if(!m_bTested && bRefresh)
	{
		m_panelDlg.SetInfoCaption(m_pStrTable->String(E_STR_ID_MESSAGE_113), bRefresh);//提示
	}

	return m_bTested;
}


void CMotionAreDlg::DrawMotion(bool draw, bool bRefresh)
{
	unsigned short cx = 32;
	if (m_width <= 800)
	{
		cx = 16;
	}

	unsigned short x = (m_cell_cx*2) + ((m_cell_cx-cx) >> 1);//(m_width / 2) - (cx >> 1);
	unsigned short y = (m_cell_cy*2) + ((m_cell_cy-cx) >> 1);//(m_height / 2) - (cx >> 1);

	if(m_bMotion)
	{
		if(m_bDrawMotion != draw || m_bReDraw)
		{
			if(m_bDrawMotion)
			{
				GUI::CWnd::DrawClear(x, y, cx, cx);
			}

			if(draw)
			{
				unsigned long dataLen = cx * cx * FRAME_BUFFER_PIXSEL_NUM;
				GUI::CWnd::DrawImage(m_pImgMotion, dataLen, x, y, cx, cx, cx);
			}

			if(bRefresh)
			{
				GUI::CWnd::Refresh(x, y, cx, cx);
			}

			m_bReDraw = false;
		}

		m_bDrawMotion = draw;
	}
	else
	{
		if(m_bDrawMotion)
		{
			GUI::CWnd::DrawClear(x, y, cx, cx);

			m_bDrawMotion = false;

			GUI::CWnd::Refresh(x, y, cx, cx);
		}
	}
}

void CMotionAreDlg::TestReDraw(const CELL& cell)
{
	unsigned short x = m_rect.Width() - 30;
	unsigned short y = m_rect.Height() - 30;
	CELL mtCell1 = GetCell(x, y);
	CELL mtCell2 = GetCell(x + 16, y);
	CELL mtCell3 = GetCell(x, y + 16);
	CELL mtCell4 = GetCell(x + 16, y + 16);

	if( (mtCell1.x == cell.x && mtCell1.y == cell.y) ||
		(mtCell2.x == cell.x && mtCell2.y == cell.y) ||
		(mtCell3.x == cell.x && mtCell3.y == cell.y) ||
		(mtCell4.x == cell.x && mtCell4.y == cell.y) )
	{
		m_bReDraw = true;
	}
}

//end
