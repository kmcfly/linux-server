#include "BackDoorLogoDlg.h"
#include "MessageMan.h"

using namespace GUI;

#define BACKDOOR_LOGO_MOUNT_PATH  "/mnt/bdlogo"
#define BACKDOOR_WEB_LOGO_DIR_PATH "/mnt/mtd/WebSites/logo"
#define BACKDOOR_DVR_LOGO_DIR_PATH "/mnt/mtd/ui"

#define BACKDOOR_WEB_LOGO_NAME		"ielogo.png"
#define BACKDOOR_LOGO_NAME			"icon_logo.bmp"
#define BACKDOOR_LOGO_TRANS_NAME	"icon_logo_trans.bmp"

CBackDoorLogoDlg::CBackDoorLogoDlg()
{
	m_pPathWeb = NULL;
	m_pPathDvr = NULL;
	m_pPathDvrTrans = NULL;
}

CBackDoorLogoDlg::~CBackDoorLogoDlg()
{

}

void CBackDoorLogoDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	//////////////////////////////////////////////////////////////////////////

	SetCaption("MODIFY LOGO", false);
	
	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 20;
	
	unsigned short height = m_length[STATIC_HEIGHT];
	unsigned short offset = 6;

	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	unsigned short sc_width[3] = {120, 120, 60};
	sc_width[1] = cx - sc_width[0] - sc_width[2];

	m_scWebDisp.Create(GetRandWndID(), x, y + height, cx, height, this);
	m_scDvrDisp.Create(GetRandWndID(), x, y + height + (height + offset) * 2, cx, height, this);
	m_scDvrTransDisp.Create(GetRandWndID(), x, y + height+ (height + offset) * 4, cx, height, this);

	for(int i = 0; i < 3; i++)
	{
		m_scWeb[i].Create(GetRandWndID(), x, y, sc_width[i], height, this);
		m_scDvr[i].Create(GetRandWndID(), x, y + (height + offset) * 2, sc_width[i], height, this);
		m_scDvrTrans[i].Create(GetRandWndID(), x, y + (height + offset) * 4, sc_width[i], height, this);
	
		x += sc_width[i];
	}

	////
	m_pPathWeb = new char[256];
	m_pPathDvr = new char[256];
	m_pPathDvrTrans = new char[256];

	memset(m_pPathWeb, 0, 256);
	memset(m_pPathDvr, 0, 256);
	memset(m_pPathDvrTrans, 0, 256);
}

void CBackDoorLogoDlg::OnPostInitial()
{
	CDialog::OnPostInitial();

	m_scWeb[0].SetCaption("Client logo:", false);
	m_scDvr[0].SetCaption("DVR logo:", false);
	m_scDvrTrans[0].SetCaption("DVR trans.logo:", false);

	////
	m_scWeb[1].SetCaption(BACKDOOR_WEB_LOGO_NAME, false);
	m_scDvr[1].SetCaption(BACKDOOR_LOGO_NAME, false);
	m_scDvrTrans[1].SetCaption(BACKDOOR_LOGO_TRANS_NAME, false);

	char str[256] = {0};
	snprintf(str, 256, "Size=180x40, Bit depth=16, Format=bmp, Background color = RGB(%d,%d,%d)", R_VALUE(COLOR_DLG_CHILD_BK), G_VALUE(COLOR_DLG_CHILD_BK), B_VALUE(COLOR_DLG_CHILD_BK));
	//

	m_scWebDisp.SetCaption("Size=160x60, Bit depth=32, Format=png, Transparent background", false);
	m_scDvrDisp.SetCaption(str, false);
	m_scDvrTransDisp.SetCaption("Size=180x40, Bit depth=16 Format=bmp, Background color = RGB(255,0,255)", false);
}

void CBackDoorLogoDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	if(m_pPathWeb)
	{
		delete m_pPathWeb;
		m_pPathWeb = NULL;
	}

	if(m_pPathDvr)
	{
		delete m_pPathDvr;
		m_pPathDvr = NULL;
	}
	
	if(m_pPathDvrTrans)
	{
		delete m_pPathDvrTrans;
		m_pPathDvrTrans = NULL;
	}
}

unsigned long CBackDoorLogoDlg::OnOK()
{
	bool ret = CheckImageFile();
	if(ret)
	{
		//¿½±´
		char strCmd[256] = {0};

		//¿Í»§¶Ëlogo
		snprintf(strCmd, 256, "cp %s %s/logo.png", m_pPathWeb, BACKDOOR_WEB_LOGO_DIR_PATH, BACKDOOR_WEB_LOGO_NAME);
		DVRSystem(strCmd);

		//DVR
		snprintf(strCmd, 256, "cp %s %s/icon_logo.bmp", m_pPathDvr, BACKDOOR_DVR_LOGO_DIR_PATH, BACKDOOR_LOGO_NAME);
		DVRSystem(strCmd);

		snprintf(strCmd, 256, "cp %s %s/icon_logo_trans.bmp", m_pPathDvrTrans, BACKDOOR_DVR_LOGO_DIR_PATH, BACKDOOR_LOGO_TRANS_NAME);
		DVRSystem(strCmd);

		UmountUsb();
		return GUI::CDialog::OnOK();
	}
	else
	{
		MessageBox("Logo image error.", "ERROR", DEFAULT_BTN_OK);

		return KEY_VIRTUAL_MSG_STOP;
	}
}

bool CBackDoorLogoDlg::CheckImageFile( )
{
#ifndef __ENVIRONMENT_WIN32__
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;
	DVR_DISK_INFO *pPartionInfo = NULL;
	int partionNum = 0;
	char szCmd[256] = {0};
	CMyList<DVR_DISK_INFO> deviceInfoList;

	
	//
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_REMOVABLE_PARTION, NULL, 0, msgData);
	pPartionInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
	partionNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
	if (partionNum > 0)
	{
		for (int i = 0; i < partionNum; ++i)
		{	
			DVR_DISK_INFO &tmp = pPartionInfo[i];
			deviceInfoList.AddTail(tmp);
		}
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	//
	if (0 == deviceInfoList.GetCount())
	{
		return false;
	}

	//
	bool bOk = false;
	POS pos = deviceInfoList.GetHeadPosition();
	while (NULL != pos)
	{
		DVR_DISK_INFO &diskInfo = deviceInfoList.GetAt(pos);
		snprintf(szCmd, 256, "mkdir -p %s", BACKDOOR_LOGO_MOUNT_PATH);
		DVRSystem(szCmd);
		snprintf(szCmd, 256, "umount %s", BACKDOOR_LOGO_MOUNT_PATH);
		DVRSystem(szCmd);		
		snprintf(szCmd, 256, "mount -t vfat %s %s", diskInfo.diskname, BACKDOOR_LOGO_MOUNT_PATH);
		if (0 != DVRSystem(szCmd))
		{
			deviceInfoList.GetNext(pos);
			snprintf(szCmd, 256, "rm -rf %s", BACKDOOR_LOGO_MOUNT_PATH);
			DVRSystem(szCmd);
			continue;
		}

		
		snprintf(szCmd, 256, "%s/%s", BACKDOOR_LOGO_MOUNT_PATH, BACKDOOR_WEB_LOGO_NAME);
		if (0 != access(szCmd, R_OK))
		{
			snprintf(szCmd, 256, "umount %s", BACKDOOR_LOGO_MOUNT_PATH);
			DVRSystem(szCmd);
			snprintf(szCmd, 256, "rm -rf %s", BACKDOOR_LOGO_MOUNT_PATH);
			DVRSystem(szCmd);
			deviceInfoList.GetNext(pos);
			continue;
		}
		snprintf(m_pPathWeb, 256, "%s", szCmd);

		snprintf(szCmd, 256, "%s/%s", BACKDOOR_LOGO_MOUNT_PATH, BACKDOOR_LOGO_NAME);
		if (0 != access(szCmd, R_OK))
		{
			snprintf(szCmd, 256, "umount %s", BACKDOOR_LOGO_MOUNT_PATH);
			DVRSystem(szCmd);
			snprintf(szCmd, 256, "rm -rf %s", BACKDOOR_LOGO_MOUNT_PATH);
			DVRSystem(szCmd);
			deviceInfoList.GetNext(pos);
			continue;
		}
		snprintf(m_pPathDvr, 256, "%s", szCmd);

		snprintf(szCmd, 256, "%s/%s", BACKDOOR_LOGO_MOUNT_PATH, BACKDOOR_LOGO_TRANS_NAME);
		if (0 != access(szCmd, R_OK))
		{
			snprintf(szCmd, 256, "umount %s", BACKDOOR_LOGO_MOUNT_PATH);
			DVRSystem(szCmd);
			snprintf(szCmd, 256, "rm -rf %s", BACKDOOR_LOGO_MOUNT_PATH);
			DVRSystem(szCmd);
			deviceInfoList.GetNext(pos);
			continue;
		}
		snprintf(m_pPathDvrTrans, 256, "%s", szCmd);

		bOk = true;
		break;
	}
	deviceInfoList.RemoveAll();

	return bOk;
#else
	return false;
#endif
}

void CBackDoorLogoDlg::UmountUsb()
{
	char szCmd[256] = {0};
	snprintf(szCmd, 256, "umount %s", BACKDOOR_LOGO_MOUNT_PATH);
	if (0 == DVRSystem(szCmd))
	{
		snprintf(szCmd, 256, "rm -rf %s", BACKDOOR_LOGO_MOUNT_PATH);
		DVRSystem(szCmd);
	}
}

//end
