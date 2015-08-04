/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 
** Name         : 
** Version      : 
** Description  :
** Modify Record:
1:       
***********************************************************************/
#ifndef _YZ_SNAPMAN_H_
#define _YZ_SNAPMAN_H_

#include "PUB_common.h"
#include "DdnsClient.h"
#include "NetProtocol.h"
#include "NetInterface.h"

#define BASE64_BUF_SIZE 2*20*1024
#define YZ_MAX_SEND_JPEG_BUFFER  2*20*1024+500
typedef struct _yz_snap_info_
{
	int channel;
	LONGLONG time;
	int length;
	unsigned char *pData;
}YZ_SNAP_INFO;

typedef CMyList<YZ_SNAP_INFO> YZ_SNAP_INFO_LIST;



class CBase64
{
public:
	CBase64(void);
	~CBase64(void);

	void SetLineMaxLen(long lineMaxLen);

	long Encode(const unsigned char *pSrc, long srcLen, char *pDest, long destLen);
	long Decode(const char *pSrc, long srcLen, unsigned char *pDest, long destLen);

private:
	long			m_lineMaxLen;
};


class CYZJpegSendClinet:public CDdnsClient
{
public:
	CYZJpegSendClinet();
	~CYZJpegSendClinet();

	bool Init();
	void Quit();

	void SetSendPara(YZ_SNAP_INFO info, char ascode[64], char server[128], char object[128], unsigned short port);
	unsigned long DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName );		
private:

	YZ_SNAP_INFO m_sendInfo;
	char m_ascode[64];
	char m_serverUrl[128];
	char m_urlObject[128];
	unsigned short m_urlPort;

	char *m_pBase64Buffer ;
	char *m_pSendXmlBuffer;
	CBase64 m_base64;
};



class CYZSnapMan
{
public:
	//
	static CYZSnapMan * Instance();
	~CYZSnapMan();

	//
	bool Initial(int maxchnnNum);
	void Quit();

    //
	bool Start();//开启线程
	void Stop();//停止线程

	void Action(bool bAction);

	//
	void SetYZSnapPara(YUANZHENG_SNAP_PARAM snapPara);
	void SetYuanZhengParam(YUANZHENG_PARAM param);
	bool SnapPicture(unsigned char *buf, int nLen, int nChannel, LONGLONG time);


private:
	CYZSnapMan ();
	static	PUB_THREAD_RESULT PUB_THREAD_CALL YZSendJpegThread(void *pParam);
	void YZSendJpegProc();

	
private:

	

	PUB_thread_t		m_yzSendJpegID;
	bool				m_bYZSendJpegProc;


	bool m_bYZSnap;
	int m_curSnapChannel;
	LONGLONG m_lastSnapTime;
	unsigned long m_nMaxTime;
	YZ_SNAP_INFO_LIST m_yzSendPicList;

	char m_yzAscod[64];
	char m_yzJpegServer[128];
	char m_urlObject[128];
	unsigned short m_urlPort;

	CYZJpegSendClinet m_yzJpegSendClinet;


	CPUB_Lock m_lock;
	int m_maxchnnNum;//最多的通道数

};

#endif
