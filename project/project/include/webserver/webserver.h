#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2008-11-11
** Name         : webserver.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef __ENVIRONMENT_LINUX__
#include "SWL_ListenProc.h"
#else
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>
#endif

#include "common.h"
#include "mylist.cpp"
#include "LogMan.h"

#include "MessageMan.h"

const unsigned long MAX_PART_NUM = 32;
const unsigned long MAX_CLIENT = 63;

#define ERROR_LOG_DIR		"error_log"
#define ERROR_LOG_OVERVIEW	"error_log_overview.cgi"
#define DEFAULT_PAGE		"index.html"

#define FD_LIFE_FULL	3
#define CMD_ID_SERVE_STATIC		0x10000000
#define CMD_ID_SERVE_EXE		0x10000001

#define MAX_EXIST_LANGUAGE_NUM 5
//#define MAX_EXIST_SCRIPT_NUM 14

#ifdef __ENVIRONMENT_LINUX__
typedef int				FILE_D;
#else
typedef CSWL_TCPLink	*FILE_D;
#endif

typedef enum _page_type
{
	PAGE_STATIC		= 0,
	PAGE_EXE		= 1
}PAGE_TYPE;

typedef struct _http_header_info
{
	unsigned long	bXMLBody;
	unsigned long	Content_Length;
}HTTP_HEADER_INFO;

//typedef struct _cmd_data
//{
//	int						readyState;		//0:不可用.每次处理命令时取该值最大的命令执行.
//	unsigned long			cmdId;
//	long					paramLen;
//	unsigned char			*pParam;
//}CMD_DATA;
//
//typedef struct _client_fd
//{
//	int				life;	//正在使用的连接对象生命指数为3,在不用了之后生命指数降为2，只有在生命指数变为零时才可复用，
//							//如果找不到可重用的连接，把所有连接减1，再找。目的是避免一个不用的连接马上
//							//被使用而带来上一连接未处理的遗留问题
//	FILE_D			fdServe;
//	CMD_DATA		reqCmd;
//	CMD_DATA		resCmd;
//}CLIENT_FD;

typedef struct _client_cmd_data
{
	unsigned long	dwClientID;
	char			*pData;
	long			iLen;
}CLIENT_CMD_DATA;

typedef struct _client_info_ex
{
	unsigned long	clientID;
	unsigned long	cmdNum;
	FILE_D			client;
}CLIENT_INFO_EX;

typedef CMyList <CLIENT_INFO_EX> CLIENT_FD_LIST;

class CWebServer
{
public:
	CWebServer();
	~CWebServer();

	static CWebServer *Instance();
	bool Initial (unsigned long videoNum, const char *pFileName = NULL);
	void Quit();

	bool Start(unsigned short nHttpPort = 80, unsigned short nStreamPort = 2000);
	void Stop();
	//////////////////////////////////////////////////////////////////////////
//	int GetRequestCmd(unsigned short &clientId, unsigned long &cmdId, unsigned char* &pParam, unsigned long &paramLen);
//	void ReleaseParam(unsigned char *pParam);
//	int SetResponse(unsigned short clientId, unsigned long cmdId, unsigned char *pParam, unsigned long paramLen);

//	int GetRequestCmd(CLIENT_CMD_DATA & cmdData);
//	void ReleaseParam(CLIENT_CMD_DATA & cmdData);
//	int SetResponse(CLIENT_CMD_DATA & cmdData);

	int ChangeServerHttpPort( unsigned short newHttpPort, unsigned short newDataPort = 6036 );
protected:
	int ParseURI(const char *pURI, char *pFileName, char *pArgs);
	bool CheckURLAuth(const char * pFileName);

	int GetFileType(char *pFileName, char *pFileType);
	
	int ReadRequestHdrs(FILE_D fd, RIO_READER &reader, HTTP_HEADER_INFO *pHeaderInfo = NULL);
	int ServeStatic(FILE_D fd, char *pFileName, int fileSize);
	int ServeNocontent(FILE_D fd);
	int ServeError(FILE_D fd, char *pCause, char *pErrNum, char *pShortMsg, char *pLongMsg);
	int ServeXml(FILE_D fd, const char *pXmlData);

	int RecvRequest(FILE_D fd, unsigned long clientId);
	int SendResponse(FILE_D fd, CLIENT_CMD_DATA &cmd);

	int ErrorLogOverviewProc(FILE_D fd);
//thread proc
#ifdef __ENVIRONMENT_LINUX__
	static RESULT WINAPI ListenThread(void *pParam);
	void	ListenProc();
#else
	static int ListenProc(CSWL_TCPLink *pTCPLink, void *pParam1, void *pParam2);
#endif

	static RESULT WINAPI ReceiveThread(void *pParam);
	static RESULT WINAPI SendThread(void *pParam);
	void ReceiveProc();
	void SendProc();
#ifdef __SUPPORT_WAP__
	static RESULT WINAPI GetJpegThread(void *pParam);
	void GetJpegProc();
	int ServeCgiJpeg(FILE_D fd, unsigned long channel);
	int ServerCgiVideoNumJs(FILE_D fd, unsigned long channel);
#endif

	void WriteLog(LOG_TYPE type, char *pInfo);
protected:
	bool		m_bStart;
	char *	m_pFileName;

	static CWebServer *m_pInstance;

#ifdef __ENVIRONMENT_LINUX__
	bool				m_bListenThd;
#endif
	bool				m_bRecvThd;
	bool				m_bSendThd;

#ifdef __ENVIRONMENT_LINUX__
	int					m_fdListen;
	PUB_thread_t		m_listenThreadId;
#else
	CSWL_ListenProc		*m_pListen;
#endif

	PUB_thread_t		m_recvThreadId;
	PUB_thread_t		m_sendThreadId;

	int					m_fdIndex;
	fd_set				m_fdReadSet;
	CLIENT_FD_LIST		m_clientList;
	CPUB_Lock			m_fdListLock;

	CMyList<CLIENT_CMD_DATA> m_ResponseCmdList;
	CPUB_Lock			m_CmdListLock;
	
	CPUB_Lock			m_langLock;

	unsigned short		m_httpPort;
	unsigned short		m_dataPort;

	unsigned long		m_videoNum;

#ifdef __SUPPORT_WAP__
	CFrameData			**m_ppJpegFrame;
	PUB_thread_t		m_getJpegThread;
	bool				m_bGettingJpeg;
	CPUB_Lock			m_jpegLock;
	unsigned long		m_monitorChannel;
#endif
};

#endif // _WEBSERVER_H_
