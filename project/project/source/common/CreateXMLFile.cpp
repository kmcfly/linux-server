#include "CreateXMLFile.h"
#include "StringTable.h"
#include "IDToStringID.h"

#ifdef __ENVIRONMENT_LINUX__
#include "TimeCtrl.h"
#endif


#ifdef __ENVIRONMENT_WIN32__
	#include <fcntl.h>
	#include <direct.h>
#else
	#include <sys/types.h>
	#include <dirent.h>
#endif

CCreateXMLFile::CCreateXMLFile()
{
	m_xmlFileFd=-1;
	m_xmlFileNameFd=-1;
	m_pfnIdToString = NULL;
	m_pCallbackParam = NULL;

	m_pButtonTipInfo = NULL;
	m_tipCount = 0;

	memset(m_xmlDirName, 0, sizeof(m_xmlDirName));
	m_bUseCDBackup = false;
}
CCreateXMLFile::~CCreateXMLFile()
{
	if( -1 != m_xmlFileFd )
	{
		close( m_xmlFileFd );
		m_xmlFileFd = -1;
	}


	if( -1 != m_xmlFileNameFd )
	{
		close( m_xmlFileNameFd );
		m_xmlFileNameFd = -1;
	}

	if ( NULL != m_pButtonTipInfo)
	{
		delete [] m_pButtonTipInfo;
		m_pButtonTipInfo = NULL;
	}

	m_tipCount = 0;

	m_bUseCDBackup = false;
}

void CCreateXMLFile::SetDescriptionString(ID_TO_STRING pfnIdToString, void *pParam)
{
	assert(pfnIdToString != NULL);
	m_pfnIdToString = pfnIdToString;
	m_pCallbackParam = pParam;
}

void CCreateXMLFile::SetButtonTip(BUTTON_TIP_INFO *pButtonTipInfo, int tipCount)
{
	if (( NULL != pButtonTipInfo) && (0 < tipCount))
	{
		m_pButtonTipInfo = new BUTTON_TIP_INFO [tipCount];
		if ( NULL != m_pButtonTipInfo)
		{
			memcpy(m_pButtonTipInfo, pButtonTipInfo, sizeof(BUTTON_TIP_INFO) * tipCount );
			m_tipCount = tipCount;
		} 
		else
		{
			printf("%s:%s:%d,设置按钮上的提示字符错误\n", __FUNCTION__, __FILE__, __LINE__);
			assert(false);
		}
	} 
	else
	{
		assert(false);
		printf("%s:%s:%d,设置按钮上的提示字符错误\n", __FUNCTION__, __FILE__, __LINE__);
	}

}
void CCreateXMLFile::SetExportDir(const char *pExportDir )
{
	if(NULL==pExportDir)
	{
		memset(m_xmlDirName, 0, sizeof(m_xmlDirName));
	}
	else
	{
		snprintf(m_xmlDirName,sizeof(m_xmlDirName),"%s", pExportDir);
	}

	m_bUseCDBackup = false;
}

void CCreateXMLFile::SetUseCDBackup(bool bUseCDBackup)
{
	m_bUseCDBackup = bUseCDBackup;
}

/***********************************************************************
** Function     : CreateXMLFile()
** Description  : 
					1：根据format创建xml数据文件并写数据到该文件
					2：产生xmlFileName.xml文件，用来存放xml数据文件的文件名。在用JS解析的时候先从该文件中读取XML数据文件名然后解析数据
					3：loglist的XML数据文件的格式如下所示， eventList的数据格式和loglist的一样
					<?xml version="1.0" encoding="gb2312" ?>			xml文件头
					<Information>
						
						<AttributeWidth>
							<TableWidth>400</TableWidth>				html表格第一列的宽度
							<TableWidth>200</TableWidth>				html表格第二列的宽度
							<TableWidth>200</TableWidth>				html表格第三列的宽度
							<TableWidth>200</TableWidth>				html表格第四列的宽度
						</AttributeWidth>

						<HeadElementTitleInfor>							
							<HeadTitle>日志列表</HeadTitle>				html文件标题
							<ElementNum>4</ElementNum>					html表格的列数
							<ElementTitle>类型</ElementTitle>			html表格第一列的名字
							<ElementTitle>用户名</ElementTitle>			html表格第二列的名字
							<ElementTitle>时间</ElementTitle>			html表格第三列的名字
							<ElementTitle>静态网络地址</ElementTitle>	html表格第四列的名字
						</HeadElementTitleInfor>

						<Element>										数据
							<Section>查看事件信息</Section> 
							<Section>admin</Section> 
							<Section>04/21/2010 10:59:04</Section> 
							<Section>127.000.000.001</Section> 
						</Element>
						………………………………
						………………………………
						………………………………
						<Element>
							<Section>查看日志记录</Section> 
							<Section>admin</Section> 
							<Section>04/21/2010 10:58:35</Section> 
							<Section>127.000.000.001</Section> 
						</Element>
					</Information>


** Input		:
					1：pHeadTitle html文件标题
					2：ppElementTitle html表格的列标题
					3：pdata xml数据
					4：datalen pdata的长度 字节个数
					5：format 要生成何种xml文件
					
** Output		:
					返回true表示生成xml数据文件成功
					返回false表示生成xml数据文件失败
** Modify Record:
1:
***********************************************************************/

bool CCreateXMLFile::CreateXMLFile(const char *pHeadTitle, const char **ppElementTitle, const char *pData, int dataLen, XML_FORMAT format)
{
	if( NULL == pData )
	{
		assert(false);
		return false;
	}

	m_xmlFormat = format;
	snprintf(m_xmlHeadTitle, sizeof(m_xmlHeadTitle), "%s", pHeadTitle);


	if(!InitialXMLFile(format))
	{
		assert(false);
		return false;
	}

	struct tm tmTime = DVRTime32ToTm(GetCurrTime32());
	memset(m_xmlFile, 0, sizeof(m_xmlFile));
	if (m_bUseCDBackup)
	{
		switch(format)
		{
		case XML_FORMAT_LOG:
			snprintf(m_xmlFile, 512, "%s","LogList");
			break;
		case XML_FORMAT_EVENT:
			snprintf(m_xmlFile, 512, "%s","EventList");
			break;
		case XML_BACKUP_EVENT:
			snprintf(m_xmlFile, 512, "%s", "BkEventList");
			break;
		default:
			assert(false);
			return false;
		}
	}
	else
	{
		switch(format)
		{
		case XML_FORMAT_LOG:
			snprintf(m_xmlFile, 512, "%04d%02d%02d%02d%02d%02d%s",tmTime.tm_year+1900, tmTime.tm_mon + 1, tmTime.tm_mday, tmTime.tm_hour,tmTime.tm_min,tmTime.tm_sec,"LogList");
			break;
		case XML_FORMAT_EVENT:
			snprintf(m_xmlFile, 512, "%04d%02d%02d%02d%02d%02d%s",tmTime.tm_year+1900, tmTime.tm_mon + 1, tmTime.tm_mday, tmTime.tm_hour,tmTime.tm_min,tmTime.tm_sec,"EventList");
			break;
		case XML_BACKUP_EVENT:
			snprintf(m_xmlFile, 512, "%04d%02d%02d%02d%02d%02d%s",tmTime.tm_year+1900, tmTime.tm_mon + 1, tmTime.tm_mday, tmTime.tm_hour,tmTime.tm_min,tmTime.tm_sec,"BackupEventList");
			break;
		default:
			assert(false);
			return false;
		}
	}	

	
	if( !WriteStartMark("FileName", true, m_xmlFileNameFd) )
	{
		close(m_xmlFileNameFd);
		m_xmlFileFd = -1;
		assert(false);
		return false;
	}

	bool bRetVal = true;
	if(XML_FORMAT_LOG==format)
	{
		const OPERATOR_LOG *pInfor = reinterpret_cast<const OPERATOR_LOG *>(pData);
		int elementCount = (dataLen / sizeof(OPERATOR_LOG));							//日志个数

		int lastFileElementCount = elementCount%100;									//最后一个日志文件中日志的个数
		int fileCount = elementCount / 100;												//生成的日志文件数, 每个日志文件中100个日志信息
		if(lastFileElementCount)
		{
			//日志列表不是正100的情况下文件个数加1
			fileCount++;
		}

		for(int i=0; i<fileCount; i++)
		{
			//写文件名到xmlFileName.xml
			char newXMLFileName[132]={0};
			snprintf(newXMLFileName, sizeof(newXMLFileName),  "%s%d.xml",m_xmlFile, i);
			if(!AddItem("xmlFileName", newXMLFileName, m_xmlFileNameFd))
			{
				assert(false);
				bRetVal=false;
				break;
			}
			

			//产生新的loglist.xml文件
			char filePath[264]={0};
			snprintf(filePath,sizeof(filePath), "%s/%s", m_xmlFilesPath, newXMLFileName);

#if defined(__X_LINUX__)
			m_xmlFileFd = open(filePath,O_RDWR);
#else
			m_xmlFileFd = open(filePath,O_RDWR|O_CREAT);
#endif
			chmod( filePath,666 );
			if( -1== m_xmlFileFd )
			{
				perror("open()");
				assert(false);
				bRetVal=false;
				break;
			}

			int count = write( m_xmlFileFd, _XMLdeclare_, strlen(_XMLdeclare_));
			if( count != strlen(_XMLdeclare_))
			{
				perror("write()");
				close(m_xmlFileFd);
				m_xmlFileFd = -1;
				bRetVal=false;
				assert(false);
				break;
			}

			//写loglist.xml文件
			WriteStartMark("Information", true);
			if (!AddButtonTip(m_pButtonTipInfo, m_tipCount))
			{
				assert(false);
				close(m_xmlFileFd);
				m_xmlFileFd = -1;
				bRetVal=false;
				break;
			}

			if(!AddLogTitleInfo(pHeadTitle,ppElementTitle))
			{
				assert(false);
				close(m_xmlFileFd);
				m_xmlFileFd = -1;
				bRetVal=false;
				break;
			}

			int elCount=100;
			if(i==(fileCount-1))			//写到最后一个文件。最后一个文件的个数小于或等于100
			{
				elCount=lastFileElementCount;
			}
			for(int j=0; j<elCount; j++,pInfor++)
			{
				if(!AddLogElement(pInfor))
				{
					printf("%s:%s:%d,AddElementLog() error\n", __FUNCTION__, __FILE__, __LINE__);
					assert(false);
					close(m_xmlFileFd);
					m_xmlFileFd = -1;
					bRetVal = false;
					break;
				}
			}

			WriteEndMark("Information", true);
			if( !FileFlush( m_xmlFileFd, "rw") )
			{
				assert(false);
				printf("%s:%s:%d, FileFlush() error\n", __FUNCTION__, __FILE__, __LINE__);
				bRetVal = false;
				break;
			}
			close(m_xmlFileFd);
			
			m_xmlFileFd=-1;
		}
	}
	else if(XML_FORMAT_EVENT== format)
	{
		const EVENT_LOG *pInfor = reinterpret_cast<const EVENT_LOG *>(pData);
		int elementCount = (dataLen / sizeof(EVENT_LOG));
		int fileCount = elementCount / 100;
		int lastFileElementCount = elementCount % 100;
		if( lastFileElementCount )
		{
			fileCount++;
		}

		for(int i=0; i<fileCount; i++)
		{
			//写文件名到xmlFileName.xml
			char newXMLFileName[132]={0};
			snprintf(newXMLFileName, sizeof(newXMLFileName), "%s%d.xml", m_xmlFile, i);
			if(!AddItem("xmlFileName", newXMLFileName, m_xmlFileNameFd))
			{
				assert(false);
				bRetVal = false;
				break;
			}


			//产生新的eventlist.xml文件
			char filePath[256]={0};
			snprintf(filePath, sizeof(filePath), "%s/%s", m_xmlFilesPath, newXMLFileName);

#if defined(__X_LINUX__)
			m_xmlFileFd = open(filePath,O_RDWR);
#else
			m_xmlFileFd = open(filePath,O_RDWR|O_CREAT);
#endif
			chmod( filePath,666 );
			if( -1== m_xmlFileFd )
			{
				perror("open()");
				bRetVal=false;
				assert(false);
				break;
			}

			int count = write( m_xmlFileFd, _XMLdeclare_, strlen(_XMLdeclare_));
			if( count != strlen(_XMLdeclare_))
			{
				perror("write()");
				close(m_xmlFileFd);
				m_xmlFileFd = -1;
				assert(false);
				bRetVal=false;
				break;
			}

			//写eventlist.xml文件
			WriteStartMark("Information", true);

			if (!AddButtonTip(m_pButtonTipInfo, m_tipCount))
			{
				assert(false);
				close(m_xmlFileFd);
				m_xmlFileFd = -1;
				bRetVal=false;
				break;
			}

			if(!AddEventTitleInfo(pHeadTitle,ppElementTitle))
			{
				close(m_xmlFileFd);
				m_xmlFileFd = -1;
				assert(false);
				bRetVal=false;
				break;
			}

			int elCount=100;
			if(i==(fileCount-1))
			{
				elCount=lastFileElementCount;
			}
			for(int j=0; j<elCount; j++,pInfor++)
			{
				if(!AddEventElement(pInfor))
				{
					printf("%s:%s:%d,AddElementLog() error\n", __FUNCTION__, __FILE__, __LINE__);
			
					close(m_xmlFileFd);
					m_xmlFileFd = -1;
					bRetVal = false;
					assert(false);
					break;
				}
			}

			WriteEndMark("Information", true);

			if( !FileFlush( m_xmlFileFd, "rw") )
			{
				assert(false);
				printf("%s:%s:%d, FileFlush() error\n", __FUNCTION__, __FILE__, __LINE__);
				bRetVal = false;
				break;
			}
			close(m_xmlFileFd);
			m_xmlFileFd=-1;
		}

	}
	else if(XML_BACKUP_EVENT== format)
	{
		const EVENT_LOG *pInfor = reinterpret_cast<const EVENT_LOG *>(pData);
		int elementCount = (dataLen / sizeof(EVENT_LOG));
		int fileCount = elementCount / 100;
		int lastFileElementCount = elementCount % 100;
		if( lastFileElementCount )
		{
			fileCount++;
		}

		for(int i=0; i<fileCount; i++)
		{
			//写文件名到xmlFileName.xml
			char newXMLFileName[132]={0};
			snprintf(newXMLFileName, 132, "%s%d.xml", m_xmlFile, i);
			if(!AddItem("xmlFileName", newXMLFileName, m_xmlFileNameFd))
			{
				assert(false);
				bRetVal = false;
				break;
			}


			//产生新的backupeventlist.xml文件
			char filePath[256]={0};
			snprintf(filePath, 256, "%s/%s", m_xmlFilesPath, newXMLFileName);

#if defined(__X_LINUX__)
			m_xmlFileFd = open(filePath,O_RDWR);
#else
			m_xmlFileFd = open(filePath,O_RDWR|O_CREAT);
#endif
			chmod( filePath,666 );
			if( -1== m_xmlFileFd )
			{
				printf("%s,%d, open() error\n",__FILE__,__LINE__);
				bRetVal=false;
				assert(false);
				break;
			}

			int count = write( m_xmlFileFd, _XMLdeclare_, strlen(_XMLdeclare_));
			if( count != strlen(_XMLdeclare_))
			{
				printf("%s,%d, write() error\n",__FILE__,__LINE__);				
				close(m_xmlFileFd);
				m_xmlFileFd = -1;
				assert(false);
				bRetVal=false;
				break;
			}

			//写eventlist.xml文件
			WriteStartMark("Information", true);

			if (!AddButtonTip(m_pButtonTipInfo, m_tipCount))
			{
				assert(false);
				close(m_xmlFileFd);
				m_xmlFileFd = -1;
				bRetVal=false;
				break;
			}

			if(!AddBackupEventTitleInfo(pHeadTitle,ppElementTitle,5))
			{
				close(m_xmlFileFd);
				m_xmlFileFd = -1;
				assert(false);
				bRetVal=false;
				break;
			}

			int elCount=100;
			if(i==(fileCount-1))
			{
				elCount=lastFileElementCount;
			}
			for(int j=0; j<elCount; j++,pInfor++)
			{
				if(!AddBackupEventElement(pInfor,j))
				{
					printf("%s:%s:%d,AddElementLog() error\n", __FUNCTION__, __FILE__, __LINE__);

					close(m_xmlFileFd);
					m_xmlFileFd = -1;
					bRetVal = false;
					assert(false);
					break;
				}
			}

			WriteEndMark("Information", true);

			if( !FileFlush( m_xmlFileFd, "rw") )
			{
				assert(false);
				printf("%s:%s:%d, FileFlush() error\n", __FUNCTION__, __FILE__, __LINE__);
				bRetVal = false;
				break;
			}
			close(m_xmlFileFd);
			m_xmlFileFd=-1;
		}
	}

	WriteEndMark("FileName", true, m_xmlFileNameFd);
	if( !FileFlush( m_xmlFileNameFd, "rw") )
	{
		assert(false);
		printf("%s:%s:%d, FileFlush() error\n", __FUNCTION__, __FILE__, __LINE__);
		bRetVal = false;
	}
	close(m_xmlFileNameFd);
	m_xmlFileNameFd=-1;

	return bRetVal;
}


//如果创建XML文件成功就拷贝html文件到相关目录
//如果创建文件失败要删除残留文件
bool CCreateXMLFile::CopyHtmlFile(bool bIsCreateXMLFileOK)
{
	if(bIsCreateXMLFileOK)
	{
		FILE *pHtmlfp=NULL;
		if(m_xmlFormat != XML_BACKUP_EVENT)
		{
			pHtmlfp=fopen(_html_file_, "rb");
		}
		else
		{
			pHtmlfp=fopen(_backup_event_html_file_, "rb");
		}
		

		if(NULL==pHtmlfp)
		{
			assert(false);
			perror("fopen()");
			return false;
		}

		fseek(pHtmlfp,0,SEEK_END);

		long fileLen=ftell(pHtmlfp);
		fseek(pHtmlfp, 0, SEEK_SET);
		if(0>=fileLen)
		{
			assert(false);
			fclose(pHtmlfp);
			pHtmlfp=NULL;
			return false;
		}

		char *pFileContent=new char [fileLen+1];
		assert(NULL !=pFileContent);
		
		if(1!=fread(pFileContent,fileLen,1,pHtmlfp))
		{
			perror("fread()");
			assert(false);
			fclose(pHtmlfp);
			pHtmlfp=NULL;
			delete [] pFileContent;
			pFileContent = NULL;
			return false;
		}
		fclose(pHtmlfp);
		pHtmlfp=NULL;

		pFileContent[fileLen]='\0';

		char destFilePath[264]={0};
		switch(m_xmlFormat)
		{
		case XML_FORMAT_LOG:
			snprintf(destFilePath,sizeof(destFilePath),"%s/%s.html",m_xmlDirName,"loglist");
			break;
		case XML_FORMAT_EVENT:
			snprintf(destFilePath,sizeof(destFilePath),"%s/%s.html",m_xmlDirName,"eventlist");
			break;
		case XML_BACKUP_EVENT:
			snprintf(destFilePath,sizeof(destFilePath),"%s/%s.html",m_xmlDirName,"backupeventlist");
			break;
		default:
			assert(false);
			if( NULL != pFileContent )
			{
				delete [] pFileContent;
				pFileContent=NULL;
			}
			return false;
			break;
		}
		
		FILE *pDestFp=fopen(destFilePath,"w+b");
		if(NULL==pDestFp)
		{
			perror("fopen()");
			assert(false);
			if( NULL != pFileContent )
			{
				delete [] pFileContent;
				pFileContent=NULL;
			}
			return false;
		}

		if( fileLen != fprintf(pDestFp, "%s",pFileContent))
		{
			perror("fprintf()");
			assert(false);
			if( NULL != pFileContent )
			{
				delete [] pFileContent;
				pFileContent=NULL;
			}

			fclose(pDestFp);
			pDestFp = NULL;
			return false;
		}

		if( 0 != fflush( pDestFp ))
		{
			bIsCreateXMLFileOK = false;
		}

		fclose(pDestFp);
		if( NULL != pFileContent )
		{
			delete [] pFileContent;
			pFileContent=NULL;
		}
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		DeleteDirectory(m_xmlDirName);
#else	
		char seCmd[256]={0};
		snprintf(seCmd, sizeof(seCmd),"rm -rf %s",m_xmlDirName);
		DVRSystem(seCmd);
#endif		
	}

	return bIsCreateXMLFileOK;
}

const char *CCreateXMLFile::GetExportPath()
{
	return m_xmlDirName;
}

/***********************************************************************
** Function     : InitialXMLFile(XML_FORMAT format)
** Description  : 
					1：以当前的时间创建xml文件，并写xml文件头
					2：把生成的文件名存放到xmlFileName.xml文件中

** Input		:
					1：format 要生成何种xml文件，用来控制文件名格式。比如日志文件的形式20100421111445LogList.xml,事件文件的形式为20100421105904EventList.xml

** Output		:
					返回true表示操作成功
					返回false表示操作失败
** Modify Record:
1:
***********************************************************************/
bool CCreateXMLFile::InitialXMLFile(XML_FORMAT format)
{	
	//在该目录下创建一个目录存放XML文件
	char dirTemp[256]={0};
	struct tm tmTime = DVRTime32ToTm(GetCurrTime32());

	if(0 == strlen(m_xmlDirName))
	{
		printf("%s:%s:%d, 导出目录错误\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return false;
	}

	if (m_bUseCDBackup)
	{
		switch(m_xmlFormat)
		{
		case XML_FORMAT_LOG:
			snprintf(dirTemp, sizeof(dirTemp), "%s/%s", m_xmlDirName, "log");
			break;
		case XML_FORMAT_EVENT:
			snprintf(dirTemp, sizeof(dirTemp), "%s/%s", m_xmlDirName, "event");
			break;
		case XML_BACKUP_EVENT:
			snprintf(dirTemp, sizeof(dirTemp), "%s/%s", m_xmlDirName, "bkEvent");
			break;
		default:
			assert(false);
			return false;
			break;
		}
	}
	else
	{
		switch(m_xmlFormat)
		{
		case XML_FORMAT_LOG:
			snprintf(dirTemp, sizeof(dirTemp), "%s/%04d-%02d-%02d-%02d-%02d-%02d-%s", m_xmlDirName, tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec, "log");
			break;
		case XML_FORMAT_EVENT:
			snprintf(dirTemp, sizeof(dirTemp), "%s/%04d-%02d-%02d-%02d-%02d-%02d-%s", m_xmlDirName, tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec, "event");
			break;
		case XML_BACKUP_EVENT:
			snprintf(dirTemp, sizeof(dirTemp), "%s/%04d-%02d-%02d-%02d-%02d-%02d-%s", m_xmlDirName, tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec, "backupEvent");
			break;
		default:
			assert(false);
			return false;
			break;
		}
	}
	
	strcpy(m_xmlDirName, dirTemp);
	
	printf("%s:%s:%d, 导出的日志/事件目录为:%s\n", __FUNCTION__, __FILE__, __LINE__, m_xmlDirName);
	snprintf(m_xmlFilesPath, sizeof(m_xmlFilesPath), "%s/%s", m_xmlDirName, "xmlfiles");


	//创建导出文件主目录和xml文件目录
	//需要先删除该目录中的所有信息
#ifdef __ENVIRONMENT_WIN32__
	DeleteDirectory(m_xmlDirName);
	mkdir(m_xmlDirName);
	chmod(m_xmlDirName, 666);
	mkdir(m_xmlFilesPath);

#else
	char seCmd[256]={0};
	snprintf(seCmd, sizeof(seCmd),"rm -rf %s",m_xmlDirName);
	DVRSystem(seCmd);

	snprintf(seCmd, sizeof(seCmd)," mkdir %s",m_xmlDirName);
	DVRSystem(seCmd);

	snprintf(seCmd, sizeof(seCmd),"mkdir %s",m_xmlFilesPath);
	DVRSystem(seCmd);

#endif


	//产生一个XML文件，存放信息的文件名
	char str[256]={0};
	snprintf(str,sizeof(str),"%s/%s", m_xmlFilesPath,"xmlFileName.xml");

#if defined(__X_LINUX__)
	m_xmlFileNameFd = open(str,O_RDWR);
#else
	m_xmlFileNameFd = open(str,O_RDWR|O_CREAT);
#endif

	chmod(str, 666);
	if(-1 == m_xmlFileNameFd)
	{
		perror("open()");
		assert(false);
		return false;
	}
	int count = write( m_xmlFileNameFd, _XMLdeclare_, strlen(_XMLdeclare_));
	if( count != strlen(_XMLdeclare_))
	{
		perror("write()");
		close(m_xmlFileNameFd);
		m_xmlFileFd = -1;
		assert(false);
		return false;
	}
	
	return true;
}

/***********************************************************************
** Function     : AddTitle()
** Description  : 
					1:添加xml中标题数据。包括生成的html标题和html中列标题

** Input		:
					1：pHeadTitle html标题
					2：ppElementTitle html中的列标题
					3：titleNum 列标题个数

** Output		:
					返回true表示操作成功
					返回false表示操作失败
** Modify Record:
1:
***********************************************************************/

bool CCreateXMLFile::AddTitle(const char *pHeadTitle, const char **ppElementTitle, int titleNum)
{
	if(!WriteStartMark("HeadElementTitleInfor", true))
	{
		assert(false);
		return false;
	}
	
	if(!AddItem("HeadTitle", pHeadTitle))
	{
		assert(false);
		return false;
	}


	char str[100]={0};
	sprintf(str,"%d",titleNum);
	if(!AddItem("ElementNum", str))
	{
		assert(false);
		return false;
	}

	for(int i=0; i<titleNum; i++)
	{
		sprintf(str,"%s", "ElementTitle");
		if(!AddItem(str,ppElementTitle[i]))
		{
			assert(false);
			return false;
		}
	}

	if(!WriteEndMark("HeadElementTitleInfor", true))
	{
		assert(false);
		return false;
	}

	return true;
}



bool CCreateXMLFile::AddButtonTip(const BUTTON_TIP_INFO *pButtonTipInfo , int tipCount)
{
	if ( (NULL != pButtonTipInfo) && (0 < tipCount) )
	{
		if(!WriteStartMark("buttonTip", true))
		{
			assert(false);
			return false;
		}

		bool retVal  = true;

		for (int i=0; i<tipCount; i++)
		{
			switch(pButtonTipInfo[i].tipNum)
			{
			case BUTTON_TIP_FIRST:
				retVal &= AddItem("first", pButtonTipInfo[i].tipValue);
				break;
			case BUTTON_TIP_PRE:
				retVal &= AddItem("preview", pButtonTipInfo[i].tipValue);
				break;
			case BUTTON_TIP_NEXT:
				retVal &= AddItem("next", pButtonTipInfo[i].tipValue);
				break;
			case BUTTON_TIP_LAST:
				retVal &= AddItem("last", pButtonTipInfo[i].tipValue);
				break;
			case BUTTON_TIP_RECORD:
				retVal &= AddItem("record", pButtonTipInfo[i].tipValue);
				break;
			case BUTTON_TIP_IS_THE_LAST:
				retVal &= AddItem("isTheLast", pButtonTipInfo[i].tipValue);
				break;
			case BUTTON_TIP_IS_THE_FIRST:
				retVal &= AddItem("isTheFirst", pButtonTipInfo[i].tipValue);
				break;
			default:
				break;
			}
		}
		if(!WriteEndMark("buttonTip", true))
		{
			assert(false);
			return false;
		}


		if (!retVal)
		{
			assert(false);
			return false;
		}

		return true;
	} 
	else
	{
		if (NULL == pButtonTipInfo && 0 == tipCount)
		{
			assert(false);
			return true;
		}

		return false;
	}

}

/***********************************************************************
** Function     : AddLogTitleInfo()
** Description  : 
					1：添加loglist xml文件的列属性，现在只有列宽
					2：添加xml中标题数据。

** Input		:
					1：pHeadTitle html标题
					2：ppElementTitle html中的列标题


** Output		:
					返回true表示操作成功
					返回false表示操作失败
** Modify Record:
1:
***********************************************************************/

bool CCreateXMLFile::AddLogTitleInfo(const char *pHeadTitle, const char **ppElementTitle)
{
	assert(NULL!=pHeadTitle);
	assert(NULL!=ppElementTitle);
	assert(NULL!=ppElementTitle[0]);
	assert(NULL!=ppElementTitle[1]);
	assert(NULL!=ppElementTitle[2]);
	assert(NULL!=ppElementTitle[3]);

	if(NULL==pHeadTitle)
	{
		printf("%s:%s:%d, xml文件标题不能为空\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return false;
	}

	for(int i=0; i<4; i++)
	{
		if(NULL == ppElementTitle[i])
		{
			printf("%s:%s:%d, xml文件元素标题不能为空\n", __FUNCTION__, __FILE__, __LINE__);
			assert(false);
			return false;
		}
		sprintf(m_xmlElementTitle[i],"%s", ppElementTitle[i]);

	}

	//添加表格宽度属性
	if(!WriteStartMark("AttributeWidth", true))
	{
		assert(false);
		return false;
	}
	char str[20]={0};
	sprintf(str, "%d", 400);
	bool bRet = AddItem("TableWidth", str);
	sprintf(str,"%d",200);
	bRet &= AddItem("TableWidth", str);
	bRet &= AddItem("TableWidth", str);
	bRet &= AddItem("TableWidth", str);
	if( !bRet )
	{
		printf("%s:%s:%d, 添加列表宽度属性失败\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return false;
	}

	if(!WriteEndMark("AttributeWidth", true))
	{
		assert(false);
		return false;
	}

	return AddTitle(pHeadTitle, ppElementTitle, 4);
}

/***********************************************************************
** Function     : AddLogElement()
** Description  : 
					1：添加一个loglist xml数据元素
** Input		:
					pInfor loglist 数据

** Output		:
					返回true表示操作成功
					返回false表示操作失败
** Modify Record:
1:
***********************************************************************/

bool CCreateXMLFile::AddLogElement(const OPERATOR_LOG *pInfor)						//添加一个xml元素
{
	if(!WriteStartMark("Element", true))
	{
		assert(false);
		return false;
	}

	//日志类型
	char str [512] = {0};
	unsigned long type = (pInfor->contentID << 16) | pInfor->type;
	//unsigned long strID=LogIDToStrID(type);
	//strcpy(str, CStringTable::Instance()->String(static_cast<ENUM_STRING_ID>(strID)));
	assert(m_pfnIdToString != NULL);
	strcpy(str, m_pfnIdToString(type, XML_FORMAT_LOG, m_pCallbackParam));

	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	//用户名
	if(!AddItem("Section", pInfor->name))
	{
		assert(false);
		return false;
	}

	//发生时间
#ifdef __ENVIRONMENT_LINUX__
	string st;
	GUI::CTimeCtrl::GetTime32Text(st, pInfor->time);
	strcpy(str, st.c_str());
#else
	sprintf(str, "%s", CTime(pInfor->time).Format("%Y-%m-%d %H:%M:%S"));
#endif
	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	//IP地址
	sprintf(str, "%03d.%03d.%03d.%03d", pInfor->IP&0xff, (pInfor->IP>>8)&0xff, (pInfor->IP>>16)&0xff, pInfor->IP>>24);
	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	if(!WriteEndMark("Element", true))
	{
		assert(false);
		return false;
	}

	return true;
}



/***********************************************************************
** Function     : AddEventTitleInfo()
** Description  : 
					1：添加eventlist xml文件的列属性，现在只有列宽
					2：添加xml中标题数据。
** Input		:
					1：pHeadTitle html标题
					2：ppElementTitle html中的列标题

** Output		:
					返回true表示操作成功
					返回false表示操作失败
** Modify Record:
1:
***********************************************************************/

bool CCreateXMLFile::AddEventTitleInfo(const char *pHeadTitle, const char **ppElementTitle)
{
	assert(NULL!=pHeadTitle);
	assert(NULL!=ppElementTitle);
	assert(NULL!=ppElementTitle[0]);
	assert(NULL!=ppElementTitle[1]);
	assert(NULL!=ppElementTitle[2]);
	assert(NULL!=ppElementTitle[3]);

	if(NULL==pHeadTitle)
	{
		printf("%s:%s:%d, xml文件标题不能为空\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}

	for(int i=0; i<4; i++)
	{
		if(NULL == ppElementTitle[i])
		{
			printf("%s:%s:%d, xml文件元素标题不能为空\n", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}
		sprintf(m_xmlElementTitle[i],"%s", ppElementTitle[i]);
	}

	//添加表格宽度属性
	if(!WriteStartMark("AttributeWidth", true))
	{
		assert(false);
		return false;
	}
	char str[20]={0};
	sprintf(str, "%d", 100);
	bool bRet = AddItem("TableWidth", str);
	sprintf(str,"%d",300);
	bRet &= AddItem("TableWidth", str);
	bRet &= AddItem("TableWidth", str);
	bRet &= AddItem("TableWidth", str);
	if( !bRet )
	{
		assert(false);
		return false;
	}

	if(!WriteEndMark("AttributeWidth", true))
	{
		assert(false);
		return false;
	}


	return AddTitle(pHeadTitle, ppElementTitle, 4);
}

bool CCreateXMLFile::AddBackupEventTitleInfo(const char *pHeadTitle, const char **ppElementTitle,int count)
{
	assert(NULL!=pHeadTitle);
	assert(NULL!=ppElementTitle);
	assert(NULL!=ppElementTitle[0]);
	assert(NULL!=ppElementTitle[1]);
	assert(NULL!=ppElementTitle[2]);
	assert(NULL!=ppElementTitle[3]);
	assert(NULL!=ppElementTitle[4]);

	if(NULL==pHeadTitle)
	{
		printf("%s:%s:%d, xml文件标题不能为空\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}

	for(int i=0; i<count; i++)
	{
		if(NULL == ppElementTitle[i])
		{
			printf("%s:%s:%d, xml文件元素标题不能为空\n", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}
		sprintf(m_xmlElementTitle[i],"%s", ppElementTitle[i]);
	}

	//添加表格宽度属性
	if(!WriteStartMark("AttributeWidth", true))
	{
		assert(false);
		return false;
	}
	char str[20]={0};
	sprintf(str, "%d", 100);
	bool bRet = AddItem("TableWidth", str);
	sprintf(str,"%d",300);
	bRet &= AddItem("TableWidth", str);
	bRet &= AddItem("TableWidth", str);
	bRet &= AddItem("TableWidth", str);
	bRet &= AddItem("TableWidth", str);
	if( !bRet )
	{
		assert(false);
		return false;
	}

	if(!WriteEndMark("AttributeWidth", true))
	{
		assert(false);
		return false;
	}


	return AddTitle(pHeadTitle, ppElementTitle, count);
}
/***********************************************************************
** Function     : AddEventElement()
** Description  : 
					1：添加一个eventlist xml数据元素
** Input		:
					pInfor eventlist 数据

** Output		:
					返回true表示操作成功
					返回false表示操作失败
** Modify Record:
1:
***********************************************************************/

bool CCreateXMLFile::AddEventElement(const EVENT_LOG *pInfor)
{
	if(!WriteStartMark("Element", true))
	{
		assert(false);
		return false;
	}

	//通道
	char str [512] = {0};
	sprintf(str,"%d",pInfor->chnn + 1);
	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	//类型
	assert(m_pfnIdToString != NULL);
	strcpy(str, m_pfnIdToString(pInfor->type, XML_FORMAT_EVENT, m_pCallbackParam));
	//switch(pInfor->type)
	//{
	//case EVENT_TYPE_MOTION:
	//	{
	//		strcpy(str, CStringTable::Instance()->String(E_STR_ID_MOTION));
	//	}
	//	break;
	//case EVENT_TYPE_SENSOR:

	//	{
	//		strcpy(str, CStringTable::Instance()->String(E_STR_ID_SENSOR));
	//	}
	//	break;
	//case EVENT_TYPE_V_LOSS:
	//	{
	//		strcpy(str, CStringTable::Instance()->String(E_STR_ID_VIDEO_LOSS));
	//	}
	//	break;
	//default:
	//	assert(false);
	//	return false;
	//}

	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	//开始时间
#ifdef __ENVIRONMENT_LINUX__
	string st;
	GUI::CTimeCtrl::GetTime32Text(st, pInfor->startTime);
	strcpy(str, st.c_str());
#else
	sprintf(str, "%s", CTime(pInfor->startTime).Format("%Y-%m-%d %H:%M:%S"));
#endif
	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	//结束时间 
#ifdef __ENVIRONMENT_LINUX__
	GUI::CTimeCtrl::GetTime32Text(st, pInfor->endTime);
	strcpy(str, st.c_str());
#else
	sprintf(str, "%s", CTime(pInfor->endTime).Format("%Y-%m-%d %H:%M:%S"));
#endif
	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	if(!WriteEndMark("Element", true))
	{
		assert(false);
		return false;
	}

	return true;
}
bool CCreateXMLFile::AddBackupEventElement(const EVENT_LOG *pInfor,int Elementcount)
{
	if(!WriteStartMark("Element", true))
	{
		assert(false);
		return false;
	}

	//索引
	char str [512] = {0};
	snprintf(str, 512, "%d",Elementcount + 1);
	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	//通道E_STR_ID_CHANNEL
	memset(str,0,sizeof(str));
#ifdef  __ENVIRONMENT_WIN32__
	snprintf(str,512,"%s%d","Channel",pInfor->chnn + 1);
#else
	snprintf(str,512,"%s%d",CStringTable::Instance()->String(E_STR_ID_CHANNEL),pInfor->chnn + 1);
#endif
	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	//类型
	memset(str,0,sizeof(str));
	if(EVENT_TYPE_MOTION & (pInfor->type))
	{
#ifdef __ENVIRONMENT_WIN32__
		strcpy(str, "Motion");
#else
		strcpy(str, CStringTable::Instance()->String(E_STR_ID_MOTION));
#endif
	}
	else if(EVENT_TYPE_SENSOR  & (pInfor->type))
	{
#ifdef __ENVIRONMENT_WIN32__
		strcpy(str, "Sensor");
#else
		strcpy(str, CStringTable::Instance()->String(E_STR_ID_SENSOR));
#endif
	}
	else if(EVENT_TYPE_V_LOSS   & (pInfor->type))
	{
#ifdef __ENVIRONMENT_WIN32__
		strcpy(str, "VideoLoss");
#else
		strcpy(str, CStringTable::Instance()->String(E_STR_ID_VIDEO_LOSS));
#endif
	}

	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	//开始时间
	memset(str,0,sizeof(str));
#ifdef __ENVIRONMENT_LINUX__
	string st;
	GUI::CTimeCtrl::GetTime32Text(st, pInfor->startTime);
	strcpy(str, st.c_str());
#else
	sprintf(str, "%s", CTime(pInfor->startTime).Format("%Y-%m-%d %H:%M:%S"));
#endif
	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	memset(str,0,sizeof(str));
	//结束时间 
#ifdef __ENVIRONMENT_LINUX__
	GUI::CTimeCtrl::GetTime32Text(st, pInfor->endTime);
	strcpy(str, st.c_str());
#else
	sprintf(str, "%s", CTime(pInfor->endTime).Format("%Y-%m-%d %H:%M:%S"));
#endif
	if(!AddItem("Section", str))
	{
		assert(false);
		return false;
	}

	if(!WriteEndMark("Element", true))
	{
		assert(false);
		return false;
	}

	return true;
}


bool CCreateXMLFile::AddItem(const char *pMark, const char *pInfor, int fd)
{

	if( !WriteStartMark( pMark, false, fd ))
	{
		assert(false);
		return false;
	}
	

	if(!WriteInforMark(pInfor, fd))
	{
		assert(false);
		return false;
	}
	

	if( !WriteEndMark( pMark , true,fd))
	{
		assert(false);
		return false;
	}

	return true;
}



bool CCreateXMLFile::WriteStartMark(const char *pMark, bool bEnd, int fd)								//写XML文件中的开始标记
{
	assert(NULL!=pMark);
	char *pStartMark = new char[strlen(pMark)+10];
	if(NULL != pStartMark)
	{
		memset(pStartMark, 0 ,sizeof(pStartMark));
		if( bEnd )
		{
			sprintf( pStartMark,"\r\n<%s>\r\n\r\n", pMark);
		}
		else
		{
			sprintf( pStartMark,"<%s>", pMark);
		}
		
		int count = 0;
		if( -1 == fd)
		{ 
			count = write(m_xmlFileFd, pStartMark,strlen(pStartMark));
		}else
		{
			count = write(fd, pStartMark,strlen(pStartMark));
		}
		if(count!=strlen(pStartMark))
		{
			printf("%s:%s:%d, write the start mark =%s error, mark=%s\n", __FUNCTION__, __FILE__,__LINE__, pStartMark, pMark);
			assert(false);
			delete [] pStartMark;
			pStartMark = NULL;
			return false;
		}

		delete [] pStartMark;
		pStartMark = NULL;

		return true;
	}

	assert(false);
	return false;
}


bool CCreateXMLFile::WriteInforMark(const char *pInfor, int fd)								//写内容
{
	assert(NULL!=pInfor);

	int count =0;
	if( -1 == fd)
	{
		count = write(m_xmlFileFd, pInfor,strlen(pInfor));
	}
	else
	{
		count = write(fd, pInfor,strlen(pInfor));
	}

	if(count!=strlen(pInfor))
	{
		printf("%s:%s:%d, write the mark information=%s error\n", __FUNCTION__, __FILE__,__LINE__, pInfor);
		assert( false );
		return false;
	}

	return true;

}



bool CCreateXMLFile::WriteEndMark(const char *pMark, bool bEnd, int fd)									//写结束标记
{

	assert(NULL!=pMark);
	char *pEndMark = new char[strlen(pMark)+10];
	if(NULL != pEndMark)
	{
		memset(pEndMark, 0 ,sizeof(pEndMark));
		if( bEnd )
		{
			sprintf( pEndMark,"</%s>\r\n\r\n", pMark);
		}
		else
		{
			sprintf( pEndMark,"</%s>", pMark);
		}

		int count = 0;
		if( -1 == fd)
		{
			count = write(m_xmlFileFd, pEndMark,strlen(pEndMark));
		}
		else
		{
			count = write(fd, pEndMark,strlen(pEndMark));
		}

		if(count!=strlen(pEndMark))
		{
			printf("%s:%s:%d, write the end mark =%s error, mark=%s\n", __FUNCTION__, __FILE__,__LINE__, pEndMark, pMark);
			assert(false);
			delete [] pEndMark;
			pEndMark = NULL;
			return false;
		}

		delete [] pEndMark;
		pEndMark = NULL;

		return true;
	}

	assert(false);
	return false;
}


bool CCreateXMLFile::DeleteFiles()
{
	return true;
}

bool CCreateXMLFile::FileFlush( int des, const char *mode )
{
	//if( (des <= 0) || ( NULL == mode ) )
	//{
	//	assert(false);
	//	printf("%s:%s:%d, FileFlush() error\n", __FUNCTION__, __FILE__, __LINE__);
	//	return false;
	//}

	//FILE *pfd  = fdopen( des,mode);
	//if( NULL == pfd )
	//{
	//	assert(false);
	//	perror("fdopen()");
	//	return false;
	//}
	//else
	//{
	//	if( 0 != fflush( pfd ))
	//	{
	//		assert(false);
	//		perror("fflush()");
	//		return false;
	//	}

	//	return true;
	//}
	return true;
}

#ifdef __ENVIRONMENT_WIN32__
bool CCreateXMLFile::DeleteDirectory(char *DirName)
{
	CFileFind tempFind;
	char tempFileFind[200];

	memset(tempFileFind, 0, sizeof(tempFileFind));
	sprintf(tempFileFind,"%s/*.*",DirName);

	BOOL IsFinded=(BOOL)tempFind.FindFile(tempFileFind);

	while(IsFinded)
	{
		IsFinded=(BOOL)tempFind.FindNextFile();
		if(!tempFind.IsDots())
		{
			char foundFileName[200];
			memset(foundFileName, 0, sizeof(foundFileName));
			strcpy(foundFileName,tempFind.GetFileName().GetBuffer(200));
			if(tempFind.IsDirectory())
			{
				char tempDir[200];
				memset(tempDir, 0, sizeof(tempDir));
				sprintf(tempDir,"%s/%s",DirName,foundFileName);
				DeleteDirectory(tempDir);
			}
			else
			{
				char tempFileName[200];
				memset(tempFileName, 0, sizeof(tempFileName));
				sprintf(tempFileName,"%s/%s",DirName,foundFileName);
				DeleteFile(tempFileName);
			}
		}
	}

	tempFind.Close();
	if(!RemoveDirectory(DirName))
	{
		return FALSE;
	}

	return TRUE;
}
#endif

