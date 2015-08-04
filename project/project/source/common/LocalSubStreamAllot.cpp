/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xian shi wei
* Date         : 2015-01-09
* Name         : LocalStreamAllotDevice.cpp
* Version      : 1.0
* Description  : 

* Modify Record:

*/

#include "LocalSubStreamAllot.h"
#include "LocalDevice.h"
#include "MessageMan.h"
#include "FuncCustom.h"


CLocalSubStreamAllot * CLocalSubStreamAllot::Instance()
{
	static CLocalSubStreamAllot netSubStreamAllot;
	return &netSubStreamAllot;
}

CLocalSubStreamAllot::CLocalSubStreamAllot()
{
	m_chnnNum = 0;				//设备通道数
	m_autoAllotChnnCount = 0;	//可自动分配的帧个数，以CIF为单位
}

CLocalSubStreamAllot::~CLocalSubStreamAllot()
{

}

bool CLocalSubStreamAllot::Init(PRODUCT_TYPE productID, unsigned long chnnNum, unsigned long autoAllotChnnCount)
{
	m_productID = productID;
	m_chnnNum = chnnNum;
	m_autoAllotChnnCount = autoAllotChnnCount;

	if (VIDEO_FORMAT_PAL == g_video_format)
	{
		m_autoAllotChnnCount = m_autoAllotChnnCount * 25;
		//m_limitRateCount = m_limitRateCount * 25;
	}
	else
	{
		m_autoAllotChnnCount = m_autoAllotChnnCount * 30;
		//m_limitRateCount = m_limitRateCount * 30;
	}

	m_maxNetStreamChnnCount = chnnNum;
	
	memset(m_secondStreamEncodeInfoOriginal, 0, sizeof(ENCODE_INFO) * MAX_CHNN_NUM);
	/*for(unsigned int size=0;size<MAX_CHNN_NUM;++size)
	{
		m_secondStreamEncodeInfoOriginal[size].rate = -1;
	}*/
	memset(m_secondStreamEncodeInfo, 0, sizeof(ENCODE_INFO) * MAX_CHNN_NUM);
	memset(m_chnnVideoInfo, 0, sizeof(m_chnnVideoInfo));
	return true;
}

/*
*IN：
*	netChnn 当前请求的子码流通道
*	mobileChnn 当前请求的子码流通道
*	netStreamChnnCount 当前请求的网络份数
*OUT:
*	true 由自动分配模块分配网络子码流
*	false 由外部分配网络子码流情况
*/
bool CLocalSubStreamAllot::AllotLocalSubStream(ULONGLONG netChnn, ULONGLONG mobileChnn, ULONGLONG mobileHigh, unsigned long netStreamChnnCount, bool bUseLimitRateCount)
{
	//如果这个产品不支持自动分配就返回
	if (!SupportProduct())
	{
		return false;
	}

	//检查网络码流
	unsigned long chnnCount = 0;
	for (int i=0; i<m_chnnNum; i++)
	{
		if ((netChnn >> i) & 0x01)
		{
			chnnCount++;
		}
	}

	//检查手机码流
	unsigned long mobileCount = 0;
	for (int i=0; i<m_chnnNum; i++)
	{
		if ((mobileChnn >> i) & 0x01)
		{
			mobileCount++;
		}
	}

	//检查高速手机码流
	unsigned long mobileHighCount = 0;
	for (int i=0; i<m_chnnNum; i++)
	{
		if ((mobileHigh >> i) & 0x01)
		{
			mobileHighCount++;
		}
	}

	unsigned long videoSize = DVRVIDEO_SIZE_CIF;
	unsigned long rate = 1;
	unsigned long mobileRate = 3;
	unsigned long bitStream = 512;
	unsigned long setBitStream = 0;
    
	unsigned long netIeNum = netStreamChnnCount - mobileCount;

	//连一个手机码流的时候只传3帧图片，多于一个通道时只传1帧图片
	if (1 < mobileCount)
	{
		mobileRate = 1;
	}
	else if (1 == mobileCount)
	{
		mobileRate = 3;
	}

	////如果只有 1路IE ，配置的是什么就给什么分辨率（最大为 1080p）
	//if (1 == netIeNum)
	//{
	//	videoSize = 0;//下面会做处理 
	//} 
	//else if((1< netIeNum) && (netIeNum <= 4))   //如果  2-->4 路IE端 （最大只能 配置 4路 720p）
	//{
	//	videoSize = DVRVIDEO_SIZE_HD720;
	//}
	//else if((4< netIeNum)&& ( netStreamChnnCount <= m_chnnNum))
	//{
	//	videoSize = DVRVIDEO_SIZE_D1;
	//} 
	//else if(m_chnnNum<netIeNum)
	//{
	//	videoSize = DVRVIDEO_SIZE_CIF;
	//}

	//设置码流信息到SDK,如果没有改变参数就不操作SDK，防止频繁操作导致系统变慢
	for (int chnn=0; chnn<m_chnnNum; chnn++)
	{
		unsigned long rateTemp = 0;
		if ((mobileChnn >> chnn) & 0x01)
		{
			//手机码流默认分辨率为CIF
			if (DVRVIDEO_SIZE_CIF != m_chnnVideoInfo[chnn].videoSize)
			{
				m_chnnVideoInfo[chnn].videoSize = DVRVIDEO_SIZE_CIF;
				CLocalDevice::Instance()->SetVideoSize(chnn, DVRVIDEO_SIZE_CIF, false);
				CLocalDevice::Instance()->SetFrameRateQuality(  chnn, mobileRate, static_cast<VIDEO_ENCODE_MODE>(m_secondStreamEncodeInfo[chnn].encodeType),\
					m_secondStreamEncodeInfo[chnn].hBitStream, m_secondStreamEncodeInfo[chnn].quality, false);
				CMessageMan::Instance()->RefreshOSDName(chnn);
			}

			if ((m_chnnVideoInfo[chnn].rate != mobileRate) || (m_chnnVideoInfo[chnn].encodeType != m_secondStreamEncodeInfo[chnn].encodeType) || 
				(m_chnnVideoInfo[chnn].lBitStream != m_secondStreamEncodeInfo[chnn].hBitStream) || (m_chnnVideoInfo[chnn].quality != m_secondStreamEncodeInfo[chnn].quality))
			{
				m_chnnVideoInfo[chnn].rate = mobileRate;
				m_chnnVideoInfo[chnn].encodeType = m_secondStreamEncodeInfo[chnn].encodeType;
				m_chnnVideoInfo[chnn].lBitStream = m_secondStreamEncodeInfo[chnn].hBitStream;
				m_chnnVideoInfo[chnn].quality	 = m_secondStreamEncodeInfo[chnn].quality;

				CLocalDevice::Instance()->SetFrameRateQuality(  chnn, mobileRate, static_cast<VIDEO_ENCODE_MODE>(m_secondStreamEncodeInfo[chnn].encodeType),\
					m_secondStreamEncodeInfo[chnn].hBitStream, m_secondStreamEncodeInfo[chnn].quality, false);
			}
			printf("%s,%d,chnn:%d,rat:%d,encode:%d,stream:%d,quality:%d,resolution:%d\n",\
				__FILE__,__LINE__,chnn, mobileRate, static_cast<VIDEO_ENCODE_MODE>(m_secondStreamEncodeInfo[chnn].encodeType),\
				m_secondStreamEncodeInfo[chnn].hBitStream, m_secondStreamEncodeInfo[chnn].quality, DVRVIDEO_SIZE_CIF);
		}
		else if ( ((netChnn >> chnn) & 0x01) || ((mobileHigh >> chnn) & 0x01) )
		{
			if (1 == netIeNum)//如果只有 1路IE ，配置的是什么就给什么分辨率（最大为 1080p）
			{
				videoSize = m_secondStreamEncodeInfo[chnn].resolution;
				rate = m_secondStreamEncodeInfo[chnn].rate;
				bitStream = m_secondStreamEncodeInfo[chnn].hBitStream;
			}
			else if((1 < netIeNum) && (netIeNum <= 4))   //如果  2-->4 路IE端 （最大只能 配置 4路 720p）
			{
				if (DVRVIDEO_SIZE_HD1080 != m_secondStreamEncodeInfo[chnn].resolution)
				{
					videoSize = m_secondStreamEncodeInfo[chnn].resolution;
				}
				else
				{
					videoSize = DVRVIDEO_SIZE_HD720;
				}

				rate = m_secondStreamEncodeInfo[chnn].rate;
				bitStream = m_secondStreamEncodeInfo[chnn].hBitStream;
			}
			else if ((4 < netIeNum) && (netIeNum <= m_chnnNum)) // 5-->all 路 （最大只能 配置 D1）
			{
				if (m_secondStreamEncodeInfo[chnn].resolution > DVRVIDEO_SIZE_D1)
				{
					videoSize = DVRVIDEO_SIZE_D1;
				}
				else
				{
					videoSize = m_secondStreamEncodeInfo[chnn].resolution;
				}

				rate = m_secondStreamEncodeInfo[chnn].rate;
				if (1024 < m_secondStreamEncodeInfo[chnn].hBitStream)
				{
					bitStream = 1024;
				}
				else
				{
					bitStream = m_secondStreamEncodeInfo[chnn].hBitStream;
				}
			}
			else// 大于 ALL 分辨率 DVRVIDEO_SIZE_D1,按照以下方式降帧率
			{
				if (m_secondStreamEncodeInfo[chnn].resolution > DVRVIDEO_SIZE_D1)
				{
					videoSize = DVRVIDEO_SIZE_D1;
				}
				else
				{
					videoSize= m_secondStreamEncodeInfo[chnn].resolution;
				}

				rateTemp = m_autoAllotChnnCount / netIeNum;
				if (rateTemp < m_secondStreamEncodeInfo[chnn].rate)	//降帧
				{
					rate = rateTemp;
				}
				else
				{
					rate = m_secondStreamEncodeInfo[chnn].rate;
				}

				if (1024 > m_secondStreamEncodeInfo[chnn].hBitStream)
				{
					bitStream = 1024;
				}
				else
				{
					bitStream = m_secondStreamEncodeInfo[chnn].hBitStream;
				}
			}

			if (videoSize != m_chnnVideoInfo[chnn].videoSize)
			{
				CLocalDevice::Instance()->SetVideoSize(chnn, videoSize, false);
			}

			CLocalDevice::Instance()->SetFrameRateQuality(chnn, rate, static_cast<VIDEO_ENCODE_MODE>(m_secondStreamEncodeInfo[chnn].encodeType), bitStream, m_secondStreamEncodeInfo[chnn].quality, false);

			m_chnnVideoInfo[chnn].videoSize	= videoSize;
			m_chnnVideoInfo[chnn].rate		= rate;
			m_chnnVideoInfo[chnn].lBitStream = bitStream;
			m_chnnVideoInfo[chnn].encodeType = m_secondStreamEncodeInfo[chnn].encodeType;
			m_chnnVideoInfo[chnn].quality = m_secondStreamEncodeInfo[chnn].quality;
			m_chnnVideoInfo[chnn].chnn = chnn;

			/*
			printf("%s,%d,chnn:%d,rat:%d,encode:%d,stream:%d,quality:%d,resolution:%d,netIeNum:%d\n",\
				__FILE__,__LINE__,chnn, rate, static_cast<VIDEO_ENCODE_MODE>(m_secondStreamEncodeInfo[chnn].encodeType),\
				bitStream, m_secondStreamEncodeInfo[chnn].quality, videoSize,netIeNum);
			*/
		}
		else  //不连的时候给 分辨率：CIF   帧率： 1帧  
		{
			//默认分辨率为CIF 
			videoSize = DVRVIDEO_SIZE_CIF;
			rate = 1;
			bitStream = 128;

			if (videoSize != m_chnnVideoInfo[chnn].videoSize)
			{
				CLocalDevice::Instance()->SetVideoSize(chnn, videoSize, false);
			}

			if ((rate != m_chnnVideoInfo[chnn].rate) || (bitStream != m_chnnVideoInfo[chnn].lBitStream))
			{
				CLocalDevice::Instance()->SetFrameRateQuality(chnn, rate, static_cast<VIDEO_ENCODE_MODE>(m_secondStreamEncodeInfo[chnn].encodeType), bitStream, m_secondStreamEncodeInfo[chnn].quality, false);
			}

			m_chnnVideoInfo[chnn].videoSize	= videoSize;
			m_chnnVideoInfo[chnn].rate		= rate;
			m_chnnVideoInfo[chnn].lBitStream = bitStream;
			m_chnnVideoInfo[chnn].encodeType = m_secondStreamEncodeInfo[chnn].encodeType;
			m_chnnVideoInfo[chnn].quality = m_secondStreamEncodeInfo[chnn].quality;
			m_chnnVideoInfo[chnn].chnn = chnn;

			/*
			printf("%s,%d,chnn:%d,rat:%d,encode:%d,stream:%d,quality:%d,resolution:%d\n",\
				__FILE__,__LINE__,chnn, rate, static_cast<VIDEO_ENCODE_MODE>(m_secondStreamEncodeInfo[chnn].encodeType),\
				bitStream, m_secondStreamEncodeInfo[chnn].quality, DVRVIDEO_SIZE_CIF);
			*/
		}
	}

	return true;
}

bool CLocalSubStreamAllot::SupportProduct()
{
	return  (/*(TD_2704AS_SL == m_productType) || (TD_2708AS_SL == m_productType) || (TD_2716AS_SL == m_productType) ||
			(TD_2704AS_SL_A == m_productType) || (TD_2708AS_SL_A == m_productType) || (TD_2716AS_SL_A == m_productType) ||
			(TD_2704AE_SL == m_productType) || (TD_2708AE_SL == m_productType) || (TD_2716AE_SL == m_productType) ||
			(TD_2704AS_S == m_productType)	|| (TD_2708AS_S == m_productType)  || (TD_2716AS_S == m_productType)  ||
			(TD_2704AS_S_A == m_productType)|| (TD_2708AS_S_A == m_productType)  || (TD_2716AS_S_A == m_productType)  ||*/
			(TD_2704TS_PL == m_productID) || (TD_2708TS_PL == m_productID) || (TD_2716TS_PL == m_productID) ||
			/*(TD_2704TD_PL == m_productType) || (TD_2708TD_PL == m_productType) || */(TD_2716TD_PL == m_productID) ||
			(TD_2732TD_PL == m_productID) ||
			(TD_2704TD_P == m_productID)  || (TD_2708TD_P == m_productID)  || (TD_2716TD_P == m_productID)  ||\
			/*(TD_2704TE_M == m_productType)  || (TD_2708TE_M == m_productType)  || (TD_2716TE_M == m_productType)  ||*/
			(TD_2704TS_S == m_productID)  || (TD_2708TS_S == m_productID)  || (TD_2716TS_S == m_productID)  ||\
			/*(TD_2004AL == m_productType) || (TD_2008AS == m_productType)*/
			(TD_2704AS_PL == m_productID) || (TD_2708AS_PL == m_productID) || (TD_2716AS_PL == m_productID) ||\
			(TD_2704AE_PL == m_productID) || (TD_2708AE_PL == m_productID) || (TD_2716AE_PL == m_productID) );
}

void CLocalSubStreamAllot::SetChnnEncodeInfo(unsigned long chnn, const ENCODE_INFO &encodeInfo)
{
	m_secondStreamEncodeInfo[chnn] = encodeInfo;
	m_secondStreamEncodeInfoOriginal[chnn].rate = 0;
}

void CLocalSubStreamAllot::GetUsedNetVideoSize(unsigned long &size)
{
	if (SupportProduct())
	{
		size = DVRVIDEO_SIZE_CIF;
	}
}