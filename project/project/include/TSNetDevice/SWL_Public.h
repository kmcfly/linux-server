// SWL_Public.h: interface for the CSWL_Public class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_SWL_PUBLIC_H__3834396E_DB0F_48F9_B5A2_DA88C1EA87A4__INCLUDED_
#define AFX_SWL_PUBLIC_H__3834396E_DB0F_48F9_B5A2_DA88C1EA87A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SWL_TypeDefine.h"

#ifdef  __ENVIRONMENT_WIN32__
//#include <Winsock2.h>		//liujiang
#include <afxsock.h>		//liuhao
#elif defined  __ENVIRONMENT_LINUX__
#include <sys/types.h>
#include <sys/socket.h>
#else
	#error  UNKNOWN ENVIRONMENT	
#endif


//sssssssssssssssssssssssssss
#define   _DEBUG_SWL
#ifdef   _DEBUG_SWL
#include  <stdio.h>
#define   SWL_PRINTF(format,argc) printf(format, ##argc)
#else
#define   SWL_PRINTF(format,argc)
#endif


//��ʼ������socket�Ŀ�
int SWL_Init();	//���̰߳�ȫ

//����socket�Ŀ�
int SWL_Quit();	//���̰߳�ȫ

//socket��ĳ����ӡ��Ϣ
void SWL_PrintError(const char* pFile, int iLine);

//����һ��socket,����������socket�������˷���������
SWL_socket_t SWL_CreateSocket(int iDomain, int iType, int iProtocol);

//�ͷ�socket��Դ
int SWL_CloseSocket(SWL_socket_t sSock);

//�ر��Ѿ������ϵ�socket
int SWL_Shutdown(SWL_socket_t sSock, int iHow);

//��socket
int SWL_Bind(SWL_socket_t sSock, const struct sockaddr *name, SWL_socklen_t len);

//����socket
int SWL_Listen(SWL_socket_t sSock, int iBacklog);

//accept socket
int SWL_Accept(SWL_socket_t sSock, struct sockaddr *pAddr, SWL_socklen_t *pLen);

//��������
//int SWL_Send(SWL_socket_t sSock, const void* pBuf, int iLen, int iFlags);

//��������
//int SWL_Recv(SWL_socket_t sSock, void* pBuf, int iLen, int iFlags);

//��������
//return value: >=0 ���ͳ������ݳ��ȣ� 
//              SWL_SOCKET_ERROR ʧ��
//��������� pBuf Ҫ���͵����ݵ�ָ��  iLen Ҫ���͵����ݵĳ���  iFlags ������
//BUGS: linux�µ�sendԭ��Ϊ	  ssize_t send(int s, const void *buf, size_t len, int flags);
//		windows�µ�sendԭ��Ϊ int send(SOCKET s, const char FAR *buf,   int len,  int flags);
//      ����û�а�windows��linux�µ���������ͳһ������ֱ�Ӻ��������ͣ�ʹ����int
inline int SWL_Send(SWL_socket_t sock, const void* pBuf, int iLen, int iFlags)
{
#ifdef            __ENVIRONMENT_WIN32__
	return send(sock, static_cast<const char*>(pBuf), iLen, 0);
#elif defined     __ENVIRONMENT_LINUX__
	return static_cast<int>(send(sock, pBuf, static_cast<size_t>(iLen), MSG_DONTWAIT | MSG_NOSIGNAL));
#endif
}

//��������
//return value: >0 �ɹ��յ��������� SWL_SOCKET_ERROR �д����� 0 �Է�socket�����ر�
//BUGS:linux��ԭ��Ϊ ssize_t recv(int s, void *buf, size_t len, int flags);
//     windows��ԭ�� int recv(SOCKET s, char FAR *buf, int len, int flags);
//      ����û�а�windows��linux�µ���������ͳһ������ֱ�Ӻ��������ͣ�ʹ����int
inline int SWL_Recv(SWL_socket_t sock, void* pBuf, int iLen, int iFlags)
{
#ifdef            __ENVIRONMENT_WIN32__
	return recv(sock, static_cast<char*>(pBuf), iLen, 0);
#elif defined     __ENVIRONMENT_LINUX__
	return static_cast<int>(recv(sock, pBuf, static_cast<size_t>(iLen), MSG_DONTWAIT));
#endif
}

//�첽����
int SWL_Connect(SWL_socket_t sSock, const struct sockaddr *pServAddr, SWL_socklen_t len, int iTimeOut);

//��The socket is marked as nonblocking and no connections are present to be accepted
//��������socket����ʱ��һ�Ṧ����û�����ݹ���
//��ʱ�򷵻�true
bool SWL_EWOULDBLOCK();

#endif // !defined(AFX_SWL_PUBLIC_H__3834396E_DB0F_48F9_B5A2_DA88C1EA87A4__INCLUDED_)

