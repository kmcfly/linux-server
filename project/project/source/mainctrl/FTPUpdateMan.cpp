/***********************************************************************
** 深圳同为数码科技有限公司保留所有权利。
** 作者			: huangxunliang
** 日期         : 2012-09-24
** 名称         : FtpUpdateMan.cpp
** 版本号		: 1.0
** 描述			:
** 修改记录		:
***********************************************************************/

#include "FTPUpdateMan.h"
#include "FtpLoadMan.h"
#include "NetInterface.h"
#include "Mp4File.h"

#ifdef _DEBUG
#define  new DEBUG_NEW
#endif

#ifdef WIN32
unsigned long CFtpUpdateMan::m_sUploadMp4FileLen = 8*1024*1024;
#else
unsigned long CFtpUpdateMan::m_sUploadMp4FileLen = 32*1024*1024;
#endif


CFtpUpdateMan::CFtpUpdateMan(void)
{
	m_updateStauts = UPDATE_STATUS_NONE;
}

CFtpUpdateMan::~CFtpUpdateMan(void)
{
}

CFtpUpdateMan * CFtpUpdateMan::Instance()
{
	static CFtpUpdateMan ftpMan;
	return &ftpMan;
}

bool CFtpUpdateMan::Initial(unsigned long ftpVideoBufLen)
{
	for (int i=0; i<TVT_MAX_CAMERA_NUM; i++)
	{
		m_ftpThreadInfo[i].bTest			= false;
		m_ftpThreadInfo[i].recType			= RECORD_TYPE_NULL;
		m_ftpThreadInfo[i].bUpload			= 0;
		m_ftpThreadInfo[i].upload_ID		= PUB_THREAD_ID_NOINIT;
		m_ftpThreadInfo[i].usedBuflen		= 0;
		m_ftpThreadInfo[i].pFtpUpdateMan	= NULL;
		m_ftpThreadInfo[i].bOpenFile		= 0;
		m_ftpThreadInfo[i].threadFrameList.clear();
		m_ftpThreadInfo[i].enableUploadCam	= 0;
		m_ftpThreadInfo[i].enableSchedule	= 1;
		memset(&m_ftpThreadInfo[i].uploadSchedule, 0, sizeof(WEEK_SCHEDULE));
		m_ftpThreadInfo[i].ftpVideoBufLen	= ftpVideoBufLen;
		m_ftpThreadInfo[i].needKeyFrame		= 1;
		m_ftpThreadInfo[i].channel			= i;
		m_ftpThreadInfo[i].fileStartTime	= 0;
		m_ftpThreadInfo[i].pMp4File			= NULL;
	}

	return true;
}

void CFtpUpdateMan::Quit()
{
	for (unsigned long chnn = 0; chnn<TVT_MAX_CAMERA_NUM; chnn++)
	{
		//先关闭发送线程
		ExitTread(chnn);

		//清除数据缓存的数据	
		ClearFrame(chnn);

		if (NULL != m_ftpThreadInfo[chnn].pMp4File)
		{
			delete m_ftpThreadInfo[chnn].pMp4File;
			m_ftpThreadInfo[chnn].pMp4File = NULL;
		}
		
	}
}

void CFtpUpdateMan::ClearStream()
{
	//释放缓存的视频数据
	for (unsigned long chnn = 0; chnn<TVT_MAX_CAMERA_NUM; chnn++)
	{
		ClearFrame(chnn);
	}
}

void CFtpUpdateMan::SetupFtpUpdateInfo(const TVT_FTP_CONFIG * pTVTFtpConfig)
{
	FTP_THREAD_INFO * pFtpThreadInfo = NULL;

	for (int i=0; i<TVT_MAX_CAMERA_NUM; i++)
	{
		pFtpThreadInfo =  &m_ftpThreadInfo[i];

		pFtpThreadInfo->updateThreadLock.Lock();

		//只有FTP开关和相应通道启用，才打开这个通道的FTP上传
		if (pTVTFtpConfig->enableFTP && pTVTFtpConfig->uploadVideo[i].enableUploadCam)
		{
			pFtpThreadInfo->enableUploadCam = 1;
		}
		else
		{
			pFtpThreadInfo->enableUploadCam = 0;
		}

		pFtpThreadInfo->updateThreadLock.UnLock();
	}

	if ((32 > pTVTFtpConfig->uploadVideoLen) || (64 < pTVTFtpConfig->uploadVideoLen))
	{
		m_sUploadMp4FileLen = 32 * 1024 * 1024;
	}
	else
	{
		m_sUploadMp4FileLen = pTVTFtpConfig->uploadVideoLen * 1024 * 1024;
	}
	
}

void CFtpUpdateMan::PutFrame(CFrameData *pFrameData)
{
	if (UPDATE_STATUS_NONE == m_updateStauts)
	{
		FRAME_INFO_EX & frameInfo = pFrameData->GetFrameInfo();
		FTP_THREAD_INFO * pFtpThreadInfo = &m_ftpThreadInfo[frameInfo.channel];

		pFtpThreadInfo->updateThreadLock.Lock();

		//如果当前没有打开上传的文件就不放数据到缓存
		if (pFtpThreadInfo->bOpenFile)
		{
			if (((FRAME_TYPE_VIDEO == frameInfo.frameType) || (FRAME_TYPE_VIDEO_FORMAT == frameInfo.frameType)) && (!pFtpThreadInfo->needKeyFrame || frameInfo.keyFrame))
			{
				if (pFtpThreadInfo->needKeyFrame && frameInfo.keyFrame)
				{
					pFtpThreadInfo->needKeyFrame = 0;
				}

				pFtpThreadInfo->threadFrameList.push_back(pFrameData);
				pFrameData->Add();
				pFtpThreadInfo->usedBuflen += frameInfo.length;

				if (pFtpThreadInfo->usedBuflen > pFtpThreadInfo->ftpVideoBufLen)
				{
					printf("%s:%s:%d chnn %d drop,type=0x%08x,bOpenFile=%d,bUpload=%d,usedBuflen=%d,ftpVideoBufLen=%d size=%d\n", __FUNCTION__, __FILE__, __LINE__,pFtpThreadInfo->channel, pFtpThreadInfo->recType, pFtpThreadInfo->bOpenFile, pFtpThreadInfo->bUpload, pFtpThreadInfo->usedBuflen, pFtpThreadInfo->ftpVideoBufLen, pFtpThreadInfo->threadFrameList.size());
					pFtpThreadInfo->bTest = true;


					DropFrame(pFtpThreadInfo);

					printf("%s:%s:%d chnn %d drop,type=0x%08x,bOpenFile=%d,bUpload=%d,usedBuflen=%d,ftpVideoBufLen=%d size=%d\n", __FUNCTION__, __FILE__, __LINE__,pFtpThreadInfo->channel, pFtpThreadInfo->recType, pFtpThreadInfo->bOpenFile, pFtpThreadInfo->bUpload, pFtpThreadInfo->usedBuflen, pFtpThreadInfo->ftpVideoBufLen, pFtpThreadInfo->threadFrameList.size());
				}
			}
		}
		else
		{
			//否则表示，处于暂停状态，此时应当清空发送列表
			if (!pFtpThreadInfo->threadFrameList.empty())
			{
				ClearFrame(pFtpThreadInfo);
			}
		}

		pFtpThreadInfo->updateThreadLock.UnLock();
	}
	else
	{
		bool bAllFileClosed = true;
		for (unsigned long chnn = 0; chnn<TVT_MAX_CAMERA_NUM; chnn++)
		{
			if (m_ftpThreadInfo[chnn].bOpenFile)
			{
				bAllFileClosed = false;
				break;
			}
		}

		if (bAllFileClosed)
		{
			m_updateStauts = UPDATE_STATUS_NONE;
		}
	}
}

bool CFtpUpdateMan::StartUpdate(unsigned long chnn, RECORD_TYPE recType)
{
	FTP_THREAD_INFO * pFtpThreadInfo = &m_ftpThreadInfo[chnn];

	//线程没启动，先启动上传FTP的线程
	if (PUB_THREAD_ID_NOINIT == pFtpThreadInfo->upload_ID)
	{
		pFtpThreadInfo->pFtpUpdateMan = this;

		pFtpThreadInfo->upload_ID = PUB_CreateThread(UpdateProcThread, pFtpThreadInfo, &pFtpThreadInfo->bUpload);
		if(PUB_CREATE_THREAD_FAIL == pFtpThreadInfo->upload_ID)
		{
			printf("%s:%s:%d, Initial ftp update thread fail!\n", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}
	}

	pFtpThreadInfo->recType |= recType;
	
	printf("%s:%s:%d, Ftp channel =%d , channel=%d, rec type =0x%08x\n", __FUNCTION__, __FILE__, __LINE__, chnn, pFtpThreadInfo->channel, pFtpThreadInfo->recType);

	return true;
}

void CFtpUpdateMan::StopUpdate(unsigned long chnn, RECORD_TYPE recType)
{
	m_ftpThreadInfo[chnn].recType &= (~recType);
}
/*
*
*In put:
*	bStop 为真表示暂停传输，为假表示恢复传输
*
*
*/
void CFtpUpdateMan::Action(bool bAction)
{
	if (bAction)
	{
		m_updateStauts = UPDATE_STATUS_POUSE_DELAY;
	}
	else
	{	
		m_updateStauts = UPDATE_STATUS_POUSE;

		//清除所有帧数据
		ClearStream();
	}
}

void CFtpUpdateMan::ExitTread(unsigned long chnn)
{
	FTP_THREAD_INFO * pFtpThreadInfo = &m_ftpThreadInfo[chnn];

	if (PUB_THREAD_ID_NOINIT != pFtpThreadInfo->upload_ID)
	{		
		PUB_ExitThread(&pFtpThreadInfo->upload_ID, &pFtpThreadInfo->bUpload);

		StopUpdate(chnn, RECORD_TYPE_ALL);
	}
}

PUB_THREAD_RESULT PUB_THREAD_CALL CFtpUpdateMan::UpdateProcThread(void *  lpParameter)
{
	FTP_THREAD_INFO * pFtpThreadInfo = reinterpret_cast<FTP_THREAD_INFO *>(lpParameter);
	CFtpUpdateMan *pThis = pFtpThreadInfo->pFtpUpdateMan;

#ifndef WIN32
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, channel=%d, pid = %d, tid=%d\n", __FILE__, __LINE__, pFtpThreadInfo->channel, pid, tid);
#else
	DWORD pid = ::GetCurrentThreadId();
	printf("%s, %d, pid = %d\n", __FILE__, __LINE__, pid);
#endif

	pThis->UpdateProc(pFtpThreadInfo);

	return 0;
}

void CFtpUpdateMan::UpdateProc(FTP_THREAD_INFO * pFtpThreadInfo)
{
	CFrameData *pFrameData	= NULL;
	pFtpThreadInfo->bOpenFile	= false;
	pFtpThreadInfo->pMp4File	= new CMp4File(ENUM_MP4_DESTYPE_FTP);
	ULONGLONG sTime = 0;
	ULONGLONG eTime = 0;

	while (pFtpThreadInfo->bUpload)
	{
		if (!pFtpThreadInfo->recType && !pFtpThreadInfo->bOpenFile)
		{
			//如果没开启上传，并且没有打开上传文件，就等待上传开始
			PUB_Sleep(100);
			continue;
		}

		if (!pFtpThreadInfo->bOpenFile)
		{
			if (!OpenMp4File(pFtpThreadInfo))
			{
				PUB_Sleep(100);
				continue;
			}
		}
		
		pFtpThreadInfo->updateThreadLock.Lock();

		if (pFtpThreadInfo->bTest)
		{
			printf("%s:%s:%d, channel=%d have %d frame in list\n", __FUNCTION__, __FILE__, __LINE__, pFtpThreadInfo->channel, pFtpThreadInfo->threadFrameList.size());
		}

		if (pFtpThreadInfo->threadFrameList.size())
		{
			pFrameData = pFtpThreadInfo->threadFrameList.front();
			FRAME_INFO_EX & frameInfo = pFrameData->GetFrameInfo();

			bool bCloseFile = false;

			//文件写满，关闭文件
			if (pFtpThreadInfo->videoFileLen <= frameInfo.length + pFtpThreadInfo->pMp4File->GetDataCnt())
			{
				bCloseFile = true;
			}

			//超时也关闭文件，有可能是视频丢失上传
			if ((0 < pFtpThreadInfo->fileStartTime) && (UPLOAD_MP4_MAX_FILE_TIME < ((frameInfo.time / 1000000) - pFtpThreadInfo->fileStartTime)))
			{
				bCloseFile = true;
			}
			
			if (bCloseFile)
			{
				CloseMp4File(pFtpThreadInfo);
			}
			else
			{
				//写文件
				if (pFtpThreadInfo->bTest)
				{
					sTime = GetCurrTime();
				}

				if (WriteMp4File(pFtpThreadInfo, &frameInfo))
				{
					if (pFtpThreadInfo->bTest)
					{
						eTime = GetCurrTime();

						printf("%s:%s:%d, channel %d Write frame %d use time:", __FUNCTION__, __FILE__, __LINE__, pFtpThreadInfo->channel, frameInfo.length);
						PrintfBit64(eTime - sTime, true);
					}

					if (0 == pFtpThreadInfo->fileStartTime)
					{
						pFtpThreadInfo->fileStartTime = (frameInfo.time / 1000000);
					}

					//这三个步骤的顺序不能动
					pFtpThreadInfo->usedBuflen -= frameInfo.length;
					pFrameData->Dec();
					pFtpThreadInfo->threadFrameList.pop_front();

					if (pFtpThreadInfo->bTest)
					{
						eTime = GetCurrTime();

						printf("%s:%s:%d, channel %d Write frame %d use time:", __FUNCTION__, __FILE__, __LINE__, pFtpThreadInfo->channel, frameInfo.length);
						PrintfBit64(eTime - sTime, true);
					}
				}
				else
				{
					printf("%s:%s:%d, Write mp4 file error\n", __FUNCTION__, __FILE__, __LINE__);
					PUB_Sleep(100);
				}
			}

			pFtpThreadInfo->updateThreadLock.UnLock();
		} 
		else
		{
			if (UPDATE_STATUS_NONE != m_updateStauts)
			{
				CloseMp4File(pFtpThreadInfo);
			}
			
			pFtpThreadInfo->updateThreadLock.UnLock();
			PUB_Sleep(500);
		}
	}

	//关闭文件
	CloseMp4File(pFtpThreadInfo);

	//清理残留数据
	ClearFrame(pFtpThreadInfo);
}


void CFtpUpdateMan::CreateFileName(char *fileName, unsigned long chnn)
{
	char pIpAddr[128] = {0};
#ifdef WIN32
	strncpy(pIpAddr, "192.168.3.96", sizeof(pIpAddr));
#else
	unsigned long len = sizeof(pIpAddr);
	unsigned long ip = 0;
	
	if (!CNetInterface::Instance()->GetIPText(pIpAddr, sizeof(pIpAddr)))
	{
		strncpy(pIpAddr, "192.168.3.96", sizeof(pIpAddr));	
	}
#endif

	ULONGLONG curLLTime = GetCurrTime();
	unsigned long microsecond = curLLTime % 1000000;
	struct tm curTmTime = DVRTime32ToTm(GetCurrTime32());
	sprintf(fileName, "%s/%04d-%02d-%02d/CH%02lu/%02d_%02d_%02d_%06d.mp4", pIpAddr, 
		curTmTime.tm_year+1900, curTmTime.tm_mon + 1, curTmTime.tm_mday, chnn + 1,\
		curTmTime.tm_hour, curTmTime.tm_min, curTmTime.tm_sec, microsecond);
}

void CFtpUpdateMan::DropFrame(FTP_THREAD_INFO * pFtpThreadInfo)
{
	bool bFirstFrame = true;

	pFtpThreadInfo->updateThreadLock.Lock();

	for(std::list<CFrameData *>::iterator iter=pFtpThreadInfo->threadFrameList.begin(); iter !=pFtpThreadInfo->threadFrameList.end();)
	{
		CFrameData * pFrameData = *iter;
		FRAME_INFO_EX frameInfoEx = pFrameData->GetFrameInfo();

		if (bFirstFrame)
		{
			bFirstFrame = false;
		}
		else if (frameInfoEx.keyFrame)
		{
			break;
		}

		pFtpThreadInfo->usedBuflen -= frameInfoEx.length;
		pFrameData->Dec();

		//printf("%s:%s:%d, Need drop frame, channel=%d\n", __FUNCTION__, __FILE__, __LINE__, pFtpThreadInfo->channel);
		iter = pFtpThreadInfo->threadFrameList.erase(iter);
	}

	if (pFtpThreadInfo->threadFrameList.empty())
	{
		//链表空的，说明没有删除到下一个关键帧，
		printf("%s:%s:%d, need key frame\n", __FUNCTION__, __FILE__, __LINE__);
		pFtpThreadInfo->needKeyFrame = 1;
	}

	pFtpThreadInfo->updateThreadLock.UnLock();
}

void CFtpUpdateMan::ClearFrame(unsigned long chnn)
{
	ClearFrame(&m_ftpThreadInfo[chnn]);
}

void CFtpUpdateMan::ClearFrame(FTP_THREAD_INFO * pFtpThreadInfo)
{
	//清除数据		
	pFtpThreadInfo->updateThreadLock.Lock();
	for(std::list<CFrameData *>::iterator iter=pFtpThreadInfo->threadFrameList.begin(); iter !=pFtpThreadInfo->threadFrameList.end(); iter++)
	{
		(*iter)->Dec();
	}
	pFtpThreadInfo->threadFrameList.clear();
	pFtpThreadInfo->needKeyFrame = 1;
	pFtpThreadInfo->usedBuflen = 0;
	pFtpThreadInfo->updateThreadLock.UnLock();
}

bool CFtpUpdateMan::CheckSchedule(FTP_THREAD_INFO * pFtpThreadInfo)
{
	//查看是否在计划内
	unsigned long currentTime = GetCurrTime32();
	bool bInSchedule = false;

	if (pFtpThreadInfo->enableSchedule)
	{
		bInSchedule = true;
	}
	else
	{
		printf("%s:%s:%d,enableUploadCam=%d,  enableSchedule=%d\n", __FUNCTION__, __FILE__, __LINE__, pFtpThreadInfo->enableUploadCam, pFtpThreadInfo->enableSchedule);
	}

	return bInSchedule;
}

bool CFtpUpdateMan::OpenMp4File(FTP_THREAD_INFO * pFtpThreadInfo)
{
	if (!pFtpThreadInfo->bOpenFile)
	{
		//如果有报警录像类型、允许这个通道进行录像、并且在计划内、不处于暂停状态，则上传FTP文件
		if ((RECORD_TYPE_NULL != pFtpThreadInfo->recType) && pFtpThreadInfo->enableUploadCam && CheckSchedule(pFtpThreadInfo) && (UPDATE_STATUS_NONE == m_updateStauts))
		{
			char fileName[1024] = {0};

			//文件命名
			CreateFileName(fileName, pFtpThreadInfo->channel);

			//打开失败是否应该关闭上传，可否继续来打开。
			pFtpThreadInfo->videoFileLen = m_sUploadMp4FileLen;
			if (pFtpThreadInfo->pMp4File->Open(fileName, MP4_MODE_WRITE, pFtpThreadInfo->videoFileLen))
			{
				//打开文件成功,必须保证，第一帧是关键帧
				DropFrame(pFtpThreadInfo);

				pFtpThreadInfo->bOpenFile = 1;

				printf("%s:%s:%d, channel %d open file %s ok\n", __FUNCTION__, __FILE__, __LINE__, pFtpThreadInfo->channel, fileName);

				pFtpThreadInfo->bTest = false;

				return true;
			}
			else
			{
				printf("%s:%s:%d, channel %d open file %s error\n", __FUNCTION__, __FILE__, __LINE__, pFtpThreadInfo->channel, fileName);
			}
		}

		return false;
	}

	assert(false);

	return true;
}

bool CFtpUpdateMan::CloseMp4File(FTP_THREAD_INFO * pFtpThreadInfo)
{
	if (pFtpThreadInfo->bOpenFile)
	{
		pFtpThreadInfo->pMp4File->Close();
		pFtpThreadInfo->bOpenFile = 0;
		pFtpThreadInfo->fileStartTime = 0;
	}

	return true;
}

bool CFtpUpdateMan::WriteMp4File(FTP_THREAD_INFO * pFtpThreadInfo, FRAME_INFO_EX * pFrameInfo)
{
	if (0 != pFtpThreadInfo->pMp4File->Write(pFrameInfo))
	{
		//写时不清理
		CloseMp4File(pFtpThreadInfo);

		return false;
	}

	return true;
}

