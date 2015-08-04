#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>


#include <sys/timex.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "TZoneDZoneCfg.h"
#include "MessageMan.h"
#include "NetInterface.h"



char CTZoneDZoneCfg::m_ntpServerName[132]={0};
DAYLIGHT_INFO CTZoneDZoneCfg::m_lastDaylightInfo;


CTZoneDZoneCfg::CTZoneDZoneCfg() : m_curZone(-1)
{
	memset(m_TimeZone, 0, sizeof(m_TimeZone));
	memset(m_DaylightZone, 0, sizeof(m_DaylightZone));
	memset(m_DSTStart, 0, sizeof(m_DSTStart));
	memset(m_DSTEnd, 0, sizeof(m_DSTEnd));
	memset(m_TZoneDZoneInfo, 0, sizeof(m_TZoneDZoneInfo));

}

CTZoneDZoneCfg::~CTZoneDZoneCfg()
{

}

CTZoneDZoneCfg * CTZoneDZoneCfg::Instance()
{
	static CTZoneDZoneCfg TZoneDZoneCfg;
	return &TZoneDZoneCfg;
}


bool CTZoneDZoneCfg::SetTimeZone(const long lTZone)
{
	assert( 0 <= lTZone );
	assert( TIMEZONECOUNT > lTZone );
	if( ( lTZone < 0 ) || ( lTZone > TIMEZONECOUNT ) )
	{
		printf("%s:%s:%d, the time zone is not right\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}
	//printf("%s:%s:%d, set time zone\n", __FUNCTION__, __FILE__, __LINE__);

	//int i;
	//for( i=0; i<TIMEZONECOUNT;i++)
	//{
	//	if( lTZone == TimeZoneValue[i] )
	//		break;
	//}
	//assert( TIMEZONECOUNT != i);

	//printf("%s():%s:%d, timezone=%d\n", __FUNCTION__, __FILE__, __LINE__, lTZone);
	GetEnviromentTZ();
	//printf("%s():%s:%d, at now the TZ info=%s, m_TimeZone=%s, m_DaylightZone=%s, m_DSTStart=%s, m_DSTEnd=%s\n",
	//	__FUNCTION__, __FILE__,__LINE__,
	//	m_TZoneDZoneInfo, m_TimeZone, m_DaylightZone, m_DSTStart, m_DSTEnd);

	memset( m_TimeZone, 0, sizeof( m_TimeZone ));
	strcpy( m_TimeZone, TimeZoneName[lTZone]);
	//printf("%s():%s:%d, the ");
	if( TIME_ZONE_GMT != lTZone )
	{
		char c = '\0';
		if( '+' == m_TimeZone[3] )
			c = '-';
		else if( '-' == m_TimeZone[3] )
			c = '+';
		m_TimeZone[3] = c;
	}
	//printf("%s:%s:%d, the new time zone=%s\n", __FUNCTION__, __FILE__, __LINE__, m_TimeZone);

	MakeDaylightInfo( &m_lastDaylightInfo);

	m_curZone = lTZone;
	return SetTZToSystem();

}

long CTZoneDZoneCfg::GetTimeZone()
{
	return m_curZone;
}

bool CTZoneDZoneCfg::SetDaylightZone(const DAYLIGHT_INFO *pDaylightInfo)
{
	assert( NULL != pDaylightInfo );
	//printf("%s:%s:%d, set daylight zone\n", __FUNCTION__, __FILE__, __LINE__);
	GetEnviromentTZ();

	MakeDaylightInfo( pDaylightInfo );

	m_lastDaylightInfo = *pDaylightInfo;

	bool bretval = SetTZToSystem();
	//printf("%s:%s:%d, will set the daylight zone info\n", __FUNCTION__, __FILE__, __LINE__);
	return bretval;

}

/*****************************************************************
Copyright (C),2009-2012, tongwei video Tech. Co., Ltd.

FileName:LocalTimeSYNNTPServer(const bool bSYNTime)

Author: TVT-ZHL-146  Version:	Date:

Description:
1:把本地时间和NTP服务器时间同步。
2:bSYNTime=true时设置一个间隔计时器， 每天0点时和NTP服务器同步一次
3：bSYNTime=flase时关闭间隔计时器功能

Version:

Function List:

History:

<author>  <time>  <version> <desc>
******************************************************************/

bool CTZoneDZoneCfg::LocalTimeSYNNTPServer(const bool bSYNTime)
{
	if( true == bSYNTime )
	{
		unsigned int currentTime;
		unsigned int firstAlarm;

		struct itimerval iTimer;
		struct sigaction SigAction;

		//设置信号处理函数
		SigAction.sa_handler = ReviseSystemTime;
		SigAction.sa_flags = 0;
		sigemptyset(&SigAction.sa_mask);
		sigaction(SIGALRM, &SigAction, NULL);

		//设置间隔计时器
		currentTime = ( GetCurrTime() / 1000000 );
		firstAlarm = GetZeroTime32OfDate( currentTime + DAYINSECOND ) - currentTime;
#if 1
		//printf("%s:%s:%d, the first time sys will hapen after %d second\n", __FUNCTION__, __FILE__, __LINE__, firstAlarm);
		iTimer.it_value.tv_sec = firstAlarm;
		iTimer.it_value.tv_usec = 0;
		iTimer.it_interval.tv_sec = WEAKINSECOND;
		iTimer.it_interval.tv_usec = 0;
		setitimer(ITIMER_REAL, &iTimer,NULL);
#else
		iTimer.it_value.tv_sec = FIVEMIN;
		iTimer.it_value.tv_usec = 0;
		iTimer.it_interval.tv_sec = TENMIN;
		iTimer.it_interval.tv_usec = 0;
		setitimer(ITIMER_REAL, &iTimer,NULL);
#endif
	}
	else
	{
		//printf("%s:%s:%d, stop to syn to the ntp server\n", __FUNCTION__, __FILE__, __LINE__);
		sigaction(SIGALRM, NULL, NULL);
		setitimer(ITIMER_REAL,NULL, NULL);
	}

}




//从这一年的第几天到到tm转换	
void  CTZoneDZoneCfg::ydayTotm( struct tm *pTmTime, char *pYday, bool bStart )
{
	assert( NULL != pTmTime );
	assert( NULL != pYday );


	bool bHaveYday = false;
	int day = 0, hour = 0,  min = 0 , sec = 0;
	LONGLONG currentUTCTime = 0;
	char *pYdayTemp = pYday;
	time_t utctime = (time_t)0;

	//如果pYday为空就把pTmTime初始化为当前系统时间
	if( (*pYdayTemp == 'J' ) || ( *pYdayTemp == 'j') )
	{
		pYdayTemp++;
		if( sscanf( pYdayTemp, "%d/%d:%d:%d", &day, &hour, &min, &sec) >0)
		{
			currentUTCTime = GetCurrTime();	
			struct tm time = DVRTimeToTm(currentUTCTime);

			pTmTime->tm_mon = 0;
			pTmTime->tm_mday = day;
			pTmTime->tm_hour = hour;
			pTmTime->tm_min = min;
			pTmTime->tm_sec = sec;
			pTmTime->tm_year = time.tm_year;

			//转换为UTC时间
			currentUTCTime = TmToDVRTime( *pTmTime );
			memset( pTmTime, 0, sizeof( struct tm ) );
			if( true == bStart )
				*pTmTime = DVRTimeToTm( currentUTCTime - 1000000 );
			else
				*pTmTime = DVRTimeToTm( currentUTCTime );

			bHaveYday = true;

		}
	}

	if( false == bHaveYday )
	{
		currentUTCTime = GetCurrTime();
		*pTmTime = DVRTimeToTm(currentUTCTime);

		pTmTime->tm_hour = 0;
		pTmTime->tm_min = 0;
		pTmTime->tm_sec = 0;
	}

}


//从tm转换到这一年的第几天
void  CTZoneDZoneCfg::tmToyday( struct tm *pTmTime , char *pYday, bool bDSTEnd )
{
	assert(NULL != pTmTime );
	assert(NULL != pYday );
	struct tm tmTime;
	memset( &tmTime, 0, sizeof(struct tm));

	pTmTime->tm_year -= 1900;
	pTmTime->tm_isdst = 0;					//该标志为0表示mktime时不考虑DST信息

	//设置到TZ变量中的时间要和界面中输入的时间相同，不能添加时区和DST信息
	time_t gmTime=mktime(pTmTime);			//转换为标准UTC时间。去掉了时区和DST信息
	gmTime -= timezone;						//添加当前时区信息，这样才和界面输入时间对应

	if ( !bDSTEnd )
	{
		gmTime += 1;
	}

	if( NULL == gmtime_r( &gmTime, &tmTime))//把以秒为单位的时间转换为UTC的时间
	{
		printf("set tz and dz\n");
		assert(false);
	}

	//printf("%s:%s:%d, %d-%d-%d, %d:%d:%d\n", __FUNCTION__, __FILE__, __LINE__,tmTime.tm_year + 1900, tmTime.tm_mon + 1, 
	//		tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min,tmTime.tm_sec);
	sprintf( pYday,"J%d/%d:%d:%d", tmTime.tm_yday + IsLeapYear(pTmTime->tm_year, pTmTime->tm_mon, pTmTime->tm_mday), tmTime.tm_hour,tmTime.tm_min,tmTime.tm_sec );
}




bool CTZoneDZoneCfg::GetEnviromentTZ()
{
	const char *pnowTZ = NULL;
	char *pFirst = NULL;
	char *pSecond = NULL;
	int i;

	memset( m_TimeZone, 0, sizeof( m_TimeZone ) );
	memset( m_DaylightZone, 0, sizeof( m_DaylightZone ) );
	memset( m_DSTStart, 0, sizeof( m_DSTStart ) );
	memset( m_DSTEnd, 0, sizeof( m_DSTEnd ) );
	memset( m_TZoneDZoneInfo, 0, sizeof(m_TZoneDZoneInfo));

	pnowTZ = getenv("TZ");
	pnowTZ = ( NULL != pnowTZ)?pnowTZ:DEFAULTTZONE;
	strcpy( m_TZoneDZoneInfo, pnowTZ );

	//printf("%s():%s:%d, before the time zone info: TZ=%s, daylight=%d, timezone=%ld, time zone name=%s:%s\n",\
	//	__FUNCTION__, __FILE__, __LINE__,\
	//	m_TZoneDZoneInfo, daylight, timezone,tzname[0], tzname[1]);

	pFirst = strstr(m_TZoneDZoneInfo, "GMT");
	assert( NULL != pFirst );
	if( NULL != pFirst )
		pSecond = strstr(pFirst+3, "GMT");
	if( NULL !=  pSecond )
	{
		//系统设置了时区和夏令时功能
		strncpy(m_TimeZone, m_TZoneDZoneInfo, ( pSecond - pFirst) );		//读时区字符串
		m_TimeZone[pSecond-pFirst] = '\0';

		pFirst = NULL;														//读DST时区信息
		pFirst = strstr(pSecond, ",");
		if( NULL != pFirst )
		{
			strncpy(m_DaylightZone, pSecond, (pFirst-pSecond));
			m_DaylightZone[pFirst-pSecond]='\0';
		}
		else
		{
			strcpy(m_DaylightZone, pSecond);

			//printf("%s:%s:%d get TZ info OK\n", __FUNCTION__, __FILE__, __LINE__);
			return true;
		}

		pFirst += 1;														//读DST开始时间
		pSecond = strstr(pFirst, ",");
		if( NULL == pSecond )
		{
			//printf("%s:%s:%d, set the DST end time error\n", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}
		assert(NULL != pSecond);
		strncpy(m_DSTStart, pFirst, (pSecond-pFirst));
		m_DSTStart[pSecond-pFirst] = '\0';

		pSecond += 1;														//读DST结束时间
		strcpy( m_DSTEnd, pSecond);

		//printf("%s:%s:%d get TZ info OK\n", __FUNCTION__, __FILE__, __LINE__);		
		return true;
	}
	else
	{
		//系统设置了时区当没设置夏令时功能
		memset( m_TimeZone, 0, sizeof(m_TimeZone));
		strcpy( m_TimeZone, m_TZoneDZoneInfo );		

		//printf("%s:%s:%d get TZ info OK\n", __FUNCTION__, __FILE__, __LINE__);
		return true;

	}
}

bool CTZoneDZoneCfg::SetTZToSystem()
{
	//printf("%s:%s:%d, set the TZ info to the system\n", __FUNCTION__, __FILE__, __LINE__);
	const char *pnowTZ = NULL;
	memset(m_TZoneDZoneInfo, 0, sizeof(m_TZoneDZoneInfo));
	strcat(m_TZoneDZoneInfo, m_TimeZone);						//始终有时区信息
	strcat(m_TZoneDZoneInfo,m_DaylightZone);

	if( ( 0 < strlen(m_DSTEnd)) && ( 0 < strlen(m_DSTStart)))
	{
		strcat(m_TZoneDZoneInfo, ",");
		strcat(m_TZoneDZoneInfo, m_DSTStart);

		strcat(m_TZoneDZoneInfo, ",");
		strcat(m_TZoneDZoneInfo, m_DSTEnd);
	}
	//printf("%s():%s:%d, set new TZ=%s\n", __FUNCTION__, __FILE__, __LINE__, m_TZoneDZoneInfo);
	tzset();
	if( 0 != setenv("TZ", m_TZoneDZoneInfo, 1) )
	{
		printf("%s():%s:%d, set the time zone error\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}
	tzset();

	pnowTZ = getenv("TZ");
	pnowTZ = pnowTZ ?pnowTZ:DEFAULTTZONE;
	//printf("%s():%s:%d, the new TZ = %s\n", __FUNCTION__, __FILE__, __LINE__,m_TZoneDZoneInfo);
	//printf("%s():%s:%d, after the time zone info: TZ=%s, daylight=%d, timezone=%ld, time zone name=%s:%s\n",\
	//	__FUNCTION__, __FILE__, __LINE__,\
	//	pnowTZ, daylight, timezone,tzname[0], tzname[1]);

	return true;
}


void CTZoneDZoneCfg::MakeDaylightInfo(const DAYLIGHT_INFO *pDaylightInfo, bool bOnlyDZone)
{

	assert( ( DAYLIGHT_TYPE_WEEK == pDaylightInfo->type ) || ( DAYLIGHT_TYPE_DAY == pDaylightInfo->type ) );

	char c;
	//printf("%s():%s;%d, useDST=%d\n", __FUNCTION__, __FILE__, __LINE__, m_lastDaylightInfo.enable);
	int inHour = 0, inMin = 0, inSecond = 0;
	int outHour = 0, outMin = 0, outSecond = 0;

	//计算daylight zone信息
	int tzHour = 0, tzMin = 0, tzSecond = 0;
	int dzHour = 0, dzMin = 0, dzSecond = 0;

	unsigned long offset;

	if( ( !( pDaylightInfo->enable ) ) || ( 0 == pDaylightInfo->offSet ) )
	{
		//printf("%s:%s:%d, not used the dst info\n", __FUNCTION__, __FILE__, __LINE__);
		memset(m_DaylightZone, 0, sizeof(m_DaylightZone));
		memset(m_DSTStart, 0, sizeof(m_DSTStart));
		memset(m_DSTEnd, 0, sizeof(m_DSTEnd));
		return;
	}

	if( 0 == strcmp(m_TimeZone, "GMT+00:00") )
	{
		c = '-';
		offset = pDaylightInfo->offSet;
	}
	else
	{
		c = m_TimeZone[3];
		sscanf(m_TimeZone+4,"%d:%d:%d", &tzHour, &tzMin, &tzSecond);
		int i = -1;
		if( '-' == m_TimeZone[3] )
		{
			i = 1;
		}

		offset =  ( (tzHour * 60 * 60 ) + ( tzMin * 60 ) + tzSecond ) + ( i * pDaylightInfo->offSet );
		//printf("%s:%s:%d, pDaylightInfo->offSet=%d\n", __FUNCTION__, __FILE__, __LINE__, pDaylightInfo->offSet);
	}
	dzHour = offset / ( 60 * 60 );
	dzMin  = ( offset % ( 60 * 60 ) ) / 60;
	dzSecond = ( offset % ( 60 * 60 ) ) % 60;

	sprintf(m_DaylightZone, "GMT%c%d:%d:%d",c, dzHour, dzMin, dzSecond );

	if( true == bOnlyDZone )
	{
		//printf("%s:%s:%d, not used the dst info\n", __FUNCTION__, __FILE__, __LINE__);

		return;
	}
	inHour = pDaylightInfo->InSecond /( 60 * 60 );							//DST开始的小时、分钟、秒
	inMin  = ( pDaylightInfo->InSecond %( 60 * 60) ) / 60;
	inSecond = ( pDaylightInfo->InSecond %( 60 * 60) ) % 60;


	outHour = pDaylightInfo->OutSecond /( 60 * 60 );						//DST结束的小时、分钟、秒
	outMin  = ( pDaylightInfo->OutSecond %( 60 * 60) ) / 60;
	outSecond = ( pDaylightInfo->OutSecond %( 60 * 60) ) % 60;

	if( DAYLIGHT_TYPE_DAY == pDaylightInfo->type )
	{
		//日期模式时，TZ变量使用的是从这一年的第几天开始DST到第几天结束DST
		//比如TZ=PST-8:0:0PDT-7:0:0,J65/02:0:0,J270/02:0:0表示美国太平洋时间为西八区，从每年的第65天的凌晨2点进入DST时间，到这一年的270天的凌晨2点结束DST时间。DST的偏移量为1小时
		//如果在南半球设置为TZ=GST-8GDT-7, J300/02:0:0,J80/02:0:0
		//每个符号间不能有空格
		struct tm tmTimeStart;
		struct tm tmTimeEnd;

		memset( &tmTimeStart, 0 ,sizeof( struct tm ) );
		memset( &tmTimeEnd, 0 ,sizeof( struct tm ) );

		tmTimeStart.tm_year = pDaylightInfo->InYear;
		tmTimeStart.tm_mon  = pDaylightInfo->InMonth;
		tmTimeStart.tm_mday = pDaylightInfo->InMday;
		tmTimeStart.tm_hour = inHour;
		tmTimeStart.tm_min  = inMin;
		tmTimeStart.tm_sec  = inSecond;
		tmToyday( &tmTimeStart, m_DSTStart );

		tmTimeEnd.tm_year = pDaylightInfo->OutYear;
		tmTimeEnd.tm_mon  = pDaylightInfo->OutMonth;
		tmTimeEnd.tm_mday = pDaylightInfo->OutMday;
		tmTimeEnd.tm_hour = outHour;
		tmTimeEnd.tm_min  = outMin;
		tmTimeEnd.tm_sec  = outSecond;
		tmToyday( &tmTimeEnd, m_DSTEnd, true);

	}
	else if(DAYLIGHT_TYPE_WEEK == pDaylightInfo->type)
	{	
		//周格式为TZ=CST+08:30:00CST+10:00:00,M3.1.0/02:0:0,M10.1.0/02:0:0.表示DST从3月第1周的周日
		//在周模式时月份从1到12、 周从1开始、这一周的第一天为星期天，用0来表示
		//周模式时应该理解为第几个星期几，而不是第几周的星期几,比如美国使用3月的第二个星期日(M3.2.0/02:00:00)做为DST的开始，使用11月的第一个星期天(M11.1.0/02:0:0)作为DST的结束
		//在表示第几个星期几的时候，最多有5个星期
		
		int monthWeaks = 0;
		int monthWUse = 0;
		int firstwday = 0;
		int lastwday = 0;

		struct tm tmTime = DVRTimeToTm(GetCurrTime());					
		tmTime.tm_year += 1900;

		monthWeaks = GetMonthWeeks( tmTime.tm_year, pDaylightInfo->InMonth + 1, firstwday, lastwday);
		//修正周，如果选择最后一周，则使用的是实际上这个月有几周的数字值
		monthWUse = ( ( pDaylightInfo->InWeekIndex + 1 ) >  monthWeaks )?(monthWeaks):( pDaylightInfo->InWeekIndex + 1 );
		//monthUse = ( ( pDaylightInfo->InWeekIndex + 1 ) >  5 )?(5):( pDaylightInfo->InWeekIndex + 1 );
		//如果选择的这一周的是最后一周，要检测星期是否越界。比如说2012年四月最后一个星期二是4月24号，也就是这个月的第4周的星期二。
		//但是4月最后一个星期一则是4月30号
		if (monthWeaks == monthWUse)
		{
			//如果是最后一周，并且星期值大于最后一天的星期值，就把周减一
			if (pDaylightInfo->InWday > lastwday)
			{
				monthWUse = monthWeaks - 1;
				if (pDaylightInfo->InWday < firstwday)
				{
					monthWUse -= 1;
				}
			}
			else 
			{
				if (0 != firstwday && pDaylightInfo->InWday < firstwday)
				{
					//如果小于这个月第一天的星期值就需要把周减一
					monthWUse = monthWeaks - 1;
				}
			}
		}
		else
		{
			//if (0 < firstwday && pDaylightInfo->InWday <= firstwday)
			//{
			//	monthWUse = monthWUse + 1;
			//}
		}

		assert( 7> monthWeaks );
		sprintf(m_DSTStart, "M%d.%d.%d/%d:%d:%d",pDaylightInfo->InMonth + 1, monthWUse  ,pDaylightInfo->InWday, inHour, inMin,inSecond );

		if( pDaylightInfo->InMonth > pDaylightInfo->OutMonth )				//DST跨年的情况
			tmTime.tm_year += 1;

		monthWeaks = 0;
		monthWeaks = GetMonthWeeks( tmTime.tm_year, pDaylightInfo->OutMonth + 1, firstwday, lastwday );
		monthWUse = ( ( pDaylightInfo->OutWeekIndex + 1 ) > monthWeaks )?monthWeaks:( pDaylightInfo->OutWeekIndex + 1 );
		//monthWeaks = ( ( pDaylightInfo->OutWeekIndex + 1 ) > 5 )?5:( pDaylightInfo->OutWeekIndex + 1 );
		if (monthWeaks == monthWUse)
		{
			//如果是最后一周，并且星期值大于最后一天的星期值，就把周减一
			if (pDaylightInfo->OutWday > lastwday)
			{
				monthWUse = monthWeaks - 1;
			}
			else if (0 != firstwday && pDaylightInfo->OutWday < firstwday)
			{
				//如果小于这个月第一天的星期值就需要把周减一
				monthWUse = monthWeaks - 1;
			}
		}
		else
		{
			//if (0 < firstwday && pDaylightInfo->OutWday <= firstwday)
			//{
			//	monthWUse = monthWUse + 1;
			//}
		}
		sprintf(m_DSTEnd, "M%d.%d.%d/%d:%d:%d",pDaylightInfo->OutMonth + 1, monthWUse, pDaylightInfo->OutWday, outHour, outMin,outSecond );

	}

	//printf("%s:%s:%d, make  the dst info OK\n", __FUNCTION__, __FILE__, __LINE__);
}

int CTZoneDZoneCfg::GetMonthWeeks( const int year, const int month, int &firstwday, int &lastwday )
{
	// 计算本月有几周
	CCalendar Calendar;
	struct tm tbuf;
	//int lastwday;
	//int firstwday;

	memset( &tbuf, 0, sizeof(struct tm) );
	tbuf.tm_year = year - 1900;
	tbuf.tm_mon  = month - 1;

	int monthdsys = Calendar.GetMonthDays( year,month );

	tbuf.tm_mday = 1;
	mktime( &tbuf );
	firstwday = tbuf.tm_wday;

	tbuf.tm_mday = monthdsys;
	mktime( &tbuf );
	lastwday = tbuf.tm_wday;

	int weaks = ( monthdsys + firstwday + ( SUM_WK_DAYS - lastwday - 1 ) ) / SUM_WK_DAYS;

	return weaks;

}

int CTZoneDZoneCfg::IsLeapYear(int year, int month, int mday)
{
	if (0 == (year & 3) && (0 != year % 100 || 0 == year % 400))
	{
		//是闰年
		if (1 < month)
		{
			return 0;
		}
		else if (1 == month && 29 == mday)
		{
			return 0;
		}
	}

	return 1;
}

void CTZoneDZoneCfg::ReviseSystemTime(int sigNum)
{
	// printf("%s:%s:%d, get the alarm signal %d\n", __FUNCTION__, __FILE__, __LINE__, sigNum);

	//printf("%s():%s:%d, will connect to the ntp server\n", __FUNCTION__, __FILE__, __LINE__);

	struct sockaddr_in ntpServerIn;
	int NTPServerfd = -1;
	struct hostent *pNtpServerHost = NULL;
	socklen_t sockLen = sizeof(struct sockaddr_in);
	struct  timeval timeOut;

#ifdef __SUPPORT_3G__
	if (!CNetInterface::Instance()->GetNICRunning("ppp0"))
	{
		if(!CNetInterface::Instance()->GetNICRunning())
		{
			printf("%s:%s:%d, 没有插网线\n", __FUNCTION__, __FILE__, __LINE__);
			return ;
		}
	}
#else
	if(!CNetInterface::Instance()->GetNICRunning())
	{
		printf("%s:%s:%d, 没有插网线\n", __FUNCTION__, __FILE__, __LINE__);
		return ;
	}
#endif

	struct in_addr serverIp;
	serverIp.s_addr = inet_addr(m_ntpServerName);
	if (INADDR_NONE == serverIp.s_addr)
	{
#if USE_LOCAL_DNSRESOLVE
		pNtpServerHost = CNetInterface::Instance()->GetHostByName( m_ntpServerName );
#else
		pNtpServerHost = gethostbyname( m_ntpServerName );
#endif
		if( NULL == pNtpServerHost )
		{
			perror("gethostbyname() error");
			return;
		}
		else
		{
			serverIp = *(struct in_addr *)(pNtpServerHost->h_addr);

#if USE_LOCAL_DNSRESOLVE
			CNetInterface::Instance()->ReleaseHost( pNtpServerHost );
#endif
		}
	}

	//printf("the NTP host name=%s\n", pNtpServerHost->h_name);

	memset(&ntpServerIn, 0, sizeof(struct sockaddr_in));
	ntpServerIn.sin_family = AF_INET;
	ntpServerIn.sin_port = htons(NTPPORT);
	ntpServerIn.sin_addr = serverIp;

	//printf("the NTP server ip=%s\n", inet_ntoa(ntpServerIn.sin_addr));

	NTPServerfd = socket(PF_INET,  SOCK_DGRAM, 0);
	if( -1 == NTPServerfd )
	{
		perror("socket() error");
		return;
	}

	timeOut.tv_sec   = 2;
	timeOut.tv_usec = 0;
	if( -1 ==  setsockopt(NTPServerfd, SOL_SOCKET,  SO_RCVTIMEO, &timeOut, sizeof(struct timeval)) )
	{
		perror("setsockopt() error");
		close( NTPServerfd );
		NTPServerfd = -1;
		return;
	}

	//printf("%s():%s:%d, the m_NTPServerfd=%d\n", __FUNCTION__, __FILE__, __LINE__, NTPServerfd);
	NTP_DATA ntp_packets;
	unsigned char transmit[NTP_PACKET_MIN]={0};
	if (!MakeNtpPacket(ntp_packets))
	{
		close(NTPServerfd);
		NTPServerfd = -1;
		//printf("(%s %d) Failed: MakeNtpPacket failed,%s\n",__FILE__,__LINE__,strerror(errno));
		return ;
	}
	PackNtpPacket(transmit,&ntp_packets);
	char revBuffer[240] = {0};
	int revLength = 0;
	//printf("%s:%s:%d, will send NTP message to the ntp server %s\n", __FUNCTION__, __FILE__, __LINE__,m_ntpServerName);
	for (int i = 0; i<3; i++)
	{
		revLength = sendto(NTPServerfd, (const char*)transmit, NTP_PACKET_MIN, 0, (struct sockaddr *)&ntpServerIn, sizeof(struct sockaddr));
		if ( -1==revLength )
		{
			continue;
		}

		memset(revBuffer,0,240);
		revLength = recvfrom(NTPServerfd,revBuffer,240,0,(sockaddr*)&ntpServerIn,&sockLen);
		if (revLength > 0)
		{
			NTP_DATA ntp_temp;
			UnpackNtpPacket(&ntp_temp,(unsigned char*)revBuffer,revLength);
			if (ntp_temp.originate == 0.0 || ntp_temp.receive == 0.0)
			{
				//printf("(%s %d) Failed: insanity data!\n",__FILE__,__LINE__);
				continue;
			}
			if ( (ntp_temp.transmit == 0.0) || (ntp_temp.stratum != 0 && ntp_temp.reference == 0.0) )
			{        
				//printf("(%s %d) Failed: insanity data!\n",__FILE__,__LINE__);
				continue;
			}
			if ( (ntp_temp.reference != 0.0) && (ntp_temp.receive < ntp_temp.reference) ||(ntp_temp.dispersion > NTP_INSANITY) )
			{
				//printf("(%s %d) Failed: insanity data!\n",__FILE__,__LINE__);
				continue;
			}

			//check data sanity
			double delay1 = ntp_temp.transmit-ntp_temp.receive;
			if ( (delay1 < 0.0) || (delay1 > NTP_INSANITY) )
			{
				//printf("(%s %d) Failed: insanity data!\n",__FILE__,__LINE__);
				continue;
			}

			double delay2 = ntp_temp.current-ntp_temp.originate;
			if ( delay2 < 0.0 )
			{
				//printf("(%s %d) Failed: insanity data!\n",__FILE__,__LINE__);
				continue;
			}

			double newtime = ntp_temp.transmit-JAN_1970;
			unsigned long ntptime = (unsigned long)newtime;
			close(NTPServerfd);
			long diffTime = GetCurrTime32() - ntptime;
			diffTime = ( 0 > diffTime )?( -diffTime ):diffTime;
			if( MINDIFFTIME < diffTime )
			{
				CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_L_CHANGE_TIME, (unsigned char *)&ntptime, sizeof(unsigned long));
			}
			else
			{

			}
			return;
		}
		else
		{
			perror("get message from server");
		}
	}
	close(NTPServerfd);

}

bool CTZoneDZoneCfg::UpdateTimeNow( char *pNTPServerName )
{
	assert( NULL != pNTPServerName );
	if( NULL == pNTPServerName )
	{
		return false;
	}
#ifdef __SUPPORT_3G__
	if (!CNetInterface::Instance()->GetNICRunning("ppp0"))
	{
		if(!CNetInterface::Instance()->GetNICRunning())
		{
			printf("%s:%s:%d, 没有插网线\n", __FUNCTION__, __FILE__, __LINE__);
			return false;
		}
	}
#else
	if(!CNetInterface::Instance()->GetNICRunning())
	{
		printf("%s:%s:%d, 没有插网线\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}
#endif

	struct in_addr serverIp;
	serverIp.s_addr = inet_addr(pNTPServerName);
	if (INADDR_NONE == serverIp.s_addr)
	{
#if USE_LOCAL_DNSRESOLVE
		struct hostent *pNtpServerHost = CNetInterface::Instance()->GetHostByName( pNTPServerName );
#else
		struct hostent *pNtpServerHost = gethostbyname( pNTPServerName );
#endif
		if( NULL == pNtpServerHost )
		{
			perror("gethostbyname() error");
			return false;
		}
		else
		{
			serverIp = *(struct in_addr *)(pNtpServerHost->h_addr);

#if USE_LOCAL_DNSRESOLVE
			CNetInterface::Instance()->ReleaseHost( pNtpServerHost );
#endif
		}
	}

	struct sockaddr_in ntpServerIn;
	memset(&ntpServerIn, 0, sizeof(struct sockaddr_in));
	ntpServerIn.sin_family = AF_INET;
	ntpServerIn.sin_port = htons(NTPPORT);
	ntpServerIn.sin_addr = serverIp;

	int NTPServerfd = socket(PF_INET,  SOCK_DGRAM, 0);
	if( -1 == NTPServerfd )
	{
		perror("socket() error");
		return false;
	}

	struct  timeval timeOut = { 2, 0 };
	if( -1 ==  setsockopt(NTPServerfd, SOL_SOCKET,  SO_RCVTIMEO, &timeOut, sizeof(struct timeval)) )
	{
		perror("setsockopt() error");
		close( NTPServerfd );
		NTPServerfd = -1;
		return false;
	}

	NTP_DATA ntp_packets;
	unsigned char transmit[NTP_PACKET_MIN]={0};
	if (!MakeNtpPacket(ntp_packets))
	{
		close(NTPServerfd);
		NTPServerfd = -1;
		return false;
	}
	PackNtpPacket(transmit,&ntp_packets);
	char revBuffer[240] = {0};
	int revLength = 0;
	for (int i = 0; i<3; i++)
	{
		revLength = sendto(NTPServerfd, (const char*)transmit, NTP_PACKET_MIN, 0, (struct sockaddr *)&ntpServerIn, sizeof(struct sockaddr));
		if ( -1==revLength )
		{
			continue;
		}

		memset(revBuffer,0,240);
		socklen_t sockLen = sizeof(struct sockaddr_in);
		revLength = recvfrom(NTPServerfd,revBuffer,240,0,(sockaddr*)&ntpServerIn,&sockLen);
		if (revLength > 0)
		{
			NTP_DATA ntp_temp;
			UnpackNtpPacket(&ntp_temp,(unsigned char*)revBuffer,revLength);
			if (ntp_temp.originate == 0.0 || ntp_temp.receive == 0.0)
			{
				continue;
			}
			if ( (ntp_temp.transmit == 0.0) || (ntp_temp.stratum != 0 && ntp_temp.reference == 0.0) )
			{        
				continue;
			}
			if ( (ntp_temp.reference != 0.0) && (ntp_temp.receive < ntp_temp.reference) ||(ntp_temp.dispersion > NTP_INSANITY) )
			{
				continue;
			}

			//check data sanity
			double delay1 = ntp_temp.transmit-ntp_temp.receive;
			if ( (delay1 < 0.0) || (delay1 > NTP_INSANITY) )
			{
				continue;
			}

			double delay2 = ntp_temp.current-ntp_temp.originate;
			if ( delay2 < 0.0 )
			{
				continue;
			}

			double newtime = ntp_temp.transmit-JAN_1970;
			unsigned long ntptime = (unsigned long)newtime;
			close(NTPServerfd);
			long diffTime = GetCurrTime32() - ntptime;
			diffTime = ( 0 > diffTime )?( -diffTime ):diffTime;
			if( MINDIFFTIME < diffTime )
			{
				CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_L_CHANGE_TIME, (unsigned char *)&ntptime, sizeof(unsigned long));
			}
			else
			{

			}

			return true;
		}
		else
		{
			perror("get message from server");
		}
	}

	close(NTPServerfd);
	NTPServerfd = -1;
	return false;
}


bool CTZoneDZoneCfg::MakeNtpPacket(ntp_data& ntp_packets)
{
	memset( &ntp_packets, 0, sizeof(ntp_packets) );
	ntp_packets.status = 0;
	ntp_packets.stratum = 15;
	ntp_packets.reference = ntp_packets.dispersion = 0.0;
	ntp_packets.version = 3; //ntp 3.0 version
	ntp_packets.mode = 3;       //client
	ntp_packets.polling = 8;
	ntp_packets.precision = 0;
	ntp_packets.receive = ntp_packets.originate = 0.0;
	struct timeval current;
	if (gettimeofday(&current,NULL))
	{
		return false;
	}
	ntp_packets.current = ntp_packets.transmit = JAN_1970+current.tv_sec+1.0e-6*current.tv_usec;
	return true;
}


bool CTZoneDZoneCfg::PackNtpPacket(unsigned char *packet, ntp_data* data)
{	 
	int i, k;double d;
	packet[0] = (data->status<<6)|(data->version<<3)|data->mode;
	packet[1] = data->stratum;
	packet[2] = data->polling;
	packet[3] = data->precision;
	d = data->originate/NTP_SCALE;
	for (i = 0; i < 8; ++i) {
		if ((k = (int)(d *= 256.0)) >= 256) k = 255;
		packet[NTP_ORIGINATE+i] = k;
		d -= k;
	}
	d = data->receive/NTP_SCALE;
	for (i = 0; i < 8; ++i)
	{
		if ((k = (int)(d *= 256.0)) >= 256) k = 255;
		packet[NTP_RECEIVE+i] = k;
		d -= k;
	}
	d = data->transmit/NTP_SCALE;
	for (i = 0; i < 8; ++i)
	{
		if ((k = (int)(d *= 256.0)) >= 256) k = 255;
		packet[NTP_TRANSMIT+i] = k;
		d -= k;
	}  

	return true;
}


bool CTZoneDZoneCfg::UnpackNtpPacket(ntp_data *data, unsigned char *packet, int length)
{
	int i;double d; 
	struct timeval current;
	errno = 0;
	if (gettimeofday(&current,NULL))
		return false;

	data->current = JAN_1970+current.tv_sec+1.0e-6*current.tv_usec;
	data->status = (packet[0] >> 6);
	data->version = (packet[0] >> 3)&0x07;
	data->mode = packet[0]&0x07;
	data->stratum = packet[1];
	data->polling = packet[2];
	data->precision = packet[3];d = 0.0;
	for (i = 0; i < 4; ++i) d = 256.0*d+packet[NTP_DISP_FIELD+i];
	data->dispersion = d/65536.0;
	d = 0.0;
	for (i = 0; i < 8; ++i) d = 256.0*d+packet[NTP_REFERENCE+i];
	data->reference = d/NTP_SCALE;
	d = 0.0;
	for (i = 0; i < 8; ++i) d = 256.0*d+packet[NTP_ORIGINATE+i];
	data->originate = d/NTP_SCALE;
	d = 0.0;
	for (i = 0; i < 8; ++i) d = 256.0*d+packet[NTP_RECEIVE+i];
	data->receive = d/NTP_SCALE;
	d = 0.0;
	for (i = 0; i < 8; ++i) d = 256.0*d+packet[NTP_TRANSMIT+i];
	data->transmit = d/NTP_SCALE;
	return true;
}


