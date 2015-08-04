/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-16
** Name         : ShanghaiSnapMan.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef __SHANGHAI_SNAPMAN_H__
#define __SHANGHAI_SNAPMAN_H__
#include "MessageMan.h"
#include <list>
#include "ShanghaiCmd.h"
#include "ShanghaiConnectMan.h"
#include "PUB_common.h"
#include "uuid.h"
#include "CharCodeTable.h"
using namespace std;

const int maxpicLen = 20*1024;
typedef struct _PreSnap_
{
	unsigned char *pData;//ͼƬ����
	int nLen;//ͼƬ���ݵĳ���
	int chnn;//ͨ����
	LONGLONG time;
	unsigned long type;
	unsigned short width;
	unsigned short height;
	unsigned short bLock;
}PRE_SNAP_INFO;

class CShanghaiSnapMan
{
public:
	static CShanghaiSnapMan *GetInstance();
	~CShanghaiSnapMan();
		
	bool Initial(unsigned char channelNum);
	void GetDailyCfg(const SNAP_INFOEX *pData,int count);//��������Ч����ȡ�ճ����õĶ�ʱ��Ϣ
	void Quit();

	//������ҳ�汨����ͼ����
	void SetScreenSnapParam(unsigned long chnn);
	//��������Ϊͨ���ţ�ץͼʱ�䣬ץͼ������
	void SetManualParam(ULONGLONG chnnBits,ULONGLONG snapTime,unsigned long imagetype);
	//��ȡ�ճ�ͼƬ
	//void *buf  --���������ȡ��ͼƬ������ֱ�ӱ��浽�ڴ�鿴
	bool ShanghaiDailySnapPicture(void *buf, int nLen, int nChannel, LONGLONG time,unsigned short width, unsigned short height);
	//�ֶ�ץͼ���ϴ�
	void ShanghaiManualSnapPicture(void *buf, int nLen, int nChannel, LONGLONG time,unsigned short width, unsigned short height);
	//�����ϴ�ͼƬ
	void  ShanghaiOnDemandSnapPicture(void *buf, int nLen, int nChannel, LONGLONG time);
	
	//�����������ϵ��ֶ�ץͼ�ͱ���ץͼ
	bool SetSnapPicture(unsigned long chnnl, unsigned long type);
	//����ץͼ��ҳ���ϵ��ֶ�ץͼ��������
	bool ShanghaiAlarmManualSnapPicture(void *buf, int nLen, int nChannel, LONGLONG time , unsigned short width, unsigned short height, bool bLock = false);

	//��ȡ��ǰץͼ��������Ϣ
	void GetPreAlarmCfg(const unsigned long *preSec);
	void GetPostAlarmCfg(const unsigned long *pPostSec);//��ȡ�����ӳٵ�������Ϣ
	void GetAlarmTime(unsigned long chnn);//��ȡ����ʱ��ʱ��

	void SetSnapTimeS(const unsigned long timeS);//����ץͼ���
	void SetSnapImgNum(const unsigned long imgNum);//����ץͼʱ��
	
	unsigned long GetSnapTime();
	void GetAlarmType(unsigned long alarmType,unsigned long chnn);

	void SetCheckPiliceNum(const char *pNote);
private:
	CShanghaiSnapMan();
	//�Ƚ�������ʱ���ʱ����ͻ�ȡʱ���е�ʱ����,
	//һ�·���true,���򷵻�false
	bool CmpDailyTime(unsigned long cfgTime,ULONGLONG time);
	void InsertPreSnapList(void *buf, int nLen, int nChannel, LONGLONG time,unsigned short width,unsigned short height,unsigned short block);
	//��װOPER_INFO info�����͸�ShanghaiConnectman
	//bool isbManual �������ֶ������ճ�ץͼ true���ֶ�ץͼ��false���ճ�ץͼ
	void CreateoperInfo(void *buf, int nLen, int nChannel, LONGLONG time,bool isbManual);
	//��װ����ץͼOPER_INFO info
	void CreateAlarmoperInfo(void *buf, int nLen, int nChannel, LONGLONG time,uuid_t theOneUUid);
	
	//����һ�鱨��ץͼ��Ψһͷ
	void CreateAlarmtitleInfo(uuid_t theOneUUid,LONGLONG time,unsigned long chnn);

	unsigned long GetChnnMAXSnap(unsigned long chnn);//����ÿ��ͨ������ȡ������

private:
	SNAP_INFOEX      *m_pData;//�ճ�ͼ�����������еĶ�ʱ(ֻ��ʱ��������)��ͨ��
	unsigned long    m_count; //�ճ����ù��м���
	ULONGLONG		 m_chnnBits;//�ֶ�ץͼ�ϴ�ʱѡ���ͨ����
	ULONGLONG		 m_manualSnapTime;//�ֶ�ץͼ�ϴ������ʱ��
	bool		     m_bmanualSnap;//�ֶ�ץͼ�ϴ�����

	LONGLONG	m_OnDemandChnnBits;
	LONGLONG	m_OnDemandSnapTime;

	unsigned long   	*m_pChannType;
	int                 *m_pChnnStatus;	
	int                 *m_pChnnCount;//�������ֶ���ͼ�ļ���
	LONGLONG            *m_pLastTime;//�������ֶ�ץͼ��һ��ͼƬ����ʱ�䣬���ں͵�ǰʱ��Ƚ��Ƿ�����趨��ʱ��
	LONGLONG			*m_pAlarmPrevTime;//����ץͼ��һ��ͼƬ��ʱ�䣨ԤץͼҲʹ�ø�ʱ�䣩
	int                  m_nMaxSnap;
	CPUB_Lock            m_lock;
	unsigned long        m_nMaxTime;
	int                 m_nCurSnapNum;

	list<PRE_SNAP_INFO> m_list[32];
	
	unsigned long		*m_preSec;//�������ж�ȡ�ı���ǰץͼ����
	unsigned long		m_maxchnn;
	unsigned long		*m_postSec;//�������ж�ȡ�ı�����ץͼ����
	//��ȡÿ��ͨ��������ͼ�Ŀ�ʼʱ�䣬�ڱ�����ͼ�ĳ����ڼ䣬
	//��ʹ�ֶ���ͼ����������������ɻ�ȡ��ͼֱ��������ʱ����
	unsigned long		*m_startSnapTime;

	unsigned long m_manualImageType;

	unsigned long *m_Alarmtype;//����������


	unsigned long *m_snapcount;//����ʱÿ��ͨ�������˶���ͼƬ
	uuid_t m_theoneMotion;
	uuid_t m_theoneSensor;
	char m_policeNote[256];

	ULONGLONG *m_repaetChnnBits;//�����ڵ��ظ��ճ�ͨ������2������0

	unsigned long * m_pScreenSnapKey;//��ҳ��ץͼ���,�����ƶ����ʱ����ҳ���ֶ���ͼ
};

#endif

