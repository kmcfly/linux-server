#ifndef _TIMEZONE_DSTCFG_H_
#define _TIMEZONE_DSTCFG_H_

#include <time.h>
#include <sys/time.h>
#include <string.h>

#include "PUB_common.h"
#include "dvrdvsdef.h"
#include "Calendar.h"

#define TIMEZONECOUNT		34
#define DEFAULTTZONE		TimeZoneName[14]
#define DAYINSECOND			24*60*60
#define WEAKINSECOND		7*DAYINSECOND

#define MINDIFFTIME			2*60
#define TENMIN				10*60
#define FIVEMIN				5*60


#define NTPPORT				123
#define NTP_PACKET_MIN		48

#define NTP_PACKET_MIN       48        /* Without authentication */
#define JAN_1970   2208988800.0        /* 1970 - 1900 in seconds */
#define NTP_SCALE  4294967296.0        /* 2^32, of course! */
#define NTP_ORIGINATE        24        /* Offset of originate timestamp */
#define NTP_RECEIVE          32        /* Offset of receive timestamp */
#define NTP_TRANSMIT         40        /* Offset of transmit timestamp */
#define NTP_DISP_FIELD        8        /* Offset of dispersion field */
#define NTP_REFERENCE        16        /* Offset of reference timestamp */
#define NTP_INSANITY     3600.0        /* Errors beyond this are hopeless */

typedef struct NTP_DATA {
	unsigned char status, version, mode, stratum, polling;
	signed char precision;
	double dispersion, reference, originate, receive, transmit, current;
} ntp_data;



const char TimeZoneName[][16] = { "GMT-12:00", "GMT-11:00", "GMT-10:00", "GMT-09:00", "GMT-08:00", 
								  "GMT-07:00", "GMT-06:00", "GMT-05:00", "GMT-04:30", "GMT-04:00", 
								  "GMT-03:30",  "GMT-03:00", "GMT-02:00", "GMT-01:00", "GMT+00:00",
								  "GMT+01:00", "GMT+02:00", "GMT+03:00", "GMT+03:30", "GMT+04:00", 
								  "GMT+04:30", "GMT+05:00", "GMT+05:30", "GMT+05:45", "GMT+06:00",
								  "GMT+06:30", "GMT+07:00", "GMT+08:00", "GMT+09:00", "GMT+09:30", 
								  "GMT+10:00", "GMT+11:00", "GMT+12:00", "GMT+13:00", "0"};



class CTZoneDZoneCfg
{
public:
	static CTZoneDZoneCfg *Instance();

	~CTZoneDZoneCfg();

	bool SetTimeZone(const long lTZone);										//设置时区信息
	long GetTimeZone(); //返回-1 为无效值
	bool SetDaylightZone(const DAYLIGHT_INFO *pDaylightInfo);					//设置daylight信息
	
	inline void SetNtpServer(const char *pNTPServerName);						//设置ntp名称
	bool LocalTimeSYNNTPServer(const bool bSYNTime);							//设置本地时间和NTP服务器同步
	
	static void ReviseSystemTime(int sigNum);									//
	bool UpdateTimeNow(char *pNTPServerName);														//立即同步时间

protected:
	
	void  ydayTotm( struct tm *pTmTime, char *pYday, bool bStart=true );		//从这一年的第几天到到tm转换
	void  tmToyday( struct tm *pTmTime , char *pYday,bool bDSTEnd=false );		//从tm转换到这一年的第几天
	bool  GetEnviromentTZ();													//获得系统环境变量TZ的值
	bool  SetTZToSystem();														//从新设置TZ环境变量
	void  MakeDaylightInfo(const DAYLIGHT_INFO *pDaylightInfo, bool bOnlyDZone=false);	//生成DST的相关信息，包括偏移量、开始、结束时间
	int   GetMonthWeeks( const int year, const int month, int &firstwday, int &lastwday );						//获得这一年的某月有几个星期
	int IsLeapYear(int year, int month, int mday);

	static bool MakeNtpPacket(ntp_data &ntpData);
	static bool PackNtpPacket(unsigned char *transmit, ntp_data *ntp_packets);
	static bool UnpackNtpPacket(ntp_data *data, unsigned char *packet, int length);

private:
	CTZoneDZoneCfg();

	char m_TimeZone[20];
	char m_DaylightZone[20];
	char m_DSTStart[20];
	char m_DSTEnd[20];
	char m_TZoneDZoneInfo[100];

	long m_curZone; //当前的时区													
	static char	m_ntpServerName[132];											//存储网络时间同步服务器名字
	static DAYLIGHT_INFO m_lastDaylightInfo;									//存储上次设置的DST信息
	//static struct sigaction m_LastSigAct;										

};

void CTZoneDZoneCfg::SetNtpServer(const char *pNTPServerName)
{
	assert( NULL != pNTPServerName );
	strcpy( m_ntpServerName, pNTPServerName );
	//printf("%s():%s:%d, set the NTP server=%s\n", __FUNCTION__, __FILE__, __LINE__, m_ntpServerName);
}

#endif

