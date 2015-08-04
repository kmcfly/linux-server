#include "DetailsInfoDlg.h"

using namespace GUI;

CDetailsInfoDlg::CDetailsInfoDlg()
{
	m_cxMaxFirst = 0;
	m_cxMaxSecond = 0;
}

CDetailsInfoDlg::~CDetailsInfoDlg()
{

}

void CDetailsInfoDlg::OnInitial()
{
	GUI::CDialog::OnInitial();

	if(m_cxMaxFirst + m_cxMaxSecond + 40 < 480)
	{
		SetImage(BMP_MEM_DLG_SMALL, false);
	}
	else
	{
		SetImage(BMP_MEM_DLG_NORMAL, false);
	}
	SetDefaultBtn( DEFAULT_BTN_OK );

	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_INFORMATION), false);
}

void CDetailsInfoDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	m_infoVec.clear();
	m_cxMaxFirst = 0;
	m_cxMaxSecond = 0;
}

void CDetailsInfoDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);

	unsigned short x = m_rect.m_left + m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_rect.m_top + m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - m_length[TITLE_HEIGHT] - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_TOP] - m_length[DLG_OFFSET_BOTTOM];

	unsigned short rowHeight = m_length[STATIC_HEIGHT] + 6;
	unsigned short count = m_infoVec.size();

	if(count * rowHeight < height)
	{
		y += (height - count * rowHeight) / 2;
	}

	if((m_cxMaxFirst + m_cxMaxSecond) < (width - 80))
	{
		x += (width - m_cxMaxFirst - m_cxMaxSecond - 20) / 2;
	}

	std::vector<DETAILS_INFO>::iterator iter;
	char strDot[] = ":";

	for(iter = m_infoVec.begin(); iter != m_infoVec.end(); iter++)
	{
		//×ó±ß×Ö·û
		GUI::CWnd::DrawText(iter->first.c_str(), iter->first.length(), x, y, m_cxMaxFirst, m_length[STATIC_HEIGHT]);

		//ÖÐ¼ä :
		GUI::CWnd::DrawText(strDot, strlen(strDot), x + m_cxMaxFirst + 10, y, 10, m_length[STATIC_HEIGHT]);

		//ÓÒ±ß×Ö·û
		GUI::CWnd::DrawText(iter->second.c_str(), iter->second.length(), x + m_cxMaxFirst + 20, y, m_cxMaxSecond, m_length[STATIC_HEIGHT]);

		y += rowHeight;
	}
}

void CDetailsInfoDlg::AddInfo(const std::string& str1, const std::string& str2)
{
	DETAILS_INFO info;
	info.first = str1;
	info.second = str2;

	m_infoVec.push_back(info);

	////
	unsigned short cx1 = 0, cy1 = 0;
	unsigned short cx2 = 0, cy2 = 0;
	TextSize(info.first.c_str(), info.first.length(), cx1, cy1);
	TextSize(info.second.c_str(), info.second.length(), cx2, cy2);

	if(cx1 > m_cxMaxFirst)
	{
		m_cxMaxFirst = cx1;
	}

	if(cx2 > m_cxMaxSecond)
	{
		m_cxMaxSecond = cx2;
	}
}

//end
