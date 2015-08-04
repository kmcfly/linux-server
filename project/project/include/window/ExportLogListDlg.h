/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2010-04-28
** Name         : ExportLogListDlg.h
** Version      : 1.0
** Description  : ��־�����Ľ��棬����̳��ⲿ�洢�豸������(CUSBExploreDlg),
				  ����OnOK()��������������־�ļ���ѡ����Ŀ¼��
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
	//m_pHeadTitle, m_pHeadTitle, m_pData��ʹ�õ����ݿռ���LogInfoDlg.cpp��������ͷ�
	char *	m_pHeadTitle;					
	char **	m_ppElementTitle;
	char *	m_pData;
	int		m_dataLen;
	int		m_fileFormat;
};
#endif
