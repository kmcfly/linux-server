#if defined(__GROUY_SEARCH__)
#include "SearchAddGroupDlg.h"
#include "StringDefine.h"

CSearchAddGroupDlg::CSearchAddGroupDlg()
{
	m_chnnNum=0;
	m_bIsEdit=false;
	m_nItem=0;
	m_curStr.clear();
	m_pcfgList = NULL;
}

CSearchAddGroupDlg::~CSearchAddGroupDlg()
{

}

void CSearchAddGroupDlg::SetInitialInfo(unsigned long chnnNum, ULONGLONG authCH,GROUP_ITEM_LIST *pcfgGroup)
{
	m_chnnNum = chnnNum;
	m_authCH  = authCH;
	m_pcfgList = pcfgGroup;
	
}
void CSearchAddGroupDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	
	if(m_chnnNum>16)
	{
		SetImage(BMP_MEM_DLG_NORMAL, false);
	}
	else
	{
		SetImage(BMP_MEM_DLG_SMALL, false);
	}
	
	SetDefaultBtn (DEFAULT_BTN_CANCEL |DEFAULT_BTN_OK);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String( E_STR_ID_EXIT ) );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_SAVE) );

	SetCaption(m_pStrTable->String(E_STR_ID_SEARCH_GROUP), false);
	
	unsigned long editWidth = m_length[BTN_WIDTH];
	unsigned long editHeight = m_length[EDIT_HEIGHT];
	unsigned short x = (m_rect.Width() - 5*editWidth/2) / 2, y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short groupX=m_length[DLG_OFFSET_LEFT]+15;
	m_static.Create(GetRandWndID(),groupX, y, 2*editWidth, m_length[STATIC_HEIGHT], this,m_pStrTable->String(E_STR_ID_SEARCH_GROUP_NAME));
	x=groupX+3*editWidth/2;
	m_editAdd.Create(GetRandWndID(), x, y, 3*editWidth, editHeight, this, 0,1);
	m_editAdd.SetCaption("",false);


	unsigned char groupNum = 0;
	groupNum = (m_chnnNum + 3)/4;
		
	m_pCKGroup = new GUI::CCheckCtrl [groupNum];
	y+=(m_length[BTN_HEIGHT] + 13);
	m_selectStatic.Create(GetRandWndID(),groupX, y, 3*editWidth/2, m_length[STATIC_HEIGHT], this,m_pStrTable->String(E_STR_ID_SEARCH_SEL_CH));
	

	
	y+=(m_length[BTN_HEIGHT] + 13);
	unsigned long Checky=y;
	for (int i=0; i<groupNum; ++i)
	{
		m_pCKGroup[i].Create(CTRLID_SEARCH_GROUP_0 + i, x, y, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT], this, 0, i+2);
		y += m_length[CHECK_HEIGHT];
		m_pCKGroup[i].SetCheck(false,false);
	}

	int localChnnNum = CProduct::Instance()->LocalVideoInputNum();
	char ch [] = "32";
	m_pCKCH = new GUI::CCheckCtrl [m_chnnNum];

	y=Checky;
	unsigned long offset = 54;
	for (int i=0; i<m_chnnNum; ++i)
	{
		if(i < localChnnNum)
		{
			sprintf(ch, "%d", i+1);
		}
		else
		{
			sprintf(ch, "D%d", i+1-localChnnNum);
		}

		m_pCKCH[i].Create(CTRLID_SEARCH_CH_0+i, x+(i%4+1)*offset, y+(i/4)*m_length[CHECK_HEIGHT], offset, m_length[CHECK_HEIGHT], this, i%4+1, 1+i/4);
		m_pCKCH[i].SetCaption(ch, false, false);	
		m_pCKCH[i].SetCheck(false,false);
		if (m_authCH & ((ULONGLONG)(0x01) << i))
		{
			m_pCKCH[i].SetEnable(true,false);
		}
		else
		{
			m_pCKCH[i].SetEnable(false,false);
		}
	}
	
	//如果是编辑模式，在页面上刷对应的名字和打钩信息
	if(m_bIsEdit)
	{
		for(GROUP_ITEM_LIST::iterator it=m_pcfgList->begin();it!=m_pcfgList->end();it++)
		{
			if(strcmp(it->szItem,m_curStr.c_str())==0)
			{
				SetChannel(it->chanbits);
				m_editAdd.SetCaption(m_curStr,false);
			}
		}
	}
}

void CSearchAddGroupDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();
	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_OK));
	if (NULL != pBtn)
	{
		pBtn->SetEnable(false, false);
	}
	else
	{
		printf("NULL point\n");
		assert(false);
	}
	
}
void CSearchAddGroupDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);
}
void CSearchAddGroupDlg::GetEditEnable(bool bIsEdit,string curStr,int nItem)
{
	m_bIsEdit = bIsEdit;
	m_curStr = curStr;
	m_nItem = nItem;
}

void CSearchAddGroupDlg::OnDestroy()
{
	if(m_pCKCH!=NULL)
	{
		delete []m_pCKCH;
		m_pCKCH=NULL;
	}
	if(m_pCKGroup!=NULL)
	{
		delete []m_pCKGroup;
		m_pCKGroup=NULL;
	}
}

BEGIN_MSG_MAP(CSearchAddGroupDlg, GUI::CDialog)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_CH_0, CTRLID_SEARCH_CH_31, KEY_VIRTUAL_ENTER, OnClickChnn)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_GROUP_0, CTRLID_SEARCH_GROUP_7, KEY_VIRTUAL_ENTER, OnClickGroup)
	ON_MSG(m_editAdd.GetID(),KEY_VIRTUAL_ENTER,OnClickEdit)
END_MSG_MAP()

unsigned long CSearchAddGroupDlg::OnClickEdit()
{
	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_OK));
	if (NULL != pBtn)
	{
		pBtn->SetEnable(true, false);
	}
	Repaint();
	return  KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CSearchAddGroupDlg::OnOK()
{
	bool bsave=AddGroup();//是否保存成功
	if(!bsave)
	{
		GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_OK));
		if (NULL != pBtn)
		{
			pBtn->SetEnable(false, false);
		}
		Repaint();
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	GUI:CDialog::OnOK();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchAddGroupDlg::OnClickChnn( unsigned long ctrlID )
{
	DoClickChnn( ctrlID - CTRLID_SEARCH_CH_0 , false );
	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_OK));
	if (NULL != pBtn)
	{
		pBtn->SetEnable(true, false);
	}
	Repaint();
	return  KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CSearchAddGroupDlg::OnClickGroup( unsigned long ctrlID )
{
	int lineNum =0;
	lineNum = GetLineNum();
	unsigned long groupNum = ctrlID - CTRLID_SEARCH_GROUP_0;

	bool check = m_pCKGroup[groupNum].IsCheck();
	for ( int i = 0; i < lineNum; i++ )
	{
		if(m_chnnNum <= (groupNum * lineNum + i))
		{
			break;
		}

		if (m_authCH & ((ULONGLONG)(0x01) << (groupNum * 4 + i)))
		{
			
			if( m_pCKCH[groupNum * lineNum + i].IsCheck() != check )
			{
				m_pCKCH[groupNum * lineNum + i].SetCheck( check, false );
			}
		}
	}
	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_OK));
	if (NULL != pBtn)
	{
		pBtn->SetEnable(true, false);
	}
	Repaint();
	return  KEY_VIRTUAL_MSG_CONTINUE;
}


unsigned long CSearchAddGroupDlg::OnCancel()
{
	GUI:CDialog::OnCancel();
	return KEY_VIRTUAL_MSG_STOP;
}

void CSearchAddGroupDlg::SetChannel(const ULONGLONG chanbits)
{
	//设置具体小项
	for (int i = 0; i < m_chnnNum; i++)
	{
		if ((chanbits & ((ULONGLONG)0x01 << i)) && !m_pCKCH[i].IsCheck())
		{
			m_pCKCH[i].SetCheck(true, false);
		}
		else if (!(chanbits & ((ULONGLONG)0x01 << i)) && m_pCKCH[i].IsCheck())
		{
			m_pCKCH[i].SetCheck(false, false);
		}
	}

	//设置4个小项组成的组项
	int groupNum = 0,lineNum=0;
	lineNum=GetLineNum();
	groupNum=GetGroupNum();
	for (int i = 0; i < groupNum; i++)
	{
		bool bCheck = true;
		for (int j = 0; j < lineNum; j++)
		{
			if (!m_pCKCH[lineNum * i + j].IsCheck())
			{
				bCheck = false;
				break;
			}
		}

		if (bCheck)
		{
			m_pCKGroup[i].SetCheck(true, false);
		}
		else
		{
			m_pCKGroup[i].SetCheck(false, true);
		}
	}

	GetRootWnd()->Repaint();
}

ULONGLONG CSearchAddGroupDlg::GetCurChannel() const
{
	ULONGLONG longDate = 0;
	for (int i = 0; i < m_chnnNum; i++)
	{
		if (m_pCKCH[i].IsCheck())
		{
			longDate |= m_authCH & ((ULONGLONG)0x01 << i);
			//printf("%#x\n", longDate);
		}
	}
	return longDate;
}

unsigned long CSearchAddGroupDlg::DoClickChnn( unsigned long index, bool bRefresh )
{
	int groupNum = 0,lineNum=0;
	lineNum = GetLineNum();
	groupNum = index / lineNum;

	bool check = true;
	for ( int i = 0; i < lineNum; i++ )
	{
		if( groupNum * lineNum + i > m_chnnNum - 1 )
		{
			break;
		}

		if( !m_pCKCH[groupNum * lineNum + i].IsCheck() )
		{
			check = false;
			break;
		}
	}

	if( m_pCKGroup[groupNum].IsCheck() != check )
	{
		m_pCKGroup[groupNum].SetCheck( check, bRefresh );
	}

	return KEY_VIRTUAL_MSG_STOP;
}

bool CSearchAddGroupDlg::AddGroup()
{
	string editStr=m_editAdd.GetCaption();
	if((editStr.length()==0)||(editStr == " "))
	{
		//长度等于0，是有问题的
		MessageBox(m_pStrTable->String(E_STR_ID_SEARCH_INPUT_NAME),  m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		return false;
	}
	//在配置里面搜索是否有相同的名字
	if(!m_bIsEdit)
	{
		for(GROUP_ITEM_LIST::iterator it=m_pcfgList->begin();it!=m_pcfgList->end();it++)
		{
			if(strcmp(it->szItem,editStr.c_str())==0)
			{
				MessageBox(m_pStrTable->String(E_STR_ID_SEARCH_INPUT_AGAIN),  m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
				return false;
			}
		}
	}
	
	ULONGLONG chanbits = GetCurChannel();
	if(chanbits == 0)//没有勾选的不允许保存
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34),  m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		return false;
	}

	if(m_bIsEdit)
	{
		for(GROUP_ITEM_LIST::iterator it=m_pcfgList->begin();it!=m_pcfgList->end();it++)
		{
			if(strcmp(it->szItem,m_curStr.c_str())==0)
			{
				snprintf(it->szItem,32,"%s",editStr.c_str());
				it->chanbits = chanbits;
				m_curStr = editStr;
				break;
			}
		}
	}
	else
	{
		int totalnum=0;
		totalnum = m_pcfgList->size();

		if(totalnum>=MAX_GROUP_ITEM_COUNT)
		{
			MessageBox("out of max group num", m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		}
		else
		{
			NCFG_INFO_GROUP_ITEM item;
			snprintf(item.szItem,32,"%s",editStr.c_str());
			item.chanbits=chanbits;
			m_pcfgList->push_back(item);
			m_nItem = m_pcfgList->size()-1;
		}
	}
	
	return true;
}

int CSearchAddGroupDlg::SendCurnItem()
{
	return m_nItem;
}

int  CSearchAddGroupDlg::GetLineNum()
{
	int lineNum=0;
	lineNum =4;
	return lineNum;
}

int  CSearchAddGroupDlg::GetGroupNum()
{
	int groupNum=0;
	groupNum =m_chnnNum/4;
	return groupNum;
}

#endif
//End


