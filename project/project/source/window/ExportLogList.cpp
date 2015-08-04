#include "ExportLogList.h"
#include "ExportLogListDlg.h"

GUI::DLG_END_VALUE PopupExportCtrlDlg(GUI::CWnd* pWnd, char *pHeadTitle, char **ppElementTitle, char *pData, int dataLen, int fileFormat)
{
	CExportLogListDlg dlg;

	dlg.SetWriteFileInfo(pHeadTitle, ppElementTitle, pData, dataLen, fileFormat);
	
	return dlg.DoModal(pWnd);
}

