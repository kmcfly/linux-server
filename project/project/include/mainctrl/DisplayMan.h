
#ifndef _DISPLAY_MAN_H_
#define _DISPLAY_MAN_H_

#include "dvrdvsdef.h"
#include "PUB_common.h"
#include "NetProtocol.h"
#include "mylist.cpp"
#include <stack>

#define DISPLAY_DEV_MAX					2		//��������SPOT
#define DISPLAY_LAYER_MAX				5		//�ֳ����طš�����������΢ͼ + 1
#define DISPLAY_INDEX_MAX				36		//6x6

enum
{
	DISPLAY_UNRECORD	= 0,
	DISPLAY_RECORD		= 1,

	DISPLAY_UNNOTIFY	= 0,
	DISPLAY_NOTIFY		= 1,

	DISPLAY_FREEZE		= INVALID_INDEX(unsigned long),
	DISPLAY_INVALID		= INVALID_INDEX(unsigned char),
};

class CDisplayMan
{
protected:
	CDisplayMan();

public:
	static CDisplayMan* Instance();
	virtual ~CDisplayMan();

	enum SPLIT
	{
		SPLIT_1X1 = 0,
		SPLIT_2X2,
		SPLIT_2X3,
		SPLIT_3X3,
		SPLIT_4X4,
		SPLIT_4X6,
		SPLIT_5X5,
		SPLIT_6X6,

		SPLIT_COUNT
	};

	struct DISPLAY_LAYER
	{
		unsigned char type;									//�� DISPLAY_LAYER_TYPE
		unsigned char z_order;								//��z��˳��
		unsigned char signal_format;						//�ź�����
		unsigned char num_chnn;								//��Чͨ����Ŀ
		unsigned long split_mode;							//��ָ�ģʽ
		unsigned char area_chnn[DISPLAY_INDEX_MAX];			//���������Ӧ��ͨ��
	};

	//��ʾ��Ϣ
	struct DISPLAY_INFO
	{
		unsigned short		record;							//����ʾ��Ϣ�Ƿ񱻼�¼
		unsigned short		num_attrib;						//��Ч�������ݸ���
		unsigned long		time_out;						//��ʱ
	
		DISPLAY_LAYER		layer[DISPLAY_LAYER_MAX];		//��
		DISPLAY_ATTRIB		display_attrib[MAX_CHNN_NUM];	//��ʾ��������

		DISPLAY_INFO();
		bool operator!=(const DISPLAY_INFO& display) const;
	};

	//��ʾ�豸
	struct DISPLAY_DEV
	{
		unsigned long		num;							//��ʾ�豸����
		DISPLAY_DEV_INFO*	pMajorDev;						//ָ����Ҫ��ʾ�豸
		DISPLAY_DEV_INFO	devInfo[DISPLAY_DEV_MAX];		//��ʾ�豸��Ϣ
	};

/************************************************************************/
/*  ���ʼ��,�˳�                                                       */
/************************************************************************/
public:
	bool Initial(unsigned char localVideoInputNum,unsigned char videoInputNum, unsigned char videoOutNum,unsigned char netVideoInputNum);
	void Quit();

	//��ʼ��������ʼ�������豸��Ϣ
	void SetDisplayDevInfo(const DISPLAY_DEV_INFO* pDispDevInfo, int num);
	void SetUnInitial();

	//�߳�
	bool Start();
	void Stop();

	void Dump();

	unsigned char GetLocalVideoInputNum() { return m_localVideoInputNum; };
	unsigned char GetVideoInputNum() { return m_videoInputNum; };
	unsigned char GetVideoOutNum() { return m_videoOutNum; };
	unsigned char GetNetVideoInputNum() { return m_netVideoInputNum; };

	void SendClickPos(unsigned long x,unsigned long y);
	void SendInitChnn(unsigned long chnn);

/************************************************************************/
/*  ��ʾ����                                                            */
/************************************************************************/
public:
	//����:�ָ�ģʽ[��],�ָ�ģʽ��Ŀ,ͨ��,ͨ����,��ʱ[-1��ʾ����],��¼[�Ƿ���ӵ�������¼],֪ͨˢ��[�Ƿ�֪ͨ��Ҫˢ����Ļ]
	unsigned long ChangeDisplay(unsigned long* pSplitMode, unsigned long splitLen, unsigned long* pChnn, int chnnLen, unsigned long timeout = 30, unsigned short bRec = 0x1, unsigned long bNotify = 0, bool bFilter = false, bool bCmd=false);
	unsigned long ChangeDisplay(unsigned long x, unsigned long y, unsigned long timeout = 30, unsigned short bRec = 0x1, unsigned long bNotify = 0);
	unsigned long ChangeDisplay(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long timeout = 30, unsigned short bRec = 0x1, unsigned long bNotify = 0);

	//��������ͼ��ʾλ��
	void SetMiniWndPos(unsigned long percentX, unsigned long percentY, unsigned long percentCx, unsigned long percentCy);

	//��ȡ��ǰ��ʾ
	unsigned long GetDisplayInfo(NET_DISPLAY_INFO& outDisplay, unsigned short x = 0, unsigned short y = 0, bool bUsePt = false);

	//x,y����ʾ�����ͨ��
	int HitChnn(unsigned short x, unsigned short y, unsigned long* pHits, int bufflen);

	//���ᵱǰ��ʾ
	unsigned long FreezeDisplay();

	//�ָ���ʾ״̬ջ�е����һ����ʾ״̬����������
	unsigned long PopDisplay();

protected:
	//���浱ǰ��ʾ״̬
	void PushDisplay();

	//x,y����ʾ�����ͨ��
	int HitTest(unsigned short x, unsigned short y, const DISPLAY_INFO& dispinfo, unsigned long* pHit, int len);

	//����ĳһ�ֱ����µ�DISPLAY_ATTRIB
	void CalculateDisplayInfo(const DISPLAY_DEV_INFO* pDevInfo, DISPLAY_INFO& inOutDisplay);

	//
	unsigned long ChangeDisplayEx(unsigned long* pSplitMode, unsigned long splitLen, unsigned long* pChnn, int chnnLen, unsigned long timeout = 30, unsigned short bRec = 0x1, unsigned long bNotify = 0, bool bFilter = false);
	//�޸���ʾ����
	unsigned long ChangeDisplayFormat(const DISPLAY_DEV_INFO* pDevInfo, DISPLAY_INFO& display, unsigned long bNotify = 0, bool bFilter = false);

	//�ж��Ƿ�Ϊ֧�ֵ�ģʽ
	bool IsInvalidSplitMode(unsigned long splitMode);

	//���,������ʾģʽ
	unsigned long CheckSplitMode(unsigned long splitMode, int numChnn);

	//֪ͨˢ����ʾ
	void NotifyRefreshDisplay(const DISPLAY_INFO& display);
	//��Чͨ��������̨
	void UpdateInvalidDwellTime();
	//��ʾ�������
	DISPLAY_INFO				m_display_info;		//��ʾ��Ϣ����
	int							m_max_history;		//��ʷ��¼��С
	CMyList<DISPLAY_INFO>		m_display_history;	//�޸���ʾ����ʷ��¼
	int							m_max_stack;		//���֧��ջ��С
	std::stack<DISPLAY_INFO>	m_display_stack;	//��ʾ״̬��ջ��¼
	unsigned long				m_time_out;			//����ʱ��,-1��ʾ��Զ����,ֱ���´λָ�
	unsigned long				m_tm_wait;			//��ʾ�����ʱ,����ͣ��̨��ʱ,��������ʱ���ָ���̨��ʱ


/************************************************************************/
/*  ��̨                                                                */
/************************************************************************/
public:
	void SetEnable(bool enable);
	bool IsEnable();
	void Pause();
	bool IsPause();
	void Resume();

	void SetSplitMode(unsigned long splitMode);
	void SetDwellTime(const unsigned long* pDwellTime, int len);
	void SetSplitData(SPLIT split, const unsigned long* pData,int len);//len=unsigned long �ĸ���
	void SetDwellTimeSpot(unsigned long dwellTime);
	void SetSplitDataSpot(SPLIT split, const unsigned long* pData,int len);//len=unsigned long �ĸ���

protected:
	void AttachActiveData();			//���ݷָ�ģʽ,ѡ���Ӧ����̨����
	void CalculateVisibleChnn();		//���㴦����ʾ��ͨ��

	void ProcessSpot();
	unsigned long ProcessSpot1X1(unsigned long group, unsigned long* pData);

	unsigned long ProcessDwell();
	unsigned long ProcessNXN(int n,unsigned long group,unsigned long* pData);
	unsigned long Process1X1(unsigned long group,unsigned long* pData);
	unsigned long Process2X2(unsigned long group,unsigned long* pData);
	unsigned long Process2X3(unsigned long group,unsigned long* pData);
	unsigned long Process3X3(unsigned long group,unsigned long* pData);
	unsigned long Process4X4(unsigned long group,unsigned long* pData);
	unsigned long Process4X6(unsigned long group,unsigned long* pData);
	unsigned long Process5X5(unsigned long group,unsigned long* pData);
	unsigned long Process6X6(unsigned long group,unsigned long* pData);


	//��̨�̲߳���
	static RESULT WINAPI TimerThread(LPVOID lpParameter);
	void Timer();

	bool			m_bTimer;							//
	PUB_thread_t	m_timer_ID;							//�߳�ID

	//��̨���ݲ���
	unsigned long	m_splitMode;						//��ǰ��̨ʹ�õķָ�ģʽ
	unsigned long	m_dwellTime;						//��̨ʱ��

	int				m_activeData;						//���ڼ�������ݼ�
	int				m_splitDataLen[SPLIT_COUNT];		//�������ݼ��ϴ�С
	unsigned long*	m_pSplitData[SPLIT_COUNT];			//ȫ���ָ����ݼ�
	unsigned char	m_bVisibleChnn[DISPLAY_INDEX_MAX];	//��ʾͨ���б�
	int				m_dwellTimeDataLen;					//��̨ʱ�����ݳ���
	unsigned long*  m_pDwellTimeData;					//��̨ʱ����

	unsigned long*	m_pTmpData;			//��ʱ������
	int				m_tmpDataLen;		//��ʱ����������
	CPUB_Lock		m_lock;				//��д������,ȫ���Ǻ�ʱ����
	bool			m_bEnable;			//�Ƿ�������̨
	bool			m_bPause;			//��̨�Ƿ�����ͣ״̬
	unsigned long	m_tmCount;			//��̨��ʱ
	int				m_group;			//��ǰ��̨����

	bool            m_bHaveSpot;
	unsigned long	m_dwellTimeSpot;	//SPOT���ʱ��
	int				m_splitDataLenSpot;	//SPOT���ݳ���
	unsigned long*	m_pSplitDataSpot;	//SPOT����
	unsigned long	m_tmCountSpot;		//SPOT��ʱ��
	int				m_groupSpot;		//��ǰSPOT����


/************************************************************************/
/*  ��������                                                            */
/************************************************************************/
	bool			m_bInit;			//���ʼ��
	bool			m_bInitResolution;	//�ֱ��ʳ�ʼ��
	bool			m_bFullChannelDisplay;//�豸����ʱ��Ҫ��ȫͨ����ʾ�������ڱ�����������ᵼ������

	//��Ƶͨ��
	unsigned char	m_localVideoInputNum;
	unsigned char	m_videoInputNum;
	unsigned char	m_videoOutNum;
	unsigned char	m_netVideoInputNum;

	DISPLAY_DEV		m_dispDev;			//�豸��Ϣ

	//����ͼλ��, �ٷֱ�
	unsigned long	m_posPercentX;
	unsigned long	m_posPercentY;
	unsigned long	m_posPercentCx;
	unsigned long	m_posPercentCy;

	unsigned long   m_prev_well_chnnData[DISPLAY_INDEX_MAX];
	bool            m_bDisplayChanged;


public:
	enum ERR_DISP
	{
		ERR_OK					= 0,		//�ɹ�
		ERR_UNKNOWN				= 0x10000,	//δ֪����
		ERR_UNINITIAL			= 0x10001,	//δ��ȫ��ʼ��
		ERR_FREEZE				= 0x10002,	//������[������]״̬

		ERR_STACK_EMPTY			= 0x10003,	//��ʾջ,�Ѿ���
		ERR_STACK_OVER			= 0x10004,	//��ʾջ���

		ERR_INVALID_PT			= 0x10005,	//��Ч�������λ��

		ERR_INVALID_CHNN		= 0x10006,	//��Ч��ͨ������
		ERR_INVALID_SPLITMODE	= 0x10007,	//��Ч�ķָ�ģʽ

		ERR_HISTORY_EMPTY		= 0x10008,	//��ʷ��¼Ϊ��

		ERR_DISP_DATA_SAME		= 0x10009,	//��ͬ����ʾ��Ϣ, ������ȥ�޸������豸
	};
};

#endif

//end
