#ifndef _SNAP_CAPABILITY_MAN_H_
#define _SNAP_CAPABILITY_MAN_H_
#include "LocalDevice.h"

class CSnapCapabilityMan
{
public:
	
	//
	~CSnapCapabilityMan();

	//
	static CSnapCapabilityMan * Instance();

	//
	bool Initial(int channelNum, int rate, int rateTime, VIDEO_SIZE videoSize);

	void Quit();

	//
	void StarSnapJpeg(int chnn, bool bPhone);

	//
	void StopSnapJpeg(int chnn, bool bPhone);

	//
	void ChangeJPEGVideoSize(VIDEO_SIZE videoSize);

	//
	void ChangeJPEGVideoLevel(unsigned long videLevel);


protected:

private:

	//
	CSnapCapabilityMan();

	//
	int              m_chnnNum;	                //总共通道个数
	VIDEO_SIZE       m_snapVideSize;            //jpeg图片的分辨率
	unsigned long    m_jpegLevel;               //jpeg图片的画质级别
	int              m_snapRate;                //正常情况下抓jpeg图片的帧率
	int              m_snapRateTime;            //

	//
	int              m_phoneSnapRate;          //手机抓jpeg图片的帧率,
	int              m_phoneSnapRaiseChnn;     //把手机抓jpeg图片的帧率给了那个通道
	              

	//
	unsigned char    *m_pChnnNoPhoneEableCount;  //非手机请求开启抓图的次数
	unsigned char    *m_pChnnPhoneEableCount;    //手机请求开启抓图的次数


	CPUB_Lock  m_setSnapRateLock;
	static	CSnapCapabilityMan * m_pSnapCapabilityMan;
};

#endif

