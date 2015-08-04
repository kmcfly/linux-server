#include "Wnd.h"
using namespace GUI;

GUI::CWnd* GUI::CWnd::MoveFocusUp(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus < m_hCtrlNum) && (yFocus < m_vCtrlNum));

	CWnd* pCtrl= NULL; 

	//向上方向搜索
	while (0 < yFocus) 
	{
		-- yFocus;

		pCtrl = m_pFocusMap[(yFocus * m_hCtrlNum) + xFocus];

		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			return pCtrl;
		}
	}

	return NULL;	
}

GUI::CWnd* GUI::CWnd::MoveFocusDown(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus < m_hCtrlNum) && (yFocus < m_vCtrlNum));

	CWnd* pCtrl= NULL; 

	//向下方向搜索
	while (yFocus < (m_vCtrlNum - 1)) 
	{
		++ yFocus;

		pCtrl = m_pFocusMap[(yFocus * m_hCtrlNum) + xFocus];

		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			return pCtrl;
		}
	}

	return NULL;
}

GUI::CWnd* GUI::CWnd::MoveFocusLeft(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus < m_hCtrlNum) && (yFocus < m_vCtrlNum));

	CWnd* pCtrl= NULL; 

	unsigned long num = 0;
	unsigned long count = (m_hCtrlNum * m_vCtrlNum);

	//一个可以先左再上走“之字”型的方向搜索，与右键相反
	do
	{
		if (0 < xFocus)
		{
			-- xFocus;
		}
		else
		{
			if (0 < yFocus)
			{
				//上一行，最右边
				-- yFocus;
				xFocus = (m_hCtrlNum - 1);
			}
			else
			{
				xFocus = 0;//(m_hCtrlNum - 1);
				yFocus = 0;//(m_vCtrlNum - 1);
			}
		}

		pCtrl = m_pFocusMap[(yFocus * m_hCtrlNum) + xFocus];

		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			return pCtrl;
		}

		++ num;
	} while(num < count);

	return NULL;
}

GUI::CWnd* GUI::CWnd::MoveFocusRight(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus < m_hCtrlNum) && (yFocus < m_vCtrlNum));

	CWnd* pCtrl= NULL; 

	unsigned long num = 0;
	unsigned long count = (m_hCtrlNum * m_vCtrlNum);

	++ xFocus;
	//一个可以先右再下走“之字”型的方向搜索，与左键相反
	do 
	{
		if (xFocus >= m_hCtrlNum)
		{
			xFocus = 0;

			++ yFocus;

			if (yFocus >= m_vCtrlNum)
			{
				yFocus = 0;
			}
		}

		pCtrl = m_pFocusMap[(yFocus * m_hCtrlNum) + xFocus];

		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			return pCtrl;
		}

		++ num;
		++ xFocus;
	} while(num < count);

	return NULL;
}

GUI::CWnd* GUI::CWnd::MoveFocusRightEx(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus<m_hCtrlNum) && (yFocus<m_vCtrlNum));

	//unsigned long count = m_hCtrlNum * m_vCtrlNum;
	unsigned long ctrlIndex = yFocus * m_hCtrlNum + xFocus;
	unsigned long index = ctrlIndex;
	unsigned long endIndex = yFocus * m_hCtrlNum + m_hCtrlNum - 1;
	CWnd* pCurCtrl = m_pFocusMap[index];
	CWnd* pCtrl= pCurCtrl; 
	//向右搜索
	do 
	{
		++index;	
		if (index> endIndex)
		{
			index = yFocus * m_hCtrlNum;
		} 
		pCtrl = m_pFocusMap[index];
		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			if (pCtrl->CanSetFocus())
			{
				return pCtrl;
				break;
			}			
		}
	} while(index != ctrlIndex);

	if ((NULL != pCtrl) && (pCtrl->CanSetFocus()) )
	{
		return pCtrl;
	} 
	else
	{
		return NULL;
	}
}


GUI::CWnd* GUI::CWnd::MoveFocusLeftEx(unsigned long xFocus, unsigned long yFocus) const
{
	if (NULL == m_pFocusMap)
	{
		return NULL;
	}

	assert((xFocus<m_hCtrlNum) && (yFocus<m_vCtrlNum));

	//unsigned long count = m_hCtrlNum * m_vCtrlNum;
	unsigned long ctrlIndex = yFocus * m_hCtrlNum + xFocus;
	unsigned long index = ctrlIndex;
	unsigned long endIndex = yFocus * m_hCtrlNum;
	CWnd* pCurCtrl = m_pFocusMap[index];
	CWnd* pCtrl= pCurCtrl; 
	//向左上方向搜索
	do 
	{
		if (endIndex == index)
		{
			index = yFocus * m_hCtrlNum + m_hCtrlNum - 1;
		} 
		else
		{
			--index;
		}
		pCtrl = m_pFocusMap[index];
		if ( (NULL != pCtrl) && (pCtrl->CanSetFocus()) )
		{
			return pCtrl;
			break;
		}
	} while(index != ctrlIndex);

	if ((NULL != pCtrl) && (pCtrl->CanSetFocus()))
	{
		return pCtrl;
	} 
	else
	{
		return NULL;
	}
}

GUI::CWnd* GUI::CWnd::GetDefaultFocusCtrl(DEFUALT_FOCUS mode)
{
	if ( (NULL == m_pActiveCtrl) && (NULL != m_pFocusMap) )
	{
		switch (mode)
		{
		case DEFUALT_FOCUS_NONE:
			break;
		case DEFUALT_FOCUS_TOP_LEFT:
			{
				unsigned long count = m_vCtrlNum * m_hCtrlNum;
				for (unsigned long i = 0; i < count; ++i)
				{
					if ( (NULL != m_pFocusMap[i]) && (m_pFocusMap[i]->CanSetFocus()) )
					{
						return m_pFocusMap[i];
					}
				}
			}
			break;
		case DEFUALT_FOCUS_TOP_RIGHT:
			{
				CWnd* pBaseCtrl = NULL;
				for (int y = 0; y < m_vCtrlNum; ++y)
				{
					for (int x = m_hCtrlNum - 1; x >= 0; --x)
					{
						pBaseCtrl = m_pFocusMap[x + y * m_hCtrlNum];
						if ( (NULL != pBaseCtrl) && (pBaseCtrl->CanSetFocus()) )
						{
							return pBaseCtrl;
						}
					}
				}
			}
			break;
		case DEFUALT_FOCUS_BOTTOM_LEFT:
			{
				CWnd* pBaseCtrl = NULL;
				for (int y = m_vCtrlNum - 1; y >= 0 ; --y)
				{
					for (int x = 0; x < m_hCtrlNum; ++x)
					{
						pBaseCtrl = m_pFocusMap[x + y * m_hCtrlNum];
						if ( (NULL != pBaseCtrl) && (pBaseCtrl->CanSetFocus()) )
						{
							return pBaseCtrl;
						}
					}
				}
			}
			break;
		case DEFUALT_FOCUS_BOTTOM_RIGHT:
			{
				unsigned long count = m_vCtrlNum * m_hCtrlNum;
				for (unsigned long i = count - 1; i >= 0; --i)
				{
					if ( (NULL != m_pFocusMap[i]) && (m_pFocusMap[i]->CanSetFocus()) )
					{
						return m_pFocusMap[i];
					}
				}
			}
			break;
		default:
			break;
		}
	}

	return NULL;
}

unsigned long GUI::CWnd::OnFocusProc(unsigned long keyID, unsigned long xPoint, unsigned long yPoint)
{
	if (!IsShow() || !m_bEnable)
	{
		return KEY_VIRTUAL_MSG_CONTINUE;
	}

	if (NULL == m_pFocusMap)
	{	//无焦点地图，直接返回
		return KEY_VIRTUAL_MSG_CONTINUE;
	}

	switch (keyID)
	{
	case KEY_VIRTUAL_MOVE:
		{
			if (KEY_VIRTUAL_MSG_STOP == OnFocusProcEx(keyID, xPoint, yPoint))
			{
				return KEY_VIRTUAL_MSG_STOP;
			}

			if ((NULL != m_pActiveCtrl) && m_pActiveCtrl->IsShow() && m_pActiveCtrl->IsEnable() && \
				m_pActiveCtrl->IsInTheRect(xPoint, yPoint))//如果光标在活动子窗口上
			{
				return  m_pActiveCtrl->OnFocusProc(keyID, xPoint, yPoint);
			} 
			else//光标不在活动子窗口上
			{
				CWnd* pCtrl = InWhichActiceCtrl(xPoint, yPoint);
				if (NULL != pCtrl)//当前光标在某个子窗口上
				{
					pCtrl->SetDefaultFocusMode(DEFUALT_FOCUS_NONE);
					bool ret = SetCtrlFocus(pCtrl, true);
					assert(ret);//造成断言失败的原因很可能是由于（可设置焦点的）控件的焦点坐标重合了（不使能的控件覆盖了使能的控件）

					return  pCtrl->OnFocusProc(keyID, xPoint, yPoint);
				} 
				else//当前光标不在任何子窗口上
				{
					if (NULL != m_pActiveCtrl)
					{
						SetCtrlFocus(m_pActiveCtrl, false);
					}
				}
			}

			return KEY_VIRTUAL_MSG_CONTINUE;
		}
		break;
	case KEY_VIRTUAL_UP:
	case KEY_VIRTUAL_DOWN:
	case KEY_VIRTUAL_LEFT:
	case KEY_VIRTUAL_RIGHT:
		{
			if (KEY_VIRTUAL_MSG_STOP == OnFocusProcEx(keyID, xPoint, yPoint))
			{
				return KEY_VIRTUAL_MSG_STOP;
			}

			if (NULL != m_pActiveCtrl)
			{
				if (KEY_VIRTUAL_MSG_STOP == m_pActiveCtrl->OnFocusProc(keyID, xPoint, yPoint))
				{
					return KEY_VIRTUAL_MSG_STOP;
				}

				int xFocus = 0, yFocus = 0;		
				m_pActiveCtrl->GetFocusPos(xFocus, yFocus);

				CWnd* pCtrl = NULL;
				switch (keyID)
				{
				case KEY_VIRTUAL_UP:
					pCtrl = MoveFocusUp(xFocus, yFocus);
					if (NULL != pCtrl)
					{
						pCtrl->SetDefaultFocusMode(DEFUALT_FOCUS_BOTTOM_RIGHT);
						int tmp_x_focus = xFocus;
						int tmp_y_focus = yFocus;
						pCtrl->GetFocusPos(xFocus, yFocus);
						if ((NULL != m_pParent) && (yFocus >= tmp_y_focus ) && (xFocus >= tmp_x_focus ))
						{
							return KEY_VIRTUAL_MSG_CONTINUE;
						}
					}				
					break;
				case KEY_VIRTUAL_DOWN:
					pCtrl = MoveFocusDown(xFocus, yFocus);
					if (NULL != pCtrl)
					{
						pCtrl->SetDefaultFocusMode(DEFUALT_FOCUS_TOP_LEFT);
						int tmp_x_focus = xFocus;
						int tmp_y_focus = yFocus;
						pCtrl->GetFocusPos(xFocus, yFocus);
						if ((NULL != m_pParent) && (yFocus <= tmp_y_focus) && (xFocus <= tmp_x_focus))
						{
							return KEY_VIRTUAL_MSG_CONTINUE;
						}
					}
					break;
				case KEY_VIRTUAL_LEFT:
					pCtrl = MoveFocusLeft(xFocus, yFocus);
					if (NULL != pCtrl)
					{
						pCtrl->SetDefaultFocusMode(DEFUALT_FOCUS_BOTTOM_RIGHT);
						int tmp_x_focus = xFocus;
						int tmp_y_focus = yFocus;
						pCtrl->GetFocusPos(xFocus, yFocus);
						if ((NULL != m_pParent) && (xFocus >= tmp_x_focus) && (yFocus == tmp_y_focus))
						{
							return KEY_VIRTUAL_MSG_CONTINUE;
						}
					}				
					break;
				case KEY_VIRTUAL_RIGHT:
					pCtrl = MoveFocusRight(xFocus, yFocus);
					if (NULL != pCtrl)
					{
						pCtrl->SetDefaultFocusMode(DEFUALT_FOCUS_TOP_LEFT);
						int tmp_x_focus = xFocus;
						int tmp_y_focus = yFocus;
						pCtrl->GetFocusPos(xFocus, yFocus);
						if ((NULL != m_pParent) && (xFocus <= tmp_x_focus ) && (yFocus <= tmp_y_focus ))
						{
							return KEY_VIRTUAL_MSG_CONTINUE;
						}
					}				
					break;
				default:
					return KEY_VIRTUAL_MSG_CONTINUE;
					break;
				}

				if (NULL == pCtrl)//返回空，不做任何动作
				{
					//	bool ret = SetCtrlFocus(m_pActiveCtrl, false);
					//	assert(ret);//造成断言失败的原因很可能是由于（可设置焦点的）控件的焦点坐标重合了（不使能的控件覆盖了使能的控件）
					return KEY_VIRTUAL_MSG_CONTINUE;
				}
				else if ( pCtrl != m_pActiveCtrl )
				{
					bool ret = SetCtrlFocus(pCtrl, true);
					assert(ret);//造成断言失败的原因很可能是由于（可设置焦点的）控件的焦点坐标重合了（不使能的控件覆盖了使能的控件）

					return KEY_VIRTUAL_MSG_STOP;
				}
				else
				{
					//与活动子窗口相同，不做任何操作
					return KEY_VIRTUAL_MSG_STOP;
				}
			} 
			else //无活动子窗口，设置默认焦点
			{
				return SetDefaultFocus();
			}
		}
		break;
	default:
		{
			if ((m_width <= xPoint) && (m_height <= yPoint) && (0 < m_childList.size()))
			{
				if ((NULL == m_pActiveCtrl) || \
					(NULL != m_pActiveCtrl) && (!m_pActiveCtrl->IsShow() || !m_pActiveCtrl->IsEnable()))
				{
					WND_LIST::const_iterator iter;
					for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
					{
						if ((*iter)->IsShow() && (*iter)->IsEnable() && (*iter)->IsSupportFocus())
						{
							if (KEY_VIRTUAL_MSG_STOP == (*iter)->OnFocusProc(keyID, xPoint, yPoint))
							{
								m_pActiveCtrl = (*iter);
								return KEY_VIRTUAL_MSG_STOP;
							}
							else
							{
								SetCtrlFocus((*iter), true);
								m_pActiveCtrl = (*iter);
								return KEY_VIRTUAL_MSG_STOP;
							}
						}
					}
				}
			}

			return KEY_VIRTUAL_MSG_CONTINUE;
		}
		break;
	}
}

unsigned long GUI::CWnd::SetDefaultFocus(bool bRefresh/* = true*/)
{
	CWnd* pWnd = GetDefaultFocusCtrl(m_defaultFocus);
	if (NULL != pWnd)
	{
		SetCtrlFocus(pWnd, true, bRefresh);
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		if (bRefresh)
		{
			if (0 == m_childList.size())
			{
				Repaint();
			}
		}

		return KEY_VIRTUAL_MSG_CONTINUE;
	}
}


/***********************************************************
函数名：  SetCtrlFocus
功能描述：设置控件焦点；
输入参数：pCtrl：控件指针； bFocus：是否有焦点；
输出参数：无；
返回值：  无；
说明：    （1）设置对话框中控件的焦点，必须且只能调用此函数；
（2）对话框最多能有一个具有焦点的控件；
（3）当bFocus为true:  改变对话框当前控件的焦点 和 当前控件的位置；
当bFocus为false: 仅将控件的焦点去掉，不做其他操作；

创建者/修改者                   创建/修改日期                  备注
黄展                            28/08/07                       创建
***********************************************************/
bool GUI::CWnd::SetCtrlFocus(CWnd* pCtrl, bool bFocus, bool bRefresh/* = true*/)
{
	assert(NULL != pCtrl);

	if (bFocus) //设置有焦点的情况
	{	
		if ( !pCtrl->CanSetFocus() ) //如果控件不能设置焦点，返回失败
		{
			return false;
		}
		else //开始设置控件焦点， 返回成功
		{
			unsigned long newCtrlID = pCtrl->GetID();
			unsigned long oldCtrlID = INVALID_ID;

			if (NULL != m_pActiveCtrl) //去除当前具有焦点控件的焦点
			{
				if (m_pActiveCtrl->IsFocus())
				{
					if (!m_pActiveCtrl->SetFocus(false, bRefresh))
					{
						//当前控件必然可以设置焦点 ? 失败原因可能是由于m_pActive不可见或者不使能
						m_pActiveCtrl->m_bFocus = false;
//						assert(false);
					}

					oldCtrlID = m_pActiveCtrl->GetID();
				}
			}

			pCtrl->SetFocus(true, bRefresh); //设置改控件的焦点
			m_pActiveCtrl = pCtrl;//设置当前控件位置
			OnChangeFocus(oldCtrlID, newCtrlID);

			return true;
		}
	}
	else //设置无焦点的情况
	{
		//注意，此时当前控件的位置没有改变
		if (pCtrl->IsFocus())
		{
			assert (m_pActiveCtrl == pCtrl);
			m_pActiveCtrl = NULL;
			
			if (pCtrl->SetFocus(false, bRefresh))
			{
				OnChangeFocus(pCtrl->GetID(), INVALID_ID);
				return true;
			} 
			else
			{
				//失败原因可能是父对话框不可显示，这里仅改变状态
				return false;
			}

		}
		else
		{
			return true;
		}
	}	
}


bool GUI::CWnd::CanSetFocus() const
{
	if (IsSupportFocus())
	{
		return (IsShow() && m_bEnable);
	} 
	else
	{
		return false;
	}
}

bool GUI::CWnd::IsSupportFocus() const
{
	if (m_style & WND_CHILD)
	{
		if (m_style & WND_DISABLE_FOCUS)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

void GUI::CWnd::CalculateFocusCount(int& x, int & y) const
{
	int xFocus ,yFocus;
	const CWnd* pCtrl = NULL;
	WND_LIST::const_iterator iter;
	for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		pCtrl = *iter;

		if ( pCtrl->IsSupportFocus() )
		{
			pCtrl->GetFocusPos(xFocus, yFocus);

			if ( xFocus >= x )
			{
				x = xFocus + 1;
			}

			if ( yFocus >= y )
			{
				y = yFocus + 1;
			}
		}
	}
}
/***********************************************************
函数名：  CreateFocusMap
功能描述：建立焦点地图；
输入参数：无；
输出参数：无；
返回值：  无；
说明：    （1）创建焦点地图的目的是为了处理焦点移动问题；
（2）当对话框中不存在支持焦点的控件， 焦点地图（m_pFocusMap）为空(NULL)，
且横坐标长度（m_hCtrlNum）和纵坐标长度（m_vCtrlNum）都为0；
（3）当完成“控件加入对话框”的操作后，调用此函数建立焦点地图。

创建者/修改者                   创建/修改日期                  备注
黄展                            28/08/07                       创建
***********************************************************/
bool GUI::CWnd::CreateFocusMap()
{
	if (NULL != m_pFocusMap)
	{
		delete [] m_pFocusMap;
		m_pFocusMap = NULL;
	}

	m_hCtrlNum = 0;
	m_vCtrlNum = 0;

	//计算焦点地图的长宽（不支持焦点的控件，不计入焦点地图）
	CalculateFocusCount(m_hCtrlNum, m_vCtrlNum);

	//如果有支持焦点的控件, 建立焦点地图
	if ( (m_vCtrlNum>0) && (m_hCtrlNum>0) ) 
	{	
		unsigned long size = m_vCtrlNum*m_hCtrlNum;
		m_pFocusMap = new CWnd* [size];
		memset(m_pFocusMap, 0, size*sizeof(CWnd*));//根据某坐标上的控件指针是否为空，可判断此坐标上是否有控件

		CWnd* pCtrl = NULL;
		int xFocus, yFocus;

		WND_LIST::iterator iter;
		for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
		{
			pCtrl = *iter;
			if ( pCtrl->IsSupportFocus() )
			{		
				pCtrl->GetFocusPos(xFocus, yFocus);
				assert( (xFocus<m_hCtrlNum) && (yFocus<m_vCtrlNum) );

				m_pFocusMap[ (yFocus * m_hCtrlNum) + xFocus ] = pCtrl;
			}
		}

		//改进一行占多格，焦点无法移动的情况 20080613 hz
		/*for (int y = 0; y < m_vCtrlNum; ++y)
		{
		for (int x = 1; x < m_hCtrlNum; ++x)
		{
		if (NULL == m_pFocusMap[ (y * m_hCtrlNum) + x ])
		{
		m_pFocusMap[ (y * m_hCtrlNum) + x ] = m_pFocusMap[ (y * m_hCtrlNum) + x - 1];
		}
		}
		}*/
		return true;
	}
	else
	{
		return false;
	}
}

bool GUI::CWnd::SetFocus(bool bFocus, bool bRefresh /* = true */)
{
	if (CanSetFocus())
	{
		if (m_bFocus != bFocus)
		{
			if (bFocus)
			{
				//先改变状态,再刷新
				m_bFocus = bFocus;

				CWnd* pWnd = GetDefaultFocusCtrl(m_defaultFocus);
				if (NULL != pWnd)
				{
					SetCtrlFocus(pWnd, true, false);
				}
				else
				{
					//原来修改是因为排程控件那里，当鼠标进入后设置了焦点，但是不能变化，所以
					//修改了为 (NULL == m_pActiveCtrl)。但是发现这种机制会导致鼠标移到过程中，
					//产生多次重绘，效率不高。所以改回去，但是会造成带子窗体控件在设置焦点时
					//不能刷新现象。下面相同原因。
					if (0 == m_childList.size())
					//if (NULL == m_pActiveCtrl)
					{
						Repaint();
					}
				}

				//SetDefaultFocus(false);	
			} 
			else
			{	
				m_bFocus = bFocus;

				//在去除焦点时，如果窗口存在拥有焦点的子窗口，去除子窗口焦点
				if (NULL != m_pActiveCtrl)
				{
					SetCtrlFocus(m_pActiveCtrl, false, false);
				}
				else
				{
					if (0 == m_childList.size())
					//if (NULL == m_pActiveCtrl)
					{
						Repaint();
					}
				}
			}
		}		

		return true;
	} 
	else
	{
		//失败原因可能是父对话框不可显示，这里仅改变状态
		if (!bFocus)
		{
			m_bFocus = false;
		}
		return false;
	}
}

/***********************************************************
函数名:OnChangeFocus
说明:焦点变化,最先由所处窗口获取.依次向上级窗口
传递消息事件
***********************************************************/
void GUI::CWnd::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	GUI::CWnd* pWnd = GetParent();
	if( pWnd )
	{
		pWnd->OnChangeFocus(oldID,newID);
	}
}
