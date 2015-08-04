#ifndef _USER_MAN_H
#define _USER_MAN_H

#include "mylist.h"
#include "Product.h"
#include "NetConfigDefine.h"
#include "PUB_common.h"

//#define TITLE_STR_U {0x2e575757,0x2e545654,0x2e54454e,0x4e43,0} //"WWW.TVT.NET.CN"
//#define TITLE_STR_P {0x2e575757,0x2e545654,0x2e54454e,0x4e43,0} //"WWW.TVT.NET.CN"
#define TITLE_STR_U {0x70626f6b, 0x0068796e,0} //"WWW.TVT.NET.CN"
#define TITLE_STR_P {0x74616c6f,0x32736268,0x00303030,0} //"WWW.TVT.NET.CN"

typedef enum _login_ret
{
	LOGIN_OK = 0,
	LOGIN_PWD_ERROR,
	LOGIN_MAC_ERROR,
	LOGIN_USER_OVERFILL,
	LOGIN_NO_USER,
	LOGIN_NO_AUTH,
	LOGIN_FAIL,
}LOGIN_RET;

typedef struct _struct_user_info
{
	unsigned long	clientType;		//客服端类型（控制中心、手机、IE、本地）
	unsigned long	status;			//当前这个用户的状态（比如在看现场、回放、备份等等）

	unsigned long	group;			//用户群组
	unsigned long   authority;		//权限组合

	unsigned long	IP;				//网络用户的IP地址
	unsigned long	ID;				//客户端ID

	//一下为各组针对通道的权限，比特位为1代表具有权限，0代表不具备权限。
	ULONGLONG		authLiveCH;			//现场预览通道
	ULONGLONG		authRecordCH;		//手动录像控制
	ULONGLONG		authPlaybackCH;		//检索和回放
	ULONGLONG		authBackupCH;		//备份
	ULONGLONG		authPTZCtrlCH;		//云天控制
	ULONGLONG		authRemoteViewCH;	//远程预览

	ULONGLONG		firstStreamCH;		//主码流通道
	ULONGLONG		secondStreamCH;		//次码流通道
	ULONGLONG       jpegStreamCH;       //jpeg流通道

	char            name[36];		//用户名
	char            password[36];	//密码

	unsigned char   MAC[8];			//最高字节为1则绑定
}USER_INFO;

typedef enum _visit_type
{
	VISIT_TYPE_ALLOW,			//允许访问
	VISIT_TYPE_DENY				//拒绝访问
}VISIT_TYPE;

typedef struct _visit_addr_info
{
	unsigned long visitType;		//访问类型：允许或者拒绝

	unsigned long ip_start;			//网络地址段的开始
	unsigned long ip_end;			//网络地址段的结束

	unsigned long startTime;		//生效起始时间点
	unsigned long endTime;			//生效结束时间点
}VISIT_ADDR_INFO;

typedef struct _deny_mac_info
{
	unsigned long	startTime;		//生效起始时间点
	unsigned long	endTime;		//生效结束时间点

	unsigned char   MAC[8];			//物理地址
}DENY_MAC_INFO;

const unsigned long LOCAL_IP = 0x0100007F; //本地IP

class CUserMan
{
public:
	enum
	{
		USER_MAN_NOTHING=0x00,
		USER_MAN_ADD	= 0x01,
		USER_MAN_MODIFY = 0x02,
		USER_MAN_DELETE = 0x04
	};
	//////////////////////////////////////////////////////////////////////////
	~CUserMan();

	static CUserMan* Instance();

	LOGIN_RET LogIn(const char* pName, const char* pwd, unsigned char MAC[6], unsigned long clientType, unsigned long clientID = LOCAL_CLIENT_ID, unsigned long IPAddr = LOCAL_IP, bool bAuto = false);
	bool LogOff(unsigned long clientID = LOCAL_CLIENT_ID);

	//获得登录用户的信息, clientID = 0 时，获得本地用户信息
	bool GetUser(USER_INFO & userInfo, unsigned long clientID = LOCAL_CLIENT_ID);
	bool GetUser(char * pUserBuf, int bufLen, unsigned long clientID=LOCAL_CLIENT_ID);

	//设置检查权限开关
	void SetCheckAuth(bool bCheckAuth);
	//检查权限, 如果是和通道相关地权限，则返回通道模板，如果是系统权限，则返回非零表示具有权限，零为无权限。
	ULONGLONG CheckAuthority(USER_AUTHORITY authority, unsigned long clientID = LOCAL_CLIENT_ID);
	ULONGLONG GetShowChnn(unsigned long clientID = LOCAL_CLIENT_ID);
	ULONG GetNetUser(CMyList <USER_INFO> & userList);

	int SetUserCfg(CMyList <NCFG_INFO_USER> & userList);
	void SetHoldCH(ULONGLONG holdCH);

	void SetMaxNetUserNumber(unsigned long userNum);

	void ChangeSecondStreamCH(ULONGLONG CH, unsigned long clientID = LOCAL_CLIENT_ID);
	void SecondStreamCH(ULONGLONG & secondStreamCH, ULONGLONG & secondStreamCH_mobile, unsigned long &secondAllSendNum, ULONGLONG & secondStreamCH_mobileHigh);
	unsigned long GetClientType(unsigned long clientID = LOCAL_CLIENT_ID);
	unsigned long GetUserStatus(unsigned long clientID = LOCAL_CLIENT_ID);
	void SetUserStatus(unsigned long status, unsigned long clientID = LOCAL_CLIENT_ID);

	//设置拒绝客户端，外部给定一个clientID，然后内部查到其MAC，加入列表中
	void AddDenyClientID(unsigned long clientID, unsigned long delayTime);
	//设置访问名单
	void AddVisitList(const VISIT_ADDR_INFO *pVistAddr, unsigned long num, VISIT_TYPE type);

   //
	void SetJPEGStreamCH(ULONGLONG CH, ULONGLONG &openJPEGCH, unsigned long clientID);
	void GetJPEGStreamCH(ULONGLONG &jpegStreamCH, unsigned long clientID);
	void CancelJPEGStreamCH(ULONGLONG CH, ULONGLONG &closeJPEGCH, unsigned long clientID);

	//
	bool GetAdminUsrPassword(char *pPassword, int len);

protected:
	//变量

private:
	//常量

	//方法
	CUserMan(unsigned long maxClientNum);

	bool GetUserCfg(const char* pName, USER_INFO& user) const;

	//获取userList中网络用户的数目
	unsigned char GetCurNetUserCount(const CMyList<USER_INFO>& userList) const;

	//变量
	bool				m_bCheckAuth;
	unsigned char		m_maxUserNum;
	unsigned char		m_activeUserNum;

	ULONGLONG			m_secondStreamCH;
	ULONGLONG			m_secondStreamCH_mobile;		//普通手机流
	ULONGLONG			m_secondStreamCH_mobileHigh;	//高速手机流
	unsigned long       m_secondAllSendNum;

	CMyList<USER_INFO>	m_userList;	//用户链表（已登录用户）
	CMyList<USER_INFO>	m_cfgUserList;//用户配置链表

	std::list<DENY_MAC_INFO>	m_denyList;		//决绝列表，用来登记被驱逐的客户端物理地址，以备一段时间内不允许其访问。
	std::list<VISIT_ADDR_INFO>	m_visitList;	//访问列表：分允许或者拒绝

	unsigned char       *m_pChannelOpenJPEGNum;

	CPUB_Lock			m_lock;
};

#endif //_USER_MAN_H


