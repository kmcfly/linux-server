/////////////////////////////////////////////////////////////////////////////
#ifndef _RTP_HEADER_H_
#define _RTP_HEADER_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>

// #include "TVT_PubCom.h"			//3.0不包含
// #include "SWL_TypeDefine.h"		//3.0不包含
#include "RTSPHeader.h"
#include "RTPSock.h"

#include "RTPTimestamp.h"

//const unsigned short	MAX_RTP_PAYLOAD_DATA_LEN = 1024*8 - 24;//增加效率  
const unsigned short	MAX_RTP_PAYLOAD_DATA_LEN = 1460;// 1500 - 12/*rtp header*/ - 8/*udp header*/ - 20/*ip header*/

const unsigned long		RTP_VERSION			= 2;
//const unsigned short	RTP_SERVER_PORT		= 6000;
const unsigned long		START_CODE_LEN		= 4;
const unsigned short	RTP_PAYLOAD_TYPE_H264_MAX = 99;
const unsigned short	RTP_PAYLOAD_TYPE_H264 = 98;
const unsigned short	RTP_PAYLOAD_TYPE_H264_ = 96;
const unsigned short	RTP_PAYLOAD_TYPE_G711 = 8;
const unsigned long		MAX_CH_NUM	= 32;
const unsigned long		MAX_STREAM_NUM = MAX_CH_NUM * 2;//假定一个通道包含一个现场一个回放
const unsigned short	PACKET_POOL_SIZE = 500;//D1下，1000个包大约为4S左右
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
	NAL_UNIT_TYPE_ZERO	= 0,					//未规定
	SLICE_LAYER_WITHOUT_PARTITIONING_RBSP	= 1,//非IDR图像中不采用数据划分的片段
	SLICE_DATA_PARTION_A_LAYER_RBSP			= 2,//非IDR图像中A类数据划分片段
	SLICE_DATA_PARTION_B_LAYER_RBSP			= 3,//非IDR图像中B类数据划分片段
	SLICE_DATA_PARTION_C_LAYER_RBSP			= 4,//非IDR图像中C类数据划分片段
	SLICE_LAYER_WITH_PARTITIONING_RBSP		= 5,//IDR图像的片段
	SEI_RBSP								= 6,//补充增强信息 (SEI)
	SEQ_PARAMETER_SET_RBSP					= 7,//序列参数集
	PIC_PARAMETER_SET_RBSP					= 8,//图像参数集
	ACCESS_UNIT_DELIMITER_RBSP				= 9,//分割符
	END_OF_SEQ_RBSP							= 10,//序列结束符
	END_OF_STREAM_RBSP						= 11,//流结束符
	FILLER_DATA_RBSP						= 12,//填充数据
	//13-23保留  24-31未定义
	SINGLE_TIME_AGGREGATION_PACKET_A		= 24,  //单时间聚合包
	SINGLE_TIME_AGGREGATION_PACKET_B		= 25,  //单时间聚合包
	MULTI_TIME_AGGREGATION_PACKET_A			= 26,  //多时间聚合包
	MULTI_TIME_AGGREGATION_PACKET_B			= 27,  //多时间聚合包
	FRAGMENTATION_UNIT_A					= 28,	//分片的包
	FRAGMENTATION_UNIT_B					= 29,	//分片的包
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
	unsigned char type:5;			//和原始数据中nalu中的type一样
	unsigned char reserve_bit:1;	//保留位，必须是0；
	unsigned char end_bit:1;		//指示是不是这一帧的最后一个分片
	unsigned char start_bit:1;		//指示是不是这一帧的第一个分片

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

///////////////////////////////////////RTCP相关/////////////////////////////////////////////
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



//RTCP接收者报告

typedef struct _extended_seq_num_
{
	unsigned short		CyclesCount;//网络字节顺序  大端模式  CyclesCount在高位
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
		unsigned long	lsrNTP;			//上一个SR中ntp数据的中间32位，每个RR中的lsr和该数据相同就表示这些RR是这个SR的应答
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
	unsigned long	ntpTimeH;				//存放NTP时间的秒部分，UTC时间+2,208,988,800
	unsigned long	ntpTimeL;				//存放秒以下的部分，单位毫秒
	unsigned long	rtpTimestamp;
	unsigned long	senderPacketCount;
	unsigned long	senderOctetCount;
}RTCP_SR_DATA;

typedef struct _rtcp_sender_reports_
{
	RTCP_HEADER		rtcpHeader;
	unsigned long	reporterSSRC;
	unsigned long	ntpTimeH;				//存放NTP时间的秒部分，UTC时间+2,208,988,800
	unsigned long	ntpTimeL;				//存放秒以下的部分，单位毫秒
	unsigned long	rtpTimestamp;
	unsigned long	senderPacketCount;
	unsigned long	senderOctetCount;
}RTCP_SENDER_REPORTS;


typedef enum _sdes_item_type_
{
	SDES_ITEM_END	= 0,
	SDES_ITEM_CNAME	= 1,			//canonical name 标准名称，是个稳定不变的ID，可以用来确定一个具体的参与者
									//当一个会话有多个流时，客户端使用这个ID来确定数据来自同一个数据源。
									//比如说有一个音频和视频，接收端根据这个项可以知道这个音视频需要同步。
	SDES_ITEM_NAME	= 2,
	SDES_ITEM_EMAIL	= 3,
	SDES_ITEM_PHONE	= 4,
	SDES_ITEM_LOC	= 5,
	SDES_ITEM_TOOL	= 6,
	SDES_ITEM_NOTE	= 7,
	SDES_ITEM_PRIV	= 8,
}SDES_ITEM_TYPE;

//往返时间计算方式 RTT = (recvRRTime - sendTime - dlsr) 
typedef struct _sended_sreports_info_
{
	ULONGLONG		sendTime;		//发送时间
	ULONGLONG		recvRRTime;		//收到这个发送报告对应的第一个应答的时间
	union _lsrNTPTime_
	{
		unsigned long	lsrNTP;			//这个SR中ntp数据的中间32位，每个RR中的lsr和该数据相同就表示这些RR是这个SR的应答
		struct _lsrTime_
		{
			unsigned short lsrNTPH;
			unsigned short lsrNTPL;
		}lsrTime;
	}lsrNTPTime;
	unsigned long	dlsr;			//RR中标记的延迟时间。接收者收到SR后会延时一段时间后才发送RR给发送者。 单位微妙
}SENDED_SREPORTS_INFO;

#define lsrNTP		lsrNTPTime.lsrNTP
#define lsrNTPH		lsrNTPTime.lsrTime.lsrNTPH
#define lsrNTPL		lsrNTPTime.lsrTime.lsrNTPL

typedef struct _rtcp_sr_info_
{
	unsigned long		timestamp;
	ULONGLONG			ctsUTCTime;				//刚发送出去的RTP数据报的UTC时间戳
	ULONGLONG			nextSReportsTime;		//下发送SR的时间
	unsigned long		srInterval;				//发送SR的间隔
	unsigned long		senderPacketCount;
	unsigned long		senderOctetCount;
	unsigned long		senderReportCount;		//已经发送出去的SR报告个数
	RTCP_SENDER_REPORTS	rtcpSR;					//rtcp 发送者报告，每发送一个数据包就跟新一次该数据

	std::list<SENDED_SREPORTS_INFO *> srportsInfoList;	//存放已经发送到网络中的SR信息，
														//每发送一个就向链表中添加一项，
														//收到的RR应答中的NTP时间就删除该时间前面的所有SR信息
}RTCP_SR_INFO;

typedef struct _rtcp_report_info_
{	
	CPUB_Lock		lock;
	unsigned long	ssrc;
//	CRTPTime		*pRTPTime;
	ULONGLONG		reportTime;						//发送给对端报告的时间
	unsigned long	totalPacCount;					//作为发送端，是发送的数据包个数，接收端是接收到的数据包个数
	unsigned long	totalOctetCount;				//发送或者接收到的字节数
	unsigned long	pacCount;						//发送端，表示从上次发送SR后发送的数据包个数；接收端表示，从上次接收到SR后接受到RTP的个数
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
	unsigned long		ssrc;					//流的源同步标识
	char				*pStrmName;				//码流名称，H264等
	unsigned long		frequence;				//采样频率
	unsigned long		frameRate;				//流的帧率，音频可以没帧率
	unsigned long		timestampUnit;			//每发送一个RTP数据包时间戳的增加个数
	unsigned long		payloadType;
	unsigned long		rtt;					//当前的往返时间  RTT(i+1) = 0.9RTT(i) + 0.1RTT(new)
	float				packetLoseRate;			//数据报丢失率		R(i+1) = (1 - 0.3)R(i) + 0.3P(FL)
	unsigned long		rtpChannel;				//rtp数据在RTSP数据中占用的通道号
	unsigned long		rtcpChannel;			//rtcp数据在RTSP数据中占用的通道号
	CRTPSock			rtpSock;
	CRTPSock			rtcpSock;
}RTP_STREAM_INFO;

typedef struct _rtp_session_info_
{
	unsigned long		sessionID;				//会话ID，在RTSP会话时产生，每个ID对应一个RTSP会话和一个RTP/RTCP传输
	SWL_socket_t		rtspSockfd;				//rtsp的套接字
	STREAM_TYPE			streamType;				//区分现场、回放、和录像码流
	unsigned long		channel;				//请求的视频通道
	bool				bHasAudio;				//标记这个通道是否有音频数据
	RTSP_SESSION_STATUS	sessionStatus;			//当前会话的状态
	struct sockaddr_in	rtspSocketAddr;			//rtsp地址和端口，对RTP来说只有IP地址有用
	char				*pServerDes;			//服务器的描述信息
	char				*pFunction;				//支持的功能
	char				*pContentType;
	unsigned long		contentLength;
	bool				bISTcpSession;			//是否使用TCP传输RTP数据
	bool				bISUnicast;				//是否是单播

	RTP_STREAM_INFO		audioStreamInfo;		//音频流信息
	RTP_STREAM_INFO		videoStreamInfo;		//视频流信息

	RTCP_SR_INFO		rtcpASRInfo;			//音频的发送报告信息
	RTCP_SR_INFO		rtcpVSRInfo;			//视频的发送报告信息
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
