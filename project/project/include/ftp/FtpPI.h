////////////////////////////////////////////////////////////////////
//
/*
*FTP Э������� protocol interpreter
*�������������������������FTP�����������������Ӧ��
*����Ӧ�����ݵĲ�ͬ������������
*1:��ͨ�����Ӧ��Ϣ�����������׽ӿ��ϣ���Ϣ�Ƚ϶̣�ֻ����֪ͨ���������Ĳ����Ƿ�ɹ�
*2:������Ӧ��������Щ�����Ӧ�����ݵĳ����ǲ��̶��ģ�����Ŀ¼������������᷵�ص�ǰĿ¼�е��ļ�������Ϣ
*3:�ļ����������Щ�������������ļ��ϴ������ز���
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
	
	//��¼���ǳ�ftp������
	bool	FtpLogIn(const char *pHost, unsigned short port = FTP_SERVER_PORT, const char *pUser = FTP_ANONYMOUS_NAME, const char *pPas = FTP_ANONYMOUS_PASS, bool bUseSSL = false);
	void	FtpLogOut();

	bool	Ls(std::list<char *> &infoList);			//�鿴��ǰĿ¼�е��ļ���Ŀ¼����Ϣ
	bool	Cd(const char *pDir);						//�л���ǰ����Ŀ¼
	bool	PWD();
	//
	bool	Rm(const char *pFileName);					//ɾ���ļ�
	bool	MkDir(const char *pDirName);				//����Ŀ¼
	bool	RmDir(const char *pDirName);				//ɾ��Ŀ¼

	//
	bool	Put(const char *pFileName);					//�ϴ��ļ�
	//add a parameter
	bool	Get(const char *pFileName, const char *pLocalFilePath);					//�����ļ�
	
	bool	IsFileTransOver();							//����ļ��Ƿ������
	
	bool	FileNameExtract(char *pFileName);
	bool	MLSD(const char* pDirName, std::list<char *> &infoList);
	bool	IsLogIn();

	unsigned long	GetReplayCode();					//����ʧ��ʱ���������صĴ�����
	const char *	GetReplayInfo();					//����ʧ��ʱ���������صĴ�����Ϣ

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
