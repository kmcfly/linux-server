#ifndef __FRAME_CHECK_H__
#define __FRAME_CHECK_H__

#include "dvrdvsdef.h"

// typedef struct __support_enc_info__
// {
// 	unsigned long	ulRes;			//分辨率
// 	unsigned short	usMaxRate;		//最大帧率
// 	unsigned short	usMaxBitrate;	//最大码率，当前不使用，还用老的方式做处理
// 	unsigned long	ulDefBitrate;	//默认码率，当前不使用，还用老的方式做处理
// }SUPPORT_ENC_INFO;

typedef struct __device_enc_info__
{
	unsigned short		usTotalRate;		//总帧率
	unsigned long		ulRes;				//分辨率
	unsigned short		usTotalBitrate;		//总码率
	unsigned short		usBCurrentRes[32];	//是否是当前用的分辨率
	SUPPORT_ENC_INFO *	pSupportEncInfo;	//每个通道该分辨率的信息, 长度为sizeof(ENC_INFO) * channelNum
}DEVICE_ENC_INFO;


class CEncodeCheck
{
public:
	~CEncodeCheck();

	static CEncodeCheck * Instance();

	bool Initial(unsigned long ulChnn);
	void Quit();

	int AddEncodeInfo(unsigned long ulRes, unsigned short usTotalRate, unsigned short usMaxRate, unsigned short usTotalBitrate);

	int GetEncodeInfo(SUPPORT_ENC_INFO * pSupportEncInfo, unsigned long & dataLen);
	bool CheckEncodeParam(unsigned long ulChnn, CHECK_ENC_INFO * pCheckEncInfo);
	void SetChannelRes(unsigned long ulChnn, unsigned long ulRes);
	void ClearData();
protected:
	CEncodeCheck();
private:
	unsigned long GetDefBitrate(unsigned long ulRes);
	
private:
	static CEncodeCheck	* m_pEncodeCheck;
	unsigned long		m_channel;
	unsigned long		m_resNum;

	DEVICE_ENC_INFO		m_deviceEncInfo[128];
	bool				m_bSourceSelf;

	bool				m_bHaveInitial;
};

#endif

