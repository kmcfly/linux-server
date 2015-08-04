
#ifndef _FUNC_UI_CUSTOM_H_
#define _FUNC_UI_CUSTOM_H_

#include "WndStructDef.h"

/*
* ���ܲ��춨��
* ���沿��ר��, ���沿�ֲ���ȫ�������ڴ��ļ�
*/

//���Ա���������Ϣ
void TrySaveStartupInfo(WND_STARTUP_INFO* pCfgInfo, WND_STARTUP_INFO* pCurInfo, bool bForce = false);

int GetSplitIndex(unsigned long splitMode);

ULONGLONG GetMainMonitorCfg();

//(x1,y1) (x2,y2) ��������
bool IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);
//(x1,y1) (x2,y2) �������ܴ�
bool IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);

//////////////////////////////////////////////////////////////////////////
//

bool IsSupportFunction(_FUNC_TYPE funcType);

bool IsStaticFrame();

//�Ƿ�֧����̨
bool IsSupportPTZ();

//�Ƿ�֧�ֿ�����
bool IsSupportWzd();

//�Ƿ�֧�ֵ��ڻط�ɫ��
bool IsSupportChangePlayBackColor();

//�Ƿ�֧�ֵ��ڹ�����ɫ��
bool IsSupportChangeToolBarColor();

//�Ƿ�֧�ֵ����ֳ�ɫ��
bool IsSupportChangeLiveColor();

//�Ƿ�֧��ץͼ
bool IsSupportSnapPic();

//�Ƿ�֧�ֹػ�
bool IsSupportShutDown();

//�Ƿ�֧�̵ּ����������
bool IsSupportAlarmOut();

//�Ƿ�֧��SPOT���
bool IsSupportSpotOut();

//�Ƿ���¼�����ѡ��״̬��,����ѡ��(�Ϻ����ո���ֻ����ѡ��D1,�̶�����25,��������1600kbps,�򷵻�false)
bool IsSupportRecordResChange();
bool IsSupportRecordVideoSize();

//�Ƿ�֧�������� �ֱ��ʿɵ�
bool IsSupportSubCode();

//////////////////////////////////////////////////////////////////////////
//

//�Ƿ�֧�ֻ��л�[picture in picture]
bool IsSupportPip();

//�Ƿ�֧�ֻط�ʱ��ʾ�ֳ�[live in playback]
bool IsSupportLiPb(int& max);

bool IsSupportDiskSmart();
//////////////////////////////////////////////////////////////////////////
//__NVR_BASIC__ ͨ�����

//�Ƿ�ֻ֧��ĳЩͨ��
bool IsSupportCH(ULONGLONG& CH, int* pChnnNum = NULL, int* pLocalVideoInputNum = NULL, int* pNetVideoInputNum = NULL, int* pVideoOuputNum = NULL);

//�Ƿ�֧�ֻ����Ƶ��ʾ
bool IsSupportHybridDisplay();

//��ȡ�������ֱ��ʵ��������
unsigned long GetMainStreamMaxBitRate(unsigned long resolution);

//��ȡ�������ֱ��ʵ��������
unsigned long GetSubStreamMaxBitRate(unsigned long resolution);

#endif

//end
