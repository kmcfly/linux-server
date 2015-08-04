/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2007-10-08
** Name         : RecordProc.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "RecordProc.h"

//public
CRecordProc::CRecordProc()
{
	m_channel           = ~(0x0);
	m_pReclogManEx      = NULL;
	m_pMsgMan           = NULL;
	m_pFilePath         = NULL;  

	m_startType		    = RECORD_TYPE_NULL;
	m_currType		    = RECORD_TYPE_NULL;

	m_bRecycleRecord    = false;

	m_pBitmapInfo       = NULL;
	m_pWaveFormat       = NULL;

	m_lastFrameTime     = 0;
	m_preRecordType     = PRERECORD_MODE_MEMORY;

	m_actualPrimaryStreamChnnNum = 0;
}

CRecordProc::~CRecordProc()
{

}

bool CRecordProc::Initial(unsigned long preRecordType, unsigned long channel, CReclogManEX *pReclogManEx, const char *pFilePath /* = NULL */, char *pPrerecordPath /* = NULL */, CMessageMan *pMsgMan /* = NULL */)
{
	assert(NULL != pReclogManEx);

	m_channel = channel;
	if (NULL == pReclogManEx)
	{
		return false;
	}
	m_pReclogManEx	= pReclogManEx;
	m_pFilePath		= new char [512];
	if (NULL != pFilePath)
	{
		strcpy(m_pFilePath, pFilePath);
	}
	else
	{
		memset(m_pFilePath, 0, 512);
	}
	m_pMsgMan		= pMsgMan;

	m_startType		= RECORD_TYPE_NULL;
	m_currType		= RECORD_TYPE_NULL;

	m_bRecycleRecord = false;
	m_mainRecWriter.bAction = true;
	m_mainRecWriter.bNeedNewFile = true;
	m_mainRecWriter.bOpen = false;
	memset(&m_mainRecWriter.fileInfo, 0, sizeof(FILE_INFO_EX));
	m_mainRecWriter.fileInfo.chnn = m_channel;
	m_mainRecWriter.startType = RECORD_TYPE_NULL;
	{
#if defined(__PRIMARY_WRDISK__)
		unsigned long ulLocalVideoInputNum = CProduct::Instance()->LocalVideoInputNum();
		unsigned long ulNetVideoInputNum = CProduct::Instance()->NetVideoInputNum();

		if ((0 <= channel) && (channel < ulLocalVideoInputNum))
		{
			m_mainRecWriter.recWriter.SetFileBufType(FILE_BUF_TYPE_LOCAL);
		}
		else if (channel < ulLocalVideoInputNum + ulNetVideoInputNum)
		{
			m_mainRecWriter.recWriter.SetFileBufType(FILE_BUF_TYPE_IP);
		}
		else
		{
			m_mainRecWriter.recWriter.SetFileBufType(FILE_BUF_TYPE_MIN);
		}
#endif
	}
	m_reduRecWriter.bAction = false;
	m_reduRecWriter.bNeedNewFile = true;
	m_reduRecWriter.bOpen = false;
	memset(&m_reduRecWriter.fileInfo, 0, sizeof(FILE_INFO_EX));
	m_reduRecWriter.fileInfo.chnn = m_channel;
	m_reduRecWriter.startType = RECORD_TYPE_NULL;
	if (CProduct::Instance()->SupportRedunRec())
	{
#if defined(__PRIMARY_WRDISK__)
		m_reduRecWriter.pRecWriter = new CPrimaryRecWriter;

		unsigned long ulLocalVideoInputNum = CProduct::Instance()->LocalVideoInputNum();
		unsigned long ulNetVideoInputNum = CProduct::Instance()->NetVideoInputNum();

		if ((0 <= channel) && (channel < ulLocalVideoInputNum))
		{
			m_reduRecWriter.pRecWriter->SetFileBufType(FILE_BUF_TYPE_LOCAL);
		}
		else if (channel < ulLocalVideoInputNum + ulNetVideoInputNum)
		{
			m_reduRecWriter.pRecWriter->SetFileBufType(FILE_BUF_TYPE_IP);
		}
		else
		{
			m_reduRecWriter.pRecWriter->SetFileBufType(FILE_BUF_TYPE_MIN);
		}
#else
		m_reduRecWriter.pRecWriter = new CRecWriter;
#endif
	}
	else
	{
		m_reduRecWriter.pRecWriter = NULL;
	}

	m_pBitmapInfo	= NULL;//new BITMAPINFOHEADER;
	m_pWaveFormat	= NULL;//new WAVEFORMATEX;

	m_lastFrameTime = 0;

	m_preRecordType = preRecordType;
	if (PRERECORD_MODE_DISK == m_preRecordType)
	{
		m_preDiskRecord.Initial(channel, pPrerecordPath);
	}

	m_actualPrimaryStreamChnnNum = CProduct::Instance()->VideoInputNum();
	//printf("m_actualPrimaryStreamChnnNum:%d\n",m_actualPrimaryStreamChnnNum);
	return true;
}

bool CRecordProc::IsCanPreDiskRecord()
{
	if (PRERECORD_MODE_DISK == m_preRecordType)
	{
		return m_preDiskRecord.IsCanPreRecord();
	}

	return false;
}

void CRecordProc::Quit()
{
	if (m_mainRecWriter.bOpen)
	{
		if (RECORD_TYPE_ALL & m_mainRecWriter.startType)
		{
			WriteReclogEnd(m_mainRecWriter.fileInfo, m_mainRecWriter.startType);
		}
		m_mainRecWriter.recWriter.Close();
		CancelFileIndex(m_mainRecWriter.fileInfo);
		m_mainRecWriter.bNeedNewFile = true;
		m_mainRecWriter.bOpen = false;
		memset(&m_mainRecWriter.fileInfo, 0, sizeof(FILE_INFO_EX));
		m_mainRecWriter.fileInfo.chnn = ~(0x0);
		m_mainRecWriter.startType = RECORD_TYPE_NULL;
	}

	if (m_reduRecWriter.bOpen)
	{
		if (RECORD_TYPE_ALL & m_reduRecWriter.startType)
		{
			WriteReclogEnd(m_reduRecWriter.fileInfo, m_reduRecWriter.startType);
		}
		if (NULL != m_reduRecWriter.pRecWriter)
		{
			m_reduRecWriter.pRecWriter->Close();
		}
		CancelFileIndex(m_reduRecWriter.fileInfo);
		m_reduRecWriter.bNeedNewFile = true;
		m_reduRecWriter.bOpen = false;
		memset(&m_reduRecWriter.fileInfo, 0, sizeof(FILE_INFO_EX));
		m_reduRecWriter.fileInfo.chnn = ~(0x0);
		m_reduRecWriter.startType = RECORD_TYPE_NULL;		
	}

	if (NULL != m_reduRecWriter.pRecWriter)
	{
		delete m_reduRecWriter.pRecWriter;
		m_reduRecWriter.pRecWriter = NULL;
	}
	
	if (m_pBitmapInfo != NULL)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
	if (m_pWaveFormat != NULL)
	{
		delete m_pWaveFormat;
		m_pWaveFormat = NULL;
	}
	
	m_startType		= RECORD_TYPE_NULL;
	m_currType		= RECORD_TYPE_NULL;
	
	m_pReclogManEx	= NULL;

	m_lastFrameTime = 0;

	m_channel = ~(0x0);


	delete [] m_pFilePath;
	m_pFilePath = NULL;
}


void CRecordProc::Reset()
{
	if (m_mainRecWriter.bOpen)
	{
		if (RECORD_TYPE_ALL & m_mainRecWriter.startType)
		{
			WriteReclogEnd(m_mainRecWriter.fileInfo, m_mainRecWriter.startType);
		}
		m_mainRecWriter.recWriter.Close();
#ifndef __PRIMARY_WRDISK__
		m_mainRecWriter.recWriter.ClearAudioBuffer();
#endif
		CancelFileIndex(m_mainRecWriter.fileInfo);
		m_mainRecWriter.bNeedNewFile = true;
		m_mainRecWriter.bOpen = false;
		memset(&m_mainRecWriter.fileInfo, 0, sizeof(FILE_INFO_EX));
		m_mainRecWriter.fileInfo.chnn = m_channel;
		m_mainRecWriter.startType = RECORD_TYPE_NULL;		
	}

	if (m_reduRecWriter.bOpen)
	{
		if (RECORD_TYPE_ALL & m_reduRecWriter.startType)
		{
			WriteReclogEnd(m_reduRecWriter.fileInfo, m_reduRecWriter.startType);
		}
		if (NULL != m_reduRecWriter.pRecWriter)
		{
			m_reduRecWriter.pRecWriter->Close();
#ifndef __PRIMARY_WRDISK__
			m_reduRecWriter.pRecWriter->ClearAudioBuffer();
#endif
		}
		CancelFileIndex(m_reduRecWriter.fileInfo);
		m_reduRecWriter.bNeedNewFile = true;
		m_reduRecWriter.bOpen = false;
		memset(&m_reduRecWriter.fileInfo, 0, sizeof(FILE_INFO_EX));
		m_reduRecWriter.fileInfo.chnn = m_channel;
		m_reduRecWriter.startType = RECORD_TYPE_NULL;
	}
	
	//由于在DateProc中采用的是：用“m_pBitmapInfo是否为空” 作为 “是否有视频格式”的判断条件。
	//所以如果需要视频格式，就要delete m_pBitmapInfo;
	/*
	if (NULL != m_pBitmapInfo)
	{
		memset(m_pBitmapInfo, 0, sizeof(BITMAPINFOHEADER));
	}	
	if (NULL != m_pWaveFormat)
	{
		memset(m_pWaveFormat, 0, sizeof(WAVEFORMATEX));
	}
	*/
	if (NULL != m_pBitmapInfo)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}	
	if (NULL != m_pWaveFormat)
	{
		delete m_pWaveFormat;
		m_pWaveFormat = NULL;
	}
	//end 2010-01-20 14:07 xwm
	

	m_startType		= RECORD_TYPE_NULL;
	m_currType		= RECORD_TYPE_NULL;
	if (NULL != m_pMsgMan)
	{
		m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
	}
	m_lastFrameTime = 0;
}


RECORD_WRITE_STATUS CRecordProc::DateProc(CFrameData & frameInfo, bool & bKeyFrame)
{
	assert(NULL != m_pReclogManEx);
	assert(CHANNEL_ID(m_channel) == CHANNEL_ID(frameInfo.GetFrameInfo().channel));

	m_channel = frameInfo.GetFrameInfo().channel;
	bKeyFrame	= false;//m_bNeedKeyFrame;

	/*
	处理视频、音频格式。
	如果原来没有格式，且当前送入的为非对应格式，则返回无格式提示。
	如果原来有格式，且当前送入格式数据，则删除原来的格式，替换用新的格式数据。
	*/
	//modify 2010-1-27 xwm
	//修改问题：在连接IE时，格式化硬盘会出现下面的断言：./td3520: AVDataSendMan.cpp: 521: long int CAVDataSendMan::PutAudioData(CFrameData*): Assertion `false' failed.
	//原因：有些通道没有音频，我们的代码只保证了不送入音频帧，但是没有保证不送入音频格式。
	{
		FRAME_TYPE frameType = (FRAME_TYPE)(frameInfo.GetFrameInfo().frameType);
		if (FRAME_TYPE_VIDEO_FORMAT == frameType)
		{
			if (NULL == m_pBitmapInfo)
			{
				m_pBitmapInfo = new BITMAPINFOHEADER;
				assert(NULL != m_pBitmapInfo);
			}
			else
			{
				BITMAPINFOHEADER *pBitMap = (BITMAPINFOHEADER *)(frameInfo.GetFrameInfo().pData);

				//特殊处理:小码流录像分辨率变化时不会切换文件
				if (((m_pBitmapInfo->biWidth != pBitMap->biWidth)||(m_pBitmapInfo->biHeight != pBitMap->biHeight))\
					 && m_channel<m_actualPrimaryStreamChnnNum)
				{
					//表明数据出错或修改了分辨率，如由CIF修改成D1。
					printf("(%s,%d):frame error or change video size from (W:%d, H:%d) to (W:%d, H:%d) \n",
						__FILE__, __LINE__, m_pBitmapInfo->biWidth, m_pBitmapInfo->biHeight, pBitMap->biWidth, pBitMap->biHeight);
					if (PRERECORD_MODE_DISK == m_preRecordType)
					{
						m_preDiskRecord.RemoveAll();
					}	
					else
					{

					}
					if (m_mainRecWriter.bAction)
					{
						if (m_mainRecWriter.bOpen)
						{
							if (RECORD_TYPE_ALL & m_mainRecWriter.startType)
							{
								WriteReclogEnd(m_mainRecWriter.fileInfo, m_mainRecWriter.startType);
							}
							m_mainRecWriter.recWriter.Close();
							CancelFileIndex(m_mainRecWriter.fileInfo);
							m_mainRecWriter.bNeedNewFile = true;
							m_mainRecWriter.bOpen = false;
							memset(&m_mainRecWriter.fileInfo, 0, sizeof(FILE_INFO_EX));
							m_mainRecWriter.fileInfo.chnn = m_channel;						
							m_mainRecWriter.startType = RECORD_TYPE_NULL;
						}
					}
					if (m_reduRecWriter.bAction)
					{					
						if (m_reduRecWriter.bOpen)
						{
							if (RECORD_TYPE_ALL & m_reduRecWriter.startType)
							{
								WriteReclogEnd(m_reduRecWriter.fileInfo, m_reduRecWriter.startType);
							}
							if (NULL != m_reduRecWriter.pRecWriter)
							{
								m_reduRecWriter.pRecWriter->Close();
							}
							CancelFileIndex(m_reduRecWriter.fileInfo);
							m_reduRecWriter.bNeedNewFile = true;
							m_reduRecWriter.bOpen = false;
							memset(&m_reduRecWriter.fileInfo, 0, sizeof(FILE_INFO_EX));
							m_reduRecWriter.fileInfo.chnn = m_channel;
							m_reduRecWriter.startType = RECORD_TYPE_NULL;
						}
					}
					m_currType |= m_startType;
					m_startType = RECORD_TYPE_NULL;
				}
			}
			memset(m_pBitmapInfo, 0, sizeof(BITMAPINFOHEADER));
			memcpy(m_pBitmapInfo, frameInfo.GetFrameInfo().pData, sizeof(BITMAPINFOHEADER));
			return RECORD_WRITE_SUCC;
		}
		else if (FRAME_TYPE_VIDEO == frameType)
		{
			if (NULL == m_pBitmapInfo)
			{
				return RECORD_WRITE_NO_BITMAPINFO;
			}			
		}
		else if (FRAME_TYPE_AUDIO_FORMAT == frameType)
		{
			if (NULL == m_pWaveFormat)
			{
				m_pWaveFormat = new WAVEFORMATEX;
				assert(NULL != m_pWaveFormat);
			}
			memset(m_pWaveFormat, 0, sizeof(WAVEFORMATEX));
			memcpy(m_pWaveFormat, frameInfo.GetFrameInfo().pData, sizeof(WAVEFORMATEX));
			return RECORD_WRITE_SUCC;
		}		
		else if (FRAME_TYPE_AUDIO == frameType)
		{
			if (NULL == m_pWaveFormat)
			{
				return RECORD_WRITE_NO_WAVEFORMAT;
			}
		}		
	}

	/*
	检查数据是否合法
	必须得失视频、音频数据，且视频数据的长宽正确。音频暂时没有办法进行检查，以后可以考虑用长度来检查。
	*/
	{
		assert((FRAME_TYPE_VIDEO == frameInfo.GetFrameInfo().frameType) || (FRAME_TYPE_AUDIO == frameInfo.GetFrameInfo().frameType));
		if ((FRAME_TYPE_VIDEO != frameInfo.GetFrameInfo().frameType) && (FRAME_TYPE_AUDIO != frameInfo.GetFrameInfo().frameType))
		{
			return RECORD_WRITE_FAIL;	
		}

		if (FRAME_TYPE_VIDEO == frameInfo.GetFrameInfo().frameType)
		{
			//特殊处理:小码流录像分辨率变化时不会切换文件
			if (((m_pBitmapInfo->biWidth != (long)frameInfo.GetFrameInfo().width) || (m_pBitmapInfo->biHeight != (long)frameInfo.GetFrameInfo().height))\
						&& m_channel<m_actualPrimaryStreamChnnNum)
			{
				//表明数据出错或修改了分辨率，如由CIF修改成D1。
				printf("(%s,%d):frame error or change video size from (W:%d, H:%d) to (W:%d, H:%d) \n",
					   __FILE__, __LINE__, m_pBitmapInfo->biWidth, m_pBitmapInfo->biHeight, (long)frameInfo.GetFrameInfo().width, (long)frameInfo.GetFrameInfo().height);
				if (PRERECORD_MODE_DISK == m_preRecordType)
				{
					m_preDiskRecord.RemoveAll();
				}
				else
				{

				}
				if (m_mainRecWriter.bAction)
				{
					if (m_mainRecWriter.bOpen)
					{
						if (RECORD_TYPE_ALL & m_mainRecWriter.startType)
						{
							WriteReclogEnd(m_mainRecWriter.fileInfo, m_mainRecWriter.startType);
						}
						m_mainRecWriter.recWriter.Close();
						CancelFileIndex(m_mainRecWriter.fileInfo);
						m_mainRecWriter.bNeedNewFile = true;
						m_mainRecWriter.bOpen = false;
						memset(&m_mainRecWriter.fileInfo, 0, sizeof(FILE_INFO_EX));
						m_mainRecWriter.fileInfo.chnn = m_channel;						
						m_mainRecWriter.startType = RECORD_TYPE_NULL;
					}
				}
				if (m_reduRecWriter.bAction)
				{					
					if (m_reduRecWriter.bOpen)
					{
						if (RECORD_TYPE_ALL & m_reduRecWriter.startType)
						{
							WriteReclogEnd(m_reduRecWriter.fileInfo, m_reduRecWriter.startType);
						}
						if (NULL != m_reduRecWriter.pRecWriter)
						{
							m_reduRecWriter.pRecWriter->Close();
						}
						CancelFileIndex(m_reduRecWriter.fileInfo);
						m_reduRecWriter.bNeedNewFile = true;
						m_reduRecWriter.bOpen = false;
						memset(&m_reduRecWriter.fileInfo, 0, sizeof(FILE_INFO_EX));
						m_reduRecWriter.fileInfo.chnn = m_channel;
						m_reduRecWriter.startType = RECORD_TYPE_NULL;
					}
				}
				m_currType |= m_startType;
				m_startType = RECORD_TYPE_NULL;
				if (m_pBitmapInfo != NULL)
				{
					delete m_pBitmapInfo;
					m_pBitmapInfo = NULL;
				}
				if (m_pWaveFormat != NULL)
				{
					delete m_pWaveFormat;
					m_pWaveFormat = NULL;
				}
				return RECORD_WRITE_NO_BITMAPINFO;
			}
		}
	}

	/*
	检查当前的录像状态。
	非录像状态，则进行预录像。
	*/
	{
		if ((RECORD_TYPE_NULL == m_startType) && (RECORD_TYPE_NULL == m_currType))
		{
			//加入预录像列表。
			if (PRERECORD_MODE_DISK == m_preRecordType)
			{
				m_preDiskRecord.AddFrame(&frameInfo, bKeyFrame);
			}
			else
			{
				m_preMemRecord.AddFrame(&frameInfo, bKeyFrame);
			}		

			return RECORD_WRITE_NO_RECORDING;
		}
	}

	
	/*
	检查是否需要打开新的文件。
	成功打开文件则继续往下走。
	检查关键帧放在该步骤后面，是为了能在关键帧来之前打开文件，而不是等来关键帧再花时间打开文件。
	如果没有打开文件，则把当前帧送入预录像列表，即满足预录像要求也满足缓存要求，做到不丢帧。
	*/

	if (m_mainRecWriter.bAction && (!m_reduRecWriter.bAction))
	{
		if (m_mainRecWriter.bNeedNewFile)
		{
			if (!GetFile(m_mainRecWriter.fileInfo.chnn, m_mainRecWriter.fileInfo.diskIndex, m_mainRecWriter.fileInfo.fileIndex, NET_DISK_READ_WRITE, &(m_mainRecWriter.recWriter)))
			{
				if (PRERECORD_MODE_DISK == m_preRecordType)
				{
					m_preDiskRecord.AddFrame(&frameInfo, bKeyFrame);
				}
				else
				{
					m_preMemRecord.AddFrame(&frameInfo, bKeyFrame);
				}
				return RECORD_WRITE_NO_FILE;
			}
			else
			{
				m_mainRecWriter.bNeedNewFile = false;
				m_mainRecWriter.bOpen = true;
			}
		}
	}
	else if (m_mainRecWriter.bAction && m_reduRecWriter.bAction)
	{
		if ((m_mainRecWriter.bNeedNewFile == m_reduRecWriter.bNeedNewFile))
		{
			if (m_mainRecWriter.bNeedNewFile)
			{
				bool bGetMainFile = GetFile(m_mainRecWriter.fileInfo.chnn, m_mainRecWriter.fileInfo.diskIndex, m_mainRecWriter.fileInfo.fileIndex, NET_DISK_READ_WRITE, &(m_mainRecWriter.recWriter));
				bool bGetReduFile = GetFile(m_reduRecWriter.fileInfo.chnn, m_reduRecWriter.fileInfo.diskIndex, m_reduRecWriter.fileInfo.fileIndex, NET_DISK_REDUNDANCY, m_reduRecWriter.pRecWriter);

				if (!bGetMainFile && !bGetReduFile)
				{
					if (PRERECORD_MODE_DISK == m_preRecordType)
					{
						m_preDiskRecord.AddFrame(&frameInfo, bKeyFrame);
					}
					else
					{
						m_preMemRecord.AddFrame(&frameInfo, bKeyFrame);
					}		
					return RECORD_WRITE_NO_FILE;
				}
				else
				{
					//分两种情况：1.都得到，2.只有一个得到。
					m_mainRecWriter.bNeedNewFile = (!bGetMainFile);
					m_reduRecWriter.bNeedNewFile = (!bGetReduFile);
					m_mainRecWriter.bOpen = bGetMainFile;
					m_reduRecWriter.bOpen = bGetReduFile;
				}
			}
			else
			{
				//已经有可以写数据的文件了
			}
		}
		else if ((m_mainRecWriter.bNeedNewFile != m_reduRecWriter.bNeedNewFile))
		{
			if (m_mainRecWriter.bNeedNewFile)
			{
				bool bGetMainFile = GetFile(m_mainRecWriter.fileInfo.chnn, m_mainRecWriter.fileInfo.diskIndex, m_mainRecWriter.fileInfo.fileIndex, NET_DISK_READ_WRITE, &(m_mainRecWriter.recWriter));
				if (bGetMainFile)
				{
					m_mainRecWriter.bNeedNewFile = (!bGetMainFile);
					m_mainRecWriter.bOpen = bGetMainFile;
					
					//需要停冗余录像，为了使主录像和冗余录像同时打开和关闭文件。
					assert(false == m_reduRecWriter.bNeedNewFile);
					assert(true == m_reduRecWriter.bOpen);
					if (m_reduRecWriter.bOpen)
					{
						if (NULL != m_reduRecWriter.pRecWriter)
						{
							m_reduRecWriter.pRecWriter->Close();
						}						
						m_reduRecWriter.bNeedNewFile = false;
						m_reduRecWriter.bOpen = false;
						WriteReclogEnd(m_reduRecWriter.fileInfo, m_reduRecWriter.startType);//把日志写完整。
						CancelFileIndex(m_reduRecWriter.fileInfo);
						m_reduRecWriter.startType = RECORD_TYPE_NULL;
					}
					bool bGetReduFile = GetFile(m_reduRecWriter.fileInfo.chnn, m_reduRecWriter.fileInfo.diskIndex, m_reduRecWriter.fileInfo.fileIndex, NET_DISK_REDUNDANCY, m_reduRecWriter.pRecWriter);
					m_reduRecWriter.bNeedNewFile = (!bGetReduFile);		
					m_reduRecWriter.bOpen = bGetReduFile;

					m_currType |= m_startType;
					m_startType = RECORD_TYPE_NULL;

					if (NULL != m_pMsgMan)
					{
						m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
					}
				}
				else
				{
					//失败，就等下进来时继续获取文件，
				}
			}
			else
			{
				assert(m_reduRecWriter.bNeedNewFile);
				bool bGetReduFile = GetFile(m_reduRecWriter.fileInfo.chnn, m_reduRecWriter.fileInfo.diskIndex, m_reduRecWriter.fileInfo.fileIndex, NET_DISK_REDUNDANCY, m_reduRecWriter.pRecWriter);
				if (bGetReduFile)
				{					
					m_reduRecWriter.bNeedNewFile = (!bGetReduFile);
					m_reduRecWriter.bOpen = bGetReduFile;
					//停主录像,为了使主录像和冗余录像同时打开和关闭文件。
					assert(false == m_mainRecWriter.bNeedNewFile);
					assert(true == m_mainRecWriter.bOpen);
					if (m_mainRecWriter.bOpen)
					{
						m_mainRecWriter.recWriter.Close();						
						m_mainRecWriter.bNeedNewFile = false;
						m_mainRecWriter.bOpen = false;
						WriteReclogEnd(m_mainRecWriter.fileInfo, m_mainRecWriter.startType);//把日志写完整。
						CancelFileIndex(m_mainRecWriter.fileInfo);
						m_mainRecWriter.startType = RECORD_TYPE_NULL;
					}
					bool bGetMainFile = GetFile(m_mainRecWriter.fileInfo.chnn, m_mainRecWriter.fileInfo.diskIndex, m_mainRecWriter.fileInfo.fileIndex, NET_DISK_READ_WRITE, &(m_mainRecWriter.recWriter));
					m_mainRecWriter.bNeedNewFile = (!bGetMainFile);
					m_mainRecWriter.bOpen = bGetMainFile;

					m_currType |= m_startType;
					m_startType = RECORD_TYPE_NULL;
					if (NULL != m_pMsgMan)
					{
						m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
					}
				}
				else
				{
					//失败，就等下进来时继续获取文件，
				}
			}
		}
	}
	else
	{
		assert(false);
	}



	/*
	检查是否需要关键帧。
	当需要关键帧时，连音频也给过滤掉。
	输出需要关键帧。
	因为一切正常，所以返回成功，表示无异常情况出现。
	*/
	{
#if 0
		if (m_bNeedKeyFrame)
		{
			if ((FRAME_TYPE_VIDEO != frameInfo.GetFrameInfo().frameType) || !frameInfo.GetFrameInfo().keyFrame)
			{
				bKeyFrame = true;
				return RECORD_WRITE_SUCC;
			}

			m_bNeedKeyFrame = false;	//转换状态
		}
#endif
	}

	/*
	预录像数据处理，把数据写入文件。
	1、先检查预录像列表里是否有数据，有则以帧为单位写入文件，每写入成功一帧，则将其从预录像列表删除。
	2、如果预录像列表为空或者已经全部写入文件，则把当前帧写入文件。一旦写失败则把当前帧加入预录像列表，
	以免丢帧。
	3、如果写成功，则检查是否需要写日志，需要则写日志。
	4、如果写失败了，则关闭当前文件并把日志写完整后。
	*/
	{
		bool bWriteMainFile = false;
		bool bWriteReduFile = false;
		if (m_mainRecWriter.bAction && !m_reduRecWriter.bAction)
		{
			assert(false == m_mainRecWriter.bNeedNewFile); //已经打开录像文件了
			assert(true == m_mainRecWriter.bOpen);
			bWriteMainFile = true;
			bWriteReduFile = false;
		}
		else if (m_mainRecWriter.bAction && m_reduRecWriter.bAction)
		{
			assert((false == m_mainRecWriter.bNeedNewFile) || (false == m_reduRecWriter.bNeedNewFile));
			assert((true == m_mainRecWriter.bOpen) || (true	== m_reduRecWriter.bOpen));
			bWriteMainFile = m_mainRecWriter.bOpen;
			bWriteReduFile = m_reduRecWriter.bOpen;
		}
		

		CFrameData *pFrameData = NULL;
		bool bHead = true;
		do 
		{
			unsigned long mainRecRet = WRITE_FRAME_SUCC;
			unsigned long reduRecRet = WRITE_FRAME_SUCC;			

			//遍历列表，为空意味着要么没有数据，要么已经处理完，马上需要把当前帧写入，然后才能返回。
			if (PRERECORD_MODE_DISK == m_preRecordType)
			{
				if (!bHead)
				{
					if (m_preDiskRecord.CurKeyFrame())
					{
						m_preDiskRecord.AddFrame(&frameInfo, bKeyFrame, false);
						return RECORD_WRITE_SUCC;													
					}
					else
					{
						pFrameData = m_preDiskRecord.GetFrame();
					}
				}
				else
				{
					pFrameData = m_preDiskRecord.GetFrame();
				}

				bHead = false;
			}
			else
			{ 
				pFrameData = m_preMemRecord.GetFrame();
			}
			
			if (NULL != pFrameData)
			{
				if (bWriteMainFile)
				{
					mainRecRet = m_mainRecWriter.recWriter.WriteData(pFrameData->GetFrameInfo());
				}

				if (bWriteReduFile)
				{
					if (NULL != m_reduRecWriter.pRecWriter)
					{
						reduRecRet = m_reduRecWriter.pRecWriter->WriteData(pFrameData->GetFrameInfo());
					}					
				}
			}
			else
			{
				if (bWriteMainFile)
				{
					mainRecRet = m_mainRecWriter.recWriter.WriteData(frameInfo.GetFrameInfo());			
				}
				if (bWriteReduFile)
				{
					if (NULL != m_reduRecWriter.pRecWriter)
					{
						reduRecRet = m_reduRecWriter.pRecWriter->WriteData(frameInfo.GetFrameInfo());
					}					
				}
			}

			if (bWriteMainFile)
			{
				if ((WRITE_FRAME_NEED_VIDEO_FORMAT == mainRecRet) || (WRITE_FRAME_NEED_AUDIO_FORMAT == mainRecRet))
				{
					if (WRITE_FRAME_NEED_VIDEO_FORMAT == mainRecRet)
					{
						FRAME_INFO_EX info = frameInfo.GetFrameInfo();
						info.frameType	= FRAME_TYPE_VIDEO_FORMAT;
						info.pData	= reinterpret_cast<unsigned char *>(m_pBitmapInfo);
						info.length	= sizeof(BITMAPINFOHEADER);

						mainRecRet = m_mainRecWriter.recWriter.WriteData(info);
					}
					else
					{
						FRAME_INFO_EX info = frameInfo.GetFrameInfo();
						info.frameType	= FRAME_TYPE_AUDIO_FORMAT;
						info.pData	= reinterpret_cast<unsigned char *>(m_pWaveFormat);
						info.length	= sizeof(WAVEFORMATEX);

						mainRecRet = m_mainRecWriter.recWriter.WriteData(info);
					}

					//再写一次数据
					if (NULL != pFrameData)
					{
						mainRecRet = m_mainRecWriter.recWriter.WriteData(pFrameData->GetFrameInfo());
					}
					else
					{
						mainRecRet = m_mainRecWriter.recWriter.WriteData(frameInfo.GetFrameInfo());
					}
				}

			}

			if (bWriteReduFile)
			{
				if ((WRITE_FRAME_NEED_VIDEO_FORMAT == reduRecRet) || (WRITE_FRAME_NEED_AUDIO_FORMAT == reduRecRet))
				{
					if (WRITE_FRAME_NEED_VIDEO_FORMAT == reduRecRet)
					{
						FRAME_INFO_EX info = frameInfo.GetFrameInfo();
						info.frameType	= FRAME_TYPE_VIDEO_FORMAT;
						info.pData	= reinterpret_cast<unsigned char *>(m_pBitmapInfo);
						info.length	= sizeof(BITMAPINFOHEADER);

						if (NULL != m_reduRecWriter.pRecWriter)
						{
							reduRecRet = m_reduRecWriter.pRecWriter->WriteData(info);
						}						
					}
					else
					{
						FRAME_INFO_EX info = frameInfo.GetFrameInfo();
						info.frameType	= FRAME_TYPE_AUDIO_FORMAT;
						info.pData	= reinterpret_cast<unsigned char *>(m_pWaveFormat);
						info.length	= sizeof(WAVEFORMATEX);

						if (NULL != m_reduRecWriter.pRecWriter)
						{
							reduRecRet = m_reduRecWriter.pRecWriter->WriteData(info);
						}						
					}

					//再写一次数据
					if (NULL != pFrameData)
					{
						if (NULL != m_reduRecWriter.pRecWriter)
						{
							reduRecRet = m_reduRecWriter.pRecWriter->WriteData(pFrameData->GetFrameInfo());
						}						
					}
					else
					{
						if (NULL != m_reduRecWriter.pRecWriter)
						{
							reduRecRet = m_reduRecWriter.pRecWriter->WriteData(frameInfo.GetFrameInfo());
						}						
					}
				}
			}

			//有个录像写失败，则需要把两个录像都停掉。目的是：为了使主录像和冗余录像同时打开和关闭文件。
			if ((WRITE_FRAME_FULL == mainRecRet)
			  ||(WRITE_FRAME_FAIL == mainRecRet)
			  ||(WRITE_FRAME_FULL == reduRecRet)
			  ||(WRITE_FRAME_FAIL == reduRecRet))
			{
				if (bWriteMainFile)
				{
					m_mainRecWriter.recWriter.Close();					
					m_mainRecWriter.bNeedNewFile = true;
					m_mainRecWriter.bOpen = false;
					WriteReclogEnd(m_mainRecWriter.fileInfo, m_mainRecWriter.startType);//把日志写完整。
					CancelFileIndex(m_mainRecWriter.fileInfo);//CancelFileIndex一定要放到WriteReclogEnd后面调用。
					                                          //原因：WriteReclogEnd函数写的是段结束信息。CancelFileIndex写的是文件结束信息。
					                                          //要先写完段信息之后，再写文件结束
					m_mainRecWriter.startType = RECORD_TYPE_NULL;
				}

				if (bWriteReduFile)
				{
					if (NULL != m_reduRecWriter.pRecWriter)
					{
						m_reduRecWriter.pRecWriter->Close();					
					}					
					m_reduRecWriter.bNeedNewFile = true;
					m_reduRecWriter.bOpen = false;
					WriteReclogEnd(m_reduRecWriter.fileInfo, m_reduRecWriter.startType);//把日志写完整。
					CancelFileIndex(m_reduRecWriter.fileInfo);
					m_reduRecWriter.startType = RECORD_TYPE_NULL;
				}

				m_currType |= m_startType;			//转换到需要写日志的状态。
				m_startType = RECORD_TYPE_NULL;		//当前录像状态转入非录像状态。
				if (NULL != m_pMsgMan)
				{
					m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
				}
								
				//有个录像写失败，则将最近送入的数据帧加入预录像列表。
				//保证换文件后，能保持一致。
				if (PRERECORD_MODE_DISK == m_preRecordType)
				{
					if (m_preDiskRecord.AddFrame(&frameInfo, bKeyFrame))
					{
						return RECORD_WRITE_SUCC;
					}	
				}
				else
				{
					if (m_preMemRecord.AddFrame(&frameInfo, bKeyFrame))
					{
						return RECORD_WRITE_SUCC;
					}	
				}
				
				return RECORD_WRITE_FAIL;
			}

			//写成功，则记录当前视频帧的时间
			bool bVideoFrame = false;
			if ((WRITE_FRAME_SUCC == mainRecRet)
			  &&(WRITE_FRAME_SUCC == reduRecRet))
			{
				if (NULL != pFrameData)
				{
					if (FRAME_TYPE_VIDEO == pFrameData->GetFrameInfo().frameType)
					{
						assert (m_lastFrameTime <= pFrameData->GetFrameInfo().time);
						m_lastFrameTime = pFrameData->GetFrameInfo().time;
						bVideoFrame = true;
					}
				}
				else
				{
					if (FRAME_TYPE_VIDEO == frameInfo.GetFrameInfo().frameType)
					{

						assert (m_lastFrameTime <= frameInfo.GetFrameInfo().time);
						m_lastFrameTime = frameInfo.GetFrameInfo().time;
						bVideoFrame = true;
					}
				}
			}

			//写成功，且数据来自预录像列表，则从列表清楚该数据。
			if (NULL != pFrameData)
			{		
				if (PRERECORD_MODE_DISK == m_preRecordType)
				{
					m_preDiskRecord.RemoveHead();
				}
				else
				{
					m_preMemRecord.RemoveHead();
				}
			}

			/*
			写入日志。
			如果是不需要写日志状态，则不做任何处理。
			1、如果写入失败，则关闭当前文件，并重新获取文件后再次尝试写入日志。再次失败则意味着异常。
			2、如果写入成功，则往后返回成功。
			*/
			//modify 2010-01-20 14:32 xwm 
			//原来的做法为：if (RECORD_TYPE_ALL & m_currType) 这样做带来的问题是：
			//如果一开始m_lastFrameTime为0，而我们对于m_lastFrameTime的做法是：只有在写入成功的视频帧后，才会修改m_lastFrameTime的值。
			//而这时刚刚写入成功是音频帧，那么m_lastFrameTime还是0，而if (RECORD_TYPE_ALL & m_currType)不会阻拦我们去写WriteReclogStart, 必然就会带来下面的问题：
			//ReclogEx.cpp: 1444: bool CReclogEx::WriteReclogStart(short unsigned int, long unsigned int, RECORD_TYPE, long unsigned int): Assertion `0 < startTime' failed.
            //修改方法：只有在写入成功的视频帧后才去写日志
			if (bVideoFrame && (RECORD_TYPE_ALL & m_currType))
			{
				bool bWriteMainLog = true;
				bool bWriteReduLog = true;
				if (bWriteMainFile)
				{
					bWriteMainLog = WriteReclogStart(m_mainRecWriter.fileInfo, m_currType, m_lastFrameTime, m_mainRecWriter.startType);
					m_mainRecWriter.recWriter.FFlush(); //由于recWrite采用缓存，这样就会导致，reclog中写开始时间的帧，有可能没有刷到硬盘里
					                                    //而是放到缓存里，这样就会导致reclog中记录了的时间却在录像数据里没有。
				}
				if (bWriteReduFile)
				{
					bWriteReduLog = WriteReclogStart(m_reduRecWriter.fileInfo, m_currType, m_lastFrameTime, m_reduRecWriter.startType);
					if (NULL != m_reduRecWriter.pRecWriter)
					{
						m_reduRecWriter.pRecWriter->FFlush();
					}					
				}

				if (bWriteMainLog && bWriteReduLog)
				{
					m_startType |= m_currType;
					m_currType = RECORD_TYPE_NULL;	
					if (NULL != m_pMsgMan)
					{
						m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
					}
				}				
				else
				{
					//因为日志写满了导致写日志失败，则换文件保证下次再次写入。
					if (bWriteMainFile)
					{
						m_mainRecWriter.recWriter.Close();						
						m_mainRecWriter.bNeedNewFile = true;
						m_mainRecWriter.bOpen = false;
						WriteReclogEnd(m_mainRecWriter.fileInfo, m_mainRecWriter.startType);
						CancelFileIndex(m_mainRecWriter.fileInfo);
						m_mainRecWriter.startType = RECORD_TYPE_NULL;
					}

					if (bWriteReduFile)
					{
						if (NULL != m_reduRecWriter.pRecWriter)
						{
							m_reduRecWriter.pRecWriter->Close();					
						}						
						m_reduRecWriter.bNeedNewFile = true;
						m_reduRecWriter.bOpen = false;
						WriteReclogEnd(m_reduRecWriter.fileInfo, m_reduRecWriter.startType);
						CancelFileIndex(m_reduRecWriter.fileInfo);
						m_reduRecWriter.startType = RECORD_TYPE_NULL;
					}

					//要转换状态，以备新文件打开后写开始。
					
					m_currType |= m_startType;
					m_startType = RECORD_TYPE_NULL;
					if (NULL != m_pMsgMan)
					{
						m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
					}
					//需要跳出循环，否则会再次进入（pFrameData = m_preRecord.GetFrame();），但是
					//此时文件已经关闭，玩里面写东西会触发断言。
					break;
				}
			}

		} while(NULL != pFrameData);
	}

	return RECORD_WRITE_SUCC;
}

bool CRecordProc::Record(RECORD_TYPE type)
{
	assert(RECORD_TYPE_END > type);

	//接StopRecord()接口的描述,因为有m_startType |= m_currType,所以如果再次开启该种类型的录像,因为
	//(m_startType & type)条件满足,则新的一段该类型的录像将不能成功开启,所以就出现了明明停止了录像,
	//但是录像状态仍然处于开启状态,且不能再停止和再开启的现象.可以通过快速点击手动录像来重现。

	//解决办法：开始时，加入一个判断条件，(m_currType & type)，如果其为真，则表示上述情况出现了，
	//如果在停止时处理了该情况，那么该情况是不允许在再次开启时出现，所以使用断言来保障这一点。

	if ((m_startType & type) || (m_currType & type))	//该种类型已经开启过
	{
		return false;
	}
	else
	{
		m_currType |= type;
		return true;
	}
}

bool CRecordProc::StopRecord(RECORD_TYPE type)
{
	assert(RECORD_TYPE_END > type);

	if  (m_currType & type)
	{
		m_currType &= (~type);//只能把对应的类型去掉，其他没有停止的不能修改

		//如果录像全部停止就刷新缓冲的录像数据到磁盘中
		if (RECORD_TYPE_NULL == m_currType)
		{
			FlushRecDataToDisk();
		}

		return true;
	}

	if (m_startType & type)
	{
		assert(NULL != m_pReclogManEx);
		/*PrintfBit64(m_lastFrameTime, false);
		PrintfBit64(GetCurrTime(), false);
		PrintfBit32Time(GetCurrTime32());*/
		assert((0 < m_lastFrameTime) && (m_lastFrameTime <= GetCurrTime()));
		bool bWriteMainLog = true;
		bool bWriteReduLog = true;
		assert((true == m_mainRecWriter.bOpen) || (true == m_reduRecWriter.bOpen));
		if (m_mainRecWriter.bAction)
		{
			if (m_mainRecWriter.bOpen)
			{
				m_mainRecWriter.fileInfo.type = type;
				m_mainRecWriter.fileInfo.endTime = (unsigned long)(m_lastFrameTime/1000000);
                bWriteMainLog = m_pReclogManEx->WriteLogEnd(m_mainRecWriter.fileInfo);
				m_mainRecWriter.recWriter.FFlush();
				m_mainRecWriter.startType &= (~type);

				RUN_LOG(RUN_LOG_ITEM_REC_WRITE_END, CRunLog::RUN_Time("The time is:", m_mainRecWriter.fileInfo.endTime), CRunLog::RUN_Num("chnn:", m_mainRecWriter.fileInfo.chnn), "write reclog end");
			}
		}

		if (m_reduRecWriter.bAction)
		{
			if (m_reduRecWriter.bOpen)
			{
				m_reduRecWriter.fileInfo.type = type;
				m_reduRecWriter.fileInfo.endTime = (unsigned long)(m_lastFrameTime/1000000);
				bWriteReduLog = m_pReclogManEx->WriteLogEnd(m_reduRecWriter.fileInfo);
				if (NULL != m_reduRecWriter.pRecWriter)
				{
					m_reduRecWriter.pRecWriter->FFlush();
				}				
				m_reduRecWriter.startType &= (~type);

				RUN_LOG(RUN_LOG_ITEM_REC_WRITE_END, CRunLog::RUN_Time("The time is:", m_reduRecWriter.fileInfo.endTime), CRunLog::RUN_Num("chnn:", m_reduRecWriter.fileInfo.chnn), "write reclog end");
			}
		}
        
		//如果写失败，表示硬盘有问题。
		assert(bWriteMainLog && bWriteReduLog);
        m_startType &= (~type);	
		if (NULL != m_pMsgMan)
		{
			m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
		}

		//如果录像全部停止就刷新缓冲的录像数据到磁盘中
		if (RECORD_TYPE_NULL == m_startType)
		{
			FlushRecDataToDisk();
		}

		return true;
	}
	else
	{
		return false;
	}
}

void CRecordProc::FlushRecDataToDisk()
{
	if (m_reduRecWriter.bOpen)
	{
		m_reduRecWriter.pRecWriter->StopRecrod();
	}

	m_mainRecWriter.recWriter.StopRecrod();
}

void CRecordProc::RemoveData()
{
	if (PRERECORD_MODE_DISK == m_preRecordType)
	{
		m_preDiskRecord.RemoveAll();
	}
	else
	{
		m_preMemRecord.RemoveAll();
	}	
}

void CRecordProc::SetPreRecTime(unsigned long time)
{
	if (PRERECORD_MODE_DISK == m_preRecordType)
	{
		m_preDiskRecord.SetPreRecTime(time);
	}
	else
	{
		m_preMemRecord.SetPreRecTime(time);
	}	
}

void CRecordProc::SetPreRecDataLen(unsigned long maxPreRecDataLen)
{
	if(PRERECORD_MODE_MEMORY == m_preRecordType)
	{
		m_preMemRecord.SetPreRecDataLen(maxPreRecDataLen);
	}
}

void CRecordProc::RecoverPreDiskRecTime()
{
	if (PRERECORD_MODE_DISK == m_preRecordType)
	{
		m_preDiskRecord.RecoverPreRecTime();
	}
	
}

void CRecordProc::SetRecycleRecord(bool bEnable)
{
	m_bRecycleRecord = bEnable;
	m_pReclogManEx->SetRecycleRecord(m_bRecycleRecord);
}

void CRecordProc::SetRedundancyRecord(bool bEnable)
{
	if (!CProduct::Instance()->SupportRedunRec())
	{
		return;
	}
	//assert(m_reduRecWriter.bAction != bEnable);
	assert(m_mainRecWriter.bAction);
	if (bEnable == m_reduRecWriter.bAction)
	{
		return ;
	}

	if (bEnable)
	{
		if (m_mainRecWriter.bAction)
		{
			if (m_mainRecWriter.bOpen)
			{
				assert(false == m_mainRecWriter.bNeedNewFile);
				m_mainRecWriter.recWriter.Close();				
				m_mainRecWriter.bNeedNewFile = true;
				m_mainRecWriter.bOpen = false;
				WriteReclogEnd(m_mainRecWriter.fileInfo, m_mainRecWriter.startType);
				CancelFileIndex(m_mainRecWriter.fileInfo);
				m_mainRecWriter.startType = RECORD_TYPE_NULL;
				m_currType |= m_startType;
				m_startType = RECORD_TYPE_NULL;
				if (NULL != m_pMsgMan)
				{
					m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
				}
			}
		}
		m_reduRecWriter.bAction = bEnable;
		assert(m_reduRecWriter.bNeedNewFile);
		assert(false == m_reduRecWriter.bOpen);
		m_reduRecWriter.bNeedNewFile = true;
		m_reduRecWriter.bOpen = false;
		m_reduRecWriter.startType = RECORD_TYPE_NULL;
	}
	else
	{
		if (m_mainRecWriter.bAction)
		{
			if (m_mainRecWriter.bOpen)
			{
				assert(false == m_mainRecWriter.bNeedNewFile);
				m_mainRecWriter.recWriter.Close();				
				m_mainRecWriter.bNeedNewFile = true;
				m_mainRecWriter.bOpen = false;
				WriteReclogEnd(m_mainRecWriter.fileInfo, m_mainRecWriter.startType);
				CancelFileIndex(m_mainRecWriter.fileInfo);
				m_mainRecWriter.startType = RECORD_TYPE_NULL;
			}
		}
        
		assert(m_reduRecWriter.bAction);
		if (m_reduRecWriter.bAction)
		{
			if (m_reduRecWriter.bOpen)
			{
				assert(false == m_reduRecWriter.bNeedNewFile);
				if (NULL != m_reduRecWriter.pRecWriter)
				{
					m_reduRecWriter.pRecWriter->Close();					
				}				
				m_reduRecWriter.bNeedNewFile = true;
				m_reduRecWriter.bOpen = false;
				WriteReclogEnd(m_reduRecWriter.fileInfo, m_reduRecWriter.startType);
				CancelFileIndex(m_reduRecWriter.fileInfo);
				m_reduRecWriter.startType = RECORD_TYPE_NULL;
			}
		}

		m_currType |= m_startType;
		m_startType = RECORD_TYPE_NULL;
		if (NULL != m_pMsgMan)
		{
			m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
		}

		m_reduRecWriter.bAction = bEnable;
		assert(m_reduRecWriter.bNeedNewFile);
		assert(false == m_reduRecWriter.bOpen);
		m_reduRecWriter.bNeedNewFile = true;
		m_reduRecWriter.bOpen = false;
		m_reduRecWriter.startType = RECORD_TYPE_NULL;
	}
	
}
//protected

//private
void CRecordProc::WriteReclogEnd(FILE_INFO_EX &fileInfo, unsigned long startType)
{
	fileInfo.endTime = (unsigned long)(m_lastFrameTime/1000000);
	RUN_LOG(RUN_LOG_ITEM_REC_WRITE_END, CRunLog::RUN_Time("The time is:", fileInfo.endTime), CRunLog::RUN_Num("chnn:", fileInfo.chnn), "write reclog end");

	
	if (RECORD_TYPE_MANUAL & startType)
	{
		fileInfo.type = RECORD_TYPE_MANUAL;
		bool bSuccess = m_pReclogManEx->WriteLogEnd(fileInfo);	
		assert(bSuccess);
	}

	if (RECORD_TYPE_SCHEDULE & startType)
	{
		fileInfo.type = RECORD_TYPE_SCHEDULE;
		bool bSuccess = m_pReclogManEx->WriteLogEnd(fileInfo);
		assert(bSuccess);
	}

	if (RECORD_TYPE_MOTION & startType)
	{
		fileInfo.type = RECORD_TYPE_MOTION;
		bool bSuccess = m_pReclogManEx->WriteLogEnd(fileInfo);
		assert(bSuccess);
	}

	if (RECORD_TYPE_SENSOR & startType)
	{
		fileInfo.type = RECORD_TYPE_SENSOR;
		bool bSuccess = m_pReclogManEx->WriteLogEnd(fileInfo);		
		assert(bSuccess);
	}
}

bool CRecordProc::WriteReclogStart(FILE_INFO_EX &fileInfo, unsigned char type, LONGLONG startTime, unsigned long &startType)
{
	RUN_LOG(RUN_LOG_ITEM_REC_WRITE_START, CRunLog::RUN_Time("The time is:", startTime), CRunLog::RUN_Num("chnn:", fileInfo.chnn), "write reclog start");

	unsigned long  falseTimes = 0;
	fileInfo.startTime = (unsigned long)(startTime/1000000);	
	
	if (RECORD_TYPE_MANUAL & type)
	{
		fileInfo.type = RECORD_TYPE_MANUAL;
		if (!m_pReclogManEx->WriteLogStart(fileInfo))
		{
			++ falseTimes;
		}
		else
		{
			assert (!(RECORD_TYPE_MANUAL & startType));
			startType |= RECORD_TYPE_MANUAL;
		}
	}

	if (RECORD_TYPE_SCHEDULE & type)
	{
		fileInfo.type = RECORD_TYPE_SCHEDULE;
		if (!m_pReclogManEx->WriteLogStart(fileInfo))
		{
			++ falseTimes;
		}
		else
		{
			assert (!(RECORD_TYPE_SCHEDULE & startType));
			startType |= RECORD_TYPE_SCHEDULE;
		}
	}

	if (RECORD_TYPE_MOTION & type)
	{
		fileInfo.type = RECORD_TYPE_MOTION;
		if (!m_pReclogManEx->WriteLogStart(fileInfo))
		{
			++ falseTimes;
		}
		else
		{
			assert (!(RECORD_TYPE_MOTION & startType));
			startType |= RECORD_TYPE_MOTION;
		}
	}

	if (RECORD_TYPE_SENSOR & type)
	{
		fileInfo.type = RECORD_TYPE_SENSOR;
		if (!m_pReclogManEx->WriteLogStart(fileInfo))
		{
			++ falseTimes;
		}
		else
		{
			assert (!(RECORD_TYPE_SENSOR & startType));
			startType |= RECORD_TYPE_SENSOR;
		}
	}

	return (falseTimes > 0) ? false : true;
	return false;
}

#if defined(__PRIMARY_WRDISK__)
bool CRecordProc::GetFile(unsigned long channel, unsigned short &diskIndex, unsigned short &fileIndex, unsigned char type, CPrimaryRecWriter *recWriter)
#else
bool CRecordProc::GetFile(unsigned long channel, unsigned short &diskIndex, unsigned short &fileIndex, unsigned char type, CRecWriter *recWriter)
#endif
{
	if (NULL == recWriter)
	{
		return false;
	}

	assert(m_channel == channel);
	bool bRet = false;
	do 
	{
		bRet = m_pReclogManEx->GetInvalidFile(channel, diskIndex, fileIndex, type);
		if (bRet)
		{
			break;
		}

		if (m_bRecycleRecord)
		{
			FILE_INFO_EX fileInfoTmp;
			if (!m_pReclogManEx->FindRecycleFile(fileInfoTmp, type))
			{
				break;
			}		
			if (!m_pReclogManEx->DeleteFile(fileInfoTmp))
			{
				break;
			}
			if (!m_pReclogManEx->GetInvalidFile(channel, diskIndex, fileIndex, type))
			{
				break;
			}
			bRet = true;
		}
	} while(0);
	
	if (!bRet)
	{
		return false;
	}
	
	//打开文件
	char path [512] = {0};
#ifdef __ENVIRONMENT_WIN32__
	if (1 < strlen(m_pFilePath))
	{
		sprintf(path, "%s\\%02d\\%08d.dat", m_pFilePath, diskIndex, fileIndex);
	}
	else
	{
		assert (1 == strlen(m_pFilePath));
		sprintf(path, "%c:\\%08d.dat", m_pFilePath[0]+diskIndex, fileIndex);
	}
#else
	sprintf(path, "%s/%02d/%08d.dat", m_pFilePath, diskIndex, fileIndex);
#endif

	bRet = recWriter->Open(path,MAX_RECORD_FILE_LEN_M,false);

	if (bRet)
	{
		RUN_LOG(RUN_LOG_ITEM_REC_OPEN_FILE_SUCC, CRunLog::RUN_Num("The type is:", type), CRunLog::RUN_Num("chnn:", channel), path);

		//打开成功后，注册到链表中
		{
			FILE_INFO_EX temp;
			temp.chnn = channel;
			temp.diskIndex = diskIndex;
			temp.fileIndex = fileIndex;
			RegisterFileIndex(temp);
		}

		//add 2010-01-20 xwm 
		//在DateProc函数中对音视频格式帧的处理为：如果当前写入的是视频帧，但是发现没有写过视频格式帧，那么反回需要视频格式帧。
		//如果当前写入的是音频帧，但是发现没有写过音频格式帧，那么返回需要音频格式帧。
		//DateProc的做法没有错误。我们在打开成功后就写入音视频格式。这样做的原因：只是保证写入录像文件的前两帧为音视频格式帧。
		//modify 2010-01-26 xwm 打开成功后就写入音视频格式方式会带来其它的问题：在没有录音频的情况下，客户备份正在录像的文件，会宕机。
		/*
		if (m_pBitmapInfo != NULL)
		{
			FRAME_INFO_EX info;
			info.channel = channel;
			info.frameType = FRAME_TYPE_VIDEO_FORMAT;
			info.pData = reinterpret_cast<unsigned char *>(m_pBitmapInfo);
			info.length	= sizeof(BITMAPINFOHEADER);
			recWriter.WriteData(info);
		}
		if (m_pWaveFormat != NULL)
		{
			FRAME_INFO_EX info;
			info.channel = channel;
			info.frameType = FRAME_TYPE_AUDIO_FORMAT;
			info.pData = reinterpret_cast<unsigned char *>(m_pWaveFormat);
			info.length	= sizeof(WAVEFORMATEX);
			recWriter.WriteData(info);
		}
		//end
		*/
	}
	else
	{
		RUN_LOG(RUN_LOG_ITEM_REC_OPEN_FILE_FAIL, CRunLog::RUN_Num("The type is:", type), CRunLog::RUN_Num("chnn:", channel), path);
		assert(false);//为了发现未打开文件的情况
	}	

	return bRet;
}


void CRecordProc::RegisterFileIndex(FILE_INFO_EX &fileInfo)
{
	m_pReclogManEx->RegisterRecordFile(fileInfo.chnn, fileInfo.diskIndex, fileInfo.fileIndex);

}

void CRecordProc::CancelFileIndex(FILE_INFO_EX &fileInfo)
{
	m_pReclogManEx->CancelRecordFile(fileInfo.chnn, fileInfo.diskIndex, fileInfo.fileIndex);
}


void CRecordProc::ResetPreDiskRecordPath(int prerecordIndex, bool &bPreRecord)
{
	if (PRERECORD_MODE_DISK == m_preRecordType)
	{
		if (-1 == prerecordIndex)
		{
			m_preDiskRecord.ResetPreRecordPath(NULL, bPreRecord);
		}
		else
		{
			char szPrerecordPath[64] = {0};
			snprintf(szPrerecordPath, 64, "%s/%02d", m_pFilePath, prerecordIndex);
			m_preDiskRecord.ResetPreRecordPath(szPrerecordPath, bPreRecord);
		}	
	}	
}
//end


