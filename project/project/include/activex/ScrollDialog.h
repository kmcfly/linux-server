#ifndef _SCROLL_DIALOG_H_
#define _SCROLL_DIALOG_H_

#include "Wnd.h"
#include "ScrollCtrl.h"
#include "Dialog.h"
#include "ScrollWnd.h"
#include <vector>

namespace GUI
{

class CScrollDialog : public GUI::CDialog
{
public:
	CScrollDialog();
	virtual ~CScrollDialog();

public:
	bool CreateWndFocusMap();

	//��Ӵ���Ԫ��,�����ǵ�(0,0)����
	GUI::CWnd* AddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xFocus,int yFocus, int index = -1);

	GUI::CScrollWnd& GetScrollWnd();

	void AddDrawBorderPos( int x, int y );

	void ClearAllDrawBorderPos();

	//���õ�ǰ����λ��
	void SetCurScrollPos(int x,int y,bool bRefresh=true);
	//��ȡ��ǰ����λ��
	void GetCurScrollPos(int& x, int& y);

public:
	//����ScrollWnd���ھ���Dlg�߽���Ծ���
	void SetScrollWndPos(int left=0,int top=0,int right=0,int bottom=0);

	//���ô��ڱ߿��С
	void SetDlgBorder(int border = 1);

	//��ɫ
	void SetBackgroundColor(unsigned long color) {m_color.backgroud=color;}
	void SetBorderColor(unsigned long color) {m_color.border=color;}
	void SetWndBorderColor(unsigned long color) {m_color.wnd_border=color;}
	void SetMouseOverColor(unsigned long color) {m_color.mouse_over=color;}

	void SetMouseOverOffset(int offset) {m_size.mouse_over_offset=offset;}

protected:
	virtual void OnInitial();
	virtual void OnPostInitial();
	virtual void OnDestroy();
	virtual void OnPaint(PAINT_STATUS status);
	virtual void OnErase(PAINT_STATUS status);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnScrollchangeV(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnScrollchangeH(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	unsigned long OnMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	unsigned long OnAdd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDec(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	void ScrollViewTo(int xIndex,int yIndex,bool bRefresh=true);

protected:
	GUI::CScrollWnd	m_wnd;
	GUI::CScrollCtrl	m_scroll_h;
	GUI::CScrollCtrl	m_scroll_v;

	int							m_scroll_x;			//ˮƽ������,λ��
	int							m_scroll_y;			//��ֱ������,λ��
	bool						m_init_scroll_x;		//�Ƿ��ʼ��ˮƽ������
	bool						m_init_scroll_y;		//�Ƿ��ʼ����ֱ������

protected:

	struct POS_INDEX
	{
		int		x;
		int		y;
	};
	std::vector<POS_INDEX>		m_posDrawBorder;//��Ҫ�������򱳾��߿�������

	POS_INDEX			m_cur_pos;		//��ǰ��������ڵ���ͼ�������

protected:

	//��ɫ����
	struct ITEM_COLOR
	{
		unsigned long backgroud;		//������ɫ
		unsigned long border;				//�߿���ɫ
		unsigned long wnd_border;		//������߿���ɫ
		unsigned long mouse_over;		//��껬��ѡ����ɫ
	};
	ITEM_COLOR		m_color;

	//λ�á���С����
	struct ITEM_SIZE
	{
		int mouse_over_offset;			//��껬��ѡ�����ƫ��ֵ

		int wnd_left;
		int wnd_top;
		int wnd_right;
		int wnd_bottom;

		int dlg_border;
	};
	ITEM_SIZE			m_size;
};

}


#endif

