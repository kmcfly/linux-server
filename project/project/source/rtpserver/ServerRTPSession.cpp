#include "ServerRTPSession.h"

#ifdef _DEBUG
#define  new DEBUG_NEW
#endif


CServerRTPSession::CServerRTPSession(struct sockaddr_in clientAddr, SWL_socket_t rtspSockfd)
{
	Initial();
	m_pRTSPSock = new CRTPSock;
	if (NULL == m_pRTSPSock)
	{
		assert(false);
	}
	m_pRTSPSock->SetSockfd(clientAddr, rtspSockfd, IPPROTO_TCP);
	m_pRTSPSock->SetIP(clientAddr.sin_addr.s_addr);
	
	m_pRecvBuf = new unsigned char[MAX_RTSP_MSG_LEN];
	m_pSendBuf = new unsigned char[MAX_RTSP_MSG_LEN];
	if (NULL == m_pRecvBuf || NULL == m_pSendBuf)
	{
		assert(false);
	}
}

CServerRTPSession::CServerRTPSession(RTPMediaInfo *pMediaInfo)
{
	Initial();
	unsigned short port[2];
	port[0] = pMediaInfo->RemoteRTPPort;
	port[1] = pMediaInfo->RemoteRTCPPort;
	CreateRTPPort(pMediaInfo->LocalRTPPort, port, pMediaInfo->RemoteIp, 2);
	m_bIsStartTransPacket = true;
	m_Channel = pMediaInfo->Channel;
	m_SessionID = pMediaInfo->SessionID;
	m_ClientID = pMediaInfo->ClientID;
	m_RandSeed = pMediaInfo->SSRC;
	if (pMediaInfo->StreamType == 1 || pMediaInfo->StreamType == 2)
	{
		m_bIsPlayBack = true;
		m_Channel += MAX_CH_NUM;
	}
	m_MessageID = pMediaInfo->MessageID;
	m_pSipCallBack = pMediaInfo->pSipCallBack;

}

CServerRTPSession::~CServerRTPSession()
{
	printf("~CServerRTPSession\n");
	if (NULL != m_pRTSP)
	{
		delete m_pRTSP;
		m_pRTSP = NULL;
	}
	if (NULL != m_pRecvBuf)
	{
		delete m_pRecvBuf;
		m_pRecvBuf = NULL;
	}
	if (NULL != m_pSendBuf)
	{
		delete m_pSendBuf;
		m_pSendBuf = NULL;
	}

	for (int i=0; i<MAX_RTSP_MEDIA_COUNT*2; i++)
	{
		if (m_pUDPSock[i] != NULL)
		{
			m_pUDPSock[i]->CloseSock();
			delete m_pUDPSock[i];
			m_pUDPSock[i] = NULL;
		}
	}
	for (int i=0; i<MAX_RTSP_MEDIA_COUNT; i++)
	{
		if (m_pSR[i] != NULL)
		{
			delete m_pSR[i];
			m_pSR[i] = NULL;
		}
	}
	if (NULL != m_pRTSPSock)
	{
		m_pRTSPSock->CloseSock();
		delete m_pRTSPSock;
		m_pRTSPSock = NULL;
	}

}

void CServerRTPSession::Initial()
{
	m_SendLen = 0;
	m_RecvLen = 0;
	m_bRTSPWaitSend = false;
	m_bIsStartTransPacket = false;
	m_bIsDiscardEnd = true;

	m_RandSeed = (unsigned long)rand();

	m_SendWaitTimes = 0;

	m_FrameRate = 0;
	m_bIsUDP = false;
	for (int i=0; i<MAX_RTSP_MEDIA_COUNT ; i++)
	{
		m_pSR[i] = NULL;
		m_LastPacketTimeStamp[i] = 0;
		m_LastNTPSec[i] = 0;
		m_LastNTPMicroSec[i] = 0;
		m_pUDPSock[i*2] = NULL;
		m_pUDPSock[i*2+1] = NULL;
		m_SenderOctetCount[i] = 0;
		m_SenderPacketCount[i] = 0;
	}
	m_pRTSPSock = NULL;
	m_pSendBuf = NULL;
	m_pRecvBuf = NULL;
	m_pRTSP = NULL;
	m_bIsPlayBack = false;
	m_pSipCallBack = NULL;
	m_AudioType = -1;
	m_MessageID = 0;

	m_bPause = false;
	m_LastTime = 0;
	m_bCHeckMark = false;
	m_Scale = 1;
}

int CServerRTPSession::Quit()
{
	//服务器需要发送BYE吗？

	m_Lock.Lock();
	int key;
	std::list<CRTPPacket*>::iterator listIter;
	std::map<unsigned short, std::list<CRTPPacket*>*>::iterator iter = m_mapPacket.begin();
	for (; iter!=m_mapPacket.end(); ++iter)
	{
		key = iter->first;
		listIter = iter->second->begin();
		for (; listIter!= iter->second->end(); ++listIter)
		{
			(*listIter)->Dec();
		}
		delete iter->second;
		iter->second = NULL;
	}
	
	for (int i=0; i<MAX_RTSP_MEDIA_COUNT*2; i++)
	{
		if (m_pUDPSock[i] != NULL)
		{
			m_pUDPSock[i]->CloseSock();
			delete m_pUDPSock[i];
			m_pUDPSock[i] = NULL;
		}
	}

	if (m_pRTSPSock)
	{
		m_pRTSPSock->CloseSock();
	}
	
	m_Lock.UnLock();

	return 0;
}

bool CServerRTPSession::SetMediaInfo(unsigned short ServerRTPPort)
{
	m_Lock.Lock();
	m_FrameRate = 30;
	m_pRTSP = new CServerRTSP(RTP_PAYLOAD_TYPE_H264, /*RTP_PAYLOAD_TYPE_G711*/ -1, ServerRTPPort, m_FrameRate);
	m_VideoType = RTP_PAYLOAD_TYPE_H264;
	m_AudioType = -1;
	m_Lock.UnLock();

	return true;
}

void CServerRTPSession::GetRTPPacket(CRTPPacket *pPacket, unsigned short streamType)
{
	m_Lock.Lock();
	if (!m_bIsStartTransPacket)
	{
		m_Lock.UnLock();
		return;
	}

	if (m_bIsFirstStart)
	{
		if (!pPacket->IsKeyFrame())
		{
			printf("GetRTPPacket Not Key Frame\n");
			m_Lock.UnLock();
			return;
		}
		else
		{
			printf("Key Frame!!!!!!!!!!\n");
			m_bIsFirstStart = false;
		}
	}

	std::map<unsigned short, std::list<CRTPPacket*>*>::iterator it;
	it = m_mapPacket.find(streamType);
	if (it == m_mapPacket.end())
	{
		m_mapPacket[streamType] = new std::list<CRTPPacket*>();
		m_mapStreamInfo[streamType].DiscardPos = 0;
		m_mapStreamInfo[streamType].SSRC = m_RandSeed;
		m_RandSeed = (unsigned long)rand();
	}

	if (STREAM_LIVE_AUDIO == streamType)
	{
		while(m_mapPacket[streamType]->size() + 1 > RTP_SESSION_AUDIO_SIZE)
		{
			printf("channel %d Loss Audio\n", m_Channel);
			m_mapPacket[streamType]->front()->Dec();
			m_mapPacket[streamType]->pop_front();
		}
		m_mapPacket[streamType]->push_back(pPacket);
		pPacket->Inc();
	}
	else if(STREAM_LIVE_VIDEO == streamType)
	{
		if (!m_bIsDiscardEnd)
		{
			if (pPacket->IsKeyFrame())//丢包结束
			{
				m_bIsDiscardEnd = true;
			}
			else//丢包继续，新来的包直接丢弃
			{
				m_Lock.UnLock();
				return;
			}
		}

		while(m_mapPacket[streamType]->size() + 1 > RTP_SESSION_VIDEO_SIZE)
		{
			printf("channel %d Loss Video\n", m_Channel);
			DeleteOneFrame();
			while(!m_mapPacket[streamType]->empty() && !m_mapPacket[streamType]->front()->IsKeyFrame() )
			{
				DeleteOneFrame();
			}
			if (m_mapPacket[streamType]->empty())
			{
				if (!pPacket->IsKeyFrame())//丢弃新来的包
				{
					m_bIsDiscardEnd = false;
					m_Lock.UnLock();
					return;
				}
			}
		}
		m_mapPacket[streamType]->push_back(pPacket);
		pPacket->Inc();
	}
	else
	{
		assert(false);
	}

	m_Lock.UnLock();
}

int CServerRTPSession::SendRTPPacket()
{
	m_Lock.Lock();
	if (!m_bIsStartTransPacket || m_bPause)
	{
		m_Lock.UnLock();
		return 0;
	}
	CRTPPacket *pPacket = NULL;
	unsigned char *pData = NULL;
	unsigned long dataLen = 0;;
	unsigned char *pSendBuf = NULL;
	int sendLen;
	int ret;
	int type;
	bool bSleep = false;

	static int count = 0;

	std::map<unsigned short, std::list<CRTPPacket*>*>::iterator iter = m_mapPacket.begin();
	for (; iter!=m_mapPacket.end(); ++iter)
	{
		int key = iter->first;
		if (!m_mapPacket[key]->empty())
		{
//  			if (++count % 500 == 0)
//  			{
//  				printf("Session Packet Count %lu\n", m_mapPacket[key]->size());
//  			}

			//发送
 			pPacket = m_mapPacket[key]->front();

//国标添加


//控制发送间隔
// 			if (m_bCHeckMark)
// 			{
// 				if (m_LastTime == 0)
// 				{
// 					m_LastTime = GetTickCount();
// 				}
// 				else
// 				{
// 					m_ThisTime = GetTickCount();
// 
////  					if (m_Scale > 1)
////  					{
////   						if ((m_ThisTime - m_LastTime) < 10)//25帧理应是40  但是由于不可避免的sleep造成时间间隔不用准确
////   						{
////   							bSleep = true;
////   							break;
////   						}
////  					}
//					if(m_Scale == 1)
// 					{
// 						if ((m_ThisTime - m_LastTime) < 30)//25帧理应是40  但是由于不可避免的sleep造成时间间隔不用准确
// 						{
// 							bSleep = true;
// 							break;
// 						}
// 					}
// 					else if(m_Scale < 0)
// 					{
// 						if ((m_ThisTime - m_LastTime) < (30 * abs(m_Scale)))//25帧理应是40  但是由于不可避免的sleep造成时间间隔不用准确
// 						{
// 							bSleep = true;
// 							break;
// 						}
// 					}
// 					
// 					m_bCHeckMark = false;
// 					m_LastTime = m_ThisTime;
// 				}
// 			}
// 
// 			if (pPacket->IsMark())
// 			{
// 				m_bCHeckMark = true;
// 			}
//
//			if (pPacket->IsPause())
//			{
//				m_bPause = true;
//				m_LastTime = 0;
//				printf("Paused\n");
//			}
		
//国标添加结束
 			pPacket->SetSSRC(htonl(m_mapStreamInfo[key].SSRC));
 			if (m_bIsUDP)
 			{
 				pData = pPacket->GetRTPData(dataLen);
 			}
 			else
 			{
 				pData = pPacket->GetRTPDataTCP(dataLen);
 				if (-1 == m_AudioType)//如果不包含音频，interleaved.channel不为2  而为0
 				{
 					((INTERLEAVED_INFO*)pData)->channel = 0;
 				}
 			}
 			pSendBuf = pData;
 			sendLen = dataLen;
 			if (0 != m_mapStreamInfo[key].DiscardPos)
 			{
 				pSendBuf = pData + m_mapStreamInfo[key].DiscardPos;
 				sendLen = dataLen - m_mapStreamInfo[key].DiscardPos;
 			}
			if (m_bIsUDP)
			{
				ret = m_pUDPSock[key*2]->SendData((const char*)(pSendBuf), sendLen, 100);
				//printf("SendData %d %lu, %d\n", ret, m_pUDPSock[key*2]->GetDestIP(), m_pUDPSock[key*2]->GetRemotePort());
			}
			else
			{
				ret = m_pRTSPSock->SendData((const char*)(pSendBuf), sendLen, 100);
			}

 			if (ret == sendLen)
 			{
				//printf("RTP Send Data Suc , Len : %d\n", ret);
 				//更新保存时间信息
 				type = pPacket->ISAudio() ? STREAM_LIVE_AUDIO : STREAM_LIVE_VIDEO;
 				m_LastPacketTimeStamp[type] = pPacket->GetRTPTimestamp();
 				m_LastNTPSec[type] = pPacket->GetNTPSec();
 				m_LastNTPMicroSec[type] = pPacket->GetNTPMicroSec();
 
 				m_SenderPacketCount[type] = pPacket->GetSeqNumber();
 				m_SenderOctetCount[type] += pPacket->GetLength();
 
 				pPacket->Dec();
 				m_mapStreamInfo[key].DiscardPos = 0;
 				m_mapPacket[key]->pop_front();
 				SendWaitTimeClear();
 			}
 			else if(ret > 0)
 			{
				printf("Send Packet ret > 0  ret != send len\n");
 				m_mapStreamInfo[key].DiscardPos = dataLen - sendLen + ret;
 				SendWaitTimeClear();
 			}
 			else if(ret < 0)
 			{
				printf("Send Packet ret < 0\n");
 				m_Lock.UnLock();
 				return -1;
 			}
 			else//ret == 0
 			{
				printf("Send Packet ret == 0\n");
 				SendWaitTimeInc();
 				m_Lock.UnLock();
 				return 1;
 			}
		}
	}

	m_Lock.UnLock();

	if (bSleep)
	{
		PUB_Sleep(1);
	}

	return 0;
}

int CServerRTPSession::RecvMsg()
{
	int ret = 0;

	m_Lock.Lock();
	if (m_RecvLen >= MAX_RTSP_MSG_LEN - 300)
	{
		printf("RTSP Recv Buff Insufficient : %d\n", MAX_RTSP_MSG_LEN - m_RecvLen);
	}
	ret = m_pRTSPSock->RecvData((char *)(m_pRecvBuf + m_RecvLen), MAX_RTSP_MSG_LEN - m_RecvLen, 300);
	if (0 <= ret)
	{
		m_RecvLen += ret;
		ret = HandleRecvMsg();
	}
	else
	{
		m_Lock.UnLock();
		return -1;
	}
	m_Lock.UnLock();

	return ret;
}


int CServerRTPSession::HandleRecvMsg()
{
	int ret;
	m_Lock.Lock();
	if (m_RecvLen == 0 || m_pRTSP == NULL)
	{
		
		m_Lock.UnLock();
		return 0;
	}

	if (INTERLEAVED_DOLLAR_SIGN == *m_pRecvBuf)
	{
		HandleRTCP();
	}
	else
	{
		char buf[1024]={0};
		snprintf(buf, m_RecvLen, "%s", m_pRecvBuf);
		printf("%s", buf);

		ret = m_pRTSP->HandleMsg((const char*)(m_pRecvBuf), (char *)m_pSendBuf, m_SendLen);
		if(ret > 0)
		{
			m_bRTSPWaitSend = true;
			m_RecvLen -= ret;
			if (RTSP_SESSION_END == m_pRTSP->GetStatus())
			{
				m_Lock.UnLock();
				return -1;
			}
		}
		else//会话异常，退出
		{
			m_Lock.UnLock();
			return -1;
		}
	}
	m_Lock.UnLock();
	return 0;
}

int CServerRTPSession::SendMsg(SWL_socket_t &fd)
{
	m_Lock.Lock();
	if (m_bRTSPWaitSend)
	{
		int ret;
		ret = m_pRTSPSock->SendData((const char*)(m_pSendBuf), m_SendLen, 300);
		if (ret == m_SendLen)
		{
			printf("%s", m_pSendBuf);
			printf("____________________m_pSendBuf End\n");
			m_bRTSPWaitSend = false;
			if (!m_bIsStartTransPacket)
			{
				if (RTSP_SESSION_PLAY == m_pRTSP->GetStatus())
				{
					m_bIsStartTransPacket = true;
					m_bIsFirstStart = true;
					if (m_pRTSP->IsTransOverUDP())
					{
						CreateRTPPort(m_pRTSP->GetUDPServerPort(), m_pRTSP->GetUDPClientPort(), m_pRTSPSock->GetDestIP(), m_pRTSP->GetSetUpCount()*2);
						fd = m_pUDPSock[STREAM_LIVE_VIDEO]->GetSockfd();
					}
				}
			}
			SendWaitTimeClear();
		}
		else if(ret > 0)
		{
			memmove(m_pSendBuf, m_pSendBuf + ret, m_SendLen - ret);
			m_SendLen -= ret;
			SendWaitTimeClear();
		}
		else if(ret < 0)
		{
			m_Lock.UnLock();
			return -1;
		}
		else// ret == 0
		{
			SendWaitTimeInc();
		}
	}

	m_Lock.UnLock();
	return 0;
}

int CServerRTPSession::HandleRTCP()
{
	INTERLEAVED_INFO* pInterleaved = (INTERLEAVED_INFO*)m_pRecvBuf;
	unsigned long Len = ntohs(pInterleaved->datalength);

	if (m_RecvLen < Len)
	{
		return -1;
	}
	Len += sizeof(INTERLEAVED_INFO);
	memmove(m_pRecvBuf, m_pRecvBuf + Len, m_RecvLen - Len);
	m_RecvLen -= Len;
	return 0;
}


bool CServerRTPSession::DeleteOneFrame()
{
	CRTPPacket *pPacket = m_mapPacket[STREAM_LIVE_VIDEO]->front();
	unsigned long timestamp = 0;

	timestamp = pPacket->GetRTPTimestamp();

	while(!m_mapPacket[STREAM_LIVE_VIDEO]->empty())
	{
		pPacket = m_mapPacket[STREAM_LIVE_VIDEO]->front();
		if (timestamp != pPacket->GetRTPTimestamp())
		{
			break;
		}
		pPacket->Dec();
		m_mapPacket[STREAM_LIVE_VIDEO]->pop_front();
	}

	return true;
}

unsigned char CServerRTPSession::GetChannel()
{
	unsigned char channel;
	m_Lock.Lock();
	if (m_pRTSP)
	{
		channel = m_pRTSP->GetChannel();
	}
	else
	{
		channel = m_Channel;
	}
	m_Lock.UnLock();
	return channel;
}

SWL_socket_t CServerRTPSession::GetSocketFd()
{
	SWL_socket_t tmp;
	m_Lock.Lock();
	if (m_pRTSPSock)
	{
		tmp = m_pRTSPSock->GetSockfd();
	}
	else
	{
		tmp = SWL_INVALID_SOCKET;
	}
	m_Lock.UnLock();
	return tmp;
}

SWL_socket_t CServerRTPSession::GetRTPSocketFd()
{
	SWL_socket_t tmp = SWL_INVALID_SOCKET;
	m_Lock.Lock();
 	if (m_pRTSP)
 	{
		if (m_pRTSP->IsTransOverUDP())
		{
			tmp = m_pUDPSock[STREAM_LIVE_VIDEO]->GetSockfd();
		}
 	}
	else
	{
		tmp = m_pUDPSock[STREAM_LIVE_VIDEO]->GetSockfd();
	}

	m_Lock.UnLock();
	return tmp;
}

int CServerRTPSession::GetServerRTPPort()
{
	int tmp;
	m_Lock.Lock();
	if (m_pRTSP)
	{
		m_Lock.UnLock();
		return m_pRTSP->GetUDPServerPort();
	}
	else
	{
		tmp = m_pUDPSock[STREAM_LIVE_VIDEO]->GetPort();
		m_Lock.UnLock();
	}
	
	return tmp;
}

bool CServerRTPSession::CreateRTPPort(unsigned short ServerPort, unsigned short *ClientPort, unsigned long DestIP, int num)
{
	if (m_pUDPSock[0] != NULL)
	{
		return true;
	}

	for (int i=0; i<num; i++)
	{
		m_pUDPSock[i] = new CRTPSock;
		if (NULL == m_pUDPSock[i])
		{
			printf("%s %d Err\n", __FILE__, __LINE__);
			return false;
		}
		if (m_pUDPSock[i]->CreateSock(ServerPort+i, ClientPort[i], DestIP, IPPROTO_UDP) < 0)
		{
			printf("%s %d Err\n", __FILE__, __LINE__);
			for (int j=0; j<i; j++)
			{
				delete m_pUDPSock[j];
				m_pUDPSock[j] = NULL;
			}
			return false;
		}
	}
	m_bIsUDP = true;

	return true;
}

bool CServerRTPSession::IsSendOverUDP()
{
 	bool ret;
 	m_Lock.Lock();
	if (!m_pRTSP)
	{
		if (m_pUDPSock[0])
		{
			ret = true;
		}
		else
		{
			ret = false;
		}
	}
	else
	{
		ret = m_pRTSP->IsTransOverUDP();
	}
 	m_Lock.UnLock();
 	return ret;
}

bool CServerRTPSession::CreateSR(int type)
{
	if (STREAM_LIVE_VIDEO != type && STREAM_LIVE_AUDIO != type)
	{
		return false;
	}

	if (!m_pSR[type])
	{
		m_pSR[type] = new RTCP_SENDER_REPORTS;
		if(!m_pSR[type])
		{
			printf("%s %d new Err\n", __FILE__, __LINE__);
			return false;
		}
		memset(m_pSR[type], 0, sizeof(RTCP_SENDER_REPORTS));

		m_pSR[type]->rtcpHeader.version = 2;
		m_pSR[type]->rtcpHeader.packetType = RTCP_PACKET_SR;
	}
	m_Lock.Lock();

	m_pSR[type]->reporterSSRC = ntohl(m_mapStreamInfo[type].SSRC);

	unsigned long sec = RTSPGetCurrTime().seconds;
	unsigned long microsec = RTSPGetCurrTime().microsecond;

	ULONGLONG offset = (sec - m_LastNTPSec[type])*1000000 + (microsec - m_LastNTPMicroSec[type]);//偏移了多少微秒
	double timeIncFrac = (double)(type == STREAM_LIVE_VIDEO ? RTP_VIDEO_CAPTURE_FREQUENCY : RTP_AUDIO_CAPTURE_FREQUENCY)/1000000;
	unsigned long TimeStamp = m_LastPacketTimeStamp[type] + offset * timeIncFrac;

	m_pSR[type]->ntpTimeH = htonl(sec+NTP_OFFSET_FROM_1900_TO_1970);
	m_pSR[type]->ntpTimeL = htonl(microsec * 4294);//4294 = 0x100000000 / 1000000
	m_pSR[type]->rtpTimestamp = htonl(TimeStamp);

	m_pSR[type]->senderPacketCount = htonl(m_SenderPacketCount[type]);
	m_pSR[type]->senderOctetCount = htonl(m_SenderOctetCount[type]);
	m_pSR[type]->rtcpHeader.length = htons(6);

	m_Lock.UnLock();
	return true;
}

int CServerRTPSession::SendRTCP()
{
	if (!IsSendOverUDP())
	{
		return 0;
	}

	m_Lock.Lock();
	if (!m_bIsStartTransPacket)
	{
		m_Lock.UnLock();
		return 0;
	}
	char pBuf[1024] = {0};
	unsigned long Len = 0;
// 	if (m_AudioType >= 0)
// 	{
// 		CreateSDES(pBuf, Len, m_mapStreamInfo[STREAM_LIVE_AUDIO].SSRC);
// 		m_pUDPSock[STREAM_LIVE_AUDIO*2+1]->SendData(pBuf, Len, 300);
// 		CreateSR(STREAM_LIVE_VIDEO);
// 		m_pUDPSock[STREAM_LIVE_AUDIO*2+1]->SendData((const char*)m_pSR[STREAM_LIVE_AUDIO], sizeof(RTCP_SENDER_REPORTS), 300);
// 	}
	if (m_VideoType >= 0)
	{
		CreateSDES(pBuf, Len, m_mapStreamInfo[STREAM_LIVE_VIDEO].SSRC);
		m_pUDPSock[STREAM_LIVE_VIDEO*2+1]->SendData(pBuf, Len, 300);
		CreateSR(STREAM_LIVE_VIDEO);
		m_pUDPSock[STREAM_LIVE_VIDEO*2+1]->SendData((const char*)m_pSR[STREAM_LIVE_VIDEO], sizeof(RTCP_SENDER_REPORTS), 300);
	}
	m_Lock.UnLock();
	return 0;
}

void CServerRTPSession::CreateSDES(char *pBuf, unsigned long &len, unsigned long csrc)
{
	char *pPos = pBuf;

	RTCP_HEADER *pRTCPHeader = (RTCP_HEADER*)pPos;
	pRTCPHeader->version = 0x02;
	pRTCPHeader->packetType = RTCP_PACKET_SDES;
	pRTCPHeader->padding = 0;
	pRTCPHeader->itemCount = 1;
	unsigned long csrc1 = htonl(csrc);

	pPos += sizeof(RTCP_HEADER);
	memcpy(pPos, &csrc1, sizeof(unsigned long));
	pPos += sizeof(unsigned long);
	*pPos = SDES_ITEM_CNAME;
	pPos += 1;
	unsigned char SDESLen = 255;//HOST_NAME_MAX
	char SDESText[255] = {0};
	if (-1 == gethostname(SDESText, SDESLen))
	{
		assert(false);
		len = 0;
		return;
	}

	//test
	strcpy(SDESText, "TVT");
	
	SDESLen = strlen(SDESText);
	*pPos = SDESLen;
	pPos += 1;
	memcpy(pPos, SDESText, SDESLen);
	pPos += SDESLen;;
	unsigned char alignLen = (4 - (SDESLen + 2 + sizeof(RTCP_HEADER) + sizeof(unsigned long)) % 4);
	for(int i=0; i<alignLen ; ++i)
	{
		*pPos = 0;
		pPos++;
	}

	pRTCPHeader->length = htons((sizeof(unsigned long) + 2 + SDESLen + alignLen) / 4 );

	len = pPos - pBuf;
}

int CServerRTPSession::CallCallBack(int channel)
{
	m_pSipCallBack(channel);

	return 0;
}

void CServerRTPSession::SetPause(bool bPause)
{
	m_Lock.Lock();
	m_bPause = bPause;
	m_Lock.UnLock();
}

void CServerRTPSession::SetScale(int Scale)
{
	m_Lock.Lock();
	m_Scale = Scale;
	m_Lock.UnLock();
}