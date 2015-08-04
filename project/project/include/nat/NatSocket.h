// NatSocket.h: interface for the NatSocket type.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_SOCKET_H_
#define _NAT_SOCKET_H_

#include "NatUserDefine.h"

#include <set>

typedef void* NatSocket;

#define NAT_SOCKET_INVALID NULL

typedef std::set<NatSocket> NatSocketSet;

/**
 * NAT�ͻ��˴���
 */
enum NAT_CLIENT_ERROR
{
    NAT_CLI_OK		= 0,               // �ɹ�
    NAT_CLI_ERR_UNKNOWN,                // δ֪����
    NAT_CLI_ERR_TIMEOUT,               // �ͻ��˳�ʱ
    NAT_CLI_ERR_DEVICE_OFFLINE,	       // �豸������
    NAT_CLI_ERR_SERVER_OVERLOAD,       // ���������ع��أ��޷����ṩ����
    NAT_CLI_ERR_DEVICE_NO_RELAY,       // �豸��֧���м�ͨ�Ź���
    NAT_CLI_ERR_SYS,                   // ϵͳ����
    NAT_CLI_ERR_NETWORK                // �������
};

/**
 * NAT�ͻ���״̬
 */
enum NAT_CLIENT_STATE
{
	NAT_CLI_STATE_INIT		= 0,		// ����ϵͳ����
	NAT_CLI_STATE_P2P_SERVER,			// P2P��͸ģʽ�£��������ͨ�ŵĹ���
	NAT_CLI_STATE_P2P_DEVICE,			// P2P��͸ģʽ�£����豸ͨ�ŵĹ���
	NAT_CLI_STATE_RELAY_SERVER,			// RELAYģʽ�£��������ͨ�ŵĹ���
	NAT_CLI_STATE_RELAY_DEVICE,			// RELAYģʽ�£����豸ͨ�ŵĹ���
};

const char* NAT_GetClientErrorText(NAT_CLIENT_ERROR error);

enum NAT_SOCKET_CATEGORY
{
	NAT_SOCKET_CAT_NONE		= 0x00000000,	/**< �޷��࣬��ʾsocket��Ч */
	NAT_SOCKET_CAT_P2P		= 0x00000001,	/**< �㵽�㴩͸ */
	NAT_SOCKET_CAT_RELAY	= 0x00000002,	/**< ת�� */
	NAT_SOCKET_CAT_DIRECT	= 0x00000004,	/**< ֱ�� */
	NAT_SOCKET_CAT_TCP		= 0x00000008,   /**< ������Tcp����*/
};

typedef struct _nat_socket_info_
{
	unsigned long  remoteIp;
	unsigned short remotePort;
	NAT_SOCKET_CATEGORY category;
}NAT_SOCKET_INFO;

const char* NAT_GetSocketCategoryText(NAT_SOCKET_CATEGORY category);

/**
 * NATģ��ĳ�ʼ������Ҫ�ڳ���ʼ����ʱ����
 * ���̰߳�ȫ
 */
int NAT_Init();

/**
 * NATģ�����ֹ�����ͷ�NATģ����ռ�õ�ȫ����Դ����Ҫ�ڳ����������ʱ����
 * ���̰߳�ȫ
 */
int NAT_Quit();

int NAT_CloseSocket(NatSocket sock);

int NAT_GetSocketInfo(NatSocket sock, NAT_SOCKET_INFO &info);

int NAT_Select(NatSocketSet *readSet, NatSocketSet *writeSet, int iTimeout);

int NAT_Send(NatSocket sock, const void *pBuf, int iLen);

int NAT_Recv(NatSocket sock, void *pBuf, int iLen);

/**
 * ��ȡ��ǰ���Է������ݵ��ֽڴ�С
 * @return ���û�д����򷵻ص�ǰ���Է������ݵ��ֽڴ�С��0��ʾ���ͻ��������ˣ����򣬳�����-1
 */
int NAT_GetSendAvailable(NatSocket sock);

/**
 * ��ȡ���Խ������ݵ��ֽڴ�С����Ҫ����������������
 * @return ���û�д����򷵻ص�ǰ���Խ������ݵ��ֽڴ�С��0��ʾ�����ݣ����򣬳�����-1
 */
int NAT_GetRecvAvailable(NatSocket sock);

bool NAT_IsSocketInSet(NatSocket sock, NatSocketSet *socketSet);

void NAT_EraseSocketFromSet(NatSocket sock, NatSocketSet *socketSet);

void NAT_InsertSocketToSet(NatSocket sock, NatSocketSet *socketSet);

#endif//_NAT_SOCKET_H_