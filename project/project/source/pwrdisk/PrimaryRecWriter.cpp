
//RecWriter.cpp
#include "PUB_common.h"
#include "PrimaryRecWriter.h"

#ifdef _WIN32
#include "fsinfo.h"
#include "Scsi_hdd_cmd.h"
#else
#include "Mount.h"
#include "trace_define.h"
#endif

unsigned long CPrimaryRecWriter::m_lastFFlushTime = 0;
unsigned long CPrimaryRecWriter::m_openIndex = 0;

CPrimaryRecWriter::CPrimaryRecWriter ()
{
	m_bCombined = false;
	m_pRec = new CRecord_file();
	m_fileBufType = FILE_BUF_TYPE_LOCAL;
	
#if defined(__CHIP3531__) || defined(__CHIP3535__)
	m_headspace = m_openIndex * 1024 * 1024;
	m_openIndex ++;
	m_openIndex = (m_openIndex % 32);
#else
	m_headspace = 0;
#endif
}

CPrimaryRecWriter::~CPrimaryRecWriter ()
{
	if(NULL != m_pRec)
	delete m_pRec;
}

bool CPrimaryRecWriter::Open(const char *pPath,unsigned long fileSizeM, bool bCombined)
{
	char dev[20] = {0};
	char fileName[20] = {0};
	int ret = 0;
	int align_offset = 0; //补齐扇区大小
	int idx_size = 0;

#ifdef _WIN32
	const char* pFile;
	DWORD diskNumberPtr,partitionNumberPtr;
	BOOL bRet;

	CFSInfoTable * pfsInfo = CFSInfoTable::getSingleton();
	CFat32Part * pfat = NULL;
	if((pFile = strstr(pPath,"\\")))
	{
		strcpy(fileName,pFile+1);
	}
	else
	{
		TRACE("File path err.\n");
		return false;
	}
	//先使用WindowsAPI进行获取 卷名对应的 设备名和分区号  F:  PhysicalDrive1  partition1
	bRet = CSCSI_HDD::GetDiskAndPartitionNumbers(pPath[0],&diskNumberPtr,&partitionNumberPtr);
	if(bRet)
	{
		sprintf(dev, "\\\\.\\PhysicalDrive%d%d", diskNumberPtr,partitionNumberPtr);
		pfat = pfsInfo->GetPart((char)diskNumberPtr,(int)partitionNumberPtr);
		pfsInfo->SetMapFat(pPath[0],pfat);
		pfat->SetPhysicalPart(dev);
	}
	else
	{
		pfat = pfsInfo->GetPart(pPath[0]);
		strcpy(dev,pfat->GetPhysicalPart());
	}

	if(pfat == NULL)
	{
		return false;
	}
	pfat->SetDrivePath((char*)pPath);

	ret = pfat->lcok_volume();
	if(ret < 0)
	{
		TRACE("Lock volume .\n");
	//	return false;
	}
#else
	int tryCount = 0;

	while (!CMount::Instance()->GetMountInfo(dev, fileName, pPath))
	{
		CMount::Instance()->LoadMountInfo();
		tryCount++;
		if (100 < tryCount)
		{
			break;
		}
		PUB_Sleep(100);
	}
#endif
	m_bCombined = bCombined;
	if (128 == fileSizeM)
	{
		m_MAX_REC_SUB_OFF_LEN	  = 4 * 1024 * 1024;			//当剩余空间小于此值时，一旦进入关键帧，则不能写入且返回失败
		m_MAX_REC_FILE_LEN	      = 128 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 16*m_MAX_REC_FILE_LEN/(1024*1024);	//一个文件最多可以写入的关键帧索引（根据码流256KBPS，和100帧一个关键帧算出）。
	}
	else if (512 == fileSizeM)
	{
#if defined(__CHIP3531__) || defined(__CHIP3535__)
		m_MAX_REC_SUB_OFF_LEN	  = 36 * 1024 * 1024;
#else
		m_MAX_REC_SUB_OFF_LEN	  = 10 * 1024 * 1024;
#endif
		m_MAX_REC_FILE_LEN	      = 512 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 8192;
	}
	else if (256 == fileSizeM)
	{
#if defined(__CHIP3531__) || defined(__CHIP3535__)
		m_MAX_REC_SUB_OFF_LEN	  = 36 * 1024 * 1024;
#else
		m_MAX_REC_SUB_OFF_LEN	  = 10 * 1024 * 1024;
#endif
		m_MAX_REC_FILE_LEN	      = 256 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 4096;
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

#if 0
	fileName = "00000000.dat";
#endif
	if(m_pRec->fopen(dev,fileName,SCSI_WRONLY, m_fileBufType))
	{
		TRACE_ERR("Open ts %s fail.\n",dev,fileName);
		goto ERR_OUT;
	}

	memset(&m_fileHeader, 0, sizeof(REC_FILE_HEADER));
	if (m_bCombined)
	{
		m_fileHeader.hFlag = HEADER_FLAG_COMBINE;		//该信息头标记
	}
	else
	{
		m_fileHeader.hFlag = HEADER_FLAG;					//该信息头标记
	}
	m_fileHeader.hLen = sizeof(REC_FILE_HEADER);		//该信息头长度
	m_fileHeader.frameInfoLen = sizeof(REC_FRAME_INFO);				//
	m_fileHeader.fileFlag = FILE_FLAG;					//文件标记——"FTVT"

	//m_fileHeader.fileLen = m_FILE_BASIC_LEN;			//文件总长度

	m_fileHeader.writeFlg = WRITE_ING_FLAG;				//写标记
	m_fileHeader.videoFormatAddr = m_VIDEO_FORMAT_POS;	//视频格式位置
	m_fileHeader.audioFormatAddr = m_AUDIO_FORMAT_POS;	//音频格式位置
	m_fileHeader.indexAddr = INDEX_INFO_POS;			//索引区位置

	//m_fileHeader.dataAddr = m_FRAME_DATA_POS;			//数据区位置
	//将数据区的地址对齐到扇区的倍数
	m_fileHeader.fileLen = m_fileHeader.dataAddr = SIZE_ROUND_UP(m_FRAME_DATA_POS,m_pRec->get_sec_size());			//数据区位置
	m_startTime = 0;
	m_endTime = 0;

	/*
	//  简化文件头创建，以下步骤全部合并到一起
	//  创建录像文件头
	//  清空索引区
	//  清空视频格式
	//  清空音频格式
	//  写组合备份可选
	//  还需要清空第一帧的信息
	*/
	idx_size = m_MAX_KEY_FRAME_NUM * sizeof(INDEX_INFO);
	ret = m_pRec->InitHead((unsigned char*)&m_fileHeader,sizeof(REC_FILE_HEADER),idx_size,INDEX_INFO_POS,m_fileHeader.dataAddr);
	if(0 != ret)
	{
		TRACE("Init rec-head fail. ret %d \n", ret);
		goto ERR_OUT;
	}

	if(0 != m_pRec->Seek(m_fileHeader.dataAddr,SEEK_SET))
	{
		goto ERR_OUT;
	}
	
	//初始化相关变量
	m_IndexNum	= 0;
	m_videoNum	= 0;
	m_audioNum	= 0;
	m_textNum	= 0;

	m_indexPos	= INDEX_INFO_POS;
	m_filePos = m_fileHeader.dataAddr; // m_bufInfo.filePos = m_fileHeader.dataAddr;

	m_bHasVideo	= false;
	m_bHasAudio	= false;

	memset(&m_frameInfo, 0, sizeof(REC_FRAME_INFO));
	memset(&m_indexInfo, 0, sizeof(INDEX_INFO));
	m_indexInfo.flag = I_VIDEO_FLAG;
	
	//前面参数都初始化好就可以开始刷新数据
	m_pRec->Start_FlushData();

	return true;
ERR_OUT:
	m_pRec->close();
	TRACE_ERR("Open  rec-file %s %s  fail.\n", dev, fileName);
	return false;
}

int CPrimaryRecWriter::WriteFileEventInfo(REC_FILE_EVENT_INFO *pEventInfo, int num)
{
	int writeNum = num;
	int writeSize = 0;
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

	writeSize =  sizeof(REC_FILE_EVENT_INFO) * writeNum;
	TRACE("WriteFileEventInfo .");
	if(writeSize != m_pRec->Write_pos(m_REC_FILE_EVENT_INFO_POS,(unsigned char* )pEventInfo,writeSize))
	{
		TRACE("WriteFileEventInfo fail.");
		return -1;
	}
	return 0;
}

void  CPrimaryRecWriter::StopRecrod()
{
	m_pRec->m_bsuspense = true;
	//TRACE("CH %d Stop rec \n",m_pRec->get_channel())
}

void CPrimaryRecWriter::Close ()
{

	//清除音频列表
	while(m_audioList.GetCount() > 0)
	{
		WriteFrame(m_audioList.GetHead());

		delete [] m_audioList.RemoveHead().pData;
	}
	//写文件头
	{
		m_fileHeader.fileLen = m_filePos;
		m_fileHeader.indexNum = m_IndexNum;
		m_fileHeader.videoNum = m_videoNum;
		m_fileHeader.audioNum = m_audioNum;
		m_fileHeader.textNum = m_textNum;
		m_fileHeader.startTime = m_startTime;
		m_fileHeader.endTime = m_endTime;
		m_fileHeader.writeFlg = WRITE_END_FLAG;
		//写入结束的文件头
		//TRACE("Rec close  video FormatAddr %u  , audioFormatAddr %u  indexAddr %u dataAddr %u \n",m_fileHeader.videoFormatAddr,m_fileHeader.audioFormatAddr,
		//		m_fileHeader.indexAddr,m_fileHeader.dataAddr);
		//PrintfBit64(m_startTime,false);
		//PrintfBit64(m_endTime,false);
	}
	//将文件头信息写完
	m_pRec->SetHeadData(CHANGED_HEAD,0,(unsigned char *)&m_fileHeader,sizeof(REC_FILE_HEADER));
	//关闭录像文件，在这过程中，会将FIFO缓冲中的所有录像数据写入到硬盘当中，
	m_pRec->close();

}

bool CPrimaryRecWriter::FFlush()
{
	return true;
}

/******************************************************************************
函数功能：写入一帧数据(音频，视频或文本),要求每一类型各自有序输入
参数说明：frame：要写入的数据帧，数据段keyFrame、frameType、length、pData、time
		  必须填充
返回值：成功返回：WRITE_FRAME_SUCC
		失败返回：WRITE_FRAME_NEED_VIDEO_FORMAT(需要视频格式)
				  WRITE_FRAME_NEED_AUDIO_FORMAT(需要音频格式)
				  WRITE_FRAME_FULL(文件已经写满)
				  WRITE_FRAME_FAIL(磁盘写出错或其他异常错误)
******************************************************************************/
unsigned long CPrimaryRecWriter::WriteData(const FRAME_INFO_EX & frame)
{
	if (m_pRec->GetBufSize(m_fileBufType) <= frame.length)
	{
		return WRITE_FRAME_SUCC;
	}

	if (!m_bHasVideo && (FRAME_TYPE_VIDEO_FORMAT == frame.frameType))
	{
		assert (sizeof(BITMAPINFOHEADER) == frame.length);

		if(sizeof(BITMAPINFOHEADER) == m_pRec->SetHeadData(CHANGED_VIDEO_FMT,m_fileHeader.videoFormatAddr,frame.pData,sizeof(BITMAPINFOHEADER)))
		{
		//	TRACE_DBG("Write videoFormat OK. CH %d \n",frame.channel);
			m_pRec->set_channel(frame.channel);
		//	TRACE_DBG("Set channel OK. CH %d  Lenth %d frameType %d \n",frame.channel,frame.length,frame.frameType);
			m_bHasVideo = true;
			return WRITE_FRAME_SUCC;
		}

		return WRITE_FRAME_FAIL;
	}
	else if (!m_bHasAudio && (FRAME_TYPE_AUDIO_FORMAT == frame.frameType))
	{
		assert (sizeof(WAVEFORMATEX) == frame.length);
		if(sizeof(WAVEFORMATEX) == m_pRec->SetHeadData(CHANGED_AUDIO_FMT,m_fileHeader.audioFormatAddr,frame.pData,sizeof(WAVEFORMATEX)))
		{
			//TRACE_DBG("Write audioFormatAddr OK.\n");;
			m_bHasAudio = true;
			return WRITE_FRAME_SUCC;
		}

		return WRITE_FRAME_FAIL;
	}
	else if (m_bHasVideo && (FRAME_TYPE_VIDEO == frame.frameType))
	{
		return OnWriteOneVideo(frame);
	}
	else if (FRAME_TYPE_AUDIO == frame.frameType)//要求输入的音频按时间有序,把新的音频帧加入到音频帧列表尾
	{
		if(m_videoNum==0)
		{
			printf("Audio frames come;No Video!\n ");
			return  WRITE_FRAME_SUCC;
		}
		if(!m_bHasAudio)	//音频格式尚未到达
		{

			return WRITE_FRAME_NEED_AUDIO_FORMAT;
		}

#if 0
//#if defined(__TDFH__)
		//音频流的时间小于于当前文件的开始时间则认为音频时间错
		//音频流的时间和视频流的时间不能相差很大，如果相差很大，会导致丢掉一些音频数据
		if (frame.time < m_fileHeader.startTime)
		{
			//printf("%s:%s:%d, The audio time is < the file start time\n", __FUNCTION__, __FILE__, __LINE__);
			//PrintfBit64(frame.time, true);
			//PrintfBit64(m_fileHeader.startTime, true);
			//PrintfBit64(frame.time, false);
			//PrintfBit64(m_fileHeader.startTime, false);
			return WRITE_FRAME_SUCC;
		}
#endif
		//音频数量过多时，从列表中删除最老的一帧音频，以在加入最新的一帧音频时数量没有超标
		if (m_audioList.GetCount() >= AUDIO_BUF_NUM)
		{
			delete [] m_audioList.RemoveHead().pData;
		}

		//音频要求递增有序，否则断言且不加入列表

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
	else if (FRAME_TYPE_TEXT == frame.frameType)	//文本数据暂不处理
	{
		//将文本有序地加入到文本列表中;
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
函数功能：向文件中写入帧索引
调用顺序：要求在关键帧来时
******************************************************************************/
unsigned long CPrimaryRecWriter::WriteIndex(unsigned long index, unsigned long pos, LONGLONG time)
{
	//意味着预留的Index区域已经用完，不能再写入新的帧，所以返回失败。
	int ret;
	if (m_MAX_KEY_FRAME_NUM <= m_IndexNum)
	{
		TRACE_ERR("Key Frame is over flow cur-idx %d ,max-idx %d",m_IndexNum, m_MAX_KEY_FRAME_NUM);
		return WRITE_FRAME_FULL;
	}

	m_indexInfo.index		= index;
	m_indexInfo.position	= pos;
	m_indexInfo.time		= time;

	//利用底层CRecord_file类提供的FIFO 缓冲，录像线程当中会不断循环去刷新
	 ret = m_pRec->Set_KeyFrameIdx((unsigned char* )&m_indexInfo,sizeof(INDEX_INFO),pos);

#if defined(__TDFH__)
	//FH平台在操作系统层缓冲数据太多，导致在异常断电时关键帧的索引没有写入
 	//FFlushEx(m_pFile);
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
函数功能：写入一帧数据的具体实现
实现方法：先将数据写入缓冲区，然后再多帧统一写入文件
******************************************************************************/
unsigned long CPrimaryRecWriter::WriteFrame(const FRAME_INFO_EX & frame)
{
	//构造帧信息结构
	uint32_t write_size;	//写入数据大小
	uint32_t rec_size;		//写入完成后，录像文件的大小
	int ret;				//返回值

	{
		if (FRAME_TYPE_VIDEO == frame.frameType)
		{
			m_frameInfo.flag	= frame.keyFrame ? I_VIDEO_FLAG : B_VIDEO_FLAG;
		}
		else if (FRAME_TYPE_AUDIO == frame.frameType)
		{
			if(m_videoNum==0)
			{
				printf("Audio frames come;No Video!\n ");
				return  WRITE_FRAME_SUCC;
			}
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
		//写第一帧数据
		if(0 == m_frameInfo.nextPos)
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
#if 0
//#if defined(__TDFH__)
		m_frameInfo.timeStampFlag = 0;
		m_frameInfo.timeStampFlag = RecFrameInfoChsum(&m_frameInfo);
#else
		m_frameInfo.timeStampFlag = 1;
#endif
	}

	//问题：备份一个录像长度大约为1小时20分钟的文件，必然失败。
	//原因：问题出现在这个录像文件本身，是在当时在写这个录像文件时，就出现问题了。
	//由于之前是先判断录像文件是否写满，如果没有写满，就帧写入到文件里，然后判断
	//关键帧是否写满，如果写满就返回失败。这就出现下面的问题：关键帧已经写入文件里，但是返回失败。
	//所以在此处先判断关键帧是否写满。
	if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame)
	{
		//m_IndexNum编号从0开始计数，表示已经写入关键帧的个数。
		//如果当前m_IndexNum为MAX_KEY_FRAME_NUM，即文件已经写入了MAX_KEY_FRAME_NUM个关键帧，
		//而在这时再来一个关键帧的话，那么就没有地方写这个关键帧的索引了，所以要返回满
		if (m_MAX_KEY_FRAME_NUM <= m_IndexNum)
		{
			TRACE_DBG("Rec-file is full.\n");
			return WRITE_FRAME_FULL;
		}
	}

	rec_size =  m_filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO);
	/*写入一帧数据，如果总长度超出文件总长度，则返回文件已经满*/
	if (m_MAX_REC_FILE_LEN >= rec_size)
	{
		write_size = sizeof(REC_FRAME_INFO);
		//写入帧信息头
		if((ret = m_pRec->Write_FIFO((unsigned char* )&m_frameInfo,write_size)) != write_size)
		{
			if(FIFO_ERR_TIMEOUT == ret)
			{
				TRACE_ERR("2 Give up frame.!!!Write frame head time out. ret %d write_size \n",ret);
				return WRITE_FRAME_SUCC;
			}
			TRACE_ERR("Write frame head fail .ret %d \n",ret);
			assert (false);
			return WRITE_FRAME_FAIL;
		}

		//写入帧数据
		if((ret = m_pRec->Write_FIFO(frame.pData,m_frameInfo.length)) != m_frameInfo.length)
		{
			if(FIFO_ERR_TIMEOUT == ret)
			{
				TRACE_ERR("3 Give up frame.!!!Write body data time out. ret %d write_size  %d \n",ret,m_frameInfo.length);
				return WRITE_FRAME_SUCC;
			}
			TRACE_ERR("Write frame body fail.ret %d \n",ret);
			assert (false);
			return WRITE_FRAME_FAIL;
		}
		m_filePos += (sizeof(REC_FRAME_INFO) + frame.length);

	}
	else
	{
		TRACE_ERR("data over size %u max rec lenth %u",rec_size,m_MAX_REC_FILE_LEN);
		return WRITE_FRAME_FULL;
	}

	//视频关键帧，则需要写入索引
	if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame)
	{
		//写失败，则把当前写入数据也看着失败
		if (WRITE_FRAME_FAIL == WriteIndex(m_videoNum, m_frameInfo.nextPos-frame.length-sizeof(REC_FRAME_INFO), frame.time))
		{

			return WRITE_FRAME_FAIL;
		}
	}

	if (FRAME_TYPE_VIDEO == frame.frameType)
	{
		++ m_videoNum;

		if (0 == m_startTime)	//第一个视频帧，需要记录开始时间
		{
			m_startTime = frame.time;
			m_fileHeader.startTime = m_startTime;

		//	TRACE("Write first frame, CH %02d start time ",m_pRec->get_channel());
		//	PrintfBit64(m_startTime,false);
			if(sizeof(REC_FILE_HEADER) != m_pRec->SetHeadData(CHANGED_HEAD,0,(unsigned char*)&m_fileHeader,sizeof(REC_FILE_HEADER)))
			{
				assert(false);
			}
		}
		m_endTime = frame.time;	//只更新视频的时间
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

unsigned long CPrimaryRecWriter::OnWriteOneVideo(const FRAME_INFO_EX & frame)
{
	//2010-01-25 13:48:00 YSW
	//由于采用了缓存和预录像机制，所以及时关键帧的产生有一定的时延，所以在文件写满后
	//再请求关键帧，在时延作用下，必然产生一定的丢失情况（新文件从关键帧开始读取，关键帧前面
	//的帧不能读出，相当于丢失）。但是如果能在一个文件将写满的时候，一旦发现视频关键帧，就不写入
	//该文件，且返回写入失败。外部模块则会将该关键帧先送入预录像模块进行缓存，然后打开新的文件并
	//尝试写入，这样实质上就能达到我们不丢一帧，而文件开头也是关键帧的目的。
	//另外一个限制（关键帧数目的限制），由于写入前首先检查是否为关键帧及是否已经达到最大数目，所以
	//只要是这种情况（关键帧数目满），其在关闭前送入的一帧一定是关键帧。那么也会返回文件满的值，外部处理
	//也是将该帧送入预录像缓存，并打开新文件再写入。

	//根据以上描述，在此检查当前文件中实质写入的数据数目
	if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame)
	{

		//当把此帧写入后，将会进入到文件最后的保留区，则返回满。
		//if ((m_MAX_REC_FILE_LEN - m_MAX_REC_SUB_OFF_LEN) < (m_filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO) + m_bufInfo.dataLen))
		if ((m_MAX_REC_FILE_LEN - m_MAX_REC_SUB_OFF_LEN + m_headspace) < (m_filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO)))
		{
			printf("%s:%s:%d, file full m_filePos %d , file len=%d\n", __FUNCTION__, __FILE__, __LINE__, m_filePos, m_MAX_REC_FILE_LEN - m_MAX_REC_SUB_OFF_LEN + m_headspace);
			return WRITE_FRAME_FULL;
		}
	}

	//将音频列表中比当前帧时间早的帧写入文件,并将这些音频从列表中清除
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

	//将文本列表中比当前帧时间早的帧写入文件

	//写入视频帧
	return WriteFrame(frame);
}

void CPrimaryRecWriter::FFlushEx(FILE * pFile)
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
	//	if (fsync(fileno(pFile)))
		{
			printf("%s:%d, errno=%d\n", __FILE__, __LINE__, errno);
		}
#endif
		m_lastFFlushTime = GetCurrTime32();
	}
}
