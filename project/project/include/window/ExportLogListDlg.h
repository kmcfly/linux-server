/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2010-04-28
** Name         : ExportLogListDlg.h
** Version      : 1.0
** Description  : 日志导出的界面，该类继承外部存储设备控制类(CUSBExploreDlg),
				  重载OnOK()函数用来导出日志文件到选定的目录中
** Modify Record:
1:
***********************************************************************/

#ifndef _EXPORT_LOGLIST_DLG_H_
#define _EXPORT_LOGLIST_DLG_H_

#include "USBExploreDlg.h"
#include "CreateXMLFile.h"
#include "Dialog.h"

using namespace GUI;
class CExportLogListDlg : public CUSBExploreDlg
{
public:
	CExportLogListDlg();
	~CExportLogListDlg();
	
	void SetWriteFileInfo(char *pHeadTitle, char **ppElementTitle, char *pData, int dataLen, int fileFormat);
protected:
	void OnInitial();
	unsigned long OnOK();
	void EndDialog(DLG_END_VALUE result);

	static const char *IdToString(unsigned long id, long xmlFormat, void *pParam);
	const char *String(unsigned long id, long xmlFormat);
private:
	//m_pHeadTitle, m_pHeadTitle, m_pData所使用的数据空间在LogInfoDlg.cpp中申请和释放
	char *	m_pHeadTitle;					
	char **	m_ppElementTitle;
	char *	m_pData;
	int		m_dataLen;
	int		m_fileFormat;
};
#endif
