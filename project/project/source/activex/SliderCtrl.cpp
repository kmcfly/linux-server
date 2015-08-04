#include "SliderCtrl.h"


using namespace GUI;

GUI::CSliderCtrl::CSliderCtrl() : m_curPos(0)
, m_max(100)
, m_min(0)
, m_boxLen(0)
, m_clickPos(0)
, m_bHorizontal(true)
, m_bkColor(COLOR_DLG_CHILD_BK)
, m_bIgnore(false)
{
	m_ctrlType = GUI::WND_TYPE_SLIDER;
}

GUI::CSliderCtrl::~CSliderCtrl()
{

}

bool GUI::CSliderCtrl::Create(unsigned long ctrlID, 
						  unsigned long x, 
						  unsigned long y, 
						  unsigned long len,
						  CWnd* pParent, 
						  int xFocus, 
						  int yFocus,
						  bool bHorizontal/* = true*/)
{
	m_bHorizontal = bHorizontal;
	unsigned long cx = len;
	unsigned long cy = m_length[SLIDER_WIDTH];
	if (!bHorizontal)
	{
		cx = m_length[SLIDER_WIDTH];
		cy = len;
	}
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, WND_CHILD | WND_VISIBLE, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CSliderCtrl::OnInitial()
{
	if (m_bHorizontal)
	{
		m_imageMap[PAINT_STATUS_NORMAL] = BMP_SLIDER_BOX;
		m_imageMap[PAINT_STATUS_FOCUS] = BMP_SLIDER_BOX_FOUCS;
		m_imageMap[PAINT_STATUS_DISABLE] = BMP_SLIDER_BOX_DISABLE;

		int image_cx = 0;
		GetImageSize(BMP_SLIDER_BK, image_cx, m_bkHeight);
	} 
	else
	{
//		assert(false);//暂不支持
		m_imageMap[PAINT_STATUS_NORMAL] = BMP_SLIDER_BOX_V;
		m_imageMap[PAINT_STATUS_FOCUS] = BMP_SLIDER_BOX_V_FOUCS;
		m_imageMap[PAINT_STATUS_DISABLE] = BMP_SLIDER_BOX_V_DISABLE;
	}
	
	//计算滑块长度
	m_boxLen = CalculateBoxLen();
}

void GUI::CSliderCtrl::OnDestroy()
{
	GUI::CWnd::OnDestroy();
	m_curPos = 0;
}

void GUI::CSliderCtrl::SetSliderBoxImage(BITMAP_ID normal, BITMAP_ID focus, BITMAP_ID disable)
{
	m_imageMap[PAINT_STATUS_NORMAL] = normal;
	m_imageMap[PAINT_STATUS_FOCUS] = focus;
	m_imageMap[PAINT_STATUS_DISABLE] = disable;

	m_boxLen = CalculateBoxLen();
}

int GUI::CSliderCtrl::CalculateBoxLen() const
{
//计算滑块宽度
	int cx = 0;
	int cy = 0;
	SLIDER_IMAGE_MAP::const_iterator iter = m_imageMap.begin();
	assert(iter != m_imageMap.end());
	GetImageSize(iter->second, cx, cy);
	if (m_bHorizontal)
	{
		return cx;
	} 
	else
	{
		return cy;
	}
}

void GUI::CSliderCtrl::OnPaint(PAINT_STATUS status)
{
	//画背景
	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), m_bkColor);
	
	//画滑块
	SLIDER_IMAGE_MAP::const_iterator iter = m_imageMap.find(status);
	if (iter == m_imageMap.end())
	{
		assert(false);
		return;
	}

	BITMAP_ID sliderBoxID = iter->second;
	//GUI::CWnd::DrawImage(image.bkID, m_rect.m_left, m_rect.m_top);

	if (m_bHorizontal)
	{
		CDrawManager drawMan;
		const unsigned char* pData = drawMan.Stretch(BMP_SLIDER_BK, m_rect.Width(), m_bkHeight, 4, CDrawManager::INVLAID, NULL, true);
		int len = drawMan.GetDataLen();

		if (NULL != pData)
		{
			GUI::CWnd::DrawImage(pData, len, m_rect.m_left, m_rect.m_top + (m_rect.Height() - m_bkHeight)/2, m_rect.Width(), m_rect.Width(),m_bkHeight);
		}

		
		int pos = m_curPos * (m_rect.Width() - m_boxLen) /(m_max - m_min) - m_min;
		GUI::CWnd::DrawImage(sliderBoxID, m_rect.m_left + pos, m_rect.m_top);
	} 
	else
	{
		int line_width = 4;
		//GUI::CWnd::DrawRect(m_rect.m_left + (m_rect.Width() - line_width) / 2, m_rect.m_top , line_width, m_rect.Height(), COLOR_LINE_GRAY);

		int pos = m_curPos * (m_rect.Height() - m_boxLen) /(m_max - m_min) - m_min;
		GUI::CWnd::DrawImage(sliderBoxID, m_rect.m_left, m_rect.m_top + pos);
	}
}


void GUI::CSliderCtrl::SetPos(int x)
{
	if (m_bHorizontal)
	{
		if ((x >= m_rect.m_left) && (x <= m_rect.m_right))
		{
			int pos = 0;
			if (x >= (m_rect.m_right - m_boxLen))
			{
				pos = (m_rect.Width() - m_boxLen);
			}
			else
			{
				pos = x - m_rect.m_left;
			}	
			m_curPos = pos * (m_max - m_min)/(m_rect.Width() - m_boxLen) + m_min;

			Repaint();
		}
	} 
	else
	{
		if ((x >= m_rect.m_top) && (x <= m_rect.m_bottom))
		{
			int pos = 0;
			if (x >= (m_rect.m_bottom - m_boxLen))
			{
				pos = (m_rect.Height() - m_boxLen);
			}
			else
			{
				pos = x - m_rect.m_top;
			}	
			m_curPos = pos * (m_max - m_min)/(m_rect.Height() - m_boxLen) + m_min;

			Repaint();
		}
	}	
}

int GUI::CSliderCtrl::GetSliderBoxPos() const
{
	if (m_bHorizontal)
	{
		return m_curPos * (m_rect.Width() - m_boxLen) /(m_max - m_min) - m_min + m_rect.m_left;
	} 
	else
	{
		return m_curPos * (m_rect.Height() - m_boxLen) /(m_max - m_min) - m_min + m_rect.m_top;
	}	
}

unsigned long GUI::CSliderCtrl::SendChange() const
{
	return (KEY_VIRTUAL_MSG_DEFAULT_TAG | m_curPos);
}

BEGIN_MSG_MAP(GUI::CSliderCtrl, GUI::CWnd)
	ON_MSG_EX(GUI::CSliderCtrl::GetID(), KEY_VIRTUAL_ENTER, OnDragStart)
	ON_MSG_EX(GUI::CSliderCtrl::GetID(), KEY_VIRTUAL_DRAG, OnDragIng)
	ON_MSG_EX(GUI::CSliderCtrl::GetID(), KEY_VIRTUAL_BLUP, OnDragEnd)
	ON_MSG(GUI::CSliderCtrl::GetID(), KEY_VIRTUAL_LEFT, OnLeftKey)
	ON_MSG(GUI::CSliderCtrl::GetID(), KEY_VIRTUAL_RIGHT, OnRightKey)
	ON_MSG(GUI::CSliderCtrl::GetID(), KEY_VIRTUAL_UP, OnUpKey)
	ON_MSG(GUI::CSliderCtrl::GetID(), KEY_VIRTUAL_DOWN, OnDownKey)
	ON_MSG(GUI::CSliderCtrl::GetID(), KEY_VIRTUAL_ADD, OnAddKeyEx)
	ON_MSG(GUI::CSliderCtrl::GetID(), KEY_VIRTUAL_DEC, OnDecKeyEx)
END_MSG_MAP()



unsigned long GUI::CSliderCtrl::OnDragStart(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int sliderBox = GetSliderBoxPos();
	int pos = xPos;
	if (!m_bHorizontal)
	{
		pos = yPos;
	}
	if ( (pos >= sliderBox) && (pos <= sliderBox + m_boxLen))
	{
		m_clickPos = pos - GetSliderBoxPos();
	}
	else
	{
		SetPos(pos);
//		return SendChange();
	}
	
	return KEY_VIRTUAL_MSG_CONTINUE;
}


unsigned long GUI::CSliderCtrl::OnDragIng(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int sliderBox = GetSliderBoxPos();

#if 0
	if (m_bHorizontal)
	{
		SetPos(xPos -  m_clickPos);	
	} 
	else
	{
		SetPos(yPos - m_clickPos);
	}
#else
	if(m_bIgnore)
	{
		//如果面积超过, 屏幕面积的 1/5 则将滑动块特殊处理
		//滑动过程中将增加步长
		
		if (m_bHorizontal)
		{
			int minOffset = m_rect.Width() / 5;
			int curPos = m_rect.m_left + m_curPos * (m_rect.Width() - m_boxLen) / (m_max - m_min);
			int offsetPos = (xPos > curPos) ? (xPos - curPos) : (curPos - xPos);

			if( (offsetPos < minOffset) && 
				(yPos > (m_rect.m_left + minOffset)) &&
				(yPos < (m_rect.m_right - minOffset)))
			{
				//滑块两端部分, 不过滤消息
				return KEY_VIRTUAL_MSG_STOP;
			}

			SetPos(xPos -  m_clickPos);	
		} 
		else
		{
			int minOffset = m_rect.Height() / 5;
			int curPos = m_rect.m_top + m_curPos * (m_rect.Height() - m_boxLen) / (m_max - m_min);
			int offsetPos = (yPos > curPos) ? (yPos - curPos) : (curPos - yPos);

			if( (offsetPos < minOffset) && 
				(yPos > (m_rect.m_top + minOffset)) &&
				(yPos < (m_rect.m_bottom - minOffset)))
			{
				//滑块两端部分, 不过滤消息
				return KEY_VIRTUAL_MSG_STOP;
			}

			SetPos(yPos - m_clickPos);
		}
	}
	else
	{
		if (m_bHorizontal)
		{
			SetPos(xPos -  m_clickPos);	
		} 
		else
		{
			SetPos(yPos - m_clickPos);
		}
	}
#endif
	return KEY_VIRTUAL_MSG_CONTINUE;
}


unsigned long GUI::CSliderCtrl::OnDragEnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int sliderBox = GetSliderBoxPos();
	int pos = xPos;
	if (!m_bHorizontal)
	{
		pos = yPos;
	}
	if ( (pos >= sliderBox) && (pos <= sliderBox + m_boxLen))
	{
		m_clickPos = pos - GetSliderBoxPos();
	}
	else
	{
		SetPos(pos);
		//		return SendChange();
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CSliderCtrl::OnAddKeyEx()
{
	if (m_bHorizontal)
	{
		return OnAddKey();
	}
	else
	{
		return OnDecKey();
	}

}

unsigned long GUI::CSliderCtrl::OnDecKeyEx()
{
	if (m_bHorizontal)
	{
		return OnDecKey();
	}
	else
	{
		return OnAddKey();
	}

}

unsigned long GUI::CSliderCtrl::OnDecKey()
{
	if (m_curPos > m_min)
	{
		--m_curPos;
		Repaint();
		return SendChange();
	} 
	else
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}	
}

unsigned long GUI::CSliderCtrl::OnAddKey()
{
	if (m_curPos < m_max)
	{
		++m_curPos;
		Repaint();
		return SendChange();
	}
	else
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
}

unsigned long GUI::CSliderCtrl::OnLeftKey()
{
	if (m_bHorizontal)
	{
		OnDecKey();
		return SendChange();
	}
	else
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
}

unsigned long GUI::CSliderCtrl::OnRightKey()
{
	if (m_bHorizontal)
	{
		OnAddKey();
		return SendChange();
	}
	else
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
}

unsigned long GUI::CSliderCtrl::OnUpKey()
{
	if (!m_bHorizontal)
	{
		OnDecKey();
		return SendChange();
	}
	else
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
}

unsigned long GUI::CSliderCtrl::OnDownKey()
{
	if (!m_bHorizontal)
	{
		OnAddKey();
		return SendChange();
	}	
	else
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
}
