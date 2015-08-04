#include "CPrintQRCodeDlg.h"

extern VGA_RESOLUTION g_resolution;

CPrintQRCodeDlg::CPrintQRCodeDlg()
{
	m_pMaix = NULL;
	memset(m_desc, 0, sizeof(m_desc));
	memset(m_descUrl, 0, sizeof(m_descUrl));
}

CPrintQRCodeDlg::~CPrintQRCodeDlg()
{

}

void CPrintQRCodeDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
	if(NULL != m_pMaix)
	{
		delete []m_pMaix;
		m_pMaix = NULL;
	}
	
}

void CPrintQRCodeDlg::SetMaix(unsigned char * pMaix,unsigned long w,unsigned long h)
{
	assert(w > 0);
	assert(h > 0);
	assert(pMaix != NULL);
	m_maxW = w;
	m_maxH = h;
	m_pMaix = new unsigned char[w*h];
	memcpy(m_pMaix,pMaix,w*h);
	
}

void CPrintQRCodeDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption("QRCode", false);

	SetDefaultBtn(DEFAULT_BTN_OK);
	SetDefaultBtnCaption(DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_EXIT) );

	#if defined (__CUSTOM_ZNV__) && (!defined (__ZNV_SC__) || defined(__ZNV_WNRL__))
		unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
		unsigned short height = m_rect.Height() - m_length[DLG_OFFSET_TOP] - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];
		m_stcDesc.Create(CTRLID_STATIC, m_length[DLG_OFFSET_LEFT], m_length[DLG_OFFSET_TOP] + height - 3*m_length[STATIC_HEIGHT], width, m_length[STATIC_HEIGHT], this, m_desc);
		m_stcDesc.SetAlignText(GUI::ALIGN_TEXT_CENTER);
	#else
		unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
		unsigned short height = m_rect.Height() - m_length[DLG_OFFSET_TOP] - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

		unsigned long xPos = m_length[DLG_OFFSET_LEFT];
		unsigned long yPos = m_length[DLG_OFFSET_TOP] + height;
		if (VGA_800X600 < g_resolution)
		{
			yPos -= 3 * m_length[STATIC_HEIGHT];
		}
		else
		{
			yPos -= m_length[STATIC_HEIGHT];
		}

		m_stcDesc.Create(CTRLID_STATIC, xPos, yPos, width, m_length[STATIC_HEIGHT], this, m_desc);
		m_stcDesc.SetAlignText(GUI::ALIGN_TEXT_CENTER);

		if (VGA_800X600 < g_resolution)
		{
			yPos += 2 * m_length[STATIC_HEIGHT];
		}
		else
		{
			yPos += m_length[STATIC_HEIGHT];
		}
		
		m_stcDescURL.Create(CTRLID_STATIC, xPos, yPos, width, m_length[STATIC_HEIGHT], this, m_descUrl);
		m_stcDescURL.SetAlignText(GUI::ALIGN_TEXT_CENTER);
	#endif
}

void CPrintQRCodeDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();
}

void CPrintQRCodeDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);	
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - m_length[DLG_OFFSET_TOP] - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];
	unsigned long x = m_rect.m_left+m_length[DLG_OFFSET_LEFT] + width/2-4*(m_maxW+1);
	unsigned long y = m_rect.m_top+m_length[DLG_OFFSET_TOP] + height/2-4*(m_maxW+1);
	if(m_pMaix != NULL)
	{
		GUI::CWnd::DrawRect(x,y,8*(m_maxH+1),8*(m_maxW+1),COLOR_LINE_WHITE);
		for(unsigned long w =0;w<m_maxW;++w)
		{
			for(unsigned long h =0;h<m_maxH;++h)
			{
				if(0x01 & (*(m_pMaix+w*m_maxW+h)))
				{
					//ºÚ¿é
					GUI::CWnd::DrawRect(x+8*(h%m_maxH)+4, y+8*(w%m_maxW)+4, 8, 8, COLOR_LINE_BLACK);
				}
				else
				{	//°×¿é
					GUI::CWnd::DrawRect(x+8*(h%m_maxH)+4, y+8*(w%m_maxW)+4, 8, 8, COLOR_LINE_WHITE);
				}
			}
		}
	}
}

unsigned long CPrintQRCodeDlg::OnOK()
{
	return GUI::CDialog::OnOK();	
}

void CPrintQRCodeDlg::SetDesc(char * desc)
{
	strcpy(m_desc, desc);
}
//end
