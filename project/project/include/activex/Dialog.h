#ifndef _DIALOG_H_
#define _DIALOG_H_
#include <map>
#include "Wnd.h"
#include "KeyProc.h"
#include "CtrlID.h"
#include "Button.h"
#include "PictureCtrl.h"
#include "Static.h"

namespace GUI
{

class CDialog : public GUI::CWnd
{
public:
	CDialog(void);
	virtual ~CDialog(void);
	//DoModalʱ�����õ���Create
	virtual bool Create( unsigned long ctrlID, 
		unsigned long x, 
		unsigned long y,
		unsigned long cx, 
		unsigned long cy,
		CWnd* pParent,
		int xFocus,
		int yFocus,
		bool bVisible = true,
		const char* pCaption = NULL);

	//pWnd��pWnd�ĸ�������DoModal���ú����أ�DoModal��ɺ�ԭ��ΪNULLʱ��������
	//x��yΪ��ʾ�Ի����λ�ã����x��yΪINVALID_POS���ֱ�Ϊˮƽ���С���ֱ����
	DLG_END_VALUE DoModal(CWnd* pWnd = NULL, int x = CENTER_POS, int y = CENTER_POS, int cx = MAX_SCREEN_X, int cy = MAX_SCREEN_Y, bool bTopWndShow = true);
	
	//���ñ���ͼƬID
	void SetImage(BITMAP_ID id, bool bRefresh);

	//����Tip��ʾ
	//������ʾ��������Tip
	static void SetShowTipType( unsigned long type );
	//��ȡ��ʾ��������Tip,һ�β�ѯһ��
	static bool GetShowTipType( unsigned long type=TIP_MSG);
	//���ؼ����Tip��Ϣ(ע:��ӵ����㴰��)
	void AddTip(unsigned long ctrlID, unsigned long strID );
	//Ϊ���㴰�ڴ���Tip�ؼ�
	void CreateTipWnd(int cx=-1,int x=-1, int y=-1);
	//�޸�Tip�ĵ�ɫ
	void ChangeTipBKColor(unsigned long color);
	//����ÿ��Tip����ʾ����ɫ
	void SetTipTextColor(unsigned long color,unsigned long type=TIP_MSG);
	//////////////////////////////////////////////////////////////////////////
	static void AddKeyValue(const KEY_VALUE_INFO & keyValue);
	static void AddKeyValueEx(const KEY_VALUE_INFO & keyValue);

	virtual unsigned long ResponseHideCommond( unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos){return KEY_VIRTUAL_MSG_CONTINUE;}
protected:
	DECLARE_MSG_MAP()
	//////////////////////////////////////////////////////////////////////////
	virtual bool CreateDoModal(int x, int y, int cx, int cy);
	virtual void OnInitial();
	virtual void OnDestroy();
	virtual void OnPostInitial();
	virtual void OnPaint(PAINT_STATUS status);

	virtual void OnIdle();//���У�DoModalģʽ���ް���
	virtual void OnOutOfDlg(unsigned long keyID){};//����ڶԻ��������
	virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);
	virtual void OnExitProc() {}
	//��ӦĬ�ϰ�ť
	virtual unsigned long OnOK();
	virtual unsigned long OnCancel();
	virtual unsigned long OnApply();

	//���㷵�ضԻ���ߴ磬�ı�ߴ�����д
	virtual bool OnCalculateSize(int& cx, int& cy) const;
	//����ģʽ�Ի��� 
	virtual void EndDialog(DLG_END_VALUE result);
	virtual unsigned long OnVirtualEsc(unsigned long keyID, unsigned long xPos, unsigned long yPos);

	//������Ϣ�Ի��� type:DEFAULT_CTRL���
	GUI::DLG_END_VALUE MessageBox(const char* szText,const char* szCaption,unsigned long type = DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	//����Ĭ�Ͽؼ�, DEFAULT_CTRL��� 
	void SetDefaultBtn(unsigned long defaultBtn) { m_defaultCtrlType = defaultBtn; }
	void SetDefaultBtnCaption(DEFAULT_CTRL ctrl, const char* szCaption) { m_defaultBtnCaption[ctrl] = szCaption; }
	void SetDefaultBtnImage(DEFAULT_CTRL ctrl, BITMAP_ID normal, BITMAP_ID focus, BITMAP_ID disable);
	//���ñ�����
	//void SetTitle(bool bTitle);
	//bool IsTitle() const { return (0 == (m_style & WND_NO_TITLE)); }

	//Tip�����Ϣ,�����ȴ���Tip�ؼ�
	virtual void SetTip(){}
	GUI::DLG_END_VALUE MessageOut(const char* szText,unsigned long tipType = TIP_MSG, bool bRefresh = true,unsigned long type = DEFAULT_BTN_OK|DEFAULT_ICON_QUESTION);
	GUI::DLG_END_VALUE MessageOut(ENUM_STRING_ID strID,unsigned long tipType = TIP_MSG,bool bRefresh = true,unsigned long type = DEFAULT_BTN_OK|DEFAULT_ICON_QUESTION);
	GUI::DLG_END_VALUE MessageOut(CWnd* pWnd,ENUM_STRING_ID strID,unsigned long tipType = TIP_MSG,bool bRefresh = true,unsigned long type = DEFAULT_BTN_OK|DEFAULT_ICON_QUESTION);
	GUI::DLG_END_VALUE MessageOut(CWnd* pWnd,const char* szText,unsigned long tipType = TIP_MSG,bool bRefresh = true,unsigned long type = DEFAULT_BTN_OK|DEFAULT_ICON_QUESTION);
	
	//Ĭ�ϰ�ť����
	enum DEFAULT_BTN_ATTR
	{
		MAX_DEFAULT_BTN = 3,
	};

	GUI::CButton m_btnExit;
	GUI::CButton* m_pDefaultBtn; //Ĭ�ϰ�ť
	unsigned long m_defaultCtrlType;//Ĭ�Ͽؼ����ͣ�DEFAULT_CTRL���

	typedef std::map<DEFAULT_CTRL, std::string> DEFAULT_BTN_CAPTION;
	DEFAULT_BTN_CAPTION m_defaultBtnCaption;//Ĭ�ϰ�ť����

	struct DEFAULT_BTN_IMAGE
	{
		BITMAP_ID bmpNormal;
		BITMAP_ID bmpFocus;
		BITMAP_ID bmpDisable;
	};
	typedef std::map<DEFAULT_CTRL, DEFAULT_BTN_IMAGE> DEFAULT_BTN_IMAGELIST;
	DEFAULT_BTN_IMAGELIST m_defaultBtnImage;

	//Tip��ʾ��Ϣ
	typedef std::map<unsigned long, unsigned long> TIP_MAP;
	TIP_MAP m_tipInfo;	//�Կؼ�����ʾ��Ϣ(�ؼ�ID,�ַ���ID)
	unsigned long m_tipType;//Tip������
	bool m_tipInit;//�Ƿ񱻳�ʼ��
	bool	m_tipShow;//tip������
	int m_tipCount;//��ʱ����(��ʱδ����,Ŀǰʹ�ý����Ǩ���)
	int m_tipPosX;	//λ��x
	int m_tipPosY;	//λ��y
	unsigned long m_tipColor[TIP_COUNT];//tip��ʾ��ɫ
	GUI::CPictureCtrl m_picTip;	//tip��ʾͼƬ�ؼ�
	GUI::CStatic m_scTip;//tip�ַ���ʾ�ؼ�
	BITMAP_ID	m_idBmpTip;//tip��ʾͼƬ:����,����,ѯ��
	static unsigned long m_cfgTip;//����tip�Ƿ����

	//////////////////////////////////////////////////////////////////////////
	bool m_bInModal;			//�Ƿ�ģʽ�״̬
	DLG_END_VALUE	m_endValue;	//�˳��Ի���ķ���ֵ

	//////////////////////////////////////////////////////////////////////////
	KEY_MODE		m_keyMode;	//����̨���ơ��طŽ����ϸ�����ֵ
	bool			m_bTenAdd;	//��disktop���洦��10+����
	MESSAGE_TYPE	m_lastMsgType;	//��¼�ϴ��������Ϣ����
	unsigned long	m_tenAddTime;

	bool			m_bAddAdd;	//��disktop���洦��10+����
	bool			m_bPreset;	//��ͬΪ˿ӡ������Ԥ�õ㰴��״̬
	MESSAGE_TYPE	m_lastMsgAddAddType;	//��¼�ϴ��������Ϣ����
	unsigned long	m_addAddTime;
	bool            m_bAddFirstKey;
	unsigned long   m_addAddFirstKey;

	static  unsigned long	m_CommandPram; //�洢�ⲿ��������Ĳ����������Ԥ�õ�
	static	LONGLONG	m_showTime;
	static  LONGLONG	m_lastShowTime;
private:
	void CreateDefaultBtn();//����Ĭ�ϰ�ť
	void ShowTip(bool bShow, const char* szText, BITMAP_ID bmpID, unsigned long color, bool bRefresh = true );
	//////////////////////////////////////////////////////////////////////////
	bool GetKeyValue(KEY_VALUE_INFO & keyValue);
	//////////////////////////////////////////////////////////////////////////
	static std::list<KEY_VALUE_INFO> m_keyValueList;
	static CPUB_Lock m_keyListLock;//����������
};


}
#endif


