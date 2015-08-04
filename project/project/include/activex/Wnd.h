#ifndef _BASE_CTRL_H_
#define _BASE_CTRL_H_
#include <list>
#include <string.h>
#include "Rect.h"
#include "gui.h"
#include "ImageList.h"
#include "KeyValue.h"
#include "FBInterface.h"
#include "StringTable.h"
#include "GuiAssert.h"

//////////////////////////////////////////////////////////////////////////
//extern unsigned long COLOR_TEXT_NORMAL = GUI_RGB(200, 200, 200);
//////////////////////////////////////////////////////////////////////////
//ȱ��
//1.�����Ƶ�����ֻ��һ�н���ʱ�������ϼ���û��ʵ�ֽ����ƶ�ѭ�� 090720
//2.��̬�ؼ�ˢ������

namespace GUI
{
	class CWnd/* : public GUI::CWnd*/
	{
		friend class GUI::CImageList;
	public:
		CWnd();
		virtual ~CWnd();

		typedef std::list<CWnd*> WND_LIST;
		//���÷ֱ��ʣ���̬�ӿڣ�ʹ�þ�̬������¼��ߣ����ڶ�̬���㴰�����ꡣ
		static void SetSize(unsigned short width, unsigned short height);

		//��������ȡ������,�������кܶ�����
		//Ψһ���ô��Ǽ�¼����[����]��������,��������ʱˢ��������������
		static GUI::CWnd* GetRootWnd();
		//ͨ���ýӿڣ�����ֱ��ˢ���ֳ���ֻ����FB�п��Ƶķָ��ߺ�״̬��Ϣ�ȣ�
		static void RefreshLive();
		//���ٹ�������
		static void Quit();

		////����������
		virtual bool Create ( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y, 
			unsigned long cx, 
			unsigned long cy, 
			CWnd* pParent,
			unsigned long style,
			int xFocus,
			int yFocus
			);

		void Destroy();

		/////�������Բ���
		CTRLID GetID() const { return static_cast<CTRLID>(m_windowID); }//��ȡID
		unsigned long GetStyle() const{ return m_style;}//��ȡ���
		void SetStyle(unsigned long style) { m_style = style; }//���÷��
		GUI::WND_TYPE GetCtrlType() const {return m_ctrlType;}//��ȡ��������
		//�轹��
		virtual bool SetFocus(bool bFocus, bool bRefresh = true);
		bool IsFocus() const { return m_bFocus; }
		//ʹ��
		virtual void SetEnable(bool bEnable, bool bRefresh = true);
		bool IsEnable() const { return m_bEnable; }
		//�ɼ�
		void Show(bool bShow, bool bRefresh = true);
		bool IsShow() const;
		//����
		virtual CRect GetWindowRect() const;//�����Ļ����
		virtual CRect GetClientRect() const { return GUI::CRect(0, 0, m_rect.Width() - 1, m_rect.Height() - 1); }//��ÿͻ�������
		void ClientToScreen(unsigned long & x, unsigned long & y) const;//�ͻ���һ��ת������Ļ����ϵ
		void ScreenToClient(int & x, int & y) const;
		void ScreenToClient(CRect& rect) const;
	
		//��û�Ӵ���
		CWnd* GetActiveCtrl() { return m_pActiveCtrl; }//��õ�ǰ�Ӵ��ڣ����н���Ĵ���
		CWnd* GetCtrl(CTRLID ctrlID);//����ID��ȡ�Ӵ���
		//���ô��ڱ���
		virtual void SetCaption(const std::string& strCaption, bool bRefresh = true);
		virtual std::string GetCaption() const { return m_strCaption; }
		//��ý�������
		void GetFocusPos(int & xFocus, int & yFocus) const 
		{
			xFocus = m_xFocus;
			yFocus = m_yFocus;
		}
		//����\���ͼƬ
		virtual void SetImage(BITMAP_ID id, bool bRefresh);
		virtual BITMAP_ID GetImage() const { return m_imageID; }

		////���ڲ��
		//��ȡ�Ӵ�����������㴰��
		CWnd* GetTopestWnd();
		//��ȡ��chuangk
		CWnd* GetParent() { return m_pParent; }

		////������,���԰�ĳ������������ָ��
		void SetData(LONGLONG pData) { m_data = pData;}
		LONGLONG GetData() const { return m_data; }

		//�ƶ����� ע�⣺����ˢ�´���
		virtual void MoveWnd(int x, int y);
		void SetWndSize(bool bOriginal = true, bool bRefresh = true, int cx = 0, int cy = 0);

		//���ô���λ��
		void SetWndPos(const CRect& rect);
		void SetWndPos(int x, int y, int cx, int cy);

		//�ػ�
		void Repaint();
		//////////////////////////////////////////////////////////////////////////
		////���ϵͳ�����һ������ID
		static CTRLID GetRandWndID() { return static_cast<CTRLID>(++s_randWndID); }
	protected:
		////���ڼ̳���д
		//��ʼ���뷴��ʼ��
		virtual void OnInitial() {};
		virtual void OnDestroy() {};
		//��ʼ����Ĳ���
		virtual void OnPostInitial() {};
		//���ƴ���
		virtual void OnPaint(PAINT_STATUS status){};
		virtual void OnErase(PAINT_STATUS status){};
		//����ı���Ӧ
		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);
		virtual unsigned long OnFocusProcEx(unsigned long keyID, unsigned long xPoint, unsigned long yPoint)
		{return keyID;}
		////��Ӧ����
		//��Ӧ�����Ӵ��ڵİ�����Ϣ
		virtual unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
		{ return keyID; }
		virtual unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
		{ return keyID; }
		
		
		////���̳���д
		//��������Ϣ�����㴦��������Ӧ
		unsigned long OnKeyProc(unsigned long keyID, unsigned long xPoint, unsigned long yPoint);
		//������
		unsigned long OnFocusProc(unsigned long keyID, unsigned long xPoint, unsigned long yPoint);
		//ͨ��ID����Ӵ��ڣ�δʵ��
		CWnd* GetChild(unsigned long id) { return NULL;}
		//������ĸ���Ч�Ŀؼ���
		CWnd* InWhichActiceCtrl(unsigned long xPoint, unsigned long yPoint); 
		//����Ƿ��ڴ�����
		bool IsInTheRect(unsigned long xPoint, unsigned long yPoint) const;
		//���������ͼ
		bool CreateFocusMap();
		//��ȡ������Ŀ
		void GetFocusCountInMap(int& x, int & y) const;
		void CalculateFocusCount(int& x, int & y) const;

		//��ʾͼƬ
		//bool DrawImage(const std::string& strFile, int x, int y);
		bool DrawImage(BITMAP_ID id, int x, int y);
		bool GetImageSize(BITMAP_ID id, int& cx, int& cy) const;
		
		//////////////////////////////////////////////////////////////////////////
		void DrawClear(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bImmediate = false);
		void DrawRect(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color, bool bImmediate = false);
		void DrawRectBorder(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color, unsigned short height = 1, unsigned short width = 0, bool bImmediate = false);//���ƾ��α߿�
		void DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long color, unsigned short width = 1, bool bImmediate = false);
		unsigned long DrawText(const char *pStr, unsigned long len, unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color = COLOR_TEXT_NORMAL, bool bImmediate = false, unsigned char zoom = 1, bool bResort = true);
		void DrawImage(const unsigned char *pImage, unsigned long len, unsigned short x, unsigned short y, unsigned short width, unsigned short cx, unsigned short cy, bool bImmediate = false);
		bool TextSize(const char *pStr, unsigned long len, unsigned short &cx, unsigned short &cy, unsigned char zoom = 1);
		void Refresh(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy);
		bool ReSortText(char* pText, const char* pSor, int len);
		//////////////////////////////////////////////////////////////////////////
	
		//���������ʾ��Ϣ,��gui_assert���ʹ��.�����쳣ʱ,Dump��������Ϣ
		virtual void Dump(){};

	public://���º���ע��ʹ��
		//ɾ���ӶԻ���
		void DestroyChild();
		//�������Ƴ��Ӵ���		
		bool AddChildList(CWnd* pNode);
		bool RemoveChild(CWnd* pNode);

	private:
		//���������
		void Draw();
		void Clear();
		
		////�������
		//�Ƿ�����ý���
		bool CanSetFocus() const;
		//���ݰ��������Ӵ��ڽ���		
		//Ѱ������һ���������ý�����Ӵ���
		CWnd* MoveFocusUp(unsigned long xFocus, unsigned long yFocus) const;   
		CWnd* MoveFocusDown(unsigned long xFocus, unsigned long yFocus) const; 
		CWnd* MoveFocusLeft(unsigned long xFocus, unsigned long yFocus) const; 
		CWnd* MoveFocusRight(unsigned long xFocus, unsigned long yFocus) const;

		CWnd* MoveFocusRightEx(unsigned long xFocus, unsigned long yFocus) const;//��һ���ڣ��������ƣ�֪���пؼ����п����ý���Ϊֹ��ʧ�ܷ���NULL
		CWnd* MoveFocusLeftEx(unsigned long xFocus, unsigned long yFocus) const;
		//����Ĭ�Ͻ���
		unsigned long SetDefaultFocus(bool bRefresh = true);
		//����Ĭ�Ͻ���ģʽ
		void SetDefaultFocusMode(DEFUALT_FOCUS mode)
		{
			m_defaultFocus = mode;
		}
		//��ȡĬ�Ͻ����Ӵ���
		CWnd* GetDefaultFocusCtrl(DEFUALT_FOCUS mode);
		//���ÿؼ�����
		bool SetCtrlFocus(CWnd* pCtrl, bool bFocus, bool bRefresh = true); 
		//�Ƿ��֧�����ý���
		bool IsSupportFocus() const;
		PAINT_STATUS PaintStatus();
	public:
		////����
		static unsigned short m_width, m_height;
		static unsigned short m_length [LENGTH_NUM];
		static CStringTable	*m_pStrTable;
		
		//��������
		CRect m_showRect;//ָ������ʾ���򣬿��ܱ�ʵ��Ҫ�������С
		CRect m_rect;//��������
		bool m_bShow;//��ʾ״̬
		bool m_bFocus;//����״̬
		bool m_bEnable;//ʹ��״̬
		unsigned long m_style;//���ڷ��
		unsigned long m_windowID;//����ID
		WND_TYPE m_ctrlType;//��������
		std::string m_strCaption;//����
		BITMAP_ID m_imageID;//����ͼƬID

		////���ڽ��
		WND_LIST m_childList;//�Ӵ�������
		CWnd* m_pParent;//������

		////����
		CWnd *m_pActiveCtrl;//��Ӵ��ڣ����н����
		int m_hCtrlNum;	//ˮƽ������(֧�ֽ����)�ؼ��������Ŀ
		int m_vCtrlNum;	//��ֱ������(֧�ֽ����)�ؼ��������Ŀ
		int m_xFocus;//�������� x
		int m_yFocus;//�������� y
		CWnd**	m_pFocusMap;	//�����ͼ
		DEFUALT_FOCUS m_defaultFocus;//Ĭ�Ͻ���λ��

		//������
		//void* m_pData;
		LONGLONG m_data;
	private:
		static GUI::CFBInterface *m_pFBInterface;
		static int s_randWndID;//ϵͳ����ID

	private:
		//�����ڲ����κ���,ֻ��¼���е�Popup����
		//������ĿǰΨһ���ô���ˢ�µ�ǰ������Ļ
		static GUI::CWnd* m_pRootWnd;
	};
}
#endif

