#include "APIStreamDispatch.h"
#include "NetInterface.h"

//

CAPIStreamDispatch::CAPIStreamDispatch(void)
{
	m_bDispatchStreamRun = false;
	m_hDispatchStreamThread = PUB_THREAD_ID_NOINIT;
	m_pMsgDistributor = NULL;
	m_pTvtDev = NULL;
	m_pStreamCtrl = NULL;
	m_bInit = false;
}

CAPIStreamDispatch::~CAPIStreamDispatch(void)
{

}

CAPIStreamDispatch* CAPIStreamDispatch::Instance()
{
	static CAPIStreamDispatch sObj;
	return &sObj;
}

bool CAPIStreamDispatch::Initial()
{
	if (!m_pTvtDev)
	{
		m_pTvtDev = new CTvtDevInterface();
	}
	
	if (!m_pStreamCtrl)
	{
		m_pStreamCtrl = new CTvtRtspStreamCtrl;
	}

	if (!m_pMsgDistributor)
	{
		m_pMsgDistributor = new CAPIMsgDistributor;
	}
	m_pMsgDistributor->AddReceiver(m_pTvtDev);
	m_pMsgDistributor->AddReceiver(m_pStreamCtrl);

	CProduct::Instance()->GetProductInfo(m_Productinfo, "", "", "", "");
	int maxChannel = m_Productinfo.localVideoInputNum + m_Productinfo.netVideoInputNum;

	NET_SERVER_Initial(maxChannel, m_Productinfo.relayOutputNum, m_Productinfo.sensorInputNum ,m_pTvtDev, m_pStreamCtrl, (PROTOCOL_TVT_API | PROTOCOL_ONVIF), NETNS::DEVICE_DVR);

	m_ClientID = CMessageMan::Instance()->AllocNetClientID();
	m_pTvtDev->SetClientID(m_ClientID);
	m_pStreamCtrl->SetClientID(m_ClientID);

	m_pTvtDev->SetMaxChnnel( maxChannel );

	m_bInit = true;

	return true;
}

void CAPIStreamDispatch::Quit()
{
	if (m_pTvtDev)
	{
		delete m_pTvtDev;
	}
}

bool CAPIStreamDispatch::Start()
{
	if (!m_bInit)
	{
		return false;
	}

	if(m_bDispatchStreamRun)
	{
		return true;
	}

	m_hDispatchStreamThread = PUB_CreateThread(DispatchStreamThread, this, &m_bDispatchStreamRun);
	if(PUB_CREATE_THREAD_FAIL == m_hDispatchStreamThread)
	{
		m_bDispatchStreamRun = false;
		return false;
	}
	
	int rtspPort = 554;
#ifndef WIN32
	NET_SERVER_Start(80, "./WebServer", "./apiConf.xml");//TODO
#else
	NET_SERVER_Start(80, "../WebSites", "./apiConf.xml");//TODO
#endif
	NET_SERVER_RTSPStart( rtspPort , m_pStreamCtrl, m_pTvtDev);

	int maxChannel = m_Productinfo.localVideoInputNum + m_Productinfo.netVideoInputNum;
	char url[256] = {0};
	char ip[64] = "192.168.3.126";	
#ifndef WIN32
	unsigned long IP = 0;
	CNetInterface::Instance()->GetIPAddress(IP);
	struct in_addr in;
	in.s_addr = IP;
	strncpy(ip, inet_ntoa(in), sizeof(ip));
#endif
	for (int i=0; i<maxChannel; i++)
	{
		memset( url, 0, 256 );
 		sprintf(url, "chID=%d&streamType=main&linkType=tcp", i);
 		NET_SERVER_SetLiveRTSPUrl( i, LIVE_FRAME_FIRST_STREAM, url);
		printf("main : -------- %s\n", url);

		memset( url, 0, 256 );
		sprintf(url, "chID=%d&streamType=sub&linkType=tcp", i);
		NET_SERVER_SetLiveRTSPUrl( i, LIVE_FRAME_SECOND_STREAM, url);
		printf("sub : -------- %s\n", url);

	}

	return true;
}

bool CAPIStreamDispatch::Restart( unsigned short httpPort, unsigned short rtspPort )
{

	return true;
}

void CAPIStreamDispatch::Stop()
{
	if(m_bDispatchStreamRun)
	{
		PUB_ExitThread(&m_hDispatchStreamThread, &m_bDispatchStreamRun);
	}
}

PUB_THREAD_RESULT PUB_THREAD_CALL CAPIStreamDispatch::DispatchStreamThread(void *pParam)
{
	CAPIStreamDispatch *pThis = (CAPIStreamDispatch *)pParam;
	pThis->DispatchStreamProc();

#ifndef WIN32
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	return 0;
}

void CAPIStreamDispatch::DispatchStreamProc()
{
	CMessageMan *pMessageMan = CMessageMan::Instance();
	long streams[4] = {LIVE_FRAME_FIRST_STREAM, LIVE_FRAME_SECOND_STREAM, LIVE_FRAME_TALK, LIVE_FRAME_JPEG};

	CFrameData *pFrameData;
	CFrameData *pPBFrameData;
	FRAME_INFO_EX frameInfo;
	NETNS::RTSP_FRAME frame;
	memset( &frame, 0, sizeof(frame));
	unsigned short ch = 0;

	bool bRequest = false;

	while(m_bDispatchStreamRun)
	{
		while(pMessageMan->GetPBFrame(&pPBFrameData, API_SERVER))
		{
			frameInfo = pPBFrameData->GetFrameInfo();
			if (frameInfo.frameType == FRAME_TYPE_VIDEO)
			{
				bRequest = true;
				if (PackFrame(frame, frameInfo, false))
				{
					NET_SERVER_PlayStreamIn(frameInfo.streamID, frame, frameInfo.pData);
					RecordPbFrame(frameInfo.streamID, frameInfo.frameIndex, frameInfo.relativeTime);
					//fwrite(frameInfo.pData, 1, frameInfo.length, pFile);
				}
			}
			else if(frameInfo.frameType == FRAME_TYPE_VIDEO_FORMAT)
			{
				printf("FRAME_TYPE_VIDEO_FORMAT %lu\n", frameInfo.streamID);
				pPBFrameData->Dec();
				continue;
			}
			else if(frameInfo.frameType == FRAME_TYPE_END)
			{
				printf("FRAME_TYPE_END %lu\n", frameInfo.streamID);
				PbEnd(frameInfo.streamID);
			}

			pPBFrameData->Dec();
		}
		PBFrameFeedback();

		for(int i = 0; i < 4; i++)
		{
			while(pMessageMan->GetFrame(&pFrameData, streams[i], API_SERVER))
			{
				frameInfo = pFrameData->GetFrameInfo();
				if (PackFrame(frame, frameInfo))
				{
					NET_SERVER_LiveStreamIn(frameInfo.channel, streams[i], frame, frameInfo.pData);
				}
				
				pFrameData->Dec();

			}
		}

		PUB_Sleep(10);
	}
}


bool CAPIStreamDispatch::PackFrame( NETNS::RTSP_FRAME &frame, FRAME_INFO_EX &frameInfo, bool bUseRelativeTime )
{
	do 
	{
		if (frameInfo.keyFrame)
		{
			frame.bKeyFrame = true;
		}
		else
		{
			frame.bKeyFrame = false;
		}

		if (frameInfo.frameType == FRAME_TYPE_VIDEO)
		{
			frame.frameType = NETNS::RTSP_FRMAE_TYPE_VIDEO;
			frame.dwHz = 90000;
			frame.encodeType.videoEnc = NETNS::RTSP_VIDEO_ENCODE_TYPE_H264;

			frame.dwWidth = frameInfo.width;
			frame.dwHeight = frameInfo.height;
		}
		else if(frameInfo.frameType == FRAME_TYPE_AUDIO)
		{
			frame.frameType = NETNS::RTSP_FRMAE_TYPE_AUDIO;
			frame.dwHz = 8000;
			frame.encodeType.audioEnc = NETNS::RTSP_AUDIO_ENCODE_TYPE_G711;
		}
		else if(frameInfo.frameType == FRAME_TYPE_JPEG)
		{
			frame.frameType = NETNS::RTSP_FRMAE_TYPE_PIC;
			//frame.encodeType.picEnc = RTSP_PIC_ENCODE_TYPE_BMP  //JPEG
		}
		else
		{
			break;
		}

		if (bUseRelativeTime)
		{
			frame.ullFrametime = frameInfo.relativeTime;
		}
		else
		{
			frame.ullFrametime = frameInfo.time;
		}
		frame.dwFrameLen = frameInfo.length;

		return true;
	} while(0);

	return false;
}

void CAPIStreamDispatch::PBFrameFeedback()
{
	m_PbLock.Lock();
	std::map<int , PBFrameFb>::iterator it = m_PbCtrl.begin();
	unsigned long streamID = 0;
	for (; it!=m_PbCtrl.end(); ++it)
	{
		streamID = it->first;

		unsigned long currTick = GetTickCount();
		unsigned long timeDiff = (currTick - m_PbCtrl[streamID].lastFbTick) * 1000;
		unsigned long timeCmp = (unsigned long)(m_PbCtrl[streamID].scale * timeDiff / m_PbCtrl[streamID].frameTimeDiff);
		if (m_PbCtrl[streamID].frameCount > 0 && timeCmp > m_PbCtrl[streamID].frameCount)
		{
			m_PbCtrl[streamID].frameCount = 0;
			m_PbCtrl[streamID].lastFbTick = currTick;
			
			SendPbFeedback(streamID, m_PbCtrl[streamID].lastFrameIndex);
		}
	}

	m_PbLock.UnLock();
}

bool CAPIStreamDispatch::StartPlayBack( unsigned long streamID, float scale )
{
	bool ret = false;
	m_PbLock.Lock();
	if (m_PbCtrl.find(streamID) == m_PbCtrl.end())
	{
		PBFrameFb frameFb;
		frameFb.lastFbTick = GetTickCount();
		frameFb.scale = scale;
		frameFb.frameCount = 0;
		frameFb.lastFrameTime = 0;
		frameFb.currFrameTime = 0;
		frameFb.frameTimeDiff = 40000;
		m_PbCtrl.insert(std::pair<int, PBFrameFb>(streamID, frameFb));
		ret = true;
	}
	m_PbLock.UnLock();

	return ret;
}

void CAPIStreamDispatch::SendPbFeedback( unsigned long streamID, unsigned long frameIndex )
{
	PACKCMD				*PackCmd;
	NET_DATA_SEARCH		*NetDataSerch;
	MESSAGE_DATA		MsgData;

	//回放控制
	MsgData.clientID = m_ClientID;
	MsgData.deviceID = 0;
	MsgData.dataLen = sizeof(PACKCMD) + sizeof(NET_DATA_SEARCH);
	MsgData.pData = new char[MsgData.dataLen];
	assert(MsgData.pData != NULL);
	memset(MsgData.pData, 0, MsgData.dataLen);

	PackCmd = (PACKCMD *)MsgData.pData;
	NetDataSerch = reinterpret_cast<NET_DATA_SEARCH *>(MsgData.pData + sizeof(PACKCMD));

	//回放请求
	PackCmd->cmdType = CMD_REQUEST_DATA_INDEX;
	PackCmd->cmdID   = 0;
	PackCmd->cmdVer = NET_PROTOCOL_VER;
	PackCmd->dataLen = sizeof(NET_DATA_SEARCH);
	memset(NetDataSerch, 0, sizeof(NET_DATA_SEARCH));
	NetDataSerch->dataType = frameIndex;
	NetDataSerch->streamID = streamID;

	CMessageMan::Instance()->PutMsgToServer(MsgData, API_SERVER);
}

void CAPIStreamDispatch::PbEnd( unsigned long streamID )
{
	m_PbLock.Lock();
	if (m_PbCtrl.find(streamID) != m_PbCtrl.end())
	{
		m_PbCtrl.erase(streamID);
	}
	m_PbLock.UnLock();
	m_pStreamCtrl->ClosePlayStream(streamID);
}

void CAPIStreamDispatch::RecordPbFrame( unsigned long streamID, unsigned long frameIndex, LONGLONG frameTime )
{
	m_PbLock.Lock();
	if (m_PbCtrl.find(streamID) != m_PbCtrl.end())
	{
		m_PbCtrl[streamID].frameCount++;
		m_PbCtrl[streamID].frameTimeDiff = m_PbCtrl[streamID].currFrameTime - m_PbCtrl[streamID].lastFrameTime;
		m_PbCtrl[streamID].lastFrameTime = m_PbCtrl[streamID].currFrameTime;
		m_PbCtrl[streamID].currFrameTime = frameTime;
		if (m_PbCtrl[streamID].frameTimeDiff > 4000000 || m_PbCtrl[streamID].frameTimeDiff == 0)//4s
		{
			m_PbCtrl[streamID].frameTimeDiff = 40000;//40ms
		}
		m_PbCtrl[streamID].lastFrameIndex = frameIndex;
	}
	m_PbLock.UnLock();
}
