#include "ComboCtrlEx.h"
#include "CtrlID.h"
#include "KeyboardNumDlg.h"
#include "NumEditCtrl.h"

using namespace  GUI;

CComboCtrlEx::CComboCtrlEx() : m_curItem(INVALID_ITEM)
, m_pEdit(NULL), m_editEnable(false)
{
	m_ctrlType = GUI::WND_TYPE_COMBO_EX;
}

CComboCtrlEx::~CComboCtrlEx()
{

}

bool CComboCtrlEx::Create(unsigned long ctrlID, 
							unsigned long x, 
							unsigned long y,
							unsigned long cx, 
							unsigned long cy,
							CWnd* pParent, 
							int xFocus, 
							int yFocus,
							bool bNumEdit/* = false*/)
{
	//创建窗口
	unsigned long w = m_length[COMBO_WIDTH] + m_length[COMBO_ARROW_WIDTH] * 2;
	cx = (cx < w) ? w : cx;
	cy = (cy < m_length[COMBO_HEIGHT]) ? m_length[COMBO_HEIGHT] : cy;

	//建立编辑框
	if (bNumEdit)
	{
		m_pEdit = new CNumEditCtrl;
	} 
	else
	{
		m_pEdit = new CEditCtrl;
	}

	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, WND_CHILD|WND_VISIBLE, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CComboCtrlEx::OnDestroy()
{
	GUI::CWnd::OnDestroy();
	m_itemList.clear();
	if (NULL != m_pEdit)
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}
}

void CComboCtrlEx::OnInitial()
{
	GUI::CWnd::OnInitial();

	assert(NULL != m_pEdit);
	m_pEdit->Create(CTRLID_COMBO_EDIT, 0, 0, m_rect.Width() - m_length[COMBO_ARROW_WIDTH] * 2, m_length[COMBO_HEIGHT], this, 0, 0);
	EnableInput(false, false);
	SetMaxStringLen(32);
	
	m_btnLeft.Create(GetRandWndID(), m_rect.Width() - m_length[COMBO_ARROW_WIDTH] * 2, 0, m_length[COMBO_ARROW_WIDTH], m_length[COMBO_HEIGHT], this, 1, 0);
	m_btnLeft.SetImage(PAINT_STATUS_NORMAL, BMP_COMBO_BTN_LEFT, false);
	m_btnLeft.SetImage(PAINT_STATUS_FOCUS, BMP_COMBO_BTN_LEFT_FOCUS, false);
	m_btnLeft.SetImage(PAINT_STATUS_DISABLE, BMP_COMBO_BTN_LEFT_DISABLE, false);
	
	m_btnRight.Create(GetRandWndID(), m_rect.Width() - m_length[COMBO_ARROW_WIDTH], 0, m_length[COMBO_ARROW_WIDTH], m_length[COMBO_HEIGHT], this, 2, 0);
	m_btnRight.SetImage(PAINT_STATUS_NORMAL, BMP_COMBO_BTN_RIGHT, false);
	m_btnRight.SetImage(PAINT_STATUS_FOCUS, BMP_COMBO_BTN_RIGHT_FOCUS, false);
	m_btnRight.SetImage(PAINT_STATUS_DISABLE, BMP_COMBO_BTN_RIGHT_DISABLE, false);
}

void CComboCtrlEx::EnableInput(bool bEnable, bool bRefresh /* = false */)
{
	m_editEnable = bEnable;

	m_pEdit->SetEnable(m_editEnable, bRefresh);
}

int CComboCtrlEx::AddItem(const char* pStr, LONGLONG data/* = 0*/)
{
	COMBO_ITEM comboItem;
	comboItem.item = m_itemList.size();
	comboItem.str = pStr;
	comboItem.data = data;

	m_itemList.push_back(comboItem);
	return comboItem.item;
}

void CComboCtrlEx::RemoveAllItem(bool bRefresh)
{
	m_itemList.clear();
	if (NULL != m_pEdit)
	{
		m_pEdit->SetCaption("", false);
	}

	if (bRefresh)
	{
		Repaint();
	}
}

bool CComboCtrlEx::SetItemData(int item, LONGLONG data)
{
	if (item >= m_itemList.size())
	{
		assert(false);
		return false;
	} 
	else
	{
		m_itemList[item].data = data;
		return true;
	}
}

LONGLONG CComboCtrlEx::GetItemData(int item) const
{
	if ((item >= m_itemList.size()) || (item < 0))
	{
//		assert(false);
		return -1;
	} 
	else
	{
		return m_itemList[item].data;
	}
}

void CComboCtrlEx::SetCurItem(int item, bool bRefresh /* = false */)
{
	if (IsValidItem(item))
	{
		m_pEdit->SetCaption(m_itemList[item].str, bRefresh);
		m_curItem = item;
	}
}

bool CComboCtrlEx::SetCurItemData(LONGLONG data, bool bRefresh)
{
	int item = GetItemFromData(data);
	if (INVALID_ITEM != item)
	{
		SetCurItem(item, bRefresh);
		return true;
	} 
	else
	{
		printf("CComboCtrlEx::SetCurItemData Error ID:%d\n", GetID());
		assert(false);
		return false;
	}
}

LONGLONG CComboCtrlEx::GetCurItemData() const
{
	int item = GetCurItem();
	if (INVALID_ITEM != item)
	{
		return GetItemData(item);
	} 
	else
	{
//		assert(false);
		return -1;
	}
}

int CComboCtrlEx::GetItemFromData(LONGLONG data) const
{
	for (ITEM_VECTOR::const_iterator iter = m_itemList.begin(); iter != m_itemList.end(); ++iter)
	{
		if ((*iter).data == data)
		{
			return (*iter).item;
		}
	}

	return INVALID_ITEM;
}

bool CComboCtrlEx::IsValidItem(int item) const
{
	if ((item >= 0) && (item < m_itemList.size()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CComboCtrlEx::SetCurStringToList()
{
	if (IsValidItem(m_curItem))
	{
		m_itemList[m_curItem].str = m_pEdit->GetCaption();
	}
	else
	{
		AddItem(m_pEdit->GetCaption().c_str());
		SetCurItem(0, true);
	}
}

void CComboCtrlEx::SetMaxStringLen(int maxLen)
{
	assert( m_pEdit );
	m_pEdit->SetMaxStringLen( maxLen );
}

int CComboCtrlEx::GetMaxStringLen() const
{
	assert(m_pEdit);
	return m_pEdit->GetMaxStringLen();
}

void CComboCtrlEx::SetEnable(bool bEnable, bool bRefresh /*= true*/)
{
	GUI::CWnd::SetEnable( bEnable, bRefresh );

	if( m_pEdit )
	{
		m_pEdit->SetEnable( m_editEnable, bRefresh );
	}
}

void CComboCtrlEx::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	return;
}

BEGIN_MSG_MAP(CComboCtrlEx, GUI::CWnd)
	ON_MSG(m_btnLeft.GetID(), KEY_VIRTUAL_ENTER, OnClickLeftBtn)
	ON_MSG(m_btnRight.GetID(), KEY_VIRTUAL_ENTER, OnClickRightBtn)
	//ON_MSG(CTRLID_COMBO_EDIT, KEY_VIRTUAL_ENTER, OnClickEdit)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_ADD, OnKeyAdd)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_DEC, OnKeyDec)
	ON_MSG_KEY_RANGE(CTRLID_COMBO_EDIT, KEY_VIRTUAL_0, KEY_VIRTUAL_9, OnInput)
END_MSG_MAP()

unsigned long CComboCtrlEx::OnInput(unsigned long keyID)
{
	SetCurStringToList();
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CComboCtrlEx::OnClickLeftBtn()
{
	PrevItem();
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CComboCtrlEx::OnClickRightBtn()
{
	NextItem();
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CComboCtrlEx::OnKeyDec()
{
	PrevItem();
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CComboCtrlEx::OnKeyAdd()
{
	NextItem();
	return KEY_VIRTUAL_MSG_CONTINUE;
}

void CComboCtrlEx::PrevItem()
{
	if (!m_itemList.empty())
	{
		--m_curItem;
		if (m_curItem < 0)
		{
			m_curItem = m_itemList.size() - 1;
		}
		SetCurItem(m_curItem, true);
	}
}

void CComboCtrlEx::NextItem()
{
	if (!m_itemList.empty())
	{
		++m_curItem;
		if (m_curItem >= m_itemList.size())
		{
			m_curItem = 0;
		}
		SetCurItem(m_curItem, true);
	}
}
