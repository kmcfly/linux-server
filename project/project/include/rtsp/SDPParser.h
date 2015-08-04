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
	int nPayLoadType[SDP_MAX_MEDIA_NUM];
	int nMediaPort[SDP_MAX_MEDIA_NUM];
	char MediaName[SDP_MAX_MEDIA_NUM][256];
	char RequestURL[SDP_MAX_MEDIA_NUM][256];
	int audioMediaType[SDP_MAX_MEDIA_NUM];
}SDP_MEDIA_INFO;


class CSDPParser
{
public:
	CSDPParser();
	~CSDPParser();
	bool Initial(const char *pInfo);
	
	int GetMediaCount();
	//SDP信息包含多个媒体 取得第num个媒体的信息
	int GetMediaType(int num);
	int GetMediaPort(int num);
	int GetMediaPayload(int num);
	int GetMediaAudioType(int num);
	unsigned short GetBandWidth(int num);
	float GetRate(int num);
	const char* GetMediaURL(int num);
	unsigned char GetPacketicationMode();
	unsigned short GetVideoWidth(int num);
	unsigned short GetVideoHeight(int num);

	char* GetSpsPps( char *pDes, int &len );

	bool ResetObject(void);

private:
	int Parse(char *pData);

	void Handlefmtp(const char *pData, int mediaCount);

private:
	bool				m_bIsInit;
	SDP_MEDIA_INFO		m_MediaInfo;

	char				m_SPS[512];
	char				m_PPS[512];
	unsigned int		m_SPSLen;
	unsigned int		m_PPSLen;

};

#endif