// AVIWriter.cpp: implementation of the CAVIWriterEx class.
//

#include "AVIWriter.h"
#include "PUB_common.h"

CAVIWriter::CAVIWriter()
{
	m_bFileOpen = false; 
	m_bDataArrived = false;
	m_pFile = NULL;
	m_TextFormat = 0;
	m_pData = NULL;
	m_bufDataLen = 0;
	m_bufFramesNum = 0;
	m_bufVideoNum = 0;
	m_curWritePos = 0;
	m_pIndexData = NULL;
	m_nMaxIndex = 0;
	m_indexNum = 0;
	m_firstFrameTime = -1;
	m_nPreFrameIndex = 0;
	m_FileStatus = AVICHECK_NOOPEN;
}

CAVIWriter::~CAVIWriter()
{
}
/************************************************************************
  aviType:保存的文件类别：
	AVI_TYPE_STD,		//标准AVI文件
	AVI_TYPE_DEFSIZE,	//含有时间流的定长AVI文件
	AVI_TYPE_NOSIZE		//含有时间流的非定长AVI文件
************************************************************************/
bool CAVIWriter::Open(const char *szFilename,BITMAPINFOHEADER *bmpInfo, WAVEFORMATEX *wFormat,\
						bool isPal, AVI_TYPE aviType, BYTE type)
{
#if TEST
	assert(!m_bFileOpen);
	assert(m_pFile == NULL);
	assert(szFilename != NULL);
	assert(bmpInfo != NULL);
#endif
	m_aviType = aviType;
	
	if (AVI_WRITE_NOAMAL == type)
	{
//		ULONG data=0;
//		ULONG dfldata = 65;
//		FILE *pFile = fopen("/flash/Buf.txt", "r");
//		if (pFile == NULL)
//		{
//			pFile = fopen("/flash/Buf.txt", "w+");
//			fprintf(pFile,"%ld", dfldata);
//			fseek(pFile, 0, SEEK_SET);
//		}
//		fscanf(pFile, "%ld", &data);
//		fclose(pFile);
//#if TEST
//		assert(data >0);
// #endif
		m_maxBufSize = 65*1024;
		m_maxBufFmNum = 20;
		m_maxBufIndexNum = 6*m_maxBufFmNum;
	}
	else
	{		
		m_maxBufSize = 65*1024;
		m_maxBufFmNum = 20;
		m_maxBufIndexNum = 6*m_maxBufFmNum;
	}
	
	if(m_aviType == AVI_TYPE_DEFSIZE)
	{
		m_pFile = fopen(szFilename, "rb+");
		if(m_pFile == NULL)
		{
			return false;
		}

		if (0 != fseek(m_pFile, 0, SEEK_SET))
		{
			return false;
		}
	}
	else
	{
		m_pFile = fopen(szFilename, "wb");
		if(m_pFile == NULL)
		{
			return false;
		}
	}

	m_bFileOpen = true;
	m_bDataArrived = false;
	m_indexNum = 0;
	m_bufVideoNum = 0;
	m_bufDataLen = 0;
	m_bufFramesNum = 0;
	m_FileStatus = AVICHECK_NOWRITEDATA;
	m_bufVideoNum = 0;
	m_curWritePos = 0;
	m_nPreFrameIndex = 0;

	if(m_aviType == AVI_TYPE_DEFSIZE)
	{
		m_pIndexData = new AVIINDEXENTRY[m_maxBufIndexNum*2];
#if TEST
	assert(m_pIndexData != NULL);
#endif
		memset(m_pIndexData, 0, sizeof(AVIINDEXENTRY)*m_maxBufIndexNum*2);
	}
	else
	{
		m_pIndexData = new AVIINDEXENTRY[AVI_FRAME_INDEX_UNIT];
#if TEST
	assert(m_pIndexData != NULL);
#endif
		memset(m_pIndexData, 0, sizeof(AVIINDEXENTRY)*AVI_FRAME_INDEX_UNIT);
	}

	m_nMaxIndex = AVI_FRAME_INDEX_UNIT;

	m_pData = new unsigned char[m_maxBufSize];

#if TEST
	assert(m_pData != NULL);
#endif
	
	if(bmpInfo != NULL)
	{
		// Create a video stream hdr
		memset(&m_strvhdr, 0, sizeof(AVISTREAMHEADER));
		m_strvhdr.fcc					= ckidSTREAMHEADER;
		m_strvhdr.cb					= sizeof(AVISTREAMHEADER)-8;
		m_strvhdr.fccType				= streamtypeVIDEO;// stream type
		m_strvhdr.fccHandler			= bmpInfo->biCompression;

		if (isPal)
		{// PAL/SECAM 25 fps
			m_strvhdr.dwScale = 1; 
			m_strvhdr.dwRate  = 25;
		}
		else
		{// NTSC 29.97 fps
			m_strvhdr.dwScale=1001; 
			m_strvhdr.dwRate=30000;
		}
		m_strvhdr.dwLength				= 0;
		m_strvhdr.dwQuality				= 0xFFFFFFFF;
		m_strvhdr.dwSuggestedBufferSize = 0;
		m_strvhdr.rcFrame.left = 0;
		m_strvhdr.rcFrame.top = 0;
		m_strvhdr.rcFrame.right = (unsigned short)bmpInfo->biWidth;
		m_strvhdr.rcFrame.bottom = (unsigned short)bmpInfo->biHeight;

		memset(&m_strvTimehdr,0,sizeof(m_strvTimehdr));
		if(m_aviType != AVI_TYPE_STD)
		{
			memcpy(&m_strvTimehdr,&m_strvhdr,sizeof(m_strvTimehdr));
			m_strvTimehdr.fccType=streamtypeTEXT;
			m_strvTimehdr.fccHandler=FLAG_FTMS;//设置时间流标识
		}
	}
	
	memset(&m_strahdr, 0, sizeof(AVISTREAMHEADER));
	memset(&m_straTimehdr,0,sizeof(m_straTimehdr));
	
	if(NULL != wFormat)
	{
		// Make a audio stream hdr ///////////////////////////////////
		m_nAudioType = wFormat->wFormatTag;	
		m_strahdr.fcc					= ckidSTREAMHEADER;
		m_strahdr.cb					= sizeof(AVISTREAMHEADER) - 8;
		m_strahdr.fccType               = streamtypeAUDIO;// stream type
		m_strahdr.fccHandler            = 0;
		m_strahdr.dwScale				= wFormat->nBlockAlign; 
		m_strahdr.dwRate				= wFormat->nAvgBytesPerSec;	
		m_strahdr.dwLength				= 0;
		m_strahdr.dwQuality				= (unsigned int)-1;

#if TEST
		assert(wFormat->nSamplesPerSec > 0);
#endif

		m_strahdr.dwSuggestedBufferSize  = wFormat->nAvgBytesPerSec;//(wFormat->nAvgBytesPerSec)/(wFormat->nSamplesPerSec);
		if(IME6400_AM_ADPCM == m_nAudioType)
			m_strahdr.dwSampleSize			= (wFormat->nAvgBytesPerSec)/(wFormat->nSamplesPerSec);
		else
			m_strahdr.dwSampleSize			= (wFormat->nAvgBytesPerSec)/(wFormat->nSamplesPerSec);

		if(0 == m_strahdr.dwSampleSize)
		{
			m_strahdr.dwSampleSize = 1;
		}
		
		
		if(m_aviType != AVI_TYPE_STD)
		{
			memcpy(&m_straTimehdr,&m_strahdr,sizeof(m_straTimehdr));
			m_straTimehdr.fccType=streamtypeTEXT;
			m_straTimehdr.fccHandler=FLAG_FTMS;//设置时间流标识
		}
	}

//	AVIMAINHEADER Initialization
	memset(&m_aviHeader, 0, sizeof(AVIMAINHEADER));
	m_aviHeader.fcc = ckidAVIMAINHDR;
	m_aviHeader.cb = sizeof(AVIMAINHEADER)-8;
	m_aviHeader.dwStreams = (m_aviType == AVI_TYPE_STD ? 2 : 4);
	m_aviHeader.dwMicroSecPerFrame = (unsigned long)(1000000./m_strvhdr.dwRate*m_strvhdr.dwScale + 0.5);
	m_aviHeader.dwMaxBytesPerSec = 0;
	m_aviHeader.dwFlags = AVIF_HASINDEX | AVIF_ISINTERLEAVED;
	m_aviHeader.dwTotalFrames = 0;
	m_aviHeader.dwInitialFrames	= 0;
	m_aviHeader.dwSuggestedBufferSize = m_strvhdr.dwSuggestedBufferSize;
	m_aviHeader.dwWidth = m_strvhdr.rcFrame.right;
	m_aviHeader.dwHeight = m_strvhdr.rcFrame.bottom;

	// Write AVI header
	m_flagRIFF[0]					= FLAG_RIFF;
	if(m_aviType == AVI_TYPE_DEFSIZE)
		m_flagRIFF[1]					= AVIFILELENGTH - 2*sizeof(unsigned long);
	else
		m_flagRIFF[1]					= MOVI_POS_OFFSET - 8;//空文件大小
	m_flagRIFF[2]					= formtypeAVI;
	m_flagList[0]					= FLAG_LIST;
	m_flagList[1]					= sizeof(m_flagHeadList) + sizeof(AVIMAINHEADER)\
										+ sizeof(m_flagStrList)\
										+ sizeof(AVISTREAMHEADER)*4\
										+ sizeof(m_flagStrFormat)*4\
										+ sizeof(BITMAPINFOHEADER)\
										+ sizeof(WAVEFORMATEX)\
										+ sizeof(LONGLONG)*2\
										+ sizeof(m_flagTotalFrame);
	if(wFormat != NULL)
	{
		m_flagList[1] += wFormat->cbSize;
	}
	m_flagHeadList[0]				= listtypeAVIHEADER;

	for(int i=0; i<4; i++)
	{
		m_flagStrList[i][0]				= FLAG_LIST;
		m_flagStrList[i][2]				= listtypeSTREAMHEADER;
	}

	m_flagStrFormat[0]				= ckidSTREAMFORMAT;

	m_flagTotalFrame[0]				= FLAG_LIST;
	m_flagTotalFrame[1]				= sizeof(m_flagTotalFrame) - 2*sizeof(unsigned long);
	m_flagTotalFrame[2]				= listtypeODMLExtendedAVIHeader;
	m_flagTotalFrame[3]				= ckidODMLExtendedAVIHeader;
	m_flagTotalFrame[4]				= sizeof(unsigned long);
	m_flagTotalFrame[5]				= 0;

	m_flagJunk[0]					= ckidAVIPADDING;
	m_flagJunk[1]					= MOVI_POS_OFFSET - sizeof(m_flagRIFF)\
										- sizeof(m_flagList) - m_flagList[1]\
										- sizeof(m_flagJunk)
										- sizeof(m_flagMovie);

	m_flagMovie[0]					= FLAG_LIST;
	if(m_aviType == AVI_TYPE_DEFSIZE)
		m_flagMovie[1] = AVIFILELENGTH - INDEX_LENGTH - (MOVI_POS_OFFSET-4);
	else
		m_flagMovie[1]					= 4;
	m_flagMovie[2]					= listtypeAVIMOVIE;

	m_flagIndex[0]					= ckidAVINEWINDEX;
	m_flagIndex[1]					= 0;
	
	//写文件头
	fwrite(m_flagRIFF, sizeof(m_flagRIFF), 1, m_pFile);
	fwrite(m_flagList, sizeof(m_flagList), 1, m_pFile);
	fwrite(m_flagHeadList, sizeof(m_flagHeadList), 1, m_pFile);
	fwrite(&m_aviHeader, sizeof(AVIMAINHEADER), 1, m_pFile);

	m_flagStrFormat[1] = sizeof(BITMAPINFOHEADER);
	m_flagStrList[0][1] = sizeof(AVISTREAMHEADER) + 3*sizeof(unsigned long)+m_flagStrFormat[1];
	fwrite(m_flagStrList[0], sizeof(m_flagStrList[0]), 1, m_pFile);
	fwrite(&m_strvhdr, sizeof(AVISTREAMHEADER), 1, m_pFile);
	fwrite(m_flagStrFormat, sizeof(m_flagStrFormat), 1, m_pFile);
	fwrite(bmpInfo, sizeof(BITMAPINFOHEADER), 1, m_pFile);

	m_flagStrFormat[1] = sizeof(WAVEFORMATEX);
	if(wFormat != NULL)
	{
		m_flagStrFormat[1] += wFormat->cbSize;
	}
	m_flagStrList[1][1] = sizeof(AVISTREAMHEADER) + 3*sizeof(unsigned long)+m_flagStrFormat[1];
	fwrite(m_flagStrList[1], sizeof(m_flagStrList[1]), 1, m_pFile);
	fwrite(&m_strahdr, sizeof(AVISTREAMHEADER), 1, m_pFile);
	fwrite(m_flagStrFormat, sizeof(m_flagStrFormat), 1, m_pFile);

	if (NULL != wFormat)
	{
		fwrite(wFormat, sizeof(WAVEFORMATEX)+wFormat->cbSize, 1, m_pFile);
	}
	else
	{
		fseek(m_pFile, sizeof(WAVEFORMATEX), SEEK_CUR);
	}
	
	m_flagStrFormat[1] = sizeof(m_TextFormat);
	m_flagStrList[2][1] = sizeof(AVISTREAMHEADER) + 3*sizeof(unsigned long)+m_flagStrFormat[1];

	fwrite(m_flagStrList[2], sizeof(m_flagStrList[2]), 1, m_pFile);
	fwrite(&m_strvTimehdr, sizeof(AVISTREAMHEADER), 1, m_pFile);
	fwrite(m_flagStrFormat, sizeof(m_flagStrFormat), 1, m_pFile);
	fwrite(&m_TextFormat, sizeof(m_TextFormat), 1, m_pFile);

	m_flagStrFormat[1] = sizeof(m_TextFormat);
	m_flagStrList[3][1] = sizeof(AVISTREAMHEADER) + 3*sizeof(unsigned long)+m_flagStrFormat[1];

	fwrite(m_flagStrList[3], sizeof(m_flagStrList[3]), 1, m_pFile);
	fwrite(&m_straTimehdr, sizeof(AVISTREAMHEADER), 1, m_pFile);
	fwrite(m_flagStrFormat, sizeof(m_flagStrFormat), 1, m_pFile);
	fwrite(&m_TextFormat, sizeof(m_TextFormat), 1, m_pFile);

	fwrite(m_flagTotalFrame, sizeof(m_flagTotalFrame), 1, m_pFile);
	fwrite(m_flagJunk, sizeof(m_flagJunk), 1, m_pFile);

//	写文件版本号
	if (0 != fseek(m_pFile, POS_AVIVER, SEEK_SET))
		return false;
	unsigned long ver = AVI_VERSION;
	fwrite(&ver, SIZE_AVIVER, 1, m_pFile);
//	写文件结束标记
	if (0 != fseek(m_pFile, POS_AVINORMAL, SEEK_SET))
		return false;
	unsigned long endMark = ~AVI_NORMAL;
	fwrite(&endMark, SIZE_AVINORMAL, 1, m_pFile);

	if (0 != fseek(m_pFile, MOVI_POS_OFFSET-sizeof(m_flagMovie), SEEK_SET))
		return false;
	fwrite(m_flagMovie, sizeof(m_flagMovie), 1, m_pFile);
	FileFlush(m_pFile);
	m_curWritePos = MOVI_POS_OFFSET;
//写索引头及清空索引
	if(m_aviType == AVI_TYPE_DEFSIZE)
	{

		//csg,先将最大帧数写进去，这样保证任何没有正常结束的数据也可以用MediaPlay播放
		m_flagIndex[1]=INDEX_LENGTH-40;
		if (0 != fseek(m_pFile, INDEX_OFFSET-sizeof(m_flagIndex), SEEK_SET))
			return false;
		if(1 != fwrite(m_flagIndex, sizeof(m_flagIndex), 1, m_pFile))
			return false;
		m_flagIndex[1]=0;

		memset(&m_pIndexData[0], 0, sizeof(AVIINDEXENTRY));
		if(1 != fwrite(&m_pIndexData[0], sizeof(AVIINDEXENTRY), 1, m_pFile) )
			return false;
		FileFlush(m_pFile);
		fseek(m_pFile, MOVI_POS_OFFSET, SEEK_SET);
	}
	return true;
}
/******************************************************************************************
	time:当文件类别为AVI_TYPE_STD时，表示当前帧序号,否则表示当前帧时间
******************************************************************************************/
int CAVIWriter::WriteData(BYTE *pData, int Len, LONGLONG time, bool bIsVideo, bool bKeyFrame, bool bTimeStamp)
{
#if TEST
	assert(pData != NULL);
	assert(Len > 0);
	assert(m_bFileOpen);
#endif
	int hr;
	if(!m_bDataArrived)
	{
		if(!bKeyFrame)
		{
			return AVI_WRITER_SECC;
		}

		m_bDataArrived = true;
		m_firstFrameTime = time;
	}

	if(m_aviType == AVI_TYPE_DEFSIZE)
	{
		if(m_FileStatus == AVICHECK_FILEFULL)
		{
			return AVI_WRITER_FAIL;
		}
		//buf多减8个字节，帧多减4防止边界误差
		if( (m_curWritePos+m_bufDataLen+Len+2*sizeof(m_flagDataBlock)+sizeof(time)+1/*时间戳占一个BYTE*/) >= (INDEX_OFFSET - sizeof(m_flagIndex)-8)
			|| (m_flagTotalFrame[5]+m_bufFramesNum+2) >= (MAXFRAMES-4) )
		{
			m_FileStatus = AVICHECK_FILEFULL;
			return AVI_WRITER_FAIL;
		}
	}

	if(bIsVideo)
	{
		if(m_aviType != AVI_TYPE_STD) //非标准AVI文件
		{
			m_flagDataBlock[0] = INDEX_VIDEOTIME;
		
			hr =WriteTimeToFile(time,bTimeStamp);

			if(hr != AVI_WRITER_SECC)
			{
				m_FileStatus = AVICHECK_FILEFULL;
				return hr;
			}
		}
		else
		{
			for(int index=m_nPreFrameIndex+1; index<time; index++)
			{
				m_flagDataBlock[0] = INDEX_CVIDEO;
				m_flagDataBlock[1] = 0;
				m_strvhdr.dwLength++;
				hr = AVIWriteNoSize((BYTE*)m_flagDataBlock, sizeof(m_flagDataBlock),NULL, 0, false);
				if(hr != AVI_WRITER_SECC)
				{
					m_FileStatus = AVICHECK_FILEFULL;
					return hr;
				}
			}

			m_nPreFrameIndex = (unsigned long)time;	//此时time代表index，所以可以安全转换。
		}
		
		if(bKeyFrame)
		{
			m_flagDataBlock[0] = INDEX_BVIDEO;
		}
		else
		{
			m_flagDataBlock[0] = INDEX_CVIDEO;
		}

		m_flagDataBlock[1] = Len;
		if(m_strvhdr.dwSuggestedBufferSize < (DWORD)Len)
		{
			m_strvhdr.dwSuggestedBufferSize = Len;
		}

		m_strvhdr.dwLength++;
		m_bufVideoNum++;
		m_flagTotalFrame[5] += 2;
#if TEST
			assert(m_flagDataBlock[1] == (unsigned long)Len);
#endif
		if(m_aviType == AVI_TYPE_DEFSIZE)
		{
			//单帧数据长度超出BUF长度
			if ((sizeof(m_flagDataBlock)+Len) > m_maxBufSize)
			{
				printf("\n\n Please playback the record data, Whether normal? ");
				PrintfBit64(time, false);
//				printf("%s %d FrameSize = %ld\n\n\n", __FILE__, __LINE__, sizeof(m_flagDataBlock)+Len);
			}
			
			hr = AVIWriteDefSize((BYTE*)m_flagDataBlock, sizeof(m_flagDataBlock), pData, Len, bKeyFrame);
		}
		else
		{
			hr = AVIWriteNoSize((BYTE*)m_flagDataBlock, sizeof(m_flagDataBlock), pData, Len, bKeyFrame);
		}
	}
	else
	{
		if(m_aviType != AVI_TYPE_STD)
		{
			m_flagTotalFrame[5] += 2;
			m_flagDataBlock[0] = INDEX_AUDIOTIME;
			hr =WriteTimeToFile(time,bTimeStamp);

			if(hr != AVI_WRITER_SECC)
			{
				return hr;
			}
		}

		m_flagDataBlock[0] = INDEX_AUDIO;
		m_flagDataBlock[1] = Len;
		m_strahdr.dwLength += Len;//声音每个采样为一帧
		if(m_aviType == AVI_TYPE_DEFSIZE)
		{
			hr = AVIWriteDefSize((BYTE*)m_flagDataBlock, sizeof(m_flagDataBlock), pData, Len, bKeyFrame);
		}
		else
		{
			hr = AVIWriteNoSize((BYTE*)m_flagDataBlock, sizeof(m_flagDataBlock), pData, Len, bKeyFrame);
		}
	}

	if(m_aviType == AVI_TYPE_DEFSIZE)
	{
		if((m_curWritePos+m_bufDataLen)+AVI_CHANGEFILESIZE >= (INDEX_OFFSET - sizeof(m_flagIndex))
			 || (long)(m_flagTotalFrame[5]+m_bufFramesNum+AVI_CHANGEFILEFRAMES) >= MAXFRAMES)
		{
			m_FileStatus = AVICHECK_MAYCHANGEFILE;
		}
		else if( (m_curWritePos+m_bufDataLen)+AVI_WOULDFULLSIZE >= (INDEX_OFFSET - sizeof(m_flagIndex))
			 || (long)(m_flagTotalFrame[5]+m_bufFramesNum+AVI_WOULDFULLFRAMES) >= MAXFRAMES )
		{
			m_FileStatus = AVICHECK_FILEWOULDFULL;
		}
		else
		{
			m_FileStatus = AVICHECK_NORMAL;
		}
	}
	return hr;
}

void CAVIWriter::Close(void)
{
	if(m_bFileOpen)
	{

//把缓存数据写入文件
		if(m_aviType == AVI_TYPE_DEFSIZE)
		{
			//	把缓存中的数据写入文件
			if(m_bufDataLen > 0 )
				fwrite(m_pData, m_bufDataLen, 1, m_pFile);	//把数据写入文件
			FileFlush(m_pFile);
			
			UpdateFrameIndex(m_bufFramesNum);
			
			m_flagMovie[1] += m_bufDataLen;				//更新数据块大小
			m_flagIndex[1] += m_bufFramesNum;			//更新索引数
		}
		else
		{
			if(m_bufDataLen > 0 )
				fwrite(m_pData, 1, m_bufDataLen, m_pFile);	//把数据写入文件
			m_curWritePos += m_bufDataLen;				//更新文件大小
			m_flagMovie[1] += m_bufDataLen;				//更新数据块大小
			int align = m_flagMovie[1] % 4;
			m_flagMovie[1] = m_flagMovie[1]+4-align;
			m_curWritePos = m_curWritePos + 4 - align;
			m_curWritePos += sizeof(m_flagIndex);
			m_curWritePos += sizeof(AVIINDEXENTRY) * m_indexNum;
			m_flagRIFF[1] = m_curWritePos-sizeof(m_flagRIFF)+sizeof(m_flagRIFF[2]);
		}
		
		fseek(m_pFile, 0, SEEK_SET);
		fwrite(m_flagRIFF, sizeof(m_flagRIFF), 1, m_pFile);

		fseek(m_pFile, sizeof(m_flagList)+sizeof(m_flagHeadList), SEEK_CUR); 
		fwrite(&m_aviHeader, sizeof(AVIMAINHEADER), 1, m_pFile);
		
		fseek(m_pFile, sizeof(m_flagStrList[0]), SEEK_CUR);
		fwrite(&m_strvhdr, sizeof(AVISTREAMHEADER), 1, m_pFile);
		fseek(m_pFile, m_flagStrList[0][1]-sizeof(AVISTREAMHEADER)-sizeof(unsigned long), SEEK_CUR);
		
		fseek(m_pFile, sizeof(m_flagStrList[1]), SEEK_CUR);
		fwrite(&m_strahdr, sizeof(AVISTREAMHEADER), 1, m_pFile);
		fseek(m_pFile, m_flagStrList[1][1]-sizeof(AVISTREAMHEADER)-sizeof(unsigned long), SEEK_CUR);

		fseek(m_pFile, sizeof(m_flagStrList[2]), SEEK_CUR);
		fwrite(&m_strvTimehdr, sizeof(AVISTREAMHEADER), 1, m_pFile);
		fseek(m_pFile, m_flagStrList[2][1]-sizeof(AVISTREAMHEADER)-sizeof(unsigned long), SEEK_CUR);

		fseek(m_pFile, sizeof(m_flagStrList[3]), SEEK_CUR);
		fwrite(&m_straTimehdr, sizeof(AVISTREAMHEADER), 1, m_pFile);
		fseek(m_pFile, m_flagStrList[3][1]-sizeof(AVISTREAMHEADER)-sizeof(unsigned long), SEEK_CUR);

		fwrite(m_flagTotalFrame, sizeof(m_flagTotalFrame), 1, m_pFile);

		if(m_aviType == AVI_TYPE_DEFSIZE)
		{
			m_flagIndex[1] *= sizeof(AVIINDEXENTRY);
			fseek(m_pFile, INDEX_OFFSET-sizeof(m_flagIndex), SEEK_SET);
			fwrite(m_flagIndex, sizeof(m_flagIndex), 1, m_pFile);
		}
		else
		{
			fseek(m_pFile, MOVI_POS_OFFSET-sizeof(m_flagMovie), SEEK_SET);
			fwrite(m_flagMovie, sizeof(m_flagMovie), 1, m_pFile);

			m_flagIndex[1] = m_indexNum*sizeof(AVIINDEXENTRY);
			fseek(m_pFile, m_flagMovie[1]-sizeof(m_flagMovie[2]), SEEK_CUR);
			fwrite(m_flagIndex, sizeof(m_flagIndex), 1, m_pFile);
			if(m_indexNum > 0)
				fwrite(m_pIndexData, sizeof(AVIINDEXENTRY), m_indexNum, m_pFile);
		}
		//	写文件结束标记(正常结束)
		fseek(m_pFile, POS_AVINORMAL, SEEK_SET);
		unsigned long endMark = AVI_NORMAL;
		fwrite(&endMark, SIZE_AVINORMAL, 1, m_pFile);
		FileFlush(m_pFile);

#if defined(__TDFH__) || defined(__CHIPTI__)
		if (fsync(fileno(m_pFile)))
		{
			printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
		}
#endif

		fclose(m_pFile);
		m_pFile = NULL;
		if(m_pData != NULL)
		{
			delete [] m_pData;
			m_pData = NULL;
		}
		if(m_pIndexData != NULL)
		{
			delete [] m_pIndexData;
			m_pIndexData = NULL;
		}
	}
	m_bFileOpen = false;
	m_FileStatus = AVICHECK_NOOPEN;
}

int CAVIWriter::AVIWriteNoSize(BYTE *pCKHeader, int Len1, BYTE *pData, int Len2, bool bKeyFrame)
{
	if( ((m_bufDataLen+Len1+Len2) >= m_maxBufSize) ||
		m_bufVideoNum >= m_maxBufFmNum)
	{
		if (m_bufDataLen > 0)
		{
			if(1 != fwrite(m_pData, m_bufDataLen, 1, m_pFile))	//把数据写入文件
			{
				return AVI_WRITER_FAIL;
			}
		}

#if SUPPORT_FILE_FLUSH
		FileFlush(m_pFile);
#endif

		m_curWritePos += m_bufDataLen;					//更新文件大小
		m_flagMovie[1] += m_bufDataLen;				//更新数据块大小
		m_bufFramesNum = 0;
		m_bufDataLen = 0;
		m_bufVideoNum = 0;

		if ((unsigned long)(Len1+Len2) >= m_maxBufSize)
		{
			if(1 != fwrite(pCKHeader, Len1, 1, m_pFile))	//把数据写入文件
			{
				return AVI_WRITER_FAIL;
			}
			
			if(1 != fwrite(pData, Len2, 1, m_pFile))	//把数据写入文件
			{
				return AVI_WRITER_FAIL;
			}
			
#if SUPPORT_FILE_FLUSH
			FileFlush(m_pFile);
#endif
			
			m_pIndexData[m_indexNum].ckid = ((unsigned long*)pCKHeader)[0];
			m_pIndexData[m_indexNum].dwFlags = bKeyFrame ? AVIIF_KEYFRAME : 0;
			m_pIndexData[m_indexNum].dwChunkOffset = m_flagMovie[1] + m_bufDataLen;
			m_pIndexData[m_indexNum].dwChunkLength = Len2;
			m_indexNum++;

			m_curWritePos += (Len1+Len2);					//更新文件大小
			m_flagMovie[1] += (Len1+Len2);				//更新数据块大小
			
			if(m_indexNum >= m_nMaxIndex)
			{
				m_nMaxIndex += AVI_FRAME_INDEX_UNIT;
				AVIINDEXENTRY *pIndex = new AVIINDEXENTRY[m_nMaxIndex];
				memcpy(pIndex, m_pIndexData, sizeof(AVIINDEXENTRY)*m_indexNum);
				delete [] m_pIndexData;
				m_pIndexData = pIndex;
			}

			return AVI_WRITER_SECC;
		}
	}
	
	m_pIndexData[m_indexNum].ckid = ((unsigned long*)pCKHeader)[0];
	m_pIndexData[m_indexNum].dwFlags = bKeyFrame ? AVIIF_KEYFRAME : 0;
	m_pIndexData[m_indexNum].dwChunkOffset = m_flagMovie[1] + m_bufDataLen;
	m_pIndexData[m_indexNum].dwChunkLength = Len2;
	m_indexNum++;

	if(m_indexNum >= m_nMaxIndex)
	{
		m_nMaxIndex += AVI_FRAME_INDEX_UNIT;
		AVIINDEXENTRY *pIndex = new AVIINDEXENTRY[m_nMaxIndex];
		memcpy(pIndex, m_pIndexData, sizeof(AVIINDEXENTRY)*m_indexNum);
		delete [] m_pIndexData;
		m_pIndexData = pIndex;
	}

	memcpy(m_pData+m_bufDataLen, pCKHeader, Len1);
	m_bufDataLen += Len1;
	if(Len2 > 0)
	{
		memcpy(m_pData+m_bufDataLen, pData, Len2);
		m_bufDataLen += Len2;
	}
	m_bufFramesNum++;

	return AVI_WRITER_SECC;
}

int CAVIWriter::AVIWriteDefSize(BYTE *pCKHeader, int Len1, BYTE *pData, int Len2, bool bKeyFrame)
{
	if( ((m_bufDataLen+Len1+Len2) >= m_maxBufSize) ||
		m_bufVideoNum >= m_maxBufFmNum ||
		(m_bufFramesNum+2) >= m_maxBufIndexNum)
	{
		if (m_bufDataLen > 0)
		{
			if(1 != fwrite(m_pData, m_bufDataLen, 1, m_pFile))	//把数据写入文件
			{
				return AVI_WRITER_FAIL;
			}
#if CHECK_IO_TEST
			else
			{
				FileFlush(m_pFile);

				long offset = 0 - m_bufDataLen;
				if (0 == fseek(m_pFile, offset, SEEK_CUR))
				{
					unsigned char *pBuf = new unsigned char [m_bufDataLen];
					if (1 == fread(pBuf, m_bufDataLen, 1, m_pFile))
					{
						for (unsigned long i=0; i<m_bufDataLen; ++i)
						{
							assert (pBuf[i] == m_pData[i]);
						}
					}
					delete [] pBuf;
				}
			}
#endif
		}

#if SUPPORT_FILE_FLUSH
		FileFlush(m_pFile);
#endif
		
		if(!UpdateFrameIndex(m_bufFramesNum))
		{
			return AVI_WRITER_FAIL;
		}
		
		m_curWritePos += m_bufDataLen;
		m_flagMovie[1] += m_bufDataLen;				//更新数据块大小
		m_flagIndex[1] += m_bufFramesNum;			//更新索引数
		m_bufFramesNum = 0;
		m_bufDataLen = 0;
		m_bufVideoNum = 0;
		fseek(m_pFile, m_curWritePos, SEEK_SET);
		
		//单帧数据长度超出BUF长度
		if ((unsigned long)(Len1+Len2) > m_maxBufSize)
		{
			if(1 != fwrite(pCKHeader, Len1, 1, m_pFile))	//把数据写入文件
			{
				return AVI_WRITER_FAIL;
			}
			
			if(1 != fwrite(pData, Len2, 1, m_pFile))	//把数据写入文件
			{
				return AVI_WRITER_FAIL;
			}
#if CHECK_IO_TEST
			else
			{
				FileFlush(m_pFile);

				long offset = 0 - Len2;
				if (0 == fseek(m_pFile, offset, SEEK_CUR))
				{
					unsigned char *pBuf = new unsigned char [Len2];
					if (1 == fread(pBuf, Len2, 1, m_pFile))
					{
						for (unsigned long i=0; i<Len2; ++i)
						{
							assert (pBuf[i] == pData[i]);
						}
					}
					delete [] pBuf;
				}
			}
#endif

#if SUPPORT_FILE_FLUSH
			FileFlush(m_pFile);
#endif
			
			m_pIndexData[0].ckid = ((unsigned long*)pCKHeader)[0];
			m_pIndexData[0].dwFlags = bKeyFrame ? AVIIF_KEYFRAME : 0;
			m_pIndexData[0].dwChunkOffset = m_curWritePos-MOVI_POS_OFFSET+4 + m_bufDataLen;
			m_pIndexData[0].dwChunkLength = Len2;
			if(!UpdateFrameIndex(1))
			{
				return AVI_WRITER_FAIL;
			}
			
			m_curWritePos += (Len1+Len2);
			m_flagMovie[1] += (Len1+Len2);				//更新数据块大小
			++ m_flagIndex[1];			//更新索引数
			fseek(m_pFile, m_curWritePos, SEEK_SET);
			return AVI_WRITER_SECC;
		}
	}
	
	m_pIndexData[m_bufFramesNum].ckid = ((unsigned long*)pCKHeader)[0];
	m_pIndexData[m_bufFramesNum].dwFlags = bKeyFrame ? AVIIF_KEYFRAME : 0;
	m_pIndexData[m_bufFramesNum].dwChunkOffset = m_curWritePos-MOVI_POS_OFFSET+4 + m_bufDataLen;
	m_pIndexData[m_bufFramesNum].dwChunkLength = Len2;

	memcpy(m_pData+m_bufDataLen, pCKHeader, Len1);
	m_bufDataLen += Len1;
	memcpy(m_pData+m_bufDataLen, pData, Len2);
	m_bufDataLen += Len2;
	m_bufFramesNum++;

	return AVI_WRITER_SECC;
}
//刷新帧索引缓冲到文件
bool CAVIWriter::UpdateFrameIndex(unsigned long bufFramesNum )
{
	//清空后面的索引，因为这些索引也会被写进磁盘来避免异常关机的错误
	memset(&m_pIndexData[bufFramesNum], 0, sizeof(AVIINDEXENTRY)*(m_maxBufIndexNum-bufFramesNum));
	if (0 != fseek(m_pFile, INDEX_OFFSET+m_flagIndex[1]*sizeof(AVIINDEXENTRY), SEEK_SET))
	{
		return false;
	}
	int realwriteindex=sizeof(AVIINDEXENTRY)*(m_maxBufIndexNum*2);
	//判断是否超过了文件长度
	if((long)(INDEX_OFFSET+(m_flagIndex[1]+m_maxBufIndexNum*2)*sizeof(AVIINDEXENTRY)) > (AVIFILELENGTH))
	{
		//可以写的实际空间
		realwriteindex=(AVIFILELENGTH)-(INDEX_OFFSET+m_flagIndex[1]*sizeof(AVIINDEXENTRY));
		//取完整帧数量
		realwriteindex=(realwriteindex/sizeof(AVIINDEXENTRY))*sizeof(AVIINDEXENTRY);
	}
	if(1 != fwrite(m_pIndexData, realwriteindex, 1, m_pFile))
	{
		return false;
	}

#if SUPPORT_FILE_FLUSH
	FileFlush(m_pFile);
#endif

	return true;
}

/******************************************************************************
*	查询文件的大小用于换文件，有以下几种情况
*	1.	如果文件没有打开返回-1
*	2.	如果文件里自打开后还没有写过一帧数据返回0
*	3.	其它情况直接返回当前文件的实际大小，单位为字节
******************************************************************************/
int CAVIWriter::GetFileLength(void)
{
	if(!m_bFileOpen)
		return -1;
	if(!m_bDataArrived)
		return 0;
	return m_curWritePos + sizeof(m_flagIndex) + sizeof(AVIINDEXENTRY)*m_indexNum
			+ m_bufDataLen;
}

//得到写入文件的第一帧视频或声音的时间
LONGLONG CAVIWriter::GetFirstFrameTime(void)
{
	if(m_bFileOpen && m_bDataArrived)
		return m_firstFrameTime;
	return -1;
}

int CAVIWriter::FileCheck(void)
{
	return m_FileStatus;
}

void CAVIWriter::Flush()
{
}

int CAVIWriter::WriteTimeToFile(LONGLONG time, bool timestamp)
{	
	int hr=0;
	const BYTE timeBuflen=TimeBufLen;
	m_flagDataBlock[1] = timeBuflen;
	m_straTimehdr.dwLength++;
	BYTE timeBuf[timeBuflen];
	memcpy(timeBuf,&time,sizeof(LONGLONG));
	if(timestamp)
		timeBuf[timeBuflen-1]=1;
	else
		timeBuf[timeBuflen-1]=0;

	if(m_aviType == AVI_TYPE_DEFSIZE)
	{
		hr = AVIWriteDefSize((BYTE*)m_flagDataBlock, sizeof(m_flagDataBlock),\
			(BYTE*)timeBuf, timeBuflen, true);
	}
	else
	{
		hr = AVIWriteNoSize((BYTE*)m_flagDataBlock, sizeof(m_flagDataBlock),\
			(BYTE*)timeBuf, timeBuflen, true);
	}
	return hr;
}

