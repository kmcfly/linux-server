// NatSocketBase.h: interface for the CNatSocketBase class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_SOCKET_BASE_H_
#define _NAT_SOCKET_BASE_H_

#include "NatCommon.h"
#include "NatSocket.h"
#include "NatRunner.h"

class CNatSocketBase;

/**
 * Socketͨ�����ݴ���ص�֪ͨ
 */
class CNatSocketCommNotifier
{
public:
    virtual ~CNatSocketCommNotifier() {};
    /**
     * �������ݻص�������
     * �������ݽ��պ󣬴����ûص���
     * @param sock �׽��֡�
     * @param pBuf ����ָ�룬ָ���ѽ��յ����ݡ�
     * @param iLen Ҫ�������ݵ��ֽڴ�С��
     * @return �����Ѿ�����������ֽڴ�С
     */
    virtual int OnRecv(CNatSocketBase *sock, const void* pBuf, int iLen) = 0;
    
};

/**
 * NatSocket�����࣬
 * ������NatSocket����Ϊ�ӿڣ�����Send()��Recv()�Լ���׼��״̬��
 */
class CNatSocketBase: public CNatRunner
{
public:
    /**
     * NatSocket��״̬
     */
    enum SocketState
    {
        STATE_CLOSED,       /**< �ѹرգ���ʾNatSocket�Ѿ�������Ч״̬ */
        STATE_INIT,         /**< �ѳ�ʼ������ʾNatSocket�Ѿ���ʼ������ʱ���ڴ������ӹ��� */
		STATE_CONNECTING,   /**< �������ӣ���ʾIP��������ѽ�����Э�����������ڽ���*/
        STATE_CONNECTED,    /**< �����ӣ���ʾNatSocket�Ѿ��������ӣ����������շ����� */
        STATE_DISCONNECTED  /**< �ѶϿ����ӣ���ʾNatSocket�Ѿ��Ͽ����ӣ��������ر� */
    };
public:
    /**
     * ���캯��
     */
    CNatSocketBase();
    /**
     * ��������
     */
    virtual ~CNatSocketBase();
    /**
     * �Ƿ��Ѿ���������
     * @return �����Ƿ��Ѿ���������
     */
    bool IsConnected();
    /**
     * �Ƿ��Ѿ���ʼ��
     * @return �����Ƿ��Ѿ���ʼ��
     */
    bool IsInited();

	/**
	 * ��ȡNAT Socket�ķ���
	 * @return ����Socket����
	 */
	virtual NAT_SOCKET_CATEGORY GetSocketCategory() = 0;
    /**
     * �������ݡ�
     * @param[in] pBuf ����ָ�룬ָ��Ҫ���͵����ݡ�
     * @param[in] iLen Ҫ�������ݵ��ֽڴ�С��
     * @return ���û�д����򷵻�ʵ�ʷ��͵��ֽ���������ҪС�ڻ����iLen�����򷵻ش���ֵ-1��
     */    
    virtual int Send(const void *pBuf, int iLen) = 0;

    /**
     * ��������
     * �����������ݣ���Ҫ�ڷ��Զ�����ģʽ�²��������ã�����ģʽĬ��������Ƿ��Զ�ģʽ
     * @param[in] pBuf ����ָ�룬ָ��Ҫ�������ݵ��ڴ档
     * @param[in] iLen Ҫ�������ݵ��ֽڴ�С��
     * @return ���û�д����򷵻�ʵ�ʽ��յ��ֽ���������ҪС�ڻ����iLen�����򷵻ش���ֵ-1��
     */
    virtual int Recv(void *pBuf, int iLen) = 0;

    /**
     * ��ȡ��ǰ���Է������ݵ��ֽڴ�С
     * @return ���û�д����򷵻ص�ǰ���Է������ݵ��ֽڴ�С��0��ʾ���ͻ��������ˣ����򣬳�����-1
     */
    virtual int GetSendAvailable() = 0;

    /**
     * ��ȡ���Խ������ݵ��ֽڴ�С����Ҫ����������������
     * @return ���û�д����򷵻ص�ǰ���Խ������ݵ��ֽڴ�С��0��ʾ�����ݣ����򣬳�����-1
     */
    virtual int GetRecvAvailable() = 0;

    /**
     * �����Ƿ������Զ�����ģʽ
     * NatSocketĬ��������Ǵ��ڷ��Զ�����ģʽ
     * @param[in] bAutoRecv �Ƿ������Զ�����ģʽ
     * @return ���û�д��󣬷���0�����򣬳�����-1��һ����NatSocket�Ѿ����ڶϿ����ӻ��ѹر�״̬��
     */
    virtual int SetAutoRecv(bool bAutoRecv) = 0;
    
    /**
     * ���ô���ͨ�ţ�Communication��֪ͨ�ص�
     * ������ֻ�����ø�֪ͨ�ص������ܽ����Զ�����ģʽ�����յ�������
     * @param[in] notifier ����ͨ��֪ͨ�ص�
     */
    virtual void SetCommNotifier( CNatSocketCommNotifier* notifier );

    /**
     * �ر�NatSocket
     * ִ�йرպ�NatSocketӦ�ô���Closing״̬���ڲ�����ᾡ�콫�������Ĺرա�
     * @return ���û�д�����0�����򣬷��ش���ֵ-1��
     */
    virtual int Close();

    /**
     * ��ȡԶ�����ӵ�IP��ַ
     * ֻ�е�NatSocket�Ѿ����������Ժ󣬲���ȷ����ֵ�Ǿ�ȷ��
     * @return ����Զ�����ӵ�IP��ַ
     */
    virtual unsigned long GetRemoteIp() = 0;
    /**
     * ��ȡԶ�����ӵĶ˿ں�
     * ֻ�е�NatSocket�Ѿ����������Ժ󣬲���ȷ����ֵ�Ǿ�ȷ��
     * @return ����Զ�����ӵĶ˿ں�
     */
    virtual unsigned short GetRemotePort() = 0;

    /**
     * ��ȡNatSocket��״̬
     * @return ����NatSocket��״̬
     */
    SocketState GetState();

	/**
     * ��ʾ�ѱ�ʹ�ã�����ֱ�ӱ��ͷ�
     */
	void Use();

	 /**
     * �Ƿ�����ʹ��
     * @see Use()
     * @return �Ƿ�������ʹ�õĹ���
     */
    bool IsUsing();
protected:
    /**
     * ֪ͨ�����ݽ���
     * �������Զ�����ģʽ�����յ��˶Է����͵����ݣ���Ӧ���øú������Դ������ݽ��յĻص�֪ͨ
     * �������ʹ��
     */
    virtual int NotifyOnRecv(const void* pBuf, int iLen);

    /**
     * ����NatSocket��״̬
     * �������ʹ��
     */
    virtual void ChangeState(SocketState state);
protected:
    CNatSocketCommNotifier* m_commNotifier;
    SocketState m_socketState;
    bool m_bIsUsing;
    CPUB_Lock m_notifierLock;
};

/**
 * Socket���ӻص�֪ͨ
 */
class CNatSocketConnectNotifier
{
public:
    virtual ~CNatSocketConnectNotifier() {};
    virtual void OnConnect(NatSocket sock, int iErrorCode)= 0;
};


#endif//_NAT_SOCKET_BASE_H_