
#ifndef _FUNC_UI_CUSTOM_H_
#define _FUNC_UI_CUSTOM_H_

#include "WndStructDef.h"

/*
* 功能差异定义
* 界面部分专用, 界面部分差异全部定义在此文件
*/

//尝试保存启动信息
void TrySaveStartupInfo(WND_STARTUP_INFO* pCfgInfo, WND_STARTUP_INFO* pCurInfo, bool bForce = false);

int GetSplitIndex(unsigned long splitMode);

ULONGLONG GetMainMonitorCfg();

//(x1,y1) (x2,y2) 距离相差不大
bool IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);
//(x1,y1) (x2,y2) 距离相差很大
bool IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);

//////////////////////////////////////////////////////////////////////////
//

bool IsSupportFunction(_FUNC_TYPE funcType);

bool IsStaticFrame();

//是否支持云台
bool IsSupportPTZ();

//是否支持开机向导
bool IsSupportWzd();

//是否支持调节回放色彩
bool IsSupportChangePlayBackColor();

//是否支持调节工具条色彩
bool IsSupportChangeToolBarColor();

//是否支持调节现场色彩
bool IsSupportChangeLiveColor();

//是否支持抓图
bool IsSupportSnapPic();

//是否支持关机
bool IsSupportShutDown();

//是否支持继电器报警输出
bool IsSupportAlarmOut();

//是否支持SPOT输出
bool IsSupportSpotOut();

//是否在录像参数选择状态下,都可选择(上海验收高清只允许选择D1,固定码率25,码流大于1600kbps,则返回false)
bool IsSupportRecordResChange();
bool IsSupportRecordVideoSize();

//是否支持子码流 分辨率可调
bool IsSupportSubCode();

//////////////////////////////////////////////////////////////////////////
//

//是否支持画中画[picture in picture]
bool IsSupportPip();

//是否支持回放时显示现场[live in playback]
bool IsSupportLiPb(int& max);

bool IsSupportDiskSmart();
//////////////////////////////////////////////////////////////////////////
//__NVR_BASIC__ 通道相关

//是否只支持某些通道
bool IsSupportCH(ULONGLONG& CH, int* pChnnNum = NULL, int* pLocalVideoInputNum = NULL, int* pNetVideoInputNum = NULL, int* pVideoOuputNum = NULL);

//是否支持混合视频显示
bool IsSupportHybridDisplay();

//获取主码流分辨率的最大码率
unsigned long GetMainStreamMaxBitRate(unsigned long resolution);

//获取子码流分辨率的最大码率
unsigned long GetSubStreamMaxBitRate(unsigned long resolution);

#endif

//end
