#include "ServerRTPPacket.h"

#ifdef _DEBUG
#define  new DEBUG_NEW
#endif

//#ifdef assert
//#undef assert
//#endif
//#define assert(a) printf("%s %s %d, assert false\n", __FILE__, __FUNCTION__, __LINE__)

//#include "Profile.h"

CRTPPacket::CRTPPacket()
{
	m_pRTPData = new unsigned char[RTP_PACKET_BUFFER_LEN];
	memset(m_pRTPData, 0, RTP_PACKET_BUFFER_LEN);
	if (NULL == m_pRTPData)
	{
		assert(false);
	}
	m_pInterleaved = (INTERLEAVED_INFO*)m_pRTPData;
	m_pRTPHeader = (RTP_HEADER*)(m_pRTPData + sizeof(INTERLEAVED_INFO));
	m_pNALUHeader = (NALU_HEADER*)(m_pRTPData + sizeof(INTERLEAVED_INFO) + sizeof(RTP_HEADER));
	m_pFUHeader = (FU_HEADER*)(m_pRTPData + sizeof(INTERLEAVED_INFO) + sizeof(RTP_HEADER) + sizeof(NALU_HEADER));
	m_bIsPause = false;
	m_bMark = false;
}

CRTPPacket::~CRTPPacket()
{
	if (NULL != m_pRTPData)
	{
		delete []m_pRTPData;
		m_pRTPData = NULL;
	}
}

void CRTPPacket::Clear()
{
	m_rtpDataLen  = 0;
	m_PadLenth = 0;
	m_bISAudio = false;
	m_bIsKeyFrame = false;
	m_ReferenceCount = 0;
	m_channel = 0;
	m_bIsPause = false;
	m_bMark = false;
}


int CRTPPacket::NewRTPAPacket(unsigned short channel, unsigned short sequenceNumber, unsigned long timeStamp, RTSP_DATE_TIME time, \
							  const unsigned char *pRTPData, unsigned long dataLen, unsigned char mi)
{
	if (dataLen > MAX_RTP_PAYLOAD_DATA_LEN)
	{
		assert(false);
		return -1;
	}
	m_Sec = time.seconds;
	m_MicroSec = time.microsecond;
	Clear();

	m_channel = channel;
	m_bISAudio = true;
	m_pRTPHeader->sequenceNumber = htons(sequenceNumber);
	m_pRTPHeader->timeStamp = htonl(timeStamp);

	m_rtpDataLen = (dataLen + sizeof(RTP_HEADER));
	
	CreateCommonInfo(RTP_PAYLOAD_TYPE_G711, false, STREAM_LIVE_AUDIO);

	memcpy(m_pRTPData + sizeof(RTP_HEADER) + sizeof(INTERLEAVED_INFO), pRTPData, dataLen);

	return 0;
}

//直接添加RTP头，不按照RFC3984/6184打包
int CRTPPacket::NewRTPPacket(unsigned short channel, unsigned short sequenceNumber, unsigned long timeStamp, RTSP_DATE_TIME time, \
				 const unsigned char *pRTPData, unsigned long dataLen, bool bIsKeyFrame, bool mark, unsigned char payloadType, bool bIsPause)
{
	if (dataLen > MAX_RTP_PAYLOAD_DATA_LEN)
	{
		assert(false);
		return -1;
	}
	m_Sec = time.seconds;
	m_MicroSec = time.microsecond;
	Clear();

	m_bIsKeyFrame = bIsKeyFrame;
	m_channel = channel;
	m_pRTPHeader->sequenceNumber = htons(sequenceNumber);
	m_pRTPHeader->timeStamp = htonl(timeStamp);

	m_rtpDataLen = dataLen + sizeof(RTP_HEADER);

	CreateCommonInfo(payloadType, mark, STREAM_LIVE_VIDEO);
	m_bMark = mark;

	memcpy(m_pRTPData + sizeof(RTP_HEADER) + sizeof(INTERLEAVED_INFO), pRTPData, dataLen);

	m_bIsPause = bIsPause;

	return 0;
}


//
int CRTPPacket::NewRTPPacket(unsigned short channel, unsigned short sequenceNumber, unsigned long timeStamp, RTSP_DATE_TIME time, \
							 const unsigned char *pRTPData, unsigned long dataLen, bool bIsKeyFrame)
{
	if (dataLen > MAX_RTP_PAYLOAD_DATA_LEN)
	{
		assert(false);
		return -1;
	}
	m_Sec = time.seconds;
	m_MicroSec = time.microsecond;
	Clear();

	m_bIsKeyFrame = bIsKeyFrame;
	m_channel = channel;
	m_pRTPHeader->sequenceNumber = htons(sequenceNumber);
	m_pRTPHeader->timeStamp = htonl(timeStamp);

	m_rtpDataLen = dataLen + sizeof(RTP_HEADER);

	CreateCommonInfo(RTP_PAYLOAD_TYPE_H264, true, STREAM_LIVE_VIDEO);

	memcpy(m_pRTPData + sizeof(RTP_HEADER) + sizeof(INTERLEAVED_INFO), pRTPData, dataLen);

	return 0;
}

///////////////////////////////////////////////没有分片的RTP数据//////////////////////////////////////
int CRTPPacket::NewRTPPacket(unsigned short channel, unsigned short sequenceNumber, unsigned long timeStamp, RTSP_DATE_TIME time, \
							 const NALU_HEADER *pNaluHeader, const unsigned char *pPayloadData, unsigned long dataLen, bool bIsKeyFrame)
{
	if (dataLen > MAX_RTP_PAYLOAD_DATA_LEN)
	{
		assert(false);
		return -1;
	}
	m_Sec = time.seconds;
	m_MicroSec = time.microsecond;
	Clear();
	
	m_bIsKeyFrame = bIsKeyFrame;
	m_channel = channel;
	m_pRTPHeader->sequenceNumber = htons(sequenceNumber);
	m_pRTPHeader->timeStamp = htonl(timeStamp);

	m_rtpDataLen = sizeof(char) + dataLen + sizeof(RTP_HEADER);

	*m_pNALUHeader = *pNaluHeader;
	CreateCommonInfo(RTP_PAYLOAD_TYPE_H264, true, STREAM_LIVE_VIDEO);

	memcpy(m_pRTPData + sizeof(RTP_HEADER) + sizeof(NALU_HEADER) + sizeof(INTERLEAVED_INFO), pPayloadData, dataLen);

	return 0;
}

///////////////////////////////////////////////分过片的RTP数据////////////////////////////////////////
int CRTPPacket::NewRTPPacket(unsigned short channel, unsigned short sequenceNumber, bool marker, unsigned long timeStamp, RTSP_DATE_TIME time, \
							 const NALU_HEADER *pFUIndicator, const FU_HEADER *pFUHeader, const unsigned char *pPayloadData, unsigned long dataLen, bool bIsKeyFrame)
{
	if (dataLen > MAX_RTP_PAYLOAD_DATA_LEN)
	{
		assert(false);
		return -1;
	}
	m_Sec = time.seconds;
	m_MicroSec = time.microsecond;
	Clear();

	m_bIsKeyFrame = bIsKeyFrame;
	m_channel = channel;
	m_pRTPHeader->sequenceNumber = htons(sequenceNumber);
	m_pRTPHeader->timeStamp = htonl(timeStamp);

	m_rtpDataLen = sizeof(char)*2 + dataLen + sizeof(RTP_HEADER);

	*m_pNALUHeader = *pFUIndicator;
	*m_pFUHeader = *pFUHeader;
	
	CreateCommonInfo(RTP_PAYLOAD_TYPE_H264, marker, STREAM_LIVE_VIDEO);

	memcpy(m_pRTPData + sizeof(RTP_HEADER) + sizeof(NALU_HEADER) + sizeof(FU_HEADER) + sizeof(INTERLEAVED_INFO), pPayloadData, dataLen);

	return 0;
}

void CRTPPacket::CreateCommonInfo(unsigned char PayloadType, bool bMarker, STREAM_TYPE StreamType)
{
// 	if (m_rtpDataLen % 4)
// 	{
// 		m_PadLenth = 4 - m_rtpDataLen % 4;
// 		m_rtpDataLen += m_PadLenth;
// 
// 		m_pRTPHeader->padding = 1;
// 		for (int i=1; i< m_PadLenth; i++)
// 		{
// 			m_pRTPData[m_rtpDataLen + sizeof(INTERLEAVED_INFO) - 1 - i] = 0;
// 		}
// 		m_pRTPData[m_rtpDataLen + sizeof(INTERLEAVED_INFO) - 1] = m_PadLenth;
// 	}
// 	else
	{
		m_pRTPHeader->padding = 0;
	}

	m_pInterleaved->mark = INTERLEAVED_DOLLAR_SIGN;
	m_pInterleaved->channel = (STREAM_LIVE_VIDEO == StreamType) ? 2 : 0;
	m_pInterleaved->datalength = htons(m_rtpDataLen);

	m_pRTPHeader->marker = (true == bMarker) ? 1 : 0;
	m_pRTPHeader->payloadtype = PayloadType;
	m_pRTPHeader->ssrc = htonl(0);//not set here
	m_pRTPHeader->version = 2;
	m_pRTPHeader->extension = 0;
	m_pRTPHeader->csrccount = 0;
}
