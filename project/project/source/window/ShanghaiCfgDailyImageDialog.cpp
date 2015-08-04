
#include "ShanghaiCfgDailyImageDialog.h"
#include "ListCtrlEx.cpp"
#include "ShanghaiSnapMan.h"
extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	char str [512] = {0};
	//#ifdef __ENVIRONMENT_LINUX__
	SNAP_INFOEX &it = node;

	int ChnnNum = CProduct::Instance()->LocalVideoInputNum()+CProduct::Instance()->NetVideoInputNum();
	if (0 == col)
	{
		for(int i=0;i<ChnnNum;i++)
		{
			if(it.chnnbit & (0x01<<i))
			{
				snprintf(str+strlen(str),512-strlen(str),"%d,",i+1);
			}
		}
	}

	if (1 == col)
	{
		tm tmpTm;
		memset(&tmpTm,0,sizeof(tm));
		tmpTm = DVRTime32ToTm(it.startTime);
		snprintf(str,512,"%02d:%02d:%02d",tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);

	}

	m_str = str;
	return m_str;
}

CShanghaiCfgDailyimageDialog::CShanghaiCfgDailyimageDialog()
{
	m_codeTable = CCharCodeTable::Instance();
}

CShanghaiCfgDailyimageDialog::~CShanghaiCfgDailyimageDialog()
{
	
}

unsigned long CShanghaiCfgDailyimageDialog::SetListData(SNAP_INFOEX *pListData)
{
	pListData = m_listCtrlEx.GetListData();
	return m_listCtrlEx.GetCount();
}

void CShanghaiCfgDailyimageDialog::OnInitial()
{
	CCfgDlg::OnInitial();
	unsigned short choseDlgWidth = 3*(m_length[BTN_WIDTH]+5);
	unsigned short xLeft = m_length[DLG_OFFSET_SUB_LEFT], yTop = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short listWidth = width - choseDlgWidth - 4;
	unsigned short x = xLeft, y = yTop;
	int offset_x = m_length[DEFAULT_BTN_OFFSET];

	unsigned short listH = m_rect.Height() - yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM] ;
	m_listCtrlEx.Create(GUI::CWnd::GetRandWndID(), x, y, listWidth, listH, this, 0, 1, true );
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_CH), listWidth*16/30);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_START_TIME), listWidth*14/30-m_length[CHECK_WIDTH]);
	m_listCtrlEx.EnableSingleSelect();

	//调整List位置
	GUI::CRect rect = m_listCtrlEx.GetClientRect();
	unsigned short yOff = ( m_rect.Height()- yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM] - rect.Height() )  / 2;
	x = xLeft;
	y = yTop + yOff;
	m_listCtrlEx.MoveWnd( m_rect.m_left + x, m_rect.m_top + y );
	x = xLeft+ width - choseDlgWidth;
	m_dailySelectDialog.SetInitialInfo(g_p_login_succ->productInfo.videoInputNum);
	m_dailySelectDialog.Create(GUI::CWnd::GetRandWndID(), x, y, choseDlgWidth, rect.Height(), this, 1, 1);

	string outStr;
	outStr.clear();
	outStr = ChangeChinese("删除");
	x=xLeft;
	y = yTop + rect.Height()+15;
	m_BtnDel.Create(GetRandWndID(),x,y,1.5*m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,2,1,outStr.c_str());

	x = xLeft+ width - choseDlgWidth;
	y = yTop + rect.Height()+15;
	outStr.clear();
	outStr = ChangeChinese("添加");
	m_BtnAdd.Create(GetRandWndID(),x,y,1.5*m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,3,1,outStr.c_str());

}

bool CShanghaiCfgDailyimageDialog::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	switch (cfgID)
	{
	case NCFG_ITEM_NET_SHANGHAI_DAILY_IMAGE:
		{
			assert(len == sizeof(SNAP_INFOEX)*MAX_DAILY_SNAP_NUM);
			int count = 0;
			int i=0;
			int j=0;
			const SNAP_INFOEX* pInfo = reinterpret_cast<const SNAP_INFOEX*>(pData);

			for(i=0;i<MAX_DAILY_SNAP_NUM;i++)
			{
				if(1 == pInfo[i].bEnable)
				{
					++count;
				}
			}

			if(0 < count)
			{
				SNAP_INFOEX *pSnapInfoEx = new SNAP_INFOEX[count];
				for(j=0,i=0;j<MAX_DAILY_SNAP_NUM;j++)
				{
					if(1 == pInfo[j].bEnable)
					{
						pSnapInfoEx[i] = pInfo[j];
						++i;
					}

				}
				m_listCtrlEx.PutData(pSnapInfoEx, count);
				m_BtnDel.SetEnable(true,false);
				pSnapInfoEx = NULL;
			}

		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

bool CShanghaiCfgDailyimageDialog::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);
	switch(cfgID)
	{
	case NCFG_ITEM_NET_SHANGHAI_DAILY_IMAGE:
		{
			assert(len == sizeof(SNAP_INFOEX)*MAX_DAILY_SNAP_NUM);
			SNAP_INFOEX* pInfo =reinterpret_cast<SNAP_INFOEX*>(pData);
			const SNAP_INFOEX* pInfoEx = m_listCtrlEx.GetListData();
			int num = m_listCtrlEx.GetCount();
			if(num >0)
			{
				memset(pInfo,0,sizeof(SNAP_INFOEX)*MAX_DAILY_SNAP_NUM);
				memcpy(pInfo,pInfoEx,sizeof(SNAP_INFOEX)*num);
			}
			else if(0 == num)
			{
				memset(pInfo,0,sizeof(SNAP_INFOEX)*MAX_DAILY_SNAP_NUM);
			}

		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

BEGIN_MSG_MAP(CShanghaiCfgDailyimageDialog, CCfgDlg)
	ON_MSG(m_BtnAdd.GetID(),KEY_VIRTUAL_ENTER,OnBtnAdd)
	ON_MSG(m_BtnDel.GetID(),KEY_VIRTUAL_ENTER,OnBtnDel)
	//ON_MSG(m_BtnSnap.GetID(),KEY_VIRTUAL_ENTER,OnBtnSnap)
END_MSG_MAP()

unsigned long CShanghaiCfgDailyimageDialog::OnBtnAdd()
{
	//先获取列表内容组合成新的再加入列表
	bool bfind = false;
	unsigned long total = 0;
	SNAP_INFOEX *pListData = NULL;
	SNAP_INFOEX snapInfo;
	memset(&snapInfo,0,sizeof(SNAP_INFOEX));
	snapInfo.bEnable =1;
	snapInfo.startTime = m_dailySelectDialog.TimeStart();
	snapInfo.chnnbit = m_dailySelectDialog.CH();

	total = m_listCtrlEx.GetCount();
	pListData = m_listCtrlEx.GetListData();

	for(int i=0;i<total;i++)
	{
		if( 0 == memcmp(&snapInfo,&pListData[i],sizeof(SNAP_INFOEX)) )
		{
			//找到了，已经有了，不再添加
			bfind =true;
			break;
		}
	}

	if(!bfind)
	{
		m_listCtrlEx.AddItem(snapInfo,true);
		m_BtnDel.SetEnable(true,false);
		Repaint();
		if (!IsModify())
		{
			SetModify(true);
			EnableOkBtn(true,false);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}
	else
	{
		MessageBox("param already int the list", m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}
	

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CShanghaiCfgDailyimageDialog::OnBtnDel()
{
	//获取要删除的数据
	SNAP_INFOEX *pDelData = NULL;
	SNAP_INFOEX *pListData = NULL;
	SNAP_INFOEX *pTmpData = NULL;
	unsigned long count = 0;
	unsigned long total = m_listCtrlEx.GetCount();

	pDelData = m_listCtrlEx.GetSelectData(count);
	pListData = m_listCtrlEx.GetListData();
	if(total >0)
	{
		pTmpData = new SNAP_INFOEX[total];
		memset(pTmpData,0,sizeof(SNAP_INFOEX)*total);
		memcpy(pTmpData,pListData,sizeof(SNAP_INFOEX)*total);
	}

	if(count > 0)
	{
		for(int index=0;index<count;++index)
		{
			//在整个列表中寻找是属于哪一行的
			for(int i=0;i<total;i++)
			{
				if( 0 == memcmp(&pDelData[index],&pTmpData[i],sizeof(SNAP_INFOEX)) )
				{
					//找到了，设置该行内容为空
					memset(&pTmpData[i],0,sizeof(SNAP_INFOEX));
				}
			}
		}
		
		m_listCtrlEx.PutData(NULL,0);
		for(int k=0;k<total;++k)
		{
			if(1 == pTmpData[k].bEnable)
			{
				m_listCtrlEx.AddItem( pTmpData[k],false);
			}
		}

		if(NULL != pTmpData)
		{
			delete []pTmpData;
			pTmpData = NULL;
		}
		if( 0 == total)
		{
			m_BtnDel.SetEnable(false,false);
		}
		else
		{
			m_BtnDel.SetEnable(true,false);
		}
		Repaint();
		if (!IsModify())
		{
			SetModify(true);
			EnableOkBtn(true,false);
			
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}

	return KEY_VIRTUAL_MSG_STOP; 	
}

string CShanghaiCfgDailyimageDialog::ChangeChinese(const char *pstr)
{
	string outStr;
	if(NULL != m_codeTable)
	{
		if (-1 == m_codeTable->Convert(outStr, CCT_CODE_UTF8, pstr, CCT_CODE_GBK))
		{
			outStr = pstr;
		}
	}
	else
	{
		outStr = pstr;
	}
	return outStr;
}

//end
