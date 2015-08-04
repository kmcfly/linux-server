// SWL_Public.h: interface for the CSWL_Public class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SWL_PUBLIC_H__3834396E_DB0F_48F9_B5A2_DA88C1EA87A4__INCLUDED_)
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
int SWL_Send(SWL_socket_t sSock, const void* pBuf, int iLen, int iFlags);

//��������
int SWL_Recv(SWL_socket_t sSock, void* pBuf, int iLen, int iFlags);

//�첽����
int SWL_Connect(SWL_socket_t sSock, const struct sockaddr *pServAddr, SWL_socklen_t len, int iTimeOut);

//��The socket is marked as nonblocking and no connections are present to be accepted
//��������socket����ʱ��һ�Ṧ����û�����ݹ���
//��ʱ�򷵻�true
bool SWL_EWOULDBLOCK();

#endif // !defined(AFX_SWL_PUBLIC_H__3834396E_DB0F_48F9_B5A2_DA88C1EA87A4__INCLUDED_)

