
#ifndef  __NAT_SOCK_MANAGER__
#define __NAT_SOCK_MANAGER__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "NatSocketBase.h"


//�ϲ�Ӧ��ͨ��key����CNatSocketBase,�����ϲ�͵ײ�֮��Ĺ�ϵ
class CNatSockManager
{
public:
    CNatSockManager();
    ~CNatSockManager();

    static CNatSockManager* GetSocketManager();
  
    NatSocket Add(CNatSocketBase *pSockBase);

    int Remove(NatSocket sock);

    void Disable(CNatSocketBase *pSockBase);
    
    int CloseSocket(NatSocket sock);

    int GetSocketInfo(NatSocket sock, NAT_SOCKET_INFO &info);

    int Select(NatSocketSet *readSet, NatSocketSet *writeSet, int iTimeout);

    int Send(NatSocket sock, const void *pBuf, int iLen);

    int Recv(NatSocket sock, void *pBuf, int iLen);

    /**
     * ��ȡ��ǰ���Է������ݵ��ֽڴ�С
     * @return ���û�д����򷵻ص�ǰ���Է������ݵ��ֽڴ�С��0��ʾ���ͻ��������ˣ����򣬳�����-1
     */
    int GetSendAvailable(NatSocket sock);

    /**
     * ��ȡ���Խ������ݵ��ֽڴ�С����Ҫ����������������
     * @return ���û�д����򷵻ص�ǰ���Խ������ݵ��ֽڴ�С��0��ʾ�����ݣ����򣬳�����-1
     */
    int GetRecvAvailable(NatSocket sock);

private:
    CNatSocketBase* FindSock(NatSocket sock);
private:
    typedef std::map<void*, CNatSocketBase*> NatSocketMap;
private:
    CPUB_Lock		  m_lock;
    NatSocketMap      m_sockMap;	

};

#endif//__NAT_SOCK_MANAGER__
