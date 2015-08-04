#ifndef __NET_STREAM_ALLOT_DEVICE_H__
#define __NET_STREAM_ALLOT_DEVICE_H__

#include "PUB_common.h"
#include "device.h"


class CNetStreamAllotDevice
{
public:
	~CNetStreamAllotDevice();
	static CNetStreamAllotDevice * Instance();
	bool Init(PRODUCT_TYPE productID, unsigned long chnnNum, unsigned long autoAllotChnnCount, unsigned long autoAllotD1ChnnCount, unsigned long limitD1ChnnCount);
	void Quit();

	bool AllotNetStream(ULONGLONG netChnn, ULONGLONG mobileChnn, ULONGLONG mobileHigh, unsigned long netStreamChnnCount, bool bUseLimitRateCount=false);
	void SetChnnEncodeInfo(unsigned long chnn, const ENCODE_INFO &encodeInfo);
	void GetUsedNetVideoSize(unsigned long &size);
protected:
private:
	static const unsigned long MAX_CHNN_NUM		= 64;
	static const unsigned long VIDEO_SIZE_CIF	= 1;
	static const unsigned long VIDEO_SIZE_D1	= 4;
	static const unsigned long VIDEO_SIZE_960H	= 7;
	static const unsigned long VIDEO_SIZE_720P	= 15;
	static const unsigned long VIDEO_SIZE_1080P	= 40;
private:
	CNetStreamAllotDevice();
	bool CheckProduct();

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
	ENCODE_INFO		m_secondStreamEncodeInfo[MAX_CHNN_NUM];

	CHNN_VIDEO_INFO m_chnnVideoInfo[MAX_CHNN_NUM];
};
#endif
