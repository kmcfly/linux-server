///////////////////////////////////////////////////////////////////////////
#ifndef _RTP_PACKET_H_
#define _RTP_PACKET_H_

#include "RTPHeader.h"
#include "DataBuffer.h"
#include "RTSP_Common.h"

const unsigned short RTP_PACKET_BUFFER_LEN = MAX_RTP_PAYLOAD_DATA_LEN + sizeof(RTP_HEADER) + sizeof(NALU_HEADER) + sizeof(FU_HEADER) + sizeof(INTERLEAVED_INFO) + 4;//4为padding

class CRTPPacket
{
public:
	CRTPPacket();
	~CRTPPacket();

	bool IsKeyFrame()	{return m_bIsKeyFrame;}
	inline unsigned short GetChannel() {return m_channel;}
	int NewRTPAPacket(unsigned short channel, unsigned short sequenceNumber, unsigned long timeStamp, RTSP_DATE_TIME time, \
		const unsigned char *pRTPData, unsigned long dataLen, unsigned char mi);
	int NewRTPPacket(unsigned short channel, unsigned short sequenceNumber, unsigned long timeStamp, RTSP_DATE_TIME time, \
		const unsigned char *pRTPData, unsigned long dataLen, bool bIsKeyFrame);
	int NewRTPPacket(unsigned short channel, unsigned short sequenceNumber, unsigned long timeStamp, RTSP_DATE_TIME time, \
		const NALU_HEADER *pNaluHeader, const unsigned char *pPayloadData, unsigned long dataLen, bool bIsKeyFrame);
	int NewRTPPacket(unsigned short channel, unsigned short sequenceNumber, bool marker, unsigned long timeStamp, RTSP_DATE_TIME time, \
		const NALU_HEADER *pFUIndicator, const FU_HEADER *pFUHeader, const unsigned char *pPayloadData, unsigned long dataLen, bool bIsKeyFrame);
	
	int NewRTPPacket(unsigned short channel, unsigned short sequenceNumber, unsigned long timeStamp, RTSP_DATE_TIME time, \
		const unsigned char *pRTPData, unsigned long dataLen, bool bIsKeyFrame, bool mark, unsigned char payloadType, bool bIsPause = false);

	RTP_HEADER *RecvRTPPacket(const char *pRTPData, unsigned long dataLen);
	unsigned short GetSeqNumber(){return ntohs(m_pRTPHeader->sequenceNumber);}
	unsigned long GetRTPTimestamp(){return ntohl(m_pRTPHeader->timeStamp);}
	bool IsEndPac(){return m_pRTPHeader->marker;}
	bool ISAudio(){return m_bISAudio;}

	void Inc() {m_Lock.Lock(); ++m_ReferenceCount; m_Lock.UnLock();}
	void Dec() {m_Lock.Lock(); --m_ReferenceCount; m_Lock.UnLock();}
	int	 GetCount() {int ret=0;m_Lock.Lock();ret=m_ReferenceCount;m_Lock.UnLock();return ret;}
	unsigned int GetLength() {return m_rtpDataLen;}
	unsigned long GetNTPSec() {return m_Sec;}
	unsigned long GetNTPMicroSec() {return m_MicroSec;}

	inline void SetSSRC(unsigned long ssrc)
	{
		m_pRTPHeader->ssrc = ssrc;
	}

	inline unsigned char *GetRTPDataTCP(unsigned long &dataLen)
	{
		dataLen = m_rtpDataLen + sizeof(INTERLEAVED_INFO);
		return m_pRTPData;
	}

	inline unsigned char *GetRTPData(unsigned long &dataLen)
	{
		dataLen = m_rtpDataLen;
		return m_pRTPData+sizeof(INTERLEAVED_INFO);
	}

	bool IsPause() {return m_bIsPause;}
	bool IsMark() {return m_bMark;}

private:
	inline void Clear();
	inline void CreateCommonInfo(unsigned char PayloadType, bool bMarker, STREAM_TYPE StreamType);

private:
	bool			m_bIsKeyFrame;
	unsigned short	m_channel;
	INTERLEAVED_INFO *m_pInterleaved;
	RTP_HEADER		*m_pRTPHeader;
	NALU_HEADER		*m_pNALUHeader;
	FU_HEADER		*m_pFUHeader;

	unsigned long	m_ssrc;

	unsigned char	*m_pRTPData;
	unsigned short	m_rtpDataLen;
	unsigned char	m_PadLenth;

	bool			m_bISAudio;
	CPUB_Lock		m_Lock;
	int				m_ReferenceCount;

//packet创建时的NTP时间
	unsigned long	m_Sec;
	unsigned long	m_MicroSec;

//国标添加
	bool			m_bIsPause;
	bool			m_bMark;
};
#endif /*_RTP_PACKET_H_*/
///////////////////////////////////////////////////////////////////////////
