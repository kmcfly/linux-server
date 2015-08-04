#ifndef _GB28181_SDP_H
#define _GB28181_SDP_H

#include "SWL_TypeDefine.h"
#include "NetServer.h"
#include "NetProtocol.h"
#include "FrameData.h"
#include "FrameBuffer.h"
#include "dvrdvsdef.h"
#include "NetConfigDefine.h"
#ifdef __ENVIRONMENT_LINUX__
#include "sys/ioctl.h"
#include "net/if.h"
#include "NetInterface.h"
#endif
typedef enum _play_type
{
	PLAY= 0,     //实时流
	PLAYBACK,   //回放
	DOWNLOAD,   //下载
	PLAY_COUNT,
}PALY_TYPE;


typedef  struct _sdp_u_info
{
	char m_cType;  //简捷方式和普通方式
	char m_url[256];  //普通方式

	char m_cDeviceID[32]; //设备ID 简捷方式
	int  m_lParam;     //参数
}SDP_U_INFO;


typedef enum  _video_encode_format
{
	VIDEO_FORMAT_MPEG4=1,
	VIDEO_FORMAT_H264,
	VIDEO_FORMAT_SVAC,
	VIDEO_FORMAT_3GP,
	VIDEO_FORMAT_UNKNOW,
}VIDEO_ENCODE_FORMAT;


typedef enum  _encode_resolution
{
	RESOLUTION_QCIF = 1,
	RESOLUTION_CIF,
	RESOLUTION_4CIF,
	RESOLUTION_D1,
	RESOLUTION_720P,
	RESOLUTION_1080P,
	RESOLUTION_UNKNOW,
}ENCODE_RESOLUTION;

typedef enum  _quality_control_type
{
	QUALITY_CONTROL_CBR=1,
	QUALITY_CONTROL_VBR,
	QUALITY_CONTROL_UNKNOW,
}QUALITY_CONTROL_TYPE;

typedef enum  _audio_encode_format
{
	AUDIO_FORMAT_G711 = 1,
	AUDIO_FORMAT_G7231,
	AUDIO_FORMAT_G729,
	AUDIO_FORMAT_G7221,
	AUDIO_FORMAT_UNKNOW,
}AUDIO_ENCODE_FORMAT;

/***************************************************************************/
/*f = v/编码格式/分辨率/帧率/码率类型/码率大小 a/编码格式/码率大小/采样率
/*若只有视频时，音频中的各参数项可以不填写，但应保持“a///”的结构： 
/*f = v/编码格式/分辨率/帧率/码率类型/码率大小 a/// 
/*若只有音频时也类似处理，视频中的各参数项可以不填写，但应保持 “v/////”的结构
/*f = v/////a/编码格式/码率大小/采样率 
/*f 字段中视、音频参数段之间不需空格分割
/****************************************************************************/

typedef struct _sdp_f_info
{
	unsigned int  m_ulVidEncodeFormat; //编码格式 1 –MPEG-4     2 –H.264     3 – SVAC     4 –3GP 
	unsigned int  m_ulVidResolution;    //分辨率 1–QCIF   2–CIF    3–4CIF    4–D1  5-720P  6–1080P/I 
	unsigned int  m_ulVidFramerate;   //帧率 0 ~ 99 
	unsigned int  m_ulVidBitRateType;      //码率类型 1–固定码率（CBR）  2–可变码率（VBR）
	unsigned int  m_ulVidBitRate;     //码率大小 0 ~ 100000 （注：如  1表示1kbps）

	unsigned  int m_ulAudEncodeForamt;  //音频编码格式 1 – G.711   2 – G.723.1   3 – G.729   4 – G.722.1 
	unsigned  int m_ulAudBitRate;    //音频码率大小   1-5.3 kbps  2-6.3 kbps 3-8 kbps  4-16 kbps 5-24 kbps 6-32 kbps 7- 48 kbps 8-64 kbps

	unsigned  int m_ulAudSampleRate;   //音频采样率 1-8 kHz(G.711/G.723.1/G.729) 2- 14 kHz(G.722.1) 3-16 kHz(G.722.1) 4-32 kHz(G.722.1) 

}SDP_F_INFO;

//解析invite请求的信息
typedef struct _call_info
{	
	int did;							/**< unique id for SIP dialogs */
	int cid;							/**< unique id for SIP calls (but multiple dialogs!) */
	unsigned  int   StreamType;			//媒体流流类型   0：现场 1：回放 2：下载
	unsigned  long  ulChannelId;			//通道号
	unsigned short	usReqPort;		    //媒体服务器的端口号
	unsigned int	ulReqIp;              //媒体服务器的ip地址
	unsigned int	ulTransProtocol;      //媒体流的传输协议 0:TCP  1:UDP  2:RTP/AVP
	unsigned int 	VideoPlayload;	    //视频的playload类型
	unsigned int 	AudioPlayload;     //音频的playload类型
	unsigned long   ulStartTime;        //开始时间
	unsigned long   ulStopTime;         //结束时间
	unsigned int 	ulSsrc;              //同步源   
	
	PALY_TYPE      PlayType;     //播放类型，0：实时流；1：回放；2：下载
	SDP_U_INFO	   Sdp_u_Info;     //sdp  u字段
	SDP_F_INFO     Sdp_f_Info;     //sdp  f字段
}CALLINFO;


int SetVideoSdp_F_Info(SDP_F_INFO &Sdp_f_Info,char *p,int value);  
int SetAudioSdp_F_Info(SDP_F_INFO &Sdp_f_Info,char *p,int value);  

int GetSdp_F_V_Info(SDP_F_INFO &Sdp_f_Info,char *str);   //获取sdp视频的基本参数

int GetSdp_F_A_Info(SDP_F_INFO &Sdp_f_Info,char *str); //获取sdp音频的基本参数

int ParseSdpYUFInfo(CALLINFO *pCallInfo,char *pStr); //解析sdp中的Y，U,F字段
#endif