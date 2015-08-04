/*
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xian shi wei
** Date         : 2014-10-15
** Name         : SetROIDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
*/

#ifndef __SET_ROI_DLG__
#define __SET_ROI_DLG__

#include "Dialog.h"
#include "CfgROIPanelDlg.h"
#include <list>

using namespace GUI;
class CSetROIDlg : public GUI::CDialog
{
	friend class CCfgROIPanelDlg;
public:
	CSetROIDlg();
	~CSetROIDlg();

	enum ROI_STATE{
		STATE_NULL = 0,
		STATE_ADD,
	};
	struct RectROI{
		VIDEO_CONFIG_ROI area;
		GUI::CRect		rect;
		
		unsigned long	id;
		unsigned long	old;
		unsigned long	color;
	};
	void SetCurrentChnn( unsigned int chnn){ 
		m_currChnn = chnn;
	}
	unsigned int GetCurrentChnn() const{ 
		return m_currChnn;
	}
	void SetROIInfo(const VIDEO_CONFIG_ROI_INFO& coverInfo);
	void GetROIInfo(VIDEO_CONFIG_ROI_INFO& coverInfo);

	void GetPanelParamter(unsigned int& enable,unsigned int& sensitivety)
	{
		m_panelDlg.SetParamter(enable,sensitivety);
	}
	inline bool IsModify() const;
    bool RefreshROIInfo();
protected:
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	void EndDialog(DLG_END_VALUE result);

	inline void SetModify(bool modify = true);
	inline bool PointInArea(unsigned long xPos, unsigned long yPos, unsigned long& idHit);
	//(x1,y1) (x2,y2) ��������
	inline bool IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);
	
	inline bool FindAreaById(unsigned long id, GUI::CRect& rect);
	inline void SetExistAreaById(unsigned long id, GUI::CRect rect);

	inline void GetSingleRoiInfo(unsigned long id, RectROI& roi);

	void DrawRectPt2(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, bool bDraw = true);
	void RefreshPt2(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2);
	void DrawRectSel(const GUI::CRect& rect, bool bDraw = true, bool bImmediate = true);
	void ColourRect(const GUI::CRect& rect, bool bDraw,bool bImmediate);
protected:
DECLARE_MSG_MAP()
	unsigned long OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDoubleClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
    unsigned long OnClickPanel();

public:
	CCfgROIPanelDlg         m_panelDlg; 

private:
	
	bool					m_bFirstDrag;			//��һ���϶����
	bool					m_bModify;				//����Ƿ��޸Ĺ�
	unsigned long			m_idSel;	            //��ѡ�е�����
	unsigned long			m_x_enter, m_y_enter;	//��������x,yֵ
	int                     m_sensitivity;
	int                     m_iEnable;
    int                     m_currChnn;
	int						m_areaMax;            	//�����������
	std::list<RectROI>		m_arealist;	            //��������
	ROI_STATE				m_state;				//��ǰ����״̬
	VIDEO_CONFIG_ROI_INFO	m_ROIInfo;              //���ⲿ�ӿڻ�ȡ������
};
#endif