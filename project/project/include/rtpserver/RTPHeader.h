/////////////////////////////////////////////////////////////////////////////
#ifndef _RTP_HEADER_H_
#define _RTP_HEADER_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>

// #include "TVT_PubCom.h"			//3.0������
// #include "SWL_TypeDefine.h"		//3.0������
#include "RTSPHeader.h"
#include "RTPSock.h"

#include "RTPTimestamp.h"

//const unsigned short	MAX_RTP_PAYLOAD_DATA_LEN = 1024*8 - 24;//����Ч��  
const unsigned short	MAX_RTP_PAYLOAD_DATA_LEN = 1460;// 1500 - 12/*rtp header*/ - 8/*udp header*/ - 20/*ip header*/

const unsigned long		RTP_VERSION			= 2;
//const unsigned short	RTP_SERVER_PORT		= 6000;
const unsigned long		START_CODE_LEN		= 4;
const unsigned short	RTP_PAYLOAD_TYPE_H264_MAX = 99;
const unsigned short	RTP_PAYLOAD_TYPE_H264 = 98;
const unsigned short	RTP_PAYLOAD_TYPE_H264_ = 96;
const unsigned short	RTP_PAYLOAD_TYPE_G711 = 8;
const unsigned long		MAX_CH_NUM	= 32;
const unsigned long		MAX_STREAM_NUM = MAX_CH_NUM * 2;//�ٶ�һ��ͨ������һ���ֳ�һ���ط�
const unsigned short	PACKET_POOL_SIZE = 500;//D1�£�1000������ԼΪ4S����
const unsigned short	PACKET_POOL_RELLOC_SIZE = 100;
const unsigned short	RTP_SESSION_VIDEO_SIZE = 400;
const unsigned short	RTP_SESSION_AUDIO_SIZE = 200;
const unsigned long		FRAME_DATE_LIMIT_SIZE = 1024*1024*2;
const unsigned long		RTP_VIDEO_CAPTURE_FREQUENCY = 90000;
const unsigned long		RTP_AUDIO_CAPTURE_FREQUENCY = 8000;
const unsigned long		NTP_OFFSET_FROM_1900_TO_1970 = 2208988800;
const unsigned short	MAX_RTSP_MSG_LEN = 65535;
const unsigned short	MAX_RTSP_MEDIA_COUNT = 2;

typedef enum _stream_type_
{
	STREAM_LIVE_VIDEO		= 0,
	STREAM_LIVE_AUDIO		= 1,
	STREAM_RECORD_VIDEO		= 2,
	STREAM_RECORD_AUDIO		= 3,
	STREAM_PLAY_BACK		= 4,
	STREAM_NONE				= 5,
}STREAM_TYPE;

typedef enum _nal_unit_type_
{
	NAL_UNIT_TYPE_ZERO	= 0,					//δ�涨
	SLICE_LAYER_WITHOUT_PARTITIONING_RBSP	= 1,//��IDRͼ���в��������ݻ��ֵ�Ƭ��
	SLICE_DATA_PARTION_A_LAYER_RBSP			= 2,//��IDRͼ����A�����ݻ���Ƭ��
	SLICE_DATA_PARTION_B_LAYER_RBSP			= 3,//��IDRͼ����B�����ݻ���Ƭ��
	SLICE_DATA_PARTION_C_LAYER_RBSP			= 4,//��IDRͼ����C�����ݻ���Ƭ��
	SLICE_LAYER_WITH_PARTITIONING_RBSP		= 5,//IDRͼ���Ƭ��
	SEI_RBSP								= 6,//������ǿ��Ϣ (SEI)
	SEQ_PARAMETER_SET_RBSP					= 7,//���в�����
	PIC_PARAMETER_SET_RBSP					= 8,//ͼ�������
	ACCESS_UNIT_DELIMITER_RBSP				= 9,//�ָ��
	END_OF_SEQ_RBSP							= 10,//���н�����
	END_OF_STREAM_RBSP						= 11,//��������
	FILLER_DATA_RBSP						= 12,//�������
	//13-23����  24-31δ����
	SINGLE_TIME_AGGREGATION_PACKET_A		= 24,  //��ʱ��ۺϰ�
	SINGLE_TIME_AGGREGATION_PACKET_B		= 25,  //��ʱ��ۺϰ�
	MULTI_TIME_AGGREGATION_PACKET_A			= 26,  //��ʱ��ۺϰ�
	MULTI_TIME_AGGREGATION_PACKET_B			= 27,  //��ʱ��ۺϰ�
	FRAGMENTATION_UNIT_A					= 28,	//��Ƭ�İ�
	FRAGMENTATION_UNIT_B					= 29,	//��Ƭ�İ�
}NAL_UNIT_TYPE;

typedef struct _nalu_header_
{
	unsigned char type:5;
	unsigned char nri:2;
	unsigned char forbidden_zero_bit:1;
}NALU_HEADER;

typedef struct _fu_indicator_
{
	unsigned char type:5;
	unsigned char nri:2;
	unsigned char forbidden_zero_bit:1;
}FU_INDICATOR;

typedef struct _fragmentation_unit_header_
{
	unsigned char type:5;			//��ԭʼ������nalu�е�typeһ��
	unsigned char reserve_bit:1;	//����λ��������0��
	unsigned char end_bit:1;		//ָʾ�ǲ�����һ֡�����һ����Ƭ
	unsigned char start_bit:1;		//ָʾ�ǲ�����һ֡�ĵ�һ����Ƭ

}FU_HEADER;

typedef struct _rtp_header_
{
#ifdef RTP_BIG_ENDIAN
	unsigned char version:2;
	unsigned char padding:1;
	unsigned char extension:1;
	unsigned char csrccount:4;

	unsigned char marker:1;
	unsigned char payloadtype:7;
#else // little endian
	unsigned char csrccount:4;
	unsigned char extension:1;
	unsigned char padding:1;
	unsigned char version:2;

	unsigned char payloadtype:7;
	unsigned char marker:1;
#endif // RTP_BIG_ENDIAN

	unsigned short sequenceNumber;
	unsigned long timeStamp;
	unsigned long ssrc;
}RTP_HEADER;

///////////////////////////////////////RTCP���/////////////////////////////////////////////
typedef struct _rtcp_header_
{
#ifdef RTP_BIG_ENDIAN
	unsigned char version:2;
	unsigned char padding:1;
	unsigned char itemCount:5;

	unsigned char packetType;
#else // little endian
	unsigned char itemCount:5;
	unsigned char padding:1;
	unsigned char version:2;

	unsigned char packetType;
#endif // RTP_BIG_ENDIAN
	unsigned short length;
}RTCP_HEADER;

typedef enum _rtcp_packet_type_
{
	RTCP_PACKET_SR		= 200,
	RTCP_PACKET_RR		= 201,
	RTCP_PACKET_SDES	= 202,
	RTCP_PACKET_BYE		= 203,
	RTCP_PACKET_APP		= 204,
}RTCP_PACKET_TYPE;



//RTCP�����߱���

typedef struct _extended_seq_num_
{
	unsigned short		CyclesCount;//�����ֽ�˳��  ���ģʽ  CyclesCount�ڸ�λ
	unsigned short		HighestSequenceNum;
}EXTENDED_SEQ_NUM;

typedef struct _rtcp_receiver_reports_
{
	unsigned long	rtporterSSRC;
	unsigned long	rtporteeSSRC;

	long			fractionLost:8;
	long			cumulativeLost:24;

	EXTENDED_SEQ_NUM extendSeqNum;

	unsigned long	interarrivalJitter;
	
	union _lsrNTPTime_
	{
		unsigned long	lsrNTP;			//��һ��SR��ntp���ݵ��м�32λ��ÿ��RR�е�lsr�͸�������ͬ�ͱ�ʾ��ЩRR�����SR��Ӧ��
		struct _lsrTime_
		{
			unsigned short lsrNTPL;
			unsigned short lsrNTPH;
		}lsrTime;
	}lsrNTPTime;

	unsigned long	delaySinceLSR;
}RTCP_RECEIVER_REPORTS;

typedef struct _rtcp_sender_report_data_
{
	unsigned long	reporterSSRC;
	unsigned long	ntpTimeH;				//���NTPʱ����벿�֣�UTCʱ��+2,208,988,800
	unsigned long	ntpTimeL;				//��������µĲ��֣���λ����
	unsigned long	rtpTimestamp;
	unsigned long	senderPacketCount;
	unsigned long	senderOctetCount;
}RTCP_SR_DATA;

typedef struct _rtcp_sender_reports_
{
	RTCP_HEADER		rtcpHeader;
	unsigned long	reporterSSRC;
	unsigned long	ntpTimeH;				//���NTPʱ����벿�֣�UTCʱ��+2,208,988,800
	unsigned long	ntpTimeL;				//��������µĲ��֣���λ����
	unsigned long	rtpTimestamp;
	unsigned long	senderPacketCount;
	unsigned long	senderOctetCount;
}RTCP_SENDER_REPORTS;


typedef enum _sdes_item_type_
{
	SDES_ITEM_END	= 0,
	SDES_ITEM_CNAME	= 1,			//canonical name ��׼���ƣ��Ǹ��ȶ������ID����������ȷ��һ������Ĳ�����
									//��һ���Ự�ж����ʱ���ͻ���ʹ�����ID��ȷ����������ͬһ������Դ��
									//����˵��һ����Ƶ����Ƶ�����ն˸�����������֪���������Ƶ��Ҫͬ����
	SDES_ITEM_NAME	= 2,
	SDES_ITEM_EMAIL	= 3,
	SDES_ITEM_PHONE	= 4,
	SDES_ITEM_LOC	= 5,
	SDES_ITEM_TOOL	= 6,
	SDES_ITEM_NOTE	= 7,
	SDES_ITEM_PRIV	= 8,
}SDES_ITEM_TYPE;

//����ʱ����㷽ʽ RTT = (recvRRTime - sendTime - dlsr) 
typedef struct _sended_sreports_info_
{
	ULONGLONG		sendTime;		//����ʱ��
	ULONGLONG		recvRRTime;		//�յ�������ͱ����Ӧ�ĵ�һ��Ӧ���ʱ��
	union _lsrNTPTime_
	{
		unsigned long	lsrNTP;			//���SR��ntp���ݵ��м�32λ��ÿ��RR�е�lsr�͸�������ͬ�ͱ�ʾ��ЩRR�����SR��Ӧ��
		struct _lsrTime_
		{
			unsigned short lsrNTPH;
			unsigned short lsrNTPL;
		}lsrTime;
	}lsrNTPTime;
	unsigned long	dlsr;			//RR�б�ǵ��ӳ�ʱ�䡣�������յ�SR�����ʱһ��ʱ���ŷ���RR�������ߡ� ��λ΢��
}SENDED_SREPORTS_INFO;

#define lsrNTP		lsrNTPTime.lsrNTP
#define lsrNTPH		lsrNTPTime.lsrTime.lsrNTPH
#define lsrNTPL		lsrNTPTime.lsrTime.lsrNTPL

typedef struct _rtcp_sr_info_
{
	unsigned long		timestamp;
	ULONGLONG			ctsUTCTime;				//�շ��ͳ�ȥ��RTP���ݱ���UTCʱ���
	ULONGLONG			nextSReportsTime;		//�·���SR��ʱ��
	unsigned long		srInterval;				//����SR�ļ��
	unsigned long		senderPacketCount;
	unsigned long		senderOctetCount;
	unsigned long		senderReportCount;		//�Ѿ����ͳ�ȥ��SR�������
	RTCP_SENDER_REPORTS	rtcpSR;					//rtcp �����߱��棬ÿ����һ�����ݰ��͸���һ�θ�����

	std::list<SENDED_SREPORTS_INFO *> srportsInfoList;	//����Ѿ����͵������е�SR��Ϣ��
														//ÿ����һ���������������һ�
														//�յ���RRӦ���е�NTPʱ���ɾ����ʱ��ǰ�������SR��Ϣ
}RTCP_SR_INFO;

typedef struct _rtcp_report_info_
{	
	CPUB_Lock		lock;
	unsigned long	ssrc;
//	CRTPTime		*pRTPTime;
	ULONGLONG		reportTime;						//���͸��Զ˱����ʱ��
	unsigned long	totalPacCount;					//��Ϊ���Ͷˣ��Ƿ��͵����ݰ����������ն��ǽ��յ������ݰ�����
	unsigned long	totalOctetCount;				//���ͻ��߽��յ����ֽ���
	unsigned long	pacCount;						//���Ͷˣ���ʾ���ϴη���SR���͵����ݰ����������ն˱�ʾ�����ϴν��յ�SR����ܵ�RTP�ĸ���
	unsigned long	lossFrac:8;
	unsigned long	totalPacLost:24;
	unsigned long	jitter;
	unsigned long	lastSTS;
	CRTPTimestamp	rtpTS;
	EXTENDED_SEQ_NUM extendSeqNum;
}RTCP_REPORT_INFO;


typedef struct _rtcp_sdes_info_
{
	struct _sdes_
	{
		unsigned char type;
		unsigned char length;
		unsigned char *pData;
	}SDES[9];

	unsigned char *pRTCPSDESData;
	unsigned long sdesDataLen;
}RTCP_SDES_INFO;

typedef struct _rtcp_bye_info_
{
	RTCP_HEADER rtcpHeader;
	unsigned long ssrc;
	unsigned char optionalLen;
	char *		optionalReason;
}RTCP_BYE_INFO;

typedef struct _rtp_stream_info_
{
	unsigned long		ssrc;					//����Դͬ����ʶ
	char				*pStrmName;				//�������ƣ�H264��
	unsigned long		frequence;				//����Ƶ��
	unsigned long		frameRate;				//����֡�ʣ���Ƶ����û֡��
	unsigned long		timestampUnit;			//ÿ����һ��RTP���ݰ�ʱ��������Ӹ���
	unsigned long		payloadType;
	unsigned long		rtt;					//��ǰ������ʱ��  RTT(i+1) = 0.9RTT(i) + 0.1RTT(new)
	float				packetLoseRate;			//���ݱ���ʧ��		R(i+1) = (1 - 0.3)R(i) + 0.3P(FL)
	unsigned long		rtpChannel;				//rtp������RTSP������ռ�õ�ͨ����
	unsigned long		rtcpChannel;			//rtcp������RTSP������ռ�õ�ͨ����
	CRTPSock			rtpSock;
	CRTPSock			rtcpSock;
}RTP_STREAM_INFO;

typedef struct _rtp_session_info_
{
	unsigned long		sessionID;				//�ỰID����RTSP�Ựʱ������ÿ��ID��Ӧһ��RTSP�Ự��һ��RTP/RTCP����
	SWL_socket_t		rtspSockfd;				//rtsp���׽���
	STREAM_TYPE			streamType;				//�����ֳ����طš���¼������
	unsigned long		channel;				//�������Ƶͨ��
	bool				bHasAudio;				//������ͨ���Ƿ�����Ƶ����
	RTSP_SESSION_STATUS	sessionStatus;			//��ǰ�Ự��״̬
	struct sockaddr_in	rtspSocketAddr;			//rtsp��ַ�Ͷ˿ڣ���RTP��˵ֻ��IP��ַ����
	char				*pServerDes;			//��������������Ϣ
	char				*pFunction;				//֧�ֵĹ���
	char				*pContentType;
	unsigned long		contentLength;
	bool				bISTcpSession;			//�Ƿ�ʹ��TCP����RTP����
	bool				bISUnicast;				//�Ƿ��ǵ���

	RTP_STREAM_INFO		audioStreamInfo;		//��Ƶ����Ϣ
	RTP_STREAM_INFO		videoStreamInfo;		//��Ƶ����Ϣ

	RTCP_SR_INFO		rtcpASRInfo;			//��Ƶ�ķ��ͱ�����Ϣ
	RTCP_SR_INFO		rtcpVSRInfo;			//��Ƶ�ķ��ͱ�����Ϣ
}RTP_SESSION_INFO;

typedef struct _stream_data_
{
	unsigned long	ssrc;
	unsigned long	channel;
	unsigned long	sessionID;
	STREAM_TYPE		streamType;
	unsigned char	*pFrameData;
	unsigned long	frameDataLen;
	ULONGLONG		captureTime;
}STREAM_DATA;

#endif /*_RTP_HEADER_H_*/
