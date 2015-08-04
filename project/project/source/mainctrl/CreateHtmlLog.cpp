#include "CreateHtmlLog.h"
#include "UsbBackupProc.h"
#include "LocalDevice.h"
#include "TimeCtrl.h"
#include "DateCtrl.h"
#include "LogMan.h"
#include "UserMan.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
CCreateHtmlLog *CCreateHtmlLog::m_pHtmlLog = NULL;
extern LANGUAGE	g_language;
CCreateHtmlLog::CCreateHtmlLog()
{
	
}

CCreateHtmlLog::~CCreateHtmlLog()
{
	
}

CCreateHtmlLog * CCreateHtmlLog::Instance()
{
	if (NULL == m_pHtmlLog)
	{
		m_pHtmlLog = new CCreateHtmlLog;
	}

	return m_pHtmlLog;
}



void CCreateHtmlLog::Initial(bool bcdbackup,BACKUP_INITIAL_INFO *pBackupInitialInfo,char desDataPath[])
{
	m_linkpath = NULL;
	m_aviNameVec.clear();
	m_bcdbackup = bcdbackup;
	m_backupStartTime = 0;
	m_backupEndTime = 0;
	m_allTimeOfFiles = 0;
	memset(m_mac,0,sizeof(m_mac));
	m_pBackupInitialInfo = NULL;
	memset(m_desDataPath,0,sizeof(m_desDataPath));
	memset(m_operdirname,0,sizeof(m_operdirname));
	memset(m_eventdirname,0,sizeof(m_eventdirname));

	//MAC
	char macstr[64]={0};
	CLocalDevice::GetMacFromFlash(macstr, 50);
	snprintf(m_mac, 64, "%s", macstr);

	m_pBackupInitialInfo=pBackupInitialInfo;
	snprintf(m_desDataPath, 128, "%s", desDataPath);


	for (unsigned long i = 0; i < m_pBackupInitialInfo->fileNum; ++i)
	{
		unsigned long startTimeTmp = m_pBackupInitialInfo->pFileInfoBackup[i].fileInfo.startTime;
		unsigned long endTimeTmp   = m_pBackupInitialInfo->pFileInfoBackup[i].fileInfo.endTime;
		if (!m_bcdbackup)
		{
			if (startTimeTmp < m_pBackupInitialInfo->startTime)
			{
				startTimeTmp = m_pBackupInitialInfo->startTime;
			}
			if (endTimeTmp > m_pBackupInitialInfo->endTime)
			{
				endTimeTmp = m_pBackupInitialInfo->endTime;
			}
		}

		if (endTimeTmp > startTimeTmp)
		{
			m_allTimeOfFiles+=(endTimeTmp - startTimeTmp);
		}
				

		if (0 == i)
		{
			m_backupStartTime = startTimeTmp;
			m_backupEndTime = endTimeTmp;
		}
		else
		{
			if (startTimeTmp < m_backupStartTime)
			{
				m_backupStartTime = startTimeTmp;
			}

			if (endTimeTmp > m_backupEndTime)
			{
				m_backupEndTime = endTimeTmp;
			}
		}
	}

	m_bCreateOperLog = false;
	m_bCreatEventLog = false;

	
	
}

bool CCreateHtmlLog::WirteHtmlLogAction()
{
	USER_INFO userInfo;
	memset(&userInfo, 0, sizeof(USER_INFO));
	memset(m_szUsrName, 0, sizeof(m_szUsrName));
	if(CUserMan::Instance()->GetUser(userInfo, LOCAL_CLIENT_ID))
	{
		snprintf(m_szUsrName, 36, "%s", userInfo.name);	
	}
	else
	{
		snprintf(m_szUsrName, 36, "admin");
	}

	m_bCreateOperLog = false;
	m_bCreatEventLog = false;
	m_bCreateOperLog = WriteOperationLoginfo();
	m_bCreatEventLog = WriteEventLoginfo();
	return true;
}

bool CCreateHtmlLog::CreateHtmlAction()
{	
	char logPath[128]={0};
	memset(logPath,0,sizeof(logPath));
	snprintf(logPath,128,"%s/%s",m_desDataPath,"log.html");
	bool bwrite=WriteHtmlLog(logPath, m_bCreateOperLog, m_bCreatEventLog);
	if(!bwrite)
	{
		printf("bwrite fail\n");
		return false;
	}
	return true;
}

void CCreateHtmlLog::Quit()
{
	m_bcdbackup=false;
	memset(m_mac,0,sizeof(m_mac));
	m_pBackupInitialInfo = NULL;
	memset(m_desDataPath,0,sizeof(m_desDataPath));
	memset(m_operdirname,0,sizeof(m_operdirname));
	memset(m_eventdirname,0,sizeof(m_eventdirname));
	m_aviNameVec.clear();
}



bool CCreateHtmlLog::ExportEventLog(EVENT_LOG *pLog,int LogNum)
{
	if((NULL != pLog) && (0 != LogNum))
	{
		//有日志数据，并且数据个数不为0的时候才允许生成xml文件
		char headTitle[132]={0};
		char *pElememtTitle[5]={NULL};
		pElememtTitle[0]=new char [132];
		pElememtTitle[1]=new char [132];
		pElememtTitle[2]=new char [132];
		pElememtTitle[3]=new char [132];
		pElememtTitle[4]=new char [132];

		snprintf(headTitle,        132, "%s", CStringTable::Instance()->String(E_STR_ID_TITLE_EVENT_LIST));
		snprintf(pElememtTitle[0], 132, "%s", CStringTable::Instance()->String(E_STR_ID_INDEX));
		snprintf(pElememtTitle[1], 132, "%s", CStringTable::Instance()->String(E_STR_ID_CAMERA_NAME));
		snprintf(pElememtTitle[2], 132, "%s", CStringTable::Instance()->String(E_STR_ID_TYPE));
		snprintf(pElememtTitle[3], 132, "%s", CStringTable::Instance()->String(E_STR_ID_START_TIME));
		snprintf(pElememtTitle[4], 132, "%s", CStringTable::Instance()->String(E_STR_ID_END_TIME));

		CExportHtmlLog exLog;
		exLog.SetWriteFileInfo(m_bcdbackup, reinterpret_cast<char *>(&headTitle), reinterpret_cast<char **>(&pElememtTitle), reinterpret_cast<char *>(pLog),  LogNum*sizeof(EVENT_LOG), 2,m_desDataPath);
		bool bex=exLog.ExportLog();
		if(!bex)
		{
			printf("event logout err\n");
		}
		else
		{
			exLog.SendDirname(m_eventdirname);
		}
		delete [] pElememtTitle[0];
		delete [] pElememtTitle[1];
		delete [] pElememtTitle[2];
		delete [] pElememtTitle[3];
		delete [] pElememtTitle[4];
		return bex;
	}
	else
	{
		return false;
	}

	return true;
}

bool CCreateHtmlLog::WriteOperationLoginfo()
{
	NET_LOG_SEARCH searchCond;
	searchCond.startTime=m_backupStartTime;
	searchCond.endTime=m_backupEndTime;
	unsigned long condition=0;
	condition = LOG_TYPE_SYSTEM_CTRL | LOG_TYPE_CONFIG | LOG_TYPE_PLAYBACK | LOG_TYPE_BACKUP | LOG_TYPE_SEARCH | LOG_TYPE_VIEW_INFO | LOG_TYPE_ERROR_INFO;
	searchCond.content=(condition >> 16);

	OPERATOR_LOG_LIST list;
	unsigned long num = CLogMan::Instance()->GetOperatorLog(searchCond.startTime, searchCond.endTime, searchCond.content, list);
	if (num <= 0)
	{
		return false;
	}

	OPERATOR_LOG *pLog = new OPERATOR_LOG[num];
	if (NULL == pLog)
	{
		list.RemoveAll();
		return false;
	}

	unsigned long i = 0;
	POS pos = list.GetTailPosition();
	while(NULL != pos)
	{
		OPERATOR_LOG & node = list.GetPrev(pos);
		pLog[i++] = node;
	}
	list.RemoveAll();


	bool bsucc=ExportOperationLog(pLog,num);
	if(pLog!=NULL)
	{
		delete [] pLog;
		pLog=NULL;
	}

	return bsucc;
}

bool CCreateHtmlLog::WriteEventLoginfo()
{
	NET_EVENT_SEARCH searchCond;
	memset(&searchCond, 0, sizeof(NET_EVENT_SEARCH));
	searchCond.type |= EVENT_TYPE_MOTION | EVENT_TYPE_SENSOR | EVENT_TYPE_V_LOSS ;
	searchCond.startTime = m_backupStartTime;
	searchCond.endTime = m_backupEndTime;
	searchCond.chnnBits = CH();

	EVENT_LOT_LIST list;
	unsigned long num = CLogMan::Instance()->GetEventLog(searchCond.startTime, searchCond.endTime, searchCond.type, searchCond.chnnBits, list);
	if (num <= 0)
	{
		return false;
	}

	EVENT_LOG *pEventLog = new EVENT_LOG [num];
	if (NULL == pEventLog)
	{
		list.RemoveAll();
		return false;
	}

	unsigned long i = 0;
	POS pos = list.GetTailPosition();
	while(NULL != pos)
	{
		EVENT_LOG & node = list.GetPrev(pos);
		pEventLog[i++] = node;
	}
	list.RemoveAll();

	bool bSuc = ExportEventLog(pEventLog,num);

	if (pEventLog != NULL)
	{
		delete [] pEventLog;
		pEventLog = NULL;
	}

	return bSuc;
}


bool CCreateHtmlLog::ExportOperationLog(OPERATOR_LOG *pLog,int LogNum)
{
	if((NULL != pLog) && (0 != LogNum))
	{
		//有日志数据，并且数据个数不为0的时候才允许生成xml文件
		char headTitle[132]={0};
		char *pElememtTitle[4]={NULL};
		pElememtTitle[0]=new char [132];
		pElememtTitle[1]=new char [132];
		pElememtTitle[2]=new char [132];
		pElememtTitle[3]=new char [132];

		snprintf(headTitle,        132, "%s", CStringTable::Instance()->String(E_STR_ID_TITLE_LOG_LIST));
		snprintf(pElememtTitle[0], 132, "%s", CStringTable::Instance()->String(E_STR_ID_TYPE));
		snprintf(pElememtTitle[1], 132, "%s", CStringTable::Instance()->String(E_STR_ID_USER_NAME));
		snprintf(pElememtTitle[2], 132, "%s", CStringTable::Instance()->String(E_STR_ID_TIME));
		snprintf(pElememtTitle[3], 132, "%s", CStringTable::Instance()->String(E_STR_ID_IP));

		CExportHtmlLog exLog;
		exLog.SetWriteFileInfo(m_bcdbackup,reinterpret_cast<char *>(&headTitle), reinterpret_cast<char **>(&pElememtTitle), reinterpret_cast<char *>(pLog),  LogNum*sizeof(OPERATOR_LOG), 0,m_desDataPath);
		bool bret = exLog.ExportLog();
		if (bret)
		{
			exLog.SendDirname(m_operdirname);
		}
		
		delete [] pElememtTitle[0];
		delete [] pElememtTitle[1];
		delete [] pElememtTitle[2];
		delete [] pElememtTitle[3];
		return bret;
	}
	else
	{
		return false;
	}

	return true;

}

bool CCreateHtmlLog::WriteHtmlLog(char *logname, bool bCreateOperLog, bool bCreateEventLog)
{
	if (m_pBackupInitialInfo->fileNum <= 0)
	{
		return false;
	}

	//从配置获取通道号对应的名称
	CMyList <NCFG_ITEM_ID> cfgitemList;
	cfgitemList.AddTail(NCFG_ITEM_CAMERA_NAME);
	unsigned char *pConfigData = NULL;
	unsigned long dataLen = 0;
	if (!CConfigEx::Instance()->GetConfigEx(cfgitemList, &pConfigData, dataLen))
	{
		printf("Get config data fail!\n");
		return false;
	}

	CConfigBlock cfgBlock;
	cfgBlock.CreateBlock(pConfigData, dataLen);

	unsigned char *pData = NULL;
	cfgBlock.GetConfig(NCFG_ITEM_CAMERA_NAME, &pData, dataLen);
	NCFG_INFO_SHORT_NAME *pname=reinterpret_cast<NCFG_INFO_SHORT_NAME*>(pData);

	//解析并写入html文件
	//创建一个XML的文档对象。
	TiXmlDocument *myDocument = new TiXmlDocument();
	//创建一个根元素并连接。
	TiXmlElement *RootElement = new TiXmlElement("HTML");
	myDocument->LinkEndChild(RootElement);

	
	//创建一个meta元素并连接。
	TiXmlElement *Pmeta = new TiXmlElement("meta");
	RootElement->LinkEndChild(Pmeta);
	//设置meta元素的属性。
	Pmeta->SetAttribute("http-equiv", "Content-Type");
	Pmeta->SetAttribute("content", "text/html");
	Pmeta->SetAttribute("charset", "utf-8");

	TiXmlElement *Pbody = new TiXmlElement("BODY");
	Pmeta->LinkEndChild(Pbody);
	Pbody->SetAttribute("bgcolor", "#FFFFFF");
	Pbody->SetAttribute("text", "#000000");
	Pbody->SetAttribute("link", "#5F0000");
	Pbody->SetAttribute("vlink", "#5F0000");
	Pbody->SetAttribute("alink", "#5F0000");

	//Table
	TiXmlElement *ptable = new TiXmlElement("TABLE");
	Pbody->LinkEndChild(ptable);
	ptable->SetAttribute("width", "850");
	ptable->SetAttribute("cellspacing", "2");
	ptable->SetAttribute("cellpadding", "4");
	ptable->SetAttribute("border", "0");
	ptable->SetAttribute("style", "color:#000000;font-size:10pt;height=20");
	
	char strarr[256]={0};

	//获取现在的时间
	unsigned long curtime=GetCurrTime32();
	std::string curTimeOfDay;
	std::string curTimeOfHour;	
	GUI::CTimeCtrl::GetTime32Text(curTimeOfDay, curtime, GUI::CDateCtrl::SECTION_YEAR|GUI::CDateCtrl::SECTION_MON|GUI::CDateCtrl::SECTION_DAY);
	GUI::CTimeCtrl::GetTime32Text(curTimeOfHour, curtime, GUI::CTimeCtrl::SECTION_HOUR|GUI::CTimeCtrl::SECTION_MIN|GUI::CTimeCtrl::SECTION_SEC);
   	printf("%s,##\n",curTimeOfDay.c_str());
	printf("%s,##\n",curTimeOfHour.c_str());

	//获取用户名
	
	//与中文一样的：芬兰  繁体中文
	//与英文一直的：土耳其语 匈牙利语 德语 葡萄牙语  泰语 保加利亚语 捷克语 荷兰 丹麦 印尼 日文 西班牙  波兰 希腊 意大利 法国
    //不确定的：   越南语， 俄文， 挪威，波斯，瑞典 立陶宛
	if ((LANGUAGE_CHINESE_S == g_language) || (LANGUAGE_CHINESE_B == g_language) || (LANGUAGE_FINNISH == g_language))
	{
		snprintf(strarr, 256, CStringTable::Instance()->String(E_STR_ID_BAK_GENERATED), m_szUsrName, curTimeOfDay.c_str(), curTimeOfHour.c_str());
	}
	else if ((LANGUAGE_HEBREW == g_language) || (LANGUAGE_PERSIAN == g_language))
	{
		snprintf(strarr, 256, "%s;%s;%s %s", m_szUsrName, curTimeOfDay.c_str(), curTimeOfHour.c_str(),CStringTable::Instance()->String(E_STR_ID_BAK_GENERATED));
	}
	else
	{
		snprintf(strarr, 256, CStringTable::Instance()->String(E_STR_ID_BAK_GENERATED), curTimeOfDay.c_str(), curTimeOfHour.c_str(),m_szUsrName);
	}

	CreateNode(ptable,const_cast<char *>("Generated"),strarr,const_cast<char *>("center"),true,false,NULL,const_cast<char *>("background-color:#FF0000"));
	//文件名
	//Filename
	memset(strarr,0,sizeof(strarr));
	int fileNum=m_pBackupInitialInfo->fileNum;
	char *pfind=NULL;
	char tmpsave[128];
	if(m_pBackupInitialInfo->dataType == NET_BACKUP_DATA_TYPE_AVI)
	{
		int size=m_aviNameVec.size();
		vector<std::string>::iterator it=m_aviNameVec.begin();
		if(1 == size)
		{
			memset(tmpsave,0,sizeof(tmpsave));
			snprintf(tmpsave,128,"%s",it->c_str());
			pfind=strstr(tmpsave,"CH");
			snprintf(strarr,256,"%s",pfind);
			CreateNode(ptable,CStringTable::Instance()->String(E_STR_ID_BAK_FILENAME),strarr,const_cast<char *>("center"));
		}
		else if (size > 1)
		{
			string strFileName;
			strFileName.clear();
			for(;it!=m_aviNameVec.end();it++)
			{
				pfind=NULL;
				memset(tmpsave,0,sizeof(tmpsave));
				memset(strarr,0,sizeof(strarr));
				if(it != m_aviNameVec.end()-1)//容器的最后一个元素
				{
					snprintf(tmpsave,128,"%s,",it->c_str());
				}
				else
				{
					snprintf(tmpsave,128,"%s",it->c_str());
				}
				pfind=strstr(tmpsave,"CH");
				snprintf(strarr,256,"%s",pfind);
				strFileName+=strarr;
			}
			CreateNode(ptable,CStringTable::Instance()->String(E_STR_ID_BAK_FILENAME),strFileName.c_str(),const_cast<char *>("left"));
		}
	}
	else
	{
		
		if(1==fileNum)
		{
			snprintf(strarr,256,"00000000.dat");
		}
		else if(fileNum>1)
		{
			char szname[20]={0};
			GetFileName(szname,fileNum-1);
			snprintf(strarr,256,"00000000.dat-%s",szname);
		}
		CreateNode(ptable,CStringTable::Instance()->String(E_STR_ID_BAK_FILENAME),strarr,const_cast<char *>("center"));
	}
	pfind=NULL;
	
	//record type
	//获取dvr的硬件版本号
	memset(strarr,0,sizeof(strarr));
	strcpy(strarr, g_p_login_succ->productInfo.hardwareVersion);
	CreateNode(ptable,CStringTable::Instance()->String(E_STR_ID_BAK_RECORDER),strarr,const_cast<char *>("center"),false,false,NULL,const_cast<char *>("background-color:#DFDFDF"));
	//mac
	CreateNode(ptable,CStringTable::Instance()->String(E_STR_ID_BAK_MAC),m_mac,const_cast<char *>("center"));
	//cam no
	unsigned long chnn=0;
	memset(strarr,0,sizeof(strarr));
	set<unsigned long> sort_set;
	for(int i=0;i<fileNum;i++)
	{
		chnn=m_pBackupInitialInfo->pFileInfoBackup[i].fileInfo.chnn;
		sort_set.insert(chnn);
	}
	set<unsigned long>::reverse_iterator  revit=sort_set.rbegin();
	for(set<unsigned long>::iterator it=sort_set.begin();it!=sort_set.end();it++)
	{
		if(*it == *revit)//容器的最后一个项目
		{
			sprintf(strarr+strlen(strarr),"%d ",*it+1);
		}
		else
		{
			sprintf(strarr+strlen(strarr),"%d, ",*it+1);
		}
	}
	CreateNode(ptable,CStringTable::Instance()->String(E_STR_ID_BAK_CHID),strarr,const_cast<char *>("center"),false,false,NULL,const_cast<char *>("background-color:#DFDFDF"));

	//读取通道名称
	chnn=0;
	string linkstring;
	revit=sort_set.rbegin();
	for(set<unsigned long>::iterator it=sort_set.begin();it!=sort_set.end();it++)
	{
		memset(strarr,0,sizeof(strarr));
		chnn=*it;
		if(*it == *revit)//容器的最后一个项目
		{
			snprintf(strarr,256,"%s ",pname[chnn].name);
		}
		else
		{
			snprintf(strarr,256,"%s, ",pname[chnn].name);
		}
		linkstring+=strarr;
	}
	sort_set.clear();
	//销毁block
	cfgBlock.DestroyBlock();
	pConfigData=NULL;
	CreateNode(ptable,CStringTable::Instance()->String(E_STR_ID_BAK_CHNAME),linkstring.c_str(),const_cast<char *>("center"));

	//备份日志的开始时间
	std::string strStartTime;
	GUI::CTimeCtrl::GetTime32Text(strStartTime, m_backupStartTime);
	memset(strarr,0,sizeof(strarr));
	snprintf(strarr,256,"%s",strStartTime.c_str());
	CreateNode(ptable,CStringTable::Instance()->String(E_STR_ID_BAK_START),strarr,const_cast<char *>("center"),false,false,NULL,const_cast<char *>("background-color:#DFDFDF"));

	//备份日志的结束时间
	std::string strEndTime;
	GUI::CTimeCtrl::GetTime32Text(strEndTime, m_backupEndTime);
	memset(strarr,0,sizeof(strarr));
	snprintf(strarr,256,"%s", strEndTime.c_str());
	CreateNode(ptable,CStringTable::Instance()->String(E_STR_ID_BAK_END),strarr,const_cast<char *>("center"));
	
	//Duration	
	memset(strarr,0,sizeof(strarr));
	snprintf(strarr,256,"%d(seconds)",m_allTimeOfFiles);
	CreateNode(ptable,CStringTable::Instance()->String(E_STR_ID_BAK_TIMELENG),strarr,const_cast<char *>("center"),false,false,NULL,const_cast<char *>("background-color:#DFDFDF"));
	
	
	float filesize=0;	
	if (NET_BACKUP_DATA_TYPE_DVR == m_pBackupInitialInfo->dataType)
	{
		char filename[256]={0};
		for(int i=0;i<fileNum;i++)
		{
			memset(strarr,0,sizeof(strarr));
			GetFileName(filename,i);
			if(!m_bcdbackup)
			{
				snprintf(strarr,256,"%s/%s",m_desDataPath,filename);
			}
			else
			{
				snprintf(strarr, 256,"%s/%02d/%s",m_pBackupInitialInfo->pSrcFilePath, m_pBackupInitialInfo->pFileInfoBackup[i].fileInfo.diskIndex,filename);
			}
			
			filesize+=get_file_size(strarr);
		}
	}
	else if (NET_BACKUP_DATA_TYPE_AVI == m_pBackupInitialInfo->dataType)
	{
		for(vector<std::string>::iterator it=m_aviNameVec.begin();it!=m_aviNameVec.end();it++)
		{
			filesize+=get_file_size(it->c_str());
		}
	}
	else
	{
		filesize = 0;
	}
	
	memset(strarr,0,sizeof(strarr));
	snprintf(strarr,256,"%.3f MB",filesize/(1024*1024));
	CreateNode(ptable,CStringTable::Instance()->String(E_STR_ID_BAK_SIZE),strarr,const_cast<char *>("center"));

	//获取操作和事件日志的目录名
	char tmpdir[256]={0};
	pfind=NULL;
	if (bCreateOperLog)
	{
		memset(tmpdir,0,sizeof(tmpdir));
		if(!m_bcdbackup)
		{
			pfind=strstr(m_operdirname,USB_BACKUP_MOUNT);
			if(pfind!=NULL)
			{
				snprintf(tmpdir,256,"../%s/loglist.html",pfind+strlen(USB_BACKUP_MOUNT)+1);
			}
		}
		else
		{
			pfind=strstr(m_operdirname,"HtmlData");
			if(pfind!=NULL)
			{
				snprintf(tmpdir,256,"./%s/loglist.html",pfind+strlen("HtmlData")+1);
			}
			
		}
	/*	printf("operatorLog tmpdir=%s,m_operdirname=%s\n",tmpdir,m_operdirname);*/
		CreateNode(ptable,"Operation Log",CStringTable::Instance()->String(E_STR_ID_BAK_OPERLOG),const_cast<char *>("center"),true,true,tmpdir,const_cast<char *>("background-color:#DFDFDF"));
	}
	
	if (bCreateEventLog)
	{
		memset(tmpdir,0,sizeof(tmpdir));
		pfind=NULL;
		if(!m_bcdbackup)
		{
			pfind=strstr(m_eventdirname,USB_BACKUP_MOUNT);
			if(pfind!=NULL)
			{
				memset(tmpdir,0,sizeof(tmpdir));
				snprintf(tmpdir,256,"../%s/backupeventlist.html",pfind+strlen(USB_BACKUP_MOUNT)+1);
			}
		}
		else
		{			
			pfind=strstr(m_eventdirname,"HtmlData");
			if(pfind!=NULL)
			{
				snprintf(tmpdir,256,"./%s/backupeventlist.html",pfind+strlen("HtmlData")+1);
			}
		}
		//printf("eventlog tmpdir=%s,m_eventdirname=%s\n",tmpdir,m_eventdirname);
		CreateNode(ptable,const_cast<char *>("Event Log"),CStringTable::Instance()->String(E_STR_ID_BAK_EVENTLOG),const_cast<char *>("center"),true,true,tmpdir,const_cast<char *>("background-color:#DFDFDF"));
	}
	
		
	bool bsave=myDocument->SaveFile(logname);
	delete myDocument;
	if(!bsave)
	{
		printf("save err\n");
		return false;
	}
	if(m_bcdbackup)
	{
		if(m_linkpath!=NULL)
		{
			if (bCreateOperLog)
			{
				printf("%s,%d,m_operdirname:%s,m_linkpath:%s\n",__FILE__,__LINE__,m_operdirname,m_linkpath);
				LinkHtmlFile(m_operdirname,m_linkpath);
			}
			if (bCreateEventLog)
			{
				printf("%s,%d,m_eventdirname:%s,m_linkpath:%s\n",__FILE__,__LINE__,m_eventdirname,m_linkpath);
				LinkHtmlFile(m_eventdirname,m_linkpath);
			}
			printf("%s,%d,logname:%s,m_linkpath:%s\n",__FILE__,__LINE__,logname,m_linkpath);
			LinkHtmlFile(logname,m_linkpath);
		}
		else
		{
			assert(false);
			printf("LInk html err\n");
			return false;
		}
		m_linkpath=NULL;
	}
	
	return true;
}

void CCreateHtmlLog::CreateNode(TiXmlElement *ptable,const char *ElementName,const char *ElementResult,char *alignType,bool bcolspan,bool blink,char *path,char *color)
{
	TiXmlElement *ptr = new TiXmlElement("TR");
	ptable->LinkEndChild(ptr);
	if(color!=NULL)
	{
		ptr->SetAttribute("style",color);
	}

	TiXmlElement *ptd1 = new TiXmlElement("TD");
	ptr->LinkEndChild(ptd1);

	if(!bcolspan)
	{	
		ptd1->SetAttribute("width","200");
		ptd1->SetAttribute("align","center");
		ptd1->SetAttribute("height","50");

		TiXmlText *ptdContent1 = new TiXmlText(ElementName);
		ptd1->LinkEndChild(ptdContent1);
		TiXmlElement *ptdResult = new TiXmlElement("TD");
		ptr->LinkEndChild(ptdResult);
		ptdResult->SetAttribute("align",alignType);
		ptdResult->SetAttribute("height","30");
		TiXmlText *ptdResultContent = new TiXmlText(ElementResult);
		ptdResult->LinkEndChild(ptdResultContent);
	}
	else
	{
		ptd1->SetAttribute("align","center");
		ptd1->SetAttribute("colspan","2");
		ptd1->SetAttribute("height","50");
		if(blink)
		{
			TiXmlElement *pOperation = new TiXmlElement("A");
			pOperation->SetAttribute("href",path);
			ptd1->LinkEndChild(pOperation);
			TiXmlText *ptdResultContent = new TiXmlText(ElementResult);
			pOperation->LinkEndChild(ptdResultContent);
		}
		else
		{
			TiXmlText *ptdResultContent = new TiXmlText(ElementResult);
			ptd1->LinkEndChild(ptdResultContent);
		}

	}
}

void CCreateHtmlLog::GetFileName(char *filename,int fileNum)
{
	char sz[20]={0};
	int tmp=fileNum;
	for(int j=0;j<8;j++)
	{
		sz[7-j]='0'+tmp%10;
		tmp=tmp/10;
	}
	strcpy(&sz[8],".dat");
	strcpy(filename,sz);
}

unsigned long CCreateHtmlLog::get_file_size(const char *filename)
{
#ifndef __ENVIRONMENT_WIN32__
	struct stat buf;
	if(stat(filename, &buf)<0)
	{
		return 0;
	}
	return (unsigned long)buf.st_size;  
#else
	FILE *pFile = fopen(filename, "rb");
	if(NULL == pFile)
	{
		return 0;
	}

	fseek(pFile, 0, SEEK_END);
	unsigned long len = ftell(pFile);
	fclose(pFile);

	return len;
#endif
}


vector<std::string> * CCreateHtmlLog::GetAllAviFileName()
{
	return &m_aviNameVec;
}
//
//bool CCreateHtmlLog::OnSearch()
//{
//	/*MESSAGE_DATA msgData;
//	NET_DATA_SEARCH searchCond;
//	memset(&searchCond, 0, sizeof(searchCond));
//	searchCond.searchType = DATA_SEARCH_TYPE_EVENT;
//	searchCond.videoCH = CH();
//	searchCond.startTime = m_backupStartTime;
//	searchCond.endTime =  m_backupEndTime;
//	searchCond.RecordTypeMASK = RECORD_TYPE_MANUAL | RECORD_TYPE_SCHEDULE | RECORD_TYPE_MOTION | RECORD_TYPE_SENSOR;
//	searchCond.dataOwnerby = m_pBackupInitialInfo->diskOwnerby;
//
//	memset(&msgData, 0, sizeof(MESSAGE_DATA));
//	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&searchCond, sizeof(NET_DATA_SEARCH), msgData);
//	RECLOG_INFO_EX *pDate = (RECLOG_INFO_EX *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
//	int num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(RECLOG_INFO_EX);
//	if (0 == num)
//	{
//		CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
//		return false;
//	}
//
//	for (int count = 0; count < num; ++count)
//	{
//		m_dataVect.push_back(pDate[count]);
//	}
//
//	CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
//	return true;*/
//
//}

ULONGLONG CCreateHtmlLog::CH()
{
	ULONGLONG chnnBits = 0;
	if(m_pBackupInitialInfo!=NULL)
	{
		unsigned long count = m_pBackupInitialInfo->fileNum;		
		for(int j=0;j<count;j++)
		{
			chnnBits |= (0x01 << (m_pBackupInitialInfo->pFileInfoBackup[j].fileInfo.chnn));			
		}		
	}
	else
	{
		assert(false);
		printf("err\n");
	}
	return chnnBits;
}

int CCreateHtmlLog::LinkHtmlFile(char *pSrcFilePath,char *pDesPath)
{
	char cmd[256]={0};
	memset(cmd,0,sizeof(cmd));
	snprintf(cmd,256,"ln -s %s %s",pSrcFilePath,pDesPath);
	//snprintf(cmd,256,"cp -rf %s %s",pSrcFilePath,pDesPath);
	printf("%s,%d,cmd:%s\n",__FILE__,__LINE__,cmd);
	if (0 != DVRSystem(cmd))
	{
		return -1;
	}
	return 0;
}

void CCreateHtmlLog::SetLinkPath(char *plinkPath)
{
	m_linkpath=plinkPath;
}

//end 
