/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
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
	
	//������DateProc�в��õ��ǣ��á�m_pBitmapInfo�Ƿ�Ϊ�ա� ��Ϊ ���Ƿ�����Ƶ��ʽ�����ж�������
	//���������Ҫ��Ƶ��ʽ����Ҫdelete m_pBitmapInfo;
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
	������Ƶ����Ƶ��ʽ��
	���ԭ��û�и�ʽ���ҵ�ǰ�����Ϊ�Ƕ�Ӧ��ʽ���򷵻��޸�ʽ��ʾ��
	���ԭ���и�ʽ���ҵ�ǰ�����ʽ���ݣ���ɾ��ԭ���ĸ�ʽ���滻���µĸ�ʽ���ݡ�
	*/
	//modify 2010-1-27 xwm
	//�޸����⣺������IEʱ����ʽ��Ӳ�̻��������Ķ��ԣ�./td3520: AVDataSendMan.cpp: 521: long int CAVDataSendMan::PutAudioData(CFrameData*): Assertion `false' failed.
	//ԭ����Щͨ��û����Ƶ�����ǵĴ���ֻ��֤�˲�������Ƶ֡������û�б�֤��������Ƶ��ʽ��
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

				//���⴦��:С����¼��ֱ��ʱ仯ʱ�����л��ļ�
				if (((m_pBitmapInfo->biWidth != pBitMap->biWidth)||(m_pBitmapInfo->biHeight != pBitMap->biHeight))\
					 && m_channel<m_actualPrimaryStreamChnnNum)
				{
					//�������ݳ�����޸��˷ֱ��ʣ�����CIF�޸ĳ�D1��
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
	��������Ƿ�Ϸ�
	�����ʧ��Ƶ����Ƶ���ݣ�����Ƶ���ݵĳ�����ȷ����Ƶ��ʱû�а취���м�飬�Ժ���Կ����ó�������顣
	*/
	{
		assert((FRAME_TYPE_VIDEO == frameInfo.GetFrameInfo().frameType) || (FRAME_TYPE_AUDIO == frameInfo.GetFrameInfo().frameType));
		if ((FRAME_TYPE_VIDEO != frameInfo.GetFrameInfo().frameType) && (FRAME_TYPE_AUDIO != frameInfo.GetFrameInfo().frameType))
		{
			return RECORD_WRITE_FAIL;	
		}

		if (FRAME_TYPE_VIDEO == frameInfo.GetFrameInfo().frameType)
		{
			//���⴦��:С����¼��ֱ��ʱ仯ʱ�����л��ļ�
			if (((m_pBitmapInfo->biWidth != (long)frameInfo.GetFrameInfo().width) || (m_pBitmapInfo->biHeight != (long)frameInfo.GetFrameInfo().height))\
						&& m_channel<m_actualPrimaryStreamChnnNum)
			{
				//�������ݳ�����޸��˷ֱ��ʣ�����CIF�޸ĳ�D1��
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
	��鵱ǰ��¼��״̬��
	��¼��״̬�������Ԥ¼��
	*/
	{
		if ((RECORD_TYPE_NULL == m_startType) && (RECORD_TYPE_NULL == m_currType))
		{
			//����Ԥ¼���б�
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
	����Ƿ���Ҫ���µ��ļ���
	�ɹ����ļ�����������ߡ�
	���ؼ�֡���ڸò�����棬��Ϊ�����ڹؼ�֡��֮ǰ���ļ��������ǵ����ؼ�֡�ٻ�ʱ����ļ���
	���û�д��ļ�����ѵ�ǰ֡����Ԥ¼���б�������Ԥ¼��Ҫ��Ҳ���㻺��Ҫ����������֡��
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
					//�����������1.���õ���2.ֻ��һ���õ���
					m_mainRecWriter.bNeedNewFile = (!bGetMainFile);
					m_reduRecWriter.bNeedNewFile = (!bGetReduFile);
					m_mainRecWriter.bOpen = bGetMainFile;
					m_reduRecWriter.bOpen = bGetReduFile;
				}
			}
			else
			{
				//�Ѿ��п���д���ݵ��ļ���
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
					
					//��Ҫͣ����¼��Ϊ��ʹ��¼�������¼��ͬʱ�򿪺͹ر��ļ���
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
						WriteReclogEnd(m_reduRecWriter.fileInfo, m_reduRecWriter.startType);//����־д������
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
					//ʧ�ܣ��͵��½���ʱ������ȡ�ļ���
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
					//ͣ��¼��,Ϊ��ʹ��¼�������¼��ͬʱ�򿪺͹ر��ļ���
					assert(false == m_mainRecWriter.bNeedNewFile);
					assert(true == m_mainRecWriter.bOpen);
					if (m_mainRecWriter.bOpen)
					{
						m_mainRecWriter.recWriter.Close();						
						m_mainRecWriter.bNeedNewFile = false;
						m_mainRecWriter.bOpen = false;
						WriteReclogEnd(m_mainRecWriter.fileInfo, m_mainRecWriter.startType);//����־д������
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
					//ʧ�ܣ��͵��½���ʱ������ȡ�ļ���
				}
			}
		}
	}
	else
	{
		assert(false);
	}



	/*
	����Ƿ���Ҫ�ؼ�֡��
	����Ҫ�ؼ�֡ʱ������ƵҲ�����˵���
	�����Ҫ�ؼ�֡��
	��Ϊһ�����������Է��سɹ�����ʾ���쳣������֡�
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

			m_bNeedKeyFrame = false;	//ת��״̬
		}
#endif
	}

	/*
	Ԥ¼�����ݴ���������д���ļ���
	1���ȼ��Ԥ¼���б����Ƿ������ݣ�������֡Ϊ��λд���ļ���ÿд��ɹ�һ֡�������Ԥ¼���б�ɾ����
	2�����Ԥ¼���б�Ϊ�ջ����Ѿ�ȫ��д���ļ�����ѵ�ǰ֡д���ļ���һ��дʧ����ѵ�ǰ֡����Ԥ¼���б�
	���ⶪ֡��
	3�����д�ɹ��������Ƿ���Ҫд��־����Ҫ��д��־��
	4�����дʧ���ˣ���رյ�ǰ�ļ�������־д������
	*/
	{
		bool bWriteMainFile = false;
		bool bWriteReduFile = false;
		if (m_mainRecWriter.bAction && !m_reduRecWriter.bAction)
		{
			assert(false == m_mainRecWriter.bNeedNewFile); //�Ѿ���¼���ļ���
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

			//�����б�Ϊ����ζ��Ҫôû�����ݣ�Ҫô�Ѿ������꣬������Ҫ�ѵ�ǰ֡д�룬Ȼ����ܷ��ء�
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

					//��дһ������
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

					//��дһ������
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

			//�и�¼��дʧ�ܣ�����Ҫ������¼��ͣ����Ŀ���ǣ�Ϊ��ʹ��¼�������¼��ͬʱ�򿪺͹ر��ļ���
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
					WriteReclogEnd(m_mainRecWriter.fileInfo, m_mainRecWriter.startType);//����־д������
					CancelFileIndex(m_mainRecWriter.fileInfo);//CancelFileIndexһ��Ҫ�ŵ�WriteReclogEnd������á�
					                                          //ԭ��WriteReclogEnd����д���Ƕν�����Ϣ��CancelFileIndexд�����ļ�������Ϣ��
					                                          //Ҫ��д�����Ϣ֮����д�ļ�����
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
					WriteReclogEnd(m_reduRecWriter.fileInfo, m_reduRecWriter.startType);//����־д������
					CancelFileIndex(m_reduRecWriter.fileInfo);
					m_reduRecWriter.startType = RECORD_TYPE_NULL;
				}

				m_currType |= m_startType;			//ת������Ҫд��־��״̬��
				m_startType = RECORD_TYPE_NULL;		//��ǰ¼��״̬ת���¼��״̬��
				if (NULL != m_pMsgMan)
				{
					m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
				}
								
				//�и�¼��дʧ�ܣ���������������֡����Ԥ¼���б�
				//��֤���ļ����ܱ���һ�¡�
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

			//д�ɹ������¼��ǰ��Ƶ֡��ʱ��
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

			//д�ɹ�������������Ԥ¼���б�����б���������ݡ�
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
			д����־��
			����ǲ���Ҫд��־״̬�������κδ���
			1�����д��ʧ�ܣ���رյ�ǰ�ļ��������»�ȡ�ļ����ٴγ���д����־���ٴ�ʧ������ζ���쳣��
			2�����д��ɹ��������󷵻سɹ���
			*/
			//modify 2010-01-20 14:32 xwm 
			//ԭ��������Ϊ��if (RECORD_TYPE_ALL & m_currType) �����������������ǣ�
			//���һ��ʼm_lastFrameTimeΪ0�������Ƕ���m_lastFrameTime�������ǣ�ֻ����д��ɹ�����Ƶ֡�󣬲Ż��޸�m_lastFrameTime��ֵ��
			//����ʱ�ո�д��ɹ�����Ƶ֡����ôm_lastFrameTime����0����if (RECORD_TYPE_ALL & m_currType)������������ȥдWriteReclogStart, ��Ȼ�ͻ������������⣺
			//ReclogEx.cpp: 1444: bool CReclogEx::WriteReclogStart(short unsigned int, long unsigned int, RECORD_TYPE, long unsigned int): Assertion `0 < startTime' failed.
            //�޸ķ�����ֻ����д��ɹ�����Ƶ֡���ȥд��־
			if (bVideoFrame && (RECORD_TYPE_ALL & m_currType))
			{
				bool bWriteMainLog = true;
				bool bWriteReduLog = true;
				if (bWriteMainFile)
				{
					bWriteMainLog = WriteReclogStart(m_mainRecWriter.fileInfo, m_currType, m_lastFrameTime, m_mainRecWriter.startType);
					m_mainRecWriter.recWriter.FFlush(); //����recWrite���û��棬�����ͻᵼ�£�reclog��д��ʼʱ���֡���п���û��ˢ��Ӳ����
					                                    //���Ƿŵ�����������ͻᵼ��reclog�м�¼�˵�ʱ��ȴ��¼��������û�С�
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
					//��Ϊ��־д���˵���д��־ʧ�ܣ����ļ���֤�´��ٴ�д�롣
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

					//Ҫת��״̬���Ա����ļ��򿪺�д��ʼ��
					
					m_currType |= m_startType;
					m_startType = RECORD_TYPE_NULL;
					if (NULL != m_pMsgMan)
					{
						m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
					}
					//��Ҫ����ѭ����������ٴν��루pFrameData = m_preRecord.GetFrame();��������
					//��ʱ�ļ��Ѿ��رգ�������д�����ᴥ�����ԡ�
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

	//��StopRecord()�ӿڵ�����,��Ϊ��m_startType |= m_currType,��������ٴο����������͵�¼��,��Ϊ
	//(m_startType & type)��������,���µ�һ�θ����͵�¼�񽫲��ܳɹ�����,���Ծͳ���������ֹͣ��¼��,
	//����¼��״̬��Ȼ���ڿ���״̬,�Ҳ�����ֹͣ���ٿ���������.����ͨ�����ٵ���ֶ�¼�������֡�

	//����취����ʼʱ������һ���ж�������(m_currType & type)�������Ϊ�棬���ʾ������������ˣ�
	//�����ֹͣʱ�����˸��������ô������ǲ��������ٴο���ʱ���֣�����ʹ�ö�����������һ�㡣

	if ((m_startType & type) || (m_currType & type))	//���������Ѿ�������
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
		m_currType &= (~type);//ֻ�ܰѶ�Ӧ������ȥ��������û��ֹͣ�Ĳ����޸�

		//���¼��ȫ��ֹͣ��ˢ�»����¼�����ݵ�������
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
        
		//���дʧ�ܣ���ʾӲ�������⡣
		assert(bWriteMainLog && bWriteReduLog);
        m_startType &= (~type);	
		if (NULL != m_pMsgMan)
		{
			m_pMsgMan->RefreshRecStatus(m_startType, (unsigned char)(CHANNEL_ID(m_channel)));
		}

		//���¼��ȫ��ֹͣ��ˢ�»����¼�����ݵ�������
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
	
	//���ļ�
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

		//�򿪳ɹ���ע�ᵽ������
		{
			FILE_INFO_EX temp;
			temp.chnn = channel;
			temp.diskIndex = diskIndex;
			temp.fileIndex = fileIndex;
			RegisterFileIndex(temp);
		}

		//add 2010-01-20 xwm 
		//��DateProc�����ж�����Ƶ��ʽ֡�Ĵ���Ϊ�������ǰд�������Ƶ֡�����Ƿ���û��д����Ƶ��ʽ֡����ô������Ҫ��Ƶ��ʽ֡��
		//�����ǰд�������Ƶ֡�����Ƿ���û��д����Ƶ��ʽ֡����ô������Ҫ��Ƶ��ʽ֡��
		//DateProc������û�д��������ڴ򿪳ɹ����д������Ƶ��ʽ����������ԭ��ֻ�Ǳ�֤д��¼���ļ���ǰ��֡Ϊ����Ƶ��ʽ֡��
		//modify 2010-01-26 xwm �򿪳ɹ����д������Ƶ��ʽ��ʽ��������������⣺��û��¼��Ƶ������£��ͻ���������¼����ļ�����崻���
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
		assert(false);//Ϊ�˷���δ���ļ������
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


