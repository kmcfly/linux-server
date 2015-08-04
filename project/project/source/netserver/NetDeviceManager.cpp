#include "Typedef.h"
#include "PUB_common.h"
#include "NetDeviceManager.h"
#include "SWL_ConnectProcEx.h"
#include "LocalDevice.h"
#include "IPCDecDisplayMan.h"
#include "MessageMan.h"
#include "NetDevicePlatform.h"
#include "Base64.h"
#include "StreamProc.h"
#include <omp.h>

//#include "ClientRTPMain.h"

#ifdef __ENVIRONMENT_LINUX__
#include "NetInterface.h"
#endif

extern VIDEO_FORMAT g_video_format;
extern CStreamProc * g_pStreamProc;
ULONGLONG g_canSnapPicCH = 0;
extern VIDEO_SIZE_BITRATE_INFO g_vsBitRateInfo[];


#define NDP_INFO_MANUFACTURER		"TongWei"

unsigned long DDVideoSizeToVideoSize(unsigned long ddvideoSize)
{
	long retSize = 0;

	if(ddvideoSize & DD_VIDEO_SIZE_960P)
	{
		retSize |= DVRVIDEO_SIZE_960P;
	}
	if(ddvideoSize & DD_VIDEO_SIZE_SXGA)
	{
		retSize |= DVRVIDEO_SIZE_SXGA;
	}

	if(ddvideoSize & DD_VIDEO_SIZE_3M)
	{
		retSize |= DVRVIDEO_SIZE_3M;
	}

	if(ddvideoSize & DD_VIDEO_SIZE_16_9_3M)
	{
		retSize |= DVRVIDEO_SIZE_16_9_3M;
	}

	if(ddvideoSize & DD_VIDEO_SIZE_4M)
	{
		retSize |= DVRVIDEO_SIZE_4M;
	}

	if(ddvideoSize & DD_VIDEO_SIZE_5M)
	{
		retSize |= DVRVIDEO_SIZE_5M;
	}

	if(ddvideoSize & DD_VIDEO_SIZE_QVGA)
	{
		retSize |= DVRVIDEO_SIZE_QVGA;
	}
	if(ddvideoSize & DD_VIDEO_SIZE_VGA)
	{
		retSize |= DVRVIDEO_SIZE_VGA;
	}
	if(ddvideoSize & DD_VIDEO_SIZE_720P)
	{
		retSize |= DVRVIDEO_SIZE_HD720;
	}
	if(ddvideoSize & DD_VIDEO_SIZE_1080P)
	{
		retSize |= DVRVIDEO_SIZE_HD1080;
	}
	if(ddvideoSize & DD_VIDEO_SIZE_D1)
	{
		retSize |= DVRVIDEO_SIZE_D1;
	}
	if(ddvideoSize & DD_VIDEO_SIZE_CIF)
	{
		retSize |= DVRVIDEO_SIZE_CIF;
	}

	return retSize;
}

unsigned long VideoSizeToDDVideoSize(unsigned long videoSize)
{
	long retSize = 0;

	if(videoSize & DVRVIDEO_SIZE_960P)
	{
		retSize |= DD_VIDEO_SIZE_960P;
	}
	if(videoSize & DVRVIDEO_SIZE_SXGA)
	{
		retSize |= DD_VIDEO_SIZE_SXGA;
	}

	if(videoSize & DVRVIDEO_SIZE_3M)
	{
		retSize |= DD_VIDEO_SIZE_3M;
	}

	if(videoSize & DVRVIDEO_SIZE_16_9_3M)
	{
		retSize |= DD_VIDEO_SIZE_16_9_3M;
	}

	if(videoSize & DVRVIDEO_SIZE_4M)
	{
		retSize |= DD_VIDEO_SIZE_4M;
	}

	if(videoSize & DVRVIDEO_SIZE_5M)
	{
		retSize |= DD_VIDEO_SIZE_5M;
	}


	if(videoSize & DVRVIDEO_SIZE_QVGA)
	{
		retSize |= DD_VIDEO_SIZE_QVGA;
	}
	if(videoSize & DVRVIDEO_SIZE_VGA)
	{
		retSize |= DD_VIDEO_SIZE_VGA;
	}
	if(videoSize & DVRVIDEO_SIZE_HD720)
	{
		retSize |= DD_VIDEO_SIZE_720P;
	}
	if(videoSize & DVRVIDEO_SIZE_HD1080)
	{
		retSize |= DD_VIDEO_SIZE_1080P;
	}
	if(videoSize & DVRVIDEO_SIZE_D1)
	{
		retSize |= DD_VIDEO_SIZE_D1;
	}
	if(videoSize & DVRVIDEO_SIZE_CIF)
	{
		retSize |= DD_VIDEO_SIZE_CIF;
	}

	return retSize;
}



unsigned long GetMaxVideoSize(unsigned long videoSize)
{
	if (DVRVIDEO_SIZE_5M & videoSize)
	{
		return DVRVIDEO_SIZE_5M;
	}

	if (DVRVIDEO_SIZE_4M & videoSize)
	{
		return DVRVIDEO_SIZE_4M;
	}

	if (DVRVIDEO_SIZE_16_9_3M & videoSize)
	{
		return DVRVIDEO_SIZE_16_9_3M;
	}

	if (DVRVIDEO_SIZE_3M & videoSize)
	{
		return DVRVIDEO_SIZE_3M;
	}

	if (DVRVIDEO_SIZE_HD1080 & videoSize)
	{
		return DVRVIDEO_SIZE_HD1080;
	}

	if (DVRVIDEO_SIZE_SXGA & videoSize)
	{
		return DVRVIDEO_SIZE_SXGA;
	}

	if (DVRVIDEO_SIZE_960P & videoSize)
	{
		return DVRVIDEO_SIZE_960P;
	}

	if (DVRVIDEO_SIZE_HD720 & videoSize)
	{
		return DVRVIDEO_SIZE_HD720;
	}

	if (DVRVIDEO_SIZE_960H & videoSize)
	{
		return DVRVIDEO_SIZE_960H;
	}

	if (DVRVIDEO_SIZE_D1 & videoSize)
	{
		return DVRVIDEO_SIZE_D1;
	}

	if (DVRVIDEO_SIZE_VGA & videoSize)
	{
		return DVRVIDEO_SIZE_VGA;
	}

	if (DVRVIDEO_SIZE_HD1 & videoSize)
	{
		return DVRVIDEO_SIZE_HD1;
	}

	if (DVRVIDEO_SIZE_CIF & videoSize)
	{
		return DVRVIDEO_SIZE_CIF;
	}

	if (DVRVIDEO_SIZE_QVGA & videoSize)
	{
		return DVRVIDEO_SIZE_QVGA;
	}

	if (DVRVIDEO_SIZE_QCIF & videoSize)
	{
		return DVRVIDEO_SIZE_QCIF;
	}

	return DVRVIDEO_SIZE_QCIF;
}

bool IsUnderLess3MVideoSize(unsigned long videoSize, unsigned long checkVideoSize)
{
	int src_max_width = 2304;
	int src_max_height = 1296;

	int dst_max_width = 2688;
	int dst_max_height = 1944;

	if (DVRVIDEO_SIZE_16_9_3M & videoSize)
	{
		src_max_width = 2304;
		src_max_height = 1296;
	}
	else if (DVRVIDEO_SIZE_4M & videoSize)
	{
		src_max_width = 2688;
		src_max_height = 1520;
	}
	else if (DVRVIDEO_SIZE_5M & videoSize)
	{
		src_max_width = 2592;
		src_max_height = 1944;
	}


	if (DVRVIDEO_SIZE_16_9_3M & videoSize)
	{
		dst_max_width = 2304;
		dst_max_height = 1296;
	}
	else if (DVRVIDEO_SIZE_3M & videoSize)
	{
		dst_max_width = 2048;
		dst_max_height = 1536;
	}
	else if (DVRVIDEO_SIZE_4M & videoSize)
	{
		dst_max_width = 2688;
		dst_max_height = 1520;
	}
	else if (DVRVIDEO_SIZE_5M & videoSize)
	{
		dst_max_width = 2592;
		dst_max_height = 1944;
	}
	else
	{
		dst_max_width = 1920;
		dst_max_height = 1080;
	}

	if( dst_max_width <= src_max_width && dst_max_height <= src_max_height )
	{
		return true;
	}
	else
	{
		return false;
	}

	return true;
}

bool IsUnderVideoSize(unsigned long videoSize, unsigned long checkVideoSize)
{
	if( (DVRVIDEO_SIZE_16_9_3M & videoSize) || (DVRVIDEO_SIZE_4M & videoSize) || (DVRVIDEO_SIZE_5M & videoSize) )
	{
		return IsUnderLess3MVideoSize( videoSize, checkVideoSize );
	}

	if (DVRVIDEO_SIZE_3M & videoSize)
	{
		return true;
	}

	if (DVRVIDEO_SIZE_HD1080 & videoSize)
	{
		if ((DVRVIDEO_SIZE_3M & checkVideoSize))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	if (DVRVIDEO_SIZE_SXGA & videoSize)
	{
		if ((DVRVIDEO_SIZE_3M & checkVideoSize) || (DVRVIDEO_SIZE_HD1080 & checkVideoSize))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	if (DVRVIDEO_SIZE_960P & videoSize)
	{
		if ((DVRVIDEO_SIZE_3M & checkVideoSize) || (DVRVIDEO_SIZE_HD1080 & checkVideoSize) || (DVRVIDEO_SIZE_SXGA & checkVideoSize))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	if (DVRVIDEO_SIZE_HD720 & videoSize)
	{
		if (  (DVRVIDEO_SIZE_3M & checkVideoSize) || (DVRVIDEO_SIZE_HD1080 & checkVideoSize) || (DVRVIDEO_SIZE_SXGA & checkVideoSize)
			||(DVRVIDEO_SIZE_960P & checkVideoSize))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	if (DVRVIDEO_SIZE_960H & videoSize)
	{
		if (  (DVRVIDEO_SIZE_3M & checkVideoSize)   || (DVRVIDEO_SIZE_HD1080 & checkVideoSize) || (DVRVIDEO_SIZE_SXGA & checkVideoSize)
			||(DVRVIDEO_SIZE_960P & checkVideoSize) || (DVRVIDEO_SIZE_HD720 & checkVideoSize))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	if (DVRVIDEO_SIZE_D1 & videoSize)
	{
		if (  (DVRVIDEO_SIZE_3M & checkVideoSize)   || (DVRVIDEO_SIZE_HD1080 & checkVideoSize) || (DVRVIDEO_SIZE_SXGA & checkVideoSize)
			||(DVRVIDEO_SIZE_960P & checkVideoSize) || (DVRVIDEO_SIZE_HD720 & checkVideoSize)  || (DVRVIDEO_SIZE_960H & checkVideoSize))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	if (DVRVIDEO_SIZE_VGA & videoSize)
	{
		if (   (DVRVIDEO_SIZE_3M & checkVideoSize)   || (DVRVIDEO_SIZE_HD1080 & checkVideoSize) || (DVRVIDEO_SIZE_SXGA & checkVideoSize)
			|| (DVRVIDEO_SIZE_960P & checkVideoSize) || (DVRVIDEO_SIZE_HD720 & checkVideoSize)  || (DVRVIDEO_SIZE_960H & checkVideoSize)
			|| (DVRVIDEO_SIZE_D1 & checkVideoSize))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	if (DVRVIDEO_SIZE_HD1 & videoSize)
	{
		if (  (DVRVIDEO_SIZE_3M & checkVideoSize)   || (DVRVIDEO_SIZE_HD1080 & checkVideoSize) || (DVRVIDEO_SIZE_SXGA & checkVideoSize)
			||(DVRVIDEO_SIZE_960P & checkVideoSize) || (DVRVIDEO_SIZE_HD720 & checkVideoSize)  || (DVRVIDEO_SIZE_960H & checkVideoSize)
			|| (DVRVIDEO_SIZE_D1 & checkVideoSize)   || (DVRVIDEO_SIZE_VGA & checkVideoSize))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	if (DVRVIDEO_SIZE_CIF & videoSize)
	{
		if ((DVRVIDEO_SIZE_3M & checkVideoSize)     || (DVRVIDEO_SIZE_HD1080 & checkVideoSize) || (DVRVIDEO_SIZE_SXGA & checkVideoSize)
			||(DVRVIDEO_SIZE_960P & checkVideoSize) || (DVRVIDEO_SIZE_HD720 & checkVideoSize)  || (DVRVIDEO_SIZE_960H & checkVideoSize)
			|| (DVRVIDEO_SIZE_D1 & checkVideoSize)  || (DVRVIDEO_SIZE_VGA & checkVideoSize)    || (DVRVIDEO_SIZE_HD1 & checkVideoSize))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	if (DVRVIDEO_SIZE_QVGA & videoSize)
	{
		if (   (DVRVIDEO_SIZE_3M & checkVideoSize)   || (DVRVIDEO_SIZE_HD1080 & checkVideoSize) || (DVRVIDEO_SIZE_SXGA & checkVideoSize)
			|| (DVRVIDEO_SIZE_960P & checkVideoSize) || (DVRVIDEO_SIZE_HD720 & checkVideoSize)  || (DVRVIDEO_SIZE_960H & checkVideoSize)
			|| (DVRVIDEO_SIZE_D1 & checkVideoSize)   || (DVRVIDEO_SIZE_VGA & checkVideoSize)    || (DVRVIDEO_SIZE_HD1 & checkVideoSize)
			|| (DVRVIDEO_SIZE_CIF & checkVideoSize))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	if (DVRVIDEO_SIZE_QCIF & videoSize)
	{
		if (   (DVRVIDEO_SIZE_3M & checkVideoSize)	 || (DVRVIDEO_SIZE_HD1080 & checkVideoSize) || (DVRVIDEO_SIZE_SXGA & checkVideoSize)
			|| (DVRVIDEO_SIZE_960P & checkVideoSize) || (DVRVIDEO_SIZE_HD720 & checkVideoSize)  || (DVRVIDEO_SIZE_960H & checkVideoSize)
			|| (DVRVIDEO_SIZE_D1 & checkVideoSize)   || (DVRVIDEO_SIZE_VGA & checkVideoSize)    || (DVRVIDEO_SIZE_HD1 & checkVideoSize)
			|| (DVRVIDEO_SIZE_CIF & checkVideoSize)  || (DVRVIDEO_SIZE_QVGA & checkVideoSize))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}


long CNetDeviceManager::m_nextDeviceId = 0;

CNetDeviceManager::CNetDeviceManager()
{
	m_localVideoInputNum	= -1;
	m_netVideoInputNum		= -1;
	m_videoInputNum			= -1;

	m_pRegDeviceLog			= NULL;
	m_pCfgNetDeviceMan		= NULL;
	m_pNdLibMan				= NULL;
	m_clientID				= INVALID_CLIENT_ID;

	m_pSecondStreamBuffer	= NULL;
	m_pJepgStreamBuffer		= NULL;

	m_pSecondStreamEncode	= NULL;
	m_pDevInfo				= NULL;

	m_pThirdStreamEncode    = NULL;
	m_pThirdDevInfo         = NULL;

	m_pPoeChannelSn			= NULL;

	m_ulLiveDisplayMode     = 0;

	m_bUsedD1ShowImage = false;
	m_ulUsedD1Channel = 0xffff;
	memset(m_bSetSecondCfg, false, sizeof(m_bSetSecondCfg));
	memset(m_bSetThirdCfg, false, sizeof(m_bSetThirdCfg));

	memset( m_resyncTimeCount, 0, sizeof(unsigned long)*g_MaxNetChannCnt);
}

CNetDeviceManager::~CNetDeviceManager()
{

}

/********************************************************************************
*	从CRegNetDeviceLog中提取需要接入的设备，放入列表中
********************************************************************************/
bool CNetDeviceManager::Initial(long localVideoInputNum, long netVideoInputNum, long localSensorInNum)
{
	m_refreshDisplayLock.Lock();
	m_bRefreshDisplay = false;
	m_checkRefreshTime = GetCurrTime32();
	m_refreshDisplayLock.UnLock();
	m_bLinkInfoChanged = false;
	m_bPoeLinkChg = false;
	m_bRunning = false;
	
	m_pRegDeviceLog = CRegNetDeviceLog::Instance();
	m_pCfgNetDeviceMan = CConfigNetDeviceMan::Instance();
	m_pNdLibMan = CNdLibMan::Instance();

	m_localVideoInputNum = localVideoInputNum;
	m_netVideoInputNum = netVideoInputNum;
	m_videoInputNum = m_localVideoInputNum + m_netVideoInputNum;
	m_localSensorInNum = localSensorInNum;
	m_poeInputNum = CProduct::Instance()->PoeInputNum();

	m_pNdLibMan->SetCurDevicePoeChnnNum(m_poeInputNum );

	if(m_poeInputNum > 0)
	{
		NET_DEVICE_LINK_INFO *pDeviceLinkInfo = new NET_DEVICE_LINK_INFO[m_poeInputNum];
		memset(pDeviceLinkInfo, 0, sizeof(NET_DEVICE_LINK_INFO)*m_poeInputNum);

		//第一次创建IPCAM数据库时，首先把POE的设备加入
		if(m_pRegDeviceLog->GetNetDeviceCount() == 0)
		{
			for(unsigned char i = 0; i < m_poeInputNum; i++)
			{
				InitPoeChnnParam( pDeviceLinkInfo[i], i );
			}

			m_pRegDeviceLog->SetNetDevice(pDeviceLinkInfo, m_poeInputNum);
		}
		else
		{
			m_pRegDeviceLog->GetNetDevice(pDeviceLinkInfo,m_poeInputNum);
		}
#ifndef __ENVIRONMENT_WIN32__
		m_poeDeviceMan.Initial(m_poeInputNum);
#endif
		m_pPoeChannelSn = new POE_CHANNEL_SN[m_poeInputNum];
		memset(m_pPoeChannelSn, 0, sizeof(POE_CHANNEL_SN) * m_poeInputNum);
		for (int i = 0; i < m_poeInputNum; i++)
		{
			m_pPoeChannelSn[i].channel = i;
			m_pPoeChannelSn[i].poe_ipc_status = -1;
			m_pPoeChannelSn[i].sn[0] = '\0';
		}

		//根据sn确定图像的位置
		for (int i = 0; i < m_poeInputNum; i++)
		{
			int chnnx = pDeviceLinkInfo[i].channel;

			if( chnnx < m_poeInputNum )
			{
				m_pPoeChannelSn[chnnx].channel = chnnx;
				strcpy(m_pPoeChannelSn[chnnx].sn, pDeviceLinkInfo[i].szID);

				//当前表示是POE通道上接了eth0的IPC
				if(!IsPoeDevice(pDeviceLinkInfo[i]))
				{
					m_pPoeChannelSn[chnnx].poe_ipc_status = -2;
				}
				else
				{
					//如果是POE设备连接，则szID长度肯定对于0
					if( strlen( pDeviceLinkInfo[i].szID ) > 0 )
					{
						m_pPoeChannelSn[chnnx].poe_ipc_status = 0;
					}
					else
					{
						//表示POE通道上没有连接IPC设备
						m_pPoeChannelSn[chnnx].poe_ipc_status = -1;
					}
				}
			}
		}

		m_poeOnvifDeviceMan.SetPoeUsedMode( m_pRegDeviceLog->GetPoeUsedPtoPMode() );
		m_poeOnvifDeviceMan.Initial(m_poeInputNum, m_pPoeChannelSn);
		m_poeOnvifDeviceMan.SetOnlineCallback(OnvifOnlineCallBack, this);
		m_poeOnvifDeviceMan.SetOfflineCallback(OnvifOfflineCallBack, this);
		m_poeOnvifDeviceMan.SetGetIpcPhyChnByMac(GetPhyChannelByMacAddrCallBack, this);
		delete [] pDeviceLinkInfo;
	}

	m_maxSecondStreamCopies = CProduct::Instance()->GetSecondStreamCopies();
	m_secondLimitRateForFirstStreamNum = INVALID_INDEX(unsigned long);

	m_ullSecondCH = 0;

	if(m_dataBufferMan[NET_DEVICE_MAIN_STREAM].Initial(NET_DEVICE_MAIN_STREAM))
	{
		long decChipNum = CLocalDevice::Instance()->GetIPCDecChipNum();
		m_dataBufferMan[NET_DEVICE_MAIN_STREAM].SetDecodeChipNum(decChipNum);

		unsigned char *pBuffer, *pMinorBuffer;
		unsigned long bufferSize, minorBufSize;
		for(long chipId = 0; chipId < decChipNum; chipId++)
		{
			CLocalDevice::Instance()->GetIPCBuffer(chipId, pBuffer, bufferSize, pMinorBuffer, minorBufSize);
			assert(pBuffer != NULL);
			assert(bufferSize > 0);
			memset(pBuffer, 0, bufferSize);
			m_dataBufferMan[NET_DEVICE_MAIN_STREAM].SetChipBuffer(chipId, pBuffer, bufferSize);
		}

		unsigned long *pVideoSize = CProduct::Instance()->GetDigitChannelSupportVideoSize();
		unsigned long *pDDVideoSize = new unsigned long [m_videoInputNum];

		for(long i = m_localVideoInputNum; i < m_videoInputNum; i++)
		{
			pDDVideoSize[i] = VideoSizeToDDVideoSize(pVideoSize[i]);
		}
		m_dataBufferMan[NET_DEVICE_MAIN_STREAM].SetSupportVideoSize(pDDVideoSize, m_videoInputNum);

		delete [] pDDVideoSize;
	}

	if(m_dataBufferMan[NET_DEVICE_SUB_STREAM].Initial(NET_DEVICE_SUB_STREAM))
	{
		m_dataBufferMan[NET_DEVICE_SUB_STREAM].SetDecodeChipNum(1);

		unsigned long bufferSize = m_netVideoInputNum * 1024 * 512;
		m_pSecondStreamBuffer = new unsigned char[bufferSize];
		memset(m_pSecondStreamBuffer, 0, bufferSize);

		m_dataBufferMan[NET_DEVICE_SUB_STREAM].SetChipBuffer(0, m_pSecondStreamBuffer, bufferSize);

		unsigned long *pDDVideoSize = new unsigned long [m_videoInputNum];
		for(long i = m_localVideoInputNum; i < m_videoInputNum; i++)
		{
			pDDVideoSize[i] = DD_VIDEO_SIZE_QVGA|DD_VIDEO_SIZE_VGA|DD_VIDEO_SIZE_CIF|DD_VIDEO_SIZE_D1|DD_VIDEO_SIZE_720P|DD_VIDEO_SIZE_1080P|DD_VIDEO_SIZE_960P|DD_VIDEO_SIZE_SXGA|DD_VIDEO_SIZE_3M;
			pDDVideoSize[i] |= DD_VIDEO_SIZE_QCIF;
			pDDVideoSize[i] |= DD_VIDEO_SIZE_CIF;
			pDDVideoSize[i] |= DD_VIDEO_SIZE_HD1;
			pDDVideoSize[i] |= DD_VIDEO_SIZE_16_9_3M;
			pDDVideoSize[i] |= DD_VIDEO_SIZE_4M;
			pDDVideoSize[i] |= DD_VIDEO_SIZE_5M;
		}
		m_dataBufferMan[NET_DEVICE_SUB_STREAM].SetSupportVideoSize(pDDVideoSize, m_videoInputNum);

		delete [] pDDVideoSize;
	}


	if(CProduct::Instance()->IPCSupportDisplayMinor())
	{
		if(m_dataBufferMan[NET_DEVICE_MINOR_STREAM].Initial(NET_DEVICE_MINOR_STREAM))
		{
			long decChipNum = CLocalDevice::Instance()->GetIPCDecChipNum();
			m_dataBufferMan[NET_DEVICE_MINOR_STREAM].SetDecodeChipNum(decChipNum);

			unsigned char *pBuffer, *pMinorBuffer;
			unsigned long bufferSize, minorBufSize;
			for(long chipId = 0; chipId < decChipNum; chipId++)
			{
				CLocalDevice::Instance()->GetIPCBuffer(chipId, pBuffer, bufferSize, pMinorBuffer, minorBufSize);
				assert(pBuffer != NULL);
				assert(bufferSize > 0);

				memset(pMinorBuffer, 0, minorBufSize);

				m_dataBufferMan[NET_DEVICE_MINOR_STREAM].SetChipBuffer(chipId, pMinorBuffer, minorBufSize);
			}

			unsigned long *pDDVideoSize = new unsigned long [m_videoInputNum];
			for(long i = m_localVideoInputNum; i < m_videoInputNum; i++)
			{
				if( i == m_localVideoInputNum )
				{
					pDDVideoSize[i] = DD_VIDEO_SIZE_QCIF|DD_VIDEO_SIZE_QVGA|DD_VIDEO_SIZE_CIF|DD_VIDEO_SIZE_HD1|DD_VIDEO_SIZE_VGA|DD_VIDEO_SIZE_D1|DD_VIDEO_SIZE_720P|DD_VIDEO_SIZE_960P;
				}
				else
				{
					pDDVideoSize[i] = DD_VIDEO_SIZE_QCIF|DD_VIDEO_SIZE_QVGA|DD_VIDEO_SIZE_CIF|DD_VIDEO_SIZE_HD1|DD_VIDEO_SIZE_VGA|DD_VIDEO_SIZE_D1;
				}
			}
			m_dataBufferMan[NET_DEVICE_MINOR_STREAM].SetSupportVideoSize(pDDVideoSize, m_videoInputNum);

			delete [] pDDVideoSize;
		}
	}

	/***图片流***/
	if (m_dataBufferMan[NET_DEVICE_JPEG_STREAM].Initial(NET_DEVICE_JPEG_STREAM))
	{
		m_dataBufferMan[NET_DEVICE_JPEG_STREAM].SetDecodeChipNum(1);
		
		unsigned bufferSize = m_netVideoInputNum * 128 * 1024 + 1024*1024;
		m_pJepgStreamBuffer = new unsigned char[bufferSize];
		memset(m_pJepgStreamBuffer, 0, bufferSize);

		m_dataBufferMan[NET_DEVICE_JPEG_STREAM].SetChipBuffer(0,m_pJepgStreamBuffer,bufferSize);

		unsigned long *pDDVideoSize = new unsigned long [m_videoInputNum];
		for(long i = m_localVideoInputNum; i < m_videoInputNum; i++)
		{
			pDDVideoSize[i] = DD_VIDEO_SIZE_QVGA|DD_VIDEO_SIZE_VGA|DD_VIDEO_SIZE_720P|DD_VIDEO_SIZE_1080P|DD_VIDEO_SIZE_QQVGA|DD_VIDEO_SIZE_960P|DD_VIDEO_SIZE_SXGA|DD_VIDEO_SIZE_3M;
			pDDVideoSize[i] |= DD_VIDEO_SIZE_16_9_3M;
			pDDVideoSize[i] |= DD_VIDEO_SIZE_4M;
			pDDVideoSize[i] |= DD_VIDEO_SIZE_5M;
		}
		m_dataBufferMan[NET_DEVICE_JPEG_STREAM].SetSupportVideoSize(pDDVideoSize, m_videoInputNum);

		delete [] pDDVideoSize;
	}

	//////////////////////////////////////////////////////////////////////////
	//分配子码流配置资源
	m_pSecondStreamEncode = new ENCODE_INFO_EX[m_videoInputNum];
	memset(m_pSecondStreamEncode, 0, sizeof(ENCODE_INFO_EX) * m_videoInputNum);
	m_pDevInfo = new ND_DEV_INFO[m_videoInputNum];
	memset(m_pDevInfo, 0, sizeof(ND_DEV_INFO) * m_videoInputNum);
	unsigned long chnnMaxRate = (VIDEO_FORMAT_NTSC == g_video_format) ? 30 : 25;

	for(unsigned char chnn = m_localVideoInputNum; chnn < m_videoInputNum; chnn++)
	{
// 		m_pSecondStreamEncode[chnn].encodeType = VIDEO_ENCODE_MODE_CBR;
// 		m_pSecondStreamEncode[chnn].lBitStream = 32;
// 		m_pSecondStreamEncode[chnn].hBitStream = 512;
// 		m_pSecondStreamEncode[chnn].resolution = DVRVIDEO_SIZE_CIF;
// 		m_pSecondStreamEncode[chnn].rate = chnnMaxRate;

		m_pDevInfo[chnn].videoSize = DVRVIDEO_SIZE_QCIF | DVRVIDEO_SIZE_QVGA | DVRVIDEO_SIZE_CIF | DVRVIDEO_SIZE_HD1 | DVRVIDEO_SIZE_VGA \
			| DVRVIDEO_SIZE_D1 | DVRVIDEO_SIZE_960H | DVRVIDEO_SIZE_HD720 | DVRVIDEO_SIZE_HD1080 | DVRVIDEO_SIZE_960P | DVRVIDEO_SIZE_3M;
		m_pDevInfo[chnn].videoSize |= DVRVIDEO_SIZE_16_9_3M | DVRVIDEO_SIZE_4M | DVRVIDEO_SIZE_5M;

		memset(m_pDevInfo[chnn].ucRate, (VIDEO_FORMAT_NTSC == g_video_format) ? 30 : 25, sizeof(m_pDevInfo[chnn].ucRate));
	}

	//子码流初始编码参数从配置中读取
	NCFG_ITEM_HEAD itemHead;
	itemHead.itemID = NCFG_ITEM_ENCODE_MINOR_EX;
	unsigned char *pConfigBlock;
	unsigned long configLen;
	if(CConfigEx::Instance()->GetConfig((unsigned char *)&itemHead, sizeof(NCFG_ITEM_HEAD), &pConfigBlock, configLen))
	{
		ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(pConfigBlock+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD));
		memcpy(m_pSecondStreamEncode, pEncodeInfo, sizeof(ENCODE_INFO_EX) * m_videoInputNum);
	}

	//////////////////////////////////////////////////////////////////////////
	//分配第三码流配置资源
	m_pThirdStreamEncode = new ENCODE_INFO_EX[m_videoInputNum];
	memset(m_pThirdStreamEncode, 0, sizeof(ENCODE_INFO_EX) * m_videoInputNum);
	m_pThirdDevInfo = new ND_DEV_INFO[m_videoInputNum];
	memset(m_pThirdDevInfo, 0, sizeof(ENCODE_INFO_EX) * m_videoInputNum );

	//第三码流编码参数根据产品参数制定
	for(unsigned char chnn = 0; chnn < m_videoInputNum; chnn++)
	{
		m_pThirdStreamEncode[chnn].encodeType = VIDEO_ENCODE_MODE_VBR;
		m_pThirdStreamEncode[chnn].lBitStream = 32;

		if (!CProduct::Instance()->IsDispD1Device())
		{
			if (4 == m_videoInputNum)
			{
				m_pThirdStreamEncode[chnn].hBitStream = 512;
				m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_D1;
			}
			else
			{
#if defined(__CUSTOM_IL03__)
				m_pThirdStreamEncode[chnn].hBitStream = 256;
#else
				m_pThirdStreamEncode[chnn].hBitStream = 512;
#endif
				m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_CIF;
			}
		}
		else
		{
			if (16 >= m_videoInputNum)
			{
				m_pThirdStreamEncode[chnn].hBitStream = 512;
				m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_D1;
			}
			else
			{
#if defined(__CUSTOM_IL03__)
				m_pThirdStreamEncode[chnn].hBitStream = 256;
#else
				m_pThirdStreamEncode[chnn].hBitStream = 512;
#endif
				m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_CIF;
			}
		}

		m_pThirdStreamEncode[chnn].rate = chnnMaxRate;

		m_pThirdDevInfo[chnn].videoSize = DVRVIDEO_SIZE_QCIF | DVRVIDEO_SIZE_QVGA | DVRVIDEO_SIZE_CIF | DVRVIDEO_SIZE_HD1 | DVRVIDEO_SIZE_VGA \
			| DVRVIDEO_SIZE_D1 | DVRVIDEO_SIZE_960H | DVRVIDEO_SIZE_HD720 | DVRVIDEO_SIZE_HD1080 | DVRVIDEO_SIZE_960P | DVRVIDEO_SIZE_3M;
		m_pThirdDevInfo[chnn].videoSize |= DVRVIDEO_SIZE_16_9_3M | DVRVIDEO_SIZE_4M | DVRVIDEO_SIZE_5M;

		memset(m_pThirdDevInfo[chnn].ucRate, (VIDEO_FORMAT_NTSC == g_video_format) ? 30 : 25, sizeof(m_pThirdDevInfo[chnn].ucRate));
	}

	m_maxMainStreamTotalBitRate = INVALID_INDEX(unsigned long);

	//////////////////////////////////////////////////////////////////////////
	//各分辨率与CIF换算倍数表
	m_resolutionInfo[RESOLUTION_QCIF].resolution = DVRVIDEO_SIZE_QCIF;
	m_resolutionInfo[RESOLUTION_QVGA].resolution = DVRVIDEO_SIZE_QVGA;
	m_resolutionInfo[RESOLUTION_CIF].resolution = DVRVIDEO_SIZE_CIF;
	m_resolutionInfo[RESOLUTION_HD1].resolution = DVRVIDEO_SIZE_HD1;
	m_resolutionInfo[RESOLUTION_VGA].resolution = DVRVIDEO_SIZE_VGA;
	m_resolutionInfo[RESOLUTION_D1].resolution = DVRVIDEO_SIZE_D1;
	m_resolutionInfo[RESOLUTION_960H].resolution = DVRVIDEO_SIZE_960H;
	m_resolutionInfo[RESOLUTION_HD720].resolution = DVRVIDEO_SIZE_HD720;
	m_resolutionInfo[RESOLUTION_HD1080].resolution = DVRVIDEO_SIZE_HD1080;

	m_resolutionInfo[RESOLUTION_960P].resolution = DVRVIDEO_SIZE_960P;
	m_resolutionInfo[RESOLUTION_SXGA].resolution = DVRVIDEO_SIZE_SXGA;
	m_resolutionInfo[RESOLUTION_3M].resolution = DVRVIDEO_SIZE_3M;
	m_resolutionInfo[RESOLUTION_16_9_3M].resolution = DVRVIDEO_SIZE_16_9_3M;
	m_resolutionInfo[RESOLUTION_4M].resolution = DVRVIDEO_SIZE_4M;
	m_resolutionInfo[RESOLUTION_5M].resolution = DVRVIDEO_SIZE_5M;

	m_resolutionInfo[RESOLUTION_QCIF].multiNumCIF = 1;
	m_resolutionInfo[RESOLUTION_QVGA].multiNumCIF = 1;
	m_resolutionInfo[RESOLUTION_CIF].multiNumCIF = 1;
	m_resolutionInfo[RESOLUTION_HD1].multiNumCIF = 2;
	m_resolutionInfo[RESOLUTION_VGA].multiNumCIF = 4;
	m_resolutionInfo[RESOLUTION_D1].multiNumCIF = 4;
	m_resolutionInfo[RESOLUTION_960H].multiNumCIF = 5;
	m_resolutionInfo[RESOLUTION_HD720].multiNumCIF = 10;
	m_resolutionInfo[RESOLUTION_HD1080].multiNumCIF = 20;

	m_resolutionInfo[RESOLUTION_960P].multiNumCIF = 15;
	m_resolutionInfo[RESOLUTION_SXGA].multiNumCIF = 16;
	m_resolutionInfo[RESOLUTION_3M].multiNumCIF = 38;
	m_resolutionInfo[RESOLUTION_16_9_3M].multiNumCIF = 38;
	m_resolutionInfo[RESOLUTION_4M].multiNumCIF = 54;
	m_resolutionInfo[RESOLUTION_5M].multiNumCIF = 70;
	m_resolutionInfo[RESOLUTION_4K].multiNumCIF = 99;

	return true;
}

/********************************************************************************
*	把接入设备列表清空，并且断开并释放所有已接入的设备资源
********************************************************************************/
void CNetDeviceManager::Quit()
{
	if(NULL != m_pPoeChannelSn)
	{
		delete [] m_pPoeChannelSn;
		m_pPoeChannelSn = NULL;
	}
	if(NULL != m_pSecondStreamBuffer)
	{
		delete [] m_pSecondStreamBuffer;
		m_pSecondStreamBuffer = NULL;
	}

	if (NULL != m_pJepgStreamBuffer)
	{
		delete [] m_pJepgStreamBuffer;
		m_pJepgStreamBuffer = NULL;
	}

	if(NULL != m_pSecondStreamEncode)
	{
		delete [] m_pSecondStreamEncode;
		m_pSecondStreamEncode = NULL;
	}

	if( NULL != m_pThirdStreamEncode )
	{
		delete [] m_pThirdStreamEncode;
		m_pThirdStreamEncode = NULL;
	}

	if( NULL != m_pThirdDevInfo )
	{
		delete [] m_pThirdDevInfo;
		m_pThirdDevInfo = NULL;
	}
	
	if (NULL != m_pDevInfo)
	{
		delete [] m_pDevInfo;
		m_pDevInfo = NULL;
	}
	
	m_dataBufferMan[NET_DEVICE_MAIN_STREAM].Quit();
	m_dataBufferMan[NET_DEVICE_SUB_STREAM].Quit();
	if(CProduct::Instance()->IPCSupportDisplayMinor())
	{
		m_dataBufferMan[NET_DEVICE_MINOR_STREAM].Quit();
	}
	m_dataBufferMan[NET_DEVICE_JPEG_STREAM].Quit();

	if (m_poeInputNum > 0)
	{

#ifndef __ENVIRONMENT_WIN32__
		m_poeDeviceMan.Quit();
#endif
		m_poeOnvifDeviceMan.Quit();
	}

	m_localVideoInputNum = -1;
	m_netVideoInputNum = -1;
	m_videoInputNum = -1;

	m_pRegDeviceLog = NULL;
	m_pCfgNetDeviceMan = NULL;
	m_pNdLibMan = NULL;
}

bool CNetDeviceManager::Start()
{
	bool bRet = true;

	unsigned int chipId[EXT_DVR_MAX_CHANNEL] = {0};
	unsigned long chipCH = 0;

	long channel = 0;
	for(channel = 0; channel < m_netVideoInputNum; channel++)
	{
		chipCH |= (0x01 << channel);
	}


	CLocalDevice::Instance()->IPCStart(chipCH, 0, chipId);
	CLocalDevice::Instance()->IPCStop();

	for(channel = 0; channel < m_netVideoInputNum; channel++)
	{
		chipId[channel+m_localVideoInputNum] = chipId[channel];
		printf("chipId[%d] = %d\n",channel, chipId[channel]);
	}

	for(channel = 0; channel < m_localVideoInputNum; channel++)
	{
		chipId[channel] = -1;
	}
	
	m_dataBufferMan[NET_DEVICE_MAIN_STREAM].SetChannelDecChip(m_videoInputNum, (long *)chipId);
	m_dataBufferMan[NET_DEVICE_MINOR_STREAM].SetChannelDecChip(m_videoInputNum, (long *)chipId);

	for(channel = 0; channel < m_netVideoInputNum; channel++)
	{
		chipId[channel+m_localVideoInputNum] = 0;
	}
	m_dataBufferMan[NET_DEVICE_SUB_STREAM].SetChannelDecChip(m_videoInputNum, (long *)chipId);
	
	m_dataBufferMan[NET_DEVICE_JPEG_STREAM].SetChannelDecChip(m_videoInputNum, (long *)chipId);

//////////////////////////////////////////////////////////////////////////
	list<NET_DEVICE_LINK_INFO>	newLinkList;
	list<NET_DEVICE_LINK_INFO>::iterator it;

	m_clientID = m_pRegDeviceLog->GetClientID();
	printf("%s %d, m_clientID = %d\n", __FILE__, __LINE__, m_clientID);

	//得到当前绑定到通道的设备列表，将会对这些设备进行网络连接
	m_pRegDeviceLog->GetChBindNetDevice(newLinkList);

	NET_LINK_INFO_EX linkInfoEx;
	memset(&linkInfoEx, 0, sizeof(NET_LINK_INFO_EX));

	long count = newLinkList.size();
	if(count > 0)
	{
		IP_DEVICE_INFO *pNetDevice = new IP_DEVICE_INFO[count];
		int i = 0;

		for(it = newLinkList.begin(); it != newLinkList.end(); it++, i++)
		{
			pNetDevice[i].deviceID = (*it).deviceID;
			pNetDevice[i].channel = (*it).channel;
			pNetDevice[i].nPort= (*it).nPort;
			pNetDevice[i].nHttpPort = (*it).nHttpPort;
			pNetDevice[i].nCtrlPort= (*it).nCtrlPort;
			strcpy(pNetDevice[i].szID, (*it).szID);
			strcpy(pNetDevice[i].szServer, (*it).szServer);
			strcpy(pNetDevice[i].username, (*it).username);
			strcpy(pNetDevice[i].password, (*it).password);
			pNetDevice[i].manufacturerId = (*it).manufacturerId;
			strcpy(pNetDevice[i].manufacturerName, (*it).manufacturerName);
			strcpy(pNetDevice[i].productModel, (*it).productModel);
			strcpy(pNetDevice[i].szEtherName,(*it).szEtherName);

			pNetDevice[i].bUseDefaultCfg = (*it).bUseDefaultCfg;

			pNetDevice[i].status = ND_STATUS_DISCONNECT;
			assert((*it).enabled);
		}

		m_pCfgNetDeviceMan->SetIPDeviceInfo(pNetDevice, count);

		delete [] pNetDevice;
	}

	//把读取到的设备信息添加到后备列表中
	//m_listNetLinkInfo.clear();
	ClearAllNetLinkInfoEx(m_netLinkInfo);
	while(newLinkList.size() > 0)
	{
		NET_DEVICE_LINK_INFO &linkInfo = newLinkList.front();

		memcpy(&linkInfoEx.linkInfo, &linkInfo, sizeof(NET_DEVICE_LINK_INFO));
		linkInfoEx.status = ND_STATUS_DISCONNECT;
		linkInfoEx.bPOEOnLine = false;
//		NewNetDevice(linkInfoEx);
		m_netLinkInfo[linkInfoEx.linkInfo.channel-m_localVideoInputNum] = linkInfoEx;
		NewNetDevice(m_netLinkInfo[linkInfoEx.linkInfo.channel-m_localVideoInputNum]);
		newLinkList.pop_front();
	}
//////////////////////////////////////////////////////////////////////////
	for(unsigned char channCnt=0;channCnt<g_MaxNetChannCnt;channCnt++)
	{
		m_undealChannFlag[channCnt] = false;
	}
//////////////////////////////////////////////////////////////////////////

	m_threadConnect = PUB_CreateThread(ConnectThread, this, &m_bRunning);

	if(m_threadConnect == PUB_CREATE_THREAD_FAIL)
	{
		bRet = false;
	}

	m_threadTimer = PUB_CreateThread(TimerThread, this, &m_bRunning);
	if(m_threadTimer == PUB_CREATE_THREAD_FAIL)
	{
		PUB_ExitThread(&m_threadConnect, &m_bRunning);
		bRet = false;
	}
	if (m_poeInputNum > 0)
	{
#ifndef __ENVIRONMENT_WIN32__
		m_poeDeviceMan.Start();
#endif
		m_poeOnvifDeviceMan.Start();
	}

	return bRet;
}

void CNetDeviceManager::Stop()
{
	if(m_threadConnect != PUB_CREATE_THREAD_FAIL)
	{
		PUB_ExitThread(&m_threadConnect, &m_bRunning);
	}

	if(m_threadTimer != PUB_CREATE_THREAD_FAIL)
	{
		PUB_ExitThread(&m_threadTimer, &m_bRunning);
	}
	if (m_poeInputNum > 0)
	{
#ifndef __ENVIRONMENT_WIN32__
		m_poeDeviceMan.Stop();
#endif
		m_poeOnvifDeviceMan.Stop();
	}
	
	for(long netTemp = 0;netTemp<m_netVideoInputNum;netTemp++)
	{
		m_netLinkInfoLock[netTemp].Lock();
		CNetDevice *pNetDevice = reinterpret_cast<CNetDevice *>(m_netLinkInfo[netTemp].pNetDevice);
		if(pNetDevice == NULL)
		{
			m_netLinkInfoLock[netTemp].UnLock();
			continue;
		}

		pNetDevice->Quit();
		m_pNdLibMan->DestroyNetDevice(pNetDevice);
		m_netLinkInfo[netTemp].pNetDevice = NULL;
		m_netLinkInfoLock[netTemp].UnLock();
	}

	ClearAllNetLinkInfoEx(m_netLinkInfo);
	m_tempLinkInfoList.clear();

}

void CNetDeviceManager::NotifyRegDeviceChanged1()
{
	list<NET_DEVICE_LINK_INFO>	newLinkList;
	list<NET_LINK_INFO_EX>::iterator it;
//	list<NET_LINK_INFO_EX>::iterator it2;
	bool bFindIPC = false;

	m_clientID = m_pRegDeviceLog->GetClientID();

/*
	for(it = m_tempLinkInfoList.begin(); it != m_tempLinkInfoList.end(); it++)
	{
		printf("%s:%s:%d, szid=%s, channel=%d\n", __FUNCTION__, __FILE__, __LINE__, (*it).linkInfo.szID, (*it).linkInfo.channel);
	}

	for(int netCnt=0;netCnt<m_netVideoInputNum;netCnt++)//it2 = m_listNetLinkInfo.begin(); it2 != m_listNetLinkInfo.end(); it2++)
	{
		if(m_netLinkInfo[netCnt].pNetDevice !=NULL)
		{
			printf("%s:%s:%d, szid=%s, channel=%d\n", __FUNCTION__, __FILE__, __LINE__, m_netLinkInfo[netCnt].linkInfo.szID, m_netLinkInfo[netCnt].linkInfo.channel);
		}
	}
*/
	//遍历当前设备列表，把新列表中相同的设备进行数据更新，这些设备无需断开重连
	//对于在新列表中已不存在的设备及改变了绑定通道的设备，要先关闭数据网传;
	for(it = m_tempLinkInfoList.begin(); it != m_tempLinkInfoList.end(); it++)
	{
		bFindIPC = false;
		long netCnt=0;
		for(netCnt=0;netCnt<m_netVideoInputNum;netCnt++)//it2 = m_listNetLinkInfo.begin(); it2 != m_listNetLinkInfo.end(); it2++)
		{
			if(m_netLinkInfo[netCnt].pNetDevice !=NULL)
			{
				if(0 == strcmp(m_netLinkInfo[netCnt].linkInfo.szID, (*it).linkInfo.szID))
				{
					bFindIPC = true;
					break;
				}
			}
		}

		if( !bFindIPC )
		{
			if (0 < strlen((*it).linkInfo.szID))
			{
				WriteLog(m_clientID, LOG_TYPE_IPCAM_ADD);
			}
		}
		else
		{
			if(!CheckIPCIsSame( m_netLinkInfo[netCnt].linkInfo, (*it).linkInfo))
			{
				WriteLog(m_clientID, LOG_TYPE_IPCAM_SETUP);
			}
		}
	}

	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		if(m_netLinkInfo[netCnt].pNetDevice != NULL)
		{
			bFindIPC = false;
			for(it = m_tempLinkInfoList.begin(); it != m_tempLinkInfoList.end(); it++)
			{
				if(0 == strlen(m_netLinkInfo[netCnt].linkInfo.szID) || 0 == strcmp(m_netLinkInfo[netCnt].linkInfo.szID, (*it).linkInfo.szID))
				{
					bFindIPC = true;
					break;
				}
			}
			
			if(!bFindIPC)
			{
				WriteLog(m_clientID, LOG_TYPE_IPCAM_DELETE );
			}
		}
	}

	for (int i = 0; i < m_poeInputNum; i++)
	{
		m_pPoeChannelSn[i].channel = i;
		m_pPoeChannelSn[i].poe_ipc_status = -1;
		m_pPoeChannelSn[i].sn[0] = '\0';
	}

	for(it = m_tempLinkInfoList.begin(); it != m_tempLinkInfoList.end(); it++)
	{
		if((*it).linkInfo.channel < m_poeInputNum)
		{
			m_pPoeChannelSn[(*it).linkInfo.channel].channel = (*it).linkInfo.channel;
			strcpy( m_pPoeChannelSn[(*it).linkInfo.channel].sn, (*it).linkInfo.szID );

			//当前POE设备已经通过POE连接
			if( (*it).bPOEOnLine )
			{
				m_pPoeChannelSn[(*it).linkInfo.channel].poe_ipc_status = 0;
			}
			else
			{
				//当前POE通道没有连接IPC设备
				if( IsPoeDevice( (*it).linkInfo ) )
				{
					m_pPoeChannelSn[(*it).linkInfo.channel].poe_ipc_status = -1;
				}
				//当期通道连接eth0的IPC设备
				else
				{
					m_pPoeChannelSn[(*it).linkInfo.channel].poe_ipc_status = -2;
				}
			}
		}
	}

}


void CNetDeviceManager::NotifyRegDeviceChanged()
{
	list<NET_DEVICE_LINK_INFO>	newLinkList;
	//list<NET_LINK_INFO_EX>::iterator it;
	list<NET_LINK_INFO_EX>::iterator it2;

	return;
	
	m_clientID = m_pRegDeviceLog->GetClientID();
	
	//遍历当前设备列表，把新列表中相同的设备进行数据更新，这些设备无需断开重连
	//对于在新列表中已不存在的设备及改变了绑定通道的设备，要先关闭数据网传;
	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		for(it2 = m_tempLinkInfoList.begin(); it2 != m_tempLinkInfoList.end(); it2++)
		{
			if(m_netLinkInfo[netCnt].pNetDevice != NULL)
			{
				if((*it2).linkInfo.deviceID == m_netLinkInfo[netCnt].linkInfo.deviceID)
				{
					(*it2).status = m_netLinkInfo[netCnt].status;
					(*it2).pNetDevice = m_netLinkInfo[netCnt].pNetDevice;

					//POE接入的通道如果序列号不为空，则保持上次的上线状态
					if(IsPoeDevice((*it2).linkInfo) && IsPoeDevice(m_netLinkInfo[netCnt].linkInfo) && ('\0' != (*it2).linkInfo.szID[0]))
					{
						(*it2).bPOEOnLine = (*it2).bPOEOnLine ? (*it2).bPOEOnLine : m_netLinkInfo[netCnt].bPOEOnLine;
					}

					if(!IsPoeDevice((*it2).linkInfo))	//当前设备是非即插即用设备
					{
						if((*it2).linkInfo.channel < m_poeInputNum)
						{
							m_pPoeChannelSn[(*it2).linkInfo.channel].channel = -1;
							m_pPoeChannelSn[(*it2).linkInfo.channel].sn[0] = '\0';
							printf("empty sn[%d], %s, %d\n", (*it2).linkInfo.channel, __FILE__, __LINE__);
						}
						(*it2).bPOEOnLine = true;
					}
					else if(!IsPoeDevice(m_netLinkInfo[netCnt].linkInfo))	//该通道之前是非即插即用设备，但现在是
					{
						m_pPoeChannelSn[(*it2).linkInfo.channel].channel = (*it2).linkInfo.channel;
						WriteLog(m_clientID, LOG_TYPE_IPCAM_SETUP);
					}

					//设备信息发生的变化或绑定的通道发生的变化
					if( 0 != strcmp((*it2).linkInfo.szServer, m_netLinkInfo[netCnt].linkInfo.szServer) \
						|| ((*it2).linkInfo.nPort != m_netLinkInfo[netCnt].linkInfo.nPort) \
						|| ((*it2).linkInfo.manufacturerId != m_netLinkInfo[netCnt].linkInfo.manufacturerId) \
						|| ((*it2).linkInfo.nHttpPort != m_netLinkInfo[netCnt].linkInfo.nHttpPort) \
						|| ((*it2).linkInfo.channel != m_netLinkInfo[netCnt].linkInfo.channel))
					{
						(*it2).status = ND_STATUS_DISCONNECT;
						WriteLog(m_clientID, LOG_TYPE_IPCAM_SETUP);
					}
					
					break;
				}
			}
		}
		
		//这个设备已经解除与本地通道的绑定
		if(it2 == m_tempLinkInfoList.end())
		{
			if(m_netLinkInfo[netCnt].linkInfo.channel < m_poeInputNum)
			{
				m_pPoeChannelSn[m_netLinkInfo[netCnt].linkInfo.channel].channel = m_netLinkInfo[netCnt].linkInfo.channel;
			}
			WriteLog(m_clientID, LOG_TYPE_IPCAM_DELETE);
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}
	
	//判断是否有添加新设备
	bool bHasNewDevice = true;
	for (it2 = m_tempLinkInfoList.begin(); it2 != m_tempLinkInfoList.end(); ++it2)
	{
		bHasNewDevice = true;
		for (long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
		{
			m_netLinkInfoLock[netCnt].Lock();
			if(m_netLinkInfo[netCnt].pNetDevice != NULL)
			{
				if ((*it2).linkInfo.deviceID == m_netLinkInfo[netCnt].linkInfo.deviceID)
				{
					bHasNewDevice = false;
					m_netLinkInfoLock[netCnt].UnLock();
					break;
				}
			}
			m_netLinkInfoLock[netCnt].UnLock();
		}

		if (bHasNewDevice)
		{
			WriteLog(m_clientID, LOG_TYPE_IPCAM_ADD);
			break;
		}
	}
	
	m_bLinkInfoChanged = true;
	

	m_poeOnvifDeviceMan.ChangeChannelSn(m_pPoeChannelSn, false);
}

CNetDeviceManager * CNetDeviceManager::Instance()
{
	static CNetDeviceManager s_netDevice;
	return &s_netDevice;
}

bool CNetDeviceManager::GetDataBuffer(long chipId, unsigned char *&pBuffer, unsigned long &dataLen, unsigned long stream)
{
	if(!m_bRunning)
	{
		return false;
	}

	return m_dataBufferMan[stream].GetDataBuffer(chipId, pBuffer, dataLen);
}

void CNetDeviceManager::ReleaseDataBuffer(long chipId, unsigned char *pBuffer, unsigned long dataLen, unsigned long stream)
{
	if(!m_bRunning)
	{
		return;
	}

	m_dataBufferMan[stream].ReleaseDataBuffer(chipId, pBuffer, dataLen);
}

long CNetDeviceManager::GetBufferedNum(long chipId, unsigned long stream)
{
	if(!m_bRunning)
	{
		return 0;
	}
	
	return m_dataBufferMan[stream].GetBufferedNum(chipId);
}

bool CNetDeviceManager::IsBufferFull(long chipId, unsigned long stream /* = 0 */)
{
	if(!m_bRunning)
	{
		return false;
	}

	return m_dataBufferMan[stream].IsBufferFull(chipId);
}

bool CNetDeviceManager::PrintBufferInfo(long chipId, unsigned long stream /* = 0 */)
{
	if(!m_bRunning)
	{
		return false;
	}

	return m_dataBufferMan[stream].PrintBufferInfo(chipId);
}

unsigned long CNetDeviceManager::GetAverageRate(unsigned long stream /* = 0 */)
{
	return m_dataBufferMan[stream].GetAverageRate();
}


void CNetDeviceManager::DeviceAction(bool bAction)
{
	if(!m_bRunning)
	{
		return;
	}

	m_dataBufferMan[NET_DEVICE_MAIN_STREAM].Action(bAction);
	m_dataBufferMan[NET_DEVICE_SUB_STREAM].Action(bAction);
	m_dataBufferMan[NET_DEVICE_MINOR_STREAM].Action(bAction);
	m_dataBufferMan[NET_DEVICE_JPEG_STREAM].Action(bAction);
}

void CNetDeviceManager::ClearDataBuffer(unsigned long stream /*= NET_DEVICE_MAX_STREAM*/)
{
	if(!m_bRunning)
	{
		return;
	}
	assert(stream<=NET_DEVICE_MAX_STREAM);
	if(stream==NET_DEVICE_MAX_STREAM)
	{
		m_dataBufferMan[NET_DEVICE_MAIN_STREAM].ClearDataBuffer();
		m_dataBufferMan[NET_DEVICE_SUB_STREAM].ClearDataBuffer();
		m_dataBufferMan[NET_DEVICE_MINOR_STREAM].ClearDataBuffer();
		m_dataBufferMan[NET_DEVICE_JPEG_STREAM].ClearDataBuffer();
	}
	else
	{
		m_dataBufferMan[stream].ClearDataBuffer();
	}
}

bool CNetDeviceManager::KeyFrame(ULONGLONG ullCH, ULONGLONG ullSecondCH, ULONGLONG ullThirdCH)
{
	if(!(ullCH | ullSecondCH | ullThirdCH))
	{
		return true;
	}

	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if(m_netLinkInfo[netCnt].pNetDevice != NULL)
		{
			NET_LINK_INFO_EX &linkInfo = m_netLinkInfo[netCnt];
			if(linkInfo.status != ND_STATUS_OK)
			{
				m_netLinkInfoLock[netCnt].UnLock();
				//如果只要当前连接的网络设备
				continue;
			}
			if((ullCH | ullSecondCH | ullThirdCH) & (0x01 << linkInfo.linkInfo.channel))
			{
				linkInfo.pNetDevice->RequestKeyFrame(ullCH, ullSecondCH, ullThirdCH);
			}
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}

	return true;
}

void CNetDeviceManager::GetMotionStatus(ULONGLONG &status)
{	
	status = 0;
	ULONGLONG netAlarmInfo;

	unsigned char i=0;

	
	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if((NULL != m_netLinkInfo[netCnt].pNetDevice) && (ND_STATUS_OK == m_netLinkInfo[netCnt].status))
		{
			netAlarmInfo = m_netLinkInfo[netCnt].pNetDevice->GetMotionAlarm();
			status |= (netAlarmInfo << m_netLinkInfo[netCnt].linkInfo.channel);
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}
	
}

void CNetDeviceManager::GetSensorInStatus(ULONGLONG &status)
{

	status = 0;
	ULONGLONG netAlarmInfo;

	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if((NULL != m_netLinkInfo[netCnt].pNetDevice) && (ND_STATUS_OK == m_netLinkInfo[netCnt].status))
		{
			netAlarmInfo = m_netLinkInfo[netCnt].pNetDevice->GetSensorAlarm();
			long sensorInIndex = m_netLinkInfo[netCnt].linkInfo.channel + m_localSensorInNum - m_localVideoInputNum;
			status |= (netAlarmInfo << sensorInIndex);
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}

}

void CNetDeviceManager::GetVideoLossStatus(ULONGLONG &status)
{

	status = 0;
	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if(m_netLinkInfo[netCnt].pNetDevice!= NULL)
		{
// 			if(m_netLinkInfo[netCnt].linkInfo.channel >= m_poeInputNum)
// 			{
// 				m_netLinkInfoLock[netCnt].UnLock();
// 				continue;
// 			}

			if((m_netLinkInfo[netCnt].status != ND_STATUS_OK) && m_netLinkInfo[netCnt].pNetDevice->IsReConnectting())
			{
				status |= (ULONGLONG)0x01 << m_netLinkInfo[netCnt].linkInfo.channel;
			}
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}

	//status = 0;

}

void CNetDeviceManager::GetOtherAlarm(ULONGLONG &status)
{
	status = 0;
	//NET_DEVICE::NET_ALARM_INFO netAlarmInfo;
	//list<NET_LINK_INFO_EX>::iterator it;
	//for(it = m_listNetLinkInfo.begin(); it != m_listNetLinkInfo.end(); it++)
	//{
	//	if((NULL != (*it).pNetDevice) && (ND_STATUS_OK == (*it).status))
	//	{
	//		netAlarmInfo = (*it).pNetDevice->GetAlarm(netAlarmInfo);
	//		status |= netAlarmInfo.IVS;
	//	}
	//}
}

void CNetDeviceManager::GetExceptionAlarm(ULONGLONG &status)
{
	status = 0;
	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if(m_netLinkInfo[netCnt].pNetDevice != NULL)
		{
			if(!IsPoeDevice(m_netLinkInfo[netCnt].linkInfo))
			{
				m_netLinkInfoLock[netCnt].UnLock();
				continue;
			}

			if(m_netLinkInfo[netCnt].bPOEOnLine && (m_netLinkInfo[netCnt].status != ND_STATUS_OK))
			{
				if((GetTickCount() - m_netLinkInfo[netCnt].POEOnLineTime) > 100*1000)	//link状态超过100秒没有连接上，认为POE网络断开了
				{
					status |= static_cast<ULONGLONG>(0x01) << EXCEPTION_ALARM_TYPE_POE_NET;
				}
			}
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}

	status = 0;
}

bool CNetDeviceManager::PTZAction(unsigned long channel, unsigned char cmdtype, unsigned long ptzParam, unsigned char speed, unsigned char waiteTime)
{
	/*
	通道编号从本地通道开始算起
	*/

	if((channel < m_localVideoInputNum) || (channel >= m_videoInputNum))
	{
		return false;
	}

	//库中绑定通道从0 算起
	long bindChnn = channel /*- m_localVideoInputNum*/;

	m_netLinkInfoLock[channel].Lock();

	//只能获取当前连接的设备信息
	CNetDevice *pNetDevice = GetNetDeviceByChnn(bindChnn, true);
	if(NULL == pNetDevice)
	{
		m_netLinkInfoLock[channel].UnLock();
		return true;
	}

	bool bRet = pNetDevice->PTZControl(0, cmdtype, ptzParam, speed, waiteTime);

	m_netLinkInfoLock[channel].UnLock();

	return bRet;
}

bool CNetDeviceManager::SetVideoColor(unsigned char channel, unsigned char brightness, unsigned char hue, unsigned char saturation, unsigned char contrast)
{
	/*
		通道编号从本地通道开始算起
	*/

	if((channel < m_localVideoInputNum) || (channel >= m_videoInputNum))
	{
		return false;
	}

	//库中绑定通道从0 算起
	long bindChnn = channel /*- m_localVideoInputNum*/;


	m_netLinkInfoLock[channel].Lock();
	//只能获取当前连接的设备信息
	CNetDevice *pNetDevice = GetNetDeviceByChnn(bindChnn, true);
	if(NULL == pNetDevice)
	{
		m_netLinkInfoLock[channel].UnLock();
		return false;
	}

	DD_VIDEO_COLOR netVideoColor;
	memset(&netVideoColor, 0, sizeof(DD_VIDEO_COLOR));

	netVideoColor.brightness = brightness;
	netVideoColor.hue = hue;
	netVideoColor.saturation = saturation;
	netVideoColor.contrast = contrast;
	if(!pNetDevice->ColorControl(0, ND_CTRL_COLOR_SET, netVideoColor))
	{
		m_netLinkInfoLock[channel].UnLock();
		return false;
	}
	m_netLinkInfoLock[channel].UnLock();

	return true;
}

bool CNetDeviceManager::SaveVideoColor(unsigned char channel, unsigned char brightness, unsigned char hue, unsigned char saturation, unsigned char contrast)
{
	/*
		通道编号从本地通道开始算起
	*/

	if((channel < m_localVideoInputNum) || (channel >= m_videoInputNum))
	{
		return false;
	}

	//库中绑定通道从0 算起
	long bindChnn = channel /*- m_localVideoInputNum*/;

	m_netLinkInfoLock[channel].Lock();
	//只能获取当前连接的设备信息
	CNetDevice *pNetDevice = GetNetDeviceByChnn(bindChnn, true);
	if(NULL == pNetDevice)
	{
		m_netLinkInfoLock[channel].UnLock();
		return false;
	}

	DD_VIDEO_COLOR netVideoColor;
	memset(&netVideoColor, 0, sizeof(DD_VIDEO_COLOR));

	netVideoColor.brightness = brightness;
	netVideoColor.hue = hue;
	netVideoColor.saturation = saturation;
	netVideoColor.contrast = contrast;
	if(!pNetDevice->ColorControl(0, ND_CTRL_COLOR_SAVE, netVideoColor))
	{
		m_netLinkInfoLock[channel].UnLock();
		return false;
	}
	m_netLinkInfoLock[channel].UnLock();

	return true;
}

bool CNetDeviceManager::GetDefaultVideoColor(unsigned char channel, unsigned char& brightness, unsigned char& hue, unsigned char& saturation, unsigned char& contrast)
{
	/*
	通道编号从本地通道开始算起
	*/

	if((channel < m_localVideoInputNum) || (channel >= m_videoInputNum))
	{
		return false;
	}

	//库中绑定通道从0 算起
	long bindChnn = channel /*- m_localVideoInputNum*/;

	m_netLinkInfoLock[channel].Lock();
	
	//只能获取当前连接的设备信息
	CNetDevice *pNetDevice = GetNetDeviceByChnn(bindChnn, true);
	if(NULL == pNetDevice)
	{
		m_netLinkInfoLock[channel].UnLock();
		return false;
	}

	DD_VIDEO_COLOR netVideoColor;
	memset(&netVideoColor, 0, sizeof(DD_VIDEO_COLOR));

	if(!pNetDevice->ColorControl(0, ND_CTRL_COLOR_GET_DEFAULT, netVideoColor))
	{
		m_netLinkInfoLock[channel].UnLock();
		return false;
	}

	m_netLinkInfoLock[channel].UnLock();
	
	brightness = netVideoColor.brightness;
	hue = netVideoColor.hue;
	saturation = netVideoColor.saturation;
	contrast = netVideoColor.contrast;

	return true;
}
bool CNetDeviceManager::SetVideoOther(unsigned int channel,unsigned char *pOther,unsigned long lType)
{
	if((channel < m_localVideoInputNum) || (channel >= m_videoInputNum))
	{
		return false;
	}
	if ((NULL == pOther)&&(CMD_REQUEST_SET_IMAGE_ROI != lType))
	{
		return false;
	}
	//库中绑定通道从0 算起
	long bindChnn = channel /*- m_localVideoInputNum*/;
	m_deviceListLock.Lock();
	//只能获取当前连接的设备信息
	CNetDevice *pNetDevice = GetNetDeviceByChnn(bindChnn, true);
	if(NULL == pNetDevice)
	{
		m_deviceListLock.UnLock();
		printf("%s,  %s,  %d,  %d,  %s",__FUNCTION__,__FILE__,__LINE__,bindChnn," #######fail########");
		return false;
	}
    switch( lType )
	{
	case CMD_REQUEST_SET_IMAGE_MIRROR:
		if(!pNetDevice->OtherControl(m_localVideoInputNum, ND_CTRL_OTHER_MIRROR, pOther))
		{
			m_deviceListLock.UnLock();
			return false;
		}
		break;
	case CMD_REQUEST_SET_IMAGE_FILP:
		if(!pNetDevice->OtherControl(m_localVideoInputNum, ND_CTRL_OTHER_FLIP, pOther))
		{
			m_deviceListLock.UnLock();
			return false;
		}
		break;
	case CMD_REQUEST_SET_IMAGE_ROTATO:
		if(!pNetDevice->OtherControl(m_localVideoInputNum, ND_CTRL_OTHER_ROTATO, pOther))
		{
			m_deviceListLock.UnLock();
			return false;
		}
		break;
	case CMD_REQUEST_SET_AUTOFOCUS:
		if (!pNetDevice->OtherControl(m_localVideoInputNum,ND_CTRL_OTHER_AUTOFOCUS,pOther))
		{
			return false;
		}
		break;
	case CMD_REQUEST_SET_IMAGE_DISTORTION:
		if(!pNetDevice->OtherControl(m_localVideoInputNum, ND_CTRL_OTHER_DISTORTION, pOther))
		{
			m_deviceListLock.UnLock();
			return false;
		}
		break;
	case CMD_REQUEST_SET_IMAGE_ROI:
		if(!pNetDevice->OtherControl(m_localVideoInputNum, ND_CTRL_OTHER_ROI, pOther))
		{
			m_deviceListLock.UnLock();
			return false;
		}
		break;
	default:
		printf("%s,  %s,  %d,  %d,  %s",__FUNCTION__,__FILE__,__LINE__," #######fail########");
		m_deviceListLock.UnLock();
		return false;
	}
	m_deviceListLock.UnLock();
	return true;
}

bool CNetDeviceManager::SetNetConfig(const unsigned char* pCfgData, unsigned long dataLen)
{

	m_pCfgNetDeviceMan->SetNetConfig(this, NULL, pCfgData, dataLen);
	m_pCfgNetDeviceMan->TestUpdataInitData(pCfgData, dataLen);


	return true;
}

void CNetDeviceManager::SyncTime(unsigned char channel)
{

	m_netLinkInfoLock[channel].Lock();
	CNetDevice* pNetDevice = GetNetDeviceByChnn(channel);

	if(pNetDevice)
	{
		SyncTime(pNetDevice);
	}
	m_netLinkInfoLock[channel].UnLock();

}

void CNetDeviceManager::SyncTimeAll()
{
	/*
		同步所有连接上的IPC时间
	*/
	//CClientRTPMain::Instance->HandleSystemTimeChanged();

	
	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if(m_netLinkInfo[netCnt].pNetDevice != NULL)
		{
			if(m_netLinkInfo[netCnt].status == ND_STATUS_OK)
			{
				SyncTime(m_netLinkInfo[netCnt].pNetDevice);
			}
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}
}

void CNetDeviceManager::SyncTime(CNetDevice* pNetDevice)
{
	assert(pNetDevice);

	//同步时间
	tm nowTm = DVRTimeToTm(GetCurrTime());

	DD_TIME ddTm;
	memset(&ddTm, 0, sizeof(DD_TIME));
	ddTm.year = nowTm.tm_year + 1900;
	ddTm.month = nowTm.tm_mon;
	ddTm.mday = nowTm.tm_mday;
	ddTm.hour = nowTm.tm_hour;
	ddTm.minute = nowTm.tm_min;
	ddTm.second = nowTm.tm_sec;

	pNetDevice->SetDateTime(ddTm);

	printf("(%s.%d)Sync Net Device Time : chnn = %d.  time = %02d/%02d/%02d %02d:%02d:%02d. \n", __FILE__, __LINE__, 
			pNetDevice->GetBindChannel() + m_localVideoInputNum,
			ddTm.year, ddTm.month + 1, ddTm.mday, ddTm.hour, ddTm.minute, ddTm.second);
}

bool CNetDeviceManager::SetNetDevice1(IP_DEVICE_INFO *pNetDevice, unsigned long num)
{
	
//	m_tempLinkInfoList.clear();
	//设置网络设备信息时，需要把使用默认值编码参数的配置写入配置文件
	CMyList<NCFG_ITEM_ID>	idList;
	idList.AddTail(NCFG_ITEM_ENCODE_MAJOR_EX);
	idList.AddTail(NCFG_ITEM_ENCODE_MINOR_EX);
	unsigned char *pDataBlock = NULL;
	unsigned long blockLen = 0;
	CConfigEx::Instance()->GetConfigEx(idList, &pDataBlock, blockLen);
	ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(pDataBlock + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * 2);
	ENCODE_INFO_EX *pEncodeInfoMinor = pEncodeInfo + m_videoInputNum;
	int maxRate = (g_video_format == VIDEO_FORMAT_NTSC) ? 30 : 25;
	bool bNeedWriteCfg = false;

	/*设置所有网络设备*/
	unsigned long count = m_pRegDeviceLog->GetMaxDeviceCount();
	count = (count >= num) ? num : count;

	NET_DEVICE_LINK_INFO *pNetLinkInfo = new NET_DEVICE_LINK_INFO[count+m_poeInputNum];
	memset(pNetLinkInfo, 0, sizeof(NET_DEVICE_LINK_INFO)*(count+m_poeInputNum));

	NET_LINK_INFO_EX *pNetLinkInfoEx = new NET_LINK_INFO_EX[count+m_poeInputNum];
	memset(pNetLinkInfoEx, 0, sizeof(NET_LINK_INFO_EX)*(count+m_poeInputNum));


	m_pRegDeviceLog->GetNetDevice(pNetLinkInfo, m_poeInputNum);

	for(unsigned char i = 0; i < m_poeInputNum; i++)
	{
		//删除的分配给POE口的地址需要回收
		if(!IsPoeDevice(pNetLinkInfo[i]))
		{
			pNetLinkInfo[i].enabled = true;
#if defined(__CUSTOM_PL14__)
			strcpy(pNetLinkInfo[i].username, "root");
			strcpy(pNetLinkInfo[i].password, "pass");
#elif defined(__CUSTOM_CNB__)
			strcpy(pNetLinkInfo[i].username, "root");
			strcpy(pNetLinkInfo[i].password, "admin");
#else
			strcpy(pNetLinkInfo[i].username, "admin");
			strcpy(pNetLinkInfo[i].password, "123456");
#endif
			pNetLinkInfo[i].manufacturerId = MID_TVT;
			strcpy(pNetLinkInfo[i].productModel, "");
			strcpy(pNetLinkInfo[i].szEtherName, POE_ETHER);
			
			in_addr ipaddr;
			ipaddr.s_addr = m_switchIP + htonl(i+1);
			strcpy(pNetLinkInfo[i].szServer, inet_ntoa(ipaddr));
		}
	}

	int index = m_poeInputNum;
	for(int i = 0; i < count; i++)
	{
		if(pNetDevice[i].channel < m_poeInputNum)
		{
			if (0 < strlen(pNetDevice[i].szID))
			{
				for(int m = 0; m < m_poeInputNum; m++)
				{
					if(strcmp(pNetLinkInfo[m].szID, pNetDevice[i].szID) == 0)
					{
						if(pNetLinkInfo[m].channel != pNetDevice[i].channel)
						{
							pNetLinkInfo[m].enabled = true;
#if defined(__CUSTOM_PL14__)
							strcpy(pNetLinkInfo[m].username, "root");
							strcpy(pNetLinkInfo[m].password, "pass");
#elif defined(__CUSTOM_CNB__)
							strcpy(pNetLinkInfo[m].username, "root");
							strcpy(pNetLinkInfo[m].password, "admin");
#else
							strcpy(pNetLinkInfo[m].username, "admin");
							strcpy(pNetLinkInfo[m].password, "123456");
#endif
							pNetLinkInfo[m].manufacturerId = MID_TVT;
							strcpy(pNetLinkInfo[m].productModel, "");
							strcpy(pNetLinkInfo[m].szEtherName, POE_ETHER);
							strcpy(pNetLinkInfo[m].szID, "");

							in_addr ipaddr;
							ipaddr.s_addr = m_switchIP + htonl(m+1);
							strcpy(pNetLinkInfo[m].szServer, inet_ntoa(ipaddr));

							strcpy(m_pPoeChannelSn[pNetDevice[i].channel].sn, m_pPoeChannelSn[pNetLinkInfo[m].channel].sn);
							m_pPoeChannelSn[pNetLinkInfo[m].channel].sn[0] = '\0';
							pNetLinkInfoEx[pNetDevice[i].channel].bPOEOnLine = true;
							pNetLinkInfoEx[pNetLinkInfo[m].channel].bPOEOnLine = false;
						}
						break;
					}
				}
			}

			pNetLinkInfo[pNetDevice[i].channel].nPort= pNetDevice[i].nPort;
			pNetLinkInfo[pNetDevice[i].channel].nHttpPort = pNetDevice[i].nHttpPort;
			pNetLinkInfo[pNetDevice[i].channel].nCtrlPort= pNetDevice[i].nCtrlPort;
			strcpy(pNetLinkInfo[pNetDevice[i].channel].szID, pNetDevice[i].szID);
			strcpy(pNetLinkInfo[pNetDevice[i].channel].szServer, pNetDevice[i].szServer);
			strcpy(pNetLinkInfo[pNetDevice[i].channel].username, pNetDevice[i].username);
			strcpy(pNetLinkInfo[pNetDevice[i].channel].password, pNetDevice[i].password);
			pNetLinkInfo[pNetDevice[i].channel].manufacturerId = pNetDevice[i].manufacturerId;
			strcpy(pNetLinkInfo[pNetDevice[i].channel].manufacturerName, pNetDevice[i].manufacturerName);
			strcpy(pNetLinkInfo[pNetDevice[i].channel].productModel, pNetDevice[i].productModel);

			unsigned long deviceIP = inet_addr(pNetDevice[i].szServer);
			if((deviceIP & m_switchMask) != (m_switchIP & m_switchMask))
			{
				strcpy(pNetLinkInfo[pNetDevice[i].channel].szEtherName, "eth0");
			}
		}
		else
		{
			pNetLinkInfo[index].deviceID = pNetDevice[i].deviceID;
			pNetLinkInfo[index].channel = pNetDevice[i].channel;
			pNetLinkInfo[index].nPort= pNetDevice[i].nPort;
			pNetLinkInfo[index].nHttpPort = pNetDevice[i].nHttpPort;
			pNetLinkInfo[index].nCtrlPort= pNetDevice[i].nCtrlPort;
			strcpy(pNetLinkInfo[index].szID, pNetDevice[i].szID);
			strcpy(pNetLinkInfo[index].szServer, pNetDevice[i].szServer);
			strcpy(pNetLinkInfo[index].szEtherName, pNetDevice[i].szEtherName);
			strcpy(pNetLinkInfo[index].username, pNetDevice[i].username);
			strcpy(pNetLinkInfo[index].password, pNetDevice[i].password);
			pNetLinkInfo[index].manufacturerId = pNetDevice[i].manufacturerId;
			strcpy(pNetLinkInfo[index].manufacturerName, pNetDevice[i].manufacturerName);
			strcpy(pNetLinkInfo[index].productModel, pNetDevice[i].productModel);
			index++;
		}

		//对使用默认配置的网络设备，更新其网络设备编码信息
		if(pNetDevice[i].bUseDefaultCfg && ((pNetDevice[i].channel >= m_netVideoInputNum) && (pNetDevice[i].channel < m_videoInputNum)))
		{
			//主码流
			unsigned char chnn = pNetDevice[i].channel;
			pEncodeInfo[chnn].resolution = CProduct::Instance()->IPCDefVideoSize();
			pEncodeInfo[chnn].rate = CProduct::Instance()->IPCDefaChnnRate();
			pEncodeInfo[chnn].hBitStream = CProduct::Instance()->IPCDefaBitRateValue();

			if(g_video_format == VIDEO_FORMAT_NTSC)
			{
				pEncodeInfo[chnn].rate = (pEncodeInfo[chnn].rate + 1) * 6 / 5;
				if (30 > pEncodeInfo[chnn].rate)
				{
					pEncodeInfo[chnn].rate = 30;
				}
			}
			bNeedWriteCfg = true;

			if (NULL != g_pStreamProc)
			{
				g_pStreamProc->SetNetVideoFRate(chnn - m_localVideoInputNum, pEncodeInfo[chnn].rate, pEncodeInfo[chnn].hBitStream);
			}

			//子码流
			pEncodeInfoMinor[chnn].resolution = CProduct::Instance()->IPCNetDefVideoSize();
			pEncodeInfoMinor[chnn].rate = CProduct::Instance()->IPCNetDefaChnnRate();
			pEncodeInfoMinor[chnn].hBitStream = CProduct::Instance()->IPCNetDefaBitRateValue();

			if(g_video_format == VIDEO_FORMAT_NTSC)
			{
				pEncodeInfoMinor[chnn].rate = pEncodeInfoMinor[chnn].rate * 6 / 5;
			}
		}
	}

	//保存及应用网络设备信息
	m_pCfgNetDeviceMan->SetIPDeviceInfo(pNetDevice, count);

	//当4路POE设备，接了2路POE和2路非POE设备，在改IP地址后如果删除两路非POE设备会出现连接4个IPC的情况，实际上只应该有2路设备
	//if (index > count)
	//{
	//	index = count;
	//}

	bool bRet = m_pRegDeviceLog->SetNetDevice(pNetLinkInfo, index);
	
	list<NET_DEVICE_LINK_INFO> netLinkList;
	m_pRegDeviceLog->GetChBindNetDevice(netLinkList);
	int i = 0;
	for(list<NET_DEVICE_LINK_INFO>::iterator it = netLinkList.begin(); it != netLinkList.end(); it++)
	{
		NET_DEVICE_LINK_INFO &linkInfo = *it;
		memcpy(&pNetLinkInfoEx[i].linkInfo, &linkInfo, sizeof(NET_DEVICE_LINK_INFO));
		pNetLinkInfoEx[i].status = ND_STATUS_DISCONNECT;
		if(linkInfo.channel >= m_poeInputNum)
		{
			pNetLinkInfoEx[i].bPOEOnLine = true;
		}
		m_tempLinkInfoList.push_back(pNetLinkInfoEx[i]);
		i++;
	}
	netLinkList.clear();


	//更新配置信息
	if(bNeedWriteCfg)
	{
		CConfigEx::Instance()->SetConfig(pDataBlock, blockLen);
	}

	delete [] pDataBlock;
	delete [] pNetLinkInfo;
return bRet;
}

bool CNetDeviceManager::SetNetDevice(IP_DEVICE_INFO *pNetDevice, unsigned long num)
{
	m_tempLinkInfoList.clear();
	//设置网络设备信息时，需要把使用默认值编码参数的配置写入配置文件
	CMyList<NCFG_ITEM_ID>	idList;
	idList.AddTail(NCFG_ITEM_ENCODE_MAJOR_EX);
	idList.AddTail(NCFG_ITEM_ENCODE_MINOR_EX);
	unsigned char *pDataBlock = NULL;
	unsigned long blockLen = 0;
	CConfigEx::Instance()->GetConfigEx(idList, &pDataBlock, blockLen);
	ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(pDataBlock + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * 2);
	ENCODE_INFO_EX *pEncodeInfoMinor = pEncodeInfo + m_videoInputNum;
	int maxRate = (g_video_format == VIDEO_FORMAT_NTSC) ? 30 : 25;
	bool bNeedWriteCfg = false;
	bool bCurDebug = true;

	/*设置所有网络设备*/
	unsigned long count = m_pRegDeviceLog->GetMaxDeviceCount();
	count = (count >= num) ? num : count;

	unsigned int total_alloc_chn_cnt = count + m_poeInputNum + 1;

	NET_DEVICE_LINK_INFO *pNetLinkInfo = new NET_DEVICE_LINK_INFO[total_alloc_chn_cnt];
	memset(pNetLinkInfo, 0, sizeof(NET_DEVICE_LINK_INFO)*(total_alloc_chn_cnt));
	//表示当前是否有poe设备
	bool bPoeMustLinkOnline[64];
	memset( bPoeMustLinkOnline, 0, sizeof(bPoeMustLinkOnline) );

	//保存已经存在的POE上IPC列表
	NET_DEVICE_LINK_INFO *pPoeNetLinkInfo = new NET_DEVICE_LINK_INFO[total_alloc_chn_cnt];
	memset(pPoeNetLinkInfo, 0, sizeof(NET_DEVICE_LINK_INFO)*(total_alloc_chn_cnt));


	NET_LINK_INFO_EX *pNetLinkInfoEx = new NET_LINK_INFO_EX[total_alloc_chn_cnt];
	memset(pNetLinkInfoEx, 0, sizeof(NET_LINK_INFO_EX)*(total_alloc_chn_cnt));

	while( m_bLinkInfoChanged || m_bPoeLinkChg )
	{
		DVRSleep(20);
	}
	printf( "current netlink change is over(%d,%d,%d) %s,%d\n", count, num, total_alloc_chn_cnt,__FILE__, __LINE__ );
	int x = 0;
	int i = 0;

	if( bCurDebug )
	{
		for( x = 0; x < num; x++ )
		{
			printf( "net input ipc setnet %d, %s,%s,%s\n", pNetDevice[x].channel, 
				pNetDevice[x].szServer, pNetDevice[x].szID, pNetDevice[x].szEtherName );
		}
	}


	//获取已经存在的POE上IPC的列表
	m_pRegDeviceLog->GetNetDevice(pPoeNetLinkInfo, m_poeInputNum);

	for( i = 0; i < m_poeInputNum; i++)
	{
		InitPoeChnnParam( pNetLinkInfo[i], i );
	}

	int index = m_poeInputNum;
	int netlink_index = 0;
	bool bFindSameChnIpc = false;
	for( i = 0; i < count; i++)
	{
		// 注意这里界面使用的是INVALID_INDEX(unsigned char)表示没有使用的通道
		if(INVALID_INDEX(unsigned char) != pNetDevice[i].channel && pNetDevice[i].channel < m_poeInputNum)
		{
			netlink_index = pNetDevice[i].channel;
		}
		else
		{
			bFindSameChnIpc = false;
			for( x = m_poeInputNum; x < index; x++ )
			{
				// 注意这里界面使用的是INVALID_INDEX(unsigned char)表示没有使用的通道
				if( pNetDevice[i].channel != INVALID_INDEX(unsigned char) && pNetDevice[i].channel == pNetLinkInfo[x].channel )
				{
					printf( "current channel error %d,%d -- %s,%d\n", i, pNetDevice[i].channel, __FILE__, __LINE__ );
					netlink_index = x;
					bFindSameChnIpc = true;
					break;
				}
			}

			if( !bFindSameChnIpc )
			{
				netlink_index = index;
				index++;
			}
		}

		pNetLinkInfo[netlink_index].deviceID = pNetDevice[i].deviceID;
		pNetLinkInfo[netlink_index].channel = pNetDevice[i].channel;
		pNetLinkInfo[netlink_index].nPort= pNetDevice[i].nPort;
		pNetLinkInfo[netlink_index].nHttpPort = pNetDevice[i].nHttpPort;
		pNetLinkInfo[netlink_index].nCtrlPort= pNetDevice[i].nCtrlPort;
		strcpy(pNetLinkInfo[netlink_index].szID, pNetDevice[i].szID);
		strcpy(pNetLinkInfo[netlink_index].szServer, pNetDevice[i].szServer);
		strcpy(pNetLinkInfo[netlink_index].szEtherName, pNetDevice[i].szEtherName);
		strcpy(pNetLinkInfo[netlink_index].username, pNetDevice[i].username);
		strcpy(pNetLinkInfo[netlink_index].password, pNetDevice[i].password);
		pNetLinkInfo[netlink_index].manufacturerId = pNetDevice[i].manufacturerId;
		strcpy(pNetLinkInfo[netlink_index].manufacturerName, pNetDevice[i].manufacturerName);
		strcpy(pNetLinkInfo[netlink_index].productModel, pNetDevice[i].productModel);

		if(INVALID_INDEX(unsigned char) != pNetDevice[i].channel && pNetDevice[i].channel < m_poeInputNum)
		{
			unsigned long deviceIP = inet_addr(pNetDevice[i].szServer);
			if((deviceIP & m_switchMask) != (m_switchIP & m_switchMask))
			{
				strcpy(pNetLinkInfo[netlink_index].szEtherName, "eth0");
			}

			//如果是eth1设备，则认为存在poe设备
			if( IsPoeDevice( pNetLinkInfo[netlink_index] ) )
			{
				bPoeMustLinkOnline[netlink_index] = true;
			}
		}

		//对使用默认配置的网络设备，更新其网络设备编码信息
		if(pNetDevice[i].bUseDefaultCfg && (INVALID_INDEX(unsigned char) != pNetDevice[i].channel && pNetDevice[i].channel < m_videoInputNum))
		{
			//主码流
			unsigned char chnn = pNetDevice[i].channel;
			pEncodeInfo[chnn].resolution = CProduct::Instance()->IPCDefVideoSize();
			pEncodeInfo[chnn].rate = CProduct::Instance()->IPCDefaChnnRate();
			pEncodeInfo[chnn].hBitStream = CProduct::Instance()->IPCDefaBitRateValue();

			if(g_video_format == VIDEO_FORMAT_NTSC)
			{
				pEncodeInfo[chnn].rate = (pEncodeInfo[chnn].rate + 1) * 6 / 5;
				if (30 < pEncodeInfo[chnn].rate)
				{
					pEncodeInfo[chnn].rate = 30;
				}
			}
			bNeedWriteCfg = true;

			if (NULL != g_pStreamProc)
			{
				g_pStreamProc->SetNetVideoFRate(chnn - m_localVideoInputNum, pEncodeInfo[chnn].rate, pEncodeInfo[chnn].hBitStream);
			}

			//子码流
			pEncodeInfoMinor[chnn].resolution = CProduct::Instance()->IPCNetDefVideoSize();
			pEncodeInfoMinor[chnn].rate = CProduct::Instance()->IPCNetDefaChnnRate();
			pEncodeInfoMinor[chnn].hBitStream = CProduct::Instance()->IPCNetDefaBitRateValue();

			if(g_video_format == VIDEO_FORMAT_NTSC)
			{
				pEncodeInfoMinor[chnn].rate = pEncodeInfoMinor[chnn].rate * 6 / 5;
			}

			unsigned long chnnMaxRate = (VIDEO_FORMAT_NTSC == g_video_format) ? 30 : 25;

			if (!CProduct::Instance()->IsDispD1Device())
			{
				//第三码流初始化
#if defined(__CUSTOM_IL03__)
				m_pThirdStreamEncode[chnn].encodeType = VIDEO_ENCODE_MODE_VBR;
#else
				m_pThirdStreamEncode[chnn].encodeType = VIDEO_ENCODE_MODE_CBR;
#endif
				m_pThirdStreamEncode[chnn].lBitStream = 32;
#if defined(__CUSTOM_IL03__)
				m_pThirdStreamEncode[chnn].hBitStream = 256;
#else
				m_pThirdStreamEncode[chnn].hBitStream = 512;
#endif
				m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_CIF;
				m_pThirdStreamEncode[chnn].rate = chnnMaxRate;
			}
			else
			{
				if (16 >= m_netVideoInputNum)
				{
					m_pThirdStreamEncode[chnn].encodeType = VIDEO_ENCODE_MODE_CBR;
					m_pThirdStreamEncode[chnn].lBitStream = 32;

#if defined(__CUSTOM_IL03__)
					m_pThirdStreamEncode[chnn].hBitStream = 512;
#else
					m_pThirdStreamEncode[chnn].hBitStream = 1024;
#endif
					m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_D1;
					m_pThirdStreamEncode[chnn].rate = chnnMaxRate;
				}
				else
				{
					//第三码流初始化
#if defined(__CUSTOM_IL03__)
					m_pThirdStreamEncode[chnn].encodeType = VIDEO_ENCODE_MODE_VBR;
#else
					m_pThirdStreamEncode[chnn].encodeType = VIDEO_ENCODE_MODE_CBR;
#endif
					m_pThirdStreamEncode[chnn].lBitStream = 32;
#if defined(__CUSTOM_IL03__)
					m_pThirdStreamEncode[chnn].hBitStream = 256;
#else
					m_pThirdStreamEncode[chnn].hBitStream = 512;
#endif
					m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_CIF;
					m_pThirdStreamEncode[chnn].rate = chnnMaxRate;
				}
			}

			m_pThirdDevInfo[chnn].videoSize = DVRVIDEO_SIZE_QCIF | DVRVIDEO_SIZE_QVGA | DVRVIDEO_SIZE_CIF | DVRVIDEO_SIZE_HD1 | DVRVIDEO_SIZE_VGA \
				| DVRVIDEO_SIZE_D1 | DVRVIDEO_SIZE_960H | DVRVIDEO_SIZE_HD720 | DVRVIDEO_SIZE_HD1080 | DVRVIDEO_SIZE_960P | DVRVIDEO_SIZE_3M;
			m_pThirdDevInfo[chnn].videoSize |= DVRVIDEO_SIZE_16_9_3M | DVRVIDEO_SIZE_4M | DVRVIDEO_SIZE_5M;

			memset(m_pThirdDevInfo[chnn].ucRate, (VIDEO_FORMAT_NTSC == g_video_format) ? 30 : 25, sizeof(m_pThirdDevInfo[chnn].ucRate));
		}
	}

	//保存及应用网络设备信息
	m_pCfgNetDeviceMan->SetIPDeviceInfo(pNetDevice, count);

	if( bCurDebug )
	{
		for( x = 0; x < index; x++ )
		{
			printf( "net ipc prev no chg setnet %d, %s,%s,%s,%d -- %d\n", pNetLinkInfo[x].channel, 
				pNetLinkInfo[x].szServer, pNetLinkInfo[x].szID, pNetLinkInfo[x].szEtherName, 
				bPoeMustLinkOnline[x], __LINE__ );
		}
		printf( "\n" );

		for( x = 0; x < m_poeInputNum; x++ )
		{
			printf( "net poe ipc newstatus setnet %d, %s,%s,%s,%d -- %d\n", pPoeNetLinkInfo[x].channel, 
				pPoeNetLinkInfo[x].szServer, pPoeNetLinkInfo[x].szID, pPoeNetLinkInfo[x].szEtherName, 
				bPoeMustLinkOnline[x], __LINE__ );
		}
		printf( "\n" );
	}


	bool bUserIsChangePoeChnnPos = false;

	//处理在配置期间，有poe设备变化
	if( m_poeInputNum > 0 )
	{
		//检测用户是否更改POE的通道位置
		for( x = 0; x < m_poeInputNum; x++ )
		{
			if( IsPoeDevice(pNetLinkInfo[x]) && bPoeMustLinkOnline[x] )
			{
				if( 0 != strcmp( pNetLinkInfo[x].szID, pPoeNetLinkInfo[x].szID ) )
				{
					bUserIsChangePoeChnnPos = true;
				}
			}
		}

		bool bFindExistPoeSn = false;
		bool bPoeIpcIsOnline = false;
		for( x = 0; x < m_poeInputNum; x++ )
		{
			if( x != pPoeNetLinkInfo[x].channel )
			{
				printf( "cur channel error ---------------- %d,%d,%s,%d\n", x, pPoeNetLinkInfo[x].channel, __FILE__, __LINE__ );
			}

			//已经存在的列表如果是POE口，确认POE口上存在设备
			if ( IsPoeDevice(pPoeNetLinkInfo[x]) && strlen( pPoeNetLinkInfo[x].szID) <= 0)
			{
				continue;
			}

			//已经存在的列表如果是非POE设备
			if( !IsPoeDevice( pPoeNetLinkInfo[x]) )
			{
				continue;
			}


			//获取已经存在的列表当前设备的是否在线状态
			//list<NET_LINK_INFO_EX>::iterator itz;
			for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
			{
				if(m_netLinkInfo[netCnt].pNetDevice != NULL)
				{
					if(m_netLinkInfo[netCnt].linkInfo.channel == x )
					{
						bPoeIpcIsOnline = m_netLinkInfo[netCnt].bPOEOnLine;
						break;
					}
				}
			}

			//搜索新列表中是否已经存在sn
			bFindExistPoeSn = false;
			for( i = 0; i < m_poeInputNum; i++ )
			{
				if( 0 == strcmp( pPoeNetLinkInfo[x].szID, pNetLinkInfo[i].szID ) )
				{
					bFindExistPoeSn = true;
					break;
				}
			}
			
			//如果没有找到，则认为要么是新的sn，要么是之前没有激活的sn
			if( !bFindExistPoeSn )
			{
				//获取新列表中对应通道的IPC状态
				bool bPoeOnline = bPoeMustLinkOnline[x];
				if( !bPoeOnline )
				{
					if( !IsPoeDevice(pNetLinkInfo[x]))
					{
						bPoeOnline = true;
					}
				}

				//新列表中当前通道已经打开poe
				if( bPoeOnline )
				{
					//如果已经存在的列表中当前通道上已经存在poe设备,并且激活状态， 则需要在新列表中寻找新的通道
					if( bPoeIpcIsOnline )
					{
						int mmx = 0;
						int find_new_chn = -1;
						for( mmx = 0; mmx < m_poeInputNum; mmx++ )
						{
							//检测当前没有使用的poe通道，并且没有eth0设备
							if( !bPoeMustLinkOnline[mmx] && IsPoeDevice(pNetLinkInfo[mmx]))
							{
								//如果没有sn的优先使用
								if( strlen(pNetLinkInfo[mmx].szID) <= 0 )
								{
									find_new_chn = mmx;
									break;
								}
								else
								{
									find_new_chn = mmx;
								}
							}
						}

						if( -1 != find_new_chn )
						{
							memcpy( &pNetLinkInfo[find_new_chn], &pPoeNetLinkInfo[x], sizeof(NET_DEVICE_LINK_INFO) );
							pNetLinkInfo[find_new_chn].channel = find_new_chn;
							bPoeMustLinkOnline[find_new_chn] = true;

							printf( "cur ipc %s add to not channel (%d,%d), %s,%d\n", 
								pPoeNetLinkInfo[x].szID, find_new_chn, x, __FILE__, __LINE__ );
						}
						else
						{
							//当前IPC认为丢失
							printf( "cur ipc %s loss %s,%d\n", pPoeNetLinkInfo[x].szID, __FILE__, __LINE__ );
						}
					}
					else
					{
						//如果当前IPC未激活，则认为丢失
						printf( "cur ipc channel is used %s loss %s,%d\n", pPoeNetLinkInfo[x].szID, __FILE__, __LINE__ );
					}
				}
				else
				{
					//当前新列表中poe通道上没有poe设备
					memcpy( &pNetLinkInfo[x], &pPoeNetLinkInfo[x], sizeof(NET_DEVICE_LINK_INFO) );
					//如果为激活状态，则同样为激活状态
					if( bPoeIpcIsOnline )
					{
						bPoeMustLinkOnline[x] = true;
					}

					printf( "cur ipc same channel exist %s,%d-- %s,%d\n", pPoeNetLinkInfo[x].szID, bPoeMustLinkOnline[x], __FILE__, __LINE__ );
				}

			}
			else
			{
				//如果存在相同的sn，则确认Poe的状态
				if( bPoeIpcIsOnline )
				{
					bPoeMustLinkOnline[i] = true;
				}
				else
				{
					bPoeMustLinkOnline[i] = false;
				}
				printf( "cur ipc sn yiyang %s,%d,%s,%d \n", pNetLinkInfo[i].szID, bPoeMustLinkOnline[i], __FILE__, __LINE__ );
			}
		}

		//处理非Poe通道
		for( x = 0; x < m_poeInputNum; x++ )
		{
			if( bPoeMustLinkOnline[x] )
			{
				//去掉非poe中的和poe通道相同sn或者ip
				i = m_poeInputNum;
				while( i < index )
				{
					if( 0 == strcmp( pNetLinkInfo[x].szID, pNetLinkInfo[i].szID)
						|| 0 == strcmp( pNetLinkInfo[x].szServer, pNetLinkInfo[i].szServer ) )
					{
						printf( "cur ipc same ---- will delete %s,%s,%d,%d,%s,%d\n",
							pNetLinkInfo[x].szID, pNetLinkInfo[i].szID, x, i, __FILE__, __LINE__ );
						if( i == index-1 )
						{
							index -= 1;
							break;
						}
						else
						{
							memcpy( &pNetLinkInfo[i], &pNetLinkInfo[index-1], sizeof(NET_DEVICE_LINK_INFO) );
							pNetLinkInfo[index-1].szID[0] = '\0';
							pNetLinkInfo[index-1].szServer[0] = '\0';
							index -= 1;
						}
					}

					i++;
				}
			}
		}
	}

	if( bCurDebug )
	{
		for( x = 0; x < index; x++ )
		{
			printf( "net ipc setnet %d, %s,%s,%s,%d -- %d\n", pNetLinkInfo[x].channel, 
				pNetLinkInfo[x].szServer, pNetLinkInfo[x].szID, pNetLinkInfo[x].szEtherName, 
				bPoeMustLinkOnline[x], __LINE__ );
		}
	}

	bool bRet = m_pRegDeviceLog->SetNetDevice(pNetLinkInfo, index);
	
	//获取已经绑定的通道IPC
	list<NET_DEVICE_LINK_INFO> netLinkList;
	m_pRegDeviceLog->GetChBindNetDevice(netLinkList);

	i = 0;
	for(list<NET_DEVICE_LINK_INFO>::iterator it = netLinkList.begin(); it != netLinkList.end(); it++)
	{
		NET_DEVICE_LINK_INFO &linkInfo = *it;

		memcpy(&pNetLinkInfoEx[i].linkInfo, &linkInfo, sizeof(NET_DEVICE_LINK_INFO));
		pNetLinkInfoEx[i].status = ND_STATUS_DISCONNECT;

		//确认但前通道号是在输入IPC参数中, 如果不确认有可能出现没有连接的POE设备
		//如果确定是POE设备
		if( bPoeMustLinkOnline[linkInfo.channel] && IsPoeDevice( pNetLinkInfoEx[i].linkInfo ) )
		{
			pNetLinkInfoEx[i].bPOEOnLine = true;
		}

		m_tempLinkInfoList.push_back(pNetLinkInfoEx[i]);

		if( bCurDebug )
		{
			printf( "net get will set ipc setnet %d, %s,%s,%s --%s,%s -- %d\n", pNetLinkInfoEx[i].linkInfo.channel, 
				pNetLinkInfoEx[i].linkInfo.szServer, pNetLinkInfoEx[i].linkInfo.szID, 
				pNetLinkInfoEx[i].linkInfo.szEtherName, pNetLinkInfoEx[i].linkInfo.username, "password",
				pNetLinkInfoEx[i].bPOEOnLine );
		}

		i++;
	}

	NotifyRegDeviceChanged1();

	m_bLinkInfoChanged = true;

	netLinkList.clear();


	//在设备列表锁之后，防止死锁
	if( m_poeInputNum > 0 )
	{
		m_poeOnvifDeviceMan.ChangeChannelSn(m_pPoeChannelSn,bUserIsChangePoeChnnPos);

		m_pRegDeviceLog->SetPoeUsedPtoPMode( m_poeOnvifDeviceMan.GetPoeUsedMode() );
	}


	//更新配置信息
	if(bNeedWriteCfg)
	{
		CConfigEx::Instance()->SetConfig(pDataBlock, blockLen);
	}

	delete [] pDataBlock;
	delete [] pNetLinkInfo;
	delete [] pNetLinkInfoEx;
	delete [] pPoeNetLinkInfo;

	return bRet;
}

void CNetDeviceManager::GetNetDeviceEffect(std::list<NET_DEVICE_LINK_INFO> &linkInfoList)
{
	/*
		获取有效网络设备[绑定了通道的为有效网络设备]
	*/
#if 1
	m_pRegDeviceLog->GetChBindNetDevice(linkInfoList);
#else
	linkInfoList.clear();

	m_deviceListLock.Lock();
	list<NET_LINK_INFO_EX>::iterator it;
	if(!m_tempLinkInfoList.empty())
	{
		for(it = m_tempLinkInfoList.begin(); it != m_tempLinkInfoList.end(); it++)
		{
			NET_LINK_INFO_EX &netlinkInfoEx = *it;
			if(!IsPoeDevice(netlinkInfoEx.linkInfo) || netlinkInfoEx.bPOEOnLine)
			{
				linkInfoList.push_back(netlinkInfoEx.linkInfo);
			}
		}
	}
	else
	{
		for(it = m_listNetLinkInfo.begin(); it != m_listNetLinkInfo.end(); it++)
		{
			NET_LINK_INFO_EX &netlinkInfoEx = *it;
			if(!IsPoeDevice(netlinkInfoEx.linkInfo) || netlinkInfoEx.bPOEOnLine)
			{
				linkInfoList.push_back(netlinkInfoEx.linkInfo);
			}
		}
	}
	m_deviceListLock.UnLock();
#endif
}

/*获取所有网络设备*/
unsigned long CNetDeviceManager::GetNetDevice(IP_DEVICE_INFO *pNetDevice, unsigned long num)
{
	//等待所有更改已经生效
	while((!IfCheckStatusOver())||(m_bLinkInfoChanged || m_bPoeLinkChg ))
	{
		DVRSleep(20);
	}
	printf( "current getnetdevice netlink change is over %s,%d\n", __FILE__, __LINE__ );

	NET_DEVICE_LINK_INFO *pLinkInfo = new NET_DEVICE_LINK_INFO[num];
	int count = m_pRegDeviceLog->GetNetDevice(pLinkInfo, num);

	list<NET_LINK_INFO_EX>::iterator it;
//	list<NET_LINK_INFO_EX> *pLinkList = &m_tempLinkInfoList;
	bool bUseNetLinkFlag = true;
//	if(m_tempLinkInfoList.empty())
//	{
//		bUseNetLinkFlag = true;
//	}
	int i = 0, j = 0;
	long netCnt = 0;
	for(; i < count; i++)
	{
		for(netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
		{
			if(m_netLinkInfo[netCnt].pNetDevice != NULL)
			{
				if(m_netLinkInfo[netCnt].linkInfo.enabled && (m_netLinkInfo[netCnt].linkInfo.channel == pLinkInfo[i].channel))
				{
					break;
				}
			}
		}
	
		if(netCnt != m_netVideoInputNum)
		{
			if(IsPoeDevice(m_netLinkInfo[netCnt].linkInfo) && !m_netLinkInfo[netCnt].bPOEOnLine)
			{
				continue;
			}
		}
		else
		{
		}
		

		
		printf("pLinkInfo[%d].szServer = %s \n",pLinkInfo[i].channel+1,pLinkInfo[i].szServer);
		if(pLinkInfo[i].channel < m_poeInputNum)
		{
			pNetDevice[j].deviceID  =INVALID_INDEX(unsigned long);
		}
		else
		{
			pNetDevice[j].deviceID = pLinkInfo[i].deviceID;
		}
		pNetDevice[j].channel = pLinkInfo[i].channel;
		pNetDevice[j].nPort= pLinkInfo[i].nPort;
		pNetDevice[j].nHttpPort = pLinkInfo[i].nHttpPort;
		pNetDevice[j].nCtrlPort= pLinkInfo[i].nCtrlPort;
		strcpy(pNetDevice[j].szID, pLinkInfo[i].szID);
		strcpy(pNetDevice[j].szServer, pLinkInfo[i].szServer);
		strcpy(pNetDevice[j].szEtherName, pLinkInfo[i].szEtherName);
		strcpy(pNetDevice[j].username, pLinkInfo[i].username);
		strcpy(pNetDevice[j].password, pLinkInfo[i].password);
		pNetDevice[j].manufacturerId = pLinkInfo[i].manufacturerId;
		strcpy(pNetDevice[j].manufacturerName, pLinkInfo[i].manufacturerName);
		strcpy(pNetDevice[j].productModel, pLinkInfo[i].productModel);

		pNetDevice[j].bUseDefaultCfg = pLinkInfo[i].bUseDefaultCfg;
		assert(pLinkInfo[i].enabled);

		if(netCnt != m_netVideoInputNum)
		{
			pNetDevice[j].status = m_netLinkInfo[netCnt].status;
			strcpy(pNetDevice[j].productModel, m_netLinkInfo[netCnt].linkInfo.productModel);
			strcpy(pNetDevice[j].szID, m_netLinkInfo[netCnt].linkInfo.szID);
			strcpy(pNetDevice[j].username, m_netLinkInfo[netCnt].linkInfo.username);
			strcpy(pNetDevice[j].password, m_netLinkInfo[netCnt].linkInfo.password);
		}

		if(IsPoeDevice(pLinkInfo[i]))
		{
			pNetDevice[j].bPOEDevice = 1;
		}
		else
		{
			pNetDevice[j].bPOEDevice = 0;
		}

		j++;
	}

//	UnLockAllDeviceList();

	delete [] pLinkInfo;

	return j;
}

unsigned long CNetDeviceManager::GetNetDeviceCount()
{
	/*
		获取所有有网路设备数目
	*/
	return m_pRegDeviceLog->GetNetDeviceCount();
}

unsigned long CNetDeviceManager::GetNetDeviceMax()
{
	/*
		获取最大可以配置的网络设备个数
	*/
	return m_pRegDeviceLog->GetMaxDeviceCount();
}

CNetDevice* CNetDeviceManager::GetNetDeviceByChnn(unsigned char channel, bool bConnectOnly)
{
	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		if(m_netLinkInfo[netCnt].pNetDevice != NULL)
		{
			if(bConnectOnly && (m_netLinkInfo[netCnt].status != ND_STATUS_OK))
			{
				//如果只要当前连接的网络设备
				continue;
			}
			if(m_netLinkInfo[netCnt].linkInfo.channel == channel)
			{
				return m_netLinkInfo[netCnt].pNetDevice;
			}
		}
	}

	return NULL;
}

bool CNetDeviceManager::IsNetDeviceHaveConnect(ND_DEVICE_INFO * pNDDeviceInfo)
{

	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		if(m_netLinkInfo[netCnt].pNetDevice != NULL)
		{
			if(strcmp(m_netLinkInfo[netCnt].linkInfo.szServer, pNDDeviceInfo->szAddr) == 0)
			{
				return true;
			}
		}
	}

	return false;
}

long CNetDeviceManager::GetActiveCount(bool bConnectOnly)
{
	int count = 0;
	if(bConnectOnly)
	{
		for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
		{
			if(m_netLinkInfo[netCnt].pNetDevice != NULL)
			{
				if(m_netLinkInfo[netCnt].status == ND_STATUS_OK)
				{
					count++;
				}
			}
		}
	}
	else
	{
		for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
		{
			if(m_netLinkInfo[netCnt].pNetDevice != NULL)
			{
					count++;
			}
		}
	}
	return count;
}

bool CNetDeviceManager::NewNetDevice(NET_LINK_INFO_EX &netLinkInfoEx)
{
	bool bRet = false;
	netLinkInfoEx.status = ND_STATUS_DISCONNECT;
	netLinkInfoEx.disconnectTime = 0;
	netLinkInfoEx.bWriteConnectLog = 1;

	netLinkInfoEx.pNetDevice = m_pNdLibMan->CreateNetDevice(netLinkInfoEx.linkInfo.manufacturerId);
	netLinkInfoEx.pNetDevice->SetDataBufferMan(m_dataBufferMan);

	if (32 <= m_netVideoInputNum)
	{
		netLinkInfoEx.pNetDevice->SetUseBaseLine(true);
	}

	if(netLinkInfoEx.linkInfo.channel < m_poeInputNum)
	{
#ifdef __CUSTOM_SHANGHAI__
		netLinkInfoEx.pNetDevice->SetNetTimeout(4000);
#else
		netLinkInfoEx.pNetDevice->SetNetTimeout(10000);
#endif
	}
	else
	{
#ifdef __CUSTOM_SHANGHAI__
		netLinkInfoEx.pNetDevice->SetNetTimeout(4000);
#else
		netLinkInfoEx.pNetDevice->SetNetTimeout(10000);
#endif
	}

	return bRet;
}

void CNetDeviceManager::DelNetDevice(NET_LINK_INFO_EX &netLinkInfoEx)
{
	CNetDevice *pNetDevice = netLinkInfoEx.pNetDevice;

	if(pNetDevice != NULL)
	{
		m_ullMinorCH &= ~(0x01<<netLinkInfoEx.linkInfo.channel);

		pNetDevice->RequestStream(0, 0, 0, 0);
		pNetDevice->RequestJpegStream(0);
		pNetDevice->Quit();

		m_pNdLibMan->DestroyNetDevice(pNetDevice);
	}
}

RESULT WINAPI CNetDeviceManager::ConnectThread(void * pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	int pid = GetThisThreadId();
	printf("%s, %d, pid = %d\n", __FILE__, __LINE__, pid);
#endif

	CNetDeviceManager *pThis = (CNetDeviceManager *)pParam;
	
	pThis->ConnectProc();
	
	return 0;
}

void CNetDeviceManager::ConnectProc()
{
//	ULONGLONG ullNetStatus = 0;
//	ULONGLONG ullPoeOnlineStatus = 0;
	unsigned long onlineCHNum = 0, prevOnlineNum = 0;
	ULONGLONG ullCanSnapPicCH = 0;

	
	m_dataBufferMan[NET_DEVICE_MAIN_STREAM].SetOneBlockSize(256*1024*m_netVideoInputNum);
	m_dataBufferMan[NET_DEVICE_SUB_STREAM].SetOneBlockSize(32*1024*m_netVideoInputNum);
	if(CProduct::Instance()->IPCSupportDisplayMinor())
	{
		m_dataBufferMan[NET_DEVICE_MINOR_STREAM].SetOneBlockSize(16*1024*m_netVideoInputNum);
	}
	
	int netDevThreadCnt = CProduct::Instance()->NetVideoInputNum();
	
	#pragma omp parallel for num_threads(netDevThreadCnt) 
	for(int i=0;i<netDevThreadCnt;i++)
	{
		while(m_bRunning)
		{
			m_deviceListLock2.Lock();
			RefreshTempNetLinkInfo(i);
			RefreshNetLinkInfo(m_netLinkInfo[i],i);
			m_deviceListLock2.UnLock();
			
			NET_LINK_INFO_EX &linkInfoEx = m_netLinkInfo[i];
			if((linkInfoEx.pNetDevice == NULL))
			{
				PUB_Sleep(1000);
				continue;
			}
			if((linkInfoEx.status == ND_STATUS_OK) && (linkInfoEx.pNetDevice->GetDevStatus() == ND_STATUS_DISCONNECT))
			{
				if(IsPoeDevice(linkInfoEx.linkInfo))
				{
					linkInfoEx.disconnectTime = GetTickCount();
					linkInfoEx.disconnectTime = (linkInfoEx.disconnectTime != 0) ? linkInfoEx.disconnectTime : 1;
				}
				else
				{
					WriteLog(m_clientID, LOG_TYPE_IPCAM_DISCONNECTED, linkInfoEx.linkInfo.szServer, GetCurrTime32());
					linkInfoEx.disconnectTime = 0;
					linkInfoEx.bWriteConnectLog = true;
				}

				m_refreshDisplayLock.Lock();
				if(!m_bRefreshDisplay)
				{
					m_checkRefreshTime = GetCurrTime32();
					//m_bRefreshDisplay = true;
				}
				if(IsPoeDevice(linkInfoEx.linkInfo) && !linkInfoEx.bPOEOnLine)
				{
					m_checkRefreshTime = 0;
				}
				m_refreshDisplayLock.UnLock();

			//	printf("!**! Dectect device disconnect [%d] .............", linkInfoEx.linkInfo.channel);
				PrintfBit32Time(GetCurrTime32());


				m_ullMinorCH &= ~(0x01<<linkInfoEx.linkInfo.channel);

//#ifdef __CUSTOM_SHANGHAI__
				CMessageMan::Instance()->SetNetDeviceStatus(false, linkInfoEx.linkInfo.channel);
//#endif

				linkInfoEx.pNetDevice->Quit();

				m_netLinkInfoLock[i].Lock();
				linkInfoEx.status = ND_STATUS_DISCONNECT;

				m_netLinkInfoLock[i].UnLock();
				
				//if(IsPoeDevice(linkInfoEx.linkInfo))
				//{
				//	//这个端口空闲出来了
				//	m_pPoeChannelSn[linkInfoEx.linkInfo.channel].channel = linkInfoEx.linkInfo.channel;
				//	m_poeOnvifDeviceMan.ChangeChannelSn(m_pPoeChannelSn);
				//}
			}
			if(linkInfoEx.status != ND_STATUS_OK)
			{
				if((linkInfoEx.disconnectTime != 0) && ((GetTickCount() - linkInfoEx.disconnectTime) >= 60000))
				{
					WriteLog(m_clientID, LOG_TYPE_IPCAM_DISCONNECTED, linkInfoEx.linkInfo.szServer, GetCurrTime32() - 60);
					linkInfoEx.disconnectTime = 0;
					linkInfoEx.bWriteConnectLog = true;
				}
			}

			///////modify----更新显示状态
			if(ND_STATUS_OK == linkInfoEx.status)
			{
				CMessageMan::Instance()->SetNetDeviceStatus(true, linkInfoEx.linkInfo.channel);

				onlineCHNum++;
				if (TD_2832PD_C16 != CProduct::Instance()->ProductType())
				{
					if (linkInfoEx.linkInfo.manufacturerId == MID_TVT)
					{
						m_snapPicLock.Lock();
						g_canSnapPicCH |= (ULONGLONG)0x01 << linkInfoEx.linkInfo.channel;
						m_snapPicLock.UnLock();
					}
				}
				else
				{
					m_snapPicLock.Lock();
					g_canSnapPicCH |= (ULONGLONG)0x01 << linkInfoEx.linkInfo.channel;
					m_snapPicLock.UnLock();
				}
			}
			else
			{
				CMessageMan::Instance()->SetNetDeviceStatus(false, linkInfoEx.linkInfo.channel);
				m_resyncTimeCount[linkInfoEx.linkInfo.channel] = 0;
			}

			//如果是POE连接，或者是非POE设备
			if(linkInfoEx.bPOEOnLine || !IsPoeDevice(linkInfoEx.linkInfo))
			{
				CMessageMan::Instance()->SetNetPoeDeviceStatus(false, linkInfoEx.linkInfo.channel);
			}
			else 
			{
				CMessageMan::Instance()->SetNetPoeDeviceStatus(true, linkInfoEx.linkInfo.channel);
			}
			if((linkInfoEx.status != ND_STATUS_OK) && (linkInfoEx.bPOEOnLine || !IsPoeDevice(linkInfoEx.linkInfo) ))
			{
				ND_MAC mac;
				memset(&mac, 0, sizeof(mac));
#ifdef __ENVIRONMENT_LINUX__
				CNetInterface::Instance()->GetMAC((char *)mac.mac);
#endif
				bool bRet;
				if(IsPoeDevice(linkInfoEx.linkInfo))
				{
					if(linkInfoEx.linkInfo.manufacturerId == MID_TVT)
					{
#ifndef __ENVIRONMENT_WIN32__
						const char *pPassword = m_poeDeviceMan.GetDevicePassword(linkInfoEx.linkInfo.szID);
						if(NULL != pPassword)
						{
							CBase64 base64;
							base64.Decode(pPassword, strlen(pPassword), (unsigned char *)linkInfoEx.linkInfo.password, sizeof(linkInfoEx.linkInfo.password));
						}
#endif
#if 0					//因为在搜索时已经过滤掉我们自己IPCAM的ONVIF了，所以这里不可能有相同IP的设备
						//检查有没有相同IP的ONVIF连接，有的话，需要删除
						for(long netCnt=0;netCnt<m_videoInputNum;netCnt++)
						{
							m_netLinkInfoLock[netCnt].Lock();
							NET_LINK_INFO_EX &linkInfoTmp = m_netLinkInfo[netCnt];
							if(/*(netCnt != i)&&*/(linkInfoTmp.linkInfo.manufacturerId == MID_ONVIF) && (strcmp(linkInfoTmp.linkInfo.szServer, linkInfoEx.linkInfo.szServer) == 0))
							{
								if(linkInfoTmp.pNetDevice != NULL)
								{
									m_ullMinorCH &= ~(0x01<<linkInfoTmp.linkInfo.channel);
									linkInfoTmp.pNetDevice->Quit();
									linkInfoTmp.status = ND_STATUS_DISCONNECT;
									linkInfoTmp.bPOEOnLine = false;
									linkInfoTmp.pNetDevice = NULL;
									m_refreshDisplayLock.Lock();
									m_bRefreshDisplay = true;
									m_refreshDisplayLock.UnLock();
								}
								m_netLinkInfoLock[netCnt].UnLock();
								break;
							}
							m_netLinkInfoLock[netCnt].UnLock();
						}
#endif
					}

					printf("!**! linkInfoEx.pNetDevice(%d)[%d]->Initial(%s, %d, %s, %s, %s) start.................\n\n", linkInfoEx.linkInfo.manufacturerId,\
						linkInfoEx.linkInfo.channel, linkInfoEx.linkInfo.szServer, linkInfoEx.linkInfo.nPort, \
						linkInfoEx.linkInfo.username, "psw", linkInfoEx.linkInfo.szEtherName);
					bRet = linkInfoEx.pNetDevice->Initial( m_nextDeviceId++, linkInfoEx.linkInfo.channel, \
															linkInfoEx.linkInfo.szServer, linkInfoEx.linkInfo.nPort, \
															linkInfoEx.linkInfo.nHttpPort, linkInfoEx.linkInfo.nCtrlPort, \
															linkInfoEx.linkInfo.username, linkInfoEx.linkInfo.password, \
															mac, linkInfoEx.linkInfo.szEtherName,
															linkInfoEx.linkInfo.channel - m_localVideoInputNum);
				}
				else
				{
					printf("!**! linkInfoEx.pNetDevice[%d]->Initial(%s)(%s,%s) start.................\n\n", 
						linkInfoEx.linkInfo.channel, linkInfoEx.linkInfo.szServer,
						linkInfoEx.linkInfo.username, "psw" );
					bRet = linkInfoEx.pNetDevice->Initial( m_nextDeviceId++, linkInfoEx.linkInfo.channel, \
						linkInfoEx.linkInfo.szServer, linkInfoEx.linkInfo.nPort, \
						linkInfoEx.linkInfo.nHttpPort, linkInfoEx.linkInfo.nCtrlPort, \
						linkInfoEx.linkInfo.username, linkInfoEx.linkInfo.password, mac, "eth0", 
						linkInfoEx.linkInfo.channel - m_localVideoInputNum);
				}

				if( bRet )
				{
					if( MID_ONVIF == linkInfoEx.linkInfo.manufacturerId )
					{
						if( !CheckVideoResolution( linkInfoEx ) )
						{
							linkInfoEx.pNetDevice->Quit();
							m_netLinkInfoLock[i].Lock();
							linkInfoEx.status = ND_STATUS_DISCONNECT;
							m_netLinkInfoLock[i].UnLock();
							bRet = false;
						}
					}
				}
				
				if(bRet)
				{
					UpdateConfigOnConnect(linkInfoEx);

					//检查是否需要初始化IPCAM的配置
					if(linkInfoEx.pNetDevice->IfNeedInitNetConfig())
					{
						//对刚连接的IPC设置本地的初始化
						m_pCfgNetDeviceMan->UpdateInitData(i,linkInfoEx.pNetDevice);
						m_pCfgNetDeviceMan->SetInitNetConfig(this, linkInfoEx.pNetDevice,i);
						//同步时间
						SyncTime(linkInfoEx.pNetDevice);
					}

					m_netLinkInfoLock[i].Lock();
					linkInfoEx.status = ND_STATUS_OK;

					m_ullSecondCH |= (1<<linkInfoEx.linkInfo.channel);			//是否要加锁
					m_ullMinorCH |= (1<<linkInfoEx.linkInfo.channel);			//是否要加锁

					SetNetDeviceSecondStreamCfg(linkInfoEx.pNetDevice);
					SetNetDeviceThirdStreamCfg(linkInfoEx.pNetDevice);
				
					m_netLinkInfoLock[i].UnLock();

					//开始请求码流
					linkInfoEx.pNetDevice->RequestJpegStream(0x1);

					if(!linkInfoEx.pNetDevice->RequestStream(0x01, 0x01, 0x01, 0x01))
					{
						m_netLinkInfoLock[i].Lock();
						m_ullMinorCH &= ~(0x01<<linkInfoEx.linkInfo.channel);
						m_ullSecondCH &= ~(0x01<<linkInfoEx.linkInfo.channel);
						linkInfoEx.status = ND_STATUS_DISCONNECT;
						linkInfoEx.pNetDevice->Quit();
						m_netLinkInfoLock[i].UnLock();
					}
					else
					{
						//ullNetStatus &= ~((ULONGLONG)0x01 << linkInfoEx.linkInfo.channel);
						//CMessageMan::Instance()->RefreshNetDeviceStatus(ullNetStatus);
						CMessageMan::Instance()->SetNetDeviceStatus(true,linkInfoEx.linkInfo.channel);
						linkInfoEx.disconnectTime = 0;
						if(linkInfoEx.bWriteConnectLog)
						{
							WriteLog(m_clientID, LOG_TYPE_IPCAM_CONNECTED, linkInfoEx.linkInfo.szServer);
							linkInfoEx.bWriteConnectLog = 0;
						}
					}
					//m_deviceListLock.UnLock();

					m_resyncTimeCount[linkInfoEx.linkInfo.channel] = 0;
				}
			}
			else if(linkInfoEx.status == ND_STATUS_OK)
			{
				//已连接 做配置等等
				m_netLinkInfoLock[i].Lock();
				if(m_bSetThirdCfg[i])
				{
					m_bSetThirdCfg[i] = false;
					SetNetDeviceThirdStreamCfg(linkInfoEx.pNetDevice);
				}
				if(m_bSetSecondCfg[i])
				{
					m_bSetSecondCfg[i] = false;
					SetNetDeviceSecondStreamCfg(linkInfoEx.pNetDevice, false);
				}

				{
					m_resyncTimeCount[linkInfoEx.linkInfo.channel]++;

					if( m_resyncTimeCount[linkInfoEx.linkInfo.channel] > 60*60*24 )
					{
						printf( "resync ipc channel %d\n", linkInfoEx.linkInfo.channel );
						linkInfoEx.pNetDevice->ResyncIPCSystemTime( 0 );
						m_resyncTimeCount[linkInfoEx.linkInfo.channel] = 0;
					}
				}

				m_netLinkInfoLock[i].UnLock();
			}
			else if((!linkInfoEx.bPOEOnLine) && IsPoeDevice(linkInfoEx.linkInfo) )
			{

			}
			

		PUB_Sleep(1000);
		}
	}
//	################################################################################
// ############################################################################
	
}

void CNetDeviceManager::RefreshTempNetLinkInfo(int &channl)
{
	if(m_bLinkInfoChanged || m_bPoeLinkChg)	
	{
		m_netLinkInfoLock[channl].Lock();
		for(unsigned char chanCnt=0;chanCnt<m_netVideoInputNum;chanCnt++)
		{
			m_undealChannFlag[chanCnt] = true;
		}
		m_bLinkInfoChanged = false;
		m_bPoeLinkChg = false;
		ClearAllNetLinkInfoEx(m_tempNetLinkInfo);


		for(list<NET_LINK_INFO_EX>::iterator it2 = m_tempLinkInfoList.begin(); it2 != m_tempLinkInfoList.end(); it2++)
		{
			m_tempNetLinkInfo[(*it2).linkInfo.channel-m_localVideoInputNum] = *it2;				
		}
		m_tempLinkInfoList.clear();
		
		m_refreshDisplayLock.Lock();
		m_bRefreshDisplay = true;
		m_checkRefreshTime = 0;
		m_refreshDisplayLock.UnLock();
		m_netLinkInfoLock[channl].UnLock();
	}
}

void CNetDeviceManager::RefreshNetLinkInfo(NET_LINK_INFO_EX &netLinkInfoEx,int &channl)
{
	if((m_undealChannFlag[channl] == true)&&(m_bRunning))
	{
		m_netLinkInfoLock[channl].Lock();


		if((IfChannNetLinkInfoValid(m_tempNetLinkInfo[channl])))
		{
			if(CheckIPCIsSame(m_tempNetLinkInfo[channl].linkInfo, netLinkInfoEx.linkInfo) )
			{//更新信息 不重连
				memcpy(&netLinkInfoEx.linkInfo, &m_tempNetLinkInfo[channl].linkInfo, sizeof(NET_DEVICE_LINK_INFO));
				netLinkInfoEx.POEOnLineTime = m_tempNetLinkInfo[channl].POEOnLineTime;
				if(netLinkInfoEx.bPOEOnLine != m_tempNetLinkInfo[channl].bPOEOnLine)
				{

				}
				netLinkInfoEx.bPOEOnLine = m_tempNetLinkInfo[channl].bPOEOnLine;
			}
			else
			{
				if(netLinkInfoEx.pNetDevice != NULL)
				{//删除设备	
					DelNetDevice(netLinkInfoEx);
				}
				netLinkInfoEx = m_tempNetLinkInfo[channl];
			//	ClearNetLinkInfoEx(netLinkInfoEx);
				netLinkInfoEx.pNetDevice =NULL;
			}
			if(netLinkInfoEx.pNetDevice == NULL)
			{//
				NewNetDevice(netLinkInfoEx);
			}
		}
		else
		{
			if(netLinkInfoEx.pNetDevice != NULL)
			{//删除设备
				
				DelNetDevice(netLinkInfoEx);
				ClearNetLinkInfoEx(netLinkInfoEx);
			}
		}
		m_undealChannFlag[channl] = false;
		m_netLinkInfoLock[channl].UnLock();
	}
}

RESULT WINAPI CNetDeviceManager::TimerThread(void *pParam)
{
	CNetDeviceManager *pThis = (CNetDeviceManager *)pParam;
	int count = 0;

	//unsigned long ulCurTime = GetCurrTime32();
	while(pThis->m_bRunning)
	{
		pThis->m_deviceListLock2.Lock();
		for(long netCnt=0;netCnt<pThis->m_netVideoInputNum;netCnt++)
		{
			if(pThis->m_netLinkInfo[netCnt].pNetDevice != NULL)
			{
				NET_LINK_INFO_EX &linkInfoEx = pThis->m_netLinkInfo[netCnt];
				linkInfoEx.pNetDevice->OnTimer(GetCurrTime32());
			}
		}
		pThis->m_deviceListLock2.UnLock();

		pThis->m_pNdLibMan->OnTimer(GetCurrTime32());

		PUB_Sleep(1000);
	}

	return 0;
}

bool CNetDeviceManager::IfNeedRefresh()
{
	//如果有IPCAM断开需要刷新的话，以10秒一次的频率来进行，防止刷新频率过高时，因刷新过程中需要请求每个通道的关键帧而带来性能的过大消耗
	unsigned long curTime = GetCurrTime32();
	if((curTime < m_checkRefreshTime) || (curTime - m_checkRefreshTime >= 10))
	{
		return m_bRefreshDisplay;
	}
	else
	{
		return false;
	}
}

void CNetDeviceManager::RefreshDisplay()
{
	MESSAGE_DATA msgData, msgDataTemp;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_DISPLAY_INFO, NULL, 0, msgData);
	NET_DISPLAY_INFO* pDisplay = reinterpret_cast<NET_DISPLAY_INFO *>(msgData.pData + sizeof(PACKCMD));
	pDisplay->type = NET_DISPLAY_CHANGE_DATA_PACK;

	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char *)pDisplay, msgData.dataLen, msgDataTemp);
	CMessageMan::Instance()->ReleaseMsgBuff(msgDataTemp);

	m_refreshDisplayLock.Lock();
	m_bRefreshDisplay = false;
	m_refreshDisplayLock.UnLock();

	CMessageMan::Instance()->RefreshLiveTrue();
}

void CNetDeviceManager::SecondStreamMaxRateForFirstStreamNum(unsigned long limitRateForFirstNum)
{
	m_secondLimitRateForFirstStreamNum = limitRateForFirstNum;
}

void CNetDeviceManager::SetSecondStreamLimitRate(unsigned long maxCopies)
{
	m_maxSecondStreamCopies = maxCopies;
}

void CNetDeviceManager::ChangeSecondStreamCH(ULONGLONG secondStreamCH, ULONGLONG secondStreamCH_mobile, unsigned long secondAllSendNum)
{
	m_secondStreamCH = secondStreamCH;
	m_secondStreamCH_mobile = secondStreamCH_mobile;
	m_secondAllSendNum = secondAllSendNum;

	ENCODE_INFO_EX *pOldEncodeInfo = new ENCODE_INFO_EX[m_videoInputNum];
	memcpy(pOldEncodeInfo, m_pSecondStreamEncode, sizeof(ENCODE_INFO_EX) * m_videoInputNum);

	SetSecondStreamEncode();

	//优先处理私有协议接入的通道
	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if(m_netLinkInfo[netCnt].pNetDevice != NULL)
		{
			if(m_netLinkInfo[netCnt].linkInfo.manufacturerId != MID_TVT)
			{
				m_netLinkInfoLock[netCnt].UnLock();
				continue;
			}

			if(m_netLinkInfo[netCnt].status == ND_STATUS_OK)
			{
				unsigned char chnn = m_netLinkInfo[netCnt].pNetDevice->GetBindChannel();
				if(memcmp(&pOldEncodeInfo[chnn], &m_pSecondStreamEncode[chnn], sizeof(ENCODE_INFO_EX)) != 0)
				{
					m_bSetSecondCfg[netCnt] = true;
				}

				if(secondStreamCH & ((ULONGLONG)0x01 << chnn))
				{
					m_netLinkInfo[netCnt].pNetDevice->RequestKeyFrame(0x0, 0x01, 0x0);
				}
			}
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}

	delete [] pOldEncodeInfo;

//	UnLockAllDeviceList();
}

void CNetDeviceManager::SetFirstStreamLimitBitRate(unsigned long maxTotalBitRate)
{
	NCFG_ITEM_HEAD itemHead;
	itemHead.itemID = NCFG_ITEM_ENCODE_MAJOR_EX;

	unsigned char *pConfigBlock;
	unsigned long configLen;

	if(!CConfigEx::Instance()->GetConfig((unsigned char *)&itemHead, sizeof(NCFG_ITEM_HEAD), &pConfigBlock, configLen))
	{
		return;
	}

	ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(pConfigBlock+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD));
	
	unsigned long curTotalBitRate = 0;


	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if(m_netLinkInfo[netCnt].pNetDevice != NULL)
		{
			if(m_netLinkInfo[netCnt].status == ND_STATUS_OK)
			{
				curTotalBitRate += pEncodeInfo[m_netLinkInfo[netCnt].linkInfo.channel].hBitStream;
			}
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}

	if((curTotalBitRate <= maxTotalBitRate) && (curTotalBitRate <= m_maxMainStreamTotalBitRate))
	{
		delete [] pConfigBlock;
		return;
	}

	if(maxTotalBitRate != INVALID_INDEX(unsigned long))	//限制
	{
		for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
		{
			m_netLinkInfoLock[netCnt].Lock();
			if(m_netLinkInfo[netCnt].pNetDevice != NULL)
			{
				if(m_netLinkInfo[netCnt].status == ND_STATUS_OK)
				{
					pEncodeInfo[m_netLinkInfo[netCnt].linkInfo.channel].hBitStream *= maxTotalBitRate;
					pEncodeInfo[m_netLinkInfo[netCnt].linkInfo.channel].hBitStream /= curTotalBitRate;

					pEncodeInfo[m_netLinkInfo[netCnt].linkInfo.channel].hBitStream = AdjustBitStream(pEncodeInfo[m_netLinkInfo[netCnt].linkInfo.channel].hBitStream);
				}
			}
			m_netLinkInfoLock[netCnt].UnLock();
		}
	}
	else	//恢复
	{
		for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
		{
			m_netLinkInfoLock[netCnt].Lock();
			if(m_netLinkInfo[netCnt].pNetDevice != NULL)
			{
				if(m_netLinkInfo[netCnt].status == ND_STATUS_OK)
				{
					pEncodeInfo[m_netLinkInfo[netCnt].linkInfo.channel].lBitStream += 1;
				}
			}
			m_netLinkInfoLock[netCnt].UnLock();
		}
	}

	m_maxMainStreamTotalBitRate = maxTotalBitRate;

	m_pCfgNetDeviceMan->SetNetConfig(this, NULL, pConfigBlock, configLen);

	delete [] pConfigBlock;
}

void CNetDeviceManager::OnNetworkRestart()
{
	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if(m_netLinkInfo[netCnt].pNetDevice != NULL)
		{
			NET_LINK_INFO_EX &linkInfoEx = m_netLinkInfo[netCnt];
			if(ND_STATUS_OK == linkInfoEx.status)
			{
				m_ullMinorCH &= ~(0x01<<linkInfoEx.linkInfo.channel);
				linkInfoEx.pNetDevice->Quit();
				linkInfoEx.status = ND_STATUS_DISCONNECT;
			}
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}

	OPERATOR_LOG		operatorLog;
	memset(&operatorLog, 0, sizeof(OPERATOR_LOG));
	operatorLog.IP = IP_ADDRESS(127, 0, 0, 1);

	strcpy(operatorLog.name, "net_driver");
	operatorLog.contentID = GetContentID(LOG_TYPE_NETWORK_ERR);
	operatorLog.type = (LOG_TYPE_NETWORK_ERR & 0xffff);
	operatorLog.time = GetCurrTime32();
	CLogMan::Instance()->WriteOperatorLog(operatorLog);
}

void CNetDeviceManager::SetCurDisplaySpiltMode( unsigned int live_spilt_mode )
{
	m_ulLiveDisplayMode = live_spilt_mode;
}

void CNetDeviceManager::SetDispModeBigImageUsedD1( unsigned int channel, bool bUsedD1 )
{
	m_bUsedD1ShowImage = bUsedD1;
	m_ulUsedD1Channel = channel;
}

int CNetDeviceManager::GetNetEncodeInfo(unsigned long streamType, NET_CHANNEL_ENC_INFO * pNetChannelEncInfo)
{
	//
	const unsigned long * pSupportVS = CProduct::Instance()->GetDigitChannelSupportVideoSize();
	
	if ((0 == streamType) || (1 == streamType))
	{
		CNetDevice * pNetDevice = NULL;
		
		for (int chnn=m_localVideoInputNum; chnn < m_localVideoInputNum + m_netVideoInputNum; chnn++)
		{
			pNetChannelEncInfo[chnn - m_localVideoInputNum].chnn = chnn;

			pNetDevice = NULL;
			int netPos = -1;
			for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
			{
				if(m_netLinkInfo[netCnt].pNetDevice != NULL)
				{
					if(m_netLinkInfo[netCnt].linkInfo.channel == chnn)
					{
						pNetDevice = m_netLinkInfo[netCnt].pNetDevice;
						netPos = netCnt;
						break;
					}
				}
			}

			if (NULL != pNetDevice)
			{
				if (ND_STATUS_OK == pNetDevice->GetDevStatus() && ND_STATUS_OK == m_netLinkInfo[netPos].status)
				{
					pNetChannelEncInfo[chnn - m_localVideoInputNum].online = 1;

					ND_DEV_INFO ndDevInfo;
					memset(&ndDevInfo, 0, sizeof(ND_DEV_INFO));

					if (0 == streamType)
					{
						pNetDevice->GetDevInfo(0, ndDevInfo);
					}
					else if (1 == streamType)
					{
						pNetDevice->GetDevInfo(1, ndDevInfo);
					}
					else
					{
						assert(false);
					}

					int j = 0;

// 				if ((1 == streamType) && !(DD_VIDEO_SIZE_CIF & ndDevInfo.videoSize))
// 				{
// 					//如果子码流没有CIF分辨率，默认添加一个CIF分辨率
// 					pNetChannelEncInfo[chnn - m_localVideoInputNum].encInfo[j].videoSize = DVRVIDEO_SIZE_CIF;
// 
// 					pNetChannelEncInfo[chnn - m_localVideoInputNum].encInfo[j].ucRate = 30;
// 					if (VIDEO_FORMAT_PAL == g_video_format)
// 					{
// 						pNetChannelEncInfo[chnn - m_localVideoInputNum].encInfo[j].ucRate = 25;
// 					}
// 					pNetChannelEncInfo[chnn - m_localVideoInputNum].encInfo[j].maxBitRate = 768;
// 
// 					j++;
// 				}

					for (int i=0; i<32; i++)
					{
						if ((0x01 << i) & ndDevInfo.videoSize)
						{
							unsigned long videoSize = DDVideoSizeToVideoSize(0x01 << i);
							if ((0 == streamType) && !(videoSize & pSupportVS[chnn]))
							{
								//去掉主码流不支持的分辨率
								continue;
							}

							pNetChannelEncInfo[chnn - m_localVideoInputNum].encInfo[j].videoSize = videoSize;
							if (DVRVIDEO_SIZE_QVGA == pNetChannelEncInfo[chnn - m_localVideoInputNum].encInfo[j].videoSize)
							{
								pNetChannelEncInfo[chnn - m_localVideoInputNum].encInfo[j].videoSize = DVRVIDEO_SIZE_CIF;
							}

							if (DVRVIDEO_SIZE_VGA == pNetChannelEncInfo[chnn - m_localVideoInputNum].encInfo[j].videoSize)
							{
								pNetChannelEncInfo[chnn - m_localVideoInputNum].encInfo[j].videoSize = DVRVIDEO_SIZE_D1;
							}

							pNetChannelEncInfo[chnn - m_localVideoInputNum].encInfo[j].ucRate = ndDevInfo.ucRate[i];
							pNetChannelEncInfo[chnn - m_localVideoInputNum].encInfo[j].maxBitRate = ndDevInfo.maxBitRate[i];
							j++;
						}
					}
				}
				else
				{
					pNetChannelEncInfo[chnn - m_localVideoInputNum].online = 2;
				}

			}
			else
			{
				pNetChannelEncInfo[chnn - m_localVideoInputNum].online = 0;
			}
		}

		return 0;
	}
	else
	{
		return -1;
	}
}

int CNetDeviceManager::SetNetEncodeInfo(int chnn, int iStream, char * pSZResolution, int iRate, int iEncodeType, int iQuality, int iBitRateStream)
{
	return 0;
}


int CNetDeviceManager::GetEncodeInfo(int chnn, char * pVideoEncodeInfo, int iVideoEncodeInfoLen)
{
	CNetDevice * pNetDevice = GetNetDeviceByChnn(chnn, true);

	if (NULL != pNetDevice)
	{
		int iRet = pNetDevice->GetEncodeInfo(pVideoEncodeInfo, iVideoEncodeInfoLen);

		if ((0 < iRet) && (iRet > sizeof(CHN_VIDEO_ENCODE_INFO_DETAIL) * 1 + sizeof(CHN_VIDEO_ENCODE_INFO)))
		{
			//追加当前设置的编码信息
			int iEncodeCount = (iRet - sizeof(CHN_VIDEO_ENCODE_INFO)) / sizeof(CHN_VIDEO_ENCODE_INFO_DETAIL);

			CHN_VIDEO_ENCODE_INFO_DETAIL * pChnnVideoEncodeDetail = (CHN_VIDEO_ENCODE_INFO_DETAIL * )(pVideoEncodeInfo + sizeof(CHN_VIDEO_ENCODE_INFO));
			CHN_VIDEO_ENCODE_INFO_DETAIL * pChnnVideoEncodeDetailTemp = pChnnVideoEncodeDetail;

			pChnnVideoEncodeDetail += iEncodeCount - 2;

			//当前子码流
			pChnnVideoEncodeDetail->bCrrentUse			= 1;
			pChnnVideoEncodeDetail->encodeType			= m_pSecondStreamEncode[chnn].encodeType;
			pChnnVideoEncodeDetail->lBitRateStream[0]	= m_pSecondStreamEncode[chnn].hBitStream;
			pChnnVideoEncodeDetail->quality				= m_pSecondStreamEncode[chnn].quality;
			pChnnVideoEncodeDetail->rate				= m_pSecondStreamEncode[chnn].rate;
			pChnnVideoEncodeDetail->stream				= 1;

			if (DVRVIDEO_SIZE_QCIF == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "176X144");
			}
			else if (DVRVIDEO_SIZE_CIF == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution),"352X288");
			}
			else if (DVRVIDEO_SIZE_HD1 == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "704X288");
			}
			else if (DVRVIDEO_SIZE_D1 == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "704X576");
			}
			else if (DVRVIDEO_SIZE_QVGA == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "160X120");
			}
			else if (DVRVIDEO_SIZE_QVGA == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "320X240");
			}
			else if (DVRVIDEO_SIZE_VGA == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "640X480");
			}
			else if (DVRVIDEO_SIZE_HD720 == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "1280X720");
			}
			else if (DVRVIDEO_SIZE_HD1080 == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution),"1920X1080");
			}
			else if (DVRVIDEO_SIZE_960P == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "1280X960");
			}
			else if (DVRVIDEO_SIZE_SXGA == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "1280X1024");
			}
			else if (DVRVIDEO_SIZE_3M == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2048X1536");
			}
			else if (DVRVIDEO_SIZE_16_9_3M == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2304X1296");
			}
			else if (DVRVIDEO_SIZE_4M == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2688X1520");
			}
			else if (DVRVIDEO_SIZE_5M == m_pSecondStreamEncode[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2592X1944");
			}
			else
			{
				assert(false);
			}

			//读取编码配置信息
			CMyList<NCFG_ITEM_ID> itemList;
			itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR_EX);
			unsigned char *pDataBlock = NULL;
			unsigned long blockLen = 0;
			CConfigEx::Instance()->GetConfigEx(itemList, &pDataBlock, blockLen);
			ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(pDataBlock + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
			//当前主码流
			pChnnVideoEncodeDetail++;
			pChnnVideoEncodeDetail->bCrrentUse			= 1;
			pChnnVideoEncodeDetail->encodeType			= pEncodeInfo[chnn].encodeType;
			pChnnVideoEncodeDetail->lBitRateStream[0]	= pEncodeInfo[chnn].hBitStream;
			pChnnVideoEncodeDetail->quality				= pEncodeInfo[chnn].quality;
			pChnnVideoEncodeDetail->rate				= pEncodeInfo[chnn].rate;
			pChnnVideoEncodeDetail->stream				= 0;

			if (DVRVIDEO_SIZE_QCIF == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "176X144");
			}
			else if (DVRVIDEO_SIZE_CIF == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution),"352X288");
			}
			else if (DVRVIDEO_SIZE_HD1 == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "704X288");
			}
			else if (DVRVIDEO_SIZE_D1 == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "704X576");
			}
			else if (DVRVIDEO_SIZE_QVGA == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "160X120");
			}
			else if (DVRVIDEO_SIZE_QVGA == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "320X240");
			}
			else if (DVRVIDEO_SIZE_VGA == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "640X480");
			}
			else if (DVRVIDEO_SIZE_HD720 == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "1280X720");
			}
			else if (DVRVIDEO_SIZE_HD1080 == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution),"1920X1080");
			}
			else if (DVRVIDEO_SIZE_960P == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "1280X960");
			}
			else if (DVRVIDEO_SIZE_SXGA == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "1280X1024");
			}
			else if (DVRVIDEO_SIZE_3M == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2048X1536");
			}
			else if (DVRVIDEO_SIZE_16_9_3M == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2304X1296");
			}
			else if (DVRVIDEO_SIZE_4M == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2688X1520");
			}
			else if (DVRVIDEO_SIZE_5M == pEncodeInfo[chnn].resolution)
			{
				sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2592X1944");
			}
			else
			{
				assert(false);
			}

			for (int i=0; i<iEncodeCount; i++)
			{
				pChnnVideoEncodeDetailTemp[i].encodeType = 1;
			}

			delete [] pDataBlock;
			pDataBlock = NULL;

			return iRet;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int CNetDeviceManager::SetEncodeInfo(int chnn, int iStream, char * pSZResolution, int iRate, int iEncodeType, int iQuality, int iBitRateStream)
{
	if ((0 >chnn) || (chnn > m_videoInputNum) || (0 == iStream))
	{
		return -1;
	}

	ENCODE_INFO_EX encodeInfo;
	memset(&encodeInfo, 0, sizeof(ENCODE_INFO_EX));

	if (0 == strncmp(pSZResolution, "176X144", strlen("176X144")))		//QCIF
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_QCIF;
	}
	else if (0 == strncmp(pSZResolution, "352X288", strlen("352X288")))		//CIF
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_CIF;
	}
	else if (0 == strncmp(pSZResolution, "704X288", strlen("704X288")))		//HD1
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_HD1;
	}
	else if (0 == strncmp(pSZResolution, "704X576", strlen("704X576")))		//D1
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_D1;
	}
	else if (0 == strncmp(pSZResolution, "160X120", strlen("160X120")))		//QQVGA
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_QVGA;
	}
	else if (0 == strncmp(pSZResolution, "320X240", strlen("320X240")))		//QVGA
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_QVGA;
	}
	else if (0 == strncmp(pSZResolution, "640X480", strlen("640X480")))		//VGA
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_VGA;
	}
	else if (0 == strncmp(pSZResolution, "1280X720", strlen("1280X720")))		//720P
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_HD720;
	}
	else if (0 == strncmp(pSZResolution, "1920X1080", strlen("1920X1080")))		//1080P
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_HD1080;
	}
	else if (0 == strncmp(pSZResolution, "1280X960", strlen("1280X960")))		//960P
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_960P;
	}
	else if (0 == strncmp(pSZResolution, "1280X1024", strlen("1280X1024")))		//1.4M
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_SXGA;
	}
	else if (0 == strncmp(pSZResolution, "2048X1536", strlen("2048X1536")))		//3M
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_3M;
	}
	else if (0 == strncmp(pSZResolution, "2304X1296", strlen("2304X1296")))		//3M
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_16_9_3M;
	}
	else if (0 == strncmp(pSZResolution, "2688X1520", strlen("2688X1520")))		//3M
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_4M;
	}
	else if (0 == strncmp(pSZResolution, "2592X1944", strlen("2592X1944")))		//3M
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_5M;
	}
	else
	{
		return -1;
	}


	if (1 == iEncodeType)
	{
		encodeInfo.encodeType = VIDEO_ENCODE_MODE_CBR;
	}
	else
	{
		encodeInfo.encodeType = VIDEO_ENCODE_MODE_VBR;
	}

	encodeInfo.quality = iQuality;
	encodeInfo.hBitStream = iBitRateStream;
	encodeInfo.rate = iRate;

	//如果连接的这个通道被手机连，则用手机码流，不能改变当前码流
	if ((m_secondStreamCH_mobile >> chnn) & 0x01)
	{
		return -1;
	}

	//如果当前非手机码流的个数为1，则可以修改分辨率为720P以下，如果当前非手机连接的个数为2~4,可以修改到D1分辨率，其他情况下只能使用CIF分辨率
	int nonMobileNum = 0;
	int mobileNum = 0;
	for (unsigned char i = m_localVideoInputNum; i < m_videoInputNum; ++i)
	{
		if (m_secondStreamCH & ((ULONGLONG)(0x01) << i))
		{
			++ nonMobileNum;
		}
		if ((m_secondStreamCH_mobile & ((ULONGLONG)(0x01) << i)))
		{
			++ mobileNum;
		}
	}

	unsigned long videoSize = DDVideoSizeToVideoSize(m_pDevInfo[chnn].videoSize);
	unsigned long maxVideoSize = GetMaxVideoSize(videoSize);

	if (1 == nonMobileNum)
	{
		//为1时可以配置设备支持的最大分辨率以下的分辨率到IPC
		if (!IsUnderVideoSize(maxVideoSize, encodeInfo.resolution))
		{
			encodeInfo.resolution = DVRVIDEO_SIZE_HD720;
		}
	}
	else if (4 >= nonMobileNum)
	{
		if (DVRVIDEO_SIZE_D1 & videoSize)
		{
			if (!IsUnderVideoSize(DVRVIDEO_SIZE_D1, encodeInfo.resolution))
			{
				encodeInfo.resolution = DVRVIDEO_SIZE_D1;
			}
		}
		else
		{
			if (!IsUnderVideoSize(DVRVIDEO_SIZE_CIF, encodeInfo.resolution))
			{
				encodeInfo.resolution = DVRVIDEO_SIZE_CIF;
			}
		}
	}
	else
	{
		encodeInfo.resolution = DVRVIDEO_SIZE_CIF;

		//资源无剩余，做降帧处理
		if (encodeInfo.rate > m_pSecondStreamEncode[chnn].rate)
		{
			encodeInfo.rate = m_pSecondStreamEncode[chnn].rate;
		}
	}

	int iRet = -1;

	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if(m_netLinkInfo[netCnt].pNetDevice != NULL)
		{
			//只处理私有协议接入的通道
			if((m_netLinkInfo[netCnt].linkInfo.manufacturerId == MID_TVT) && (m_netLinkInfo[netCnt].status == ND_STATUS_OK) && (chnn == m_netLinkInfo[netCnt].linkInfo.channel))
			{
				if(0 != ((m_ullSecondCH >> chnn) & 0x01))
				{
					//该通道已经开启了子码流才可以去修改码流信息
					if(0 != ((m_secondStreamCH >> chnn) & 0x01))
					{
						//TVT接入才会重新下配置
						m_pSecondStreamEncode[chnn] = encodeInfo;
						SetNetDeviceSecondStreamCfg(m_netLinkInfo[netCnt].pNetDevice);

						iRet = 0;
					}
				}
			}
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}


	return iRet;
}

int CNetDeviceManager::GetCurrentEncodeInfo(int chnn,char * pVideoEncodeInfo, int iVideoEncodeInfoLen)
{
	CNetDevice * pNetDevice = GetNetDeviceByChnn(chnn, true);

	if (NULL != pNetDevice)
	{
		CHN_VIDEO_ENCODE_INFO * pChnVideoEncodeInfo = (CHN_VIDEO_ENCODE_INFO *)(pVideoEncodeInfo);

		pChnVideoEncodeInfo->channel = chnn;
		pChnVideoEncodeInfo->iLength = sizeof(CHN_VIDEO_ENCODE_INFO_DETAIL);

		CHN_VIDEO_ENCODE_INFO_DETAIL * pChnnVideoEncodeDetail = (CHN_VIDEO_ENCODE_INFO_DETAIL *)(pVideoEncodeInfo + sizeof(CHN_VIDEO_ENCODE_INFO));

		//当前子码流
		pChnnVideoEncodeDetail->bCrrentUse			= 1;
		pChnnVideoEncodeDetail->encodeType			= m_pSecondStreamEncode[chnn].encodeType;
		pChnnVideoEncodeDetail->lBitRateStream[0]	= m_pSecondStreamEncode[chnn].hBitStream;
		pChnnVideoEncodeDetail->quality				= m_pSecondStreamEncode[chnn].quality;
		pChnnVideoEncodeDetail->rate				= m_pSecondStreamEncode[chnn].rate;
		pChnnVideoEncodeDetail->stream				= 1;

		if (DVRVIDEO_SIZE_QCIF == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "176X144");
		}
		else if (DVRVIDEO_SIZE_CIF == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution),"352X288");
		}
		else if (DVRVIDEO_SIZE_HD1 == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "704X288");
		}
		else if (DVRVIDEO_SIZE_D1 == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "704X576");
		}
		else if (DVRVIDEO_SIZE_QVGA == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "160X120");
		}
		else if (DVRVIDEO_SIZE_QVGA == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "320X240");
		}
		else if (DVRVIDEO_SIZE_VGA == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "640X480");
		}
		else if (DVRVIDEO_SIZE_HD720 == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "1280X720");
		}
		else if (DVRVIDEO_SIZE_HD1080 == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution),"1920X1080");
		}
		else if (DVRVIDEO_SIZE_960P == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "1280X960");
		}
		else if (DVRVIDEO_SIZE_SXGA == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "1280X1024");
		}
		else if (DVRVIDEO_SIZE_3M == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2048X1536");
		}
		else if (DVRVIDEO_SIZE_16_9_3M == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2304X1296");
		}
		else if (DVRVIDEO_SIZE_4M == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2688X1520");
		}
		else if (DVRVIDEO_SIZE_5M == m_pSecondStreamEncode[chnn].resolution)
		{
			sprintf((char *)(pChnnVideoEncodeDetail->resolution), "2592X1944");
		}
		else
		{
			assert(false);
		}

		pChnnVideoEncodeDetail->encodeType = 1;

		return sizeof(CHN_VIDEO_ENCODE_INFO) + sizeof(CHN_VIDEO_ENCODE_INFO_DETAIL);
	}
	else
	{
		return -1;
	}
}

void CNetDeviceManager::GetIPCCurrentResolution( int channel, int* width, int* height )
{

	*width = 0;
	*height = 0;

	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if(m_netLinkInfo[netCnt].pNetDevice != 	NULL)
		{
			if(m_netLinkInfo[netCnt].status == ND_STATUS_OK)
			{
				if( channel == m_netLinkInfo[netCnt].linkInfo.channel )
				{
					m_netLinkInfo[netCnt].pNetDevice->GetDevResolutionVal(width, height);
					m_netLinkInfoLock[netCnt].UnLock();
					break;
				}
			}
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}

}

void CNetDeviceManager::RequestMinorStream(ULONGLONG minorStreamCH)
{
	if(0 == minorStreamCH)
	{
		return;
	}

	int total_chn_cnt = CLocalDevice::Instance()->CurrentDispChannelNum();
	int bChgSecondStream;

	if (CProduct::Instance()->IsDispD1Device())
	{
		unsigned char chnn = 0;
		
		for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
		{
			m_netLinkInfoLock[netCnt].Lock();
			if(m_netLinkInfo[netCnt].pNetDevice == NULL)
			{
				m_netLinkInfoLock[netCnt].UnLock();
				continue;
			}

			chnn = m_netLinkInfo[netCnt].linkInfo.channel;

			if(!((minorStreamCH >> chnn) & 0x01))
			{
				m_netLinkInfoLock[netCnt].UnLock();
				continue;
			}

			if(total_chn_cnt <= 16)
			{
				if( DVRVIDEO_SIZE_D1 != m_pThirdStreamEncode[chnn].resolution )
				{
					m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_D1;
					m_pThirdStreamEncode[chnn].hBitStream = 1024;
					m_bSetThirdCfg[netCnt] = true;
				}
			}
			else
			{
				if( DVRVIDEO_SIZE_CIF != m_pThirdStreamEncode[chnn].resolution )
				{
					m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_CIF;
					m_pThirdStreamEncode[chnn].hBitStream = 512;
					m_bSetThirdCfg[netCnt] = true;
				}
			}

			if(m_netLinkInfo[netCnt].status != ND_STATUS_OK)
			{
				m_netLinkInfoLock[netCnt].UnLock();
				continue;
			}

			m_netLinkInfoLock[netCnt].UnLock();
		}
		
		return ;
	}

	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		m_netLinkInfoLock[netCnt].Lock();
		if(m_netLinkInfo[netCnt].pNetDevice == NULL)
		{
			m_netLinkInfoLock[netCnt].UnLock();
			continue;
		}

		if(m_netLinkInfo[netCnt].status == ND_STATUS_OK)
		{
			unsigned char chnn = m_netLinkInfo[netCnt].linkInfo.channel;
			//该通道已经开启了子码流
			{
				if(0 == ((minorStreamCH >> chnn) & 0x01))
				{
				}
				else
				{
					if( m_bUsedD1ShowImage && chnn == m_ulUsedD1Channel )
					{
						if( DVRVIDEO_SIZE_D1 != m_pThirdStreamEncode[chnn].resolution )
						{
							m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_D1;
							m_bSetThirdCfg[netCnt] = true;
						}
					}
					else if( m_ulLiveDisplayMode && total_chn_cnt <= 4 )
					{
						if( DVRVIDEO_SIZE_D1 != m_pThirdStreamEncode[chnn].resolution )
						{
							m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_D1;
							m_bSetThirdCfg[netCnt] = true;
						}
					}
					else
					{
						if( DVRVIDEO_SIZE_CIF != m_pThirdStreamEncode[chnn].resolution )
						{
							m_pThirdStreamEncode[chnn].resolution = DVRVIDEO_SIZE_CIF;
							m_bSetThirdCfg[netCnt] = true;
						}
					}
					m_netLinkInfo[netCnt].pNetDevice->RequestKeyFrame(0, 0, 0x01);
				}
			}
		}
		m_netLinkInfoLock[netCnt].UnLock();
	}
}

void CNetDeviceManager::ChangeSwitchIP(unsigned long switchIP, unsigned long switchMask)
{
	
//	m_tempLinkInfoList.clear();
	m_switchIP = switchIP;
	m_switchMask = switchMask;
	if(m_poeInputNum > 0)
	{
		// 关闭poe的discover搜索线程，把处理逻辑简单化
		m_poeOnvifDeviceMan.EnablePoeDiscoverThreadRun( false );


		unsigned long deviceNum = m_pRegDeviceLog->GetNetDeviceCount();
		NET_DEVICE_LINK_INFO *pLinkInfo = new NET_DEVICE_LINK_INFO[deviceNum];
		m_pRegDeviceLog->GetNetDevice(pLinkInfo, deviceNum);

		for(unsigned char i = 0; i < m_poeInputNum; i++)
		{
			if(IsPoeDevice(pLinkInfo[i]))
			{
				in_addr ipaddr;
				ipaddr.s_addr = m_switchIP + htonl(i+1);
				strcpy(pLinkInfo[i].szServer, inet_ntoa(ipaddr));
			}
		}

	//	list<NET_LINK_INFO_EX>::iterator it;
		NET_LINK_INFO_EX linkInfoEx;
		for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
		{
			m_netLinkInfoLock[netCnt].Lock();
			if(m_netLinkInfo[netCnt].pNetDevice != NULL)
			{
				linkInfoEx = m_netLinkInfo[netCnt];
				if(!IsPoeDevice(linkInfoEx.linkInfo))
				{
					m_tempLinkInfoList.push_back(linkInfoEx);
					m_netLinkInfoLock[netCnt].UnLock();
					continue;
				}

				linkInfoEx.bPOEOnLine = false;
				strcpy(linkInfoEx.linkInfo.szServer, pLinkInfo[linkInfoEx.linkInfo.channel].szServer);
				if(NULL != linkInfoEx.pNetDevice)
				{
					m_ullMinorCH &= ~(0x01<<linkInfoEx.linkInfo.channel);
					linkInfoEx.pNetDevice->Quit();
				}
				linkInfoEx.status = ND_STATUS_DISCONNECT;
				linkInfoEx.POEOnLineTime = GetTickCount();

				m_tempLinkInfoList.push_back(linkInfoEx);
			}
			m_netLinkInfoLock[netCnt].UnLock();
		}
		if(!m_tempLinkInfoList.empty())
		{
			m_bLinkInfoChanged = true;
		}

		m_pRegDeviceLog->SetNetDevice(pLinkInfo, deviceNum);


		for (int i = 0; i < m_poeInputNum; i++)
		{
			m_pPoeChannelSn[i].channel = i;
			m_pPoeChannelSn[i].poe_ipc_status = -1;
			m_pPoeChannelSn[i].sn[0] = '\0';
		}

		//根据sn确定图像的位置
		for (int i = 0; i < m_poeInputNum; i++)
		{
			int chnnx = pLinkInfo[i].channel;

			if( chnnx < m_poeInputNum )
			{
				m_pPoeChannelSn[chnnx].channel = chnnx;
				strcpy(m_pPoeChannelSn[chnnx].sn, pLinkInfo[i].szID);

				//当前表示是POE通道上接了eth0的IPC
				if(!IsPoeDevice(pLinkInfo[i]))
				{
					m_pPoeChannelSn[chnnx].poe_ipc_status = -2;
				}
				else
				{
					//如果是POE设备连接，则szID长度肯定对于0
					if( strlen( pLinkInfo[i].szID ) > 0 )
					{
						m_pPoeChannelSn[chnnx].poe_ipc_status = 0;
					}
					else
					{
						//表示POE通道上没有连接IPC设备
						m_pPoeChannelSn[chnnx].poe_ipc_status = -1;
					}
				}
			}
		}
		//在设备列表锁之后，防止死锁
		m_poeOnvifDeviceMan.ChangeChannelSn(m_pPoeChannelSn, false);
		m_pRegDeviceLog->SetPoeUsedPtoPMode( m_poeOnvifDeviceMan.GetPoeUsedMode() );

		m_poeOnvifDeviceMan.ChangeSwitchIP(m_switchIP, m_switchMask);

#ifdef __ENVIRONMENT_LINUX__
		CNetInterface::Instance()->SetIP(m_switchIP, POE_ETHER);
		CNetInterface::Instance()->SetIPMask(m_switchMask, POE_ETHER);
#endif

		// 打开poe的discover搜索线程
		m_poeOnvifDeviceMan.EnablePoeDiscoverThreadRun( true );

	}
}

void CNetDeviceManager::WriteLog(unsigned long clientID, LOG_TYPE type, const char *pDevIP /* = NULL */, unsigned long logTime /* = 0 */)
{
	OPERATOR_LOG		operatorLog;
	memset(&operatorLog, 0, sizeof(OPERATOR_LOG));
	if ((LOG_TYPE_IPCAM_CONNECTED == type) || (LOG_TYPE_IPCAM_DISCONNECTED == type))
	{
		if (NULL != pDevIP)
		{
			operatorLog.IP = inet_addr(pDevIP);
		}
		else
		{
			operatorLog.IP = IP_ADDRESS(127, 0, 0, 1);
		}
		
		strcpy(operatorLog.name, "system");
		operatorLog.contentID = GetContentID(type);
		operatorLog.type = (type & 0xffff);
		operatorLog.time = (logTime != 0) ? logTime : GetCurrTime32();
		CLogMan::Instance()->WriteOperatorLog(operatorLog);
	}
	else
	{
		USER_INFO user;
		if (CUserMan::Instance()->GetUser(user, clientID))
		{
			operatorLog.IP		= user.IP;
			strcpy(operatorLog.name, user.name);

			operatorLog.contentID = GetContentID(type);
			operatorLog.type = (type & 0xffff);

			operatorLog.time = (logTime != 0) ? logTime : GetCurrTime32();

			CLogMan::Instance()->WriteOperatorLog(operatorLog);
		}
	}
}

void CNetDeviceManager::SetSecondStreamEncode()
{
	unsigned long chnnMaxRate = (VIDEO_FORMAT_NTSC == g_video_format) ? 30 : 25;
	unsigned long chnnRate = chnnMaxRate;
	unsigned long nonMobileNum = 0, mobileNum = 0;

	//读取编码配置信息
	CMyList<NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_ENCODE_MINOR_EX);
	unsigned char * pDataBlock = NULL;
	unsigned long blockLen = 0;
	CConfigEx::Instance()->GetConfigEx(itemList, &pDataBlock, blockLen);
	ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(pDataBlock + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));

	for (unsigned char i = 0; i < m_videoInputNum; ++i)
	{
		if (m_secondStreamCH & ((ULONGLONG)(0x01) << i))
		{
			++ nonMobileNum;
		}
		if ((m_secondStreamCH_mobile & ((ULONGLONG)(0x01) << i)))
		{
			++ mobileNum;
		}
	}

	if ((m_maxSecondStreamCopies != INVALID_INDEX(unsigned long)) && (m_secondAllSendNum > 0))
	{
		chnnRate = m_maxSecondStreamCopies * chnnMaxRate/m_secondAllSendNum;
	}

	//针对手机客户端做帧率及码率限制
	for (unsigned char chnn = m_localVideoInputNum; chnn < m_videoInputNum; ++chnn)
	{
		if (m_secondStreamCH_mobile & ((ULONGLONG)(0x01) << chnn))
		{
			//手机流，分辨率为CIF，单路为3帧,多路手机则每路1帧。

			pEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_CIF;
			pEncodeInfo[chnn].rate = (1 == mobileNum) ? 3 : 1;
			pEncodeInfo[chnn].rate = (pEncodeInfo[chnn].rate < chnnRate) ? pEncodeInfo[chnn].rate : chnnRate;
			pEncodeInfo[chnn].hBitStream = 64;
		}
	}

	for (int chnn=m_localVideoInputNum; chnn<m_videoInputNum; chnn++)
	{
		m_pSecondStreamEncode[chnn].resolution	= pEncodeInfo[chnn].resolution;
		m_pSecondStreamEncode[chnn].rate		= pEncodeInfo[chnn].rate;
		m_pSecondStreamEncode[chnn].encodeType	= pEncodeInfo[chnn].encodeType;
		m_pSecondStreamEncode[chnn].quality		= pEncodeInfo[chnn].quality;
		m_pSecondStreamEncode[chnn].lBitStream	= pEncodeInfo[chnn].lBitStream;
		m_pSecondStreamEncode[chnn].hBitStream	= pEncodeInfo[chnn].hBitStream;
		printf("%s:%d, channel %d rate=%d, resolution=%d, bitstream=%d\n", __FILE__, __LINE__, chnn, m_pSecondStreamEncode[chnn].rate, m_pSecondStreamEncode[chnn].resolution, m_pSecondStreamEncode[chnn].hBitStream);
	}

	delete [] pDataBlock;
}

void CNetDeviceManager::SetNetDeviceThirdStreamCfg(CNetDevice *pNetDevice)
{
	//读取编码配置信息
	CMyList<NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR_EX);
	unsigned char *pDataBlock = NULL;
	unsigned long blockLen = 0;
	CConfigEx::Instance()->GetConfigEx(itemList, &pDataBlock, blockLen);
	ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(pDataBlock + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));

	//调整分辨率及帧率
	unsigned long chnnMaxRate = (VIDEO_FORMAT_NTSC == g_video_format) ? 30 : 25;
	unsigned char chnn = pNetDevice->GetBindChannel();
	ENCODE_INFO_EX thirdEncodeInfo = m_pThirdStreamEncode[chnn];//这个值是否要加锁


	unsigned long resolutionIndex = GetResolutionIndex(thirdEncodeInfo.resolution);
	resolutionIndex = AdjustThirdResolution(chnn, resolutionIndex);
	thirdEncodeInfo.resolution = m_resolutionInfo[resolutionIndex].resolution;

	thirdEncodeInfo.rate = AdjustThirdFrameRate(chnn, thirdEncodeInfo.rate*m_resolutionInfo[resolutionIndex].multiNumCIF, chnnMaxRate, resolutionIndex);

	//thirdEncodeInfo.hBitStream = (512 * m_resolutionInfo[resolutionIndex].multiNumCIF *thirdEncodeInfo.rate / chnnMaxRate) & ~127;//修正成128的整数倍
	//thirdEncodeInfo.hBitStream = 512;
	if(0 == thirdEncodeInfo.hBitStream)
	{
		thirdEncodeInfo.hBitStream = 64;
	}

	if( pNetDevice )
	{
		DD_ENCODE_CONFIG* pThirdEncCfg = new DD_ENCODE_CONFIG[chnn + 1];
		DD_ENCODE_CONFIG* pTemp = pThirdEncCfg + chnn;

		memset( pThirdEncCfg, 0, (chnn+1)*sizeof(DD_ENCODE_CONFIG) );
		pTemp->iSize = sizeof(DD_ENCODE_CONFIG);
		pTemp->encodeType = thirdEncodeInfo.encodeType;
		pTemp->rate = thirdEncodeInfo.rate;
		((25 == pTemp->rate)||(30==pTemp->rate)) ? pTemp->rate =chnnMaxRate : NULL;
		pTemp->maxBitrate = thirdEncodeInfo.hBitStream;
		pTemp->minBitrate = thirdEncodeInfo.lBitStream;
		pTemp->quality = thirdEncodeInfo.quality;
		pTemp->resolution = VideoSizeToDDVideoSize(thirdEncodeInfo.resolution);
		pNetDevice->SetConfig(chnn, DD_CONFIG_ITEM_ENCODE_THIRD_STREAM, (const unsigned char*)pThirdEncCfg, (chnn+1)*sizeof(DD_ENCODE_CONFIG), true);

		delete[] pThirdEncCfg;
	}

	delete [] pDataBlock;
}

void CNetDeviceManager::SetNetDeviceSecondStreamCfg(CNetDevice *pNetDevice, bool bAdj)
{
	//读取编码配置信息
	CMyList<NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_ENCODE_MINOR_EX);
	unsigned char *pDataBlock = NULL;
	unsigned long blockLen = 0;
	CConfigEx::Instance()->GetConfigEx(itemList, &pDataBlock, blockLen);
	ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(pDataBlock + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));

	//调整分辨率及帧率
	unsigned long chnnMaxRate = (VIDEO_FORMAT_NTSC == g_video_format) ? 30 : 25;
	unsigned char chnn = pNetDevice->GetBindChannel();
	ENCODE_INFO_EX &minorEncodeInfo = m_pSecondStreamEncode[chnn];

	unsigned long resolutionIndex = GetResolutionIndex(minorEncodeInfo.resolution);
	resolutionIndex = AdjustResolution(chnn, resolutionIndex);
	minorEncodeInfo.resolution = m_resolutionInfo[resolutionIndex].resolution;

	if (bAdj)
	{
		if(m_maxSecondStreamCopies != INVALID_INDEX(unsigned long))
		{
			minorEncodeInfo.rate = AdjustFrameRate(chnn, minorEncodeInfo.rate*m_resolutionInfo[resolutionIndex].multiNumCIF, pEncodeInfo[chnn].rate, resolutionIndex);
		}
		else
		{
			minorEncodeInfo.rate = AdjustFrameRate(chnn, 0xFFFF, pEncodeInfo[chnn].rate, resolutionIndex);
		}

		//minorEncodeInfo.hBitStream = (minorEncodeInfo.hBitStream * minorEncodeInfo.rate / chnnMaxRate) & ~127;//修正成128的整数倍
		if(0 == minorEncodeInfo.hBitStream)
		{
			minorEncodeInfo.hBitStream = 64;
		}
	}

	//因为主码流与子码流结构体相同，且通道数也相同，直接重用主码流配置的内存就行了
	NCFG_ITEM_HEAD *pItemHead = (NCFG_ITEM_HEAD *)(pDataBlock+sizeof(NCFG_BLOCK_HEAD));
	pItemHead->itemID = NCFG_ITEM_ENCODE_MINOR_EX;
	memset(pEncodeInfo, 0, sizeof(ENCODE_INFO_EX) * m_videoInputNum);
	memcpy(pEncodeInfo, m_pSecondStreamEncode, sizeof(ENCODE_INFO_EX) * m_videoInputNum);
	m_pCfgNetDeviceMan->SetNetConfig(this, pNetDevice, pDataBlock, blockLen);

	delete [] pDataBlock;
}

bool CNetDeviceManager::CheckVideoResolution(NET_LINK_INFO_EX &netlinkInfo)
{
	ND_DEV_INFO devInfo;

	memset( &devInfo, 0, sizeof(ND_DEV_INFO));
	netlinkInfo.pNetDevice->GetDevInfo(0, devInfo);
	if( devInfo.videoSize <= 0 )
	{
		return false;
	}

	devInfo.videoSize = DDVideoSizeToVideoSize(devInfo.videoSize);
	unsigned long *pVideoSize = CProduct::Instance()->GetDigitChannelSupportVideoSize();
	unsigned char chnn = netlinkInfo.linkInfo.channel;

	if( !(pVideoSize[chnn] & devInfo.videoSize) )
	{
		return false;
	}

	return true;
}

void CNetDeviceManager::UpdateConfigOnConnect(NET_LINK_INFO_EX &netlinkInfo)
{
	//读取编码配置信息
	CMyList<NCFG_ITEM_ID>	idList;
	idList.AddTail(NCFG_ITEM_ENCODE_MAJOR_EX);
	unsigned char *pDataBlock = NULL;
	unsigned long blockLen = 0;
	m_configInfoLock.Lock();
	CConfigEx::Instance()->GetConfigEx(idList, &pDataBlock, blockLen);
	ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(pDataBlock + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
#if 0
 	for(int i = 0; i < m_netVideoInputNum; i++)
 	{
 		printf("!**! chnn %d : resolution = %d, rate = %d\n", i, pEncodeInfo[i].resolution, pEncodeInfo[i].rate);
 	}
#endif

	//获取网络设备支持的分辨率
	ND_DEV_INFO devInfo;
	memset( &devInfo, 0, sizeof(ND_DEV_INFO));
	netlinkInfo.pNetDevice->GetDevInfo(0, devInfo);
	devInfo.videoSize = DDVideoSizeToVideoSize(devInfo.videoSize);

	unsigned char chnn = netlinkInfo.linkInfo.channel;

	if(!(pEncodeInfo[chnn].resolution & devInfo.videoSize))
	{
		int maxRate = (g_video_format == VIDEO_FORMAT_NTSC) ? 30 : 25;
		long prevRate = pEncodeInfo[chnn].rate;
		long vgaNum = pEncodeInfo[chnn].rate;
		if(pEncodeInfo[chnn].resolution == DVRVIDEO_SIZE_960P)
		{
			vgaNum  *= 4;
		}
		else if(pEncodeInfo[chnn].resolution == DVRVIDEO_SIZE_SXGA)
		{
			vgaNum  *= 5;
		}
		else if(pEncodeInfo[chnn].resolution == DVRVIDEO_SIZE_3M)
		{
			vgaNum  *= 8;
		}
		else if(pEncodeInfo[chnn].resolution == DVRVIDEO_SIZE_16_9_3M)
		{
			vgaNum  *= 8;
		}
		else if(pEncodeInfo[chnn].resolution == DVRVIDEO_SIZE_4M)
		{
			vgaNum  *= 12;
		}
		else if(pEncodeInfo[chnn].resolution == DVRVIDEO_SIZE_5M)
		{
			vgaNum  *= 16;
		}
		else if(pEncodeInfo[chnn].resolution == DVRVIDEO_SIZE_HD1080)
		{
			vgaNum  *= 6;
		}
		else if(pEncodeInfo[chnn].resolution == DVRVIDEO_SIZE_HD720)
		{
			vgaNum *= 3;
		}

		if(devInfo.videoSize & DVRVIDEO_SIZE_5M)
		{
			pEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_5M;
			pEncodeInfo[chnn].rate = vgaNum/16;
		}
		else if(devInfo.videoSize & DVRVIDEO_SIZE_4M)
		{
			pEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_4M;
			pEncodeInfo[chnn].rate = vgaNum/12;
		}
		else if(devInfo.videoSize & DVRVIDEO_SIZE_16_9_3M)
		{
			pEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_16_9_3M;
			pEncodeInfo[chnn].rate = vgaNum/8;
		}
		else if(devInfo.videoSize & DVRVIDEO_SIZE_3M)
		{
			pEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_3M;
			pEncodeInfo[chnn].rate = vgaNum/8;
		}
		else if(devInfo.videoSize & DVRVIDEO_SIZE_HD1080)
		{
			pEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_HD1080;
			pEncodeInfo[chnn].rate = vgaNum/6;
		}
		else if(devInfo.videoSize & DVRVIDEO_SIZE_SXGA)
		{
			pEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_SXGA;
			pEncodeInfo[chnn].rate = vgaNum/5;
		}
		else if(devInfo.videoSize & DVRVIDEO_SIZE_960P)
		{
			pEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_960P;
			if (CProduct::Instance()->IsUseStaticVideoBitRate())
			{
				pEncodeInfo[chnn].rate = vgaNum/3;
			}
			else
			{
				pEncodeInfo[chnn].rate = vgaNum/4;
			}
		}
		else if(devInfo.videoSize & DVRVIDEO_SIZE_HD720)
		{
			pEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_HD720;
			pEncodeInfo[chnn].rate = vgaNum/3;
		}
		else if(devInfo.videoSize & DVRVIDEO_SIZE_D1)
		{
			pEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_D1;
			pEncodeInfo[chnn].rate = vgaNum;
		}
		else if(devInfo.videoSize & DVRVIDEO_SIZE_VGA)
		{
			pEncodeInfo[chnn].resolution = DVRVIDEO_SIZE_VGA;
			pEncodeInfo[chnn].rate = vgaNum;
		}
		else
		{
			assert(false);
		}

		printf("!**! first adjust -- CH %d : resolution = %d, rate = %d\n", chnn, pEncodeInfo[chnn].resolution, pEncodeInfo[chnn].rate);

		unsigned long ddVideoSize = VideoSizeToDDVideoSize(pEncodeInfo[chnn].resolution);
		int tempRate = devInfo.ucRate[GetBitIndex(ddVideoSize)];
		if ((VIDEO_FORMAT_NTSC == g_video_format) && (25 == tempRate))
		{
			tempRate = maxRate;
		}
		maxRate = (maxRate < tempRate) ? maxRate : tempRate;

		printf("!**! maxRate[%d] = %d, tempRate=%d\n", chnn, maxRate, tempRate);

		if(pEncodeInfo[chnn].rate > maxRate)
		{
			pEncodeInfo[chnn].hBitStream = pEncodeInfo[chnn].hBitStream * maxRate / pEncodeInfo[chnn].rate;
			if (CProduct::Instance()->IsUseStaticVideoBitRate())
			{
				for (int i=0; 0 != g_vsBitRateInfo[i].videoSize; i++)
				{
					if (g_vsBitRateInfo[i].videoSize == pEncodeInfo[chnn].resolution)
					{
						pEncodeInfo[chnn].hBitStream = g_vsBitRateInfo[i].bitRate;
						break;
					}
				}
			}
			pEncodeInfo[chnn].hBitStream = AdjustBitStream(pEncodeInfo[chnn].hBitStream);
			pEncodeInfo[chnn].rate = maxRate;
		}
		else
		{
			//校正一些常用帧率
			if((7 == prevRate) && (30 == maxRate))	//NTSC制下30帧时，7帧为1/4满帧，当分辨率由1080P改为720P时，帧率应调整为1/2
			{
				if(14 == pEncodeInfo[chnn].rate)
				{
					pEncodeInfo[chnn].rate = 15;
				}
			}

			if((12 == prevRate) && (25 == maxRate))	//PAL制下25帧时，12帧为1/2满帧，当分辨率由1080P改为720P时，帧率应调整为满帧
			{
				if(24 == pEncodeInfo[chnn].rate)
				{
					pEncodeInfo[chnn].rate = 25;
				}
			}
		}

		printf("!**! last adjust -- CH %d : resolution = %d, rate = %d\n", chnn, pEncodeInfo[chnn].resolution, pEncodeInfo[chnn].rate);

		CConfigEx::Instance()->SetConfig(pDataBlock, blockLen);

		//更新这个通道的解码帧率
		if (NULL != g_pStreamProc)
		{
			g_pStreamProc->SetNetVideoFRate(chnn - m_localVideoInputNum, pEncodeInfo[chnn].rate, pEncodeInfo[chnn].hBitStream);
		}
	}
	delete [] pDataBlock;
	m_configInfoLock.UnLock();

	//对于子码流
	netlinkInfo.pNetDevice->GetDevInfo(1, m_pDevInfo[netlinkInfo.linkInfo.channel]);
	m_pDevInfo[netlinkInfo.linkInfo.channel].videoSize = DDVideoSizeToVideoSize(m_pDevInfo[netlinkInfo.linkInfo.channel].videoSize);
	
	netlinkInfo.pNetDevice->GetDevInfo(2, m_pThirdDevInfo[netlinkInfo.linkInfo.channel]);
	m_pThirdDevInfo[netlinkInfo.linkInfo.channel].videoSize = DDVideoSizeToVideoSize(m_pThirdDevInfo[netlinkInfo.linkInfo.channel].videoSize);
}

void CNetDeviceManager::GetPhyChannelByMacAddrCallBack( char* macAddr, int len, bool bOnvifDevice, int* chnn, bool bClearOnvifDeviceMac, void* pUser )
{
	CNetDeviceManager *pThis = reinterpret_cast<CNetDeviceManager *>(pUser);
	if(NULL != pThis)
	{
		pThis->GetPhyChannelByMacAddr( macAddr, len, bOnvifDevice, chnn, bClearOnvifDeviceMac );
	}
}

void CNetDeviceManager::GetPhyChannelByMacAddr( char* macAddr, int len, bool bOnvifDevice, int* chnn, bool bClearOnvifDeviceMac )
{
	int indx = -1;

#ifndef __ENVIRONMENT_WIN32__
	indx = m_poeDeviceMan.GetIPCMacChannel( (const char*)macAddr, len, bOnvifDevice, bClearOnvifDeviceMac );
#endif

	*chnn = indx;

	return;
}


void CNetDeviceManager::OnvifOnlineCallBack(POE_ONVIF_DEVICE_INFO &deviceInfo, POE_IPC_EXT_CTRL_PARAM& stCtrlParam, void *pUser)
{
	CNetDeviceManager *pThis = reinterpret_cast<CNetDeviceManager *>(pUser);
	if(NULL != pThis)
	{
		pThis->OnOnvifOnline(deviceInfo, stCtrlParam);
	}
}

void CNetDeviceManager::OnvifOfflineCallBack(POE_ONVIF_DEVICE_INFO &deviceInfo, POE_IPC_EXT_CTRL_PARAM& stCtrlParam, void *pUser)
{
	CNetDeviceManager *pThis = reinterpret_cast<CNetDeviceManager *>(pUser);
	if(NULL != pThis)
	{
		pThis->OnOnvifOffline( deviceInfo, stCtrlParam );
	}
}

void CNetDeviceManager::OnOnvifOnline(POE_ONVIF_DEVICE_INFO &deviceInfo, POE_IPC_EXT_CTRL_PARAM& stCtrlParam )
{
	
//	m_tempLinkInfoList.clear();
	if( deviceInfo.channel >= m_poeInputNum )
	{
		printf( "current poe ipc channel is error ===== %d,%s,%d\n",
			deviceInfo.channel, __FILE__, __LINE__ );
		return;
	}

	while( (m_bLinkInfoChanged)||(!IfCheckStatusOver()) )
	{
		DVRSleep(20);
	}
	printf( "current poeonvif online change is over chl %d --  %s,%d\n", deviceInfo.channel, __FILE__, __LINE__ );

	if(!m_bPoeLinkChg)
	{
		m_tempLinkInfoList.clear();
		for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
		{
			m_netLinkInfoLock[netCnt].Lock();
			if(m_netLinkInfo[netCnt].pNetDevice != NULL)
			{
				m_tempLinkInfoList.push_back(m_netLinkInfo[netCnt]);
			}
			m_netLinkInfoLock[netCnt].UnLock();
		}
	}

	list<NET_LINK_INFO_EX>::iterator itertmp = m_tempLinkInfoList.begin(), endtmp = m_tempLinkInfoList.end();
	for(; itertmp != endtmp; itertmp++)
	{
		if(deviceInfo.channel == (*itertmp).linkInfo.channel)
		{
			break;
		}
	}

	if(itertmp == endtmp)
	{
		printf("****!!!!!!NO chann match the poe chann = %d.....%s.....%d\n",deviceInfo.channel,__FILE__,__LINE__);
		assert(false);
		return;
	}

	bool bLogDevice = false;
	bool bIPCHasChanged = false;
	int deviceNum = m_pRegDeviceLog->GetNetDeviceCount();
	NET_DEVICE_LINK_INFO *pLinkInfo = new NET_DEVICE_LINK_INFO[deviceNum];
	m_pRegDeviceLog->GetNetDevice(pLinkInfo, deviceNum);

	list<NET_LINK_INFO_EX>::iterator iter = m_tempLinkInfoList.begin(), end = m_tempLinkInfoList.end();
	for (; iter!=end;)
	{
		NET_LINK_INFO_EX &linkInfo = (*iter);
		if (linkInfo.linkInfo.channel == deviceInfo.channel)
		{
			printf("Open ONVIF device on discover! %d %s\t",linkInfo.linkInfo.channel, linkInfo.linkInfo.szServer);
			PrintfBit64(GetCurrTime(), false);

			bIPCHasChanged = true;
			if(!linkInfo.bPOEOnLine)
			{
				linkInfo.bPOEOnLine = true;
				linkInfo.POEOnLineTime = GetTickCount();
				strcpy(linkInfo.linkInfo.szEtherName, POE_ETHER);
				// POE上线状态已经更改
				m_bPoeLinkChg = true;

				//如果这个通道只是因为UDP组播丢包原因错认为掉线的话（sn没有变化，协议没有变化，IP也没有变化），则直接跳出这个流程
				if((ND_STATUS_OK == linkInfo.status) && (strcmp(linkInfo.linkInfo.szID, deviceInfo.sn) == 0)\
					&& (linkInfo.linkInfo.manufacturerId == deviceInfo.manufacturerId) && (inet_addr(linkInfo.linkInfo.szServer) == deviceInfo.dwIP))
				{
					bIPCHasChanged = false;
				}
			}

			if( bIPCHasChanged )
			{
				//strcpy(m_pPoeChannelSn[deviceInfo.channel].sn, deviceInfo.sn);

				//两个其实是一样的对象
				//itertmp = iter;

				printf( "change ipc == %d\n", deviceInfo.channel );

				in_addr ipAddr;
				ipAddr.s_addr = deviceInfo.dwIP;
				strcpy((*itertmp).linkInfo.szServer, inet_ntoa(ipAddr));
				strcpy((*itertmp).linkInfo.szID, deviceInfo.sn);
				(*itertmp).linkInfo.nPort = deviceInfo.dataPort;
				strcpy((*itertmp).linkInfo.productModel, deviceInfo.productModel);
				(*itertmp).linkInfo.manufacturerId = deviceInfo.manufacturerId;

				strcpy((*itertmp).linkInfo.manufacturerName, deviceInfo.manufacturerName);

				for(int i = 0; i < m_poeInputNum; i++)
				{
					if(pLinkInfo[i].channel == deviceInfo.channel)
					{
						memcpy(&pLinkInfo[i], &(*itertmp).linkInfo, sizeof(NET_DEVICE_LINK_INFO));
						bLogDevice = true;
						break;
					}
				}

				m_bPoeLinkChg = true;
			}

			iter++;
		}
		//POE搜索处理的相同的IPC
		else if(stCtrlParam.delChnMsk & (1<<linkInfo.linkInfo.channel))
		{
			bool bIPSameFlag = (inet_addr(linkInfo.linkInfo.szServer) == deviceInfo.dwIP);
			bool bSnSameFlag = (strcmp(linkInfo.linkInfo.szID, deviceInfo.sn) == 0);

			if( bIPSameFlag || bSnSameFlag )
			{
				//删除相同sn或者相同IP的IPC
				if( linkInfo.linkInfo.channel < m_poeInputNum )
				{
					if( linkInfo.bPOEOnLine )
					{
						linkInfo.bPOEOnLine = false;
					}

					printf( "sames poe ipc == %d, %d, %d\n", linkInfo.linkInfo.channel, (int)bIPSameFlag, (int)bSnSameFlag );

					if( bSnSameFlag )
					{
						linkInfo.linkInfo.szID[0] = '\0';
					}
					memset( linkInfo.linkInfo.szServer, 0, sizeof(linkInfo.linkInfo.szServer) );

					for(int i = 0; i < m_poeInputNum; i++)
					{
						if(pLinkInfo[i].channel == linkInfo.linkInfo.channel )
						{
							memcpy(&pLinkInfo[i], &linkInfo.linkInfo, sizeof(NET_DEVICE_LINK_INFO));
							bLogDevice = true;
							break;
						}
					}

					m_bPoeLinkChg = true;

					iter++;
				}
				else
				{
					iter = m_tempLinkInfoList.erase( iter );

					//减少一个相同的IPC
					for(int i = 0; i < deviceNum; i++)
					{
						if(pLinkInfo[i].channel == linkInfo.linkInfo.channel )
						{
							printf( "sames eth0 ipc == %d\n", linkInfo.linkInfo.channel );

							if( i != deviceNum - 1 )
							{
								memcpy(&pLinkInfo[i], &pLinkInfo[deviceNum-1], sizeof(NET_DEVICE_LINK_INFO));
							}
							deviceNum -= 1;
							bLogDevice = true;
							break;
						}
					}
				}
			}
			else
			{
				iter++;
			}
		}
		else
		{
			iter++;
		}
	}

	if( bLogDevice )
	{
		m_pRegDeviceLog->SetNetDevice(pLinkInfo, deviceNum);
	}
	delete [] pLinkInfo;

}

void CNetDeviceManager::OnOnvifOffline(POE_ONVIF_DEVICE_INFO &devicInfo, POE_IPC_EXT_CTRL_PARAM& stCtrlParam )
{
	int channel = devicInfo.channel;

	if( channel >= m_poeInputNum )
	{
		printf( "current poe ipc disconnect channel is error ===== %d,%s,%d\n",
			channel, __FILE__, __LINE__ );
		return;
	}

	while( (m_bLinkInfoChanged)||(!IfCheckStatusOver()) )
	{
		DVRSleep(20);
	}
	printf( "current poeonvif offline change is over chl %d -- %s,%d\n", channel, __FILE__, __LINE__ );


	if(!m_bPoeLinkChg)
	{
		m_tempLinkInfoList.clear();
		for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
		{
			m_netLinkInfoLock[netCnt].Lock();
			if(m_netLinkInfo[netCnt].pNetDevice != NULL)
			{
				m_tempLinkInfoList.push_back(m_netLinkInfo[netCnt]);
			}
			m_netLinkInfoLock[netCnt].UnLock();
		}
	}

	list<NET_LINK_INFO_EX>::iterator iter = m_tempLinkInfoList.begin(),
		end = m_tempLinkInfoList.end();
	for (; iter != end;iter++ )
	{
		NET_LINK_INFO_EX &linkInfo = (*iter);
		if (linkInfo.linkInfo.channel == channel)
		{
			if(linkInfo.bPOEOnLine)
			{
				linkInfo.bPOEOnLine = false;

				m_bPoeLinkChg = true;

				printf("Close Onvif Device Success! %d \t",channel);
				PrintfBit64(GetCurrTime(), false);

				break;
			}
		}
	}

	if (iter == end)
	{
		printf("Close Device Faild!\n");
	}
	
	//如果需要刷新，就马上检查刷新
	m_checkRefreshTime = 0;

	
}

unsigned long CNetDeviceManager::AdjustBitStream(unsigned long bitStream)
{
	unsigned long count = CProduct::Instance()->LocalBitRateCount();

	for(unsigned long i = 0; i < count; i++)
	{
		if(CProduct::Instance()->LocalBitRateValue(i) > bitStream)
		{
			if(i > 0)
			{
				long diff = bitStream - CProduct::Instance()->LocalBitRateValue(i-1);
				if((bitStream + diff) < CProduct::Instance()->LocalBitRateValue(i))
				{
					return CProduct::Instance()->LocalBitRateValue(i-1);
				}
				else
				{
					return CProduct::Instance()->LocalBitRateValue(i);
				}
			}
			else
			{
				return CProduct::Instance()->LocalBitRateValue(i);
			}
		}
		else if(CProduct::Instance()->LocalBitRateValue(i) == bitStream)
		{
			return CProduct::Instance()->LocalBitRateValue(i);
		}
	}

	return CProduct::Instance()->LocalBitRateValue(count - 1);
}

unsigned long CNetDeviceManager::AdjustFrameRate(unsigned long chnn, unsigned long chnnAvailRate, unsigned long maxRate, unsigned long resolutionIndex)
{
	int resolution = VideoSizeToDDVideoSize(m_resolutionInfo[resolutionIndex].resolution);
	int index = GetBitIndex(resolution);
	unsigned long chnnRate = chnnAvailRate / m_resolutionInfo[resolutionIndex].multiNumCIF;
	if (chnnRate < 1)
	{
		chnnRate = 1;
	}
	else if(chnnRate > maxRate)
	{
		chnnRate = maxRate;
	}
	
	if(chnnRate > m_pDevInfo[chnn].ucRate[index])
	{
		chnnRate = m_pDevInfo[chnn].ucRate[index];
	}

	return chnnRate;
}

unsigned long CNetDeviceManager::AdjustResolution(unsigned long channel, unsigned long resolutionIndex)
{
	if(!(m_resolutionInfo[resolutionIndex].resolution & m_pDevInfo[channel].videoSize))
	{
		int j;
		for(j = resolutionIndex-1; j >= 0; j--)
		{
			if(m_resolutionInfo[j].resolution & m_pDevInfo[channel].videoSize)
			{
				resolutionIndex = j;
				break;
			}
		}

		if(j < 0)
		{
			for(j = resolutionIndex+1; j < RESOLUTION_COUNT; j++)
			{
				if(m_resolutionInfo[j].resolution & m_pDevInfo[channel].videoSize)
				{
					resolutionIndex = j;
					break;
				}
			}
		}

		if(RESOLUTION_COUNT == j)
		{
			assert(false);
		}
	}

	return resolutionIndex;
}

unsigned long CNetDeviceManager::AdjustThirdBitStream(unsigned long bitStream)
{
	unsigned long count = CProduct::Instance()->LocalBitRateCount();

	for(unsigned long i = 0; i < count; i++)
	{
		if(CProduct::Instance()->LocalBitRateValue(i) > bitStream)
		{
			if(i > 0)
			{
				long diff = bitStream - CProduct::Instance()->LocalBitRateValue(i-1);
				if((bitStream + diff) < CProduct::Instance()->LocalBitRateValue(i))
				{
					return CProduct::Instance()->LocalBitRateValue(i-1);
				}
				else
				{
					return CProduct::Instance()->LocalBitRateValue(i);
				}
			}
			else
			{
				return CProduct::Instance()->LocalBitRateValue(i);
			}
		}
		else if(CProduct::Instance()->LocalBitRateValue(i) == bitStream)
		{
			return CProduct::Instance()->LocalBitRateValue(i);
		}
	}

	return CProduct::Instance()->LocalBitRateValue(count - 1);
}

unsigned long CNetDeviceManager::AdjustThirdFrameRate(unsigned long chnn, unsigned long chnnAvailRate, unsigned long maxRate, unsigned long resolutionIndex)
{
	int resolution = VideoSizeToDDVideoSize(m_resolutionInfo[resolutionIndex].resolution);
	int index = GetBitIndex(resolution);
	unsigned long chnnRate = chnnAvailRate / m_resolutionInfo[resolutionIndex].multiNumCIF;
	if (chnnRate < 1)
	{
		chnnRate = 1;
	}
	else if(chnnRate > maxRate)
	{
		chnnRate = maxRate;
	}

	if(chnnRate > m_pThirdDevInfo[chnn].ucRate[index])
	{
		chnnRate = m_pThirdDevInfo[chnn].ucRate[index];
	}

	return chnnRate;
}

unsigned long CNetDeviceManager::AdjustThirdResolution(unsigned long channel, unsigned long resolutionIndex)
{
	if(!(m_resolutionInfo[resolutionIndex].resolution & m_pThirdDevInfo[channel].videoSize))
	{
		int j;
		for(j = resolutionIndex-1; j >= 0; j--)
		{
			if(m_resolutionInfo[j].resolution & m_pThirdDevInfo[channel].videoSize)
			{
				resolutionIndex = j;
				break;
			}
		}

		if(j < 0)
		{
			for(j = resolutionIndex+1; j < RESOLUTION_COUNT; j++)
			{
				if(m_resolutionInfo[j].resolution & m_pThirdDevInfo[channel].videoSize)
				{
					resolutionIndex = j;
					break;
				}
			}
		}

		if(RESOLUTION_COUNT == j)
		{
			//assert(false);
		}
	}

	return resolutionIndex;
}



unsigned char CNetDeviceManager::GetBitIndex(unsigned long mask)
{
	unsigned char bitCount = sizeof(unsigned long) * 8;
	for(unsigned char i = 0; i < bitCount; i++)
	{
		if((0x01 << i) & mask)
		{
			return i;
		}
	}

	return 255;
}

unsigned long CNetDeviceManager::GetResolutionIndex(unsigned long resolution)
{
	for(unsigned long i = 0; i < RESOLUTION_COUNT; i++)
	{
		if(m_resolutionInfo[i].resolution == resolution)
		{
			return i;
		}
	}

	assert(false);
	return RESOLUTION_COUNT;
}

void CNetDeviceManager::InitPoeChnnParam( NET_DEVICE_LINK_INFO &stDeviceInfo, int chnn )
{
	stDeviceInfo.deviceID = -1;
	stDeviceInfo.enabled = true;
	stDeviceInfo.channel = chnn;
	strcpy(stDeviceInfo.szEtherName, POE_ETHER);
#if defined(__CUSTOM_PL14__)
	strcpy(stDeviceInfo.username, "root");
	strcpy(stDeviceInfo.password, "pass");
#elif defined(__CUSTOM_CNB__)
	strcpy(stDeviceInfo.username, "root");
	strcpy(stDeviceInfo.password, "admin");
#else
	strcpy(stDeviceInfo.username, "admin");
	strcpy(stDeviceInfo.password, "123456");
#endif
	stDeviceInfo.manufacturerId = MID_TVT;
	strcpy(stDeviceInfo.productModel, "");
}


bool CNetDeviceManager::CheckIPCIsSame( const NET_DEVICE_LINK_INFO &linkInfo1, const NET_DEVICE_LINK_INFO &linkInfo2 )
{
	if( (linkInfo1.manufacturerId == linkInfo2.manufacturerId) 
		&& (linkInfo1.nPort == linkInfo2.nPort)
		&& (linkInfo1.nHttpPort == linkInfo2.nHttpPort )
		&& (0 == strcmp(linkInfo1.szServer, linkInfo2.szServer))
		&& (0 == strcmp(linkInfo1.szID, linkInfo2.szID)) )
	{
		return true;
	}

	return false;
}

bool CNetDeviceManager::IsPoeDevice(const NET_DEVICE_LINK_INFO &linkInfo)
{
	if(linkInfo.channel >= m_poeInputNum)
	{
		return false;
	}
	if(strcmp(linkInfo.szEtherName, POE_ETHER) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CNetDeviceManager::EnablePoeDiscoverThreadRun( bool bRunPoeDiscover )
{
	if( m_poeInputNum > 0 )
	{
		m_poeOnvifDeviceMan.EnablePoeDiscoverThreadRun( bRunPoeDiscover );
	}
}

unsigned long CNetDeviceManager::GetMaxRate(unsigned long chnn, unsigned long videoSize)
{
	if ((m_localVideoInputNum < chnn) && (chnn > m_videoInputNum))
	{
		return 1;
	}

	CNetDevice * pNetDevice = GetNetDeviceByChnn(chnn, true);

	if ((NULL != pNetDevice) && (ND_STATUS_OK == pNetDevice->GetDevStatus()))
	{
		unsigned long ddvideoSize = VideoSizeToDDVideoSize(videoSize);

		for (int i=0; i<32; i++)
		{
			if ((0x01 << i) & ddvideoSize)
			{
				return m_pDevInfo[chnn].ucRate[i];
			}
		}
	}

	return 1;
}

unsigned long CNetDeviceManager::GetMaxBitrate(unsigned long chnn, unsigned long videoSize)
{
	if ((m_localVideoInputNum < chnn) && (chnn > m_videoInputNum))
	{
		return 64;
	}

	CNetDevice * pNetDevice = GetNetDeviceByChnn(chnn, true);

	if ((NULL != pNetDevice) && (ND_STATUS_OK == pNetDevice->GetDevStatus()))
	{
		unsigned long ddvideoSize = VideoSizeToDDVideoSize(videoSize);

		for (int i=0; i<32; i++)
		{
			if ((0x01 << i) & ddvideoSize)
			{
				return m_pDevInfo[chnn].maxBitRate[i];
			}
		}
	}

	return 64;
}

void CNetDeviceManager::ClearNetLinkInfoEx(NET_LINK_INFO_EX &netLinkInfoEx)
{
	memset(&netLinkInfoEx, 0, sizeof(NET_LINK_INFO_EX));
	netLinkInfoEx.linkInfo.channel = INVALID_INDEX(unsigned char);
	netLinkInfoEx.status = ND_STATUS_DISCONNECT;
	netLinkInfoEx.pNetDevice =NULL;
	
//	netLinkInfoEx.linkInfo.channel=INVALID_INDEX(unsigned char);
//	CMessageMan::Instance()->SetNetDeviceStatus(true, netLinkInfoEx.linkInfo.channel);
}
void CNetDeviceManager::ClearAllNetLinkInfoEx(NET_LINK_INFO_EX *pNetLinkInfo)
{
	for(long netCnt=0;netCnt<m_netVideoInputNum;netCnt++)
	{
		ClearNetLinkInfoEx(pNetLinkInfo[netCnt]);
	}
}


bool CNetDeviceManager::IfCheckStatusOver()
{
	bool ret = true;
	for(unsigned char chanCnt=0;chanCnt<m_netVideoInputNum;chanCnt++)
	{
		if(m_undealChannFlag[chanCnt] == true)
		{
			ret = false;
		}
	}
	return ret;
}

bool CNetDeviceManager::IfChannNetLinkInfoValid(NET_LINK_INFO_EX &netLinkInfoEx)
{
	if( (netLinkInfoEx.linkInfo.deviceID == 0) 
		&& (netLinkInfoEx.linkInfo.nPort == 0)
		&& (netLinkInfoEx.linkInfo.nHttpPort == 0)
		&& (netLinkInfoEx.linkInfo.nCtrlPort == 0)
		&& (0 == strcmp(netLinkInfoEx.linkInfo.szServer,""))
		&& (0 == strcmp(netLinkInfoEx.linkInfo.szID,""))
		&& (netLinkInfoEx.linkInfo.channel == INVALID_INDEX(unsigned char)))
	{
		return false;
	}

	return true;
}

//end
