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
	unsigned long	m_chnnNum;			//设备通道数
	unsigned long	m_autoAllotChnnCount;	//可自动分配的最大发送份数
	unsigned long	m_autoAllotRateCount;	//可自动分配的帧个数，以CIF为单位
	unsigned long	m_limitRateCount;	//本地回放、有网络主码流时自动分配的总帧个数
	unsigned long	m_maxNetStreamChnnCount;

	unsigned long	m_usedRate;			//当前使用的帧率个数
	ENCODE_INFO		m_secondStreamEncodeInfo[MAX_CHNN_NUM];

	CHNN_VIDEO_INFO m_chnnVideoInfo[MAX_CHNN_NUM];
};
#endif
