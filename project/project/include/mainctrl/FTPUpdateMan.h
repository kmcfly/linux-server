/***********************************************************************
** ����ͬΪ����Ƽ����޹�˾��������Ȩ����
** ����			: huangxunliang
** ����         : 2012-09-24
** ����         : FTPUpdateMan.h
** �汾��		: 1.0
** ����			:
** �޸ļ�¼		:
***********************************************************************/
#ifndef __FTP_UPDATE_MAN_H__
#define __FTP_UPDATE_MAN_H__

#include "Schedule.h"
#include "FrameData.h"

//ftp�ϴ�����
typedef struct __ftp_upload_config_
{
	unsigned long	uploadCamNum;							//����֧���ϴ���Ƶ��ͨ������(ֻ��)
	unsigned long	maxChBufLen;							//һ��ͨ���������󻺴泤��
	struct  
	{
		unsigned short  enableUploadCam;  //��Щͨ�������ϴ���Ƶ;1-������0-������
		unsigned short  enableSchedule;   //�Ƿ��ϴ���Ƶͨ���ļƻ�
		WEEK_SCHEDULE	uploadSchedule;   //�ϴ���Ƶͨ���ļƻ�
	}uploadVideo[TVT_MAX_CAMERA_NUM];
}FTP_UPLOAD_CONFIG;

const unsigned long	ONE_HOUR_SECONDS	= 60 * 60;
const unsigned long UPLOAD_BUF_LEN		= 512 * 1024;
const unsigned long UPLOAD_PIC_BUF_LEN	= 512 * 1024;

const unsigned long UPLOAD_MP4_MAX_FILE_TIME = 60 * 10;		//mp4�ļ�����ϴ�ʱ��Ϊ10����
class CMp4File;

class CFtpUpdateMan
{
public:
	static CFtpUpdateMan * Instance();

	~CFtpUpdateMan();

	bool Initial(unsigned long ftpVideoBufLen);
	void Quit();

	void ClearStream();

	//�����ϴ��Ĳ���
	void SetupFtpUpdateInfo(const TVT_FTP_CONFIG * pTVTFtpConfig);
	//��������֡
	void PutFrame(CFrameData *pFrameData);

	//����ͨ��chnn���ϴ�
	bool StartUpdate(unsigned long chnn, RECORD_TYPE recType);
	//ֹͣͨ��chnn���ϴ�
	void StopUpdate(unsigned long chnn, RECORD_TYPE recType);

	//��ͣ��ǰ����
	void Action(bool bAction);

private:
	CFtpUpdateMan();

	typedef struct __ftp_thread_info__
	{
		bool			bTest;					//��⴫��״̬
		int				recType;				//ftp�ϴ��ļ�����
		bool			bUpload;				//�ϴ��߳̿���
		PUB_thread_t	upload_ID;				//�߳�ID
		unsigned long	usedBuflen;				//ÿ��ͨ���Ѿ���������ݳ���
		unsigned long	videoFileLen;
		CFtpUpdateMan * pFtpUpdateMan;
		unsigned long	bOpenFile;				//�Ƿ񴴽����ļ�
		CPUB_Lock		updateThreadLock;
		std::list<CFrameData*>	threadFrameList;
		unsigned short  enableUploadCam;		//��Щͨ�������ϴ���Ƶ;1-������0-������
		unsigned short  enableSchedule;			//�Ƿ��ϴ���Ƶͨ���ļƻ�
		WEEK_SCHEDULE	uploadSchedule;			//�ϴ���Ƶͨ���ļƻ�
		CSchedule		updateSchedule;			//ftp�ϴ��ƻ���
		unsigned long	ftpVideoBufLen;		
		unsigned long	needKeyFrame;			//��Ҫ�ؼ�֡
		unsigned long	channel;
		unsigned long	fileStartTime;			//�ϴ��ĵ�һ֡ͼƬʱ��
		CMp4File	  * pMp4File;				//mp4�ļ�
	}FTP_THREAD_INFO;

	void ExitTread(unsigned long chnn);

	static PUB_THREAD_RESULT PUB_THREAD_CALL UpdateProcThread(void *lpParameter);
	//�ϴ���Ƶ���߳�
	void UpdateProc(FTP_THREAD_INFO * pFtpThreadInfo);

	//�����ļ�������·��������
	void CreateFileName(char *fileName, unsigned long chnn);
	//����������������ݵ��ؼ�֡
	void DropFrame(FTP_THREAD_INFO * pFtpThreadInfo);
	//������������
	void ClearFrame(unsigned long chnn);
	void ClearFrame(FTP_THREAD_INFO * pFtpThreadInfo);

	//����Ƿ����ų���
	bool CheckSchedule(FTP_THREAD_INFO * pFtpThreadInfo);

	//�򿪺͹ر�MP4�ļ�
	bool OpenMp4File(FTP_THREAD_INFO * pFtpThreadInfo);
	bool CloseMp4File(FTP_THREAD_INFO * pFtpThreadInfo);
	bool WriteMp4File(FTP_THREAD_INFO * pFtpThreadInfo, FRAME_INFO_EX * pFrameInfo);

	FTP_THREAD_INFO		m_ftpThreadInfo[TVT_MAX_CAMERA_NUM];

	static unsigned long m_sUploadMp4FileLen;

	typedef enum _update_status_
	{
		UPDATE_STATUS_NONE,			//����״̬
		UPDATE_STATUS_POUSE,		//��ͣ״̬
		UPDATE_STATUS_POUSE_DELAY,	//��ͣ��ʱ̬
	}UPDATE_STATUS;
	UPDATE_STATUS	m_updateStauts;
};
#endif //__FTP_UPDATE_MAN_H__

