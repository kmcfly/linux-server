
#include "FuncCustomDlg.h"
#include "CheckCtrl.h"
#include "FuncCustom.h"
#include "MessageMan.h"

using namespace GUI;

CFuncCustomDlg::CFuncCustomDlg()
{

}

CFuncCustomDlg::~CFuncCustomDlg()
{

}

void CFuncCustomDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetCaption("SETTING", false);
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] - 2;
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - 2 * m_length[DLG_OFFSET_BOTTOM];

	m_listCtrl.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 0, false);
	m_listCtrl.AddColumn(NULL, cx / 2);
	m_listCtrl.AddColumn(NULL, cy / 2);

	m_listCtrl.AddItem(0, 0, "Enable PTZ:");
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.AddItem(0, 1, WND_TYPE_CHECK));
	assert(pCheck);
	pCheck->SetCheck(IsSupportPTZ(), false);

	m_listCtrl.CreateListFocusMap();
}

unsigned long CFuncCustomDlg::OnOK()
{
	unsigned long bitCount = sizeof(unsigned char) * 8;
	unsigned char func[64];
	memset(func, 0xFF, sizeof(func));

	//校验位
	func[0] = 0xC3;
	func[5] = 0x3C;

	//PTZ是否启用
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.GetItemWnd(0, 1));
	assert(pCheck);
	if(!pCheck->IsCheck())
	{
		func[1 + FUNC_PTZ_CTRL/bitCount] &= (~(0x01 << (FUNC_PTZ_CTRL % bitCount)));
	}

	if(DLG_OK == MessageBox(m_pStrTable->String(E_STR_ID_TIP_00_06), m_pStrTable->String(E_STR_ID_TITLE_WARNING)))
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_SET_FUNC, func, 6, msgData);
		PACKCMD* pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		NET_PROTOCOL cmdType = (NET_PROTOCOL)pCmd->cmdType;
		if (CMD_REPLY_CTRL_SUCC != cmdType)
		{
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			MessageBox(m_pStrTable->String(E_STR_ID_TIP_00_07), m_pStrTable->String(E_STR_ID_TITLE_ERROR));
			return KEY_VIRTUAL_MSG_STOP;
		}
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		return CDialog::OnOK();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//end
