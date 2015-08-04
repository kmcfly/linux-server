#include "PreBackUpDlg.h"
#include "MessageMan.h"
#include "ListCtrlEx.cpp"
#include "PreBackupAudioInfo.h"
#include "CutKeepLog.h"
int MAX_BACKUP_SEQ=33;
CPreBackUpDlg::CPreBackUpDlg()
{
	m_line=0;
}

CPreBackUpDlg::~CPreBackUpDlg()
{

}

void CPreBackUpDlg::SetSEARCHData(BACKUP_DATA_SEQ_VECTOR *pBackupData)
{
	CpreBackupAudioInfo audioInfo;
	audioInfo.Initial();
	m_pBackupData = pBackupData;
	for (int i = 0; i < m_pBackupData->size();++i)
	{
		((*m_pBackupData)[i]).hasCalculate = 0;
	}

	vector<BACKUP_DATA_SEQ>::iterator it = m_pBackupData->begin();
	while(it!=m_pBackupData->end())
	{
		if (1 != it->hasCalculate)
		{
			if(audioInfo.OpenAndCheckFile((it->backupinfo).fileInfo.diskIndex,(it->backupinfo).fileInfo.fileIndex, (it->backupinfo).fileInfo.startTime,(it->backupinfo).fileInfo.endTime))
			{
				if (audioInfo.CheckfileAudio())
				{
					it->hasAudio = 1;
					(it->backupinfo).hasBackupAudio = 1;
				}
				else
				{
					it->hasAudio = 0;
					(it->backupinfo).hasBackupAudio = 0;
				}
				it->size = audioInfo.FileSize((it->backupinfo).fileInfo.startTime,(it->backupinfo).fileInfo.endTime);
				audioInfo.CloseFile();

				it->hasCalculate = 1;
				it++;
			}
			else
			{
				m_pBackupData->erase(it);
				it = m_pBackupData->begin();
			}
		}
		else
		{
			it++;
		}

	}
	audioInfo.Quit();

	m_line = m_pBackupData->size();	
}

void CPreBackUpDlg::OnInitial()
{	
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_VIDEO_AUDIO_EDIT), false);
	CreateTipWnd();

	SetDefaultBtn(DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_BTN_APPLY );
	SetDefaultBtnCaption(DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_BACKUP));
	SetDefaultBtnCaption(DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));
	SetDefaultBtnCaption(DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_DELETE));

	unsigned short xLeft = m_length[DLG_OFFSET_LEFT], yTop = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short listWidth = width - 4;
	unsigned short x = xLeft, y = yTop;

	unsigned short listH = m_rect.Height() - yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM] ;
	m_preBackUpList.Create(GUI::CWnd::GetRandWndID(), x, y, listWidth, listH, this, 0, 1, true );

	m_preBackUpList.AddColumn(" ",m_length[CHECK_WIDTH]+5);
	m_preBackUpList.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 2 / 32);
	m_preBackUpList.AddColumn(m_pStrTable->String(E_STR_ID_SEQUENCE), width * 4 / 32);
	if(m_width<=800)
	{
		m_preBackUpList.AddColumn(m_pStrTable->String(E_STR_ID_START_TIME), width * 5 / 32);
		m_preBackUpList.AddColumn(m_pStrTable->String(E_STR_ID_END_TIME), width * 5 / 32);
	}
	else
	{
		m_preBackUpList.AddColumn(m_pStrTable->String(E_STR_ID_START_TIME), width * 7 / 32 + 10);
		m_preBackUpList.AddColumn(m_pStrTable->String(E_STR_ID_END_TIME), width * 7 / 32 + 10);
	}
	m_preBackUpList.AddColumn(m_pStrTable->String(E_STR_ID_AUDIO), width * 2/ 32);
	m_preBackUpList.AddColumn(m_pStrTable->String(E_STR_ID_EDIT), width * 2 / 32);
	
	if(m_width<=800)
	{
		m_preBackUpList.AddColumn(m_pStrTable->String(E_STR_ID_SIZE), width * 4 / 32);
	}
	else
	{
		m_preBackUpList.AddColumn(m_pStrTable->String(E_STR_ID_SIZE), width * 3/ 32);
	}
	m_preBackUpList.AddColumn(m_pStrTable->String(E_STR_ID_KEEP), width * 3/ 32- m_length[SCROLL_WIDTH] - 2);
	//调整List位置
	GUI::CRect rect = m_preBackUpList.GetClientRect();
	unsigned short yOff = ( m_rect.Height()- yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM] - rect.Height() )  / 2;
	x = xLeft;
	y = yTop + yOff;
	m_preBackUpList.MoveWnd( m_rect.m_left + x, m_rect.m_top + y );

	CreateCtrl();
	m_preBackUpList.CreateListFocusMap();
	m_checkAll.Create(GetRandWndID(),xLeft,m_rect.Height()-m_length[DEFAULT_BTN_BOTTOM_YPOS]-5,m_length[BTN_WIDTH],m_length[CHECK_HEIGHT],this,0,2);
	m_checkAll.SetCaption(m_pStrTable->String(E_STR_ID_ALL),false);
	m_checkAll.SetCheck(false,false);
	
}
void CPreBackUpDlg::CreateCtrl()
{
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo=NULL;
	GUI::CPictureCtrl* picAudio=NULL;//音频图标
	GUI::CCheckCtrl* pKeepCheck=NULL;//保存状态
	if(m_line==0)
	{
		return;
	}
	unsigned long saveseq=0;
	bool bfind=false;
	//根据保存列表确定的最大顺序
	for(int i=0;i<m_line;i++)
	{
		if((((*m_pBackupData)[i]).seq > saveseq)&&(((*m_pBackupData)[i]).seq!=MAX_BACKUP_SEQ))
		{
			saveseq=(*m_pBackupData)[i].seq;
			bfind=true;
		}
	}
	if(bfind)
	{
		saveseq=saveseq+1;
	}
	printf("saveseq=%d\n",saveseq);

	char str[128] = {0};
	std::string strtime;
	
	for(int i=0;i<m_line;i++)
	{
		
		BACKUP_DATA_SEQ tempInfo;
		tempInfo = (*m_pBackupData)[i];


		pCheck=dynamic_cast<GUI::CCheckCtrl*>( m_preBackUpList.AddItem(i, COL_CHECK, WND_TYPE_CHECK));
		if (1 == tempInfo.hasCheck)
		{
			pCheck->SetCheck(true,false);
		}
		else
		{
			pCheck->SetCheck(false, false);
		}
		
		snprintf(str,128,"%d",tempInfo.backupinfo.fileInfo.chnn+1);
		m_preBackUpList.AddItem(i,COL_CHNN,str);

		pCombo=dynamic_cast<GUI::CComboCtrl*>( m_preBackUpList.AddItem( i,COL_SEQ, WND_TYPE_COMBO));
		unsigned long maxseq=0,tmpseq=0;
		if((m_line<32)&&(m_line>1))
		{
			tmpseq=m_line+1;
		}
		else if((m_line >= 32)||(m_line==0))
		{
			tmpseq=MAX_BACKUP_SEQ;
		}
		else if(m_line==1)
		{
			tmpseq=1;
		}

		if(tmpseq>=saveseq)
		{
			maxseq=tmpseq;
		}
		else
		{
			maxseq=saveseq;
		}

		for(int j = 1; j < maxseq; ++j)
		{
			snprintf(str,128,"%d",j);
			pCombo->AddItem(str,j);
		}
		pCombo->AddItem(" ",MAX_BACKUP_SEQ);

		if (tempInfo.seq < 1)
		{
			pCombo->SetCurItemData(1,false);
		}
		else if (tempInfo.seq  > MAX_BACKUP_SEQ)
		{
			pCombo->SetCurItemData(MAX_BACKUP_SEQ,false);
		}
		else
		{
			pCombo->SetCurItemData(tempInfo.seq ,false);
		}	
			
		strtime.empty();
		CTimeCtrl::GetTime32Text(strtime,tempInfo.backupinfo.fileInfo.startTime);
		strcpy(str,strtime.c_str());
		m_preBackUpList.AddItem(i,COL_STARTTIME,str);

		strtime.empty();
		CTimeCtrl::GetTime32Text(strtime,tempInfo.backupinfo.fileInfo.endTime);
		strcpy(str,strtime.c_str());
		m_preBackUpList.AddItem(i,COL_ENDTIME,str);

		if (1 == tempInfo.hasAudio)
		{			
			picAudio=dynamic_cast<GUI::CPictureCtrl*>( m_preBackUpList.AddItem( i, COL_AUDIO, WND_TYPE_PICTURE));
			pCheck=dynamic_cast<GUI::CCheckCtrl*>( m_preBackUpList.AddItem( i, COL_EDIT, WND_TYPE_CHECK));
			if (0 == tempInfo.backupinfo.hasBackupAudio)
			{
				pCheck->SetCheck(false,false);	
			}
			else
			{
				pCheck->SetCheck(true, false);
			}			
		}
		
		float fileEditSize =  (tempInfo.size / ((float)1024)) /((float)1024) ;		
		snprintf(str,128, "%0.1fMB", fileEditSize);			
		m_preBackUpList.AddItem(i,COL_SIZE,str);	
		//新加一个check框
		pKeepCheck=dynamic_cast<GUI::CCheckCtrl*>( m_preBackUpList.AddItem( i, COL_KEEP, WND_TYPE_CHECK));
		if(0 == tempInfo.changeTime)//永久保留
		{
			pKeepCheck->SetCheck(true,false);
		}
		else
		{
			pKeepCheck->SetCheck(false,false);
		}

	}
	
	
}
void  CPreBackUpDlg::OnDestroy()
{
	GUI::CWnd::OnDestroy();
}

BEGIN_MSG_MAP(CPreBackUpDlg, GUI::CDialog)
	ON_MSG_EX(m_preBackUpList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG(m_checkAll.GetID(),KEY_VIRTUAL_ENTER,OnCheckAll)
END_MSG_MAP()

unsigned long CPreBackUpDlg::OnCheckAll()
{
	GUI::CCheckCtrl* pCheck = NULL;//是否勾选选择整项
	
	if(m_checkAll.IsCheck())
	{
		//所有的都被勾选
		for(int i=0;i<m_line;i++)
		{
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_preBackUpList.GetItemWnd(i, COL_CHECK) );
			((*m_pBackupData)[i]).hasCheck=1;
			pCheck->SetCheck(true,false);
		}
	}
	else
	{
		for(int i=0;i<m_line;i++)
		{
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_preBackUpList.GetItemWnd(i, COL_CHECK) );
			((*m_pBackupData)[i]).hasCheck=0;
			pCheck->SetCheck(false,false);
		}
	}
	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CPreBackUpDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	unsigned long ret = KEY_VIRTUAL_MSG_STOP;
	GUI::CCheckCtrl* pCheck = NULL;//是否勾选选择整项
	GUI::CCheckCtrl* pEditCheck=NULL;//是否勾选选择声音
	GUI::CComboCtrl* pCombo=NULL;
	GUI::CCheckCtrl* pKeepCheck=NULL;
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_preBackUpList.GetItemWnd(row, COL_CHECK) );
	pEditCheck= dynamic_cast<GUI::CCheckCtrl*>(m_preBackUpList.GetItemWnd(row,COL_EDIT));
	pCombo=dynamic_cast<GUI::CComboCtrl*>(m_preBackUpList.GetItemWnd(row,COL_SEQ));
	pKeepCheck=dynamic_cast<GUI::CCheckCtrl*>(m_preBackUpList.GetItemWnd(row, COL_KEEP) );
	switch (col)
	{
		case COL_CHECK:
			{
				assert(pCheck!=NULL);
				if (pCheck->IsCheck())
				{
					(*m_pBackupData)[row].hasCheck = 1;
				}
				else
				{
					(*m_pBackupData)[row].hasCheck = 0;
				}
				ret = KEY_VIRTUAL_MSG_CONTINUE;				
			}
			break;
		case COL_SEQ:
			{
				//如果选择了2个相同的顺序，提示出错,当前选择项设置为默认
				bool bsame=false;//是否选择了2个相同的值
				int seq=pCombo->GetCurItemData();				
				char str[128]={0};

				//在列表中查找该项目的值
				if (seq != MAX_BACKUP_SEQ)
				{
					for(int i=0;i<m_line;i++)
					{
						if(((*m_pBackupData)[i].seq == seq)&&(i!=row))
						{							
							MessageBox(m_pStrTable->String(E_STR_ID_SEQ_USED), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);	
							pCombo->SetCurItemData(MAX_BACKUP_SEQ,false);
							bsame=true;
							(*m_pBackupData)[row].seq = MAX_BACKUP_SEQ;
							break;
						}
					}

					if((!bsame))
					{
						(*m_pBackupData)[row].seq=seq;
					}
				}
				else
				{
					(*m_pBackupData)[row].seq=seq;
				}
				ret = KEY_VIRTUAL_MSG_CONTINUE;				
			}
			break;
		case COL_AUDIO://点击音频图标不刷新，防止闪烁图标
			{
				return ret;
			}
			break;
		case COL_EDIT:
			{				
				//检查ROW_CHECK有无勾选
				if(pEditCheck!=NULL)
				{
					if(pEditCheck->IsCheck())
					{
						(*m_pBackupData)[row].backupinfo.hasBackupAudio=1;
					}
					else
					{
						(*m_pBackupData)[row].backupinfo.hasBackupAudio=0;
					}
				}
				ret = KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case COL_KEEP:
			{
				if(pKeepCheck!=NULL)
				{
					if(pKeepCheck->IsCheck())
					{
						(*m_pBackupData)[row].changeTime=0;
					}
					else
					{
						(*m_pBackupData)[row].changeTime=GetCurrTime32();
					}
				}
			}
			break;
		default:
			break;
	}
	Repaint();
	return ret;
}
unsigned long  CPreBackUpDlg::OnOK()
{	
	
	MESSAGE_DATA msgData;
	DVR_DISK_INFO *pPartionInfo = NULL;
	DVR_DISK_INFO_LIST partionInfoList;
	int partionNum = 0;
	partionInfoList.RemoveAll();
	memset(&msgData, 0, sizeof(MESSAGE_DATA));	
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_REMOVABLE_PARTION, NULL, 0, msgData);
	pPartionInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
	partionNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
	if (partionNum > 0)
	{
		for (int i = 0; i < partionNum; ++i)
		{	
			DVR_DISK_INFO &tmp =	pPartionInfo[i];
			partionInfoList.AddTail(tmp);
		}
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	
	if (0 == partionInfoList.GetCount())
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07),m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}


	//按照顺序传送数据给新的页面
	//首先查找列表，抛掉无勾选的,并按照seq顺序排好，没有顺序的按照row排序
	//启用2个临时数组，分别存放有seq的（按照seq排序）和没有seq的（按照row排序）
	//最后把临时数组内的东西复制到一起，整个传给Backupdlg

	FILE_INFO_EX_BACKUP *pDataBackup = NULL;
	int fileNum = 0;
	unsigned long totalSize=0;
	for (int index = 0; index < m_line; ++index)
	{
		if ((*m_pBackupData)[index].hasCheck==1)
		{
			fileNum++;
			totalSize+=(*m_pBackupData)[index].size;
		}
	}
	if (0 == fileNum)
	{
		partionInfoList.RemoveAll();
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_50), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (totalSize > 512*1024*1024 - 10*1024*1024)
	{
		partionInfoList.RemoveAll();
		MessageBox(m_pStrTable->String(E_STR_ID_NO_EXCEED), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}

	pDataBackup = new FILE_INFO_EX_BACKUP[fileNum];
	
	int count = 0;
	for (int seqTemp = 1; seqTemp <= MAX_BACKUP_SEQ; seqTemp++)
	{		
		for (int fileIndexTemp = 0; fileIndexTemp < m_line; ++fileIndexTemp)
		{			
			if (((*m_pBackupData)[fileIndexTemp].hasCheck==1) && ((*m_pBackupData)[fileIndexTemp].seq == seqTemp))
			{				
				pDataBackup[count] = (*m_pBackupData)[fileIndexTemp].backupinfo;
				count++;
				if (seqTemp < MAX_BACKUP_SEQ)
				{
					break;
				}				
			}
		}		
		
	}	

	//此处应该放到下一个类里面，现在先打印
	CBackUpShowInfoDlg BackUpShowInfoDlg;
	BackUpShowInfoDlg.SetInitialInfo(&partionInfoList, pDataBackup, fileNum, totalSize);
	BackUpShowInfoDlg.DoModal();
	
	if (NULL != pDataBackup)
	{
		delete [] pDataBackup;
		pDataBackup = NULL;
	}	
	partionInfoList.RemoveAll();

	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CPreBackUpDlg::OnCancel()
{
	GUI::CDialog::OnCancel();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long  CPreBackUpDlg::OnApply()
{
	GUI::CDialog::OnApply();

	
	for(vector<BACKUP_DATA_SEQ>::iterator it = m_pBackupData->begin();it!=m_pBackupData->end();)
	{
		if (1 == it->hasCheck)
		{
			m_pBackupData->erase(it);
			it = m_pBackupData->begin();
		}
		else
		{
			it++;
		}
	}
	
	for(vector<BACKUP_DATA_SEQ>::iterator it =m_pBackupData->begin();it!=m_pBackupData->end();it++)
	{
		printf("it chnn=%d\n",it->backupinfo.fileInfo.chnn);
	}

	CCutKeepLog KeepLog;
	KeepLog.Dellogfile();
	m_preBackUpList.RemoveAllItem(false);
	m_line=m_pBackupData->size();
	CreateCtrl();
	Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}


//右键退出
void CPreBackUpDlg::EndDialog(DLG_END_VALUE result)
{
	WriteCutFile();
	CDialog::EndDialog(result);	
}

void CPreBackUpDlg::WriteCutFile()
{
	//退出时候保存到cutkeepfile文件
	CCutKeepLog KeepLog;
	if (!KeepLog.OpenKeepFile())
	{
		return ;
	}
	if (KeepLog.WriteKeepFile(m_pBackupData))
	{
		KeepLog.Quit();
	}
	else
	{
		KeepLog.Dellogfile();
		KeepLog.Quit();
	}	
}

