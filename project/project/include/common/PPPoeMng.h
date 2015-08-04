/////PPPoeMng.h
#ifndef FILE_PPPOEMNG_H
#define FILE_PPPOEMNG_H

#include <pthread.h>
#include "PUB_common.h"

const int  MaxLengthIfName=6;

const char configFileName[]="/etc/ppp/pppoe.conf";
const char ifName[]=    "/etc/ppp/activeIfName";
const char pppoe_start[]="pppoe-start";
const char pppoe_start_bg[]="pppoe-start &";						//以后台方式执行，在启动的时候防止等待时间太长
const char pppoe_setup[]="pppoe-setup > /tmp/pppoesetupinfo";
const char pppoe_stop[]= "pppoe-stop";
const char pppoe_status[]="pppoe-status /etc/ppp/pppoe.conf>/tmp/pppoe-status-file;echo \"\\$?\"=$?>>/tmp/pppoe-status-file";
const char pppoe_status_file[]="/tmp/pppoe-status-file";


enum _pppoe_status_
{
	PPPOE_STATUS_UNKNOW		= 0,				//检测PPPOE状态时出错
	PPPOE_STATUS_STOP		= 1,				//没有运行PPPOE
	PPPOE_STATUS_RUNNING	= 2,				//PPPOE运行中，但没有有效信息，比如没有获得有效IP
	PPPOE_STATUS_VALID		= 3,				//PPPOE有效
};

class CPPPOEMng
{
public:
	~CPPPOEMng();
	static CPPPOEMng* Instance();
	
	char* GetIfName() const;				//access variable pIfName
	void Initial();
	bool Start(const char *userName, const char *passWord);//pppoe connect
	bool Stop();							//pppoe disconnet
	int TestPppoe( const char *userName, const char *passWord );
	int IsPppoeRunning();					//检测PPPOE是否在运行中

private:
	CPPPOEMng();
	bool SetIfName();						//set variable(pIfName) value
	bool SetUserInfo(const char *userName, const char *passWord);			//save user name and password
	bool Login();							//pppoe conection or disconnection
	bool LoginBG();
	bool Logout();
	bool StartAutoLogin();
	static RESULT WINAPI AutoLogin(LPVOID lpParameter);	// callback function for auto login


private:


	char* m_pIfName;						//active interface name
	bool m_HasLonginSuc;
	bool m_IsAutoLogin;						//mark of start AutoLogin for auto login!
	HANDLE m_ThreadHandle;					//thread handle of autologin
	CWaitEvent m_WaiteStop;					//waite for use to stop autoLogin thread
	CPUB_Lock m_MuxAcessFile;				//mutex for access config files
	
};
#endif



