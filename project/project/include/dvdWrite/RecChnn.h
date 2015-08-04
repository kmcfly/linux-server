#ifndef __REC_CHNN_H__
#define __REC_CHNN_H__

#include "FileManager.h"
#include "FrameData.h"
#include "RecWriter.h"
#include "LocalDevice.h"


typedef enum _rec_chnn_fail_
{
	GET_VIDEO_AUDIO_FORMAT_FAIL = 0x0010,    //获取音视频格式失败
	SWITCH_REC_FILE_FAIL,                    //切换录像文件失败
	NOT_KEY_FRAME_OF_FIRST,                  //写文件头时发现非关键帧
	OPEN_FILE_FAIL,                          //打开文件失败(RecWrite)
}REC_CHNN_FAIL;

class CRecChnn
{
public:
	CRecChnn(void);
	virtual ~CRecChnn(void);

	bool Initial(long chnnNum, unsigned long fileSize, CFileManager* pFileManager);
	void Quit();

	void StartRec();
	void StopRec();

	void Active(bool bActive);

	void SetEnable(bool bEnable = false);

	unsigned long RecData(CFrameData& framedata);

private:
	void Close();
	unsigned long DealFirstKeyFrameAndSwitchFile(CFrameData& data);
	bool GetVideoAndAudioFormat();

// private:
public:
	bool              m_bWaitFirstKeyFrame;//录像文件第一帧必须是关键帧
	bool              m_bRecording;        //通道是否开启记录
	bool              m_bEnable;           //通道是否可用
	bool              m_bActive;           //通道是否激活（修改系统时间时，通道需暂停）

	long              m_chnnNum;           //标记该通道序号
	unsigned long     m_fileSize;
	CFileManager     *m_pFileManager;
	CRecWriter       *m_pRecWriter;
	FileAttr         *m_pFileAttr;
	CLocalDevice     *m_pLocalDev;
	CFrameData       *m_pAudioFrame;
	CFrameData       *m_pVideoFrame;

	unsigned long g_writed_file_len;  //test
};

#endif
