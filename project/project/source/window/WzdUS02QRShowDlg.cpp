#include "WzdUS02QRShowDlg.h"

CWzdUS02QRShowDlg::CWzdUS02QRShowDlg()
{
	m_pMaix = NULL;
	m_drawStartX = 0;
	m_drawStartY = 0;

}

CWzdUS02QRShowDlg::~CWzdUS02QRShowDlg()
{

}

void CWzdUS02QRShowDlg::OnDestroy()
{
	if(NULL != m_pMaix)
	{
		delete []m_pMaix;
		m_pMaix = NULL;
	}

}


void CWzdUS02QRShowDlg::SetMaix(unsigned char * pMaix,unsigned long w,unsigned long h)
{
	assert(w > 0);
	assert(h > 0);
	assert(pMaix != NULL);
	m_maxW = w;
	m_maxH = h;
	m_pMaix = new unsigned char[w*h];
	memcpy(m_pMaix,pMaix,w*h);

}

void CWzdUS02QRShowDlg::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x;
	unsigned long cy = (m_rect.Height() - 2 * y)/2-m_length[BTN_HEIGHT];
	m_drawStartX = m_length[DLG_OFFSET_SUB_LEFT];
	m_drawStartY = y+cy;
	m_richTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);
	y = m_drawStartY + 4*(m_maxW+1)+m_length[BTN_HEIGHT];
	cy = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP] -y;
	m_noterichTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP];

	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];

	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void CWzdUS02QRShowDlg::InitCtrlInfo()
{

}

void CWzdUS02QRShowDlg::SetTipDisk(const std::string& str)
{
	m_richTextCtrl.SetText(str, false);
}

void CWzdUS02QRShowDlg::SetNoteText(const std::string& str)
{
	m_noterichTextCtrl.SetText(str, false);
}

void CWzdUS02QRShowDlg::SetWzdInfo(US02_WzdInfo * pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdUS02QRShowDlg::ShowWzdData(bool bRefresh)
{
	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}
}

void CWzdUS02QRShowDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - m_length[DLG_OFFSET_TOP] - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];
	//unsigned long x = m_rect.m_left+m_length[DLG_OFFSET_LEFT] + width/2-1*(m_maxW+1);
	//unsigned long y = m_rect.m_top+m_length[DLG_OFFSET_TOP] + height/2-1*(m_maxW+1);
	unsigned long x = m_rect.m_left+m_length[DLG_OFFSET_LEFT]+m_drawStartX;
	unsigned long y = m_rect.m_top+m_length[DLG_OFFSET_TOP]+m_drawStartY;

	if(m_pMaix != NULL)
	{
		GUI::CWnd::DrawRect(x,y,4*(m_maxH+1),4*(m_maxW+1),COLOR_LINE_WHITE);
		for(unsigned long w =0;w<m_maxW;++w)
		{
			for(unsigned long h =0;h<m_maxH;++h)
			{
				if(0x01 & (*(m_pMaix+w*m_maxW+h)))
				{
					//ºÚ¿é
					GUI::CWnd::DrawRect(x+4*(h%m_maxH)+2, y+4*(w%m_maxW)+2, 4, 4, COLOR_LINE_BLACK);
				}
				else
				{	//°×¿é
					GUI::CWnd::DrawRect(x+4*(h%m_maxH)+2, y+4*(w%m_maxW)+2, 4, 4, COLOR_LINE_WHITE);
				}
			}
		}
	}
	
}


BEGIN_MSG_MAP(CWzdUS02QRShowDlg, CWzdUS02PageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()

unsigned long  CWzdUS02QRShowDlg::OnCheck()
{
	if(m_checkCtrl.IsCheck())
	{
		m_pWzdInfo->ShowWzd = 1;
	}
	else
	{
		m_pWzdInfo->ShowWzd = 0;
	}
	return KEY_VIRTUAL_STOP;
}
//end

