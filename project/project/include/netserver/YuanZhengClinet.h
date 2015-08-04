#ifndef _YUANZHEN_CLINET_H_
#define _YUANZHEN_CLINET_H_

#include "DdnsClient.h"
#include "Typedef.h"
#include "PUB_common.h"
#include "NetProtocol.h"
#include "NetInterface.h"

#define YZ_UPDATE_SLEEP      12  ///多少个10秒

typedef enum _yz_alarm_tyep_
{
	YZ_ALARM_VIDEOLOSS,
	YZ_ALARM_DISK_FULL,
	YZ_ALARM_DISK_DISCONNET,
	YZ_ALARM_TYPE_NUM,
}YZ_ALARMTYPE;

class CYuanZhengClinet:public CDdnsClient
{
public:
	CYuanZhengClinet();
	~CYuanZhengClinet();

	void Init(char *pMac);

	void SetYuanZhengParam(YUANZHENG_PARAM param);
	void SetServerPort(unsigned short serverPort);
	
	unsigned long DdnsIPupdate(const char *pUserName, const char *pPassword, const char *pmyDomainName );	
	void SetYZAlarm(unsigned long yzAlarmType,ULONGLONG   yzVieolossChnnBits);
private:

	char m_dvrmac[36];
	char m_CmdBuf[1024];
	char m_xmlBuf[512];
	char m_serUrl[128];
	char m_urlObject[128];
	unsigned short m_urlPort;
	unsigned short m_serverPort;
	unsigned long  m_alarmID;
	YUANZHENG_PARAM m_yuanZhengParam;
	unsigned long  m_yzAlarmType;//报警类型
	ULONGLONG      m_yzVieolossChnnBits;//视频丢失通道号

};

class CYuanZhengMan
{
public:
	static CYuanZhengMan *Instance();

	CYuanZhengMan();
	~CYuanZhengMan();

	bool Inital();
	void Quit();

	bool Start();
	void Stop();

	void SetYuanZhengParam(YUANZHENG_PARAM param);
	void SetServerPort(unsigned short serverPort);
	void SetYZAlarm(unsigned long yzAlarmType,bool bopenalarm,ULONGLONG   yzVieolossChnnBits);

private:
	static	PUB_THREAD_RESULT PUB_THREAD_CALL YuanZhengUpdateThread(void *pParam);
	void YuanZhengUpdateProc();


	PUB_thread_t		m_hIpUpdateID;
	bool				m_bIpUpdateProc;

	CYuanZhengClinet    m_yuanZhengClient;

	int                 m_count;
	CPUB_Lock           m_lock;

	YUANZHENG_PARAM m_yuanZhengParam;
	unsigned short m_serverPort;

	unsigned long  m_yzAlarmType;//报警类型
	ULONGLONG      m_yzVieolossChnnBits;//视频丢失通道号

};




#endif
