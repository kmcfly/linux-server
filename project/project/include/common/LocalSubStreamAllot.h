/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xianshiwei
* Date         : 2015-01-09
* Name         : LocalStreamAllotDevice.h
* Version      : 1.0
* Description  : 

* Modify Record:

*/

#ifndef __LOCAL_STREAM__ALLOT_DEVICE_H__
#define __LOCAL_STREAM__ALLOT_DEVICE_H__

#include "PUB_common.h"
#include "device.h"

class CLocalSubStreamAllot{
public:
	~CLocalSubStreamAllot();
	static CLocalSubStreamAllot * Instance();
	bool Init(PRODUCT_TYPE productID, unsigned long chnnNum, unsigned long autoAllotChnnCount);

	bool AllotLocalSubStream(ULONGLONG netChnn, ULONGLONG mobileChnn, ULONGLONG mobileHigh,\
							unsigned long netStreamChnnCount, bool bUseLimitRateCount=false);

	void SetChnnEncodeInfo(unsigned long chnn, const ENCODE_INFO &encodeInfo);

	void GetUsedNetVideoSize(unsigned long &size);

private:
	CLocalSubStreamAllot();
	bool SupportProduct();
	static const unsigned long MAX_CHNN_NUM		= 128;

	typedef struct __chnn_video_info__
	{
		unsigned long chnn;
		unsigned long videoSize;
		unsigned long rate;
		unsigned long encodeType;
		unsigned long lBitStream;
		unsigned long quality;
	}CHNN_VIDEO_INFO;
private:
	PRODUCT_TYPE	m_productID;
	unsigned long	m_chnnNum;			//�豸ͨ����
	unsigned long	m_autoAllotChnnCount;	//���Զ����������ͷ���
	unsigned long	m_autoAllotRateCount;	//���Զ������֡��������CIFΪ��λ
	unsigned long	m_limitRateCount;	//���ػطš�������������ʱ�Զ��������֡����
	unsigned long	m_maxNetStreamChnnCount;
	unsigned long	m_usedRate;			//��ǰʹ�õ�֡�ʸ���
	
	ENCODE_INFO_RECORD	m_secondStreamEncodeInfoOriginal[MAX_CHNN_NUM];//δ��֡��ʱ��ı�����Ϣ
	ENCODE_INFO		m_secondStreamEncodeInfo[MAX_CHNN_NUM];
	CHNN_VIDEO_INFO m_chnnVideoInfo[MAX_CHNN_NUM];

};
#endif