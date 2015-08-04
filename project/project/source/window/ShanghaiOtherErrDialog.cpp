#include "ShanghaiOtherErrDialog.h"
#include "ShanghaiConnectMan.h"
CShanghaiOtherErrDialog::CShanghaiOtherErrDialog()
{
	m_curSel = 0;
	m_codeTable = CCharCodeTable::Instance();
}

CShanghaiOtherErrDialog::~CShanghaiOtherErrDialog()
{

}

void CShanghaiOtherErrDialog::GetAssignInfo()
{
	OPER_INFO info;
	memset(&info,0,sizeof(OPER_INFO));

	std::string str="GB2312:";
	LONGLONG type = m_errCombo.GetCurItemData();
	switch(type)
	{
	case FAULT_VIDEOALARM:
		{
			info.operType = VIDEOMONITOR_ERR;
			str += "��Ƶ��ع���,";
		}
		break;
	case FAULT_AROUNDALARM:
		{
			info.operType = AROUNDALARM_ERR;
			str += "�ܽ籨������,";
		}
		break;
	case FAULT_NETALARM:
		{
			info.operType = NETALARM_ERR;
			str += "������������,";
		}
		break;
	case FAULT_LOCALALARM:
		{
			info.operType = LOCALALARM_ERR;
			str += "���ر�������,";
		}
		break;
	case FAULT_INTRUSIONALARM:
		{
			info.operType = LOCALALARM_ERR;
			str += "���ֱ�������,";
		}
		break;
	case FAULT_BUILDINGINTERCOM:
		{
			info.operType = BUILDINTERCOM_ERR;
			str += "¥��Խ�����,";
		}
		break;
	case FAULT_ACCESSMANAGE:
		{
			info.operType = BUILDINTERCOM_ERR;
			str += "�Ž��������,";
		}
		break;
	case FAULT_ENTRANCECONTROL:
		{
			info.operType = ENTRANCECONTROL_ERR;
			str += "����ڿ��ƹ���,";
		}
		break;
	case FAULT_ELECTRONICPATROL:
		{
			info.operType = ELECTRIALPATROL_ERR;
			str += "����Ѳ������,";
		}
		break;
	case FAULT_OTHERS:
		{
			info.operType = OTHERSYS_ERR;
			str += "����ϵͳ����,";
		}
		break;
	default:
		break;
	}
	ULONGLONG CurTime = GetCurrTime();
	
	for(int index=0;index<3;++index)
	{
		if(m_FaultCheck[index].IsCheck())
		{
			switch(index)
			{
				case 0:
					{
						str+="ǰ���豸����,";
					}
					break;
				case 1:
					{
						str+="�������,";
					}
					break;
				case 2:
					{
						str += "����ϵͳ����";
					}
					break;
				default:
					break;
			}
		}
	}

	//�༭����ܴ��ڵ�����
	string editStr = m_editCtrl.GetCaption();
	string outstr;
	outstr.clear();
	//�༭��������Ҫת�룬תΪgbk����
	if ( -1 == m_codeTable->Convert(outstr, CCT_CODE_GBK, editStr.c_str(),CCT_CODE_UTF8) )
	{
		outstr = editStr;
	}
	str += outstr;
	info.pPic =NULL;//û��ͼƬ
	info.picLen = 0;//ͼƬ����Ϊ0

	int bufSize= str.size();
	char *pTmp = new char[bufSize+1];
	memset(pTmp,0,bufSize+1);
	strcpy(pTmp,str.c_str());
	CShanghaiConnectMan::Instance()->GetLogCmd(info.operType,info.chnn,pTmp,bufSize,NULL,0,CurTime);
	if(NULL != pTmp)
	{
		delete []pTmp;
		pTmp = NULL;
	}
	
}

void CShanghaiOtherErrDialog::OnInitial()
{
	GUI::CDialog::OnInitial();
	unsigned short left = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short x= left,y=m_length[DLG_OFFSET_SUB_TOP];
	unsigned short xOffset = 15, yOffset = m_length[CHECK_HEIGHT];
	unsigned short  cx = 3*72, cy = m_length[CHECK_HEIGHT];
	unsigned short xFocus = 0, yFocus = 1;

	x = 50;
	std::string outStr;
	outStr.clear();
	outStr = ChangeChinese("��������");
	outStr += ":";
	m_static.Create(GetRandWndID(),x,y,m_length[BTN_WIDTH],m_length[STATIC_HEIGHT],this,outStr.c_str());//��������
	x += m_length[BTN_WIDTH]+5;
	m_errCombo.Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,xFocus,yFocus);	

	outStr.clear();
	outStr = ChangeChinese("��Ƶ��ع���");
	m_errCombo.AddItem(outStr.c_str(),FAULT_VIDEOALARM);	
	outStr.clear();
	outStr = ChangeChinese("�ܽ籨������");
	m_errCombo.AddItem(outStr.c_str(),FAULT_AROUNDALARM);			//�ܽ籨������
	outStr.clear();
	outStr = ChangeChinese("������������");
	m_errCombo.AddItem(outStr.c_str(),FAULT_NETALARM);					//������������
	outStr.clear();
	outStr = ChangeChinese("���ر�������");
	m_errCombo.AddItem(outStr.c_str(),FAULT_LOCALALARM);				//���ر�������
	outStr.clear();
	outStr = ChangeChinese("���ֱ�������");
	m_errCombo.AddItem(outStr.c_str(),FAULT_INTRUSIONALARM);		//���ֱ�������
	outStr.clear();
	outStr = ChangeChinese("¥��Խ�����");
	m_errCombo.AddItem(outStr.c_str(),FAULT_BUILDINGINTERCOM);	//¥��Խ�����
	outStr.clear();
	outStr = ChangeChinese("�Ž��������");
	m_errCombo.AddItem(outStr.c_str(),FAULT_ACCESSMANAGE);		//�Ž��������
	outStr.clear();
	outStr = ChangeChinese("����ڿ��ƹ���");
	m_errCombo.AddItem(outStr.c_str(),FAULT_ENTRANCECONTROL);	//����ڿ��ƹ���
	outStr.clear();
	outStr = ChangeChinese("����Ѳ������");
	m_errCombo.AddItem(outStr.c_str(),FAULT_ELECTRONICPATROL);	//����Ѳ������
	outStr.clear();
	outStr = ChangeChinese("����ϵͳ����");
	m_errCombo.AddItem(outStr.c_str(),FAULT_OTHERS);				//����ϵͳ����
	m_curSel = FAULT_AROUNDALARM;
	m_errCombo.SetCurItemData(m_curSel,false);

	x=left;
	y += m_length[BTN_HEIGHT];
	CreateCheckCtrl(0,x,y,xFocus,yFocus,"ǰ���豸����");	//ǰ���豸����
	CreateCheckCtrl(1,x,y,xFocus,yFocus,"������·����");	//������·����
	CreateCheckCtrl(2,x,y,xFocus,yFocus,"����ϵͳ����");	//����ϵͳ����

	x = left;
	y+= 2*m_length[CHECK_HEIGHT];
	outStr.clear();
	outStr = ChangeChinese("��¼����");
	m_Editstatic.Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[STATIC_HEIGHT],this,outStr.c_str()); //��¼˵��
	++ xFocus;
	y += m_length[STATIC_HEIGHT];
	x = left;
	m_editCtrl.Create(GetRandWndID(),x,y,m_rect.Width()-2*m_length[DLG_OFFSET_SUB_LEFT],10*m_length[BTN_HEIGHT],this,xFocus,yFocus);

}

void CShanghaiOtherErrDialog::CreateCheckCtrl(int index,unsigned short &x,unsigned short &y,unsigned short &xFocus,unsigned short &yFocus,const char *pstr)
{
	unsigned short leftX = 50;
	unsigned short xOffset = 15, yOffset = m_length[CHECK_HEIGHT];
	unsigned short cx = 3*72,cy = m_length[CHECK_HEIGHT];
	x = (0 == (index % 3)) ? leftX : (x + cx + xOffset);
	y = (0 == (index % 3)) ? (y + yOffset) : y;
	++xFocus;
	yFocus = (0 == (xFocus % 3)) ? (yFocus + 1) : yFocus;
	xFocus = (xFocus % 3);
	m_FaultCheck[index].Create(GetRandWndID(),x,y,cx,cy,this,xFocus,yFocus);
	string outStr;
	outStr.clear();
	outStr = ChangeChinese(pstr);
	m_FaultCheck[index].SetCaption(outStr.c_str(),false,false);	
}

string CShanghaiOtherErrDialog::ChangeChinese(const char *pstr)
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

