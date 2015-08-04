#include "ExportHtmlLog.h"
CExportHtmlLog::CExportHtmlLog()
{
	m_pHeadTitle		= NULL;
	m_ppElementTitle	= NULL;
	m_pData				= NULL;
	m_dataLen			= 0;
	m_fileFormat		= 0;
	memset(m_desLogPath,0,sizeof(m_desLogPath));
	memset(m_dirname,0,sizeof(m_dirname));
}

CExportHtmlLog::~CExportHtmlLog()
{
	m_pHeadTitle		= NULL;;
	m_ppElementTitle	= NULL;
	m_pData				= NULL;
	m_dataLen			= 0;
	m_fileFormat		= 0;
	memset(m_desLogPath,0,sizeof(m_desLogPath));
	memset(m_dirname,0,sizeof(m_dirname));

}

void CExportHtmlLog::SetWriteFileInfo(bool bUseCDBackup, char *pHeadTitle, char **ppElementTitle, char *pData, int dataLen, int fileFormat,char desLogPath[])
{
	m_pHeadTitle = pHeadTitle;
	m_ppElementTitle = ppElementTitle;
	m_pData = pData;
	m_dataLen = dataLen;
	m_fileFormat = fileFormat;
	strcpy(m_desLogPath,desLogPath);
	m_bUseCDBackup = bUseCDBackup; 
}

void CExportHtmlLog::SendDirname(char *dirname)
{
	assert(dirname!=NULL);
	strcpy(dirname,m_dirname);
}

bool CExportHtmlLog::ExportLog()
{
	switch(m_fileFormat)
	{
	case 0:
		CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_EXPORT_LOG, NULL, 0);
		break;
	case 2:
		printf("backup event log\n");
		break;
	default:
		return false;
	}

	CCreateXMLFile createXMLFile;
	createXMLFile.SetDescriptionString(IdToString, reinterpret_cast<void *>(this));
	createXMLFile.SetExportDir(m_desLogPath);
	createXMLFile.SetUseCDBackup(m_bUseCDBackup);

	//多语言是IE上面按钮的提示字符串要和导出的语言相同
	BUTTON_TIP_INFO *pTipInfo = new BUTTON_TIP_INFO [7];
	pTipInfo[0].tipNum = BUTTON_TIP_FIRST;
	snprintf(pTipInfo[0].tipValue, sizeof(pTipInfo[0].tipValue), "%s", CStringTable::Instance()->String(E_STR_ID_IE_LOG_TIP_FIRST));

	pTipInfo[1].tipNum = BUTTON_TIP_PRE;
	snprintf(pTipInfo[1].tipValue, sizeof(pTipInfo[1].tipValue), "%s", CStringTable::Instance()->String(E_STR_ID_IE_LOG_TIP_PRE));

	pTipInfo[2].tipNum = BUTTON_TIP_NEXT;
	snprintf(pTipInfo[2].tipValue, sizeof(pTipInfo[2].tipValue), "%s", CStringTable::Instance()->String(E_STR_ID_IE_LOG_TIP_NEXT));

	pTipInfo[3].tipNum = BUTTON_TIP_LAST;
	snprintf(pTipInfo[3].tipValue, sizeof(pTipInfo[3].tipValue), "%s", CStringTable::Instance()->String(E_STR_ID_IE_LOG_TIP_LAST));

	pTipInfo[4].tipNum = BUTTON_TIP_RECORD;
	snprintf(pTipInfo[4].tipValue, sizeof(pTipInfo[4].tipValue), "%s", CStringTable::Instance()->String(E_STR_ID_IE_LOG_TIP_RECORD));

	pTipInfo[5].tipNum = BUTTON_TIP_IS_THE_LAST;
	snprintf(pTipInfo[5].tipValue, sizeof(pTipInfo[5].tipValue), "%s", CStringTable::Instance()->String(E_STR_ID_IE_LOG_TIP_ISTHELAST));

	pTipInfo[6].tipNum = BUTTON_TIP_IS_THE_FIRST;
	snprintf(pTipInfo[6].tipValue, sizeof(pTipInfo[6].tipValue), "%s", CStringTable::Instance()->String(E_STR_ID_IE_LOG_TIP_ISTHEFIRST));

	createXMLFile.SetButtonTip(pTipInfo, 7);
	bool bRetVal = createXMLFile.CreateXMLFile(	reinterpret_cast<const char *>(m_pHeadTitle),const_cast<const char **>(m_ppElementTitle), reinterpret_cast<const char *>(m_pData),m_dataLen,static_cast<CCreateXMLFile::XML_FORMAT>(m_fileFormat));
	if(!bRetVal)
	{
		printf("CreateXMLFile err\n");
	}
	else
	{
		const char *pdir=createXMLFile.GetExportPath();
		strcpy(m_dirname,pdir);
	}
	bRetVal = createXMLFile.CopyHtmlFile( bRetVal );

	if (NULL != pTipInfo)
	{
		delete [] pTipInfo;
		pTipInfo = NULL;
	}

	if(!bRetVal)
	{
		return false;
	}
	return true;
}


const char * CExportHtmlLog::IdToString(unsigned long id, long xmlFormat, void *pParam)
{

	if (NULL == pParam)
	{
		assert(false);
		return "";
	}
	else
	{
		CExportHtmlLog *pThis = reinterpret_cast<CExportHtmlLog *>(pParam);
		return pThis->String(id, xmlFormat);
	}

}


const char * CExportHtmlLog::String(unsigned long id, long xmlFormat)
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

