// NatUdt.h: interface for the CNatUdt class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_UDT_H_
#define _NAT_UDT_H_

#include "NatCommon.h"
#include "NatRunner.h"
#include "NatHeapDataManager.h"
#include "NatUdtTransport.h"
#include <list>
#include <map>

#define		RTT_RESOLUTION_1

static const uint16 NAT_UDT_VERSION_0_1 = NatMakeUint16(0, 1); // UDT version "0.1"
static const uint16 NAT_UDT_VERSION_1_0 = NatMakeUint16(1, 0); // UDT version "0.1"
static const uint16 NAT_UDT_VERSION = NAT_UDT_VERSION_1_0;

const byte	UDP_DATA_TYPE_SYN		= 0x0;	//��������
const byte	UDP_DATA_TYPE_RELIABLE	= 0x1;	//�ɿ�����
const byte	UDP_DATA_TYPE_ACK		= 0x2;	//��ȷ�����ݣ���͸�����������������ȷ��
const byte	UDP_DATA_TYPE_RST		= 0x3;	//�Ͽ�


const byte SWL_UDP_DATA_STATUS_NOSEND = 0;		//û�з��͹�
const byte SWL_UDP_DATA_STATUS_SENDED = 0x01;	//���͹�������û�б�ȷ�Ϲ������λΪ0��û�з��͹�
const byte SWL_UDP_DATA_STATUS_TIMEOUT = 0x02;	//�Ѿ���ʱ
const byte SWL_UDP_DATA_STATUS_BEYOND = 0x04;	//�Է��Ѿ��յ��˳��������ݱ�ʶ�İ�

const byte SWL_UDP_SENDMODE_SLOW_STARTUP = 0;		//������
const byte SWL_UDP_SENDMODE_CROWDED_AVOID = 1;		//ӵ������
const byte SWL_UDP_SENDMODE_TIMEOUT_RESEND = 2;	//��ʱ�ش�
const byte SWL_UDP_SENDMODE_FAST_RESEND = 4;		//�����ش��ָ�
//const byte SWL_UDP_SENDMODE_RESET		= 8;	//�ش���������

const byte SWL_UDP_ACK_STATUS_NONE = 0;		//û��ȷ�Ͽɷ�
const byte SWL_UDP_ACK_STATUS_DELAY = 1;		//��ʱ���ٷ��������Ӵ�����
const byte SWL_UDP_ACK_STATUS_NOW = 2;			//���������ϵ������ݵ��ˣ�����Ҫ��������ȷ��
const byte SWL_UDP_ACK_STATUS_DISORDER = 3;	//�յ��������Ҫ��������ȷ��


#define UDP_DISORDER_SEND_TIMES	3		//�����ȷ����෢����	
#define UDP_TIMEOUT				60000	//����

#define UDP_CONNECT_INTERVAL	1000	//����Ϊ��λ����ü��û�յ����ӻظ������ط�һ������ظ�
#define SEND_ACK_INTERVAL		100		//ÿ���೤ʱ�䷢��һ��ȷ�ϣ������ȷ����Ҫ����
#define MAX_RESEND_TIME			5000	//�����ش��������Ϊ����Ϊ��λ
#define MIN_RESEND_TIME			200	    //��С���ش��������Ϊ����Ϊ��λ

#define MIN_RTT_TIME			100		
#define MIN_SSTHRESH			20
#define MIN_CWND_WIN			15

static const int UDT_ESTABLISH_TIMEOUT			= 60 * 1000;	// UDT�������ӵĳ�ʱʱ�䣬��λ��ms
static const int UDT_CONNECTION_TIMEOUT			= 60 * 1000;	// UDT�������ӣ������ӣ��ĳ�ʱʱ�䣬��λ��ms
static const int UDT_KEEPING_HOLE_INTERVAL		= 3 * 1000;		// UDT������ά����·�����ϵġ���������ӳ�䣩����Ҫ���͵�TTL���ݰ��ļ��ʱ�䣬��λ��ms
static const int UDT_KEEPING_HOLE_TTL			= 5;			// UDT������ά����·�����ϵġ���������ӳ�䣩����Ҫ���Ͱ���TTL����֧�ּ�������·������
static const int UDT_HEARTBEAT_INTERVAL			= 20 * 1000;	// UDT�������ݰ����ͼ������Ҫ��Ϊ�˱���UDT����

/**
 * UDTͨ��Э�����
 */
static const byte NAT_UDT_CATEGORY_INVALID		= 0x00;
static const byte NAT_UDT_CATEGORY_P2P			= 0x01; /**< P2Pͨ�ŷ��� */
static const byte NAT_UDT_CATEGORY_P2NS			= 0x02; /**< Peer-to-NatServerͨ�ŷ��� */
/**
 * UDT ͨ��Э���ǣ�һ�����������syn��
 */
static const uint16 NAT_UDT_PROTOCOL_TAG		= 0xfefe;

#pragma pack(push, 1)

typedef NAT_VERSION_2 UDT_P2P_VERSION_TYPE;

typedef struct _udp_wrapper_head
{
	byte	category;			// ���࣬�������ֲ�ͬ��ͨ������
	byte	cmdId : 4;			// �����ʶ
	byte	options1 : 4;		// ѡ��1�������ֶ�
	byte	optionEnable:1;		// ѡ���Ƿ���ã�������Options1������Options2������Reserved1����SYN���ġ�Version���ֶ�
	byte	options2:7;			// Options2�ı����ֶ�
	byte	reserved;			// �����ֶ�
	uint32	connectionID;		// Connection ID,����ͬIPͬport������
	uint32	sendIndex;			// ���Ͱ���ʶ��ÿ����һ�����ݾ�+1�������ش���ȷ�ϣ���������
	uint32	recvIndex;			// �յ��������İ���ʶ
	uint32	seqNum;				// ���͵����к�,ֻ�з��İ������������ʱ��SeqNum�ż�1
	uint32	ackNum;				// ���յ����к�
}UDP_WRAPPER_HEAD;

#define MAX_WRAPPER_DATA_BUFFSIZE  1500
//(296 - 20 - 8) * 2 + 20 +8,Ϊ�˷�ֹ��·��������DOS�������Ѱ��������Ϊ�������Ĵ�С
#define MTU_LEN						1300 //IPV6Ҫ����·����С����Ϊ1280
#define MAX_WRAPPER_DATA_LEN	(MTU_LEN - 20 - 8) //(MTU_LEN - 20 - 8) 20 IPͷ����С���� 8 UDPͷ

#define WRAPPER_VALIDATA_LEN	(MAX_WRAPPER_DATA_LEN - sizeof(UDP_WRAPPER_HEAD))//��������ݵ���Ч������󳤶�

#define MAX_SEND_RELIABLE_BUFFSIZE					1024*1024

/*modified by cc : MAX_SEND_BUFF_PACKET_COUNTֻ������MAX_SEND_RELIABLE_BUFFSIZE��WRAPPER_VALIDATA_LEN*/
const uint32 MAX_SEND_BUFF_PACKET_COUNT = MAX_SEND_RELIABLE_BUFFSIZE / WRAPPER_VALIDATA_LEN + 1;

typedef struct _udp_ack
{
	// 	uint16	nIndex;			//����ʶ
	// 	uint16	nRecvTimes;		//�չ�������
	uint32	ackSeq;		//ȷ�ϵڼ��Ű�
}UDP_ACK;

const uint32 MAX_ACK_COUNT = WRAPPER_VALIDATA_LEN / sizeof(UDP_ACK);

typedef struct _udp_syn_data_
{
	uint16	protocolTag;
	uint16	version;
}UDP_SYN_DATA;

typedef struct _udp_wrapper_data
{
	UDP_WRAPPER_HEAD head;
	union
	{
		char			data[WRAPPER_VALIDATA_LEN];
		UDP_ACK			acks[MAX_ACK_COUNT];
		UDP_SYN_DATA	syn;
	}wrapper_dat;
}UDP_WRAPPER_DATA;

typedef struct _udp_syn_packet_
{
	UDP_WRAPPER_HEAD	head;
	UDP_SYN_DATA		data;
}UDP_SYN_PACKET;

typedef struct _udp_nodata_packet_
{
	UDP_WRAPPER_HEAD	head;
}UDP_NODATA_PACKET;

#pragma pack(pop)

typedef struct _udp_send_packet_data
{
	int32			iLen;							//���ΪWRAPPER_VALIDATA_LEN
	uint32			lastSendTick;					//��һ�η��͵�ʱ��
	uint32			lastSendIndex;					//��һ�η���ʱ�������ݵı�ʶ
	byte			dataStatus;						//�Է��յ��Ѿ�ȷ�ϣ��������ȷ�ϱ�����ȷ�ϵģ�����˳��ȷ�ϵģ�������ʱûɾ��

	UDP_WRAPPER_DATA		wrapperData;
}UDP_SEND_PACKET_DATA;


// typedef struct _udp_disorder_seq
// {
// 	uint32 disorderSeq;								//����������
// 	uint32 sendTimes;								//���͵Ĵ���
// }UDP_DISORDER_SEQ;

typedef struct _udp_reliable_send_data_info
{
	uint32								nextSeq;				//��һ�����Ž�listӦ�õ���ţ���ʹ�ã��ټ�һ
	int32								remainedBuffSize;		//���ͻ��滹��ʹ�õ����ݳ���
	int32								remainHeapPacketNum;	//���ͻ��滹��ʹ�õĶѿռ�İ�����
	std::list<UDP_SEND_PACKET_DATA*>	listReliableSendData;	//�����͵Ŀɿ�����
	std::list<UDP_SEND_PACKET_DATA*>	listUnackedData;		//���ͺ�δacked������
}R_SEND_INFO;


typedef struct _nat_udt_recv_data_
{
    int32				iPacketLen;  /**< UDT�������ݰ��ֽڳ��� */
    UDP_WRAPPER_DATA	pPacket;    /**< UDT�������ݰ� */

    /**
     * UDT�������ݰ��������������ݴ�С�������С��
     */
    inline int32 PacketSize()
    {
        return iPacketLen - sizeof(pPacket.head);
    }

}NAT_UDT_RECV_DATA;

typedef std::map<uint32, NAT_UDT_RECV_DATA*> NatUdtRecvDataList;

typedef struct _udp_reliable_recv_data_info
{	
	uint32				minSeq;									//Ӧ���ֵ�����ȥ��Ӧ�ò����С�������
	uint32				expectedSeq;							//ϣ����һ�����յ���˳���
	uint32				maxPeerAck;								//�Է�ȷ�ϵ����������

	NatUdtRecvDataList	recvDataList;                       // UDT���յ��������б�

	/*add by cc  һ���ֶΣ���һ�λص���Ӧ�ò�İ���pos ���Ӧ�İ�Ϊ���ΪminSeq�İ�
	Ӧ�ò���Բ���ÿ�ΰ�һ���������꣬pos��Ӧ�ò�����˸ð���ʵ�ʳ���*/
	int32				lastOnRecvPackPos;
}R_RECV_INFO;

typedef struct _udp_rtt_help
{
	uint32 sendIndex;								//��������
	uint32 sendTick;								//���͵�ʱ��
}UDP_RTT_HELP;


typedef struct _udt_remote_info_
{
	//�൱��SWL_SOCKET��ʵ�ʵ�ip��port
	uint32 ip;
	uint16 port;
	char Ip[64];
}UDT_REMOTE_INFO;

/**
 * UDT��״̬
 */
enum NAT_UDT_STATE
{
	UDT_STATE_STOPED,			// ֹͣ
	UDT_STATE_SYN_SENDING,		// ���ڷ��͵�1��SYN��
	UDT_STATE_SYN_RECVED,		// ���յ��Է���SYN��
	UDT_STATE_CONNECTED,		// �ѽ�������
	UDT_STATE_DISCONNECTED,		// �ѶϿ�����
};

typedef struct _udt_recorded_info_
{
	uint32			lastRecvTick;					//���һ�ν��յ����ݵ�tick
	uint32			lastSendTick;					//���һ�η������ݵ�tick
	byte			ackStatus;						//����ȷ�ϵ�״̬ �������� �ӳٷ��� û�÷���
	uint32			initSendSeq;
	uint32			initPeerSendSeq;
	uint32			lastSendIndex;					//��һ�η��͵ı�ʶ
	uint32			maxRecvIndex;					//�յ��ĶԷ������ı�ʶ
	uint32			maxPeerRecvIndex;				//�Է��յ������ı�ʶ
}UDT_RECORDED_INFO;

//ͳ����Ϣ
typedef struct _udt_statistic_
{
	uint32			totalSendPacketNum;		//�ܷ�����
	uint32			totalRecvPacketNum;		//���հ���
	uint32			reSendPacketNum;		//�ط��İ���
	uint32			connectTime;			//���ӽ�����ʱ��
	uint64			totalSendBytes;			//���͵����ֽ���
	uint64			totalRecvBytes;			//���յ����ֽ���
	uint64			reSendBytes;			//�ط����ֽ���
	uint16			localDataPort;			//���ط������ݵĶ˿�
}UDT_STATISTIC;

//�ش���ӵ�������㷨�����Ϣ
typedef struct _udt_ctrl_algorithm_
{
	bool				bResendRestrict;		//�ش�����
	bool				bCalculateRtt;

	//�ڼ����ش���һ�����ݰ���ÿ�ش�һ�ε�һ������+1��ÿ��ֻҪ֪���Է��յ��κ������ݣ�index������0
	//����������ڶԷ����ղ�������ʱ�������˱ܳ�ʱ�ش�
	byte				resendTime;

	//Ŀǰ�ķ���״̬����������ӵ�����⡢��ʱ�ش��������ش��ָ������»ָ���С����������
	//SWL_UDP_SENDMODE_SLOW_STARTUP  
	//SWL_UDP_SENDMODE_CROWDED_AVOID 
	//SWL_UDP_SENDMODE_TIMEOUT_RESEND		
	//SWL_UDP_SENDMODE_FAST_RESEND 
	byte				sendMode;

	//���ڼ�¼��һ�����ش��Ĵ��������ڼ�С���ڣ�ֻҪ�ǵ�2���ش����ۿ��ٻ��ǳ�ʱ�����ٴν���ʱ�ش�
	//������ᱻ�ٴμ�С
	byte				firstResend;	

	uint32				nextResendIndex;		//����ʼ�����Ƿ�Ҫ�ش������
	uint32              lastResendTick;         //�ϴ��ش�ʱ��
	uint32              lastUnackNum;           //�ϴ�δȷ�ϰ�����
	uint32              resendTimeout;          //�ش���ʱʱ��
	UDP_RTT_HELP		rttHelp;
}UDT_CTRL_ALG;

class CNatUdt;

class CUdtNotifier
{
public:
	 /**
     * ���ӽ����������ֳɹ���Ļص�
     * @param pUDT UDT����ָ��
     * @param iErrorCode 0 �ɹ� ����ֵ ʧ��
     */
    virtual void OnConnect(CNatUdt *pUDT, int iErrorCode) = 0;

    /**
     * �������ݻص���
     * �������ݽ��պ󣬴����ûص���
     * @param pUDT UDT����ָ��
     * @param pBuf ����ָ�룬ָ���ѽ��յ����ݡ�
     * @param iLen Ҫ�������ݵ��ֽڴ�С��
     * @return ���ػص���������Ѿ�����������ֽڴ�С����ֵ<=iLen
     */
    virtual int OnRecv(CNatUdt *pUDT, const void* pBuf, int iLen) = 0;

	 /**
     * �������ݻص�
     * �������������ݷ���ʱ�������ûص���
     * @param pUDT UDT����ָ��
     * @param pBuf ����ָ�룬ָ��Ҫ���͵����ݡ�
     * @param iLen Ҫ�������ݵ��ֽڴ�С��
     * @return ���سɹ���ʧ�� 0 �ɹ�  ����ֵ ʧ��
     */
	virtual int OnSendPacket(CNatUdt *pUDT, const void* pBuf, int iLen, CNatUdtTransport::SEND_OPTIONS *options) = 0;
};

typedef struct _nat_udt_config_
{
    byte					category;               // UDT�ķ���
    uint32					remoteIp;               // �Է���IP��ַ
    uint16					remotePort;             // �Է��Ķ˿ں�
    uint32					connectionID;           // ����ID
    uint32					maxRecvWindowSize;      // �����մ��ڴ�С
    CNatHeapDataManager*	pRecvHeapDataManeger;  // �������ݵĻ���ѹ����������ΪNULL�������ڲ�����maxSendWindowSize������
    uint32					maxSendWindowSize;      // ����ʹ��ڴ�С
    CNatHeapDataManager*	pSendHeapDataManeger;  // �������ݵĻ���ѹ����������ΪNULL�������ڲ�����maxSendWindowSize������
}NAT_UDT_CONFIG;

class CNatUdt
{
public:
    static const int RECV_PACKET_BUF_SIZE = MAX_WRAPPER_DATA_BUFFSIZE;    // ���� 1500
    static const int SEND_PACKET_BUF_SIZE = sizeof(UDP_SEND_PACKET_DATA); // ���� 552
public:
	static bool IsDatagramValid(const NAT_TRANSPORT_RECV_DATAGRAM* datagram);
	static byte GetDatagramCategory(const NAT_TRANSPORT_RECV_DATAGRAM* datagram);
    static uint32 GetDatagramConnectionId(const NAT_TRANSPORT_RECV_DATAGRAM* datagram);
    static bool IsDatagramSynCmd(const NAT_TRANSPORT_RECV_DATAGRAM* datagram);
public:
	CNatUdt();
	
	~CNatUdt();

	void SetHeartbeatInterval(unsigned long interval);
	void SetConnectionTimeout(unsigned long timeout);
	void SetKeepingHoleTTL(int ttl);

    bool Start(const NAT_UDT_CONFIG* config);

    void Stop();

    bool IsStarted() { return m_state != UDT_STATE_STOPED; };

    NatRunResult Run();

	uint32 GetConnectionID();
	uint32 GetRemoteIp() {return m_RemoteInfo.ip;}
    uint16 GetRemotePort() {return m_RemoteInfo.port;}
    
    bool IsMineDatagram(const NAT_TRANSPORT_RECV_DATAGRAM* datagram);

    int NotifyRecvDatagram(const NAT_TRANSPORT_RECV_DATAGRAM* datagram);

	int Send(const void *pData, int iLen);//Ӧ�ò���ô˽ӿڣ����Ϳɿ�����

	int Recv(void *pData, int iLen);//Ӧ�ò��������ô˽ӿڣ���ȡ�ɿ�����


	void SetNotifier(CUdtNotifier *pUdtNotifier);

    
	/**
     * �����ϲ���ͨ���ص��ķ�ʽ��ȡ���ݣ�����������ȡ����
     */
	void SetAutoRecv(bool bAutoRecv);

	 /**
     * ��ȡ��ǰ���Է������ݵ��ֽڴ�С
     * @return ���û�д����򷵻ص�ǰ���Է������ݵ��ֽڴ�С��0��ʾ���ͻ��������ˣ����򣬳�����-1
     */
    int GetSendAvailable();

    /**
     * ��ȡ���Խ������ݵ��ֽڴ�С����Ҫ����������������
     * @return ���û�д����򷵻ص�ǰ���Խ������ݵ��ֽڴ�С��0��ʾ�����ݣ����򣬳�����-1
     */
    int GetRecvAvailable();
private:
    int RecvPacket(const UDP_WRAPPER_DATA *packet, int packetSize);
	int OnNotifySendPacket(const void* pBuf, int iLen, CNatUdtTransport::SEND_OPTIONS *options = NULL);
	int OnNotifyRecvPacket(const void* pBuf, int iLen);
	void OnNotifyConnect(int iErrorCode);

	int HandleRecvReliableData(const UDP_WRAPPER_DATA *pWrapperData, int iLen, uint32 currentTick);
	int HandleRecvAckData(const UDP_WRAPPER_DATA *pData, int iLen, uint32 currentTick);
	int HandleRecvConnect(const UDP_WRAPPER_DATA *pData, int iLen, uint32 currentTick);
	int HandleRecvRST(const UDP_WRAPPER_DATA *pData, int iLen, uint32 currentTick);

	int CheckConnect(uint32 currentTick);
	int SendAckData(uint32 currentTick);
	int SendKeepingHole(uint32 currentTick);
	int ReleaseReliableData(uint32 seq, uint32 index, uint32 currentTick);
	uint32 CalculateRTO(uint32 currentTick, uint32 startTick);
	int ChangeMode(byte ucMode);
	int IncreaseCwnd(int iCount = 1, bool bResend = false);
	int AdjustRecvPacket();
	int RecvFirstReliableData(char **ppData, int *pLen);
	void AutoNotifyRecvPacket();

	//1	 �ش���һ������ 0 û��������Ҫ�ش���û�����ݻ���û�з��Ϸ�������������
	//-1 �쳣�������ӳ��� -2 ������ȥ
	//-4 û�е����ȷ��ͼ�� -5 δ����
	int ResendData(uint32 currentTick);

	//1	 ��ʾ������1������  
	//0	 ��ʾû���ݷ���
	//-1 �쳣������Ҫ�˳�
	//-2 ��ʾ������ʱ������ȥ
	//-3 ���������͵����һ��δȷ�ϵ��������ķ�Χ
	//-4 û�е����ȷ��ͼ��
	//-5 û��������
	//-6 ���ڲ���
	int SendReliableData(uint32 currentTick);

	int SendPacketData(UDP_SEND_PACKET_DATA *pPacket, uint32 currentTick);

	/**
     * ��������RST�źţ�֪ͨ�Զ����ӹر�
     */
	void Disconnect();

    bool InsertRecvData( uint32 seqNum, const UDP_WRAPPER_DATA* packet, int dataSize  );

    inline void DestroyRecvData(NAT_UDT_RECV_DATA* data) 
    {
        assert(data != NULL);
        m_pRecvDataManager->ReleaseMemory(data);
    }

	void InitUdtHeader(UDP_WRAPPER_HEAD &header);

	inline bool IsConnected() { return m_state == UDT_STATE_CONNECTED; };

	void Clear();
	bool IsResendTimeout(uint32 curTick);
private:
	NAT_UDT_STATE				m_state;
	CUdtNotifier				*m_pUdtNotifier;
	UDT_RECORDED_INFO			m_RecordedInfo;
	byte						m_Category;
	bool						m_bIsAutoRecv;//�ϲ�ȡ���ݵķ�ʽ�ǻص�������������

#ifndef UDT_WITHOUT_LOCK
	CPUB_Lock					m_RecvLock;//��m_RRecvInfo.mapReliableRecvData��m_RRecvInfo.minSeq
	CPUB_Lock					m_RSendInfoLock;//��m_RSendInfo
#endif

//�Զ�IP PORT����Ϣ
	UDT_REMOTE_INFO				m_RemoteInfo;

    //�������
    CNatHeapDataManager			*m_pSendDataManager;
    CNatHeapDataManager			*m_pOwnedSendDataManager; // �Լ����е�SendDataManagerָ�룬�����Ϊ�գ���Ҫ�Լ��ͷ�
    CNatHeapDataManager			*m_pRecvDataManager;
    CNatHeapDataManager			*m_pOwnedRecvDataManager; // �Լ����е�RecvDataManagerָ�룬�����Ϊ�գ���Ҫ�Լ��ͷ�
	R_SEND_INFO					m_RSendInfo;
	R_RECV_INFO					m_RRecvInfo;

	UDP_WRAPPER_DATA			m_SendTmpData;	//���ڷ���ACK SYN,��������ʱ���ٿռ�

	std::list<uint32>	m_listDisorderlyPacket;	//������������:disorderSeq

//ͳ����Ϣ
	UDT_STATISTIC				m_Statistic;

//�ش���ӵ���㷨�Ŀ�����Ϣ
	UDT_CTRL_ALG				m_CtrlAlg;
	short						m_sa;
	short						m_sv;
	uint32				m_dwRTT;
	uint32				m_dwRTO;
	int							m_iUsed;			//�²⻹�м������������� ��δ�ظ����Ҳ²�δ���������ݣ���Ϊ�������ϵİ���
	int							m_iCwnd;			//ӵ�����ڣ��������м���û�лظ������ݰ��������ϣ�һ�οɷ���������ȥ��
	int							m_iCrowdedNumerator;//ӵ��ʱ�ۼӵķ���
	int							m_iSsthresh;		//���������ޣ�ӵ��������������ʱ��ʼ����ӵ������

//��ʱʱ��
	uint32				m_connectionTimeout;
	uint32				m_heartbeatInterval;
	uint32				m_LastAckTime;
	uint32				m_LastResendTime;
	uint32				m_LastRemoveUnAckPackTime;
//
	uint32				m_dwMaxRecvReliableBuffCount;	//��¼����������ĵ���������
	uint32				m_curTickCount;
	uint32				m_keepingHoleTime;
	int					m_keepingHoleTTL;				// ���<=0������ԣ�Ĭ��Ϊ0
	UDT_P2P_VERSION_TYPE	m_remoteVersion;
};


#endif//_NAT_UDT_H_