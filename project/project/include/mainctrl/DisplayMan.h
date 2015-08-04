
#ifndef _DISPLAY_MAN_H_
#define _DISPLAY_MAN_H_

#include "dvrdvsdef.h"
#include "PUB_common.h"
#include "NetProtocol.h"
#include "mylist.cpp"
#include <stack>

#define DISPLAY_DEV_MAX					2		//监视器、SPOT
#define DISPLAY_LAYER_MAX				5		//现场、回放、网络流、缩微图 + 1
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
		unsigned char type;									//层 DISPLAY_LAYER_TYPE
		unsigned char z_order;								//层z轴顺序
		unsigned char signal_format;						//信号类型
		unsigned char num_chnn;								//有效通道数目
		unsigned long split_mode;							//层分割模式
		unsigned char area_chnn[DISPLAY_INDEX_MAX];			//视屏区域对应的通道
	};

	//显示信息
	struct DISPLAY_INFO
	{
		unsigned short		record;							//本显示信息是否被记录
		unsigned short		num_attrib;						//有效描述数据个数
		unsigned long		time_out;						//超时
	
		DISPLAY_LAYER		layer[DISPLAY_LAYER_MAX];		//层
		DISPLAY_ATTRIB		display_attrib[MAX_CHNN_NUM];	//显示描述数据

		DISPLAY_INFO();
		bool operator!=(const DISPLAY_INFO& display) const;
	};

	//显示设备
	struct DISPLAY_DEV
	{
		unsigned long		num;							//显示设备个数
		DISPLAY_DEV_INFO*	pMajorDev;						//指向主要显示设备
		DISPLAY_DEV_INFO	devInfo[DISPLAY_DEV_MAX];		//显示设备信息
	};

/************************************************************************/
/*  类初始化,退出                                                       */
/************************************************************************/
public:
	bool Initial(unsigned char localVideoInputNum,unsigned char videoInputNum, unsigned char videoOutNum,unsigned char netVideoInputNum);
	void Quit();

	//初始化、反初始化设置设备信息
	void SetDisplayDevInfo(const DISPLAY_DEV_INFO* pDispDevInfo, int num);
	void SetUnInitial();

	//线程
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
/*  显示控制                                                            */
/************************************************************************/
public:
	//参数:分割模式[组],分割模式数目,通道,通道数,超时[-1表示冻结],记录[是否添加到操作记录],通知刷新[是否通知需要刷新屏幕]
	unsigned long ChangeDisplay(unsigned long* pSplitMode, unsigned long splitLen, unsigned long* pChnn, int chnnLen, unsigned long timeout = 30, unsigned short bRec = 0x1, unsigned long bNotify = 0, bool bFilter = false, bool bCmd=false);
	unsigned long ChangeDisplay(unsigned long x, unsigned long y, unsigned long timeout = 30, unsigned short bRec = 0x1, unsigned long bNotify = 0);
	unsigned long ChangeDisplay(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long timeout = 30, unsigned short bRec = 0x1, unsigned long bNotify = 0);

	//设置迷你图显示位置
	void SetMiniWndPos(unsigned long percentX, unsigned long percentY, unsigned long percentCx, unsigned long percentCy);

	//获取当前显示
	unsigned long GetDisplayInfo(NET_DISPLAY_INFO& outDisplay, unsigned short x = 0, unsigned short y = 0, bool bUsePt = false);

	//x,y在显示区域的通道
	int HitChnn(unsigned short x, unsigned short y, unsigned long* pHits, int bufflen);

	//冻结当前显示
	unsigned long FreezeDisplay();

	//恢复显示状态栈中的最后一次显示状态、解锁冻结
	unsigned long PopDisplay();

protected:
	//保存当前显示状态
	void PushDisplay();

	//x,y在显示区域的通道
	int HitTest(unsigned short x, unsigned short y, const DISPLAY_INFO& dispinfo, unsigned long* pHit, int len);

	//计算某一分辨率下的DISPLAY_ATTRIB
	void CalculateDisplayInfo(const DISPLAY_DEV_INFO* pDevInfo, DISPLAY_INFO& inOutDisplay);

	//
	unsigned long ChangeDisplayEx(unsigned long* pSplitMode, unsigned long splitLen, unsigned long* pChnn, int chnnLen, unsigned long timeout = 30, unsigned short bRec = 0x1, unsigned long bNotify = 0, bool bFilter = false);
	//修改显示画面
	unsigned long ChangeDisplayFormat(const DISPLAY_DEV_INFO* pDevInfo, DISPLAY_INFO& display, unsigned long bNotify = 0, bool bFilter = false);

	//判断是否为支持的模式
	bool IsInvalidSplitMode(unsigned long splitMode);

	//检查,修正显示模式
	unsigned long CheckSplitMode(unsigned long splitMode, int numChnn);

	//通知刷新显示
	void NotifyRefreshDisplay(const DISPLAY_INFO& display);
	//无效通道继续跳台
	void UpdateInvalidDwellTime();
	//显示操作相关
	DISPLAY_INFO				m_display_info;		//显示信息描述
	int							m_max_history;		//历史记录大小
	CMyList<DISPLAY_INFO>		m_display_history;	//修改显示的历史记录
	int							m_max_stack;		//最大支持栈大小
	std::stack<DISPLAY_INFO>	m_display_stack;	//显示状态的栈记录
	unsigned long				m_time_out;			//锁定时间,-1表示永远锁定,直到下次恢复
	unsigned long				m_tm_wait;			//显示变更计时,会暂停跳台计时,超过锁定时间后恢复跳台计时


/************************************************************************/
/*  跳台                                                                */
/************************************************************************/
public:
	void SetEnable(bool enable);
	bool IsEnable();
	void Pause();
	bool IsPause();
	void Resume();

	void SetSplitMode(unsigned long splitMode);
	void SetDwellTime(const unsigned long* pDwellTime, int len);
	void SetSplitData(SPLIT split, const unsigned long* pData,int len);//len=unsigned long 的个数
	void SetDwellTimeSpot(unsigned long dwellTime);
	void SetSplitDataSpot(SPLIT split, const unsigned long* pData,int len);//len=unsigned long 的个数

protected:
	void AttachActiveData();			//根据分割模式,选择对应的跳台数据
	void CalculateVisibleChnn();		//计算处于显示的通道

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


	//跳台线程操作
	static RESULT WINAPI TimerThread(LPVOID lpParameter);
	void Timer();

	bool			m_bTimer;							//
	PUB_thread_t	m_timer_ID;							//线程ID

	//跳台数据部分
	unsigned long	m_splitMode;						//当前跳台使用的分割模式
	unsigned long	m_dwellTime;						//跳台时间

	int				m_activeData;						//处于激活的数据集
	int				m_splitDataLen[SPLIT_COUNT];		//各个数据集合大小
	unsigned long*	m_pSplitData[SPLIT_COUNT];			//全部分割数据集
	unsigned char	m_bVisibleChnn[DISPLAY_INDEX_MAX];	//显示通道列表
	int				m_dwellTimeDataLen;					//跳台时间数据长度
	unsigned long*  m_pDwellTimeData;					//跳台时间间隔

	unsigned long*	m_pTmpData;			//临时数据区
	int				m_tmpDataLen;		//临时数据区长度
	CPUB_Lock		m_lock;				//读写变量锁,全部非耗时操作
	bool			m_bEnable;			//是否启用跳台
	bool			m_bPause;			//跳台是否处于暂停状态
	unsigned long	m_tmCount;			//跳台计时
	int				m_group;			//当前跳台组数

	bool            m_bHaveSpot;
	unsigned long	m_dwellTimeSpot;	//SPOT间隔时间
	int				m_splitDataLenSpot;	//SPOT数据长度
	unsigned long*	m_pSplitDataSpot;	//SPOT数据
	unsigned long	m_tmCountSpot;		//SPOT计时间
	int				m_groupSpot;		//当前SPOT组数


/************************************************************************/
/*  公共数据                                                            */
/************************************************************************/
	bool			m_bInit;			//类初始化
	bool			m_bInitResolution;	//分辨率初始化
	bool			m_bFullChannelDisplay;//设备启动时，要先全通道显示，否则在报警触发大画面会导致死机

	//视频通道
	unsigned char	m_localVideoInputNum;
	unsigned char	m_videoInputNum;
	unsigned char	m_videoOutNum;
	unsigned char	m_netVideoInputNum;

	DISPLAY_DEV		m_dispDev;			//设备信息

	//迷你图位置, 百分比
	unsigned long	m_posPercentX;
	unsigned long	m_posPercentY;
	unsigned long	m_posPercentCx;
	unsigned long	m_posPercentCy;

	unsigned long   m_prev_well_chnnData[DISPLAY_INDEX_MAX];
	bool            m_bDisplayChanged;


public:
	enum ERR_DISP
	{
		ERR_OK					= 0,		//成功
		ERR_UNKNOWN				= 0x10000,	//未知错误
		ERR_UNINITIAL			= 0x10001,	//未完全初始化
		ERR_FREEZE				= 0x10002,	//被冻结[或锁定]状态

		ERR_STACK_EMPTY			= 0x10003,	//显示栈,已经空
		ERR_STACK_OVER			= 0x10004,	//显示栈溢出

		ERR_INVALID_PT			= 0x10005,	//无效的鼠标点击位置

		ERR_INVALID_CHNN		= 0x10006,	//无效的通道参数
		ERR_INVALID_SPLITMODE	= 0x10007,	//无效的分割模式

		ERR_HISTORY_EMPTY		= 0x10008,	//历史记录为空

		ERR_DISP_DATA_SAME		= 0x10009,	//相同的显示信息, 将不会去修改物理设备
	};
};

#endif

//end
