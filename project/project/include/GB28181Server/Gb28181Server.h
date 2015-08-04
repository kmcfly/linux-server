#pragma once

#include "MessageMan.h"
#include "Gb28181Config.h"

#include <map>
#include <utility>
#include <string>

#define GB_SUCCESS           0x00000000
#define GB_FAILURE           0xFFFFFFFF

typedef enum _manscdp_type
{
	UnknownType = 0,		//������
	CONTROL,				//��ʾһ�����ƵĶ���
	QUERY,					//��ʾһ����ѯ�Ķ���
	NOTIFY,					//��ʾһ��֪ͨ�Ķ���
	RESPONSE,				//��ʾһ����������Ӧ��
}MANSCDP_TYPE;


typedef enum _body_type
{
	Unknown = 0,     //������
	SDP,			 //SDP��Ϣ��
	MANSCDP_XML,     //MANSCDP+XML��Ϣ��
	MANSRTSP,		 //MANSRTSP ��Ϣ��
	DVR_CMD_REPLY_CFG_SUCC,//�������óɹ�
	DVR_CMD_REPLY_CFG_DATA,//��ȡ����
	DVR_CMD_REPLY_CFG_FAIL,//��������ʧ��
	DVR_CMD_REPLY_CHANAGE_TIME_SUCC,//����ϵͳʱ��ɹ�
	DVR_CMD_REPLY_CHANAGE_TIME_FAIL,//����ϵͳʱ��ʧ��
	DVR_CMD_REPLY_PTZ_SEND_CMD_SUC,  //����̨��������ɹ�
	DVR_CMD_REPLY_DATA_INFO,		//�ظ�������������ݣ���������
}BODY_TYPE;

typedef struct _sip_message_body
{
	BODY_TYPE		body_type;
	MANSCDP_TYPE	manscdp_type;
	std::string		strMsgBody;
	unsigned int	uiMsgLen;
}SIP_MESSAGE_BODY;

class CSipServer;
class CMsgParser;
class CGb28181Server  
{
public:
	static CGb28181Server* Instance();
	
	~CGb28181Server();
	      
	//��ʼ��CGb28181Server������ÿ��ʹ��CGb28181Server����ǰҪ���ô˺���
	int	 Initial(CMessageMan *pMessageMan);
	
	//�������ã�������sipServer
	void SetSipServerConfig(const GB28181_SIP_SERVER & Gb28181SipServer);

	//��������
	void SetGB28181Cfg(const GB28181_SIP_SERVER & Gb28181SipServer);

	void SetGB28181Info(const Gb28181_INIT_INFO & Gb28181Info);

	void SetGb28181ChannelIDConfig(const GB28181_CHANNEL_ID & Gb28181ChannelID);

	void SetGb28181AlarmIDConfig(const GB28181_ALARM_ID & Gb28181AlarmID);

	void SetGb28181CatalogInfo(const GB28181_CHANNEL_ID & Gb28181ChannelID,const GB28181_ALARM_ID & Gb28181AlarmID);

	void SetGb28181DeviceStatusInfo(const GB28181_CHANNEL_ID & Gb28181ChannelID,const GB28181_ALARM_ID & Gb28181AlarmID);


	//ж��CGb28181Server���󣬵�����ʹ��CGb28181Server�����ʱ����ô˺���
	void Quit();
	
	//CGb28181Server������ֻ�е�CGb28181Server�����ˣ������Ĳ���������Ч
	int	 Start();
	
	//CGb28181Serverֹͣ
	int	 Stop();

	//��ȡ�ͻ���ID
	unsigned long  GetClientID();

	//���ڲ�������Ϣ
	void PutMsgToServer(const MESSAGE_DATA & msgData);

	//���ڲ���ȡ��Ϣ
	bool GetMsgFromServer(MESSAGE_DATA & msgData);

	//���һ����Ϣ��������Ϣ������
	bool AddMessageToRevList(const SIP_MESSAGE_BODY &sipMsgBody);

	//�ӷ��͵���Ϣ�����л�ȡһ����Ϣ
	bool GetMessageFromSendList(SIP_MESSAGE_BODY &sipMsgBody);

	//�ӽ�����Ϣ�����л�ȡһ����Ϣ
	bool GetMessageFromRevList(SIP_MESSAGE_BODY &sipMsgBody);
	
	//���һ����Ϣ���������ͳ�ȥsip������
	bool AddMessageToSendList(const SIP_MESSAGE_BODY &sipMsgBody);
	
	//�ͷ���Ϣ�ڴ溯��
	inline void ReleaseMsgBuff(MESSAGE_DATA &msgData) {this->m_pMessageMan->ReleaseMsgBuff(msgData);}

	//ע��ɹ���ͬ����ǰϵͳʱ��
	int SetDvrTime(struct tm &tm_time); 


	//bool TrySendListLock(){ return  m_listSendMsgLock.TryLock();}
	void LockSendList() {m_listSendMsgLock.Lock();}
	void UnLockSendList() {m_listSendMsgLock.UnLock();}


	void LockRevList() {m_listRevMsgLock.Lock();}
	void UnLockRevList() {m_listRevMsgLock.UnLock();}
	
private:

	//���һ����Ϣ����Ϣ������
	bool AddMessageToList(const SIP_MESSAGE_BODY &sipMsgBody,std::list<SIP_MESSAGE_BODY> & listMessage);

	//����Ϣ�����л�ȡһ����Ϣ
	bool GetMessageFromList(SIP_MESSAGE_BODY &sipMsgBody,std::list<SIP_MESSAGE_BODY> & listMessage);

	//ʹ�õ����������ù��캯��˽��
	CGb28181Server();
	
	//��ֹʹ��Ĭ�ϵĿ������캯����=�����
	CGb28181Server(const CGb28181Server&);
	
	CGb28181Server& operator=(const CGb28181Server&);

private:

	PUB_lock_t					m_Gb28181Lock;  
	bool						m_bGb28181Quit;		//�˳���־
	bool						m_bIsStartSipServer;//�Ƿ�Ҫ����sipserver
	unsigned long				m_iClientID;		//�ͻ���ID
	CPUB_Lock					m_listRevMsgLock;			//����Sip��Ϣ������
	CPUB_Lock					m_listSendMsgLock;			//������Ϣ������
	
	std::list<SIP_MESSAGE_BODY>	m_listRevMessage;			//������Ž��յ���Sip��Ϣ
	std::list<SIP_MESSAGE_BODY>	m_listSendMessage;			//�������׼�����͵�Sip��Ϣ

	CSipServer					*m_pSipServer;			//��������SipServer
	CMsgParser					*m_pParser;				//��������Parser
	CMessageMan					*m_pMessageMan;
	GB28181_CONFIG				*m_pGB28181_CONFIG;		//����
private:
	GB28181_CHANNEL_ID          m_Gb28181ChannelID;   //��ʱ����ͨ�����õĽṹ
	GB28181_ALARM_ID			m_Gb28181AlarmID;     //��ʱ�������õĽṹ
};