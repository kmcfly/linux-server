#include "MessageBox.h"

using namespace GUI;

GUI::CMessageBox::CMessageBox()
{
	m_ctrlType = GUI::WND_TYPE_MESSAGE_BOX;
}

GUI::CMessageBox::CMessageBox(const char* szText,const char* szCaption,unsigned long type) : m_strText(szText)
{
	m_strCaption = szCaption;
	m_defaultCtrlType = type;
}

GUI::CMessageBox::~CMessageBox()
{

}

void GUI::CMessageBox::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_SMALL, false);

//	m_staticText.Create(CTRLID_STATIC, m_length[MSG_TEXT_XPOS], m_length[MSG_TEXT_YPOS], this, m_strText.c_str());

	BITMAP_ID bmpID = GetImageID(m_defaultCtrlType);
	if (BMP_INVALID != bmpID)
	{
		m_picture.Create(CTRLID_PICTURE, m_length[MSG_PICTURE_XPOS], m_length[MSG_PICTURE_YPOS], this, bmpID);
	}

	unsigned short cx = m_rect.Width() - m_length[MSG_TEXT_XPOS] - m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - m_length[MSG_TEXT_YPOS] - m_length[DLG_OFFSET_BOTTOM];
	m_richTextCtrl.Create(GetRandWndID(), m_length[MSG_TEXT_XPOS], m_length[MSG_TEXT_YPOS], cx, cy, this);
	m_richTextCtrl.SetText(m_strText, false);
}

BITMAP_ID GUI::CMessageBox::GetImageID(unsigned long type) const
{
	if (DEFAULT_ICON_ERROR & type)
	{
		return BMP_ERROR;
	}

	if (DEFAULT_ICON_WARNING & type)
	{
		return BMP_WARNING;
	}

	if (DEFAULT_ICON_QUESTION & type)
	{
		return BMP_QUESTION;
	}

	return BMP_INVALID;
}

void GUI::CMessageBox::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);

	//unsigned short len=0, x=m_rect.m_left+m_length[MSG_TEXT_XPOS], y=m_rect.m_top+m_length[MSG_TEXT_YPOS];

	//do 
	//{
	//	len += GUI::CWnd::DrawText(m_strText.c_str()+len, (m_strText.length()-len), x, y, m_rect.Width()-m_length[MSG_TEXT_XPOS]-m_length[MSG_PICTURE_XPOS], m_length[STATIC_HEIGHT]/*m_rect.Height()-y-60*/);

	//	y += 30;
	//} while(len < m_strText.length());

}

//////////////////////////////////////////////////////////////////////////

