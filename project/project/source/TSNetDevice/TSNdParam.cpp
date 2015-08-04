/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2011-04-12
** Name         : TSNdParam.cpp
** Version      : 1.0
** Description  : IPC状态参数
** Modify Record:
1:创建
***********************************************************************/

#include "TSNdParam.h"
#include "ipc_v3/platform_ipc_v3.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define ND_STEP_CFG_CMD_LEN_MAX			1024 * 4

CTSNdParam::CTSNdParam()
{
	m_p_cfg_cmd_buff = NULL;
	m_cfg_cmd_len = 0;
	m_cfg_cmd_len_max = 0;

	memset(&m_dev_info, 0, sizeof(m_dev_info));

	m_streamNum = 0;
	m_NvrShowStreamNum = 0;

	m_motion_x = 0;
	m_motion_y = 0;
	m_motion_cx = 0;
	m_motion_cy = 0;
}

CTSNdParam::~CTSNdParam()
{

}

//////////////////////////////////////////////////////////////////////////

bool CTSNdParam::Initial()
{
	assert(!m_p_cfg_cmd_buff);

	m_cfg_cmd_len = 0;
	m_cfg_cmd_len_max = ND_STEP_CFG_CMD_LEN_MAX;
	m_p_cfg_cmd_buff = new unsigned char[m_cfg_cmd_len_max];

	memset(&m_dev_info, 0, sizeof(m_dev_info));
	m_dev_info[0].chnn = 1;
	m_dev_info[0].videoSize = DD_VIDEO_SIZE_3M | DD_VIDEO_SIZE_1080P | DD_VIDEO_SIZE_720P;
	m_dev_info[0].videoSize |= DD_VIDEO_SIZE_16_9_3M | DD_VIDEO_SIZE_4M | DD_VIDEO_SIZE_5M;
	memset(m_dev_info[0].ucRate, 30, sizeof(m_dev_info[0].ucRate));

	m_streamNum = 0;

	m_motion_x = 0;
	m_motion_y = 0;
	m_motion_cx = 0;
	m_motion_cy = 0;

	m_maxColorValue.brightness = 100;
	m_maxColorValue.contrast = 100;
	m_maxColorValue.saturation = 100;
	m_maxColorValue.hue = 100;

	m_defaColorValue.brightness = 50;
	m_defaColorValue.contrast = 50;
	m_defaColorValue.saturation = 50;
	m_defaColorValue.hue = 50;

	for(int i = 0; i < TS_MAX_STREAM_NUM; i++)
	{
		m_encodeLevel[i] = 2;
		m_IFrameInterval[i] = 100;
	}

	return true;
}

void CTSNdParam::Quit()
{
	m_cfg_cmd_len = 0;
	m_cfg_cmd_len_max = 0;
	if(m_p_cfg_cmd_buff)
	{
		delete [] m_p_cfg_cmd_buff;
		m_p_cfg_cmd_buff = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////

void CTSNdParam::GetDevInfo(long videoIndex, ND_DEV_INFO& devInfo)
{
	memcpy(&devInfo, &m_dev_info[videoIndex], sizeof(ND_DEV_INFO));
}

long CTSNdParam::GetEncodeInfo(char * pVideoEncodeInfo, int iVideoEncodeInfoLen)
{
	int count = m_firstVideoEncodeInfoList.size() + m_secondVideoEncodeInfoList.size() + 2;
	int dataLen = sizeof(CHN_VIDEO_ENCODE_INFO) + sizeof(CHN_VIDEO_ENCODE_INFO_DETAIL) * count;

	if (dataLen > iVideoEncodeInfoLen)
	{
		return -1;
	}

	CHN_VIDEO_ENCODE_INFO * pChnnEncodeInfo = (CHN_VIDEO_ENCODE_INFO *) pVideoEncodeInfo;
	pChnnEncodeInfo->iLength = sizeof(CHN_VIDEO_ENCODE_INFO_DETAIL) * count;
	pChnnEncodeInfo->bValid = 1;
	pChnnEncodeInfo->pVideoEncodeInfo = NULL;

	CHN_VIDEO_ENCODE_INFO_DETAIL * pChnnVideoEncodeDetail = (CHN_VIDEO_ENCODE_INFO_DETAIL *)(pVideoEncodeInfo + sizeof(CHN_VIDEO_ENCODE_INFO));
	for (std::list<CHN_VIDEO_ENCODE_INFO_DETAIL>::iterator iter=m_firstVideoEncodeInfoList.begin(); iter != m_firstVideoEncodeInfoList.end(); iter++)
	{
		* pChnnVideoEncodeDetail = * iter;
		pChnnVideoEncodeDetail++;
	}

	for (std::list<CHN_VIDEO_ENCODE_INFO_DETAIL>::iterator iter=m_secondVideoEncodeInfoList.begin(); iter != m_secondVideoEncodeInfoList.end(); iter++)
	{
		* pChnnVideoEncodeDetail = * iter;
		pChnnVideoEncodeDetail++;
	}

	return dataLen;
}

void CTSNdParam::GetMotionArea(unsigned long& x, unsigned long& y, unsigned long& cx, unsigned long& cy)
{
	x = m_motion_x;
	y = m_motion_y;
	cx = m_motion_cx;
	cy = m_motion_cy;
}

//////////////////////////////////////////////////////////////////////////

bool CTSNdParam::GetInitialBuff(unsigned char*& pBuff, unsigned long& dataLen)
{
	//需要初始化参数信息
//	SetModify(IPC_V3::NCFG_ITEM_SHARPEN);

//	SetModify(IPC_V3::NCFG_ITEM_SENSOR_HOLD_TIME);

	SetModify(IPC_V3::NCFG_ITEM_MOTION_AREA_INFO);

	SetModify(IPC_V3::NCFG_ITEM_ENCODE_I_FRAME_INTERVAL);
//	SetModify(IPC_V3::NCFG_ITEM_ENCODE_ENCODE_LEVEL);

	SetModify(IPC_V3::NCFG_ITEM_DEFAULT_COLOR_BRIGHTNESS);
	SetModify(IPC_V3::NCFG_ITEM_DEFAULT_COLOR_CONTRAST);
	SetModify(IPC_V3::NCFG_ITEM_DEFAULT_COLOR_SATURATION);
	SetModify(IPC_V3::NCFG_ITEM_DEFAULT_COLOR_HUE);

	SetModify(IPC_V3::NCFG_ITEM_ENCODE_SUPPORT_PROPERTY);
	

	SetModify(IPC_V3::NCFG_ITEM_MAX_BRIGHT_VALUE);
	SetModify(IPC_V3::NCFG_ITEM_MAX_CONTRACT_VALUE);
	SetModify(IPC_V3::NCFG_ITEM_MAX_SATURATION_VALUE);
	SetModify(IPC_V3::NCFG_ITEM_MAX_HUE_VALUE);

	SetModify(IPC_V3::NCFG_ITEM_TIME_OSD_EX);
	SetModify(IPC_V3::NCFG_ITEM_DEV_NAME_EX);

	
	//
	if(IsUpdate() && GetBuff(pBuff, dataLen))
	{
		return true;
	}

	return false;
}


void CTSNdParam::SetModify(unsigned long itemID)
{
	//设置哪些item项, 有变化
	m_modify_item_vec.push_back(itemID);
}

bool CTSNdParam::IsUpdate()
{
	//检查是否需要重新从IPC读取数据
	m_cfg_cmd_len = 0;

	if(m_modify_item_vec.empty())
	{
		return false;
	}

	unsigned long dataLen = 0;

	std::vector<unsigned long>::iterator iter;
	for(iter = m_modify_item_vec.begin(); iter != m_modify_item_vec.end(); iter++)
	{
		int ipcItemNum = 0;
		const int ipcItemMax = 20;
		IPC_V3::NCFG_ITEM_HEAD ipcItemHead[ipcItemMax];
		memset(&ipcItemHead, 0, sizeof(IPC_V3::NCFG_ITEM_HEAD) * ipcItemMax);
		ipcItemHead[ipcItemNum++].itemID = *iter;

		for(int i = 0; i < ipcItemNum; i++)
		{
			if(ipcItemHead[i].itemID != 0)
			{
				if((dataLen + sizeof(IPC_V3::NCFG_ITEM_HEAD)) > m_cfg_cmd_len_max)
				{
					//缓存不够
					m_cfg_cmd_len_max += ND_STEP_CFG_CMD_LEN_MAX;
					unsigned char* pNewData = new unsigned char[m_cfg_cmd_len_max];

					memcpy(pNewData, m_p_cfg_cmd_buff, dataLen);

					delete [] m_p_cfg_cmd_buff;
					m_p_cfg_cmd_buff = pNewData;
				}

				memcpy(m_p_cfg_cmd_buff + dataLen, &(ipcItemHead[i]), sizeof(IPC_V3::NCFG_ITEM_HEAD));
				dataLen += sizeof(IPC_V3::NCFG_ITEM_HEAD);
			}
		}
	}

	m_cfg_cmd_len = dataLen;
	m_modify_item_vec.clear(); //清除

	if(m_cfg_cmd_len > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CTSNdParam::GetBuff(unsigned char*& pBuff, unsigned long& dataLen)
{
	//需要重新从IPC读取数据的命令Buff

	if(m_cfg_cmd_len > 0)
	{
		pBuff = m_p_cfg_cmd_buff;
		dataLen = m_cfg_cmd_len;

		return true;
	}
	else
	{
		return false;
	}
}

void CTSNdParam::SetParamByCfg(unsigned char* pData, unsigned long dataLen)
{
	//通过IPC的配置, 设置参数

	IPC_V3::NCFG_BLOCK_HEAD* pBlockHead = reinterpret_cast<IPC_V3::NCFG_BLOCK_HEAD*>(pData);
	IPC_V3::NCFG_ITEM_HEAD* pItemHead = reinterpret_cast<IPC_V3::NCFG_ITEM_HEAD*>(pData + sizeof(IPC_V3::NCFG_BLOCK_HEAD));

	unsigned long index = 0;
	unsigned long offset = sizeof(IPC_V3::NCFG_BLOCK_HEAD) + sizeof(IPC_V3::NCFG_ITEM_HEAD) * pBlockHead->ItemNum;
	for(int i = 0; i < pBlockHead->ItemNum; i++)
	{
		unsigned char* pIpcCfg = pData + offset;
		unsigned long ipcCfgLen = pItemHead[i].len;

		switch(pItemHead[i].itemID)
		{
		//case IPC_V3::NCFG_ITEM_SENSOR_HOLD_TIME:
		//	{
		//		assert(ipcCfgLen == sizeof(unsigned long));
		//		unsigned long *pHoldTime = reinterpret_cast<unsigned long *>(pIpcCfg);
		//		printf("holdTIme = %d\n", *pHoldTime);
		//		break;
		//	}
		case IPC_V3::NCFG_ITEM_MOTION_AREA_INFO:
			{
				//不同的分辨率, 区域不同
				assert(ipcCfgLen == sizeof(IPC_V3::NCFG_INFO_MOTION_INFO));

				IPC_V3::NCFG_INFO_MOTION_INFO* pMotion = reinterpret_cast<IPC_V3::NCFG_INFO_MOTION_INFO*>(pIpcCfg);

				m_motion_x = pMotion->xPixel;
				m_motion_y = pMotion->yPixel;
				m_motion_cx = pMotion->gridWidth;
				m_motion_cy = pMotion->gridHeight;
			}
			break;
		case IPC_V3::NCFG_ITEM_ENCODE_I_FRAME_INTERVAL:
			{
				assert((ipcCfgLen % sizeof(unsigned long)) == 0);
				int count = ipcCfgLen / sizeof(unsigned long);
				unsigned long *pInterval = reinterpret_cast<unsigned long*>(pIpcCfg);
				
				for(int i = 0; i < count; i++)
				{
					m_IFrameInterval[i] = pInterval[i];
				}
				m_streamNum = (m_streamNum > 0) ? m_streamNum : count;
			}
			break;
		case IPC_V3::NCFG_ITEM_ENCODE_ENCODE_LEVEL:
			{
				assert((ipcCfgLen % sizeof(unsigned long)) == 0);
				int count = ipcCfgLen / sizeof(unsigned long);
				unsigned long *pLevel = reinterpret_cast<unsigned long*>(pIpcCfg);

				for(int i = 0; i < count; i++)
				{
					m_encodeLevel[i] = pLevel[i];
				}
				m_streamNum = (m_streamNum > 0) ? m_streamNum : count;
			}
			break;
		case IPC_V3::NCFG_ITEM_DEFAULT_COLOR_BRIGHTNESS:
			{
				m_defaColorValue.brightness = *pIpcCfg;
				printf("default bright:%d\n", m_defaColorValue.brightness);
			}
			break;
		case IPC_V3::NCFG_ITEM_DEFAULT_COLOR_CONTRAST:
			{
				m_defaColorValue.contrast = *pIpcCfg;
				printf("default contrast:%d\n", m_defaColorValue.contrast);
			}
			break;
		case IPC_V3::NCFG_ITEM_DEFAULT_COLOR_SATURATION:
			{
				m_defaColorValue.saturation = *pIpcCfg;
				printf("default saturation:%d\n", m_defaColorValue.saturation);
			}
			break;
		case IPC_V3::NCFG_ITEM_DEFAULT_COLOR_HUE:
			{
				m_defaColorValue.hue = *pIpcCfg;
				printf("default hue:%d\n", m_defaColorValue.hue);
			}
			break;
		case IPC_V3::NCFG_ITEM_ENCODE_SUPPORT_PROPERTY:
			{
				assert((ipcCfgLen % sizeof(IPC_V3::NCFG_INFO_SUPPORT_FRAME_RATE)) == 0);
				int count = ipcCfgLen / sizeof(IPC_V3::NCFG_INFO_SUPPORT_FRAME_RATE);
				IPC_V3::NCFG_INFO_SUPPORT_FRAME_RATE* pFrameRate = reinterpret_cast<IPC_V3::NCFG_INFO_SUPPORT_FRAME_RATE*>(pIpcCfg);

				for(long i = 0; i < TS_MAX_STREAM_NUM; i++)
				{
					m_dev_info[i].videoSize = 0;
					memset(&m_dev_info[i].ucRate, 0, sizeof(m_dev_info[i].ucRate));
				}

				m_firstVideoEncodeInfoList.clear();
				m_secondVideoEncodeInfoList.clear();

				int index = 0;
				int cur_nvr_show_stream = 0;

				for(int i = 0; i < count; i++)
				{
					unsigned long resolution = 0;
					char szResolution[20] = {0};

					switch(pFrameRate[i].dwResolution)
					{
					case IPC_V3::DVRVIDEO_SIZE_QCIF:
						resolution = DD_VIDEO_SIZE_QCIF;
						sprintf(szResolution, "176X144");
						index = 0;
						break;
					case IPC_V3::DVRVIDEO_SIZE_CIF:
						resolution = DD_VIDEO_SIZE_CIF;
						sprintf(szResolution,"352X288");
						index = 1;
						break;
					case IPC_V3::DVRVIDEO_SIZE_HD1:
						resolution = DD_VIDEO_SIZE_HD1;
						sprintf(szResolution, "704X288");
						index = 2;
						break;
					case IPC_V3::DVRVIDEO_SIZE_D1:
						resolution = DD_VIDEO_SIZE_D1;
						sprintf(szResolution, "704X576");
						index = 3;
						break;
					case IPC_V3::DVRVIDEO_SIZE_QQVGA:
						resolution = DD_VIDEO_SIZE_QQVGA;
						sprintf(szResolution, "160X120");
						index = 7;
						break;
					case IPC_V3::DVRVIDEO_SIZE_QVGA:
						resolution = DD_VIDEO_SIZE_QVGA;
						sprintf(szResolution, "320X240");
						index = 4;
						break;
					case IPC_V3::DVRVIDEO_SIZE_VGA:
						resolution = DD_VIDEO_SIZE_VGA;
						sprintf(szResolution, "640X480");
						index = 5;
						break;
					//case IPC_V3::DVRVIDEO_SIZE_XVGA:
					//	resolution = DD_VIDEO_SIZE_XVGA;
					//	index = 6;
					//	break;
					case IPC_V3::DVRVIDEO_SIZE_720P:
						resolution = DD_VIDEO_SIZE_720P;
						sprintf(szResolution, "1280X720");
						index = 9;
						break;
					case IPC_V3::DVRVIDEO_SIZE_1080P:
						resolution = DD_VIDEO_SIZE_1080P;
						sprintf(szResolution,"1920X1080");
						index = 10;
						break;


					case IPC_V3::DVRVIDEO_SIZE_XVGA:
						resolution = DD_VIDEO_SIZE_960P;
						sprintf(szResolution, "1280X960");
						index = 13;
						break;
// 					case IPC_V3::DVRVIDEO_SIZE_14M:
// 						resolution = DD_VIDEO_SIZE_SXGA;
// 						sprintf(szResolution, "1280X1024");
// 						index = 12;
// 						break;
					case IPC_V3::DVRVIDEO_SIZE_QXGA:
						resolution = DD_VIDEO_SIZE_3M;
						sprintf(szResolution, "2048X1536");
						index = 15; //DD_VIDEO_SIZE_3M;
						break;
					case IPC_V3::DVRVIDEO_SIZE_16_9_3M:
						resolution = DD_VIDEO_SIZE_16_9_3M;
						sprintf(szResolution, "2304X1296");
						index = 14; //DD_VIDEO_SIZE_16_9_3M;
						break;
					case IPC_V3::DVRVIDEO_SIZE_4M:
						resolution = DD_VIDEO_SIZE_4M;
						sprintf(szResolution, "2688X1520");
						index = 11; //DD_VIDEO_SIZE_4M;
						break;
					case IPC_V3::DVRVIDEO_SIZE_5M:
						resolution = DD_VIDEO_SIZE_5M;
						sprintf(szResolution, "2592X1944");
						index = 6; //DD_VIDEO_SIZE_5M;
						break;

					default:
						break;
					}

					m_dev_info[pFrameRate[i].dwStreamIndex].videoSize |= resolution;
					m_dev_info[pFrameRate[i].dwStreamIndex].ucRate[index] = pFrameRate[i].dwRate;

					CHN_VIDEO_ENCODE_INFO_DETAIL videoEncodeInfo;
					memset(&videoEncodeInfo, 0, sizeof(CHN_VIDEO_ENCODE_INFO_DETAIL));
					videoEncodeInfo.rate = pFrameRate[i].dwRate;

					for(int m = 0; m < 20; m++)
					{
						videoEncodeInfo.lBitRateStream[m] = pFrameRate[i].supportDataRate[m];
						if(0 == pFrameRate[i].supportDataRate[m])
						{
							m_dev_info[pFrameRate[i].dwStreamIndex].maxBitRate[index] = pFrameRate[i].supportDataRate[m-1];
							break;
						}
					}


					videoEncodeInfo.quality = 5;
					videoEncodeInfo.encodeType = 2;
					strncpy((char *)videoEncodeInfo.resolution, szResolution, strlen(szResolution));

					if (0 == pFrameRate[i].dwStreamIndex)
					{
						videoEncodeInfo.stream = 0;
						m_firstVideoEncodeInfoList.push_back(videoEncodeInfo);
					}
					else if (1 == pFrameRate[i].dwStreamIndex)
					{
						videoEncodeInfo.stream = 1;
						m_secondVideoEncodeInfoList.push_back(videoEncodeInfo);
					}
					else
					{
					}

					if( cur_nvr_show_stream < pFrameRate[i].dwStreamIndex )
					{
						cur_nvr_show_stream = pFrameRate[i].dwStreamIndex;
					}
				}

				m_NvrShowStreamNum = cur_nvr_show_stream;
			}
			break;
		case IPC_V3::NCFG_ITEM_MAX_BRIGHT_VALUE:
			{
				m_maxColorValue.brightness = *pIpcCfg;
			}
			break;
		case IPC_V3::NCFG_ITEM_MAX_CONTRACT_VALUE:
			{
				m_maxColorValue.contrast = *pIpcCfg;
			}
			break;
		case IPC_V3::NCFG_ITEM_MAX_SATURATION_VALUE:
			{
				m_maxColorValue.saturation = *pIpcCfg;
			}
			break;
		case IPC_V3::NCFG_ITEM_MAX_HUE_VALUE:
			{
				m_maxColorValue.hue = *pIpcCfg;
			}
			break;
		case IPC_V3::NCFG_ITEM_DATE_FORMAT:
			{
				m_timeStamp.format = *pIpcCfg;
			}
			break;
		case IPC_V3::NCFG_ITEM_TIME_OSD_EX:
			{
				memcpy(&m_timeStamp, pIpcCfg, sizeof(IPC_V3::NCFG_TIME_STAMP_CONFIG));
			}
			break;
		case IPC_V3::NCFG_ITEM_DEV_NAME_EX:
			{
				memcpy(&m_CamName, pIpcCfg, sizeof(IPC_V3::NCFG_INFO_DEV_NAME_EX));
			}
			break;
		default:
			break;
		}

		offset += pItemHead[i].len;
	}
}

void CTSNdParam::SetParamByLoginReply(const IPC_V3::NET_LOGIN_SUCCESS_REPLY &loginReply)
{
	m_ipcDeviceParam = loginReply;
}

void CTSNdParam::GetMaxColorValue(IPC_V3::NET_CHNN_COLOR &maxColorVal)
{
	maxColorVal = m_maxColorValue;
}

void CTSNdParam::GetDefaColorValue(IPC_V3::NET_CHNN_COLOR &defaultColor)
{
	defaultColor = m_defaColorValue;
}

void CTSNdParam::GetTimeStamp(IPC_V3::NCFG_TIME_STAMP_CONFIG &timeStamp)
{
	timeStamp = m_timeStamp;
}

void CTSNdParam::GetCamName(IPC_V3::NCFG_INFO_DEV_NAME_EX &camName)
{
	camName = m_CamName;
}

int CTSNdParam::GetSensorInNum() const
{
	return m_ipcDeviceParam.sensorInNum;
}

unsigned long CTSNdParam::GetEncodeLevel(unsigned long stream)
{
	assert(stream < m_streamNum);
	return m_encodeLevel[stream];
}

unsigned long CTSNdParam::GetIFrameInterval(unsigned long stream)
{
	assert(stream < m_streamNum);
	return m_IFrameInterval[stream];
}

//end
