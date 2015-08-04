#pragma once

#include <map>
#include <utility>
#include <string>

#include "MarkupSTL.h"
#include "Protocol.h"
#include"MessageMan.h"

#include "Gb28181Server.h"

#include "Gb28181Config.h"
#include "Gb28181Ptz.h"
#include "Gb28181Mansrtsp.h"
#define  MAX_ALARM_NUM 16
#define  MIN_SEARCH_TIME   1262275200  //2010-01-10 00:00:00
typedef enum  _search_type
{
	SEARCH_TYPE_TIME = 0, //��ʱ�����¼���ļ�
	SEARCH_TYPE_ALARM ,   //����������¼���ļ�
	SEARCH_TYPE_MANUAL,   //���ֶ�¼������ļ�
	SEARCH_TYPE_ALL,      //��������¼���ļ�
}SEARCH_TYPE;

typedef struct _cmd_data
{
	XML_PROTOCOL_TYPE	cmdtype;	//���������
	unsigned long   channelId;      //ͨ��ID
	unsigned long 	SN;         //�������к�
	char  szDeviceID[32];				//�豸ID
	void *pdata;                   //����
	int datalen;                   //���ݳ���
}CMD_DATA;


class CGb28181Server;
class CMsgParser
{
public:
	static CMsgParser* Instance();

	~CMsgParser();

	int	Initial();

	//ж��CGb28181Server���󣬵�����ʹ��CGb28181Server�����ʱ����ô˺���
	void Quit();

	//CGb28181Server������ֻ�е�CGb28181Server�����ˣ������Ĳ���������Ч
	int	 Start();

	//CGb28181Serverֹͣ
	int	 Stop();

	//�ϱ��ļ�����,
	void NotifyFileEnd(char *m_cDeviceID);

private:
	//ʹ�õ����������ù��캯��˽��
	CMsgParser();

	//��ֹʹ��Ĭ�ϵĿ������캯����=�����
	CMsgParser(const CMsgParser&);

	CMsgParser& operator=(const CMsgParser&);

	void LockCmdList() {m_lCmdListLock.Lock();}
	void UnLockCmdList() {m_lCmdListLock.UnLock();}

	//��sip��Ϣת�����ڲ������ļ�
	int ParseMsg(const SIP_MESSAGE_BODY &sipMsgBody,MESSAGE_DATA &msgData);

	/*����xml��Ϣ*/
	int	ParseXMLMsg(const string & strMsgBody,MESSAGE_DATA & msgData);

	//���ڲ������ļ�ת����sip��Ϣ
	int ReverseParseMsg(const MESSAGE_DATA &msgData,SIP_MESSAGE_BODY &sipMsgBody);

	//������Ϣ�壬��������Ϣ
	int	ParseMsg();

	//��������Ϣ�壬������sip��Ϣ
	int	ReverseParseMsg();

	//�澯�ϱ�
	int AlarmSend();

	//����
	bool NotifyKeepAlive(); 

	//�����Ƿ�����
	bool GetHeartBreak();  

	//����Ϣѹ����У��Ȼ�messageMan��Ӧ����xml
	bool AddParseMsgToList(const SIP_MESSAGE_BODY &sipMsgBody);  

	//��ȡ���У�����xml
	bool GetParseMsgFromList(SIP_MESSAGE_BODY &sipMsgBody);

    /*����Ҫ������Ͳ���ѹ�����*/	
	bool AddCmdDataToList(const CMD_DATA &CmdData);

	/*������ʱȡ����Ҫ������Ͳ���*/
	bool GetCmdDataFromList(CMD_DATA &CmdData);

	/*�����������*/
	int ReverParseCmd(CMD_DATA &CmdData);

	/*�澯��ѯ*/
	bool  QueryAlarm();

	//�澯��Ϣ�ϱ�
	int NotifyAlarm(CHNN_STATUS_TYPE type ,ULONGLONG & value);
	
	//��䱨���ľ�����Ϣ
	void SetAlarmInfoText(CHNN_STATUS_TYPE type,CNotifyAlarmProtocol *pNotifyAlarmProtocol);

	//�ͷ���������
	void ReleaseSetCfg();

	 /*�豸����*/
	int	ControlDevice(CControlProtocol* pControlProtocol ,MESSAGE_DATA & msgData,SIP_MESSAGE_BODY &sipMsgBodyResp);

	/*��ʱ���ַ���ת��Ϊ�޷���������*/
	unsigned long ConvertTimeToULong(char *time);

	/*���޷���������ʱ��ת��Ϊ�ַ���%d-%d-%dT%d:%d:%d ��ʽ*/
	int ConvertTimeToStr(unsigned  long ulTime,char *buf);

	/*¼���ļ�����*/
	int ProcReplyDataInfo(const MESSAGE_DATA &msgData,SIP_MESSAGE_BODY &sipMsgBody);

	//��ȡ¼���ļ��ļ�����ʽ
	int GetSearchType(char *m_szType);

	//����¼���ļ�����������
	void SetSearchType(unsigned long  lSearchType,char *m_szType);

	//���¼���ļ���������
	void SetSearchDataInfo(int type,NET_DATA_SEARCH *pdataInfo);

	//�����MessageMan��ȡ������
	int ProcReplyCfgData(const MESSAGE_DATA &msgData);

	//����������Ϣ
	int ProcRespMsg(const MESSAGE_DATA &msgData,SIP_MESSAGE_BODY &sipRespMsg);

	//����sdp��xml��Ϣ�岢ת���ڲ����ò�������Ϣ�߳�
	static PUB_THREAD_RESULT PUB_THREAD_CALL ParseMsgProc(void *pParam);

	//��������Ϣ������װxml��sdpЭ�鷢�ͳ�ȥ
	static PUB_THREAD_RESULT PUB_THREAD_CALL ReverseParseMsgProc(void *pParam);

	//�澯��ѯ�ϱ��߳�
	static PUB_THREAD_RESULT PUB_THREAD_CALL AlarmSendProc(void *pParam);

	//�����������ظ���response��Ϣ
	int XmlFormControlResp(CBasicProtocol* pBasicProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);

	//�����ȡ�豸״̬��xml
	int XmlFormDeviceStatusResp(CBasicProtocol* pBasicProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);
	
	//�����ȡ�豸��Ϣ��xml
	int XmlFormDeviceInfoResp(CBasicProtocol* pBasicProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);

	//�����ȡĿ¼��xml
	int XmlFormCatalogResp(CBasicProtocol* pBasicProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);

	//¼��Ŀ¼��ѯ
	int XmlFormRecordInfoResp(CResponseRecordInfoProtocol & m_ResponseRecordInfoProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);

	//����alarm�¼���Ӧ
	int XmlFormAlarmResp(CBasicProtocol* pBasicProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);
	
private:

	std::list<SIP_MESSAGE_BODY>	m_listParseMessage;			//�������׼�����͵���Ϣ
	std::list<CMD_DATA>         m_listPackCmd;              //��������Ҫ�õ�����

	CPUB_Lock					m_lCmdListLock;			//Command  list lock

	CGb28181Server				*m_pGb28181Server;			//����Gb28181Server�ӿ�
	CMessageMan					*m_pMessageMan;             //����MessageMan�ӿ�
	GB28181_CONFIG				*m_pGb28181_Config;					//�����ļ�
	
	bool                        m_bHeartBreak;               //�����Ƿ�����
	bool						m_bParseMsgRun;            
	bool						m_bReverseParseMsgRun;
	bool                        m_bAlarmSendRun;
	bool                        m_bAlarmGuard[32];                   //�Ƿ�������
	bool                        m_bRecord;                   //�Ƿ�������

	PUB_thread_t				m_ParseMsgID;				//��Ϣ�����߳�
	PUB_thread_t				m_ReverseParsemsgID;		//��Ϣ�����߳�
	PUB_thread_t				m_AlarmSendID;		//�澯�ϱ��߳�
};
