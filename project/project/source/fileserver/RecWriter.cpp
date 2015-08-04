
//RecWriter.cpp

#include "RecWriter.h"

#ifdef __REC_WRITE_BUF_256__
unsigned long REC_DATA_WRITE_BUF_SIZE	= 256 * 1024;
#else
#ifdef __TDFH__
unsigned long REC_DATA_WRITE_BUF_SIZE	= 1024 * 1024 + 2 * 1024;
#else
unsigned long REC_DATA_WRITE_BUF_SIZE	= 128 * 1024;
#endif
#endif

unsigned long CRecWriter::m_lastFFlushTime = 0;

CRecWriter::CRecWriter ()
{
	m_pFile = NULL;
	m_pDataEndFlag = NULL;

	memset(&m_bufInfo, 0, sizeof(REC_BUF_INFO));

#if REC_WRITE_DO_BUFFER
	m_bufInfo.pData = new unsigned char [REC_DATA_WRITE_BUF_SIZE];//
	memset(m_bufInfo.pData, 0, REC_DATA_WRITE_BUF_SIZE);
	m_bufInfo.bufLen = REC_DATA_WRITE_BUF_SIZE;
#endif

	m_bCombined = false;
	m_bBuckUp = false;
}

CRecWriter::~CRecWriter ()
{
#if REC_WRITE_DO_BUFFER
	if (NULL != m_bufInfo.pData)
	{
		delete [] m_bufInfo.pData;
		memset(&m_bufInfo, 0, sizeof(REC_BUF_INFO));
	}
#endif
}

bool CRecWriter::Open(const char *pPath,unsigned long fileSizeM, bool bCombined, bool bBackup)
{
	FD_ZERO(&m_fdset);

	m_bBuckUp = bBackup;

	m_vpFrameNum = 0;	
	m_bNeedWriteKey = true;
	assert(m_pFile == NULL);
	m_bCombined = bCombined;
	if (128 == fileSizeM)
	{
		m_MAX_REC_SUB_OFF_LEN	  = 4 * 1024 * 1024;			//��ʣ��ռ�С�ڴ�ֵʱ��һ������ؼ�֡������д���ҷ���ʧ��
		m_MAX_REC_FILE_LEN	      = 128 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 16*m_MAX_REC_FILE_LEN/(1024*1024);	//һ���ļ�������д��Ĺؼ�֡��������������256KBPS����100֡һ���ؼ�֡�������
	}
	else if(256 == fileSizeM)
	{
#if defined(__CHIP3531__) || defined(__CHIP3535__)
		m_MAX_REC_SUB_OFF_LEN	  = 36 * 1024 * 1024;
#else
		m_MAX_REC_SUB_OFF_LEN	  = 10 * 1024 * 1024;
#endif
		m_MAX_REC_FILE_LEN	      = 256 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 4096;
	}
	else if (512 == fileSizeM)
	{
		m_MAX_REC_SUB_OFF_LEN	  = 10 * 1024 * 1024;			
		m_MAX_REC_FILE_LEN	      = 512 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 8192;	
	}
	else if (16 == fileSizeM)
	{
		m_MAX_REC_SUB_OFF_LEN	  = 4 * 1024 * 1024;			
		m_MAX_REC_FILE_LEN	      = 16 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 16*m_MAX_REC_FILE_LEN/(1024*1024);	
	}
	else if (32 == fileSizeM)
	{
		m_MAX_REC_SUB_OFF_LEN	  = 4 * 1024 * 1024;			
		m_MAX_REC_FILE_LEN	      = 32 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 16*m_MAX_REC_FILE_LEN/(1024*1024);	
	}
	else
	{
		m_MAX_REC_SUB_OFF_LEN	  = 4 * 1024 * 1024;			
		m_MAX_REC_FILE_LEN	      = 128 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 16*m_MAX_REC_FILE_LEN/(1024*1024);
	}

	
	m_VIDEO_FORMAT_POS	          = INDEX_INFO_POS + (m_MAX_KEY_FRAME_NUM * sizeof(INDEX_INFO));
	m_AUDIO_FORMAT_POS	          = m_VIDEO_FORMAT_POS + sizeof(BITMAPINFOHEADER) + 32;
	if (m_bCombined)
	{
		m_REC_FILE_EVENT_INFO_POS = m_AUDIO_FORMAT_POS + sizeof(WAVEFORMATEX) + 32;
		m_FRAME_DATA_POS          = m_REC_FILE_EVENT_INFO_POS + sizeof(REC_FILE_EVENT_INFO)*MAX_REC_FILE_EVENT_INFO;
	}
	else
	{
		m_REC_FILE_EVENT_INFO_POS = 0;
		m_FRAME_DATA_POS          = m_AUDIO_FORMAT_POS + sizeof(WAVEFORMATEX) + 32;
	}
	m_FILE_BASIC_LEN		= m_FRAME_DATA_POS;
	
	if(m_pFile != NULL)
	{
		return false;
	}

	m_pFile = fopen(pPath, "rb+");

	//Linux��������������´����ķ�ʽȥ���ļ���
//#ifdef __ENVIRONMENT_WIN32__
	if(m_pFile == NULL)
	{
		m_pFile = fopen(pPath, "wb");
	}
//#endif

	if(m_pFile == NULL)
	{
		return false;
	}

	m_fd = fileno(m_pFile);

	INDEX_INFO *pIndexBuf = new INDEX_INFO[m_MAX_KEY_FRAME_NUM];
	if(pIndexBuf == NULL)
	{
		assert(false);
		Release(&m_pFile, 0);
		return false;
	}
	memset(pIndexBuf, 0, sizeof(INDEX_INFO) * m_MAX_KEY_FRAME_NUM);

	memset(&m_fileHeader, 0, sizeof(REC_FILE_HEADER));
	if (m_bCombined)
	{
		m_fileHeader.hFlag = HEADER_FLAG_COMBINE;		//����Ϣͷ���
	}
	else
	{
		m_fileHeader.hFlag = HEADER_FLAG;					//����Ϣͷ���
	}	
	m_fileHeader.hLen = sizeof(REC_FILE_HEADER);		//����Ϣͷ����
	m_fileHeader.frameInfoLen = sizeof(REC_FRAME_INFO);				//
	m_fileHeader.fileFlag = FILE_FLAG;					//�ļ���ǡ���"FTVT"
	m_fileHeader.fileLen = m_FILE_BASIC_LEN;				//�ļ��ܳ���
	m_fileHeader.writeFlg = WRITE_ING_FLAG;				//д���
	m_fileHeader.videoFormatAddr = m_VIDEO_FORMAT_POS;	//��Ƶ��ʽλ��
	m_fileHeader.audioFormatAddr = m_AUDIO_FORMAT_POS;	//��Ƶ��ʽλ��
	m_fileHeader.indexAddr = INDEX_INFO_POS;			//������λ��
	m_fileHeader.dataAddr = m_FRAME_DATA_POS;				//������λ��
   
	m_startTime = 0;
	m_endTime = 0;

	if(0 != fseek(m_pFile, 0, SEEK_SET))
	{
		assert(false);
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}

	if(1 != RecFWrite(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}

	//���������
	if(0 != fseek(m_pFile, INDEX_INFO_POS, SEEK_SET))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}
	
	if(m_MAX_KEY_FRAME_NUM != RecFWrite(pIndexBuf, sizeof(INDEX_INFO), m_MAX_KEY_FRAME_NUM, m_pFile))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}

	//�����Ƶ��ʽ
	BITMAPINFOHEADER bmpInfoHeader;
	memset(&bmpInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	if(0 != fseek(m_pFile, m_VIDEO_FORMAT_POS, SEEK_SET))
	{
		Release(&m_pFile, 0);
		return false;
	}
	if(1 != RecFWrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, m_pFile))
	{
		Release(&m_pFile, 0);
		return false;
	}


	//�����Ƶ��ʽ
	WAVEFORMATEX wfx;
	memset(&wfx, 0, sizeof(WAVEFORMATEX));
	if(0 != fseek(m_pFile, m_AUDIO_FORMAT_POS, SEEK_SET))
	{
		Release(&m_pFile, 0);
		return false;
	}
	if(1 != RecFWrite(&wfx, sizeof(WAVEFORMATEX), 1, m_pFile))
	{
		Release(&m_pFile, 0);
		return false;
	}

	if (m_bCombined)
	{
		REC_FILE_EVENT_INFO eventInfo;
		memset(&eventInfo, 0, sizeof(eventInfo));
		if (0 != fseek(m_pFile, m_REC_FILE_EVENT_INFO_POS, SEEK_SET))
		{
			Release(&m_pFile, 0);
			return false;
		}
		for (int i = 0; i < MAX_REC_FILE_EVENT_INFO; ++i)
		{
			if (1 != RecFWrite(&eventInfo, sizeof(eventInfo), 1, m_pFile))
			{
				Release(&m_pFile, 0);
				return false;
			}
		}
	}	
	
	//����Ҫ��յ�һ֡����Ϣ
	if(0 != fseek(m_pFile, m_fileHeader.dataAddr, SEEK_SET))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}	
    REC_FRAME_INFO frameInfo;
	memset(&frameInfo, 0, sizeof(REC_FRAME_INFO));
	if (1 != RecFWrite(&frameInfo, sizeof(REC_FRAME_INFO), 1, m_pFile))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}

	//���ļ�ָ�붨λ���������Ŀ�ʼλ��
	if(0 != fseek(m_pFile, m_fileHeader.dataAddr, SEEK_SET))
	{
		Release(&m_pFile, 1, &pIndexBuf);
		return false;
	}	

	m_IndexNum	= 0;
	m_videoNum	= 0;
	m_audioNum	= 0;
	m_textNum	= 0;

	m_indexPos	= INDEX_INFO_POS;
	m_bufInfo.filePos = m_fileHeader.dataAddr;

	m_bHasVideo	= false;
	m_bHasAudio	= false;

	memset(&m_frameInfo, 0, sizeof(REC_FRAME_INFO));
	memset(&m_indexInfo, 0, sizeof(INDEX_INFO));
	m_indexInfo.flag = I_VIDEO_FLAG;

	delete [] pIndexBuf;
	
	m_pDataEndFlag = new unsigned char[sizeof(REC_FRAME_INFO)];
	memset(m_pDataEndFlag, 0, sizeof(REC_FRAME_INFO));
	
#if REC_WRITE_DO_BUFFER
	memset(m_bufInfo.pData, 0, REC_DATA_WRITE_BUF_SIZE);
	m_bufInfo.dataLen = 0;
#endif

	return true;
}

int CRecWriter::WriteFileEventInfo(REC_FILE_EVENT_INFO *pEventInfo, int num)
{
	int writeNum = num;
	if (writeNum > MAX_REC_FILE_EVENT_INFO)
	{
		writeNum = MAX_REC_FILE_EVENT_INFO;
	}
	if (NULL == pEventInfo)
	{
		return 0;
	}

	if (!m_bCombined)
	{
		return 0;
	}

	if (0 != fseek(m_pFile, m_REC_FILE_EVENT_INFO_POS, SEEK_SET))
	{
		return -1;
	}
	
	if (writeNum != RecFWrite(pEventInfo, sizeof(REC_FILE_EVENT_INFO), writeNum, m_pFile))
	{
		return -1;
	}
	return 0;
}

void CRecWriter::Close ()
{
	if(NULL != m_pFile)
	{
		////�����Ƶ�б�
		//while(m_audioList.GetCount() > 0)
		//{
		//	WriteFrame(m_audioList.GetHead());

		//	delete [] m_audioList.RemoveHead().pData;
		//}
#if REC_WRITE_DO_BUFFER
		//�ѻ������е�����д���ļ���
		if(m_bufInfo.dataLen > 0)
		{
			if (0 == fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
			{
				RecFWrite(m_bufInfo.pData, m_bufInfo.dataLen+sizeof(REC_FRAME_INFO), 1, m_pFile);
				m_bufInfo.filePos += m_bufInfo.dataLen;
			}
			else
			{
				assert(false);
			}
		}
#endif
		//д�ļ�ͷ
		{
			m_fileHeader.fileLen = m_bufInfo.filePos;
			m_fileHeader.indexNum = m_IndexNum;
			m_fileHeader.videoNum = m_videoNum;
			m_fileHeader.audioNum = m_audioNum;
			m_fileHeader.textNum = m_textNum;
			m_fileHeader.startTime = m_startTime;
			m_fileHeader.endTime = m_endTime;
			m_fileHeader.writeFlg = WRITE_END_FLAG;

			if(0 != fseek(m_pFile, 0, SEEK_SET))
			{
				assert(false);
			}

			if(1 != RecFWrite(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
			{
				assert(false);
			}
		}

#if defined(__TDFH__) || defined(__CHIPTI__)
		//FHƽ̨�ڲ���ϵͳ�㻺������̫�࣬�������쳣�ϵ�ʱ�ؼ�֡������û��д��
		if (fflush(m_pFile))
		{
			printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
		}


		if (fsync(fileno(m_pFile)))
		{
			printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
		}
#endif
		fclose(m_pFile);
		m_pFile = NULL;
		delete [] m_pDataEndFlag;
	}
}

void CRecWriter::ClearAudioBuffer()
{
	while(m_audioList.GetCount() > 0)
	{
		delete [] m_audioList.RemoveHead().pData;
	}
}

bool CRecWriter::FFlush()
{
#if REC_WRITE_DO_BUFFER
	if(0 == m_bufInfo.dataLen)
	{
		return true;
	}

	if (0 != fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
	{
		return false;
	}
		
	RecFWrite(m_bufInfo.pData, m_bufInfo.dataLen+sizeof(REC_FRAME_INFO), 1, m_pFile);
	fflush(m_pFile);
	//PrintfBit64(m_endTime, false);
	m_bufInfo.filePos += m_bufInfo.dataLen;
	m_bufInfo.dataLen = 0;			

	return true;
#else
	return true;
#endif

}

/******************************************************************************
�������ܣ�д��һ֡����(��Ƶ����Ƶ���ı�),Ҫ��ÿһ���͸�����������
����˵����frame��Ҫд�������֡�����ݶ�keyFrame��frameType��length��pData��time
		  �������
����ֵ���ɹ����أ�WRITE_FRAME_SUCC
		ʧ�ܷ��أ�WRITE_FRAME_NEED_VIDEO_FORMAT(��Ҫ��Ƶ��ʽ)
				  WRITE_FRAME_NEED_AUDIO_FORMAT(��Ҫ��Ƶ��ʽ)
				  WRITE_FRAME_FULL(�ļ��Ѿ�д��)
				  WRITE_FRAME_FAIL(����д����������쳣����)
******************************************************************************/
unsigned long CRecWriter::WriteData(const FRAME_INFO_EX & frame)
{
	
	assert(m_pFile != NULL);
	
	if (!m_bHasVideo && (FRAME_TYPE_VIDEO_FORMAT == frame.frameType))
	{
		assert (sizeof(BITMAPINFOHEADER) == frame.length);
		if (0 == fseek(m_pFile, m_fileHeader.videoFormatAddr, SEEK_SET))
		{
			if (1 == RecFWrite(frame.pData, sizeof(BITMAPINFOHEADER), 1, m_pFile))
			{
				m_bHasVideo = true;
				
				return WRITE_FRAME_SUCC;
			}
		}

		return WRITE_FRAME_FAIL;
	}
	else if (FRAME_TYPE_AUDIO_FORMAT == frame.frameType)
	{
		assert (sizeof(WAVEFORMATEX) == frame.length);
		if (0 == fseek(m_pFile, m_fileHeader.audioFormatAddr, SEEK_SET))
		{
			if (1 == RecFWrite(frame.pData, sizeof(WAVEFORMATEX), 1, m_pFile))
			{
				m_bHasAudio = true;
				
				return WRITE_FRAME_SUCC;
			}
		}

		return WRITE_FRAME_FAIL;
	}
	else if (m_bHasVideo && (FRAME_TYPE_VIDEO == frame.frameType))
	{
		return OnWriteOneVideo(frame);
	}
	else if (FRAME_TYPE_AUDIO == frame.frameType)//Ҫ���������Ƶ��ʱ������,���µ���Ƶ֡���뵽��Ƶ֡�б�β
	{
		if(!m_bHasAudio)	//��Ƶ��ʽ��δ����
		{
			return WRITE_FRAME_NEED_AUDIO_FORMAT;
		}

#if defined(__TDFH__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3520A__) || defined(__CHIP3521__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__)
		//��Ƶ����ʱ��С���ڵ�ǰ�ļ��Ŀ�ʼʱ������Ϊ��Ƶʱ���
		//��Ƶ����ʱ�����Ƶ����ʱ�䲻�����ܴ�������ܴ󣬻ᵼ�¶���һЩ��Ƶ����
		if (0 == m_fileHeader.startTime || frame.time < m_fileHeader.startTime)
		{
			//printf("%s:%s:%d, The audio time is < the file start time\n", __FUNCTION__, __FILE__, __LINE__);
			//PrintfBit64(frame.time, true);
			//PrintfBit64(m_fileHeader.startTime, true);
			//PrintfBit64(frame.time, false);
			//PrintfBit64(m_fileHeader.startTime, false);
			return WRITE_FRAME_SUCC;
		}
#endif
		//��Ƶ��������ʱ�����б���ɾ�����ϵ�һ֡��Ƶ�����ڼ������µ�һ֡��Ƶʱ����û�г���
		if (m_audioList.GetCount() >= AUDIO_BUF_NUM)
		{
			delete [] m_audioList.RemoveHead().pData;
		}

		//��ƵҪ��������򣬷�������Ҳ������б�
		if(!m_audioList.IsEmpty() && (frame.time < m_audioList.GetTail().time))
		{
			//printf("chnn = %d, frame.time = 0x%08x%08x, prevTime = 0x%08x%08x\n", frame.channel, frame.time, m_audioList.GetTail().time);
		}
		
		assert (m_audioList.IsEmpty() || (frame.time >= m_audioList.GetTail().time));

		if (m_audioList.IsEmpty() || (frame.time >= m_audioList.GetTail().time))
		{
			FRAME_INFO_EX audioFrame = frame;
			audioFrame.pData = new BYTE[audioFrame.length];
			if(NULL == audioFrame.pData)
			{
				assert(false);
			}
			else
			{
				memcpy(audioFrame.pData, frame.pData, audioFrame.length);
				m_audioList.AddTail(audioFrame);
			}
		}

		return  WRITE_FRAME_SUCC;
	}
	else if (FRAME_TYPE_TEXT == frame.frameType)	//�ı������ݲ�����
	{
		//���ı�����ؼ��뵽�ı��б���;
		assert (false);
		return WRITE_FRAME_FAIL;
	}
	else
	{
		if (!m_bHasVideo)
		{
			return WRITE_FRAME_NEED_VIDEO_FORMAT;
		}

		return WRITE_FRAME_FAIL;
	}
}

/******************************************************************************

				protected functions

******************************************************************************/

/******************************************************************************
�������ܣ����ļ���д��֡����
����˳��Ҫ���ڹؼ�֡��ʱ
******************************************************************************/
unsigned long CRecWriter::WriteIndex(unsigned long index, unsigned long pos, LONGLONG time)
{
	//��ζ��Ԥ����Index�����Ѿ����꣬������д���µ�֡�����Է���ʧ�ܡ�
	if (m_MAX_KEY_FRAME_NUM <= m_IndexNum)
	{
		return WRITE_FRAME_FULL;
	}

	m_indexInfo.index		= index;
	m_indexInfo.position	= pos;
	m_indexInfo.time		= time;

	if(0 != fseek(m_pFile, m_indexPos, SEEK_SET))
	{
		return WRITE_FRAME_FAIL;
	}

	if(1 != RecFWrite(&m_indexInfo, sizeof(INDEX_INFO), 1, m_pFile))
	{
		return WRITE_FRAME_FAIL;
	}

#if defined(__TDFH__) || defined(__CHIPTI__)
	//FHƽ̨�ڲ���ϵͳ�㻺������̫�࣬�������쳣�ϵ�ʱ�ؼ�֡������û��д��
 	FFlushEx(m_pFile);
	//if (fflush(m_pFile))
 //	{
 //		printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
 //	}
 //	
 //
 //	if (fsync(fileno(m_pFile)))
 //	{
 //		printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
 //	}
#endif

	++ m_IndexNum;
	m_indexPos += sizeof(INDEX_INFO);

	return WRITE_FRAME_SUCC;
}

/******************************************************************************
�������ܣ�д��һ֡���ݵľ���ʵ��
ʵ�ַ������Ƚ�����д�뻺������Ȼ���ٶ�֡ͳһд���ļ�
******************************************************************************/
unsigned long CRecWriter::WriteFrame(const FRAME_INFO_EX & frame)
{
	//����֡��Ϣ�ṹ
	{
		if (FRAME_TYPE_VIDEO == frame.frameType)
		{
			m_frameInfo.flag	= frame.keyFrame ? I_VIDEO_FLAG : B_VIDEO_FLAG;
			m_vpFrameNum++;

			if(m_vpFrameNum > 80)
			{
				m_bNeedWriteKey = true;
				m_vpFrameNum = 0;
			}
		}
		else if (FRAME_TYPE_AUDIO == frame.frameType)
		{
			m_frameInfo.flag	= I_AUDIO_FLAG;
		}
		else if (FRAME_TYPE_TEXT == frame.frameType)
		{
			m_frameInfo.flag	= I_TEXT_FLAG;
		}
		else
		{
			assert (false);
			return WRITE_FRAME_FAIL;
		}

		if(0 == m_frameInfo.nextPos)		//д��һ֡����
		{
			m_frameInfo.prevPos	= 0;
			m_frameInfo.nextPos = m_fileHeader.dataAddr + sizeof(REC_FRAME_INFO) + frame.length;
		}
		else
		{
			m_frameInfo.prevPos = m_frameInfo.nextPos - m_frameInfo.length - sizeof(REC_FRAME_INFO);
			m_frameInfo.nextPos = m_frameInfo.nextPos + sizeof(REC_FRAME_INFO) + frame.length;
		}

		
		m_frameInfo.length	= frame.length;
		m_frameInfo.time	= frame.time;
#if defined(__TDFH__) || defined(__CHIPTI__)
		m_frameInfo.timeStampFlag = 0;
		m_frameInfo.timeStampFlag = RecFrameInfoChsum(&m_frameInfo);
#else
		m_frameInfo.timeStampFlag = 1;
#endif
	}

	//���⣺����һ��¼�񳤶ȴ�ԼΪ1Сʱ20���ӵ��ļ�����Ȼʧ�ܡ�
	//ԭ��������������¼���ļ��������ڵ�ʱ��д���¼���ļ�ʱ���ͳ��������ˡ�
	//����֮ǰ�����ж�¼���ļ��Ƿ�д�������û��д������֡д�뵽�ļ��Ȼ���ж�
	//�ؼ�֡�Ƿ�д�������д���ͷ���ʧ�ܡ���ͳ�����������⣺�ؼ�֡�Ѿ�д���ļ�����Ƿ���ʧ�ܡ�
	//�����ڴ˴����жϹؼ�֡�Ƿ�д����
	if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame)
	{
		//m_IndexNum��Ŵ�0��ʼ��������ʾ�Ѿ�д��ؼ�֡�ĸ�����
		//�����ǰm_IndexNumΪMAX_KEY_FRAME_NUM�����ļ��Ѿ�д����MAX_KEY_FRAME_NUM���ؼ�֡��
		//������ʱ����һ���ؼ�֡�Ļ�����ô��û�еط�д����ؼ�֡�������ˣ�����Ҫ������
		if (m_MAX_KEY_FRAME_NUM <= m_IndexNum)
		{
			return WRITE_FRAME_FULL;
		}
	}
	//end 2010-1-23 15:52 xwm

#if REC_WRITE_DO_BUFFER
	//
	if (m_MAX_REC_FILE_LEN >= (m_bufInfo.filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO) + m_bufInfo.dataLen))
	{
		do 
		{
			if ((m_bufInfo.dataLen + sizeof(REC_FRAME_INFO) + m_frameInfo.length + sizeof(REC_FRAME_INFO)) <= m_bufInfo.bufLen)
			{
				//
				memcpy(m_bufInfo.pData + m_bufInfo.dataLen, &m_frameInfo, sizeof(REC_FRAME_INFO));
				m_bufInfo.dataLen += sizeof(REC_FRAME_INFO);
				//
				memcpy(m_bufInfo.pData + m_bufInfo.dataLen, frame.pData, m_frameInfo.length);
				m_bufInfo.dataLen += m_frameInfo.length;
				//
				memcpy(m_bufInfo.pData + m_bufInfo.dataLen, m_pDataEndFlag, sizeof(REC_FRAME_INFO));

				//ÿ��д��Ƶ�ؼ�֡ʱ��Ҳдһ�δ���
				if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame && m_bNeedWriteKey)
				{
					if (0 != fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
					{					
						assert (false);
						return WRITE_FRAME_FAIL;
					}

					if (1 != RecFWrite(m_bufInfo.pData, m_bufInfo.dataLen+sizeof(REC_FRAME_INFO), 1, m_pFile))
					{
						assert (false);
						return WRITE_FRAME_FAIL;
					}

					m_bufInfo.filePos += m_bufInfo.dataLen;
					m_bufInfo.dataLen = 0;
				}

				//
				break;
			}
			else if ((sizeof(REC_FRAME_INFO) + m_frameInfo.length + sizeof(REC_FRAME_INFO)) <= m_bufInfo.bufLen)
			{
				//��ζ�Ÿ�֡������С�ڵ��ڻ���������
				if (0 != fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				if (1 != RecFWrite(m_bufInfo.pData, m_bufInfo.dataLen+sizeof(REC_FRAME_INFO), 1, m_pFile))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				m_bufInfo.filePos += m_bufInfo.dataLen;
				m_bufInfo.dataLen = 0;
			}
			else
			{
				if (0 != fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				if (m_bufInfo.dataLen > 0)
				{
					if (1 != RecFWrite(m_bufInfo.pData, m_bufInfo.dataLen, 1, m_pFile))
					{
						assert (false);
						return WRITE_FRAME_FAIL;
					}
				}
			
				//
				if (1 != RecFWrite(&m_frameInfo, sizeof(REC_FRAME_INFO), 1, m_pFile))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				if (1 != RecFWrite(frame.pData, m_frameInfo.length, 1, m_pFile))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				if (1 != RecFWrite(m_pDataEndFlag, sizeof(REC_FRAME_INFO), 1, m_pFile))
				{
					assert (false);
					return WRITE_FRAME_FAIL;
				}

				m_bufInfo.filePos += (m_bufInfo.dataLen + sizeof(REC_FRAME_INFO) + m_frameInfo.length);
				m_bufInfo.dataLen = 0;

				break;
			}
		} while(true);
	}
	else
	{
		return WRITE_FRAME_FULL;
	}
#else
	{
		/*д��һ֡���ݣ�����ܳ��ȳ����ļ��ܳ��ȣ��򷵻��ļ��Ѿ���*/
		if (m_MAX_REC_FILE_LEN >= (m_bufInfo.filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO)))
		{
			//�������ݵ�λ��
			if(0 != fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
			{
				assert (false);				
				return WRITE_FRAME_FAIL;
			}

			//д��֡��Ϣͷ
			if (1 != RecFWrite(&m_frameInfo, sizeof(REC_FRAME_INFO), 1, m_pFile))
			{
				assert (false);
				return WRITE_FRAME_FAIL;
			}

			//д��֡����
			if (1 != RecFWrite(frame.pData, frame.length, 1, m_pFile))
			{
				assert (false);				
				return WRITE_FRAME_FAIL;
			}
			
			//д�����ݽ������
			if (1 != RecFWrite(m_pDataEndFlag, sizeof(REC_FRAME_INFO), 1, m_pFile))
			{
				assert (false);
				return WRITE_FRAME_FAIL;
			}

			m_bufInfo.filePos += (sizeof(REC_FRAME_INFO) + frame.length);
		}
		else
		{			
			return WRITE_FRAME_FULL;
		}
	}
#endif

	//��Ƶ�ؼ�֡������Ҫд������
	if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame && m_bNeedWriteKey)
	{
		//дʧ�ܣ���ѵ�ǰд������Ҳ����ʧ��
		if (WRITE_FRAME_FAIL == WriteIndex(m_videoNum, m_frameInfo.nextPos-frame.length-sizeof(REC_FRAME_INFO), frame.time))
		{			
			return WRITE_FRAME_FAIL;
		}
	}

	if (FRAME_TYPE_VIDEO == frame.frameType)
	{
		++ m_videoNum;

		if (0 == m_startTime)	//��һ����Ƶ֡����Ҫ��¼��ʼʱ��
		{
			m_startTime = frame.time;
			m_fileHeader.startTime = m_startTime;
			if(0 != fseek(m_pFile, 0, SEEK_SET))
			{
				assert(false);
			}

			if(1 != RecFWrite(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
			{
				assert(false);
			}
		}
		m_endTime = frame.time;	//ֻ������Ƶ��ʱ��
	}
	else if (FRAME_TYPE_AUDIO == frame.frameType)
	{
		++ m_audioNum;
	}
	else if (FRAME_TYPE_TEXT == frame.frameType)
	{
		++ m_textNum;
	}
	return WRITE_FRAME_SUCC;
}

unsigned long CRecWriter::OnWriteOneVideo(const FRAME_INFO_EX & frame)
{
	//2010-01-25 13:48:00 YSW
	//���ڲ����˻����Ԥ¼����ƣ����Լ�ʱ�ؼ�֡�Ĳ�����һ����ʱ�ӣ��������ļ�д����
	//������ؼ�֡����ʱ�������£���Ȼ����һ���Ķ�ʧ��������ļ��ӹؼ�֡��ʼ��ȡ���ؼ�֡ǰ��
	//��֡���ܶ������൱�ڶ�ʧ���������������һ���ļ���д����ʱ��һ��������Ƶ�ؼ�֡���Ͳ�д��
	//���ļ����ҷ���д��ʧ�ܡ��ⲿģ����Ὣ�ùؼ�֡������Ԥ¼��ģ����л��棬Ȼ����µ��ļ���
	//����д�룬����ʵ���Ͼ��ܴﵽ���ǲ���һ֡�����ļ���ͷҲ�ǹؼ�֡��Ŀ�ġ�
	//����һ�����ƣ��ؼ�֡��Ŀ�����ƣ�������д��ǰ���ȼ���Ƿ�Ϊ�ؼ�֡���Ƿ��Ѿ��ﵽ�����Ŀ������
	//ֻҪ������������ؼ�֡��Ŀ���������ڹر�ǰ�����һ֡һ���ǹؼ�֡����ôҲ�᷵���ļ�����ֵ���ⲿ����
	//Ҳ�ǽ���֡����Ԥ¼�񻺴棬�������ļ���д�롣

	//���������������ڴ˼�鵱ǰ�ļ���ʵ��д���������Ŀ
	if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame)
	{		

		//���Ѵ�֡д��󣬽�����뵽�ļ����ı��������򷵻�����
		if ((m_MAX_REC_FILE_LEN - m_MAX_REC_SUB_OFF_LEN) < (m_bufInfo.filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO) + m_bufInfo.dataLen))
		{			
			return WRITE_FRAME_FULL;
		}
	}

	//����Ƶ�б��бȵ�ǰ֡ʱ�����֡д���ļ�,������Щ��Ƶ���б������
	{
		unsigned long ret = WRITE_FRAME_FAIL;
		while(m_audioList.GetCount() > 0)
		{
			if(m_audioList.GetHead().time < frame.time)
			{
				ret = WriteFrame(m_audioList.GetHead());
				if (WRITE_FRAME_SUCC != ret)
				{					
					return ret;
				}

				delete [] m_audioList.RemoveHead().pData;
			}
			else
			{
				break;
			}
		}
	}

	//���ı��б��бȵ�ǰ֡ʱ�����֡д���ļ�

	//д����Ƶ֡
	return WriteFrame(frame);
}

void CRecWriter::FFlushEx(FILE * pFile)
{
	if (NULL == pFile)
	{
		return;
	}

	if (0 == m_lastFFlushTime)
	{
		m_lastFFlushTime = GetCurrTime32();
		return;
	}
	
	if (m_lastFFlushTime > GetCurrTime32() || 15 < (GetCurrTime32() - m_lastFFlushTime))
	{
		if (fflush(pFile))
		{
			printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
		}
#ifdef __ENVIRONMENT_LINUX__
		if (fsync(fileno(pFile)))
		{
			printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
		}
#endif
		m_lastFFlushTime = GetCurrTime32();
	}
}

size_t CRecWriter::RecFWrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	if (m_bBuckUp)
	{
		timeval timeOut = {2, 0};

		FD_ZERO(&m_fdset);
		FD_SET(m_fd, &m_fdset);
		select(m_fd + 1, NULL, &m_fdset, NULL, &timeOut);
	}

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
//end
