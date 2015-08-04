#include "ShanghaiAssignDialog.h"
#include "ShanghaiConnectMan.h"
CShanghaiAssignDialog::CShanghaiAssignDialog()
{
	m_curAssignType = ASSIGN_KEEP;
	m_curRepairType = REPAIR_NONE;
	m_checkNameMap.clear();
	memset(m_repairResult,0,sizeof(m_repairResult));
	m_codeTable = CCharCodeTable::Instance();
}

CShanghaiAssignDialog::~CShanghaiAssignDialog()
{

}

void CShanghaiAssignDialog::GetAssignInfo()
{
	OPER_INFO info;
	memset(&info,0,sizeof(OPER_INFO));

	if(ASSIGN_KEEP == m_curAssignType)
	{
		info.operType = DVRKEEPASSIGN;
	}
	else if(ASSIGN_REPAIR == m_curAssignType)
	{
		info.operType = DVRREPAIRASSIGN;
	}
	info.chnn =0;//和通道无关
	
	ULONGLONG CurTime = GetCurrTime();
	
	std::string str="GB2312:";
	map<int,string >::iterator it;
	for(int index=0;index<8;++index)
	{
		if(m_typeCheck[index].IsCheck())
		{
			it = m_checkNameMap.find(index);
			if(it == m_checkNameMap.end())
			{
				
			}
			else
			{
				str += it->second;
				str += ",";
			}
		}
	}
	//编辑框可能存在的文字
	string editStr =  m_inputEdit.GetCaption();
	string outstr;
	outstr.clear();
	//编辑框文字需要转码，转为gbk看看
	if ( -1 == m_codeTable->Convert(outstr, CCT_CODE_GBK, editStr.c_str(),CCT_CODE_UTF8) )
	{
		outstr = editStr;
	}
	str += outstr;
	//如果是修理，还需要补上修理的结果
	if(0 != strlen(m_repairResult))
	{
		str += m_repairResult;
	}
		
	info.pPic =NULL;//没有图片
	info.picLen = 0;//图片长度为0

	int bufSize= str.size();
	char *pTmp = new char[bufSize+1];
	memset(pTmp,0,bufSize+1);
	strcpy(pTmp,str.c_str());
	printf("pTmp =%s,%s,%d\n",pTmp,__FILE__,__LINE__);
	CShanghaiConnectMan::Instance()->GetLogCmd(info.operType,info.chnn,pTmp,bufSize,NULL,0,CurTime);
	if(NULL != pTmp)
	{
		delete []pTmp;
		pTmp = NULL;
	}
	memset(m_repairResult,0,sizeof(m_repairResult));

}

void CShanghaiAssignDialog::OnInitial()
{
	GUI::CDialog::OnInitial();
	
	unsigned short leftX = m_length[DLG_OFFSET_SUB_LEFT], topY = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short xOffset = 15, yOffset = m_length[CHECK_HEIGHT];
	unsigned short x = leftX, y = topY, cx = 3*72, cy = m_length[CHECK_HEIGHT];
	unsigned short xFocus = 0, yFocus = 1;
	//人员类型签到
	x = (m_rect.Width()- 2*m_length[DLG_OFFSET_SUB_LEFT])/2 - 2*m_length[BTN_WIDTH];
	m_rdAssignKeep.Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,xFocus,yFocus,&m_rdGroupAssign);
	x += 2*m_length[BTN_WIDTH];
	++xFocus;
	m_rdAssignRepair.Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,xFocus,yFocus,&m_rdGroupAssign);
	m_rdGroupAssign.AddRadio(&m_rdAssignKeep);
	m_rdGroupAssign.AddRadio(&m_rdAssignRepair);
	std::string outStr;
	outStr.clear();
	outStr = ChangeChinese("维保签到");
	m_rdAssignKeep.SetCaption(outStr.c_str(),false,false);//保养人员签到
	outStr.clear();
	outStr = ChangeChinese("维修签到");
	m_rdAssignRepair.SetCaption(outStr.c_str(),false,false);//维修人员签到
	m_rdAssignKeep.SetData(ASSIGN_KEEP);
	m_rdAssignRepair.SetData(ASSIGN_REPAIR);
	m_rdAssignKeep.SetCheck(true,false);

	x=leftX;
	y += m_length[BTN_HEIGHT];
	outStr.clear();
	outStr = ChangeChinese("项目选择");
	m_typeStatic.Create(GetRandWndID(),x,y,m_rect.Width()-2*m_length[DLG_OFFSET_SUB_LEFT],m_length[STATIC_HEIGHT],this,outStr.c_str());//项目选择

	CreateTypeCheckCtrl(0,x,y,xFocus,yFocus,"视频监控");	//视频监控
	CreateTypeCheckCtrl(1,x,y,xFocus,yFocus,"周界报警");		//周界报警
	CreateTypeCheckCtrl(2,x,y,xFocus,yFocus,"入侵报警");		//入侵报警
	CreateTypeCheckCtrl(3,x,y,xFocus,yFocus,"楼宇对讲");	//楼宇对讲
	CreateTypeCheckCtrl(4,x,y,xFocus,yFocus,"门禁管理");	//门禁管理
	CreateTypeCheckCtrl(5,x,y,xFocus,yFocus,"出入口控制");	//出入口控制
	CreateTypeCheckCtrl(6,x,y,xFocus,yFocus,"电子巡更");	//电子巡更
	CreateTypeCheckCtrl(7,x,y,xFocus,yFocus,"其他系统");				//其他系统

	y += 2*m_length[CHECK_HEIGHT];
	x = leftX;
	outStr.clear();
	outStr = ChangeChinese("记录描述");
	m_Editstatic.Create(GetRandWndID(),x,y,m_rect.Width()-3*m_length[DLG_OFFSET_SUB_LEFT],m_length[STATIC_HEIGHT],this,outStr.c_str());//记录说明

	y += m_length[CHECK_HEIGHT];
	++xFocus;
	m_inputEdit.Create(GetRandWndID(),x,y,m_rect.Width()-2*m_length[DLG_OFFSET_SUB_LEFT],8*m_length[BTN_HEIGHT],this,xFocus,yFocus);
	//修复结果单选按钮
	++xFocus;
	x	= (m_rect.Width()-2*m_length[DLG_OFFSET_SUB_LEFT])/3-m_length[BTN_WIDTH];
	y = y+8*m_length[BTN_HEIGHT]+25;
	m_rdRepairNone.Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,xFocus,yFocus,&m_rdGroupRepair);
	x+= 2*m_length[BTN_WIDTH]+15;

	++xFocus;
	m_rdRepairPart.Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,xFocus,yFocus,&m_rdGroupRepair);
	x+= 2*m_length[BTN_WIDTH]+15;
	++xFocus;
	m_rdRepairAll.Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,xFocus,yFocus,&m_rdGroupRepair);
	m_rdGroupRepair.AddRadio(&m_rdRepairNone);
	m_rdGroupRepair.AddRadio(&m_rdRepairPart);
	m_rdGroupRepair.AddRadio(&m_rdRepairAll);
	
	outStr.clear();
	outStr = ChangeChinese("故障没修复");
	m_rdRepairNone.SetCaption(outStr,false,false);
	outStr.clear();
	outStr = ChangeChinese("故障部分修复");
	m_rdRepairPart.SetCaption(outStr,false,false);
	outStr.clear();
	outStr = ChangeChinese("故障完全修复");
	m_rdRepairAll.SetCaption(outStr,false,false);
	m_rdRepairNone.SetData(REPAIR_NONE);
	m_rdRepairPart.SetData(REPAIR_PART);
	m_rdRepairAll.SetData(REPAIR_FIX);
	m_rdRepairNone.SetCheck(true,false);
	m_rdRepairNone.Show(false,false);
	m_rdRepairPart.Show(false,false);
	m_rdRepairAll.Show(false,false);

}

BEGIN_MSG_MAP(CShanghaiAssignDialog, GUI::CDialog)
	ON_MSG(m_rdAssignKeep.GetID(),KEY_VIRTUAL_ENTER,OnClickRadioAssign)
	ON_MSG(m_rdAssignRepair.GetID(),KEY_VIRTUAL_ENTER,OnClickRadioAssign)
	ON_MSG(m_rdRepairNone.GetID(),KEY_VIRTUAL_ENTER,OnClickRadioRepair)
	ON_MSG(m_rdRepairPart.GetID(),KEY_VIRTUAL_ENTER,OnClickRadioRepair)
	ON_MSG(m_rdRepairAll.GetID(),KEY_VIRTUAL_ENTER,OnClickRadioRepair)
END_MSG_MAP()

unsigned long CShanghaiAssignDialog::OnClickRadioAssign()
{
	GUI::CRadioCtrl* pRadio = m_rdGroupAssign.GetCheckRadio();
	ASSIGN_TYPE type= static_cast<ASSIGN_TYPE>(pRadio->GetData());
	if(ASSIGN_KEEP == type)
	{
		m_rdRepairNone.Show(false,false);
		m_rdRepairPart.Show(false,false);
		m_rdRepairAll.Show(false,false);

	}
	else if(ASSIGN_REPAIR == type)
	{
		m_rdRepairNone.Show(true,false);
		m_rdRepairPart.Show(true,false);
		m_rdRepairAll.Show(true,false);
		m_rdRepairNone.SetCheck(true,false);//默认选择没修复
		strcpy(m_repairResult,"故障没修复,");
	}
	m_curAssignType = type;
	Repaint();
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CShanghaiAssignDialog::OnClickRadioRepair()
{
	GUI::CRadioCtrl* pRadio = m_rdGroupRepair.GetCheckRadio();
	REPAIR_INFO type= static_cast<REPAIR_INFO>(pRadio->GetData());
	memset(m_repairResult,0,sizeof(m_repairResult));
	switch(type)
	{
	case REPAIR_NONE:
		{
			strcpy(m_repairResult,"故障没修复,");
		}
		break;
	case REPAIR_PART:
		{
			strcpy(m_repairResult,"故障部分修复,");
		}
		break;
	case REPAIR_FIX:
		{
			strcpy(m_repairResult,"故障完全修复,");
		}
		break;
	default:
		memset(m_repairResult,0,sizeof(m_repairResult));
		break;
	}
	return KEY_VIRTUAL_MSG_STOP;
}

void CShanghaiAssignDialog::CreateTypeCheckCtrl(int index,unsigned short &x,unsigned short &y,unsigned short &xFocus,unsigned short &yFocus,const char *pstr)
{
	unsigned short leftX = 50;
	unsigned short xOffset = 15, yOffset = m_length[CHECK_HEIGHT];
	unsigned short cx = 3*72,cy = m_length[CHECK_HEIGHT];
	x = (0 == (index % 4)) ? leftX : (x + cx + xOffset);
	y = (0 == (index % 4)) ? (y + yOffset) : y;
	++xFocus;
	yFocus = (0 == (xFocus % 4)) ? (yFocus + 1) : yFocus;
	xFocus = (xFocus % 4);
	m_checkNameMap.insert(pair<int,string>(index,pstr));
	
	std::string outStr;
	outStr.clear();
	outStr = ChangeChinese(pstr);
	m_typeCheck[index].Create(GetRandWndID(),x,y,cx,cy,this,xFocus,yFocus);
	m_typeCheck[index].SetCaption(outStr,false,false);	
}

string  CShanghaiAssignDialog::ChangeChinese(const char *pstr)
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
