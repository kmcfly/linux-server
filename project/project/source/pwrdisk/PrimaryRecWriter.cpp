
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
	int align_offset = 0; //����������С
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
	//��ʹ��WindowsAPI���л�ȡ ������Ӧ�� �豸���ͷ�����  F:  PhysicalDrive1  partition1
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
		m_MAX_REC_SUB_OFF_LEN	  = 4 * 1024 * 1024;			//��ʣ��ռ�С�ڴ�ֵʱ��һ������ؼ�֡������д���ҷ���ʧ��
		m_MAX_REC_FILE_LEN	      = 128 * 1024 * 1024;
		m_MAX_KEY_FRAME_NUM	      = 16*m_MAX_REC_FILE_LEN/(1024*1024);	//һ���ļ�������д��Ĺؼ�֡��������������256KBPS����100֡һ���ؼ�֡�������
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
		m_fileHeader.hFlag = HEADER_FLAG_COMBINE;		//����Ϣͷ���
	}
	else
	{
		m_fileHeader.hFlag = HEADER_FLAG;					//����Ϣͷ���
	}
	m_fileHeader.hLen = sizeof(REC_FILE_HEADER);		//����Ϣͷ����
	m_fileHeader.frameInfoLen = sizeof(REC_FRAME_INFO);				//
	m_fileHeader.fileFlag = FILE_FLAG;					//�ļ���ǡ���"FTVT"

	//m_fileHeader.fileLen = m_FILE_BASIC_LEN;			//�ļ��ܳ���

	m_fileHeader.writeFlg = WRITE_ING_FLAG;				//д���
	m_fileHeader.videoFormatAddr = m_VIDEO_FORMAT_POS;	//��Ƶ��ʽλ��
	m_fileHeader.audioFormatAddr = m_AUDIO_FORMAT_POS;	//��Ƶ��ʽλ��
	m_fileHeader.indexAddr = INDEX_INFO_POS;			//������λ��

	//m_fileHeader.dataAddr = m_FRAME_DATA_POS;			//������λ��
	//���������ĵ�ַ���뵽�����ı���
	m_fileHeader.fileLen = m_fileHeader.dataAddr = SIZE_ROUND_UP(m_FRAME_DATA_POS,m_pRec->get_sec_size());			//������λ��
	m_startTime = 0;
	m_endTime = 0;

	/*
	//  ���ļ�ͷ���������²���ȫ���ϲ���һ��
	//  ����¼���ļ�ͷ
	//  ���������
	//  �����Ƶ��ʽ
	//  �����Ƶ��ʽ
	//  д��ϱ��ݿ�ѡ
	//  ����Ҫ��յ�һ֡����Ϣ
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
	
	//��ʼ����ر���
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
	
	//ǰ���������ʼ���þͿ��Կ�ʼˢ������
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

	//�����Ƶ�б�
	while(m_audioList.GetCount() > 0)
	{
		WriteFrame(m_audioList.GetHead());

		delete [] m_audioList.RemoveHead().pData;
	}
	//д�ļ�ͷ
	{
		m_fileHeader.fileLen = m_filePos;
		m_fileHeader.indexNum = m_IndexNum;
		m_fileHeader.videoNum = m_videoNum;
		m_fileHeader.audioNum = m_audioNum;
		m_fileHeader.textNum = m_textNum;
		m_fileHeader.startTime = m_startTime;
		m_fileHeader.endTime = m_endTime;
		m_fileHeader.writeFlg = WRITE_END_FLAG;
		//д��������ļ�ͷ
		//TRACE("Rec close  video FormatAddr %u  , audioFormatAddr %u  indexAddr %u dataAddr %u \n",m_fileHeader.videoFormatAddr,m_fileHeader.audioFormatAddr,
		//		m_fileHeader.indexAddr,m_fileHeader.dataAddr);
		//PrintfBit64(m_startTime,false);
		//PrintfBit64(m_endTime,false);
	}
	//���ļ�ͷ��Ϣд��
	m_pRec->SetHeadData(CHANGED_HEAD,0,(unsigned char *)&m_fileHeader,sizeof(REC_FILE_HEADER));
	//�ر�¼���ļ�����������У��ὫFIFO�����е�����¼������д�뵽Ӳ�̵��У�
	m_pRec->close();

}

bool CPrimaryRecWriter::FFlush()
{
	return true;
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
	else if (FRAME_TYPE_AUDIO == frame.frameType)//Ҫ���������Ƶ��ʱ������,���µ���Ƶ֡���뵽��Ƶ֡�б�β
	{
		if(m_videoNum==0)
		{
			printf("Audio frames come;No Video!\n ");
			return  WRITE_FRAME_SUCC;
		}
		if(!m_bHasAudio)	//��Ƶ��ʽ��δ����
		{

			return WRITE_FRAME_NEED_AUDIO_FORMAT;
		}

#if 0
//#if defined(__TDFH__)
		//��Ƶ����ʱ��С���ڵ�ǰ�ļ��Ŀ�ʼʱ������Ϊ��Ƶʱ���
		//��Ƶ����ʱ�����Ƶ����ʱ�䲻�����ܴ�������ܴ󣬻ᵼ�¶���һЩ��Ƶ����
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
		//��Ƶ��������ʱ�����б���ɾ�����ϵ�һ֡��Ƶ�����ڼ������µ�һ֡��Ƶʱ����û�г���
		if (m_audioList.GetCount() >= AUDIO_BUF_NUM)
		{
			delete [] m_audioList.RemoveHead().pData;
		}

		//��ƵҪ��������򣬷�������Ҳ������б�

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
unsigned long CPrimaryRecWriter::WriteIndex(unsigned long index, unsigned long pos, LONGLONG time)
{
	//��ζ��Ԥ����Index�����Ѿ����꣬������д���µ�֡�����Է���ʧ�ܡ�
	int ret;
	if (m_MAX_KEY_FRAME_NUM <= m_IndexNum)
	{
		TRACE_ERR("Key Frame is over flow cur-idx %d ,max-idx %d",m_IndexNum, m_MAX_KEY_FRAME_NUM);
		return WRITE_FRAME_FULL;
	}

	m_indexInfo.index		= index;
	m_indexInfo.position	= pos;
	m_indexInfo.time		= time;

	//���õײ�CRecord_file���ṩ��FIFO ���壬¼���̵߳��л᲻��ѭ��ȥˢ��
	 ret = m_pRec->Set_KeyFrameIdx((unsigned char* )&m_indexInfo,sizeof(INDEX_INFO),pos);

#if defined(__TDFH__)
	//FHƽ̨�ڲ���ϵͳ�㻺������̫�࣬�������쳣�ϵ�ʱ�ؼ�֡������û��д��
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
�������ܣ�д��һ֡���ݵľ���ʵ��
ʵ�ַ������Ƚ�����д�뻺������Ȼ���ٶ�֡ͳһд���ļ�
******************************************************************************/
unsigned long CPrimaryRecWriter::WriteFrame(const FRAME_INFO_EX & frame)
{
	//����֡��Ϣ�ṹ
	uint32_t write_size;	//д�����ݴ�С
	uint32_t rec_size;		//д����ɺ�¼���ļ��Ĵ�С
	int ret;				//����ֵ

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
		//д��һ֡����
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
			TRACE_DBG("Rec-file is full.\n");
			return WRITE_FRAME_FULL;
		}
	}

	rec_size =  m_filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO);
	/*д��һ֡���ݣ�����ܳ��ȳ����ļ��ܳ��ȣ��򷵻��ļ��Ѿ���*/
	if (m_MAX_REC_FILE_LEN >= rec_size)
	{
		write_size = sizeof(REC_FRAME_INFO);
		//д��֡��Ϣͷ
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

		//д��֡����
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

	//��Ƶ�ؼ�֡������Ҫд������
	if ((FRAME_TYPE_VIDEO == frame.frameType) && frame.keyFrame)
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

		//	TRACE("Write first frame, CH %02d start time ",m_pRec->get_channel());
		//	PrintfBit64(m_startTime,false);
			if(sizeof(REC_FILE_HEADER) != m_pRec->SetHeadData(CHANGED_HEAD,0,(unsigned char*)&m_fileHeader,sizeof(REC_FILE_HEADER)))
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

unsigned long CPrimaryRecWriter::OnWriteOneVideo(const FRAME_INFO_EX & frame)
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
		//if ((m_MAX_REC_FILE_LEN - m_MAX_REC_SUB_OFF_LEN) < (m_filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO) + m_bufInfo.dataLen))
		if ((m_MAX_REC_FILE_LEN - m_MAX_REC_SUB_OFF_LEN + m_headspace) < (m_filePos + sizeof(REC_FRAME_INFO) + frame.length + sizeof(REC_FRAME_INFO)))
		{
			printf("%s:%s:%d, file full m_filePos %d , file len=%d\n", __FUNCTION__, __FILE__, __LINE__, m_filePos, m_MAX_REC_FILE_LEN - m_MAX_REC_SUB_OFF_LEN + m_headspace);
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
