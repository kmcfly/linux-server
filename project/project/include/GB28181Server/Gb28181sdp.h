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
	PLAY= 0,     //ʵʱ��
	PLAYBACK,   //�ط�
	DOWNLOAD,   //����
	PLAY_COUNT,
}PALY_TYPE;


typedef  struct _sdp_u_info
{
	char m_cType;  //��ݷ�ʽ����ͨ��ʽ
	char m_url[256];  //��ͨ��ʽ

	char m_cDeviceID[32]; //�豸ID ��ݷ�ʽ
	int  m_lParam;     //����
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
/*f = v/�����ʽ/�ֱ���/֡��/��������/���ʴ�С a/�����ʽ/���ʴ�С/������
/*��ֻ����Ƶʱ����Ƶ�еĸ���������Բ���д����Ӧ���֡�a///���Ľṹ�� 
/*f = v/�����ʽ/�ֱ���/֡��/��������/���ʴ�С a/// 
/*��ֻ����ƵʱҲ���ƴ�����Ƶ�еĸ���������Բ���д����Ӧ���� ��v/////���Ľṹ
/*f = v/////a/�����ʽ/���ʴ�С/������ 
/*f �ֶ����ӡ���Ƶ������֮�䲻��ո�ָ�
/****************************************************************************/

typedef struct _sdp_f_info
{
	unsigned int  m_ulVidEncodeFormat; //�����ʽ 1 �CMPEG-4     2 �CH.264     3 �C SVAC     4 �C3GP 
	unsigned int  m_ulVidResolution;    //�ֱ��� 1�CQCIF   2�CCIF    3�C4CIF    4�CD1  5-720P  6�C1080P/I 
	unsigned int  m_ulVidFramerate;   //֡�� 0 ~ 99 
	unsigned int  m_ulVidBitRateType;      //�������� 1�C�̶����ʣ�CBR��  2�C�ɱ����ʣ�VBR��
	unsigned int  m_ulVidBitRate;     //���ʴ�С 0 ~ 100000 ��ע����  1��ʾ1kbps��

	unsigned  int m_ulAudEncodeForamt;  //��Ƶ�����ʽ 1 �C G.711   2 �C G.723.1   3 �C G.729   4 �C G.722.1 
	unsigned  int m_ulAudBitRate;    //��Ƶ���ʴ�С   1-5.3 kbps  2-6.3 kbps 3-8 kbps  4-16 kbps 5-24 kbps 6-32 kbps 7- 48 kbps 8-64 kbps

	unsigned  int m_ulAudSampleRate;   //��Ƶ������ 1-8 kHz(G.711/G.723.1/G.729) 2- 14 kHz(G.722.1) 3-16 kHz(G.722.1) 4-32 kHz(G.722.1) 

}SDP_F_INFO;

//����invite�������Ϣ
typedef struct _call_info
{	
	int did;							/**< unique id for SIP dialogs */
	int cid;							/**< unique id for SIP calls (but multiple dialogs!) */
	unsigned  int   StreamType;			//ý����������   0���ֳ� 1���ط� 2������
	unsigned  long  ulChannelId;			//ͨ����
	unsigned short	usReqPort;		    //ý��������Ķ˿ں�
	unsigned int	ulReqIp;              //ý���������ip��ַ
	unsigned int	ulTransProtocol;      //ý�����Ĵ���Э�� 0:TCP  1:UDP  2:RTP/AVP
	unsigned int 	VideoPlayload;	    //��Ƶ��playload����
	unsigned int 	AudioPlayload;     //��Ƶ��playload����
	unsigned long   ulStartTime;        //��ʼʱ��
	unsigned long   ulStopTime;         //����ʱ��
	unsigned int 	ulSsrc;              //ͬ��Դ   
	
	PALY_TYPE      PlayType;     //�������ͣ�0��ʵʱ����1���طţ�2������
	SDP_U_INFO	   Sdp_u_Info;     //sdp  u�ֶ�
	SDP_F_INFO     Sdp_f_Info;     //sdp  f�ֶ�
}CALLINFO;


int SetVideoSdp_F_Info(SDP_F_INFO &Sdp_f_Info,char *p,int value);  
int SetAudioSdp_F_Info(SDP_F_INFO &Sdp_f_Info,char *p,int value);  

int GetSdp_F_V_Info(SDP_F_INFO &Sdp_f_Info,char *str);   //��ȡsdp��Ƶ�Ļ�������

int GetSdp_F_A_Info(SDP_F_INFO &Sdp_f_Info,char *str); //��ȡsdp��Ƶ�Ļ�������

int ParseSdpYUFInfo(CALLINFO *pCallInfo,char *pStr); //����sdp�е�Y��U,F�ֶ�
#endif