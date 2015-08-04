//////////////////////////////////////////////////////////////////////////
#ifndef _FTP_DATA_PROC_H_
#define _FTP_DATA_PROC_H_

#include "TVTSock.h"
#include "FtpType.h"

//FTP数据处理类型
typedef enum _ftp_proc_type_
{
	FTP_PROC_BINARY_CMD			= 0,		//处理命令请求的应答，这些应答是二进制形式的
	FTP_PROC_BINARY_FILE_SEND	= 1,		//处理文件发送，数据格式为二进制形式
	FTP_PROC_BINARY_FILE_RECV	= 2,		//处理文件接收，数据格式为二进制形式
}FTP_PROC_TYPE;


class CFtpDataProc
{
public:
	CFtpDataProc();
	~CFtpDataProc();

	virtual bool Initial(const char *pParam = NULL);
	virtual void Quit() = 0;
	virtual bool Proc() = 0;

	void SetSock(CTVTSock *pSock){m_pSock = pSock;}
	
protected:

	bool			m_bHasInitial;
	CTVTSock	*	m_pSock;

	char			m_dataBuf[4096];			//接收和发送缓存
	unsigned long	m_dataLen;					//接收时表示接收到的数据长度，发送时要发送的数据长度
	unsigned long	m_totalProcDataLen;			//表示当前处理的数据长度，发送文件时表示发送了多少数据到网络，接收文件时表示当前已经接收了多少数据
												//LS命令时表示已经收到的回复数据的长度
};

//二进文件发送
class CFtpBFileSendProc : public CFtpDataProc
{
public:
	CFtpBFileSendProc();
	~CFtpBFileSendProc();

	bool Initial(const char *pParam=NULL);
	void Quit();

	bool Proc();

private:
	bool GetSendData();

private:
	FILE		* 	m_pFile;

	unsigned long	m_sendPos;
};

//二进文件接收
class CFtpBFileRecvProc : public CFtpDataProc
{
public:
	CFtpBFileRecvProc();
	~CFtpBFileRecvProc();

	bool Initial(const char *pParam=NULL);
	void Quit();

	bool Proc();

private:

	FILE		*	m_pFile;
};

//命令的响应
class CFtpBCmdResponseProc : public CFtpDataProc
{
public:
	CFtpBCmdResponseProc();
	~CFtpBCmdResponseProc();

	bool Initial(const char *pParam=NULL);
	void Quit();

	bool Proc();
	void GetResponseData(std::list<char *> &infoList);
private:

	std::list<char *>	m_responseList;
};


#endif  /*_FTP_DATA_PROC_H_*/
//////////////////////////////////////////////////////////////////////////
