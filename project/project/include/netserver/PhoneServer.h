#ifndef _PHONE_SERVER_H_
#define _PHONE_SERVER_H_

#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "SWL_TypeDefine.h"
#include  "SWL_Public.h"
#include <errno.h>
extern int errno;

#include "PUB_common.h"


#define	PHONE_MSG_HEARTBEAT		    0x080c	//注册消息
#define	PHONE_MSG_REG_Info_LEN	    200

typedef struct _phone_msg_head_t
{
	unsigned short msg_type; //消息类型
	unsigned short msg_subtype; //消息子类型
	unsigned int msg_size; //消息长度	
	unsigned char ack_flag; //确认标志
	unsigned char reserved[3];
}PHONE_MSG_HEAD_T;


typedef struct __phone_msg_heartbeat_t
{
	char reg_info [PHONE_MSG_REG_Info_LEN];
}PHONE_MSG_HEARTBEAT_T;



class CPhoneSync
{
public:

	//
	CPhoneSync();
	~CPhoneSync();

	//
	static CPhoneSync* Instance();
	
	//
	int Init(unsigned long productType);
    void Quit();

	//
	void SetPhoneServerIp(unsigned long ip);
	void SetPhoneServerPort(unsigned long usServerPort);

	//
	void SetDVRHttpPort(unsigned short usHttpPort);	
	void SetDVRServerPort(unsigned short usServerPort);
	bool SetMac(char *pMac, int len);
	bool SetDVRUsrPassword(char *pPassword, int len);
	bool SetDVRVersion(char *pVersion, int len);

	//
	bool Start();
	void Stop();

protected:

private:

	//网络数据发送线程
	static PUB_THREAD_RESULT PUB_THREAD_CALL SendPhoneRegInfoThread(void *pParam);
	int SendPhoneRegInfoRun();

	//
	void SendContent();

	//
	int RecvMsg(SWL_socket_t sock, char *pBuf, int iLen, bool bBlock);


	//
	unsigned long    m_productType;

	//
	unsigned long    m_phoneServerIp;
	unsigned long   m_phoneServerPort;

	//
	unsigned short   m_dvrHttpPort;
	unsigned short   m_dvrServerPort;
	char             m_szMac[64];             
	char             m_szUsrPassword[64];
	char             m_szVersion[64];

	//
	bool                  m_bSendInfoOk;
	PHONE_MSG_HEARTBEAT_T m_sendRegInfo;
	PHONE_MSG_HEAD_T      m_msgHead;
	int                   m_msgLen;
	char         *m_pMsg;

	//
	PUB_thread_t			m_sendDataID;
	bool					m_bSendDataRun;

	bool                     m_bChangeMsg;


	//
	CPUB_Lock         m_lock;


};
#endif

