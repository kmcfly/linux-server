#include "ClientRTSPSession.h"


CClientRTSPSession::CClientRTSPSession()
{
	ResetObject();
}

CClientRTSPSession::~CClientRTSPSession()
{

}

bool CClientRTSPSession::ResetObject(void)
{
	m_bHasSetRTP = false;
	m_RecvWaitTime = 0;
	m_bFirstRtspDataFlag = true;

	m_bIMSendPing = false;

	m_ulLastError = 0;

	p_ulCallBackError = NULL;
	m_run_lines = 0;

	m_cRtspAsPingReplayLen = 0;
	m_cRtspAsPingReplayTmp = 0;
	m_cRtspAsPingRelayMemcpyLen = 0;
	memset( m_cRtspAsPingReplayStr, 0, 256 );

	m_iFirstCheckDataError = 0;

	m_RTSP.ResetObject();
	m_RTP.ResetObject();

	return true;
}

void CClientRTSPSession::Initial()
{
	
}

void CClientRTSPSession::SetChannelNumber(int chnn, int sub_chn )
{
	m_channel_num = chnn;
	m_sub_chn_num = sub_chn;

	m_RTP.SetChannelNumber(chnn, sub_chn);
	m_RTSP.SetChannelNumber(chnn, sub_chn);
}

void CClientRTSPSession::SetAudioEnable( bool bAudioEnable )
{
	m_RTSP.SetAudioEnableFlag( bAudioEnable );
}

int CClientRTSPSession::ParseURL(char const* url, char* username, char* password, char* address,int &portNum) 
{
	if (NULL == url || NULL == username || NULL == password || NULL == address)
	{
		return -1;
	}
	
	do{
		// Parse the URL as "rtsp://[<username>[:<password>]@]<server-address-or-name>[:<port>][/<stream-name>]"
		char const* prefix = "rtsp://";
		unsigned const prefixLength = 7;
		unsigned const parseBufferSize = 100;
		char const* from = &url[prefixLength];

		// Check whether "<username>[:<password>]@" occurs next.
		// We do this by checking whether '@' appears before the end of the URL, or before the first '/'.
		char const* colonPasswordStart = NULL;
		char const* p;

		for (p = from; *p != '\0' && *p != '/'; ++p) 
		{
			if (*p == ':' && colonPasswordStart == NULL) 
			{
				colonPasswordStart = p;
			} 
			else if (*p == '@') 
			{
				if (colonPasswordStart == NULL) 
				{
					colonPasswordStart = p;
				}

				char const* usernameStart = from;
				unsigned usernameLen = colonPasswordStart - usernameStart;

				for (unsigned i = 0; i < usernameLen; ++i) 
				{
					username[i] = usernameStart[i];
				}
				username[usernameLen] = '\0';

				char const* passwordStart = colonPasswordStart;
				if (passwordStart < p) 
				{
					++passwordStart; // skip over the ':'
				}

				unsigned passwordLen = p - passwordStart;
				
				for (unsigned j = 0; j < passwordLen; ++j)
				{
					password[j] = passwordStart[j];
				}
				password[passwordLen] = '\0';
				from = p + 1; // skip over the '@'
				break;
			}
		}

		// Next, parse <server-address-or-name>
		char* to = address;//parseBuffer[0];
		unsigned i;
		for (i = 0; i < parseBufferSize; ++i) 
		{
			if (*from == '\0' || *from == ':' || *from == '/') 
			{
				// We've completed parsing the address
				*to = '\0';
				break;
			}
			*to++ = *from++;
		}
		if (i == parseBufferSize)
		{
			printf("URL is too long");
			break;
		}

		char nextChar = *from;
		int portNumInt = 554;
		if (nextChar == ':') 
		{
			if (sscanf(++from, "%d", &portNumInt) != 1)
			{
				printf("%s %s %d Bad Port Num", __FILE__, __FUNCTION__, __LINE__);
				return -1;
			}
			if (portNumInt < 1 || portNumInt > 65535) 
			{
				printf("%s %s %d Bad Port Num", __FILE__, __FUNCTION__, __LINE__);
				return -1;
			}
		}
		portNum = portNumInt;
		return 1;
	}while (0);

	return -1;
}


int	CClientRTSPSession::Start(const char *pURL, const char *pUserName, const char *pPassWd)
{
	m_RTSP.CreateFirstCommand(pURL, pUserName, pPassWd);
	m_LastGetParaTime = RTSPGetCurrTime();
	m_LastRTCPTime = m_LastGetParaTime;
	return 0;
}

int CClientRTSPSession::Stop()
{
	m_Lock.Lock();
	m_RTP.Stop();
	m_Lock.UnLock();

	if( p_ulCallBackError )
	{
		*p_ulCallBackError = GetLastError();
	}

	return 0;
}

void CClientRTSPSession::SetCallBack(RTP_CALLBACK pCallBack, void *pParam, int TVTSessionID)
{
	m_RTP.SetCallBack(pCallBack, pParam, TVTSessionID);
}

void CClientRTSPSession::SetFuncProcCallback(PRTP_FUNC_PROC_CALLBACK pFuncProcCallBack, void *pParam)
{
	m_RTP.SetFuncProcCallback( pFuncProcCallBack, pParam );
}

void CClientRTSPSession::SetGetIpcUsedInfoCallback(PGET_IPC_USED_INFO pCallfn, void *pParam )
{
	m_RTSP.SetGetIpcInfoCallback( pCallfn, pParam );
}

int CClientRTSPSession::CheckErrorDataIsLastFrameNext( const char* pBuffer, int len, bool& bFindDataFlag )
{
	int idx;
	int errPos = len;
	unsigned char* pDataTmp = (unsigned char*)pBuffer;
	INTERLEAVED_INFO stTmpInterInfo;
	RTP_HEADER stTmpRtpHeader;
	RTP_HEADER stTmpLastRtpH;
	int video_payload = m_RTP.GetCurVideoPayload();
	unsigned long ulRetValLen;
	int cur_max_ck_num = errPos > RTP_MTU ? RTP_MTU : errPos;

	cur_max_ck_num -= sizeof(INTERLEAVED_INFO) + sizeof(RTP_HEADER);
	for( idx = 0; idx < cur_max_ck_num; idx++ )
	{
		if (INTERLEAVED_DOLLAR_SIGN == (unsigned char)*pDataTmp )
		{
			memcpy( &stTmpInterInfo, pDataTmp, sizeof(INTERLEAVED_INFO));
			memcpy( &stTmpRtpHeader, pDataTmp + sizeof(INTERLEAVED_INFO), sizeof(RTP_HEADER));

			ulRetValLen = ntohs(stTmpInterInfo.datalength);
			m_ulLastError |= ONVIF_ERROR_DATA_FLAG;

			if( stTmpInterInfo.channel >= 0 && stTmpInterInfo.channel <= 3 
				&& 2 == stTmpRtpHeader.version && video_payload == stTmpRtpHeader.payloadtype )
			{
				m_RTP.GetLastRTPHeaderInfo( &stTmpLastRtpH );
				ulRetValLen = ntohs(stTmpRtpHeader.sequenceNumber);
				if( ulRetValLen < 65535 )
				{
					ulRetValLen++;
				}
				else 
				{
					ulRetValLen = 0;
				}

				if( (unsigned short)ulRetValLen != stTmpLastRtpH.sequenceNumber )
				{
					ulRetValLen = (int)pDataTmp - (int)pBuffer;
					bFindDataFlag = true;
					return ulRetValLen;
				}

				ulRetValLen = (int)pDataTmp - (int)pBuffer;

				return ulRetValLen;

			}
		}
		pDataTmp++;
	}

	return -1;

}

int CClientRTSPSession::GetData(const char* pData, unsigned long len, unsigned long recv_max_buf_len)
{
/*
判断data该往哪个模块传
思想：判断第一个字节是否为0x24 
若是则记录INTERLEAVED_INFO中的长度,RTP数据接收到此长度为止
若不是0x24，则为RTSP数据，若是继续记录INTERLEAVED_INFO长度
*/

	unsigned long remainLen;
	unsigned long packetLen;
	INTERLEAVED_INFO *pInterleaved;
	INTERLEAVED_INFO stCurInterH;
	const char *pPos = pData;

	remainLen = len;
	while( remainLen >= 4 )
	{
		if (INTERLEAVED_DOLLAR_SIGN == (unsigned char)*pPos)
		{
			m_bFirstRtspDataFlag = true;
			m_iFirstCheckDataError = 0;

			if( m_cRtspAsPingReplayTmp > 0 )
			{
				m_cRtspAsPingReplayLen = m_cRtspAsPingReplayTmp;
				m_cRtspAsPingReplayTmp = 0;
			}

			if (remainLen < 4)
			{
				return len - remainLen;
			}

			memcpy( &stCurInterH, pPos, sizeof(INTERLEAVED_INFO) );
			pInterleaved = (INTERLEAVED_INFO *)&stCurInterH;
			packetLen = ntohs(pInterleaved->datalength);
			if (remainLen - sizeof(INTERLEAVED_INFO) < packetLen)
			{
				if( len != remainLen 
					|| len < recv_max_buf_len )
				{
					return len - remainLen;
				}

				printf( "current data len too bigs error %d, %d, %d\n", 
					m_channel_num, packetLen, len );

				m_bIMSendPing = true;
				m_RTP.TellNextDataIsError(true);

				return len;
			}

			pPos += sizeof(INTERLEAVED_INFO);
			remainLen -= sizeof(INTERLEAVED_INFO);

			m_Lock.Lock();
			int ret = m_RTP.GetRTPData(pPos, packetLen, pInterleaved->channel, false );
			m_Lock.UnLock();

			if (packetLen != ret)
			{
				// 当前数据错误。
				printf( "current GetRTPData channel %d, packlen %d  \n", m_channel_num, ret );

				m_bIMSendPing = true;
				m_RTP.TellNextDataIsError(true);

				//return len - remainLen + packetLen;
				return len;
			}

			pPos += packetLen;
			remainLen -= packetLen;

		}
		else//RTSP
		{
			//printf("\n\n1234567\r\n%s\r\n", pPos );
			unsigned long ulRtspFlag;
			int	retVal;
			bool bCurDataError = false;

			memcpy( &ulRtspFlag, pPos, sizeof(unsigned long));
			if( ( m_bFirstRtspDataFlag && 0x50535452 == ulRtspFlag ) || !m_bFirstRtspDataFlag )
			{
				m_bFirstRtspDataFlag = false;

				bool bRtspPlaying = m_RTSP.GetIsPlaying();
				if( bRtspPlaying )
				{
					if( m_cRtspAsPingReplayLen > 0 )
					{
						if( remainLen < m_cRtspAsPingReplayLen + 4 )
						{
							return len - remainLen;
						}

						if (INTERLEAVED_DOLLAR_SIGN == (unsigned char)*(pPos+m_cRtspAsPingReplayLen))
						{
							pPos += m_cRtspAsPingReplayLen;
							remainLen -= m_cRtspAsPingReplayLen;
						}
						else
						{
							m_cRtspAsPingReplayLen = 0;
							m_cRtspAsPingReplayTmp = 0;
							m_cRtspAsPingRelayMemcpyLen = 0;
						}
					}
					else
					{
						retVal = SundayChangeSearch((const unsigned char*)pPos, remainLen, (const unsigned char*)"\r\n\r\n", 4);
						if( retVal > 0 )
						{
							if( m_cRtspAsPingReplayTmp < 254 )
							{
								int tmp_cp = retVal > (254 - m_cRtspAsPingReplayTmp) ? (254 - m_cRtspAsPingReplayTmp) : retVal;
								memcpy( m_cRtspAsPingReplayStr + m_cRtspAsPingReplayTmp, pPos, tmp_cp );
								m_cRtspAsPingRelayMemcpyLen += tmp_cp;
							}
							m_cRtspAsPingReplayTmp += retVal;

							pPos += retVal;
							remainLen -= retVal;
						}
						else
						{
							if( len == remainLen )
							{
								if( remainLen > RTP_MTU )
								{
									bCurDataError = true;
								}
							}

							if( !bCurDataError )
							{
								return len - remainLen;
							}
						}
					}
				}
				else
				{
					retVal = SundayChangeSearch((const unsigned char*)pPos, remainLen, (const unsigned char*)"\r\n\r\n", 4);
					if( retVal > 0 )
					{
						int rtspLen = retVal;
						retVal = SundayChangeSearch((const unsigned char*)pPos, rtspLen, (const unsigned char*)"ength:", 6);
						if( retVal > 0 )
						{
							char con_lenth[16] = {0};
							memcpy( con_lenth, pPos + retVal - 15, 15 );
							x_strlwr( con_lenth );

							if( 0 == memcmp( con_lenth, "content-length", 14 ) )
							{
								rtspLen += atoi(pPos + retVal + 1);
								if (remainLen < rtspLen)
								{
									return len - remainLen;
								}

								m_ulLastError |= ONVIF_ERROR_HAS_ENGTH;
							}
						}

						m_Lock.Lock();
						int rtsp_ret = m_RTSP.HandleData(pPos, rtspLen);
						m_Lock.UnLock();
						if( rtsp_ret < 0 )
						{
							m_ulLastError |= ONVIF_ERROR_RTSP_PARSR;
						}

						pPos += rtspLen;
						remainLen -= rtspLen;
					}
					else
					{
						//数据没有处理
						if( len == remainLen )
						{
							if( remainLen > RTP_MTU )
							{
								bCurDataError = true;
							}
						}

						if( !bCurDataError )
						{
							return len - remainLen;
						}
					}
				}
			}
			else
			{
				bCurDataError = true;
			}

			//数据错误处理。
			if ( bCurDataError )
			{
				if( remainLen <= RTP_MTU )
				{
					return len - remainLen;
				}

				m_iFirstCheckDataError++;

				bool bOnlyFindDataFlag = false;
				int ret_find_data_flag = CheckErrorDataIsLastFrameNext( pPos, remainLen, bOnlyFindDataFlag );
				if( ret_find_data_flag > 0 )
				{
					//如果只是找到数据标志，RTP序列已经变化这认为需要重新I帧
					if( bOnlyFindDataFlag )
					{
						//如果是第一次出错，并且数据丢弃在256个字节内的可以认为不需要关键桢
						if( m_iFirstCheckDataError > 1 || ret_find_data_flag > 256 )
						{
							m_bIMSendPing = true;
							m_RTP.TellNextDataIsError(true);
						}
					}

					//printf( "cur chl %d,%d, refind flag %d,%d\n", m_channel_num, m_sub_chn_num, 
					//	(int)bOnlyFindDataFlag, ret_find_data_flag );

					pPos += ret_find_data_flag;
					remainLen -= ret_find_data_flag;
				}
				else
				{
					int to_loss_data_len = 0;
					if( m_cRtspAsPingRelayMemcpyLen > 0 )
					{
						retVal = SundayChangeSearch((const unsigned char*)pPos, remainLen, 
							(const unsigned char*)m_cRtspAsPingReplayStr, m_cRtspAsPingRelayMemcpyLen );
						if( retVal > 0 )
						{
							to_loss_data_len = retVal - m_cRtspAsPingRelayMemcpyLen;
						}
					}
					else
					{
						retVal = SundayChangeSearch((const unsigned char*)pPos, remainLen, (const unsigned char*)"RTSP", 4);
						if( retVal > 0 )
						{
							to_loss_data_len = retVal - 4;
						}
					}

					m_bIMSendPing = true;
					m_RTP.TellNextDataIsError(true);
					//重新检测
					m_bFirstRtspDataFlag = true;

					if( to_loss_data_len > 0 )
					{
						pPos += to_loss_data_len;
						remainLen -= to_loss_data_len;

						//printf( "current channel xx %d,%d lost data len %d\n", m_channel_num, m_sub_chn_num, to_loss_data_len );
					}
					else
					{
						to_loss_data_len = remainLen - RTP_MTU;

						pPos += to_loss_data_len;
						remainLen -= to_loss_data_len;

						//printf( "current channel yy %d,%d lost data len %d\n", m_channel_num, m_sub_chn_num, to_loss_data_len );

						return len - remainLen;
					}

				}
			}
		}//end of else RTSP
	}//end of while
	

	if(!m_bHasSetRTP)//RTSP得到信息后把媒体信息告诉RTP
	{
		m_Lock.Lock();
		if (RTSP_METHOD_PLAY == m_RTSP.GetCurMethod())//已经发送了 PLAY  说明已经SETUP
		{
			m_bHasSetRTP = true;
			m_RTP.SetUp(m_RTSP.IsHasVideo(), m_RTSP.IsHasAudio(), 
				m_RTSP.GetFrameRate(), m_RTSP.GetWidth(), m_RTSP.GetHeight(),
				m_RTSP.GetVideoPayloadType(), m_RTSP.GetAudioPayloadType(),
				m_RTSP.GetAudioDataType() );

			char tmp[1024] = {0};
			int tmpLen = 0;
			if (m_RTSP.GetSpsPps(tmp, tmpLen))
			{
				m_RTP.SetSpsPps(tmp, tmpLen);
			}

		}
		m_Lock.UnLock();
	}

	return len - remainLen;
}

int CClientRTSPSession::SendTearDown(char *pData)
{
	m_RTSP.CreateTeardown(pData);

	return 0;
}

int CClientRTSPSession::SendData(char* pData, unsigned long &len)
{
	int ret = -1;
	if (NULL == pData)
	{
		return ret;
	}

	m_Lock.Lock();

	do 
	{
		if (!m_RTSP.IsSendBufEmpty())
		{
			ret = m_RTSP.GetBufferData(pData, len);
		}

		if (RTSP_SESSION_PLAY == m_RTSP.GetStatus())
		{
			int seconds = RTSPGetCurrTime().seconds;

			//RTSP会告之服务器的超时时间  在超时时间之内使用无entity的Get_PARAMETER(相当于ping)
			if (m_RTSP.IsSupportGetParameter())
			{
				if ((seconds - m_LastGetParaTime.seconds >= m_RTSP.GetTimeOut()/2) || m_bIMSendPing )
				{
					m_LastGetParaTime.seconds = seconds;
					ret = m_RTSP.CreateGetParameter();
					ret = m_RTSP.GetBufferData(pData, len);

					if( m_bIMSendPing )
					{
						//printf( "current channel Senddta %d,%d, %s,%d\n",
						//	m_channel_num, m_sub_chn_num, __FILE__, __LINE__ );

						m_bIMSendPing = false;
					}

					break;;
				}
			}

			if  (seconds - m_LastRTCPTime.seconds >= 5)
			{
				m_LastRTCPTime.seconds = seconds;
				ret = m_RTP.SendRTCPData(pData, len);
			}
		}

	} while (0);

	m_Lock.UnLock();
	return ret;
}

int CClientRTSPSession::SundayChangeSearch(const unsigned char *text, int n, const unsigned char *patt, int m)
{

	unsigned char temp[256]; 
	unsigned int i, patt_size = m, text_size = n; 


	if (m <= 0 || m > 254 )
	{
		return -1;
	}

	if (n < m)
	{
		return -1;
	}

	memset( temp, patt_size+1, 256 );
	for( i=0; i < patt_size; i++ ) 
	{
		temp[*(patt+i)] = patt_size-i;
	}

	size_t limit = text_size-patt_size+1; 
	//unsigned char *match_text;
	//size_t match_size;
	int idx;

	i = 0;
	while( i < limit )
	{
		if( text[i] == *patt )
		{
			for( idx = 1; idx < m; idx++ )
			{
				if( text[i+idx] != patt[idx] )
				{
					break;
				}
			}

			if( idx >= m )
			{
				return i+m;
			}
		}

		if( i + m < n )
		{
			i += temp[text[i+m]];
		}
		else
		{
			break;
		}
	}

	return -1;


#if 0
	for( i=0; i < limit; i += temp[ text[i+patt_size] ] ) 
	{ 
		if( text[i] == *patt ) 
		{ 
			match_text = (unsigned char*)text+i+1; 
			match_size = 1; 
			do 
			{// 输出所有匹配的位置 
				if( match_size == patt_size ) 
				{
					return (int)match_text - (int)text;
				}
			} 
			while( (*match_text++) == patt[match_size++] ); 
		} 
	}

	return -1;

#endif
}


int CClientRTSPSession::FindString(const char *pSrc, const int dataLen, const char *pPattern, int patter_len)
{
	//Sunday算法
	int patternLen = patter_len;
	int i=0,j=0;//分别为目标串和模式串当前匹配的位置
	int pe = patternLen - 1;//目标串后定位

	while(i < dataLen && j<patternLen)
	{
		if (pSrc[i] == pPattern[j])
		{
			i++;
			j++;
		}
		else//计算NEXT值
		{
			int k = patternLen - 1;
			while(k>=0 && pSrc[pe + 1] != pPattern[k])//查找模式串中是否有pe
			{
				k--;
			}
			int gap = patternLen - k;
			i += gap;
			pe = i + patternLen - 1;
			j = 0;
		}
	}
	if (j == patternLen)
	{
		return i;
	}

	return -1;
}

void CClientRTSPSession::HandleConnectionBreak()
{
	m_Lock.Lock();
	m_RTP.HandleConnectionBreak();
	m_Lock.UnLock();
}

void CClientRTSPSession::HandleSystemTimeChanged()
{
	m_RTP.HandleSystemTimeChanged();
}

bool CClientRTSPSession::GetIsMustResyncSystem()
{
	return m_RTP.GetIsMustResyncSystem();
}

