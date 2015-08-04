#ifndef __REC_CHNN_H__
#define __REC_CHNN_H__

#include "FileManager.h"
#include "FrameData.h"
#include "RecWriter.h"
#include "LocalDevice.h"


typedef enum _rec_chnn_fail_
{
	GET_VIDEO_AUDIO_FORMAT_FAIL = 0x0010,    //��ȡ����Ƶ��ʽʧ��
	SWITCH_REC_FILE_FAIL,                    //�л�¼���ļ�ʧ��
	NOT_KEY_FRAME_OF_FIRST,                  //д�ļ�ͷʱ���ַǹؼ�֡
	OPEN_FILE_FAIL,                          //���ļ�ʧ��(RecWrite)
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
	bool              m_bWaitFirstKeyFrame;//¼���ļ���һ֡�����ǹؼ�֡
	bool              m_bRecording;        //ͨ���Ƿ�����¼
	bool              m_bEnable;           //ͨ���Ƿ����
	bool              m_bActive;           //ͨ���Ƿ񼤻�޸�ϵͳʱ��ʱ��ͨ������ͣ��

	long              m_chnnNum;           //��Ǹ�ͨ�����
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
