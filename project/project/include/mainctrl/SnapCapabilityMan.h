#ifndef _SNAP_CAPABILITY_MAN_H_
#define _SNAP_CAPABILITY_MAN_H_
#include "LocalDevice.h"

class CSnapCapabilityMan
{
public:
	
	//
	~CSnapCapabilityMan();

	//
	static CSnapCapabilityMan * Instance();

	//
	bool Initial(int channelNum, int rate, int rateTime, VIDEO_SIZE videoSize);

	void Quit();

	//
	void StarSnapJpeg(int chnn, bool bPhone);

	//
	void StopSnapJpeg(int chnn, bool bPhone);

	//
	void ChangeJPEGVideoSize(VIDEO_SIZE videoSize);

	//
	void ChangeJPEGVideoLevel(unsigned long videLevel);


protected:

private:

	//
	CSnapCapabilityMan();

	//
	int              m_chnnNum;	                //�ܹ�ͨ������
	VIDEO_SIZE       m_snapVideSize;            //jpegͼƬ�ķֱ���
	unsigned long    m_jpegLevel;               //jpegͼƬ�Ļ��ʼ���
	int              m_snapRate;                //���������ץjpegͼƬ��֡��
	int              m_snapRateTime;            //

	//
	int              m_phoneSnapRate;          //�ֻ�ץjpegͼƬ��֡��,
	int              m_phoneSnapRaiseChnn;     //���ֻ�ץjpegͼƬ��֡�ʸ����Ǹ�ͨ��
	              

	//
	unsigned char    *m_pChnnNoPhoneEableCount;  //���ֻ�������ץͼ�Ĵ���
	unsigned char    *m_pChnnPhoneEableCount;    //�ֻ�������ץͼ�Ĵ���


	CPUB_Lock  m_setSnapRateLock;
	static	CSnapCapabilityMan * m_pSnapCapabilityMan;
};

#endif

