//////////////////////////////////////////////////////////////////////////
#ifndef _FTP_DATA_PROC_H_
#define _FTP_DATA_PROC_H_

#include "TVTSock.h"
#include "FtpType.h"

//FTP���ݴ�������
typedef enum _ftp_proc_type_
{
	FTP_PROC_BINARY_CMD			= 0,		//�������������Ӧ����ЩӦ���Ƕ�������ʽ��
	FTP_PROC_BINARY_FILE_SEND	= 1,		//�����ļ����ͣ����ݸ�ʽΪ��������ʽ
	FTP_PROC_BINARY_FILE_RECV	= 2,		//�����ļ����գ����ݸ�ʽΪ��������ʽ
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

	char			m_dataBuf[4096];			//���պͷ��ͻ���
	unsigned long	m_dataLen;					//����ʱ��ʾ���յ������ݳ��ȣ�����ʱҪ���͵����ݳ���
	unsigned long	m_totalProcDataLen;			//��ʾ��ǰ��������ݳ��ȣ������ļ�ʱ��ʾ�����˶������ݵ����磬�����ļ�ʱ��ʾ��ǰ�Ѿ������˶�������
												//LS����ʱ��ʾ�Ѿ��յ��Ļظ����ݵĳ���
};

//�����ļ�����
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

//�����ļ�����
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

//�������Ӧ
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
