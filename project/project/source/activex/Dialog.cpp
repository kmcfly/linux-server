#include "Dialog.h"
#include "CtrlID.h"
#include "MessageBox.h"
#include "MessageMan.h"

extern DVR_SYSTEM_STATUS g_system_status;
extern VGA_RESOLUTION g_resolution;
extern bool g_bChangOutDevice;
extern bool g_bLcdScreemOn = true;
using namespace GUI;
std::list<KEY_VALUE_INFO> GUI::CDialog::m_keyValueList;
unsigned long GUI::CDialog::m_CommandPram = 0;
LONGLONG	GUI::CDialog::m_showTime = 0;
LONGLONG	GUI::CDialog::m_lastShowTime = 0;
CPUB_Lock GUI::CDialog::m_keyListLock;

#if defined(__TW01_RILI__)
unsigned long GUI::CDialog::m_cfgTip = TIP_NULL;
#else
unsigned long GUI::CDialog::m_cfgTip = TIP_ALL;
#endif

//////////////////////////////////////////////////////////////////////////
extern unsigned long COLOR_TEXT_TITLE;

GUI::CDialog::CDialog(void) : m_bInModal(false)
, m_defaultCtrlType(0)
, m_keyMode(KEY_MODE_UI)
, m_pDefaultBtn(NULL)
, m_tipPosX(-1)
, m_tipPosY(-1)
, m_tipInit(false)
, m_bTenAdd(false)
, m_lastMsgType(KEY_TYPE_VIRTUAL)
{
	m_ctrlType	= GUI::WND_TYPE_DIALOG;
	//m_imageID	= BMP_INVALID;
	//m_style		= WND_POPUP | WND_VISIBLE | WND_NO_IMAGE_BK;
	m_bAddAdd = false;
	m_bPreset = false;
	m_lastMsgAddAddType = KEY_TYPE_VIRTUAL;
	m_bAddFirstKey = false;
	m_addAddFirstKey = 0;
}

GUI::CDialog::~CDialog(void)
{
}

bool GUI::CDialog::Create(unsigned long ctrlID, 
						  unsigned long x, 
						  unsigned long y,
						  unsigned long cx, 
						  unsigned long cy, 
						  CWnd* pParent, 
						  int xFocus, 
						  int yFocus, 
						  bool bVisible/* = ture*/,
						  const char* pCaption /* = NULL */)
{
	//设置对话框风格
	unsigned long style = WND_CHILD | WND_NO_IMAGE_BK;
	style |= (bVisible ? WND_VISIBLE : 0);

	//创建窗口
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus))
	{
		if (NULL != pCaption)
		{
			SetCaption(pCaption, false);
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool GUI::CDialog::CreateDoModal(int x, int y, int cx, int cy)
{
	//风格
	unsigned long style = WND_POPUP /*| WND_VISIBLE*/ | WND_NO_IMAGE_BK;

	if (MAX_SCREEN_X == cx)
	{
		cx =  CFBInterface::Instance()->GetResolutionWidth();
	}
	else
	{
		assert(cx > 0);
		//调整显示位置，使之不超出屏幕范围
		if (x + cx > m_width)
		{
			x = m_width - cx - 3;
		}
	}

	if (MAX_SCREEN_Y == cy)
	{
		cy = CFBInterface::Instance()->GetResolutionHeight();
	}
	else
	{
		assert(cy > 0);
		//调整显示位置，使之不超出屏幕范围
		if (y + cy > m_height)
		{
			y = y - cy - 1 - m_length[BTN_HEIGHT];//偏移加上了按钮的默认高度
		}
	}

	unsigned short right = CFBInterface::Instance()->GetResolutionWidth()-1;
	unsigned short bottom = CFBInterface::Instance()->GetResolutionHeight()-1;

	CRect rect(0, 0, right, bottom);
	CRect rcThis = rect.GetCenterRect(cx, cy);
	if (CENTER_POS != x)
	{
		rcThis.MoveToX(x);
	}
	else
	{
		style |= WND_CENTER_HORIZONTAL;
	}

	if (CENTER_POS != y)
	{
		rcThis.MoveToY(y);
	}
	else
	{
		style |= WND_CENTER_VERTICAL;
	}

	//创建窗口
	if (GUI::CWnd::Create(CTRLID_DOMODAL_DLG, rcThis.m_left, rcThis.m_top, rcThis.Width(), rcThis.Height(), NULL, style, 0, 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CDialog::OnInitial()
{
	GUI::CWnd::OnInitial();

	//设置默认按钮标题
	m_defaultBtnCaption[DEFAULT_BTN_OK] = m_pStrTable->String(E_STR_ID_OK);
	m_defaultBtnCaption[DEFAULT_BTN_CANCEL] = m_pStrTable->String(E_STR_ID_CANCEL);
	m_defaultBtnCaption[DEFAULT_BTN_APPLY] = m_pStrTable->String(E_STR_ID_APPLY);
}

void GUI::CDialog::OnDestroy()
{
	GUI::CWnd::OnDestroy();

	if (NULL != m_pDefaultBtn)
	{
		delete [] m_pDefaultBtn;
		m_pDefaultBtn = NULL;
	}

	m_tipInfo.clear();
}

void GUI::CDialog::OnPostInitial() 
{
	CreateDefaultBtn();

	if ((0 == (WND_NO_TITLE & m_style)) && (0 == (WND_CHILD & m_style)))
	{
		m_btnExit.Create(CTRLID_DEFAULT_BTN_CANCEL, 
			m_rect.Width() - m_length[TITLE_EXIT_BTN_RIGHT_XPOS], 
			m_length[TITLE_EXIT_BTN_YPOS],
			m_length[TITLE_EXIT_BTN_WIDTH], 
			m_length[TITLE_EXIT_BTN_HEIGHT],
			this, 0, 0);

		m_btnExit.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_EXIT, false);
		m_btnExit.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_EXIT_F, false);
	}

	SetTip();
}

//注意bTopWndShow的意义
GUI::DLG_END_VALUE GUI::CDialog::DoModal(CWnd* pWnd /* = NULL */, int x /* = CENTER_POS */, int y /* = CENTER_POS */,\
											int cx/* = MAX_SCREEN_X*/, int cy/* = MAX_SCREEN_Y*/, bool bTopWndShow/* = true*/)
{
	//隐藏上层对话框
	CWnd* pTopWnd = NULL;
	if (NULL != pWnd)
	{
		pTopWnd = pWnd->GetTopestWnd();
	}

	if ((NULL != pTopWnd) && !bTopWndShow)
	{
		//此处bTopWndShow的意义为指定其顶层窗体实现继续显示。
		pTopWnd->Show(bTopWndShow, false);
	}

	//2010-01-27 14:37:00 YSW
	//由于某些对话框（比如备份、回放），在初始化对话框后采用发布一个消息（就是备份和播放的虚拟按键值）来启动业务
	//而此处的删除只是为了把当前对话框未响应前累积在列表中的消息清空，所以一定要放在CreateDoModal前面，否则会影响到
	//上面相关地功能实现。
	//删除按键列表中的已有消息，因为可能存在鼠标消息来得频繁，处理过多的情况
	m_keyListLock.Lock();
	m_keyValueList.clear();
	m_keyListLock.UnLock();

	//创建窗口
	if (!CreateDoModal(x, y, cx, cy))
	{
		assert(false);
		return DLG_ESC;
	}
	else
	{
		GetRootWnd()->AddChildList(this);
	}

	//2010-03-13 11:30:00 YSW
	//当有顶层窗体，并顶层窗体比当前窗体大，且需要隐藏时
	//应该统一进行刷新界面，才能保证不残留一些顶层窗体的东西。
	if ((NULL != pTopWnd) && !bTopWndShow)
	{
		GUI::CRect rcTop = pTopWnd->GetWindowRect();

		if ((m_showRect.m_left < rcTop.m_left) && \
			(rcTop.m_right < m_showRect.m_right) && \
			(m_showRect.m_top < rcTop.m_top) && \
			(rcTop.m_bottom < m_showRect.m_bottom))
		{
			//当前窗体能覆盖顶层窗体，且顶层窗体需要隐藏，则可以直接显示当前窗体（可把顶层窗体遮盖）。
			Show(true);
		}
		else
		{
			//当前窗体不能覆盖整个顶层窗体，且顶层窗体需要隐藏时，则需要刷新全部。
			pTopWnd->Show(false, false);

			Show(true, false);

			GetRootWnd()->Repaint();
		}
	}
	else
	{
		//显示本窗口
		Show(true);
	}

	KEY_VALUE_INFO keyValue;
	m_bInModal = true;
	while(m_bInModal && (DVR_SYSTEM_RUNING == g_system_status))
	{	
		if ( GetKeyValue(keyValue) )
		{
			unsigned long keyID = CKeyProc::TranslateKey(m_keyMode, keyValue);//按键转换
			
			//2010-04-12 15:16:00 YSW
			if (KEY_VIRTUAL_REDRAW == keyID)
			{
				g_bChangOutDevice = true;
				g_system_status = DVR_SYSTEM_REDRAW;
				continue;
			}

			if (KEY_VIRTUAL_LCD_ON_OFF == keyID)
			{
				bool bTemp = g_bLcdScreemOn;
				g_bLcdScreemOn = !bTemp;
				printf("%s,%d,KEY_VIRTUAL_LCD_ON_OFF == keyID,g_bLcdScreemOn:%d => %d\n",__FILE__,__LINE__,bTemp, g_bLcdScreemOn);
				continue;
			}

			//2010-03-13 09:39:00 YSW
			//处理组合键10+
			{
				//如果按了10+（5秒）后，再次输入，则不保留10+状态。
				if (m_bTenAdd && (5 < (GetCurrTime32() - m_tenAddTime)))
				{
					m_bTenAdd = false;
				}

				//如果输入10+
				if(KEY_VIRTUAL_10_A == keyID)
				{
					//原来为非10+状态，则需要修改为10+状态，且及时返回读取下一次输入。
					if (!m_bTenAdd)
					{
						m_bTenAdd = true;

						m_lastMsgType = static_cast<MESSAGE_TYPE>(keyValue.type);

						m_tenAddTime = GetCurrTime32();
						continue;
					}

					//原来为10+状态，且上次10+的输入类型和本次相同，则需要修改为10.
					if (m_lastMsgType == static_cast<MESSAGE_TYPE>(keyValue.type))
					{
						keyID = KEY_VIRTUAL_10;//注意下面的处理，此处把keyID值做了修改。
					}	
				}

				//10+状态，且是需要处理10以上的数字按键（两次输入类型相同），则修正keyID值。
				if (m_bTenAdd && (KEY_VIRTUAL_0 <= keyID) && (keyID <= KEY_VIRTUAL_6) && (m_lastMsgType == static_cast<MESSAGE_TYPE>(keyValue.type)))
				{
					keyID = (KEY_VIRTUAL_10 + (keyID - KEY_VIRTUAL_0));
				}

				//到此，如果是需要处理10+状态的已经处理，再就是不需要处理的，其10+状态都应该消除。
				m_bTenAdd = false;
			}

			{
				//如果按了++（5秒）后，再次输入，则不保留++状态。
				if (m_bAddAdd && (5 < (GetCurrTime32() - m_addAddTime)))
				{
					m_bAddAdd = false;
				}

				//如果输入++
				if(KEY_VIRTUAL_A_A == keyID)
				{					
					if (!m_bAddAdd)
					{
						printf("start KEY_VIRTUAL_A_A\n");
						m_bAddAdd = true;
						m_lastMsgAddAddType = static_cast<MESSAGE_TYPE>(keyValue.type);
						m_addAddTime = GetCurrTime32();
						m_bAddFirstKey = true;
						m_addAddFirstKey = 0;
						continue;
					}
					else if (m_lastMsgAddAddType == static_cast<MESSAGE_TYPE>(keyValue.type))
					{
						printf("KEY_VIRTUAL_A_A => KEY_VIRTUAL_0\n");
						keyID = KEY_VIRTUAL_0;
					}
					else
					{
						printf("KEY_VIRTUAL_A_A => error\n");
						m_bAddAdd = false;
						continue;
					}
				}

				//
				if (m_bAddAdd && m_bAddFirstKey && (KEY_VIRTUAL_0 <= keyID) && (keyID <= KEY_VIRTUAL_9) && (m_lastMsgAddAddType == static_cast<MESSAGE_TYPE>(keyValue.type)))
				{					
					m_addAddFirstKey = keyID - KEY_VIRTUAL_0;
					m_bAddFirstKey = false;
					printf("after KEY_VIRTUAL_A_A first keyID:%d,m_addAddFirstKey:%d\n",keyID,m_addAddFirstKey);
					continue;
				}
				if (m_bAddAdd && !m_bAddFirstKey && (KEY_VIRTUAL_0 <= keyID) && (keyID <= KEY_VIRTUAL_9) && (m_lastMsgAddAddType == static_cast<MESSAGE_TYPE>(keyValue.type)))
				{
					unsigned long tempKeyID = 0;
					tempKeyID = m_addAddFirstKey * 10 + (keyID - KEY_VIRTUAL_0);
					printf("after KEY_VIRTUAL_A_A second keyID:%d,tempKeyID:%d ",keyID,tempKeyID);
					if ((tempKeyID > 32) || (tempKeyID <= 0))
					{
						printf("  ==> error,>32 || <=0\n");
						m_bAddAdd = false;
						continue;
					}					
					keyID = KEY_VIRTUAL_0 + tempKeyID;					
					printf(" ===> keyID:%d\n",keyID);
				}

				//到此，如果是需要处理10+状态的已经处理，再就是不需要处理的，其10+状态都应该消除。
				m_bAddAdd = false;
			}

			{
				if ((KEY_VIRTUAL_PRESET == keyID) && (KEY_TYPE_TWSY == keyValue.type))
				{
					m_bPreset = true;
					m_CommandPram = 0x00;
					continue;
				}
				if(m_bPreset)
				{
					if ((KEY_VIRTUAL_0 <= keyID) && (KEY_VIRTUAL_9)>= keyID)
					{
						m_CommandPram = ( m_CommandPram * 10 );
						m_CommandPram += (keyID - KEY_VIRTUAL_0);
						continue;
					}
					else if( KEY_VIRTUAL_ENTER == keyID)
					{
						keyID = KEY_VIRTUAL_PRESET;
						m_CommandPram |= (keyValue.type << 8);
						m_bPreset = false;
					}
					else
					{
						m_bPreset = false;
						continue;
					}
				}
			}
			{
				if( KEY_TYPE_MY1003 == keyValue.type )
				{
					//记录名扬键盘传递来的参数
					//value 中0-7八位数据是键值，8-15八位数据是该键值操作的参数
					m_CommandPram = ( keyValue.value >> 8 );
					m_CommandPram |= ( keyValue.type << 8 );
				}
				else if (KEY_TYPE_TWSY != keyValue.type)
				{
					//不是名扬键盘时只存放按键的类型
					m_CommandPram = 0x0;
					m_CommandPram |= ( keyValue.type << 8 );
				}
			}

			if ((KEY_VIRTUAL_ESC == keyID) || (KEY_VIRTUAL_EXIT == keyID)) //首先响应ESC键，直接结束对话框
			{
				if (KEY_TYPE_MOUSE == keyValue.type)
				{

				}
				else if (KEY_TYPE_VIRTUAL == keyValue.type)
				{
					keyValue.xPos = m_width / 2;
					keyValue.yPos = m_height / 2;
				}
				else
				{
					keyValue.xPos = INVALID_INDEX(unsigned long);
					keyValue.yPos = INVALID_INDEX(unsigned long);
				}

				unsigned long retVirEsc = OnVirtualEsc(keyID, keyValue.xPos, keyValue.yPos);
				if ((KEY_VIRTUAL_ESC == retVirEsc) || (KEY_VIRTUAL_EXIT == retVirEsc))
				{
					EndDialog(DLG_ESC);
				}
			}
			else
			{
				if (KEY_TYPE_MOUSE == keyValue.type)
				{
					if (IsInTheRect(keyValue.xPos, keyValue.yPos))
					{	//如果光标在窗口上，进行坐标转换，相对与窗口
					//	ScreenToClient(keyValue.xPos, keyValue.yPos);
					} 
					else
					{	//如果光标不再窗口上
						OnOutOfDlg(keyID);

						//PUB_Sleep(20);
						continue;
					}
				} 
				else if (KEY_TYPE_VIRTUAL == keyValue.type)
				{
					keyValue.xPos = m_width / 2;
					keyValue.yPos = m_height / 2;
				}
				else
				{
					if ( KEY_TYPE_MY1003 == keyValue.type)
					{
						keyValue.xPos = KEY_MY1003_POS;
						keyValue.yPos = KEY_MY1003_POS;
					}
					else
					{
						keyValue.xPos = INVALID_INDEX(unsigned long);
						keyValue.yPos = INVALID_INDEX(unsigned long);
					}
				}
				
				//处理子窗口焦点,移动焦点
				if (KEY_VIRTUAL_MSG_STOP == OnFocusProc(CKeyProc::TranslateKey(KEY_MODE_UI, keyValue), keyValue.xPos, keyValue.yPos))
				{
					continue;
				}

				if (KEY_VIRTUAL_MSG_STOP == OnKeyProc(keyID, keyValue.xPos, keyValue.yPos))//按键处理
				{
					//PUB_Sleep(20);
					continue;
				}			
			}
		}
		else
		{	
			OnIdle();

			//2010-01-26 11:57:00 YSW
			//由于存在着要在任何界面时，在背景部分（录像标记、视频丢失标记等等）需要刷新时，必须刷新整个现场。
			{
				m_showTime = GetCurrTime();
#if defined(__TD2316SES_SPEC__) || defined(__TDFH__)
				if (((m_showTime - m_lastShowTime) > 500000) || (m_showTime < m_lastShowTime))
#else
				if (((m_showTime - m_lastShowTime) > 250000) || (m_showTime < m_lastShowTime))
#endif
				{
					m_lastShowTime = m_showTime;

					if (CMessageMan::Instance()->RefreshLive())
					{
						GetRootWnd()->Repaint();
					}	
				}
			}

#if defined(__TDFH__)
			PUB_Sleep(10);
#endif
		}
#ifndef __TDFH__
		PUB_Sleep(10);
#endif
	}

	GetRootWnd()->RemoveChild(this);

	//自身隐藏，只是绘制，但是不刷新在显示区。
	Show(false, false);

	Destroy();

	if (NULL != pTopWnd)
	{
		GUI::CRect rcTop = pTopWnd->GetWindowRect();

		if ((rcTop.m_left <= m_showRect.m_left) && \
			(m_showRect.m_right <= rcTop.m_right) && \
			(rcTop.m_top <= m_showRect.m_top) && \
			(m_showRect.m_bottom <= rcTop.m_bottom))
		{
			//当当前DoModal的窗体整个区域在顶层窗体内部时，则重新绘制顶层窗体（不包括当前DoModal的窗体），然后刷新
			//当前DoModal窗体覆盖的区域。
			pTopWnd->Show(true, false);

			//不需要刷新整个窗体，只要刷新topWnd对应的区域就行了。
			Refresh(m_showRect.m_left, m_showRect.m_top, m_showRect.Width(), m_showRect.Height());
		}
		else
		{
			if (!pTopWnd->IsShow())
			{
				pTopWnd->Show(true, false);
			}

			//刷新整个窗体
			GetRootWnd()->Repaint();
		}
	}
	else
	{
		//刷新整个窗体
		GetRootWnd()->Repaint();
	}

	//删除按键列表中的已有消息，因为可能存在鼠标消息来得频繁，处理过多的情况
	m_keyListLock.Lock();
	m_keyValueList.clear();
	m_keyListLock.UnLock();

	//在界面退出以后，做一些事情
	OnExitProc();

	return m_endValue;
}

void GUI::CDialog::OnPaint(PAINT_STATUS status)
{
	if (BMP_INVALID != m_imageID)
	{
		assert (0 == (WND_NO_IMAGE_BK & m_style));
		//背景图片
		DrawImage(m_imageID, m_rect.m_left, m_rect.m_top);
	}
	else if (WND_CHILD & m_style)
	{
		GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_DLG_CHILD_BK);
		GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_SELECT, 2);

		////上横线
		//GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_right, m_rect.m_top, COLOR_SELECT, 2);
		////左竖线
		//GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_left, m_rect.m_bottom, COLOR_SELECT, 2);

		////右竖线
		//GUI::CWnd::DrawLine(m_rect.m_right, m_rect.m_top, m_rect.m_right, m_rect.m_bottom, COLOR_SELECT, 2);
		////下横线
		//GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_bottom, m_rect.m_right, m_rect.m_bottom, COLOR_SELECT, 2);
	}
	else if (WND_NO_IMAGE_BK & m_style)
	{
		GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_DLG_CHILD_BK);

		if (0 == (WND_NO_TITLE & m_style))
		{
			GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top+m_length[TITLE_HEIGHT], m_rect.m_left+m_rect.Width()-1, m_rect.m_top+m_length[TITLE_HEIGHT], COLOR_LINE_GRAY, 1);
		}

		//上横线
		GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_right, m_rect.m_top, COLOR_LINE_GRAY, 1);
		//左竖线
		GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_left, m_rect.m_bottom, COLOR_LINE_GRAY, 1);
		//右竖线
		GUI::CWnd::DrawLine(m_rect.m_right, m_rect.m_top, m_rect.m_right, m_rect.m_bottom, COLOR_LINE_BLACK, 1);
		//下横线
		GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_bottom, m_rect.m_right, m_rect.m_bottom, COLOR_LINE_BLACK, 1);
	}
	else 
	{
		
	}

	//显示标题
	if (!m_strCaption.empty())
	{
		unsigned long x = m_length[TITLE_CAPTION_XPOS];
		unsigned long y = m_length[TITLE_CAPTION_YPOS];
		GUI::CWnd::ClientToScreen(x, y);
		GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), x, y, m_rect.m_left + m_rect.Width() - x - m_length[TITLE_EXIT_BTN_RIGHT_XPOS] - m_length[EDIT_XPOS], m_rect.m_top + m_rect.Height() - y, COLOR_TEXT_TITLE);
	}	
}

//void GUI::CDialog::SetTitle(bool bTitle)
//{
//	if (bTitle)
//	{
//		m_style &= (~WND_NO_TITLE);
//	} 
//	else
//	{
//		m_style |= WND_NO_TITLE;
//	}
//}

/***********************************************************
函数名：  EndDialog
功能描述：结束对话框；
输入参数：result：结束码；
输出参数：无；
返回值：  无；

创建者/修改者                   创建/修改日期                  备注
黄展                            28/08/07                       创建
***********************************************************/
void GUI::CDialog::EndDialog(DLG_END_VALUE result)
{
	m_endValue = result;
	m_bInModal = false;

	//必须锁住，否则可能存在刚调用了EndDialog()，
	//但DoModal()还未退出，且刚刚检查了列表不为空，然后去获取头，于是出现
	//对空列表进行RemoveHead().
	/*m_keyLock.Lock();

	m_keyValueList.RemoveAll();

	m_keyLock.UnLock();*/
}

unsigned long GUI::CDialog::OnVirtualEsc(unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	return keyID;
}

void GUI::CDialog::AddKeyValue(const KEY_VALUE_INFO & keyValue)
{
	m_keyListLock.Lock();
#if defined(__CHIPGM__)
	if (8 > m_keyValueList.size())
#else 
	if (5 > m_keyValueList.size())
#endif
	{
		m_keyValueList.push_back(keyValue);
	}
	m_keyListLock.UnLock();
}


void GUI::CDialog::AddKeyValueEx(const KEY_VALUE_INFO & keyValue)
{
	m_keyListLock.Lock();
	m_keyValueList.push_back(keyValue);
	m_keyListLock.UnLock();
}

//////////////////////////////////////////////////////////////////////////
bool GUI::CDialog::GetKeyValue(KEY_VALUE_INFO & keyValue)
{
	m_keyListLock.Lock();
	if (0 < m_keyValueList.size())
	{
		keyValue = m_keyValueList.front();
		m_keyValueList.pop_front();

		m_keyListLock.UnLock();
		return true;
	}
	else
	{
		m_keyListLock.UnLock();
		return false;
	}
}

bool GUI::CDialog::OnCalculateSize(int& cx, int& cy) const
{
	if (WND_NO_IMAGE_BK & m_style)
	{
		cx = (0 == cx)? m_length[RECT_WIDTH] : cx;
		cy = (0 == cy)? m_length[RECT_HEIGHT] : cy;

		return true;
	}
	else
	{
		assert (BMP_INVALID != m_imageID);
		return GetImageSize(m_imageID, cx, cy);
	}
}

void GUI::CDialog::CreateDefaultBtn()
{
	unsigned long defaultBtn = m_defaultCtrlType;
	if (0 == defaultBtn)
	{
		return;
	}
	assert(NULL == m_pDefaultBtn);
	//计算默认按钮数目
	m_pDefaultBtn = new GUI::CButton[MAX_DEFAULT_BTN];
	//计算默认按钮参数
	int xFocus = 0;
	int yFocus = 0;
	CalculateFocusCount(xFocus, yFocus);
	xFocus = MAX_DEFAULT_BTN;

	int x = m_rect.m_right - m_length[DEFAULT_BTN_RIGHT_XPOS];
	int y = m_rect.m_bottom - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	int offset_x = m_length[DEFAULT_BTN_OFFSET];
	int bmp_cx = 0;
	int bmp_cy = 0;
	ScreenToClient(x, y);


	DEFAULT_CTRL btnType[MAX_DEFAULT_BTN] = { DEFAULT_BTN_CANCEL, DEFAULT_BTN_OK, DEFAULT_BTN_APPLY};
	CTRLID btnID[MAX_DEFAULT_BTN] = {CTRLID_DEFAULT_BTN_CANCEL, CTRLID_DEFAULT_BTN_OK, CTRLID_DEFAULT_BTN_APPLY};
	std::string strCaption[MAX_DEFAULT_BTN];

	strCaption[0] = m_pStrTable->String(E_STR_ID_CANCEL);
	strCaption[1] = m_pStrTable->String(E_STR_ID_OK);
	strCaption[2] = m_pStrTable->String(E_STR_ID_APPLY);

	DEFAULT_BTN_CAPTION::const_iterator iter;
	DEFAULT_BTN_IMAGELIST::const_iterator iter_image;

	for (int index = 0; index < MAX_DEFAULT_BTN; ++index)
	{
		if (btnType[index] & defaultBtn)
		{
			assert(index < MAX_DEFAULT_BTN);
			
			iter_image = m_defaultBtnImage.find(btnType[index]);
			if (iter_image != m_defaultBtnImage.end())
			{
				GetImageSize(iter_image->second.bmpNormal, bmp_cx, bmp_cy);

				m_pDefaultBtn[index].Create(btnID[index], x - bmp_cx, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, xFocus, yFocus);

				m_pDefaultBtn[index].SetImage(PAINT_STATUS_NORMAL, iter_image->second.bmpNormal, false);
				m_pDefaultBtn[index].SetImage(PAINT_STATUS_FOCUS, iter_image->second.bmpFocus, false);
				m_pDefaultBtn[index].SetImage(PAINT_STATUS_DISABLE, iter_image->second.bmpDisable, false);
			} 
			else
			{
				iter = m_defaultBtnCaption.find(btnType[index]);
				if (iter != m_defaultBtnCaption.end())
				{
					m_pDefaultBtn[index].Create(btnID[index], x - m_length[BTN_WIDTH], y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, xFocus, yFocus, iter->second.c_str());
				}
				else
				{
					assert(false);
				}
			}
			
			x -= (m_pDefaultBtn[index].GetWindowRect().Width() + offset_x);
			--xFocus;
		}
	}
}

GUI::DLG_END_VALUE GUI::CDialog::MessageBox(const char* szText,const char* szCaption,unsigned long type/* = DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL*/)
{
	GUI::CMessageBox Box(szText, szCaption, type);
	return Box.DoModal(this);
}

GUI::DLG_END_VALUE GUI::CDialog::MessageOut(const char* szText,unsigned long tipType/* = TIP_MSG*/,bool bRefresh,unsigned long type /*= DEFAULT_ICON_QUESTION*/ )
{
	//只有当启用相应的设置才输出信息
	if( ( m_tipInit && (m_cfgTip&TIP_DFOCUS) && (tipType&TIP_DFOCUS) ) ||
		( m_tipInit && (m_cfgTip&TIP_MSG) && (tipType&TIP_MSG) ) ||
		( (m_cfgTip&TIP_ERR) && (tipType&TIP_ERR) ) )
	{
		BITMAP_ID imageID = m_idBmpTip;

		if( DEFAULT_ICON_ERROR & type )
		{
			imageID = BMP_ICON_INFO;
		}

		if( DEFAULT_ICON_WARNING & type )
		{
			imageID = BMP_ICON_INFO;
		}

		if( DEFAULT_ICON_QUESTION & type )
		{
			imageID = BMP_ICON_INFO;
		}

		m_tipType = TIP_MSG;
		int colorid = 2;
		if( tipType & TIP_DFOCUS )
		{
			m_tipType = TIP_DFOCUS;
			colorid = 1;
		}
		if(tipType & TIP_MSG)
		{
			m_tipType = TIP_MSG;
			colorid = 2;
		}
		if(tipType & TIP_ERR)
		{
			m_tipType = TIP_ERR;
			colorid = 3;
		}

		m_tipShow = true;
		m_tipCount = 1000;

		ShowTip(m_tipShow,szText,imageID,m_tipColor[colorid],bRefresh);

		return DLG_OK;
	}

	//只有是错误信息Tip但未设置时,弹出Box输出信息
	if( tipType&TIP_ERR )
	{
		std::string szCaption = m_pStrTable->String(E_STR_ID_TITLE_WARNING);
		if( DEFAULT_ICON_QUESTION & type ||
			DEFAULT_ICON_WARNING & type)
		{
			szCaption = m_pStrTable->String(E_STR_ID_TITLE_WARNING);
		}
		if( DEFAULT_ICON_ERROR & type )
		{
			szCaption = m_pStrTable->String(E_STR_ID_TITLE_ERROR);
		}

		//默认确认按钮
		unsigned long dlgtype = type;
		if( dlgtype & DEFAULT_BTN_OK |
			dlgtype & DEFAULT_BTN_CANCEL |
			dlgtype & DEFAULT_BTN_APPLY)
		{
			//设置了默认按钮,不做处理
		}
		else
		{
			dlgtype |= DEFAULT_BTN_OK;
		}

		GUI::CMessageBox box(szText, szCaption.c_str(), dlgtype);
		return box.DoModal(this);
	}

	return DLG_OK;
}

GUI::DLG_END_VALUE GUI::CDialog::MessageOut(ENUM_STRING_ID strID,unsigned long tipType,bool bRefresh,unsigned long type /*= DEFAULT_ICON_QUESTION*/ )
{
	return MessageOut(m_pStrTable->String(strID),tipType,bRefresh,type);
}

GUI::DLG_END_VALUE GUI::CDialog::MessageOut(CWnd* pWnd,ENUM_STRING_ID strID,unsigned long tipType,bool bRefresh,unsigned long type /*= DEFAULT_ICON_QUESTION*/)
{
	GUI::CDialog* pTopWnd = dynamic_cast<GUI::CDialog*>(pWnd);
	assert(pTopWnd);
	return pTopWnd->MessageOut(strID,tipType,bRefresh,type);
}

GUI::DLG_END_VALUE GUI::CDialog::MessageOut(CWnd* pWnd,const char* szText,unsigned long tipType,bool bRefresh,unsigned long type /*= DEFAULT_ICON_QUESTION*/)
{
	GUI::CDialog* pTopWnd = dynamic_cast<GUI::CDialog*>(pWnd);
	assert(pTopWnd);
	return pTopWnd->MessageOut(szText,tipType,bRefresh,type);
}

BEGIN_MSG_MAP(GUI::CDialog, GUI::CWnd)
	ON_MSG(CTRLID_DEFAULT_BTN_OK, KEY_VIRTUAL_ENTER, OnOK)
	ON_MSG(CTRLID_DEFAULT_BTN_CANCEL, KEY_VIRTUAL_ENTER, OnCancel)
	ON_MSG(CTRLID_DEFAULT_BTN_APPLY, KEY_VIRTUAL_ENTER, OnApply)
END_MSG_MAP()

unsigned long GUI::CDialog::OnOK()
{
	EndDialog(DLG_OK);
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CDialog::OnCancel()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CDialog::OnApply()
{
	return KEY_VIRTUAL_MSG_CONTINUE;
}

void GUI::CDialog::SetImage(BITMAP_ID id, bool bRefresh)
{
	m_style &= ~(WND_NO_IMAGE_BK);
	m_imageID = id;

	//计算尺寸
	int cx = 0, cy = 0;
	if (GetImageSize(m_imageID, cx, cy))
	{
		m_rect.Modify(m_rect.m_left, m_rect.m_top, cx, cy);

		unsigned short right = CFBInterface::Instance()->GetResolutionWidth()-1;
		unsigned short bottom = CFBInterface::Instance()->GetResolutionHeight()-1;

		CRect rcScreen(0, 0, right, bottom);
		CRect rcCenter = rcScreen.GetCenterRect(m_rect.Width(), m_rect.Height());

		if ((m_style & WND_CENTER_VERTICAL) && (m_style & WND_CENTER_HORIZONTAL))//将窗口置中
		{
			m_rect = rcCenter;
		} 
		else 
		{
			if (m_style & WND_CENTER_VERTICAL)
			{
				m_rect.MoveToY(rcCenter.m_top);
			} 
			else if (m_style & WND_CENTER_HORIZONTAL)
			{
				m_rect.MoveToX(rcCenter.m_left);
			}
		}

		m_showRect = m_rect;

		if (bRefresh)
		{
			Repaint();
		}
	}
	else
	{
		assert(false);
	}
}

void GUI::CDialog::OnIdle()
{
	//遍历所有子对话框
	WND_LIST::iterator iter;
	for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		CDialog* pDlg = dynamic_cast<CDialog*>(*iter);
		if (NULL != pDlg)
		{
			pDlg->OnIdle();
		}
	}
}

void GUI::CDialog::SetDefaultBtnImage(DEFAULT_CTRL ctrl, BITMAP_ID normal, BITMAP_ID focus, BITMAP_ID disable)
{
	DEFAULT_BTN_IMAGE btnImage;
	btnImage.bmpNormal = normal;
	btnImage.bmpFocus = focus;
	btnImage.bmpDisable = disable;

	m_defaultBtnImage[ctrl] = btnImage;
}

void GUI::CDialog::SetShowTipType( unsigned long type )
{
	m_cfgTip = type;
}

bool GUI::CDialog::GetShowTipType(unsigned long type)
{
	return (m_cfgTip & type);
}

void GUI::CDialog::AddTip(unsigned long ctrlID, unsigned long strID )
{
	GUI::CDialog* pTopWnd = dynamic_cast<GUI::CDialog*>(GetTopestWnd());
	assert(pTopWnd);

	if( pTopWnd )
	{
		pTopWnd->m_tipInfo.insert(std::make_pair(ctrlID,strID));
	}
}

void GUI::CDialog::CreateTipWnd(int cx,int x, int y)
{
	m_tipPosX = x;
	m_tipPosY = y;

	if (m_tipPosX < 0)
	{
		m_tipPosX = m_length[STATUS_INFO_XPOS];
	}

	if (m_tipPosY < 0)
	{
		m_tipPosY = m_rect.Height() - m_length[STATUS_INFO_BOTTOM_YPOS];
	}

	int width = cx-24;
	if(width<0)
	{
		width = m_rect.Width() - m_tipPosX -  3 * m_length[BTN_WIDTH] - m_length[DEFAULT_BTN_RIGHT_XPOS] - 2 * m_length[DEFAULT_BTN_OFFSET] - 28;
	}

#ifndef __CHIP3515__
	if(!g_langRightOrder)
	{
		m_picTip.Create(GetRandWndID(),m_tipPosX,m_tipPosY,this,BMP_ICON_INFO,false);
		m_scTip.Create(GetRandWndID(),m_tipPosX+24,m_tipPosY,width,m_length[STATIC_HEIGHT],this,NULL);
	}
	else
	{
		m_scTip.Create(GetRandWndID(),m_tipPosX,m_tipPosY,width + 24,m_length[STATIC_HEIGHT],this,NULL);
	}
#else
	m_scTip.Create(GetRandWndID(),m_tipPosX,m_tipPosY,width + 24,m_length[STATIC_HEIGHT],this,NULL);
#endif

#ifdef __CUSTOM_IL03__
	m_scTip.SetBKColor(COLOR_BLACK);
#else
	m_scTip.SetBKColor(COLOR_DLG_CHILD_BK);
#endif
	m_scTip.Show(false,false);

	m_tipInit = true;
	m_tipShow = false;

	for ( int i=0;i<TIP_COUNT;i++)
	{
		m_tipColor[i] = COLOR_TEXT_NORMAL;
	}
}

void GUI::CDialog::ChangeTipBKColor(unsigned long color)
{
	m_scTip.SetBKColor(color);
}


void GUI::CDialog::SetTipTextColor(unsigned long color,unsigned long type)
{
	if( type & TIP_FOCUS )
	{
		m_tipColor[0] = color;
	}

	if( type & TIP_DFOCUS )
	{
		m_tipColor[1] = color;
	}

	if( type & TIP_MSG )
	{
		m_tipColor[2] = color;
	}

	if ( type & TIP_ERR )
	{
		m_tipColor[3] = color;
	}
}

void GUI::CDialog::ShowTip(bool bShow, const char* szText, BITMAP_ID bmpID, unsigned long color, bool bRefresh )
{
	assert(m_tipInit);

	if( bShow )
	{
#ifndef __CHIP3515__
		if(!g_langRightOrder)
		{
			if( bmpID != m_idBmpTip )
			{
				m_idBmpTip = bmpID;
				m_picTip.SetImage(m_idBmpTip,false);
			}

			m_picTip.Show(true, bRefresh);
		}
#endif

		m_scTip.Show(true, false);
		m_scTip.SetTextColor(color);
		m_scTip.SetCaption(szText, bRefresh);
	}
	else
	{
#ifndef __CHIP3515__
		if(!g_langRightOrder)
		{
			m_picTip.Show(false, bRefresh);
		}
#endif

		m_scTip.Show(false, bRefresh);
	}
}

void GUI::CDialog::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	if ( (m_cfgTip & TIP_ALL) && m_tipInit )
	{
		//顶层Dlg才具有Tip控件,能进来处理则不需要再向上传递消息

		if( (m_cfgTip & TIP_FOCUS) && !m_tipInfo.empty() )
		{
			TIP_MAP::iterator iter = m_tipInfo.find(newID);
			if (iter != m_tipInfo.end())
			{
				//记录上一次显示字串ID,如果当前需要显示的ID与上一次相同,则不重刷
				static unsigned long s_pre_str_id = E_STR_ID_NULL;
				if( !m_tipShow )
				{
					s_pre_str_id = E_STR_ID_NULL;
				}
				if( s_pre_str_id != iter->second )
				{
					m_tipType = TIP_FOCUS;//0x1,表示位于子窗口控件上的提示
					m_tipShow = true;
#if !defined(__TW01_RILI__)
					ShowTip(m_tipShow,m_pStrTable->String(static_cast<ENUM_STRING_ID>(iter->second)),BMP_ICON_INFO,m_tipColor[0]);
#endif
					s_pre_str_id = iter->second;
				}
				return;
			}
		}

		//采用焦点变更清除残留Tip信息
		unsigned long invalid = INVALID_ID;
		if( m_tipShow && newID != invalid )
		{
			m_tipShow = false;
			ShowTip(m_tipShow,"",BMP_ICON_INFO,m_tipColor[0]);
		}
	}
	else
	{
		GUI::CWnd::OnChangeFocus(oldID, newID);
	}
}
//end
