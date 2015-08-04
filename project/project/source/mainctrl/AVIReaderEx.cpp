// AVIReaderEx.cpp: implementation of the AVIReader class.
//
//////////////////////////////////////////////////////////////////////

#include "AVIReaderEx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAVIReader::CAVIReader()
{
	m_pFile = NULL;
	m_bFileOpen = false;
	m_pVideoIndex = NULL;
	m_pAudioIndex = NULL;

	m_bCanRead     = true;
	m_read_handle  = NULL;
	Thread_read_ID = 0;
	m_thread_read  = false;

	m_ReadEvent.CreateWaitEvent();
}

CAVIReader::~CAVIReader()
{

}

//////////////////////////////////////////////////////////////////////////
//	public

/**************************************************************************
* �������ƣ� int Open(const char *pstrFilename,int &ver,bool isRead)
* ���������� 
* ���ʵı� 
* �޸ĵı� 
* �������
* ��������� 
* �� �� ֵ��EFOPEN_OK--�ɹ�  EFOPEN_ERROR--�ļ���ʧ��  EFFRMT_ERROR--��ʽ����  
*			EFILE_DAMAGE--�ļ����𣬽���ر��޸����´�
* ����˵���� 
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2002/08/01	     V1.0	    XXXX	      XXXX
**************************************************************************/
bool CAVIReader::Open(const char * pFilePath)
{
#if TEST
	assert(NULL != pFilePath);
	assert(!m_bFileOpen);
	assert(m_pFile == NULL);
#endif

#if AVI_READER_FILE_NAME
	printf("Open AVI file %s ...\n", pstrFilename);
#endif

	m_aCurIndex = 0;
	m_vCurIndex = 0;
	memset(&m_curReadState, 0, sizeof(m_curReadState));
	ULONG EndMark;

	m_pFile = fopen(pFilePath, "rb");
	if(m_pFile == NULL)
		return false;
	
	if(fread(m_flagRIFF, sizeof(m_flagRIFF), 1, m_pFile) < 1)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}
	if( (m_flagRIFF[0] != FLAG_RIFF) ||(m_flagRIFF[2] != formtypeAVI) )
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}
	
	if(fseek(m_pFile, POS_AVINORMAL, SEEK_SET) !=0)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}
	if(fread(&EndMark, sizeof(ULONG), 1, m_pFile) < 1)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}
	if(EndMark != AVI_NORMAL)
	{
		m_bEndNormal = false;
		//			return EFILE_DAMAGE;
	}
	else
	{
		m_bEndNormal = true;
	}
	
	//	���ļ��汾��
	if (0 != fseek(m_pFile, POS_AVIVER, SEEK_SET))
		return false;
	unsigned long ver = AVI_VERSION;
	if(fread(&ver, SIZE_AVIVER, 1, m_pFile) < 1)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}
	else
	{
		m_FileVer=ver;
		if(m_FileVer == AVI_VERSION1)
			m_TimeBufLen=sizeof(LONGLONG);
		else
			m_TimeBufLen=TimeBufLen;
	}
	
	m_bFileOpen = true;

	ReadHeaderInfo();

	if(m_flagRIFF[1] == (AVIFILELENGTH - 2*sizeof(unsigned long)))	//64M DVR File
	{
		ReadIndexItem();
	}
	else if(m_nVideoFrames != 0)
	{
		ReadIndexItemEx();
	}
	else
	{
		m_bFileOpen = false;
		fclose(m_pFile);
		m_pFile = NULL;
		return false;
	}
	
	memset(m_ReadBuf, 0, AVI_READ_BUF_LEN);
	m_bufHeadOffset = 0;
	m_bufTailOffset = 0;
	m_currOffset    = 0;
	m_preReadType	= FRAME_TYPE_NONE;
	
	if (CreateReadThd())
	{
		ReadCurVideoFrmInfo();
		ReadCurAudioFrmInfo();
		return true;
	}
	else
	{
		fclose(m_pFile);
		m_pFile = NULL;
		m_bFileOpen = false;
		return false;
	}
}
/**************************************************************************
* �������ƣ� int GetFrameNum(int StreamType)
* ���������� 
* ���ʵı� 
* �޸ĵı� 
* ��������������ͣ�streamtypeAUDIO--��Ƶ��, streamtypeVIDIO--��Ƶ��
* ��������� 
* �� �� ֵ��ָ��������֡�� 
* ����˵���� 
* �޸�����    �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2002/08/01	     V1.0	    XXXX	      XXXX
**************************************************************************/

unsigned long CAVIReader::GetFrameNum(FRAME_TYPE type)
{
	if(type == FRAME_TYPE_VIDEO)
	{
		return m_nVideoFrames;
	}
	else if(type == FRAME_TYPE_AUDIO)
	{
		return m_nAudioFrames;
	}
	else
	{
		assert(false);
		return 0;
	}
}

unsigned long CAVIReader::GetVideoInfo(BITMAPINFOHEADER *pBitmapInfo)
{
	assert(m_bFileOpen);
	if(!m_bFileOpen)
	{
		return ERR_READ_FILE_NO_OPEN;
	}
	
	memcpy(pBitmapInfo, &m_bmpInfoHeader, sizeof(BITMAPINFOHEADER));
	return ERR_READ_FILE_OK;
}

unsigned long CAVIReader::GetAudioInfo(WAVEFORMATEX *pWaveInfo)
{
	assert(m_bFileOpen);
	if(!m_bFileOpen)
	{
		return ERR_READ_FILE_NO_OPEN;
	}
	
	if (0 == m_nAudioFrames)
	{
		return ERR_READ_FILE_NO_AUDIO;
	}
	else
	{
		memcpy(pWaveInfo, &m_waveFormat, sizeof(WAVEFORMATEX));
		return ERR_READ_FILE_OK;
	}	
}

FRAME_TYPE CAVIReader::GetCurrFrameType()
{
	if(m_pFile == NULL)
	{
		assert(false);
		return FRAME_TYPE_NONE;
	}

	if(m_curReadState.videoTime == 0)
	{
// 		assert(false);
// 		return FRAME_TYPE_NONE;
		if(m_curReadState.audioTime != 0)
		{
			return FRAME_TYPE_AUDIO;
		}
		return FRAME_TYPE_NONE;
	}
	if((m_curReadState.audioTime != 0) && (m_curReadState.audioTime < m_curReadState.videoTime))
	{
		return FRAME_TYPE_AUDIO;
	}
	
	return FRAME_TYPE_VIDEO;
}

unsigned long CAVIReader::GetCurrFrameLen()
{
	if(m_pFile == NULL)
	{
		assert(false);
		return 0;
	}

	if(m_curReadState.videoTime == 0)
	{
// 		assert(false);
// 		return 0;
		if(m_curReadState.audioTime != 0)
		{
			return m_curReadState.audioFrmSize;
		}
		return 0;
	}
	if((m_curReadState.audioTime != 0) && (m_curReadState.audioTime < m_curReadState.videoTime))
	{
		return m_curReadState.audioFrmSize;
	}
	
	return m_curReadState.videoFrmSize;
}

LONGLONG CAVIReader::GetCurrFrameTime()
{
	if(m_pFile == NULL)
	{
		assert(false);
		return 0;
	}

	if(m_curReadState.videoTime == 0)
	{
//		assert(false);
//		return 0;
		return m_curReadState.audioTime;
	}
	if((m_curReadState.audioTime != 0) && (m_curReadState.audioTime < m_curReadState.videoTime))
	{
		return m_curReadState.audioTime;
	}
	
	return m_curReadState.videoTime;
}

unsigned long CAVIReader::GetCurrFrame(FRAME_INFO_EX & frameInfo)
{
// 	if(m_curReadState.videoTime == 0)
// 	{
// 		assert(false);
// 		return ERR_READ_FILE_EXCEPTION;
// 	}
	if(m_curReadState.audioTime != 0)
	{
		if((m_curReadState.audioTime < m_curReadState.videoTime) || (m_curReadState.videoTime == 0))
		{
			if(frameInfo.length < m_curReadState.audioFrmSize)
			{
				assert(false);
				return ERR_READ_FILE_BUF_SMALL;
			}
			m_preReadType = FRAME_TYPE_AUDIO;
			frameInfo.frameType = FRAME_TYPE_AUDIO;
			frameInfo.time = m_curReadState.audioTime;
			frameInfo.length = m_curReadState.audioFrmSize;
			frameInfo.keyFrame = m_curReadState.audioKeyFlag;
			frameInfo.frameIndex = m_aCurIndex;
			if(m_aCurIndex >= m_nAudioFrames)
			{
				return ERR_READ_FILE_SEEK_TO_END;
			}
			
			if(m_nAudioFrames == 0)
			{
				assert(false);
			}
			if (0 != AVIFseek(m_pFile, MOVI_POS_OFFSET-4+m_pAudioIndex[m_aCurIndex].dwChunkOffset+sizeof(m_flagDataBlock), SEEK_SET))
			{
				return ERR_READ_FILE_SEEK_FAILED;
			}

			if (1 != AVIFread(frameInfo.pData, frameInfo.length, 1, m_pFile))
			{
				return ERR_READ_FILE_READ_FAILED;
			}

			return ERR_READ_FILE_OK;
		}
	}

	if(frameInfo.length < m_curReadState.videoFrmSize)
	{
		assert(false);
		return ERR_READ_FILE_BUF_SMALL;
	}

	m_preReadType = FRAME_TYPE_VIDEO;
	frameInfo.frameType = FRAME_TYPE_VIDEO;
	frameInfo.time = m_curReadState.videoTime;
	frameInfo.length = m_curReadState.videoFrmSize;
	frameInfo.keyFrame = m_curReadState.videoKeyFlag;
	frameInfo.width = m_bmpInfoHeader.biWidth;
	frameInfo.height = m_bmpInfoHeader.biHeight;
	frameInfo.frameIndex = m_vCurIndex;
	if(m_vCurIndex >= m_nVideoFrames)
	{
		return ERR_READ_FILE_SEEK_TO_END;
	}
	
	if (0 != AVIFseek(m_pFile, MOVI_POS_OFFSET-4+m_pVideoIndex[m_vCurIndex].dwChunkOffset+sizeof(m_flagDataBlock), SEEK_SET))
	{
		return ERR_READ_FILE_SEEK_FAILED;
	}

	if (1 != AVIFread(frameInfo.pData, frameInfo.length, 1, m_pFile))
	{
		return ERR_READ_FILE_READ_FAILED;
	}

	return ERR_READ_FILE_OK;
}

unsigned long CAVIReader::SeekVideoFrame(SEEK_MODE mode, unsigned long seekNum /* = 1 */)
{
	if(m_pFile == NULL)
	{
		assert(false);
		return ERR_READ_FILE_NO_OPEN;
	}

	memset(&m_curReadState, 0, sizeof(m_curReadState));
	m_preReadType = FRAME_TYPE_NONE;

	if(mode == SEEK_NEXT_VFRM)
	{
		if(m_vCurIndex+1 >= m_nVideoFrames)
		{
			return ERR_READ_FILE_SEEK_TO_END;
		}
		m_vCurIndex++;
	}
	else if(mode == SEEK_PREV_VFRM)
	{
		if(m_vCurIndex == 0)
		{
			return ERR_READ_FILE_SEEK_TO_BEGIN;
		}
		m_vCurIndex--;
	}
	else	//�ؼ�֡��λ
	{
		if(mode == SEEK_NEXT_KEY)
		{
			unsigned long num = 0;
			while((num < seekNum) && ((++m_vCurIndex) < m_nVideoFrames))
			{
				if(m_pVideoIndex[m_vCurIndex].dwFlags == AVIIF_KEYFRAME)
				{
					num++;
				}
			}
			if(m_vCurIndex >= m_nVideoFrames)
			{
				return ERR_READ_FILE_SEEK_TO_END;
			}
		}
		else if(mode == SEEK_PREV_KEY)
		{
			unsigned long num = 0;
			while((num < seekNum) && ((--m_vCurIndex) >= 0))
			{
				if(m_pVideoIndex[m_vCurIndex].dwFlags == AVIIF_KEYFRAME)
				{
					num++;
				}
			}
			if(m_vCurIndex < 0)
			{
				return ERR_READ_FILE_SEEK_TO_BEGIN;
			}
		}
		else if(mode == SEEK_FIRST_KEY)
		{
			m_vCurIndex = 0;
		}
		else if(mode == SEEK_LAST_KEY)
		{
			m_vCurIndex = m_nVideoFrames-1;
			while(m_vCurIndex >= 0)
			{
				if(m_pVideoIndex[m_vCurIndex].dwFlags == AVIIF_KEYFRAME)
				{
					break;
				}
				m_vCurIndex--;
			}
			if(m_vCurIndex < 0)
			{
				return ERR_READ_FILE_SEEK_TO_BEGIN;
			}
		}
		else
		{
			assert(false);
			return ERR_READ_FILE_PARAM_INVALID;
		}
	}

	return ReadCurVideoFrmInfo();
}

unsigned long CAVIReader::SeekNextAudioFrame()
{
	if(m_pFile == NULL)
	{
		assert(false);
		return ERR_READ_FILE_NO_OPEN;
	}

	memset(&m_curReadState, 0, sizeof(m_curReadState));
	m_preReadType = FRAME_TYPE_NONE;

	if(m_aCurIndex+1 >= m_nAudioFrames)
	{
		return ERR_READ_FILE_SEEK_TO_END;
	}

	m_aCurIndex++;

	return ReadCurAudioFrmInfo();
}

//��λ����ȡ��Ƶ����Ƶ״̬
unsigned long CAVIReader::SeekNextFrame()
{
	if(m_pFile == NULL)
	{
		assert(false);
		return ERR_READ_FILE_NO_OPEN;
	}

	if(m_waveFormat.nAvgBytesPerSec == 0)
	{
		return ERR_READ_FILE_NO_AUDIO;
	}

	if((m_preReadType == FRAME_TYPE_VIDEO) || (m_curReadState.videoTime == 0))
	{
		if((m_vCurIndex+1) >= m_nVideoFrames)
		{
			if(m_aCurIndex == m_nAudioFrames)
			{
				return ERR_READ_FILE_SEEK_TO_END;
			}
			else
			{
				m_vCurIndex++;
				m_curReadState.videoTime = MAX_TIME;
				return ERR_READ_FILE_OK;
			}
		}
		m_vCurIndex++;
		unsigned long ret = ReadCurVideoFrmInfo();
		if(ret != ERR_READ_FILE_OK)
		{
			assert(false);
			return ret;
		}
	}

	if((m_preReadType == FRAME_TYPE_AUDIO) || (m_curReadState.audioTime == 0))
	{
		//����Ƶ���ݲŽ�����Ƶ���ݲ���
		if(m_waveFormat.nAvgBytesPerSec > 0)
		{
			if((m_aCurIndex+1) >= m_nAudioFrames)
			{
				if(m_vCurIndex == m_nVideoFrames)
				{
					return ERR_READ_FILE_SEEK_TO_END;
				}
				else
				{
					m_aCurIndex++;
					m_curReadState.audioTime = MAX_TIME;
					return ERR_READ_FILE_OK;
				}
			}
			m_aCurIndex++;
			unsigned long ret = ReadCurAudioFrmInfo();
			if(ret != ERR_READ_FILE_OK)
			{
				assert(false);
				return ret;
			}
		}
	}
	return ERR_READ_FILE_OK;
}

unsigned long CAVIReader::SeekKeyFrameToTime(LONGLONG time)
{
	m_preReadType = FRAME_TYPE_NONE;
	m_vCurIndex = FrameTimetoFrameIndex(streamtypeVIDEO, time);
	if(m_vCurIndex < 0)
	{
		assert(false);
		 return ERR_READ_FILE_SEEK_FAILED;
	}

	while((m_vCurIndex >= 0) && (m_pVideoIndex[m_vCurIndex].ckid != INDEX_BVIDEO))
	{
		m_vCurIndex--;
	}

	if(m_vCurIndex < 0)
	{
		assert(false);
		return ERR_READ_FILE_SEEK_TO_BEGIN;
	}

	unsigned long ret = ReadCurVideoFrmInfo();
	if(ERR_READ_FILE_OK != ret)
	{
		return ret;
	}

	if(0 != m_nAudioFrames)
	{
		m_aCurIndex = FrameTimetoFrameIndex(streamtypeAUDIO, m_curReadState.videoTime);
		
		return ReadCurAudioFrmInfo();
	}
	return ret;
}

bool CAVIReader::Close()
{
	if(m_bFileOpen)
	{
		m_bFileOpen = false;
		CloseReadThd();
		delete [] m_pVideoIndex;
		if(m_pAudioIndex != NULL)
		{
			delete [] m_pAudioIndex;
		}
		
		fclose(m_pFile);

		m_pVideoIndex	= NULL;
		m_pAudioIndex	= NULL;
		m_pFile			= NULL;
		m_nVideoFrames	= 0;
		m_nAudioFrames	= 0;
		m_aCurIndex		= 0;
 		m_vCurIndex		= 0;
	}

	return true;
}
	
bool CAVIReader::Repair(const char *pstrFilename, bool bDefSize, LONGLONG *pEndTime)
{
	if(pEndTime != NULL)
	{
		*pEndTime = 0;
	}
	
	FILE			*pFile;
	ULONG			EndMark;
	ULONG			flagRIFF[3];
	AVIINDEXENTRY * pIndexData;
	AVIMAINHEADER		aviHeader;
	AVISTREAMHEADER		videoHeader;
	AVISTREAMHEADER		audioHeader;
	AVISTREAMHEADER		vTimeHeader;
	AVISTREAMHEADER		aTimeHeader;
	ULONG				flagTotalFrame[6];
	ULONG				flagIndex[2];
	ULONG				nVideoFrames = 0;
	ULONG				nVTFrames = 0;
	ULONG				nAudioFrames = 0;
	ULONG				nATFrames = 0;
	ULONG				nTotalFrames = 0;

#if AVI_READER_FILE_NAME
	printf("Repair AVI file %s ...\n", pstrFilename);
#endif
	
	pFile = fopen(pstrFilename, "rb+");
	if(pFile == NULL)
		return false;
	
	if(fread(flagRIFF, sizeof(flagRIFF), 1, pFile) < 1)
	{
		fclose(pFile);
		return false;
	}
	if( (flagRIFF[0] != FLAG_RIFF) ||(flagRIFF[2] != formtypeAVI) )
	{
		fclose(pFile);
		return false;
	}
	
	if(fseek(pFile, POS_AVINORMAL, SEEK_SET) !=0)
	{
		fclose(pFile);
		return false;
	}
	if(fread(&EndMark, sizeof(ULONG), 1, pFile) < 1)
	{
		fclose(pFile);
		return false;
	}
	if(EndMark == AVI_NORMAL)
	{
		fclose(pFile);
		return true;
	}

	pIndexData = new AVIINDEXENTRY[MAXFRAMES];

	fseek(pFile, sizeof(LONG)*6, SEEK_SET);
	fread(&aviHeader, sizeof(AVIMAINHEADER), 1, pFile);

	fseek(pFile, sizeof(LONG)*3, SEEK_CUR);
	fread(&videoHeader, sizeof(AVISTREAMHEADER), 1, pFile);
	fseek(pFile, sizeof(LONG)*2+sizeof(BITMAPINFOHEADER), SEEK_CUR);

	fseek(pFile, sizeof(LONG)*3, SEEK_CUR);
	fread(&audioHeader, sizeof(AVISTREAMHEADER), 1, pFile);
	fseek(pFile, sizeof(LONG)*2+sizeof(WAVEFORMATEX), SEEK_CUR);
	audioHeader.dwLength = 0;

	fseek(pFile, sizeof(LONG)*3, SEEK_CUR);
	fread(&vTimeHeader, sizeof(AVISTREAMHEADER), 1, pFile);
	fseek(pFile, sizeof(LONG)*2+sizeof(ULONGLONG), SEEK_CUR);

	fseek(pFile, sizeof(LONG)*3, SEEK_CUR);
	fread(&aTimeHeader, sizeof(AVISTREAMHEADER), 1, pFile);
	fseek(pFile, sizeof(LONG)*2+sizeof(ULONGLONG), SEEK_CUR);

	fread(flagTotalFrame, sizeof(flagTotalFrame), 1, pFile);
//////////////////////////////////////////////////////////////////////////

	long index = 0;			//֡����

	//64M�̶���С���ļ�
	if(bDefSize)
	{
		int ret = fseek(pFile, INDEX_OFFSET-8, SEEK_SET);
#if TEST
		assert(ret == 0);
#endif
		ret = (int)fread(flagIndex, sizeof(flagIndex), 1, pFile);
		ret = (int)fread(pIndexData, MAXFRAMES*sizeof(AVIINDEXENTRY), 1, pFile);
#if TEST
		assert(ret == 1);
#endif
		
		for(index=0; index<MAXFRAMES; index++)
		{
			if( (pIndexData[index].ckid == INDEX_BVIDEO)||(pIndexData[index].ckid == INDEX_CVIDEO) )
			{
				nVideoFrames++;
			}
			else if(pIndexData[index].ckid == INDEX_AUDIO)
			{
				audioHeader.dwLength += pIndexData[index].dwChunkLength;
				nAudioFrames++;
			}
			else if(pIndexData[index].ckid == INDEX_VIDEOTIME)
			{
				nVTFrames++;
			}
			else if(pIndexData[index].ckid == INDEX_AUDIOTIME)
			{
				nATFrames++;
			}
			else
			{
				break;
			}
		}

		//���������֡����
		for(int k=0; k<2; k++)
		{
			index--;
			if( (pIndexData[index].ckid == INDEX_BVIDEO)||(pIndexData[index].ckid == INDEX_CVIDEO) )
			{
				nVideoFrames--;
			}
			else if(pIndexData[index].ckid == INDEX_AUDIO)
			{
				audioHeader.dwLength -= pIndexData[index].dwChunkLength;
				nAudioFrames--;
			}
			else if(pIndexData[index].ckid == INDEX_VIDEOTIME)
			{
				if(pEndTime != NULL)
				{
					fseek(pFile, MOVI_POS_OFFSET-4+pIndexData[index].dwChunkOffset+2*sizeof(LONG), SEEK_SET);
					fread(pEndTime, sizeof(LONGLONG), 1, pFile);
					if( *pEndTime < 0 )
					{
						assert(false);
						*pEndTime = 0;
					}
				}
				nVTFrames--;
			}
			else if(pIndexData[index].ckid == INDEX_AUDIOTIME)
			{
				nATFrames--;
			}
		}
		
		//��֤����Ƶ����������Ƶ����ʱ��֡����Ӧ���
		nVideoFrames = (nVideoFrames <= nVTFrames) ? nVideoFrames : nVTFrames;
		nAudioFrames = (nAudioFrames <= nATFrames) ? nAudioFrames : nATFrames;
	}
	else		//�䳤�ļ���֡��ȡ����
	{
		BYTE *pDataBuffer = new BYTE[704 * 576 *2];
		UINT dataBlock[2];

		//�������ݶε���ʼλ��
		if(fseek(pFile, MOVI_POS_OFFSET, SEEK_SET) != 0)
		{
			fclose(pFile);
			return false;
		}

		//��֡��ȡ���ݣ��ָ���������
		while(true)
		{
			//��������Ϣ�����ݱ�ʶ�����ݿ��С��
			if(fread(dataBlock, sizeof(dataBlock), 1, pFile) != 1)
			{
				goto ERROR_PROC;
			}
			if(dataBlock[0] == INDEX_BVIDEO)		//��Ƶ�ؼ�֡
			{
				pIndexData[index].dwFlags = AVIIF_KEYFRAME;
				nVideoFrames++;
			}
			else if(dataBlock[0] == INDEX_CVIDEO)	//��Ƶ�ǹؼ�֡
			{
				pIndexData[index].dwFlags = 0;
				nVideoFrames++;
			}
			else if(dataBlock[0] == INDEX_AUDIO)	//��Ƶ֡
			{
				audioHeader.dwLength += dataBlock[1];
				pIndexData[index].dwFlags = AVIIF_KEYFRAME;
				nAudioFrames++;
			}
			else if(dataBlock[0] == INDEX_VIDEOTIME)	//��Ƶʱ��֡
			{
				pIndexData[index].dwFlags = AVIIF_KEYFRAME;
				nVTFrames++;
			}
			else if(dataBlock[0] == INDEX_AUDIOTIME)	//��Ƶʱ��֡
			{
				pIndexData[index].dwFlags = AVIIF_KEYFRAME;
				nATFrames++;
			}
			else
			{
				assert(false);
				goto ERROR_PROC;
			}

			pIndexData[index].ckid = dataBlock[0];
			if(dataBlock[1] >= 704 * 576 * 2)			//���ݳ����쳣��������д���̳��������
			{
				assert(false);
				goto ERROR_PROC;
			}
			pIndexData[index].dwChunkLength = dataBlock[1];
			if(index == 0)
			{
				pIndexData[index].dwChunkOffset = 4;		//��ʶռ���ĸ��ֽ�
			}
			else
			{
				pIndexData[index].dwChunkOffset = pIndexData[index-1].dwChunkOffset \
					+ pIndexData[index-1].dwChunkLength + sizeof(dataBlock);
			}

			//��ȡ��֡������
			if(fread(pDataBuffer, dataBlock[1], 1, pFile) != 1)	//��ȡ����֡ʧ��
			{
				goto ERROR_PROC;
			}
			if((dataBlock[0] == INDEX_VIDEOTIME) && (pEndTime != NULL))
			{
				memcpy(pEndTime, pDataBuffer, sizeof(LONGLONG));
			}
			index++;
			continue;
ERROR_PROC:
			if ((pIndexData[index-1].ckid == INDEX_AUDIOTIME) \
				|| (pIndexData[index-1].ckid == INDEX_VIDEOTIME) )		//��֤ʱ��֡������֡�����
			{
				if(index == 0)
				{
					fclose(pFile);
					return false;
				}
				index--;
			}
			break;
		}//while(true)

		//���ݹ��� index ֡
		if(index <= 0)		//û������
		{
			fclose(pFile);
			return false;
		}

		//�������ܳ���д��������ʼλ�õ����ݳ��ȼ�¼��Ԫ��
		UINT nDataLength = pIndexData[index-1].dwChunkOffset + pIndexData[index-1].dwChunkLength + sizeof(dataBlock);
		nDataLength = (nDataLength + 3)/4 * 4;		//�����ݽ������ֽڶ���
		fseek(pFile, MOVI_POS_OFFSET - 2*sizeof(LONG), SEEK_SET);
		fwrite(&nDataLength, sizeof(UINT), 1, pFile);

		//��֡����д���ļ�ĩβ
		fseek(pFile, MOVI_POS_OFFSET - sizeof(LONG) + nDataLength, SEEK_SET);
		flagIndex[0] = ckidAVINEWINDEX;
		flagIndex[1] = index * sizeof(AVIINDEXENTRY);
		fwrite(flagIndex, sizeof(flagIndex), 1, pFile);		//д֡�������
		fwrite(pIndexData, flagIndex[1], 1, pFile);			//д֡����

		//д�ļ��ܳ���
		fseek(pFile, sizeof(LONG), SEEK_SET);
		flagRIFF[1] = MOVI_POS_OFFSET - 3 * sizeof(LONG) + nDataLength + flagIndex[1] + sizeof(flagIndex);
		fwrite(&flagRIFF[1], sizeof(LONG), 1, pFile);

		delete [] pDataBuffer;

	}//end if (�ļ����ǹ̶���С)
	
	nTotalFrames = (nAudioFrames + nVideoFrames) * 2;
	videoHeader.dwLength = nVideoFrames;
	vTimeHeader.dwLength = nVTFrames;
	aTimeHeader.dwLength = nATFrames;
	flagTotalFrame[5] = nTotalFrames;
	flagIndex[1] = nTotalFrames*sizeof(AVIINDEXENTRY);

	//дAVI�ļ�ͷ
	fseek(pFile, sizeof(LONG)*6, SEEK_SET);
	fwrite(&aviHeader, sizeof(AVIMAINHEADER), 1, pFile);
	
	//д��Ƶͷ
	fseek(pFile, sizeof(LONG)*3, SEEK_CUR);
	fwrite(&videoHeader, sizeof(AVISTREAMHEADER), 1, pFile);
	fseek(pFile, sizeof(LONG)*2+sizeof(BITMAPINFOHEADER), SEEK_CUR);
	
	//д��Ƶͷ
	fseek(pFile, sizeof(LONG)*3, SEEK_CUR);
	fwrite(&audioHeader, sizeof(AVISTREAMHEADER), 1, pFile);
	fseek(pFile, sizeof(LONG)*2+sizeof(WAVEFORMATEX), SEEK_CUR);
	
	//д��Ƶʱ��ͷ
	fseek(pFile, sizeof(LONG)*3, SEEK_CUR);
	fwrite(&vTimeHeader, sizeof(AVISTREAMHEADER), 1, pFile);
	fseek(pFile, sizeof(LONG)*2+sizeof(ULONGLONG), SEEK_CUR);
	
	//д��Ƶʱ��ͷ
	fseek(pFile, sizeof(LONG)*3, SEEK_CUR);
	fwrite(&aTimeHeader, sizeof(AVISTREAMHEADER), 1, pFile);
	fseek(pFile, sizeof(LONG)*2+sizeof(ULONGLONG), SEEK_CUR);
	
	//д��֡��
	fwrite(flagTotalFrame, sizeof(flagTotalFrame), 1, pFile);

	//д֡����
	if(bDefSize)
	{
		fseek(pFile, INDEX_OFFSET-8, SEEK_SET);
		fwrite(flagIndex, sizeof(flagIndex), 1, pFile);
	}

	//д�������
	EndMark = AVI_NORMAL;
	fseek(pFile, POS_AVINORMAL, SEEK_SET);
	fwrite(&EndMark, sizeof(ULONG), 1, pFile);
	fclose(pFile);

	delete [] pIndexData;
	return true;
}


//////////////////////////////////////////////////////////////////////////
//		protected

//�õ�ָ��ʱ���֡����
int CAVIReader::FrameTimetoFrameIndex(int StreamType,LONGLONG tval)
{
#if TEST
	assert(m_bFileOpen);
#endif
	int start, last;
	LONGLONG startTime, LastTime, tempTime;
	long offset = MOVI_POS_OFFSET-4;

	if(StreamType == streamtypeVIDEO)
	{
		start = 0;
		last = m_nVideoFrames-1;
		
		if (0 == fseek(m_pFile, offset+m_pVideoIndex[start].dwChunkOffset-m_TimeBufLen, SEEK_SET))
		{
			if (1 == fread(&startTime, sizeof(LONGLONG), 1, m_pFile))
			{
				if (0 == fseek(m_pFile, offset+m_pVideoIndex[last].dwChunkOffset-m_TimeBufLen, SEEK_SET))
				{
					if (1 == fread(&LastTime, sizeof(LONGLONG), 1, m_pFile))
					{
						if (tval <= startTime)
						{
							return start;
						}
						else if (tval >= LastTime)
						{
							return last;
						}

						//tval ���� [0, LastTime]
						if(tval < LastTime)
						{
							int m = 0;
							//���ö��ַ����в���
							while(start < last)
							{
								m = (start+last)/2;

								if (0 == fseek(m_pFile, offset+m_pVideoIndex[m].dwChunkOffset-m_TimeBufLen, SEEK_SET))
								{
									if (1 == fread(&tempTime, sizeof(LONGLONG), 1, m_pFile))
									{
										if(tval == tempTime)
										{
											return m;
										}
										else if(tval < tempTime)
										{
											last = m;
										}
										else if (start == m)
										{
											return last;
										}
										else
										{
											start = m;
										}
									}
								}
							}

							//��IO����
							if (start >= last)
							{
								return start;
							}
						}
					}
				}
			}
		}
	}
	else if(StreamType == streamtypeAUDIO)
	{
		if(m_nAudioFrames == 0)
		{
			return -1;
		}
		start = 0;
		last = m_nAudioFrames-1;

		if (0 == fseek(m_pFile, offset+m_pAudioIndex[start].dwChunkOffset-m_TimeBufLen, SEEK_SET))
		{
			if (1 == fread(&startTime, sizeof(LONGLONG), 1, m_pFile))
			{
				if (0 == fseek(m_pFile, offset+m_pAudioIndex[last].dwChunkOffset-m_TimeBufLen, SEEK_SET))
				{
					if (1 == fread(&LastTime, sizeof(LONGLONG), 1, m_pFile))
					{
						if (tval <= startTime)
						{
							return start;
						}
						else if (tval == LastTime)
						{
							return last;
						}

						if(tval <= LastTime)
						{
							int m = 0;

							while(start < last)
							{
								m = (start+last)/2;

								if (0 == fseek(m_pFile, offset+m_pAudioIndex[m].dwChunkOffset-m_TimeBufLen, SEEK_SET))
								{
									if (1 == fread(&tempTime, sizeof(LONGLONG), 1, m_pFile))
									{
										if(tval == tempTime)
										{
											return m;
										}
										else if(tval < tempTime)
										{
											last = m;
										}
										else if (start == m)
										{
											return last;
										}
										else
										{
											start = m;
										}
									}
								}
							}

							//��IO����
							if (start >= last)
							{
								return start;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		return -1;
	}

	return -1;
}

void CAVIReader::ReadHeaderInfo()
{
#if TEST
	assert(m_bFileOpen);
#endif
	fseek(m_pFile, sizeof(m_flagRIFF)+sizeof(m_flagList)+sizeof(m_flagHeadList), SEEK_SET);
	fread(&m_aviHeader, sizeof(AVIMAINHEADER), 1, m_pFile);

	fseek(m_pFile, sizeof(m_flagStrList[0]), SEEK_CUR);
	fread(&m_videoHeader, sizeof(AVISTREAMHEADER), 1, m_pFile);
	m_nVideoFrames = m_videoHeader.dwLength;
	fseek(m_pFile, sizeof(m_flagStrFormat), SEEK_CUR);
	fread(&m_bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, m_pFile);

	fseek(m_pFile, sizeof(m_flagStrList[1]), SEEK_CUR);
	fread(&m_audioHeader, sizeof(AVISTREAMHEADER), 1, m_pFile);

	fseek(m_pFile, sizeof(m_flagStrFormat), SEEK_CUR);
	fread(&m_waveFormat, sizeof(WAVEFORMATEX), 1, m_pFile);

	fseek(m_pFile, sizeof(m_flagStrList[2]), SEEK_CUR);
	fread(&m_vTimeHeader, sizeof(AVISTREAMHEADER), 1, m_pFile);
	fseek(m_pFile, sizeof(m_flagStrFormat)+sizeof(ULONGLONG), SEEK_CUR);

	fseek(m_pFile, sizeof(m_flagStrList[3]), SEEK_CUR);
	fread(&m_aTimeHeader, sizeof(AVISTREAMHEADER), 1, m_pFile);
	fseek(m_pFile, sizeof(m_flagStrFormat)+sizeof(ULONGLONG), SEEK_CUR);

	fread(m_flagTotalFrame, sizeof(m_flagTotalFrame), 1, m_pFile);
	m_nAudioFrames = m_flagTotalFrame[5]/2 - m_nVideoFrames;
	//assert(m_nAudioFrames >= 0);

	fseek(m_pFile, MOVI_POS_OFFSET-sizeof(m_flagMovie), SEEK_SET);
	fread(m_flagMovie, sizeof(m_flagMovie), 1, m_pFile);
	m_nIndexOffset = MOVI_POS_OFFSET+m_flagMovie[1]-sizeof(m_flagMovie[2]);
}

void CAVIReader::ReadIndexItem()
{
#if TEST
	assert(m_bFileOpen);
#endif
	int ret;
	long nATFrames = 0, nVTFrames = 0;

	AVIINDEXENTRY * pIndexData = new AVIINDEXENTRY[MAXFRAMES];
	memset(pIndexData, 0, sizeof(AVIINDEXENTRY)*MAXFRAMES);

	if(m_bEndNormal)
	{
		ret = fseek(m_pFile, INDEX_OFFSET-sizeof(m_flagIndex), SEEK_SET);
#if TEST
		assert(ret == 0);
#endif
		ret = (int)fread(m_flagIndex, sizeof(m_flagIndex), 1, m_pFile);
#if TEST
		assert(ret == 1);
#endif
		ret = (int)fread(pIndexData, m_flagIndex[1], 1, m_pFile);
#if TEST
		assert(ret == 1);
#endif
		nVTFrames = m_nVideoFrames;
		nATFrames = m_nAudioFrames;
	}
	else		//���������������ҳ���Ч����
	{
		m_nVideoFrames = 0;
		m_nAudioFrames = 0;
		ret = fseek(m_pFile, INDEX_OFFSET, SEEK_SET);
#if TEST
		assert(ret == 0);
#endif
		ret = (int)fread(pIndexData, MAXFRAMES*sizeof(AVIINDEXENTRY), 1, m_pFile);
#if TEST
		assert(ret == 1);
#endif
		long index = 0;
		for(index=0; index<MAXFRAMES; index++)
		{
			if( (pIndexData[index].ckid == INDEX_BVIDEO)||(pIndexData[index].ckid == INDEX_CVIDEO) )
			{
				m_nVideoFrames++;
			}
			else if(pIndexData[index].ckid == INDEX_AUDIO)
			{
				m_nAudioFrames++;
			}
			else if(pIndexData[index].ckid == INDEX_VIDEOTIME)
			{
				nVTFrames++;
			}
			else if(pIndexData[index].ckid == INDEX_AUDIOTIME)
			{
				nATFrames++;
			}
			else
			{
				break;
			}
		}

		for(int k=0; k<2; k++)
		{
			index--;
			if( (pIndexData[index].ckid == INDEX_BVIDEO)||(pIndexData[index].ckid == INDEX_CVIDEO) )
			{
				m_nVideoFrames--;
			}
			else if(pIndexData[index].ckid == INDEX_AUDIO)
			{
				m_nAudioFrames--;
			}
			else if(pIndexData[index].ckid == INDEX_VIDEOTIME)
			{
				nVTFrames--;
			}
			else if(pIndexData[index].ckid == INDEX_AUDIOTIME)
			{
				nATFrames--;
			}
		}

		m_nVideoFrames = (m_nVideoFrames <= nVTFrames) ? m_nVideoFrames : nVTFrames;
		m_nAudioFrames = (m_nAudioFrames <= nATFrames) ? m_nAudioFrames : nATFrames;
	}
	m_pVideoIndex = new AVIINDEXENTRY[m_nVideoFrames];
	if(m_nAudioFrames > 0)
	{
		m_pAudioIndex = new AVIINDEXENTRY[m_nAudioFrames];
		memset(m_pAudioIndex, 0, sizeof(AVIINDEXENTRY)*m_nAudioFrames);
	}
	memset(m_pVideoIndex, 0, sizeof(AVIINDEXENTRY)*m_nVideoFrames);

	int nVFrames = 0; 
	int nAFrames = 0;
	for(long index=0; index<(m_nAudioFrames + m_nVideoFrames)*2; index++)
	{
		if( (pIndexData[index].ckid == INDEX_BVIDEO)||(pIndexData[index].ckid == INDEX_CVIDEO) )
		{
			m_pVideoIndex[nVFrames] = pIndexData[index];
			nVFrames++;
		}
		else if(pIndexData[index].ckid == INDEX_AUDIO)
		{
			m_pAudioIndex[nAFrames] = pIndexData[index];
			nAFrames++;
		}
	}

	m_nVideoFrames = nVFrames;
	m_nAudioFrames = nAFrames;

	delete [] pIndexData;
	pIndexData = NULL;
}

void CAVIReader::ReadIndexItemEx()
{
	int ret;
	long nATFrames = 0, nVTFrames = 0;

	AVIINDEXENTRY * pIndexData = new AVIINDEXENTRY[MAXFRAMES];
	memset(pIndexData, 0, sizeof(AVIINDEXENTRY)*MAXFRAMES);
	if(m_bEndNormal)
	{
		ret = fseek(m_pFile, MOVI_POS_OFFSET-sizeof(LONG)+m_flagMovie[1], SEEK_SET);
#if TEST
		assert(ret == 0);
#endif
		ret = (int)fread(m_flagIndex, sizeof(m_flagIndex), 1, m_pFile);
#if TEST
		assert(ret == 1);
#endif
		ret = (int)fread(pIndexData, m_flagIndex[1], 1, m_pFile);
#if TEST
		assert(ret == 1);
#endif
		nVTFrames = m_nVideoFrames;
		nATFrames = m_nAudioFrames;
	}
	else		//�����������ݣ��ؽ���������
	{
		m_nVideoFrames = 0;
		m_nAudioFrames = 0;

		int nFrameCount = 0;
		int nOffset = 4;
		ret = fseek(m_pFile, MOVI_POS_OFFSET, SEEK_SET);
#if TEST
		assert(ret == 0);
#endif
		long index = 0;
		while(fread(m_flagDataBlock, sizeof(m_flagDataBlock), 1, m_pFile) == 1)
		{
			pIndexData[nFrameCount].dwFlags = AVIIF_KEYFRAME;
			if( (m_flagDataBlock[0] == INDEX_BVIDEO) ||(m_flagDataBlock[0] == INDEX_CVIDEO) )
			{
				if(m_flagDataBlock[0] == INDEX_CVIDEO)
				{
					pIndexData[nFrameCount].dwFlags = 0;
				}
				m_nVideoFrames++;
			}
			else if(m_flagDataBlock[0] == INDEX_AUDIO)
			{
				m_nAudioFrames++;
			}
			else if(m_flagDataBlock[0] == INDEX_VIDEOTIME)
			{
				nVTFrames++;
			}
			else if(m_flagDataBlock[0] == INDEX_AUDIOTIME)
			{
				nATFrames++;
			}
			else
			{
				break;
			}
			pIndexData[nFrameCount].ckid = m_flagDataBlock[0];
			pIndexData[nFrameCount].dwChunkLength = m_flagDataBlock[1];
			pIndexData[nFrameCount].dwChunkOffset = nOffset;

			nFrameCount++;
			fseek(m_pFile, m_flagDataBlock[1], SEEK_CUR);
			nOffset += sizeof(m_flagDataBlock)+m_flagDataBlock[1];
		}

		m_nVideoFrames = (m_nVideoFrames <= nVTFrames) ? m_nVideoFrames : nVTFrames;
		m_nAudioFrames = (m_nAudioFrames <= nATFrames) ? m_nAudioFrames : nATFrames;

		m_nIndexOffset = MOVI_POS_OFFSET+nOffset-sizeof(m_flagMovie[2]);
	}
	m_pVideoIndex = new AVIINDEXENTRY[m_nVideoFrames];
	if(m_nAudioFrames > 0)
	{
		m_pAudioIndex = new AVIINDEXENTRY[m_nAudioFrames];
		memset(m_pAudioIndex, 0, sizeof(AVIINDEXENTRY)*m_nAudioFrames);
	}
	memset(m_pVideoIndex, 0, sizeof(AVIINDEXENTRY)*m_nVideoFrames);

	int nVFrames = 0; 
	int nAFrames = 0;

	assert( (m_nAudioFrames + m_nVideoFrames) * 2 <= MAXFRAMES );
	for(long index=0; index<(m_nAudioFrames + m_nVideoFrames)*2; index++)
	{
		if( (pIndexData[index].ckid == INDEX_BVIDEO)||(pIndexData[index].ckid == INDEX_CVIDEO) )
		{
			m_pVideoIndex[nVFrames] = pIndexData[index];
			nVFrames++;
		}
		else if(pIndexData[index].ckid == INDEX_AUDIO)
		{
			m_pAudioIndex[nAFrames] = pIndexData[index];
			nAFrames++;
		}
	}

	m_nVideoFrames = nVFrames;
	m_nAudioFrames = nAFrames;

	delete [] pIndexData;
	pIndexData = NULL;
}

int CAVIReader::AVIFseek(FILE *pFile, long offset, int seek)
{
	if (!m_bCanRead)
	{
		return -1;
	}

	m_ReadLock.Lock();

	size_t ofsSetNum = 0;
	if (SEEK_SET == seek)
	{
		ofsSetNum = offset;
	}
	else if (SEEK_CUR == seek)
	{
		ofsSetNum = (m_bufHeadOffset + m_currOffset + offset);
	}
	else
	{
		ofsSetNum = 0;
	}

	//�Ѿ��ڻ������
	if ((m_bufHeadOffset <= ofsSetNum) && (ofsSetNum < m_bufTailOffset))
	{
		m_currOffset = (ofsSetNum - m_bufHeadOffset);

		m_bCanRead = true;
		m_ReadLock.Unlock();
		return 0;
	}
	else //���ڻ���أ�����»���أ����µ�offsetΪ���
	{
		if (0 == fseek(pFile, (long)ofsSetNum, SEEK_SET))
		{
			size_t len = ((long)(AVI_READ_BUF_LEN + ofsSetNum) <= m_nIndexOffset) ? AVI_READ_BUF_LEN : (m_nIndexOffset - ofsSetNum);
			if (1 == fread(m_ReadBuf, len, 1, pFile))
			{
				m_bufHeadOffset = ofsSetNum;
				m_bufTailOffset = (ofsSetNum + len);

				m_currOffset	= 0;

				m_bCanRead = true;
				m_ReadLock.Unlock();
				return 0;
			}
		}
	}

	m_bCanRead = false;
	m_ReadLock.Unlock();
	return -1;
}

unsigned long CAVIReader::ReadCurVideoFrmInfo()
{
	if (0 != AVIFseek(m_pFile, MOVI_POS_OFFSET-4+m_pVideoIndex[m_vCurIndex].dwChunkOffset-m_TimeBufLen, SEEK_SET))
	{
		return ERR_READ_FILE_SEEK_FAILED;
	}
	
	if(1 != AVIFread(&m_curReadState.videoTime, sizeof(LONGLONG), 1, m_pFile))
	{
		return ERR_READ_FILE_READ_FAILED;
	}
	if(m_TimeBufLen > sizeof(LONGLONG))
	{
		if(0 != AVIFseek(m_pFile, m_TimeBufLen - sizeof(LONGLONG), SEEK_CUR))
		{
			return ERR_READ_FILE_SEEK_FAILED;
		}
	}
	if(1 != AVIFread(m_flagDataBlock, sizeof(m_flagDataBlock), 1, m_pFile))
	{
		return ERR_READ_FILE_READ_FAILED;
	}
	m_curReadState.videoFrmSize = m_flagDataBlock[1];
	if(m_flagDataBlock[0] == INDEX_BVIDEO)
	{
		m_curReadState.videoKeyFlag = true;
	}
	else if(m_flagDataBlock[0] == INDEX_CVIDEO)
	{
		m_curReadState.videoKeyFlag = false;
	}
	else
	{
		return ERR_READ_FILE_EXCEPTION;
	}
	return ERR_READ_FILE_OK;
}

unsigned long CAVIReader::ReadCurAudioFrmInfo()
{
	if(m_nAudioFrames == 0)
	{
		return ERR_READ_FILE_READ_FAILED;
	}

	if (0 != AVIFseek(m_pFile, MOVI_POS_OFFSET-4+m_pAudioIndex[m_aCurIndex].dwChunkOffset-m_TimeBufLen, SEEK_SET))
	{
		return ERR_READ_FILE_SEEK_FAILED;
	}
	
	if(1 != AVIFread(&m_curReadState.audioTime, sizeof(LONGLONG), 1, m_pFile))
	{
		return ERR_READ_FILE_READ_FAILED;
	}
	if(m_TimeBufLen > sizeof(LONGLONG))
	{
		if(0 != AVIFseek(m_pFile, m_TimeBufLen - sizeof(LONGLONG), SEEK_CUR))
		{
			return ERR_READ_FILE_SEEK_FAILED;
		}
	}
	if(1 != AVIFread(m_flagDataBlock, sizeof(m_flagDataBlock), 1, m_pFile))
	{
		return ERR_READ_FILE_READ_FAILED;
	}
	m_curReadState.audioFrmSize = m_flagDataBlock[1];
	if(m_flagDataBlock[0] != INDEX_AUDIO)
	{
		return ERR_READ_FILE_EXCEPTION;
	}
	return ERR_READ_FILE_OK;
}

size_t  CAVIReader::AVIFread(void *pBuf, size_t len, size_t count, FILE *pFile)
{
#if TEST
	assert(NULL != pFile);
	assert(NULL != pBuf);
	assert(count > 0);
	assert(len   > 0);
#endif

	if (!m_bCanRead)
	{
		return 0;
	}

	m_ReadLock.Lock();

	size_t totalLen = len * count;
	if(totalLen > AVI_READ_BUF_LEN)
	{
		size_t offset = m_bufHeadOffset + m_currOffset;
		if (0 == fseek(pFile, (long)offset, SEEK_SET))
		{
			if (1 == fread(pBuf, totalLen, 1, pFile))
			{
				offset += totalLen;
				if (0 == fseek(pFile, (long)offset, SEEK_SET))
				{
					size_t length = ((long)(AVI_READ_BUF_LEN + offset) <= m_nIndexOffset) ? AVI_READ_BUF_LEN : (m_nIndexOffset - offset);
					
					if (1 == fread(m_ReadBuf, length, 1, pFile))
					{
						m_bufHeadOffset = offset;
						m_bufTailOffset = (offset + length);
						
						m_currOffset	= 0;
						m_bCanRead = true;
					}
					else
					{
						m_bCanRead = false;
					}
				}
				else
				{
					m_bCanRead = false;
				}
				m_ReadLock.Unlock();
				return count;
			}
			else
			{
				m_ReadLock.Unlock();
				m_bCanRead = false;
				return 0;
			}
		}
		else
		{
			m_ReadLock.Unlock();
			m_bCanRead = false;
			return 0;
		}
	}
	else if ((totalLen + m_currOffset) > AVI_READ_BUF_LEN)//����buf����
	{
		size_t offset = m_bufHeadOffset + m_currOffset;
		if (0 == fseek(pFile, (long)offset, SEEK_SET))
		{
			size_t length = ((long)(AVI_READ_BUF_LEN + offset) <= m_nIndexOffset) ? AVI_READ_BUF_LEN : (m_nIndexOffset - offset);

			if (1 == fread(m_ReadBuf, length, 1, pFile))
			{
				m_bufHeadOffset = offset;
				m_bufTailOffset = (offset + length);

				m_currOffset	= 0;
				m_bCanRead = true;
			}
			else
			{
				m_bCanRead = false;
				m_ReadLock.Unlock();
				return 0;
			}
		}
		else
		{
			m_bCanRead = false;
			m_ReadLock.Unlock();
			return 0;
		}
	}
	
	if (totalLen <= (m_bufTailOffset - m_bufHeadOffset))
	{
		//��ȡ����
		memcpy(pBuf, m_ReadBuf+m_currOffset, totalLen);
		m_currOffset += totalLen;
		
		m_bCanRead = true;
		m_ReadLock.Unlock();
		return count;
	}
	else
	{
		m_bCanRead = false;
		m_ReadLock.Unlock();
		return 0;
	}
}

bool CAVIReader::CreateReadThd()
{
	if (m_read_handle != 0)
		return true;
	
	m_thread_read = true;
	m_read_handle = ComCreateThread(&Thread_read_ID, (void *)this, thread_read);
		
	if (0 == m_read_handle)
		m_thread_read = false;
	
	return m_thread_read;
}

void CAVIReader::CloseReadThd()
{
	m_thread_read = false;

	m_ReadEvent.SetEvent();
	ComExitThread(m_read_handle);

	m_read_handle = 0;
	Thread_read_ID = 0;
}

void CAVIReader::ReadProc()
{
	while(m_thread_read)
	{
		m_ReadEvent.WaitForCond();
		//
		m_ReadLock.Lock();
		
		//ʣ�����ݲ��࣬�����¶�ȡ
		if ((m_currOffset+AVI_READ_BUF_SUB) >= AVI_READ_BUF_LEN)
		{
			size_t offset = m_bufHeadOffset + m_currOffset;
			if (0 == fseek(m_pFile, (long)offset, SEEK_SET))
			{
				size_t len = ((long)(AVI_READ_BUF_LEN + offset) <= m_nIndexOffset) ? AVI_READ_BUF_LEN : (m_nIndexOffset - offset);
				if (1 == fread(m_ReadBuf, len, 1, m_pFile))
				{
					m_bufHeadOffset = offset;
					m_bufTailOffset = (offset + len);
					
					m_currOffset	= 0;
					m_bCanRead = true;
				}
				else
				{
					m_bCanRead = false;
				}
			}
			else
			{
				m_bCanRead = false;
			}
		}
		else
		{
			m_bCanRead = true;
		}

		m_ReadLock.Unlock();
	}
}

RESULT WINAPI CAVIReader::thread_read(void * arg)
{
	CAVIReader *pAVIReader = (CAVIReader *)arg;
	pAVIReader->ReadProc();
	return 0;
}

//end

