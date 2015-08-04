#ifndef __FTP_LOAD_STREAM__
#define __FTP_LOAD_STREAM__

#include "FtpLoad.h"

class CFtpLoadStream : public CFtpLoad
{
public:
	enum __ftp_put_stream_buff_size__
	{
		FTP_PUT_STREAM_BUFF_SIZE = 1024*512,
		TCP_SEND_SINGLE_LENGTH = 1024*50,//ÿ��TCP���͵�����
	};
	CFtpLoadStream(unsigned int buffLen = FTP_PUT_STREAM_BUFF_SIZE);
	~CFtpLoadStream();
	bool Start();
	void Stop();
	bool IsOpen();
	void SetClose();
	int FOpen(const char* pFileName);
	int FWrite(const char *pData, unsigned int Length);
	int	SendData(const char *pData, unsigned long length);//ֻ�ǰ����ݷ���buff

protected:
	void FtpDataProc();
	static	PUB_THREAD_RESULT PUB_THREAD_CALL FtpDataProcThread(void *pParam);
	int		RealSend();
	int		RealClose();

private:
	typedef enum _ftp_load_stream_status_
	{
		FTP_LOAD_STREAM_STATUS_CLOSED,		//�ر�״̬������ȥ��
		FTP_LOAD_STREAM_STATUS_OPENNING,	//�򿪹�����
		FTP_LOAD_STREAM_STATUS_OPENED,		//��״̬������д���ݣ����Թر�
		FTP_LOAD_STREAM_STATUS_WRITE_ERROR,	//д���ݴ��󣬲�����д���ݣ����ǿ��Թر�
	}FTP_LOAD_STREAM_STATUS;
private:
	FTP_LOAD_STREAM_STATUS	m_ftpLoadStatus;
	bool					m_bClose;
	char					m_CurFileName[256];

	bool					m_bLogOut;
	bool					m_bIsTransferOver;

	unsigned long			m_CurBufPos;//��ǰ������������ʼλ��
	unsigned long			m_TotalLength;//δ���������ܳ���
	unsigned long			m_BufferLen;//buffer�ܴ�С
	char					*m_SendBuf;//��ѭ��ʹ�ã��൱�ڶ���
};

#endif
