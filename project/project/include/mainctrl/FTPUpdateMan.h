/***********************************************************************
** 深圳同为数码科技有限公司保留所有权利。
** 作者			: huangxunliang
** 日期         : 2012-09-24
** 名称         : FTPUpdateMan.h
** 版本号		: 1.0
** 描述			:
** 修改记录		:
***********************************************************************/
#ifndef __FTP_UPDATE_MAN_H__
#define __FTP_UPDATE_MAN_H__

#include "Schedule.h"
#include "FrameData.h"

//ftp上传策略
typedef struct __ftp_upload_config_
{
	unsigned long	uploadCamNum;							//机器支持上传视频的通道个数(只读)
	unsigned long	maxChBufLen;							//一个通道分配的最大缓存长度
	struct  
	{
		unsigned short  enableUploadCam;  //哪些通道启动上传视频;1-启动，0-不启动
		unsigned short  enableSchedule;   //是否上传视频通道的计划
		WEEK_SCHEDULE	uploadSchedule;   //上传视频通道的计划
	}uploadVideo[TVT_MAX_CAMERA_NUM];
}FTP_UPLOAD_CONFIG;

const unsigned long	ONE_HOUR_SECONDS	= 60 * 60;
const unsigned long UPLOAD_BUF_LEN		= 512 * 1024;
const unsigned long UPLOAD_PIC_BUF_LEN	= 512 * 1024;

const unsigned long UPLOAD_MP4_MAX_FILE_TIME = 60 * 10;		//mp4文件最大上传时间为10分钟
class CMp4File;

class CFtpUpdateMan
{
public:
	static CFtpUpdateMan * Instance();

	~CFtpUpdateMan();

	bool Initial(unsigned long ftpVideoBufLen);
	void Quit();

	void ClearStream();

	//设置上传的参数
	void SetupFtpUpdateInfo(const TVT_FTP_CONFIG * pTVTFtpConfig);
	//送入数据帧
	void PutFrame(CFrameData *pFrameData);

	//开启通道chnn的上传
	bool StartUpdate(unsigned long chnn, RECORD_TYPE recType);
	//停止通道chnn的上传
	void StopUpdate(unsigned long chnn, RECORD_TYPE recType);

	//暂停当前传输
	void Action(bool bAction);

private:
	CFtpUpdateMan();

	typedef struct __ftp_thread_info__
	{
		bool			bTest;					//检测传输状态
		int				recType;				//ftp上传文件类型
		bool			bUpload;				//上传线程控制
		PUB_thread_t	upload_ID;				//线程ID
		unsigned long	usedBuflen;				//每个通道已经缓存的数据长度
		unsigned long	videoFileLen;
		CFtpUpdateMan * pFtpUpdateMan;
		unsigned long	bOpenFile;				//是否创建了文件
		CPUB_Lock		updateThreadLock;
		std::list<CFrameData*>	threadFrameList;
		unsigned short  enableUploadCam;		//哪些通道启动上传视频;1-启动，0-不启动
		unsigned short  enableSchedule;			//是否上传视频通道的计划
		WEEK_SCHEDULE	uploadSchedule;			//上传视频通道的计划
		CSchedule		updateSchedule;			//ftp上传计划类
		unsigned long	ftpVideoBufLen;		
		unsigned long	needKeyFrame;			//需要关键帧
		unsigned long	channel;
		unsigned long	fileStartTime;			//上传的第一帧图片时间
		CMp4File	  * pMp4File;				//mp4文件
	}FTP_THREAD_INFO;

	void ExitTread(unsigned long chnn);

	static PUB_THREAD_RESULT PUB_THREAD_CALL UpdateProcThread(void *lpParameter);
	//上传视频主线程
	void UpdateProc(FTP_THREAD_INFO * pFtpThreadInfo);

	//创建文件的完整路径的名字
	void CreateFileName(char *fileName, unsigned long chnn);
	//缓冲区满了清除数据到关键帧
	void DropFrame(FTP_THREAD_INFO * pFtpThreadInfo);
	//清除缓存的数据
	void ClearFrame(unsigned long chnn);
	void ClearFrame(FTP_THREAD_INFO * pFtpThreadInfo);

	//检查是否在排程内
	bool CheckSchedule(FTP_THREAD_INFO * pFtpThreadInfo);

	//打开和关闭MP4文件
	bool OpenMp4File(FTP_THREAD_INFO * pFtpThreadInfo);
	bool CloseMp4File(FTP_THREAD_INFO * pFtpThreadInfo);
	bool WriteMp4File(FTP_THREAD_INFO * pFtpThreadInfo, FRAME_INFO_EX * pFrameInfo);

	FTP_THREAD_INFO		m_ftpThreadInfo[TVT_MAX_CAMERA_NUM];

	static unsigned long m_sUploadMp4FileLen;

	typedef enum _update_status_
	{
		UPDATE_STATUS_NONE,			//正常状态
		UPDATE_STATUS_POUSE,		//暂停状态
		UPDATE_STATUS_POUSE_DELAY,	//暂停延时态
	}UPDATE_STATUS;
	UPDATE_STATUS	m_updateStauts;
};
#endif //__FTP_UPDATE_MAN_H__

