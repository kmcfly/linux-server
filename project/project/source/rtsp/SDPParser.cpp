#include "SDPParser.h"
#include "RTPHeader.h"
#include "Base64.h"
#include "Base16.h"
#include "H264Parser.h"

#ifdef assert
#undef assert
#define assert(a) (printf("%s %s %d : assert false\n", __FILE__, __FUNCTION__, __LINE__))
#endif

CSDPParser::CSDPParser()
{
	ResetObject();
}

CSDPParser::~CSDPParser()
{

}

bool CSDPParser::ResetObject(void)
{
	m_bIsInit = false;
	memset(&m_MediaInfo, 0, sizeof(SDP_MEDIA_INFO));

	memset(m_SPS, 0, sizeof(m_SPS));
	memset(m_PPS, 0, sizeof(m_PPS));
	m_SPSLen = 0;
	m_PPSLen = 0;

	return true;
}

bool CSDPParser::Initial(const char *pInfo)
{
	if (NULL == pInfo)
	{
		return false;
	}
	unsigned short len = strlen(pInfo);
	char pTmp[10*1024] = {0};
	strcpy(pTmp, pInfo);
	if (0 > Parse(pTmp))
	{
		return false;
	}
	m_bIsInit = true;

	return true;
}


int CSDPParser::GetMediaCount()
{
	if (!m_bIsInit)
	{
		return -1;
	}
	return m_MediaInfo.nMediaCount;
}

int CSDPParser::GetMediaType(int num)
{
	if (!m_bIsInit)
	{
		return -1;
	}
	if (num >= SDP_MAX_MEDIA_NUM)
	{
		return -1;
	}
	return m_MediaInfo.nMediatype[num];
}

int CSDPParser::GetMediaPort(int num)
{
	if (!m_bIsInit)
	{
		return -1;
	}
	if (num >= SDP_MAX_MEDIA_NUM)
	{
		return -1;
	}
	return m_MediaInfo.nMediaPort[num];
}

int CSDPParser::GetMediaPayload(int num)
{
	if (!m_bIsInit)
	{
		return -1;
	}
	if (num >= SDP_MAX_MEDIA_NUM)
	{
		return -1;
	}
	return m_MediaInfo.nPayLoadType[num];
}


int CSDPParser::GetMediaAudioType(int num)
{
	if (!m_bIsInit)
	{
		return -1;
	}
	if (num >= SDP_MAX_MEDIA_NUM)
	{
		return -1;
	}
	return m_MediaInfo.audioMediaType[num];

}

unsigned short CSDPParser::GetBandWidth(int num)
{
	if (!m_bIsInit)
	{
		return 0;
	}
	if (num >= SDP_MAX_MEDIA_NUM)
	{
		return -1;
	}
	return m_MediaInfo.uBandWidth[num];
}

float CSDPParser::GetRate(int num)
{
	if (!m_bIsInit)
	{
		return 0;
	}
	if (num >= SDP_MAX_MEDIA_NUM)
	{
		return -1;
	}
	return m_MediaInfo.uFrameRate[num];
}

const char* CSDPParser::GetMediaURL(int num)
{
	if (!m_bIsInit)
	{
		return NULL;
	}
	if (num >= SDP_MAX_MEDIA_NUM)
	{
		return NULL;
	}
	
	return &m_MediaInfo.RequestURL[num][0];
}

unsigned char CSDPParser::GetPacketicationMode()
{
	if (!m_bIsInit)
	{
		return -1;
	}
	return m_MediaInfo.uPacketicationMode;
}

int CSDPParser::Parse(char *pData)
{
	if (NULL == pData)
	{
		return -1;
	}
	int count = -1;
	int control_count = 0;
	int rtpmap_count =0;
	char *pPos = pData;
	char *pPosEnd = pData;
	char *iter = NULL;
	char pAnswer[1024] = {0};
	char pTmp[256] = {0};

	if (NULL == pAnswer || NULL == pTmp)
	{
		printf("%s %d err\n", __FILE__, __LINE__);
		return -1;
	}
	int port	= 0;
	int pt	= 0;

	int b_media_info_valid[SDP_MAX_MEDIA_NUM] = {0};


	while (NULL != pPosEnd && '\0' != *pPosEnd)
	{
		memset(pAnswer, 0, 1024);
		pPos = pPosEnd;

		for (iter=pPos; *iter != '\r' && *iter != '\n'; iter++)
		{
			if ((*iter) == '\0')
			{
				break;
			}
		}
		pPosEnd = iter;
		pPosEnd++;
		strncpy(pAnswer, pPos, pPosEnd-pPos);

		if (!strncasecmp(pAnswer, "m=", 2))
		{
			count = -1;
		}

		if (!strncasecmp(pAnswer, "m=audio", 7))
		{
			port = 0;
			pt = 0;
			if (2 == sscanf(pAnswer, "%*[^ ] %d %*[^ ] %d", &port, &pt))
			{
				if( 0 == b_media_info_valid[SDP_AUDIO_STREAM_POS] )
				{
					count = SDP_AUDIO_STREAM_POS; //++;
					//if (pt == RTP_PAYLOAD_TYPE_G711)
					{
						m_MediaInfo.nMediaPort[count] = port;
						m_MediaInfo.nPayLoadType[count] = pt;
						m_MediaInfo.nMediatype[count] = STREAM_LIVE_AUDIO;
					}

					if( 0 == pt )
					{
						m_MediaInfo.audioMediaType[count] = RTP_PAYLOAD_TYPE_G711_U;
						b_media_info_valid[count] = 1;
					}
					else if( 8 == pt )
					{
						m_MediaInfo.audioMediaType[count] = RTP_PAYLOAD_TYPE_G711_A;
						b_media_info_valid[count] = 1;
					}
					else
					{
						b_media_info_valid[count] = 0;
					}
				}
			}
		}

		if (!strncasecmp(pAnswer, "m=video", 7))
		{
			port = 0;
			pt = 0;
			if (2 == sscanf(pAnswer, "%*[^ ] %d %*[^ ] %d", &port, &pt))
			{
				//if (pt == RTP_PAYLOAD_TYPE_PS || pt == RTP_PAYLOAD_TYPE_H264)
				if( 0 == b_media_info_valid[SDP_VIDEO_STREAM_POS] )
				{
					count = SDP_VIDEO_STREAM_POS ;//++;
					//if (count >= SDP_MAX_MEDIA_NUM)
					//{
					//	printf("%s %d err\n", __FILE__, __LINE__);
					//	break;
					//}
					m_MediaInfo.nMediaPort[count] = port;
					m_MediaInfo.nPayLoadType[count] = pt;
					m_MediaInfo.nMediatype[count] = STREAM_LIVE_VIDEO;

					b_media_info_valid[count] = 1;
				}
			}
		}

		if (!strncasecmp(pAnswer, "c=IN", 4))
		{
		}

		if (!strncasecmp(pAnswer, "a=rtpmap:", 9))
		{
			//if (count >= rtpmap_count)
			if( count >= 0 )
			{
				rtpmap_count++;
				memset(pTmp, 0, 256);
				unsigned long clockrate = 0;
 				if (2 == sscanf(pAnswer, "%*[^:]:%d %[^/]s", &pt, pTmp))
 				{
 					if (m_MediaInfo.nPayLoadType[count] != pt)
 					{
						//printf("%s %d PayloadType Err\n", __FILE__, __LINE__);
						//return -1;

						b_media_info_valid[count] = 0;
 					}

					if( b_media_info_valid[count] )
					{
						strcpy(m_MediaInfo.MediaName[count], pTmp);
					}
 				}

				if( b_media_info_valid[count] )
				{

					if (1 == sscanf(pAnswer, "%*[^/]/%lu", &clockrate))
					{
						m_MediaInfo.uClockRate[count] = clockrate;
					}

					if( SDP_VIDEO_STREAM_POS == count )
					{
						char* pFindH264 = strstr( pAnswer, "H264" );

						if( !pFindH264 )
						{
							b_media_info_valid[count] = 0;
						}
					}
					else if( SDP_AUDIO_STREAM_POS == count )
					{
						char* pPCMX = strstr( pAnswer, "PCMU" );
						if( pPCMX )
						{
							m_MediaInfo.audioMediaType[count] = RTP_PAYLOAD_TYPE_G711_U;
						}
						else
						{
							pPCMX = strstr( pAnswer, "PCMA" );
							if( pPCMX )
							{
								m_MediaInfo.audioMediaType[count] = RTP_PAYLOAD_TYPE_G711_A;
							}
						}
					}
				}
			}
		}

		if (!strncasecmp(pAnswer, "a=framerate", 11))
		{
			float rate = 0.0;
			if( count >= 0 )
			{
				if (1 == sscanf(pAnswer, "%*[^:]:%f", &rate))
				{
					m_MediaInfo.uFrameRate[count] = rate;
				}

			}
		}

		//服务器告诉客户端  应该请求哪些媒体
		if (!strncasecmp(pAnswer, "a=control:", 10))
		{
			if(count >= 0)
			{
				control_count++;
				memset(pTmp, 0, 256);
				if (1 == sscanf(pAnswer+10, "%s[^\r^\n^\0]", pTmp))
				{
					//m_MediaInfo.RequestURL[count] = pTmp;
					memset( m_MediaInfo.RequestURL[count], 0, 256 );
					strcpy(m_MediaInfo.RequestURL[count], pTmp);
				}
			}
		}

		if (!strncasecmp(pAnswer, "a=fmtp:", 7))
		{
			int mediaType = 0;
			if( count >= 0 )
			{
				if (1 == sscanf(pAnswer, "%*[^:]:%d", &mediaType))
				{
					//if (mediaType == RTP_PAYLOAD_TYPE_H264 || mediaType == RTP_PAYLOAD_TYPE_PS)
					{
						Handlefmtp(pAnswer, count);
					}
				}

			}
		}
		if (!strncasecmp(pAnswer, "a=framesize:", 12))
		{
			if(count >= 0 )
			{
				sscanf(pAnswer, "%*[^ ] %d", &m_MediaInfo.uMediaWidth[count]);
				sscanf(pAnswer, "%*[^-]-%d", &m_MediaInfo.uMediaHeight[count]);

			}
		}
	}

	//if (count < 0 || count >= SDP_MAX_MEDIA_NUM)
	//{
	//	return -1;
	//}

	int items_cnt = 0;
	for( count = 0; count < SDP_MAX_MEDIA_NUM; count++ )
	{
		if( !b_media_info_valid[count] )
		{
			break;
		}

		items_cnt++;
	}

	m_MediaInfo.nMediaCount = items_cnt;

	return 0;
}

unsigned short CSDPParser::GetVideoWidth(int num)
{
	if (!m_bIsInit)
	{
		return -1;
	}

	return m_MediaInfo.uMediaWidth[num];
}

unsigned short CSDPParser::GetVideoHeight(int num)
{
	if (!m_bIsInit)
	{
		return -1;
	}

	return m_MediaInfo.uMediaHeight[num];
}


void CSDPParser::Handlefmtp(const char *pData,  int mediaCount)
{
	char *pPos = strstr((char *)pData, "packetization-mode=");
	if (NULL != pPos)
	{
		m_MediaInfo.uPacketicationMode = atoi(pPos + 19);
	}

	pPos = strstr((char *)pData, "profile-level-id=");
	if (NULL != pPos)
	{
		CBase16 Base16Obj;
		char Base16[33] = {0};
		char *pTmp = pPos + 17;
		pPos = pTmp;
		while(*pPos != '\r' && *pPos != '\n' && *pPos != '\0' && *pPos != ';')
		{
			pPos++;
		}
		Base16Obj.Decode(pTmp, pPos - pTmp, Base16);
	}

	pPos = strstr((char *)pData, "sprop-parameter-sets=");
	if (NULL != pPos)
	{
		CBase64 Base64Obj;
		char *pTmp = pPos + 21;
		while(1)
		{
			pPos = pTmp;
			while(*pPos != ',' && *pPos != '\r' && *pPos != '\n' && *pPos != '\0' && *pPos != ';')
			{
				pPos++;
			}
			unsigned char pResult[256] = {0};

			long ret = Base64Obj.Decode(pTmp, pPos - pTmp, pResult, 256);
			unsigned short  width, height;
			if ((pResult[0] & 0x1f) == SEQ_PARAMETER_SET_RBSP)
			{
				if (ret > 0 && ret < 512)
				{
					memcpy(m_SPS, pResult, ret);
					m_SPSLen = ret;
				}

				ParseSPS(pResult + 1, ret, width, height);
				if (m_MediaInfo.uMediaWidth[mediaCount] == 0)
				{
					m_MediaInfo.uMediaWidth[mediaCount] = width;
				}
				if (m_MediaInfo.uMediaHeight[mediaCount] == 0)
				{
					m_MediaInfo.uMediaHeight[mediaCount] = height;
				}
			}
			else if ((pResult[0] & 0x1f) == PIC_PARAMETER_SET_RBSP)
			{
				if (ret > 0 && ret < 512)
				{
					memcpy(m_PPS, pResult, ret);
					m_PPSLen = ret;
				}				
			}

			if (*pPos == ';' || *pPos == '\r' || *pPos == '\n' || *pPos == '\0')
			{
				break;
			}
			else
			{
				pTmp = pPos + 1;
			}
		}
	}
}

char* CSDPParser::GetSpsPps( char *pDes, int &len )
{
	if (!pDes)
	{
		return NULL;
	}
	if (m_SPSLen <= 0 || m_PPSLen <= 0)
	{
		return NULL;
	}

	int offset = 0;
	*pDes = 0x00;
	*(pDes+1) = 0x00;
	*(pDes+2) = 0x00;
	*(pDes+3) = 0x01;
	offset += 4;
	memcpy(pDes+offset, m_SPS, m_SPSLen);
	offset += m_SPSLen;


	*(pDes+offset) = 0x00;
	*(pDes+offset+1) = 0x00;
	*(pDes+offset+2) = 0x00;
	*(pDes+offset+3) = 0x01;
	offset += 4;
	memcpy(pDes+offset, m_PPS, m_PPSLen);
	offset += m_PPSLen;

	len = offset;

	return pDes;
}


