#include "PUB_common.h"
#include "PreRecordDataImp.h"
#include "mylist.cpp"

int g_bufsize = 0x80000;  //缓存大小，即预录像文件的大小
int g_buffFileNum = 30;   //预先为预录像准备的文件的个数；这个值要大于 (PRE_RECORD_MAX_LEN/g_bufsize)

CPreRecordDataImp::CPreRecordDataImp(void):
m_nDelDataLen(0),
m_nAddDataLen(0),
m_nFileCount(0),
m_channel(0),
m_bCanRecord(false)
{
	memset(m_szDirPath, 0, sizeof(m_szDirPath));
	m_pBuf = NULL;
	m_pFileDataBuf = NULL;
	m_bufferFileNum = 0;
	m_pBufferFile = NULL;
	m_pFile = NULL;
}

CPreRecordDataImp::~CPreRecordDataImp(void)
{
	m_bCanRecord = false;

	m_fileList.RemoveAll();
	m_nFileCount = 0;
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	if (m_pFileDataBuf != NULL)
	{
		delete [] m_pFileDataBuf;
		m_pFileDataBuf = NULL;
	}

	if (m_pBuf != NULL)
	{
		delete []m_pBuf;
		m_pBuf = NULL;
	}
	m_nAddDataLen = 0;
	m_nDelDataLen = 0;

	memset(m_szDirPath, 0, sizeof((m_szDirPath)));
	
	m_bufferFileNum = 0;
	if (m_pBufferFile != NULL)
	{
		delete [] m_pBufferFile;
		m_pBufferFile = NULL;
	}

}

bool CPreRecordDataImp::Initial(unsigned char channel, char *path)
{
	assert(m_pBuf == NULL);
	assert(m_pFileDataBuf == NULL);
	m_channel = channel;
	m_bCanRecord = false;
	memset(m_szDirPath, 0, sizeof(m_szDirPath));
	m_pBuf = NULL;
	m_nDelDataLen = 0;
	m_nAddDataLen = 0;	
	m_fileList.RemoveAll();
	m_nFileCount = 0;	
	m_pFile = NULL;	
	m_pFileDataBuf = NULL;
	m_bufferFileNum = 0;
	m_pBufferFile = NULL;

	if (NULL == path)
	{
		return false;
	}	

	m_pBuf = new unsigned char[g_bufsize];
	if (NULL == m_pBuf)
	{
		return false;
	}
	memset(m_pBuf, 0, g_bufsize);

	snprintf(m_szDirPath, sizeof(m_szDirPath), "%s/%02d", path, m_channel);
	char szTmp[64] = {0};
	snprintf(szTmp, 64, "mkdir -p %s", m_szDirPath);
	if (0 != DVRSystem(szTmp))
	{
		delete [] m_pBuf;
		m_pBuf = NULL;
		memset(m_szDirPath, 0, sizeof(m_szDirPath));
		return false;
	}
	//snprintf(szTmp, 64, "rm %s/* -rf", m_szDirPath);
	//if (0 != DVRSystem(szTmp))
	//{
	//	delete [] m_pBuf;
	//	m_pBuf = NULL;
	//	memset(m_szDirPath, 0, sizeof(m_szDirPath));
	//	return false;
	//}

	bool bCreateSuc = true;
	m_bufferFileNum = g_buffFileNum;
	m_pBufferFile = new PRERECORD_BUFFER_FILE [m_bufferFileNum];
	for (int i = 0; i < m_bufferFileNum; ++i)
	{
		struct stat stBufTmp;
		bool bHaveFile = false;
		snprintf(m_pBufferFile[i].szName, 16, "%d.dat", i);
		m_pBufferFile[i].bUse = false;
		snprintf(szTmp, 64, "%s/%s", m_szDirPath, m_pBufferFile[i].szName);
#ifdef __ENVIRONMENT_LINUX__
		if (0 == stat(szTmp, &stBufTmp))
		{
			if (g_bufsize == stBufTmp.st_size)
			{
				bHaveFile = true;
			}
		}
#endif
		if (bHaveFile)
		{
			//printf("%s,%d, have  file %s\n",__FILE__,__LINE__,szTmp);
		}
		else
		{
			//printf("%s,%d, create file %s\n",__FILE__,__LINE__,szTmp);
			FILE *fp = fopen(szTmp, "wb");
			if (NULL == fp)
			{
				bCreateSuc = false;
				break;
			}
			if (g_bufsize != PreRecFWrite(m_pBuf, sizeof(char), g_bufsize, fp))
			{
				if (fp != NULL)
				{
					fclose(fp);
					fp = NULL;
				}
				bCreateSuc = false;
				break;
			}
			if (fp != NULL)
			{
				fclose(fp);
				fp = NULL;
			}
			
		}		
	}

	if (!bCreateSuc)
	{		
		printf("%s,%d,chnn:%d, create prerecord file failed\n",__FILE__,__LINE__, m_channel);

		m_bufferFileNum = 0;
		delete [] m_pBufferFile;
		m_pBufferFile = NULL;

		snprintf(szTmp, 64, "rm %s/* -rf", m_szDirPath);
		DVRSystem(szTmp);

		delete [] m_pBuf;
		m_pBuf = NULL;
		memset(m_szDirPath, 0, sizeof(m_szDirPath));
		return false;
	}

	m_bCanRecord = true;
	return true;
}

void CPreRecordDataImp::Quid()
{
	m_bCanRecord = false;
	
	m_fileList.RemoveAll();
	m_nFileCount = 0;
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	if (m_pFileDataBuf != NULL)
	{
		delete [] m_pFileDataBuf;
		m_pFileDataBuf = NULL;
	}
	
	if (m_pBuf != NULL)
	{
		delete []m_pBuf;
		m_pBuf = NULL;
	}
	m_nAddDataLen = 0;
	m_nDelDataLen = 0;

	if (strlen(m_szDirPath) > 0)
	{
		char szTmp[64] = {0};
		snprintf(szTmp, 64, "rm -rf %s", m_szDirPath);
		DVRSystem(szTmp);
		memset(m_szDirPath, 0, sizeof((m_szDirPath)));
	}

	m_bufferFileNum = 0;
	if (m_pBufferFile != NULL)
	{
		delete [] m_pBufferFile;
		m_pBufferFile = NULL;
	}

}

//0代表成功；-1代表失败，且这种失败不能再预录像了；-2代表失败，这种失败是由于缓存的文件不够导致的，且这种失败是可以再预录像的。
int CPreRecordDataImp::InsertData(unsigned char *buf, int nLen)
{
	if (!m_bCanRecord)
	{
		return -1;
	}

	unsigned long fileIndexTemp = 0;

	if (m_nAddDataLen + nLen <= g_bufsize)
	{
		memcpy(m_pBuf + m_nAddDataLen, buf, nLen);
		m_nAddDataLen += nLen;
	}
	else
	{		
		if (m_nAddDataLen == 0)
		{
			assert(nLen > g_bufsize);
			int ret = WriteBufToFile(buf, nLen, fileIndexTemp);
			if (ret >= 0)
			{
				PRERECORD_FILE_INFO fileInfo;
				fileInfo.fileIndex = fileIndexTemp;
				fileInfo.fileSize = nLen;
				m_fileList.AddTail(fileInfo);							
			}
			else
			{
				printf("%s,%d, chnn:%d, write to file failed\n",__FILE__,__LINE__,m_channel);
				return ret;
			}
			m_nAddDataLen = 0;
			printf("%s,%d, chnn:%d,write big buf to file filename = %d, size = %d\n", __FILE__,__LINE__, m_channel,fileIndexTemp, nLen);
		}
		else if (m_nAddDataLen > 0)
		{
			int ret = WriteBufToFile(m_pBuf, m_nAddDataLen, fileIndexTemp);
			if (ret >= 0)
			{		
				PRERECORD_FILE_INFO fileInfo;
				fileInfo.fileIndex = fileIndexTemp;
				fileInfo.fileSize = m_nAddDataLen;
				m_fileList.AddTail(fileInfo);						
			}
			else
			{
				printf("%s,%d, chnn:%d, write to file failed\n",__FILE__,__LINE__,m_channel);
				return ret;
			}
			
			memset(m_pBuf, 0, g_bufsize);
			if (nLen > g_bufsize)  //写入数据大于最大buf时， 将写入数据单独写成一个文件。
			{
				int ret = WriteBufToFile(buf, nLen, fileIndexTemp);
				if (ret >= 0)
				{
					PRERECORD_FILE_INFO fileInfo;
					fileInfo.fileIndex = fileIndexTemp;
					fileInfo.fileSize = nLen;
					m_fileList.AddTail(fileInfo);							
				}
				else
				{
					printf("%s,%d, chnn:%d, write to file failed\n",__FILE__,__LINE__,m_channel);
					return ret;
				}
				m_nAddDataLen = 0;
				printf("%s,%d, chnn:%d,write big buf to file filename = %d, size = %d\n", __FILE__,__LINE__, m_channel, fileIndexTemp, nLen);
			}
			else
			{
				memcpy(m_pBuf, buf, nLen);
				m_nAddDataLen = nLen;
			}
		}
	}
	
	return 0;
}

bool CPreRecordDataImp::GetData(unsigned char **buf, int nLen)
{
	if (!m_bCanRecord)
	{
		return false;
	}

	int nRead = 0;
	char szPath[64] = {0};

	if (m_nDelDataLen + nLen <= g_bufsize)
	{	
		if (m_fileList.GetCount() >0 )
		{
			if (NULL == m_pFile)
			{
				assert(NULL == m_pFileDataBuf);
				if (m_pFileDataBuf != NULL)
				{
					delete [] m_pFileDataBuf;
					m_pFileDataBuf = NULL;
				}
				PRERECORD_FILE_INFO fileInfo = m_fileList.GetHead();
				assert(fileInfo.fileSize > 0);
				snprintf(szPath, 64, "%s/%s", m_szDirPath, m_pBufferFile[fileInfo.fileIndex].szName);
				m_pFile = fopen(szPath, "rb");
				if (m_pFile == NULL)
				{
					printf("%s,%d,chnn:%d open file %s fail\n", __FILE__,__LINE__, m_channel,szPath);
					return false;
				}				
				m_pFileDataBuf = new unsigned char [fileInfo.fileSize];
				nRead = fread(m_pFileDataBuf, sizeof(unsigned char), fileInfo.fileSize, m_pFile);
				if (nRead != fileInfo.fileSize)
				{
					printf("%s,%d,chnn:%d,fread file faild:%s\n",__FILE__,__LINE__,m_channel,szPath);
					fclose(m_pFile);
					m_pFile = NULL;
					delete [] m_pFileDataBuf;
					m_pFileDataBuf = NULL;
					return false;
				}
			}
			assert(m_pFile != NULL);
			assert(m_pFileDataBuf != NULL);			
			*buf = m_pFileDataBuf+m_nDelDataLen;
		}
		else
		{			
			*buf = m_pBuf + m_nDelDataLen;
		}
	}
	else
	{
		if (m_pFile != NULL)
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
		if (m_pFileDataBuf != NULL)
		{
			delete [] m_pFileDataBuf;
			m_pFileDataBuf = NULL;
		}

		if (m_nDelDataLen == 0)
		{
			assert(m_fileList.GetCount() > 0);
			POS pos = m_fileList.GetHeadPosition();
			PRERECORD_FILE_INFO fileInfo = m_fileList.GetAt(pos);
			snprintf(szPath, 64, "%s/%s", m_szDirPath, m_pBufferFile[fileInfo.fileIndex].szName);
			m_pFile = fopen(szPath, "rb");
			if (m_pFile == NULL)
			{
				printf("%s,%d,chnn:%d open file %s fail\n", __FILE__,__LINE__,m_channel, szPath);
				return false;
			}	
			assert(fileInfo.fileSize > 0);
			m_pFileDataBuf = new unsigned char [fileInfo.fileSize];
			nRead = fread(m_pFileDataBuf, sizeof(char), fileInfo.fileSize, m_pFile);
			if (nRead != fileInfo.fileSize)
			{
				printf("%s,%d,chnn:%d,fread file faild:%s\n",__FILE__,__LINE__,m_channel,szPath);
				fclose(m_pFile);
				m_pFile = NULL;
				delete [] m_pFileDataBuf;
				m_pFileDataBuf = NULL;
				return false;
			}
			*buf = m_pFileDataBuf;
		}
		else if (m_nDelDataLen > 0)
		{
			assert(m_fileList.GetCount() > 0); 			
			if (nLen > g_bufsize)
			{
				assert(m_fileList.GetCount() >= 2);
				POS pos = m_fileList.GetHeadPosition();
				m_fileList.GetNext(pos);
				PRERECORD_FILE_INFO fileInfo = m_fileList.GetAt(pos);
				snprintf(szPath, 64, "%s/%s", m_szDirPath, m_pBufferFile[fileInfo.fileIndex].szName);
				m_pFile = fopen(szPath, "rb");
				if (m_pFile == NULL)
				{
					printf("%s,%d,chnn:%d open file %s fail\n", __FILE__,__LINE__,m_channel, szPath);
					return false;
				}			
				assert(fileInfo.fileSize > 0);
				m_pFileDataBuf = new unsigned char [fileInfo.fileSize];
				nRead = fread(m_pFileDataBuf, sizeof(char), fileInfo.fileSize, m_pFile);
				if (nRead != fileInfo.fileSize)
				{
					printf("%s,%d,chnn:%d,fread file faild:%s\n",__FILE__,__LINE__,m_channel,szPath);
					fclose(m_pFile);
					m_pFile = NULL;
					delete [] m_pFileDataBuf;
					m_pFileDataBuf = NULL;
					return false;
				}			
				*buf = m_pFileDataBuf;
			}
			else
			{
				if (m_fileList.GetCount() >= 2)
				{
					POS pos = m_fileList.GetHeadPosition();
					m_fileList.GetNext(pos);
					PRERECORD_FILE_INFO fileInfo = m_fileList.GetAt(pos);
					snprintf(szPath, 64, "%s/%s", m_szDirPath, m_pBufferFile[fileInfo.fileIndex].szName);
					m_pFile = fopen(szPath, "rb");
					if (m_pFile == NULL)
					{
						printf("%s,%d,chnn:%d open file %s fail\n", __FILE__,__LINE__,m_channel, szPath);
						return false;
					}			
					assert(fileInfo.fileSize > 0);
					m_pFileDataBuf = new unsigned char [fileInfo.fileSize];
					nRead = fread(m_pFileDataBuf, sizeof(char), fileInfo.fileSize, m_pFile);
					if (nRead != fileInfo.fileSize)
					{
						printf("%s,%d,chnn:%d,fread file faild:%s\n",__FILE__,__LINE__,m_channel,szPath);
						fclose(m_pFile);
						m_pFile = NULL;
						delete [] m_pFileDataBuf;
						m_pFileDataBuf = NULL;
						return false;
					}			
					*buf = m_pFileDataBuf;					
				}
				else
				{
					*buf = m_pBuf;
				}
			}
		}
	}
	return true;
}

bool CPreRecordDataImp::DeleteData(int nLen)
{
	if (!m_bCanRecord)
	{
		return false;
	}

	char szCmd[64] = {0};
	if (m_nDelDataLen + nLen <= g_bufsize)
	{
		m_nDelDataLen += nLen;
	}
	else
	{
		if (m_nDelDataLen == 0)
		{
			assert(nLen > g_bufsize);
			assert(m_fileList.GetCount() > 0);
			if (m_pFile != NULL)
			{
				fclose(m_pFile);
				m_pFile = NULL;
			}
			if (m_pFileDataBuf != NULL)
			{
				delete [] m_pFileDataBuf;
				m_pFileDataBuf = NULL;
			}			

			PRERECORD_FILE_INFO fileInfo = m_fileList.RemoveHead();
			//snprintf(szCmd, 64, "rm -rf %s/%d.dat", m_szDirPath, fileInfo.fileIndex);
			//DVRSystem(szCmd);
			//snprintf(szCmd, 64, "%s/%d.dat", m_szDirPath, fileInfo.fileIndex);
			//remove(szCmd);
			m_pBufferFile[fileInfo.fileIndex].bUse = false;
			m_nDelDataLen = 0;
		}
		else if (m_nDelDataLen > 0)
		{
			assert(m_fileList.GetCount() > 0);
			PRERECORD_FILE_INFO fileInfo = m_fileList.RemoveHead();
			//snprintf(szCmd, 64, "rm -rf %s/%d.dat", m_szDirPath, fileInfo.fileIndex);
			//DVRSystem(szCmd);
			//snprintf(szCmd, 64, "%s/%d.dat", m_szDirPath, fileInfo.fileIndex);
			//remove(szCmd);
			m_pBufferFile[fileInfo.fileIndex].bUse = false;

			if (nLen > g_bufsize)
			{
				assert(m_fileList.GetCount() > 0);
				if (m_pFile!= NULL)
				{	
					fclose(m_pFile);
					m_pFile = NULL;
				}
				if (m_pFileDataBuf != NULL)
				{
					delete [] m_pFileDataBuf;
					m_pFileDataBuf = NULL;
				}
				PRERECORD_FILE_INFO fileInfo = m_fileList.RemoveHead();
				//snprintf(szCmd, 64, "rm -rf %s/%d.dat", m_szDirPath, fileInfo.fileIndex);
				//DVRSystem(szCmd);
				//snprintf(szCmd, 64, "%s/%d.dat", m_szDirPath, fileInfo.fileIndex);
				//remove(szCmd);
				m_pBufferFile[fileInfo.fileIndex].bUse = false;
				m_nDelDataLen = 0;	
			}
			else
			{
				m_nDelDataLen = nLen;
			}
		}
	}
	return true;
}


bool CPreRecordDataImp::DeleteDataEx(int nLen)
{
	if (!m_bCanRecord)
	{
		return false;
	}

	char szCmd[64] = {0};
	if (m_nDelDataLen + nLen <= g_bufsize)
	{
		m_nDelDataLen += nLen;
	}
	else
	{
		if (m_nDelDataLen == 0)
		{
			assert(nLen > g_bufsize);
			assert(m_fileList.GetCount() > 0);
			if (m_pFile != NULL)
			{
				fclose(m_pFile);
				m_pFile = NULL;
			}
			if (m_pFileDataBuf != NULL)
			{
				delete [] m_pFileDataBuf;
				m_pFileDataBuf = NULL;
			}			

			PRERECORD_FILE_INFO fileInfo = m_fileList.RemoveHead();
			//snprintf(szCmd, 64, "rm -rf %s/%d.dat", m_szDirPath, fileInfo.fileIndex);
			//DVRSystem(szCmd);
			//snprintf(szCmd, 64, "%s/%d.dat", m_szDirPath, fileInfo.fileIndex);
			//remove(szCmd);
			m_pBufferFile[fileInfo.fileIndex].bUse = false;
			m_nDelDataLen = 0;
		}
		else if (m_nDelDataLen > 0)
		{
			if (m_pFile != NULL)
			{
				fclose(m_pFile);
				m_pFile = NULL;
			}
			if (m_pFileDataBuf != NULL)
			{
				delete [] m_pFileDataBuf;
				m_pFileDataBuf = NULL;
			}
			assert(m_fileList.GetCount() > 0);
			PRERECORD_FILE_INFO fileInfo = m_fileList.RemoveHead();
			//snprintf(szCmd, 64, "rm -rf %s/%d.dat", m_szDirPath, fileInfo.fileIndex);
			//DVRSystem(szCmd);
			//snprintf(szCmd, 64, "%s/%d.dat", m_szDirPath, fileInfo.fileIndex);
			//remove(szCmd);
			m_pBufferFile[fileInfo.fileIndex].bUse = false;

			if (nLen > g_bufsize)
			{
				assert(m_fileList.GetCount() > 0);
				if (m_pFile!= NULL)
				{	
					fclose(m_pFile);
					m_pFile = NULL;
				}
				if (m_pFileDataBuf != NULL)
				{
					delete [] m_pFileDataBuf;
					m_pFileDataBuf = NULL;
				}
				PRERECORD_FILE_INFO fileInfo = m_fileList.RemoveHead();
				//snprintf(szCmd, 64, "rm -rf %s/%d.dat", m_szDirPath, fileInfo.fileIndex);
				//DVRSystem(szCmd);
				//snprintf(szCmd, 64, "%s/%d.dat", m_szDirPath, fileInfo.fileIndex);
				//remove(szCmd);
				m_pBufferFile[fileInfo.fileIndex].bUse = false;
				m_nDelDataLen = 0;	
			}
			else
			{
				m_nDelDataLen = nLen;
			}
		}
	}
	return true;
}

void CPreRecordDataImp::DeleteAllData()
{
	if (!m_bCanRecord)
	{
		return;
	}

	m_fileList.RemoveAll();
	m_nFileCount = 0;
	if (m_pFile != NULL)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	if (m_pFileDataBuf != NULL)
	{
		delete [] m_pFileDataBuf;
		m_pFileDataBuf = NULL;
	}

	if (m_pBuf != NULL)
	{
		memset(m_pBuf, 0, g_bufsize);
	}
	m_nAddDataLen = 0;
	m_nDelDataLen = 0;

	assert(strlen(m_szDirPath) > 0);
	if (strlen(m_szDirPath) > 0)
	{
		//char szTmp[64] = {0};
		//snprintf(szTmp, 64, "rm -rf %s/*", m_szDirPath);
		//DVRSystem(szTmp);
	}	

	/*
	m_bufferFileNum = 0;
	if (m_pBufferFile != NULL)
	{
		delete [] m_pBufferFile;
		m_pBufferFile = NULL;
	}*/
	if (m_pBufferFile != NULL)
	{
		for (int i = 0; i < m_bufferFileNum; ++i)
		{
			m_pBufferFile[i].bUse = false;
		}
	}

	return;
}
//
//bool CPreRecordDataImp::CreateDir()
//{
//#ifdef __ENVIRONMENT_LINUX__
//	if (strlen(m_szDirPath)<=0)
//	{
//		return false;
//	}
//	char szTmp[128] = {0};
//	if(access(m_szDirPath,0)==0)
//	{
//		printf("path is exist!\n");
//		return true;
//	} 
//	sprintf(szTmp, "mkdir -p %s", m_szDirPath);
//	if (0 == DVRSystem(szTmp))
//	{
//		return true;
//	}
//	return false;
//#endif
//}

//bool CPreRecordDataImp::ClearDir()
//{
//#ifdef __ENVIRONMENT_LINUX__
//	if (m_pFile != NULL)
//	{
//		fclose(m_pFile);
//		m_pFile = NULL;
//	}
//	if (strlen(m_szDirPath)<= 0)
//	{
//		return false;
//	}
//	char szTmp[128] = {0};
//	sprintf(szTmp, "rm -rf %s*", m_szDirPath);
//	if (0 == DVRSystem(szTmp))
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//#endif
//}

//bool CPreRecordDataImp::SetSaveDirPath(char * path, int iLen, int nSubarea)
//{
//	char szTmp[128] = {0};
//	int nSub = (m_nObjCount-1)/nSubarea;
//
//	if (m_bCanRecord)
//	{
//#ifdef __ENVIRONMENT_LINUX__
//		sprintf(szTmp, "rm -rf %s", m_szDirPath);
//		DVRSystem(szTmp);
//		memset(m_szDirPath, 0, sizeof(m_szDirPath));	
//#endif
//		if (path == NULL)
//		{
//			m_bCanRecord = false;
//		}
//		else
//		{
//			if (m_nObjID)
//			{
//			}
//			sprintf(m_szDirPath, "%s%02d/%d/", path, (m_nObjID-1)/nSubarea, m_nObjID);
//			m_bCanRecord = CreateDir();
//		}
//	}
//	else
//	{
//		if (path == NULL)
//		{
//			m_bCanRecord = false;
//		}
//		else
//		{
//			sprintf(m_szDirPath, "%s%02d/%d/", path, (m_nObjID-1)/nSubarea, m_nObjID);
//			m_bCanRecord = CreateDir();	
//		}
//	}
//
//	if (m_bCanRecord)
//	{
//		ClearDir();
//	}
//	return m_bCanRecord;
//}
//

int CPreRecordDataImp::FindBufferFileIndex()
{
	if (NULL == m_pBufferFile)
	{		
		return -1;
	}
	for (int i = 0; i < m_bufferFileNum; ++i)
	{		
		if (!m_pBufferFile[i].bUse)
		{			
			return i;
		}
	}

	return -2;
}

int CPreRecordDataImp::WriteBufToFile(unsigned char* buf ,int nLen, unsigned long &fileIndex)
{	
	assert(nLen > 0);
	char szPath[32] = {0};
	FILE *fp = NULL;
	int nWrite = 0;

	
	int temp = FindBufferFileIndex();	
	if (temp < 0)
	{
		return temp;
	}

	snprintf(szPath, 32, "%s/%s", m_szDirPath, m_pBufferFile[temp].szName);
	fp = fopen(szPath, "rb+");
	if (fp == NULL)
	{
		return -1;
	}

	nWrite = PreRecFWrite(buf, sizeof(char), nLen, fp);	
	fclose(fp);
	if (nWrite != nLen)
	{		
		return -1;
	}

	m_pBufferFile[temp].bUse = true;
	fileIndex = temp;
	return 0;
}

void CPreRecordDataImp::CleanPreRecordPath()
{
	Quid();
}

bool CPreRecordDataImp::ResetPreRecordPath(char * path)
{
	Quid();
	return Initial(m_channel, path);
}

size_t CPreRecordDataImp::PreRecFWrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
#if defined(__PACKID_14__) || defined(__PACKID_16__) || defined(__PACKID_22__) || defined(__PACKID_27__)
	int wCount = 60;
	size_t ret = 0;

	do 
	{
		ret = fwrite(ptr, size, nmemb, stream);
		if (0 < ret)
		{
			break;
		}
		PUB_Sleep(1000);
	} while (--wCount);

	return ret;
#else
	return fwrite(ptr, size, nmemb, stream);
#endif
}


