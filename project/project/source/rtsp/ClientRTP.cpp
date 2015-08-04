//#define Test_SaveData

#include "ClientRTP.h"
#include "H264Parser.h"

#if defined __ENVIRONMENT_LINUX__
#include <unistd.h>
#endif

#define RTP_AUDIO_G711_LENS 320


CClientRTP::CClientRTP()
{
	m_pRTPRecvRam = new char[CLIENT_RTP_RECV_BUF_LEN];//多分配内存用于RTSP_DATA_FRAME头
	assert(m_pRTPRecvRam);
	memset(m_pRTPRecvRam, 0, CLIENT_RTP_RECV_BUF_LEN);
	m_pRTPRecvBuf = m_pRTPRecvRam + sizeof(RTSP_DATA_FRAME);
	m_RTPRecvBufLen = CLIENT_RTP_RECV_BUF_LEN - sizeof(RTSP_DATA_FRAME);
	m_pRTPCurPos = m_pRTPRecvBuf;

	m_pRTPARecvRam = new char[CLIENT_AUDIO_RECV_LENS];
	assert(m_pRTPARecvRam);
	memset( m_pRTPARecvRam, 0, CLIENT_AUDIO_RECV_LENS);
	m_RTPAudioBufLen = CLIENT_AUDIO_RECV_LENS - sizeof(RTSP_DATA_FRAME);
	m_RTPAudioFrameLens = 0;
	m_pRTPARecvBuf = m_pRTPARecvRam + sizeof(RTSP_DATA_FRAME);

	m_pRTCPRecvBuf = new char[CLIENT_RTCP_RECV_BUF_LEN];
	assert(m_pRTCPRecvBuf);
	memset(m_pRTCPRecvBuf, 0, CLIENT_RTCP_RECV_BUF_LEN);

	m_pSpsPps = NULL;
	m_SpsPpsLen = 0;

	ResetObject();

}

CClientRTP::~CClientRTP()
{
	if (NULL != m_pRTPRecvRam)
	{
		delete []m_pRTPRecvRam;
		m_pRTPRecvRam = NULL;
	}
	if (NULL != m_pRTCPRecvBuf)
	{
		delete []m_pRTCPRecvBuf;
		m_pRTCPRecvBuf = NULL;
	}
	if (NULL != m_pRTPARecvRam)
	{
		delete []m_pRTPARecvRam;
		m_pRTPARecvRam = NULL;
	}

	if (m_pSpsPps)
	{
		delete [] m_pSpsPps;
		m_pSpsPps = NULL;
	}

}

bool CClientRTP::ResetObject(void)
{
	m_pRTPRecvBuf = m_pRTPRecvRam + sizeof(RTSP_DATA_FRAME);
	m_pRTPCurPos = m_pRTPRecvBuf;

	m_pRTPARecvBuf = m_pRTPARecvRam + sizeof(RTSP_DATA_FRAME);
	m_RTPAudioBufLen = CLIENT_AUDIO_RECV_LENS - sizeof(RTSP_DATA_FRAME);
	m_RTPAudioFrameLens = 0;

	m_bHasAudio = true;
	m_bHasVideo = true;

	m_bFrameDistributeNeedWait = false;
	m_pFrameStartPos = NULL;
	m_pFrameEndPos = NULL;

	m_RR[STREAM_LIVE_VIDEO].rtporterSSRC = (unsigned long)rand();
	m_RR[STREAM_LIVE_AUDIO].rtporterSSRC = m_RR[STREAM_LIVE_VIDEO].rtporterSSRC + 1;
	for (int i=0; i<MAX_RTSP_MEDIA_COUNT; i++)
	{
		m_RR[i].extendSeqNum.CyclesCount = 0;
		m_RTCPHeader[i].version = 2;
		m_LastJitter[i] = 0;
		m_LastRecvSRTimeSec[i] = 0;
		m_LastRecvSRTimeUSec[i] = 0;
		memset(&m_lastSR[i], 0, sizeof(RTCP_SENDER_REPORTS));
		m_LastPacketTime[i] = 0;
		m_LastButOnePacketTime[i] = 0;
		m_bIsFirstDistribute[i] = true;
		m_bSystemTimeChanged[i] = false;
	}
	m_pCallBack = NULL;
	m_pParam = NULL;
	m_TVTSessionID = 0;
	m_LastVFrameTs = 0;
	m_bIsKeyFrame = false;
	m_bRTCPSendControl = true;
	m_bIsVFrameDistribute = false;
	m_bForceFrameOnSameTimestamp = false;

	m_bMustCurFrameIsKeyframe = true;
	m_bSessionTellDataError = false;

	m_video_payload = (unsigned int)-1;
	m_audio_payload = (unsigned int)-1;

	m_audio_data_type = (unsigned int)-1;

	m_stCheckRtspDatatime.seconds = 0;
	m_stCheckRtspDatatime.microsecond = 0;

	m_stAudioChkFrmTime.seconds = 0;
	m_stAudioChkFrmTime.microsecond = 0;

	m_stSaveResyncTimes.seconds = 0;
	m_stSaveResyncTimes.microsecond = 0;

	m_iPrintVideoSyncCnt = 0;
	m_iPrintAudioSyncCnt = 0;

	m_iChannelNum = -1;

	m_bNvrIpcSystimeResync = false;

	m_iCheckAudioFrameSendContinue = 0;

	memset( &m_stSavePrevRTPHeader, 0, sizeof(RTP_HEADER));
	m_bFirstRtpHeader = true;

	m_iReallocBufProcess = 0;

	m_iNonIntervalPackCnt = 0;

	m_ulLastError = 0;
	m_iVideoPrintCount = 0;
	m_iAudioPrintCount = 0;

	m_run_lines = 0;
	m_bIsHasSpsInfo = false;

	m_iCheckSpsCount = 0;

	m_audio_frame_data_len = 0;

	m_iAudioSpsFrmCnt = 0;

	m_pFuncProcCallback = NULL;
	m_pFuncProcParam = NULL;

	m_iRtspProcFrameNumber = 0;

	return true;
}

int CClientRTP::Stop()
{
#ifdef Test_SaveData
 	int ret = fclose(m_file);
 	assert(0 == ret);
 
 	ret = fclose(m_fileAudio);
 	assert(0 == ret);
#endif
	return 0;
}

void CClientRTP::SetFuncProcCallback(PRTP_FUNC_PROC_CALLBACK pFuncProcCallBack, void *pParam)
{
	m_pFuncProcCallback = pFuncProcCallBack;
	m_pFuncProcParam = pParam;
}

void CClientRTP::SetCallBack(RTP_CALLBACK pCallBack, void *pParam, int TVTSessionID)
{
	m_pCallBack = pCallBack;
	m_pParam = pParam;
	m_TVTSessionID = TVTSessionID;

	//for test
#ifdef Test_SaveData
  	char FileName[64] = {0};
  	sprintf(FileName, "d:\\RTP_TEST%d.dat", TVTSessionID);
  	m_file = fopen(FileName, "wb");
  	assert(NULL != m_file);
  
  	sprintf(FileName, "d:\\Audio_Test%d.dat", TVTSessionID);
  	m_fileAudio = fopen(FileName, "wb");
  	assert(NULL != m_fileAudio);
#endif
}

bool CClientRTP::SetUp(bool bHasVideo, bool bHasAudio, float frameRate, 
					   unsigned short width, unsigned short height,
					   unsigned int video_payload, unsigned int audio_payload,
					   unsigned int audio_data_type )
{

	m_bHasVideo = bHasVideo;
	m_bHasAudio = bHasAudio;

	m_Width = width;
	m_Height = height;
	if (frameRate == 0)
	{
		frameRate = 25;
	}
	m_VideoTimeInc = 1000000 / frameRate;

	m_video_payload = video_payload;
	m_audio_payload = audio_payload;

	if( RTP_PAYLOAD_TYPE_G711_U == audio_data_type
		|| RTP_PAYLOAD_TYPE_G711_A == audio_data_type )
	{
		m_audio_data_type = audio_data_type;
	}

	m_iCheckSpsCount = 0;

	return true;
}

int CClientRTP::GetRTPData(const char *pData,const unsigned long len, 
		unsigned char InterleavedType, bool bPrevFrameData)
{

	if (NULL == pData)
	{
		return -1;
	}

	//printf( "current chann %d, len %d, %s,%d\n", m_iChannelNum, len, __FILE__, __LINE__ );
	
	unsigned long CurDataLen = m_pRTPCurPos - m_pRTPRecvBuf;
	if (m_RTPRecvBufLen - CurDataLen < len)
	{
		//追加缓存
		if( !ReAllocBuf(CurDataLen, (len - (m_RTPRecvBufLen - CurDataLen) + 512)) )
		{
			ResetVideoBufferParamInfo();
			return -1;
		}
	}

	if( m_bSessionTellDataError )
	{
		//printf( "current session tell error %d\n", m_iChannelNum );
		ResetVideoBufferParamInfo();

		m_bSessionTellDataError = false;
	}

	m_iRtspProcFrameNumber++;
	if( m_pFuncProcCallback && m_pFuncProcParam )
	{
		if( 0 == m_subChnnNum )
		{
			RTSP_FUNC_PROC_CALLBACK_INFO stFuncProcCallback;

			memset( &stFuncProcCallback, 0, sizeof(RTSP_FUNC_PROC_CALLBACK_INFO) );
			stFuncProcCallback.func_type = RTSP_FUNC_PROC_TYPE_REF_COUNT;
			stFuncProcCallback.ref_count = m_iRtspProcFrameNumber;

			m_pFuncProcCallback( &stFuncProcCallback, m_pFuncProcParam );
		}
	}

	unsigned long RemainLen = 0;
	if (0 == InterleavedType % 2)
	{
		bool bFalse = false;
		bool bIsVideoData = false;
		RemainLen = HandleRawData(pData, len, InterleavedType, bFalse, bIsVideoData );
		if (bFalse)
		{
			if( bIsVideoData )
			{
				ResetVideoBufferParamInfo();
				RemainLen = 0;

				return -1;
			}
			else
			{
				RemainLen = 0;
			}

		}
		else
		{
			RemainLen = 0;
		}
	}
	else
	{
		RemainLen = HandleRTCP(pData, len, InterleavedType);
	}

	return len-RemainLen;
}

void CClientRTP::ResetVideoBufferParamInfo()
{
	m_bFrameDistributeNeedWait = false;
	m_bIsVFrameDistribute = false;
	m_bForceFrameOnSameTimestamp = false;
	m_bMustCurFrameIsKeyframe = true;
	m_pFrameStartPos = NULL;
	m_pFrameEndPos = NULL;
	m_bIsKeyFrame = false;
	m_pRTPCurPos = m_pRTPRecvBuf;

	memset( &m_stSavePrevRTPHeader, 0, sizeof(RTP_HEADER));
	m_bFirstRtpHeader = true;

	//reset time
	for (int i=0; i<MAX_RTSP_MEDIA_COUNT; i++)
	{
		m_bIsFirstDistribute[i] = true;
	}

}

bool CClientRTP::ReAllocForDecBuf( int curFrameLen )
{
	if( curFrameLen <= CLIENT_RTP_NORMAL_RECV_LEN )
	{
		m_iReallocBufProcess++;
	}
	else
	{
		m_iReallocBufProcess = 0;
	}

	if( m_RTPRecvBufLen > CLIENT_RTP_NORMAL_RECV_LEN 
		&& m_iReallocBufProcess > 10000 )
	{
		printf( "realloc for dec %d buf %s,%d\n", m_iChannelNum, __FILE__, __LINE__ );


		char *pTmpBuf = new char[CLIENT_RTP_RECV_BUF_LEN];
		if( !pTmpBuf )
		{
			return false;
		}

		memset( pTmpBuf, 0, CLIENT_RTP_RECV_BUF_LEN );

		delete []m_pRTPRecvRam;

		m_pRTPRecvRam = pTmpBuf;
		m_pRTPRecvBuf = m_pRTPRecvRam + sizeof(RTSP_DATA_FRAME);
		m_RTPRecvBufLen = CLIENT_RTP_RECV_BUF_LEN - sizeof(RTSP_DATA_FRAME);
		m_pRTPCurPos = m_pRTPRecvBuf;

	}

	return true;

}

bool CClientRTP::ReAllocBuf(unsigned long CurDataLen, int frame_len_alloc )
{
	unsigned long StartPosOffset = m_pFrameStartPos - m_pRTPRecvBuf;
	unsigned long EndPosOffset = m_pFrameEndPos - m_pRTPRecvBuf;

	unsigned long lens;

	lens = ((frame_len_alloc + (CLIENT_RTP_RECV_BUF_LEN-1))/CLIENT_RTP_RECV_BUF_LEN)*CLIENT_RTP_RECV_BUF_LEN;
	lens += m_RTPRecvBufLen + sizeof(RTSP_DATA_FRAME);

	if( lens > CLIENT_RTP_MAX_RECV_LENS )
	{
		printf( "error realloc current lens = %d, %d, %s,%d\n", lens, m_iChannelNum, __FILE__, __LINE__ );
		return false;
	}
	else
	{
		printf( "realloc buffer lens = %d, (%d %d,) %s,%d\n", lens, m_iChannelNum, m_subChnnNum, __FILE__, __LINE__ );
	}

	char *pTmpBuf = new char[lens];
	if (NULL == pTmpBuf)
	{
		printf("realloc new char buffer errror %s %d err\n", __FILE__, __LINE__);
		return false;
	}

	memset( pTmpBuf, 0, lens );
	memcpy(pTmpBuf + sizeof(RTSP_DATA_FRAME), m_pRTPRecvBuf, CurDataLen);

	m_RTPRecvBufLen = lens-sizeof(RTSP_DATA_FRAME);
	m_pRTPCurPos = pTmpBuf + CurDataLen + sizeof(RTSP_DATA_FRAME);

	delete []m_pRTPRecvRam;
	m_pRTPRecvRam = pTmpBuf;
	m_pRTPRecvBuf = m_pRTPRecvRam + sizeof(RTSP_DATA_FRAME);

	m_pFrameStartPos = StartPosOffset + m_pRTPRecvBuf;
	m_pFrameEndPos = EndPosOffset + m_pRTPRecvBuf;

	return true;
}


unsigned long CClientRTP::HandleRawData(const char* pData,const unsigned long len, 
					unsigned char InterleavedChannel, bool &IsFalse, bool &bIsVideoData )
{
	IsFalse = false;
	RTP_HEADER *pRTPHeader = NULL;
	NALU_HEADER *pNALUHeader = NULL;
	FU_HEADER *pFUHeader = NULL;
	unsigned char NaluType = 0;
	char FU_A_NAL;
	unsigned short PacketLen = 0;
	unsigned long sendPosOffset = 0;
	unsigned long RealSendLen = 0;
	unsigned long RemainLen = len;
	unsigned long offset = 0;
	unsigned char type;
	unsigned int i_ext_data_lens;

	unsigned int audio_left_lens = 0;

	RTP_HEADER stCurRtpH;
	NALU_HEADER stCurNALUH;
	FU_HEADER stCurFUH;

	while(RemainLen > 0)
	{
		// 下面这个条件 ？？？
		//if(RemainLen < sizeof(RTP_HEADER) + sizeof(NALU_HEADER))//下次接收到更多的数据再处理
		//{
		//	printf("current data %s %d RemainLen not enough\n", __FILE__, __LINE__);
		//	break;
		//}

		memcpy( &stCurRtpH, pData + offset, sizeof(RTP_HEADER) );
		pRTPHeader = (RTP_HEADER*)&stCurRtpH;
		if( pRTPHeader->version != 2 )
		{
			IsFalse = true;
			bIsVideoData = true;
			//printf( "current channel %d,%d version is not %d,%d,%d %s,%d\n",
			//	m_iChannelNum, m_subChnnNum, 
			//	pRTPHeader->version, pRTPHeader->csrccount, pRTPHeader->extension,
			//	__FILE__, __LINE__ );

			m_ulLastError |= ONVIF_ERROR_RTP_HEAD_VER;

			return 0;
		}

		int cur_rtp_sequencnum = ntohs( pRTPHeader->sequenceNumber );
		if( !m_bFirstRtpHeader )
		{
			if( 0 != cur_rtp_sequencnum )
			{
				if( cur_rtp_sequencnum != m_stSavePrevRTPHeader.sequenceNumber+1 )
				{
					//printf( "current RTPHeader error (%d,%d) %d,%d, %d,%d\n",
					//	m_iChannelNum, m_subChnnNum,
					//	cur_rtp_sequencnum, (int)m_stSavePrevRTPHeader.sequenceNumber,
					//	(int)pRTPHeader->ssrc, (int)m_stSavePrevRTPHeader.ssrc );

					m_ulLastError |= ONVIF_ERROR_RTP_SERIAL;
				}
			}
		}
		memcpy( &m_stSavePrevRTPHeader, pRTPHeader, sizeof(RTP_HEADER) );
		m_stSavePrevRTPHeader.sequenceNumber = cur_rtp_sequencnum;


		i_ext_data_lens = sizeof(RTP_HEADER);
		if( pRTPHeader->csrccount > 0 )
		{
			i_ext_data_lens += pRTPHeader->csrccount*4;
			//printf( "current csrccount = %d,%d, %d\n ", m_iChannelNum, m_subChnnNum, pRTPHeader->csrccount );
			m_ulLastError |= ONVIF_ERROR_HAS_CSRC_DATA;
		}

		if( 1 == pRTPHeader->extension )
		{
			unsigned short extlen = *(unsigned short*)(pData+offset+i_ext_data_lens+2);

			extlen = ntohs(  extlen );

			i_ext_data_lens += 4;
			if( extlen > 0 )
			{
				i_ext_data_lens += extlen*4;
			}

			m_ulLastError |= ONVIF_ERROR_HAS_EXTERN_DATA;
			//printf( "current extension =%d,%d %d extlen = %d\n ", m_iChannelNum, m_subChnnNum, pRTPHeader->extension, extlen );
		}

		if( i_ext_data_lens >= RemainLen )
		{
			m_ulLastError |= ONVIF_ERROR_CRSC_EXT_LEN;
			return 0;
		}

		memcpy( &stCurNALUH, (pData + offset + i_ext_data_lens), sizeof(NALU_HEADER) );
		pNALUHeader = (NALU_HEADER*)&stCurNALUH;
		if (RemainLen > i_ext_data_lens + sizeof(NALU_HEADER))
		{
			memcpy( &stCurFUH,(pData + offset + i_ext_data_lens + sizeof(NALU_HEADER)),sizeof(NALU_HEADER) );
			pFUHeader = (FU_HEADER*)&stCurFUH;
		}
		else
		{
			pFUHeader = NULL;
		}
		
//  		printf("type: %u    Sequence: %u    TimeStamp: %lu     SSRC: %lu     Marker :%u    ", \
//  			pRTPHeader->payloadtype, htons(pRTPHeader->sequenceNumber), htonl(pRTPHeader->timeStamp), htonl(pRTPHeader->ssrc), pRTPHeader->marker);
//  		printf("NALU: type:%u  nri:%u  f:%u   ", pNALUHeader->type, pNALUHeader->nri, pNALUHeader->forbidden_zero_bit);
//  		printf("FU: type:%u  start:%u  end:%u  reserve:%u \n", pFUHeader->type, pFUHeader->start_bit, pFUHeader->end_bit, pFUHeader->reserve_bit);

		RealSendLen = RemainLen;

		offset += i_ext_data_lens;
		RealSendLen -= i_ext_data_lens;

		//type = (pRTPHeader->payloadtype == RTP_PAYLOAD_TYPE_H264 || pRTPHeader->payloadtype == RTP_PAYLOAD_TYPE_PS) ? STREAM_LIVE_VIDEO : STREAM_LIVE_AUDIO;
		type = (m_video_payload == pRTPHeader->payloadtype ) ? STREAM_LIVE_VIDEO : STREAM_LIVE_AUDIO;

#if 0
		if( STREAM_LIVE_VIDEO == type && 0 == m_iChannelNum && 1 == m_subChnnNum )
		{
			static int bktimstamp = 0;
			int xtmp = ntohl(pRTPHeader->timeStamp) - ntohl(bktimstamp);
			if( xtmp < 0 || xtmp > 4000 )
			{
				printf( "payload = %4d, %4d,%4d,%16d,%16u,%16u  %16u %4d,  %4d,%4d,%4d  %4d \n", m_subChnnNum,
					pRTPHeader->payloadtype, pRTPHeader->extension, ntohl(pRTPHeader->sequenceNumber), 
					ntohl(pRTPHeader->timeStamp), xtmp, RTSPGetTime(),
					pNALUHeader->type,
					pFUHeader->type, pFUHeader->start_bit, pFUHeader->end_bit,
					RemainLen );
			}
			//printf( "NALU %4d,%4d,%4d -- %x,%x,%x\n",pNALUHeader->type, pNALUHeader->nri, pNALUHeader->forbidden_zero_bit, 
			//	*(unsigned char*)pNALUHeader, *((unsigned char*)pData + offset + i_ext_data_lens+1), 
			//	*((unsigned char*)pData + offset + i_ext_data_lens+2) );

			bktimstamp = pRTPHeader->timeStamp;
		}

		//if (STREAM_LIVE_AUDIO != type)
		//{
		//	int tmp;
		//
		//	for( tmp = 0; tmp < 16; tmp++ )
		//	{
		//		printf( "%2x,", *((unsigned char*)pData + offset + tmp) );
		//	}
		//	printf( "--%d--%d--%d\n", ntohl(pRTPHeader->timeStamp), pFUHeader->type, pNALUHeader->type );
		//}
		
#endif		

		//RTP信息保存，供RTCP使用
		{
			m_Lock.Lock();
			m_LastButOneRTPHeader[type] = m_LastRTPHeader[type];
			m_LastButOnePacketTime[type] = m_LastPacketTime[type];
			unsigned long Sec = RTSPGetCurrTime().seconds;
			unsigned long uSec = RTSPGetCurrTime().microsecond ;//最大为10^6   模拟器下的不准  只精确到毫秒  精度达不到
			uSec =  4294 * uSec ;//转换为NTP_frac形式 4294 = 0x100000000 / 1000000
			m_LastPacketTime[type] = (Sec & 0x0000ffff)<<16 | uSec;
			m_LastRTPHeader[type].ssrc = ntohl(pRTPHeader->ssrc);
			m_LastRTPHeader[type].timeStamp = ntohl(pRTPHeader->timeStamp);
			m_LastRTPHeader[type].sequenceNumber = ntohs(pRTPHeader->sequenceNumber);
			if (m_LastRTPHeader[type].sequenceNumber == 0)
			{
				m_RR[type].extendSeqNum.CyclesCount++;
			}
			m_Lock.UnLock();
		}

		//音频或其他通过RFC3550打包的媒体
		if (STREAM_LIVE_AUDIO == type)
		{
			if(RemainLen > RTP_MTU)
			{
				return 0;
			}

			if( -1 == m_audio_data_type)
			{
				if( 0 == pRTPHeader->payloadtype )
				{
					m_audio_payload = 0;
					m_audio_data_type = RTP_PAYLOAD_TYPE_G711_U;
				}
				else if( 8 == pRTPHeader->payloadtype )
				{
					m_audio_payload = 8;
					m_audio_data_type = RTP_PAYLOAD_TYPE_G711_A;
				}
			}

			bIsVideoData = false;

			if( m_RTPAudioFrameLens + RealSendLen > m_RTPAudioBufLen )
			{
				m_RTPAudioFrameLens = 0;
				return 0;
			}

			memcpy(m_pRTPARecvBuf + m_RTPAudioFrameLens, (char*)pData + offset, RealSendLen);
			m_RTPAudioFrameLens += RealSendLen;
			if( m_RTPAudioFrameLens < RTP_AUDIO_G711_LENS*5 )
			{
				return 0;
			}

			audio_left_lens = m_RTPAudioFrameLens % (RTP_AUDIO_G711_LENS*5);
			m_RTPAudioFrameLens -= audio_left_lens;

			DistributeFrame(pRTPHeader->payloadtype);

			if( audio_left_lens > 0 )
			{
				memmove( m_pRTPARecvBuf, m_pRTPARecvBuf + m_RTPAudioFrameLens, audio_left_lens );
			}
			m_RTPAudioFrameLens = audio_left_lens;

			return 0;
		}
		else
		{
			bIsVideoData = true;
		}

		if (0 != pRTPHeader->padding)//去除padding
		{
			unsigned char paddingNum = pData[len-1];
			if (paddingNum >= 4)
			{
				IsFalse = true;
				m_ulLastError |= ONVIF_ERROR_RTP_PADDING;
				printf("pRTPHeader->padding error %d %s %d err\n", 
					paddingNum, __FILE__, __LINE__);
				break;
			}
			else
			{
				RealSendLen -= paddingNum;
			}
		}

		if( !m_bIsVFrameDistribute && m_iNonIntervalPackCnt > 0 
			&& (FRAGMENTATION_UNIT_A != pNALUHeader->type  || (FRAGMENTATION_UNIT_A == pNALUHeader->type && !pFUHeader && pFUHeader->start_bit) ) )
		{
			m_pFrameEndPos = m_pRTPCurPos;
			m_bIsVFrameDistribute = true;
		}

		if (m_bIsVFrameDistribute)
		{
			m_bIsVFrameDistribute = false;
			if(!DistributeFrame(pRTPHeader->payloadtype, m_bForceFrameOnSameTimestamp ))
			{
				printf("channel ret DistributeFrame %d,%d %s %d err\n", 
					m_iChannelNum, m_subChnnNum, __FILE__, __LINE__);
				IsFalse = true;
				m_bForceFrameOnSameTimestamp = false;
				break;
			}

			m_iNonIntervalPackCnt = 0;
			m_bForceFrameOnSameTimestamp = false;
		}

		if (FRAGMENTATION_UNIT_A == pNALUHeader->type)//FU_A  non_interleaved不支持FU_B
		{
			if (!pFUHeader)
			{
				printf("channel pFUHeader %d %s %d err\n", m_iChannelNum, __FILE__, __LINE__);
				IsFalse = true;

				m_ulLastError |= ONVIF_ERROR_NO_FUHEADER;
				break;
			}

			if( RealSendLen < 2 )
			{
				m_ulLastError |= ONVIF_ERROR_NO_FUHEADER;
				return 0;
			}

			offset += 2;
			RealSendLen -= 2;

			if( SLICE_LAYER_WITH_PARTITIONING_RBSP == pFUHeader->type 
				&& pFUHeader->start_bit )
			{
				m_iCheckSpsCount++;
				if( m_iCheckSpsCount > 1 && !m_bIsHasSpsInfo )
				{
					if( m_SpsPpsLen > 0 )
					{
						memcpy(m_pRTPCurPos, m_pSpsPps, m_SpsPpsLen);
						m_pRTPCurPos += m_SpsPpsLen;

						if( !m_bIsKeyFrame )
						{
							m_bIsKeyFrame = true;
						}
					}
				}
			}

			if (pFUHeader->start_bit || 0 == m_iNonIntervalPackCnt )
			{
				if (!m_bFrameDistributeNeedWait)
				{
					m_pFrameStartPos = m_pRTPCurPos;
				}

				memcpy(m_pRTPCurPos, RTP_START_CODE, 4);
				m_pRTPCurPos += 4;
				FU_A_NAL = (pNALUHeader->nri)<<5 | (pNALUHeader->forbidden_zero_bit)<<6  | pFUHeader->type;
				memcpy(m_pRTPCurPos, &FU_A_NAL, 1);
				m_pRTPCurPos += 1;
			}
			else if (pFUHeader->end_bit || pRTPHeader->marker)
			{	
// 				if(m_pFrameStartPos == NULL || m_pFrameEndPos != NULL)
// 				{
// 					IsFalse = true;
// 					break;
// 				}
				m_pFrameEndPos = m_pRTPCurPos + RealSendLen;
				m_bIsVFrameDistribute = true;
				m_bForceFrameOnSameTimestamp = true;
			}
			else//中间的包，不处理
			{

			}

			//printf( "---%d,%d,%d  %d,%d\n", pFUHeader->start_bit, pFUHeader->end_bit, pFUHeader->type, m_iChannelNum, m_subChnnNum );

			m_iNonIntervalPackCnt += RealSendLen;
		}
		else if(1 <= pNALUHeader->type && 23 >= pNALUHeader->type)
		{
			if (SEQ_PARAMETER_SET_RBSP == pNALUHeader->type)
			{
				m_bIsKeyFrame = true;
				unsigned short width, height;
 				ParseSPS((unsigned char*)(pData + offset + 1), RealSendLen, width, height);
				if (m_Width != width || m_Height != height)
				{
					printf("current channel %d %d m_Width, m_Height %d %d   Width, Height %d %d\n", 
						m_iChannelNum, m_subChnnNum, m_Width, m_Height, width, height);
					m_Width = width;
					m_Height = height;
				}

				m_bIsHasSpsInfo = true;

				if( m_Width <= 144 || m_Height < 96 )
				{
					m_bIsKeyFrame = false;
					m_bIsHasSpsInfo = false;
					IsFalse = true;
					bIsVideoData = true;

					printf( "current widht height ------------ too small %d,%d\n", m_Width, m_Height );
					break;
				}
			}

			if (!m_bFrameDistributeNeedWait)
			{
				m_pFrameStartPos = m_pRTPCurPos;
			}

			memcpy(m_pRTPCurPos, RTP_START_CODE, 4);
			m_pRTPCurPos += 4;

			m_pFrameEndPos = m_pRTPCurPos + RealSendLen;
			m_bIsVFrameDistribute = true;	

			m_iNonIntervalPackCnt = 0;
		}
		else if(SINGLE_TIME_AGGREGATION_PACKET_A == pNALUHeader->type)// non-interleaved 只有STAP-A
		{
			//待完善
			unsigned short NALUnSize;
			int cur_data_len = (int)RealSendLen;
			char* pRtpTmpSavePos = m_pRTPCurPos;
			char* pNALUDataAddr = (char*)pData+offset+1;
			bool bErrorProc = false;
			
			cur_data_len -= 1;
			while( cur_data_len > 0 )
			{
				memcpy(pRtpTmpSavePos, RTP_START_CODE, 4);
				pRtpTmpSavePos += 4;

				NALUnSize = ntohs((unsigned short)(*(pNALUDataAddr)));
				if( NALUnSize > 0 && NALUnSize < cur_data_len )
				{
					memcpy( pRtpTmpSavePos, pNALUDataAddr+2, NALUnSize );
					
					pRtpTmpSavePos += NALUnSize;
					pNALUDataAddr += 2 + NALUnSize;

					cur_data_len -= 2 + NALUnSize;
				}
				else
				{
					bErrorProc = true;
					break;
				}
			}

			if( !bErrorProc )
			{
				if (!m_bFrameDistributeNeedWait)
				{
					m_pFrameStartPos = m_pRTPCurPos;
				}

				m_pFrameEndPos = m_pRTPCurPos + (pRtpTmpSavePos - m_pRTPCurPos);
				m_pRTPCurPos = pRtpTmpSavePos;

				m_bIsVFrameDistribute = true;

				return 0;
			}
			else
			{
				printf("SINGLE_TIME_AGGREGATION_PACKET_A %d, %s %d err\n", m_iChannelNum, __FILE__, __LINE__);
				IsFalse = true;
				m_ulLastError |= ONVIF_ERROR_RTP_STA_B;
			}

			m_iNonIntervalPackCnt = 0;

			break;
		}
		else if( NAL_UNIT_TYPE_ZERO == pNALUHeader->type
			|| NAL_UNIT_TYPE_30VAL == pNALUHeader->type 
			|| NAL_UNIT_TYPE_31VAL == pNALUHeader->type )
		{
			m_iNonIntervalPackCnt = 0;
			return 0;
			break;
		}
		else
		{
			IsFalse = true;

			m_iNonIntervalPackCnt = 0;

			m_ulLastError |= ONVIF_ERROR_RTP_STA_C;

			printf("extern pNALUHeader->type = %d (%d,%d) %s %d err\n", 
				pNALUHeader->type, m_iChannelNum, m_subChnnNum, __FILE__, __LINE__);
			break;
		}
		
		memcpy(m_pRTPCurPos, pData + offset, RealSendLen);
		m_pRTPCurPos += RealSendLen;

		offset += RealSendLen;
		if (len < offset)
		{
			IsFalse = true;
			printf("lens %d,%d,%d  %s %d err\n", 
				m_iChannelNum, len, offset, __FILE__, __LINE__);
			break;
		}

		RemainLen = 0;

	}//end of while

	return RemainLen;
}

bool CClientRTP::DistributeFrame(int streamType, bool bForceFrame )
{
	RTSP_DATA_FRAME *pDataFrame = NULL;
	int ret_val;

	//if(RTP_PAYLOAD_TYPE_H264 == streamType || RTP_PAYLOAD_TYPE_PS == streamType)
	if( m_video_payload == streamType )
	{
		if (m_pFrameStartPos == NULL || m_pFrameEndPos == NULL || m_pFrameEndPos <= m_pFrameStartPos)
		{
			//printf("current video no keyframe error %d,%d (%x,%x), %s %d err\n", 
			//	m_iChannelNum, m_subChnnNum, (int)m_pFrameStartPos, (int)m_pFrameEndPos, __FILE__, __LINE__);

			if (m_pFrameStartPos == NULL)//有可能第一帧传的不是关键帧，这时候忽略
			{
				ResetVideoBufferParamInfo();

				return true;
			}
			else
			{
				return false;
			}
		}

		pDataFrame = (RTSP_DATA_FRAME *)(m_pRTPRecvRam);
		pDataFrame->pData = (unsigned char*)pDataFrame;
		pDataFrame->streamID = m_TVTSessionID;
		pDataFrame->frame.length = m_pFrameEndPos - m_pFrameStartPos;
		pDataFrame->length = pDataFrame->frame.length + sizeof(RTSP_DATA_FRAME);
		pDataFrame->frame.bKeyFrame = m_bIsKeyFrame;

		unsigned char *pCheck = pDataFrame->pData + sizeof(RTSP_DATA_FRAME);
		if (*pCheck!=0x00 || *(pCheck+1)!=0x00 || *(pCheck+2)!=0x00 || *(pCheck+3)!=0x01)
		{
			printf("!**! %s %d frame err   ssrc:%lu\n", __FILE__, __LINE__, m_LastRTPHeader[STREAM_LIVE_VIDEO].ssrc);
			return false;
		}

		pDataFrame->frame.width = m_Width;
		pDataFrame->frame.height = m_Height;

		if (m_Height == 1088 || m_Height == 1072)
		{
			pDataFrame->frame.height = 1080;
		}
		
		pDataFrame->frame.encodeType = streamType;
		pDataFrame->frame.streamType = RTSP_STREAM_TYPE_VIDEO_1;

		m_Lock.Lock();
		if (m_bIsFirstDistribute[STREAM_LIVE_VIDEO] || m_bSystemTimeChanged[STREAM_LIVE_VIDEO])
		{
			m_LastFrameTime[STREAM_LIVE_VIDEO] = RTSPGetCurrTime();
			pDataFrame->frame.time = m_LastFrameTime[STREAM_LIVE_VIDEO];
			m_LastVFrameTs = m_LastRTPHeader[STREAM_LIVE_VIDEO].timeStamp;

			RTSP_DATE_TIME tmpCmp;

			tmpCmp.seconds = 0;
			tmpCmp.microsecond = 0;
			if( tmpCmp == m_stCheckRtspDatatime )
			{
				m_stCheckRtspDatatime = pDataFrame->frame.time;
			}
			else if( m_bSystemTimeChanged[STREAM_LIVE_VIDEO] )
			{
				m_stCheckRtspDatatime = pDataFrame->frame.time;
			}

			m_stSaveResyncTimes = m_LastFrameTime[STREAM_LIVE_VIDEO];

			m_bSystemTimeChanged[STREAM_LIVE_VIDEO] = false;
			m_bIsFirstDistribute[STREAM_LIVE_VIDEO] = false;

			//认为开始帧
			if (m_LastVFrameTs == m_LastRTPHeader[STREAM_LIVE_VIDEO].timeStamp)
			{
				m_bFrameDistributeNeedWait = true;

				m_Lock.UnLock();

				return true;	
			}
			m_bFrameDistributeNeedWait = false;

		}
		else
		{
			unsigned long tm_dec_num = 0;
			int b_resync_time_flag = 0;

			//相同timestamp认为是同一帧
			if ( !bForceFrame && m_LastVFrameTs == m_LastRTPHeader[STREAM_LIVE_VIDEO].timeStamp)
			{
				m_bFrameDistributeNeedWait = true;

				m_Lock.UnLock();

				return true;	
			}
			m_bFrameDistributeNeedWait = false;


			if( m_LastRTPHeader[STREAM_LIVE_VIDEO].timeStamp >= m_LastVFrameTs )
			{
				tm_dec_num = m_LastRTPHeader[STREAM_LIVE_VIDEO].timeStamp - m_LastVFrameTs;
				tm_dec_num = tm_dec_num * 100 / 9;
				if( tm_dec_num > 2*1000*1000 )
				{
					b_resync_time_flag = 1;
				}
				else
				{
					int sps_secs_count = 0;
					RTSP_DATE_TIME sps_time = m_LastFrameTime[STREAM_LIVE_VIDEO];

					//检测和系统时间的差异

					sps_time += tm_dec_num;
					sps_secs_count = sps_time.seconds - m_stSaveResyncTimes.seconds;
					if( sps_secs_count > 5*60 && !m_bMustCurFrameIsKeyframe )
					{
						RTSP_DATE_TIME stTmpCurTime = RTSPGetCurrTime();
						RTSP_DATE_TIME stTmpFrmTime = pDataFrame->frame.time;
						LONGLONG ullDecCount = 0;

						if( stTmpCurTime > stTmpFrmTime )
						{
							ullDecCount = stTmpCurTime.Dec( stTmpFrmTime );
						}
						else
						{
							ullDecCount = stTmpFrmTime.Dec( stTmpCurTime );
						}

						if( ullDecCount > 500000 )
						{
							b_resync_time_flag = 1;
						}
						else
						{
							m_stSaveResyncTimes = stTmpCurTime;
						}
					}
				}
			}
			else
			{
				b_resync_time_flag = 1;
				//m_bIsFirstDistribute[STREAM_LIVE_VIDEO] = true;
			}

			if( b_resync_time_flag )
			{				
				RTSP_DATE_TIME stTmpCurTime = RTSPGetCurrTime();
				long long real_time_dec = stTmpCurTime.Dec( m_LastFrameTime[STREAM_LIVE_VIDEO] );

				printf( "onvif chnn %d,%d resync time %d\n", m_iChannelNum, m_subChnnNum, tm_dec_num);

				m_LastFrameTime[STREAM_LIVE_VIDEO] = stTmpCurTime;
				m_stSaveResyncTimes = stTmpCurTime;
				tm_dec_num = 0;
			}

			m_LastFrameTime[STREAM_LIVE_VIDEO] += tm_dec_num;	//(1000000 / 90000)
			pDataFrame->frame.time = m_LastFrameTime[STREAM_LIVE_VIDEO];
		}
		m_Lock.UnLock();

		m_LastVFrameTs = m_LastRTPHeader[STREAM_LIVE_VIDEO].timeStamp;

		//处理时间跳变问题
		if( m_stCheckRtspDatatime >= pDataFrame->frame.time )
		{
			m_stCheckRtspDatatime += 20*1000;
			pDataFrame->frame.time = m_stCheckRtspDatatime;
			m_iPrintVideoSyncCnt++;
		}
		else if( m_iPrintVideoSyncCnt > 0 )
		{
			if( m_iPrintVideoSyncCnt > 1 )
			{
				printf( " onvif video chnn %d,%d resync time %d\n", m_iChannelNum, m_subChnnNum, m_iPrintVideoSyncCnt );
			}
			m_iPrintVideoSyncCnt = 0;
		}
		m_stCheckRtspDatatime = pDataFrame->frame.time;

		//printf("frame time : %d  %d\n", pDataFrame->frame.time.seconds, pDataFrame->frame.time.microsecond);

#ifdef Test_SaveData
  		if (1 != fwrite(pDataFrame->pData+sizeof(RTSP_DATA_FRAME), pDataFrame->frame.length, 1, m_file))
  		{
  			printf("%s %d err\n", __FILE__, __LINE__);
  			return false;
  		}
#endif

		if( m_bMustCurFrameIsKeyframe )
		{
			if( m_bIsKeyFrame )
			{
				m_bMustCurFrameIsKeyframe = false;
				m_bFirstRtpHeader =false;
			}
			else
			{
				ResetVideoBufferParamInfo();

				return true;
			}
		}

		if (true == m_bIsKeyFrame)
		{
			m_bIsKeyFrame = false;
		}

		m_pFrameStartPos = NULL;
		m_pFrameEndPos = NULL;
		m_pRTPCurPos = m_pRTPRecvBuf;
	}
	//else if(RTP_PAYLOAD_TYPE_G711_U == streamType || RTP_PAYLOAD_TYPE_G711_A == streamType)
	else if(m_audio_payload == streamType && m_audio_data_type != (unsigned int)-1 )
	{
		pDataFrame = (RTSP_DATA_FRAME *)(m_pRTPARecvRam);
		pDataFrame->pData = (unsigned char*)pDataFrame;
		pDataFrame->streamID = m_TVTSessionID;
		pDataFrame->frame.length = m_RTPAudioFrameLens;
		pDataFrame->length = pDataFrame->frame.length + sizeof(RTSP_DATA_FRAME);
		pDataFrame->frame.encodeType = m_audio_data_type; //streamType;
		pDataFrame->frame.streamType = RTSP_STREAM_TYPE_AUDIO;

		if( 0 != (pDataFrame->frame.length%RTP_AUDIO_G711_LENS))
		{
			return true;
		}

		if( m_audio_frame_data_len != pDataFrame->frame.length )
		{
			//printf( "pDataFrame->frame.length audio == %d,%d,%d -- %d,%d\n", 
			//	pDataFrame->frame.length, m_audio_frame_data_len, m_audio_data_type,
			//	m_iChannelNum, m_subChnnNum );
			m_audio_frame_data_len = pDataFrame->frame.length;
		}

		m_Lock.Lock();
		if (m_bIsFirstDistribute[STREAM_LIVE_AUDIO]|| m_bSystemTimeChanged[STREAM_LIVE_AUDIO])
		{
			m_bSystemTimeChanged[STREAM_LIVE_AUDIO] = false;
			m_bIsFirstDistribute[STREAM_LIVE_AUDIO] = false;
			m_LastFrameTime[STREAM_LIVE_AUDIO] = RTSPGetCurrTime();
			m_stAudioChkFrmTime = m_LastFrameTime[STREAM_LIVE_AUDIO];
			pDataFrame->frame.time = m_LastFrameTime[STREAM_LIVE_AUDIO];

			m_iAudioSpsFrmCnt = 0;
		}
		else
		{
			m_LastFrameTime[STREAM_LIVE_AUDIO] += (m_LastRTPHeader[STREAM_LIVE_AUDIO].timeStamp - m_LastButOneRTPHeader[STREAM_LIVE_AUDIO].timeStamp) \
				* 125;		//(1000000 / 8000)
			pDataFrame->frame.time = m_LastFrameTime[STREAM_LIVE_AUDIO];

			m_iAudioSpsFrmCnt++;
			if( m_iAudioSpsFrmCnt > 2*60*25 )
			{
				RTSP_DATE_TIME stTmpCurTime = RTSPGetCurrTime();
				RTSP_DATE_TIME stTmpFrmTime = pDataFrame->frame.time;
				LONGLONG ullDecCount = 0;

				if( stTmpCurTime > stTmpFrmTime )
				{
					ullDecCount = stTmpCurTime.Dec( stTmpFrmTime );
				}
				else
				{
					ullDecCount = stTmpFrmTime.Dec( stTmpCurTime );
				}

				if( ullDecCount > 1000000 )
				{
					m_LastFrameTime[STREAM_LIVE_AUDIO] = RTSPGetCurrTime();
					pDataFrame->frame.time = m_LastFrameTime[STREAM_LIVE_AUDIO];
				}

				m_iAudioSpsFrmCnt = 0;
			}

			//处理时间跳变问题
			if( m_stAudioChkFrmTime >= pDataFrame->frame.time )
			{
				m_stAudioChkFrmTime += 20*1000;
				pDataFrame->frame.time = m_stAudioChkFrmTime;
				m_iPrintAudioSyncCnt++;
			}
			else if( m_iPrintAudioSyncCnt > 0 )
			{
				if( m_iPrintAudioSyncCnt > 1 )
				{
					printf( " onvif aduio chnn %d,%d resync time %d\n", m_iChannelNum, m_subChnnNum, m_iPrintAudioSyncCnt );
				}
				m_iPrintAudioSyncCnt = 0;
			}
			m_stAudioChkFrmTime = pDataFrame->frame.time;
		}
		m_Lock.UnLock();

		//pDataFrame->frame.localTime = pDataFrame->frame.time.LocalTime();
	}
	else
	{
		//printf("%s %d err\n", __FILE__, __LINE__);
		//return false;
		return true;
	}


	if( 0 == ( (m_iVideoPrintCount+1) % 10000) 
		|| 0 == ( (m_iAudioPrintCount+1) % 10000) )
	{
		printf( "send frame data (%d,%d),%d,%d-- %d,%d\n", 
			m_iChannelNum, m_subChnnNum, pDataFrame->length, streamType, 
			m_iVideoPrintCount, m_iAudioPrintCount  );
	}

	if( m_video_payload == streamType )
	{
		m_Lock.Lock();
		ret_val = m_pCallBack(pDataFrame, m_TVTSessionID, m_pParam);
		m_Lock.UnLock();


#if 0
		if( 1 == m_subChnnNum && 0 == m_iChannelNum )
		{
			printf( "current frame length = %d\n", pDataFrame->frame.length );
		}

		if( 0 ) //1 == m_subChnnNum && 0 == m_iChannelNum )
		{
			static long long bkllprevtime = 0;
			static long lprevtimestamp = 0;
			static long long bksystimexy = 0;
			
			RTSP_DATE_TIME stxxxxx = RTSPGetCurrTime();
			long long llcurtimes = pDataFrame->frame.time.seconds;
			long curtimestamp = m_LastVFrameTs;
			long long xxyy = stxxxxx.seconds;

			llcurtimes *= 1000000;
			llcurtimes += pDataFrame->frame.time.microsecond;

			xxyy *= 1000000;
			xxyy += stxxxxx.microsecond;

			if( llcurtimes - bkllprevtime > 42*1000 || ((curtimestamp - lprevtimestamp) > 180000) )
			{
				printf( "chnn time error %16llu, %16d, %16llu, %16d\n",
					llcurtimes - bkllprevtime, 
					curtimestamp - lprevtimestamp, 
					xxyy - bksystimexy,
					pDataFrame->frame.length );
			}
			bkllprevtime = llcurtimes;
			lprevtimestamp = curtimestamp;
			bksystimexy = xxyy;
		}
#endif

		if( 0 != ret_val )
		{
			//printf( "current must keyframe ret = %d, %d, %s,%d\n", 
			//	ret_val, m_iChannelNum, __FILE__, __LINE__ );

			m_iCheckAudioFrameSendContinue = 8;
			//return false;
		}
		else
		{
			m_iCheckAudioFrameSendContinue = 0;
		}
		++m_iVideoPrintCount;
		ReAllocForDecBuf(pDataFrame->length);
	}
	else if( m_audio_payload == streamType )
	{
		if( !m_bMustCurFrameIsKeyframe 
			&& (++m_iCheckAudioFrameSendContinue) < 16 )
		{
			m_Lock.Lock();
			ret_val = m_pCallBack(pDataFrame, m_TVTSessionID, m_pParam);
			m_Lock.UnLock();

			++m_iAudioPrintCount;
		}
	}

	return true;
}

int CClientRTP::SendRTCPData(char *pData, unsigned long &len)
{
	int ret;

	m_Lock.Lock();
	ret = SendUnLockRTCPData( pData, len );
	m_Lock.UnLock();

	return ret;

}

int CClientRTP::SendUnLockRTCPData(char *pData, unsigned long &len)
{
	int ret = 0;
	unsigned long tmpLen = 0;
	INTERLEAVED_INFO *Interleaved = (INTERLEAVED_INFO *)pData;
	Interleaved->mark = INTERLEAVED_DOLLAR_SIGN;


	if (m_bHasAudio && m_bHasVideo)
	{
		if (m_bRTCPSendControl)
		{
			m_bRTCPSendControl ^= 1;
			Interleaved->channel = 0x01;
			CreateVideoRR(pData+sizeof(INTERLEAVED_INFO), tmpLen);
			len = tmpLen;
			CreateSDES(pData+len+sizeof(INTERLEAVED_INFO), tmpLen, m_RR[STREAM_LIVE_VIDEO].rtporterSSRC);
			len += tmpLen;
			Interleaved->datalength = htons(len);
			len += 4;
		}
		else
		{
			m_bRTCPSendControl ^= 1;
			Interleaved->channel = 0x03;
			CreateAudioRR(pData+sizeof(INTERLEAVED_INFO), tmpLen);
			len = tmpLen;
			CreateSDES(pData+len+sizeof(INTERLEAVED_INFO), tmpLen, m_RR[STREAM_LIVE_AUDIO].rtporterSSRC);
			len += tmpLen;
			Interleaved->datalength = htons(len);
			len += 4;
		}

	}
	else if (m_bHasAudio)
	{
		Interleaved->channel = 0x01;
		CreateAudioRR(pData + sizeof(INTERLEAVED_INFO), tmpLen);
		len = tmpLen;
		Interleaved->datalength = htons(len);
		len += 4;
	}
	else if (m_bHasVideo)
	{
		Interleaved->channel = 0x01;
		CreateVideoRR(pData + sizeof(INTERLEAVED_INFO), tmpLen);
		len = tmpLen;
		Interleaved->datalength = htons(len);
		len += 4;
	}
	else
	{
		printf("%s %d err\n", __FILE__, __LINE__);
		pData = NULL;
		len = 0;
		ret = -1;
		return ret;
	}

	return ret;
}


void CClientRTP::CreateAudioRR(char *pBuf, unsigned long &len)
{
	m_RTCPHeader[STREAM_LIVE_AUDIO].version = 0x02;
	m_RTCPHeader[STREAM_LIVE_AUDIO].padding = 0;
	m_RTCPHeader[STREAM_LIVE_AUDIO].itemCount = 0;
	m_RTCPHeader[STREAM_LIVE_AUDIO].packetType = RTCP_PACKET_RR;
	m_RTCPHeader[STREAM_LIVE_AUDIO].length = htons(1);

	unsigned long ssrc = htonl(m_RR[STREAM_LIVE_AUDIO].rtporterSSRC);
		

	len = sizeof(RTCP_HEADER) + sizeof(unsigned long);

	memcpy(pBuf, &m_RTCPHeader[STREAM_LIVE_AUDIO], sizeof(RTCP_HEADER));
	memcpy(pBuf+sizeof(RTCP_HEADER), &ssrc, sizeof(unsigned long));
}


void CClientRTP::CreateVideoRR(char *pBuf, unsigned long &len)
{
	RTCP_RECEIVER_REPORTS *pTmpRR;
	unsigned char type = STREAM_LIVE_VIDEO;

	m_RTCPHeader[type].padding = 0;
	m_RTCPHeader[type].itemCount = 1;
	m_RTCPHeader[type].packetType = RTCP_PACKET_RR;
	m_RTCPHeader[type].length = htons(7);

	m_RR[type].rtporteeSSRC = m_LastRTPHeader[type].ssrc;
	m_RR[type].fractionLost = 0;
	m_RR[type].cumulativeLost = 0;
	m_RR[type].extendSeqNum.HighestSequenceNum = m_LastRTPHeader[type].sequenceNumber;

	m_RR[type].interarrivalJitter = m_LastJitter[type] + \
		(abs((m_LastRTPHeader[type].timeStamp - m_LastPacketTime[type]) \
		- (m_LastButOneRTPHeader[type].timeStamp - m_LastButOnePacketTime[type])) \
		- m_LastJitter[type])>>4;
	if (0 == m_RR[type].interarrivalJitter)
	{
		m_RR[type].interarrivalJitter = 192;
	}
	m_LastJitter[type] = m_RR[type].interarrivalJitter;
	m_RR[type].lsrNTPH = m_lastSR[type].rtpTimestamp>>16;
	m_RR[type].lsrNTPL = m_lastSR[type].rtpTimestamp;
	m_RR[type].delaySinceLSR = (m_LastRecvSRTimeSec[type] == 0) ? 0 : \
		((RTSPGetCurrTime().seconds - m_LastRecvSRTimeSec[type]) + (RTSPGetCurrTime().microsecond - m_LastRecvSRTimeUSec[type])<<16) * 65535;

	pTmpRR = (RTCP_RECEIVER_REPORTS *)(pBuf + sizeof(RTCP_HEADER));
	pTmpRR->rtporterSSRC = htonl(m_RR[type].rtporterSSRC);
	pTmpRR->rtporteeSSRC = htonl(m_RR[type].rtporteeSSRC);
	pTmpRR->fractionLost = m_RR[type].fractionLost;
	pTmpRR->cumulativeLost = (m_RR[type].cumulativeLost & 0x0000ff)<<16 | (m_RR[type].cumulativeLost & 0xffff00)>>8;//24位转化为网络字节序
	pTmpRR->extendSeqNum.CyclesCount = htons(m_RR[type].extendSeqNum.CyclesCount);
	pTmpRR->extendSeqNum.HighestSequenceNum = htons(m_RR[type].extendSeqNum.HighestSequenceNum);
	pTmpRR->interarrivalJitter = htonl(m_RR[type].interarrivalJitter);
	pTmpRR->lsrNTPH = htons(m_RR[type].lsrNTPH);
	pTmpRR->lsrNTPL = htons(m_RR[type].lsrNTPL);
	pTmpRR->delaySinceLSR = htonl(m_RR[type].delaySinceLSR);

	len = sizeof(RTCP_HEADER) + sizeof(RTCP_RECEIVER_REPORTS);

	memcpy(pBuf, &m_RTCPHeader[type], sizeof(RTCP_HEADER));
	//memcpy(pData+sizeof(INTERLEAVED_INFO)+sizeof(RTCP_HEADER), &m_RR, sizeof(RTCP_RECEIVER_REPORTS));
}


void CClientRTP::CreateSDES(char *pBuf, unsigned long &len, unsigned long csrc)
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
		printf("%s %d err\n", __FILE__, __LINE__);
		len = 0;
		return;
	}
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

unsigned long CClientRTP::HandleRTCP(const char *pData, const unsigned long len, unsigned char InterleavedChannel)
{

	return 0;
}

void CClientRTP::HandleConnectionBreak()
{
	//通知上层数据流中断了
	m_Lock.Lock();
	if (NULL != m_pCallBack)
	{
		m_pCallBack(NULL, m_TVTSessionID, m_pParam);
	}
	m_Lock.UnLock();
}

void CClientRTP::HandleSystemTimeChanged()
{
	m_bSystemTimeChanged[STREAM_LIVE_VIDEO] = true;
	m_bSystemTimeChanged[STREAM_LIVE_AUDIO] = true;

	printf( "HandleSystemTimeChanged %d reset system time \n", m_iChannelNum  );
}

void CClientRTP::SetChannelNumber(int chnn, int sub_chn)
{
	m_iChannelNum = chnn;
	m_subChnnNum = sub_chn;
}

void CClientRTP::TellNextDataIsError( bool bDataErr )
{
	m_bSessionTellDataError = bDataErr;
}

bool CClientRTP::GetIsMustResyncSystem()
{
	bool bSync = m_bNvrIpcSystimeResync;

	m_bNvrIpcSystimeResync = false;

	return bSync;
}

void CClientRTP::GetLastRTPHeaderInfo(RTP_HEADER* pRTPInfo )
{
	memcpy(  pRTPInfo, &m_stSavePrevRTPHeader, sizeof( RTP_HEADER) );
}

bool CClientRTP::SetSpsPps( const char *pData, int len )
{
	if (!pData || len <= 0)
	{
		return false;
	}
	if (m_pSpsPps)
	{
		delete m_pSpsPps;
	}
	m_pSpsPps = new char[len];
	m_SpsPpsLen = len;
	memcpy(m_pSpsPps, pData, len);
}


