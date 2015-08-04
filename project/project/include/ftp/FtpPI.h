////////////////////////////////////////////////////////////////////
//
/*
*FTP 协议解释器 protocol interpreter
*用来解析请求命令，并发送请求到FTP服务器、接收命令的应答
*根据应答数据的不同，有三种命令
*1:普通命令，回应消息在命令连接套接口上，消息比较短，只有是通知请求端请求的操作是否成功
*2:有数据应答的命令，这些命令的应答数据的长度是不固定的，比如目录内容请求命令，会返回当前目录中的文件属性信息
*3:文件请求命令，这些命令用来进行文件上传、下载操作
*/
#ifndef _FTP_PI_H_
#define _FTP_PI_H_

#include "TVTSock.h"
#include "FtpDTP.h"
#include "FtpType.h"
#include "FtpDataProc.h"
#include "SSLControl.h"

class CFtpPI
{
public:
	CFtpPI();
	~CFtpPI();
	
	//登录、登出ftp服务器
	bool	FtpLogIn(const char *pHost, unsigned short port = FTP_SERVER_PORT, const char *pUser = FTP_ANONYMOUS_NAME, const char *pPas = FTP_ANONYMOUS_PASS, bool bUseSSL = false);
	void	FtpLogOut();

	bool	Ls(std::list<char *> &infoList);			//查看当前目录中的文件、目录的信息
	bool	Cd(const char *pDir);						//切换当前工作目录
	bool	PWD();
	//
	bool	Rm(const char *pFileName);					//删除文件
	bool	MkDir(const char *pDirName);				//创建目录
	bool	RmDir(const char *pDirName);				//删除目录

	//
	bool	Put(const char *pFileName);					//上传文件
	//add a parameter
	bool	Get(const char *pFileName, const char *pLocalFilePath);					//下载文件
	
	bool	IsFileTransOver();							//检查文件是否传输结束
	
	bool	FileNameExtract(char *pFileName);
	bool	MLSD(const char* pDirName, std::list<char *> &infoList);
	bool	IsLogIn();

	unsigned long	GetReplayCode();					//请求失败时服务器返回的错误码
	const char *	GetReplayInfo();					//请求失败时服务器返回的错误信息

	SWL_socket_t GetSock(){return m_pControlSock->GetSocket();}
	int		FTPCmd(const char *pCmd, const char *pArg=NULL);
	bool	IsRequestOK();

	int		CheckResponse(const char *pCmd = NULL);

	void	DestroyDTP();
	inline	void	DestoryDataProc();
private:	
	
	bool	CreateDTP(CFtpDataProc *pFtpDataProc);
	
	const char * RecvMsg(unsigned long &recvLen);


	inline	CFtpDataProc *CreateDataProc(FTP_PROC_TYPE procType, const char *pParam=NULL);
	

	bool StartFtpDtp();
private:
	bool				m_bIsUploading;
	bool				m_bIsDownloading;
	bool				m_bIsLogin;
	CTVTSock		*	m_pControlSock;
	bool				m_bUseSSL;
	CSSLClient		*	m_pSSLClient;
	CFtpDTP			*	m_pFtpDTP;
	CFtpDataProc	*	m_pDataProc;

	char				m_recvBuf[MSG_BUF_LEN];
	unsigned long		m_recvLen;

	unsigned long		m_errCode;
	char				m_errText[MSG_BUF_LEN];

};
#endif /*_FTP_PI_H_*/

////////////////////////////////////////////////////////////////////
