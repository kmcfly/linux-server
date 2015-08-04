// NatTraversalProtocol.h: interface for the Protocol of traversal.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_TEST_PROTOCOL_H
#define _NAT_TEST_PROTOCOL_H

#include "NatBaseTypes.h"

////////////////////////////////////////////////////////////////////////////////
// Simple Data Block protocol interface

/**
 * Ĭ�������˿ڣ�UDP��
 */
#define NAT_TEST_PROT_SERVICE_PORT		8993

/**
 * ����Э��汾����ǰ=1.0
 */
#define NAT_TEST_PROT_VER_HIGH		1
#define NAT_TEST_PROT_VER_LOW		1

/**
 * Э����ʼ��־
 */
static const byte NAT_TEST_PROT_START_TAG[2] = {0xEF, 0xEF};

/**
 * ����Э�����ͣ�NAT_TEST_PORT_PROT_XX
 */
static const byte NAT_TEST_PORT_PROT_TCP = 0;
static const byte NAT_TEST_PORT_PROT_UDP = 1;


/**
 * ������������ID��NAT_TEST_ID_XX_REQ
 */
static const uint16 NAT_TEST_ID_SPEED_REQ = 0x0101;				// ������������
static const uint16 NAT_TEST_ID_PUB_IP_REQ = 0x0102;			// �⹫��IP��IPv4����������
static const uint16 NAT_TEST_ID_PORT_CONNECT_REQ = 0x0103;		// ��˿�������������

/**
 * ����Ӧ������ID��NAT_TEST_ID_XX_REPLY
 */
static const uint16 NAT_TEST_ID_SPEED_REPLY = 0x0201;			// ����Ӧ������
static const uint16 NAT_TEST_ID_PUB_IP_REPLY = 0x0202;			// �⹫��IP��IPv4��Ӧ������
static const uint16 NAT_TEST_ID_PORT_CONNECT_REPLY = 0x0203;	// ��˿�����Ӧ������


/**
 * ��������ģʽ NAT_TEST_SPEED_MODE_XX
 */
static const byte NAT_TEST_SPEED_MODE_ECHO = 0x01; // ���Բ�������
// ��������ģʽ������ֵ����ʾ�����Բ�������


#pragma pack(push)
#pragma pack(1)


/**
 * ��͸����Э��ͷ
 */
typedef struct _nat_test_prot_header
{
	byte		startTag[2];	// ��ʼ��־��������ֵ�İ�����������0xEFEF
	byte		versionHigh;	// �汾�Ÿ�λ
	byte		versionLow;		// �汾�ŵ�λ
	uint16		seq;			// ������ţ�������������ȡֵ��������������Ӧ������������𸴵���������������
	uint16		cmdId;			// ����ID��С����ʽ  
}NAT_TEST_PROT_HEADER;


/**
 * ������������Ľṹ
 */
typedef struct _nat_test_speed_req
{
	NAT_TEST_PROT_HEADER	header;			// Э��ͷ
	byte					mode;			// ����ģʽ��
	byte					reserved[3];	// �����ֶ�
	uint32					timestamp;		// �����ʱ���
	char					testData[512];	// �������ݣ���󳤶���512 Bytes
}NAT_TEST_SPEED_REQ;

/**
 * ����Ӧ������Ľṹ
 */
typedef struct _nat_test_speed_reply
{
	NAT_TEST_PROT_HEADER	header;			// Э��ͷ
	uint32					ip;				// ���󷽵Ĺ���IP
	uint16					port;			// ���󷽵Ĺ����˿�
	byte					mode;			// ����ģʽ��
	byte					reserved;		// �����ֶ�
	uint32					timestamp;		// �����ʱ���	
	char					testData[512];	// �������ݣ���󳤶���512 Bytes
}NAT_TEST_SPEED_REPLY;

/**
 * �⹫��IP��IPv4����������Ľṹ
 */
typedef struct _nat_test_pub_ip_req
{
	NAT_TEST_PROT_HEADER	header;			// Э��ͷ
	/* ������ */
}NAT_TEST_PUB_IP_REQ;

/**
 * �⹫��IP��IPv4��Ӧ������Ľṹ
 */
typedef struct _nat_test_pub_ip_reply
{
	NAT_TEST_PROT_HEADER	header;			// Э��ͷ
	uint32					ip;				// ���󷽵Ĺ���IP
	uint16					port;			// ���󷽵Ĺ����˿�
}NAT_TEST_PUB_IP_REPLY;

/**
 * ��˿�������������Ľṹ
 */
typedef struct _nat_test_port_connect_req
{
	NAT_TEST_PROT_HEADER	header;			// Э��ͷ
	uint16					port;			// ��Ҫ�����Զ˿�
	byte					netProtocol;	// ����Э�����ͣ�NAT_TEST_PORT_PROT_XX
	byte					reserved;		// �����ֶ�
}NAT_TEST_PORT_CONNECT_REQ;

/**
 * ��˿�����Ӧ������Ľṹ
 */
typedef struct _nat_test_port_connect_reply
{
	NAT_TEST_PROT_HEADER	header;			// Э��ͷ
	uint32					ip;				// ���󷽵Ĺ���IP
	uint16					port;			// ���󷽵Ĺ����˿�
}NAT_TEST_PORT_CONNECT_REPLY;

#pragma pack(pop)

#endif//_NAT_TEST_PROTOCOL_H