#ifndef __FTP_LOAD_STREAM__
#define __FTP_LOAD_STREAM__

#include "FtpLoad.h"

class CFtpLoadStream : public CFtpLoad
{
public:
	enum __ftp_put_stream_buff_size__
	{
		FTP_PUT_STREAM_BUFF_SIZE = 1024*512,
		TCP_SEND_SINGLE_LENGTH = 1024*50,//每次TCP发送的数量
	};
	CFtpLoadStream(unsigned int buffLen = FTP_PUT_STREAM_BUFF_SIZE);
	~CFtpLoadStream();
	bool Start();
	void Stop();
	bool IsOpen();
	void SetClose();
	int FOpen(const char* pFileName);
	int FWrite(const char *pData, unsigned int Length);
	int	SendData(const char *pData, unsigned long length);//只是把数据放入buff

protected:
	void FtpDataProc();
	static	PUB_THREAD_RESULT PUB_THREAD_CALL FtpDataProcThread(void *pParam);
	int		RealSend();
	int		RealClose();

private:
	typedef enum _ftp_load_stream_status_
	{
		FTP_LOAD_STREAM_STATUS_CLOSED,		//关闭状态，可以去打开
		FTP_LOAD_STREAM_STATUS_OPENNING,	//打开过程中
		FTP_LOAD_STREAM_STATUS_OPENED,		//打开状态，可以写数据，可以关闭
		FTP_LOAD_STREAM_STATUS_WRITE_ERROR,	//写数据错误，不能再写数据，但是可以关闭
	}FTP_LOAD_STREAM_STATUS;
private:
	FTP_LOAD_STREAM_STATUS	m_ftpLoadStatus;
	bool					m_bClose;
	char					m_CurFileName[256];

	bool					m_bLogOut;
	bool					m_bIsTransferOver;

	unsigned long			m_CurBufPos;//当前待发的数据起始位置
	unsigned long			m_TotalLength;//未发的数据总长度
	unsigned long			m_BufferLen;//buffer总大小
	char					*m_SendBuf;//可循环使用，相当于队列
};

#endif
