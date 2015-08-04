#include "SDPParser.h"
#include "RTPHeader.h"
#include "math.h"
#include "Base64.h"
#include "Base16.h"

#ifdef assert
#undef assert
#define assert(a) (printf("%s %s %d : assert false\n", __FILE__, __FUNCTION__, __LINE__))
#endif

CSDPParser::CSDPParser()
{
	m_bIsInit = false;
	memset(&m_MediaInfo, 0, sizeof(SDP_MEDIA_INFO));
}

CSDPParser::~CSDPParser()
{

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
	char *pAnswer = new char[1024];
	char *pTmp = new char[256];

	if (NULL == pAnswer || NULL == pTmp)
	{
		assert(false);
		return -1;
	}
	int port	= 0;
	int pt	= 0;

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

		if (!strncasecmp(pAnswer, "m=audio", 7))
		{
			port = 0;
			pt = 0;
			if (2 == sscanf(pAnswer, "%*[^ ] %d %*[^ ] %d", &port, &pt))
			{
				if (pt == RTP_PAYLOAD_TYPE_G711)
				{
					count++;
					m_MediaInfo.nMediaPort[count] = port;
					m_MediaInfo.nMediatype[count] = pt;
				}
			}
		}

		if (!strncasecmp(pAnswer, "m=video", 7))
		{
			port = 0;
			pt = 0;
			if (2 == sscanf(pAnswer, "%*[^ ] %d %*[^ ] %d", &port, &pt))
			{
				if (pt >= RTP_PAYLOAD_TYPE_H264_ && pt <= RTP_PAYLOAD_TYPE_H264_MAX)
				{
					count++;
					if (count >= SDP_MAX_MEDIA_NUM)
					{
						assert(false);
						break;
					}
					m_MediaInfo.nMediaPort[count] = port;
					m_MediaInfo.nMediatype[count] = pt;
				}
			}
		}

		if (!strncasecmp(pAnswer, "c=IN", 4))
		{
		}

		if (!strncasecmp(pAnswer, "a=rtpmap:", 9))
		{
			if (count >= rtpmap_count)
			{
				rtpmap_count++;
				memset(pTmp, 0, 256);
				unsigned long clockrate = 0;
 				if (2 == sscanf(pAnswer, "%*[^:]:%d %[^/]s", &pt, pTmp))
 				{
 					if (m_MediaInfo.nMediatype[count] != pt)
 					{
 						assert(false);
 					}
					strcpy(m_MediaInfo.MediaName[count], pTmp);
 				}
				if (1 == sscanf(pAnswer, "%*[^/]/%lu", &clockrate))
				{
					m_MediaInfo.uClockRate[count] = clockrate;
				}
			}
		}

		if (!strncasecmp(pAnswer, "a=framerate", 11))
		{
			float rate = 0.0;
			if (1 == sscanf(pAnswer, "%*[^:]:%f", &rate))
			{
				m_MediaInfo.uFrameRate[count] = rate;
			}
		}

		//服务器告诉客户端  应该请求哪些媒体
		if (!strncasecmp(pAnswer, "a=control:", 10))
		{
			if(count >= control_count)
			{
				control_count++;
				memset(pTmp, 0, 256);
				if (1 == sscanf(pAnswer+10, "%s[^\r^\n^\0]", pTmp))
				{
					//m_MediaInfo.RequestURL[count] = pTmp;
					strcpy(m_MediaInfo.RequestURL[count], pTmp);
				}
			}
		}

		if (!strncasecmp(pAnswer, "a=fmtp:", 7))
		{
			int mediaType = 0;
			if (1 == sscanf(pAnswer, "%*[^:]:%d", &mediaType))
			{
				if (mediaType >= RTP_PAYLOAD_TYPE_H264_ && mediaType <= RTP_PAYLOAD_TYPE_H264_MAX)
				{
					Handlefmtp(pAnswer, count);
				}
			}
		}
		if (!strncasecmp(pAnswer, "a=framesize:", 12))
		{
			sscanf(pAnswer, "%*[^ ] %d", &m_MediaInfo.uMediaWidth[count]);
			sscanf(pAnswer, "%*[^-]-%d", &m_MediaInfo.uMediaHeight[count]);
		}
	}

	delete [] pAnswer;
	pAnswer = NULL;
	delete [] pTmp;
	pTmp = NULL;

	if (count < 0 || count >= SDP_MAX_MEDIA_NUM)
	{
		return -1;
	}

	m_MediaInfo.nMediaCount = count+1;

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


int CSDPParser::H264_Ue_Decode(bool *pBinaryArray, int &Des)
{
	if (pBinaryArray == NULL)
	{
		return -1;
	}
	
	int iter = 0;
	int NumOfZero = 0;
	int M_bit = 0;
	int INFO = 0;
	double result;

	//找到第一个1
	while(pBinaryArray[iter++] == 0)
	{
		NumOfZero++;
	}
	for (int i=NumOfZero-1; i>=0; i--)
	{
		INFO += pBinaryArray[iter++]<<i;
	}
	result = pow((double)2, (double)NumOfZero) + INFO - 1;
	Des = result;
 	
	return iter;
}

int CSDPParser::H264_Se_Decode(bool *pBinaryArray, int &Des)
{
	int ret = H264_Ue_Decode(pBinaryArray, Des);
	if ((Des & 0x1) == 0) 
	{
		Des >>= 1;
		Des = 0 - Des;
	}
	else
	{
		Des = (Des + 1) >> 1;
	}

	return ret;
}

int CSDPParser::ParseSPS(unsigned char *pSrc, unsigned long SrcLen, unsigned long &VideoWidth, unsigned long &VideoHeight)
{
	int ret = 0;
	unsigned long offset = 0;
	bool *pBinaryArray = new bool[SrcLen*8];
	unsigned long iter = 0;
	unsigned char mask = 0x80;
	for (int i=0; i<SrcLen; i++)
	{
		mask = 0x80;
		for (int j=0; j<8; j++)
		{
			if (pSrc[i] & (mask >> j))
			{
				pBinaryArray[iter] = 1;
			}
			else
			{
				pBinaryArray[iter] = 0;
			}
			iter++;
		}
	}
	iter = 0;

	unsigned char profile_idc = *pSrc;
	int seq_parameter_set_id;
	offset = 24;
	ret = H264_Ue_Decode(pBinaryArray+offset, seq_parameter_set_id);
	offset += ret;
	if (profile_idc == 100 || profile_idc == 110 || profile_idc == 122 || profile_idc == 144)
	{
		int chroma_format_idc;
		ret = H264_Ue_Decode(pBinaryArray+offset, chroma_format_idc);
		offset += ret;
		if(chroma_format_idc == 3)
		{
			offset += 1;
		}
		int bit_depth_luma_minus8, bit_depth_chroma_minus8;
		ret = H264_Ue_Decode(pBinaryArray+offset, bit_depth_luma_minus8);
		offset += ret;
		ret = H264_Ue_Decode(pBinaryArray+offset, bit_depth_chroma_minus8);
		offset += ret;
		offset += 1;
		bool seq_scaling_matrix_present_flag = false;
		seq_scaling_matrix_present_flag = pBinaryArray[offset];
		offset += 1;
		if (seq_scaling_matrix_present_flag)
		{
			unsigned long offset_tmp;
			for (int ix=0; ix<8; ix++)
			{
				if (pBinaryArray[offset++])
				{
					offset_tmp = Scaling_list(ix < 6 ? 16 : 64, pBinaryArray + offset);
					offset += offset_tmp;
				}
			}
		}
	}
	int log2_max_frame_num_minus4, pic_order_cnt_type;
	ret = H264_Ue_Decode(pBinaryArray+offset, log2_max_frame_num_minus4);
	offset += ret;
	ret = H264_Ue_Decode(pBinaryArray+offset, pic_order_cnt_type);
	offset += ret;
	if (pic_order_cnt_type == 0)
	{
		int log2_max_pic_order_cnt_lsb_minus4;
		ret = H264_Ue_Decode(pBinaryArray+offset, pic_order_cnt_type);
		offset += ret;
	}
	else if (pic_order_cnt_type == 1)
	{
		offset += 1;
		int offset_for_non_ref_pic, offset_for_top_to_bottom_field, num_ref_frames_in_pic_order_cnt_cycle, offset_for_ref_frame;
		ret = H264_Se_Decode(pBinaryArray+offset, offset_for_non_ref_pic);
		offset += ret;
		ret = H264_Se_Decode(pBinaryArray+offset, offset_for_top_to_bottom_field);
		offset += ret;
		ret = H264_Ue_Decode(pBinaryArray+offset, num_ref_frames_in_pic_order_cnt_cycle);
		offset += ret;
		for (int i=0; i<num_ref_frames_in_pic_order_cnt_cycle ;i++)
		{
			ret = H264_Se_Decode(pBinaryArray+offset, offset_for_ref_frame);
			offset += ret;
		}
	}
	int num_ref_frames;
	ret = H264_Ue_Decode(pBinaryArray+offset, num_ref_frames);
	offset += ret;
	offset += 1;
	int pic_width_in_mbs_minus1, pic_height_in_map_units_minus1, frame_mbs_only_flag;
	ret = H264_Ue_Decode(pBinaryArray+offset, pic_width_in_mbs_minus1);
	offset += ret;
	ret = H264_Ue_Decode(pBinaryArray+offset, pic_height_in_map_units_minus1);
	offset += ret;
	frame_mbs_only_flag = pBinaryArray[offset];

	VideoWidth = (pic_width_in_mbs_minus1 + 1) * 16;
	VideoHeight = (2 - frame_mbs_only_flag) * (pic_height_in_map_units_minus1 + 1) * 16;

	delete []pBinaryArray;
	pBinaryArray = NULL;
	return 0;
}

unsigned long CSDPParser::Scaling_list (int sizeOfScalingList, bool *pBinaryArray)
{
	int lastScale = 8, nextScale = 8;
	int j, ret;
	unsigned long offset = 0;

	for (j = 0; j < sizeOfScalingList; j++)
	{
		if (nextScale != 0)
		{
			int deltaScale;
			ret = H264_Se_Decode(pBinaryArray + offset, deltaScale);
			offset += ret;
			nextScale = (lastScale + deltaScale + 256) % 256;
		}
		if (nextScale == 0)
		{
			lastScale = lastScale;
		} 
		else
		{
			lastScale = nextScale;
		}
	}
	return offset;
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
			while(*pPos != ',', *pPos != '\r' && *pPos != '\n' && *pPos != '\0' && *pPos != ';')
			{
				pPos++;
			}
			unsigned char *pResult = new unsigned char[256];
			if (NULL == pResult)
			{
				assert(false);
				return;
			}
			memset(pResult, 0, 256);
			long ret = Base64Obj.Decode(pTmp, pPos - pTmp, pResult, 256);
			unsigned long  width, height;
			if ((pResult[0] & 0x1f) == SEQ_PARAMETER_SET_RBSP)
			{
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
			delete [] pResult;
			pResult = NULL;

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