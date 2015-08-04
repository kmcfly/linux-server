#include "CutKeepLog.h"

CCutKeepLog::~CCutKeepLog()
{

}

CCutKeepLog::CCutKeepLog()
{
	m_pFile=NULL;
}

bool CCutKeepLog::OpenKeepFile()
{
	if(m_pFile!=NULL)
	{
		fclose(m_pFile);
		m_pFile=NULL;
	}
	
	m_pFile=fopen(CUT_BACKUP_INFO_PATH,"rb+");
	if(NULL==m_pFile)
	{
		printf("m_pfile open err\n");
		if (m_pFile = fopen(CUT_BACKUP_INFO_PATH, "wb+"))
		{
			if (NULL == m_pFile)
			{
				return false;
			}
		}		
	}
	
	return true;
}

bool CCutKeepLog::WriteKeepFile(BACKUP_DATA_SEQ_VECTOR *pLogExVec)//一次性写入全文
{
	assert(pLogExVec!=NULL);
	assert(m_pFile!=NULL);

	if (NULL == m_pFile)
	{
		return false;
	}	

	//读写头信息	
	int writeNum = pLogExVec->size();
	if (writeNum > CUT_BACKUP_INFO_MAX_NUM)
	{
		writeNum = CUT_BACKUP_INFO_MAX_NUM;
	}

	CUT_BACKUP_HEAD_INFO headInfo;
	memset(&headInfo,0,sizeof(CUT_BACKUP_HEAD_INFO));
	headInfo.version = CUT_BACKUP_INFO_VER;
	headInfo.filenum = writeNum;
	
	//跳到文件的开头
	if(0!=fseek(m_pFile, 0, SEEK_SET))
	{
		return false;
	}
	
	if(1!=fwrite(&headInfo,sizeof(CUT_BACKUP_HEAD_INFO),1,m_pFile))
	{
		return false;
	}

	//写实际文件长度
	for(BACKUP_DATA_SEQ_VECTOR::iterator it=pLogExVec->begin();it!=pLogExVec->end();it++)
	{		
		if(1!=fwrite(&(*it),sizeof(BACKUP_DATA_SEQ),1,m_pFile))
		{			
			return false;
		}
	}
	return true;
}

bool CCutKeepLog::ReadKeepFile(BACKUP_DATA_SEQ_VECTOR *pLogExVec)//读取全文
{

	assert(pLogExVec!=NULL);
	assert(m_pFile!=NULL);

	if (NULL == m_pFile)
	{
		return false;
	}

	//跳到文件的开头
	if(0!=fseek(m_pFile,0,SEEK_SET))
	{
		return false;
	}

	//读取头信息
	CUT_BACKUP_HEAD_INFO headInfo;
	memset(&headInfo,0,sizeof(CUT_BACKUP_HEAD_INFO));
	printf("headInfo size=%d\n",sizeof(CUT_BACKUP_HEAD_INFO));
	if(1!=fread(&headInfo,sizeof(CUT_BACKUP_HEAD_INFO),1,m_pFile))
	{	
		return false;//文件不存在或者出错或文件内容为空
	}

	if (headInfo.version != CUT_BACKUP_INFO_VER)
	{
		return false;
	}

	if ((headInfo.filenum <= 0) || (headInfo.filenum > CUT_BACKUP_INFO_MAX_NUM))
	{
		return false;
	}

	BACKUP_DATA_SEQ logex_info;
	memset(&logex_info,0,sizeof(BACKUP_DATA_SEQ));
	for(int i=0;i<headInfo.filenum;i++)
	{
		if(1!=fread(&logex_info,sizeof(BACKUP_DATA_SEQ),1,m_pFile))
		{			
			return false;
		}
		else
		{
			//比较记录时间和当前时间的差值
			if(logex_info.changeTime!=0)//保留一天后删除,=0永久保留
			{
				if(GetCurrTime32()<=logex_info.changeTime)
				{
					
				}
				else
				{
					unsigned long offset=GetCurrTime32()-logex_info.changeTime;
					unsigned long daylong=1*24*60*60;
					if(offset<daylong)
					{
						pLogExVec->push_back(logex_info);
					}
				}
			}
			else
			{
				pLogExVec->push_back(logex_info);
			}
		}
	}
	
	return true;
}


void CCutKeepLog::Quit()
{
	if(m_pFile!=NULL)
	{
		fclose(m_pFile);
		m_pFile=NULL;
	}
}

//bool CCutKeepLog::cmplogfile(BACKUP_DATA_SEQ_VECTOR *pLogExVec)
//{
//	//比较文件内容和容器内是否一样，一样就不写了
//	if(pLogExVec==NULL)
//	{
//		return false;
//	}
//	bool bsame=true;
//	BACKUP_DATA_SEQ_VECTOR cmpVec;
//	bool bopen=OpenKeepFile();
//	if(!bopen)
//	{
//		dellogfile();
//		return false;
//	}
//	bool bread=ReadKeepFile(&cmpVec);
//	if(!bread)
//	{
//		Quit();
//		return false;
//	}
//	Quit();
//	int num=pLogExVec->size();
//	if(num==0)
//	{
//		dellogfile();
//		return false;
//	}
//	unsigned long   filechangeTime,VecChangeTime;
//
//	for(int i=0;i<num;i++)
//	{
//		filechangeTime=0;
//		VecChangeTime=0;
//		filechangeTime=cmpVec[i].changeTime;
//		VecChangeTime=((*pLogExVec)[i]).changeTime;
//		if(filechangeTime!=VecChangeTime)
//		{
//			bsame=false;
//		}
//	}
//	if(!bsame)
//	{
//		dellogfile();
//	}
//	cmpVec.clear();
//	return bsame;
//
//}

void  CCutKeepLog::Dellogfile()
{
#ifdef __ENVIRONMENT_LINUX__
	char cmd[128]={0};
	snprintf(cmd, 128, "rm -rf %s", CUT_BACKUP_INFO_PATH);
	DVRSystem(cmd);	
#endif
}
