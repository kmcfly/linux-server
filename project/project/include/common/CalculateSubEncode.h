/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xian shi wei
* Date         : 2015-01-05
* Name         : EncodeSubCalculate.h
* Version      : 1.0
* Description  : 

* Modify Record:

*/


#ifndef ___CALCULATE_SUB_ENCODE__H___
#define ___CALCULATE_SUB_ENCODE__H___

#include "CalculateEncode.h"

#define RESOLUTION_NUM 6
#define RESOLUTION_TOTAL_NUM_ 16

typedef struct __device_enc_sub_info__
{
	unsigned short			usTotalRate;		//总帧率
	unsigned short			usTotalBitrate;		//总码率
	unsigned short			usBCurrentRes[32];	//是否是当前用的分辨率
	unsigned long			ulRes;				//分辨率
	SUPPORT_SUB_ENC_INFO *	pSupportEncInfo;	//每个通道该分辨率的信息, 长度为sizeof(ENC_INFO) * channelNum
}DEVICE_SUB_ENC_INFO;



class CCalculateSubEncode:
	public CCalculateEncode{

public:
	~CCalculateSubEncode();

	static CCalculateSubEncode * Instance();

	bool Initial(unsigned long ulChnn);
	/*
		添加分辨率的时候，要从小到大开始添加，否则可能会去问题
	*/
	int AddEncodeInfo(unsigned long ulRes, unsigned short usTotalRate, unsigned short usMaxRate, unsigned short usTotalBitrate);
	int GetEncodeInfo(SUPPORT_SUB_ENC_INFO * pSupportEncInfo, unsigned long & dataLen);
	bool CheckEncodeParam(unsigned long ulChnn, CHECK_ENC_INFO * pCheckEncInfo);
	void SetChannelRes(unsigned long ulChnn, unsigned long ulRes);
   
	/*
		修改各个通道分辨率范围的掩码
	*/
	int SetChannelResMask(unsigned long ulChnn, unsigned long vedioSize);

	unsigned long GetResNum()const{ return m_resNum; }
	unsigned long GetChannelNum()const{ return m_channel;}
	/*
		取出所有通道的编码信息
	*/
    int GetEncodeAllInfo(SUPPORT_SUB_ENC_INFO * pSupportEncInfo, unsigned long & dataLen);

	//inline unsigned long GetResNum() const{ return m_resNum;}
	//inline unsigned long GetChannel() const { return m_channel;}
	//inline DEVICE_SUB_ENC_INFO * GetAllEncodeInfo() const{ return m_deviceEncInfo;}
	void ClearData();

protected:
	CCalculateSubEncode();
private:
	unsigned long GetDefBitrate(unsigned long ulRes);


private:
	bool				m_bSourceSelf;
	bool				m_bHaveInitial;
	static CCalculateSubEncode	* m_pSubEncode;

	unsigned long		m_channel;
	unsigned long		m_resNum;
	DEVICE_SUB_ENC_INFO	m_deviceEncInfo[RESOLUTION_TOTAL_NUM_];
	/*SUPPORT_SUB_ENC_INFO__ *m_pChannelInfo;*/
	unsigned long     * m_pChannelResMask;

	//每个通道所接设备的分辨率
	unsigned long deviceLocalRes[64];
    //list<DEVICE_SUB_ENC_INFO> m_deviceEncInfo;

};

#endif