#include "PreBackupAudioInfo.h"
CpreBackupAudioInfo::CpreBackupAudioInfo()
{
	m_bOpen = false;
}
CpreBackupAudioInfo::~CpreBackupAudioInfo()
{
	
}
void CpreBackupAudioInfo::Initial()
{
	m_bOpen = false;	
}
void CpreBackupAudioInfo::Quit()
{
	
}

bool CpreBackupAudioInfo::OpenAndCheckFile(unsigned long diskindex,unsigned long fileindex, unsigned long startTime, unsigned long endTime)
{
	char SrcFilePathName[255]={0};
	unsigned long fileStartTime = 0;
	unsigned long fileEndTime = 0;
	unsigned long rangTime = 5*60;
	sprintf(SrcFilePathName, "%s/%02d/%08d.dat", "/mnt",diskindex,fileindex);
	if (!m_RecReader.Open(SrcFilePathName,FILE_BUF_TYPE_LOCAL))
	{
		m_bOpen = false;
		return false;
	}

	m_RecReader.GetTimeRange(fileStartTime, fileEndTime);

	if (fileEndTime <= fileStartTime)
	{
		m_bOpen = false;
		m_RecReader.Close();
		return false;
	}

	if (fileStartTime <= rangTime)
	{
		fileStartTime = 0;
	}
	else
	{
		fileStartTime -= rangTime;
	}

	fileEndTime += rangTime;

	if ((startTime >= fileStartTime) && (endTime <= fileEndTime))
	{
		m_bOpen = true;
		return true;
	}
		
	m_bOpen = false;
	m_RecReader.Close();
	return false;

}

void CpreBackupAudioInfo::CloseFile()
{
	if (m_bOpen)
	{
		m_RecReader.Close();
		m_bOpen = false;
	}	
}

bool CpreBackupAudioInfo::CheckfileAudio()//测试备份文件是不是带有音频
{
	if (!m_bOpen)
	{
		return false;
	}

	bool bAudio=false;
	WAVEFORMATEX waveFormat;
	if(m_RecReader.GetFrameNum(FRAME_TYPE_AUDIO) > 0)
	{
		if (ERR_READ_FILE_OK == m_RecReader.GetAudioInfo(&waveFormat))
		{
			bAudio = true;
		}
	}
	return bAudio;
}
unsigned long CpreBackupAudioInfo::FileSize(unsigned long startTime, unsigned long endTime)
{
	unsigned long startoffset=0,endoffset=0,offset=0;
	int ret1 = RECTell(startTime, &startoffset);
	int ret2 = RECTell(endTime, &endoffset);
	if ((ret1 < 0) || (ret2 < 0))
	{		
		return 128*1024*1024;
	}
	else
	{
		if(endoffset<startoffset)
		{
			startoffset=0;//如果出现异常。直接设置开始时间偏移为0;
		}
		offset=endoffset-startoffset;
		if(offset<=1024*1024)
		{
			return 1*1024*1024;
		}
		else
		{
			return offset;
		}
	}	
}

int CpreBackupAudioInfo::RECTell(unsigned long time, unsigned long *pOffSet)
{
	unsigned long ret=m_RecReader.SeekKeyFrameToTime((LONGLONG)(time)*1000000);
	if ((ERR_READ_FILE_OK == ret) || (ERR_READ_FILE_SEEK_TO_BEGIN == ret) || (ERR_READ_FILE_SEEK_TO_END == ret))
	{
		*pOffSet = m_RecReader.RECFtell();
		if((*pOffSet>0)&&(*pOffSet<=MAX_RECORD_FILE_LEN_M*1024*1024))
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

