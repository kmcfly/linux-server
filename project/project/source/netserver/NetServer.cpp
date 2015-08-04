// NetServer.cpp: implementation of the CNetServer class.
//
//////////////////////////////////////////////////////////////////////
#include "NetServer.h"
#include "NetProtocol.h"
#include "FrameData.h"
#include "FrameBuffer.h"
#include "dvrdvsdef.h"
#include "NetConfigDefine.h"
#include "SWL_Public.h"


#ifdef __ENVIRONMENT_LINUX__
#include "sys/ioctl.h"
#include "net/if.h"
#include "NetInterface.h"
#endif
#include "MulticastHeader.h"

#include "Product.h"

extern bool              g_curNet3G;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetServer::CNetServer()
{
	m_pBitmapInfo_F			= NULL;
	m_pBitmapInfo_S			= NULL;
	m_pWaveFormatEx			= NULL;
	m_pFastVideoNum			= NULL;

	m_pListenProc			= NULL;
	m_pNatListenManager		= NULL;
	m_dwMaxLink				= 0;
	m_pCallbackObject		= NULL;
	m_sendDataID			= 0;
	m_bSendDataRun			= false;
	m_pMessageMan			= NULL;
	m_pStatusData			= NULL;
	m_pRecStatusData		= NULL;
	m_statusDataLen			= 0;
	m_recStatusDataLen		= 0;
	m_bEnableMasterStream	= true;
    m_NatIsStarted         = false;

	m_ban_allow_type		= BAN_ALLOW_TYPE_BAN;


	memset(&m_netInitInfo, 0, sizeof(m_netInitInfo));
	memset(&m_messageData, 0, sizeof(m_messageData));
	memset(&m_talkAudioFormat, 0, sizeof(WAVEFORMATEX));
	
	m_bLimitFirstStreamSendNum = false;
	m_maxFirstStreamSendNum = 0;
	m_curFirstStreamSendNum = 0;
#ifdef __SUPPORT_3G__
	m_bStartPlayback = false;
#endif
	m_maxIPCTransferChnnNum = 0;
	m_curIPCTransferChnnNum = 0;

	memset(&m_natDeviceCfg, 0, sizeof(NAT_DEVICE_CONFIG));
	memset(&m_natSeverInfo, 0, sizeof(NAT_SERVER_INFO));

	PUB_InitLock(&m_StartLock);
}

CNetServer::~CNetServer()
{
	PUB_DestroyLock(&m_StartLock);
}

/******************************************************************************
*
*							public method
*
******************************************************************************/
CNetServer* CNetServer::Instance()
{
	static CNetServer s_NetServer;
	return &s_NetServer;
}

int	CNetServer::Initial(const NET_INIT_INFO	*pNetInitInfo, CMessageMan *pMessageMan, long maxSupportFastVideo, bool bSupportDoubleStream)
{
	m_netInitInfo = *pNetInitInfo;
	m_pMessageMan = pMessageMan;
	memset(&m_messageData, 0, sizeof(MESSAGE_DATA));
	m_statusDataLen = sizeof(PACKCMD) + sizeof(NET_STATUS);
	m_pStatusData = new unsigned char [m_statusDataLen];
	memset(m_pStatusData, 0, m_statusDataLen);

	m_recStatusDataLen = sizeof(PACKCMD) + sizeof(NET_REC_STATUS);
	m_pRecStatusData = new unsigned char [m_recStatusDataLen];
	memset(m_pRecStatusData, 0, m_recStatusDataLen);
	
	m_maxSupportFastVideo  = maxSupportFastVideo;
	
	m_pBitmapInfo_F = new BITMAPINFOHEADER[pNetInitInfo->dwMaxVideo];
	m_pBitmapInfo_S = new BITMAPINFOHEADER[pNetInitInfo->dwMaxVideo];
	m_pWaveFormatEx = new WAVEFORMATEX[pNetInitInfo->dwMaxVideo];
	m_pFastVideoNum = new unsigned char[pNetInitInfo->dwMaxVideo];

	memset(m_pBitmapInfo_F, 0, sizeof(BITMAPINFOHEADER) * pNetInitInfo->dwMaxVideo);
	memset(m_pBitmapInfo_S, 0, sizeof(BITMAPINFOHEADER) * pNetInitInfo->dwMaxVideo);
	memset(m_pWaveFormatEx, 0, sizeof(WAVEFORMATEX) * pNetInitInfo->dwMaxVideo);
	memset(m_pFastVideoNum, 0, sizeof(unsigned char) * pNetInitInfo->dwMaxVideo);

	//分配一个监听对象
	m_pListenProc = NULL;

	m_bSupportDoubleStream = bSupportDoubleStream;

	m_bLimitFirstStreamSendNum = false;
	m_maxFirstStreamSendNum = 0;
	m_curFirstStreamSendNum = 0;

	m_maxIPCTransferChnnNum = 0;
	m_curIPCTransferChnnNum = 0;


	m_bAutoReportRun = false;
	m_autoReportThreadID = PUB_THREAD_ID_NOINIT;
	m_bConectOk = false;
	m_bSupportAutoReport = false;


	return 0;
}


int CNetServer::AutoReportInitial(bool bSupport, const NET_AUTO_REPORT *pNetAutoReport)
{
	m_bSupportAutoReport = bSupport;
	m_netAutoReport.bUse = pNetAutoReport->bUse;
	m_netAutoReport.dwPort= pNetAutoReport->dwPort;
	m_netAutoReport.ID = pNetAutoReport->ID;
	strcpy(m_netAutoReport.host, pNetAutoReport->host);
	return 0;
}


void CNetServer::SetMaxFirstStreamSendNum(bool bLimitSendNum, unsigned char maxPlayBackChnnNum)
{
	m_bLimitFirstStreamSendNum = bLimitSendNum;
	m_maxFirstStreamSendNum = maxPlayBackChnnNum;
}

void CNetServer::SetMaxIPCTransferChnnNum(unsigned long maxIPCTransferChnnNum)
{
	printf("maxIPCTransferChnnNum = %d\n", maxIPCTransferChnnNum);
	m_maxIPCTransferChnnNum = maxIPCTransferChnnNum;
}

void CNetServer::Quit()
{
	delete [] m_pRecStatusData;
	delete [] m_pStatusData;
	delete [] m_pBitmapInfo_F;
	delete [] m_pBitmapInfo_S;
	delete [] m_pWaveFormatEx;
	delete [] m_pFastVideoNum;
}

int CNetServer::ChangeListenPort(short int nPort)
{
	if( nPort == m_netInitInfo.nPort ) 
	{
		return 0;
	}

	if( !m_bSendDataRun )
	{
		//修改.../WebServer/script/server.js文件， 该文件中存放的是数据端口号
		//在导出配置后，第一次启动时要写数据端口
		//但是这会导致每次启动都要写一次该文件
		if( 0 != RewriteServerJS( nPort ) )
		{
			assert(0);
		}

		return 0;
	}

	{
		CPUB_LockAction funcLock(&m_StartLock);
		//停止监听线程
		m_pListenProc->StopListen();
		delete m_pListenProc;
		m_pListenProc = NULL;

		//清空所有的客户连接信息
		POS PrePos = NULL;
		POS pos = m_lstClientServiceInfo.GetHeadPosition();
		while( NULL != pos )
		{
			PrePos = pos;
			CLIENT_SERVICE_INFO * pClientInfo = m_lstClientServiceInfo.GetNext( pos );			 

			if (CONNECTTYPE_ACTIVE_CONNECT != pClientInfo->connectType)
			{
				RemoveClient(pClientInfo);
				m_lstClientServiceInfo.RemoveAt(PrePos);								
			}			
		}		
	}
	
	//修改.../WebServer/script/server.js文件， 该文件中存放的是数据端口号
	if( 0 != RewriteServerJS( nPort ) )
	{
		assert(0);
	}

	m_netInitInfo.nPort = nPort;
	
	//从新监听
	m_pListenProc = new CSWL_ListenProcEx(AcceptLinkCallback, this);
	m_pListenProc->StartListen( m_netInitInfo.nPort );
	

	//重启NAT的监听
	if (NULL != m_pNatListenManager)
	{
		m_pNatListenManager->SetDeviceDataPort(nPort);
	}

	return 0;
}

int CNetServer::ChangeHttpPort(short int nPort)
{
	CPUB_LockAction funcLock(&m_StartLock);

	m_netInitInfo.nHttpPort = nPort;

	//重启NAT的监听
	if (NULL != m_pNatListenManager)
	{
		m_pNatListenManager->SetDeviceWebPort(nPort);
	}

	return 0;
}

int CNetServer::ChangeMaxLink(unsigned char dwLink)
{
	CPUB_LockAction funcLock(&m_StartLock);

	m_netInitInfo.dwMaxLink = dwLink;
	return 0;
}


int CNetServer::ChangeAutoCms(NET_AUTO_REPORT cmsInfo)
{
	CPUB_LockAction funcLock(&m_StartLock);

	//清空所有的客户连接信息
	POS PrePos = NULL;
	POS pos = m_lstClientServiceInfo.GetHeadPosition();
	while( NULL != pos )
	{
		PrePos = pos;
		CLIENT_SERVICE_INFO * pClientInfo = m_lstClientServiceInfo.GetNext( pos );
		if (CONNECTTYPE_ACTIVE_CONNECT == pClientInfo->connectType)
		{
			RemoveClient(pClientInfo);
			m_lstClientServiceInfo.RemoveAt(PrePos);
		}		
	}	
	
	m_bConectOk = false;

	m_netAutoReport.bUse = cmsInfo.bUse;
	m_netAutoReport.dwPort = cmsInfo.dwPort;
	m_netAutoReport.ID = cmsInfo.ID;
	strcpy(m_netAutoReport.host,cmsInfo.host);
	return  0;

}


int CNetServer::ChangeNatServerInfo(const NAT_SERVER_INFO * pNetServerInfo)
{
	static bool bHaveStartNat = false;
#if defined(__TVT_NAT_SOCK__)
	snprintf(m_natDeviceCfg.serverIp, sizeof(m_natDeviceCfg.serverIp), "%s", pNetServerInfo->szServerAddr);
	m_natDeviceCfg.serverPort = pNetServerInfo->iServerPort;

	m_natSeverInfo = * pNetServerInfo;
    //开机是netServer还没启动时，只更新配置

    if(!bHaveStartNat) 
    {
		bHaveStartNat = true;
        return 0;
    }

    if(pNetServerInfo->bStartNat == 0) //stop
    {
        NatStop();
    }
    else
    {
		NatStart();
    }
	
#endif
return 0;
}
int CNetServer::NatStart()
{
    #if defined(__TVT_NAT_SOCK__)


	if(NatIsStarted())
	{
		if(NULL != m_pNatListenManager)
		{	
			m_pNatListenManager->ResetServer(m_natSeverInfo.szServerAddr, m_natSeverInfo.iServerPort);
		}

		return 0;
	}

    if(!m_multiNetNat.Init(CHANNEL_BUFFER_SIZE, true))
    {
        PUB_PrintError(__FILE__,__LINE__);
        return -1;
    }
    //开始监听网络连接
    m_pNatListenManager = new CNatListenManager(AcceptLinkCallback, this);

	m_pNatListenManager->SetCache("./config/NatListenCache.dat");
	m_natDeviceCfg.deviceDataPort = m_netInitInfo.nPort;
	m_natDeviceCfg.deviceWebPort  = m_netInitInfo.nHttpPort;

	printf("%s:%d, product:%d, port:%d, web:%d, ver:%s, s:%s:%d\n", __FILE__, __LINE__, m_natDeviceCfg.deviceType, m_natDeviceCfg.deviceDataPort, m_natDeviceCfg.deviceWebPort, 
		m_natDeviceCfg.deviceVersion, m_natDeviceCfg.serverIp, m_natDeviceCfg.serverPort);

    m_pNatListenManager->StartListen(&m_natDeviceCfg);
    m_NatIsStarted  = true;
#endif

	return 0;
}

int CNetServer::NatIsStarted()
{
    return m_NatIsStarted;
}
int CNetServer::NatStop()
{
#if defined(__TVT_NAT_SOCK__)
	//清空所有的穿透客户连接信息
	POS PrePos = NULL;
	POS pos = m_lstClientServiceInfo.GetHeadPosition();
	while( NULL != pos )
	{
		PrePos = pos;
		CLIENT_SERVICE_INFO * pClientInfo = m_lstClientServiceInfo.GetNext( pos );
		if (&m_multiNetNat == pClientInfo->pMultiNet)
		{
			RemoveClient(pClientInfo);
			m_lstClientServiceInfo.RemoveAt(PrePos);
		}
	}

	if (NULL != m_pNatListenManager)
	{
		m_pNatListenManager->StopListen();
		delete m_pNatListenManager;
		m_pNatListenManager = NULL;
	}
	m_multiNetNat.Destroy();
	m_NatIsStarted =false;
#endif

	return 0;
}
/******************************************************************************
*
*	启动网络服务
*	成功返回0，已经启动返回1，启动失败返回-1
*
******************************************************************************/
int CNetServer::Start()
{
	CPUB_LockAction funcLock(&m_StartLock);

	if(m_bSendDataRun)
	{
		return 1;
	}

	m_sendDataID = PUB_CreateThread(SendDataThread, this, &m_bSendDataRun);
	if(m_sendDataID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return -1;
	}

	if(!m_multiNetComm.Init(CHANNEL_BUFFER_SIZE, true))
	{
		PUB_PrintError(__FILE__,__LINE__);
		return -1;
	}

	//开始监听网络连接
	m_pListenProc = new CSWL_ListenProcEx(AcceptLinkCallback, this);
	m_pListenProc->StartListen(m_netInitInfo.nPort);

	if(m_bSupportAutoReport)
	{
		m_autoReportThreadID = PUB_CreateThread(ConectCmsThread, this, &m_bAutoReportRun);
		if(m_autoReportThreadID == PUB_CREATE_THREAD_FAIL)
		{
			PUB_PrintError(__FILE__,__LINE__);
			return -1;
		}
	}
	


#if defined(__TVT_NAT_SOCK__)
	char szMac[10] = {0};
#ifndef _WIN32
	if (CNetInterface::Instance()->GetMAC(szMac))
	{
		snprintf(m_natDeviceCfg.deviceNo, sizeof(m_natDeviceCfg.deviceNo), "%02x%02x%02x%02x%02x%02x", szMac[0],szMac[1],szMac[2],szMac[3],szMac[4],szMac[5]);
		printf("%s:%s:%d, the device number=%s\n", __FUNCTION__, __FILE__, __LINE__, m_natDeviceCfg.deviceNo);
	}
	else
#endif
	{
		snprintf(m_natDeviceCfg.deviceNo, sizeof(m_natDeviceCfg.deviceNo), "456789");
		printf("%s:%s:%d, Get device MAC error\n", __FUNCTION__, __FILE__, __LINE__);
	}

	m_natDeviceCfg.deviceDataPort = m_netInitInfo.nPort;
	m_natDeviceCfg.deviceWebPort  = m_netInitInfo.nHttpPort;

	m_natDeviceCfg.deviceType = CProduct::Instance()->ProductType();
	CProduct::Instance()->GetFirmwareVersion(m_natDeviceCfg.deviceVersion, sizeof(m_natDeviceCfg.deviceVersion));

	m_natDeviceCfg.localIp	= 0;
	m_natDeviceCfg.localPort	= 7000;

	if(m_natSeverInfo.bStartNat)
	{
		NatStart();
	}
#endif


	//启动组播线程
#ifdef   __ENVIRONMENT_LINUX__
	m_multicastID = PUB_CreateThread(MulticastThread, this, &m_bmulticastRun);
	if( m_multicastID == PUB_CREATE_THREAD_FAIL )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return -1;
	}
#endif
	return 0;
}

/******************************************************************************
*
*	停止网络服务
*	成功返回0，已经停止返回1	
*
******************************************************************************/
int CNetServer::Stop()
{
	{
		CPUB_LockAction funcLock(&m_StartLock);

		// 已经stop
		if (!m_bSendDataRun) 
		{
			return 1;
		}		
	}

	if (m_pListenProc != NULL)
	{
		m_pListenProc->StopListen();
		delete m_pListenProc;
		m_pListenProc = NULL;
	}
	
	PUB_ExitThread(&m_sendDataID, &m_bSendDataRun);

	m_multiNetComm.Destroy();
	
	NatStop();

#ifdef   __ENVIRONMENT_LINUX__
	PUB_ExitThread(&m_multicastID, &m_bmulticastRun);
#endif

	if( PUB_THREAD_ID_NOINIT != m_autoReportThreadID )
	{
		PUB_ExitThread( &m_autoReportThreadID, &m_bAutoReportRun );
	}

	return 0;
}

void CNetServer::ClearData(unsigned long stream)
{
	CPUB_LockAction funcLock(&m_StartLock);

	//同时清除主码流和子码流，意味着要清除所有的数据，改为只要清除主码流就同时清除音频（因为主码流与音频在同一个列表中）
	unsigned long type = (LIVE_FRAME_FIRST_STREAM & stream) ? CLIENT_ALL_DATA : CLIENT_VIDEO_DATA;

	POS pos = m_lstClientServiceInfo.GetHeadPosition();
	while(NULL != pos)
	{
		CLIENT_SERVICE_INFO *pServiceInfo = m_lstClientServiceInfo.GetNext(pos);
		if(!pServiceInfo->bValidateClient)
		{
			continue;
		}

		if (LIVE_FRAME_FIRST_STREAM & stream)
		{
			pServiceInfo->pAVDataSendMan->CleanLiveData(pServiceInfo->dwLiveStreamID, type, VIDEO_STREAM_FAST_NET);
		}
		
		if (LIVE_FRAME_SECOND_STREAM & stream)
		{
			pServiceInfo->pAVDataSendMan->CleanLiveData(pServiceInfo->dwLiveStreamID, type, VIDEO_STREAM_SLOW_NET);
		}
	}
}

void CNetServer::ClearPBData(long clientID, unsigned long streamID)
{
	CPUB_LockAction funcLock(&m_StartLock);

	POS pos = m_lstClientServiceInfo.GetHeadPosition();
	while(NULL != pos)
	{
		CLIENT_SERVICE_INFO *pServiceInfo = m_lstClientServiceInfo.GetNext(pos);
		if(!pServiceInfo->bValidateClient)
		{
			continue;
		}

		if(pServiceInfo->clientID != clientID)
		{
			continue;
		}

		pServiceInfo->pAVDataSendMan->CleanPBData(streamID);

		printf("CNetServer clear pbdata\n");
	}
}

unsigned long CNetServer::GetLastStreamError(long clientID)
{
	CPUB_LockAction funcLock(&m_StartLock);

	POS pos = m_lstClientServiceInfo.GetHeadPosition();
	while(NULL != pos)
	{
		CLIENT_SERVICE_INFO *pServiceInfo = m_lstClientServiceInfo.GetNext(pos);
		if(pServiceInfo->clientID == clientID)
		{
			assert(pServiceInfo->bValidateClient);

			if(pServiceInfo->pLastStreamErrorList->empty())
			{
				return pServiceInfo->lastStreamError;
			}

			unsigned long lastStreamError = pServiceInfo->pLastStreamErrorList->front();
			pServiceInfo->pLastStreamErrorList->pop_front();
			return lastStreamError;
		}
	}

	return NETERR_STREAM_OK;
}

//得到指定通道请求数据的码率
unsigned long CNetServer::GetStreamEncodedByteRate(long clientID, long channel, unsigned long frameType)
{
	CPUB_LockAction funcLock(&m_StartLock);

	POS pos = m_lstClientServiceInfo.GetHeadPosition();
	while(pos)
	{
		CLIENT_SERVICE_INFO *pServiceInfo = m_lstClientServiceInfo.GetNext(pos);
		if(!pServiceInfo->bValidateClient)
		{
			continue;
		}

		if(pServiceInfo->clientID == clientID)
		{
			ULONGLONG ullDataBytes = pServiceInfo->pAVDataSendMan->GetStreamArrivedBytes(channel, frameType);
			LONGLONG llDataTime = pServiceInfo->pAVDataSendMan->GetStatisticTime(channel);

			if(llDataTime > 0)
			{
				return ullDataBytes*1000000/llDataTime;
			}
		}
	}

	return 0;
}

//得到指定通道实际网络发送的码率
unsigned long CNetServer::GetStreamSentByteRate(long clientID, long channel, unsigned long frameType)
{
	CPUB_LockAction funcLock(&m_StartLock);

	POS pos = m_lstClientServiceInfo.GetHeadPosition();
	while(pos)
	{
		CLIENT_SERVICE_INFO *pServiceInfo = m_lstClientServiceInfo.GetNext(pos);
		if(!pServiceInfo->bValidateClient)
		{
			continue;
		}

		if(pServiceInfo->clientID == clientID)
		{
			ULONGLONG ullDataBytes = pServiceInfo->pAVDataSendMan->GetStreamSentBytes(channel, frameType);
			LONGLONG llDataTime = pServiceInfo->pAVDataSendMan->GetStatisticTime(channel);

			if(llDataTime > 0)
			{
				return ullDataBytes*1000000/llDataTime;
			}
		}
	}

	return 0;
}

//重置统计零点
void CNetServer::ResetStreamStatistics(long clientID /* = -1 */, long channel /* = ALL_CHANNELS */)
{
	CPUB_LockAction funcLock(&m_StartLock);

	POS pos = m_lstClientServiceInfo.GetHeadPosition();
	while(pos)
	{
		CLIENT_SERVICE_INFO *pServiceInfo = m_lstClientServiceInfo.GetNext(pos);
		if(!pServiceInfo->bValidateClient)
		{
			continue;
		}

		if(pServiceInfo->clientID == clientID)
		{
			pServiceInfo->pAVDataSendMan->ResetStreamBytesStatistics(channel);
			break;
		}
	}
}

void CNetServer::EnableMasterStream(bool bEnable)
{
	CPUB_LockAction funcLock(&m_StartLock);

	MESSAGE_DATA msgData;
	msgData.deviceID = 0;
	msgData.dataLen = sizeof(PACKCMD) + sizeof(NET_LIVESTREAM_CTRL_INFO);
	msgData.pData = new char [msgData.dataLen];
	memset(msgData.pData, 0, msgData.dataLen);
	PACKCMD *pPackCmd = (PACKCMD *)msgData.pData;
	NET_LIVESTREAM_CTRL_INFO *pLiveStreamReq = (NET_LIVESTREAM_CTRL_INFO *)(msgData.pData+sizeof(PACKCMD));
	
	pPackCmd->cmdType = CMD_REQUEST_KEYFRAME;
	pPackCmd->dataLen = sizeof(NET_LIVESTREAM_CTRL_INFO);

	if(bEnable)
	{
		if(m_bEnableMasterStream)
		{
			return;
		}
		
		CLIENT_SERVICE_INFO *pClientServiceInfo;
		POS pos = m_lstClientServiceInfo.GetHeadPosition();
		while(pos != NULL)
		{
			pClientServiceInfo = m_lstClientServiceInfo.GetNext(pos);
			if(pClientServiceInfo->ullMasterVideo != 0)
			{
				pClientServiceInfo->pAVDataSendMan->ChangeStream(pClientServiceInfo->ullMasterVideo, \
					pClientServiceInfo->ullSecondVideo, \
					pClientServiceInfo->ullAudio, pClientServiceInfo->dwLiveStreamID);

				msgData.clientID = pClientServiceInfo->clientID;
			}
		}
		
		m_bEnableMasterStream = true;
		pLiveStreamReq->fastVideoCH = pClientServiceInfo->ullMasterVideo;
		m_pMessageMan->PutMsgToServer(msgData);
	}
	else
	{
		if(!m_bEnableMasterStream)
		{
			return;
		}

		CLIENT_SERVICE_INFO *pClientServiceInfo;
		POS pos = m_lstClientServiceInfo.GetHeadPosition();
		while(pos != NULL)
		{
			pClientServiceInfo = m_lstClientServiceInfo.GetNext(pos);
			if(pClientServiceInfo->ullMasterVideo != 0)
			{
				pClientServiceInfo->pAVDataSendMan->ChangeStream(0, \
					pClientServiceInfo->ullMasterVideo | pClientServiceInfo->ullSecondVideo, \
					pClientServiceInfo->ullAudio, pClientServiceInfo->dwLiveStreamID);

				pLiveStreamReq->secondVideoCH = pClientServiceInfo->ullMasterVideo;
				m_pMessageMan->PutMsgToServer(msgData);
			}
		}

		m_bEnableMasterStream = false;
	}
}

/******************************************************************************
在访问根据这份名单来决定接受还是拒绝
******************************************************************************/

void CNetServer::SetBanAllowType(BAN_ALLOW_TYPE type)
{
	m_ban_allow_type = type;
}

void CNetServer::SetBanAllowList(const IP_SECTION *pIP, unsigned char num)
{
	assert (NULL != pIP);
	assert ((0 <= num) && (num <= MAX_BAN_ALLOW_LIST_NUM));

	m_ban_allow_list.clear();

	for (unsigned char i=0; i<num; ++i)
	{
		if (pIP[i].enable)
		{
			m_ban_allow_list.push_back(pIP[i]);
		}
	}
}


unsigned long CNetServer::GetFirstStreamSendNum()
{
	CPUB_LockAction funcLock(&m_StartLock);

	unsigned long retNum = 0;
	for (int i = 0; i < m_netInitInfo.dwMaxVideo; ++i)
	{
		retNum += m_pFastVideoNum[i];
	}

	return retNum;
}

ULONGLONG CNetServer::GetFirstStreamCH()
{
	CPUB_LockAction funcLock(&m_StartLock);

	ULONGLONG ullCH = 0;
	CLIENT_SERVICE_INFO *pClientServiceInfo;
	POS pos = m_lstClientServiceInfo.GetHeadPosition();
	while(pos != NULL)
	{
		pClientServiceInfo = m_lstClientServiceInfo.GetNext(pos);
		if(pClientServiceInfo->ullMasterVideo != 0)
		{
			ullCH |= pClientServiceInfo->ullMasterVideo;
		}
	}

	return ullCH;
}

/******************************************************************************
*
*							private method
*
******************************************************************************/

/******************************************************************************
*
*	监听网络上的连接，收到新的连接后，把连接进行初始化并加入到连接列表中
	如果连接已超过了最大可允许的连接数时，拒绝该连接。
*
******************************************************************************/
int CNetServer::AcceptLinkCallback(CLIENT_SOCK_INFO *pClientSockInfo, void *pParam1, void *pParam2)
{
	CNetServer *pThis = reinterpret_cast<CNetServer *>(pParam1);

	unsigned long sockType = SERVER_COMMON;
	if (NULL != pParam2)
	{
		sockType = *(unsigned long *)pParam2;
	}

	return pThis->OnGetAcceptLink(pClientSockInfo, sockType);
}

int	CNetServer::OnGetAcceptLink(CLIENT_SOCK_INFO *pClientSockInfo,unsigned long sockType)
{
#if defined(__CHIP3535__)
	int optVal = 8;
	setsockopt(pClientSockInfo->pTcpSock, IPPROTO_IP, IP_TOS, (char *)&optVal, sizeof(int));
#endif

	//处理黑白名单
	{
		bool bAccept = (BAN_ALLOW_TYPE_BAN == m_ban_allow_type) ? true : false;
		unsigned char *pIP = reinterpret_cast<unsigned char *>(&pClientSockInfo->dwIP);
		unsigned char *pStart = NULL, *pEnd = NULL;

		std::list<IP_SECTION>::iterator it;
		for (it = m_ban_allow_list.begin(); it != m_ban_allow_list.end(); ++it) 
		{
			//以网络号、子网号、主机号为序
			pStart = reinterpret_cast<unsigned char *>(&(*it).start);
			pEnd = reinterpret_cast<unsigned char *>(&(*it).end);

			if ((pStart[3] <= pIP[3]) && (pIP[3] <= pEnd[3]))
			{
				if ((pStart[2] <= pIP[2]) && (pIP[2] <= pEnd[2]))
				{	
					if ((pStart[1] <= pIP[1]) && (pIP[1] <= pEnd[1]))
					{
						if ((pStart[0] <= pIP[0]) && (pIP[0] <= pEnd[0]))
						{
							bAccept = (BAN_ALLOW_TYPE_BAN == m_ban_allow_type) ? false : true;
							break;
						}
					}
				}
			}
		}

		if (!bAccept)
		{
			//断开连接
			//SWL_CloseSocket(pClientSockInfo->pTcpSock);
			return -1;
		}
	}

	if (SERVER_NAT == sockType)
	{
#if !defined(__TVT_NAT_SOCK__)
		assert(false);
		return -1;
#endif
	}

	CPUB_LockAction funcLock(&m_StartLock);

	CLIENT_SERVICE_INFO *pClientServiceInfo = new CLIENT_SERVICE_INFO;
	memset(pClientServiceInfo, 0, sizeof(CLIENT_SERVICE_INFO));

	if (SERVER_COMMON == sockType)
	{
		pClientServiceInfo->pMultiNet = &m_multiNetComm;
	}
	else
	{
#if defined(__TVT_NAT_SOCK__)
		pClientServiceInfo->pMultiNet = &m_multiNetNat;
#else

#endif

	}

	pClientServiceInfo->clientID = m_pMessageMan->AllocNetClientID();
	pClientServiceInfo->dwIP = pClientSockInfo->dwIP;
	pClientServiceInfo->wPort = pClientSockInfo->nPort;
	if (SERVER_COMMON == sockType)
	{
		pClientServiceInfo->pMultiNet->AddNewComm(pClientServiceInfo->clientID, pClientSockInfo->pTcpSock);
	}
	else
	{
		pClientServiceInfo->pMultiNet->AddNewComm(pClientServiceInfo->clientID, (NatSocket)pClientSockInfo->pTcpSock);
	}

	pClientServiceInfo->pMultiNet->SetAutoRecvCallback(pClientServiceInfo->clientID, RecvDataCallback, this);
	pClientServiceInfo->pMultiNet->SetRecvMode(pClientServiceInfo->clientID, true);
	pClientServiceInfo->pAVDataSendMan = new CAVDataSendMan(pClientServiceInfo->clientID, pClientServiceInfo->pMultiNet, \
		m_netInitInfo.dwMaxVideo, m_netInitInfo.dwMaxAudio);
	pClientServiceInfo->pStreamCHMap = new STREAM_CH_MAP;
	pClientServiceInfo->pLastStreamErrorList = new list<unsigned long>;
	pClientServiceInfo->lastStreamError = NETERR_STREAM_OK;
    pClientServiceInfo->loginMark = 0;
	//分配发送缓冲区
	pClientServiceInfo->sendBufferSize = SEND_BUFFER_SIZE;
	pClientServiceInfo->pSendBuffer = new unsigned char [SEND_BUFFER_SIZE];
	memset(pClientServiceInfo->pSendBuffer, 0, SEND_BUFFER_SIZE);

	//设定报警状态初始值为-1
	pClientServiceInfo->netRecStatus.chnn_count = m_netInitInfo.dwMaxVideo;
	pClientServiceInfo->netRecStatus.ullManualRec = INVALID_INDEX(ULONGLONG);
	pClientServiceInfo->netRecStatus.ullScheduleRec = INVALID_INDEX(ULONGLONG);
	pClientServiceInfo->netRecStatus.ullMotionRec = INVALID_INDEX(ULONGLONG);
	pClientServiceInfo->netRecStatus.ullSensorRec = INVALID_INDEX(ULONGLONG);
	pClientServiceInfo->ullMotionState = INVALID_INDEX(ULONGLONG);
	pClientServiceInfo->ullSensorInState = INVALID_INDEX(ULONGLONG);
	pClientServiceInfo->ullVideoLoss = INVALID_INDEX(ULONGLONG);
	pClientServiceInfo->ullNetLogin = INVALID_INDEX(ULONGLONG);

	//把当前客户端加入到客户端列表
	m_lstClientServiceInfo.AddTail(pClientServiceInfo);

	printf("---------------------------new connection established\n");

	//发送预发送数据
	PRE_SEND_DATA preSendData;
	memset(&preSendData, 0, sizeof(PRE_SEND_DATA));
	preSendData.dwFlag = SWL_HEADFLAG;
	preSendData.dwDevType = m_netInitInfo.dwDeviceType;
	preSendData.dwProductType = m_netInitInfo.dwProductType;
	preSendData.dwProtocolVer = NET_PROTOCOL_VER;
	preSendData.dwConfigVer = NCFG_VERSION;
	*(reinterpret_cast<unsigned short *>(&preSendData) + 31) = m_netInitInfo.dwProductSubID;

	int ret = pClientServiceInfo->pMultiNet->SendDataSync(pClientServiceInfo->clientID, &preSendData, sizeof(PRE_SEND_DATA), true);
	if(ret != sizeof(PRE_SEND_DATA))
	{
		m_lstClientServiceInfo.RemoveTail();
		pClientServiceInfo->pMultiNet->SetAutoRecvCallback(pClientServiceInfo->clientID, NULL, this);
		pClientServiceInfo->pMultiNet->RemoveComm(pClientServiceInfo->clientID);
		delete [] pClientServiceInfo->pSendBuffer;
		delete pClientServiceInfo->pStreamCHMap;
		delete pClientServiceInfo->pAVDataSendMan;
		delete pClientServiceInfo;
	}
	
	return 0;
}



PUB_THREAD_RESULT PUB_THREAD_CALL CNetServer::ConectCmsThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CNetServer *pThis = reinterpret_cast<CNetServer *>(pParam);
	pThis->ConectCmsRun();

	return 0;
}

int CNetServer::ConectCmsRun()
{
#ifdef __ENVIRONMENT_LINUX__
	SWL_socket_t sockSend = SWL_INVALID_SOCKET;
	bool bConectOk = false;
	NET_AUTO_REPORT netAutoReport;
	struct sockaddr_in struServerAddr;
	SWL_socklen_t addrLen = sizeof(struServerAddr);

	
	while (m_bAutoReportRun)
	{
		do
		{	
			{
				CPUB_LockAction funcLock(&m_StartLock);
				if (!m_netAutoReport.bUse)
				{
					break;
				}
				else if (m_bConectOk)
				{
					break;
				}
				else
				{					
					sockSend = SWL_CreateSocket(AF_INET, SOCK_STREAM, 0);

					//当存在3G网卡情况下，优先使用ppp0设备。故在此做了绑定网络设备。
					//struct ifreq if_ppp0;
					//strncpy(if_ppp0.ifr_name, "eth0", IFNAMSIZ);
					//setsockopt(sockSend, SOL_SOCKET, SO_BINDTODEVICE, (char*)&if_ppp0, sizeof(if_ppp0));

					if(SWL_INVALID_SOCKET == sockSend)
					{							
						break;
					}
					
					netAutoReport.bUse = m_netAutoReport.bUse;
					netAutoReport.dwPort = m_netAutoReport.dwPort;
					netAutoReport.ID = m_netAutoReport.ID;
					strcpy(netAutoReport.host, m_netAutoReport.host);
				}				
			}
			struct in_addr* addr = new struct in_addr;
			if(INADDR_NONE == inet_aton(netAutoReport.host, addr))
			{
				struct hostent* pHost = CNetInterface::Instance()->GetHostByName(netAutoReport.host);
				if (pHost == NULL)
				{
					printf("%s, %d\n", __FILE__, __LINE__);
					SWL_CloseSocket(sockSend);
					sockSend = SWL_INVALID_SOCKET;
					break;
				}
				struServerAddr.sin_addr 	= *((struct in_addr*)pHost->h_addr);
				CNetInterface::Instance()->ReleaseHost( pHost );
			}
			else
			{
				struServerAddr.sin_addr  = *addr;
			}
			delete addr;
			//printf("%s, %d,s %, %d, %d\n", __FILE__, __LINE__, pHost->h_name);
			struServerAddr.sin_family = AF_INET;
			struServerAddr.sin_port = htons(netAutoReport.dwPort);
			memset(&(struServerAddr.sin_zero), '\0', 8); 


			//连接
			int iRet = SWL_Connect(sockSend, reinterpret_cast<struct sockaddr*>(&struServerAddr), addrLen, 8000);
			if (0 != iRet)
			{	
				SWL_CloseSocket(sockSend);
				sockSend = SWL_INVALID_SOCKET;
				break;
			}


			{
				CPUB_LockAction funcLock(&m_StartLock);

				if ((strcmp(netAutoReport.host, m_netAutoReport.host))
				  ||(netAutoReport.dwPort != m_netAutoReport.dwPort)
				  ||(netAutoReport.ID     != m_netAutoReport.ID)
				  ||(netAutoReport.bUse   != m_netAutoReport.bUse))
				{
					SWL_CloseSocket(sockSend);
					sockSend = SWL_INVALID_SOCKET;
					m_bConectOk = false;
					break;
				}

				CLIENT_SERVICE_INFO *pClientServiceInfo = new CLIENT_SERVICE_INFO;
				memset(pClientServiceInfo, 0, sizeof(CLIENT_SERVICE_INFO));

				pClientServiceInfo->clientID = m_pMessageMan->AllocNetClientID();
				//pClientServiceInfo->dwIP = netAutoReport.dwIP;
				pClientServiceInfo->wPort = netAutoReport.dwPort;
				pClientServiceInfo->pMultiNet = &m_multiNetComm;
				pClientServiceInfo->pMultiNet->AddNewComm(pClientServiceInfo->clientID, sockSend);
				pClientServiceInfo->pMultiNet->SetAutoRecvCallback(pClientServiceInfo->clientID, RecvDataCallback, this);
				pClientServiceInfo->pMultiNet->SetRecvMode(pClientServiceInfo->clientID, true);
				pClientServiceInfo->pAVDataSendMan = new CAVDataSendMan(pClientServiceInfo->clientID, &m_multiNetComm, \
					m_netInitInfo.dwMaxVideo, m_netInitInfo.dwMaxAudio);
				pClientServiceInfo->pStreamCHMap = new STREAM_CH_MAP;
				pClientServiceInfo->pLastStreamErrorList = new list<unsigned long>;
				pClientServiceInfo->lastStreamError = NETERR_STREAM_OK;
				pClientServiceInfo->loginMark = 0;
				//分配发送缓冲区
				pClientServiceInfo->sendBufferSize = SEND_BUFFER_SIZE;
				pClientServiceInfo->pSendBuffer = new unsigned char [SEND_BUFFER_SIZE];
				memset(pClientServiceInfo->pSendBuffer, 0, SEND_BUFFER_SIZE);

				//设定报警状态初始值为-1
				pClientServiceInfo->netRecStatus.chnn_count = m_netInitInfo.dwMaxVideo;
				pClientServiceInfo->netRecStatus.ullManualRec = INVALID_INDEX(ULONGLONG);
				pClientServiceInfo->netRecStatus.ullScheduleRec = INVALID_INDEX(ULONGLONG);
				pClientServiceInfo->netRecStatus.ullMotionRec = INVALID_INDEX(ULONGLONG);
				pClientServiceInfo->netRecStatus.ullSensorRec = INVALID_INDEX(ULONGLONG);
				pClientServiceInfo->ullMotionState = INVALID_INDEX(ULONGLONG);
				pClientServiceInfo->ullSensorInState = INVALID_INDEX(ULONGLONG);
				pClientServiceInfo->ullVideoLoss = INVALID_INDEX(ULONGLONG);
				pClientServiceInfo->ullNetLogin = INVALID_INDEX(ULONGLONG);
				pClientServiceInfo->connectType = CONNECTTYPE_ACTIVE_CONNECT;
				
				//把当前客户端加入到客户端列表
				m_lstClientServiceInfo.AddTail(pClientServiceInfo);

				printf("---------------------------new connection established\n");

				//发送预发送数据
				PRE_SEND_DATA preSendData;
				memset(&preSendData, 0, sizeof(PRE_SEND_DATA));
				preSendData.dwFlag = SWL_HEADFLAG;
				preSendData.dwDevType = m_netInitInfo.dwDeviceType;
				preSendData.dwProductType = m_netInitInfo.dwProductType;
				preSendData.dwProtocolVer = NET_PROTOCOL_VER;
				preSendData.dwConfigVer = NCFG_VERSION;
				preSendData.dwID = m_netAutoReport.ID;
				*(reinterpret_cast<unsigned short *>(&preSendData) + 31) = m_netInitInfo.dwProductSubID;

				pClientServiceInfo->pMultiNet->SendDataSync(pClientServiceInfo->clientID, &preSendData, sizeof(PRE_SEND_DATA), true);

				m_bConectOk = true;
			}			
			
		}while(0);
	
		PUB_Sleep(5000);
	}
#endif
	return 0;
}


PUB_THREAD_RESULT PUB_THREAD_CALL CNetServer::SendDataThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);

#if defined(__CHIP3535__)
	char szCommond[128] = {0};
	snprintf(szCommond, sizeof(szCommond), "taskset -p 0x2 %d", tid);
	printf("%s:%s:%d, The commond is %s\n", __FUNCTION__, __FILE__, __LINE__, szCommond);
	DVRSystem(szCommond);
	PUB_Sleep(100);
	printf("%s:%s:%d, The commond is %s\n", __FUNCTION__, __FILE__, __LINE__, szCommond);
	DVRSystem(szCommond);
#elif defined(__CHIP3531__)
	BindThreadToCpu(tid, 1);
#endif

#endif

	CNetServer *pThis = reinterpret_cast<CNetServer *>(pParam);
	pThis->SendDataRun();
	
	return 0;
}

int CNetServer::SendDataRun()
{
	bool bIdle = true;
	int iRet;
	CLIENT_SERVICE_INFO *pServiceInfo = NULL;
	CFrameData *pFrameData = NULL;

	//用来限制每次读取的视频帧数
	m_pDispatchFrameNum = new long [m_netInitInfo.dwMaxVideo];
	m_pFirstDispatchFrameTime = new LONGLONG [m_netInitInfo.dwMaxVideo];
	memset(m_pDispatchFrameNum, 0, sizeof(long) * m_netInitInfo.dwMaxVideo);
	memset(m_pFirstDispatchFrameTime, 0, sizeof(LONGLONG) * m_netInitInfo.dwMaxVideo);


	//录像及报警状态所用到的变量
	ULONGLONG ullMotionState, ullSensorInState, ullVideoLoss;
	NET_REC_STATUS netRecStatus;
	PACKCMD *pStatusCmd = (PACKCMD *)m_pStatusData;
	pStatusCmd->cmdID = 0xffffffff;
	pStatusCmd->dataLen = sizeof(NET_STATUS);

	PACKCMD *pRecStatusCmd = (PACKCMD *)m_pRecStatusData;
	pRecStatusCmd->cmdID = 0xffffffff;
	pRecStatusCmd->dataLen = sizeof(NET_REC_STATUS);
	pRecStatusCmd->cmdType = CMD_REPLY_REC_STATUS;

	int sendChnnNameNum = CProduct::Instance()->VideoInputNum();
	unsigned char* pSendChnnNameData = new unsigned char[sizeof(NET_CHNN_NAME)*sendChnnNameNum + sizeof(PACKCMD)];
	int	SendChnnNameDataLen = sizeof(NET_CHNN_NAME) * sendChnnNameNum + sizeof(PACKCMD);
	PACKCMD *pSendChnnNameCmd = (PACKCMD*)pSendChnnNameData; 
	pSendChnnNameCmd->cmdID = 0xffffffff;
	pSendChnnNameCmd->dataLen = sizeof(NET_CHNN_NAME)*sendChnnNameNum;
	pSendChnnNameCmd->cmdType = CMD_REPLY_CHNN_NAME;
	//用作数据分包发送
	unsigned char *pDataHead = NULL;

	//发送状态数据间隔
	unsigned long loopCount = 0;
	while(true)
	{
		{
			CPUB_LockAction funcLock(&m_StartLock);
			if(!m_bSendDataRun)
			{
				break;
			}

			loopCount++;

			//先把音频视频数据送打包程序中准备分包
			DispatchTalkStream();
			DispatchLiveStream(LIVE_FRAME_FIRST_STREAM, m_pBitmapInfo_F, m_pWaveFormatEx);
			DispatchLiveStream(LIVE_FRAME_SECOND_STREAM, m_pBitmapInfo_S, m_pWaveFormatEx);
			DispatchPBStream();
			DispatchJpegStream();

			if(m_lstClientServiceInfo.IsEmpty())
			{
				if(m_messageData.dataLen > 0)
				{
					delete [] m_messageData.pData;
					memset(&m_messageData, 0, sizeof(MESSAGE_DATA));
				}
			}

			bIdle = true;

			//先判断有没有待发送的数据，如果有，先尝试发送
			POS pos = m_lstClientServiceInfo.GetHeadPosition();
			while(NULL != pos)
			{
				pServiceInfo = m_lstClientServiceInfo.GetNext(pos);
				if(pServiceInfo->sendDataLen > 0)
				{
					iRet = pServiceInfo->pMultiNet->SendDataSync(pServiceInfo->clientID, pServiceInfo->pDataCusor, pServiceInfo->sendDataLen, false);
					if(iRet > 0)
					{
						pServiceInfo->sendDataLen -= iRet;
						pServiceInfo->pDataCusor += iRet;
					}

					if(0 == pServiceInfo->sendDataLen)
					{
						pServiceInfo->bBlocked = false;
					}
				}
			}

			//先发送状态信息
			if(0 == (loopCount % 20))
			{
				m_pMessageMan->GetCHStatus(CHNN_STATUS_MANUAL_REC, netRecStatus.ullManualRec);
				m_pMessageMan->GetCHStatus(CHNN_STATUS_SCHEDULE_REC, netRecStatus.ullScheduleRec);
				m_pMessageMan->GetCHStatus(CHNN_STATUS_MOTION_REC, netRecStatus.ullMotionRec);
				m_pMessageMan->GetCHStatus(CHNN_STATUS_SENSOR_REC, netRecStatus.ullSensorRec);

				m_pMessageMan->GetCHStatus(CHNN_STATUS_MOTION, ullMotionState);
				m_pMessageMan->GetCHStatus(CHNN_STATUS_SENSOR_IN, ullSensorInState);
				m_pMessageMan->GetCHStatus(CHNN_STATUS_VIDEO_LOSS, ullVideoLoss);

				POS pos = m_lstClientServiceInfo.GetHeadPosition();
				while(pos)
				{
					pServiceInfo = m_lstClientServiceInfo.GetNext(pos);

					if(!pServiceInfo->bValidateClient)
					{
						continue;
					}

					if(pServiceInfo->sendDataLen > 0)
					{
						continue;
					}

					//检查录像状态
					iRet = CheckSendRecStatusData(pServiceInfo->clientID, pServiceInfo->netRecStatus, netRecStatus);
					if(pServiceInfo->sendDataLen > 0)		//当前客户端连接阻塞，先发送其他客户端
					{
						continue;
					}

					//检查移动侦测状态
					pStatusCmd->cmdType = CMD_REPLY_MOTION;
					iRet = CheckSendStatusData(pServiceInfo->clientID, pServiceInfo->ullMotionState, ullMotionState);
					if(pServiceInfo->sendDataLen > 0)		//当前客户端连接阻塞，先发送其他客户端
					{
						continue;
					}

					//检查传感器报警状态
					pStatusCmd->cmdType = CMD_REPLY_SENSOR;
					iRet = CheckSendStatusData(pServiceInfo->clientID, pServiceInfo->ullSensorInState, ullSensorInState);
					if(pServiceInfo->sendDataLen > 0)		//当前客户端连接阻塞，先发送其他客户端
					{
						continue;
					}

					//检查视频丢失状态
					pStatusCmd->cmdType = CMD_REPLY_VIDEO_LOSS;
					iRet = CheckSendStatusData(pServiceInfo->clientID, pServiceInfo->ullVideoLoss, ullVideoLoss);
					if(pServiceInfo->sendDataLen > 0)		//当前客户端连接阻塞，先发送其他客户端
					{
						continue;
					}
					//检查是否通道名称改变
					if (CMessageMan::Instance()->CheckChangeCamName())
					{
						printf("%s, %d\n", __FILE__, __LINE__);
						NET_CHNN_NAME *pChnName =reinterpret_cast<NET_CHNN_NAME*>(pSendChnnNameData + sizeof(PACKCMD));
						memset(pChnName, 0, sizeof(NET_CHNN_NAME)*sendChnnNameNum);
						std::string name;
						for (int i = 0; i < sendChnnNameNum; i++)
						{
							CMessageMan::Instance()->GetCamName(i, name);
							pChnName[i].chnn = i;
							pChnName[i].nameLen = 36;
							snprintf(pChnName[i].name, 36, "%s", name.c_str());
						}
						SendData(pServiceInfo->clientID, pSendChnnNameData, SendChnnNameDataLen);
					}
					if(pServiceInfo->sendDataLen > 0)		//当前客户端连接阻塞，先发送其他客户端
					{
						continue;
					}
					if (pServiceInfo->ullNetLogin != INVALID_INDEX(ULONGLONG))
					{
						printf("%s, %d\n", __FILE__, __LINE__);
						NET_CHNN_NAME *pChnName =reinterpret_cast<NET_CHNN_NAME*>(pSendChnnNameData + sizeof(PACKCMD));
						memset(pChnName, 0, sizeof(NET_CHNN_NAME)*sendChnnNameNum);
						std::string name;
						for (int i = 0; i < sendChnnNameNum; i++)
						{
							CMessageMan::Instance()->GetCamName(i, name);
							pChnName[i].chnn = i;
							pChnName[i].nameLen = 36;
							snprintf(pChnName[i].name, 36, "%s", name.c_str());
						}
						SendData(pServiceInfo->clientID, pSendChnnNameData, SendChnnNameDataLen);
						pServiceInfo->ullNetLogin = INVALID_INDEX(ULONGLONG);
					}
					if(pServiceInfo->sendDataLen > 0)		//当前客户端连接阻塞，先发送其他客户端
					{
						continue;
					}
				}
				CMessageMan::Instance()->SetChangeCamName(false);
			}

			//再发送命令回复信息
			while(true)
			{
				//如果上一次有没有发送成功的命令，则先发送未成功的命令
				if(m_messageData.dataLen == 0)
				{
					if (!m_pMessageMan->GetMsgFromServer(m_messageData))
					{
						m_messageData.dataLen = 0;
						break;
					}
					else
					{
						POS pos = m_lstClientServiceInfo.GetHeadPosition();
						while(pos)
						{
							pServiceInfo = m_lstClientServiceInfo.GetAt( pos );

							if( pServiceInfo->clientID == m_messageData.clientID )
							{
								break;
							}
							m_lstClientServiceInfo.GetNext( pos );
						}

						//当前命令对应的客户端已经不存在，则过滤掉该回复命令，并且查找下一个回复命令
						if( NULL == pos )
						{
							delete [] m_messageData.pData;
							memset(&m_messageData, 0, sizeof(MESSAGE_DATA));
							continue;
						}

						if(!pServiceInfo->bValidateClient)	//当前客户端还没有被验证
						{
							//得到返回命令数据
							PACKCMD packCmd;
							memcpy(&packCmd, m_messageData.pData, sizeof(PACKCMD));
							if(packCmd.cmdType == CMD_REPLY_LONGIN_SUCC)
							{
								pServiceInfo->loginMark = 0;
								pServiceInfo->ullNetLogin = 0;
								NET_LOGIN_SUCCESS_REPLY loginReplyInfo;
								memcpy(&loginReplyInfo, m_messageData.pData + sizeof(PACKCMD), sizeof(NET_LOGIN_SUCCESS_REPLY));

								//远程预览权限
								if(loginReplyInfo.authority & AUTH_REMOTE_VIEW)
								{
									pServiceInfo->authRViewBits = loginReplyInfo.authRemoteViewCH;
								}
								else
								{
									pServiceInfo->authRViewBits = 0;
								}

								//远程回放权限
								if(loginReplyInfo.authority & AUTH_PLAYBACK)
								{
									pServiceInfo->authPlayback = loginReplyInfo.authPlaybackCH;
								}
								else
								{
									pServiceInfo->authPlayback = 0;
								}

								if(loginReplyInfo.authority & AUTH_BACKUP)
								{
									pServiceInfo->authBackup = loginReplyInfo.authBackupCH;
								}
								else
								{
									pServiceInfo->authBackup = 0;
								}

								pServiceInfo->bValidateClient = true;
							}
							else if(packCmd.cmdType == CMD_REPLY_LONGIN_FAIL)
							{
								pServiceInfo->loginMark = 0;
							}
							else	//还没有验证的客户端不可能收到除登录返回之外的其他回复
							{
//								assert(false);
								delete [] m_messageData.pData;
								memset(&m_messageData, 0, sizeof(MESSAGE_DATA));
								pServiceInfo->bBlocked = false;
								continue;
							}
						}

						//得到返回命令数据
						PACKCMD packCmd;
						memcpy(&packCmd, m_messageData.pData, sizeof(PACKCMD));
						if(packCmd.cmdType == CMD_REPLY_TALK_START_SUCC)
						{
							//开启对讲成功，在此处打开对讲发送流传输
							pServiceInfo->pAVDataSendMan->StartTalkStream();
							pServiceInfo->bTalking = true;
						}
					}//endif GetMsgFromServer(Success)
				}

				if(m_messageData.dataLen != 0)	//有命令数据待发送
				{
					POS pos = m_lstClientServiceInfo.GetHeadPosition();
					while(pos)
					{
						pServiceInfo = m_lstClientServiceInfo.GetAt( pos );

						if( pServiceInfo->clientID == m_messageData.clientID )
						{
							break;
						}
						m_lstClientServiceInfo.GetNext( pos );
					}

					//当前命令对应的客户端已经不存在，则过滤掉该回复命令，并且查找下一个回复命令
					if( NULL == pos )
					{
						delete [] m_messageData.pData;
						memset(&m_messageData, 0, sizeof(MESSAGE_DATA));
						bIdle = false;
						continue;
					}

					//有上次未发完的数据,可能是命令数据，也可能是音频视频数据
					if(pServiceInfo->sendDataLen > 0)
					{
						iRet = pServiceInfo->pMultiNet->SendDataSync(m_messageData.clientID, pServiceInfo->pDataCusor, pServiceInfo->sendDataLen, false);
						if(iRet > 0)
						{
							pServiceInfo->sendDataLen -= iRet;
							pServiceInfo->pDataCusor += iRet;
						}
					}

					//该客户端没有数据积压,发送新的数据
					if(pServiceInfo->sendDataLen == 0)
					{
						SendData(m_messageData.clientID, m_messageData.pData, m_messageData.dataLen);
						delete [] m_messageData.pData;
						memset(&m_messageData, 0, sizeof(MESSAGE_DATA));
					}

					//数据发送未完,阻塞网络
					if(pServiceInfo->sendDataLen > 0)
					{
						pServiceInfo->bBlocked = true;
						break;
					}
					else	//发送成功,继续发送下一个命令
					{
						pServiceInfo->bBlocked = false;
						bIdle = false;
						continue;
					}
				}
			}

			//发送心跳数据
			m_multiNetComm.SendHeartBeat();

#if defined(__TVT_NAT_SOCK__)
			m_multiNetNat.SendHeartBeat();
#endif
			//最后发送音频视频数据
			{
				POS pos = m_lstClientServiceInfo.GetHeadPosition();
				while(NULL != pos)
				{
					pServiceInfo = m_lstClientServiceInfo.GetNext(pos);

					if(!pServiceInfo->bValidateClient)
					{
						continue;
					}

					if(!pServiceInfo->bBlocked)
					{
						do 
						{
							if(pServiceInfo->sendDataLen > 0)
							{
								iRet = pServiceInfo->pMultiNet->SendDataSync(pServiceInfo->clientID, pServiceInfo->pDataCusor, pServiceInfo->sendDataLen, false);
								if(iRet > 0)
								{
									pServiceInfo->sendDataLen -= iRet;
									pServiceInfo->pDataCusor += iRet;

									if((pServiceInfo->sendDataLen > 0) && (iRet < 4096))
									{
										break;
									}
								}
								else
								{
									break;
								}
							}
							else
							{
								iRet = pServiceInfo->pAVDataSendMan->GetAVData(pServiceInfo->pSendBuffer+sizeof(COMM_PACK_HEAD), pServiceInfo->sendBufferSize, pDataHead);
								if(iRet <= 0)//没有数据可发送
								{
									break;
								}

								assert(pDataHead >= pServiceInfo->pSendBuffer);

								memcpy(((COMM_PACK_HEAD *)pDataHead-1)->headFlag, "1111", sizeof(char)*4);
								((COMM_PACK_HEAD *)pDataHead-1)->dataLen = iRet;

								pServiceInfo->sendDataLen = sizeof(COMM_PACK_HEAD) + iRet;
								pServiceInfo->pDataCusor = pDataHead-sizeof(COMM_PACK_HEAD);

								iRet = pServiceInfo->pMultiNet->SendDataSync(pServiceInfo->clientID, pServiceInfo->pDataCusor, pServiceInfo->sendDataLen, false);

								if(iRet > 0)
								{
									pServiceInfo->sendDataLen -= iRet;
									pServiceInfo->pDataCusor += iRet;
									if((pServiceInfo->sendDataLen > 0) && (iRet < 4096))
									{
										break;
									}
								}
								else	//发送数据失败
								{
									break;
								}
							}
						} while(true);
					}
				}
			}
		}

		if (bIdle && m_pMessageMan->IsFrameEmpty())
		{
			PUB_Sleep(2);
		}
	}

	delete [] m_pDispatchFrameNum;
	delete [] m_pFirstDispatchFrameTime;
	delete [] pSendChnnNameData;

	return 0;
}

/******************************************************************************
*
*接收数据回调函数
*返回值：如果公共存取数据缓冲区中有空间可以存储当前接收到的数据，则返回0，
		 否则返回-1表示下次重新接收这一组数据
*如果pBuf为NULL，说明客户端已断开
*
******************************************************************************/
int CNetServer::RecvDataCallback(long clientID, void* pParam, const char* pBuf, int iLen)
{
	CNetServer *pThis = reinterpret_cast<CNetServer *>(pParam);

	return pThis->RecvDataProc(clientID, pBuf, iLen);
}

int CNetServer::RecvDataProc(long clientID, const char* pBuf, int iLen)
{
	CPUB_LockAction funcLock(&m_StartLock);
	if(pBuf == NULL)
	{
		tm curTime = DVRTimeToTm(GetCurrTime());
		printf("对方socket已断开！%d-%d-%d %d:%d:%d\n", \
			curTime.tm_year, curTime.tm_mon, curTime.tm_mday, curTime.tm_hour, curTime.tm_min, curTime.tm_sec);

		CLIENT_SERVICE_INFO *pClientServiceInfo;
		POS pos = m_lstClientServiceInfo.GetHeadPosition();
		while(pos)
		{
			pClientServiceInfo = m_lstClientServiceInfo.GetAt(pos);
			if(pClientServiceInfo->clientID == clientID)
			{
				if (CONNECTTYPE_ACTIVE_CONNECT == pClientServiceInfo->connectType)
				{
					m_bConectOk = false;
				}
					
				m_lstClientServiceInfo.RemoveAt(pos);
				break;
			}
			m_lstClientServiceInfo.GetNext(pos);
		}

		if(pos == NULL)
		{
			assert(false);
			return 0;
		}

		if(!pClientServiceInfo->bValidateClient)
		{
			if(pClientServiceInfo->loginMark == 0x10)
			{
				printf("Remove the client who is waiting for login reply...\n");
				assert(false);
			}
		}

		RemoveClient(pClientServiceInfo);	
		
		
	
			
	}
	else
	{
		MESSAGE_DATA messageData;
		messageData.deviceID= 0;
		messageData.clientID = clientID;

		//解析命令，对一些命令在此作一些预处理
		PACKCMD *pPackCmd =(PACKCMD *)pBuf;

		CLIENT_SERVICE_INFO *pClientServiceInfo = NULL;
		POS pos = m_lstClientServiceInfo.GetHeadPosition();
		while(NULL != pos)
		{
			pClientServiceInfo = m_lstClientServiceInfo.GetAt(pos);
			if(pClientServiceInfo->clientID == clientID)
			{
				break;
			}
			m_lstClientServiceInfo.GetNext(pos);
		}

		if(NULL == pos)
		{
			assert(false);
			pClientServiceInfo = NULL;
		}

		if(pPackCmd->cmdType == CMD_REQUEST_LOGIN)
		{
			NET_LOGIN_INFO *pNetLoginInfo = (NET_LOGIN_INFO *)(pBuf+sizeof(PACKCMD));
			pNetLoginInfo->IP = pClientServiceInfo->dwIP;

			pClientServiceInfo->loginMark = 0x10;
			
			if (CONNECTTYPE_ACTIVE_CONNECT == pClientServiceInfo->connectType)
			{
				pNetLoginInfo->connectType = CONNECTTYPE_ACTIVE_CONNECT;
			}
			else
			{
				pClientServiceInfo->connectType = pNetLoginInfo->connectType;
			}
			

			if (CONNECTTYPE_CTRL_CENTER == pNetLoginInfo->connectType)
			{
				if (TD_TVT_BR27 == m_netInitInfo.dwProductSubID)
				{
					//巴西公司的DVR只能用巴西的CMS连接。
					//并且巴西的CMS只能连接巴西的DVR
					if (TD_TVT_BR27 != *reinterpret_cast<unsigned short *>(pNetLoginInfo->resv))
					{
						return 0;
					}
				}
				else
				{
					//其他非巴西公司的DVR只能用中性CMS连接
					//并且中性CMS只能连接其他非巴西公司的DVR
					if (0 != *reinterpret_cast<unsigned short *>(pNetLoginInfo->resv))
					{
						return 0;
					}
				}
			}
			else if(CONNECTTYPE_VOD == pNetLoginInfo->connectType)
			{
				messageData.deviceID= 0;
				messageData.clientID = clientID;
				messageData.dataLen = sizeof(PACKCMD) + sizeof(NET_LOGIN_SUCCESS_REPLY);
				messageData.pData = new char[messageData.dataLen];
				memset(messageData.pData, 0, messageData.dataLen);

				PACKCMD *pLoginPackCmd = (PACKCMD *)messageData.pData;
				NET_LOGIN_SUCCESS_REPLY *pLoginReply = (NET_LOGIN_SUCCESS_REPLY *)(messageData.pData + sizeof(PACKCMD));

				pLoginPackCmd->cmdType = CMD_REPLY_LONGIN_SUCC;
				pLoginPackCmd->cmdVer = NET_PROTOCOL_VER;
				pLoginPackCmd->dataLen = sizeof(NET_LOGIN_SUCCESS_REPLY);

				pLoginReply->authority = AUTH_REMOTE_VIEW;
				pLoginReply->authRemoteViewCH = ~((ULONGLONG)0);

				m_pMessageMan->AddClientID(clientID);

				m_pMessageMan->PutMsgToClient(messageData);

				return 0;
			}
		}
		else
		{
			if(!pClientServiceInfo->bValidateClient)	//当前客户端还没有被验证
			{
//				assert(false);
				return 0;
			}
		}

		if(pPackCmd->cmdType == CMD_REQUEST_LAST_STREAM_ERROR)
		{
			pClientServiceInfo->pLastStreamErrorList->push_back(pClientServiceInfo->lastStreamError);
		}

		if((pPackCmd->cmdType == CMD_REQUEST_STREAM_START) || \
			(pPackCmd->cmdType == CMD_REQUEST_STREAM_CHANGE) || \
			(pPackCmd->cmdType == CMD_REQUEST_STREAM_STOP))
		{
			pClientServiceInfo->lastStreamError = NETERR_STREAM_OK;

			unsigned char localVideoNum = CProduct::Instance()->LocalVideoInputNum();

			assert(iLen == (sizeof(PACKCMD) + sizeof(NET_LIVESTREAM_CTRL_INFO)));
			
			NET_LIVESTREAM_CTRL_INFO *pLiveStreamReq = (NET_LIVESTREAM_CTRL_INFO *)(pBuf+sizeof(PACKCMD));

			bool bSupReqFirstSecondStream = false;
			if((CONNECTTYPE_ECMS == pClientServiceInfo->connectType) || (CONNECTTYPE_ACTIVE_CONNECT == pClientServiceInfo->connectType))
			{
				bSupReqFirstSecondStream = true;
			}

			//3G模式下只支持字子码流
			if (g_curNet3G)
			{
				m_bSupportDoubleStream = false;
			}
			else
			{
				m_bSupportDoubleStream = CProduct::Instance()->SupportDoubleStream();
			}

			if ((!m_bSupportDoubleStream) && (pLiveStreamReq->fastVideoCH != 0))
			{
				if (bSupReqFirstSecondStream)
				{
					pLiveStreamReq->fastVideoCH = 0;
					pClientServiceInfo->lastStreamError = NETERR_STREAM_NO_CAPABILITY;
				}
				else
				{
					printf("%s,%d,no support double stream\n", __FILE__, __LINE__);	
					pLiveStreamReq->secondVideoCH |= pLiveStreamReq->fastVideoCH;
					pLiveStreamReq->fastVideoCH = 0;
				}
			}
			//远程预览权限过滤，如果所有通道都没有权限，则直接返回
			pLiveStreamReq->fastVideoCH &= pClientServiceInfo->authRViewBits;
			pLiveStreamReq->secondVideoCH &= pClientServiceInfo->authRViewBits;
			pLiveStreamReq->audioCH &= pClientServiceInfo->authRViewBits;

			if (!bSupReqFirstSecondStream)
			{
				if(!(pLiveStreamReq->fastVideoCH | pLiveStreamReq->secondVideoCH))
				{
					if(pPackCmd->cmdType == CMD_REQUEST_STREAM_START)
					{
						return 0;
					}
					else if(pPackCmd->cmdType == CMD_REQUEST_STREAM_CHANGE)
					{
						pPackCmd->cmdType = CMD_REQUEST_STREAM_STOP;
					}
				}
			}


			if (!m_bLimitFirstStreamSendNum)
			{
				//只对每个通道的主码流个数进行限制
				ULONGLONG ullForbidChannelMaster = 0;

				for(long channel = 0; channel < m_netInitInfo.dwMaxVideo; channel++)
				{
					if((0x01 << channel) & pLiveStreamReq->fastVideoCH)
					{
						if(!((0x01 << channel) & pClientServiceInfo->ullMasterVideo))
						{
							if(m_pFastVideoNum[channel] >= m_maxSupportFastVideo)
							{
								ullForbidChannelMaster |= (0x01 << channel);
							}
							else if(channel >= localVideoNum)
							{
								m_pFastVideoNum[channel]++;
								pClientServiceInfo->ullMasterVideo |= (0x01 << channel);
								m_curIPCTransferChnnNum++;
							}
							else
							{
								m_pFastVideoNum[channel]++;
								pClientServiceInfo->ullMasterVideo |= (0x01 << channel);
							}
						}
					}
					else //这次不要开启这个通道的主码流
					{
						if((0x01 << channel) & pClientServiceInfo->ullMasterVideo)
						{
							assert(m_pFastVideoNum[channel] > 0);
							if(m_pFastVideoNum[channel] > 0)
							{
								m_pFastVideoNum[channel]--;
							}
							pClientServiceInfo->ullMasterVideo &= ~(0x01 << channel);

							if(channel >= localVideoNum)
							{
								if(m_curIPCTransferChnnNum > 0)
								{
									m_curIPCTransferChnnNum--;
								}
							}
						}
					}
				}

				if(ullForbidChannelMaster != 0)
				{
					pLiveStreamReq->fastVideoCH &= ~ullForbidChannelMaster;
					if (!bSupReqFirstSecondStream)
					{
						pLiveStreamReq->secondVideoCH |= ullForbidChannelMaster;
					}
					pClientServiceInfo->lastStreamError = NETERR_STREAM_NO_CAPABILITY;
				}
				else
				{
					pClientServiceInfo->lastStreamError = NETERR_STREAM_OK;
				}
			}
			else
			{
				//对每个通道的主码流个数和整个DVR发送主码流个数做限制。
				ULONGLONG ullForbidChannelMaster = 0;
				for(long channel = 0; channel < m_netInitInfo.dwMaxVideo; channel++)
				{
					if(!((0x01 << channel) & pLiveStreamReq->fastVideoCH))
					{
						if(((0x01 << channel) & pClientServiceInfo->ullMasterVideo))
						{
							assert(m_pFastVideoNum[channel] > 0);
							if(m_pFastVideoNum[channel] > 0)
							{
								m_pFastVideoNum[channel]--;
							}
							if (m_curFirstStreamSendNum > 0)
							{
								m_curFirstStreamSendNum--;
							}
							pClientServiceInfo->ullMasterVideo &= ~(0x01 << channel);

							if(channel >= localVideoNum)
							{
								if(m_curIPCTransferChnnNum > 0)
								{
									m_curIPCTransferChnnNum--;
								}
							}
						}
					}
				}

				//如果当前有请求主码流，先清除主码流占的资源
				for(long channel = 0; channel < m_netInitInfo.dwMaxVideo; channel++)
				{
					if((0x01 << channel) & pClientServiceInfo->ullMasterVideo)
					{
						if(channel >= localVideoNum)
						{
							if(m_pFastVideoNum[channel] > 0)
							{
								m_pFastVideoNum[channel]--;
							}

							if(m_curFirstStreamSendNum > 0)
							{
								m_curFirstStreamSendNum--;
							}

							if(m_curIPCTransferChnnNum > 0)
							{
								m_curIPCTransferChnnNum--;
							}

							pClientServiceInfo->ullMasterVideo &= ~(0x01 << channel);
						}
					}
				}

				for(long channel = 0; channel < m_netInitInfo.dwMaxVideo; channel++)
				{
					if((0x01 << channel) & pLiveStreamReq->fastVideoCH)
					{
						if(!((0x01 << channel) & pClientServiceInfo->ullMasterVideo))
						{
							if (m_pFastVideoNum[channel] >= m_maxSupportFastVideo)
							{
								ullForbidChannelMaster |= (0x01 << channel);
							}
							else if (m_curFirstStreamSendNum >= m_maxFirstStreamSendNum)
							{
								ullForbidChannelMaster |= (0x01 << channel);
							}
							else if(channel >= localVideoNum)
							{
								m_pFastVideoNum[channel]++;
								pClientServiceInfo->ullMasterVideo |= (0x01 << channel);
								m_curFirstStreamSendNum++;
								m_curIPCTransferChnnNum++;
							}
							else
							{
								m_pFastVideoNum[channel]++;
								pClientServiceInfo->ullMasterVideo |= (0x01 << channel);
								m_curFirstStreamSendNum++;
							}
						}
					}
					else
					{
						if((0x01 << channel) & pClientServiceInfo->ullMasterVideo)
						{
							assert(m_pFastVideoNum[channel] > 0);
							if(m_pFastVideoNum[channel] > 0)
							{
								m_pFastVideoNum[channel]--;
							}
							if (m_curFirstStreamSendNum > 0)
							{
								m_curFirstStreamSendNum--;
							}
							if(channel >= localVideoNum)
							{
								if(m_curIPCTransferChnnNum > 0)
								{
									m_curIPCTransferChnnNum--;
								}
							}
							pClientServiceInfo->ullMasterVideo &= ~(0x01 << channel);
						}
					}
				}

				if(ullForbidChannelMaster != 0)
				{
					pLiveStreamReq->fastVideoCH &= ~ullForbidChannelMaster;
					if (!bSupReqFirstSecondStream)
					{
						pLiveStreamReq->secondVideoCH |= ullForbidChannelMaster;
					}
					pClientServiceInfo->lastStreamError = NETERR_STREAM_NO_CAPABILITY;
				}
				else
				{
					pClientServiceInfo->lastStreamError = NETERR_STREAM_OK;
				}

			}

			if (bSupReqFirstSecondStream)
			{
				if(!(pLiveStreamReq->fastVideoCH | pLiveStreamReq->secondVideoCH))
				{
					if(pPackCmd->cmdType == CMD_REQUEST_STREAM_START)
					{
						return 0;
					}
					else if(pPackCmd->cmdType == CMD_REQUEST_STREAM_CHANGE)
					{
						pPackCmd->cmdType = CMD_REQUEST_STREAM_STOP;
					}
				}
			}			

			pClientServiceInfo->ullSecondVideo = pLiveStreamReq->secondVideoCH;
			pClientServiceInfo->ullAudio = pLiveStreamReq->audioCH;
			if(pPackCmd->cmdType == CMD_REQUEST_STREAM_START)
			{
				if((pLiveStreamReq->fastVideoCH | pLiveStreamReq->secondVideoCH) == 0)
				{
					return 0;
				}
				pClientServiceInfo->pAVDataSendMan->StartStream(true, pLiveStreamReq->fastVideoCH, \
					pLiveStreamReq->secondVideoCH, pLiveStreamReq->audioCH, pLiveStreamReq->streamID);
				pClientServiceInfo->dwLiveStreamID = pLiveStreamReq->streamID;
			}
			else if(pPackCmd->cmdType == CMD_REQUEST_STREAM_CHANGE)
			{
				if((pLiveStreamReq->fastVideoCH | pLiveStreamReq->secondVideoCH) == 0)
				{
					pPackCmd->cmdType = CMD_REQUEST_STREAM_STOP;
				}
				pClientServiceInfo->pAVDataSendMan->ChangeStream(pLiveStreamReq->fastVideoCH, pLiveStreamReq->secondVideoCH, \
					pLiveStreamReq->audioCH, pLiveStreamReq->streamID);
			}
			else if(pPackCmd->cmdType == CMD_REQUEST_STREAM_STOP)
			{
				pClientServiceInfo->pAVDataSendMan->StopStream(pLiveStreamReq->streamID);
			}
			
		}
		else if(pPackCmd->cmdType == CMD_REQUEST_JPEG_START)
		{
			NET_JPEG_STREAM_INFO *pJpegInfo = (NET_JPEG_STREAM_INFO *)(pBuf+sizeof(PACKCMD));
			if(pJpegInfo->jpegCH == 0)
			{
				return 0;
			}
			pClientServiceInfo->pAVDataSendMan->StartJpegStream(pJpegInfo->jpegCH, pJpegInfo->streamID);
			pClientServiceInfo->ullJpegStream = pJpegInfo->jpegCH;
		}
		else if(pPackCmd->cmdType == CMD_REQUEST_JPEG_CHANGE)
		{
			NET_JPEG_STREAM_INFO *pJpegInfo = (NET_JPEG_STREAM_INFO *)(pBuf+sizeof(PACKCMD));
			if(pJpegInfo->jpegCH == 0)
			{
				return 0;
			}
			pClientServiceInfo->pAVDataSendMan->ChangeJpegStream(pJpegInfo->jpegCH, pJpegInfo->streamID);
			pClientServiceInfo->ullJpegStream = pJpegInfo->jpegCH;
		}
		else if(pPackCmd->cmdType == CMD_REQUEST_JPEG_STOP)
		{
			NET_JPEG_STREAM_INFO *pJpegInfo = (NET_JPEG_STREAM_INFO *)(pBuf+sizeof(PACKCMD));
			pClientServiceInfo->pAVDataSendMan->StopJpegStream(pJpegInfo->streamID);
			pClientServiceInfo->ullJpegStream = 0;
		}
		else if(pPackCmd->cmdType == CMD_REQUEST_DATA_READ)
		{			
			assert(iLen == (sizeof(PACKCMD) + sizeof(NET_DATA_SEARCH)));

			NET_DATA_SEARCH *pNetDataSearch = (NET_DATA_SEARCH *)(pBuf + sizeof(PACKCMD));

			{
				STREAM_CH_MAP::iterator it = pClientServiceInfo->pStreamCHMap->find(pNetDataSearch->streamID);

				if(it != pClientServiceInfo->pStreamCHMap->end())
				{
					printf("###%s,%d,no stop\n",__FILE__,__LINE__);
					return 0;
				}
			}
			

			{
				int maxPlayBackStreamIDNum = CProduct::Instance()->GetDVRMaxStreamIDNum();
				int playBackStreamIDNum = 0;
				int curNetPlayBackNum = 0;

				POS pos = m_lstClientServiceInfo.GetHeadPosition();
				while( NULL != pos )
				{					
					CLIENT_SERVICE_INFO * pClientInfo = m_lstClientServiceInfo.GetAt( pos );

					STREAM_CH_MAP::iterator it;
					for(it = pClientInfo->pStreamCHMap->begin(); it != pClientInfo->pStreamCHMap->end(); it++)
					{
						if ((*it).second > 0)
						{
							playBackStreamIDNum++;
							
							for(int channel = 0; channel < m_netInitInfo.dwMaxVideo; channel++)
							{
								if((0x01 << channel) & (*it).second)
								{
									curNetPlayBackNum++;
								}
							}
						}
					}

					m_lstClientServiceInfo.GetNext(pos);
				}

				for(int channel = 0; channel < m_netInitInfo.dwMaxVideo; channel++)
				{
					if((0x01 << channel) & pNetDataSearch->videoCH)
					{
						curNetPlayBackNum++;
					}
				}

				//printf("%s,%d,playBackStreamIDNum:%d,maxPlayBackStreamIDNum:%d\n",__FILE__,__LINE__,playBackStreamIDNum,maxPlayBackStreamIDNum);
				if ((playBackStreamIDNum >= maxPlayBackStreamIDNum) || (curNetPlayBackNum > CProduct::Instance()->NetVideoOutPutNum()))
				{
					pClientServiceInfo->lastStreamError = NETERR_STREAM_NO_CAPABILITY;
					return 0;
				}
			}



			//远程回放权限过滤，如果所有通道都没有权限，则直接返回
			if(NET_DATA_SEARCH_PLAYBACK == pNetDataSearch->dataType)
			{
				pNetDataSearch->videoCH &= pClientServiceInfo->authPlayback;
				pNetDataSearch->audioCH &= pClientServiceInfo->authPlayback;
			}
			else if(NET_DATA_SEARCH_BACKUP == pNetDataSearch->dataType)
			{
				pNetDataSearch->videoCH &= pClientServiceInfo->authBackup;
				pNetDataSearch->audioCH &= pClientServiceInfo->authBackup;
			}
			else
			{
				assert(false);
			}

			if (!m_bLimitFirstStreamSendNum)
			{
				//只对每个通道的主码流个数进行限制
				ULONGLONG ullForbidChannel = 0;
				for(long channel = 0; channel < m_netInitInfo.dwMaxVideo; channel++)
				{
					if((0x01 << channel) & pNetDataSearch->videoCH)
					{
						if(m_pFastVideoNum[channel] >= m_maxSupportFastVideo)
						{
							ullForbidChannel |= (0x01 << channel);
						}
						else
						{
							m_pFastVideoNum[channel]++; 
						}
					}
				}

				if(ullForbidChannel != 0)
				{
					pNetDataSearch->videoCH &= ~ullForbidChannel;
					pNetDataSearch->audioCH &= ~ullForbidChannel;
					pClientServiceInfo->lastStreamError = NETERR_STREAM_NO_CAPABILITY;
				}
				else
				{
					pClientServiceInfo->lastStreamError = NETERR_STREAM_OK;
				}
			}
			else
			{
				//对每个通道的主码流个数和整个DVR发送主码流个数做限制。
				ULONGLONG ullForbidChannel = 0;
				for(long channel = 0; channel < m_netInitInfo.dwMaxVideo; channel++)
				{
					if((0x01 << channel) & pNetDataSearch->videoCH)
					{
						if(m_pFastVideoNum[channel] >= m_maxSupportFastVideo)
						{
							ullForbidChannel |= (0x01 << channel);
						}
						else if (m_curFirstStreamSendNum >= m_maxFirstStreamSendNum)
						{
							ullForbidChannel |= (0x01 << channel);
						}
						else
						{
							m_pFastVideoNum[channel]++; 
							m_curFirstStreamSendNum++;
						}
					}
				}

				if(ullForbidChannel != 0)
				{
					pNetDataSearch->videoCH &= ~ullForbidChannel;
					pNetDataSearch->audioCH &= ~ullForbidChannel;
					pClientServiceInfo->lastStreamError = NETERR_STREAM_NO_CAPABILITY;
				}
				else
				{
					pClientServiceInfo->lastStreamError = NETERR_STREAM_OK;
				}
			}

			STREAM_CH_PAIR streamChPair;
			streamChPair.first = pNetDataSearch->streamID;
			streamChPair.second = pNetDataSearch->videoCH;
			pClientServiceInfo->pStreamCHMap->insert(streamChPair);

			if(0 == pNetDataSearch->videoCH)
			{
				return 0;
			}

			pClientServiceInfo->pAVDataSendMan->StartStream(false, pNetDataSearch->videoCH, 0, \
				pNetDataSearch->audioCH, pNetDataSearch->streamID);
		}
		else if(pPackCmd->cmdType == CMD_REQUEST_DATA_STOP)
		{
			assert(iLen == (sizeof(PACKCMD) + sizeof(NET_DATA_SEARCH)));

			NET_DATA_SEARCH netDataSearch;
			memcpy(&netDataSearch, pBuf + sizeof(PACKCMD), sizeof(NET_DATA_SEARCH));

			STREAM_CH_MAP::iterator it = pClientServiceInfo->pStreamCHMap->find(netDataSearch.streamID);

			if(it != pClientServiceInfo->pStreamCHMap->end())
			{
				for(long channel = 0; channel < m_netInitInfo.dwMaxVideo; channel++)
				{
					if((0x01 << channel) & (*it).second)
					{
						assert(m_pFastVideoNum[channel] > 0);
						if(m_pFastVideoNum[channel] > 0)
						{
							m_pFastVideoNum[channel]--;
						}

						if (m_curFirstStreamSendNum > 0)
						{
							m_curFirstStreamSendNum--;
						}

						if(channel >= CProduct::Instance()->LocalVideoInputNum())
						{
							if(m_curIPCTransferChnnNum > 0)
							{
								m_curIPCTransferChnnNum--;
							}
						}
					}
				}

				pClientServiceInfo->pStreamCHMap->erase(it);
				pClientServiceInfo->pAVDataSendMan->StopStream(netDataSearch.streamID);
			}
		}
		else if(pPackCmd->cmdType == CMD_REQUEST_TALK_START)
		{
			assert(!pClientServiceInfo->bTalking);
			if(pClientServiceInfo->bTalking)
			{
				return 0;
			}
		}
		else if(pPackCmd->cmdType == CMD_REQUEST_TALK_STOP)
		{
			assert(pClientServiceInfo->bTalking);
			if(!pClientServiceInfo->bTalking)
			{
				return 0;
			}

			pClientServiceInfo->pAVDataSendMan->StopTalkStream();
			pClientServiceInfo->bTalking = false;
		}
		else if(pPackCmd->cmdType == CMD_REQUEST_TALK_STREAM)
		{
			//如果没开对讲，直接过滤发过来的对讲数据
			if(!pClientServiceInfo->bTalking)
			{
				return 0;
			}

			//对讲数据直接在这里加入到MESSAGEMAN中
			FRAME_INFO_EX frameInfo;
			memcpy(&frameInfo, pBuf+sizeof(PACKCMD), sizeof(FRAME_INFO_EX));
			
			//如果不是对讲数据，直接过滤，不接受对讲音频格式信息
			assert(frameInfo.frameType == FRAME_TYPE_TALK_AUDIO);
			if(frameInfo.frameType != FRAME_TYPE_TALK_AUDIO)
			{
				return 0;
			}

			frameInfo.pData = new unsigned char[frameInfo.length];
			memcpy(frameInfo.pData, pBuf+sizeof(PACKCMD)+sizeof(FRAME_INFO_EX), frameInfo.length);
			m_pMessageMan->PutTalkWayFrame(frameInfo);

			return 0;
		}
		else
		{
			//不处理
		}

		//把命令加入到数据共享管理器中
		messageData.pData = new char[iLen];
		memcpy(messageData.pData, pBuf, iLen);
		messageData.dataLen = iLen;
		m_pMessageMan->PutMsgToServer(messageData);
	}

	return 0;
}

/******************************************************************************
*
*发送状态信息
*返回值：发送成功返回0
		 发送失败返回-1
		 没有状态发送，返回-2
*
******************************************************************************/
int CNetServer::CheckSendStatusData(long clientID, ULONGLONG &ullOldStatus, ULONGLONG ullNewStatus)
{
//	int retval;

	assert( NULL != m_pStatusData );
	assert( m_statusDataLen == sizeof( PACKCMD ) + sizeof( NET_STATUS ) );


	//如果状态改变就发送新状态到客户端
	if( ullNewStatus != ullOldStatus )
	{
		NET_STATUS *pNetStatus = reinterpret_cast<NET_STATUS *>(m_pStatusData + sizeof(PACKCMD) );
		pNetStatus->chnn_count = m_netInitInfo.dwMaxVideo;
		memcpy( &(pNetStatus->ullstatus), &ullNewStatus, sizeof( LONGLONG ) );

		SendData(clientID, m_pStatusData, m_statusDataLen);

		//发送成功，更新客户端状态信息
		memcpy( &ullOldStatus, &ullNewStatus, sizeof( LONGLONG ) );

		return 0;
	}

	return -2;
}

int CNetServer::CheckSendRecStatusData(long clientID, NET_REC_STATUS &oldRecStatus, NET_REC_STATUS &newRecStatus)
{
//	int retval;

	assert( NULL != m_pRecStatusData );
	assert( m_recStatusDataLen == sizeof( PACKCMD ) + sizeof( NET_REC_STATUS ) );
	newRecStatus.chnn_count = oldRecStatus.chnn_count;

	//如果状态改变就发送新状态到客户端
	if( memcmp(&oldRecStatus, &newRecStatus, sizeof(NET_REC_STATUS)) != 0)
	{
		NET_REC_STATUS *pNetRecStatus = reinterpret_cast<NET_REC_STATUS *>(m_pRecStatusData + sizeof(PACKCMD) );
		memcpy(pNetRecStatus, &newRecStatus, sizeof(NET_REC_STATUS));

		SendData(clientID, m_pRecStatusData, m_recStatusDataLen);

		//发送成功，更新客户端状态信息
		memcpy( &oldRecStatus, &newRecStatus, sizeof( NET_REC_STATUS ) );

		return 0;
	}

	return -2;
}


int CNetServer::RewriteServerJS( unsigned short newStreamPort )
{
	int retVal = -1;
	int count = 0;

	FILE *pFd = NULL;
	errno = 0;
	
	while( NULL == ( pFd = fopen( SERVERJS, "rb+") ) )
	{
		if( EINTR == errno )
		{
			continue;
		}
		else
		{
			count++;
			if( 10 > count )
			{
				assert(0);
				exit(1);
			}
		}
	}

	fseek( pFd, 0, SEEK_END );
	off_t fileLen = ftell( pFd );

	char *pFileContent = new char [fileLen*2];
	char *pNetPortName = new char [fileLen];
	char *pFileContentTemp = new char [fileLen];

	assert( NULL != pFileContent );
	assert( NULL != pNetPortName );
	assert( NULL != pFileContentTemp );

	memset( pFileContent, 0, fileLen*2 );
	memset( pNetPortName, 0, fileLen );
	memset( pFileContentTemp, 0, fileLen );

	rewind( pFd );

	if( 1 != fread( pFileContent, fileLen, 1, pFd ) )
	{
		printf("%s:%s:%d, can not get the file content\n", __FILE__, __FUNCTION__, __LINE__ );

		assert(0);
	}
	else
	{
		char *pIndex = strstr( pFileContent, "=" );
		if( NULL != pIndex )
		{
			memcpy( pNetPortName, pFileContent, pIndex - pFileContent );
			
			//liuhao 2010-02-03 除了端口信息外，还有设备类型信息
			pIndex = strstr(pFileContent, "\n");
			if( NULL != pIndex )
			{
				strcpy(pFileContentTemp, pIndex+1);
			}

			sprintf( pFileContent, "%s=%d\n%s", pNetPortName, newStreamPort, pFileContentTemp );

			fclose( pFd );
			pFd = NULL;
			pFd = fopen( SERVERJS, "wb+");
			assert( NULL != pFd );
			
			if( 1 != fwrite( pFileContent, strlen(pFileContent), 1, pFd ) )
			{
				printf("%s:%s:%d, the server changed\n", __FUNCTION__, __FILE__, __LINE__);
				
				assert(0);
			}

			retVal = 0;
		}

	}

	if( NULL != pFd )
	{
		fclose( pFd );
		pFd = NULL;
	}

	delete [] pFileContent;
	pFileContent = NULL;

	delete [] pNetPortName;
	pNetPortName = NULL;

	delete [] pFileContentTemp;
	pFileContentTemp = NULL;

	return retVal;

}

void CNetServer::SendData(long clientID, void *pDataBuf, int dataLen)
{
	POS pos = m_lstClientServiceInfo.GetHeadPosition();
	while(NULL != pos)
	{
		CLIENT_SERVICE_INFO *& pClientServiceInfo = m_lstClientServiceInfo.GetNext(pos);
		if(pClientServiceInfo->clientID == clientID)
		{
			assert(0 == pClientServiceInfo->sendDataLen);

			//如果待发送数据大于发送缓冲区，需增加缓冲区大小
			if((dataLen+sizeof(COMM_PACK_HEAD)) > pClientServiceInfo->sendBufferSize)
			{
				unsigned char *pData = pClientServiceInfo->pSendBuffer;
				pClientServiceInfo->sendBufferSize = dataLen+sizeof(COMM_PACK_HEAD);
				pClientServiceInfo->pSendBuffer = new unsigned char [dataLen+sizeof(COMM_PACK_HEAD)];
				delete [] pData;
			}

			COMM_PACK_HEAD *pCommHead = (COMM_PACK_HEAD *)pClientServiceInfo->pSendBuffer;
			memcpy(pCommHead->headFlag, "1111", sizeof(pCommHead->headFlag));
			pCommHead->dataLen = dataLen;
			memcpy(pClientServiceInfo->pSendBuffer+sizeof(COMM_PACK_HEAD), pDataBuf, dataLen);

			pClientServiceInfo->sendDataLen = sizeof(COMM_PACK_HEAD) + dataLen;
			pClientServiceInfo->pDataCusor = pClientServiceInfo->pSendBuffer;

			int sentLen = pClientServiceInfo->pMultiNet->SendDataSync(clientID, pClientServiceInfo->pDataCusor, pClientServiceInfo->sendDataLen, false);
			if(sentLen > 0)
			{
				pClientServiceInfo->sendDataLen -= sentLen;
				pClientServiceInfo->pDataCusor += sentLen;
			}

			break;
		}
	}
}

void CNetServer::DispatchTalkStream()
{
	CLIENT_SERVICE_INFO *pServiceInfo;
	CFrameData *pFrameData;
	long ret = 0;

	while(m_pMessageMan->GetFrame(&pFrameData, LIVE_FRAME_TALK))
	{
		FRAME_INFO_EX &frameInfo = pFrameData->GetFrameInfo();

		assert(pFrameData->GetDataId() == INVALID_INDEX(unsigned long));
		if(pFrameData->GetDataId() != INVALID_INDEX(unsigned long))
		{
			assert(false);
			pFrameData->Dec();
			continue;
		}

		if(FRAME_TYPE_TALK_AUDIO_FORMAT == frameInfo.frameType)
		{
			memcpy(&m_talkAudioFormat, frameInfo.pData, frameInfo.length);
		}

		POS pos = m_lstClientServiceInfo.GetHeadPosition();
		while(NULL != pos)
		{
			pServiceInfo = m_lstClientServiceInfo.GetNext(pos);

			if((frameInfo.frameType == FRAME_TYPE_TALK_AUDIO) || (frameInfo.frameType == FRAME_TYPE_TALK_AUDIO_FORMAT))
			{
				if(SEND_FRAME_NEED_FORMAT == pServiceInfo->pAVDataSendMan->PutTalkData(pFrameData))
				{
					if(m_talkAudioFormat.nAvgBytesPerSec != 0)
					{
						//发送数据格式
						FRAME_INFO_EX formatFrame = frameInfo;
						formatFrame.frameType = FRAME_TYPE_TALK_AUDIO_FORMAT;
						formatFrame.length = sizeof(WAVEFORMATEX);
						formatFrame.pData = new unsigned char[formatFrame.length];
						memcpy(formatFrame.pData, &m_talkAudioFormat, sizeof(WAVEFORMATEX));

						CFrameData *pFormatFrame = new CFrameData(formatFrame, 0, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
						//pFormatFrame->SetProtocol(CMD_REPLY_DATA_STREAM);
						//pFormatFrame->SetDataId(INVALID_INDEX(unsigned long));
						pServiceInfo->pAVDataSendMan->PutTalkData(pFormatFrame);

						//发送格式后，重新把数据放入分包模块中
						pServiceInfo->pAVDataSendMan->PutTalkData(pFrameData);
					}
					else
					{
						assert(false);
					}
				}
			}
			else
			{
				assert(false);
			}
		}//对每个客户端检查分发

		pFrameData->Dec();
	}
}

void CNetServer::DispatchLiveStream(unsigned long stream, BITMAPINFOHEADER *pBmpInfo, WAVEFORMATEX *pWaveInfo)
{
	CLIENT_SERVICE_INFO *pServiceInfo;
	CFrameData *pFrameData;
	long ret = 0;
	bool bHaveDispatchStream = false;

	while(m_pMessageMan->GetFrame(&pFrameData, stream))
	{
		bHaveDispatchStream = true;

		FRAME_INFO_EX &frameInfo = pFrameData->GetFrameInfo();

		assert(pFrameData->GetDataId() == INVALID_INDEX(unsigned long));
		if(pFrameData->GetDataId() != INVALID_INDEX(unsigned long))
		{
			continue;
		}
		
		//如果是视频格式，如果格式有变化才再次保存，在保存前检查格式的正确性
		if(frameInfo.frameType == FRAME_TYPE_VIDEO_FORMAT)
		{
			BITMAPINFOHEADER *pNewBmpInfo = (BITMAPINFOHEADER *)frameInfo.pData;
			if(memcmp(pNewBmpInfo, &pBmpInfo[CHANNEL_ID(frameInfo.channel)], sizeof(BITMAPINFOHEADER)) == 0)
			{
				pFrameData->Dec();
				continue;
			}
			if(pNewBmpInfo->biSize != sizeof(BITMAPINFOHEADER))
			{
				assert(false);
				pFrameData->Dec();
				continue;
			}
			else
			{
				memcpy(&pBmpInfo[CHANNEL_ID(frameInfo.channel)], frameInfo.pData, sizeof(BITMAPINFOHEADER));
			}
		}

		if(frameInfo.frameType == FRAME_TYPE_AUDIO_FORMAT)
		{
			memcpy(&pWaveInfo[CHANNEL_ID(frameInfo.channel)], frameInfo.pData, sizeof(WAVEFORMATEX));
		}

		POS pos = m_lstClientServiceInfo.GetHeadPosition();
		while(NULL != pos)
		{
			pServiceInfo = m_lstClientServiceInfo.GetNext(pos);

			if((frameInfo.frameType == FRAME_TYPE_VIDEO) || ((frameInfo.frameType == FRAME_TYPE_VIDEO_FORMAT)))
			{
#ifdef __SUPPORT_3G__
				if (g_curNet3G&& m_bStartPlayback && (!frameInfo.keyFrame))
				{
					//printf("m_bStartup3G =  %d, m_bStartPlayback = %d, bkeyFrame = %d  !!!!!!!!!!!!\n", m_bStartup3G, m_bStartPlayback, frameInfo.keyFrame);
				}
				else
				{
#endif
				if(SEND_FRAME_NEED_FORMAT == pServiceInfo->pAVDataSendMan->PutVideoData(pFrameData))
				{
					if(pBmpInfo[CHANNEL_ID(frameInfo.channel)].biSize == sizeof(BITMAPINFOHEADER))
					{
						//发送数据格式
						FRAME_INFO_EX formatFrame = frameInfo;
						formatFrame.frameType = FRAME_TYPE_VIDEO_FORMAT;
						formatFrame.length = sizeof(BITMAPINFOHEADER);
						formatFrame.pData = new unsigned char[formatFrame.length];
						memcpy(formatFrame.pData, &pBmpInfo[CHANNEL_ID(frameInfo.channel)], sizeof(BITMAPINFOHEADER));

						CFrameData *pFormatFrame = new CFrameData(formatFrame, 0, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
						//pFormatFrame->SetProtocol(CMD_REPLY_DATA_STREAM);
						//pFormatFrame->SetDataId(INVALID_INDEX(unsigned long));
						pServiceInfo->pAVDataSendMan->PutVideoData(pFormatFrame);

						//发送格式后，重新把数据放入分包模块中
						pServiceInfo->pAVDataSendMan->PutVideoData(pFrameData);
					}
					else
					{
						assert(false);
					}
				}
#ifdef __SUPPORT_3G__
				}
#endif
			}
			else if((frameInfo.frameType == FRAME_TYPE_AUDIO) || ((frameInfo.frameType == FRAME_TYPE_AUDIO_FORMAT)))
			{						
				if(SEND_FRAME_NEED_FORMAT == pServiceInfo->pAVDataSendMan->PutAudioData(pFrameData))
				{
					//发送音频数据格式
					FRAME_INFO_EX formatFrame = frameInfo;
					formatFrame.frameType = FRAME_TYPE_AUDIO_FORMAT;
					formatFrame.length = sizeof(WAVEFORMATEX);
					formatFrame.pData = new unsigned char[formatFrame.length];
					memcpy(formatFrame.pData, &pWaveInfo[CHANNEL_ID(frameInfo.channel)], sizeof(WAVEFORMATEX));

					CFrameData *pFormatFrame = new CFrameData(formatFrame, 0, CMD_REPLY_DATA_STREAM, INVALID_INDEX(unsigned long));
					//pFormatFrame->SetProtocol(CMD_REPLY_DATA_STREAM);
					//pFormatFrame->SetDataId(INVALID_INDEX(unsigned long));
					pServiceInfo->pAVDataSendMan->PutAudioData(pFormatFrame);

					//发送格式后，重新把数据放入分包模块中
					pServiceInfo->pAVDataSendMan->PutAudioData(pFrameData);
				}
			}
			else
			{
				assert(false);
			}
		}//对每个客户端检查发送

		if(FRAME_TYPE_VIDEO == frameInfo.frameType)
		{
			m_pDispatchFrameNum[CHANNEL_ID(frameInfo.channel)]++;
			if(0 == m_pFirstDispatchFrameTime[CHANNEL_ID(frameInfo.channel)])
			{
				m_pFirstDispatchFrameTime[CHANNEL_ID(frameInfo.channel)] = frameInfo.time;
			}

			if((m_pDispatchFrameNum[CHANNEL_ID(frameInfo.channel)] > 30) || ((frameInfo.time - m_pFirstDispatchFrameTime[CHANNEL_ID(frameInfo.channel)]) > 1000*1000))
			{
				memset(m_pDispatchFrameNum, 0, sizeof(long) * m_netInitInfo.dwMaxVideo);
				memset(m_pFirstDispatchFrameTime, 0, sizeof(LONGLONG) * m_netInitInfo.dwMaxVideo);

				pFrameData->Dec();
				break;
			}
		}

		pFrameData->Dec();
	}

	if (bHaveDispatchStream)
	{
		memset(m_pDispatchFrameNum, 0, sizeof(long) * m_netInitInfo.dwMaxVideo);
		memset(m_pFirstDispatchFrameTime, 0, sizeof(LONGLONG) * m_netInitInfo.dwMaxVideo);
	}
}

void CNetServer::DispatchPBStream()
{
	CFrameData *pFrameData;
	CLIENT_SERVICE_INFO *pServiceInfo;
	long ret = 0;

	while(m_pMessageMan->GetPBFrame(&pFrameData))
	{
		FRAME_INFO_EX &frameInfo = pFrameData->GetFrameInfo();

		POS pos = m_lstClientServiceInfo.GetHeadPosition();
		while(NULL != pos)
		{
			pServiceInfo = m_lstClientServiceInfo.GetNext(pos);
			assert(pFrameData->GetDataId() != INVALID_INDEX(unsigned long));

			if(pFrameData->GetDataId() == INVALID_INDEX(unsigned long))
			{
				continue;
			}

			if(pFrameData->GetDataId() != pServiceInfo->clientID)
			{
				continue;
			}

			if((frameInfo.frameType == FRAME_TYPE_VIDEO) || ((frameInfo.frameType == FRAME_TYPE_VIDEO_FORMAT)))
			{
				ret = pServiceInfo->pAVDataSendMan->PutVideoData(pFrameData, false);

				assert(SEND_FRAME_NEED_FORMAT != ret);
			}
			else if((frameInfo.frameType == FRAME_TYPE_AUDIO) || ((frameInfo.frameType == FRAME_TYPE_AUDIO_FORMAT)))
			{
				ret = pServiceInfo->pAVDataSendMan->PutAudioData(pFrameData, false);

				assert(SEND_FRAME_NEED_FORMAT != ret);
			}
			else if((frameInfo.frameType == FRAME_TYPE_END) || (frameInfo.frameType == FRAME_TYPE_EVENT))
			{
				pServiceInfo->pAVDataSendMan->PutOtherData(pFrameData);
			}
			else
			{
				assert(false);
			}
		}//对每个客户端检查发送

		pFrameData->Dec();
	}
}

void CNetServer::DispatchJpegStream()
{
	CLIENT_SERVICE_INFO *pServiceInfo;
	CFrameData *pFrameData;
	long ret = 0;

	while(m_pMessageMan->GetFrame(&pFrameData, LIVE_FRAME_JPEG))
	{
		FRAME_INFO_EX &frameInfo = pFrameData->GetFrameInfo();

		POS pos = m_lstClientServiceInfo.GetHeadPosition();
		while(NULL != pos)
		{
			pServiceInfo = m_lstClientServiceInfo.GetNext(pos);

			if(frameInfo.frameType == FRAME_TYPE_JPEG)
			{
				if(SEND_FRAME_SUCC != pServiceInfo->pAVDataSendMan->PutJpegData(pFrameData))
				{
					assert(false);
				}
			}
			else
			{
				assert(false);
			}
		}//对每个客户端检查发送

		pFrameData->Dec();
	}
}

void CNetServer::RemoveClient(CLIENT_SERVICE_INFO *pClient)
{
	pClient->pMultiNet->RemoveComm(pClient->clientID);
	//m_multiNetComm.RemoveComm(pClient->clientID);

	//清除这个连接对应的待发送数据及各种记录
	//释放该客户占用的主码流资源
	unsigned long localVideoNum = CProduct::Instance()->LocalVideoInputNum();
	for(long channel = 0; channel < m_netInitInfo.dwMaxVideo; channel++)
	{
		if(pClient->ullMasterVideo & (0x01 << channel))
		{
			//对所有通道回收主码流资源
			assert(m_pFastVideoNum[channel] > 0);
			if(m_pFastVideoNum[channel] > 0)
			{
				m_pFastVideoNum[channel]--;
			}
			if (m_bLimitFirstStreamSendNum && m_curFirstStreamSendNum > 0)
			{
				m_curFirstStreamSendNum--;
			}
			//对数字通道回放主码流资源
			if(channel >= localVideoNum)
			{
				assert(m_curIPCTransferChnnNum > 0);
				if(m_curIPCTransferChnnNum > 0)
				{
					m_curIPCTransferChnnNum--;
				}
			}
		}
	}

	//在回放或备份时异常中断后，一些统计信息需要重新整理
	STREAM_CH_MAP::iterator it;
	for(it = pClient->pStreamCHMap->begin(); it != pClient->pStreamCHMap->end(); it++)
	{
		for(long channel = 0; channel < m_netInitInfo.dwMaxVideo; channel++)
		{
			if((0x01 << channel) & (*it).second)
			{
				assert(m_pFastVideoNum[channel] > 0);
				if(m_pFastVideoNum[channel] > 0)
				{
					m_pFastVideoNum[channel]--;
				}
				if (m_bLimitFirstStreamSendNum && m_curFirstStreamSendNum > 0)
				{
					m_curFirstStreamSendNum--;
				}
				if(channel >= localVideoNum)
				{
					if(m_curIPCTransferChnnNum > 0)
					{
						m_curIPCTransferChnnNum--;
					}
				}
			}
		}
	}

	MESSAGE_DATA msgData;
	msgData.clientID = pClient->clientID;
	msgData.deviceID = 0;

	//发送停止网络现场码流的命令，以通知主程序实现网络帧率的自适应
	if(pClient->ullMasterVideo | pClient->ullSecondVideo)
	{
		msgData.dataLen = sizeof(PACKCMD) + sizeof(NET_LIVESTREAM_CTRL_INFO);
		msgData.pData = new char [msgData.dataLen];
		memset(msgData.pData, 0, msgData.dataLen);

		PACKCMD *pPackCmd = (PACKCMD *)msgData.pData;
		NET_LIVESTREAM_CTRL_INFO *pLiveStreamReq = (NET_LIVESTREAM_CTRL_INFO *)(msgData.pData+sizeof(PACKCMD));

		pPackCmd->cmdType = CMD_REQUEST_STREAM_STOP;
		pPackCmd->dataLen = sizeof(NET_LIVESTREAM_CTRL_INFO);

		pLiveStreamReq->streamID = pClient->dwLiveStreamID;

		m_pMessageMan->PutMsgToServer(msgData);
	}

	//发送停止回放或备份的命令
	for(it = pClient->pStreamCHMap->begin(); it != pClient->pStreamCHMap->end(); it++)
	{
		msgData.dataLen = sizeof(PACKCMD) + sizeof(NET_DATA_SEARCH);
		msgData.pData = new char [msgData.dataLen];
		memset(msgData.pData, 0, msgData.dataLen);

		PACKCMD *pPackCmd = (PACKCMD *)msgData.pData;
		NET_DATA_SEARCH *pDataSearch = (NET_DATA_SEARCH *)(msgData.pData+sizeof(PACKCMD));

		pPackCmd->cmdType = CMD_REQUEST_DATA_STOP;
		pPackCmd->dataLen = sizeof(NET_DATA_SEARCH);

		pDataSearch->streamID = (*it).first;
		m_pMessageMan->PutMsgToServer(msgData);
	}

	//发送停止JPEG流的命令
	if(pClient->ullJpegStream != 0)
	{
		msgData.dataLen = sizeof(PACKCMD) + sizeof(NET_JPEG_STREAM_INFO);
		msgData.pData = new char [msgData.dataLen];
		memset(msgData.pData, 0, msgData.dataLen);

		PACKCMD *pPackCmd = (PACKCMD *)msgData.pData;
		NET_JPEG_STREAM_INFO *pJpegInfo = (NET_JPEG_STREAM_INFO *)(msgData.pData+sizeof(PACKCMD));

		pPackCmd->cmdType = CMD_REQUEST_JPEG_STOP;
		pPackCmd->dataLen = sizeof(NET_JPEG_STREAM_INFO);

		m_pMessageMan->PutMsgToServer(msgData);
	}

	//如果开启了对讲，发送停止对讲的命令
	if(pClient->bTalking)
	{
		msgData.dataLen = sizeof(PACKCMD);
		msgData.pData = new char [msgData.dataLen];
		memset(msgData.pData, 0, msgData.dataLen);

		PACKCMD *pPackCmd = (PACKCMD *)msgData.pData;
		pPackCmd->cmdType = CMD_REQUEST_TALK_STOP;
		pPackCmd->dataLen = 0;

		m_pMessageMan->PutMsgToServer(msgData);
		pClient->bTalking = false;
	}

	{
		msgData.dataLen = sizeof(PACKCMD);
		msgData.pData = new char [msgData.dataLen];
		memset(msgData.pData, 0, msgData.dataLen);

		PACKCMD *pPackCmd = (PACKCMD *)msgData.pData;
		pPackCmd->cmdType = CMD_REQUEST_UPDATE_CANCEL;
		pPackCmd->dataLen = 0;

		m_pMessageMan->PutMsgToServer(msgData);
	}

	//发送空的指令，表示客户端已断开连接
	msgData.pData = NULL;
	msgData.dataLen = 0;
	m_pMessageMan->PutMsgToServer(msgData);

	pClient->pAVDataSendMan->StopAllStream();
	pClient->pStreamCHMap->clear();

	pClient->pLastStreamErrorList->clear();

	delete pClient->pLastStreamErrorList;
	delete pClient->pAVDataSendMan;
	delete pClient->pStreamCHMap;
	delete [] pClient->pSendBuffer;
	delete pClient;
}

#ifdef   __ENVIRONMENT_LINUX__

PUB_THREAD_RESULT PUB_THREAD_CALL CNetServer::MulticastThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif
	reinterpret_cast<CNetServer *>(pParam)->MulticastProc();
	return 0;

}

int CNetServer::MulticastProc()
{
	char szBuff[1024] = {0};	//这个缓冲区又收又发
	int iRet = 0;
	MULTICAST_INFO *pInfo = NULL;
	CNetInterface *pNetIf = CNetInterface::Instance();
	char	szGateWay[20] = {0};
	char	szIp[20] = {0};
	char	szMask[20] = {0};
	char	*pIfName = "eth0";
	bool    bHaveIp = true;
	CMulticast *pMultcast = &m_multicastEth0;

multicast_begin:
	do 
	{

#if defined    __DVS_7114__ || defined __DVS_7720__ || defined __DVS_2834__ || defined __DVS_9910__ || defined __DVS_131__ || defined __IPC_8180_7725__
		bHaveIp = true;
#else
		bHaveIp = false;
		memset(szIp, 0, sizeof(szIp));
		if (pNetIf->GetIPText(szIp, sizeof(szIp),"eth0"))
		{
			bHaveIp = true;
			if((0 == strcmp(szIp, "127.0.0.1")) || (0 == strlen(szIp)))
			{
				bHaveIp = false;
			}
		}
		if (!bHaveIp)
		{
			iRet = -1;
			PUB_Sleep(5000);
		}		
#endif

		if (bHaveIp)
		{
			if(0 != (iRet = m_multicastEth0.Init(DEFAULT_LOCAL_PORT, MULTICAST_PORT, MULTICAST_IP, "eth0")))
			{
				PUB_Sleep(200);
				continue;
			}			
		}

	/*	if (pNetIf->GetIPText(szIp, sizeof(szIp),"rausb0", 6))
		{
			if(0 != (iRet = m_multicastWifi.Init(DEFAULT_LOCAL_PORT, MULTICAST_PORT, MULTICAST_IP, "rausb0")))
			{
				m_multicastEth0.Quit();
				PUB_Sleep(200);
				continue;
			}
		}*/
	} while(m_bmulticastRun && (0 != iRet));

	while(m_bmulticastRun)
	{		
		//if (pMultcast == &m_multicastWifi)
		//{
		//	pMultcast = &m_multicastEth0;
		//	pIfName = "eth0";
		//}
		//else
		//{
		//	pMultcast = &m_multicastWifi;
		//	pIfName = "rausb0";			
		//}

		if(0 < (iRet = pMultcast->RecvFrom(szBuff, sizeof(szBuff))))
		{
			//是广播的自动搜索的包
			if (iRet == sizeof(MULTICAST_INFO)) 
			{
				pInfo = reinterpret_cast<MULTICAST_INFO*>(szBuff);
				if(pInfo->mulHead.head == FLAG_MULTIHEAD)
				{
					//收到请求
					switch(pInfo->mulHead.cmd)
					{
					case MULTICAST_CMD_SEARCH:
						{
							memset(pInfo, 0, sizeof(MULTICAST_INFO));
							pInfo->mulHead.head=FLAG_MULTIHEAD;
							pInfo->mulHead.ver=MULTICAST_VER;
							pInfo->mulHead.cmd=MULTICAST_CMD_INFO;
							strncpy(pInfo->DevName, m_netInitInfo.deviceName, DEVICE_NAME_MAX_LEN);									

							memset(szGateWay, 0, sizeof(szGateWay));
							memset(szIp, 0, sizeof(szIp));
							memset(szMask, 0, sizeof(szMask));

							if(!pNetIf->GetMAC(reinterpret_cast<char*>(pInfo->MacAddr), pIfName))
							{
								break;
							}

							if(pNetIf->GetIPText(szIp,sizeof(szIp), pIfName))
							{
								pInfo->ipaddr = inet_addr(szIp);
							}
							else
							{
								pInfo->ipaddr = 0;
							}

							if(pNetIf->GetIPMask(szMask,sizeof(szMask), pIfName))
							{
								pInfo->netmask = inet_addr(szMask);
							}
							else
							{
								pInfo->netmask = 0;
							}

							if(pNetIf->GetGateway(szGateWay, NULL, NULL, pIfName))
							{
								pInfo->route = inet_addr(szGateWay);
							}
							else
							{
								pInfo->route = 0;
							}														

							pInfo->netport= m_netInitInfo.nPort;
							pInfo->softver = m_netInitInfo.dwSoftVer;			//软件版本
							
							pInfo->devVer  = m_netInitInfo.dwDevVer;			//硬件版本
							
							pInfo->softbuilddate = m_netInitInfo.dwBuildTime;

							pInfo->nHttpPort = m_netInitInfo.nHttpPort;
							pInfo->deviceType = m_netInitInfo.dwDeviceType;
							pInfo->productType = m_netInitInfo.dwProductType;
							pInfo->serverType = CProduct::Instance()->GetDeviceType();

							GetKernelverionEx(&pInfo->kernelVersion);

							// 发送信息 
							if (pMultcast->SendTo(pInfo, sizeof(MULTICAST_INFO)) < 0) 
							{
								PUB_PrintError(__FILE__,__LINE__);
								m_multicastEth0.Quit();
								//m_multicastWifi.Quit();
								goto multicast_begin;

								//if (pMultcast == &m_multicastEth0)
								//{
								//	pMultcast->Quit();
								//	goto multicast_begin;
								//}	
							}
						}
						break;

					case MULTICAST_CMD_SET:
						if (0 == strcmp(pIfName,"rausb0"))
						{
							//if (0 == memcmp(pInfo->MacAddr, m_szMacRausb0, 6))
							//{
							//	m_pMsgCallback(m_pCallbackObject, MSG_MULTIPLE_CHANGE_IP, pIfName, pInfo);
							//}
						}
						else
						{
							//if (0 == memcmp(pInfo->MacAddr, m_szMac, 6))
							//{
							//	m_pMsgCallback(m_pCallbackObject, MSG_MULTIPLE_CHANGE_IP, pIfName, pInfo);
							//}
						}

						break;

					case MULTICAST_CMD_SET_MAC:
						//m_pMsgCallback(m_pCallbackObject, MSG_SET_MAC, pIfName, pInfo->MacAddr);
						break;

					case MULTICAST_CMD_SET_DEFAULT_VIDEOFORMAT:
						//m_pMsgCallback(m_pCallbackObject, MSG_SET_DEFAULT_INPUTFORM, NULL, &pInfo->inputform);
						break;

					case MULTICAST_CMD_SET_DEV_VER:
						//m_dwHardVer = pInfo->devVer;
						//m_pMsgCallback(m_pCallbackObject, MSG_SET_HARDWARE_VERSION, NULL, &pInfo->devVer);
						break;						
					}					
				}
			}
		}
		else
		{
#ifdef __CHIP3515__
			PUB_Sleep(1000);
#else
			PUB_Sleep(1000);
#endif
		}		
	}
	m_multicastEth0.Quit();
	//m_multicastWifi.Quit();
	return 0;
}

#endif

bool CNetServer::HasAnySecondStreamOfOther(CLIENT_SERVICE_INFO *pClient)
{
	POS pos = m_lstClientServiceInfo.GetHeadPosition();
	while(pos)
	{
		CLIENT_SERVICE_INFO* &pClientInfo = m_lstClientServiceInfo.GetNext(pos);
		if(pClientInfo != pClient)
		{
			if(pClientInfo->ullSecondVideo)
			{
				return true;
			}
		}
	}

	return false;
}

