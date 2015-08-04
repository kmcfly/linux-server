/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-16
** Name         : ShanghaiSnapMan.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef __SHANGHAI_SNAPMAN_H__
#define __SHANGHAI_SNAPMAN_H__
#include "MessageMan.h"
#include <list>
#include "ShanghaiCmd.h"
#include "ShanghaiConnectMan.h"
#include "PUB_common.h"
#include "uuid.h"
#include "CharCodeTable.h"
using namespace std;

const int maxpicLen = 20*1024;
typedef struct _PreSnap_
{
	unsigned char *pData;//图片数据
	int nLen;//图片数据的长度
	int chnn;//通道号
	LONGLONG time;
	unsigned long type;
	unsigned short width;
	unsigned short height;
	unsigned short bLock;
}PRE_SNAP_INFO;

class CShanghaiSnapMan
{
public:
	static CShanghaiSnapMan *GetInstance();
	~CShanghaiSnapMan();
		
	bool Initial(unsigned char channelNum);
	void GetDailyCfg(const SNAP_INFOEX *pData,int count);//在配置生效处获取日常配置的定时消息
	void Quit();

	//设置主页面报警截图参数
	void SetScreenSnapParam(unsigned long chnn);
	//参数依次为通道号，抓图时间，抓图的类型
	void SetManualParam(ULONGLONG chnnBits,ULONGLONG snapTime,unsigned long imagetype);
	//获取日常图片
	//void *buf  --从流里面获取的图片，测试直接保存到内存查看
	bool ShanghaiDailySnapPicture(void *buf, int nLen, int nChannel, LONGLONG time,unsigned short width, unsigned short height);
	//手动抓图并上传
	void ShanghaiManualSnapPicture(void *buf, int nLen, int nChannel, LONGLONG time,unsigned short width, unsigned short height);
	//按需上传图片
	void  ShanghaiOnDemandSnapPicture(void *buf, int nLen, int nChannel, LONGLONG time);
	
	//设置主界面上的手动抓图和报警抓图
	bool SetSnapPicture(unsigned long chnnl, unsigned long type);
	//报警抓图和页面上的手动抓图都走这里
	bool ShanghaiAlarmManualSnapPicture(void *buf, int nLen, int nChannel, LONGLONG time , unsigned short width, unsigned short height, bool bLock = false);

	//获取警前抓图的配置信息
	void GetPreAlarmCfg(const unsigned long *preSec);
	void GetPostAlarmCfg(const unsigned long *pPostSec);//获取报警延迟的配置信息
	void GetAlarmTime(unsigned long chnn);//获取报警时的时间

	void SetSnapTimeS(const unsigned long timeS);//设置抓图间隔
	void SetSnapImgNum(const unsigned long imgNum);//设置抓图时间
	
	unsigned long GetSnapTime();
	void GetAlarmType(unsigned long alarmType,unsigned long chnn);

	void SetCheckPiliceNum(const char *pNote);
private:
	CShanghaiSnapMan();
	//比较配置中时间的时分秒和获取时间中的时分秒,
	//一致返回true,否则返回false
	bool CmpDailyTime(unsigned long cfgTime,ULONGLONG time);
	void InsertPreSnapList(void *buf, int nLen, int nChannel, LONGLONG time,unsigned short width,unsigned short height,unsigned short block);
	//组装OPER_INFO info并发送给ShanghaiConnectman
	//bool isbManual 区分是手动还是日常抓图 true是手动抓图，false是日常抓图
	void CreateoperInfo(void *buf, int nLen, int nChannel, LONGLONG time,bool isbManual);
	//组装报警抓图OPER_INFO info
	void CreateAlarmoperInfo(void *buf, int nLen, int nChannel, LONGLONG time,uuid_t theOneUUid);
	
	//建立一组报警抓图的唯一头
	void CreateAlarmtitleInfo(uuid_t theOneUUid,LONGLONG time,unsigned long chnn);

	unsigned long GetChnnMAXSnap(unsigned long chnn);//计算每个通道最多截取多少张

private:
	SNAP_INFOEX      *m_pData;//日常图像配置里面有的定时(只有时分秒有用)和通道
	unsigned long    m_count; //日常配置共有几项
	ULONGLONG		 m_chnnBits;//手动抓图上传时选择的通道号
	ULONGLONG		 m_manualSnapTime;//手动抓图上传保存的时间
	bool		     m_bmanualSnap;//手动抓图上传开启

	LONGLONG	m_OnDemandChnnBits;
	LONGLONG	m_OnDemandSnapTime;

	unsigned long   	*m_pChannType;
	int                 *m_pChnnStatus;	
	int                 *m_pChnnCount;//界面上手动截图的计数
	LONGLONG            *m_pLastTime;//界面上手动抓图上一次图片来的时间，用于和当前时间比较是否大于设定的时间
	LONGLONG			*m_pAlarmPrevTime;//报警抓图上一次图片的时间（预抓图也使用该时间）
	int                  m_nMaxSnap;
	CPUB_Lock            m_lock;
	unsigned long        m_nMaxTime;
	int                 m_nCurSnapNum;

	list<PRE_SNAP_INFO> m_list[32];
	
	unsigned long		*m_preSec;//从配置中读取的报警前抓图秒数
	unsigned long		m_maxchnn;
	unsigned long		*m_postSec;//从配置中读取的报警后抓图秒数
	//获取每个通道报警截图的开始时间，在报警截图的持续期间，
	//即使手动截图超过最大张数，依旧获取截图直到报警延时结束
	unsigned long		*m_startSnapTime;

	unsigned long m_manualImageType;

	unsigned long *m_Alarmtype;//报警的类型


	unsigned long *m_snapcount;//报警时每个通道产生了多少图片
	uuid_t m_theoneMotion;
	uuid_t m_theoneSensor;
	char m_policeNote[256];

	ULONGLONG *m_repaetChnnBits;//当天内的重复日常通道，第2天则被清0

	unsigned long * m_pScreenSnapKey;//主页面抓图标记,用于移动侦测时候主页面手动截图
};

#endif

