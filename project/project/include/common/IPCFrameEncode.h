#pragma once

#include "Product.h"

const unsigned long			MAX_BITRATE_ON_VGA = 2048;
const unsigned long			MAX_BITRATE_ON_HD720 = 6144;
const unsigned long			MAX_BITRATE_ON_HD1080 = 12288;
const unsigned long			MAX_SUGGEST_BITRATE_ON_HD1080 = 6144;
const unsigned long			MAX_SUGGEST_BITRATE_ON_960P = 4096;
const unsigned long			MAX_SUGGEST_BITRATE_ON_1D3M = 5120;
const unsigned long			MAX_SUGGEST_BITRATE_ON_3M = 9216;

class CIPCFrameEncode
{
private:
	enum CLOSE_METHOD
	{
		LOW_CLOSEST,
		LOW_OR_HIGH_CLOSEST,
		HIGH_CLOSEST,
	};
	typedef struct _frame_encode_
	{
		unsigned long	frameRate;
		unsigned long	videoSize;
		unsigned long	bitRate;
	}FRAME_ENCODE;
public:
	CIPCFrameEncode(void);
	~CIPCFrameEncode(void);

	bool Initial(unsigned char chnnNum, unsigned long maxBitRateTotal, unsigned long maxFrameRateOnVGA, \
		unsigned long *pValidBitRate, unsigned long bitRateCount, \
		unsigned long defaVideoSize, unsigned long defaFrameRate, unsigned long defaBitRate);
	void Quit();

	//////////////////////////////////////////////////////////////////////////
	void AddEncode(unsigned char chnn, unsigned char frameRate, VIDEO_SIZE videoSize, unsigned long bitRate);
	bool TryEncode(unsigned char chnn, unsigned char & frameRate, VIDEO_SIZE & videoSize, unsigned long &bitRate);
	bool ModifyEncode(unsigned char chnn, unsigned char frameRate, VIDEO_SIZE videoSize, unsigned long bitRate);
	unsigned long RemainingFrameRate(VIDEO_SIZE videoSize);

	unsigned long GetMaxFrameRate(VIDEO_SIZE videoSize);

	unsigned long GetSuggestBitRate(VIDEO_SIZE videoSize, unsigned char frameRate);

private:
	bool IsInBitRateRange(unsigned long bitRate);
	unsigned long ToValidBitRate(unsigned long bitRate, CLOSE_METHOD method);

	unsigned long ToCorrespondingVGARate(unsigned long videoSize, unsigned char frameRate);

	unsigned char		m_chnnNum;
	unsigned long		m_maxBitRateTotal;
	unsigned long		m_maxFrameRateOnVGA;
	unsigned long		*m_pValidBitRate;
	unsigned long		m_bitRateCount;
	unsigned long		m_bLimitByBitRate;	//当NVR性能受限于芯片CPU能力时，NVR的性能就取决于码率的大小；
											//当NVR性能受限于芯片的解码能力时，NVR的性能就取决于帧率；

	unsigned long		m_defaVideoSizeOfVGA;
	unsigned long		m_defaFrameRate;
	unsigned long		m_defaBitRate;

	FRAME_ENCODE		*m_pFrameEncode;
	bool				m_bUseStaticVideoBitRate;
};
