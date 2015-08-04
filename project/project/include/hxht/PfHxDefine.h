/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : liwenxie
** Date         : 2010-03-30
** Name         : PfHxDefine.h
** Version      : 1.0
** Description  : 
				��ͷ�ļ��ﶨ��Ľṹ�嶨��Ĺ��ܶ�Ҫ���豸������֧�֣�
				�ṹ���ʹ�÷�ʽ��CHxPull��CHxPush��
			//  ���豸��Ҫ��:
			//	����һЩ����/�������豸û���ṩ����Ҫ�豸�������Ƶ�:
			//	1. ͼ���ڵ����ڵ���������������쵽ͼ���ڵ�����һ��
			//	2. ͼ�������ĻOSD���豸ֻ�ṩ��ʱ��ĵ��ӣ���Ҫ�ṩ����ĵ��ӣ�
			//	    ͬʱ�ṩʱ��ͱ������λ�õĲ�����������ṩPoint���͵�λ��
			//	3. �������ṩ�����ı���빦�ܣ�����ѹ���ϴ������ؽ��벥��
			//	4. PTZ���ƣ��ṩ��̨���ƣ�Ԥ�õ㣬�Զ���λ���Զ�Ѳ���ȹ��ܣ�
			//	5. ��̨�����ṩ͸�����ܣ�
			//	6. ��̨��������Ҫ�ṩ����λ��ֹͣλ��У��λ�����ط�ʽ�Ĺ��ܣ�
***********************************************************************/
#ifndef _PFHXDEFINE_H
#define _PFHXDEFINE_H




enum _HX_OPTYPE_E
{
	TIME_OP 	= 1,
	MANUAL_OP	= 2,
};

//���ڱ�ʶ����֡����
enum _HX_FRAME_TYPE_E
{
	REAL_VIDEO = 1,		//ʵʱ��Ƶ����
	REAL_AUDIO = 2,		//ʵʱ��Ƶ����
	VOD_VIDEO = 3,		//�ط���Ƶ����
	VOD_AUDIO = 4,		//�ط���Ƶ����
};

//���ڱ�ʾ������
typedef struct _PfPos
{
	int nX;
	int nY;
}PfPos;

//���ڱ�ʾ����
typedef struct _PfRect
{
	int nLeft;		//�����xֵ
	int nTop;		//������yֵ
	int nWidth;		//���ο�ֵ
	int nHeight;	//���θ�ֵ
}PfRect;

//��ʾʱ��
typedef struct _PfDateTime
{
	int nYear;
	int nMonth;
	int nDay;
	int nHour;
	int nMinute;
	int nSecond;
}PfDateTime;


//���ڽ��뻥�Ż�ͨ�����ò���
//�˲�����Ҫ���ñ�����flash�ڣ�����ϵͳһ�����
//���ڳ�ʼ�����Ż�ͨ�������
typedef struct _hx_net_init_info_
{
	char szDeviceID[32];		//�豸ID
	char szStoreID[32];			//�洢�豸ID
	char szLocalIP[16];
	char szCsgIP[16];			//���������IP
	unsigned short nCsgPort;	//�豸ע�ᵽ����������Ķ˿�
	unsigned short nStorePort;	//�洢ע�ᵽ����������Ĵ洢�˿�
	unsigned short nMsgPort;	//�豸��������Ϣ�˿�
	unsigned short nVideoPort;	//�豸��������Ƶ�˿�
	unsigned short nAudioPort;	//�豸��������Ƶ�˿�
	unsigned short nUpdatePort;	//�豸�����ĸ��¶˿�
	unsigned short nWebPort;	//�豸������Web�˿�
	unsigned short dwMaxSensorIn;	//����������
	unsigned long  dwMaxSensorOut; //�������������

	unsigned char	dwMaxVideo;
	unsigned char	dwMaxAudio;
	unsigned char	dwMaxLink;
	unsigned char	dwDeviceType;

	unsigned long	dwSoftVer;
	unsigned long	dwBuildTime;

	char			deviceName [20];
	char			szMac[8];//ֻ����ǰ6���ֽ�
}HX_NET_INIT_INFO;


//ע���HXHT�豸���صĻỰ����
typedef struct _hx_net_message_info_
{
	char szMessageID[32];
	char szName[64];
	int  heartCycle;
	char szSysRebootTime[32];
	int sysRebootCycle;
	char szDispatchId[32];
	char szPlatformTel[32];
	char configServer[48];
	char curDateTime[48];
}HX_NET_MESSAGE_INFO;

//�洢ע��������Ϣ
typedef struct _hx_net_store_info_
{
	char	szStoreID[64];
	int		stortHeartCycle;
	int		maxInputNum;
	int		maxOutputNum;
	char	szFtpUser[256];
	char	szFtpPass[256];
	int		ftpPort;
}HX_NET_STORE_INFO;

//��Ƶ�������
typedef struct _PfViEncCfg
{
	int nFrameRate;			//֡��
	int nIFrameInterval;	//I֡���
	int nQuality;			//ͼ������
	int nBitRate;			//����
	int nFormat;			//�ֱ���
}PfViEncCfg;

//��Ƶͼ�����
typedef struct _PfViParam
{
	int nBrightness;		//����
	int nContrast;			//�Աȶ�
	int nSaturation;		//���Ͷ�
	int nHue;				//ɫ��
}PfViParam;

//��Ƶ�������
typedef struct _PfAuEncCfg
{
	int nChannels;			//����ͨ����
	int wBitsPerSample;		//�������
	int nSamplesPerSec;		//�����ٶ�
	int nCompressedFormat;	//ѹ����ʽ
}PfAuEncCfg;

//��Ƶ�������
typedef struct _PfAuDecCfg
{
	int nChannels;			//����ͨ����
	int wBitsPerSample;		//�������
	int nSamplesPerSec;		//�����ٶ�
	int nCompressedFormat;	//ѹ����ʽ
}PfAuDecCfg;


//ͼ���ڵ�����
typedef struct _PfImageCover
{
	char nEnable;		//����
	int nNum;			//ʵ���ڵ��������
	PfRect strRect[8];	//�ڵ�����
}PfImageCover;

//ͼ�����OSD����
typedef struct _PfImageOSD
{
	char nEnable;		//�Ƿ�����
	char nTime;			//�Ƿ����ʱ��OSD
	char nTitle;			//�Ƿ���ӱ���OSD
	char nBitRate;		//�Ƿ��������OSD
	char szTitle[32];	//��������
	PfPos TimePos;		//ʱ��OSDλ��
	PfPos TitlePos;		//����OSDλ��
	PfPos BitRatePos;	//����OSDλ��
}PfImageOSD;

//�ƶ�������
typedef struct _PfMoveDetect
{
	char nEnable;		//�Ƿ�����
	char nDetectGrade;	//������
	short nNum;			//ʵ��������
	PfRect strRect[8];	//�ƶ��������
}PfMoveDetect;

//�������
typedef struct _PfNet
{
	char nWifiEnable;
	char szIP[16];		//IP��ַ
	char szNetMask[16];	//��������
	char szGateWay[16];	//����
}PfNet;

//485���ڲ���
typedef struct _Pf485Param
{
	int nSpeed;		//������
	int nData;		//����λ
	int nStop;		//ֹͣλ
	int nCheck;		//У��λ
	int nControl;	//���ط�ʽ
}Pf485Param;

//�����������
typedef struct _PfAlarmOut
{
	int nEnable;	//���ñ������
}PfAlarmOut;





//����Ӳ�̲���
typedef struct _PfDiskInfo
{
	char nExist;			//�Ƿ����Ӳ��
	char nRecordType;	//��ͣ/ѭ��д��
	char nRecord;		//�Ƿ�֧��¼��
	char nSnap;			//�Ƿ�֧��ץ��
	unsigned long nDiskSpace;	//���̿ռ��С
}PfDiskInfo;


//¼�����
typedef struct _PfRecordCfg
{
	int nType;		//��ʱ¼������ֶ�¼��(HX_OP_E)
	int nFileLen;	//�ļ�����
	char szTimeCfg[7][24];	//���ö�ʱ¼��ʱ��(�ֶ�¼�������Ч)
}PfRecordCfg;

//ץ�Ĳ���
typedef struct _PfSnapCfg
{
	int nType;		//��ʱץ�Ļ����ֶ�ץ��(HX_OP_E)
	char szTimeCfg[7][24];	//���ö�ʱץ��ʱ��(�ֶ�ץ�Ĵ�����Ч)
}PfSnapCfg;



//¼���ѯ
typedef struct _PfFileQuery
{
	int nChn;			//ͨ����
	int nFileType;		//�ļ�����  ¼��/ץ��
	int nRecordType;	//��ʱ/�ֶ�/����
	char szFileName[64];	//�ļ���
	unsigned long lBegin;	//��ʼʱ��
	unsigned long lEnd;		//����ʱ��
}PfFileQuery;

//�ļ���Ϣ
typedef struct _PfFileInfo
{
	int nFileType;		//�ļ����ͣ�¼��/ץ��
	int nRecordtype;	//��ʱ/�ֶ�/����
	char szFileName[32];	//�ļ�����
	unsigned long lFileTime;//��¼ʱ��
}PfFileInfo;

//��ѯ���
typedef struct _PfQueryResult
{
	int nFileNum;			//ʵ�ʷ��ؼ�¼����
	int nRemainNum;			//��Ȼ�ж��ٸ���¼
	PfFileInfo FileInfo[20];	//�ļ���Ϣ
}PfQueryResult;

//��̨����
typedef struct _PfPTZControl
{
	int nOp;	//��������
}PfPTZControl;

//Ԥ�õ�����
typedef struct _PfPresetPoint
{
	int nPresetNo;
	char szPresetName[16];
}PfPresetPoint;

//Ԥ�õ���Ϣ
typedef struct _PfPreset
{
	int nPresetNum;			//Ԥ�õ����
	PfPresetPoint Point[10];//Ԥ�õ�
}PfPreset;



//�豸����״̬����������״̬���豸Ӳ��״̬
typedef struct _PfDvsStatus
{
	char nAlarmIn;			//�Ƿ����뱨��(����)
	char nMotionAlarm;		//�Ƿ��ƶ���ⱨ��
	char nCoverAlarm;		//�Ƿ��ڵ�����
	char nVLostAlarm;		//�Ƿ���Ƶ��ʧ����
	int nCPUUseRatio;		//CPUʹ����
	int nMemUseRatio;		//�ڴ�ʹ����
	int nDiskSpaceRatio;	//���̿ռ�ʹ����
	int nDiskSpaceBalance;
	int nProcNum;			//��ǰϵͳ������
	unsigned long nCurTime;	//ϵͳ��ǰʱ��
}PfDvsStatus;
















#endif //_PFHXDEFINE_H

