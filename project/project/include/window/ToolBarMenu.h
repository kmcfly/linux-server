
#ifndef _TOOL_BAR_MENU_H_
#define _TOOL_BAR_MENU_H_

//__DVR_MENU__

#include "MenuEx.h"

class CToolBarMenu : public GUI::CMenuEx
{
public:
	CToolBarMenu();
	virtual ~CToolBarMenu();

	void InitMenuData();

	bool GetSelChnn(ULONGLONG& CH, unsigned long& splitMode, unsigned long& n);
	int GetSelPipChnn(void){return m_selPipChnn;}
	void SetCurSplitMode(unsigned long curSplitMode){ m_splitMode = curSplitMode;}

	unsigned long PopupProc(unsigned long keyID, unsigned long xPos, unsigned long yPos);

	void GetPtPos(unsigned long& xPos, unsigned long& yPos);

public:
	void EnableDwell(bool bOpen);
	void EnableAudio(bool bOpen);

protected:
	void PreUpdateUI();
	unsigned long OnClickNXN(unsigned long itemID);
	unsigned long OnClickPip(unsigned long itemID);
	unsigned long OnClickChange(unsigned long itemID);
	unsigned long OnClickSnap(unsigned long itemID);
	unsigned long OnClickCamPara(unsigned long itemID);
protected:
	int				m_maxChnn;		//最大通道数目

	bool			m_bClickChnn;	//是否点中了通道
	ULONGLONG		m_selChnnMask;	//选择的通道
	int				m_selPipChnn;	//选择的画中画通道
	unsigned long	m_splitMode;	//分割模式
	unsigned long	m_maxN;			//
	unsigned long	m_xPos;			//鼠标点击的x,y坐标
	unsigned long	m_yPos;

public:
	enum
	{
		//不要和KeyValue.h中重复
		MITEM_ID_SINGLE	= 0x100001,
		MITEM_ID_MULTI	= 0x100002,
		MITEM_ID_PIP	= 0x100003,
		MITEM_ID_SINGLE_MORE = 0x100004,
		MITEM_ID_MULTI_UNREGULAR = 0x100005,
		MITEM_ID_PIP_MORE = 0x100006,

		MITEM_ID_1X1	= 0x100010,
		MITEM_ID_2X2	= 0x100050,
		MITEM_ID_2X3	= 0x100070,
		MITEM_ID_3X3	= 0x100090,
		MITEM_ID_1A5	= 0x1000A0,
		MITEM_ID_4X4	= 0x1000B0,
		MITEM_ID_1A7	= 0x1000C0,
		MITEM_ID_1A12	= 0x1000D0,
		MITEM_ID_5X5	= 0x1000E0,
		MITEM_ID_6X6	= 0x1000F0,
		MITEM_ID_NXN	= 0x100100,		//作为通道点击的返回判定
		MITEM_ID_COLOR  = 0x100110,		//改变颜色
		MITEM_ID_SNAP   = 0x100120,		//抓图
		MITEM_ID_P_1X1  = 0x100130,		//画中画
		MITEM_ID_CAM_PARA= 0x1001C0,     //改变摄像机参数 ，MITEM_ID_CAM_PARA，否则被MITEM_ID_P_1X1+128，大或者小。
	};

	enum
	{
		DWELL_OPEN,
		DWELL_CLOSE
	};

	enum
	{
		AUDIO_OPEN,
		AUDIO_MUTE
	};
};

#endif

//end
