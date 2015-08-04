#include "ShanghaiVideoErrDialog.h"
#include "ShanghaiConnectMan.h"
CShanghaiVideoErrDialog::CShanghaiVideoErrDialog()
{
	m_checkNameMap.clear();
	m_codeTable = CCharCodeTable::Instance();
}

CShanghaiVideoErrDialog::~CShanghaiVideoErrDialog()
{

}

void CShanghaiVideoErrDialog::GetAssignInfo()
{
	OPER_INFO info;
	memset(&info,0,sizeof(OPER_INFO));
	info.operType = VIDEOMONITOR_ERR;
	info.chnn = 0;
	
	ULONGLONG CurTime = GetCurrTime();
	
	std::string str="GB2312:";
	map<int ,string >::iterator it;
	for(int index=0;index<12;++index)
	{
		if(m_FaultCheck[index].IsCheck())
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
	str += m_editCtrl.GetCaption();
	
	info.pPic =NULL;//没有图片
	info.picLen = 0;//图片长度为0

	int bufSize= str.size();
	char *pTmp = new char[bufSize+1];
	memset(pTmp,0,bufSize+1);
	strcpy(pTmp,str.c_str());
	printf("pTmp=%s,%s,%d\n",pTmp,__FILE__,__LINE__);
	CShanghaiConnectMan::Instance()->GetLogCmd(info.operType,info.chnn,pTmp,bufSize,NULL,0,CurTime);
	if(NULL != pTmp)
	{
		delete []pTmp;
		pTmp = NULL;
	}
	
}

void CShanghaiVideoErrDialog::OnInitial()
{
	GUI::CDialog::OnInitial();

	unsigned short left = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short x= left,y=m_length[DLG_OFFSET_SUB_TOP];
	unsigned short xOffset = 15, yOffset = m_length[CHECK_HEIGHT];
	unsigned short  cx = 3*72, cy = m_length[CHECK_HEIGHT];
	unsigned short xFocus = 0, yFocus = 1;
	
	std::string outStr;
	outStr.clear();
	outStr = ChangeChinese("前端设备");
	m_static[0].Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT],this,outStr.c_str());//前端设备
	
	CreateCheckCtrl(0,x,y,xFocus,yFocus, m_pStrTable->String(E_STR_ID_VIDEO_LOSS));
	CreateCheckCtrl(1,x,y,xFocus,yFocus, "图像异常");			//图像异常
	CreateCheckCtrl(2,x,y,xFocus,yFocus, "传输故障");	//传输故障
	CreateCheckCtrl(3,x,y,xFocus,yFocus, "其他故障");			//其他故障

	x = left;
	y += m_length[CHECK_HEIGHT];
	outStr.clear();
	outStr = ChangeChinese("视频矩阵");
	m_static[1].Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT],this,outStr.c_str()); //视频矩阵
	CreateCheckCtrl(4,x,y,xFocus,yFocus, "监视故障");			//监视故障
	CreateCheckCtrl(5,x,y,xFocus,yFocus, "操作异常");		//操作异常
	CreateCheckCtrl(6,x,y,xFocus,yFocus, "通讯故障");	//通讯故障
	CreateCheckCtrl(7,x,y,xFocus,yFocus, "其他故障");			//其他故障
	
	x = left;
	y += m_length[CHECK_HEIGHT];
	outStr.clear();
	outStr = ChangeChinese("硬盘录像");
	m_static[2].Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT],this,outStr.c_str());	//硬盘录像
	CreateCheckCtrl(8,x,y,xFocus,yFocus, "监视故障");			//监视故障
	CreateCheckCtrl(9,x,y,xFocus,yFocus, "录像异常");			//录像异常
	CreateCheckCtrl(10,x,y,xFocus,yFocus, "回放故障");			//回放故障
	CreateCheckCtrl(11,x,y,xFocus,yFocus, "其他故障");			//其他故障

	x=left;
	y += 2*m_length[CHECK_HEIGHT];
	outStr.clear();
	outStr = ChangeChinese("记录描述");
	m_static[3].Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT],this,outStr.c_str());//记录说明
	
	y += m_length[CHECK_HEIGHT];
	++xFocus;
	m_editCtrl.Create(GetRandWndID(),x,y,m_rect.Width()-2*m_length[DLG_OFFSET_SUB_LEFT],8*m_length[BTN_HEIGHT],this,xFocus,yFocus);
}

void CShanghaiVideoErrDialog::CreateCheckCtrl(int index,unsigned short &x,unsigned short &y,unsigned short &xFocus,unsigned short &yFocus,const char *pstr)
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
	m_FaultCheck[index].Create(GetRandWndID(),x,y,cx,cy,this,xFocus,yFocus);
	std::string outStr;
	outStr.clear();
	outStr = ChangeChinese(pstr);
	m_FaultCheck[index].SetCaption(outStr.c_str(),false,false);	
	
}

string CShanghaiVideoErrDialog::ChangeChinese(const char *pstr)
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

