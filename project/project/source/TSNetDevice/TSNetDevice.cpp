// TSNetDevice.cpp : 定义 DLL 应用程序的入口点。
//

#include "TSNetDevice.h"
#include "DDPublic.h"
#include "SWL_Public.h"
#include "SWL_MultiNetComm.h"
#include "SWL_ConnectProcEx.h"
#include "DeviceTypeHeader.h"

#ifdef __ENVIRONMENT_LINUX__
#include "lvfw.h"
#include <netdb.h>
#else
#include <vfw.h>
#include <nb30.h>
#endif

#include "IPDataBufferMan.h"
#include "ipc_v3/platform_ipc_v3.h"
#include "TSCfgToIpc.h"
#include "TSNdParam.h"

using namespace std;
using namespace DEVICE_TYPE_DEF;

struct _ts_netdevice_param
{
	bool						m_bCommInitOk;		//网络传输的对象初始化是否成功

	long						m_deviceID;			//用作网络传输对
	unsigned long				m_DevType;			//网络设备类型
	unsigned long				m_ProductType;		//网络设备产品型号
	unsigned long				m_CfgVer;			//网络设备配置版本号

	//用户名，密码加密相关
	unsigned long				encryptType;		//加密类型，0：不加密，1：简单加密
	unsigned long				encryptParam;		//加密参数

	CWaitEvent					m_recvEvent;		//用于同步接收网络数据的信号
	CPUB_Lock					m_lockFrameList;	//音视频帧列表的资源锁

	REPLY_CMD_DATA				m_replyData;		//接收到的网络数据的暂存结构
	NET_DEVICE_STATUS			m_deviceStatus;		//设备连接状态NET_DEVICE_STATUS
	NET_DEVICE_STATUS_CONNECT   m_ctrlStatus;		//设备连接是否可用的连接状态

	IPC_V3::NET_LOGIN_SUCCESS_REPLY		m_LoginReply;		//登录返回的网络设备参数
	ULONGLONG					m_VideoLossStatus;	//网络设备的丢失报警状态
	ULONGLONG					m_MotionStatus;		//移动报警状态
	ULONGLONG					m_SensorStatus;		//传感器报警状态
	ULONGLONG					m_Other;			//

	list<NET_COMBINED_INFO_EX *> m_lstCombinedInfo;	//用于对分包数据重组的辅助列表

	BITMAPINFOHEADER			m_videoFormat;		//保存视频格式
	WAVEFORMATEX				m_audioFormat;		//保存音频格式
	BITMAPINFOHEADER			m_secondVideoFormat;
	BITMAPINFOHEADER			m_minorVideoFormat;

	//当前已经请求的码流
	ULONGLONG					m_curVideo;
	ULONGLONG					m_curAudio;
	ULONGLONG					m_curSecondVideo;
	ULONGLONG					m_curMinorVideo;
	ULONGLONG					m_curJpeg;

	//绑定通道
	long						bindChannel;
	
	//为每一帧重新设定时间
	LONGLONG					prevTimeStamp;
	LONGLONG					curFrameTime;

	LONGLONG					prevSecondTimeStamp;
	LONGLONG					curSecondFrameTime;

	LONGLONG					prevMinorTimeStamp;
	LONGLONG					curMinorFrameTime;

	CIPDataBufferMan			*m_pDataBufferMan;
	DVR_IP_IN_FRM_INFO			ipFrameInfo;

	CIPDataBufferMan			*pSecondDataBufferMan;
	CIPDataBufferMan			*pMinorDataBufferMan;
	CIPDataBufferMan			*m_pJpegBufferMan;
	//用于改变绑定通道时数据的同步
	CPUB_Lock					reBindChannelLock;
	bool						bNeedIFrame;
	bool						bSecondNeedIFrame;
	bool						bMinorNeedIFrame;

	//当数据帧处理不过来时，标记下次先处理未处理过来的帧
	bool						bHasFrameWaitProcess;

	//心跳处理
	CPUB_Lock					timerLock;
	unsigned long				prevRecvTime;
	unsigned long				prevRecvTick;
#ifdef TEST_DISCONNECT
	unsigned long				prevPrintTick;
	unsigned long				recvNoDataTimes;
#endif
	unsigned long				prevHeartBeatTime;
	unsigned long				prevTimerTime;

	CPUB_Lock					netRequestLock;

	unsigned long				videoStreamIndex;
	unsigned long				secondVideoStreamIndex;
	unsigned long				minorVideoStreamIndex;

	bool						bNeedInitNetConfig;
#ifdef TEST_DISCONNECT
	char						strIP[16];
#endif
	unsigned long				netTimeout;
	unsigned long				bindSensorChannel;
};

extern CTS_MultiNetComm		*g_pMultiNetComm;
long						g_startCount = 0;
CPUB_Lock                   g_lockNetComm;

bool StartNetComm()
{
	g_lockNetComm.Lock();
	if(0 == g_startCount)
	{
		assert(g_pMultiNetComm == NULL);
		g_pMultiNetComm = new CTS_MultiNetComm;

		if(!g_pMultiNetComm->Init())
		{
			delete g_pMultiNetComm;
			g_pMultiNetComm = NULL;
			g_lockNetComm.UnLock();
			return false;
		}

		g_startCount++;
		g_lockNetComm.UnLock();
		return true;
	}

	g_startCount++;
	g_lockNetComm.UnLock();
	return true;
}

void StopNetComm()
{
	g_lockNetComm.Lock();
	if(0 == g_startCount)
	{
		g_lockNetComm.UnLock();
		return;
	}

	assert(NULL != g_pMultiNetComm);

	g_startCount--;
	if(0 == g_startCount)
	{
		g_pMultiNetComm->Destroy();
		delete g_pMultiNetComm;
		g_pMultiNetComm = NULL;
	}
	g_lockNetComm.UnLock();
}

CTSNetDevice::CTSNetDevice(void)
{
	m_pTSNetDeviceParam = NULL;
	m_pTsCfgToIpc = NULL;
	m_pTsNdParam = NULL;

	m_iIpcUsedChipFlag = 0;

	m_iAudioFrameMaxCnt = 0;
	m_localVideoNum = 0;

	InitParam();
}

CTSNetDevice::~CTSNetDevice(void)
{
	QuitParam();
}

bool CTSNetDevice::InitParam()
{
	m_pTSNetDeviceParam = new _ts_netdevice_param;

	m_pTSNetDeviceParam->m_bCommInitOk		= false;

	m_pTSNetDeviceParam->m_deviceID			= -1;
	m_pTSNetDeviceParam->m_DevType			= 0;
	m_pTSNetDeviceParam->m_ProductType		= 0;
	m_pTSNetDeviceParam->m_CfgVer			= 0;
	m_pTSNetDeviceParam->encryptType		= 0;
	m_pTSNetDeviceParam->encryptParam		= 0;
	memset(&(m_pTSNetDeviceParam->m_replyData), 0, sizeof(REPLY_CMD_DATA));
	m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_DISCONNECT;
	m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_ERROR;
	m_pTSNetDeviceParam->m_VideoLossStatus	= 0;
	m_pTSNetDeviceParam->m_MotionStatus		= 0;
	m_pTSNetDeviceParam->m_SensorStatus		= 0;
	m_pTSNetDeviceParam->m_Other			= 0;
	m_pTSNetDeviceParam->m_curVideo			= 0;
	m_pTSNetDeviceParam->m_curAudio			= 0;
	m_pTSNetDeviceParam->m_curSecondVideo	= 0;
	m_pTSNetDeviceParam->m_curMinorVideo	= 0;
	m_pTSNetDeviceParam->m_curJpeg          = 0;
	m_pTSNetDeviceParam->bindChannel		= -1;
	m_pTSNetDeviceParam->m_pDataBufferMan	= NULL;
	m_pTSNetDeviceParam->pSecondDataBufferMan = NULL;
	m_pTSNetDeviceParam->pMinorDataBufferMan = NULL;
	m_pTSNetDeviceParam->m_pJpegBufferMan   = NULL;
	m_pTSNetDeviceParam->bNeedIFrame		= true;
	m_pTSNetDeviceParam->bSecondNeedIFrame	= true;
	m_pTSNetDeviceParam->bMinorNeedIFrame	= true;
	m_pTSNetDeviceParam->prevRecvTime		= 0;
	m_pTSNetDeviceParam->prevRecvTick		= 0;
#ifdef TEST_DISCONNECT
	m_pTSNetDeviceParam->prevPrintTick		= 0;
	m_pTSNetDeviceParam->recvNoDataTimes	= 0;
#endif
	m_pTSNetDeviceParam->prevTimerTime		= 0;
	m_pTSNetDeviceParam->prevHeartBeatTime	= 0;
	m_pTSNetDeviceParam->videoStreamIndex	= 0;
	m_pTSNetDeviceParam->secondVideoStreamIndex = 1;
	m_pTSNetDeviceParam->minorVideoStreamIndex = 4;
	m_pTSNetDeviceParam->bNeedInitNetConfig	= true;
	m_pTSNetDeviceParam->netTimeout = MAX_KEEP_TIME;
	m_pTSNetDeviceParam->bindSensorChannel = -1;

	memset(&m_pTSNetDeviceParam->ipFrameInfo, 0, sizeof(DVR_IP_IN_FRM_INFO));

	memset(&m_pTSNetDeviceParam->m_videoFormat, 0, sizeof(BITMAPINFOHEADER));
	memset(&m_pTSNetDeviceParam->m_audioFormat, 0, sizeof(WAVEFORMATEX));

	m_pTSNetDeviceParam->m_videoFormat.biSize = sizeof(BITMAPINFOHEADER);
	m_pTSNetDeviceParam->m_videoFormat.biCompression = mmioFOURCC('H','2','6','4');
	m_pTSNetDeviceParam->m_videoFormat.biPlanes = 1;

	m_pTSNetDeviceParam->m_secondVideoFormat = m_pTSNetDeviceParam->m_videoFormat;
	m_pTSNetDeviceParam->m_minorVideoFormat = m_pTSNetDeviceParam->m_videoFormat;

	memset(&m_pTSNetDeviceParam->m_LoginReply, 0, sizeof(IPC_V3::NET_LOGIN_SUCCESS_REPLY));
	m_pTSNetDeviceParam->m_recvEvent.CreateWaitEvent();

	m_pTsNdParam = new CTSNdParam();
	m_pTsNdParam->Initial();

	m_pTsCfgToIpc = new CTSCfgToIpc();
	m_pTsCfgToIpc->Initial(this, m_pTsNdParam);

	return true;
}

void CTSNetDevice::QuitParam()
{
	if(m_pTsCfgToIpc != NULL)
	{
		m_pTsCfgToIpc->Quit();
		delete m_pTsCfgToIpc;
		m_pTsCfgToIpc = NULL;
	}

	if(m_pTsNdParam != NULL)
	{
		m_pTsNdParam->Quit();
		delete m_pTsNdParam;
		m_pTsNdParam = NULL;
	}

	if(m_pTSNetDeviceParam->m_replyData.pData)
	{
		delete [] m_pTSNetDeviceParam->m_replyData.pData;
		m_pTSNetDeviceParam->m_replyData.pData = NULL;
	}

	m_pTSNetDeviceParam->m_recvEvent.Close();

	if(m_pTSNetDeviceParam)
	{
		delete m_pTSNetDeviceParam;
		m_pTSNetDeviceParam = NULL;
	}
}

void CTSNetDevice::SetDataBufferMan(CIPDataBufferMan *pDataBufferMan)
{
	m_pTSNetDeviceParam->m_pDataBufferMan = pDataBufferMan + NET_DEVICE_MAIN_STREAM;
	m_pTSNetDeviceParam->pSecondDataBufferMan = pDataBufferMan + NET_DEVICE_SUB_STREAM;
	m_pTSNetDeviceParam->pMinorDataBufferMan = pDataBufferMan + NET_DEVICE_MINOR_STREAM;
	m_pTSNetDeviceParam->m_pJpegBufferMan = pDataBufferMan+ + NET_DEVICE_JPEG_STREAM;
}

bool CTSNetDevice::Initial(long deviceId, long chnn, char *pServer, unsigned short netPort, \
						   unsigned short httpPort, unsigned short ctrlPort, const std::string & user, \
						   const std::string & password, const ND_MAC & mac, const char *pEther, long sensorNum)
{
	//先退出, NVR会重复调用初始化动作, 必须先保证已经退出
	Quit();
	//初始化
	in_addr ipaddr;
	ipaddr.s_addr = inet_addr(pServer);
	if(INADDR_NONE == ipaddr.s_addr)
	{
		struct hostent *pHost = gethostbyname(pServer);
		if(NULL == pHost)
		{
			return false;
		}

		ipaddr = *(struct in_addr *)pHost->h_addr_list[0];
	}

	char *strIP = inet_ntoa(ipaddr);
#ifdef TEST_DISCONNECT
	strcpy(m_pTSNetDeviceParam->strIP, strIP);
#endif
	
	assert(!m_pTSNetDeviceParam->m_bCommInitOk);
	assert(m_pTSNetDeviceParam->m_deviceStatus == ND_STATUS_DISCONNECT);

	CSWL_ConnectProcEx *pConnectProc = CSWL_ConnectProcEx::NewConnectProc(strIP, netPort, pEther);
	if(pConnectProc == NULL)
	{
		return false;
	}

	SWL_socket_t sock = pConnectProc->ConnectSynEx(4000);
	pConnectProc->Destroy();

	if(sock == SWL_INVALID_SOCKET)
	{
		return false;
	}

	////////////////////////////////////////////////////////////////////////////
	////接受数据头
	////间隔200毫秒一次，总共40次  超时时间为8s 如果8s收不到数据就放弃
	char pDataBuf[64] = {0};
	NETDEVICE_TYPE_INFO *pDeviceTypeInfo = (NETDEVICE_TYPE_INFO *)pDataBuf;

	int  times = 0; 
	int  nRecvTotalLen = 0;
	int  nRecvLen = 0;
	while( true )
	{
		if(times > 40)
		{
			break;
		}

		if(SWL_SOCKET_ERROR == (nRecvLen = SWL_Recv(sock, pDataBuf + nRecvTotalLen, sizeof(pDataBuf) - nRecvTotalLen, 0) ) )
		{
			if( SWL_EWOULDBLOCK() )		
			{
				times ++;
				PUB_Sleep(200);
			}
			else
			{
				break;
			}
		}
		else
		{
			times ++;

			nRecvTotalLen += nRecvLen ;
			if(nRecvTotalLen >= sizeof(pDataBuf))
			{
				break;
			}
			else if( nRecvTotalLen >= sizeof(DWORD) && pDeviceTypeInfo->headFlag != SWL_HEADFLAG )
			{
				break;
			}
		}
	}

	if( (nRecvTotalLen != sizeof(pDataBuf) ) || (pDeviceTypeInfo->headFlag != SWL_HEADFLAG) )
	{
		SWL_CloseSocket(sock);
		return false;
	}

	//验证协议版本，客户端和服务器是否兼容
	unsigned long  ProtocolVer = pDeviceTypeInfo->netprotrocolver;

	m_pTSNetDeviceParam->m_deviceID			= (long)this;
	m_pTSNetDeviceParam->m_DevType			= pDeviceTypeInfo->devicetype;
	m_pTSNetDeviceParam->m_ProductType		= pDeviceTypeInfo->producttype;
	m_pTSNetDeviceParam->m_CfgVer			= pDeviceTypeInfo->configver;
	m_pTSNetDeviceParam->encryptType		= pDeviceTypeInfo->encryptType;
	m_pTSNetDeviceParam->encryptParam		= pDeviceTypeInfo->encryptParam;

	if( !StartNetComm() )
	{
		printf( "error startNetComm %s,%d\n", __FILE__, __LINE__ );
		SWL_CloseSocket(sock);
		return false;
	}
	g_pMultiNetComm->AddComm(m_pTSNetDeviceParam->m_deviceID, sock);
	g_pMultiNetComm->SetRecvBufferLen(m_pTSNetDeviceParam->m_deviceID, RECV_BUFFER_SIZE);
	g_pMultiNetComm->SetAutoRecvCallback(m_pTSNetDeviceParam->m_deviceID, ReceiveCallback, this);
	
	//登录前需要设置这些状态
	m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_OK;
	m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_READY;

	//开始登录到设备
	IPC_V3::NET_LOGIN_INFO netLoginInfo;
	memset(&netLoginInfo, 0, sizeof(IPC_V3::NET_LOGIN_INFO));
	netLoginInfo.netprotocolver = IPC_V3::NET_PROTOCOL_VER;
	netLoginInfo.connectType = IPC_V3::CONNECTTYPE_CTRL_CENTER;
	strncpy(netLoginInfo.username, user.c_str(), sizeof(netLoginInfo.username));
	strncpy(netLoginInfo.password, password.c_str(), sizeof(netLoginInfo.password));
	memcpy(netLoginInfo.MAC, mac.mac, sizeof(netLoginInfo.MAC));

	EncryptLoginInfo(&netLoginInfo);

	m_pTSNetDeviceParam->m_VideoLossStatus	= 0;
	m_pTSNetDeviceParam->m_MotionStatus		= 0;
	m_pTSNetDeviceParam->m_SensorStatus		= 0;
	m_pTSNetDeviceParam->m_Other			= 0;

	bool bRet = false;
	if(SendCmd(IPC_V3::CMD_REQUEST_LOGIN, (unsigned char*)(&netLoginInfo), sizeof(IPC_V3::NET_LOGIN_INFO)))
	{
		if(m_pTSNetDeviceParam->m_replyData.replyCmdID == IPC_V3::CMD_REPLY_LONGIN_SUCC)
		{
			bRet = true;

			assert(m_pTSNetDeviceParam->m_replyData.dataLen == sizeof(IPC_V3::NET_LOGIN_SUCCESS_REPLY));

			memcpy(&m_pTSNetDeviceParam->m_LoginReply, m_pTSNetDeviceParam->m_replyData.pData, sizeof(m_pTSNetDeviceParam->m_LoginReply));

			m_pTsNdParam->SetParamByLoginReply(m_pTSNetDeviceParam->m_LoginReply);

			m_pTSNetDeviceParam->m_curVideo = 0;
			m_pTSNetDeviceParam->m_curAudio  = 0;
			m_pTSNetDeviceParam->m_curSecondVideo = 0;
			m_pTSNetDeviceParam->m_curMinorVideo = 0;
			m_pTSNetDeviceParam->m_curJpeg = 0;

			m_pTSNetDeviceParam->prevTimeStamp = 0;
			m_pTSNetDeviceParam->curFrameTime = 0;

			m_pTSNetDeviceParam->prevSecondTimeStamp = 0;
			m_pTSNetDeviceParam->curSecondFrameTime = 0;

			m_pTSNetDeviceParam->prevMinorTimeStamp = 0;
			m_pTSNetDeviceParam->curMinorFrameTime = 0;

			m_pTSNetDeviceParam->bindChannel = chnn;
			m_pTSNetDeviceParam->bindSensorChannel = sensorNum;

			/////////////////////////////////////

			//成功登录, 则获取IPC系列参数, 
			//如果没有获取参数, 则认为过程失败
			bRet = false;
			unsigned char* pIpcBuff = NULL;
			unsigned long ipcDataLen = 0;
			if(m_pTsNdParam->GetInitialBuff(pIpcBuff, ipcDataLen))
			{
				//这里需要等待命令的回复结果
				m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_READY;
				if(SendCmd(IPC_V3::CMD_REQUEST_CFG_GET, pIpcBuff, ipcDataLen))
				{
					if(m_pTSNetDeviceParam->m_replyData.replyCmdID == IPC_V3::CMD_REPLY_CFG_DATA)
					{
						m_pTsNdParam->SetParamByCfg(m_pTSNetDeviceParam->m_replyData.pData, m_pTSNetDeviceParam->m_replyData.dataLen);
						SetInitConfig();
						bRet = true;
					}
				}
			}

			m_pTSNetDeviceParam->secondVideoStreamIndex = m_pTsCfgToIpc->GetSubStreamIndex();

			unsigned long maxVideoLen = 1.5*1024*1024;
			PostCmd(IPC_V3::CMD_REQUEST_MAX_VIDEO_LEN, (unsigned char *)&maxVideoLen, sizeof(unsigned long));
		}
	}

	//返回结果, 部分关键参数设置
	if(bRet)
	{
		m_pTSNetDeviceParam->m_bCommInitOk = true;
		m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_OK;
		m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_READY;
	}
	else
	{
		g_pMultiNetComm->RemoveComm(m_pTSNetDeviceParam->m_deviceID);
		StopNetComm();

		m_pTSNetDeviceParam->m_bCommInitOk = false;
		m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_DISCONNECT;
		m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_ERROR;
	}

	m_pTSNetDeviceParam->m_videoFormat.biHeight = 0;
	m_pTSNetDeviceParam->m_videoFormat.biWidth = 0;
	memset(&m_pTSNetDeviceParam->m_audioFormat, 0, sizeof(WAVEFORMATEX));
	m_pTSNetDeviceParam->m_secondVideoFormat = m_pTSNetDeviceParam->m_videoFormat;
	m_pTSNetDeviceParam->m_minorVideoFormat = m_pTSNetDeviceParam->m_videoFormat;

	m_iIpcUsedChipFlag = 0;
	if( m_pTSNetDeviceParam->m_ProductType >= 1600
		&& m_pTSNetDeviceParam->m_ProductType < 1700 )
	{
		printf( "current product type --------- %d\n", m_pTSNetDeviceParam->m_ProductType );
		m_iIpcUsedChipFlag = 1;
	}

	if(bRet)
	{
		m_reConnectCount ++;
	}

	return bRet;
}

void CTSNetDevice::Quit()
{
	if(m_pTSNetDeviceParam->m_bCommInitOk)
	{
		g_pMultiNetComm->SetAutoRecvCallback(m_pTSNetDeviceParam->m_deviceID, NULL, NULL);
		g_pMultiNetComm->RemoveComm(m_pTSNetDeviceParam->m_deviceID);
		StopNetComm();
	}

	m_pTSNetDeviceParam->m_bCommInitOk = false;

	m_pTSNetDeviceParam->netRequestLock.Lock();

	m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_DISCONNECT;
	m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_ERROR;


	list<NET_COMBINED_INFO_EX *>::iterator it = m_pTSNetDeviceParam->m_lstCombinedInfo.begin();
	for(; it != m_pTSNetDeviceParam->m_lstCombinedInfo.end(); it++)
	{
		delete [] (*it)->pBuffer;
		delete (*it);
	}
	m_pTSNetDeviceParam->m_lstCombinedInfo.clear();

	if(m_pTSNetDeviceParam->m_replyData.pData)
	{
		delete [] m_pTSNetDeviceParam->m_replyData.pData;
		m_pTSNetDeviceParam->m_replyData.pData = NULL;
	}
	memset(&(m_pTSNetDeviceParam->m_replyData), 0, sizeof(REPLY_CMD_DATA));

	m_pTSNetDeviceParam->netRequestLock.UnLock();
}

bool CTSNetDevice::IfNeedInitNetConfig()
{
	return m_pTSNetDeviceParam->bNeedInitNetConfig;
}

void CTSNetDevice::SetBindChannel(long chnn)
{
	m_pTSNetDeviceParam->reBindChannelLock.Lock();
	m_pTSNetDeviceParam->bindChannel = chnn;
	m_pTSNetDeviceParam->bNeedIFrame = true;
	m_pTSNetDeviceParam->bSecondNeedIFrame = true;
	m_pTSNetDeviceParam->bMinorNeedIFrame = true;

	if(ND_STATUS_OK != m_pTSNetDeviceParam->m_deviceStatus)
	{
		m_pTSNetDeviceParam->reBindChannelLock.UnLock();
		return;
	}

	if(m_pTSNetDeviceParam->bHasFrameWaitProcess)
	{
		list<NET_COMBINED_INFO_EX *>::iterator it;
		for(it = m_pTSNetDeviceParam->m_lstCombinedInfo.begin(); it != m_pTSNetDeviceParam->m_lstCombinedInfo.end(); it++)
		{
			if((*it)->dwUsed && ((*it)->dwTotalLen == (*it)->dwLen))
			{
				(*it)->dwUsed = false;
				(*it)->dwLen = 0;
			}
		}
		m_pTSNetDeviceParam->bHasFrameWaitProcess = false;
	}

	if((m_pTSNetDeviceParam->m_curVideo != 0) && (m_pTSNetDeviceParam->m_videoFormat.biWidth > 0))
	{
		//先补充视频头信息及音频头信息
		DVR_IP_IN_FRM_INFO frameInfo;
		memset(&frameInfo, 0, sizeof(DVR_IP_IN_FRM_INFO));
		frameInfo.channel = chnn;
		frameInfo.width = m_pTSNetDeviceParam->m_videoFormat.biWidth;
		frameInfo.height = m_pTSNetDeviceParam->m_videoFormat.biHeight;
		frameInfo.data_type = COM_FRAME_TYPE_VIDEO_FORMAT;
		frameInfo.len = sizeof(BITMAPINFOHEADER);
		frameInfo.vir_len = 0x1000 - sizeof(DVR_IP_IN_FRM_INFO);

		while(!m_pTSNetDeviceParam->m_pDataBufferMan->PushData(chnn, frameInfo, (unsigned char *)&m_pTSNetDeviceParam->m_videoFormat))
		{
			if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
			{
				break;
			}

			PUB_Sleep(10);
		}
	}

	if((m_pTSNetDeviceParam->m_curSecondVideo != 0) && (m_pTSNetDeviceParam->m_secondVideoFormat.biWidth > 0))
	{
		//先补充视频头信息及音频头信息
		DVR_IP_IN_FRM_INFO frameInfo;
		memset(&frameInfo, 0, sizeof(DVR_IP_IN_FRM_INFO));
		frameInfo.channel = chnn;
		frameInfo.width = m_pTSNetDeviceParam->m_secondVideoFormat.biWidth;
		frameInfo.height = m_pTSNetDeviceParam->m_secondVideoFormat.biHeight;
		frameInfo.data_type = COM_FRAME_TYPE_VIDEO_FORMAT;
		frameInfo.len = sizeof(BITMAPINFOHEADER);
		frameInfo.vir_len = 0x1000 - sizeof(DVR_IP_IN_FRM_INFO);

		while(!m_pTSNetDeviceParam->pSecondDataBufferMan->PushData(chnn, frameInfo, (unsigned char *)&m_pTSNetDeviceParam->m_secondVideoFormat))
		{
			if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
			{
				break;
			}

			PUB_Sleep(10);
		}
	}

	if((m_pTSNetDeviceParam->m_curMinorVideo != 0) && (m_pTSNetDeviceParam->m_minorVideoFormat.biWidth > 0))
	{
		//先补充视频头信息及音频头信息
		DVR_IP_IN_FRM_INFO frameInfo;
		memset(&frameInfo, 0, sizeof(DVR_IP_IN_FRM_INFO));
		frameInfo.channel = chnn;
		frameInfo.width = m_pTSNetDeviceParam->m_minorVideoFormat.biWidth;
		frameInfo.height = m_pTSNetDeviceParam->m_minorVideoFormat.biHeight;
		frameInfo.data_type = COM_FRAME_TYPE_VIDEO_FORMAT;
		frameInfo.len = sizeof(BITMAPINFOHEADER);
		frameInfo.vir_len = 0x1000 - sizeof(DVR_IP_IN_FRM_INFO);

		while(!m_pTSNetDeviceParam->pMinorDataBufferMan->PushData(chnn, frameInfo, (unsigned char *)&m_pTSNetDeviceParam->m_minorVideoFormat))
		{
			if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
			{
				break;
			}

			PUB_Sleep(10);
		}
	}

	if((m_pTSNetDeviceParam->m_curAudio != 0) && (m_pTSNetDeviceParam->m_audioFormat.nAvgBytesPerSec > 0))
	{
		DVR_IP_IN_FRM_INFO frameInfo;
		memset(&frameInfo, 0, sizeof(DVR_IP_IN_FRM_INFO));
		frameInfo.channel = chnn;
		frameInfo.data_type = COM_FRAME_TYPE_AUDIO_FORMAT;
		frameInfo.len = sizeof(WAVEFORMATEX);
		frameInfo.vir_len = 0x1000 - sizeof(DVR_IP_IN_FRM_INFO);
		while(!m_pTSNetDeviceParam->m_pDataBufferMan->PushData(chnn, frameInfo, (unsigned char *)&m_pTSNetDeviceParam->m_videoFormat))
		{
			if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
			{
				break;
			}

			PUB_Sleep(10);
		}

		//往第三码流加入音频2014.12.8
		while(!m_pTSNetDeviceParam->pMinorDataBufferMan->PushData(chnn, frameInfo, (unsigned char *)&m_pTSNetDeviceParam->m_videoFormat))
		{
			if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
			{
				break;
			}

			PUB_Sleep(10);
		}
	}

	m_pTSNetDeviceParam->reBindChannelLock.UnLock();
}

long CTSNetDevice::GetBindChannel()
{
	return m_pTSNetDeviceParam->bindChannel;
}

void CTSNetDevice::SetLocalVideoNum(long localNum)
{
	if ((localNum >=0)&&(localNum<127))
	{
		m_localVideoNum = localNum;
	}
}

long CTSNetDevice::GetDevStatus()
{
	return m_pTSNetDeviceParam->m_deviceStatus;
}

bool CTSNetDevice::IsReConnectting()
{
	//printf("%s:%s:%d, m_reConnectCount=%d, m_deviceStatus=%d\n", __FUNCTION__, __FILE__, __LINE__, m_reConnectCount, m_pTSNetDeviceParam->m_deviceStatus);

	if ((0 <= m_reConnectCount) && (ND_STATUS_OK != m_pTSNetDeviceParam->m_deviceStatus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CTSNetDevice::ResyncIPCSystemTime(int sync_flag)
{
	return 0;
}

void CTSNetDevice::SetBindSensorChannel(long sensorChnn)
{
	m_pTSNetDeviceParam->bindSensorChannel = sensorChnn;
}

long CTSNetDevice::GetBindSensorChannel()
{
	return m_pTSNetDeviceParam->bindSensorChannel;
}

long CTSNetDevice::IsReady()
{
	if(m_pTSNetDeviceParam->m_deviceStatus == ND_STATUS_OK)
	{
		return m_pTSNetDeviceParam->m_ctrlStatus;
	}

	return ND_SC_ERROR;
}

bool CTSNetDevice::RequestStream(ULONGLONG video1, ULONGLONG video2, ULONGLONG video3, ULONGLONG audio)
{
	m_pTSNetDeviceParam->netRequestLock.Lock();
	if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return false;
	}

	if(!WaitForReady())
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return false;
	}
	
	memset(&m_pTSNetDeviceParam->m_audioFormat, 0, sizeof(WAVEFORMATEX));

	m_pTSNetDeviceParam->bHasFrameWaitProcess = false;

	IPC_V3::NET_REQUEST_STREAM_CTRL liveStreamCtrl;
	memset(&liveStreamCtrl, 0, sizeof(liveStreamCtrl));

	ULONGLONG startVideo1 = video1 & ~m_pTSNetDeviceParam->m_curVideo;
	ULONGLONG stopVideo1 = m_pTSNetDeviceParam->m_curVideo & ~video1;
	ULONGLONG startVideo2 = video2 & ~m_pTSNetDeviceParam->m_curSecondVideo;
	ULONGLONG stopVideo2 = m_pTSNetDeviceParam->m_curSecondVideo & ~video2;
	ULONGLONG startVideo3 = video3 & ~m_pTSNetDeviceParam->m_curMinorVideo;
	ULONGLONG stopVideo3 = m_pTSNetDeviceParam->m_curMinorVideo & ~video3;
	ULONGLONG startAudio = audio & ~m_pTSNetDeviceParam->m_curAudio;
	ULONGLONG stopAudio = m_pTSNetDeviceParam->m_curAudio & ~audio;

	if(stopAudio != 0)
	{
		liveStreamCtrl.ucStreamType = IPC_V3::IPCAMERA_FRAME_TYPE_AUDIO;
		liveStreamCtrl.ucStreamIndex = 0;
		
		if(!PostCmd(IPC_V3::CMD_REQUEST_STREAM_STOP, (unsigned char *)&liveStreamCtrl, sizeof(liveStreamCtrl)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
		m_pTSNetDeviceParam->m_curAudio = 0;
	}
	if(stopVideo1 != 0)
	{
		liveStreamCtrl.ucStreamType = IPC_V3::IPCAMERA_FRAME_TYPE_VIDEO;
		liveStreamCtrl.ucStreamIndex = m_pTSNetDeviceParam->videoStreamIndex;
		
		if(!PostCmd(IPC_V3::CMD_REQUEST_STREAM_STOP, (unsigned char *)&liveStreamCtrl, sizeof(liveStreamCtrl)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
		m_pTSNetDeviceParam->m_curVideo = 0;
	}
	if(stopVideo2 != 0)
	{
		liveStreamCtrl.ucStreamType = IPC_V3::IPCAMERA_FRAME_TYPE_VIDEO;
		liveStreamCtrl.ucStreamIndex = m_pTSNetDeviceParam->secondVideoStreamIndex;
		
		if(!PostCmd(IPC_V3::CMD_REQUEST_STREAM_STOP, (unsigned char *)&liveStreamCtrl, sizeof(liveStreamCtrl)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
		m_pTSNetDeviceParam->m_curSecondVideo = 0;
	}
	if(stopVideo3 != 0)
	{
		liveStreamCtrl.ucStreamType = IPC_V3::IPCAMERA_FRAME_TYPE_VIDEO;
		liveStreamCtrl.ucStreamIndex = m_pTSNetDeviceParam->minorVideoStreamIndex;

		if(!PostCmd(IPC_V3::CMD_REQUEST_STREAM_STOP, (unsigned char *)&liveStreamCtrl, sizeof(liveStreamCtrl)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
		m_pTSNetDeviceParam->m_curMinorVideo = 0;
	}

	if(startVideo3 != 0)
	{
		m_pTSNetDeviceParam->m_minorVideoFormat.biHeight = 0;
		m_pTSNetDeviceParam->m_minorVideoFormat.biWidth = 0;
		//请求视频
		liveStreamCtrl.ucStreamType = IPC_V3::IPCAMERA_FRAME_TYPE_VIDEO;
		liveStreamCtrl.ucStreamIndex = m_pTSNetDeviceParam->minorVideoStreamIndex;
		if(!PostCmd(IPC_V3::CMD_REQUEST_STREAM_START, (unsigned char *)&liveStreamCtrl, sizeof(liveStreamCtrl)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
		m_pTSNetDeviceParam->m_curMinorVideo = video3;
	}
	if(startVideo2 != 0)
	{
		m_pTSNetDeviceParam->m_secondVideoFormat.biHeight = 0;
		m_pTSNetDeviceParam->m_secondVideoFormat.biWidth = 0;
		//请求视频
		liveStreamCtrl.ucStreamType = IPC_V3::IPCAMERA_FRAME_TYPE_VIDEO;
		liveStreamCtrl.ucStreamIndex = m_pTSNetDeviceParam->secondVideoStreamIndex;
		if(!PostCmd(IPC_V3::CMD_REQUEST_STREAM_START, (unsigned char *)&liveStreamCtrl, sizeof(liveStreamCtrl)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
		m_pTSNetDeviceParam->m_curSecondVideo = video2;
	}
	if(startVideo1 != 0)
	{
		m_pTSNetDeviceParam->m_videoFormat.biHeight = 0;
		m_pTSNetDeviceParam->m_videoFormat.biWidth = 0;
		//请求视频
		liveStreamCtrl.ucStreamType = IPC_V3::IPCAMERA_FRAME_TYPE_VIDEO;
		liveStreamCtrl.ucStreamIndex = m_pTSNetDeviceParam->videoStreamIndex;
		if(!PostCmd(IPC_V3::CMD_REQUEST_STREAM_START, (unsigned char *)&liveStreamCtrl, sizeof(liveStreamCtrl)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
		m_pTSNetDeviceParam->m_curVideo = video1;
	}
	if(startAudio != 0)
	{
		
		liveStreamCtrl.ucStreamType = IPC_V3::IPCAMERA_FRAME_TYPE_AUDIO;
		liveStreamCtrl.ucStreamIndex = 0;
		if(!PostCmd(IPC_V3::CMD_REQUEST_STREAM_START, (unsigned char *)&liveStreamCtrl, sizeof(liveStreamCtrl)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}

		m_pTSNetDeviceParam->m_curAudio = audio;
	}

	m_pTSNetDeviceParam->netRequestLock.UnLock();
	return true;
}


bool CTSNetDevice::RequestJpegStream(ULONGLONG channlMask)
{
	m_pTSNetDeviceParam->netRequestLock.Lock();
	if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return false;
	}

	if(!WaitForReady())
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return false;
	}

	IPC_V3::NET_REQUEST_STREAM_CTRL liveStreamCtrl;
	memset(&liveStreamCtrl, 0, sizeof(liveStreamCtrl));

	ULONGLONG startJpeg = channlMask & ~m_pTSNetDeviceParam->m_curJpeg;
	ULONGLONG stopJpeg = m_pTSNetDeviceParam->m_curJpeg & ~channlMask;


	if (stopJpeg != 0)
	{
		liveStreamCtrl.ucStreamType = IPC_V3::IPCAMERA_FRAME_TYPE_PIC;
		liveStreamCtrl.ucStreamIndex = m_pTSNetDeviceParam->videoStreamIndex;

		if(!PostCmd(IPC_V3::CMD_REQUEST_STREAM_STOP, (unsigned char *)&liveStreamCtrl, sizeof(liveStreamCtrl)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}

		m_pTSNetDeviceParam->m_curJpeg = 0;
	}

	if (startJpeg != 0)
	{
		liveStreamCtrl.ucStreamType = IPC_V3::IPCAMERA_FRAME_TYPE_PIC;
		liveStreamCtrl.ucStreamIndex = m_pTSNetDeviceParam->videoStreamIndex;

		if(!PostCmd(IPC_V3::CMD_REQUEST_STREAM_START, (unsigned char *)&liveStreamCtrl, sizeof(liveStreamCtrl)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}

		m_pTSNetDeviceParam->m_curJpeg = channlMask;
	}

	m_pTSNetDeviceParam->netRequestLock.UnLock();
	
	return true;
}

bool CTSNetDevice::RequestKeyFrame(ULONGLONG video1, ULONGLONG video2, ULONGLONG video3)
{
	m_pTSNetDeviceParam->netRequestLock.Lock();
	if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return false;
	}

	if(!WaitForReady())
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return false;
	}

	IPC_V3::NET_REQUEST_I_FRAME IFrameRequest;

	if(video1 != 0)
	{
		IFrameRequest.ucStreamIndex = m_pTSNetDeviceParam->videoStreamIndex;
		if(!PostCmd(IPC_V3::CMD_REQUEST_KEYFRAME, (unsigned char *)&IFrameRequest, sizeof(IFrameRequest)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
	}

	if(video2 != 0)
	{
		IFrameRequest.ucStreamIndex = m_pTSNetDeviceParam->secondVideoStreamIndex;
		if(!PostCmd(IPC_V3::CMD_REQUEST_KEYFRAME, (unsigned char *)&IFrameRequest, sizeof(IFrameRequest)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
	}

	if(video3 != 0)
	{
		IFrameRequest.ucStreamIndex = m_pTSNetDeviceParam->minorVideoStreamIndex;
		if(!PostCmd(IPC_V3::CMD_REQUEST_KEYFRAME, (unsigned char *)&IFrameRequest, sizeof(IFrameRequest)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
	}

	m_pTSNetDeviceParam->netRequestLock.UnLock();
	return true;
}

bool CTSNetDevice::PTZControl(long chnn, unsigned long cmdtype, unsigned long ptzParam, unsigned long speed, unsigned long waiteTime)
{
	/*
	IPC云台控制入口, 接口参数定义在PTZ.h中, 使用方式与CPtzMan一致
	*/

	m_pTSNetDeviceParam->netRequestLock.Lock();
	if(!WaitForReady())
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return false;
	}

	unsigned long ipcRequestType = 0;

	IPC_V3::NET_PTZ_CTRL ipcNetPtz;
	memset(&ipcNetPtz, 0, sizeof(IPC_V3::NET_PTZ_CTRL));

	ipcNetPtz.cmdPara = ptzParam;
	ipcNetPtz.speed = speed;

	switch(cmdtype)
	{
	case PTZ_CMD_NEAR:
	case PTZ_CMD_FAR:

	case PTZ_CMD_ZOOM_OUT:
	case PTZ_CMD_ZOOM_IN:

	case PTZ_CMD_IRIS_OPEN:
	case PTZ_CMD_IRIS_CLOSE:
	case PTZ_CMD_LEFT_UP:
	case PTZ_CMD_LEFT_DOWN:
	case PTZ_CMD_RIGHT_UP:
	case PTZ_CMD_RIGHT_DOWN:
	case PTZ_CMD_STOP:

	case PTZ_CMD_LEFT:
	case PTZ_CMD_RIGHT:
	case PTZ_CMD_UP:
	case PTZ_CMD_DOWN:
		{
			ipcNetPtz.cmdType = IPC_V3::PTZCMD_MOVE;
			ipcNetPtz.cmdPara = DDPtzToIpcPTZ(cmdtype);
			ipcRequestType = IPC_V3::CMD_REQUEST_PTZ_MOVE;
		}
		break;
	case PTZ_CMD_PRESET_SET:
		{
			ipcNetPtz.cmdType = IPC_V3::PTZCMD_PRESETSET;
			ipcNetPtz.cmdPara =  ptzParam+1;
			ipcRequestType = IPC_V3::CMD_REQUEST_PTZ_MOVE;
		}
		break;
	case PTZ_CMD_PRESET_GO:
		{
			ipcNetPtz.cmdType = IPC_V3::PTZCMD_PRESETGO;
			ipcNetPtz.cmdPara = ptzParam+1;
			ipcRequestType = IPC_V3::CMD_REQUEST_PTZ_MOVE;
		}
		break;
	//case PTZ_CMD_CRUISE_SET:
	//	{
	//		ipcNetPtz.cmdType = IPC_V3::PTZCMD_CRUISESET;
	//		ipcNetPtz.cmdPara =  ptzParam+1;
	//		ipcRequestType = IPC_V3::CMD_REQUEST_PTZ_MOVE;
	//	}
	//	break;
	case PTZ_CMD_CRUISE_DEL:
		{
			ipcNetPtz.cmdType = IPC_V3::PTZCMD_CRUISE_DEL;
			ipcNetPtz.cmdPara = ptzParam+1;
			ipcRequestType = IPC_V3::CMD_REQUEST_PTZ_MOVE;
		}
		break;
	case PTZ_CMD_CRUISE_RUN:
		{
			ipcNetPtz.cmdType = IPC_V3::PTZCMD_CRUISE_RUN;
			ipcNetPtz.cmdPara = ptzParam+1;
			ipcRequestType = IPC_V3::CMD_REQUEST_PTZ_MOVE;
		}
		break;
	case PTZ_CMD_CRUISE_STOP:
		{
			ipcNetPtz.cmdType = IPC_V3::PTZCMD_CRUISE_STOP;
			ipcNetPtz.cmdPara = ptzParam+1;
			ipcRequestType = IPC_V3::CMD_REQUEST_PTZ_MOVE;
		}
		break;
	default:
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return true;
	}

	PostCmd(ipcRequestType, (unsigned char*)(&ipcNetPtz), sizeof(IPC_V3::NET_PTZ_CTRL));

	m_pTSNetDeviceParam->netRequestLock.UnLock();
	return true;
}

bool CTSNetDevice::ColorControl(long chnn, unsigned long cmdtype, DD_VIDEO_COLOR& videoColor)
{
	/*
		视频色彩 NET_DEVICE_CTRL_COLOR
	*/

	m_pTSNetDeviceParam->netRequestLock.Lock();
	if(!WaitForReady())
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return false;
	}

	IPC_V3::NET_CHNN_COLOR ipcNetColor;
	memset(&ipcNetColor, 0, sizeof(IPC_V3::NET_CHNN_COLOR));

	switch(cmdtype)
	{
	case ND_CTRL_COLOR_GET:
		break;
	case ND_CTRL_COLOR_SET:
		{
			IPC_V3::NET_CHNN_COLOR maxColorVal;
			m_pTsNdParam->GetMaxColorValue(maxColorVal);
			IPC_V3::NET_CHNN_COLOR defaultColor;
			m_pTsNdParam->GetDefaColorValue(defaultColor);

			ipcNetColor.brightness = DVRColorToIPCColor(videoColor.brightness, DVR_DEFAULT_COLOR_BRIGHT, \
				defaultColor.brightness, maxColorVal.brightness);
			ipcNetColor.contrast = DVRColorToIPCColor(videoColor.contrast, DVR_DEFAULT_COLOR_CONTRAST, \
				defaultColor.contrast, maxColorVal.contrast);
			ipcNetColor.saturation = DVRColorToIPCColor(videoColor.saturation, DVR_DEFAULT_COLOR_SATURATION, \
				defaultColor.saturation, maxColorVal.saturation);
			ipcNetColor.hue = DVRColorToIPCColor(videoColor.hue, DVR_DEFAULT_COLOR_HUE, \
				defaultColor.hue, maxColorVal.hue);

			if(!PostCmd(IPC_V3::CMD_REQUEST_COLOR_SET, (unsigned char*)(&ipcNetColor), sizeof(IPC_V3::NET_CHNN_COLOR)))
			{
				m_pTSNetDeviceParam->netRequestLock.UnLock();
				return false;
			}

			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return true;
		}
		break;
	//case ND_CTRL_COLOR_SAVE:
	//	{
	//		ipcNetColor.brightness = videoColor.brightness;
	//		ipcNetColor.contrast = videoColor.contrast;
	//		ipcNetColor.saturation = videoColor.saturation;
	//		ipcNetColor.hue = videoColor.hue;

	//		if(!PostCmd(IPC_V3::CMD_REQUEST_COLOR_SAVE, (unsigned char*)(&ipcNetColor), sizeof(IPC_V3::NET_CHNN_COLOR)))
	//		{
	//			return false;
	//		}

	//		return true;
	//	}
	//	break;
	//case ND_CTRL_COLOR_GET_DEFAULT:
	//	{
	//		videoColor.brightness = m_pTSNetDeviceParam->m_LoginReply.defBrightness;
	//		videoColor.contrast = m_pTSNetDeviceParam->m_LoginReply.defContrast;
	//		videoColor.saturation = m_pTSNetDeviceParam->m_LoginReply.defSaturation;
	//		videoColor.hue = m_pTSNetDeviceParam->m_LoginReply.defHue;
	//		videoColor.startTime = 0;

	//		return true;
	//	}
	//	break;
	//case ND_CTRL_COLOR_SET_DEFAULT:
	//	{
	//		ipcNetColor.brightness = m_pTSNetDeviceParam->m_LoginReply.defBrightness;
	//		ipcNetColor.contrast = m_pTSNetDeviceParam->m_LoginReply.defContrast;
	//		ipcNetColor.saturation = m_pTSNetDeviceParam->m_LoginReply.defSaturation;
	//		ipcNetColor.hue = m_pTSNetDeviceParam->m_LoginReply.defHue;

	//		if(!PostCmd(IPC_V3::CMD_REQUEST_COLOR_SAVE, (unsigned char*)(&ipcNetColor), sizeof(IPC_V3::NET_CHNN_COLOR)))
	//		{
	//			return false;
	//		}

	//		return true;
	//	}
	//	break;
	default:
		break;
	}

	m_pTSNetDeviceParam->netRequestLock.UnLock();
	return false;
}
bool CTSNetDevice::OtherControl(long chnn, unsigned long cmdtype, unsigned char *pChar)
{
	m_pTSNetDeviceParam->netRequestLock.Lock();
	if(!WaitForReady())
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return false;
	}
	if ((NULL == pChar)&&(ND_CTRL_OTHER_ROI!=cmdtype)){
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return false;
	}
	if (ND_CTRL_OTHER_MIRROR == cmdtype){
		if(!PostCmd(IPC_V3::CMD_REQUEST_SET_IMAGE_MIRROR, pChar, 1))//此数据长度就 1BYTE
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
	}else if (ND_CTRL_OTHER_FLIP == cmdtype){
		if(!PostCmd(IPC_V3::CMD_REQUEST_SET_IMAGE_FILP, pChar, 1))//此数据长度就 1BYTE
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
	}else if (ND_CTRL_OTHER_ROTATO == cmdtype){
		if(!PostCmd(IPC_V3::CMD_REQUEST_SET_IMAGE_ROTATO, pChar, sizeof(unsigned long)))//此数据长度就  4BYTE
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}			
	}else if (ND_CTRL_OTHER_AUTOFOCUS == cmdtype){
		if(!PostCmd(IPC_V3::CMD_REQUEST_SET_AUTOFOCUS, pChar, sizeof(IPC_V3::AF_CONTROL_)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
	}else if (ND_CTRL_OTHER_DISTORTION == cmdtype){
		if(!PostCmd(IPC_V3::CMD_REQUEST_SET_IMAGE_DISTORTION, pChar, sizeof(IPC_V3::NCFG_LENS_DISTORTION_CONFIG)))
		{
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return false;
		}
	}else if (ND_CTRL_OTHER_ROI == cmdtype){

	}else{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return false;
	}

	/*if (0 != chnn)
	{
			m_localVideoNum = chnn;
	}*/
	m_pTSNetDeviceParam->netRequestLock.UnLock();
	return true;
}

ULONGLONG CTSNetDevice::GetMotionAlarm()
{
	ULONGLONG alarm = m_pTSNetDeviceParam->m_MotionStatus;
	return alarm;
}

ULONGLONG CTSNetDevice::GetSensorAlarm()
{
	ULONGLONG alarm = m_pTSNetDeviceParam->m_SensorStatus;
	return alarm;
}

ULONGLONG CTSNetDevice::GetVlossAlarm()
{
	ULONGLONG alarm = m_pTSNetDeviceParam->m_VideoLossStatus;
	return alarm;
}

ULONGLONG CTSNetDevice::GetOtherAlarm()
{
	ULONGLONG alarm = m_pTSNetDeviceParam->m_Other;
	return alarm;
}

long CTSNetDevice::SetConfig(long chnn, unsigned long itemID, const unsigned char* pData, unsigned long dataLen, bool bImmediately)
{
	assert(dataLen >= 0);
	assert(pData);
 	//9411不支持传感器报警
 	if ((DD_CONFIG_ITEM_SENSOR_SETUP == itemID || DD_CONFIG_ITEM_SENSOR_SCHEDULE == itemID || DD_CONFIG_ITEM_SENSOR_ALARM_OUT == itemID) 
 		&& (0 == m_pTSNetDeviceParam->m_LoginReply.sensorInNum))
 	{
 		return true;
 	}
	long ret = m_pTsCfgToIpc->SetConfig(itemID, pData, dataLen);

	if((ret == ND_ERR_OK) && bImmediately)
	{
		return FlushConfig();
	}

	return ret;
}

long CTSNetDevice::FlushConfig()
{
	m_pTSNetDeviceParam->netRequestLock.Lock();
	if(!WaitForReady())
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return -1;
	}

	unsigned long retLen = m_pTsCfgToIpc->FlushConfig();
	m_pTsCfgToIpc->ClearCfgList();							//清除先前的配置

	if (0 == retLen)
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return 0;
	}

	unsigned char* pCfgBuff = NULL;
	unsigned long ipcDataLen = 0;

	if(m_pTsCfgToIpc->GetBuff(pCfgBuff, ipcDataLen))
	{
		//设置IPC配置
		PostCmd(IPC_V3::CMD_REQUEST_CFG_SET, pCfgBuff, ipcDataLen);
	}

	if(m_pTsNdParam->IsUpdate() && m_pTsNdParam->GetBuff(pCfgBuff, ipcDataLen))
	{
		//刷新配置后, 更新参数
		PostCmd(IPC_V3::CMD_REQUEST_CFG_GET, pCfgBuff, ipcDataLen);
		m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_GET_CFG;
	}

	//如果是设置录像码流，就需要从新请求码流，使用从新连接的方式来请求
// 	if (0 <= m_pTsCfgToIpc->GetVideoStreamIndex())
// 	{
// 		if(m_pTsCfgToIpc->GetVideoStreamIndex() != m_pTSNetDeviceParam->videoStreamIndex)
// 		{
// 			m_pTSNetDeviceParam->videoStreamIndex = m_pTsCfgToIpc->GetVideoStreamIndex();
// 			m_pTSNetDeviceParam->bNeedInitNetConfig = false;
// 			m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_DISCONNECT;
// 		}
// 	}

	m_pTSNetDeviceParam->netRequestLock.UnLock();
	return 0;
}

long CTSNetDevice::SetDateTime(const DD_TIME& ddTm)
{
	m_pTSNetDeviceParam->netRequestLock.Lock();
	if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return -1;
	}

	if(!WaitForReady())
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return -1;
	}

	m_pTSNetDeviceParam->curFrameTime = 0;
	m_pTSNetDeviceParam->curSecondFrameTime = 0;
	m_pTSNetDeviceParam->curMinorFrameTime = 0;

	tm newTm;
	memset(&newTm, 0, sizeof(tm));

	newTm.tm_year = ddTm.year - 1900;
	newTm.tm_mon = ddTm.month;
	newTm.tm_mday = ddTm.mday;
	newTm.tm_hour = ddTm.hour;
	newTm.tm_min = ddTm.minute;
	newTm.tm_sec = ddTm.second;
	newTm.tm_isdst = -1;

	IPC_V3::NET_CHANGE_TIME netTime;
	memset(&netTime, 0, sizeof(IPC_V3::NET_CHANGE_TIME));
	
	netTime.llTime = TmToDVRTime(newTm);

	if(PostCmd(IPC_V3::CMD_REQUEST_CHANGE_TIME, (unsigned char*)(&netTime), sizeof(IPC_V3::NET_CHANGE_TIME)))
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return 0;
	}

	m_pTSNetDeviceParam->netRequestLock.UnLock();
	return -1;
}

long CTSNetDevice::GetDevInfo(long videoIndex, ND_DEV_INFO& ndDevInfo)
{
	m_pTsNdParam->GetDevInfo(videoIndex, ndDevInfo);
	ndDevInfo.chnn = GetBindChannel();

	return 0;
}

long CTSNetDevice::GetEncodeInfo(char * pVideoEncodeInfo, int iVideoEncodeInfoLen)
{
	int dataLen = m_pTsNdParam->GetEncodeInfo(pVideoEncodeInfo, iVideoEncodeInfoLen);

	if (0 < dataLen)
	{
		CHN_VIDEO_ENCODE_INFO * pEncodeInfo = (CHN_VIDEO_ENCODE_INFO *) pVideoEncodeInfo;
		pEncodeInfo->channel = GetBindChannel();
	}

	return dataLen;
}

void CTSNetDevice::OnTimer(unsigned long curTime)
{
	m_pTSNetDeviceParam->netRequestLock.Lock();

	if(m_pTSNetDeviceParam->m_deviceStatus == ND_STATUS_DISCONNECT)
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return;
	}

	if(!m_pTSNetDeviceParam->m_bCommInitOk)
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return;
	}

	m_pTSNetDeviceParam->timerLock.Lock();
	if(m_pTSNetDeviceParam->prevRecvTime != 0) 
	{
		unsigned long curTickCount = GetTickCount();
		if((m_pTSNetDeviceParam->prevRecvTick < curTickCount) && ((curTickCount - m_pTSNetDeviceParam->prevRecvTick) > m_pTSNetDeviceParam->netTimeout))
		{
#ifdef TEST_DISCONNECT
			if((curTickCount - m_pTSNetDeviceParam->prevPrintTick) > 2000)
			{
				printf("!**!disconnect!**! CH %d timeout : prevTime = %d, curTime = %d, prevTick = %d, curTick = %d, nodatatimes = %d\n", m_pTSNetDeviceParam->bindChannel, \
					m_pTSNetDeviceParam->prevRecvTime, GetCurrTime32(), m_pTSNetDeviceParam->prevRecvTick, curTickCount, m_pTSNetDeviceParam->recvNoDataTimes);
				PrintfBit32Time(GetCurrTime32());
				PrintfBit32Time(m_pTSNetDeviceParam->prevRecvTime);
				DVRSystem("netstat -a");
				char szCmd[64];
				sprintf(szCmd, "ping %s -w 2", m_pTSNetDeviceParam->strIP);
				DVRSystem(szCmd);
			}
#else
			m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_DISCONNECT;
#endif
			m_pTSNetDeviceParam->bNeedInitNetConfig = true;
			m_pTsCfgToIpc->SetVideoStreamIndex(-1);
			m_pTSNetDeviceParam->timerLock.UnLock();
			m_pTSNetDeviceParam->netRequestLock.UnLock();
			return;
		}
	}
	m_pTSNetDeviceParam->timerLock.UnLock();

	if(ND_SC_READY != IsReady())
	{
		m_pTSNetDeviceParam->netRequestLock.UnLock();
		return;
	}

	if(m_pTSNetDeviceParam->prevHeartBeatTime == 0)
	{
		m_pTSNetDeviceParam->prevHeartBeatTime = curTime;
	}
	else if((curTime < m_pTSNetDeviceParam->prevTimerTime) || ((curTime - m_pTSNetDeviceParam->prevHeartBeatTime) > IPC_V3::HEARTBEAT_TIMEOUT/2))
	{
		m_pTSNetDeviceParam->prevHeartBeatTime = curTime;
		//发送心跳数据，保持连接

		COMM_PACK_HEAD packHead;
		memset(&packHead, 0, sizeof(COMM_PACK_HEAD));
		memcpy(&packHead.headFlag, "1111", 4);
		unsigned long ulDataLen = sizeof(COMM_PACK_HEAD);

		//发送重试次数
		long tryTime = 0;
		long sentData = 0;
		int iRet;
		while(sentData < ulDataLen)
		{
			tryTime += 10;
			if(tryTime >= REPLY_DATA_TIME_OUT)	//心跳数据10秒钟没有发送出去，就超时断开
			{
				break;
			}

			if(m_pTSNetDeviceParam->m_deviceStatus == ND_STATUS_DISCONNECT)
			{
				m_pTSNetDeviceParam->netRequestLock.UnLock();
				return;
			}

			if(!m_pTSNetDeviceParam->m_bCommInitOk)
			{
				m_pTSNetDeviceParam->netRequestLock.UnLock();
				return;
			}

			iRet = g_pMultiNetComm->SendData(m_pTSNetDeviceParam->m_deviceID, (char *)&packHead+sentData, ulDataLen-sentData, false);
			if(iRet < 0)
			{
				break;
			}
			sentData += iRet;
			PUB_Sleep(10);
		}

		if(sentData < ulDataLen)
		{
			if(m_pTSNetDeviceParam->m_bCommInitOk)
			{
#ifdef TEST_DISCONNECT
				printf("!**!disconnect!**! CH %d send heatbeat failed, nodatatimes = %d\n", m_pTSNetDeviceParam->bindChannel, m_pTSNetDeviceParam->recvNoDataTimes);
				PrintfBit32Time(GetCurrTime32());
				PrintfBit32Time(m_pTSNetDeviceParam->prevRecvTime);
#else
				m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_DISCONNECT;
#endif
				m_pTSNetDeviceParam->bNeedInitNetConfig = true;
				m_pTsCfgToIpc->SetVideoStreamIndex(-1);
			}
		}
	}

	m_pTSNetDeviceParam->prevTimerTime = curTime;
	m_pTSNetDeviceParam->netRequestLock.UnLock();
}

void CTSNetDevice::SetNetTimeout(unsigned long msTimeout)
{
	m_pTSNetDeviceParam->netTimeout = msTimeout;
}

//数据处理成功返回0，否则返回-1，此时会进入阻塞状态
int CTSNetDevice::ReceiveCallback(long clientID, void* pParam, RECV_DATA_BUFFER *pDataBuffer)
{
	CTSNetDevice *pNetDevice = reinterpret_cast<CTSNetDevice *>(pParam);
	return pNetDevice->ProcessData(pDataBuffer);
}

//处理了数据，返回实际处理数据的长度，没有处理数据返回0
int CTSNetDevice::ProcessData(RECV_DATA_BUFFER *pDataBuffer)
{
	if(NULL != pDataBuffer)
	{
		/*判断传入参数的有效性*/
// 		if(pDataBuffer->dataSize <= 0)
// 		{
// 			assert(false);
// 			printf("!**!no data!**! \n");
// 			return 0;
// 		}

		int retLen = 0;
		char *pTempBuf = pDataBuffer->pData;
		long dataLen = pDataBuffer->dataSize;
#ifdef TEST_DISCONNECT
		static long prevDataLen[16] = {0};
#endif
		m_pTSNetDeviceParam->reBindChannelLock.Lock();

		do
		{
			if(dataLen >= sizeof(COMM_PACK_HEAD))
			{
				COMM_PACK_HEAD *pPackHead = (COMM_PACK_HEAD *)pTempBuf;

				assert(0 == memcmp(pPackHead->headFlag, "1111", 4));

				if(0 != memcmp(pPackHead->headFlag, "1111", 4))
				{
					if( 1 == m_iIpcUsedChipFlag )
					{
						// 解决智原的头出错问题
						pPackHead->headFlag[0] = 0x31;
						pPackHead->headFlag[1] = 0x31;
						pPackHead->headFlag[2] = 0x31;
						pPackHead->headFlag[3] = 0x31;
					}
					else
					{
#ifdef TEST_DISCONNECT
						printf("!**!disconnect!**! CH recved unexpected data, nodatatimes = %d\n", m_pTSNetDeviceParam->bindChannel, m_pTSNetDeviceParam->recvNoDataTimes);
						PrintfBit32Time(GetCurrTime32());
						PrintfBit32Time(m_pTSNetDeviceParam->prevRecvTime);
#endif
						int tmp;

						static char psabufdata[8]={'0'};

						if( 0 != memcmp( psabufdata, pTempBuf, 8 ) )
						{
							for( tmp = 0; tmp < 8; tmp++ )
							{
								printf( "%02x ", pTempBuf[tmp] );
							}
							printf("current frame begin data error \n");
						}
						memcpy( psabufdata, pTempBuf, 8 );
						m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_DISCONNECT;
						break;
					}
				}

				if(pPackHead->dataLen > 0)	//正常数据
				{
					if(dataLen < (sizeof(COMM_PACK_HEAD)+pPackHead->dataLen))
					{
						break;
					}

					IPC_V3::PACKCMD *pPackCmd = (IPC_V3::PACKCMD *)(pTempBuf+sizeof(COMM_PACK_HEAD));

					if (IPC_V3::CMD_REPLY_LIVE_STREAM == pPackCmd->cmdType) 
					{
						if(m_pTSNetDeviceParam->bHasFrameWaitProcess && !ProcessWaitFrame())
						{
							//m_pTSNetDeviceParam->reBindChannelLock.UnLock();
							break;
						}
						if(!ProcessFrame((unsigned char *)pTempBuf+sizeof(COMM_PACK_HEAD) + sizeof(IPC_V3::PACKCMD), pPackCmd->dataLen))
						{
							//m_pTSNetDeviceParam->reBindChannelLock.UnLock();
							break;
						}
						//m_pTSNetDeviceParam->reBindChannelLock.UnLock();
					}
					else
					{
						ProcRecvCmd(pPackCmd->cmdType, (unsigned char *)pTempBuf+sizeof(COMM_PACK_HEAD)+ sizeof(IPC_V3::PACKCMD), pPackCmd->dataLen);
					}

					pTempBuf += (sizeof(COMM_PACK_HEAD) + pPackHead->dataLen);
					dataLen -= (sizeof(COMM_PACK_HEAD) + pPackHead->dataLen);
				}
				else if(pPackHead->dataLen < 0)	//分包数据
				{
					if(dataLen < (sizeof(COMM_PACK_HEAD)+sizeof(NET_COMBINED_DATA)) )
					{
						break;
					}

					NET_COMBINED_DATA *pCombindData = (NET_COMBINED_DATA *)(pTempBuf+sizeof(COMM_PACK_HEAD));
					if(dataLen < (sizeof(COMM_PACK_HEAD)+sizeof(NET_COMBINED_DATA)+pCombindData->dwLen))
					{
						break;
					}

					//m_pTSNetDeviceParam->reBindChannelLock.Lock();
					if(m_pTSNetDeviceParam->bHasFrameWaitProcess && !ProcessWaitFrame())
					{
						//m_pTSNetDeviceParam->reBindChannelLock.UnLock();
						break;
					}

					if(!CombineFrame((unsigned char *)pTempBuf+sizeof(COMM_PACK_HEAD), sizeof(NET_COMBINED_DATA)+pCombindData->dwLen))
					{
						//m_pTSNetDeviceParam->reBindChannelLock.UnLock();
						break;
					}
					

					pTempBuf += (sizeof(COMM_PACK_HEAD)+sizeof(NET_COMBINED_DATA)+pCombindData->dwLen);
					dataLen -= (sizeof(COMM_PACK_HEAD)+sizeof(NET_COMBINED_DATA)+pCombindData->dwLen);
				}
				else	//心跳包数据，只有头
				{
					pTempBuf += sizeof(COMM_PACK_HEAD);
					dataLen -= sizeof(COMM_PACK_HEAD);
				}
			}
			else
			{
				break;
			}
		} while(true);
		
		m_pTSNetDeviceParam->reBindChannelLock.UnLock();
		
		if(pTempBuf != pDataBuffer->pData)
		{
			if(dataLen != 0)
			{
#if 0
				if((pTempBuf - pDataBuffer->pData) > dataLen)
				{
					memcpy(pDataBuffer->pData, pTempBuf, dataLen);
				}
				else
				{
					long leftLen = dataLen;
					long diffBufLen = pTempBuf - pDataBuffer->pData;
					char *pData = pDataBuffer->pData;

					while(leftLen > 0)
					{
						if(leftLen <= diffBufLen)
						{
							memcpy(pData, pTempBuf, leftLen);
							break;
						}
						else
						{
							memcpy(pData, pTempBuf, diffBufLen);
							pData += diffBufLen;
							pTempBuf += diffBufLen;
							leftLen -= diffBufLen;
						}
					}
				}
#else
				memmove(pDataBuffer->pData, pTempBuf, dataLen);
#endif
			}

			retLen = pDataBuffer->dataSize - dataLen;
			pDataBuffer->dataSize = dataLen;

			m_pTSNetDeviceParam->timerLock.Lock();
			m_pTSNetDeviceParam->prevRecvTime = GetCurrTime32();
			m_pTSNetDeviceParam->prevRecvTick = GetTickCount();
#ifdef TEST_DISCONNECT
			m_pTSNetDeviceParam->recvNoDataTimes = 0;
#endif
			m_pTSNetDeviceParam->timerLock.UnLock();
		}
#ifdef TEST_DISCONNECT
		else
		{
			if(dataLen != prevDataLen[m_pTSNetDeviceParam->bindChannel])
			{
				m_pTSNetDeviceParam->recvNoDataTimes = 0;
			}
			else
			{
				m_pTSNetDeviceParam->recvNoDataTimes++;
			}
			prevDataLen[m_pTSNetDeviceParam->bindChannel] = dataLen;
		}

		if(pDataBuffer->dataSize == pDataBuffer->bufferSize)
		{
			printf("!**!error!**! CH %d the recv buffer is full!!!!\n", m_pTSNetDeviceParam->bindChannel);
		}
#endif

		return retLen;
	}
	else
	{
		/////网络连接发生错误
		if(m_pTSNetDeviceParam->m_bCommInitOk)
		{
#ifdef TEST_DISCONNECT
			printf("!**!disconnect!**! CH %d callback says disconnect, nodatatimes = %d\n", m_pTSNetDeviceParam->bindChannel, m_pTSNetDeviceParam->recvNoDataTimes);
			PrintfBit32Time(GetCurrTime32());
			PrintfBit32Time(m_pTSNetDeviceParam->prevRecvTime);
#endif
			m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_DISCONNECT;
			m_pTSNetDeviceParam->bNeedInitNetConfig = true;
			m_pTsCfgToIpc->SetVideoStreamIndex(-1);
		}
		return 0;
	}
}

void CTSNetDevice::ProcRecvCmd(unsigned long cmdType, unsigned char *pData, unsigned long datalen)
{
	switch(cmdType)
	{
		//IPC主动发送过来的数据
	case IPC_V3::CMD_REPLY_MOTION:
		{
			assert(sizeof(IPC_V3::NET_STATUS) == datalen);
			IPC_V3::NET_STATUS *pNetStatus = (IPC_V3::NET_STATUS *)pData;
			m_pTSNetDeviceParam->m_MotionStatus = pNetStatus->status;
		}
		break;
	case IPC_V3::CMD_REPLY_SENSOR:
		{
			assert(sizeof(IPC_V3::NET_STATUS) == datalen);
			IPC_V3::NET_STATUS *pNetStatus = (IPC_V3::NET_STATUS *)pData;
			m_pTSNetDeviceParam->m_SensorStatus = pNetStatus->status;
		}
		break;
		//向IPC发送指令返回的结果
	case IPC_V3::CMD_REPLY_LONGIN_SUCC:
	case IPC_V3::CMD_REPLY_LONGIN_FAIL:
		{
			m_pTSNetDeviceParam->m_replyData.dataLen = datalen;
			if(m_pTSNetDeviceParam->m_replyData.dataLen > 0)
			{
				m_pTSNetDeviceParam->m_replyData.pData = new unsigned char[datalen];
				memcpy(m_pTSNetDeviceParam->m_replyData.pData, pData, datalen);
			}

			m_pTSNetDeviceParam->m_replyData.replyCmdID = cmdType;
			m_pTSNetDeviceParam->m_recvEvent.SetEvent();
		}
		break;
	case IPC_V3::CMD_REPLY_CHNN_CTRL_COLOR://该命令有可能是回复命令，也有可能是服务器推送的命令
		{
			//if((m_pTSNetDeviceParam->m_replyData.requestCmdID == IPC_V3::CMD_REQUEST_COLOR_GET) \
			//	|| (m_pTSNetDeviceParam->m_replyData.requestCmdID == IPC_V3::CMD_REQUEST_COLOR_SAVE))
			//{
			//	if(m_pTSNetDeviceParam->m_replyData.replyCmdID == INVALID_CMD_ID)
			//	{
			//		m_pTSNetDeviceParam->m_replyData.dataLen = datalen;

			//		m_pTSNetDeviceParam->m_replyData.pData = new unsigned char[datalen];
			//		memcpy(m_pTSNetDeviceParam->m_replyData.pData, pData, datalen);

			//		m_pTSNetDeviceParam->m_replyData.replyCmdID = cmdType;
			//		m_pTSNetDeviceParam->m_recvEvent.SetEvent();
			//	}
			//}
		}
		break;
	/*case IPC_V3::CMD_REPLY_CFG_SUCC:
		{
			//进配置成功
			if((m_pTSNetDeviceParam->m_replyData.requestCmdID == IPC_V3::CMD_REQUEST_CFG_ENTER))
			{
				m_pTSNetDeviceParam->m_replyData.dataLen = 0;

				if(m_pTSNetDeviceParam->m_ctrlStatus == ND_SC_SET_CFG)
				{
					unsigned char* pCfgBuff = NULL;
					unsigned long ipcDataLen = 0;

					if(m_pTsCfgToIpc->GetBuff(pCfgBuff, ipcDataLen))
					{
						//设置IPC配置
						PostCmd(IPC_V3::CMD_REQUEST_CFG_SET, pCfgBuff, ipcDataLen);
					}

					if(m_pTsNdParam->IsUpdate() && m_pTsNdParam->GetBuff(pCfgBuff, ipcDataLen))
					{
						//刷新配置后, 更新参数
						PostCmd(IPC_V3::CMD_REQUEST_CFG_GET, pCfgBuff, ipcDataLen);
						m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_GET_CFG;
					}
					else
					{
						PostCmd(IPC_V3::CMD_REQUEST_CFG_LEAVE, NULL, 0);
						m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_READY;
					}
				}

				m_pTSNetDeviceParam->m_replyData.replyCmdID = cmdType;
				
				m_pTSNetDeviceParam->m_recvEvent.SetEvent();

				//如果是设置录像码流，就需要从新请求码流，使用从新连接的方式来请求
				if (0 <= m_pTsCfgToIpc->GetVideoStreamIndex())
				{
					if(m_pTsCfgToIpc->GetVideoStreamIndex() != m_pTSNetDeviceParam->videoStreamIndex)
					{
						m_pTSNetDeviceParam->videoStreamIndex = m_pTsCfgToIpc->GetVideoStreamIndex();
						m_pTSNetDeviceParam->bNeedInitNetConfig = false;
						m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_DISCONNECT;
					}
				}
			}
		}
		break;
	case IPC_V3::CMD_REPLY_CFG_FAIL:
		{
			if((m_pTSNetDeviceParam->m_replyData.requestCmdID == IPC_V3::CMD_REQUEST_CFG_ENTER))
			{
				m_pTSNetDeviceParam->m_replyData.dataLen = 0;

				if(m_pTSNetDeviceParam->m_ctrlStatus == ND_SC_SET_CFG)
				{
					m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_READY;
				}
				else if(m_pTSNetDeviceParam->m_ctrlStatus == ND_SC_GET_CFG)
				{
					m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_READY;
				}

				m_pTSNetDeviceParam->m_replyData.replyCmdID = cmdType;

				m_pTSNetDeviceParam->m_recvEvent.SetEvent();
			}

			//如果是设置录像码流，就需要从新请求码流，使用从新连接的方式来请求
			if (0 <= m_pTsCfgToIpc->GetVideoStreamIndex())
			{
				if(m_pTsCfgToIpc->GetVideoStreamIndex() != m_pTSNetDeviceParam->videoStreamIndex)
				{
					m_pTSNetDeviceParam->videoStreamIndex = m_pTsCfgToIpc->GetVideoStreamIndex();
					m_pTSNetDeviceParam->bNeedInitNetConfig = false;
					m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_DISCONNECT;
				}
			}
		}
		break;*/
	case IPC_V3::CMD_REPLY_CFG_DATA:
		{
			if((m_pTSNetDeviceParam->m_replyData.requestCmdID == IPC_V3::CMD_REQUEST_CFG_GET))
			{
				m_pTSNetDeviceParam->m_replyData.dataLen = datalen;

				if(datalen > 0)
				{
					m_pTSNetDeviceParam->m_replyData.pData = new unsigned char[datalen];
					memcpy(m_pTSNetDeviceParam->m_replyData.pData, pData, datalen);
				}

				m_pTSNetDeviceParam->m_replyData.replyCmdID = cmdType;

				if(m_pTSNetDeviceParam->m_ctrlStatus == ND_SC_GET_CFG)
				{
					m_pTsNdParam->SetParamByCfg(pData, datalen);

					//PostCmd(IPC_V3::CMD_REQUEST_CFG_LEAVE, NULL, 0);
					m_pTSNetDeviceParam->m_ctrlStatus = ND_SC_READY;
				}
				m_pTSNetDeviceParam->m_recvEvent.SetEvent();
			}
		}
		break;
	default:
		break;
	}
}

#define ADJUST_FRAME_TIME	1
bool CTSNetDevice::ProcessFrame(const unsigned char *pBuf, int iLen)
{
	const IPC_V3::IPCAMERA_FRAME_INFO *pFrameInfoEx = (IPC_V3::IPCAMERA_FRAME_INFO *)pBuf;
	
	BITMAPINFOHEADER *pBitmapInfo = &m_pTSNetDeviceParam->m_videoFormat;
	CIPDataBufferMan *pDataBufferMan = m_pTSNetDeviceParam->m_pDataBufferMan;
	bool *pbNeedIFrame = &m_pTSNetDeviceParam->bNeedIFrame;
#if ADJUST_FRAME_TIME
	LONGLONG *pPrevTimeStamp = &m_pTSNetDeviceParam->prevTimeStamp;
	LONGLONG *pCurFrameTime = &m_pTSNetDeviceParam->curFrameTime;
#endif
	
 	if(pFrameInfoEx->ucStreamIndex == m_pTSNetDeviceParam->secondVideoStreamIndex)
 	{
 		pBitmapInfo = &m_pTSNetDeviceParam->m_secondVideoFormat;
 		pDataBufferMan = m_pTSNetDeviceParam->pSecondDataBufferMan;
 		pbNeedIFrame = &m_pTSNetDeviceParam->bSecondNeedIFrame;
 #if ADJUST_FRAME_TIME
 		pPrevTimeStamp = &m_pTSNetDeviceParam->prevSecondTimeStamp;
 		pCurFrameTime = &m_pTSNetDeviceParam->curSecondFrameTime;
 #endif
 	}

	if(pFrameInfoEx->ucStreamIndex == m_pTSNetDeviceParam->minorVideoStreamIndex)
	{
		pBitmapInfo = &m_pTSNetDeviceParam->m_minorVideoFormat;
		pDataBufferMan = m_pTSNetDeviceParam->pMinorDataBufferMan;
		pbNeedIFrame = &m_pTSNetDeviceParam->bMinorNeedIFrame;
#if ADJUST_FRAME_TIME
		pPrevTimeStamp = &m_pTSNetDeviceParam->prevMinorTimeStamp;
		pCurFrameTime = &m_pTSNetDeviceParam->curMinorFrameTime;
#endif
	}

	if (IPC_V3::IPCAMERA_FRAME_TYPE_PIC == pFrameInfoEx->ucFrameType)
	{
		if (m_pTSNetDeviceParam->curFrameTime)
		{
			pPrevTimeStamp = &m_pTSNetDeviceParam->prevTimeStamp;
			pCurFrameTime = &m_pTSNetDeviceParam->curFrameTime;
		}
		else
		{
			pPrevTimeStamp = &m_pTSNetDeviceParam->prevSecondTimeStamp;
			pCurFrameTime = &m_pTSNetDeviceParam->curSecondFrameTime;	
		}

		pDataBufferMan = m_pTSNetDeviceParam->m_pJpegBufferMan;
	}


	if(pFrameInfoEx->ucFrameType == IPC_V3::IPCAMERA_FRAME_TYPE_VIDEO)
	{
		//更新视频格式
		if((pBitmapInfo->biWidth != pFrameInfoEx->width) || (pBitmapInfo->biHeight != pFrameInfoEx->height))
		{
			pBitmapInfo->biWidth = pFrameInfoEx->width;
			pBitmapInfo->biHeight = pFrameInfoEx->height;

			//把视频格式放入缓冲列表
			DVR_IP_IN_FRM_INFO frameInfo;
			memset(&frameInfo, 0, sizeof(DVR_IP_IN_FRM_INFO));
			frameInfo.channel = m_pTSNetDeviceParam->bindChannel;
			frameInfo.width = pFrameInfoEx->width;
			frameInfo.height = pFrameInfoEx->height;
			frameInfo.data_type = COM_FRAME_TYPE_VIDEO_FORMAT;
			frameInfo.len = sizeof(BITMAPINFOHEADER);
			frameInfo.vir_len = 0x1000 - sizeof(DVR_IP_IN_FRM_INFO);

			while(!pDataBufferMan->PushData(frameInfo.channel, frameInfo, (unsigned char *)pBitmapInfo))
			{
				if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
				{
					break;
				}

				PUB_Sleep(10);
			}
		}
	}

	if(pFrameInfoEx->ucFrameType == IPC_V3::IPCAMERA_FRAME_TYPE_AUDIO)
	{
		//加入音频格式
		if(0 == m_pTSNetDeviceParam->m_audioFormat.nAvgBytesPerSec)
		{
			m_pTSNetDeviceParam->m_audioFormat.wFormatTag = WAVE_FORMAT_ALAW;
			m_pTSNetDeviceParam->m_audioFormat.nSamplesPerSec = 8000;
			m_pTSNetDeviceParam->m_audioFormat.nChannels = 1;
			m_pTSNetDeviceParam->m_audioFormat.wBitsPerSample = 8;
			m_pTSNetDeviceParam->m_audioFormat.nBlockAlign = 1;
			m_pTSNetDeviceParam->m_audioFormat.nAvgBytesPerSec = 8000;

			DVR_IP_IN_FRM_INFO frameInfo;
			memset(&frameInfo, 0, sizeof(DVR_IP_IN_FRM_INFO));
			frameInfo.channel = m_pTSNetDeviceParam->bindChannel;
			frameInfo.data_type = COM_FRAME_TYPE_AUDIO_FORMAT;
			frameInfo.len = sizeof(WAVEFORMATEX);
			frameInfo.vir_len = 0x1000 - sizeof(DVR_IP_IN_FRM_INFO);
			while(!m_pTSNetDeviceParam->m_pDataBufferMan->PushData(frameInfo.channel, frameInfo, (unsigned char *)&m_pTSNetDeviceParam->m_audioFormat))
			{
				if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
				{
					break;
				}

				PUB_Sleep(10);
			}

			//也同样发一份音频数据到第三码流2014.12.8  14:00
			while(!m_pTSNetDeviceParam->pMinorDataBufferMan->PushData(frameInfo.channel, frameInfo, (unsigned char *)&m_pTSNetDeviceParam->m_audioFormat))
			{
				if(m_pTSNetDeviceParam->m_deviceStatus != ND_STATUS_OK)
				{
					break;
				}

				PUB_Sleep(10);
			}
		}
	}

	long channel = m_pTSNetDeviceParam->bindChannel;
	if(*pbNeedIFrame && (pFrameInfoEx->ucFrameType == IPC_V3::IPCAMERA_FRAME_TYPE_VIDEO))
	{
		if(!pFrameInfoEx->ucKeyFrame)
		{
			return true;
		}
		else
		{
			*pbNeedIFrame = false;
		}
	}
#if ADJUST_FRAME_TIME
	LONGLONG curSysTime = GetCurrTime();
	if((pFrameInfoEx->ucFrameType == IPC_V3::IPCAMERA_FRAME_TYPE_VIDEO))
	{
		if(0 == m_pTSNetDeviceParam->ipFrameInfo.cur_time)
		{
			if(*pCurFrameTime == 0)
			{
				*pCurFrameTime = curSysTime;
			}
			else
			{
				if(*pPrevTimeStamp > pFrameInfoEx->relativeTime)	//后来的帧时间比先前帧时间还小
				{
					//printf("TIME_ERRROR diff_time:%d %s, %d\n", \
					//	(long)(*pPrevTimeStamp > pFrameInfoEx->relativeTime)/1000, \
					//	__FILE__, __LINE__);
					*pCurFrameTime += 20*1000;
				}
				else if((pFrameInfoEx->relativeTime - *pPrevTimeStamp) > MAX_KEEP_TIME*1000)	//两帧间隔大于20秒
				{
					//printf("TIME_ERRROR diff_time : %d, %s, %d\n", \
					//	(long)(pFrameInfoEx->relativeTime - *pPrevTimeStamp)/1000, \
					//	__FILE__, __LINE__);
					*pCurFrameTime = curSysTime;
				}
				else
				{
					//if((pFrameInfoEx->relativeTime - *pPrevTimeStamp) > 1000000)
					//{
					//	printf("!!!!!!!!!!!!!Lose frame, time jump : %d millsecond, %s, %d\n", \
					//		(long)(pFrameInfoEx->relativeTime - *pPrevTimeStamp)/1000, \
					//		__FILE__, __LINE__);
					//}
					*pCurFrameTime += (pFrameInfoEx->relativeTime - *pPrevTimeStamp);
				}
			}

			*pPrevTimeStamp = pFrameInfoEx->relativeTime;

			if( (curSysTime - *pCurFrameTime) > MAX_KEEP_TIME*1000)
			{
				//printf("TIME_ERRROR, curTime = %d, frameTime = %d, %s, %d\n",\
				//	(long)(curSysTime/1000000), (long)(*pCurFrameTime/1000000),\
				//	__FILE__, __LINE__);
				*pCurFrameTime = curSysTime;
			}

			if( *pCurFrameTime > curSysTime )
			{
				if((*pCurFrameTime - curSysTime) > 40000)//>40ms
				{
					//printf("TIME_ERRROR, diff_time: %dms, %s, %d\n",\
					//	(long)(*pCurFrameTime/1000 - curSysTime/1000),\
					//	__FILE__, __LINE__);
				}

				*pCurFrameTime = curSysTime;
			}
		}
	}
#endif

	if(0 == *pCurFrameTime)
	{
		memset(&m_pTSNetDeviceParam->ipFrameInfo, 0, sizeof(DVR_IP_IN_FRM_INFO));
		return true;
	}

	long dataLen = sizeof(IPC_V3::IPCAMERA_FRAME_INFO) + pFrameInfoEx->length;
	assert(iLen >= dataLen);

	m_pTSNetDeviceParam->ipFrameInfo.channel = channel | (0 << 8);
	m_pTSNetDeviceParam->ipFrameInfo.len = pFrameInfoEx->length;

	long modValue = (pFrameInfoEx->length+sizeof(DVR_IP_IN_FRM_INFO)) % 0x1000;
	m_pTSNetDeviceParam->ipFrameInfo.vir_len = pFrameInfoEx->length + ((modValue == 0) ? 0 : (0x1000 - modValue));

	if(IPC_V3::IPCAMERA_FRAME_TYPE_VIDEO == pFrameInfoEx->ucFrameType)
	{
		m_pTSNetDeviceParam->ipFrameInfo.data_type = COM_FRAME_TYPE_VIDEO;
	}
	else if(IPC_V3::IPCAMERA_FRAME_TYPE_AUDIO == pFrameInfoEx->ucFrameType)
	{ 
		m_pTSNetDeviceParam->ipFrameInfo.data_type = COM_FRAME_TYPE_AUDIO;
	}
	else if (IPC_V3::IPCAMERA_FRAME_TYPE_PIC == pFrameInfoEx->ucFrameType)
	{
		m_pTSNetDeviceParam->ipFrameInfo.data_type = COM_FRAME_TYPE_JPEG;
	}
	else
	{
		assert(false);
	}
	m_pTSNetDeviceParam->ipFrameInfo.keyframe = pFrameInfoEx->ucKeyFrame;
	m_pTSNetDeviceParam->ipFrameInfo.width = pFrameInfoEx->width;
	m_pTSNetDeviceParam->ipFrameInfo.height = pFrameInfoEx->height;
#if ADJUST_FRAME_TIME
	m_pTSNetDeviceParam->ipFrameInfo.cur_time = *pCurFrameTime;
	m_pTSNetDeviceParam->ipFrameInfo.relative_time = *pCurFrameTime;
#else
	m_pTSNetDeviceParam->ipFrameInfo.cur_time = pFrameInfoEx->time;
	m_pTSNetDeviceParam->ipFrameInfo.relative_time = pFrameInfoEx->relativeTime;
#endif

	bool bRet = true;
	//同样发送一份音频数据给主码流和第三小码流2014.12.8
	if(COM_FRAME_TYPE_AUDIO == m_pTSNetDeviceParam->ipFrameInfo.data_type)
	{
		if( ++m_iAudioFrameMaxCnt < 25 )
		{
			bRet = m_pTSNetDeviceParam->m_pDataBufferMan->PushData(channel, m_pTSNetDeviceParam->ipFrameInfo,\
				(const unsigned char *)pBuf + sizeof(IPC_V3::IPCAMERA_FRAME_INFO));

			bRet |= m_pTSNetDeviceParam->pMinorDataBufferMan->PushData(channel, m_pTSNetDeviceParam->ipFrameInfo,\
				(const unsigned char *)pBuf + sizeof(IPC_V3::IPCAMERA_FRAME_INFO));
		}
	}
	else
	{
		bRet = pDataBufferMan->PushData(channel, m_pTSNetDeviceParam->ipFrameInfo,\
			(const unsigned char *)pBuf + sizeof(IPC_V3::IPCAMERA_FRAME_INFO));
		m_iAudioFrameMaxCnt = 0;
	}

	if(bRet)
	{
#ifdef TEST_DISCONNECT
		if(m_pTSNetDeviceParam->ipFrameInfo.data_type == COM_FRAME_TYPE_VIDEO)
		{
			static unsigned long prevFrameIndex[16] = {0};
			long chnn = m_pTSNetDeviceParam->bindChannel;
 			if((pFrameInfoEx->frameIndex != 0) && (prevFrameIndex[chnn] != 0) && ((pFrameInfoEx->frameIndex - prevFrameIndex[chnn]) > 1))
 			{
#ifdef __ENVIRONMENT_WIN32__
				FILE *pFile = fopen("D:\\lostframe.txt", "r+");
				if(NULL == pFile)
				{
					pFile = fopen("D:\\lostframe.txt", "w");
				}
#else
 				FILE *pFile = fopen("/mnt/00/lostframe.txt", "r+");
 				if(NULL == pFile)
 				{
 					pFile = fopen("/mnt/00/lostframe.txt", "w");
 				}
#endif
 				if(NULL != pFile)
 				{
 					fseek(pFile, 0, SEEK_END);
 					long size = ftell(pFile);
 					if(size < 1024*1024)
 					{
						char szRemark[128] = {0};
						tm curTime = DVRTime32ToTm(GetCurrTime32());
						sprintf(szRemark, "CH %02d: time = %d-%02d-%02d %02d:%02d:%02d, lostframe = %d\n", m_pTSNetDeviceParam->bindChannel, \
							curTime.tm_year+1900, curTime.tm_mon+1, curTime.tm_mday, curTime.tm_hour, curTime.tm_min, curTime.tm_sec, \
							pFrameInfoEx->frameIndex - prevFrameIndex[chnn]);
 						fwrite(szRemark, strlen(szRemark), 1, pFile);
 					}
 					fclose(pFile);
 				}
 			}
			prevFrameIndex[chnn] = pFrameInfoEx->frameIndex;
		}
#endif
		memset(&m_pTSNetDeviceParam->ipFrameInfo, 0, sizeof(DVR_IP_IN_FRM_INFO));
	}

	return bRet;
}

//处理一个视频帧的分包数据，如果处理完成，返回true，如果处理不了返回false
bool CTSNetDevice::CombineFrame(const unsigned char *pBuf, int iLen)
{
	NET_COMBINED_DATA struCombined;
	NET_COMBINED_INFO_EX *pCombinedInfo = NULL;
	memset(&struCombined, 0, sizeof(NET_COMBINED_DATA));

	//接收组合数据包头
	memcpy(&struCombined, pBuf, sizeof(NET_COMBINED_DATA));
	long rawDataLen = struCombined.dwLen;

	if(struCombined.dwIndex == 1)
	{
		list<NET_COMBINED_INFO_EX *>::iterator it;

		IPC_V3::IPCAMERA_FRAME_INFO *pFrameInfo = (IPC_V3::IPCAMERA_FRAME_INFO *)(pBuf + sizeof(NET_COMBINED_DATA) + sizeof(IPC_V3::PACKCMD));
		//删除因服务器端丢数据而产生的没有接收完后续数据的分包
		for(it = m_pTSNetDeviceParam->m_lstCombinedInfo.begin(); it != m_pTSNetDeviceParam->m_lstCombinedInfo.end(); it++)
		{
			pCombinedInfo = *it;
			if(pCombinedInfo->streamId == pFrameInfo->ucStreamIndex)
			{
				if(pCombinedInfo->dwUsed)
				{
					pCombinedInfo->dwUsed = 0;
				}
			}
		}

		for(it = m_pTSNetDeviceParam->m_lstCombinedInfo.begin(); it != m_pTSNetDeviceParam->m_lstCombinedInfo.end(); it++)
		{
			pCombinedInfo = *it;
			if(!pCombinedInfo->dwUsed)
			{
				break;
			}
		}
		if(it == m_pTSNetDeviceParam->m_lstCombinedInfo.end())
		{
			pCombinedInfo = new NET_COMBINED_INFO_EX;
			memset(pCombinedInfo, 0, sizeof(NET_COMBINED_INFO_EX));
			pCombinedInfo->bufferLen = struCombined.dwTotalLen;
			pCombinedInfo->pBuffer = new unsigned char[pCombinedInfo->bufferLen];
			m_pTSNetDeviceParam->m_lstCombinedInfo.push_back(pCombinedInfo);
		}

		pCombinedInfo->dwUsed = 1;
		pCombinedInfo->dwDataID = struCombined.dwDataID;
		pCombinedInfo->dwTotalLen = struCombined.dwTotalLen;
		pCombinedInfo->dwTotalPack = struCombined.dwTotalPack;
		pCombinedInfo->dwLen = rawDataLen;
		pCombinedInfo->streamId = pFrameInfo->ucStreamIndex;

		if(pCombinedInfo->dwTotalLen > pCombinedInfo->bufferLen)
		{
			delete [] pCombinedInfo->pBuffer;
			pCombinedInfo->bufferLen = pCombinedInfo->dwTotalLen;
			pCombinedInfo->pBuffer = new unsigned char[pCombinedInfo->bufferLen];
		}

		memcpy(pCombinedInfo->pBuffer, pBuf+sizeof(NET_COMBINED_DATA), pCombinedInfo->dwLen);
	}
	else
	{
		list<NET_COMBINED_INFO_EX *>::iterator it;
		for(it = m_pTSNetDeviceParam->m_lstCombinedInfo.begin(); it != m_pTSNetDeviceParam->m_lstCombinedInfo.end(); it++)
		{
			pCombinedInfo = *it;
			if(pCombinedInfo->dwDataID == struCombined.dwDataID)
			{
				break;
			}
		}
		if(it == m_pTSNetDeviceParam->m_lstCombinedInfo.end())
		{
			assert(false);
			return true;
		}

		//不是正在接收的那个组合数据，这种情况在程序写正确的情况下是不会发生的
		if ((pCombinedInfo->dwTotalLen != struCombined.dwTotalLen)
			|| (pCombinedInfo->dwTotalPack != struCombined.dwTotalPack)) 
		{
			assert(false);
			pCombinedInfo->dwUsed = 0;
			return true;
		}

		assert((pCombinedInfo->dwLen + rawDataLen) <= pCombinedInfo->dwTotalLen);

		memcpy(pCombinedInfo->pBuffer + pCombinedInfo->dwLen, pBuf + sizeof(NET_COMBINED_DATA), rawDataLen);
		pCombinedInfo->dwLen += rawDataLen;

		//如果是最后一个包
		if (struCombined.dwTotalPack == struCombined.dwIndex)
		{
			assert(pCombinedInfo->dwLen == pCombinedInfo->dwTotalLen);
			IPC_V3::PACKCMD *pCmd = (IPC_V3::PACKCMD *)(pCombinedInfo->pBuffer);
			assert(IPC_V3::CMD_REPLY_LIVE_STREAM == pCmd->cmdType);

			//虽然没有加进去，不过这一包数据已经保存在pCombindInfo中了，所以对回调函数而言，数据已经处理完成了
			if(!ProcessFrame(pCombinedInfo->pBuffer+sizeof(IPC_V3::PACKCMD), pCmd->dataLen))
			{
				m_pTSNetDeviceParam->bHasFrameWaitProcess = true;
				return true;
			}
			else
			{
				pCombinedInfo->dwUsed = 0;
			}
		}
	}

	return true;
}

bool CTSNetDevice::ProcessWaitFrame()
{
	list<NET_COMBINED_INFO_EX *>::iterator it;
	NET_COMBINED_INFO_EX *pCombinedInfo;
	for(it = m_pTSNetDeviceParam->m_lstCombinedInfo.begin(); it != m_pTSNetDeviceParam->m_lstCombinedInfo.end(); it++)
	{
		pCombinedInfo = *it;
		if(pCombinedInfo->dwUsed && (pCombinedInfo->dwTotalLen == pCombinedInfo->dwLen))
		{
			if(ProcessFrame(pCombinedInfo->pBuffer+sizeof(IPC_V3::PACKCMD), pCombinedInfo->dwLen-sizeof(IPC_V3::PACKCMD)))
			{
				m_pTSNetDeviceParam->bHasFrameWaitProcess = false;
				pCombinedInfo->dwUsed = 0;
				return true;
			}
			else
			{
				return false;
			}
			break;
		}
	}

	assert(false);
	return true;
}

//等待接收网络请求的回复信息，信号每次等待50ms，总共等待REPLY_DATA_TIME_OUT
//
bool CTSNetDevice::WaitReplyData()
{
	long waitedTime = 0;
	while(true)
	{
		if(waitedTime > REPLY_DATA_TIME_OUT)
		{
			return false;
		}

		m_pTSNetDeviceParam->m_recvEvent.WaitForCond(50);

		waitedTime += 50;

		if(m_pTSNetDeviceParam->m_deviceStatus == ND_STATUS_DISCONNECT)
		{
			return false;
		}

		if(m_pTSNetDeviceParam->m_replyData.replyCmdID != INVALID_CMD_ID)
		{
			return true;
		}
	}
}

bool CTSNetDevice::PostCmd(unsigned long iCmdType, unsigned char* pData, unsigned long dataLen)
{
	if(m_pTSNetDeviceParam->m_deviceStatus == ND_STATUS_DISCONNECT)
	{
		return false;
	}

	char *pSendBuff = NULL;
	unsigned long   ulDataLen = 0;

	ulDataLen = sizeof(COMM_PACK_HEAD) + sizeof(IPC_V3::PACKCMD) + dataLen;	
	pSendBuff = new char[ulDataLen];
	memset(pSendBuff, 0, ulDataLen);

	COMM_PACK_HEAD *pPackHead = (COMM_PACK_HEAD *)pSendBuff;
	memcpy(pPackHead->headFlag, "1111", 4);
	pPackHead->dataLen = sizeof(IPC_V3::PACKCMD) + dataLen;
	/*填充请求命令头*/
	IPC_V3::PACKCMD * pCmdPack = (IPC_V3::PACKCMD *)(pSendBuff + sizeof(COMM_PACK_HEAD));
	pCmdPack->cmdType = iCmdType;
	pCmdPack->dataLen = dataLen;

	/*填充数据*/
	if(dataLen > 0)
	{
		memcpy(pSendBuff + sizeof(COMM_PACK_HEAD) + sizeof(IPC_V3::PACKCMD), pData, dataLen);
	}

	/*发送请求命令*/	
	m_pTSNetDeviceParam->m_replyData.requestCmdID = iCmdType;
	m_pTSNetDeviceParam->m_replyData.replyCmdID = INVALID_CMD_ID;
	m_pTSNetDeviceParam->m_replyData.dataLen = 0;
	if(m_pTSNetDeviceParam->m_replyData.pData)
	{
		//* 回复数据统一在这里释放, [ProcRecvCmd函数中申请], 过程中只读即可
		delete [] m_pTSNetDeviceParam->m_replyData.pData;
		m_pTSNetDeviceParam->m_replyData.pData = NULL;
	}

	//发送重试次数
	long tryTime = 0;
	long sentData = 0;
	int iRet;
	while(sentData < ulDataLen)
	{
		tryTime += 10;
		if(tryTime >= SOCK_TIME_OUT)
		{
			break;
		}

		if(m_pTSNetDeviceParam->m_deviceStatus == ND_STATUS_DISCONNECT)
		{
			delete [] pSendBuff;
			return false;
		}

		iRet = g_pMultiNetComm->SendData(m_pTSNetDeviceParam->m_deviceID, pSendBuff+sentData, ulDataLen-sentData, false);
		if(iRet < 0)
		{
			break;
		}

		sentData += iRet;

		PUB_Sleep(10);
	}
	
	if(pSendBuff)
	{
		delete [] pSendBuff;
		pSendBuff = NULL;
	}
	
	if(sentData != ulDataLen)
	{
		if(m_pTSNetDeviceParam->m_bCommInitOk)
		{
#ifdef TEST_DISCONNECT
			printf("!**!disconnect!**! CH %d send cmd data falied, nodatatimes = %d\n", m_pTSNetDeviceParam->bindChannel, m_pTSNetDeviceParam->recvNoDataTimes);
			PrintfBit32Time(GetCurrTime32());
			PrintfBit32Time(m_pTSNetDeviceParam->prevRecvTime);
#endif
			m_pTSNetDeviceParam->m_deviceStatus = ND_STATUS_DISCONNECT;
			m_pTSNetDeviceParam->bNeedInitNetConfig = true;
			m_pTsCfgToIpc->SetVideoStreamIndex(-1);
		}
		return false;
	}

	return true;
}

bool CTSNetDevice::SendCmd(unsigned long iCmdType, unsigned char* pData, unsigned long dataLen)
{
	if(PostCmd(iCmdType, pData, dataLen))
	{
		return WaitReplyData();
	}

	return false;
}

void CTSNetDevice::EncryptLoginInfo(void *pLoginInfo)
{
	if(0 == m_pTSNetDeviceParam->encryptType)
	{
		return;
	}
	else if(0x00010000 == m_pTSNetDeviceParam->encryptType)
	{
		IPC_V3::NET_LOGIN_INFO *pInfo = (IPC_V3::NET_LOGIN_INFO *)pLoginInfo;
		unsigned long *pUserName = (unsigned long *)pInfo->username;
		unsigned long *pPassword = (unsigned long *)pInfo->password;

		for(int i = 0; i < sizeof(pInfo->username)/sizeof(long); i++)
		{
			pUserName[i] ^= m_pTSNetDeviceParam->encryptParam;
			pPassword[i] ^= m_pTSNetDeviceParam->encryptParam;
		}
	}
	else
	{
		assert(false);
	}
}

bool CTSNetDevice::SetInitConfig()
{
	unsigned long itemNum = 1;
	
#ifdef __GB28181_SERVER__
	itemNum = 2;
#else
	if (m_bUseBaseLine)
	{
		itemNum = 2;
	}
#endif
	unsigned long streamNum = m_pTsNdParam->GetStreamNum();
	long cfgLen = sizeof(IPC_V3::NCFG_BLOCK_HEAD) + sizeof(IPC_V3::NCFG_ITEM_HEAD) * itemNum;
	cfgLen += sizeof(unsigned long) * streamNum;
#ifdef __GB28181_SERVER__
	cfgLen += sizeof(unsigned long) * streamNum;
#else
	if (m_bUseBaseLine)
	{
		cfgLen += sizeof(unsigned long) * streamNum;
	}
#endif

	unsigned char *pData = new unsigned char[cfgLen];

	IPC_V3::NCFG_BLOCK_HEAD* pBlockHead = reinterpret_cast<IPC_V3::NCFG_BLOCK_HEAD*>(pData);
	IPC_V3::NCFG_ITEM_HEAD* pItemHead = reinterpret_cast<IPC_V3::NCFG_ITEM_HEAD*>(pData + sizeof(IPC_V3::NCFG_BLOCK_HEAD));

	pBlockHead->biSize = sizeof(IPC_V3::NCFG_BLOCK_HEAD);
	pBlockHead->ItemNum = itemNum;
	pBlockHead->netcfgver = IPC_V3::NET_PROTOCOL_VER;

	pItemHead->itemID = IPC_V3::NCFG_ITEM_ENCODE_I_FRAME_INTERVAL;
	pItemHead->num = streamNum;
	pItemHead->subLen = sizeof(unsigned long);
	pItemHead->len = sizeof(unsigned long)*streamNum;
	pItemHead++;

#ifdef __GB28181_SERVER__
	pItemHead->itemID = IPC_V3::NCFG_ITEM_ENCODE_ENCODE_LEVEL;
	pItemHead->num = streamNum;
	pItemHead->subLen = sizeof(unsigned long);
	pItemHead->len = sizeof(unsigned long)*streamNum;
#else
	if (m_bUseBaseLine)
	{
		pItemHead->itemID = IPC_V3::NCFG_ITEM_ENCODE_ENCODE_LEVEL;
		pItemHead->num = streamNum;
		pItemHead->subLen = sizeof(unsigned long);
		pItemHead->len = sizeof(unsigned long)*streamNum;
	}
#endif

	unsigned long offset = sizeof(IPC_V3::NCFG_BLOCK_HEAD) + sizeof(IPC_V3::NCFG_ITEM_HEAD) * itemNum;

	unsigned long *pIFrameInterval = reinterpret_cast<unsigned long *>(pData+offset);
	offset += sizeof(unsigned long) * streamNum;

	unsigned long *pEncodeLevel = NULL;
#ifdef __GB28181_SERVER__
	pEncodeLevel = reinterpret_cast<unsigned long *>(pData+offset);
#else
	if (m_bUseBaseLine)
	{
		pEncodeLevel = reinterpret_cast<unsigned long *>(pData+offset);
	}
#endif
	for(unsigned long i = 0; i < streamNum; i++)
	{
		pIFrameInterval[i] = 60;
#ifdef __GB28181_SERVER__
		pEncodeLevel[i] = IPC_V3::NCFG_ENCODE_LEVEL_BASELINE;
#else
		if (m_bUseBaseLine)
		{
			pEncodeLevel[i] = IPC_V3::NCFG_ENCODE_LEVEL_BASELINE;
		}
#endif
		if (m_bUseBaseLine)
		{
			pEncodeLevel[i] = IPC_V3::NCFG_ENCODE_LEVEL_BASELINE;
		}
	}

	PostCmd(IPC_V3::CMD_REQUEST_CFG_SET, pData, cfgLen);

	delete [] pData;

	return true;
}

unsigned long CTSNetDevice::DDPtzToIpcPTZ(unsigned long ddPtzCmd)
{
	switch(ddPtzCmd)
	{
	case PTZ_CMD_NEAR:
		{
			return IPC_V3::PTZCOMMAND_NEAR;
		}
	case PTZ_CMD_FAR:
		{
			return IPC_V3::PTZCOMMAND_FAR;
		}
	case PTZ_CMD_ZOOM_OUT:
		{
			return IPC_V3::PTZCOMMAND_ZOOMOUT;
		}
	case PTZ_CMD_ZOOM_IN:
		{
			return IPC_V3::PTZCOMMAND_ZOOMIN;
		}
	case PTZ_CMD_IRIS_OPEN:
		{
			return IPC_V3::PTZCOMMAND_IRISOPEN;
		}
	case PTZ_CMD_IRIS_CLOSE:
		{
			return IPC_V3::PTZCOMMAND_IRISCLOSE;
		}
	case PTZ_CMD_STOP:
		{
			return IPC_V3::PTZCOMMAND_STOP;
		}
	case PTZ_CMD_LEFT:
		{
			return IPC_V3::PTZCOMMAND_LEFT;
		}
	case PTZ_CMD_RIGHT:
		{
			return IPC_V3::PTZCOMMAND_RIGHT;
		}
	case PTZ_CMD_UP:
		{
			return IPC_V3::PTZCOMMAND_UP;
		}
	case PTZ_CMD_DOWN:
		{
			return IPC_V3::PTZCOMMAND_DOWN;
		}
	case PTZ_CMD_LEFT_UP:
		{
			return IPC_V3::PTZCOMMAND_LEFT_UP;
		}
	case PTZ_CMD_LEFT_DOWN:
		{
			return IPC_V3::PTZCOMMAND_LEFT_DOWN;
		}
	case PTZ_CMD_RIGHT_UP:
		{
			return IPC_V3::PTZCOMMAND_RIGHT_UP;
		}
	case PTZ_CMD_RIGHT_DOWN:
		{
			return IPC_V3::PTZCOMMAND_RIGHT_DOWN;
		}
	default:
		return IPC_V3::PTZCOMMAND_STOP;
	}
}

bool CTSNetDevice::WaitForReady()
{
	long waitedTime = 0;
	while(waitedTime < REPLY_DATA_TIME_OUT)
	{
		if(m_pTSNetDeviceParam->m_deviceStatus == ND_STATUS_DISCONNECT)
		{
			return false;
		}

		if(m_pTSNetDeviceParam->m_ctrlStatus == ND_SC_READY)
		{
			return true;
		}

		PUB_Sleep(50);
		waitedTime += 50;
	}

	return false;
}

unsigned long CTSNetDevice::GetDevResolutionVal( int* width, int* height )
{
	if( m_pTsCfgToIpc )
	{
		int resol = m_pTsCfgToIpc->GetIpcResolution();
		if( DD_VIDEO_SIZE_3M == resol )
		{
			*width = 2048;
			*height = 1536;
		}
		else if (DD_VIDEO_SIZE_1080P == resol)
		{
			*width = 1920;
			*height = 1080;
		}
		else if( DD_VIDEO_SIZE_16_9_3M == resol )
		{
			*width = 2304;
			*height = 1296;
		}
		else if( DD_VIDEO_SIZE_4M == resol )
		{
			*width = 2688;
			*height = 1520;
		}
		else if( DD_VIDEO_SIZE_5M == resol )
		{
			*width = 2592;
			*height = 1944;
		}
		else if (DD_VIDEO_SIZE_960P == resol)
		{
			*width = 1280;
			*height = 960;
		}
		else if (DD_VIDEO_SIZE_SXGA == resol)
		{
			*width = 1280;
			*height = 1024;
		}
		else
		{
			*width = 1280;
			*height = 720;
		}
	}
	else
	{
		*width = 0;
		*height = 0;
	}
	return 0;
}

//end
