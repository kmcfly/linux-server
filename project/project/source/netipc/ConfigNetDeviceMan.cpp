#include "ConfigNetDeviceMan.h"
#include "NetDevicePlatform.h"
#include "NetDeviceManager.h"
#include "MessageMan.h"
#include "LocalDevice.h"
//#include "StreamProc.h"

#include "PUB_common.h"

#include <vector>
#include <algorithm>

#define DD_CONFIG_NUM_MAX				128
extern VIDEO_FORMAT	g_video_format;
//extern CStreamProc * g_pStreamProc;

CConfigNetDeviceMan* CConfigNetDeviceMan::Instance()
{
	static CConfigNetDeviceMan s_cfg_nd_man;
	return &s_cfg_nd_man;
}

CConfigNetDeviceMan::CConfigNetDeviceMan()
{
	m_pConfigEx = NULL;
	m_pConfigParserV3 = NULL;
	for(long netCnt=0;netCnt<CProduct::Instance()->NetVideoInputNum();netCnt++)
	{
		m_pNvrCfg[netCnt] = NULL;
	}
	
	memset(m_cfgLen, 0, sizeof(m_cfgLen));

	m_bRefreshData = true;

	PUB_InitLock(&m_lock_t);
}

CConfigNetDeviceMan::~CConfigNetDeviceMan()
{
	PUB_DestroyLock(&m_lock_t);
}

bool CConfigNetDeviceMan::Initial(CConfigEx* pConfigEx,
								  long videoInputNum,
								  long netVideoNum,
								  long sensorOutputNum,
								  long audioInputNum,
								  long localSensorInputNum)
{
	assert(pConfigEx);

	m_pConfigEx = pConfigEx;

	NET_SDK_DEVICEINFO netSdkDev;
	memset(&netSdkDev, 0, sizeof(NET_SDK_DEVICEINFO));

	netSdkDev.videoInputNum = videoInputNum;
	netSdkDev.netVideoNum = netVideoNum;
	netSdkDev.sensorOutputNum = sensorOutputNum;
	netSdkDev.audioInputNum = audioInputNum;
	netSdkDev.sensorInputNum = localSensorInputNum;

	m_pConfigParserV3 = new CConfigParser_V3();

	m_pConfigParserV3->SetVideoInputNum(videoInputNum - netVideoNum, netVideoNum);
	m_pConfigParserV3->SetSensorInNum(localSensorInputNum);
	m_pConfigParserV3->SetDeivceInfo(&netSdkDev);

	m_videoInputNum = videoInputNum;
	m_netVideoNum = netVideoNum;
	m_sensorOutputNum = sensorOutputNum;
	m_audioInputNum = audioInputNum;
//	m_sensorInputNum = sensorInputNum;

	m_bRefreshData = true;

	return true;
}

void CConfigNetDeviceMan::Quit()
{
	m_pConfigEx = NULL;

	if(m_pConfigParserV3)
	{
		delete m_pConfigParserV3;
		m_pConfigParserV3 = NULL;
	}

	
	for(long netCnt=0;netCnt<CProduct::Instance()->NetVideoInputNum();netCnt++)
	{
		if(m_pNvrCfg[netCnt])
		{
			delete [] m_pNvrCfg[netCnt];
			m_pNvrCfg[netCnt] = NULL;
		}
	}

	m_ipDeviceInfo.clear();

	memset(m_cfgLen, 0, sizeof(m_cfgLen));
}

void CConfigNetDeviceMan::SetNetConfig(CNetDeviceManager* pNetDeviceMan, CNetDevice* pNetDeviceOne, const unsigned char* pCfgData, unsigned long dataLen)
{
	/*
		pNetDeviceOne = NULL 时, 设置所有
	*/

	assert (NULL != pCfgData);
	assert (dataLen > 0);

	if(!pNetDeviceOne && pNetDeviceMan->GetActiveCount(true) <= 0)
	{
		return;
	}

	unsigned long cfgDataLen = 0;

	const NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<const NCFG_BLOCK_HEAD *>(pCfgData);
	nd_assert((pBlockHead->ItemNum > 0), printf("(%s.%d)pBlockHead->ItemNum = %d.\n", __FILE__, __LINE__, pBlockHead->ItemNum));
	cfgDataLen += sizeof(NCFG_BLOCK_HEAD);

	const NCFG_ITEM_HEAD *pItemHead = reinterpret_cast<const NCFG_ITEM_HEAD *>(pCfgData + sizeof(NCFG_BLOCK_HEAD));
	cfgDataLen += (pBlockHead->ItemNum * sizeof(NCFG_ITEM_HEAD));

	std::vector<SDK_ITEM_DATA> sdkItemVec;

	//缓存空间
	unsigned char* pTmpCfgBuff = new unsigned char[dataLen];
	//assert(0 != dataLen);
	unsigned long tmpCfgDataLen = 0;

	//统计有哪些配置项改变
	nd_assert((cfgDataLen <= dataLen), printf("(%s.%d)cfgDataLen = %d. dataLen = %d.\n", __FILE__, __LINE__, cfgDataLen, dataLen));
	for (unsigned short i = 0; i < pBlockHead->ItemNum; i++)
	{
		//拼装单项
		NCFG_BLOCK_HEAD* pTmpBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD*>(pTmpCfgBuff);
		NCFG_ITEM_HEAD* pTmpItemHead = reinterpret_cast<NCFG_ITEM_HEAD*>(pTmpCfgBuff + sizeof(NCFG_BLOCK_HEAD));

		pTmpBlockHead->biSize = sizeof(NCFG_BLOCK_HEAD);
		pTmpBlockHead->ItemNum = 1;
		pTmpBlockHead->netcfgver = NCFG_VERSION;
		memcpy(pTmpItemHead, &(pItemHead[i]), sizeof(NCFG_ITEM_HEAD));
		memcpy(pTmpCfgBuff + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD), pCfgData + cfgDataLen, pItemHead[i].len);
		tmpCfgDataLen = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + pItemHead[i].len;

		//使用单项查找SDK对应项
		DD_CONFIG_ITEM_HEAD ddCfgItemHead;
		memset(&ddCfgItemHead, 0, sizeof(DD_CONFIG_ITEM_HEAD));
		unsigned char* pTmpSdkBuff = NULL;
		long tmpSdkDataLen = 0;

		m_pConfigParserV3->CfgDevice2CfgSDK(pTmpCfgBuff, tmpCfgDataLen, pTmpSdkBuff, tmpSdkDataLen, &ddCfgItemHead);
		if(ddCfgItemHead.len > 0)
		{
			bool bFindItem = false;
			std::vector<SDK_ITEM_DATA>::iterator vecIter;
			for(vecIter = sdkItemVec.begin(); vecIter != sdkItemVec.end(); vecIter++)
			{
				if(vecIter->itemID == ddCfgItemHead.itemID)
				{
					bFindItem = true;
				}
			}

			if(!bFindItem)
			{
				SDK_ITEM_DATA sdkItem;
				memset(&sdkItem, 0, sizeof(SDK_ITEM_DATA));

				sdkItem.itemID = ddCfgItemHead.itemID;
				sdkItemVec.push_back(sdkItem);
			}
		}

		cfgDataLen += pItemHead[i].len;
	}

	
	if(sdkItemVec.size() > 0)
	{
		long cfgParamLenMax = DD_CONFIG_NUM_MAX * sizeof(NCFG_ITEM_HEAD);
		unsigned char* pCfgParamBuff = new unsigned char[cfgParamLenMax];

		std::vector<SDK_ITEM_DATA>::iterator vecIter;
		for(vecIter = sdkItemVec.begin(); vecIter != sdkItemVec.end(); vecIter++)
		{
			long itemLen = m_pConfigParserV3->GetCfgRequestParam(static_cast<DD_CONFIG_ITEM_ID>(vecIter->itemID), pCfgParamBuff, cfgParamLenMax);
			
			unsigned char* pCfgBlock = NULL;
			unsigned long cfgBlockLen = 0;
			if((itemLen > 0) && m_pConfigEx->GetConfig(pCfgParamBuff, itemLen, &pCfgBlock, cfgBlockLen))
			{
				if(pCfgBlock && (cfgBlockLen > 0))
				{
					unsigned char* pTmpSdkBuff = NULL;
					long tmpSdkDataLen = 0;

					SubstituteItemData(NCFG_ITEM_ENCODE_MAJOR_EX, pCfgData, pCfgBlock);
					SubstituteItemData(NCFG_ITEM_ENCODE_MINOR_EX, pCfgData, pCfgBlock);

					long retLen = m_pConfigParserV3->CfgDevice2CfgSDK(pCfgBlock, cfgBlockLen, pTmpSdkBuff, tmpSdkDataLen, NULL);
					
					DD_CONFIG_ITEM_HEAD* pDDcfgHead = reinterpret_cast<DD_CONFIG_ITEM_HEAD*>(pTmpSdkBuff);
					if(pDDcfgHead->len > 0)
					{
						//只有数据的才有意义
						vecIter->useLen = tmpSdkDataLen;
						vecIter->buffLen = tmpSdkDataLen;
						vecIter->pData = pTmpSdkBuff;
					}
					else
					{
						delete [] pTmpSdkBuff;
						pTmpSdkBuff = NULL;
						tmpSdkDataLen = 0;
					}
				}
			}

			if(pCfgBlock)
			{
				delete [] pCfgBlock;
				pCfgBlock = NULL;
			}
		}

		std::vector<CNetDevice*> waitList;

		//刷数据到网络设备
		if(pNetDeviceOne)
		{
			bool bSetConfig = false;

			for(vecIter = sdkItemVec.begin(); vecIter != sdkItemVec.end(); vecIter++)
			{
				if(vecIter->useLen > 0)
				{
					bSetConfig = true;

					pNetDeviceOne->SetConfig(0, vecIter->itemID, vecIter->pData + sizeof(DD_CONFIG_ITEM_HEAD), vecIter->useLen - sizeof(DD_CONFIG_ITEM_HEAD), false);
#if 0
					printf("(%s.%d) Set net device cfg. sdk id = %d, data len = %d.\n", __FILE__, __LINE__, vecIter->itemID, vecIter->useLen);
#endif
				}
			}

			if(bSetConfig)
			{
				pNetDeviceOne->FlushConfig();
				waitList.push_back(pNetDeviceOne);
			}
		}
		else
		{
			for(int i = (m_videoInputNum - m_netVideoNum); i < m_videoInputNum; i++)
			{
				CNetDevice* pNetDevice = pNetDeviceMan->GetNetDeviceByChnn(i, true);

				if(pNetDevice)
				{
					if ( 0 == pNetDevice->GetLocalVideoNum())
					{
						pNetDevice->SetLocalVideoNum(m_videoInputNum - m_netVideoNum);
					}

					bool bSetConfig = false;
					bool bConvert   = false;

					for(vecIter = sdkItemVec.begin(); vecIter != sdkItemVec.end(); vecIter++)
					{
						if(vecIter->useLen > 0)
						{
							bSetConfig = true;
							//防止网络通道发送将一个通道的修改发送到其他个通道
							if ((DD_CONFIG_ITEM_PTZ_CRUISE <= vecIter->itemID)&&(DD_CONFIG_ITEM_PTZ_CRUISE_END > vecIter->itemID))
							{
								if ( i == vecIter->itemID - DD_CONFIG_ITEM_PTZ_CRUISE )
								{
									pNetDevice->SetConfig(0, vecIter->itemID, vecIter->pData + sizeof(DD_CONFIG_ITEM_HEAD), vecIter->useLen - sizeof(DD_CONFIG_ITEM_HEAD), false);
									bConvert = true;
								}
								else
								{
									bSetConfig = false;
								}
							}
							else
							{
								pNetDevice->SetConfig(0, vecIter->itemID, vecIter->pData + sizeof(DD_CONFIG_ITEM_HEAD), vecIter->useLen - sizeof(DD_CONFIG_ITEM_HEAD), false);
							}
							
#if 0
							printf("(%s.%d) Set net device cfg. sdk id = %d, data len = %d.\n", __FILE__, __LINE__, vecIter->itemID, vecIter->useLen);
#endif
						}
					}

					if(bSetConfig || bConvert)
					{
						pNetDevice->FlushConfig();
						waitList.push_back(pNetDevice);
					}
				}
			}
		}

		//等待...
		std::vector<CNetDevice*>::iterator waitIter;

		unsigned long startWaitTime = GetCurrTime32();
		while(true)
		{
			for(waitIter = waitList.begin(); waitIter != waitList.end();)
			{
				if( ((*waitIter)->IsReady() <= ND_SC_BUSY) \
					|| ((GetCurrTime32() - startWaitTime) >= 3))//设定超时时间为3秒
				{
					waitIter = waitList.erase(waitIter);
				}
				else
				{
					waitIter++;
				}
			}

			if(waitList.empty())
			{
				break;
			}

			PUB_Sleep(100);
		}

		//清理数据
		delete [] pCfgParamBuff;

		for(vecIter = sdkItemVec.begin(); vecIter != sdkItemVec.end(); vecIter++)
		{
			if(vecIter->pData)
			{
				delete [] (vecIter->pData);
			}
		}
		sdkItemVec.clear();
	}

	delete [] pTmpCfgBuff;
}

bool CConfigNetDeviceMan::TestUpdataInitData(const unsigned char* pCfgData, unsigned long dataLen)
{
	CPUB_LockAction lock(&m_lock_t);

	m_bRefreshData = true;

	return m_bRefreshData;
}

bool CConfigNetDeviceMan::UpdateInitData(long channl,CNetDevice* pNetDeviceOne)
{
	//检查
	CPUB_LockAction lock(&m_lock_t);

	if(!m_bRefreshData && (m_cfgLen[channl] > 0))
	{
		//已有数据无需重读
		return true;
	}

	//填充请求配置头
	CFG_ITEM_VEC itemVec;
	GetInitialItemId(itemVec);
	assert(itemVec.size() > 0);

	unsigned long requestLen = itemVec.size() * sizeof(NCFG_ITEM_HEAD);
	unsigned char* pRequestBuff = new unsigned char[requestLen];
	memset(pRequestBuff, 0, sizeof(unsigned char) * requestLen);

	NCFG_ITEM_HEAD* pRequestItem = reinterpret_cast<NCFG_ITEM_HEAD*>(pRequestBuff);
	
	for(CFG_ITEM_VEC::iterator iter = itemVec.begin(); iter != itemVec.end(); iter++)
	{
		pRequestItem->itemID = (*iter);
		pRequestItem++;
	}

	if(m_pNvrCfg[channl])
	{
		delete [] m_pNvrCfg[channl];
		m_pNvrCfg[channl] = NULL;
	}
	m_cfgLen[channl] = 0;

	m_pConfigEx->GetConfig(pRequestBuff, requestLen, &m_pNvrCfg[channl], m_cfgLen[channl]);

	NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(m_pNvrCfg[channl]);
	NCFG_ITEM_HEAD *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(m_pNvrCfg[channl]+sizeof(NCFG_BLOCK_HEAD));

	ENCODE_INFO_EX *pEncodeEx = NULL;
	NCFG_IPC_SUPPORT_ENCODE *pSupportEncode = NULL;
	unsigned char *pBuf = m_pNvrCfg[channl] + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * pBlockHead->ItemNum;

	for(unsigned long item = 0; item < pBlockHead->ItemNum; item++)
	{
// 		if(NCFG_ITEM_ENCODE_MAJOR_EX == pItemHead[item].itemID)
// 		{
// 			pEncodeEx = reinterpret_cast<ENCODE_INFO_EX *>(pBuf);
// 			ncfgItemHeadMajorEx = pItemHead[item];
// 		}
		if(NCFG_ITEM_IPC_SUPPORT_ENCODE == pItemHead[item].itemID)
		{
			pSupportEncode = reinterpret_cast<NCFG_IPC_SUPPORT_ENCODE *>(pBuf);

			if(VIDEO_FORMAT_NTSC == g_video_format)
			{
#ifndef __CHIP3520__
				pSupportEncode->defaChnnRate = pSupportEncode->defaChnnRate * 6 / 5;
				pSupportEncode->maxChnnRate = pSupportEncode->maxChnnRate * 6 / 5;
				pSupportEncode->maxTotalRateOnVGA = pSupportEncode->maxTotalRateOnVGA * 6 / 5;
				pSupportEncode->maxChnnRateOnVGA = pSupportEncode->maxChnnRateOnVGA * 6 / 5;
#endif
			}
		}

		pBuf += pItemHead[item].len;
	}

	delete [] pRequestBuff;

	return (m_cfgLen[channl] > 0);
}

//设置初始化的配置信息
bool CConfigNetDeviceMan::SetInitNetConfig(CNetDeviceManager* pNetDeviceMan, CNetDevice* pNetDeviceOne,long channl)
{
	if(m_pNvrCfg[channl])
	{
		//初始化配置
		SetNetConfig(pNetDeviceMan, pNetDeviceOne, m_pNvrCfg[channl], m_cfgLen[channl]);

		return true;
	}

	return false;
}

void CConfigNetDeviceMan::SetIPDeviceInfo(IP_DEVICE_INFO *pIPDeviceInfo, long num)
{
	m_ipDeviceInfo.clear();
	for(int i = 0; i < num; i++)
	{
		m_ipDeviceInfo.push_back(pIPDeviceInfo[i]);
	}
}

void CConfigNetDeviceMan::GetNetDeviceInfo(IP_DEVICE_INFO *pIPDeviceInfo, int  &num)
{
	num = 0;
	for(int i = 0; i < m_ipDeviceInfo.size(); i++)
	{
		IP_DEVICE_INFO &ipDeviceInfo = m_ipDeviceInfo[i];
		pIPDeviceInfo[i] = ipDeviceInfo;
		num++;
	}
}

void CConfigNetDeviceMan::GetInitialItemId(CFG_ITEM_VEC& itemVec)
{
	//读取初始化时需要获取的配置

	//
	itemVec.push_back(NCFG_ITEM_VIDEO_FORMAT);
	itemVec.push_back(NCFG_ITEM_DATE_FORMAT);
	itemVec.push_back(NCFG_ITEM_TIME_ZONE);
	itemVec.push_back(NCFG_ITEM_DAYLIGHT_INFO);

	itemVec.push_back(NCFG_ITEM_PTZ_SERIAL);
	itemVec.push_back(NCFG_ITEM_VIDEO_COLOR);
	itemVec.push_back(NCFG_ITEM_TIME_STAMP_INFO);
	itemVec.push_back(NCFG_ITEM_CAM_NAME_STAMP_INFO);
	itemVec.push_back(NCFG_ITEM_CAM_COVER_INFO);

	itemVec.push_back(NCFG_ITEM_JPEG_SIZE);
	itemVec.push_back(NCFG_ITEM_ENCODE_MAJOR_EX);
	itemVec.push_back(NCFG_ITEM_IPC_SUPPORT_ENCODE);

	itemVec.push_back(NCFG_ITEM_SENSOR_IN_SCH_WEEK);
	itemVec.push_back(NCFG_ITEM_ALARMOUT_SCH_WEEK);
	itemVec.push_back(NCFG_ITEM_MOTION_SCH_WEEK);

	itemVec.push_back(NCFG_ITEM_SENSOR_IN_ENABLE);
	itemVec.push_back(NCFG_ITEM_SENSOR_IN_TYPE);
	itemVec.push_back(NCFG_ITEM_SENSOR_IN_HOLD_TIME);

	itemVec.push_back(NCFG_ITEM_MOTION_ENABLE);
	itemVec.push_back(NCFG_ITEM_MOTION_HOLD_TIME);
}

void CConfigNetDeviceMan::SubstituteItemData(NCFG_ITEM_ID itemID, const unsigned char *pSrcCfgData, unsigned char *pDstCfgData)
{
	const unsigned char *pSrcItemData = NULL;
	unsigned char *pDstItemData = NULL;
	unsigned long srcItemDataLen = 0;
	unsigned long dstItemDataLen = 0;

	//查找源配置项的数据地址
	{
		NCFG_BLOCK_HEAD *pBlockHead = (NCFG_BLOCK_HEAD *)pSrcCfgData;
		NCFG_ITEM_HEAD *pItemHead = (NCFG_ITEM_HEAD *)(pSrcCfgData+sizeof(NCFG_BLOCK_HEAD));
		const unsigned char *pBuf = pSrcCfgData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD)*pBlockHead->ItemNum;
		for(unsigned long item = 0; item < pBlockHead->ItemNum; item++)
		{
			if(itemID == pItemHead[item].itemID)
			{
				pSrcItemData = pBuf;
				srcItemDataLen = pItemHead[item].len;
				break;
			}
			pBuf += pItemHead[item].len;
		}
	}

	//查找目的配置项的数据地址
	{
		NCFG_BLOCK_HEAD *pBlockHead = (NCFG_BLOCK_HEAD *)pDstCfgData;
		NCFG_ITEM_HEAD *pItemHead = (NCFG_ITEM_HEAD *)(pDstCfgData+sizeof(NCFG_BLOCK_HEAD));
		unsigned char *pBuf = pDstCfgData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD)*pBlockHead->ItemNum;
		for(unsigned long item = 0; item < pBlockHead->ItemNum; item++)
		{
			if(itemID == pItemHead[item].itemID)
			{
				pDstItemData = pBuf;
				dstItemDataLen = pItemHead[item].len;
				break;
			}
			pBuf += pItemHead[item].len;
		}
	}
	
	if((NULL != pSrcItemData) && (NULL != pDstItemData))
	{
		assert(srcItemDataLen == dstItemDataLen);
		memcpy(pDstItemData, pSrcItemData, srcItemDataLen);
	}
}

//end
