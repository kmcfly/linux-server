#pragma once

//#include "SWL_ListenProcEx.h"
//#include "SWL_MultiNetComm.h"
//#include "AVDataSendMan.h"
#include <eXosip2/eXosip.h>
#include <osip2/osip_mt.h>
#include <eXosip2/eX_setup.h>
#include "eXosip2/eX_register.h"

#include <map>
#include <utility>
#include <string>

#include"ServerRTPMan.h"
#include "ServerRTPSession.h"

#include "Gb28181Server.h"
#include"Gb28181sdp.h"

#include "Gb28181Config.h"

#define GB_SUCCESS           0x00000000
#define GB_FAILURE           0xFFFFFFFF

#define ERR_INVALID_ARG		-2
#define NO_RESP				-3

#define MAX_ID_LEN		32			// ����ID����
#define MAX_CODE_LEN	36			// �����򳤶�


#define MAX_PARA_STR_LENGTH		2048
#define MAX_RECV_MESSAGE_LENGTH  4096


typedef enum _register_state
{
	GB_REGISTER_IDLE= 0,
	GB_REGISTER_FRIST,    //frist  register
	GB_REGISTER_SECOND,  //second   register
	GB_REGISTER_SUCCESS, //register  success
	GB_REGISTER_LOGINOUT,  //ע��
	GB_REFISTER_COUNT,
}REGISTER_STATE;

//��RTPServer�ص���֪ͨ�طŻ������ؽ���
void FileStreamEnd(int SessionID);

class CGb28181Server;
class CSipServer
{
public:
	static CSipServer* Instance();

	~CSipServer();

	//��ʼ��CSipServer������ÿ��ʹ��CSipServer����ǰҪ���ô˺���
	int	 Initial();

	//Osip��ʼ��
	int OsipInit();
	
	//Osip����ʼ��
	int  OsipUnInit();   

	void SetParam(); //����Ĭ�ϲ���


	//ж��CSipServer���󣬵�����ʹ��CSipServer�����ʱ����ô˺���
	void Quit();

	//CSipServer������ֻ�е�CSipServer�����ˣ������Ĳ���������Ч
	int	 Start();

	//CSipServerֹͣ
	int	 Stop();

	//ע��
	int DvrLoginOut(bool bNeedLoginOut=true);

	//startʱ�����ö�sipserver��״̬��ʼ��
	void InitSipServerState();

	//������Ϣ
	int	SendMassage(char *xmlmsg,size_t xmlmsg_length);

	//��������
	void HeartKeepLive();

	//��ȡע���״̬
	REGISTER_STATE GetRegisterState();

	//�ط��Զ�����ʱ����
	void ReleaseStream(int SessionID);

	//ͨ��channel ID��ȡsession ID
	int GetSessionID(unsigned long ulChannelID);
	
	//�����������
	bool RemoveAll();
private:

	//ʹ�õ����������ù��캯��˽��
	CSipServer();

	//invate ����
	int	InviteProc(eXosip_event_t *evt);

	//�жϸ�ͨ������Ƶ���Ƿ������
	bool StreamIsRequested(unsigned long ulChannelID);

	int AckProc(eXosip_event_t *evt);

	//�жϲ������ͣ�������Ƶ��
	int Gb28181StartStream(CALLINFO *pCallInfo);

	//��ʼ������
	int StartStream(CALLINFO *pCallInfo);

	 //�����ȡ�ط���Ƶ��
	void  RequestDataRead(CALLINFO *pCallInfo);
	
	//ֹͣ����ط���
	void StopRequestDataRead(CALLINFO *pCallInfo);

    // �ط���������Ϣ����
	int InfoMsgProc(eXosip_event_t *evt);

	//����MANsRtsp��Ϣ�����ظ�
	int  MansRtspParseAnswer(eXosip_event_t *evt, osip_message_t * info);

	//�ر���Ƶ��
	int StreamByeProc(eXosip_event_t *evt);

		//sdp������sdp����
	int SdpParseAnswer (eXosip_event_t *evt, char *buf);

	//ע���߳�
	void SipServerRegister();

	//��Ϣ����
	void RecvMsg();

	/**/
	void SendSipMsg();

	//��ȡxml����
	int	GetMessageBody(eXosip_event_t *evt,char *xmlmsg,size_t *xmlmsg_length);

	//������������һ��ע��
	int	RegisterFirst(int &regid,bool IsLoginOut=false); 

    //�ظ�ע��ʹ��
	int RegisterAgain(int regid);


	//������֤��Ϣ,�����������ڶ���ע��
	int RegisterSecond(int regid,bool IsLoginOut=false);

	//ע��ɹ���ͬ����ǰϵͳʱ��
	int ParseServerTime(osip_message_t *message,struct tm &tm_time);

	//����request����
	int MsgSendRequest(char *msg_method,char *to, char *from,char *content,char *conten_type);

	//��Ӧ���� 200ok
	int MsgSendAnswer(eXosip_event_t *evt,int status);

	//����ע���״̬
	void SetRegisterState(REGISTER_STATE  RegisterState);  

	//���������SDP��Ϣ����invate����
	bool AddInvateMsgToList(CALLINFO *pCallInfo);


	//��invate�����ж���SDP��Ϣ
	bool GetInvateMsgFromList(CALLINFO* &pCallInfo);

	int PaseRemoteSdp(sdp_message_t *remote_sdp,CALLINFO*pCallInfo);

	/*����sdp��Ϣ*/
	int formSdpInfo(char *buf,CALLINFO *pCallInfo);


	void LockStreamList() {m_lStreamLock.Lock();}
	void UnLockStreamList() {m_lStreamLock.UnLock();}

	static PUB_THREAD_RESULT PUB_THREAD_CALL RegisterProc(void *pParam);

	//��Ϣ�����߳�
	static PUB_THREAD_RESULT PUB_THREAD_CALL RecvMsgProc(void *pParam);
	
	//��Ϣ�����߳�
	static PUB_THREAD_RESULT PUB_THREAD_CALL SendMsgProc(void *pParam);

private:

	GB28181_CONFIG				*m_pGb28181_Config;					//�����ļ�

	REGISTER_STATE				RegisterState;						//ע���״̬
	char						m_strSipserverIP[MAX_ID_LEN];		//sip������IP��ַ
	unsigned short				m_iSipserverPort;					//sip�������Ự�˿�
	unsigned short				m_iSipDvrPort;						//dvr sip�Ự�˿�

	bool                        m_SipserverQuit;                   //sipserver �˳���־
	bool                        m_bRegisterRun;                      //��Ϣ�߳��Ƿ���������
	bool                        m_bRcvMsgRun;                      //��Ϣ�߳��Ƿ���������
	bool                        m_bSndMsgRun;                      //��Ϣ�����߳��Ƿ���������
	bool                        m_bRegister;                        //�Ƿ����ظ�ע��
	
	bool                        m_bLoginOut;                       //ע��

	char						m_strSipServerId[MAX_ID_LEN];		//sip������ID
	char						m_strDvrip[MAX_ID_LEN];				//DVR ip
	char						m_strDvrId[MAX_ID_LEN];						//�豸Id
	char						m_strUserID[MAX_ID_LEN];			//�û���
	char						m_strRegisterPasswd[MAX_ID_LEN];	//ע������
	unsigned int				m_uiExpires;						//����ʱ��
	unsigned int                m_uiSessionID;     //����Ƶ���ĻỰID
	
	PUB_thread_t				m_registerID;						//ע������߳�
	PUB_thread_t				m_recvmsgID;						//��Ϣ�����߳�
	PUB_thread_t				m_sendmsgID;						//��Ϣ�����߳�
	CGb28181Server				*m_pGb28181Server;					//����Gb28181Server�ӿ�          
	CServerRTPMan				*m_pRtpServer;						//��������RtpServer
	std::list<CALLINFO*>    m_listCallInfo;             //invate sdp ����
	
	CPUB_Lock					m_lStreamLock;			//stream list lock
};