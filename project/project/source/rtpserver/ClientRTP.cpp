#include "ClientRTP.h"

//#ifdef assert
//#undef assert
//#define assert(a) ((void)0)
//#endif

#include <stdlib.h>

#define Test_SaveData


CClientRTP::CClientRTP()
{
	m_pRTPRecvRam = new char[CLIENT_RTP_RECV_BUF_LEN + sizeof(RTSP_DATA_FRAME)];//多分配内存用于
	m_pRTPRecvBuf = m_pRTPRecvRam + sizeof(RTSP_DATA_FRAME);
	//m_pRTPRecvBuf = new char[CLIENT_RTP_RECV_BUF_LEN];
	m_pRTCPRecvBuf = new char[CLIENT_RTCP_RECV_BUF_LEN];
	memset(m_pRTPRecvBuf, 0, CLIENT_RTP_RECV_BUF_LEN);
	memset(m_pRTCPRecvBuf, 0, CLIENT_RTCP_RECV_BUF_LEN);
	m_RTPRecvBufLen = CLIENT_RTP_RECV_BUF_LEN;
	m_pRTPRecvPos = m_pRTPRecvBuf;

	m_bHasAudio = true;
	m_bHasVideo = true;
	m_LastPacketRemainLen = 0;

	m_bCurMarker = false;
	m_bIsSpsFound = false;
	m_bIsLastFrameButNeedWait = false;
	m_pFrameStartPos = NULL;
	m_pFrameEndPos = NULL;

	m_RR.rtporterSSRC = (unsigned long)rand();
	m_AudioRR.rtporterSSRC = (unsigned long)rand();
	m_RR.extendSeqNum.CyclesCount = 0;
	m_RTCPHeader.version = 2;
	m_LastJitter = 0;
	m_LastRecvSRTimeSec = 0;
	m_LastRecvSRTimeUSec = 0;

	memset(&m_lastSR, 0, sizeof(RTCP_SENDER_REPORTS));

	m_LastPacketTime = 0;
	m_LastButOnePacketTime= 0;
	m_pCallBack = NULL;
	m_pParam = NULL;
	m_TVTSessionID = 0;

	m_bIsKeyFrame = false;
	m_bIsFirstFrame = true;
	m_bIsFirstAudio = true;

	m_bRTCPSendControl = true;
}

CClientRTP::~CClientRTP()
{
	if (NULL != m_pRTPRecvRam)
	{
		delete m_pRTPRecvRam;
		m_pRTPRecvRam = NULL;
	}
	if (NULL != m_pRTCPRecvBuf)
	{
		delete m_pRTCPRecvBuf;
		m_pRTCPRecvBuf = NULL;
	}
}

int CClientRTP::Stop()
{
#ifdef Test_SaveData
 	int ret = fclose(m_file);
 	assert(0 == ret);
 
 	ret = fclose(m_fileAudio);
 	assert(0 == ret);
#endif
	return 0;
}


void CClientRTP::SetCallBack(RTP_CALLBACK pCallBack, void *pParam, int TVTSessionID)
{
	m_pCallBack = pCallBack;
	m_pParam = pParam;
	m_TVTSessionID = TVTSessionID;

	//for test
#ifdef Test_SaveData
  	char FileName[64] = {0};
  	sprintf(FileName, "d:\\RTP_TEST%d.dat", TVTSessionID);
  	m_file = fopen(FileName, "wb");
  	assert(NULL != m_file);
  
  	sprintf(FileName, "d:\\Audio_Test%d.dat", TVTSessionID);
  	m_fileAudio = fopen(FileName, "wb");
  	assert(NULL != m_fileAudio);
#endif
}

bool CClientRTP::SetUp(bool bHasVideo, bool bHasAudio, float frameRate, unsigned short width, unsigned short height)
{

	m_bHasVideo = bHasVideo;
	m_bHasAudio = bHasAudio;

	m_Width = width;
	m_Height = height;
	if (frameRate == 0)
	{
		frameRate = 25;
	}
	m_VideoTimeInc = 1000000 / frameRate;


	return true;
}

int CClientRTP::GetRTPData(const char *pData,const unsigned long len, unsigned char InterleavedType)
{
	if (NULL == pData)
	{
		return -1;
	}
	
	unsigned long CurDataLen = m_pRTPRecvPos - m_pRTPRecvBuf;
	if (m_RTPRecvBufLen - CurDataLen < len)
	{
		//追加缓存
		ReAllocBuf(CurDataLen);
	}
	unsigned long RemainLen;
	if (0 == InterleavedType % 2)
	{
		bool bFalse;
		RemainLen = HandleRawData(pData, len, InterleavedType, bFalse);
		if (bFalse)
		{
			return -1;
		}
	}
	else
	{
		RemainLen = HandleRTCP(pData, len, InterleavedType);
	}

	return len-RemainLen;
}

bool CClientRTP::ReAllocBuf(unsigned long CurDataLen)
{
	unsigned long tmpFrameLen;
	char *tmpPos = m_pRTPRecvPos;

	char *pTmpBuf = new char[m_RTPRecvBufLen + CLIENT_RTP_RECV_BUF_LEN + sizeof(RTSP_DATA_FRAME)];
	if (NULL == pTmpBuf)
	{
		assert(false);
		return false;
	}
	m_RTPRecvBufLen += CLIENT_RTP_RECV_BUF_LEN;
	memcpy(pTmpBuf + sizeof(RTSP_DATA_FRAME), m_pRTPRecvBuf, CurDataLen);
	m_pRTPRecvPos = pTmpBuf + CurDataLen + sizeof(RTSP_DATA_FRAME);
	delete m_pRTPRecvRam;
	m_pRTPRecvRam = pTmpBuf;
	m_pRTPRecvBuf = m_pRTPRecvRam + sizeof(RTSP_DATA_FRAME);

	assert(m_pFrameEndPos == NULL);
	m_pFrameStartPos = m_pRTPRecvBuf;

	return true;
}


unsigned long CClientRTP::HandleRawData(const char* pData,const unsigned long len, unsigned char InterleavedChannel, bool &IsFalse)
{
	IsFalse = false;
	RTP_HEADER *pRTPHeader = NULL;
	NALU_HEADER *pNALUHeader = NULL;
	FU_HEADER *pFUHeader = NULL;
	unsigned char NaluType = 0;
	char FU_A_NAL;
	unsigned short PacketLen = 0;
	unsigned long sendPosOffset = 0;
	unsigned long RealSendLen = 0;
	unsigned long RemainLen = len;
	unsigned long offset = 0;
	bool bIsDistribute = false;

	while(RemainLen > 0)
	{
		if(RemainLen <= 12+2)//下次接收到更多的数据再处理
		{
			IsFalse = true;
			break;
		}

		pRTPHeader = (RTP_HEADER*)(pData+offset);
		pNALUHeader = (NALU_HEADER*)(pData+offset+12);
		pFUHeader = (FU_HEADER*)(pData+offset+12+1);
		
//     		printf("type: %u    Sequence: %u    TimeStamp: %lu     SSRC: %lu     Marker :%u    ", \
//     			pRTPHeader->payloadtype, htons(pRTPHeader->sequenceNumber), htonl(pRTPHeader->timeStamp), htonl(pRTPHeader->ssrc), pRTPHeader->marker);
//     		printf("NALU: type:%u  nri:%u  f:%u   ", pNALUHeader->type, pNALUHeader->nri, pNALUHeader->forbidden_zero_bit);
//     		printf("FU: type:%u  start:%u  end:%u  reserve:%u \n", pFUHeader->type, pFUHeader->start_bit, pFUHeader->end_bit, pFUHeader->reserve_bit);

		RealSendLen = RemainLen;
		offset += 12;
		RealSendLen -= 12;

		if (pRTPHeader->payloadtype < RTP_PAYLOAD_TYPE_H264_ || pRTPHeader->payloadtype > RTP_PAYLOAD_TYPE_H264_MAX)
		{
			//处理音频
 			//if (1 != fwrite(pData + offset, RealSendLen, 1, m_fileAudio))
 			//{
 			//	assert(false);
 			//}

			return 0;
		}

		//RTP信息保存，供RTCP使用
		{
			m_Lock.Lock();

			m_LastButOneRTPHeader = m_LastRTPHeader;
			m_LastButOnePacketTime = m_LastPacketTime;
			unsigned long Sec = RTSPGetCurrTime().seconds;
			unsigned long uSec = RTSPGetCurrTime().microsecond ;//最大为10^6   模拟器下的不准  只精确到毫秒  精度达不到
			uSec =  4294 * uSec ;//转换为NTP_frac形式 4294 = 0x100000000 / 1000000
			m_LastPacketTime = (Sec & 0x0000ffff)<<16 | uSec;
			m_LastRTPHeader.ssrc = ntohl(pRTPHeader->ssrc);
			m_LastRTPHeader.timeStamp = ntohl(pRTPHeader->timeStamp);
			m_LastRTPHeader.sequenceNumber = ntohs(pRTPHeader->sequenceNumber);
			if (m_LastRTPHeader.sequenceNumber == 0)
			{
				m_RR.extendSeqNum.CyclesCount++;
			}
			m_Lock.UnLock();
		}

		if (0 != pRTPHeader->padding)//去除padding
		{
			unsigned char paddingNum = pData[len-1];
			if (paddingNum >= 4)
			{
				IsFalse = true;
				break;
			}
			else
			{
				RealSendLen -= paddingNum;
			}
		}

		if (FRAGMENTATION_UNIT_A == pNALUHeader->type)//FU_A  non_interleaved不支持FU_B
		{
			offset += 2;
			RealSendLen -= 2;
			if (pFUHeader->start_bit)
			{
				if(m_pFrameStartPos != NULL || m_pFrameEndPos != NULL)
				{
					IsFalse = true;
					break;
				}
				m_pFrameStartPos = m_pRTPRecvPos;

				memcpy(m_pRTPRecvPos, RTP_START_CODE, 4);
				m_pRTPRecvPos += 4;
				FU_A_NAL = (pNALUHeader->nri)<<5 | (pNALUHeader->forbidden_zero_bit)<<6  | pFUHeader->type;
				memcpy(m_pRTPRecvPos, &FU_A_NAL, 1);
				m_pRTPRecvPos += 1;
			}
			else if (pFUHeader->end_bit || pRTPHeader->marker)
			{	
				if(m_pFrameStartPos == NULL || m_pFrameEndPos != NULL)
				{
					IsFalse = true;
					break;
				}
				m_pFrameEndPos = m_pRTPRecvPos + RealSendLen;
				bIsDistribute = true;
			}
			else//中间的包，不处理
			{

			}
		}
		else if(1 <= pNALUHeader->type && 23 >= pNALUHeader->type)
		{
			///////////////////////////////for test
 			if (SEI_RBSP == pNALUHeader->type)
 			{
				RemainLen = 0;
				m_pRTPRecvPos = m_pRTPRecvBuf;
 				continue;
 			}
			////////////////////////////////
			if (SEQ_PARAMETER_SET_RBSP == pNALUHeader->type)
			{
				m_bIsKeyFrame = true;
			}
			if(m_pFrameStartPos != NULL || m_pFrameEndPos != NULL)
			{
				IsFalse = true;
				break;
			}
			m_pFrameStartPos = m_pRTPRecvPos;

			memcpy(m_pRTPRecvPos, RTP_START_CODE, 4);
			m_pRTPRecvPos += 4;

			if(m_pFrameStartPos == NULL || m_pFrameEndPos != NULL)
			{
				IsFalse = true;
				break;
			}
			m_pFrameEndPos = m_pRTPRecvPos + RealSendLen;
			bIsDistribute = true;
				
		}
		else if(SINGLE_TIME_AGGREGATION_PACKET_A == pNALUHeader->type)// non-interleaved 只有STAP-A
		{
			//待完善
			//unsigned short NALUnSize;
			//memcpy(m_pRTPRecvPos, RTP_START_CODE, 4);
			//m_pRTPRecvPos += 4;
			//NALUnSize = ntohs((unsigned short)(*(pData+offset+1)));
			IsFalse = true;
			break;
		}
		else
		{
			IsFalse = true;
			break;
		}
		

		memcpy(m_pRTPRecvPos, pData+offset, RealSendLen);
		m_pRTPRecvPos += RealSendLen;

		offset += RealSendLen;
		if (len < offset)
		{
			IsFalse = true;
			break;
		}
		RemainLen = 0;

		if (bIsDistribute)
		{
			bIsDistribute = false;
			if (pRTPHeader->payloadtype >= RTP_PAYLOAD_TYPE_H264_ && pRTPHeader->payloadtype <= RTP_PAYLOAD_TYPE_H264_MAX)
			{
				DistributeFrame(STREAM_LIVE_VIDEO);
			}
			else
			{
				DistributeFrame(STREAM_LIVE_AUDIO);
			}
		}
	}//end of while

	return RemainLen;
}

bool CClientRTP::DistributeFrame(STREAM_TYPE streamType)
{
	if (m_pFrameStartPos == NULL || m_pFrameEndPos == NULL || m_pFrameEndPos <= m_pFrameStartPos)
	{
		assert(false);
		return false;
	}

	RTSP_DATA_FRAME *pDataFrame = (RTSP_DATA_FRAME *)(m_pRTPRecvRam);
	pDataFrame->pData = (unsigned char*)pDataFrame;
	pDataFrame->frame.length = m_pFrameEndPos - m_pFrameStartPos;
	pDataFrame->length = pDataFrame->frame.length + sizeof(RTSP_DATA_FRAME);
	pDataFrame->streamID = m_TVTSessionID;
	if(STREAM_LIVE_VIDEO == streamType)
	{
		pDataFrame->frame.width = m_Width;
		pDataFrame->frame.height = m_Height;
		pDataFrame->frame.bKeyFrame = m_bIsKeyFrame;
		if (true == m_bIsKeyFrame)
		{
			m_bIsKeyFrame = false;
		}
		pDataFrame->frame.encodeType = RTP_PAYLOAD_TYPE_H264;
		pDataFrame->frame.streamType = RTSP_STREAM_TYPE_VIDEO_1;
		if (m_bIsFirstFrame)
		{
			m_bIsFirstFrame = false;
			m_LastVideoTime = RTSPGetCurrTime();
			pDataFrame->frame.time = m_LastVideoTime;
		}
		else
		{
			m_LastVideoTime += m_VideoTimeInc;
			pDataFrame->frame.time = m_LastVideoTime;
		}
#ifdef Test_SaveData
  		if (1 != fwrite(pDataFrame->pData+sizeof(RTSP_DATA_FRAME), pDataFrame->frame.length, 1, m_file))
  		{
  			assert(false);
  			return false;
  		}
#endif
	}
	else
	{
		pDataFrame->frame.encodeType = RTP_PAYLOAD_TYPE_G711;
		pDataFrame->frame.streamType = RTSP_STREAM_TYPE_AUDIO;
		//if (m_bIsFirstAudio)
		{
			m_bIsFirstAudio = false;
			m_LastAudioTime = RTSPGetCurrTime();
			pDataFrame->frame.time = m_LastAudioTime;
		}
		//else
		//{
		//	pDataFrame.frame.time = m_LastAudioTime +
		//	m_LastAudioTime
		//}
#ifdef Test_SaveData
  		if (1 != fwrite(m_pFrameStartPos, m_pFrameEndPos - m_pFrameStartPos, 1, m_fileAudio))
  		{
  			assert(false);
  			return false;
  		}
#endif
		
	}
	pDataFrame->frame.localTime = pDataFrame->frame.time.LocalTime();

	//m_pCallBack(pDataFrame, m_TVTSessionID, m_pParam);

	m_pFrameStartPos = NULL;
	m_pFrameEndPos = NULL;
	m_pRTPRecvPos = m_pRTPRecvBuf;


	return true;
}

int CClientRTP::SendRTCPData(char *pData, unsigned long &len)
{
	int ret = 0;
	unsigned long tmpLen = 0;
	INTERLEAVED_INFO *Interleaved = (INTERLEAVED_INFO *)pData;
	Interleaved->mark = INTERLEAVED_DOLLAR_SIGN;

	m_Lock.Lock();

	if (m_bHasAudio && m_bHasVideo)
	{
		if (m_bRTCPSendControl)
		{
			m_bRTCPSendControl ^= 1;
			Interleaved->channel = 0x01;
			CreateVideoRR(pData+sizeof(INTERLEAVED_INFO), tmpLen);
			len = tmpLen;
			CreateSDES(pData+len+sizeof(INTERLEAVED_INFO), tmpLen, m_RR.rtporterSSRC);
			len += tmpLen;
			Interleaved->datalength = htons(len);
			len += 4;
		}
		else
		{
			m_bRTCPSendControl ^= 1;
			Interleaved->channel = 0x03;
			CreateAudioRR(pData+sizeof(INTERLEAVED_INFO), tmpLen);
			len = tmpLen;
			CreateSDES(pData+len+sizeof(INTERLEAVED_INFO), tmpLen, m_AudioRR.rtporterSSRC);
			len += tmpLen;
			Interleaved->datalength = htons(len);
			len += 4;
		}

	}
	else if (m_bHasAudio)
	{
		Interleaved->channel = 0x01;
		CreateAudioRR(pData + sizeof(INTERLEAVED_INFO), tmpLen);
		len = tmpLen;
		Interleaved->datalength = htons(len);
		len += 4;
	}
	else if (m_bHasVideo)
	{
		Interleaved->channel = 0x01;
		CreateVideoRR(pData + sizeof(INTERLEAVED_INFO), tmpLen);
		len = tmpLen;
		Interleaved->datalength = htons(len);
		len += 4;
	}
	else
	{
		assert(false);
		pData = NULL;
		len = 0;
		ret = -1;
		return ret;
	}
	m_Lock.UnLock();

	return ret;
}

void CClientRTP::CreateAudioRR(char *pBuf, unsigned long &len)
{
	m_AudioRTCPHeader.version = 0x02;
	m_AudioRTCPHeader.padding = 0;
	m_AudioRTCPHeader.itemCount = 0;
	m_AudioRTCPHeader.packetType = RTCP_PACKET_RR;
	m_AudioRTCPHeader.length = htons(1);

	unsigned long ssrc = htonl(m_AudioRR.rtporterSSRC);
		

	len = sizeof(RTCP_HEADER) + sizeof(unsigned long);

	memcpy(pBuf, &m_AudioRTCPHeader, sizeof(RTCP_HEADER));
	memcpy(pBuf+sizeof(RTCP_HEADER), &ssrc, sizeof(unsigned long));
}


void CClientRTP::CreateVideoRR(char *pBuf, unsigned long &len)
{
	RTCP_RECEIVER_REPORTS *pTmpRR;

	m_RTCPHeader.padding = 0;
	m_RTCPHeader.itemCount = 1;
	m_RTCPHeader.packetType = RTCP_PACKET_RR;
	m_RTCPHeader.length = htons(7);

	m_RR.rtporteeSSRC = m_LastRTPHeader.ssrc;
	m_RR.fractionLost = 0;
	m_RR.cumulativeLost = 0;
	m_RR.extendSeqNum.HighestSequenceNum = m_LastRTPHeader.sequenceNumber;

	m_RR.interarrivalJitter = m_LastJitter + \
		(abs((long int)((m_LastRTPHeader.timeStamp - m_LastPacketTime) - (m_LastButOneRTPHeader.timeStamp - m_LastButOnePacketTime))) - m_LastJitter)>>4;

		if (0 == m_RR.interarrivalJitter)
	{
		m_RR.interarrivalJitter = 192;
	}
	m_LastJitter = m_RR.interarrivalJitter;
	m_RR.lsrNTPH = m_lastSR.rtpTimestamp>>16;
	m_RR.lsrNTPL = m_lastSR.rtpTimestamp;
	m_RR.delaySinceLSR = (m_LastRecvSRTimeSec == 0) ? 0 : \
		((RTSPGetCurrTime().seconds - m_LastRecvSRTimeSec) + (RTSPGetCurrTime().microsecond - m_LastRecvSRTimeUSec)<<16) * 65535;

	pTmpRR = (RTCP_RECEIVER_REPORTS *)(pBuf + sizeof(RTCP_HEADER));
	pTmpRR->rtporterSSRC = htonl(m_RR.rtporterSSRC);
	pTmpRR->rtporteeSSRC = htonl(m_RR.rtporteeSSRC);
	pTmpRR->fractionLost = m_RR.fractionLost;
	pTmpRR->cumulativeLost = (m_RR.cumulativeLost & 0x0000ff)<<16 | (m_RR.cumulativeLost & 0xffff00)>>8;//24位转化为网络字节序
	pTmpRR->extendSeqNum.CyclesCount = htons(m_RR.extendSeqNum.CyclesCount);
	pTmpRR->extendSeqNum.HighestSequenceNum = htons(m_RR.extendSeqNum.HighestSequenceNum);
	pTmpRR->interarrivalJitter = htonl(m_RR.interarrivalJitter);
	pTmpRR->lsrNTPH = htons(m_RR.lsrNTPH);
	pTmpRR->lsrNTPL = htons(m_RR.lsrNTPL);
	pTmpRR->delaySinceLSR = htonl(m_RR.delaySinceLSR);

	len = sizeof(RTCP_HEADER) + sizeof(RTCP_RECEIVER_REPORTS);

	memcpy(pBuf, &m_RTCPHeader, sizeof(RTCP_HEADER));
	//memcpy(pData+sizeof(INTERLEAVED_INFO)+sizeof(RTCP_HEADER), &m_RR, sizeof(RTCP_RECEIVER_REPORTS));
}


void CClientRTP::CreateSDES(char *pBuf, unsigned long &len, unsigned long csrc)
{
	char *pPos = pBuf;

	RTCP_HEADER *pRTCPHeader = (RTCP_HEADER*)pPos;
	pRTCPHeader->version = 0x02;
	pRTCPHeader->packetType = RTCP_PACKET_SDES;
	pRTCPHeader->padding = 0;
	pRTCPHeader->itemCount = 1;
	unsigned long csrc1 = htonl(csrc);

	pPos += sizeof(RTCP_HEADER);
	memcpy(pPos, &csrc1, sizeof(unsigned long));
	pPos += sizeof(unsigned long);
	*pPos = SDES_ITEM_CNAME;
	pPos += 1;
	unsigned char SDESLen = 255;//HOST_NAME_MAX
	char SDESText[255] = {0};
	if (-1 == gethostname(SDESText, SDESLen))
	{
		assert(false);
		len = 0;
		return;
	}
	SDESLen = strlen(SDESText);
	*pPos = SDESLen;
	pPos += 1;
	memcpy(pPos, SDESText, SDESLen);
	pPos += SDESLen;;
	unsigned char alignLen = (4 - (SDESLen + 2 + sizeof(RTCP_HEADER) + sizeof(unsigned long)) % 4);
	for(int i=0; i<alignLen ; ++i)
	{
		*pPos = 0;
		pPos++;
	}

	pRTCPHeader->length = htons((sizeof(unsigned long) + 2 + SDESLen + alignLen) / 4 );

	len = pPos - pBuf;
}

unsigned long CClientRTP::HandleRTCP(const char *pData, const unsigned long len, unsigned char InterleavedChannel)
{

	return 0;
}

void CClientRTP::HandleConnectionBreak()
{
	if (NULL != m_pCallBack)
	{
		m_pCallBack(NULL, m_TVTSessionID, m_pParam);//告诉上层，连接断开了
	}
}