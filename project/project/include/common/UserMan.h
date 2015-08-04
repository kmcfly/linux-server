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
	unsigned long	clientType;		//�ͷ������ͣ��������ġ��ֻ���IE�����أ�
	unsigned long	status;			//��ǰ����û���״̬�������ڿ��ֳ����طš����ݵȵȣ�

	unsigned long	group;			//�û�Ⱥ��
	unsigned long   authority;		//Ȩ�����

	unsigned long	IP;				//�����û���IP��ַ
	unsigned long	ID;				//�ͻ���ID

	//һ��Ϊ�������ͨ����Ȩ�ޣ�����λΪ1�������Ȩ�ޣ�0�����߱�Ȩ�ޡ�
	ULONGLONG		authLiveCH;			//�ֳ�Ԥ��ͨ��
	ULONGLONG		authRecordCH;		//�ֶ�¼�����
	ULONGLONG		authPlaybackCH;		//�����ͻط�
	ULONGLONG		authBackupCH;		//����
	ULONGLONG		authPTZCtrlCH;		//�������
	ULONGLONG		authRemoteViewCH;	//Զ��Ԥ��

	ULONGLONG		firstStreamCH;		//������ͨ��
	ULONGLONG		secondStreamCH;		//������ͨ��
	ULONGLONG       jpegStreamCH;       //jpeg��ͨ��

	char            name[36];		//�û���
	char            password[36];	//����

	unsigned char   MAC[8];			//����ֽ�Ϊ1���
}USER_INFO;

typedef enum _visit_type
{
	VISIT_TYPE_ALLOW,			//�������
	VISIT_TYPE_DENY				//�ܾ�����
}VISIT_TYPE;

typedef struct _visit_addr_info
{
	unsigned long visitType;		//�������ͣ�������߾ܾ�

	unsigned long ip_start;			//�����ַ�εĿ�ʼ
	unsigned long ip_end;			//�����ַ�εĽ���

	unsigned long startTime;		//��Ч��ʼʱ���
	unsigned long endTime;			//��Ч����ʱ���
}VISIT_ADDR_INFO;

typedef struct _deny_mac_info
{
	unsigned long	startTime;		//��Ч��ʼʱ���
	unsigned long	endTime;		//��Ч����ʱ���

	unsigned char   MAC[8];			//�����ַ
}DENY_MAC_INFO;

const unsigned long LOCAL_IP = 0x0100007F; //����IP

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

	//��õ�¼�û�����Ϣ, clientID = 0 ʱ����ñ����û���Ϣ
	bool GetUser(USER_INFO & userInfo, unsigned long clientID = LOCAL_CLIENT_ID);
	bool GetUser(char * pUserBuf, int bufLen, unsigned long clientID=LOCAL_CLIENT_ID);

	//���ü��Ȩ�޿���
	void SetCheckAuth(bool bCheckAuth);
	//���Ȩ��, ����Ǻ�ͨ����ص�Ȩ�ޣ��򷵻�ͨ��ģ�壬�����ϵͳȨ�ޣ��򷵻ط����ʾ����Ȩ�ޣ���Ϊ��Ȩ�ޡ�
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

	//���þܾ��ͻ��ˣ��ⲿ����һ��clientID��Ȼ���ڲ��鵽��MAC�������б���
	void AddDenyClientID(unsigned long clientID, unsigned long delayTime);
	//���÷�������
	void AddVisitList(const VISIT_ADDR_INFO *pVistAddr, unsigned long num, VISIT_TYPE type);

   //
	void SetJPEGStreamCH(ULONGLONG CH, ULONGLONG &openJPEGCH, unsigned long clientID);
	void GetJPEGStreamCH(ULONGLONG &jpegStreamCH, unsigned long clientID);
	void CancelJPEGStreamCH(ULONGLONG CH, ULONGLONG &closeJPEGCH, unsigned long clientID);

	//
	bool GetAdminUsrPassword(char *pPassword, int len);

protected:
	//����

private:
	//����

	//����
	CUserMan(unsigned long maxClientNum);

	bool GetUserCfg(const char* pName, USER_INFO& user) const;

	//��ȡuserList�������û�����Ŀ
	unsigned char GetCurNetUserCount(const CMyList<USER_INFO>& userList) const;

	//����
	bool				m_bCheckAuth;
	unsigned char		m_maxUserNum;
	unsigned char		m_activeUserNum;

	ULONGLONG			m_secondStreamCH;
	ULONGLONG			m_secondStreamCH_mobile;		//��ͨ�ֻ���
	ULONGLONG			m_secondStreamCH_mobileHigh;	//�����ֻ���
	unsigned long       m_secondAllSendNum;

	CMyList<USER_INFO>	m_userList;	//�û������ѵ�¼�û���
	CMyList<USER_INFO>	m_cfgUserList;//�û���������

	std::list<DENY_MAC_INFO>	m_denyList;		//�����б������ǼǱ�����Ŀͻ��������ַ���Ա�һ��ʱ���ڲ���������ʡ�
	std::list<VISIT_ADDR_INFO>	m_visitList;	//�����б���������߾ܾ�

	unsigned char       *m_pChannelOpenJPEGNum;

	CPUB_Lock			m_lock;
};

#endif //_USER_MAN_H


