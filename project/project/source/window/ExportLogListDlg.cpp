#include "ExportLogListDlg.h"
#include "MessageMan.h"
#include "IDToStringID.h"

CExportLogListDlg::CExportLogListDlg()
{
	m_pHeadTitle		= NULL;;
	m_ppElementTitle	= NULL;
	m_pData				= NULL;
	m_dataLen			= 0;
	m_fileFormat		= 0;
}
CExportLogListDlg::~CExportLogListDlg()
{

}

void CExportLogListDlg::SetWriteFileInfo(char *pHeadTitle, char **ppElementTitle, char *pData, int dataLen, int fileFormat)
{
	m_pHeadTitle = pHeadTitle;
	m_ppElementTitle = ppElementTitle;
	m_pData = pData;
	m_dataLen = dataLen;
	m_fileFormat = fileFormat;
}

void CExportLogListDlg::OnInitial()
{
	CUSBExploreDlg::OnInitial();

	unsigned short cx = m_rect.Width() - 4 * m_length[BTN_WIDTH] - 4 * m_length[DEFAULT_BTN_OFFSET] - 2 * m_length[DEFAULT_BTN_RIGHT_XPOS];
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_EXPORTLOG), false);

	CreateTipWnd(cx);
}
unsigned long CExportLogListDlg::OnOK()
{
	if(0 < m_dirPathList.GetCount())			//当前有导出设备
	{

		MessageOut( GetTopestWnd(), E_STR_ID_PLEASE_WAIT, TIP_ERR );
		
		switch(m_fileFormat)
		{
		case 0:
			CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_EXPORT_LOG, NULL, 0);
			break;
		case 1:
			CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_EXPORT_EVENT, NULL, 0);
			break;
		default:
			return KEY_VIRTUAL_MSG_STOP;
			break;
		}

		char curPath[512] ={0};						//日志导出的目录
		snprintf(curPath,sizeof(curPath),"%s",m_dirPathList.GetTail().name);

		CCreateXMLFile createXMLFile;

		createXMLFile.SetDescriptionString(IdToString, reinterpret_cast<void *>(this));
		
		createXMLFile.SetExportDir(curPath);

		//多语言是IE上面按钮的提示字符串要和导出的语言相同
		BUTTON_TIP_INFO *pTipInfo = new BUTTON_TIP_INFO [7];
		pTipInfo[0].tipNum = BUTTON_TIP_FIRST;
		snprintf(pTipInfo[0].tipValue, sizeof(pTipInfo[0].tipValue), "%s", m_pStrTable->String(E_STR_ID_IE_LOG_TIP_FIRST));

		pTipInfo[1].tipNum = BUTTON_TIP_PRE;
		snprintf(pTipInfo[1].tipValue, sizeof(pTipInfo[1].tipValue), "%s", m_pStrTable->String(E_STR_ID_IE_LOG_TIP_PRE));

		pTipInfo[2].tipNum = BUTTON_TIP_NEXT;
		snprintf(pTipInfo[2].tipValue, sizeof(pTipInfo[2].tipValue), "%s", m_pStrTable->String(E_STR_ID_IE_LOG_TIP_NEXT));

		pTipInfo[3].tipNum = BUTTON_TIP_LAST;
		snprintf(pTipInfo[3].tipValue, sizeof(pTipInfo[3].tipValue), "%s", m_pStrTable->String(E_STR_ID_IE_LOG_TIP_LAST));

		pTipInfo[4].tipNum = BUTTON_TIP_RECORD;
		snprintf(pTipInfo[4].tipValue, sizeof(pTipInfo[4].tipValue), "%s", m_pStrTable->String(E_STR_ID_IE_LOG_TIP_RECORD));

		pTipInfo[5].tipNum = BUTTON_TIP_IS_THE_LAST;
		snprintf(pTipInfo[5].tipValue, sizeof(pTipInfo[5].tipValue), "%s", m_pStrTable->String(E_STR_ID_IE_LOG_TIP_ISTHELAST));

		pTipInfo[6].tipNum = BUTTON_TIP_IS_THE_FIRST;
		snprintf(pTipInfo[6].tipValue, sizeof(pTipInfo[6].tipValue), "%s", m_pStrTable->String(E_STR_ID_IE_LOG_TIP_ISTHEFIRST));
		
		createXMLFile.SetButtonTip(pTipInfo, 7);

		
		bool bRetVal = createXMLFile.CreateXMLFile(	reinterpret_cast<const char *>(m_pHeadTitle),const_cast<const char **>(m_ppElementTitle), reinterpret_cast<const char *>(m_pData),m_dataLen,static_cast<CCreateXMLFile::XML_FORMAT>(m_fileFormat));
		bRetVal = createXMLFile.CopyHtmlFile( bRetVal );

		if (NULL != pTipInfo)
		{
			delete [] pTipInfo;
			pTipInfo = NULL;
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
		
		if(bRetVal)
		{
			MessageOut( GetTopestWnd(), E_STR_ID_TIP_04_05, TIP_ERR );
		}
		else
		{
			MessageOut( GetTopestWnd(), E_STR_ID_LOG_ERROR, TIP_ERR );
		}
		
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}
	

	return KEY_VIRTUAL_MSG_STOP;
}
void CExportLogListDlg::EndDialog(DLG_END_VALUE result)
{
	CUSBExploreDlg::EndDialog(result);
}



const char * CExportLogListDlg::IdToString(unsigned long id, long xmlFormat, void *pParam)
{

	if (NULL == pParam)
	{
		assert(false);
		return "";
	}
	else
	{
		CExportLogListDlg *pThis = reinterpret_cast<CExportLogListDlg *>(pParam);
		return pThis->String(id, xmlFormat);
	}

}


const char * CExportLogListDlg::String(unsigned long id, long xmlFormat)
{
	static char str[10] = {0};

	if ( 0 == xmlFormat )
	{
		unsigned long strID = LogIDToStrID(id);
		return CStringTable::Instance()->String(static_cast<ENUM_STRING_ID>(strID));

	} 
	else if( 1 == xmlFormat )
	{
		switch(id)
		{
		case EVENT_TYPE_MOTION:
			{
				return CStringTable::Instance()->String(E_STR_ID_MOTION);

			}
			break;
		case EVENT_TYPE_SENSOR:
			{
				return CStringTable::Instance()->String(E_STR_ID_SENSOR);

			}
			break;
		case EVENT_TYPE_V_LOSS:
			{
				return CStringTable::Instance()->String(E_STR_ID_VIDEO_LOSS);

			}
			break;
		default:
			assert(false);
			return str;
		}
	}

	assert(false);
	return str;
}
