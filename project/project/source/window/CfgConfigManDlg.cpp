/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : CfgConfigManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgConfigManDlg.h"
#include "MessageMan.h"
#include "NetProtocol.h"

extern DVR_SYSTEM_STATUS g_system_status;

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

CCfgConfigManDlg::CCfgConfigManDlg()
{

}

CCfgConfigManDlg::~CCfgConfigManDlg()
{

}

void CCfgConfigManDlg::OnInitial()
{
	CUSBExploreDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);

	unsigned short cx = m_rect.Width() - 5 * m_length[BTN_WIDTH] - 5 * m_length[DEFAULT_BTN_OFFSET] - 2 * m_length[DEFAULT_BTN_RIGHT_XPOS];
	char str [128] = {0};
	//sprintf(str, "%s / %s", m_pStrTable->String(E_STR_ID_IMPORT), m_pStrTable->String(E_STR_ID_EXPORT));
	//SetCaption(str, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_IMPORT_EXPORT), false);

	SetDefaultBtn(DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_IMPORT) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_EXPORT) );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	//////////////////////////////////////////////////////////////////////////
	
	CreateTipWnd(cx);


}
void CCfgConfigManDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();
	unsigned long x = GetWindowRect().m_left + m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 5 * m_length[BTN_WIDTH] - 4 * m_length[DEFAULT_BTN_OFFSET];
	unsigned long y = GetWindowRect().m_top + m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	m_btnAddDir.MoveWnd(x, y);
	x = x + m_length[BTN_WIDTH] + m_length[DEFAULT_BTN_OFFSET];
	m_btnDel.MoveWnd(x, y);
	//Repaint();
}
unsigned long CCfgConfigManDlg::OnOK()
{
	MessageOut( GetTopestWnd(), E_STR_ID_PLEASE_WAIT, TIP_ERR );

	int retval = NETERR_EXPORT_CONFIG_DIRERR;

	if( m_dirPathList.GetCount())
	{
		struct stat exDirStat;
		memset(&exDirStat, 0, sizeof(struct stat));


#ifdef __ENVIRONMENT_LINUX__
		//检查参数是否是目录
		if( ( 0 != stat(m_dirPathList.GetTail().name, &exDirStat) ) || ( !S_ISDIR(exDirStat.st_mode) ) )
		{
			perror("stat()");
			retval =  NETERR_EXPORT_CONFIG_DIRERR;
		}
		else
#endif
		{
			MESSAGE_DATA msgData;
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_EXPORT, NULL, 0, msgData);

			assert(NULL != msgData.pData);
			PACKCMD *pCMD = reinterpret_cast<PACKCMD *>(msgData.pData);
			if(CMD_REPLY_CFG_EXPROT_SUCC == pCMD->cmdType)
			{
				retval = NETERR_EXPORT_CONFIG_OK;
				struct tm curTMTime = DVRTime32ToTm(GetCurrTime32());
				char exportDirPath[120] = {0};
				snprintf( exportDirPath, (sizeof(exportDirPath) - 1),"%s/DVR-%05d-Config-%04d-%02d-%02d-%02d-%02d-%02d.config", m_dirPathList.GetTail().name ,g_p_login_succ->productInfo.deviceID, curTMTime.tm_year + 1900,curTMTime.tm_mon + 1, curTMTime.tm_mday, curTMTime.tm_hour, curTMTime.tm_min, curTMTime.tm_sec);

				FILE *pExFileDis = fopen(exportDirPath, "wb+");
				if( NULL == pExFileDis )
				{
					perror("fopen()");
					retval =  NETERR_EXPORT_CONFIG_DIRERR;
				}
				else
				{
				
					if(1 != fwrite( (msgData.pData + sizeof(PACKCMD) ), pCMD->dataLen, 1, pExFileDis) )
					{
						perror("fwrite()");
						assert(false);

						retval = NETERR_EXPORT_CONFIG_ERR;
					}
					fflush(pExFileDis);
					fclose(pExFileDis);
				}

			}
			else if(CMD_REPLY_CFG_EXPROT_FAIL == pCMD->cmdType)
			{
				retval = *reinterpret_cast<int *>(msgData.pData + sizeof(PACKCMD));
			}

			CMessageMan::Instance()->ReleaseMsgBuff( msgData );

			char curDirPath[256] ={0};
			snprintf(curDirPath, sizeof(curDirPath), "%s", m_dirPathList.GetTail().name);

			if( 0 != EnterDirPath(curDirPath, false))
			{
				OnClickRefresh();
			}
			else
			{
				GetTopestWnd()->Repaint();
			}
		}

	}
	
	SetErrTip(retval);
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgConfigManDlg::OnApply()
{
	if(DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_31), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_QUESTION ))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	int retval = NETERR_EXPORT_CONFIG_FILEERR;

	if(!strlen(m_selectFileName))
	{
		SetErrTip(retval);
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	//读导入文件
	FILE *pImDis = fopen(m_selectFileName, "rb");
	if(NULL == pImDis)
	{
		perror("fopen()");
		assert(false);
		
		SetErrTip(retval);
		return KEY_VIRTUAL_MSG_STOP;

	}

	fseek(pImDis, 0 , SEEK_END);
	long fileLength = ftell(pImDis);
	fseek(pImDis, 0, SEEK_SET);

	if(MAX_FILE_LENGTH < fileLength)				//选择的文件太大，配置文件肯定不会超过1M
	{
		fclose(pImDis);
		pImDis = NULL;

		SetErrTip(retval);
		return KEY_VIRTUAL_MSG_STOP;
	}

	char *pImFileContent = new char [fileLength];
	if(NULL == pImFileContent)
	{
		assert(false);

		fclose(pImDis);
		pImDis = NULL;

		SetErrTip(retval);

		return KEY_VIRTUAL_MSG_STOP;
	}

	if( 1 != fread( pImFileContent, fileLength, 1, pImDis))
	{
		perror("fread()");
		assert(false);

		if(NULL != pImFileContent)
		{
			delete [] pImFileContent;
			pImFileContent = NULL;
		}

		if(NULL != pImDis)
		{
			fclose(pImDis);
			pImDis = NULL;
		}
		

		SetErrTip(retval);
		return KEY_VIRTUAL_MSG_STOP;
	}

	MessageOut( GetTopestWnd(), E_STR_ID_PLEASE_WAIT, TIP_ERR );

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_IMPORT, (unsigned char *)pImFileContent, fileLength, msgData);

	assert(NULL != msgData.pData);
	PACKCMD *pCMD = reinterpret_cast<PACKCMD *>(msgData.pData);
	if(CMD_REPLY_CFG_IMPORT_SUCC == pCMD->cmdType)
	{
		retval = *reinterpret_cast<int *>(msgData.pData + sizeof(PACKCMD));

	}
	else if(CMD_REPLY_CFG_IMPORT_FAIL == pCMD->cmdType)
	{
		retval = *reinterpret_cast<int *>(msgData.pData + sizeof(PACKCMD));
	}

	CMessageMan::Instance()->ReleaseMsgBuff( msgData );
	
	if(NULL != pImFileContent)
	{
		delete [] pImFileContent;
		pImFileContent = NULL;
	}

	if(NULL != pImDis)
	{
		fclose(pImDis);
		pImDis = NULL;
	}
	

	char curDirPath[256] ={0};
	snprintf(curDirPath, sizeof(curDirPath), "%s", m_dirPathList.GetTail().name);


	//刷新当前目录
	if( 0 != EnterDirPath(curDirPath, false))
	{
		OnClickRefresh();
	}
	else
	{
		GetTopestWnd()->Repaint();
	}
	SetErrTip(retval);


	if(NETERR_EXPORT_CONFIG_OK == retval)
	{
		//MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_30), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_QUESTION );
		{
			g_system_status = DVR_SYSTEM_REBOOT;
		}
	}


	return KEY_VIRTUAL_MSG_STOP;
}


void CCfgConfigManDlg::SetErrTip( int errNum )
{
	char errStr[256] = {0};

	switch( errNum )
	{
	case NETERR_EXPORT_CONFIG_OK:
		snprintf(errStr, sizeof(errStr), "%s", m_pStrTable->String(E_STR_ID_TIP_04_05));
		break;

	case NETERR_EXPORT_CONFIG_ERR:							//出错
		snprintf(errStr, sizeof(errStr), "%s.",m_pStrTable->String(E_STR_ID_LOG_ERROR));
		break;

	case NETERR_EXPORT_CONFIG_DIRERR:						//目录不存在或者没有访问权限
		snprintf(errStr, sizeof(errStr), "%s. %s",m_pStrTable->String(E_STR_ID_LOG_ERROR), m_pStrTable->String(E_STR_ID_MESSAGE_07));
		break;

	case NETERR_EXPORT_CONFIG_FILEERR:						//没有选择文件
		snprintf(errStr, sizeof(errStr), "%s. %s",m_pStrTable->String(E_STR_ID_LOG_ERROR), m_pStrTable->String(E_STR_ID_MESSAGE_50));
		break;

	case NETERR_EXPORT_CONFIG_DVRIDERR:						//产品类型不同 Different product types  
		snprintf(errStr, sizeof(errStr), "%s. %s",m_pStrTable->String(E_STR_ID_LOG_ERROR), m_pStrTable->String(E_STR_ID_MESSAGE_13));
		break;

	case NETERR_EXPORT_CONFIG_FIRMWAREERR:					//固件版本不同 Different firmware version
		snprintf(errStr, sizeof(errStr), "%s. %s",m_pStrTable->String(E_STR_ID_LOG_ERROR), m_pStrTable->String(E_STR_ID_MESSAGE_14));
		break;

	case NETERR_EXPORT_CONFIG_OVERONEUSER:					//同时又多个用户执行该操作 At the same time there are multiple users to perform the operation
		snprintf(errStr, sizeof(errStr), "%s. %s",m_pStrTable->String(E_STR_ID_LOG_ERROR), m_pStrTable->String(E_STR_ID_MESSAGE_15));
		break;

	case NETERR_EXPORT_CONFIG_VERSION:						//导入文件中记录的版本和本地版本不一样
		snprintf(errStr, sizeof(errStr), "%s",m_pStrTable->String(E_STR_ID_LOG_ERROR));
		break;

	default:
		break;
	}

	MessageOut( GetTopestWnd(), errStr, TIP_ERR, true );
}


