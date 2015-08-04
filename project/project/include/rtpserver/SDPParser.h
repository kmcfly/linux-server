#ifndef __SDP_PARSER__
#define __SDP_PARSER__

#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#if defined(_MSC_VER)
#define strncasecmp _strnicmp
#endif

const int SDP_MAX_MEDIA_NUM = 4;

typedef struct __sdp_media_info_ 
{
	float uFrameRate[SDP_MAX_MEDIA_NUM];
	unsigned long uPacketicationMode;
	unsigned long uMediaWidth[SDP_MAX_MEDIA_NUM];
	unsigned long uMediaHeight[SDP_MAX_MEDIA_NUM];
	unsigned long uBandWidth[SDP_MAX_MEDIA_NUM];
	unsigned long uClockRate[SDP_MAX_MEDIA_NUM];
	int nMediaCount;
	int nMediatype[SDP_MAX_MEDIA_NUM];
	int nMediaPort[SDP_MAX_MEDIA_NUM];
	char MediaName[SDP_MAX_MEDIA_NUM][256];
	char RequestURL[SDP_MAX_MEDIA_NUM][256];
}SDP_MEDIA_INFO;


class CSDPParser
{
public:
	CSDPParser();
	~CSDPParser();
	bool Initial(const char *pInfo);
	
	int GetMediaCount();
	//SDP��Ϣ�������ý�� ȡ�õ�num��ý�����Ϣ
	int GetMediaType(int num);
	int GetMediaPort(int num);
	unsigned short GetBandWidth(int num);
	float GetRate(int num);
	const char* GetMediaURL(int num);
	unsigned char GetPacketicationMode();
	unsigned short GetVideoWidth(int num);
	unsigned short GetVideoHeight(int num);

private:
	int Parse(char *pData);

	//ָ�����ױ��������,����ֵΪ����ĳ��Ⱥͣ���Des��һ�ν���Ӧ�ý��е�ƫ����,��λΪbit
	int H264_Ue_Decode(bool *pBinaryArray, int &Des);
	int H264_Se_Decode(bool *pBinaryArray, int &Des);
	unsigned long Scaling_list (int sizeOfScalingList, bool *pBinaryArray);
	int ParseSPS(unsigned char *pSrc, unsigned long SrcLen, unsigned long &VideoWidth, unsigned long &VideoHeight);
	void Handlefmtp(const char *pData, int mediaCount);

private:
	bool				m_bIsInit;
	SDP_MEDIA_INFO		m_MediaInfo;
};

#endif