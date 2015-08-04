#ifndef __FTP_LOAD__
#define __FTP_LOAD__

#include "TVTSock.h"
#include "FtpType.h"
#include "FtpPI.h"
#include "FrameData.h"
#include "SSLClient.h"


 typedef struct _tvt_ftp_load_info_
 {
 	char CurAccount[TVT_FTP_MAX_ACCOUNT_LEN];
 	char CurPassWd[TVT_FTP_MAX_PASSWORD_LEN];
 	char CurServerURL[TVT_FTP_MAX_URL_LEN];
 	char CurRemotePath[TVT_FTP_MAX_URL_LEN];
 	bool bIsUploading;
 	bool bIsLogin;
 
 }TVT_FTP_LOAD_INFO;

class CFtpLoad
{
public:
	CFtpLoad();
	~CFtpLoad();

	bool SetServerInfo(const TVT_FTP_CONFIG &FtpConfig);

protected:
	int	CreateSock(char *pIP, unsigned short port);
	void CloseSock();

	bool GetIpAndPort(char *pIP, unsigned short &port, CFtpPI *pFtpPI);
	bool ParseFileName(const char *pFileName, char *pNewFileName, CFtpPI *pFtpPI);//�����ļ����������ļ��к��л�Ŀ¼
	char* GetDirName(char *pFileName, char *pDirName);
	bool CheckRemotePath(char *RemotePath, unsigned long Len);
	bool HandleObliqueLine(char *RemotePath, int &Len);

	bool MakeDir(CFtpPI *pFtpPI, const char *pPath, char *pFileName = NULL);

	void LongSleep(const bool &bThreadSign, unsigned long Second);
protected:
//�������
	CTVTSock					*m_pDataSock;
	CSSLClient					*m_pSSLClient;
	int							m_SockfdMax;
	fd_set						m_FdSet;

	CFtpPI						m_FtpPI;

//�߳����
	PUB_thread_t				m_ftpDataProcID;
	bool						m_bFtpDataProc;

	CPUB_Lock					m_Lock;

//��¼���
	TVT_FTP_CONFIG				m_FTP_Config;

//������
	bool						m_bReLogIn;
	TVT_FTP_LOAD_INFO			m_LoadInfo;
};

#endif