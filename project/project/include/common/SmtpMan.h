#ifndef         __SMTP_MAN__
#define         __SMTP_MAN__
/*
smtpЭ�鿴 RFC2821
MIME��ظ�ʽ �ο� RFC2045 2046 2047 2048 2049
*/

#include "Product.h"
#include "FrameData.h"	
#include "PUB_common.h"
#include "SWL_Public.h"
#include "NetInterface.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

//Smtp potocol
#define STRINGLEN        68
#define MAXDATASIZE     2048
#define SMTP_TIMEOUT   8000                            // �ӳ�
#define PORTNUM           8

//Email total memory

const int ALARM_EMAIL_TEXT_CONTENT_LENGTH = 5*1024;      // �ı�
//һ��ͼƬ���Ϊ20K��20K���ʼ�������Ŀռ�Ϊ20K*4/3,��Լ��27K
//3531��һ��ͼƬ���Ϊ512K������ֻ��3531SDIץ�ĸ���ͼƬ���ýϴ�ռ�
#if defined(__CHIP3531__) || defined(__CHIPGM__) 
	#if defined(__SUPPORT_SNAPD1__)		//3531D1ץͼ��16·ʹ�õ��ڴ��СΪ6M��32·ʹ�õ��ڴ��СΪ12M
		const int JPEG_MAX_LENGTH = 96 * 1024;
		const int ALARM_EMAIL_PICTURE_CONTENT_LENGTH = (ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT * 128 * 1024); //
	#elif defined(__SUPPORT_HONGDI__)	//3531SDIץͼ��16·ʹ�õ��ڴ��СΪ32M��32·ʹ�õ��ڴ��СΪ64M
		const int JPEG_MAX_LENGTH = 512 * 1024;
		const int ALARM_EMAIL_PICTURE_CONTENT_LENGTH = (ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT * 684 * 1024); //
	#else								//����3531��֧��D1ץͼ��16·ʹ�õ��ڴ��СΪ6M��32·ʹ�õ��ڴ��СΪ12M
		const int JPEG_MAX_LENGTH = 96 * 1024;
		const int ALARM_EMAIL_PICTURE_CONTENT_LENGTH = (ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT * 128 * 1024); //
	#endif
#elif defined(__DVR_ULTIMATE__)
	const int JPEG_MAX_LENGTH = 96 * 1024;
	const int ALARM_EMAIL_PICTURE_CONTENT_LENGTH = (ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT * 128 * 1024); //
#else
	const int JPEG_MAX_LENGTH = 20*1024;
	const int ALARM_EMAIL_PICTURE_CONTENT_LENGTH = (ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT*28*1024); //
#endif
//const int ALARM_EMAIL_LENGTH = ALARM_EMAIL_PICTURE_CONTENT_LENGTH+ALARM_EMAIL_TEXT_CONTENT_LENGTH;    // �����ʼ��ռ�

//SASL authentication mechanism
const unsigned long SASL_LOGIN       = 0;              // AUTH LOGIN SASL mechanism
const unsigned long SASL_PLAIN       = 1;              // AUTH PLAIN SASL mechanism
//const unsigned long SASL_CRAMMD5   = 2;              // AUTH CRAM-MD5 SASL mechanism

//SMTP server response code
#define SERVICE_READY                "220"             // <domain> Service ready
#define REPLY_OKAY                   "250"             // Requested mail action okay, completed

//�������Զ����MIME  �ż��߽�
static const char MAIL_BOUNDARY1[] = {"----=_NextPart_000_0049_01C9AB9E.FB4D2810"};
static const char MAIL_BOUNDARY2[] = {"----=_NextPart_000_003D_01C9AB9E.C75EEFC0"};


//�ʼ������ڵ�
typedef struct _email_alarm_info
{
	ALARM_TYPE alarmType;
	char szTime[STRINGLEN];
}EMAIL_ALARM_INFO;

typedef struct _email_jpeg_info
{
	unsigned char *pData;
	unsigned long length;
	LONGLONG      time;	
}EMAIL_JPEG_INFO;

typedef struct _email_ch_jpeg_info
{
	int                        channel;
	int                        bufferIndex;
	int                        curDataLength;
	int                        snapCount;
	CMyList <EMAIL_JPEG_INFO>  *pJPEGInfo;
}EMAIL_CH_JPEG_INFO;

typedef struct _Email_Info
{
	int                          id;
    unsigned long                chn;
    char                         szChannelName[STRINGLEN];
	CMyList <EMAIL_ALARM_INFO>   *pAlarmInfoList;
	CMyList <EMAIL_CH_JPEG_INFO> *pJPEGInfoList;
}EMAIL_INFO;


typedef struct _email_buffer
{
	bool bUse;
	unsigned char *pDataBuff;
}EMAIL_BUFFER;

class CSnapProc
{
public:	

	//
	~CSnapProc();

	//
	static CSnapProc *Instance();

	//
	bool Initial();
	void Quit();
	void Action(bool bEnable);

	//
	bool Start();
	void Stop();

	//
	void SnapJPEGStream(CFrameData *pFrameData);

	//
	void SendEmail(unsigned long channel, const char *pszChannelName,
		           ALARM_TYPE alarmType, char *pszTime, ULONGLONG toSnapCH);

	//
	void SetSnapTimeS(const unsigned long timeS);

	//
	void SetSnapImgNum(unsigned long imgNum);

	//
	void SetSnapImg(unsigned long bSnapImg);

	//
	void ReleaseBuffer(int bufferIndex);

	//
	void RemoveAllData();

	//
	LONGLONG GetSendEmailPeriod();

protected:

private:
    
	//
	CSnapProc();

	//
	//static RESULT WINAPI  SnapJPEGStreamThread(void* lpParameter);   	
	//void SnapJPEGStream();

	//
	int UseEmailBuffer();

	//
	static CSnapProc    *m_pInstance;

	//
	bool                m_bAction;

	//
	//bool	            m_bSnapStream;				
	//PUB_thread_t        m_SnapStreamID;

	//	
	int                           m_channelNum;
	int                           m_snapImgNum;
	bool                          m_bSnapImg;
	int                           m_snapPeriod;
	int                           *m_pSnapPeriodChnn;
	LONGLONG                      *m_pLastJPEGTime;

	//
	int                           m_DataBufferNum;
	EMAIL_BUFFER                  *m_pDataBuffer;

	//
	CMyList <EMAIL_INFO>          m_EmailInfoList;
	CMyList <int>                 *m_pChnnSnaptoEmailIdList;
	
	//                 *m_pSnapChnnStatus;	
	//int                 *m_pSnapChnnCount;
	

	//EMAIL_INFO          *m_pEmailInfo;
	
	//
	CPUB_Lock           m_SnapLock;
};


class CSmtpMan
{
public:
	//
    ~CSmtpMan();

	//
    static CSmtpMan *Instance();

	//
    bool Initial();
    void Quit();
	void Action(bool bEnable);
	void ChangeLanguage();

	//
    bool Start();
    void Stop();

	//
    void SetDeviceID(unsigned long DeviceID);
    void SetDeviceName(const char *pDeviceName);

	//
	void SetSendPara(const MAIL_SEND_INFO *pSendInfo);
	void GetSendPara(MAIL_SEND_INFO *pSendInfo);
    void SetReceiverPara(const MAIL_RECV_INFO *pReceviceInfo, int num);
	void GetReceiverPara(MAIL_RECV_INFO *pReceviceInfo, int *pRetNum);
	
	//
    int  TestSendEmailProcess(ALARM_TYPE alarmType, char *pszTime);
    void SendEmailProcess(EMAIL_INFO EmailInfo);
protected:
private:
	typedef struct _send_use_info 
	{
		unsigned char  *pOutBuff;  //�ʼ�����
		int            len;
		BIO            *pBio;
		SSL_CTX        *pCtx;
		SSL            *pSsl;
		SWL_socket_t   sock;             		 //����socket
		unsigned long  Authmethod;
		MAIL_SEND_INFO *pSendInfo; 
		CMyList<MAIL_RECV_INFO> *pToAddrList;
	}SEND_USE_INFO;

	//
    CSmtpMan();

	//
	static RESULT WINAPI  SendMailThread(void* lpParameter);
	void SendMailThreadProcess( );

	//
	bool SendEmailProcessImp(SEND_USE_INFO &sendInfo);	
    bool ConnectMailServer( SEND_USE_INFO &sendInfo);   
    bool SmtpReady(SEND_USE_INFO &sendInfo );
    bool SmtpEhlo(SEND_USE_INFO &sendInfo );
    bool SmtpAuth(SEND_USE_INFO &sendInfo );
    bool SmtpFrom(SEND_USE_INFO &sendInfo );
    bool SmtpRcpt(SEND_USE_INFO &sendInfo );
    bool SmtpData(SEND_USE_INFO &sendInfo );
    bool SmtpContent(SEND_USE_INFO &sendInfo);
    bool SmtpQuit(SEND_USE_INFO &sendInfo );
    void SmtpRelease(SEND_USE_INFO &sendInfo );	
	int SmtpRead(char *pBuf, int iLen, SEND_USE_INFO &sendInfo);
	int SmtpSend(const char *pBuf, int iLen, SEND_USE_INFO &sendInfo);
	
	//
	int SmtpEmailContent(CMyList<EMAIL_INFO> *pEmailInfoList, int Num, char *pOutBuff);
	int to64frombits(unsigned char *out, const unsigned char *in, int inlen, bool bAttach=false);
	int InitSSL();
	bool CheckSendInfo();
	void ReleaseEmailInfo(EMAIL_INFO *pEmailInfo);
	void GetAlarmTypeName(ALARM_TYPE alarmType, char *szName, int len);
	bool CheckReplyEnd(const char *pBuff, int iLen);

	//
    static CSmtpMan             *m_pInstance;

	//
	bool                        m_bAction;
    
	//
	bool                        m_bSendMailProc;
    PUB_thread_t                m_sendMailProc_ID;      		 //�߳�ID

	//
	char                        m_DeviceName[STRINGLEN];
	unsigned long               m_DeveiceID;
    MAIL_SEND_INFO              *m_pSendInfo;    
    CMyList<MAIL_RECV_INFO>     m_ToAddrList;
	bool                        m_bCheckSendInfo;
	CMyList<EMAIL_INFO>         m_EmailList;

	//�������ʼ�����������ַ�.(Ŀ����֧�֣��ʼ��ö������д)
	char                        m_subjectDsp1[STRINGLEN]; //Alarm Message From Device %s
	char                        m_subjectDsp2[STRINGLEN]; //Test Email Message From Device %s
	char                        m_deviceIDDsp[32]; //�豸��� ������
    char                        m_deviceNameDsp[32]; //�豸���� ������
	char                        m_channelIDDsp[32]; //
	char                        m_channelNameDsp[32]; //
	char                        m_alarmTypeDsp[32];
	char                        m_alarmTimeDsp[32];
	char                        m_alarmTypeMotion[32];
	char                        m_alarmTypeSensor[32];
	char                        m_alarmTypeLoss[32];
	char                        m_alarmTypeTest[32];
	char                        m_alarmTypeDiskFull[32];
	char                        m_alarmTypeIpConflict[32];
	char                        m_alarmTypeDisconnet[32];
	char                        m_alarmTypeDiskError[32];
	char                        m_alarmTypeDiskDisConnect[32];
	char                        m_alarmTypeOther[32];
	char                        m_sensorAlarmTypeName[32];

	//
	CPUB_Lock                   m_lock;	
};

#endif




