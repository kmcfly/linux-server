// NatTraversalProtocol.h: interface for the Protocol of traversal.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_TRAVERSAL_PROTOCOL_H
#define _NAT_TRAVERSAL_PROTOCOL_H

#include "NatBaseTypes.h"

////////////////////////////////////////////////////////////////////////////////
// NAT Traversal Protocol interface

/**
 * version for NAT Traversal XML
 */
#define NAT_TRAVERSAL_XML_VER_LEN_MAX  64			// �汾����󳤶�
static const uint32 NAT_TRAVERSAL_XML_VER_0_1_0_1	= NatMakeUint32(0, 1, 0, 1);
static const uint32 NAT_TRAVERSAL_XML_VER_0_2_0_1	= NatMakeUint32(0, 2, 0, 1);
static const uint32 NAT_TRAVERSAL_XML_VER_0_3_0_1	= NatMakeUint32(0, 3, 0, 1);
static const uint32 NAT_TRAVERSAL_XML_VER_0_4_0_1	= NatMakeUint32(0, 4, 0, 1);
static const uint32 NAT_TRAVERSAL_XML_VERSION		= NAT_TRAVERSAL_XML_VER_0_4_0_1;		// ��ǰĬ�ϵİ汾��

#define NAT_SERVER_LISTEN_PORT          8989 

/**
 * version for SimpleDataBlock
 */
static const uint16 NAT_SDB_VER_0_1		= NatMakeUint16(1, 0);
static const uint16 NAT_SDB_VER			= NAT_SDB_VER_0_1; // ��ǰĬ�ϵİ汾��

static const int32 NAT_DATA_BLOCK_MAX_SIZE = 1024;

// ��͸���豸�������ע���Ժ�ע��ĳ�ʱʱ�䣨�򱣳�ʱ�䣩����λ��ms
static const int32 NAT_TRAVERSAL_DEVICE_REG_TIMEOUT = 3 * 60 * 60 * 1000; // = 3 hours

// ��͸���豸�������ע���Ժ�˫�����෢�����ļ��ʱ�䣬��λ��ms
static const int32 NAT_TRAVERSAL_DEVICE_REG_HEARTBEAT = 1 * 60 * 60 * 1000; // = 1 hours

// ��͸���豸���������UDT���ӳ�ʱʱ�䣬��λ��ms
static const int32 NAT_TRAVERSAL_DEVICE_CONN_TIMEOUT = 3 * 60 * 1000; // = 3 minutes

// ��͸���豸����������UDT�����ļ��ʱ�䣬��λ��ms
static const int32 NAT_TRAVERSAL_DEVICE_CONN_HEARTBEAT = 10000;//1 * 60 * 1000; // = 1 minutes

// ��͸�Ŀͻ������ӷ������������ӵĳ�ʱʱ�䣨�򱣳�ʱ�䣩����λ��ms
static const int32 NAT_TRAVERSAL_CLIENT_TIMEOUT = 1 * 60 * 1000; // = 1 minutes

// ��͸�Ŀͻ�������������UDT�����ļ��ʱ�䣬��λ��ms
static const int32 NAT_TRAVERSAL_CLIENT_HEARTBEAT = 0; // = Ĭ�Ͻ�������

// ��͸�����󷢳���ĳ�ʱʱ�䣨����Чʱ�䣩����λ��ms
static const int32 NAT_TRAVERSAL_REQ_TIMEOUT = 1 * 60 * 1000; // = 1 minutes

#pragma pack(push)
#pragma pack(1)

typedef struct _nat_data_block_header
{
    NAT_VERSION_2 usVersion;
    union 
    {
        struct 
        {
            uint16 bEncrypted : 1;
        }Options;
        uint16 OptionsData;
    };
    int32  iDataLen;    
}NAT_DATA_BLOCK_HEADER;

typedef struct _nat_data_block
{
    NAT_DATA_BLOCK_HEADER header;
    union
    {
        char caData[NAT_DATA_BLOCK_MAX_SIZE + 1];
        struct 
        {
            uint32 dwOriginalLen;     // ԭ�����ݳ���
            char caOriginalData[NAT_DATA_BLOCK_MAX_SIZE + 1 - sizeof(uint32)]; //���ܺ������
        } sOriginal;
    };
}NAT_DATA_BLOCK;


////////////////////////////////////////////////////////////////////////////////
// Traversal Protocol interface
// P2Pͨ�ŵ�Э��汾��
typedef NAT_VERSION_2	NAT_P2P_PROTOCOL_VER;

typedef NAT_VERSION_4 NAT_TRAVERSAL_VERSION;

/**
 * ��͸Э������ͷ
 */
typedef struct _nat_traversal_req_header
{
    uint32         dwRequestSeq;         // �������
}NAT_TRAVERSAL_REQ_HEADER;

/**
 * ��͸Э��Ӧ��ͷ
 */
typedef struct _nat_traversal_reply_header 
{
    uint32   dwRequestSeq;        // �������
    uint32   dwStatus;            // Ӧ��״̬�����Ϊ�ǳɹ�������������ֶ�
}NAT_TRAVERSAL_REPLY_HEADER;

// DevicePeer��NatServerע��
typedef struct _nat_device_register_req
{
    NAT_TRAVERSAL_REQ_HEADER    stHeader;             // ����ͷ���������������
    char                        caDeviceNo[64];       // �豸�����к�
    NAT_P2P_PROTOCOL_VER        usP2PVersion;         // �豸P2Pͨ��Э��汾��
    byte                        ucRefuseRelay;        // �豸�Ƿ�ܾ��м̷���Ϊ0��ʾ���ܾ���Ϊ��0��ʾ�ܾ���Ĭ��Ϊ0
    byte                        ucNatType;            // �豸���������NAT���ͣ�Ϊ0��ʾδ֪���ͣ�����Ϊ�գ���ʾ0��Ĭ��Ϊ0
    uint16                      usUpnpPort;           // �豸����UDT�˿ڵ�UPNPӳ��˿ڣ�Ϊ0��ʾ��֧��UPNP������Ϊ�գ���ʾ0��Ĭ��Ϊ0
    uint32                      dwSessionId;          // �豸�ĻỰ�ţ�Ϊ0��ʾ��ֵ������Ϊ�գ���ʾ0��Ĭ��Ϊ0��
	char						caExtraInfo[512];	  // ��չ��Ϣ��������XML��ʽ���ڲ�������ֻ�����豸�Ϳͻ����Ͻ������Դ�͸��������͸����
}NAT_DEVICE_REGISTER_REQ;


// NatServer��DevicePeer����ע��Ӧ��
typedef struct _nat_device_register_reply 
{
    NAT_TRAVERSAL_REPLY_HEADER  stHeader;           // Ӧ��ͷ��������������ţ�Ӧ��״̬�����Ӧ��״̬Ϊ�ǳɹ�������������ֶ�
    uint32                      dwPeerIp;           // NatServer������DevicePeer��IP��ַ
    uint16                      usPeerPort;         // NatServer������DevicePeer�Ķ˿ں�
    uint32                      dwSessionId;          // �豸�ĻỰ�ţ�Ϊ0��ʾ��ֵ������Ϊ�գ���ʾ0��Ĭ��Ϊ0��
}NAT_DEVICE_REGISTER_REPLY;



//DevicePeer��NatServer�����޸�ע����Ϣ
typedef struct _nat_device_modify_req
{
	NAT_TRAVERSAL_REQ_HEADER	stHeader;				// ����ͷ���������������
	char						caDeviceNo[64];			// �豸�����к�
	uint16						usUpnpPort;				// �豸����UDT�˿ڵ�UPNPӳ��˿ڣ�Ϊ0��ʾ��֧��UPNP������Ϊ�գ���ʾ0��Ĭ��Ϊ0
	byte						ucRefuseRelay;			// �豸�Ƿ�ܾ��м̷���Ϊ0��ʾ���ܾ���Ϊ��0��ʾ�ܾ���Ĭ��Ϊ0
	byte						ucNatType;				// �豸���������NAT���ͣ�Ϊ0��ʾδ֪���ͣ�����Ϊ�գ���ʾ0��Ĭ��Ϊ0
	char						caExtraInfo[512];		// ��չ��Ϣ��������XML��ʽ���ڲ�������ֻ�����豸�Ϳͻ����Ͻ������Դ�͸��������͸����
	bool						bUpnpPort;				// ���ҽ���bUpnpPortΪtrueʱ��usUpnpPort�ֶ���Ч
	bool						bRefuseRelay;			// ���ҽ���bRefuseRelayΪtrueʱ��ucRefuseRelay�ֶ���Ч
	bool						bNatType;				// ���ҽ���bNatTypeΪtrueʱ��ucNatType�ֶ���Ч
	bool						bExtraInfo;				// ���ҽ���bExtraInfoΪtrueʱ��caExtraInfo�ֶ���Ч
}NAT_DEVICE_MODIFY_REQ;

//NatServer��DevicePeer�����޸�ע����Ϣ�Ľ��
typedef struct _nat_device_modify_reply
{
	NAT_TRAVERSAL_REPLY_HEADER	stHeader;		// Ӧ��ͷ��������������ţ�Ӧ��״̬�����Ӧ��״̬Ϊ�ǳɹ�������������ֶ�
}NAT_DEVICE_MODIFY_REPLY;



// ClientPeer��NatServer������DevicePeer����P2P����
typedef struct _nat_client_connect_p2p_req
{
    NAT_TRAVERSAL_REQ_HEADER    stHeader;            // ����ͷ���������������
    char                        dwDeviceNo[64];      // �豸�����к�	
    byte                        ucRequestPeerNat;    // ��������Ľ���NAT����
    NAT_P2P_PROTOCOL_VER        usP2PVersion;        // UDTЭ��İ汾��
    uint32                      dwConnectionId;      // ���ӵ�Id�����ڱ�ʶ��Ҫ������P2P����
}NAT_CLIENT_CONNECT_P2P_REQ;



// NatServer��ClientPeer����P2P��������Ӧ��
typedef struct _nat_client_connect_p2p_reply 
{
    NAT_TRAVERSAL_REPLY_HEADER  stHeader;           // Ӧ��ͷ��������������ţ�Ӧ��״̬�����Ӧ��״̬Ϊ�ǳɹ�������������ֶ�
    uint32                      dwDevicePeerIp;     // �豸�ĳ�������ip
    uint16                      usDevicePeerPort;   // �豸�ĳ������Ķ˿ں�
    byte                        ucDevicePeerNat;    // ��������Ľ���NAT����
    byte                        ucRefuseRelay;      // �豸�Ƿ�ܾ��м̷���Ϊ0��ʾ���ܾ���Ϊ��0��ʾ�ܾ�������Ϊ�գ���ʾ0��Ĭ��Ϊ0��
    uint16                      usUpnpPort;         // �豸����UDT�˿ڵ�UPNPӳ��˿ڣ�Ϊ0��ʾ��֧��UPNP������Ϊ�գ���ʾ0��Ĭ��Ϊ0
}NAT_CLIENT_CONNECT_P2P_REPLY;

// NatServer��DevicePeer֪ͨP2P��������
typedef struct _nat_notify_connect_p2p_req 
{
    NAT_TRAVERSAL_REQ_HEADER    stHeader;             // ����ͷ���������������
    uint32                      dwRequestPeerIp;      // �Ͷ˵�������ַip
    uint16                      dwRequestPeerPort;    // �ͻ��˵�������ַport
    byte                        ucRequestPeerNat;     // �������NAT����
    NAT_P2P_PROTOCOL_VER        usP2PVersion;         // UDTЭ��İ汾��
    uint32                      dwConnectionId;       // ���ӵ�Id�����ڱ�ʶ��Ҫ������P2P����
}NAT_NOTIFY_CONNECT_P2P_REQ;


// DevicePeer��NatServer����֪ͨP2P���������Ӧ��
typedef struct _nat_notify_connect_p2p_reply 
{
    NAT_TRAVERSAL_REPLY_HEADER  stHeader;           // Ӧ��ͷ��������������ţ�Ӧ��״̬�����Ӧ��״̬Ϊ�ǳɹ�������������ֶ�
}NAT_NOTIFY_CONNECT_P2P_REPLY;

// ClientPeer��NatServer������DevicePeer����Relay����
typedef struct _nat_client_connect_relay_req
{
    NAT_TRAVERSAL_REQ_HEADER    stHeader;             // ����ͷ���������������
    char                        dwDeviceNo[64];       // �豸�����к�	
}NAT_CLIENT_CONNECT_RELAY_REQ;

// NatServer��ClientPeer����Relay��������Ӧ��
typedef struct _nat_client_connect_relay_reply
{
    NAT_TRAVERSAL_REPLY_HEADER  stHeader;               // Ӧ��ͷ��������������ţ�Ӧ��״̬�����Ӧ��״̬Ϊ�ǳɹ�������������ֶ�
    uint32                      dwRelayServerIp;        // RelayServer��IP��ַ
    uint16                      usRelayServerPort;      // RelayServer�Ķ˿ں�
    uint32                      ucRelayConnectionId;    // Relay�����Ӻţ����ڱ�ʶ����������������
}NAT_CLIENT_CONNECT_RELAY_REPLY;

// NatServer��DevicePeer֪ͨRelay��������
typedef struct _nat_notify_connect_relay_req 
{
    NAT_TRAVERSAL_REQ_HEADER    stHeader;             // ����ͷ���������������
    uint32                      dwRelayServerIp;      // RelayServer��IP��ַ
    uint16                      dwRelayServerPort;    // RelayServer�Ķ˿ں�
    uint32                      dwConnectionId;       // ���ӵ�Id�����ڱ�ʶ��Ҫ������P2P����
}NAT_NOTIFY_CONNECT_RELAY_REQ;

// DevicePeer��NatServer����֪ͨRelay���������Ӧ��
typedef struct _nat_notify_connect_relay_reply 
{
    NAT_TRAVERSAL_REPLY_HEADER  stHeader;               // Ӧ��ͷ��������������ţ�Ӧ��״̬�����Ӧ��״̬Ϊ�ǳɹ�������������ֶ�
}NAT_NOTIFY_CONNECT_RELAY_REPLY;

// ClientPeer��NatServer��ȡDevicePeer����Ϣ
typedef struct _nat_fetch_device_req 
{
    NAT_TRAVERSAL_REQ_HEADER    stHeader;             // ����ͷ���������������
    char                        dwDeviceNo[64];       // �豸�����к�
}NAT_FETCH_DEVICE_REQ;

// NatServer��ClientPeer����DevicePeer����Ϣ
typedef struct _nat_fetch_device_reply
{
    NAT_TRAVERSAL_REPLY_HEADER  stHeader;               // Ӧ��ͷ��������������ţ�Ӧ��״̬�����Ӧ��״̬Ϊ�ǳɹ�������������ֶ�
    uint32                      dwDevicePeerIp;         // DevicePeer��IP��ַ����ѭ��192.168.1.1���ĸ�ʽ
    uint16                      usDevicePeerPort;       // DevicePeer�Ķ˿ں�
    byte                        ucCanRelay;             // �豸�Ƿ����ʹ���м�����
	byte                        ucNatType;              // �豸���������NAT���ͣ�Ϊ0��ʾδ֪���ͣ�����Ϊ�գ���ʾ0��Ĭ��Ϊ0
    uint16                      usUpnpPort;	            // �豸����UDT�˿ڵ�UPNPӳ��˿ڣ�Ϊ0��ʾ��֧��UPNP������Ϊ�գ���ʾ0��Ĭ��Ϊ0
	char						caExtraInfo[512];		// ��չ��Ϣ���ڲ�������ֻ�����豸�Ϳͻ����Ͻ������Դ�͸��������͸����	
}NAT_FETCH_DEVICE_REPLY;



// ��͸��������
typedef struct _nat_traversal_heartbeat
{
    char                  uaDeviceNo[64];      // �豸�����к�	
}NAT_TRAVERSAL_HEARTBEAT;

/**
 * ��͸�豸��չ��Ϣ
 * ��׼��
 */
typedef struct _nat_device_extra_info
{
		uint32         deviceType;          /**< �豸���ͣ��ȼ���ʵ���豸�еĲ�Ʒ�ͺ� */
		char           deviceVersion[128];  /**< �豸�汾�ţ���"-"Ϊ�ָ����� ��"1.0.0-XXX-XX"
												 ���У���1����Ϊ��Ҫ�汾�� */
		uint16         deviceWebPort;       /**< �豸��WEB�����˿ںţ�TCP�� */
		uint16         deviceDataPort;      /**< �豸�����������˿ڣ�TCP�� */
}NAT_DEVICE_EXTRA_INFO;

#pragma pack(pop)

enum NatTraversalCmdId
{
    /* �������� */
    NAT_ID_DEVICE_REGISTER_REQ			   = 10001,  // DevicePeer��NatServerע��
    NAT_ID_CLIENT_CONNECT_P2P_REQ         = 10002,  // ClientPeer��NatServer������DevicePeer����P2P����
    NAT_ID_NOTIFY_CONNECT_P2P_REQ         = 10003,  // NatServer��DevicePeer֪ͨP2P��������
    NAT_ID_CLIENT_CONNECT_RELAY_REQ       = 10004,  // ClientPeer��NatServer������DevicePeer����Relay����
    NAT_ID_NOTIFY_CONNECT_RELAY_REQ       = 10005,  // NatServer��DevicePeer֪ͨRelay��������
    NAT_ID_FETCH_DEVICE_REQ               = 10006,  // ClientPeer��NatServer��ȡDevicePeer����Ϣ
	NAT_ID_DEVICE_MODIFY_REQ			  = 10007,  // DevicePeer��NatServer�����޸�ע����Ϣ


    /* Ӧ������ */
    NAT_ID_DEVICE_REGISTER_REPLY           = 20001,  // NatServer��DevicePeer����ע��Ӧ��
    NAT_ID_CLIENT_CONNECT_P2P_REPLY        = 20002,  // NatServer��ClientPeer����P2P��������Ӧ��
    NAT_ID_NOTIFY_CONNECT_P2P_REPLY        = 20003,  // DevicePeer��NatServer����֪ͨP2P���������Ӧ��
    NAT_ID_CLIENT_CONNECT_RELAY_REPLY      = 20004,  // NatServer��ClientPeer����Relay��������Ӧ��
    NAT_ID_NOTIFY_CONNECT_RELAY_REPLY      = 20005,  // DevicePeer��NatServer����֪ͨRelay���������Ӧ��
    NAT_ID_FETCH_DEVICE_REPLY              = 20006,  // NatServer��ClientPeer����DevicePeer����Ϣ
	NAT_ID_DEVICE_MODIFY_REPLY			   = 20007,  // NatServer��DevicePeer�����޸�ע����Ϣ�Ľ��

    /* ��ͨ���� */
    NAT_ID_HEARTBEAT                       = 1,      // ��������
    NAT_ID_DEVICE_UNREGISTER               = 2,      // �豸ȡ��ע������
    NAT_ID_UNKNOW                          = 0       // δ֪����
};


enum TraversalReplyStatus
{
    REPLY_STATUS_OK                  = 0,      // �ɹ�
    REPLY_STATUS_UNKNOWN             = 1,      // δ֪����
    REPLY_STATUS_DEVICE_OFFLINE      = 2,	   // �豸������
    REPLY_STATUS_DEVICE_REGISTERED   = 3,      // �豸�ظ�ע��
	REPLY_STATUS_SERVER_OVERLOAD     = 4,      // ���������ع��أ��޷����ṩ����
	REPLY_STATUS_DEVICE_NO_RELAY     = 5,      // �豸��֧���м�ͨ�Ź���
	REPLY_STATUS_PEER_INVALID        = 6       // ʧЧ�Ľ��
};

const static int32 TRAVERSAL_REPLY_STATUS_COUNT = 7;

static const char* TRAVERSAL_REPLY_STATUS_NAMES[TRAVERSAL_REPLY_STATUS_COUNT] = {
    "OK",                   // REPLY_STATUS_OK
    "Unknown",              // REPLY_STATUS_UNKNOWN
    "Device Offline",       // REPLY_STATUS_DEVICE_OFFLINE
    "Device Registered",    // REPLY_STATUS_DEVICE_REGISTERED
    "Server Overload",      // REPLY_STATUS_SERVER_OVERLOAD
    "Device No Relay",      // REPLY_STATUS_DEVICE_NO_RELAY
	"Peer Invalid"          // REPLY_STATUS_PEER_INVALID
};


inline const char* GetTraversalReplyStatusName(int32 status)
{
    if (status >= 0 && status < TRAVERSAL_REPLY_STATUS_COUNT)
    {
        return TRAVERSAL_REPLY_STATUS_NAMES[status];
    }
    return TRAVERSAL_REPLY_STATUS_NAMES[REPLY_STATUS_UNKNOWN];
}

inline const char* GetTraversalReplyStatusName(TraversalReplyStatus status)
{
    return GetTraversalReplyStatusName((int32)status);
}

#endif//_NAT_TRAVERSAL_PROTOCOL_H