
//RecReader.cpp

#include "RecReader.h"
#include "fcntl.h"
#ifdef __TDFH__
#include "MemPool.h"
#endif
unsigned char MAX_READ_TRY_NUM = 1;
unsigned long REC_DATA_READ_BUF_SIZE	= 256 * 1024;

CRecReader::CRecReader ()
{
	m_pFile = NULL;
	m_pIndexInfo = NULL;

	memset(&m_bufInfo, 0, sizeof(REC_BUF_INFO));

// #if REC_READ_DO_BUFFER
// #ifdef __TDFH__
// 	m_bufInfo.pData = (unsigned char *)CMemPool::Instance()->GetMem(MEM_REC_READ, REC_DATA_READ_BUF_SIZE);//
// #else
// 	m_bufInfo.pData = new unsigned char [REC_DATA_READ_BUF_SIZE];//
// #endif
// 
// 	
// 	memset(m_bufInfo.pData, 0, REC_DATA_READ_BUF_SIZE);
// 	m_bufInfo.bufLen = REC_DATA_READ_BUF_SIZE;
// #endif
}

CRecReader::~CRecReader ()
{
#if REC_READ_DO_BUFFER
#ifdef __TDFH__
	CMemPool::Instance()->ReleaseMem(m_bufInfo.pData);
#else
	if (NULL != m_bufInfo.pData)
	{
		delete [] m_bufInfo.pData;
	}
	memset(&m_bufInfo, 0, sizeof(REC_BUF_INFO));
#endif
#endif
}

bool CRecReader::Open(const char * pFilePath,FILE_BUF_TYPE fileType, bool bForBackup)
{
	assert(m_pFile == NULL);
	 
#if REC_READ_DO_BUFFER
	if (NULL == m_bufInfo.pData)
	{
#ifdef __TDFH__
		m_bufInfo.pData = (unsigned char *)CMemPool::Instance()->GetMem(MEM_REC_READ, REC_DATA_READ_BUF_SIZE);//
#else
		m_bufInfo.pData = new unsigned char [REC_DATA_READ_BUF_SIZE];//
#endif
		memset(m_bufInfo.pData, 0, REC_DATA_READ_BUF_SIZE);
		m_bufInfo.bufLen = REC_DATA_READ_BUF_SIZE;
	}
	else
	{
		memset(m_bufInfo.pData, 0, m_bufInfo.bufLen);
	}
#endif
	unsigned int cnt=0;
	while(1)
	{
		m_pFile = fopen(pFilePath, "rb");
		if(m_pFile == NULL)
		{
			return false;
		}
		if(0 == fileno(m_pFile))
		{
			cnt++;
			fclose(m_pFile);
			m_pFile = NULL; 
			int fd0=open("/dev/null",O_RDWR);
			if(cnt >=2)
			 	return false;				
		}
		else
		{
			break;
		}
	}

	//��ȡ�ļ�ͷ��Ϣ
	if(0 != fseek(m_pFile, 0, SEEK_SET))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	if(1 != fread(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	//�ж��ļ��Ƿ�Ϊ¼���ļ�
	/*if((m_fileHeader.fileFlag != FILE_FLAG) || (0 == m_fileHeader.indexNum) || (0 == m_fileHeader.videoNum))*/
	if(m_fileHeader.fileFlag != FILE_FLAG)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	//�ж��ļ��Ƿ���д��
	if(m_fileHeader.writeFlg == WRITE_ING_FLAG)	//�ļ�δд����
	{
		m_bFileNormal = false;
	}
	else
	{
		m_bFileNormal = true;
	}

	m_bufInfo.filePos = m_fileHeader.dataAddr;
	m_bufInfo.currPos = 0;
	m_bufInfo.dataLen = 0;  //��֤ÿ���´�¼���ļ�ʱ�������ǰ�Ļ������ݡ�

	//��ȡ����Ҫ���ļ���Ϣ
	if(!ExtractInfoFromFile())
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	if(0 != RECFseek(m_pFile, m_fileHeader.dataAddr, SEEK_SET))
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}

	PrintfBit64(m_fileHeader.startTime,false);
	PrintfBit64(m_fileHeader.endTime,false);

	//��ȡ��һ֡������Ϣ
	if(SeekVideoFrame(SEEK_FIRST_KEY) != ERR_READ_FILE_OK)
	{
		delete [] m_pIndexInfo;
		m_pIndexInfo = NULL;
		fclose(m_pFile);
		m_pFile = NULL;

		return false;
	}

	return true;
}

bool CRecReader::Close()
{
	if(m_pFile == NULL)
	{
		return true;
	}

	fclose(m_pFile);
	m_pFile = NULL;

	if (NULL != m_pIndexInfo)
	{
		delete [] m_pIndexInfo;
	}
	m_pIndexInfo = NULL;


	if (NULL != m_bufInfo.pData)
	{
		delete [] m_bufInfo.pData;
	}
	m_bufInfo.pData = NULL;

	return true;
}

unsigned long CRecReader::GetFrameNum(FRAME_TYPE type)
{
	if(type == FRAME_TYPE_VIDEO)
	{
		return m_fileHeader.videoNum;
	}
	else if(type == FRAME_TYPE_AUDIO)
	{
		return m_fileHeader.audioNum;
	}
	else if(type == FRAME_TYPE_TEXT)
	{
		return m_fileHeader.textNum;
	}
	else
	{
		assert(false);
		return 0;
	}
}

unsigned long CRecReader::GetVideoInfo(BITMAPINFOHEADER *pBitmapInfo)
{
	assert(pBitmapInfo != NULL);

	if(m_pFile == NULL)
	{
		return ERR_READ_FILE_NO_OPEN;
	}
	memcpy(pBitmapInfo, &m_bitmapInfo, sizeof(BITMAPINFOHEADER));
	return ERR_READ_FILE_OK;
}

unsigned long CRecReader::GetAudioInfo(WAVEFORMATEX *pWaveInfo)
{
	assert(pWaveInfo != NULL);

	if(m_pFile == NULL)
	{
		return ERR_READ_FILE_NO_OPEN;
	}

	if(m_waveInfo.nAvgBytesPerSec == 0)
	{
		return ERR_READ_FILE_NO_AUDIO;
	}
	memcpy(pWaveInfo, &m_waveInfo, sizeof(WAVEFORMATEX));
	return ERR_READ_FILE_OK;
}

void CRecReader::GetTimeRange(unsigned long &startTime, unsigned long &endTime)
{
	startTime = static_cast<unsigned long>(m_fileHeader.startTime/1000000);
	endTime = static_cast<unsigned long>(m_fileHeader.endTime/1000000);
}

FRAME_TYPE CRecReader::GetCurrFrameType()
{
	FRAME_TYPE type;
	switch(m_curFrame.flag)
	{
	case I_VIDEO_FLAG:
	case B_VIDEO_FLAG:
		type = FRAME_TYPE_VIDEO;
		break;
	case I_AUDIO_FLAG:
		type = FRAME_TYPE_AUDIO;
		break;
	case I_TEXT_FLAG:
		type = FRAME_TYPE_TEXT;
		break;
	default:
		type = FRAME_TYPE_NONE;	
		assert(false);
	}

	return type;
}

/******************************************************************************
�������ܣ���ȡ��ǰ֡����
����˵����frameInfo����Ϊ�������ʱ����pData�ֶ�Ҫָ�������ݵĻ�������
		  length�ֶ�ΪpData�ֶ���ָ�򻺳�������
����ֵ:	�ɹ����أ�ERR_READ_FILE_OK
		ʧ�ܷ��أ�ERR_READ_FILE_READ_FAILED����ȡʧ�ܣ�
				  ERR_READ_FILE_BUF_SMALL(frameInfo�ֶ��е�pData��ָ�򻺳����ռ�̫С)
******************************************************************************/
unsigned long CRecReader::GetCurrFrame(FRAME_INFO_EX & frameInfo)
{
	assert(m_curFrame.flag != 0);
	assert(frameInfo.pData != NULL);

	frameInfo.frameType = GetCurrFrameType();
	/*if(frameInfo.length < m_curFrame.length)
	{
		assert(false);
		return ERR_READ_FILE_BUF_SMALL;
	}*/
	frameInfo.length = m_curFrame.length;
	frameInfo.time = m_curFrame.time;
	if(m_curFrame.flag == B_VIDEO_FLAG)
	{
		frameInfo.keyFrame = 0;
	}
	else
	{
		frameInfo.keyFrame = 1;
	}

	if(1 != RECFread(frameInfo.pData, frameInfo.length, 1, m_pFile))
	{
		return ERR_READ_FILE_READ_FAILED;
	}

	return ERR_READ_FILE_OK;
}

/******************************************************************************
��������:���ļ��ж�λ��ָ������Ƶ֡
����˵��:mode--��λģʽ(SEEK_NEXT,SEEK_PREV,SEEK_NEXT_KEY,SEEK_PREV_KEY,SEEK_FIRST_KEY,SEEK_LAST_KEY)
		 seekNum--��λ֡��(ֻ�����ؼ�֡ʱ����)
����ֵ	:��λ�ɹ����� ERR_READ_FILE_OK
		 ��λʧ�ܷ��� ERR_READ_FILE_NO_OPEN(�ļ�û��)
					  ERR_READ_FILE_SEEK_TO_END(�ļ��ѵ������)
					  ERR_READ_FILE_SEEK_TO_BEGIN(�ļ��ѵ��￪ʼ)
					  ERR_READ_FILE_SEEK_FAILED(�ļ���λʧ��)
					  ERR_READ_FILE_READ_FAILED(��ȡ֡��Ϣʧ��)
******************************************************************************/

unsigned long CRecReader::SeekVideoFrame(SEEK_MODE mode, unsigned long seekNum /* = 1 */)
{
	if(m_pFile == NULL)
	{
		return ERR_READ_FILE_NO_OPEN;
	}

	m_curFrame.flag = 0;
	if(mode == SEEK_NEXT_VFRM)
	{
		unsigned long ret = ERR_READ_FILE_READ_FAILED;
		while((m_curFrame.flag != I_VIDEO_FLAG) && (m_curFrame.flag != B_VIDEO_FLAG))
		{
			unsigned char tryNum = 0;
			do 
			{
#if REC_READ_DO_BUFFER
				//��ʾ����д���ļ����ҵ�һ�ζ�ȡ���ɹ��������ж�ȡ�������Ч��
				if (0 < tryNum)
				{
					if (0 == fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
					{
						unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, m_pFile);
						if ((0 < len) && (len <= m_bufInfo.bufLen))
						{
							//�ɹ�����Ӧ�ø���һ�����ݳ��ȡ�
							m_bufInfo.dataLen = len;
						}
						else
						{
							return ERR_READ_FILE_READ_FAILED;
						}
					}
					else
					{
						return ERR_READ_FILE_SEEK_FAILED;
					}
				}
#endif

				//�ҵ���һ֡��λ��
				if(0 != RECFseek(m_pFile, m_curFrame.nextPos, SEEK_SET))
				{
					return ERR_READ_FILE_SEEK_FAILED;
				}

				//��ȡ���ݱ�ʶ
				if(1 != RECFread(&m_curFrame.flag, sizeof(m_curFrame.flag), 1, m_pFile))
				{
					return ERR_READ_FILE_READ_FAILED;
				}

				//�ж����ݱ�ʶ�ĺϷ���
				if(IsValidDataFlag(m_curFrame.flag))
				{
					//��λ����ȡ��һ֡��֡��Ϣ
					if(0 != RECFseek(m_pFile, m_curFrame.nextPos, SEEK_SET))
					{
						return ERR_READ_FILE_SEEK_FAILED;
					}

					if(1 != RECFread(&m_curFrame, sizeof(m_curFrame), 1, m_pFile))
					{
						return ERR_READ_FILE_READ_FAILED;
					}

					//�ٴμ�����ݱ�ʶ�ĺϷ���
					if(IsValidDataFlag(m_curFrame.flag))
					{
						//��ȡ��һ֡��Ϣ��ȷ�����˳�ѭ����
						ret = ERR_READ_FILE_OK;
						break;
					}
					else//��ǲ���ȷ
					{
						ret = ERR_READ_FILE_SEEK_TO_END;
					}
				}
				else
				{
					//��ʾ��ǲ���ȷ�����������д���ļ������ٳ���һ�Ρ�
					ret = ERR_READ_FILE_SEEK_TO_END;
				}

				++ tryNum;
			} while(!m_bFileNormal && (tryNum <= MAX_READ_TRY_NUM));
		}

		return ret;
	}
	else if(mode == SEEK_PREV_VFRM)
	{
		unsigned long ret = ERR_READ_FILE_READ_FAILED;
		while((m_curFrame.flag != I_VIDEO_FLAG) && (m_curFrame.flag != B_VIDEO_FLAG))
		{
			unsigned char tryNum = 0;
			do 
			{
#if REC_READ_DO_BUFFER
				//��ʾ����д���ļ����ҵ�һ�ζ�ȡ���ɹ��������ж�ȡ�������Ч��
				if (0 < tryNum)
				{
					if (0 == fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
					{
						unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, m_pFile);
						if ((0 < len) && (len <= m_bufInfo.bufLen))
						{
							//�ɹ�����Ӧ�ø���һ�����ݳ��ȡ�
							m_bufInfo.dataLen = len;
						}
						else
						{
							return ERR_READ_FILE_READ_FAILED;
						}
					}
					else
					{
						return ERR_READ_FILE_SEEK_FAILED;
					}
				}
#endif

				//�ҵ���һ֡��λ��
				if(0 != RECFseek(m_pFile, m_curFrame.prevPos, SEEK_SET))
				{
					return ERR_READ_FILE_SEEK_FAILED;
				}

				//��ȡ���ݱ�ʶ
				if(1 != RECFread(&m_curFrame.flag, sizeof(m_curFrame.flag), 1, m_pFile))
				{
					return ERR_READ_FILE_READ_FAILED;
				}

				//�ж����ݱ�ʶ�ĺϷ���
				if(IsValidDataFlag(m_curFrame.flag))
				{
					//��λ����ȡ��һ֡��֡��Ϣ
					if(0 != RECFseek(m_pFile, m_curFrame.prevPos, SEEK_SET))
					{
						return ERR_READ_FILE_SEEK_FAILED;
					}

					if(1 != RECFread(&m_curFrame, sizeof(m_curFrame), 1, m_pFile))
					{
						return ERR_READ_FILE_READ_FAILED;
					}

					//�ٴμ�����ݱ�ʶ�ĺϷ���
					if(IsValidDataFlag(m_curFrame.flag))
					{
						//��ȡ��һ֡��Ϣ��ȷ�����˳�ѭ����
						ret = ERR_READ_FILE_OK;
						break;
					}
					else//��ǲ���ȷ
					{
						ret = ERR_READ_FILE_SEEK_TO_BEGIN;
					}
				}
				else
				{
					//��ʾ��ǲ���ȷ�����������д���ļ������ٳ���һ�Ρ�
					ret = ERR_READ_FILE_SEEK_TO_BEGIN;
				}

				++ tryNum;
			} while(!m_bFileNormal && (tryNum <= MAX_READ_TRY_NUM));
		}

		return ret;
	}
	else	//�ؼ�֡��λ
	{
		bool bContinue = false;
		unsigned long ret = ERR_READ_FILE_READ_FAILED;
		unsigned char tryNum = 0;
		do 
		{
#if REC_READ_DO_BUFFER
			if (0 < tryNum)
			{
				if (!ExtractInfoFromFile())
				{
					return ERR_READ_FILE_READ_FAILED;
				}
			}
#endif

			bContinue = false;
			if(mode == SEEK_NEXT_KEY)
			{
				if(m_pIndexInfo[m_curIndex].position >= m_curFrame.nextPos)
				{
					while((--m_curIndex >= 0) && (m_pIndexInfo[m_curIndex].position >= m_curFrame.nextPos))
					{
						;
					}

					assert(m_curIndex >= 0);
					m_curIndex += seekNum;
				}
				else
				{
					while((++m_curIndex < m_fileHeader.indexNum) && (m_pIndexInfo[m_curIndex].position < m_curFrame.nextPos))
					{
						;
					}

					if(m_curIndex == m_fileHeader.indexNum)
					{
						bContinue = true;
						ret = ERR_READ_FILE_SEEK_TO_END;
					}
					else if((m_curIndex - 1 + seekNum) >= m_fileHeader.indexNum)
					{
						m_curIndex = m_fileHeader.indexNum - 1;
					}
					else
					{
						m_curIndex -= 1;
						m_curIndex += seekNum;
					}
				}
			}
			else if(mode == SEEK_PREV_KEY)
			{
				//�������Ƶ�ؼ�֡���� - seekNum
				if(m_pIndexInfo[m_curIndex].position > m_curFrame.prevPos)
				{
					while((--m_curIndex >= 0) && (m_pIndexInfo[m_curIndex].position > m_curFrame.prevPos))
					{
						;
					}

					if(m_curIndex == -1)
					{
						bContinue = true;
						ret = ERR_READ_FILE_SEEK_TO_BEGIN;
					}
					else if((m_curIndex + 1) < seekNum)
					{
						m_curIndex = 0;
					}
					else
					{
						m_curIndex += 1;
						m_curIndex -= seekNum;
					}
				}
				else
				{
					while((++m_curIndex < m_fileHeader.indexNum) && (m_pIndexInfo[m_curIndex].position <= m_curFrame.prevPos))
					{
						;
					}

					assert(m_curIndex <= m_fileHeader.indexNum);
					if (m_curIndex < seekNum)
					{
						m_curIndex = 0;
					}
					else
					{
						m_curIndex -= seekNum;
					}
				}
			}
			else if(mode == SEEK_FIRST_KEY)
			{
				m_curIndex = 0;
			}
			else if(mode == SEEK_LAST_KEY)
			{
				m_curIndex = m_fileHeader.indexNum-1;
			}
			else
			{
				assert(false);
				return ERR_READ_FILE_PARAM_INVALID;
			}

			++ tryNum;

		} while(bContinue && !m_bFileNormal && (tryNum < MAX_READ_TRY_NUM));
		
		//���bContinueΪ�棬����ζ�����涨λ���ɹ������Ҫ�������´�Ҳ�Ѿ�����������������
		//��ô�ٴξ�Ӧ�÷�����ֵ������λ�����ļ�ͷ�����ļ�β�ˡ������������ζ�����ǳɹ��ġ�
		if (bContinue)
		{
			return ret;
		}

		tryNum = 0;
		do 
		{
#if REC_READ_DO_BUFFER
			//��ʾ����д���ļ����ҵ�һ�ζ�ȡ���ɹ��������ж�ȡ�������Ч��
			if (0 < tryNum)
			{
				if (0 == fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
				{
					unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, m_pFile);
					if ((0 < len) && (len <= m_bufInfo.bufLen))
					{
						//�ɹ�����Ӧ�ø���һ�����ݳ��ȡ�
						m_bufInfo.dataLen = len;
					}
					else
					{
						return ERR_READ_FILE_READ_FAILED;
					}
				}
				else
				{
					return ERR_READ_FILE_SEEK_FAILED;
				}
			}
#endif
			if(0 != RECFseek(m_pFile, m_pIndexInfo[m_curIndex].position, SEEK_SET))
			{
				return ERR_READ_FILE_SEEK_FAILED;
			}

			if(1 != RECFread(&m_curFrame, m_fileHeader.frameInfoLen, 1, m_pFile))
			{
				return ERR_READ_FILE_READ_FAILED;
			}

			if(IsValidDataFlag(m_curFrame.flag))
			{
				return ERR_READ_FILE_OK;
			}

			++ tryNum;
		} while(!m_bFileNormal && (tryNum <= MAX_READ_TRY_NUM));

		return ERR_READ_FILE_READ_FAILED;
	}
}

/******************************************************************************
��������:���ļ��ж�λ����һ֡
����˵��:
����ֵ	:��λ�ɹ����� ERR_READ_FILE_OK
		 ��λʧ�ܷ��� ERR_READ_FILE_NO_OPEN(�ļ�û��)
					  ERR_READ_FILE_SEEK_TO_END(�ļ��ѵ������)
					  ERR_READ_FILE_SEEK_TO_BEGIN(�ļ��ѵ��￪ʼ)
					  ERR_READ_FILE_SEEK_FAILED(�ļ���λʧ��)
					  ERR_READ_FILE_READ_FAILED(��ȡ֡��Ϣʧ��)
******************************************************************************/
unsigned long CRecReader::SeekNextFrame()
{
	if(m_pFile == NULL)
	{
		return ERR_READ_FILE_NO_OPEN;
	}

	unsigned char tryNum = 0;
	do 
	{
#if REC_READ_DO_BUFFER
		//��ʾ����д���ļ����ҵ�һ�ζ�ȡ���ɹ��������ж�ȡ�������Ч��
		if (0 < tryNum)
		{
			if (0 == fseek(m_pFile, m_bufInfo.filePos, SEEK_SET))
			{
				unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, m_pFile);
				if ((0 < len) && (len <= m_bufInfo.bufLen))
				{
					//�ɹ�����Ӧ�ø���һ�����ݳ��ȡ�
					m_bufInfo.dataLen = len;
				}
				else
				{
					return ERR_READ_FILE_READ_FAILED;
				}
			}
			else
			{
				return ERR_READ_FILE_SEEK_FAILED;
			}
		}
#endif

		//�ҵ���һ֡��λ��
		if(0 != RECFseek(m_pFile, m_curFrame.nextPos, SEEK_SET))
		{
			return ERR_READ_FILE_SEEK_FAILED;
		}

		//��ȡ���ݱ�ʶ
		if(1 != RECFread(&m_curFrame.flag, sizeof(m_curFrame.flag), 1, m_pFile))
		{
			return ERR_READ_FILE_READ_FAILED;
		}

		//�ж����ݱ�ʶ�ĺϷ���
		if(IsValidDataFlag(m_curFrame.flag))
		{
			//��λ����ȡ��һ֡��֡��Ϣ
			if(0 != RECFseek(m_pFile, m_curFrame.nextPos, SEEK_SET))
			{
				return ERR_READ_FILE_SEEK_FAILED;
			}

			if(1 != RECFread(&m_curFrame, sizeof(m_curFrame), 1, m_pFile))
			{
				return ERR_READ_FILE_READ_FAILED;
			}

			//�ٴμ�����ݱ�ʶ�ĺϷ���
			if(IsValidDataFlag(m_curFrame.flag))
			{
				//��ȷ���򷵻سɹ���
				return ERR_READ_FILE_OK;
			}
			//else ��ʾ��ǲ���ȷ�����������д���ļ������ٳ���һ�Ρ�
		}
		//else ��ʾ��ǲ���ȷ�����������д���ļ������ٳ���һ�Ρ�

		++ tryNum;
	} while(!m_bFileNormal && (tryNum <= MAX_READ_TRY_NUM));

	//��������ζ���ݲ������������ǵ�������β���ˡ�
	return ERR_READ_FILE_SEEK_TO_END;
}

/******************************************************************************
��������:���ļ��ж�λ��ָ��ʱ��Ľ�ǰ�ؼ�֡
����˵��:
����ֵ	:��λ�ɹ����� ERR_READ_FILE_OK
		 ��λʧ�ܷ��� ERR_READ_FILE_NO_OPEN(�ļ�û��)
					  ERR_READ_FILE_SEEK_TO_END(�ļ��ѵ������)
					  ERR_READ_FILE_SEEK_TO_BEGIN(�ļ��ѵ��￪ʼ)
					  ERR_READ_FILE_SEEK_FAILED(�ļ���λʧ��)
					  ERR_READ_FILE_READ_FAILED(��ȡ֡��Ϣʧ��)
					  ERR_READ_FILE_EXCEPTION(�ļ��쳣)
******************************************************************************/
unsigned long CRecReader::SeekKeyFrameToTime(LONGLONG time)
{
	unsigned long index = 0;
	unsigned long ret = 0;
	if(m_pFile == NULL)
	{
		return ERR_READ_FILE_NO_OPEN;
	}
	
	unsigned char tryNum = 0;
	do 
	{
#if REC_READ_DO_BUFFER
		if (0 < tryNum)
		{
			if (!ExtractInfoFromFile())
			{
				return ERR_READ_FILE_READ_FAILED;
			}
		}
#endif

		if(time < m_fileHeader.startTime)
		{
			index = 0;
			ret = ERR_READ_FILE_SEEK_TO_BEGIN;
		}
		else if(time > m_fileHeader.endTime)
		{
			index = m_fileHeader.indexNum-1;
			ret = ERR_READ_FILE_SEEK_TO_END;		
		}
		else
		{
			LONGLONG seekTime = time;
			LONGLONG startTime = m_pIndexInfo[0].time;
			LONGLONG endTime = m_pIndexInfo[m_fileHeader.indexNum-1].time;
			unsigned long startIndex = 0;
			unsigned long endIndex = (m_fileHeader.indexNum-1);

			/* ����ʹ�������ʽ�Ͷ��ֲ��ҵ�Ŀ���ǣ����ļ��ж�λ��ָ��ʱ��Ľ�ǰ�ؼ�֡��
			���������ʽ��ʱ��û�п�ε�¼���ļ����Ǻ��ʵġ�
			������ʱ�������Ծ��¼���ļ�����п��ܶ�λ��ָ��ʱ��ĵ�n���ؼ�֡.n>=1
			�����޸������������ֱ�Ӳ��ö��ֲ��ң����ù�ʽ��
			//һ������£��ؼ�֡����������ʱ����Ƴ����Թ�ϵ���˴��������Թ�ʽ��ʱ������������
			//
			//							  (Ҫ��λ����ʱ�� - ��ʼ����ʱ��) * (����������� - ��ʼ�������)
			// ������� = ��ʼ������� + ������������������������������������������������������������������
			//									     (��������ʱ�� - ��ʼ����ʱ��)
			//
			//����ֻ�ǽ��Ƴ����Թ�ϵ�����Կ���һ�β����ܶ�λ��ָ���ؼ�֡������Ҫ�������ڵ���ʱ���ֽ����
			//���ַ�������ʹ�����ٶȸ���Щ
			
			while(((index+1) < (m_fileHeader.indexNum-1)) && (m_pIndexInfo[index+1].time < time))
			{
				unsigned long inc = (unsigned long)((seekTime - startTime) * (endIndex - startIndex) / (endTime - startTime));
				if (0 == inc)
				{
					index += 1;
					break;
				}

				index = startIndex + inc;

				if(m_pIndexInfo[index].time < time)
				{
					startIndex = index;
					startTime = m_pIndexInfo[startIndex].time;
					if(m_pIndexInfo[(startIndex+endIndex)/2].time > time)
					{
						endIndex = (startIndex+endIndex)/2;
						endTime = m_pIndexInfo[endIndex].time;
					}
				}
				else
				{
					endIndex = index;
					endTime = m_pIndexInfo[endIndex].time;
					if(m_pIndexInfo[(startIndex+endIndex)/2].time < time)
					{
						startIndex = (startIndex+endIndex)/2;
						startTime = m_pIndexInfo[startIndex].time;
					}
					index = startIndex;
				}
			}
			*/

			if (time <= startTime)
			{
				index = 0;
			}
			else if (time >= endTime)
			{
				index = m_fileHeader.indexNum-1;
			}
			else
			{
				//��������㷨��Ҫ��λ��time��ֻ�������ֵ����Сֵ֮�䣬���򣬻ᶨλ��׼��
				while (startIndex <= endIndex)
				{
					index = ((startIndex + endIndex)>>1);
					if (startIndex == index)
					{
						break;
					}

					if (m_pIndexInfo[index].time <= time)
					{
						startIndex = index;
					}
					else
					{
						endIndex = index;
					}
				}

				//���˴��������ҵ�ָ��ʱ��Ľ�ǰ�ؼ�֡��
				assert(index < m_fileHeader.indexNum);
				
				//�������ָ��ʱ��֮��Ĺؼ�֡������Ļ�����ʹ�ý���ָ��ʱ��֮��Ĺؼ�֡��
				if ((m_pIndexInfo[index].time < time) && ((index + 1) < m_fileHeader.indexNum))
				{
					LONGLONG d1 = time - m_pIndexInfo[index].time;
					LONGLONG d2 = m_pIndexInfo[index + 1].time - time;
					if (d1 > 300000000)
					{
						//���ָ��ʱ�����ǰ�ؼ�֡��ʱ��������5���ӣ���˵��¼���ļ�����
						//��ʱ����Ծ����ʹ�ý���ָ��ʱ��֮��Ĺؼ�֡
						//printf("(%s,%d)SeekKeyFrameToTime: file is not serial.use next frame\n",__FILE__,__LINE__);
						index += 1;
					}
					else if ((m_pIndexInfo[index + 1].time)/1000000 == (time / 1000000))
					{
						//���ָ��ʱ�������ָ��ʱ��֮��Ĺؼ�֡��ʱ�䣬���뵥λ����ȣ�
						//��ʹ�ý���ָ��ʱ��֮��Ĺؼ�֡
						//printf("(%s,%d)SeekKeyFrameToTime: next frame time (s) == seek time (s)\n",__FILE__,__LINE__);
						index += 1;
					}
					else if (d2 < d1)
					{
						//���ָ��ʱ�������ָ��ʱ��֮��Ĺؼ�֡��ʱ��ȽϽӽ��Ļ���
						//��ʹ�ý���ָ��ʱ��֮��Ĺؼ�֡
						//printf("(%s,%d)SeekKeyFrameToTime: next frame time is near seek time (s)\n",__FILE__,__LINE__);
						index += 1;
					}
				}
			}
			ret = ERR_READ_FILE_OK;
		}

		++ tryNum;
	} while(!m_bFileNormal && (tryNum <= MAX_READ_TRY_NUM));


	tryNum = 0;
	m_cutSize=0;
	do 
	{
#if REC_READ_DO_BUFFER
		if (0 < tryNum)
		{
			if (0 == fseek(m_pFile, m_pIndexInfo[index].position, SEEK_SET))
			{
				unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, m_pFile);
				if ((0 < len) && (len <= m_bufInfo.bufLen))
				{
					//�ɹ�����Ӧ�ø���һ�����ݳ��ȡ�
					m_bufInfo.filePos	= m_pIndexInfo[index].position;
					m_bufInfo.currPos	= 0;
					m_bufInfo.dataLen	= len;
				}
				else
				{
					return ERR_READ_FILE_READ_FAILED;
				}
			}
			else
			{
				return ERR_READ_FILE_SEEK_FAILED;
			}
		}
#endif

		if(0 != RECFseek(m_pFile, m_pIndexInfo[index].position, SEEK_SET))
		{
			m_cutSize=0;
			return ERR_READ_FILE_SEEK_FAILED;
		}

		memset(&m_curFrame, 0, m_fileHeader.frameInfoLen);
		if(1 != RECFread(&m_curFrame, m_fileHeader.frameInfoLen, 1, m_pFile))
		{
			m_cutSize=0;
			return ERR_READ_FILE_READ_FAILED;
		}

		if(IsValidDataFlag(m_curFrame.flag))
		{
			m_cutSize= m_pIndexInfo[index].position;
			return ret;
		}

		++ tryNum;
	} while(!m_bFileNormal && (0 < tryNum));

	return ERR_READ_FILE_EXCEPTION;	//�ɹ�
}

#if 0
//�÷���Ŀǰû��ʹ�ã��������⡣����û�м����ļ��Ƿ��Ѿ���д������Ȼ���ټ��س�����
//֮�����Ϣ��������ʱ���Σ�����ʹ��ExtractInfoFromFile�������档
/******************************************************************************
�������ܣ�������д���ļ��ض��ļ���������ˢ�¶�ģ��ʵ���ж���ĸ��ļ���֡��������

******************************************************************************/
bool CRecReader::RefreshIndex()
{
	//����ļ��򿪵�ʱ����Ѿ���д�������ļ�����Ӧ��ˢ�¡�
	if(m_bFileNormal)
	{
		return false;
	}

	if(0 != fseek(m_pFile, WRITE_FLAG_POS, SEEK_SET))
	{
		return false;
	}

	unsigned long writeFlag;
	if(1 != fread(&writeFlag, sizeof(long), 1, m_pFile))
	{
		return false;
	}

	//�ļ��ڶ��Ĺ������Ѿ�д����,�����¶����ļ���Ϣͷ��������Ϣ
	if(writeFlag == WRITE_END_FLAG)
	{
		if(0 != fseek(m_pFile, m_fileHeader.indexAddr, SEEK_SET))
		{
			return false;
		}

		//ֻ��ȡ�ϴ�δ��ȡ����֡����
		if(1 != fread(m_pIndexInfo + m_fileHeader.indexNum, sizeof(INDEX_INFO)*(MAX_KEY_FRAME_NUM - m_fileHeader.indexNum), 1, m_pFile))
		{
			return false;
		}

		if(0 != fseek(m_pFile, 0, SEEK_SET))
		{
			return false;
		}

		if(1 != fread(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
		{
			return false;
		}
	}
	else
	{
		if(0 != fseek(m_pFile, m_fileHeader.indexAddr, SEEK_SET))
		{
			return false;
		}

		//ֻ��ȡ�ϴ�δ��ȡ����֡����
		if(1 != fread(m_pIndexInfo + m_fileHeader.indexNum, sizeof(INDEX_INFO)*(MAX_KEY_FRAME_NUM - m_fileHeader.indexNum), 1, m_pFile))
		{
			return false;
		}

		int i = 0;
		for(i = 0; i < MAX_KEY_FRAME_NUM; i++)
		{
			if(m_pIndexInfo[i].flag != I_VIDEO_FLAG)
			{
				break;
			}
		}

		m_fileHeader.indexNum = i;
		m_fileHeader.videoNum = m_pIndexInfo[m_fileHeader.indexNum-1].index+1;
	}

	if(0 != fseek(m_pFile, m_curFrame.nextPos - m_curFrame.length, SEEK_SET))
	{
		return false;
	}

	return true;
}
#endif

/******************************************************************************
�������ܣ��޸�δд������¼���ļ�
ʵ��˵������ͳ������֡������ȡ�����������������ݣ����ݱ�ʶ���б��ͳ�Ƶ����е��������ݣ�
		  ����Ƶ����ͳ�ƣ�ֻ���ڸ�����Ƶͷ�жϵ�����Ƶʱ��������Ƶ��������һ�������������
		  ����Ƶ��ͳ�ƣ������һ֡������Ӧ������֡λ�ÿ�ʼ��˳���ȡ֡��Ϣ������
		  ��Ƶ֡�Ͷ���Ƶ֡����1���ҽ���֡ʱ����Ϊ�ļ��Ľ���ʱ�䣬ֱ��������һ֡��ʶ
		  Ϊ�Ƿ���ʶʱ�������ļ�������β����ʱ�����ļ�ͷ�е��ļ�����=���һ֡����һ֡λ��
		  ��ʼʱ����ڵ�һ�ؼ�֡ʱ�䣬���ļ�ͷ��Ϣ��д���ļ�ͷλ�á�
******************************************************************************/
bool CRecReader::Repair(const char *pFilePath, LONGLONG *pEndTime)
{
	FILE *pFile = NULL;
	REC_FILE_HEADER fileHeader;
	WAVEFORMATEX waveInfo;
	INDEX_INFO *pIndexInfo = NULL;
	REC_FRAME_INFO frameInfo;
	unsigned long max_key_frame_num = 0;

	*pEndTime = 0;

	pFile = fopen(pFilePath, "rb+");
	if(pFile == NULL)
	{
		return false;
	}

	if(0 != fseek(pFile, 0, SEEK_SET))
	{
		goto ERROR_PROC;
	}

	if(1 != fread(&fileHeader, sizeof(REC_FILE_HEADER), 1, pFile))
	{
		goto ERROR_PROC;
	}

	if(fileHeader.fileFlag != FILE_FLAG)
	{
		goto ERROR_PROC;
	}

	if((fileHeader.writeFlg == WRITE_END_FLAG) && (0 < fileHeader.startTime) && (fileHeader.startTime < GetCurrTime()) && (fileHeader.startTime < fileHeader.endTime))
	{
		*pEndTime = fileHeader.endTime;
		fclose(pFile);
		return true;
	}

	if(0 != fseek(pFile, fileHeader.audioFormatAddr, SEEK_SET))
	{
		goto ERROR_PROC;
	}

	if(1 != fread(&waveInfo, sizeof(waveInfo), 1, pFile))
	{
		goto ERROR_PROC;
	}

	if(0 != fseek(pFile, fileHeader.indexAddr, SEEK_SET))
	{
		goto ERROR_PROC;
	}

	max_key_frame_num = (fileHeader.videoFormatAddr - sizeof (REC_FILE_HEADER) - 32) / sizeof(INDEX_INFO);

	pIndexInfo = new INDEX_INFO[max_key_frame_num];
	if(pIndexInfo == NULL)
	{
		goto ERROR_PROC;
	}
	if(1 != fread(pIndexInfo, sizeof(INDEX_INFO) * max_key_frame_num, 1, pFile))
	{
		goto ERROR_PROC;
	}

	fileHeader.audioNum = (waveInfo.nAvgBytesPerSec == 0) ? 0 : 0x0fffffff;

	//ͳ������֡��
	for(fileHeader.indexNum = 0; fileHeader.indexNum < max_key_frame_num; fileHeader.indexNum++)
	{
		if(pIndexInfo[fileHeader.indexNum].flag != I_VIDEO_FLAG)
		{
			break;
		}
	}

	if(fileHeader.indexNum == 0)
	{
		goto ERROR_PROC;
	}

	if (0 == fileHeader.startTime)
	{
		fileHeader.startTime = pIndexInfo[0].time;	//���ļ��Ŀ�ʼʱ��Ϊ��һ֡ʱ��
	}
	else
	{

	}

	//�����һ��֡������Ӧ��֡����λ�ÿ�ʼ�����������
	{
#if defined(__TDFH__) || defined(__CHIPTI__)
		REC_FRAME_INFO lastFrameInfo;
		memset(&lastFrameInfo, 0, sizeof(REC_FRAME_INFO));
#endif
		unsigned long i = fileHeader.indexNum;
		do 
		{
			fileHeader.indexNum = i;
			if (0 == i)
			{
				goto ERROR_PROC;
			}
			-- i;


			fileHeader.videoNum = pIndexInfo[i].index;
			if(0 != fseek(pFile, pIndexInfo[i].position, SEEK_SET))
			{
				goto ERROR_PROC;
			}

			int pCount = 0;
			while(1 == fread(&frameInfo, fileHeader.frameInfoLen, 1, pFile))
			{
				if(!IsValidDataFlag(frameInfo.flag))
				{
					printf("%s:%s:%d, The frame is invalid data, flag=0x%08x, i = %d, pCount=%d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i, pCount);
					//��ʱ��ʱ������ϸ�֡��ʱ�����Ӧ��������ʱ��.���flag��0���������ʱ�䲻����˵��֡ʱ�����
					PrintfBit64(fileHeader.startTime, true);
					PrintfBit64(fileHeader.endTime, true);
					PrintfBit64(frameInfo.time, true);

					break;
				}

				if (frameInfo.time < fileHeader.startTime)
				{
					//ʱ����������쳣���
					printf("%s:%s:%d, The frame time is < startTime, flag=0x%08x, i = %d, pCount=%d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i, pCount);
					PrintfBit64(fileHeader.startTime, true);
					PrintfBit64(fileHeader.endTime, true);
					PrintfBit64(frameInfo.time, true);

					PrintfBit64(fileHeader.startTime, false);
					PrintfBit64(fileHeader.endTime, false);
					PrintfBit64(frameInfo.time, false);

					break;
				}

#if defined(__TDFH__) || defined(__CHIPTI__)
				//��ƽ̨��timeStampFlag����У�麯����
				//����ƽ̨�Ͳ�����У���ˡ�ԭ���ǣ�Ҫ����֮ǰ��¼���ļ�
				unsigned long frameInfoChsum = frameInfo.timeStampFlag;

				frameInfo.timeStampFlag = 0;
				frameInfo.timeStampFlag = RecFrameInfoChsum(&frameInfo);

				//printf("%s:%s:%d, check  rec frame info error!frameInfoChsum=(0x%08x 0x%08x) \n", __FUNCTION__, __FILE__, __LINE__, frameInfoChsum, frameInfo.timeStampFlag);

				if (frameInfoChsum != frameInfo.timeStampFlag)
				{
					printf("%s:%s:%d, check  rec frame info error!frameInfoChsum=(0x%08x 0x%08x) \n", __FUNCTION__, __FILE__, __LINE__, frameInfoChsum, frameInfo.timeStampFlag);
					PrintfBit64(fileHeader.startTime, true);
					PrintfBit64(fileHeader.endTime, true);
					PrintfBit64(frameInfo.time, true);
					break;
				}

				//�����һ֡��ʱ���Ƿ�ȹؼ�֡��ʱ��С,����ȹؼ�֡��ʱ��С������
				//�����һ֡��ʱ��ȹؼ�֡��ʱ�����10������������Ϊ������
				//10����̫С����FHƽ̨������Ƶ��¼���ļ���¼��ʱ�䳬��10����ʱ�쳣�ϵ磬���ļ��޸�ʱ�ᷢ���е��ļ�
				//ֻдһ�����ؼ�֡�����������д�ؼ�֡ʱ��sync()������ˢ�����ݵ�Ӳ�̣�Ҳ�����ֻдһ���ֹؼ�֡������ͨ����
				//����¼����15���ӣ��е�ͨ��ֻ��40����ؼ�֡������������ͨ������150�����ϡ�FHƽ̨120֡һ���ؼ�֡
				//����Ҫ�Ӵ�ʱ���⣬�ر�ʱ�ڴ�����Ƶ��ʧ���ļ�ʱ��Ӧ�üӴ�.���ӵ�10��Сʱ
				if (pIndexInfo[i].time > frameInfo.time || 
					((frameInfo.time - pIndexInfo[i].time) / 1000000) > 36000)
				{
					printf("%s:%s:%d, The frame time is error! flag=0x%08x, i = %d , pCount=%d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i, pCount);
					PrintfBit64(frameInfo.time, true);
					PrintfBit64(pIndexInfo[i].time, true);

					PrintfBit64(fileHeader.startTime, true);
					PrintfBit64(fileHeader.endTime, true);
					PrintfBit64(frameInfo.time, true);
					break;
				}
#else
				if (frameInfo.timeStampFlag != 1)
				{
					printf("%s,%d, check  rec frame info error!timeStampFlag\n", __FILE__,__LINE__);
					PrintfBit64(fileHeader.startTime, false);
					PrintfBit64(fileHeader.endTime, false);
					PrintfBit64(frameInfo.time, false);
					break;
				}

				//Ŀǰ��֡�ĳ��������1M��
				if (frameInfo.length  > 10*1024*1024)
				{
					printf("%s,%d,frame length error:%d\n",__FILE__,__LINE__,frameInfo.length);
					break;
				}

				if((frameInfo.flag == I_VIDEO_FLAG) || (frameInfo.flag == B_VIDEO_FLAG))
				{
					if (frameInfo.time < pIndexInfo[i].time)
					{
						printf("%s,%d, frameInfo.time < pIndexInfo[%d].time,type0x%08x\n",__FILE__,__LINE__,i,frameInfo.flag);
						PrintfBit64(fileHeader.startTime, false);
						PrintfBit64(fileHeader.endTime, false);
						PrintfBit64(frameInfo.time, false);
						PrintfBit64(pIndexInfo[i].time, false);
						break;
					}

					//�����һ֡��ʱ��ȹؼ�֡��ʱ�����1440����(24Сʱ)��������Ϊ������
					if ((frameInfo.time - pIndexInfo[i].time)/1000000 > 86400)
					{
						printf("%s,%d, frame time >>>pIndexInfo[%d].time\n",__FILE__,__LINE__,i);
						PrintfBit64(fileHeader.startTime, false);
						PrintfBit64(fileHeader.endTime, false);
						PrintfBit64(frameInfo.time, false);
						PrintfBit64(pIndexInfo[i].time, false);
						break;
					}
				}
#endif

				fileHeader.fileLen = frameInfo.nextPos;
				if((frameInfo.flag == I_VIDEO_FLAG) || (frameInfo.flag == B_VIDEO_FLAG))
				{
					fileHeader.videoNum++;
					fileHeader.endTime = frameInfo.time;
#if defined(__TDFH__) || defined(__CHIPTI__)
					memcpy(&lastFrameInfo, &frameInfo, sizeof(REC_FRAME_INFO));
#endif
				}


				pCount++;
				if(0 != fseek(pFile, frameInfo.nextPos, SEEK_SET))
				{
					break;
				}

			}
		} while(0 == fileHeader.endTime);

#if defined(__TDFH__) || defined(__CHIPTI__)
		//FHƽ̨���޸�������ȥ�����10֡����ֹ��������������
		//��λ����ǰ��������֡����10֡
		int dropCount = 0;
		
		while (0 != lastFrameInfo.prevPos && 10 > dropCount && lastFrameInfo.flag != I_VIDEO_FLAG)
		{
			dropCount++;
			if( 0 != fseek(pFile, lastFrameInfo.prevPos, SEEK_SET) ||
				1 != fread(&lastFrameInfo, fileHeader.frameInfoLen, 1, pFile))
			{
				goto ERROR_PROC;
			}
		}

		fileHeader.fileLen = lastFrameInfo.nextPos;
		fileHeader.endTime = lastFrameInfo.time;
		if (fileHeader.videoNum > dropCount)
		{
			fileHeader.videoNum -= dropCount;
		}
		else
		{
			goto ERROR_PROC;
		}
#endif
	}

	//���ļ�ͷ��Ϣ��д���ļ�ͷλ��
	fileHeader.writeFlg = WRITE_END_FLAG;
	if(0 != fseek(pFile, 0, SEEK_SET))
	{
		goto ERROR_PROC;
	}

	if(1 != fwrite(&fileHeader, sizeof(REC_FILE_HEADER), 1, pFile))
	{
		goto ERROR_PROC;
	}

	fclose(pFile);
	delete [] pIndexInfo;

	//����������ֵ
	*pEndTime = fileHeader.endTime;

	return true;

ERROR_PROC:
	{
		fclose(pFile);

		if(pIndexInfo != NULL)
		{
			delete [] pIndexInfo;
		}
		return false;
	}
	
}

/******************************************************************************

						protected functions

******************************************************************************/

/******************************************************************************
�������ܣ����ļ��ж�ȡ�ļ�ͷ��Ϣ����Ƶ��ʽ����Ƶ��ʽ����������
ʵ��˵��������д�������ļ���ֱ�Ӷ�ȡ����ȷ���������ݡ�����֡������Ƶ֡������Ƶ֡��
		  ��δд�������ļ�����Ҫͳ�ơ�
		  ͳ�Ƶķ���Ϊ���ȶ�ȡ�����������������ݣ����ݱ�ʶ���б��ͳ�Ƶ����е��������ݣ�
		  Ȼ������������һ֡��֡��ż�1������ǰ��Ƶ����֡����
		  ������Ƶ����ͳ�ƣ�ֻ���ڸ�����Ƶͷ�жϵ�����Ƶʱ��������Ƶ��������һ�������������
		  
		  ��ͳ�Ƴ���Ƶ����Ƶ������Ŀ��ԭ���ǣ����ļ��ĸ��ֶ�ȡ����λ�����У����ò���
		  ���������ݣ���ֻ�����ж��ļ�������Ƶ��Ƶ����ʱ�����ϡ�

******************************************************************************/
bool CRecReader::ExtractInfoFromFile()
{
	//��δ��룬������ļ������ظ�������Ƕ�ȡ�����У����¼����ļ���Ϣ�����Ƕ���
	//��Ϊ��ȡ�����У�����д�߳��Ѿ��Ѹ��ļ�д�����ˣ�Ӧ���ʵ����޸ı�־��
	{
		//��ȡ�ļ�ͷ��Ϣ
		if(0 != fseek(m_pFile, 0, SEEK_SET))
		{
			return false;
		}

		if(1 != fread(&m_fileHeader, sizeof(REC_FILE_HEADER), 1, m_pFile))
		{
			return false;
		}

		//�ж��ļ��Ƿ���д��
		if(m_fileHeader.writeFlg == WRITE_END_FLAG)	//�ļ�δд����
		{
			m_bFileNormal = true;
		}
		else
		{
			m_bFileNormal = false;
		}
	}

	//��ȡ��Ƶ��Ϣ
	if(0 != fseek(m_pFile, m_fileHeader.videoFormatAddr, SEEK_SET))
	{
		return false;
	}

	if(1 != fread(&m_bitmapInfo, sizeof(BITMAPINFOHEADER), 1, m_pFile))
	{
		return false;
	}

	//��ȡ��Ƶ��Ϣ
	if(0 != fseek(m_pFile, m_fileHeader.audioFormatAddr, SEEK_SET))
	{
		return false;
	}

	if(1 != fread(&m_waveInfo, sizeof(WAVEFORMATEX), 1, m_pFile))
	{
		return false;
	}

	unsigned long max_key_frame_num = (m_fileHeader.videoFormatAddr - sizeof (REC_FILE_HEADER) - 32) / sizeof(INDEX_INFO);

	//��ȡ������Ϣ
	if(m_bFileNormal)
	{
		if(0 != fseek(m_pFile, m_fileHeader.indexAddr, SEEK_SET))
		{
			return false;
		}

		if (NULL != m_pIndexInfo)
		{
			delete [] m_pIndexInfo;
			m_pIndexInfo = NULL;
		}

		//2010-02-08 21:32:00 YSW
		//���ؼ�֡��Ŀ�Ƿ�Ϸ�
		if ((0 < m_fileHeader.indexNum) && (m_fileHeader.indexNum <= max_key_frame_num))
		{
			m_pIndexInfo = new INDEX_INFO[m_fileHeader.indexNum];
			if(m_pIndexInfo == NULL)
			{
				assert(false);
				return false;
			}

			if(1 != fread(m_pIndexInfo, sizeof(INDEX_INFO)*m_fileHeader.indexNum, 1, m_pFile))
			{
				delete [] m_pIndexInfo;
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		assert(0 != m_fileHeader.startTime);
		assert(0 == m_fileHeader.endTime);
		if (0 == m_fileHeader.startTime)
		{
			return false;
		}

		if(0 != fseek(m_pFile, m_fileHeader.indexAddr, SEEK_SET))
		{
			return false;
		}

		if (NULL != m_pIndexInfo)
		{
			delete [] m_pIndexInfo;
			m_pIndexInfo = NULL;
		}

		m_pIndexInfo = new INDEX_INFO[max_key_frame_num];
		if(m_pIndexInfo == NULL)
		{
			assert(false);
			return false;
		}

		if(1 != fread(m_pIndexInfo, sizeof(INDEX_INFO)*max_key_frame_num, 1, m_pFile))
		{
			delete [] m_pIndexInfo;
			return false;
		}

		int i = 0;
		for(i = 0; i < max_key_frame_num; i++)
		{
			if(m_pIndexInfo[i].flag != I_VIDEO_FLAG)
			{
				break;
			}
		}
		
		//m_fileHeader.indexNum = i;
		//m_fileHeader.videoNum = m_pIndexInfo[m_fileHeader.indexNum-1].index+1;
		if(m_waveInfo.nAvgBytesPerSec != 0)
		{
			m_fileHeader.audioNum = 0x0fffffff;
		}

		//����recWriter�п���ʹ�û���д�ķ�ʽ������ÿ�ζ�д������Ҫ�Ȼ��������д��
		//����Ҫ��ʵ�ʵ�д��Ӳ��������ݣ��õ�����ʱ��͹ؼ�֡��
		while (i > 0)
		{
			if(0 == fseek(m_pFile, m_pIndexInfo[i - 1].position, SEEK_SET))
			{
				m_fileHeader.videoNum = m_pIndexInfo[i - 1].index;
				
				REC_FRAME_INFO frameInfo;
				while(1 == fread(&frameInfo, m_fileHeader.frameInfoLen, 1, m_pFile))
				{
					if(!IsValidDataFlag(frameInfo.flag))
					{
						//����д���ļ����������������ط���flagӦ����0��������ﲻ��0�����ʾ¼��ʱûд������Ϣ
						printf("%s:%s:%d, The frame is invalid data, flag=0x%08x, i = %d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i);
						//��ʱ��ʱ������ϸ�֡��ʱ�����Ӧ��������ʱ��.���flag��0���������ʱ�䲻����˵��֡ʱ�����
						PrintfBit64(m_fileHeader.startTime, true);
						PrintfBit64(m_fileHeader.endTime, true);
						PrintfBit64(frameInfo.time, true);

						break;
					}

					if (frameInfo.time < m_fileHeader.startTime)
					{
						//ʱ����������쳣���
						printf("%s:%s:%d, The frame time is < startTime, flag=0x%08x, i = %d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i);
						PrintfBit64(m_fileHeader.startTime, true);
						PrintfBit64(m_fileHeader.endTime, true);
						PrintfBit64(frameInfo.time, true);

						break;
					}

#if defined(__TDFH__) || defined(__CHIPTI__)
					unsigned long frameInfoChsum = frameInfo.timeStampFlag;

					frameInfo.timeStampFlag = 0;
					frameInfo.timeStampFlag = RecFrameInfoChsum(&frameInfo);

					//printf("%s:%s:%d, check  rec frame info error!frameInfoChsum=(0x%08x 0x%08x) \n", __FUNCTION__, __FILE__, __LINE__, frameInfoChsum, frameInfo.timeStampFlag);

					if (frameInfoChsum != frameInfo.timeStampFlag)
					{
						printf("%s:%s:%d, check  rec frame info error!frameInfoChsum=(0x%08x 0x%08x) \n", __FUNCTION__, __FILE__, __LINE__, frameInfoChsum, frameInfo.timeStampFlag);
						PrintfBit64(m_fileHeader.startTime, true);
						PrintfBit64(m_fileHeader.endTime, true);
						PrintfBit64(frameInfo.time, true);
						break;
					}

					//�����һ֡��ʱ���Ƿ�ȹؼ�֡��ʱ��С,����ȹؼ�֡��ʱ��С������
					//�����һ֡��ʱ��ȹؼ�֡��ʱ���10Сʱ��������Ϊ������
					if (m_pIndexInfo[i - 1].time > frameInfo.time || 
						((frameInfo.time - m_pIndexInfo[i-1].time) / 1000000) > 36000)
					{
						printf("%s:%s:%d, The frame time is error! flag=0x%08x, i = %d\n", __FUNCTION__, __FILE__, __LINE__, frameInfo.flag, i);
						PrintfBit64(frameInfo.time, true);
						PrintfBit64(m_pIndexInfo[i-1].time, true);

						PrintfBit64(m_fileHeader.startTime, true);
						PrintfBit64(m_fileHeader.endTime, true);
						PrintfBit64(frameInfo.time, true);
						break;
					}

#else
					if (frameInfo.timeStampFlag != 1)
					{
						printf("%s,%d, check  rec frame info error!timeStampFlag\n", __FILE__,__LINE__);
						PrintfBit64(m_fileHeader.startTime, false);
						PrintfBit64(m_fileHeader.endTime, false);
						PrintfBit64(frameInfo.time, false);
						break;
					}

					//Ŀǰ��֡�ĳ��������1M��
					if (frameInfo.length  > 10*1024*1024)
					{
						printf("%s,%d,frame length error:%d\n",__FILE__,__LINE__,frameInfo.length);
						break;
					}

					if((frameInfo.flag == I_VIDEO_FLAG) || (frameInfo.flag == B_VIDEO_FLAG))
					{
						if (frameInfo.time < m_pIndexInfo[i-1].time)
						{
							printf("%s,%d, frameInfo.time < m_pIndexInf[%d-1].time,type0x%08x\n",__FILE__,__LINE__,i,frameInfo.flag);
							PrintfBit64(m_fileHeader.startTime, false);
							PrintfBit64(m_fileHeader.endTime, false);
							PrintfBit64(frameInfo.time, false);
							PrintfBit64(m_pIndexInfo[i-1].time, false);
							break;
						}

						//�����һ֡��ʱ��ȹؼ�֡��ʱ�����1440������������Ϊ������
						if ((frameInfo.time - m_pIndexInfo[i-1].time)/1000000 > 86400)
						{
							printf("%s,%d, frame time >>>m_pIndexInfo[%d-1].time\n",__FILE__,__LINE__,i);
							PrintfBit64(m_fileHeader.startTime, false);
							PrintfBit64(m_fileHeader.endTime, false);
							PrintfBit64(frameInfo.time, false);
							PrintfBit64(m_pIndexInfo[i-1].time, false);
							break;
						}
					}

#endif

					m_fileHeader.fileLen = frameInfo.nextPos;
					if((frameInfo.flag == I_VIDEO_FLAG) || (frameInfo.flag == B_VIDEO_FLAG))
					{
						m_fileHeader.videoNum ++;
						m_fileHeader.endTime = frameInfo.time;
					}
					
					if(0 != fseek(m_pFile, frameInfo.nextPos, SEEK_SET))
					{
						break;
					}
				}
				
				if (0 != m_fileHeader.endTime)
				{
					m_fileHeader.indexNum = i;
					break;
				}
			}

			i--;
		}

		if (0 == i)
		{
			delete [] m_pIndexInfo;
			m_pIndexInfo = NULL;

			return false;
		}

		
		assert(m_fileHeader.endTime >= m_fileHeader.startTime);
	}

	return true;
}

//�ж��ǲ�����ȷ��֡��ʶ
bool CRecReader::IsValidDataFlag(unsigned long flag)
{
	if((flag == I_VIDEO_FLAG) || (flag == B_VIDEO_FLAG) || (flag == I_AUDIO_FLAG) || (flag == I_TEXT_FLAG))
	{
		return true;
	}
	return false;
}


/******************************************************************************

					������ƴ�������ݹ���(���� ���������� ͬ������)

******************************************************************************/

int CRecReader::RECFseek(FILE *pFile, long offset, int seek)
{
#if REC_READ_DO_BUFFER
	if ((m_bufInfo.filePos <= offset) && (offset < (m_bufInfo.filePos + m_bufInfo.dataLen)))
	{
		m_bufInfo.currPos = (offset - m_bufInfo.filePos);
		return 0;
	}
	else
	{
		if (0 == fseek(pFile, offset, SEEK_SET))
		{
			unsigned long len = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, pFile);
			if ((0 < len) && (len <= m_bufInfo.bufLen))
			{
				m_bufInfo.dataLen = len;
				m_bufInfo.filePos = offset;
				m_bufInfo.currPos = 0;

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
#else
	return fseek(pFile, offset, seek);
#endif
}

size_t CRecReader::RECFread(void *pBuf, size_t len, size_t count, FILE *pFile)
{
#if REC_READ_DO_BUFFER
	assert (m_bufInfo.currPos <= m_bufInfo.dataLen);
	
	size_t length = (len * count);

	if ((m_bufInfo.currPos + length) <= m_bufInfo.dataLen)
	{
		memcpy(pBuf, m_bufInfo.pData + m_bufInfo.currPos, length);
		m_bufInfo.currPos  += length;

		return count;
	}
	else
	{
		unsigned long readLen = (m_bufInfo.dataLen - m_bufInfo.currPos);
		memcpy(pBuf, m_bufInfo.pData + m_bufInfo.currPos, readLen);
		m_bufInfo.currPos  += readLen;

		while (readLen < length)
		{
			if (0 == fseek(pFile, m_bufInfo.filePos + m_bufInfo.currPos, SEEK_SET))
			{
				size_t dataLen = fread(m_bufInfo.pData, 1, m_bufInfo.bufLen, pFile);
				if (0 < dataLen)
				{
					m_bufInfo.dataLen = dataLen;
					m_bufInfo.filePos += m_bufInfo.currPos;
					m_bufInfo.currPos = 0;

					unsigned long subLen = (length - readLen);
					subLen = (subLen <= dataLen) ? subLen : dataLen;
					memcpy((unsigned char *)pBuf+readLen, m_bufInfo.pData, subLen);

					readLen += subLen;
					m_bufInfo.currPos = subLen;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		return (readLen / len);
	}
#else
	return fread(pBuf, len, count, pFile);
#endif
}

unsigned long CRecReader::RECFtell()
{
	return m_cutSize;
}
