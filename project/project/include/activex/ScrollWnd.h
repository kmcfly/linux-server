#ifndef _SCROLL_WND_H_
#define _SCROLL_WND_H_


#include "Wnd.h"
#include "Rect.h"
#include <vector>
#include <map>

/*
  ���ù̶������ӵķ�ʽʵ��

  ȱ��:ÿ���ƶ���ͼ,����ӿؼ�������������ͼ֮��
  ���п����޷���ʾ�˿ؼ������ʱ����������������

  ����:�������ؼ���������(���������)֮��,��ͼ��
  ��һ�㰴���������Ϊ��λ

  ˵��:���ಢ������������Ļ���,���ڵı�����
  CScrollDialog��ʵ��.����ֻ�������ڱ���ͼ�������
  �ؼ�,�Լ���صĲ�����Ϣ.

  ˼·:��һ������������´�������ͼ����(�ܱ�ʵ��
  ��ʾ���ڴ�),ʵ�ʴ���(�൱��һ�龵��)һ��ֻ�ܿ�����
  ��ͼ�����е�һ����.

  ʵ��:��¼ÿ���ӿؼ������λ��,��ʼλ�ö�λ�����
  ԭ��.ÿ���ƶ���ͼʱ,���ó�ʼ���λ�úʹ��ڴ�С����
  ÿ���ӿؼ���Ϣ:����״̬��λ����Ϣ,��������ʾ״̬��
  �����ƶ�����Ӧ��λ��.

  ��չ:���÷ǹ̶�����,�ֶ�ָ��ÿ���ƶ����λ��.
  ������������

  ע��:���뱾��Ĵ��ڿؼ�ID��20λ��ʾ,���<0Ϊ������
  �ؼ��������11λ��ʾ
*/

namespace GUI
{

class CScrollWnd : public GUI::CWnd
{
	friend class CScrollDialog;
public:
	CScrollWnd();
	virtual ~CScrollWnd();

public:
	enum SW_WND_ALIGN
	{
		SW_WND_NULL		= 0x0000,
		SW_WND_OFFSET	= 0x0001,

		SW_WND_LEFT		= 0x0002,
		SW_WND_RIGHT	= 0x0004,
		SW_WND_CENTER_X = 0x0008,

		SW_WND_TOP		= 0x0010,
		SW_WND_BOTTOM	= 0x0020,
		SW_WND_CENTER_Y = 0x0040,
	};

protected:
	struct WND_ITEM 
	{
		int				Index;
		bool			Show;
		GUI::CRect		Rect;
		GUI::CWnd*		pWnd;
		WND_ITEM& operator=(const WND_ITEM& item);
	};

public:
	//Ϊ�ӿؼ���������
	bool CreateWndFocusMap();

	//��Ӵ���Ԫ��,�����ǵ�(0,0)����
	GUI::CWnd* AddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xFocus,int yFocus, int index = -1);
	GUI::CWnd* AddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xIndex, int yIndex, int xFocus,int yFocus, unsigned long align = SW_WND_NULL, int index = -1, int xOffset = 0, int yOffset = 0);

	//�������ô���Ԫ�ص�ԭʼλ��
	void ResetWndRect(int index, const GUI::CRect& rect);

	//�Ƿ���ʾĳ���Ӵ���
	void ShowSubWnd(int index, bool show = true);
	void ShowSubWndID(unsigned long ctrlID, bool show = true );

	//�ƶ���ͼ,����ֵ
	void ScrollView(int x, int y);

	//�ƶ���ͼ,��Ա��
	void ScrollViewTo(int xIndex, int yIndex);

	//���ô���λ��
	//void SetWndPos(int x, int y, int cx, int cy);

	//����������С
	void SetSizeOffset(int sizeX,int sizeY );
	//�����ӻ�ȡ��ͼ�������ֵ
	void SetViewMaxSize(int maxCx,int maxCy);
	void GetViewMaxSize(int& cx, int& cy);

	//����IDֵ��ȡĳ�Ӵ���
	GUI::CWnd* GetWnd(int index);

	//��ȡĳ����������ǵ�����,��ȡ������С
	void GetViewPos(int xIndex,int yIndex,unsigned long& x, unsigned long& y );
	void GetViewSize(int xIndex,int yIndex,unsigned long& cx, unsigned long&cy );

	//�ӵ�ǰ����[x,y]�����ȡʵ�ʶ�Ӧ��������
	bool GetCurViewIndex(int x,int y,int& xIndex, int& yIndex);

	//��ʵ��������Ż�ȡ�����ڵ�ǰ���ڵĿ���
	bool GetCurViewMap(int xIndex,int yIndex,int& xWnd,int& yWnd);

	//��ȡһ�����������
	void GetPageCount(unsigned long& x,unsigned long& y);
	//��ȡ[cx,cy]���������
	void GetMaxPageCount(unsigned long cx, unsigned long cy, unsigned long& x,unsigned long& y);

	//������ͼ����߿�
	void DrawViewBorder(int xIndex, int yIndex, unsigned long color = COLOR_LINE_GRAY, int offset = 0);

	//�Ӵ���id��ȡ����,-1Ϊ��Чֵ
	int GetItemWndIndex(unsigned long id);

protected:
	void OnPaint(PAINT_STATUS status);
	void OnDestroy();

protected:
	virtual unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CWnd* DoAddWnd(WND_TYPE wndType, unsigned long ctrlID, const GUI::CRect& rect, int xFocus,int yFocus, int index);
	bool DoAddPosOriginally( unsigned long id, const WND_ITEM& item );
	void DoScrollWnd(int x, int y);

private:
	std::map<unsigned long,WND_ITEM> m_mapPosOriginally;		//��¼�Ӵ��ڿؼ�ID,��Ӧ��ԭʼ����,�������

	int		m_xSizeOff;		//��ͼ����һ��xƫ�ƴ�С
	int		m_ySizeOff;		//��ͼ����һ��yƫ�ƴ�С
	int		m_xIndexOff;	//��ͼ����xƫ�� = m_xSizeOff * m_xIndexOff;
	int		m_yIndexOff;	//��ͼ����yƫ�� = m_ySizeOff * m_yIndexOff;

	int		m_maxCx;		//��ͼ�������߽�
	int		m_maxCy;		//��ͼ�������߽�

	//std::vector<int>	m_xOffVec;
	//std::vector<int>	m_yOffVec;
};

}

#endif

